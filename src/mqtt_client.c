/***********************************************************************************************************************
* File Name    : mqtt_client.c
* Description  : This file contains Application level Interface to MQTT API's such as Open, Subscribe, Publish etc.
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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "tx_api.h"
#include "nx_api.h"
#include "nx_secure_tls_api.h"
#include "nx_crypto_aes_sce.h"
#include "nx_crypto_sha2_sce.h"
#include "nx_crypto_sha1_sce.h"
#include "nx_crypto_rsa_sce.h"
#include "nx_crypto_hmac_sha1_sce.h"
#include "nx_crypto_hmac_sha2_sce.h"
#include "nxd_mqtt_client.h"
#include "MQTT_Config.h"
#include "iot_service_api.h"
#include "mqtt_client_api.h"
#include "mqtt_rx_thread.h"
#include "sntp_client.h"
#include "common_util.h"

#define MQTT_TIMEOUT          (30 * TX_TIMER_TICKS_PER_SECOND)

extern mqtt_client_instance_t g_mqtt_client;

static UINT NX_MQTT_Open(mqtt_client_ctrl_t * p_ctrl, mqtt_client_cfg_t const * const p_cfg);
static UINT NX_MQTT_Subscribe(mqtt_client_ctrl_t * p_ctrl, char const * p_name,
                                    mqtt_client_qos_t qos);
static UINT NX_MQTT_Unsubscribe(mqtt_client_ctrl_t * p_ctrl, char const * p_name);
static UINT NX_MQTT_Publish(mqtt_client_ctrl_t * p_ctrl, char const * p_name,
                                    void const * p_data, uint32_t len, mqtt_client_qos_t qos);
static UINT NX_MQTT_Get_Message(mqtt_client_ctrl_t * p_ctrl);
static UINT NX_MQTT_Close(mqtt_client_ctrl_t * p_ctrl);

const mqtt_client_api_t nx_mqtt_api =
{
    .open           = NX_MQTT_Open,
    .subscribe      = NX_MQTT_Subscribe,
    .unsubscribe    = NX_MQTT_Unsubscribe,
    .publish        = NX_MQTT_Publish,
    .messageGet     = NX_MQTT_Get_Message,
    .close          = NX_MQTT_Close
};

/* Callback invoked by NX MQTT */
void mqtt_client_id_callback(char * client_id, uint32_t * client_id_length)
{
    memset(client_id, 0, 256);
    strncpy(client_id, g_mqtt_client.p_cfg->devname, 256);
    *client_id_length = strlen(client_id);
}

static void NX_MQTT_log_msg(mqtt_client_ctrl_t const * p_ctrl, const char * fmt, ...)
{    
    va_list ap;
    char buf[128];

    memset(&buf, 0, sizeof(buf));
    memset(&ap, 0, sizeof(ap));

    if (p_ctrl->logMsg)
    {
        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        p_ctrl->logMsg(buf);
    }
}

static void NX_MQTT_Disconnect_Callback(NXD_MQTT_CLIENT *p_client)
{
    SSP_PARAMETER_NOT_USED (p_client);
    tx_event_flags_set(&g_sys_event_flags, SYS_EV_MQTT_DISCONN, TX_OR);
}

