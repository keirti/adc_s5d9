/*******************************************************************************
(C) COPYRIGHT Sprint Power Limited - 2020

FILE
    Enter File Name

ORIGINAL AUTHOR
    Enter Name Here

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
#include "stdio.h"
#include "stdbool.h"

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
#include "adc_interface.h"
#include "sf_thread_monitor_api.h"
#include "watchdog.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include <thread0.h>

/*=============================================================================*
 Private Defines
*=============================================================================*/
#define WD_MIN_COUNT 1
#define WD_MAX_COUNT 200

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static sf_thread_monitor_counter_min_max_t min_max_values;
static uint16_t* adc_data;

/*=============================================================================*
 Private Function Definitions (static)
*=============================================================================*/
extern void initialise_monitor_handles(void);
void vTaskMODBUS( void );

/*=============================================================================*
 Private Function Implementations (Static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Function Implementations
*=============================================================================*/
/* Adc Thread entry function */

void thread0_entry(void)
{
    uint8_t num_adcs = NUM_ADC_CHANNELS;
    uint8_t channel = ADC_REG_CHANNEL_0;
    adc_data = get_adc_arr();
    initialise_monitor_handles();

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

    /*
     * Open the ADC Module
     * Configure the scan
     */
    g_adc0.p_api->open(g_adc0.p_ctrl,g_adc0.p_cfg);
    g_adc0.p_api->scanCfg(g_adc0.p_ctrl, g_adc0.p_channel_cfg);

    /*
     * Start the ADC scan, print and error message should this fail
     */
    if(g_adc0.p_api->scanStart(g_adc0.p_ctrl) != SSP_SUCCESS)
    {
         printf("scan start failed\n");
    }

    /*
     * Thread Loop
     */
    while (1)
    {
        /*
         * Run the modbus code
         * Sleep the thread
         */
        //vTaskMODBUS();

        g_adc0.p_api->read(g_adc0.p_ctrl, channel, &adc_data[channel]);

        channel++;
        if(channel >= NUM_ADC_CHANNELS)
        {
            channel = ADC_REG_CHANNEL_0;
        }

        /*
         *  Increment the thread_counter
         */
        if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
        {
            __BKPT(0);
        }
        tx_thread_sleep (5);
    }
    g_adc0.p_api->close(g_adc0.p_ctrl);
}

/*=============================================================================*
End Of File
*=============================================================================*/
