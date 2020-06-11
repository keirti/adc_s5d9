/*******************************************************************************
(C) COPYRIGHT Smart Power Systems Limited - 2020

FILE
    can_thread_entry.c

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    Thread for the CAN stack

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
#include "watchdog.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "can_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
#define WD_MAX_COUNT 200
#define WD_MIN_COUNT 1

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
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
/*-------------------------------------------------------------------*

  NAME
    can_thread_entry

  DESCRIPTION
    Thread run function for the CAN thread.
    Will process all TX and RX messages

  PARAM
    None

  RETURNS
    None

*--------------------------------------------------------------------*/
void can_thread_entry(void)
{
    /*
     * Populate the structure counters for the thread monitor with the defines
     */
    min_max_values.maximum_count = WD_MAX_COUNT;
    min_max_values.minimum_count = WD_MIN_COUNT;

    /*
     * Register the thread with the monitor
     */
    if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->threadRegister (g_sf_thread_monitor0.p_ctrl, &min_max_values))
    {
        __BKPT(0);
    }

    /* TODO: add your own code here */
    while (1)
    {
        /*
         * Increment the monitor counter
         */
        if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
        {
            __BKPT(0);
        }

        /*
         * Sleep the thread
         */
        tx_thread_sleep (1);
    }
}

/*=============================================================================*
End Of File
*=============================================================================*/
