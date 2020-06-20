
/***********************************************************************************************************************
* File Name    : iot_service_api.h
* Description  : This file contains AWS IOT service specific data structures.
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

#ifndef IOT_SERVICE_API_H_
#define IOT_SERVICE_API_H_

#include "mqtt_client_api.h"

/** Prints a string to the console.
     * @pre None
     * @param[in] msg NULL terminated character array
     * @return None
 */
void print_to_console(const char *msg);

#define IOTS_DEBUG true

#define DEBUG_MSG(x)                do {                            \
                                        if (p_ctrl->debug)          \
                                            print_to_console(x);    \
                                    } while (0)







/** Protocol definitions */
typedef enum e_iot_service_protocol
{
    IOT_SERVICE_PROTO_MQTT,         ///< MQTT protocol
} iot_service_protocol_t;

/** IoT Service configuration structure  */
typedef struct st_iot_service_cfg
{
    const char              *p_name;
    iot_service_protocol_t  protocol;
    mqtt_client_instance_t  *p_mqtt_client;
} iot_service_cfg_t;

/* IoT Service control structure */
typedef struct st_iot_service_ctrl_t
{
    TX_SEMAPHORE            prop_sem;
    TX_MUTEX                prop_mutex;
    int                     debug;
    mqtt_client_instance_t  *p_mqtt_client;
}iot_service_ctrl_t;

/** IoT Services will implement this API */
typedef struct st_iot_service_api
{
    /** Initialize IoT Service connection; connect to service, authenticate and subscribe to topics of interest.
         * @par Implemented as
         *  - AWS_Open()
         *
         * @pre None.
         * @param[in]  p_ctrl  Pointer to control handle structure
         * @param[in]  p_cfg   Pointer to configuration structure
         */
    UINT (*open)(iot_service_ctrl_t * p_ctrl, iot_service_cfg_t const * const p_cfg);

    /** Set a property for the device in the cloud device representation (shadow/twin/instance).
         * @par Implemented as
         *  - AWS_SetProperty()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         * @param[in]  p_name     Array of property names
         * @param[in]  p_value    Array of property values
         * @param[in]  num        Number of properties
         */
    UINT (*propertySet)(iot_service_ctrl_t const * p_ctrl, char const * p_name[], char const *p_value[], uint32_t num);

    /** Get a property for the device in the cloud device representation (shadow/twin/instance).
         * @par Implemented as
         *  - AWS_GetProperty()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         * @param[in]  p_name     Array of property names
         * @param[in]  p_value    Array of property values
         * @param[in]  valLen     Size of property value element in the p_value array
         * @param[in]  num        Number of properties in array
         * @param[out] num        Number of properties retrieved
         */
    UINT (*propertyGet)(iot_service_ctrl_t const * p_ctrl);

    /** Push sensor data to AWS IoT Core.
             * @par Implemented as
             *  - AWS_Sensordata()
             *
             * @param[in]  p_ctrl     Pointer to control handle structure
             * @param[in]  p_name     Array of property names
             * @param[in]  p_value    Array of property values
             * @param[in]  num        Number of properties
             */
    UINT (*pushSensordata)(iot_service_ctrl_t const * p_ctrl, char const * p_name[], char const *p_value[], uint32_t num);

    /** Close the connection to the IoT service.
         * @par Implemented as
         *  - AWS_Close()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         */
    UINT (*close)(iot_service_ctrl_t const * p_ctrl);
} iot_service_api_t;


/** IoT Service instance structure */
typedef struct st_iot_service_instance
{
    iot_service_cfg_t   * p_cfg;
    iot_service_ctrl_t  * p_ctrl;
    const iot_service_api_t   * p_api;
} iot_service_instance_t;

#endif /* IOT_SERVICE_API_H_ */
