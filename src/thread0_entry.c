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
    ssp_err_t status;// = SSP_SUCCESS;

    initialise_monitor_handles();

    g_adc0.p_api->open(g_adc0.p_ctrl,g_adc0.p_cfg);

    g_adc0.p_api->scanCfg(g_adc0.p_ctrl, g_adc0.p_channel_cfg);

    status = g_adc0.p_api->scanStart(g_adc0.p_ctrl);
    if(status != SSP_SUCCESS)
    {
         printf("scan start failed\n");
    }


    while (1)
    {
        vTaskMODBUS();
       tx_thread_sleep (5);
    }
    g_adc0.p_api->close(g_adc0.p_ctrl);
}