static UINT NX_MQTT_tls_setup(NXD_MQTT_CLIENT *p_client, NX_SECURE_TLS_SESSION *p_tls_session,
        NX_SECURE_X509_CERT *p_cert, NX_SECURE_X509_CERT *p_trusted_cert)
{
    mqtt_client_ctrl_t *p_ctrl = NULL;
    UINT status = NX_SUCCESS;
    UINT index = 0;

    SSP_PARAMETER_NOT_USED (p_client);

    p_ctrl = g_mqtt_client.p_ctrl;

    /* Need to allocate space for the certificate coming in from the remote host. */
     memset(p_ctrl->remote_cert_buffer, 0, sizeof(p_ctrl->remote_cert_buffer));
     memset(&p_ctrl->remote_certificate, 0, sizeof(p_ctrl->remote_certificate));

     status = nx_secure_tls_remote_certificate_allocate(p_tls_session, &p_ctrl->remote_certificate,
                                                        p_ctrl->remote_cert_buffer, sizeof(p_ctrl->remote_cert_buffer));
     if (NX_SUCCESS != status) 
	 {
         NX_MQTT_log_msg(p_ctrl, "Unable to allocate memory for remote certificate, ret = 0x%x\r\n",status);
         return status;
     }

     memset(p_ctrl->remote_int_ca_cert_buffer, 0, sizeof(p_ctrl->remote_int_ca_cert_buffer));
     memset(p_ctrl->remote_intermediate_ca, 0, sizeof(p_ctrl->remote_intermediate_ca));
     for (index = 0; index < ARRAY_SIZE(p_ctrl->remote_intermediate_ca); index++) 
	 {
         status = nx_secure_tls_remote_certificate_allocate(p_tls_session, &(p_ctrl->remote_intermediate_ca[index]),
                                                            &(p_ctrl->remote_int_ca_cert_buffer[index * CERTIFICATE_BUFFER_SIZE]),
                                                            CERTIFICATE_BUFFER_SIZE);
         if (NX_SUCCESS != status) 
		 {
             NX_MQTT_log_msg(p_ctrl, "Unable to allocate memory for interemediate CA certificate, ret = 0x%x\r\n",status);
             return status;
         }
     }

     /* Add a CA Certificate to our trusted store for verifying incoming server certificates. */
     status = nx_secure_x509_certificate_initialize(p_trusted_cert,
                                                    (UCHAR *)p_ctrl->p_root_ca, p_ctrl->root_ca_len,
                                                    NX_NULL, 0, NULL, 0,
                                                    NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER);
     if (NX_SUCCESS != status) 
	 {
         NX_MQTT_log_msg(p_ctrl, "Unable to initialize CA certificate, ret = 0x%x\r\n",status);
         return status;
     }

     status = nx_secure_tls_trusted_certificate_add(p_tls_session, p_trusted_cert);
     if (NX_SUCCESS != status) 
	 {
         NX_MQTT_log_msg(p_ctrl, "Unable to add CA certificate to trusted store, ret=0x%x\r\n", status);
         return status;
     }

     if (p_ctrl->p_device_cert != NULL && p_ctrl->device_cert_len != 0)
     {
         /* Add the local certificate for client authentication. */
         status = nx_secure_x509_certificate_initialize(p_cert, (UCHAR *)p_ctrl->p_device_cert, p_ctrl->device_cert_len,
                 NX_NULL, 0, p_ctrl->p_private_key, p_ctrl->private_key_len,
                 NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER);
         if (NX_SUCCESS != status) 
		 {
             NX_MQTT_log_msg(p_ctrl, "Unable to initialize device certificate, ret=0x%x\r\n",status);
             return status;
         }
         status = nx_secure_tls_local_certificate_add(p_tls_session, p_cert);
         if (NX_SUCCESS != status) 
		 {
             NX_MQTT_log_msg(p_ctrl, "Unable to add device certificate to trusted store, ret=0x%x\r\n",status);
             return status;
         }
     }

     /* Add a timestamp function for time checking and timestamps in the TLS handshake. */
     nx_secure_tls_session_time_function_set(p_tls_session, get_time);

     return status;
}


