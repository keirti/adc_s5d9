/*******************************************************************************
(C) COPYRIGHT Smart Power Solutions Limited - 2020

FILE
    adc_thread_entry

ORIGINAL AUTHOR
    Smart Power Solutions Limited

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
#include <stdio.h>

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
#include "sf_adc_periodic_api.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "adc_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
#define EVENT_FLAG_1    0x00000001  // b 0000-0000-0000-0000-0000-0000-0000-0001
#define EVENT_FLAG_2    0x00000002  // b 0000-0000-0000-0000-0000-0000-0000-0010
#define ALL_FLAGS       (EVENT_FLAG_1 | EVENT_FLAG_2)

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static uint32_t actual_flags;
static bsp_leds_t g_leds;
static uint32_t result;

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
	g_adc_framework_user_callback
  
  DESCRIPTION
	Callback function for the adc thread
	
  PARAM
	sf_adc_periodic_callback_args_t * p_args - Pointer to the arguments
  
  RETURNS
	None

*--------------------------------------------------------------------*/
void g_adc_framework_user_callback(sf_adc_periodic_callback_args_t * p_args)
{
	/*
	 * If the event is new ADC data
	 */
    if(p_args->event == SF_ADC_PERIODIC_EVENT_NEW_DATA)
    {
		/*
		 * If the index is for event 1 then sent event 1
		 * otherwise set flag for event two
		 */
        if(p_args->buffer_index == 0)
        {
            /*
			 * Set the event flag
			 */
            tx_event_flags_set(&g_adc_flags, EVENT_FLAG_1, TX_OR);
        }
        else
        {
            /*
			 * Set the event flag
			 */
            tx_event_flags_set(&g_adc_flags, EVENT_FLAG_2, TX_OR);
        }
    }
}

/*-------------------------------------------------------------------* 

  NAME
	adc_thread_entry
  
  DESCRIPTION
	Thread entry point for the ADC Processing thread.
	Gets the LEDs and toggles them
	Starts the ADC scan
	
  PARAM
	None
  
  RETURNS
	None

*--------------------------------------------------------------------*/
void adc_thread_entry(void)
{
	/*
	 * Init the Error Flag
	 */
    ssp_err_t err = SSP_SUCCESS;

    /*
	 * Connect to the LEDs on the board
	 */
    R_BSP_LedsGet(&g_leds);

    /*
	 * Initialise the ADC Framework if disable in configuration
	 * Check the error flag
	 */
    err = (g_sf_adc_periodic0.p_api->open(g_sf_adc_periodic0.p_ctrl, g_sf_adc_periodic0.p_cfg));
    if(err != SSP_SUCCESS)
    {
		__BKPT(1);
    }

    /*
	 * Start the ADC scan
	 */
	err = (g_sf_adc_periodic0.p_api->start(g_sf_adc_periodic0.p_ctrl));
	if(err != SSP_SUCCESS)
	{
		/*
		 * Add Error correction mechanism
		 */
		__BKPT(1);
	}
	
	/*
	 * Main thread loop
	 */
	while (1)
	{
		uint8_t count = 0u;
        tx_event_flags_get (&g_adc_flags, ALL_FLAGS, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        if( EVENT_FLAG_1 == (actual_flags & EVENT_FLAG_1))
        {
				/*
				 * process ADC data accordingly
				 */
				for(count=0; count<g_sf_adc_periodic0.p_cfg->sample_count; count++)
				{
					result = g_user_buffer[count];
				}

		        g_ioport.p_api->pinWrite(g_leds.p_leds[0], IOPORT_LEVEL_LOW);
		        g_ioport.p_api->pinWrite(g_leds.p_leds[1], IOPORT_LEVEL_HIGH);
        }
        else if (EVENT_FLAG_2 == (actual_flags & EVENT_FLAG_2))
        {
			/*
			 * Loop storing the number of samples
			 */
			for(count=0; count<g_sf_adc_periodic0.p_cfg->sample_count; count++)
			{
				result = g_user_buffer[count];
			}
			/*
			 * process ADC data accordingly
			 */
				
            g_ioport.p_api->pinWrite(g_leds.p_leds[0], IOPORT_LEVEL_HIGH);
            g_ioport.p_api->pinWrite(g_leds.p_leds[1], IOPORT_LEVEL_LOW);

        }
        else
        {
        }
    }
}

/*=============================================================================*
End Of File
*=============================================================================*/
