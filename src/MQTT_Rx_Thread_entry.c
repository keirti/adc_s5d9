/***********************************************************************************************************************
* File Name    : mqtt_rx_thread_entry.c
* Description  : This file mqtt receive and parsing functionality.
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

#include <mqtt_rx_thread.h>
#include "MQTT_Thread.h"
#include "iot_service_api.h"
#include "mqtt_client_api.h"
#include "MQTT_Config.h"


/*********************************************************************************************************************
 * @brief  parse_message function
 *
 * This function parse the incoming MQTT message
 *********************************************************************************************************************/
static UINT parse_message(mqtt_client_ctrl_t *p_ctrl)
{
    if(p_ctrl == NULL)
    {
        print_to_console("\r\n NULL pointer in parse_message \r\n");
        return 1;
    }

    UINT status = 0;
    mqtt_rx_payload_t data;
    bsp_leds_t leds;
    const char msg_red_led_on[]  = "{\"state\":{\"desired\":{\"Red_LED\":\"ON\"}}}";
    const char msg_red_led_off[] = "{\"state\":{\"desired\":{\"Red_LED\":\"OFF\"}}}";
    const char msg_yellow_led_on[]  = "{\"state\":{\"desired\":{\"Yellow_LED\":\"ON\"}}}";
    const char msg_yellow_led_off[] = "{\"state\":{\"desired\":{\"Yellow_LED\":\"OFF\"}}}";
    const char msg_green_led_on[]  = "{\"state\":{\"desired\":{\"Green_LED\":\"ON\"}}}";
    const char msg_green_led_off[] = "{\"state\":{\"desired\":{\"Green_LED\":\"OFF\"}}}";

    memset(&data, 0, sizeof(data));
    R_BSP_LedsGet(&leds);

    /* Red LED */
    if(strcmp((char*)p_ctrl->rx_msg, msg_red_led_on) == 0)
    {
        print_to_console("\r\n Red LED ON\r\n");
        if(leds.led_count)
            led_on_off(leds.p_leds[2], "ON");

        /* send message to MQTT Thread to report the LED Value */
        data.type = LED_RED;
        data.value = LED_ON;
        tx_queue_send (&g_mqtt_rx_payload, &data, TX_WAIT_FOREVER);
    }
    else if(strcmp((char*)p_ctrl->rx_msg, msg_red_led_off) == 0)
    {
        print_to_console("\r\n Red LED OFF \r\n");
        if(leds.led_count)
            led_on_off(leds.p_leds[2], "OFF");

        /* send message to MQTT Thread to report the LED Value */
        data.type = LED_RED;
        data.value = LED_OFF;
        tx_queue_send(&g_mqtt_rx_payload, &data, TX_WAIT_FOREVER);
    }

    /* Yellow LED */
    if(strcmp((char*)p_ctrl->rx_msg, msg_yellow_led_on) == 0)
    {
        print_to_console("\r\n Yellow LED ON\r\n");
        if(leds.led_count)
            led_on_off(leds.p_leds[1], "ON");

        /* send message to MQTT Thread to report the LED Value */
        data.type = LED_YELLOW;
        data.value = LED_ON;
        tx_queue_send(&g_mqtt_rx_payload, &data, TX_WAIT_FOREVER);
    }
    else if(strcmp((char*)p_ctrl->rx_msg, msg_yellow_led_off) == 0)
    {
        print_to_console("\r\n Yellow LED OFF \r\n");
        if(leds.led_count)
            led_on_off(leds.p_leds[1], "OFF");

        /* send message to MQTT Thread to report the LED Value */
        data.type = LED_YELLOW;
        data.value = LED_OFF;
        tx_queue_send(&g_mqtt_rx_payload, &data, TX_WAIT_FOREVER);
    }

    /* Green LED */
    if(strcmp((char*)p_ctrl->rx_msg, msg_green_led_on) == 0)
    {
        print_to_console("\r\n Green LED ON\r\n");
        if(leds.led_count)
            led_on_off(leds.p_leds[0], "ON");


        /* send message to MQTT Thread to report the LED Value */
        data.type = LED_GREEN;
        data.value = LED_ON;
        tx_queue_send(&g_mqtt_rx_payload, &data, TX_WAIT_FOREVER);
    }
    else if(strcmp((char*)p_ctrl->rx_msg, msg_green_led_off) == 0)
    {
        print_to_console("\r\n Green LED OFF \r\n");
        if(leds.led_count)
            led_on_off(leds.p_leds[0], "OFF");

        /* send message to MQTT Thread to report the LED Value */
        data.type = LED_GREEN;
        data.value = LED_OFF;
        tx_queue_send(&g_mqtt_rx_payload, &data, TX_WAIT_FOREVER);
    }

/* Uncomment to see the message that did not match our topic */
#if 0
        print_to_console("\r\n##############\r\n");
        print_to_console((char *)p_ctrl->topic);
        print_to_console((char *)p_ctrl->msg);
        print_to_console("\r\n##############\r\n");
        status = 1;
#endif
    return status;
}

/* MQTT Rx Thread entry function */
void MQTT_Rx_Thread_entry(void)
{
    mqtt_client_ctrl_t *p_ctrl = NULL;
    mqtt_client_instance_t *p_mqtt_client = NULL;
    iot_service_instance_t *p_iots = NULL;
    iot_service_ctrl_t *p_iot_ctrl = NULL;
    UINT num_msg = 0;

    while (true)
    {

        /* Get the IoT cloud provider via the message queue */
        tx_queue_receive(&g_msg_queue, &p_iots, TX_WAIT_FOREVER);

        /* Get the selected IoT service and MQTT client */
        p_mqtt_client = p_iots->p_ctrl->p_mqtt_client;
        p_ctrl = p_mqtt_client->p_ctrl;
        p_iot_ctrl = p_iots->p_ctrl;

        do {
            /* This thread will be woken up by the MQTT notify function when
             * a message is received on the subscribed topic.
             */
            tx_semaphore_get(&g_mqtt_notify_sem, TX_WAIT_FOREVER);

            num_msg = p_ctrl->num_messages;

            while (num_msg > 0)
            {
                p_iots->p_api->propertyGet(p_iot_ctrl);

                if(parse_message(p_ctrl) == 0)
                    tx_event_flags_set(&g_sys_event_flags, SYS_EV_RX_UPDATE, TX_OR);

                num_msg--;
            }

        }while(true);
    }
}
