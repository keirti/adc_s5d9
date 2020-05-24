/* generated thread source file - do not edit */
#include "can_thread.h"

TX_THREAD can_thread;
void can_thread_create(void);
static void can_thread_func(ULONG thread_input);
static uint8_t can_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.can_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void can_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&can_thread, (CHAR *) "CAN Thread", can_thread_func, (ULONG) NULL, &can_thread_stack, 1024,
                            4, 4, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&can_thread, 0);
    }
}

static void can_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    can_thread_entry ();
}
