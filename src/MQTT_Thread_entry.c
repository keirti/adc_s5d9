#include "MQTT_Config.h"
#include "mqtt_thread.h"
#include "mqtt_rx_thread.h"
#include "console_config.h"
#include "iot_service_api.h"
#include "mqtt_client_api.h"
#include "internal_flash.h"
#include "sntp_client.h"
#include "common_util.h"


/* Function prototypes */
void set_mac_address (nx_mac_address_t*_pMacAddress);
UINT init_network_interface(mqtt_client_cfg_t *p_cfg, net_input_cfg_t *net_cfg);
UINT deinit_network_service(net_input_cfg_t *net_cfg);
UINT reset_network_interface(net_input_cfg_t net_cfg);

extern NX_DNS  *g_active_dns;
extern NX_IP   *g_active_ip;
extern mqtt_client_api_t  nx_mqtt_api;
extern const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers_synergys7;
extern mqtt_client_api_t  nx_mqtt_api;
//extern iot_service_api_t const gcloud_iot_api;

/* These global variables are used within the context of MQTT_Thread */
extern NX_DNS  *g_active_dns;
extern NX_IP   *g_active_ip;                /* Pointer to active IP interface */
extern NX_DHCP *g_active_dhcp;              /* Pointer to active DHCP client */
extern NX_SNTP_CLIENT *g_active_sntp;       /* Pointer to active SNTP client */

char *property_values[PROPERTY_COUNT];
char prop_buffer[MAX_PROPERTY_LEN * PROPERTY_COUNT];
static mqtt_client_cfg_t  g_mqtt_cfg;
static mqtt_client_ctrl_t g_mqtt_ctrl;
static iot_service_ctrl_t gcloud_iot_ctrl;

static TX_TIMER g_sys_timer;
static TX_TIMER g_retry_timer;
//extern adc_data_t* adc_data;
extern uint8_t channel;
mqtt_client_instance_t  g_mqtt_client =
{
     .p_cfg     = &g_mqtt_cfg,
     .p_ctrl    = &g_mqtt_ctrl,
     .p_api     = &nx_mqtt_api,
};

/* Initialization of the AWS service structures */
static iot_service_cfg_t gcloud_iot_cfg =
{
    .p_name     = "GCloud",
    .protocol   = IOT_SERVICE_PROTO_MQTT,
    .p_mqtt_client = &g_mqtt_client,
};

static iot_service_instance_t  gcloud_iot =
{
    .p_cfg     = &gcloud_iot_cfg,
    .p_ctrl    = &gcloud_iot_ctrl,
//    .p_api     = &gcloud_iot_api,
};

unsigned char CA_cert_der[4096];
unsigned char private_key[4096];



/*********************************************************************************************************************
 * @brief  getCert_from_flash function
 *
 * This function reads the device certificates/keys from internal flash.
 ********************************************************************************************************************/
static int getCert_from_flash(dev_cert_cfg cert_info)
{
    if(SSP_SUCCESS != int_storage_read((uint8_t *)&CA_cert_der, cert_info.rootCA_len, ROOTCA_CERT_CFG))
    {
        print_to_console("Failed to read rootCA certificate from internal storage \r\n");
        print_to_console("Load the rootCA certificate by running the \"cwiz\" command\r\n");
        return -1;
    }

    if(SSP_SUCCESS != int_storage_read((uint8_t *)&private_key, cert_info.priKey_len, PRI_KEY_CFG))
    {
        print_to_console("Failed to read private key from internal storage \r\n");
        print_to_console("Load the private key by running the \"cwiz\" command\r\n");
        return -1;
    }

    return 0;
}


/*********************************************************************************************************************
 * @brief  set_mac_address function
 *
 * Sets the unique Mac Address of the device using the FMI unique ID.
 ********************************************************************************************************************/
