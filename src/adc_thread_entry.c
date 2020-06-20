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

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "adc_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
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
     * Open the ADC Module
     * Configure the scan
     */
    g_adc.p_api->open(g_adc.p_ctrl,g_adc.p_cfg);
    g_adc.p_api->scanCfg(g_adc.p_ctrl, g_adc.p_channel_cfg);

    /*
     * Start the ADC scan, print and error message should this fail
     */
    if(g_adc.p_api->scanStart(g_adc.p_ctrl) != SSP_SUCCESS)
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
        g_adc.p_api->read(g_adc.p_ctrl, channel, &adc_data[channel].adc_raw_count);
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
        tx_thread_sleep (1);
    }

    /*
     * Only close the api when we break out of the while loop..
     * which is never
     */
    g_adc.p_api->close(g_adc.p_ctrl);
}

/*=============================================================================*
End Of File
*=============================================================================*/
