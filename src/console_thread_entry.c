/***********************************************************************************************************************
* File Name    : console_thread_entry.c
* Description  : This file contains console thread function which handles CLI events in this application.
***********************************************************************************************************************/
#include "console_thread.h"
#include "MQTT_Config.h"
#include "common_util.h"
#include "MQTT_Thread.h"
#include "console_config.h"
#include "internal_flash.h"
/*
#include "base64.h"
#include "sensors.h"
*/

uint8_t certPEM[4096];
char str[256] = {0};
unsigned int certDER[4096];
void print_to_console(const char* msg);
void print_ipv4_addr(ULONG address, char *str, size_t len);

static void print_static_addr(net_input_cfg_t net_cfg)
{
    memset(str,0, sizeof(str));

    print_to_console("\r\n  IP Address  : ");
    print_ipv4_addr(net_cfg.netif_static.address, str, sizeof(str));
    print_to_console(str);
    print_to_console("\r\n");

    print_to_console("  Netmask     : ");
    print_ipv4_addr(net_cfg.netif_static.mask, str, sizeof(str));
    print_to_console(str);
    print_to_console("\r\n");

    print_to_console("  Gateway     : ");
    print_ipv4_addr(net_cfg.netif_static.gw, str, sizeof(str));
    print_to_console(str);
    print_to_console("\r\n");

    print_to_console("  DNS Server  : ");
    print_ipv4_addr(net_cfg.netif_static.dns, str, sizeof(str));
    print_to_console(str);
    print_to_console("\r\n");
}

/*********************************************************************************************************************
 * @brief  print_to_console function
 *
 * This function Print the message in the serial port
 ********************************************************************************************************************/
void print_to_console(const char* msg)
{
    UINT status = 0;
    unsigned int i = 0, j = 0;
    char temp_str[128] = {0};

    status = tx_mutex_get(&g_console_mutex, TX_WAIT_FOREVER);
    if (status != TX_SUCCESS)
        return;

    /* The console framework's maximum write string length is set to
     * 128. If we see an input string >128 bytes, we chunk it up
     * and print it.
     */
    j = 0;
    do
    {
        /* Take the smaller of the remaining part of the message and
         * the size of the temporary buffer.
         */
        i = MIN(strlen(msg) - j, sizeof(temp_str) - 1);

        /* Copy the next chunk into the temporary buffer */
        memcpy(temp_str, &msg[j], i);
        temp_str[i] = '\0';

        /* Write it to the serial port */
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(const uint8_t*)temp_str, 10);

        /* Move to the next chunk */
        j += i;
    } while (j < strlen(msg));

    /* Release the mutex */
    tx_mutex_put(&g_console_mutex);
}

/*********************************************************************************************************************
 * @brief  main_menu function
 *
 * This function display the Main selection menu and collects the user input as well.
 ********************************************************************************************************************/
static uint8_t main_menu(void)
{
    char data[8] = {0};
    print_to_console("\r\n##################    Main Menu  #######################\r\n");
    print_to_console("\r\n 1. Network Interface Selection \r\n 2. GCloud IoT Core Configuration \r\n 3. Dump previous configuration from flash\r\n 4. Exit \r\n");
    print_to_console("\r\n Please Enter Your Choice:");
    print_to_console(">");
    GetUserInput(&data[0]);
    return((uint8_t)atoi(data));
}

/*********************************************************************************************************************
 * @brief  network_select_menu function
 *
 * This function display the network interface selection menu and collects the user input as well.
 *********************************************************************************************************************/
static uint8_t network_select_menu(void)
{
    char data[8] = {0};
    print_to_console("\r\nNetwork Interface Selection:\r\n 1. Ethernet\r\n 2. Wi-Fi\r\n 3. Exit\r\n");
    print_to_console("\r\n Please Enter Your Choice:");
    print_to_console(">");
    GetUserInput(&data[0]);
    print_to_console("\r\nEntered Network Interface: ");
    if(data[0] == '1')
    {
        print_to_console("Ethernet\r\n");
    }
/*
    else if(data[0] == '2')
    {
        print_to_console("Wi-Fi\r\n");
    }
    else if(data[0] == '3')
    {
        print_to_console("\r\n");
    }
*/
    else
    {
        print_to_console("Invalid Argument\r\n");
    }
    return((uint8_t)atoi(data));
}