void set_mac_address (nx_mac_address_t*_pMacAddress)
{
    fmi_unique_id_t id;
    ULONG lowerHalfMac = 0;

    memset(&id, 0, sizeof(id));
    /* Read FMI unique ID */
    g_fmi.p_api->uniqueIdGet(&id);

    /* REA's Vendor MAC range: 00:30:55:xx:xx:xx */
    lowerHalfMac = ((0x55000000) | (id.unique_id[0] & (0x00FFFFFF)));

    /* Return the MAC address */
    _pMacAddress->nx_mac_address_h=0x0030;
    _pMacAddress->nx_mac_address_l=lowerHalfMac;
}


/*********************************************************************************************************************
 * @brief  deinit_iot_service function
 *
 * This function De-initializes the IoT Cloud interface
 ********************************************************************************************************************/
static UINT deinit_iot_service(mqtt_client_cfg_t *p_cfg)
{
    const iot_service_api_t *p_api;
    iot_service_instance_t *p_iots;
    UINT status = NX_SUCCESS;

    if(NULL == p_cfg)
    {
        print_to_console("\r\n Null pointer reference in deinit_iot_service function\r\n");
        return 1;
    }
    p_iots = p_cfg->p_iot_service;
    p_api = p_iots->p_api;

    print_to_console("\r\nDe-initializing IoT Service... ");
    status = p_api->close(p_iots->p_ctrl);
    if(status != NX_SUCCESS)
    {
        print_to_console("Unable to de-initialize IoT service ");
        print_to_console((char *)p_iots->p_cfg->p_name);
        print_to_console("\r\n");
        return status;
    }

    print_to_console("done\r\n");

    return status;
}

static void g_mqtt_timer_cb(ULONG unused)
{
    SSP_PARAMETER_NOT_USED(unused);

    tx_event_flags_set(&g_sys_event_flags, SYS_EV_MQTT_UPDATE, TX_OR);

}

static void g_iot_nw_retry_cb(ULONG unused)
{
    SSP_PARAMETER_NOT_USED(unused);

    tx_event_flags_set(&g_sys_event_flags, SYS_EV_IOT_NW_RETRY, TX_OR);
}
/*********************************************************************************************************************
 * @brief  init_iot_service function
 *
 * This function initializes the IoT Cloud interface
 ********************************************************************************************************************/
static UINT init_iot_service(mqtt_client_ctrl_t *p_ctrl, mqtt_client_cfg_t *p_cfg, iot_input_cfg_t iotcfg, dev_cert_cfg cert_info)
{
    UINT status = NX_SUCCESS;
    iot_service_instance_t *p_iots = NULL;
    const iot_service_api_t *p_api = NULL;
    char str[64] = {0};

    if(NULL == p_cfg || NULL == p_ctrl)
    {
        print_to_console("\r\n NULL pointer reference in init_iot_service function \r\n");
        return 1;
    }
    print_to_console("Initializing Cloud Interface:");

    p_cfg->p_endpoint = (char*)iotcfg.gCloud_info.endp_address;
    p_cfg->p_root_ca = (uint8_t *) CA_cert_der;
    p_cfg->root_ca_len = (uint16_t)cert_info.rootCA_len;
    p_cfg->p_device_cert = NULL;
    p_cfg->device_cert_len = 0;
    p_cfg->p_private_key = (uint8_t *) private_key;
    p_cfg->private_key_len = (uint16_t)cert_info.priKey_len;
    p_cfg->deviceId = (char*)iotcfg.gCloud_info.device_id;
    p_cfg->projectId = (char*)iotcfg.gCloud_info.project_id;

    sprintf(p_cfg->devname,"projects/%s/locations/%s/registries/%s/devices/%s", iotcfg.gCloud_info.project_id,
            iotcfg.gCloud_info.cloud_region,iotcfg.gCloud_info.registry_id,iotcfg.gCloud_info.device_id);

    p_cfg->p_iot_service = &gcloud_iot;

#if 0
    print_to_console("\r\n");
    print_to_console("Google MQTT Device ID: ");
    print_to_console(p_cfg->deviceId);
    print_to_console("\r\n");
    print_to_console("Google Endpoint: ");
    print_to_console(p_cfg->p_endpoint);
    print_to_console("\r\n");
    print_to_console("Google Project Id: ");
    print_to_console(p_cfg->projectId);
    print_to_console("\r\n");
#endif

    if(p_cfg->server_ip == 0)
    {
        status = nx_dns_host_by_name_get(g_active_dns, (UCHAR *)p_cfg->p_endpoint, &p_cfg->server_ip, 1000);
        if (NX_SUCCESS != status)
        {
            snprintf(str,sizeof(str), "Could not resolve host name: %s, ret = %x\r\n", p_cfg->p_endpoint,status);
            print_to_console(str);
            return status;
        }
    }

    /* common values */
    p_ctrl->server_ip = p_cfg->server_ip;
    p_ctrl->logMsg = print_to_console;
    p_cfg->p_ciphers = (NX_SECURE_TLS_CRYPTO *) &nx_crypto_tls_ciphers_synergys7;
    p_cfg->thread_priority = 2;
    p_iots = p_cfg->p_iot_service;
    p_api = p_iots->p_api;

    /* Set the flag to enable IOT Service Debug Messages*/
    p_iots->p_ctrl->debug = IOTS_DEBUG;

    status = p_api->open(p_iots->p_ctrl, p_iots->p_cfg);
    if(status != NX_SUCCESS)
    {
        snprintf(str,sizeof(str), "MQTT Client Open Failed!!!, ret = 0x%x\r\n", status);
        print_to_console(str);
        return status;
    }

    print_to_console("Done\r\n");

    return status;
}

