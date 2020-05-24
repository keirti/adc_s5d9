#include <adc_thread.h>
#include "sf_thread_monitor_api.h"
#include "watchdog.h"

static sf_thread_monitor_counter_min_max_t min_max_values;

void vTaskMODBUS( void );

#define WD_MIN_COUNT 1
#define WD_MAX_COUNT 200

/* Modbus Thread entry function */
void adc_thread_entry(void)
{

    /*
     * Populate the structure counters for the thread monitor with the defines
     */
    min_max_values.maximum_count = WD_MAX_COUNT;
    min_max_values.minimum_count = WD_MIN_COUNT;

    /*
     * Register the thread with the monitor
     */
    //if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->threadRegister(g_sf_thread_monitor0.p_ctrl, &min_max_values))
    //{
       // __BKPT(0);
    //}

    /*
     * Thread Loop
     */
    while (1)
    {
        /*
         * Call the modbus function
         */
        //vTaskMODBUS();
    }
}
