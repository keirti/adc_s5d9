/***********************************************************************************************************************
* File Name    : ether_interface.c
***********************************************************************************************************************/

#include "MQTT_Config.h"
#include "iot_service_api.h"
#include "console_config.h"
#include "sntp_client.h"
#include "internal_flash.h"


/* These global variables are used within the context of MQTT_Thread */
NX_DNS  *g_active_dns = NULL;
NX_IP   *g_active_ip = NULL;                /* Pointer to active IP interface */
NX_DHCP *g_active_dhcp = NULL;              /* Pointer to active DHCP client */
NX_SNTP_CLIENT *g_active_sntp = NULL;       /* Pointer to active SNTP client */
uint8_t g_active_interface[16];             /* Active interface name */

/* Function Prototype declaration */
uint8_t configure_static_ip(uint8_t addr_mode);
UINT init_network_interface(mqtt_client_cfg_t *p_cfg, net_input_cfg_t *net_cfg);
UINT deinit_network_service(net_input_cfg_t *net_cfg);
UINT reset_network_interface(net_input_cfg_t net_cfg);
void print_ipv4_addr(ULONG address, char *str, size_t len);
extern void print_to_console(const char* msg);
/*********************************************************************************************************************
 * @brief  print_ipv4_addr function
 *
 * This function Print IPv4 address
 ********************************************************************************************************************/
void print_ipv4_addr(ULONG address, char *str, size_t len)
{
    if(NULL == str)
    {
        print_to_console("\r\n NULL pointer in function print_ipv4_addr\r\n");
        return;
    }

    snprintf(str, len, "%lu.%lu.%lu.%lu",
             (address >> 24) & 0xff,
             (address >> 16) & 0xff,
             (address >> 8) & 0xff,
             (address >> 0) & 0xff);
}

UINT reset_network_interface(net_input_cfg_t net_cfg)
{
    UINT status = NX_SUCCESS;
    NX_IP_DRIVER    driver_request;

    if(SSP_SUCCESS != int_storage_read((uint8_t *)&net_cfg, sizeof(net_cfg), NET_INPUT_CFG))
    {
        print_to_console("Failed to read network user input configuration from internal storage \r\n");
        print_to_console("Please provision the device by running the \"cwiz\" command\r\n");
        APP_ERR_TRAP(SSP_ERR_ABORTED);
    }

    if(net_cfg.netif_addr_mode == IOTKIT_ADDR_MODE_DHCP)
    {
        status = nx_dhcp_stop(g_active_dhcp);
        if(NX_SUCCESS != status)
        {
            return status;
        }
    }

    status = nx_ip_address_set(g_active_ip, 0 , 0);
    if(NX_SUCCESS != status)
    {
        return status;
    }

    if(ETHERNET == net_cfg.interface_index)
    {
        /* Call the link driver to disable the interface */
        driver_request.nx_ip_driver_ptr = g_active_ip;
        driver_request.nx_ip_driver_command = NX_LINK_DISABLE;

        (g_active_ip->nx_ip_interface[0].nx_interface_link_driver_entry)(&driver_request);

        /* Call the link driver to de-initialize the hardware */
        driver_request.nx_ip_driver_command = NX_LINK_UNINITIALIZE;
        (g_active_ip->nx_ip_interface[0].nx_interface_link_driver_entry)(&driver_request);
    }

    status = nx_ip_interface_detach(g_active_ip, 0);
    if(NX_SUCCESS != status)
    {
        return status;
    }

    return status;
}

