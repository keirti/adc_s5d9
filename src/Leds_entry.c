/*******************************************************************************
(C) COPYRIGHT Smart Power Solutions - 2020

FILE
    led_entry.c

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
#include "sf_thread_monitor_api.h"
#include "watchdog.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "Leds.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
#define WD_MAX_COUNT 2000
#define WD_MIN_COUNT 1

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static const uint32_t threadx_tick_rate_Hz = 100; /* Define the units to be used with the threadx sleep function */
static const uint32_t freq_in_hz = 5; /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
static uint32_t delay = 0u; /* Calculate the delay in terms of the threadx tick rate */
static bsp_leds_t leds; /* LED type structure */
static ioport_level_t level = IOPORT_LEVEL_LOW; /* LED state variable */
static sf_thread_monitor_counter_min_max_t min_max_values;

/*=============================================================================*
 Private Function Definitions (static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Private Function Implementations (Static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Function Implementations
*=============================================================================*/
/* Led Thread entry function */
void Leds_entry(void)
{
    /*
     * Populate the structure counters for the thread monitor with the defines
     */
    min_max_values.maximum_count = WD_MAX_COUNT;
    min_max_values.minimum_count = WD_MIN_COUNT;

    /*
     * Register the thread with the monitor
     */
    ssp_err_t err = g_sf_thread_monitor0.p_api->threadRegister (g_sf_thread_monitor0.p_ctrl, &min_max_values);
    if(SSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    /*
     * Init the local thread variables
     */
    delay = threadx_tick_rate_Hz/freq_in_hz;
    R_BSP_LedsGet(&leds);

    /*
     * Thread Loop
     */
    while (1)
    {
        /*
         * Flip the output of the pins
         */
        level = !level;
        g_ioport.p_api->pinWrite(leds.p_leds[0], !level);
        g_ioport.p_api->pinWrite(leds.p_leds[1], level);
        g_ioport.p_api->pinWrite(leds.p_leds[2], !level);

        /*
         * Increment the monitor counter
         */
        if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
        {
            __BKPT(0);
        }

        /*
         * Delay the thread by the desired amount
         */
        tx_thread_sleep(delay);
    }
}
/*=============================================================================*
End Of File
*=============================================================================*/
