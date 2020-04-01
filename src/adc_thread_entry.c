#include <stdio.h>
#include "adc_thread.h"
#include "sf_adc_periodic_api.h"

void adc_thread_entry(void);

#define EVENT_FLAG_1    0x00000001  // b 0000-0000-0000-0000-0000-0000-0000-0001
#define EVENT_FLAG_2    0x00000002  // b 0000-0000-0000-0000-0000-0000-0000-0010

#define ALL_FLAGS       (EVENT_FLAG_1 | EVENT_FLAG_2)
uint32_t actual_flags;
bsp_leds_t g_leds;
uint32_t result;
/* ADC framework example thread entry function */
void adc_thread_entry(void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* leds on board*/
    R_BSP_LedsGet(&g_leds);

    /* Initialise the ADC Framework if disable in configuration*/
    err = (g_sf_adc_periodic0.p_api->open(g_sf_adc_periodic0.p_ctrl, g_sf_adc_periodic0.p_cfg));
    if(err != SSP_SUCCESS)
    {
        __BKPT(1);
    }

    /* Start the ADC scan */
   err = (g_sf_adc_periodic0.p_api->start(g_sf_adc_periodic0.p_ctrl));

   if(err != SSP_SUCCESS)
   {
       // Add Error correction mechanism
       __BKPT(1);
   }

   while (1)
   {
           tx_event_flags_get (&g_adc_flags, ALL_FLAGS, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

           if( EVENT_FLAG_1 == (actual_flags & EVENT_FLAG_1))
           {
            // process ADC data accordingly
             for(uint32_t count=0; count<g_sf_adc_periodic0.p_cfg->sample_count; count++)
             {
              result = g_user_buffer[count];
              g_ioport.p_api->pinWrite(g_leds.p_leds[0], IOPORT_LEVEL_LOW);
              g_ioport.p_api->pinWrite(g_leds.p_leds[1], IOPORT_LEVEL_HIGH);
             }
           }
           else if (EVENT_FLAG_2 == (actual_flags & EVENT_FLAG_2))
           {
            for(uint32_t count=0; count<g_sf_adc_periodic0.p_cfg->sample_count; count++)
            {
             result = g_user_buffer[count];
            }
               // process ADC data accordingly

               g_ioport.p_api->pinWrite(g_leds.p_leds[0], IOPORT_LEVEL_HIGH);
               g_ioport.p_api->pinWrite(g_leds.p_leds[1], IOPORT_LEVEL_LOW);

           }
           else
           {
           }
      }
}


void g_adc_framework_user_callback(sf_adc_periodic_callback_args_t * p_args)
{
    if(p_args->event == SF_ADC_PERIODIC_EVENT_NEW_DATA)
    {
        if(p_args->buffer_index == 0)
        {
            /* Set the event flag */
            tx_event_flags_set(&g_adc_flags, EVENT_FLAG_1, TX_OR);
        }
        else
        {
            /* Set the event flag */
            tx_event_flags_set(&g_adc_flags, EVENT_FLAG_2, TX_OR);
        }
    }
}