static UINT NX_MQTT_Open(mqtt_client_ctrl_t * p_ctrl, mqtt_client_cfg_t const * const p_cfg)
{
    UINT status = NX_SUCCESS;
    UINT internal_status = 0;
    NXD_ADDRESS server_ip;

    memset(&server_ip, 0, sizeof(server_ip));
        
    p_ctrl->p_root_ca       = p_cfg->p_root_ca;
    p_ctrl->root_ca_len     = p_cfg->root_ca_len;
    p_ctrl->p_device_cert   = p_cfg->p_device_cert;
    p_ctrl->device_cert_len = p_cfg->device_cert_len;
    p_ctrl->p_private_key   = p_cfg->p_private_key;
    p_ctrl->private_key_len = p_cfg->private_key_len;
    p_ctrl->p_secure_client = p_cfg->p_secure_client;

    /** Create MQTT Client Instance */
    p_cfg->mqtt_init();

    status = nx_secure_tls_session_create(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session),
                                          p_cfg->p_ciphers,p_ctrl->crypto_metadata,
                                          sizeof(p_ctrl->crypto_metadata));

    if (status)
    {
        NX_MQTT_log_msg(p_ctrl, "Could not create TLS Session (0x%02x)\r\n", status);
        nxd_mqtt_client_delete(p_ctrl->p_secure_client);
        return status;
    }

    /* Allocate space for packet reassembly. */
    status = nx_secure_tls_session_packet_buffer_set(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session),
                                                     p_ctrl->tls_packet_buffer,
                                                     sizeof(p_ctrl->tls_packet_buffer));
    if(status) 
	{
        NX_MQTT_log_msg(p_ctrl, "Could not set TLS session packet buffer (0x%02x)\r\n", status);
        internal_status = nx_secure_tls_session_delete(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session));
        if(NX_SUCCESS != internal_status)
        {
            NX_MQTT_log_msg(p_ctrl, "Could not delete the TLS session (0x%02x)\r\n", internal_status);
        }

        internal_status = nxd_mqtt_client_delete(p_ctrl->p_secure_client);
        if(NX_SUCCESS != internal_status)
        {
            NX_MQTT_log_msg(p_ctrl, "Could not delete the MQTT Client (0x%02x)\r\n", internal_status);
        }
        return status;
    }

    if((strlen(p_cfg->username) != 0))
    {
        status = nxd_mqtt_client_login_set(p_ctrl->p_secure_client, (char *)p_cfg->username, strlen(p_cfg->username),
                                           (char *)p_cfg->password, strlen((char *)p_cfg->password));
        if(status)
        {
            NX_MQTT_log_msg(p_ctrl, "Could not create Login Set (0x%02x)\r\n", status);
            internal_status = nx_secure_tls_session_delete(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session));
            if(NX_SUCCESS != internal_status)
            {
                NX_MQTT_log_msg(p_ctrl, "Could not delete the TLS session (0x%02x)\r\n", internal_status);
            }

            internal_status = nxd_mqtt_client_delete(p_ctrl->p_secure_client);
            if(NX_SUCCESS != internal_status)
            {
                NX_MQTT_log_msg(p_ctrl, "Could not delete the MQTT Client (0x%02x)\r\n", internal_status);
            }
            return status;
        }
    }

    server_ip.nxd_ip_version = 4;
    server_ip.nxd_ip_address.v4 = p_ctrl->server_ip;

    status = nxd_mqtt_client_secure_connect(p_ctrl->p_secure_client, &server_ip, NXD_MQTT_TLS_PORT,
                                                NX_MQTT_tls_setup, 600, 1, MQTT_TIMEOUT);
    if (status != NX_SUCCESS)
    {
        NX_MQTT_log_msg(p_ctrl, "Could not connect to MQTT endpoint (0x%02x)\r\n", status);
        internal_status = nx_secure_tls_session_delete(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session));
        if(NX_SUCCESS != internal_status)
        {
            NX_MQTT_log_msg(p_ctrl, "Could not delete the TLS session (0x%02x)\r\n", internal_status);
        }

        internal_status = nxd_mqtt_client_delete(p_ctrl->p_secure_client);
        if(NX_SUCCESS != internal_status)
        {
            NX_MQTT_log_msg(p_ctrl, "Could not delete the MQTT Client (0x%02x)\r\n", internal_status);
        }

        return status;
    }

    status = nxd_mqtt_client_disconnect_notify_set(p_ctrl->p_secure_client, NX_MQTT_Disconnect_Callback);
    if(status != NX_SUCCESS)
    {
        NX_MQTT_log_msg(p_ctrl, "Could not register MQTT disconnect callback (0x%02x)\r\n", status);
        nx_secure_tls_session_delete(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session));
        nxd_mqtt_client_delete(p_ctrl->p_secure_client);
        return status;
    }

    return status;
}

/* Publish a MQTT message: Trivial wrapper around nxd_mqtt_client_publish() */
static UINT NX_MQTT_Publish(mqtt_client_ctrl_t * p_ctrl, char const * p_name, void const * p_data, uint32_t len, mqtt_client_qos_t qos)
{
    UINT status = NX_SUCCESS;

    status = nxd_mqtt_client_publish(p_ctrl->p_secure_client, (CHAR *)p_name, strlen(p_name),
                                                (CHAR *)p_data, len, 0, qos, MQTT_TIMEOUT);
    if(status) 
	{
        NX_MQTT_log_msg(p_ctrl, "Could not publish to MQTT topic %s (0x%02x)\r\n", p_name, status);
        return status;
    }

    return status;
}

