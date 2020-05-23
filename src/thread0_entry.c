#include <thread0.h>
#include "stdio.h"
#include "stdbool.h"

extern void initialise_monitor_handles(void);
void vTaskMODBUS( void );
uint16_t adc_data;
bsp_leds_t led;

/* Adc Thread entry function */

void thread0_entry(void)
{
    static bool led_on = false;
    /* TODO: add your own code here */
    ssp_err_t status;// = SSP_SUCCESS;

    initialise_monitor_handles();

    R_BSP_LedsGet(&led);
    /*initialize ADC : apply power to adc,pins ,clock as per configuration settings*/

    g_adc0.p_api ->open(g_adc0.p_ctrl,g_adc0.p_cfg);

    g_adc0.p_api->scanCfg(g_adc0.p_ctrl, g_adc0.p_channel_cfg);

    status = g_adc0.p_api->scanStart(g_adc0.p_ctrl);
    if(status != SSP_SUCCESS)
    {
         printf("scan start failed\n");
    }


    while (1)
    {

        vTaskMODBUS();
        g_ioport.p_api->pinWrite(led.p_leds[1], led_on);
        led_on = !led_on;
       tx_thread_sleep (5);
    }
    g_adc0.p_api->close(g_adc0.p_ctrl);
}