static void ip_mode_static_menu(net_input_cfg_t *net_cfg)
{
    char data[128] = {0};
    ULONG a0 = 0, a1 = 0, a2 = 0, a3 = 0;

    print_to_console("\r\nEnter the IP Address:\r\n");
    print_to_console(">");
    GetUserInput(&data[0]);
    if(sscanf(data,"%lu.%lu.%lu.%lu",&a3, &a2, &a1, &a0) != 4)
    {
        print_to_console("\r\nInvalid IP address\r\n");
        return;
    }

    net_cfg->netif_static.address = ((a3 & 0xff) << 24) | ((a2 & 0xff) << 16) | ((a1 & 0xff) << 8) | (a0 & 0xff);

    memset(&data[0],'\0',sizeof(data));
    print_to_console("\r\nEnter Network Mask: \r\n");
    print_to_console(">");
    GetUserInput(&data[0]);
    if (sscanf(data, "%lu.%lu.%lu.%lu", &a3, &a2, &a1, &a0) != 4)
    {
        print_to_console("\r\nInvalid network mask\r\n");
        return;
    }

    net_cfg->netif_static.mask = ((a3 & 0xff) << 24) | ((a2 & 0xff) << 16) | ((a1 & 0xff) << 8) | (a0 & 0xff);
    memset(&data[0],'\0',sizeof(data));

    print_to_console("\r\nEnter Gateway:\r\n");
    print_to_console(">");
    GetUserInput(&data[0]);
    if (sscanf(data, "%lu.%lu.%lu.%lu", &a3, &a2, &a1, &a0) != 4)
    {
        print_to_console("\r\nInvalid Gateway address\r\n");
        return;
    }

    net_cfg->netif_static.gw = ((a3 & 0xff) << 24) | ((a2 & 0xff) << 16) | ((a1 & 0xff) << 8) | (a0 & 0xff);

    memset(&data[0],'\0',sizeof(data));
    print_to_console("\r\nEnter DNS:\r\n");
    print_to_console(">");
    GetUserInput(&data[0]);
    if (sscanf(data, "%lu.%lu.%lu.%lu", &a3, &a2, &a1, &a0) != 4)
    {
        print_to_console("\r\nInvalid DNS address\r\n");
        return;
    }

    print_to_console("\r\n");
    net_cfg->netif_static.dns = ((a3 & 0xff) << 24) | ((a2 & 0xff) << 16) | ((a1 & 0xff) << 8) | (a0 & 0xff);
}

/*********************************************************************************************************************
 * @brief  ip_mode_select_menu function
 *
 * This function display the IP Address Configuration selection menu and collects the user input as well.
 *********************************************************************************************************************/
static uint8_t ip_mode_select_menu(void)
{
    char data[8] = {0};

    print_to_console("\r\n Enter IP Address Configuration Mode\r\n 1. Static\r\n 2. DHCP\r\n");
    print_to_console("Please Enter Your Choice\r\n");
    print_to_console(">");

    GetUserInput(&data[0]);

    if(strlen(data) == 1)
    {
        print_to_console("\r\nEntered IP Configuration Mode: ");
        if (data[0] == '1')
        {
            print_to_console("Static\r\n");
            return IOTKIT_ADDR_MODE_STATIC;
        }
        else if (data[0] == '2')
        {
            print_to_console("DHCP\r\n");
            return IOTKIT_ADDR_MODE_DHCP;
        }
    }

    return IOTKIT_ADDR_MODE_NOTCONFIG;
}

/*********************************************************************************************************************
 * @brief  wifi_config_menu function
 *
 * This function display the Wi-Fi Configuration selection menu and collects the user input as well.
 *********************************************************************************************************************/
