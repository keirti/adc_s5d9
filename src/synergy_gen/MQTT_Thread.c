/* generated thread source file - do not edit */
#include "MQTT_Thread.h"

TX_THREAD MQTT_Thread;
void MQTT_Thread_create(void);
static void MQTT_Thread_func(ULONG thread_input);
static uint8_t MQTT_Thread_stack[8192] BSP_PLACE_IN_SECTION_V2(".stack.MQTT_Thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
NX_SECURE_TLS_SESSION g_tls_session;

#if defined(__ICCARM__)
#define g_tls_session_err_callback_WEAK_ATTRIBUTE
#pragma weak g_tls_session_err_callback  = g_tls_session_err_callback_internal
#elif defined(__GNUC__)
#define g_tls_session_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_tls_session_err_callback_internal")))
#endif
void g_tls_session_err_callback(void *p_instance, void *p_data)
g_tls_session_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_tls_session_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error 
 * @param[in]  p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_tls_session_err_callback_internal(void *p_instance, void *p_data);
void g_tls_session_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

#ifdef NX_CRYPTO_ENGINE_SW
extern const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers;
#else
extern const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers_synergys7;
#endif

/* See the documentation for nx_secure_tls_metadata_size_calculate to find the required size. */
CHAR g_tls_session_meta_data[16384];
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void tls_dtls_session_init0(NX_SECURE_TLS_SESSION *p_tls_session)
 **********************************************************************************************************************/
void tls_dtls_session_init0(NX_SECURE_TLS_SESSION *p_tls_session)
{
    UINT g_tls_session_err;
    /* Create TLS client. */

    g_tls_session_err = nx_secure_tls_session_create (p_tls_session,
#ifdef NX_CRYPTO_ENGINE_SW
                                                      & nx_crypto_tls_ciphers,
#else
                                                      &nx_crypto_tls_ciphers_synergys7,
#endif
                                                      g_tls_session_meta_data,
                                                      sizeof(g_tls_session_meta_data));

#if NXD_TLS_TIMESTAMP_CALLBACK_USED_g_tls_session
    /* Add a timestamp function for time checking and timestamps in the TLS handshake. */
    /** Declaration of user callback function. This function MUST be defined in the user application.*/
    _nx_secure_tls_session_time_function_set(p_tls_session, NULL);
#endif

#if NXD_TLS_CERT_VERIFICATION_CALLBACK_USED_g_tls_session
    /* Setup the callback invoked when TLS has a certificate it wants to verify so we can
     do additional checks not done automatically by TLS. */
    /** Declaration of user callback function. This function MUST be defined in the user application.*/
    _nx_secure_tls_session_certificate_callback_set(p_tls_session, NULL);
#endif                

    if (NX_SUCCESS != g_tls_session_err)
    {
        g_tls_session_err_callback ((void *) p_tls_session, &g_tls_session_err);
    }
}
NX_SNTP_CLIENT g_sntp_client0;
#if defined(__ICCARM__)
#define g_sntp_client0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_sntp_client0_err_callback  = g_sntp_client0_err_callback_internal
#elif defined(__GNUC__)
#define g_sntp_client0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_sntp_client0_err_callback_internal")))
#endif
void g_sntp_client0_err_callback(void *p_instance, void *p_data)
g_sntp_client0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_sntp_client0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_sntp_client0_err_callback_internal(void *p_instance, void *p_data);
void g_sntp_client0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void sntp_client_init0(void)
 **********************************************************************************************************************/
void sntp_client_init0(void)
{
    UINT g_sntp_client0_err;
    /* Create SNTP Client. */
    g_sntp_client0_err = nx_sntp_client_create (&g_sntp_client0, &g_ip0, 0, &g_packet_pool0, leap_second_handler,
                                                kiss_of_death_handler, NULL);
    if (NX_SUCCESS != g_sntp_client0_err)
    {
        g_sntp_client0_err_callback ((void *) &g_sntp_client0, &g_sntp_client0_err);
    }
}
#if (5) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_int_storageInst) && !defined(SSP_SUPPRESS_ISR_FLASH)
SSP_VECTOR_DEFINE( fcu_frdyi_isr, FCU, FRDYI);
#endif
#endif
#if (5) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_int_storageInst) && !defined(SSP_SUPPRESS_ISR_FLASH)
SSP_VECTOR_DEFINE( fcu_fiferr_isr, FCU, FIFERR);
#endif
#endif
flash_hp_instance_ctrl_t g_int_storageInst_ctrl;
const flash_cfg_t g_int_storageInst_cfg =
{ .data_flash_bgo = true, .p_callback = flash_callback, .p_context = &g_int_storageInst, .irq_ipl = (5), .err_irq_ipl =
          (5), };
/* Instance structure to use this module. */
const flash_instance_t g_int_storageInst =
{ .p_ctrl = &g_int_storageInst_ctrl, .p_cfg = &g_int_storageInst_cfg, .p_api = &g_flash_on_flash_hp };
NX_DHCP g_dhcp_client0;
#if defined(__ICCARM__)
#define g_dhcp_client0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_dhcp_client0_err_callback  = g_dhcp_client0_err_callback_internal
#elif defined(__GNUC__)
#define g_dhcp_client0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_dhcp_client0_err_callback_internal")))
#endif
void g_dhcp_client0_err_callback(void *p_instance, void *p_data)
g_dhcp_client0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_dhcp_client0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_dhcp_client0_err_callback_internal(void *p_instance, void *p_data);
void g_dhcp_client0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void dhcp_client_init0(void)
 **********************************************************************************************************************/
void dhcp_client_init0(void)
{
    UINT g_dhcp_client0_err;
    /* Create DHCP client. */
    g_dhcp_client0_err = nx_dhcp_create (&g_dhcp_client0, &g_ip0, "g_dhcp_client0_DHCPv4");
    if (NX_SUCCESS != g_dhcp_client0_err)
    {
        g_dhcp_client0_err_callback ((void *) &g_dhcp_client0, &g_dhcp_client0_err);
    }

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0
    /* Set callback function to add user options.  */
    g_dhcp_client0_err = nx_dhcp_user_option_add_callback_set(&g_dhcp_client0,
            dhcp_user_option_add_client0);

    if (NX_SUCCESS != g_dhcp_client0_err)
    {
        g_dhcp_client0_err_callback((void *)&g_dhcp_client0,&g_dhcp_client0_err);
    }
#endif
}

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0 && DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0
UCHAR g_dhcp_client0_opt_num = 60;
CHAR *g_dhcp_client0_opt_val = "REA";

/*******************************************************************************************************************//**
 * @brief     This DHCP user options add function adds Vendor Class ID. User can change the option number and/or option
 *            value in the user defined code by simply overriding the values. This function work fine to add any option
 *            which takes string values for option. Like below
 *            g_dhcp_client0_opt_num = 43;
 *            g_dhcp_client0_opt_val = "OPT43VAL";
 *            If user wants to chain the options, should disable this function generation and provide their own definition.
 **********************************************************************************************************************/
UINT dhcp_user_option_add_client0(NX_DHCP *dhcp_ptr, UINT iface_index, UINT message_type, UCHAR *user_option_ptr, UINT *user_option_length)
{
    NX_PARAMETER_NOT_USED(dhcp_ptr);
    NX_PARAMETER_NOT_USED(iface_index);
    NX_PARAMETER_NOT_USED(message_type);

    /* Application can check if add options into the packet by iface_index and message_type.
     message_type are defined in header file, such as: NX_DHCP_TYPE_DHCPDISCOVER.  */
    /* Add Vendor Class ID option refer to RFC2132.  */

    /* Check if have enough space for this option.  */
    if (*user_option_length < (strlen(g_dhcp_client0_opt_val) + 2))
    {
        return(NX_FALSE);
    }

    /* Set the option code.  */
    *user_option_ptr = g_dhcp_client0_opt_num;

    /* Set the option length.  */
    *(user_option_ptr + 1) = (UCHAR)strlen(g_dhcp_client0_opt_val);

    /* Set the option value (Vendor class id).  */
    memcpy((user_option_ptr + 2), g_dhcp_client0_opt_val, strlen(g_dhcp_client0_opt_val));

    /* Update the option length.  */
    *user_option_length = (strlen(g_dhcp_client0_opt_val) + 2);

    return(NX_TRUE);
}
#endif
NX_DNS g_dns0;

#ifdef NX_DNS_CLIENT_USER_CREATE_PACKET_POOL
NX_PACKET_POOL g_dns0_packet_pool;
uint8_t g_dns0_pool_memory[NX_DNS_PACKET_POOL_SIZE];
#endif

#if defined(__ICCARM__)
#define g_dns0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_dns0_err_callback  = g_dns0_err_callback_internal
#elif defined(__GNUC__)
#define g_dns0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_dns0_err_callback_internal")))
#endif
void g_dns0_err_callback(void *p_instance, void *p_data)
g_dns0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_dns0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_dns0_err_callback_internal(void *p_instance, void *p_data);
void g_dns0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void dns_client_init0(void)
 **********************************************************************************************************************/
void dns_client_init0(void)
{
    UINT g_dns0_err;

#ifdef NX_DNS_CLIENT_USER_CREATE_PACKET_POOL
    g_dns0_err = nx_packet_pool_create(&g_dns0_packet_pool, "g_dns0 Packet Pool", NX_DNS_PACKET_PAYLOAD, &g_dns0_pool_memory[0], NX_DNS_PACKET_POOL_SIZE);

    if (NX_SUCCESS != g_dns0_err)
    {
        g_dns0_err_callback((void *)&g_dns0,&g_dns0_err);
    }

    g_dns0.nx_dns_packet_pool_ptr = &g_dns0_packet_pool;
#endif

    /* Create DNS Client. */
    g_dns0_err = nx_dns_create (&g_dns0, &g_ip0, (UCHAR *) "g_dns0 DNS Client");
    if (NX_SUCCESS != g_dns0_err)
    {
        g_dns0_err_callback ((void *) &g_dns0, &g_dns0_err);
    }
}
NXD_MQTT_CLIENT g_mqtt_client0;
#if defined(__ICCARM__)
#define g_mqtt_client0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_mqtt_client0_err_callback  = g_mqtt_client0_err_callback_internal
#elif defined(__GNUC__)
#define g_mqtt_client0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_mqtt_client0_err_callback_internal")))
#endif
void g_mqtt_client0_err_callback(void *p_instance, void *p_data)
g_mqtt_client0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_mqtt_client0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error 
 * @param[in]  p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_mqtt_client0_err_callback_internal(void *p_instance, void *p_data);
void g_mqtt_client0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

static UCHAR g_mqtt_client0_stack[4096] BSP_PLACE_IN_SECTION_V2(".stack.g_mqtt_client0") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
static UCHAR g_mqtt_client0_memory[10 * sizeof(MQTT_MESSAGE_BLOCK)];
char g_mqtt_client0_client_id[12];
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void mqtt_client_init0(void)
 **********************************************************************************************************************/
void mqtt_client_init0(void)
{
    UINT g_mqtt_client0_err;
    uint32_t mqtt_client_id_len = 0;

#if NXD_MQTT_CLIENT_ID_CALLBACK_USED_g_mqtt_client0
    /** Declaration of user callback function. This function MUST be defined in the user application.*/
    mqtt_client_id_callback( g_mqtt_client0_client_id, &mqtt_client_id_len );
#endif                

    /* Create MQTT client. */
    g_mqtt_client0_err = nxd_mqtt_client_create (&g_mqtt_client0, "g_mqtt_client0 MQTT Client",
                                                 g_mqtt_client0_client_id, mqtt_client_id_len, &g_ip0, &g_packet_pool0,
                                                 (VOID *) g_mqtt_client0_stack, 4096, 2,
                                                 (UCHAR *) g_mqtt_client0_memory, sizeof(g_mqtt_client0_memory));

    if (NX_SUCCESS != g_mqtt_client0_err)
    {
        g_mqtt_client0_err_callback ((void *) &g_mqtt_client0, &g_mqtt_client0_err);
    }
}
TX_EVENT_FLAGS_GROUP g_sys_event_flags;
TX_MUTEX g_mqtt_mutex;
TX_EVENT_FLAGS_GROUP g_int_storage_evt_grp;
TX_SEMAPHORE g_google_semaphore;
TX_QUEUE g_msg_queue;
static uint8_t queue_memory_g_msg_queue[4];
TX_QUEUE g_mqtt_rx_payload;
static uint8_t queue_memory_g_mqtt_rx_payload[32];
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void MQTT_Thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_sys_event_flags;
    err_g_sys_event_flags = tx_event_flags_create (&g_sys_event_flags, (CHAR *) "user_event");
    if (TX_SUCCESS != err_g_sys_event_flags)
    {
        tx_startup_err_callback (&g_sys_event_flags, 0);
    }
    UINT err_g_mqtt_mutex;
    err_g_mqtt_mutex = tx_mutex_create (&g_mqtt_mutex, (CHAR *) "MQTT Mutex", TX_NO_INHERIT);
    if (TX_SUCCESS != err_g_mqtt_mutex)
    {
        tx_startup_err_callback (&g_mqtt_mutex, 0);
    }
    UINT err_g_int_storage_evt_grp;
    err_g_int_storage_evt_grp = tx_event_flags_create (&g_int_storage_evt_grp, (CHAR *) "Internal Storage Event Flags");
    if (TX_SUCCESS != err_g_int_storage_evt_grp)
    {
        tx_startup_err_callback (&g_int_storage_evt_grp, 0);
    }
    UINT err_g_google_semaphore;
    err_g_google_semaphore = tx_semaphore_create (&g_google_semaphore, (CHAR *) "New Semaphore", 0);
    if (TX_SUCCESS != err_g_google_semaphore)
    {
        tx_startup_err_callback (&g_google_semaphore, 0);
    }
    UINT err_g_msg_queue;
    err_g_msg_queue = tx_queue_create (&g_msg_queue, (CHAR *) "New Queue", 1, &queue_memory_g_msg_queue,
                                       sizeof(queue_memory_g_msg_queue));
    if (TX_SUCCESS != err_g_msg_queue)
    {
        tx_startup_err_callback (&g_msg_queue, 0);
    }
    UINT err_g_mqtt_rx_payload;
    err_g_mqtt_rx_payload = tx_queue_create (&g_mqtt_rx_payload, (CHAR *) "New Queue", 2,
                                             &queue_memory_g_mqtt_rx_payload, sizeof(queue_memory_g_mqtt_rx_payload));
    if (TX_SUCCESS != err_g_mqtt_rx_payload)
    {
        tx_startup_err_callback (&g_mqtt_rx_payload, 0);
    }

    UINT err;
    err = tx_thread_create (&MQTT_Thread, (CHAR *) "MQTT_Thread", MQTT_Thread_func, (ULONG) NULL, &MQTT_Thread_stack,
                            8192, 5, 5, 10, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&MQTT_Thread, 0);
    }
}

static void MQTT_Thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */
    /** Call initialization function if user has selected to do so. */
#if (0)
    tls_dtls_session_init0(& g_tls_session);
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    sntp_client_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    dhcp_client_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    dns_client_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (0)
    mqtt_client_init0();
#endif

    /* Enter user code for this thread. */
    MQTT_Thread_entry ();
}
