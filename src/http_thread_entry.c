/*******************************************************************************
(C) COPYRIGHT Smarrt Power Systems - 2020

FILE
    http_thread_entry

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    HTTP server thread to TX information over the ethernet protocol

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

/*=============================================================================*
 ANSI C & System-wide Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "http_thread.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
/* None */

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static UINT err_counter=0;

/*=============================================================================*
 Private Function Definitions (static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Private Function Implementations (Static)
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Function Implementations
*=============================================================================*/
/*-------------------------------------------------------------------*

  NAME
    http_thread_entry

  DESCRIPTION
    Main thread function for the http server,
    ensures its initilised correctly and then loops running the server

  PARAM
    None

  RETURNS
    None

*--------------------------------------------------------------------*/
void http_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        /*
         * Start the server,
         * Sleep the Thread,
         * Stop the Server
         */
        if(nx_http_server_start(&g_http_server0))
        {
             err_counter++;
        }

        tx_thread_sleep(100u);

        if(nx_http_server_stop(&g_http_server0))
        {
             err_counter++;
        }
    }
}

/*=============================================================================*
End Of File
*=============================================================================*/