/*
static UINT wifi_config_menu(net_input_cfg_t *net_cfg)
{
    char data[128] = {0};

    if(NULL == net_cfg)
    {
        print_to_console("\r\n NULL pointer in function wifi_config_menu");
        return 0;
    }

    memset(&net_cfg->wifi_prov.ssid,0,sizeof(net_cfg->wifi_prov.ssid));
    memset(&net_cfg->wifi_prov.key,0,sizeof(net_cfg->wifi_prov.key));

    print_to_console("\r\n Wi-Fi Configuration");
    print_to_console("\r\nEnter the SSID associated with the Network\r\n");
    memset(&data[0], '\0', sizeof(data));
    print_to_console(">");
    GetUserInput(&data[0]);

     update SSID
    memcpy(&net_cfg->wifi_prov.ssid, &data, strlen(data));

    print_to_console("\r\nEnter the passphrase \r\n");
    print_to_console(">");
    GetUserInput(&data[0]);

     update key
    memcpy(&net_cfg->wifi_prov.key, &data, strlen(data));

    print_to_console("\r\n Enter Security Type\r\n 1. WEP\r\n 2. WPA\r\n 3. WPA2\r\n 4. None\r\n");
    print_to_console("Please Enter Your Choice\r\n");
    print_to_console(">");
    GetUserInput(&data[0]);

     Default WiFi setup
    net_cfg->wifi_prov.channel = 6;
    net_cfg->wifi_prov.encryption = SF_WIFI_ENCRYPTION_TYPE_AUTO;
    net_cfg->wifi_prov.mode = SF_WIFI_INTERFACE_MODE_CLIENT;

    print_to_console("\r\nEntered Security Type: ");

    if(data[0] == '1')
    {
        print_to_console("WEP\r\n");
        net_cfg->wifi_prov.security = SF_WIFI_SECURITY_TYPE_WEP;
    }
    else if(data[0] == '2')
    {
        print_to_console("WPA\r\n");
        net_cfg->wifi_prov.security = SF_WIFI_SECURITY_TYPE_WPA;
    }
    else if(data[0] == '3')
    {
        print_to_console("WPA2\r\n");
        net_cfg->wifi_prov.security = SF_WIFI_SECURITY_TYPE_WPA2;
    }
    else if(data[0] == '4')
    {
        print_to_console("None\r\n");
        net_cfg->wifi_prov.security = SF_WIFI_SECURITY_TYPE_OPEN;
    }
    else
    {
        print_to_console("Invalid Input\r\n");
        return 0;
    }

    return 1;
}
*/

/*********************************************************************************************************************
 * @brief  iot_service_submenu function
 *
 * This function display the IoT Cloud sub menu and collects the user input as well.
 *********************************************************************************************************************/
static uint8_t iot_service_submenu(void)
{
    char data[8] = {0};

    do {
        print_to_console("\r\n 1. Google IoT Core Setting Menu\r\n 2. Device Certificate/Keys Setting Menu\r\n 3. Exit\r\n");
        print_to_console("\r\n Please Enter Your Choice:");
        print_to_console(">");
        GetUserInput(&data[0]);

        if((data[0] == '1') || (data[0] == '2') || (data[0] == '3'))
            break;
        else
            print_to_console("\r\nInvalid Input !!!\r\n");

    }while(1);

    return((uint8_t)atoi(data));
}

/*********************************************************************************************************************
 * @brief  gcloud_submenu function
 *
 * This function gets the GCloud Thing information from the user.
 *********************************************************************************************************************/
static void gcloud_submenu(iot_input_cfg_t *gCloudcfg)
{
    char data[128] = {0};

    do {
        print_to_console("\r\n");
        print_to_console("\r\n ############ Google Cloud Settings Menu ###############\r\n");
        print_to_console("\r\n 1. Enter Project Id:\r\n 2. Enter Endpoint information:\r\n 3. Enter Device Id:\r\n");
        print_to_console(" 4. Enter Cloud Region:\r\n 5. Enter Registry Id:\r\n 6. Exit\r\n");
        print_to_console("\r\n Please Enter Your Choice:");
        print_to_console(">");
        GetUserInput(&data[0]);

        if(data[0] == '1')
        {
            print_to_console("\r\nEnter Project ID: ");
            GetUserInput(&data[0]);
            strncpy((char*)gCloudcfg->gCloud_info.project_id, data, sizeof(data));
        }
        else if(data[0] == '2')
        {
            print_to_console("\r\nEnter Endpoint information: ");
            GetUserInput(&data[0]);
            strncpy((char*)gCloudcfg->gCloud_info.endp_address, data, sizeof(data));
        }
        else if(data[0] == '3')
        {
            print_to_console("\r\nEnter Device ID: ");
            GetUserInput(&data[0]);
            strncpy((char*)gCloudcfg->gCloud_info.device_id, data, sizeof(data));
        }
        else if(data[0] == '4')
        {
            print_to_console("\r\nEnter Cloud Region: ");
            GetUserInput(&data[0]);
            strncpy((char*)gCloudcfg->gCloud_info.cloud_region, data, sizeof(data));
        }
        else if(data[0] == '5')
        {
            print_to_console("\r\nEnter Registry ID: ");
            GetUserInput(&data[0]);
            strncpy((char*)gCloudcfg->gCloud_info.registry_id, data, sizeof(data));
        }
        else if(data[0] == '6')
            break;
        else
        {
            print_to_console("Invalid Input !!! \r\n");
        }

    }while(1);
}