UINT deinit_network_service(net_input_cfg_t *net_cfg)
{
    UINT status = NX_SUCCESS;
    char str[128] = {0};

    print_to_console("\r\nDe-initializing  N/W Interface... ");

    if(net_cfg->netif_addr_mode == IOTKIT_ADDR_MODE_DHCP)
    {
        status = nx_dhcp_stop(g_active_dhcp);
        if(NX_SUCCESS != status)
        {
            snprintf(str,sizeof(str),"nx_dhcp_interface_stop failed!!!, ret = 0x%x\r\n",status);
            print_to_console(str);
            return status;
        }
    }

    status = nx_ip_address_set(g_active_ip, 0 , 0);
    if(NX_SUCCESS != status)
    {
        snprintf(str,sizeof(str),"nx_ip_interface_address_set failed!!!, ret = 0x%x\r\n", status);
        print_to_console(str);
        return status;
    }

    if(net_cfg->interface_index == WIFI)
    {
        status = nx_ip_interface_detach(g_active_ip, 0);
        if(NX_SUCCESS != status)
        {
            snprintf(str,sizeof(str), "nx_ip_interface_detach failed!!!, ret = 0x%x\r\n",status);
            print_to_console(str);
            return status;
        }
    }

    if(net_cfg->interface_index == ETHERNET)
    {
        NX_IP_DRIVER    driver_request;
        /* Call the link driver to disable the interface */
        driver_request.nx_ip_driver_ptr = g_active_ip;
        driver_request.nx_ip_driver_command = NX_LINK_DISABLE;

        (g_active_ip->nx_ip_interface[0].nx_interface_link_driver_entry)(&driver_request);

        /* Call the link driver to de-initialize the hardware */
        driver_request.nx_ip_driver_command = NX_LINK_UNINITIALIZE;
        (g_active_ip->nx_ip_interface[0].nx_interface_link_driver_entry)(&driver_request);

        status = nx_ip_interface_detach(g_active_ip, 0);
        if(NX_SUCCESS != status)
        {
            return status;
        }
    }

    print_to_console("Done\r\n");
    return status;
}

/*********************************************************************************************************************
 * @brief  configure_dhcp function
 *
 * This function configure the IP mode to DHCP
 ********************************************************************************************************************/
static UINT configure_dhcp(net_input_cfg_t *net_cfg)
{
    CHAR str[96] = {0};
    UINT status = NX_SUCCESS;
    ULONG opt[8] = {0};
    UINT opt_len = 0;
    UINT retry_cnt = 10;
    ULONG ip_status = 0;
    ULONG dns_srv_address = 0;
    ULONG address = 0, mask = 0;

    if(NULL == net_cfg)
    {
        print_to_console("\r\nNULL pointer reference in configure_dhcp function\r\n");
        return 1;
    }

    print_to_console("Starting DHCP Client...");
    status = nx_dhcp_start(g_active_dhcp);
    if(NX_DHCP_ALREADY_STARTED != status)
    {
        if (NX_SUCCESS != status)
        {
            snprintf(str,sizeof(str),"Could not start DHCP client, ret = 0x%x\r\n",status);
            print_to_console(str);
            return status;
        }

        print_to_console("done\r\n");
    }
    else
    {
        print_to_console("Already Running!!!\r\n");
    }

    print_to_console("\r\nWaiting for IP address.");

    do
    {
        /* Check for address resolution. */
        status = nx_ip_status_check (g_active_ip, NX_IP_ADDRESS_RESOLVED, (ULONG *) &ip_status, 1000);
        if(NX_SUCCESS == status && NX_IP_ADDRESS_RESOLVED == ip_status)
        {
            opt_len = sizeof(opt);
            status = nx_dhcp_user_option_retrieve(g_active_dhcp, NX_DHCP_OPTION_DNS_SVR,(UCHAR *)opt, &opt_len);
            if (NX_SUCCESS == status && opt_len >= 4)
            {
                /* Remove all old DNS servers */
                status = nx_dns_server_remove_all(g_active_dns);
                if(NX_SUCCESS != status)
                {
                    snprintf(str,sizeof(str),"Unable to remove the DNS server, ret = 0x%x\r\n",status);
                    print_to_console(str);
                    return status;
                }

                dns_srv_address = opt[0];
                status = nx_dns_server_add(g_active_dns, dns_srv_address);
                if(status != NX_SUCCESS)
                {
                    snprintf(str,sizeof(str),"nable to set DNS server address, ret = 0x%x\r\n",status);
                    print_to_console(str);
                    return status;
                }
            }
            print_to_console("done\r\n\r\n");
            break;
        }
        print_to_console(".");
    }while (--retry_cnt);

    if(retry_cnt > 0)
    {
        status = nx_ip_address_get(g_active_ip, &address, &mask);
        if(NX_SUCCESS != status)
            address = mask = 0;

        print_to_console("\r\n");
        print_to_console("IP Configuration\r\n");
        print_to_console("================\r\n");
        print_to_console("  Interface   : ");
        snprintf(str, sizeof(str), "%s\r\n", g_active_interface);
        print_to_console(str);

        print_to_console("\r\n  IP Address  : ");
        print_ipv4_addr(address, str, sizeof(str));
        print_to_console(str);
        print_to_console("\r\n");

        /* If no IP address assigned, return with error */
        if(address == 0)
            return 1;

        print_to_console("  Netmask     : ");
        print_ipv4_addr(mask, str, sizeof(str));
        print_to_console(str);
        print_to_console("\r\n");

        print_to_console("  Gateway     : ");
        address = g_active_ip->nx_ip_gateway_address;
        print_ipv4_addr(address, str, sizeof(str));
        print_to_console(str);
        print_to_console("\r\n");

        /* If no Gateway address assigned, return with error */
        if(address == 0)
            return 1;

        status = nx_dhcp_server_address_get(g_active_dhcp, &address);
        if (NX_SUCCESS != status)
            address = 0;

        /*If no DHCP Server address assigned, return with error*/
        if(address == 0)
            return status;

        print_to_console("  DHCP Server : ");
        print_ipv4_addr(address, str, sizeof(str));
        print_to_console(str);
        print_to_console("\r\n");

        print_to_console("  DNS Server  : ");
        nx_dns_server_get(g_active_dns, 0, &address);

        /*If no DNS Server address assigned, return with error*/
        if(address == 0)
            return status;

        print_ipv4_addr(address, str, sizeof(str));
        print_to_console(str);
        print_to_console("\r\n");

        print_to_console("**************************************\r\n");

        return status;
    }
    else
        return status;
}

