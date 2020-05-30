#include <http_thread.h>

UINT err_counter=0;;
/* HTTP Server Thread entry function */
void http_thread_entry(void)
{
UINT status;
ULONG flags;

    /* TODO: add your own code here */
    while (1)
    {
        if(status)
        {
            err_counter++;
        }

        status = nx_http_server_start(&g_http_server0);
        if(status)
        {
             err_counter++;
        }

        tx_thread_sleep(100u);

        status = nx_http_server_stop(&g_http_server0);
        if(status)
        {
             err_counter++;
        }
    }
}