/*********************************************************************************************************************
 * @brief  GetrootCA function
 *
 * This function gets the rootCA certificate from the user and store in the internal flash.
 *********************************************************************************************************************/
static unsigned int GetrootCA(void)
{
    unsigned int i = 0, j =0;
    
    int skipBegin = 0;
    char *begin_CA = "-----BEGIN CERTIFICATE-----";
    char *end_CA = "-----END CERTIFICATE-----";
    memset(certPEM,0,sizeof(certPEM));
    memset(certDER,0,sizeof(certDER));
    memset(str,0, sizeof(str));

    do {
        g_sf_console.p_api->read(g_sf_console.p_ctrl, (uint8_t*)&str[0], 66, TX_WAIT_FOREVER);

        if(!skipBegin)
        {
            skipBegin = 1;
            if(str[0] == '-')
            {
                i += strlen(begin_CA);
                strncpy((char*)&certPEM[0], (char*)&str[i],sizeof(str));
                i = (strlen((char*)str) - strlen(begin_CA));
            }

            continue;
        }

        else if(strcmp((char*)&str[0], end_CA) == 0)
            break;
        else
        {
            strncpy((char*)&certPEM[i], (char*)&str[0],sizeof(str));
        }

        i +=strlen((char*)str);

    }while(1);

    base64_decode(&certPEM[0], i ,&certDER[0]);

    for(j = 0; j <= (unsigned int)base64d_size(i); j++)
        certPEM[j] = (unsigned char)certDER[j];

    //store the rootCA certificate in internal flash
    if(int_storage_write((uint8_t*)&certPEM[0], (unsigned int)base64d_size(i), ROOTCA_CERT_CFG) != SSP_SUCCESS)
    {
        print_to_console("\r\nFlash Write Failed\r\n");
        APP_ERR_TRAP(1);
    }
    else
        print_to_console("\r\nrootCA Certificate stored in flash\r\n");

    return((unsigned int)base64d_size(i));
}


/*********************************************************************************************************************
 * @brief  GetPrivKey function
 *
 * This function gets the private key from the user and store in the internal flash.
 *********************************************************************************************************************/
static unsigned int GetPrivKey(void)
{
    unsigned int i = 0, j =0;
    int skipBegin = 0;    
    char *begin_CA = "-----BEGIN RSA PRIVATE KEY-----";
    char *end_CA = "-----END RSA PRIVATE KEY-----";
    memset(certPEM,0,sizeof(certPEM));
    memset(certDER,0,sizeof(certDER));
	memset(str,0, sizeof(str));

    do {
        g_sf_console.p_api->read(g_sf_console.p_ctrl, (uint8_t*)&str[0], 66, TX_WAIT_FOREVER);

        if(!skipBegin)
        {
            skipBegin = 1;
            if(str[0] == '-')
            {
                i += strlen(begin_CA);
                strncpy((char*)&certPEM[0], (char*)&str[i], sizeof(str));
                i = (strlen((char*)str) - strlen(begin_CA));
            }

            continue;
        }
        else if(strcmp((char*)&str[0], end_CA) == 0)
            break;
        else
        {
            strncpy((char*)&certPEM[i], (char*)&str[0], sizeof(str));
        }

        i +=strlen((char*)str);

    }while(1);

    base64_decode(&certPEM[0], i ,&certDER[0]);

    for(j = 0; j <= (unsigned int)base64d_size(i); j++)
        certPEM[j] = (unsigned char)certDER[j];

    /* store the private key in internal flash */
    if(int_storage_write((uint8_t*)&certPEM[0], (unsigned int)base64d_size(i), PRI_KEY_CFG) != SSP_SUCCESS)
    {
        print_to_console("Flash Write Failed\r\n");
        APP_ERR_TRAP(1);
    }
    else
        print_to_console("Private key stored in flash\r\n");

    return((unsigned int)base64d_size(i));
}

