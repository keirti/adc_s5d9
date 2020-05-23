#include "modbus_thread.h"

void vTaskMODBUS( void );

/* Modbus Thread entry function */
void modbus_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        vTaskMODBUS();
        tx_thread_sleep (1);
    }
}
