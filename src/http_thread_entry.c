/*******************************************************************************
(C) COPYRIGHT Smarrt Power Systems - 2020

FILE
    http_thread_entry

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    HTTP server thread to TX information over the ethernet protocol

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
#include "http_query.h"
#include "adc_interface.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "http_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
#define WD_MAX_COUNT 200
#define WD_MIN_COUNT 1

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static UINT err_counter=0;
static sf_thread_monitor_counter_min_max_t min_max_values;
static adc_data_t* adc_data = NULL;

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
    http_thread_entry

  DESCRIPTION
    Main thread function for the http server,
    ensures its initilised correctly and then loops running the server

  PARAM
    None

  RETURNS
    None

*--------------------------------------------------------------------*/
void http_thread_entry(void)
{
    /*
     * Populate the structure counters for the thread monitor with the defines
     */
    min_max_values.maximum_count = WD_MAX_COUNT;
    min_max_values.minimum_count = WD_MIN_COUNT;

    /*
     * Init the ADC pointer and pass it to the query
     */
    adc_data = get_adc_arr();
    if(adc_data != NULL)
    {
        if(http_query_init(adc_data))
        {
            /*
             * Register the thread with the monitor
             */
            if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->threadRegister (g_sf_thread_monitor0.p_ctrl, &min_max_values))
            {
                __BKPT(0);
            }

            while (1)
                {

                    /*
                     * Start the server,
                     * Sleep the Thread,
                     */
                    if(nx_http_server_start(&g_http_server0))
                    {
                         err_counter++;
                    }

                    /*
                     * Increment the monitor counter
                     */
                    if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
                    {
                        __BKPT(0);
                    }

                    tx_thread_sleep(100u);
                }

                /*
                 * Never stop the server
                 */
                if(nx_http_server_stop(&g_http_server0))
                {
                     err_counter++;
                }
        }
    }
}

/*=============================================================================*
End Of File
*=============================================================================*/
