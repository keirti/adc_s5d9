#include "Leds.h"

static const uint32_t threadx_tick_rate_Hz = 100; /* Define the units to be used with the threadx sleep function */
static const uint32_t freq_in_hz = 5; /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
static uint32_t delay = 0u; /* Calculate the delay in terms of the threadx tick rate */
static bsp_leds_t leds; /* LED type structure */
static ioport_level_t level = IOPORT_LEVEL_LOW; /* LED state variable */

/* Led Thread entry function */
void Leds_entry(void)
{
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
        tx_thread_sleep(delay);
    }
}
