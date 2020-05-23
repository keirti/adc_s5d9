#include <watchdog.h>

/* New Thread entry function */
void watchdog_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (5);
    }
}
