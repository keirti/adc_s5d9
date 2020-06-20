/* generated thread source file - do not edit */
#include "MQTT_Rx_Thread.h"

TX_THREAD MQTT_Rx_Thread;
void MQTT_Rx_Thread_create(void);
static void MQTT_Rx_Thread_func(ULONG thread_input);
static uint8_t MQTT_Rx_Thread_stack[1280] BSP_PLACE_IN_SECTION_V2(".stack.MQTT_Rx_Thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_SEMAPHORE g_mqtt_notify_sem;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void MQTT_Rx_Thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_mqtt_notify_sem;
    err_g_mqtt_notify_sem = tx_semaphore_create (&g_mqtt_notify_sem, (CHAR *) "New Semaphore", 0);
    if (TX_SUCCESS != err_g_mqtt_notify_sem)
    {
        tx_startup_err_callback (&g_mqtt_notify_sem, 0);
    }

    UINT err;
    err = tx_thread_create (&MQTT_Rx_Thread, (CHAR *) "MQTT RX Thread", MQTT_Rx_Thread_func, (ULONG) NULL,
                            &MQTT_Rx_Thread_stack, 1280, 6, 6, 10, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&MQTT_Rx_Thread, 0);
    }
}

static void MQTT_Rx_Thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    MQTT_Rx_Thread_entry ();
}