/*********************************************************************************************************************
 * @brief  devCert_menu function
 *
 * This function display the certificates/keys menu option and wait for the user input.
 *********************************************************************************************************************/
static void devCert_menu(dev_cert_cfg *cert_info)
{
    char data[8] = {0};

    do {
        print_to_console("\r\n");
        print_to_console("\r\n Certificate/Keys Settings Menu\r\n");
        print_to_console("\r\n 1. Enter rootCA Certificate\r\n 2. Enter Thing Private Key\r\n 3. Exit\r\n");
        print_to_console("\r\n Please Enter Your Choice:");
        print_to_console(">");
        GetUserInput(&data[0]);

        if(data[0] == '1')
        {
            print_to_console("\r\n Enter rootCA Certificate: <paste rootCA and Press Enter key> \r\n");
            cert_info->rootCA_len = GetrootCA();
        }
        else if(data[0] == '2')
        {
            print_to_console("\r\n Enter Thing Private Key: <paste Thing private key and Press Enter key> \r\n");
            cert_info->priKey_len = GetPrivKey();
        }
        else if(data[0] == '3')
            break;
        else
            print_to_console("Invalid Input !!! \r\n");

    }while(1);
}

/*********************************************************************************************************************
 * @brief  config_menu_callback function
 *
 * This function handles the cwiz user command option from CLI
 *********************************************************************************************************************/
