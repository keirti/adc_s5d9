/***********************************************************************************************************************
* File Name    : console_config.h
* Description  : This file contains console and CLI specific data structures.
***********************************************************************************************************************/

/***********************************************************************************************************************

* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#ifndef CONSOLE_CONFIG_H_
#define CONSOLE_CONFIG_H_


/** Network interface types */
typedef enum
{
    ETHERNET = 0,         ///< Ethernet
    WIFI,                 ///< Wi-Fi    
    EXIT_NET_MENU,
    NETIF_MAX,              ///< Maximum index, used to size arrays
}e_netif;

typedef enum
{
    ETHER_INF = 1,
    WIFI_INF,    
    EXIT,
}e_nwif;

/** IP Address Mode */
enum e_addr_mode
{
    IOTKIT_ADDR_MODE_DHCP,  ///< Obtain address via DHCP
    IOTKIT_ADDR_MODE_STATIC,///< Static IP address
    IOTKIT_ADDR_MODE_NOTCONFIG, ///< Mode not selected/configure
};

typedef enum
{
    STATE_NETIF_SELECTION = 1,       ///< Selection of Network Interface
    STATE_WIFI_CONFIG,               ///< Wi-Fi Configuration
    STATE_ADDR_MODE_CONFIG,          ///< IP address mode Configuration Menu
    STATE_CONFIG_EXIT,               ///< Exit from the state machine
}e_config_state;

typedef enum
{
    NET_CONFIG = 1,
    CLOUD_CONFIG,
    DUMP_CONFIG,
    MAIN_CONFIG_EXIT,
}e_main_cfg_state;



typedef enum
{
    CLOUD_CONFIG_ENTER,
    CLOUD_CONFIG_EXIT = 1,
}e_cloud_cfg_state;

typedef struct gCloud_cfg_info_t
{
    uint8_t project_id[128];
    uint8_t endp_address[128];
    uint8_t device_id[128];
    uint8_t cloud_region[128];
    uint8_t registry_id[128];
}gCloud_cfg_info;

typedef struct dev_cert_cfg_t
{
    unsigned int rootCA_len;
    unsigned int priKey_len;
}dev_cert_cfg;

typedef struct iot_input_cfg
{
    uint8_t iotserv_valid;
    gCloud_cfg_info gCloud_info;    
}iot_input_cfg_t;

typedef struct netif_static_mode
{
    ULONG address;
    ULONG mask;
    ULONG gw;
    ULONG dns;
}netif_static_mode_t;

typedef struct net_input_cfg
{
    uint8_t netif_valid;
    uint8_t netif_addr_mode;
    uint8_t interface_index;
    netif_static_mode_t netif_static;
 //   sf_wifi_provisioning_t wifi_prov;
}net_input_cfg_t;


void config_menu_callback(sf_console_cb_args_t *p_args);
void demo_service_callback(sf_console_cb_args_t *p_args);

#endif /* CONSOLE_CONFIG_H_ */
