#include <watchdog.h>

/* New Thread entry function */
void watchdog_entry(void)
{
    /*
     * Enable the WD to work in Debug Mode
     */
    R_DBG->DBGSTOPCR_b.DSWDT = 0u;

    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (5);
    }
}