void config_menu_callback(sf_console_cb_args_t *p_args)
{
    uint8_t result = NETIF_MAX;
    e_config_state net_wizard_state = STATE_NETIF_SELECTION;
    uint8_t menu_cfg = 0, addr_mode = 0;
    net_input_cfg_t net_cfg;
    iot_input_cfg_t iot_cfg;
    dev_cert_cfg cert_info;    
    uint8_t iot_serv_select = 0;

    SSP_PARAMETER_NOT_USED(p_args);

    /* clear the network input cfg structure */
    memset(&net_cfg,0,sizeof(net_cfg));
    memset(&iot_cfg,0,sizeof(iot_cfg));
    memset(&cert_info,0,sizeof(cert_info));
    memset(str,0,sizeof(str));

    while(menu_cfg != MAIN_CONFIG_EXIT)
    {
        menu_cfg = main_menu();

        switch(menu_cfg)
        {
            case NET_CONFIG:

                net_wizard_state = STATE_NETIF_SELECTION;

                while(net_wizard_state != STATE_CONFIG_EXIT)
                {
                    switch(net_wizard_state)
                    {
                        case STATE_NETIF_SELECTION:
                            /* Show Network selection menu */
                            result = network_select_menu();

                            switch(result)
                            {
                                case ETHER_INF:
                                    net_cfg.netif_valid = 1;
                                    net_cfg.interface_index = ETHERNET;
                                    net_wizard_state = STATE_ADDR_MODE_CONFIG;
                                    break;

                                case WIFI_INF:
                                    net_cfg.netif_valid = 1;
                                    net_cfg.interface_index = WIFI;
                                    net_wizard_state = STATE_WIFI_CONFIG;
                                    break;
                                case EXIT:
                                    net_wizard_state = STATE_CONFIG_EXIT;
                                    break;
                                default:
                                    print_to_console("\r\nInvalid Input !!!\r\n");
                            }

                            break;

/*
                        case STATE_WIFI_CONFIG:
                            Display Wi-Fi Configuration menu
                            if(wifi_config_menu(&net_cfg))
                            {
                                net_wizard_state = STATE_ADDR_MODE_CONFIG;
                            }
*/

                            break;

                        case STATE_ADDR_MODE_CONFIG:
                            /* IP Address Configuration mode selection */
                            addr_mode = ip_mode_select_menu();
                            if(addr_mode != IOTKIT_ADDR_MODE_NOTCONFIG)
                            {
                                net_wizard_state = STATE_CONFIG_EXIT;

                                if(addr_mode == IOTKIT_ADDR_MODE_DHCP)
                                    net_cfg.netif_addr_mode = IOTKIT_ADDR_MODE_DHCP;
                                else
                                {
                                    net_cfg.netif_addr_mode = IOTKIT_ADDR_MODE_STATIC;

                                    ip_mode_static_menu(&net_cfg);
                                }

                                /* write the netif information in the flash */
                                if(int_storage_write((uint8_t*)&net_cfg, sizeof(net_cfg), NET_INPUT_CFG) != SSP_SUCCESS)
                                {
                                    print_to_console("NETCFG Flash Write Failed\r\n");
                                    APP_ERR_TRAP(1);
                                }
                                else
                                    print_to_console("Network Configuration stored in flash\r\n");
                            }
                            else
                            {
                                net_wizard_state = STATE_ADDR_MODE_CONFIG;
                            }

                            break;

                        default:
                            print_to_console("\r\nInvalid Input !!!\r\n");
                            break;
                    }
                }

                break;

            case CLOUD_CONFIG:

                do {
                    iot_serv_select = iot_service_submenu();

                    if(iot_serv_select == 1)
                    {
                        gcloud_submenu(&iot_cfg);
                        iot_cfg.iotserv_valid = 1;

                        /* write the iot service information in the flash */
                        if(int_storage_write((uint8_t*)&iot_cfg, sizeof(iot_cfg), IOT_INPUT_CFG) != SSP_SUCCESS)
                        {
                            print_to_console("\r\nFlash Write Failed!!!\r\n");
                            APP_ERR_TRAP(1);
                        }
                        else
                        {
                            print_to_console("\r\nDevice Certificate information stored in flash\r\n");
                        }
                    }
                    else if(iot_serv_select == 2)
                    {
                        devCert_menu(&cert_info);
                        /* write the iot service information in the flash */
                        if(int_storage_write((uint8_t*)&cert_info, sizeof(cert_info), CERT_CFG_TYPE) != SSP_SUCCESS)
                        {
                            print_to_console("\r\nFlash Write Failed!!!\r\n");
                            APP_ERR_TRAP(1);
                        }
                        else
                        {
                            print_to_console("\r\nDevice Certificate information stored in flash\r\n");
                        }
                    }

                }while(iot_serv_select != 3);

                break;

            case DUMP_CONFIG:

                if(SSP_SUCCESS != int_storage_read((uint8_t *)&net_cfg, sizeof(net_cfg), NET_INPUT_CFG))
                {
                    print_to_console("\r\nFlash read failed!!!\r\n");
                    APP_ERR_TRAP(1);
                }

                if(SSP_SUCCESS != int_storage_read((uint8_t*)&iot_cfg, sizeof(iot_cfg),IOT_INPUT_CFG))
                {
                    print_to_console("\r\nFlash read failed!!!\r\n");
                    APP_ERR_TRAP(1);
                }

                if(SSP_SUCCESS != int_storage_read((uint8_t*)&cert_info, sizeof(cert_info),CERT_CFG_TYPE))
                {
                    print_to_console("\r\nFlash read failed!!!\r\n");
                    APP_ERR_TRAP(1);
                }

                print_to_console("\r\n");
                print_to_console("\r\n ################### Flash Dump Start#########################\r\n");

                if(net_cfg.netif_valid)
                {
                    if(net_cfg.interface_index == ETHERNET)
                        print_to_console("\r\nNetwork Interface selected: Ethernet\r\n");
/*
                    else if(net_cfg.interface_index == WIFI)
                        print_to_console("\r\nNetwork Interface selected: WiFi\r\n");
*/
                    
                    if(net_cfg.netif_addr_mode == IOTKIT_ADDR_MODE_DHCP)
                    {
                        sprintf(str,"IP Mode: %s", "DHCP");
                        print_to_console(str);
                        print_to_console("\r\n");
                    }
                    else
                    {
                        sprintf(str,"IP Mode: %s", "Static");
                        print_to_console(str);
                        print_to_console("\r\n");
                        print_static_addr(net_cfg);
                    }

/*                    if(net_cfg.interface_index == WIFI)
                    {
                        print_to_console("\r\n");
                        print_to_console("WiFi Configuration\r\n");
                        snprintf(str, sizeof(str), "  SSID        : %s\r\n", net_cfg.wifi_prov.ssid);
                        print_to_console(str);
                        snprintf(str, sizeof(str), "  Key         : %s\r\n", net_cfg.wifi_prov.key);
                        print_to_console(str);

                        snprintf(str, sizeof(str), "  Security    : ");
                        if (net_cfg.wifi_prov.security == SF_WIFI_SECURITY_TYPE_WPA2)
                            strcat(str, "WPA2");
                        else if (net_cfg.wifi_prov.security == SF_WIFI_SECURITY_TYPE_OPEN)
                            strcat(str, "Open");
                        else if (net_cfg.wifi_prov.security == SF_WIFI_SECURITY_TYPE_WEP)
                            strcat(str, "WEP");
                        else if (net_cfg.wifi_prov.security == SF_WIFI_SECURITY_TYPE_WPA)
                            strcat(str, "WPA");
                        else
                            strcat(str, "Unknown");

                        print_to_console(str);
                        print_to_console("\r\n");
                        print_to_console("\r\n");
                     }*/
                }
                else
                    print_to_console("No Network Interface selected. Run cwiz command\r\n");

                if(iot_cfg.iotserv_valid)
                {
                    snprintf(str,sizeof(str), "GCloud Endpoint:  %s", iot_cfg.gCloud_info.endp_address);
                    print_to_console(str);
                    print_to_console("\r\n");
                    snprintf(str,sizeof(str), "GCloud Project ID: %s", iot_cfg.gCloud_info.project_id);
                    print_to_console(str);
                    print_to_console("\r\n");
                    snprintf(str,sizeof(str), "GCloud Device ID: %s", iot_cfg.gCloud_info.device_id);
                    print_to_console(str);
                    print_to_console("\r\n");
                    snprintf(str,sizeof(str), "GCloud Cloud Region: %s", iot_cfg.gCloud_info.cloud_region);
                    print_to_console(str);
                    print_to_console("\r\n");
                    snprintf(str,sizeof(str), "GCloud Registry Id: %s", iot_cfg.gCloud_info.registry_id);
                    print_to_console(str);
                    print_to_console("\r\n");
                }

                snprintf(str, sizeof(str), "rootCA len: %d\r\n ",cert_info.rootCA_len);
                print_to_console(str);
                snprintf(str, sizeof(str), "devicePrivKey len: %d\r\n",cert_info.priKey_len);
                print_to_console(str);

                print_to_console("\r\n ################## Flash Dump End #########################\r\n");
                print_to_console("\r\n");
                break;

            case MAIN_CONFIG_EXIT:
                menu_cfg = MAIN_CONFIG_EXIT;
                print_to_console("\r\n");
                break;

            default:
                print_to_console("\r\nInvalid Input !!!\r\n");
                break;
        }
    }
}

