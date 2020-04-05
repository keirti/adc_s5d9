#include "adc_thread0.h"
#include "stdio.h"

extern void initialise_monitor_handles(void);
static uint16_t adc_data;
bsp_leds_t led;

/* Adc Thread entry function */

void adc_thread0_entry(void)
{
    /* TODO: add your own code here */
    ssp_err_t status;// = SSP_SUCCESS;

    initialise_monitor_handles();

    R_BSP_LedsGet(&led);
    /*initialize ADC : apply power to adc,pins ,clock as per configuration settings*/

    g_adc0.p_api ->open(g_adc0.p_ctrl,g_adc0.p_cfg);

    g_adc0.p_api->scanCfg(g_adc0.p_ctrl, g_adc0.p_channel_cfg);

    status = g_adc0.p_api->scanStart(g_adc0.p_ctrl);
    if(status == SSP_SUCCESS)
    {
         g_ioport.p_api->pinWrite(led.p_leds[0],0);
    }

/*
    status = g_adc0.p_api->scanStatusGet(g_adc0.p_ctrl);

    printf("scan status %d :\n",status);
    if(status == SSP_SUCCESS)
    g_ioport.p_api->pinWrite(led.p_leds[1],0);
*/

    while (1)
    {
       g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_0, &adc_data);
       printf("read channel 1 : %d\n",adc_data);
       g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_1, &adc_data);
       printf("read channel 2 : %d\n",adc_data);
       g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_2, &adc_data);
       printf("read channel 3 : %d\n",adc_data);
       g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_3, &adc_data);
       printf("read channel 4 : %d\n",adc_data);
       g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_4, &adc_data);
       printf("read channel 5 : %d\n",adc_data);
       g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_5, &adc_data);
       printf("read channel 6 : %d\n",adc_data);
       status = g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_6, &adc_data);
       printf("read channel 7 : %d\n",adc_data);

       if(status == SSP_SUCCESS)
       {
            g_ioport.p_api->pinWrite(led.p_leds[2],0);
       }
        //tx_thread_sleep (1);
    }
    g_adc0.p_api->close(g_adc0.p_ctrl);
}