static UINT update_iot_service(iot_service_instance_t *p_iots, struct sensors *old, struct sensors *new)
{
    if(old == NULL || new == NULL || p_iots == NULL)
        return 1;

    const iot_service_api_t *p_api = p_iots->p_api;
    iot_service_ctrl_t *p_ctrl = p_iots->p_ctrl;
    uint32_t i = 0;
    char ostr[MAX_PROPERTY_LEN];
    char nstr[MAX_PROPERTY_LEN];
    UINT status = NX_SUCCESS;
    mqtt_rx_payload_t rx_data;
    char const *property_names[PROPERTY_COUNT] = {NULL};

    memset(&rx_data,0,sizeof(rx_data));

    /* Check for MQTT Rx message to report */
    tx_queue_receive(&g_mqtt_rx_payload, &rx_data, 1);

    switch(rx_data.type)
    {
        case LED_RED:
            property_names[i] = PROP_STR_LED_RED;
            if(rx_data.value == LED_OFF)
            {
                strcpy(property_values[i++], "OFF");
            }
            else if(rx_data.value == LED_ON)
            {
                strcpy(property_values[i++], "ON");
            }

            /* Publish the LED state */
            if(i > 0)
            {
                status = p_api->propertySet(p_ctrl, property_names, (const char **)property_values, i);
                if(status != NX_SUCCESS)
                    return status;
            }
            break;

        case LED_YELLOW:
            property_names[i] = PROP_STR_LED_YELLOW;
            if(rx_data.value == LED_OFF)
            {
                strcpy(property_values[i++], "OFF");
            }
            else if(rx_data.value == LED_ON)
            {
                strcpy(property_values[i++], "ON");
            }

            /* Publish the LED state */
            if(i > 0)
            {
                status = p_api->propertySet(p_ctrl, property_names, (const char **)property_values, i);
                if(status != NX_SUCCESS)
                    return status;
            }
            break;

        case LED_GREEN:
            property_names[i] = PROP_STR_LED_GREEN;
            if(rx_data.value == LED_OFF)
            {
                strcpy(property_values[i++], "OFF");
            }
            else if(rx_data.value == LED_ON)
            {
                strcpy(property_values[i++], "ON");
            }

            /* Publish the LED state */
            if(i > 0)
            {
                status = p_api->propertySet(p_ctrl, property_names, (const char **)property_values, i);
                if(status != NX_SUCCESS)
                    return status;
            }
            break;

    }

    i = 0;



    return status;
}