/*********************************************************************************************************************
 * @brief  demo_service_callback function
 *
 * This function handles the demo command option from CLI
 *********************************************************************************************************************/
void demo_service_callback(sf_console_cb_args_t *p_args)
{
    net_input_cfg_t user_cfg;
    iot_input_cfg_t iot_cfg;
    dev_cert_cfg cert_info;
    ssp_err_t ssp_err = SSP_SUCCESS;

    memset(&user_cfg, 0, sizeof(user_cfg));
    memset(&iot_cfg, 0, sizeof(iot_cfg));
    memset(&cert_info, 0, sizeof(cert_info));
	
    if((strcmp((void*)p_args->p_remaining_string, "start") == 0))
    {
        ssp_err = int_storage_read((uint8_t *)&user_cfg, sizeof(user_cfg), NET_INPUT_CFG);
        if(ssp_err != SSP_SUCCESS)
        {
            print_to_console("Flash read failed\r\n");
            return;
        }

        if(SSP_SUCCESS != int_storage_read((uint8_t*)&iot_cfg, sizeof(iot_cfg),IOT_INPUT_CFG))
        {
            print_to_console("Flash read failed\r\n");
            APP_ERR_TRAP(1);
        }

        if(SSP_SUCCESS != int_storage_read((uint8_t*)&cert_info, sizeof(cert_info),CERT_CFG_TYPE))
        {
            print_to_console("Flash read failed\r\n");
            APP_ERR_TRAP(1);
        }

        if(user_cfg.netif_valid == 0 )
        {
            print_to_console("Network Interface is not selected. Run cwiz command\r\n");
            return;
        }

        if(iot_cfg.iotserv_valid == 0)
        {
            print_to_console("IoT service is not selected. Run cwiz command\r\n");
            return;
        }

        if(cert_info.rootCA_len == 0 || cert_info.priKey_len == 0)
        {
            print_to_console("Certificates not programmed. Run cwiz command\r\n");
            return;
        }

        tx_event_flags_set(&g_sys_event_flags, SYS_EV_DEMO_START, TX_OR);
    }
    else if(strcmp((void*)p_args->p_remaining_string, "stop") == 0)
        tx_event_flags_set(&g_sys_event_flags, SYS_EV_DEMO_STOP, TX_OR);
    else
        print_to_console("Invalid Argument\r\n");
}

