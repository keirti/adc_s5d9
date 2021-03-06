/*******************************************************************************
(C) COPYRIGHT Sprint Power Limited - 2020

FILE
    adc_thread_entry.c

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    adc functionality file, will allow for reading and processing all adc values
    found within the adc_interface data files.

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
#include "adc_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
#define WD_MIN_COUNT 1
#define WD_MAX_COUNT 200

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static sf_thread_monitor_counter_min_max_t min_max_values;
static adc_data_t* adc_data;

/*=============================================================================*
 Private Function Definitions (static)
*=============================================================================*/
extern void initialise_monitor_handles(void);

/*-------------------------------------------------------------------*

  NAME
    calculate_adc_voltages

  DESCRIPTION
    Converts the raw adc counts into the voltage

  PARAM
    uint8_t channel - The channel to convert

  RETURNS
    None

*--------------------------------------------------------------------*/
static void calculate_adc_voltages(uint8_t channel);

/*-------------------------------------------------------------------*

  NAME
    scale_adc

  DESCRIPTION
    Scales each ADC with the gain and offset configured in the adc_data array

  PARAM
    uint8_t channel - The channel to convert

  RETURNS
    None

*--------------------------------------------------------------------*/
static void scale_adc(uint8_t channel);

/*=============================================================================*
 Private Function Implementations (Static)
*=============================================================================*/
/*-------------------------------------------------------------------*

  NAME
    calculate_adc_voltages

  DESCRIPTION
    Converts the raw adc counts into the voltage

  PARAM
    uint8_t channel - The channel to convert

  RETURNS
    None

*--------------------------------------------------------------------*/
static void calculate_adc_voltages(uint8_t channel)
{
    /*
     * Convert the ADC counts in voltages and store within the adc_data array
     */
    adc_data[channel].adc_voltage = ((adc_data[channel].adc_raw_count/MAX_ADC_COUNT)*ADC_VREF);
}

/*-------------------------------------------------------------------*

  NAME
    scale_adc

  DESCRIPTION
    Scales each ADC with the gain and offset configured in the adc_data array

  PARAM
    uint8_t channel - The channel to convert

  RETURNS
    None

*--------------------------------------------------------------------*/
static void scale_adc(uint8_t channel)
{
    /*
     * Scale the adcs by using the gain and offset seen in the adc_data array
     */
    adc_data[channel].scaled_value = ((adc_data[channel].adc_voltage - adc_data[channel].offset)* adc_data[channel].gain);
}

/*=============================================================================*
 Public Function Implementations
*=============================================================================*/
/*-------------------------------------------------------------------*

  NAME
    adc_thread_entry

  DESCRIPTION
    Thread function, processing all the ADC values and reading the latest

  PARAM
    None

  RETURNS
    None

*--------------------------------------------------------------------*/
void adc_thread_entry(void)
{
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
         * Updating the the adc channel array
         * Increment the channel
         * Check the channel is within the range of the configured amount of adcs
         */
        g_adc0.p_api->read(g_adc0.p_ctrl, channel, &adc_data[channel].adc_raw_count);
        calculate_adc_voltages(channel);
        scale_adc(channel);
        channel++;
        if(channel >= NUM_ADC_CHANNELS)
        {
            channel = ADC_REG_CHANNEL_0;
        }

        /*
         *  Increment the thread_counter
         *  Sleep the thread
         */
        if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
        {
            __BKPT(0);
        }
        tx_thread_sleep (1);
    }

    /*
     * Only close the api when we break out of the while loop..
     * which is never
     */
    g_adc0.p_api->close(g_adc0.p_ctrl);
}

/*=============================================================================*
End Of File
*=============================================================================*/