/*********************************************************************************************************************
 * @brief  MQTT_Thread_entry function
 *
 * This function is the MQTT Thread entry function.
 ********************************************************************************************************************/
void MQTT_Thread_entry(void)
{
    UINT status = NX_SUCCESS;
    ULONG event_data = 0;
    uint32_t index = 0;
    net_input_cfg_t net_cfg = {0};
    iot_input_cfg_t iot_cfg = {0};
    dev_cert_cfg cert_info;
    iot_service_instance_t *p_iots = NULL;

    bsp_leds_t leds;
    uint8_t iot_retry_cnt = IOT_SERVICE_RETRY_CNT;
    uint8_t net_retry_cnt = NETWORK_RETRY_CNT;
    static uint8_t demo_stop = 1;
    char str[64] = {0};

    memset (&g_mqtt_cfg, 0, sizeof(g_mqtt_cfg));

    memset(&net_cfg, 0, sizeof(net_cfg));
    memset(&iot_cfg, 0, sizeof(iot_cfg));

    g_active_ip = &g_ip0;
    g_active_dhcp = &g_dhcp_client0;
    g_active_dns = &g_dns0;
    g_active_sntp = &g_sntp_client0;
    g_mqtt_cfg.p_secure_client = &g_mqtt_client0;
    g_mqtt_cfg.mqtt_init = mqtt_client_init0;

    for (index = 0; index < ARRAY_SIZE(property_values); index++)
        property_values[index] = &prop_buffer[index * MAX_PROPERTY_LEN];

    /* Initialize LED default to OFF state */
    R_BSP_LedsGet(&leds);

    if(0 < leds.led_count)
    {
        for(index = 0; index < leds.led_count; index++)
        {
            led_on_off(leds.p_leds[index], "OFF");
        }
    }

    status = int_storage_init();
    if(SSP_SUCCESS != status)
    {
        print_to_console("\r\nFlash Initialization failed !!!!");
        APP_ERR_TRAP(status);
    }

    /* Create ThreadX MQTT System Timer */
    status = tx_timer_create(&g_sys_timer, "MQTT System Timer", g_mqtt_timer_cb, 0,
                             MQTT_UPDATE_DELAY, MQTT_UPDATE_DELAY, TX_NO_ACTIVATE);
    if(TX_SUCCESS != status)
    {
        print_to_console("\r\n Creation of MQTT System Timer Failed!!!\r\n");
        APP_ERR_TRAP(status);
    }

    /* Create ThreadX IoT/NW retry timer */
    status = tx_timer_create(&g_retry_timer, "IoT_NW_Retry Timer", g_iot_nw_retry_cb, 0,
                             IOT_NW_RETRY_DELAY, IOT_NW_RETRY_DELAY, TX_NO_ACTIVATE);
    if(TX_SUCCESS != status)
    {
        print_to_console("\r\n Creation of IoT_NW_Retry Timer Failed!!!\r\n");
        APP_ERR_TRAP(status);
    }

    if(reset_network_interface(net_cfg) != NX_SUCCESS)
    {
        APP_ERR_TRAP(1);
    }

    while(true)
    {
        status = tx_event_flags_get(&g_sys_event_flags, SYS_EV_ALL, TX_OR_CLEAR, &event_data, TX_WAIT_FOREVER);

        if(event_data & SYS_EV_DEMO_START)
        {
            if(SSP_SUCCESS != int_storage_read((uint8_t *)&net_cfg, sizeof(net_cfg), NET_INPUT_CFG))
            {
                print_to_console("Failed to read network user input configuration from internal storage \r\n");
                print_to_console("Please provision the device by running the \"cwiz\" command\r\n");
                APP_ERR_TRAP(SSP_ERR_ABORTED);
            }

            if(SSP_SUCCESS != int_storage_read((uint8_t *)&iot_cfg, sizeof(iot_cfg), IOT_INPUT_CFG))
            {
                print_to_console("Failed to read IOT service user input configuration from internal storage \r\n");
                print_to_console("Please provision the device by running the \"cwiz\" command\r\n");
                APP_ERR_TRAP(SSP_ERR_ABORTED);
            }

            if(SSP_SUCCESS != int_storage_read((uint8_t *)&cert_info, sizeof(cert_info), CERT_CFG_TYPE))
            {
                print_to_console("Failed to read device Certificate configuration from internal storage \r\n");
                print_to_console("Please provision the device by running the \"cwiz\" command\r\n");
                APP_ERR_TRAP(SSP_ERR_ABORTED);
            }

            if(net_cfg.netif_valid && iot_cfg.iotserv_valid && cert_info.rootCA_len != 0 && cert_info.priKey_len != 0)
            {
                if(demo_stop == 1)
                {
                      /* Initialize the network interface*/
                      status = init_network_interface(&g_mqtt_cfg, &net_cfg);
                      if(status)
                      {
                          print_to_console(" *********** Network Interface Init failed !!!!  **********\r\n");
                          APP_ERR_TRAP(status);
                      }
                      else
                      {
                          /* Download certificate/keys from internal flash */
                          if(getCert_from_flash(cert_info) < 0)
                              APP_ERR_TRAP(1);

                          /* Initialize Cloud Interface */
                          status = init_iot_service(&g_mqtt_ctrl, &g_mqtt_cfg, iot_cfg, cert_info);
                          if(status != NX_SUCCESS)
                          {
                              snprintf(str,sizeof(str), "Cloud Init Failed!!!, ret = 0x%x\r\n", status);
                              print_to_console(str);
                              APP_ERR_TRAP(status);
                          }
                          else
                          {
                              demo_stop = 0;

                              /* pass the iot service info to MQTT Rx Thread */
                              tx_queue_send(&g_msg_queue, &g_mqtt_cfg.p_iot_service, TX_NO_WAIT);

                              /* activate system timer */
                              tx_timer_activate(&g_sys_timer);
                          }
                      }
                }
                else
                {
                    print_to_console("Stop the demo before running demo start command again!!!!\r\n");
                }
            }
        }

        if(event_data & SYS_EV_MQTT_DISCONN)
        {
            if(demo_stop == 0)
            {
                demo_stop = 1;

                /* deactivate system timer */
                tx_timer_deactivate(&g_sys_timer);

                print_to_console("MQTT Disconnect Callback event received!!!\r\n");
                if(deinit_iot_service(&g_mqtt_cfg) == SSP_SUCCESS)
                {
                    /* De-initialize network interface */
                    status = deinit_network_service(&net_cfg);
                    if(NX_SUCCESS == status)
                    {
                        /* activate retry timer */
                        tx_timer_activate(&g_retry_timer);
                    }
                }
            }
        }

        if(event_data & SYS_EV_IOT_NW_RETRY)
        {
            /* Deactivate retry timer */
            tx_timer_deactivate(&g_retry_timer);

            if(iot_retry_cnt > 0 && demo_stop == 0)
            {
                /* Initialize Iot Service */
                status = init_iot_service(&g_mqtt_ctrl, &g_mqtt_cfg, iot_cfg, cert_info);
                if(NX_SUCCESS != status)
                {
                    snprintf(str,sizeof(str), "IoT Service Init Failed. Retrying..., ret = 0x%x\r\n", status);
                    print_to_console(str);
                    iot_retry_cnt--;

                    /* Activate retry timer */
                    tx_timer_activate(&g_retry_timer);

                }
                else
                {
                    demo_stop = 0;
                    /* activate system timer */
                    tx_timer_activate(&g_sys_timer);
                    /* Reload the retry count value */
                    iot_retry_cnt = IOT_SERVICE_RETRY_CNT;
                }
            }
            else
            {
                if(net_retry_cnt > 0)
                {
                    status = init_network_interface(&g_mqtt_cfg, &net_cfg);
                    if(NX_SUCCESS != status)
                    {
                        snprintf(str,sizeof(str), "Network Interface Init Failed. Retrying..., ret = 0x%x\r\n", status);
                        print_to_console(str);
                        net_retry_cnt--;

                        /*Activate retry timer */
                        tx_timer_activate(&g_retry_timer);
                    }
                    else
                    {
                        do
                        {
                            /* Initialize Iot Service */
                            status = init_iot_service(&g_mqtt_ctrl, &g_mqtt_cfg, iot_cfg, cert_info);
                            if(NX_SUCCESS != status)
                            {
                                snprintf(str,sizeof(str), "IoT Service Init Failed. Retrying..., ret = 0x%x\r\n", status);
                                print_to_console(str);
                                iot_retry_cnt--;
                            }
                            else
                            {
                                demo_stop = 0;
                                /* activate system timer */
                                tx_timer_activate(&g_sys_timer);
                                /* deactivate retry timer */
                                tx_timer_deactivate(&g_retry_timer);

                                /* Reload the retry count value */
                                iot_retry_cnt = IOT_SERVICE_RETRY_CNT;
                                net_retry_cnt = NETWORK_RETRY_CNT;
                                break;
                            }

                        }while(iot_retry_cnt > 0);

                        if(iot_retry_cnt == 0)
                        {
                            /* Halting the demo */
                            print_to_console("\r\nHalting the Demo!!!\r\n");

                            deinit_iot_service(&g_mqtt_cfg);
                            /* De-initialize network interface */
                            deinit_network_service(&net_cfg);

                            /* deactivate retry timer */
                            tx_timer_deactivate(&g_retry_timer);

                            /* deactivate system timer */
                            tx_timer_deactivate(&g_sys_timer);
                        }
                    }

                }
                else
                {
                    print_to_console("\r\nHalting the Demo!!!\r\n");
                    demo_stop = 1;
                    /* Deactivate system timer */
                    tx_timer_deactivate(&g_sys_timer);
                    /* Deactivate retry timer */
                    tx_timer_deactivate(&g_retry_timer);
                }
            }
        }

        if ((event_data & SYS_EV_RX_UPDATE) || (event_data & SYS_EV_MQTT_UPDATE))
        {
            if(demo_stop == 0)
            {
                p_iots = g_mqtt_cfg.p_iot_service;

                /* read sensor value */
                //read_sensor(&new);

                /* Push the data to MQTT broker */
 //               status = update_iot_service(p_iots,&old,&new);
             //   status = update_iot_service(p_iots,&adc_data[channel],&adc_data[channel]);
                if(NX_SUCCESS != status)
                {
                    if(NXD_MQTT_CONNECT_FAILURE == status || NXD_MQTT_NOT_CONNECTED == status)
                    {
                        /* deactivate system timer */
                        tx_timer_deactivate(&g_sys_timer);

                        /* activate retry timer */
                        tx_timer_activate(&g_retry_timer);
                    }
                    else
                    {
                        print_to_console("\r\nHalting the Demo!!!\r\n");

                        deinit_iot_service(&g_mqtt_cfg);
                        /* De-initialize network interface */
                        deinit_network_service(&net_cfg);

                        /* deactivate retry timer */
                        tx_timer_deactivate(&g_retry_timer);

                        /* deactivate system timer */
                        tx_timer_deactivate(&g_sys_timer);
                    }
                }
            }
        }

        if(event_data & SYS_EV_DEMO_STOP)
        {
            if(demo_stop == 0)
            {
                demo_stop = 1;

                /* deactivate system timer */
                tx_timer_deactivate(&g_sys_timer);
                /* deactivate retry timer */
                tx_timer_deactivate(&g_retry_timer);

                deinit_iot_service(&g_mqtt_cfg);
                /* De-initialize network interface */
                deinit_network_service(&net_cfg);
            }
        }
    }
}