static const sf_console_command_t g_sf_console_commands[] =
{
     {
       .command = (uint8_t *)"cwiz",
       .help = (uint8_t *)"Network/Cloud Configuration Menu \r\n"
               "           Usage: cwiz \r\n",
               .callback = config_menu_callback,
               .context = NULL
      },
      {
       .command = (uint8_t*)"demo",
       .help = (uint8_t*)"Start/Stop Synergy Cloud Connectivity Demo \r\n"
             "          Usage: demo <start>/<stop> \r\n",
             .callback = demo_service_callback,
             .context = NULL
      },
};

const sf_console_menu_t g_sf_console_root_menu =
{
 .menu_prev = NULL,
 .menu_name = (uint8_t *)"",
 .num_commands = (sizeof(g_sf_console_commands)) / (sizeof(g_sf_console_commands[0])),
 .command_list = &g_sf_console_commands[0]
};

/* Center the string that needs to be displayed */
static void center_and_print_string(char *temp_str)
{
    unsigned int len = strlen(temp_str);
    unsigned int pad = 0;
    unsigned int i   = 0;

    print_to_console("*");
    /* Length of line for most terminal programs is 80 columns.
     * We leave a space at the end since some programs wrap the line
     * after 79 characters. There is an asterisk at the start and end
     * of each line. Therefore effective line width is 77 columns.
     */
    pad = (77 - len) / 2;
    for (i = 0; i < pad; i++)
        print_to_console(" ");
    print_to_console(temp_str);

    /* Padding may be unequal, adjust the trailing spaces */
    if((2 * pad) + len != 77)
        pad += 77 - ((2 * pad) + len);
    for (i = 0; i < pad; i++)
        print_to_console(" ");

    print_to_console("\r\n");
}

/*********************************************************************************************************************
 * @brief  console_thread_entry function
 *
 * This function is the console thread entry function
 *********************************************************************************************************************/
/* Console Thread entry function */
void console_thread_entry(void)
{
    uint8_t ch[1] = {0};    
    ssp_pack_version_t ssp_version;
    memset(str, 0, sizeof(str));

    g_sf_console.p_api->read (g_sf_console.p_ctrl, ch, 1, TX_WAIT_FOREVER);

    R_SSP_VersionGet(&ssp_version);

    print_to_console ("\r\n********************************************************************************\r\n");
    snprintf(str, sizeof(str), "Renesas Synergy Google IoT Cloud Connectivity Application\r\n" );
    center_and_print_string(str);
    snprintf(str, sizeof(str), "FW version  %d.%d.%d  -  %s, %s\r\n", MQTT_AP_MAJOR_VER, MQTT_AP_MINOR_VER,MQTT_AP_PATCH_VER, __DATE__, __TIME__);
    center_and_print_string(str);
    snprintf(str, sizeof(str),"Synergy Software Package Version: %d.%d.%d", ssp_version.major, ssp_version.minor, ssp_version.patch);
    center_and_print_string(str);
    print_to_console ("\r\n********************************************************************************\r\n");

    /* Initialize sensors */
/*
    if(SSP_SUCCESS != init_sensors())
    {
        print_to_console("\r\n Sensor Init Failed!!!\r\n");
        APP_ERR_TRAP(1);
    }
*/

    while (true)
    {
        g_sf_console.p_api->prompt(g_sf_console.p_ctrl, NULL, TX_WAIT_FOREVER);
    }
}