/* Unsubscribe from a MQTT topic: Trivial wrapper around nxd_mqtt_client_unsubscribe() */
static UINT NX_MQTT_Unsubscribe(mqtt_client_ctrl_t * p_ctrl, char const * p_name)
{
    UINT status = NX_SUCCESS;

    status = nxd_mqtt_client_unsubscribe(p_ctrl->p_secure_client, (CHAR *)p_name, strlen(p_name));
    if(status) 
	{
        NX_MQTT_log_msg(p_ctrl, "Could not unsubscribe from MQTT topic %s (0x%02x)\r\n", p_name, status);
        return status;
    }

    return status;
}

/* Callback function invoked when a message is received by the client.
 * We don't want this function to run the callback function of the entity using
 * this client instance since we don't know if that function may sleep/block.
 * Therefore, this function will simply wake up the MQTT notifier thread.
 */
static void NX_MQTT_notify_callback(NXD_MQTT_CLIENT * p_client, UINT number_of_messages)
{
    mqtt_client_ctrl_t *p_ctrl = NULL;

    SSP_PARAMETER_NOT_USED (p_client);

    p_ctrl = g_mqtt_client.p_ctrl;
    p_ctrl->num_messages = number_of_messages;
    tx_semaphore_put(&g_mqtt_notify_sem);
}



/* Subscribe to a MQTT topic. Call nxd_mqtt_client_subscribe() and setup the notify function */
static UINT NX_MQTT_Subscribe(mqtt_client_ctrl_t * p_ctrl, char const * p_name,
                                    mqtt_client_qos_t qos)
{
    UINT status = NX_SUCCESS;
    UINT internal_status = 0;

    status = nxd_mqtt_client_subscribe(p_ctrl->p_secure_client, (CHAR *)p_name, strlen(p_name), qos);
    if(status)
    {
        NX_MQTT_log_msg(p_ctrl, "Could not subscribe to MQTT topic %s (0x%02x)\r\n", p_name, status);
        return status;
    }

    status = nxd_mqtt_client_receive_notify_set(p_ctrl->p_secure_client, NX_MQTT_notify_callback);
    if(status) 
	{
        internal_status = nxd_mqtt_client_unsubscribe(p_ctrl->p_secure_client, (CHAR *)p_name, strlen(p_name));
        if(NX_SUCCESS != internal_status)
        {
            NX_MQTT_log_msg(p_ctrl, "MQTT Client Unsubscribe failed (0x%02x)\r\n", internal_status);
        }

        NX_MQTT_log_msg(p_ctrl, "Could not set notify function (0x%02x)\r\n", status);
        return status;
    }

    return status;
}

static UINT NX_MQTT_Close(mqtt_client_ctrl_t * p_ctrl)
{
    UINT status = NX_SUCCESS;

    /* Disconnect from the MQTT server */
    status = nxd_mqtt_client_disconnect(p_ctrl->p_secure_client);
    if (status) 
	{
        NX_MQTT_log_msg(p_ctrl, "Could not disconnect MQTT client (0x%02x)\r\n", status);
        return status;
    }

    /* Delete the MQTT client. See the following comment. */
    status = nxd_mqtt_client_delete(p_ctrl->p_secure_client);
    if (status) 
	{
        NX_MQTT_log_msg(p_ctrl, "Could not delete MQTT client (0x%02x)\r\n", status);
        return status;
    }

    /* Free up certificate buffers for next connection */
    status = nx_secure_tls_remote_certificate_free_all(&(p_ctrl->p_secure_client->nxd_mqtt_tls_session));
    if(status) 
	{
        NX_MQTT_log_msg(p_ctrl, "nx_secure_tls_remote_certificate_free_all failed!!!, ret = (0x%02x)\r\n", status);
        return status;
    }

    return status;
}

/* Get the message received from the server */
static UINT NX_MQTT_Get_Message(mqtt_client_ctrl_t * p_ctrl)
{
    UINT status = NX_SUCCESS, tlen = 0, msg_len = 0;

    if (p_ctrl == NULL)
        return NX_INVALID_PARAMETERS;

    status = nxd_mqtt_client_message_get(p_ctrl->p_secure_client, (uint8_t*)p_ctrl->topic,
            sizeof(p_ctrl->topic), &tlen, p_ctrl->rx_msg, sizeof(p_ctrl->rx_msg),
            &msg_len);
    p_ctrl->rx_msg[msg_len] = '\0';

    return status;
}
