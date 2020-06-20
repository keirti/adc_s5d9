/***********************************************************************************************************************
* File Name    : mqtt_client_api.h
* Description  : This file contains mqtt client config  level data structures.
*              : Also contains the TLS Setup,
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

#ifndef MQTT_CLIENT_API_H_
#define MQTT_CLIENT_API_H_

#include "MQTT_Thread.h"
#include "iot_service_api.h"

/** MQTT QoS definitions */
typedef enum e_mqtt_qos
{
    MQTT_QOS_0,         ///< QoS 0 - Deliver at most once
    MQTT_QOS_1,         ///< QoS 1 - Deliver at least once
    MQTT_QOS_2,         ///< QoS 2 - Deliver exactly once
} mqtt_client_qos_t;

/* Size of buffer for raw certificate data */
#define CERTIFICATE_BUFFER_SIZE     (4096)

/* Number of intermediate CA's. Increase this value if you get certificate validation errors */
#define NUM_INTERMEDIATE_CA         (4)
/** MQTT Stack size */
#define MQTT_CLIENT_STACK_SIZE      4096

/** MQTT Client memory */
#define MQTT_CLIENT_MEMORY_SIZE     1024

/* Topics and messsages */
/** Maximum length of a MQTT topic */
#define MQTT_CLIENT_TOPIC_LEN       128

#define NUM_TOPICS                  6

/** Client message buffer */
#define MQTT_CLIENT_MSG_LEN         1024

/** IoT Service configuration structure  */
typedef struct st_mqtt_client_cfg
{
    char                    *p_endpoint;    ///< Host name of IoT Service endpoint
    char                    *p_thingname;   ///< MQTT Thing Name
    char                    *projectId;     ///< Project Id for Google
    char                    *deviceId;      ///< Device Id for Google
    char                    devname[512];   ///< Device name
    char                    username[128];  ///< Username required for Google
    char                    password[1024]; ///< Password required for Google
    uint8_t                 *p_root_ca;     ///< Root CA cert for this service
    uint16_t                root_ca_len;    ///< Length of Root CA cert
    uint8_t                 *p_device_cert; ///< Device certificate used for this service
    uint16_t                device_cert_len;///< Length of device certificate
    uint8_t                 *p_private_key; ///< Device's private key for this service
    uint16_t                private_key_len;///< Length of private key
    NX_SECURE_TLS_CRYPTO    *p_ciphers;     ///< Crypto ciphers to be used with TLS
    UINT                    thread_priority;///< MQTT thread priority
    ULONG                   server_ip;      ///< Cloud Server IP address
    NXD_MQTT_CLIENT         *p_secure_client;///< MQTT client structure
    void                    *p_iot_service; ///< Pointer to IOT service control
    void                    (*mqtt_init)(void); ///< init function

} mqtt_client_cfg_t;

typedef struct st_mqtt_client_ctrl
{
    UCHAR                   tls_packet_buffer[16384];    ///< TLS packet buffer for reassembly

    /* Define scratch space for cryptographic methods:
     * 2*sizeof(NX_AES) + sizeof(NX_SHA1_HMAC) + 2*sizeof(NX_CRYPTO_RSA) +
     *     (2 * (sizeof(NX_MD5) + sizeof(NX_SHA1) + sizeof(NX_SHA256)))];
     */
    CHAR                    crypto_metadata[16384]; //8850     ///< Crypto metadata

    /* Certificate storage */
    NX_SECURE_X509_CERT     remote_intermediate_ca[NUM_INTERMEDIATE_CA]; ///< Intermediate CA's
    UCHAR                   remote_int_ca_cert_buffer[CERTIFICATE_BUFFER_SIZE * NUM_INTERMEDIATE_CA]; ///< Intermediate CA certificate buffer
    NX_SECURE_X509_CERT     remote_certificate;         ///< Remote server certificate
    UCHAR                   remote_cert_buffer[CERTIFICATE_BUFFER_SIZE]; ///< Remote server certificate buffer
    uint8_t                 *p_root_ca;     ///< Root CA cert for this service
    uint16_t                root_ca_len;    ///< Length of Root CA cert
    uint8_t                 *p_device_cert; ///< Device certificate used for this service
    uint16_t                device_cert_len;///< Length of device certificate
    uint8_t                 *p_private_key; ///< Device's private key for this service
    uint16_t                private_key_len;///< Length of private key

    NXD_MQTT_CLIENT         *p_secure_client; ///< MQTT client structure
    unsigned int            num_messages;   ///< Number of messages received on subscribed topic
    UCHAR                   topic[NUM_TOPICS][MQTT_CLIENT_TOPIC_LEN];
    UCHAR                   tx_msg[MQTT_CLIENT_MSG_LEN];
    UCHAR                   rx_msg[MQTT_CLIENT_MSG_LEN];
    ULONG                   server_ip;      ///< Cloud Server IP address
    void                    *p_iot_service_ctrl;        ///< IOT Service control struct
    void                    (*notify)(void *p_ctrl);    ///< Callback from notify
    void                    (*logMsg)(const char *msg); ///< Message logger
} mqtt_client_ctrl_t;


/** MQTT client will implement this API */
typedef struct st_mqtt_client_api
{
    /** Initialize MQTT connection; connect to service and authenticate.
         * @par Implemented as
         *  - NX_MQTT_Open()
         *
         * @pre None.
         * @param[in]  p_ctrl  Pointer to control handle structure
         * @param[in]  p_cfg   Pointer to configuration structure
         */
    UINT (*open)(mqtt_client_ctrl_t * p_ctrl, mqtt_client_cfg_t const * const p_cfg);

    /** Subscribe to a topic.
         * @par Implemented as
         *  - NX_MQTT_Subscribe()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         * @param[in]  p_name     Pointer to topic name
         */
    UINT (*subscribe)(mqtt_client_ctrl_t * p_ctrl, char const * p_name,
                            mqtt_client_qos_t qos);

    /** Unsubscribe from a topic.
         * @par Implemented as
         *  - NX_MQTT_Unsubscribe()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         * @param[in]  p_name     Pointer to topic name
         */
    UINT (*unsubscribe)(mqtt_client_ctrl_t * p_ctrl, char const * p_name);

    /** Publish data to a topic.
         * @par Implemented as
         *  - NX_MQTT_Publish()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         * @param[in]  p_name     Pointer to topic name
         * @param[in]  p_data     Pointer to data to be published
         */
    UINT (*publish)(mqtt_client_ctrl_t * p_ctrl, char const * p_name, void const * p_data, uint32_t len, mqtt_client_qos_t qos);

    /** Get a message received from a subscribed topic
     * @par Implemented as
     *  - NX_MQTT_Get_Message()
     *
     * @param[in]  p_ctrl         Pointer to control handle structure
     * @param[in]  num_msgs       Number of messages to fetch
     * @return                    SSP_SUCCESS or SSP_ERR_ABORTED
     */
    UINT (*messageGet)(mqtt_client_ctrl_t *p_ctrl);

    /** Close the connection to the IoT service.
         * @par Implemented as
         *  - NX_MQTT_Close()
         *
         * @param[in]  p_ctrl     Pointer to control handle structure
         */
    UINT (*close)(mqtt_client_ctrl_t * p_ctrl);

} mqtt_client_api_t;

/** MQTT client instance structure */
typedef struct st_mqtt_client_instance_t
{
    mqtt_client_cfg_t   *p_cfg;
    mqtt_client_ctrl_t  * const p_ctrl;
    mqtt_client_api_t   * const p_api;
} mqtt_client_instance_t;


#endif /* MQTT_CLIENT_API_H_ */