/*********************************************************************************************************************
 * @brief  iotkit_static_ip_set function
 *
 * This function sets the IP mode to static mode
 ********************************************************************************************************************/
static UINT iotkit_static_ip_set(net_input_cfg_t *net_cfg)
{
    UINT status = NX_SUCCESS;
    char str[64] = {0};

    if(NULL == net_cfg)
    {
        print_to_console("\r\nNULL pointer reference in iotkit_static_ip_set function\r\n");
        return 1;
    }

    net_cfg->netif_addr_mode = IOTKIT_ADDR_MODE_STATIC;
    status = nx_ip_address_set(g_active_ip, net_cfg->netif_static.address, net_cfg->netif_static.mask);
    if (NX_SUCCESS != status)
    {
        snprintf(str,sizeof(str),"Unable to set static IP address, ret = 0x%x\r\n",status);
        print_to_console(str);
        return status;
    }

    status = nx_ip_gateway_address_set(g_active_ip, net_cfg->netif_static.gw);
    if (NX_SUCCESS != status)
    {
        snprintf(str,sizeof(str),"Unable to set Gateway IP address, ret = 0x%x\r\n",status);
        print_to_console(str);
        return status;
    }

    /* Remove all old DNS servers */
    status = nx_dns_server_remove_all(g_active_dns);
    if (NX_SUCCESS != status)
    {
        print_to_console("Unable to remove the dns server\r\n");
        return status;
    }

    status = nx_dns_server_add(g_active_dns, net_cfg->netif_static.dns);
    if (NX_SUCCESS != status)
    {
        snprintf(str,sizeof(str),"Unable to set DNS server address, ret = 0x%x\r\n",status);
        print_to_console(str);
        return status;
    }
    return NX_SUCCESS;
}

/*********************************************************************************************************************
 * @brief  wifi_setup function
 *
 * This function provision the Wi-Fi communication interface
 ********************************************************************************************************************/
/*static UINT wifi_setup(net_input_cfg_t *net_cfg)
{
    ULONG ip_status = 0;
    UINT status = NX_SUCCESS;
    ssp_err_t ssp_err = SSP_SUCCESS;
    int i = 0;
    char str[128] = {0};

    if(NULL == net_cfg)
    {
        print_to_console("\r\n NULL pointer reference in wifi_setup fun\r\n");
        return 1;
    }

    status = nx_ip_interface_attach(g_active_ip, "wifi", IP_ADDRESS(0,0,0,0), IP_ADDRESS(0,0,0,0), g_sf_el_nx_wifi);
    if(status != NX_SUCCESS)
    {
        snprintf(str,sizeof(str),"nx_ip_interface_attach failed!!, ret = 0x%x\r\n",status);
        print_to_console(str);
        return status;
    }

    status = nx_ip_status_check(g_active_ip, NX_IP_LINK_ENABLED, &ip_status, 10000);
    if(status == NX_SUCCESS)
    {
        print_to_console("\r\n Wi-Fi setup may take a few moments and may require a few retries.\r\n");

        for(i = 0; i < WIFI_PROV_RETRY_COUNT; i++)
        {
            print_to_console("Connecting to SSID \"");
            print_to_console((char*)net_cfg->wifi_prov.ssid);
            print_to_console("\": ");
            print_to_console("Key \"");
            print_to_console((char*)net_cfg->wifi_prov.key);
            print_to_console("\": ");
            ssp_err = g_sf_wifi0.p_api->provisioningSet(g_sf_wifi0.p_ctrl, &net_cfg->wifi_prov);
            if(ssp_err != SSP_SUCCESS)
            {
                print_to_console("failed. ");
                if(i < WIFI_PROV_RETRY_COUNT - 1)
                    print_to_console("Retrying...\r\n");
                else
                    print_to_console("\r\n");
            }
            else
            {
                print_to_console("connected.\r\n\r\n");

                return status;
            }
        }
    }
    else
    {
        print_to_console("Wi-Fi link not up.\r\n");
    }

    print_to_console("\r\n");
    print_to_console("1. Please check that the Wi-Fi module is properly connected.\r\n");
    print_to_console("2. Please check your Wi-Fi SSID, password and security mode.\r\n");
    print_to_console("   If incorrect, please re-enter them using the \"wifi\" command.\r\n");
    print_to_console("3. Please check that your Wi-Fi Access Point is working properly.\r\n");
    print_to_console("\r\n");

    return 1;
}
*/

