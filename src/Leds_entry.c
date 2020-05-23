#include "Leds.h"
#include "sf_thread_monitor_api.h"
#include "watchdog.h"

#define WD_MAX_COUNT 2000
#define WD_MIN_COUNT 1

static const uint32_t threadx_tick_rate_Hz = 100; /* Define the units to be used with the threadx sleep function */
static const uint32_t freq_in_hz = 5; /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
static uint32_t delay = 0u; /* Calculate the delay in terms of the threadx tick rate */
static bsp_leds_t leds; /* LED type structure */
static ioport_level_t level = IOPORT_LEVEL_LOW; /* LED state variable */
static sf_thread_monitor_counter_min_max_t min_max_values;

/* Led Thread entry function */
void Leds_entry(void)
{
    R_DBG->DBGSTOPCR_b.DSWDT = 0u;

    min_max_values.maximum_count = WD_MAX_COUNT;
    min_max_values.minimum_count = WD_MIN_COUNT;

    ssp_err_t err = g_sf_thread_monitor0.p_api->threadRegister (g_sf_thread_monitor0.p_ctrl, &min_max_values);
    if(SSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    /*
     * Init the local thread variables
     */
    delay = threadx_tick_rate_Hz/freq_in_hz;
    R_BSP_LedsGet(&leds);
    /* TODO: add your own code here */
    while (1)
    {
        level = !level;
        g_ioport.p_api->pinWrite(leds.p_leds[0], !level);
        g_ioport.p_api->pinWrite(leds.p_leds[1], level);
        g_ioport.p_api->pinWrite(leds.p_leds[2], !level);

        if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
        {
            __BKPT(0);
        }

        tx_thread_sleep(delay);
    }
}
