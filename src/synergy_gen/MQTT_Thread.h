/* generated thread header file - do not edit */
#ifndef MQTT_THREAD_H_
#define MQTT_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
extern "C" void MQTT_Thread_entry(void);
#else
extern void MQTT_Thread_entry(void);
#endif
#include "nx_secure_tls_api.h"
#include "nxd_sntp_client.h"
#include "r_flash_hp.h"
#include "r_flash_api.h"
#include "nxd_dhcp_client.h"
#include "nxd_dns.h"
#include "nxd_mqtt_client.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern NX_SECURE_TLS_SESSION g_tls_session;
void g_tls_session_err_callback(void *p_instance, void *p_data);
void tls_dtls_session_init0(NX_SECURE_TLS_SESSION *p_tls_session);

/** TLS TIMESTAMP FUNCTION Callback */
#ifdef NULL
#define NXD_TLS_TIMESTAMP_CALLBACK_USED_g_tls_session (0)
#else
#define NXD_TLS_TIMESTAMP_CALLBACK_USED_g_tls_session (1)
#endif
#if NXD_TLS_TIMESTAMP_CALLBACK_USED_g_tls_session
/* Add a timestamp function for time checking and timestamps in the TLS handshake. */
/** Declaration of user callback function. This function MUST be defined in the user application.*/
extern ULONG NULL(void);
#endif

/** TLS TIMESTAMP FUNCTION Callback */
#ifdef NULL
#define NXD_TLS_CERT_VERIFICATION_CALLBACK_USED_g_tls_session (0)
#else
#define NXD_TLS_CERT_VERIFICATION_CALLBACK_USED_g_tls_session (1)
#endif
#if NXD_TLS_CERT_VERIFICATION_CALLBACK_USED_g_tls_session
/* Setup the callback invoked when TLS has a certificate it wants to verify so we can
 do additional checks not done automatically by TLS. */
/** Declaration of user callback function. This function MUST be defined in the user application.*/
extern ULONG NULL(NX_SECURE_TLS_SESSION *session, NX_SECURE_X509_CERT *certificate);
#endif
extern NX_SNTP_CLIENT g_sntp_client0;
#if !defined(leap_second_handler)
UINT leap_second_handler(NX_SNTP_CLIENT *client_ptr, UINT indicator);
#endif
#if !defined(kiss_of_death_handler)
UINT kiss_of_death_handler(NX_SNTP_CLIENT *client_ptr, UINT code);
#endif
#if !defined(NULL)
VOID NULL(struct NX_SNTP_CLIENT_STRUCT *client_ptr, ULONG *rand);
#endif
void g_sntp_client0_err_callback(void *p_instance, void *p_data);
void sntp_client_init0(void);
/* Flash on Flash HP Instance */
extern const flash_instance_t g_int_storageInst;
#ifndef flash_callback
void flash_callback(flash_callback_args_t *p_args);
#endif
extern NX_DHCP g_dhcp_client0;
void g_dhcp_client0_err_callback(void *p_instance, void *p_data);
void dhcp_client_init0(void);

#define DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0 (0)
#define DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0 (1)

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0
UINT dhcp_user_option_add_client0(NX_DHCP *dhcp_ptr, UINT iface_index, UINT message_type, UCHAR *user_option_ptr, UINT *user_option_length);
#if DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0
extern UCHAR g_dhcp_client0_opt_num;
extern CHAR *g_dhcp_client0_opt_val;
#endif
#endif
extern NX_DNS g_dns0;
void g_dns0_err_callback(void *p_instance, void *p_data);
void dns_client_init0(void);
extern NXD_MQTT_CLIENT g_mqtt_client0;

/** MQTT CLIENT ID Callback */
#ifdef mqtt_client_id_callback
#define NXD_MQTT_CLIENT_ID_CALLBACK_USED_g_mqtt_client0 (0)
#else
#define NXD_MQTT_CLIENT_ID_CALLBACK_USED_g_mqtt_client0 (1)
#endif
#if NXD_MQTT_CLIENT_ID_CALLBACK_USED_g_mqtt_client0
/** Declaration of user callback function. This function MUST be defined in the user application.*/
extern void mqtt_client_id_callback(char *client_id, uint32_t *client_id_length);
#endif

void g_mqtt_client0_err_callback(void *p_instance, void *p_data);
void mqtt_client_init0(void);
extern TX_EVENT_FLAGS_GROUP g_sys_event_flags;
extern TX_MUTEX g_mqtt_mutex;
extern TX_EVENT_FLAGS_GROUP g_int_storage_evt_grp;
extern TX_SEMAPHORE g_google_semaphore;
extern TX_QUEUE g_msg_queue;
extern TX_QUEUE g_mqtt_rx_payload;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* MQTT_THREAD_H_ */