/*********************************************************************************************************************
 * @brief  init_network_interface function
 *
 * This function initializes the network interface
 ********************************************************************************************************************/
UINT init_network_interface(mqtt_client_cfg_t *p_cfg, net_input_cfg_t *net_cfg)
{
    UINT status = NX_SUCCESS;
    ULONG dhcp_init_status = 0;
    ULONG     actual_events = 0;
    static uint8_t init_sntp = 0;
    char str[128] = {0};

    if(NULL == p_cfg || NULL == net_cfg)
    {
        print_to_console("\r\n NULL pointer in function init_network_interface\r\n");
        return 1;
    }

    print_to_console("\r\nInitializing Network Interface...\r\n");

    if (net_cfg->interface_index == ETHERNET)
    {
        strcpy((char*)g_active_interface, "Ethernet");

        if(net_cfg->netif_addr_mode == IOTKIT_ADDR_MODE_DHCP)
        {
            status = nx_ip_interface_attach(g_active_ip, "ethernet", IP_ADDRESS(0,0,0,0), IP_ADDRESS(0,0,0,0), g_sf_el_nx_ethernet);
            if(status != NX_SUCCESS)
            {
                snprintf(str,sizeof(str),"nx_ip_interface_attach failed!!, ret = 0x%x\r\n",status);
                print_to_console(str);
                return status;
            }

            status = nx_ip_status_check (g_active_ip, NX_IP_LINK_ENABLED, &actual_events, ETH_LINK_TIMEOUT);
            if (NX_SUCCESS != status)
            {
                snprintf(str, sizeof(str), "nx_ip_status_check failed!!, ret = 0x%x\r\n",status);
                print_to_console(str);
                return status;
            }

            dhcp_init_status = configure_dhcp(net_cfg);
            if(NX_SUCCESS != dhcp_init_status )
            {
                return dhcp_init_status;
            }
        }
        else if(net_cfg->netif_addr_mode == IOTKIT_ADDR_MODE_STATIC)
        {
            status = iotkit_static_ip_set(net_cfg);
            if(status != NX_SUCCESS)
                return status;
        }
    }
/*    else if(net_cfg->interface_index == WIFI)
    {
         Setup and provision WiFi interface
        status = wifi_setup(net_cfg);
        if(NX_SUCCESS == status)
        {
            strcpy((char*)g_active_interface, "WiFi");

            if(net_cfg->netif_addr_mode == IOTKIT_ADDR_MODE_DHCP)
            {
                dhcp_init_status = configure_dhcp(net_cfg);
                if(dhcp_init_status != NX_SUCCESS)
                {
                    return dhcp_init_status;
                }
            }
            else if(net_cfg->netif_addr_mode == IOTKIT_ADDR_MODE_STATIC)
            {
                status = iotkit_static_ip_set(net_cfg);
                if(status != NX_SUCCESS)
                    return status;
            }
        }
        else
            return status;
    }*/
    else
    {
        print_to_console("Invalid network interface!!!\r\n");
        status = NX_ENTRY_NOT_FOUND;
    }


    /* Start SNTP client */
    if(init_sntp == 0)
    {
        init_sntp = 1;

        /* Start SNTP client */
        status = sntp_setup_and_run();
        if(NX_SUCCESS != status)
           return status;
    }

    return NX_SUCCESS;
}


