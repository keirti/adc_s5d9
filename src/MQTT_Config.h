/***********************************************************************************************************************
* File Name    : MQTT_Config.h
* Description  : This file contains mqtt specific Data structures and Defines.
* ***********************************************************************************************************************/

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

#ifndef MQTT_CONFIG_H_
#define MQTT_CONFIG_H_

#include "MQTT_Thread.h"
#include "console_thread.h"
#include "common_util.h"

#define MQTT_AP_MAJOR_VER           (1)
#define MQTT_AP_MINOR_VER           (3)
#define MQTT_AP_PATCH_VER           (0)

#define MQTT_UPDATE_DELAY           (500)
#define IOT_NW_RETRY_DELAY          (500)

#define SYS_EV_MQTT_UPDATE          (0x1)
#define SYS_EV_RX_UPDATE            (0x2)
#define SYS_EV_TIME_UPDATE          (0x4)
#define SYS_EV_DEMO_START           (0x8)
#define SYS_EV_DEMO_STOP            (0x10)
#define SYS_EV_MQTT_DISCONN         (0x20)
#define SYS_EV_IOT_NW_RETRY         (0x40)
#define SYS_EV_ALL                  (0xFF)

#define IOT_SERVICE_RETRY_CNT       (5)
#define IOT_SERVICE_RETRY_DELAY     (100)
#define NETWORK_RETRY_CNT           (10)

#define PROPERTY_COUNT              (18)
#define MAX_PROPERTY_LEN            (32)
#define PROP_STR_XACC               "xacc"
#define PROP_STR_YACC               "yacc"
#define PROP_STR_ZACC               "zacc"
#define PROP_STR_TIME               "time"
#define PROP_STR_HUMIDITY           "humidity"
#define PROP_STR_PRESSURE           "pressure"
#define PROP_STR_TEMPERATURE        "temperature"
#define PROP_STR_LED_RED            "Red_LED"
#define PROP_STR_LED_GREEN          "Green_LED"
#define PROP_STR_LED_YELLOW         "Yellow_LED"
#define LED_STATE                   (1)

#define WIFI_PROV_RETRY_COUNT   (5)
#define DHCP_CLIENT_TIMEOUT     (60 * TX_TIMER_TICKS_PER_SECOND)
#define PPP_LINK_TIMEOUT        (60 * TX_TIMER_TICKS_PER_SECOND)
#define ETH_LINK_TIMEOUT        (60 * TX_TIMER_TICKS_PER_SECOND)
#define PPP_LINK_UP             (1<<0)
#define PPP_LINK_DOWN           (1<<1)

enum e_led_type
{
    LED_RED = 1,
    LED_YELLOW,
    LED_GREEN,
};

enum e_led_state
{
    LED_OFF,
    LED_ON,
};

typedef struct mqtt_rx_payload
{
    uint8_t  type;
    uint8_t  value;
}mqtt_rx_payload_t;


void print_to_console(const char* msg);


#endif /* MQTT_CONFIG_H_ */
