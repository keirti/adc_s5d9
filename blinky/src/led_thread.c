/*******************************************************************************
(C) COPYRIGHT Smart Power Solutions Limited - 2020

FILE
    led_thread.c

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    Enter file description

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

/*=============================================================================*
 ANSI C & System-wide Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "led_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
/* None */

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
TX_THREAD led_thread;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

static uint8_t led_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.led_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);

/*=============================================================================*
 Private Function Definitions (static)
*=============================================================================*/
static void led_thread_func(ULONG thread_input);
static void led_thread_entry(void);

extern void tx_startup_err_callback(void *p_instance, void *p_data);
extern void tx_startup_common_init(void);

/*=============================================================================*
 Private Function Implementations (Static)
*=============================================================================*/
static void led_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED(thread_input);

    /* Initialize common components */
    tx_startup_common_init();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    led_thread_entry();
}

static void led_thread_entry(void)
{
    /* Define the units to be used with the threadx sleep function */
	const uint32_t threadx_tick_rate_Hz = 100;
    /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
    const uint32_t freq_in_hz = 2;
    /* Calculate the delay in terms of the threadx tick rate */
    const uint32_t delay = threadx_tick_rate_Hz/freq_in_hz;
    /* LED type structure */
    bsp_leds_t leds;
    /* LED state variable */
    ioport_level_t level = IOPORT_LEVEL_LOW;

    /* Get LED information for this board */
    R_BSP_LedsGet(&leds);

    /* If this board has no leds then trap here */
    if (0 == leds.led_count)
    {
        while(1);   // There are no leds on this board
    }

    while (1)
    {
        /* Determine the next state of the LEDs */
        if(IOPORT_LEVEL_LOW == level)
        {
            level = IOPORT_LEVEL_HIGH;
        }
        else
        {
            level = IOPORT_LEVEL_LOW;
        }

        /* Update all board LEDs */
        g_ioport.p_api->pinWrite(leds.p_leds[1], level);

        /* Delay */
        tx_thread_sleep (delay);
    }
}
/*=============================================================================*
 Public Function Implementations
*=============================================================================*/
void led_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&led_thread, (CHAR *) "LED Thread", led_thread_func, (ULONG) NULL,
                            &led_thread_stack, 1024, 1, 1, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&led_thread, 0);
    }
}

/*=============================================================================*
End Of File
*=============================================================================*/
