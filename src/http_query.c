/*******************************************************************************
(C) COPYRIGHT Smart Power Systems - 2020

FILE
    http_server_query.c

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    File to TX all the information over the ethernet port

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

/*=============================================================================*
 ANSI C & System-wide Header Files
*=============================================================================*/
#include "stdbool.h"

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
#include "htmlstrings.h"
#include "common_data.h"
#include "nx_api.h"
#include "nx_http_server.h"
#include "adc_interface.h"

/*=============================================================================*
 Local Header File
*=============================================================================*/
#include "http_thread.h"
#include "http_query.h"

/*=============================================================================*
 Private Defines
*=============================================================================*/
extern UINT _nx_http_server_number_convert(UINT number, CHAR *string);

/*=============================================================================*
 Private Variable Definitions (static)
*=============================================================================*/
static adc_data_t* adc_data = NULL;

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
	http_query_init
  
  DESCRIPTION
	Function to init the query, ensuring everything is in the correct
	state at the start of execution
	
  PARAM
	adc_data_t* data - A Pointer to the data
  
  RETURNS
	bool to_return - if the init has been successful or not

*--------------------------------------------------------------------*/
bool http_query_init(adc_data_t* data)
{
	bool to_return = false;
	/*
	 * Copy the PTR passed into the internal variable,
	 * Check the current state, set return flag if the ptr is correctly
	 * initilised
	 */
    adc_data = data;
    if(adc_data != NULL)
    {
        to_return = true;
    }

	/*
	 * Return the return flag
	 */
    return to_return;
}

/*-------------------------------------------------------------------*

  NAME
    authentication_check

  DESCRIPTION
    Check the authentication requirements

  PARAM
    -

  RETURNS
    UINT - Success status

*--------------------------------------------------------------------*/
UINT authentication_check(struct NX_HTTP_SERVER_STRUCT *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm)
{
      SSP_PARAMETER_NOT_USED(resource);
      SSP_PARAMETER_NOT_USED(request_type);
      SSP_PARAMETER_NOT_USED(server_ptr);
#if 0
      *name =  "name5";
      *password = "password6";
      *realm =  "test.htm";
      return(NX_HTTP_BASIC_AUTHENTICATE);
#else
      SSP_PARAMETER_NOT_USED(name);
      SSP_PARAMETER_NOT_USED(password);
      SSP_PARAMETER_NOT_USED(realm);
      return(NX_SUCCESS);
#endif
}

/*-------------------------------------------------------------------*

  NAME
    request_notify

  DESCRIPTION
    Updates the HTML data with the latest information

  PARAM
    -

  RETURNS
    UINT to_return - Success status

*--------------------------------------------------------------------*/
UINT request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
    UINT to_return = 0u;
	CHAR string[30];
	UINT status=0;
	UINT error=0;
	NX_PACKET *resp_packet_ptr;
  
    SSP_PARAMETER_NOT_USED(request_type);
    SSP_PARAMETER_NOT_USED(packet_ptr);

    /* return requested resource or query */
    if(strcmp((const char*)resource,(const char*)"/")==0)
    {
        uint8_t i = 0u;
        /* obtain a packet for our html code to be sent to the client */
        status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);

        /* write HTML code into the packet */
        /* HTMLWRITE(p,s)  (nx_packet_data_append(p,s,strlen(s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER)) */
        status += HTMLWRITE(resp_packet_ptr, HTMLRESPONSE);
        status += HTMLWRITE(resp_packet_ptr, HTMLTAG);
        status += HTMLWRITE(resp_packet_ptr, REFRESH);
        status += HTMLWRITE(resp_packet_ptr, TITLELINE);
        status += HTMLWRITE(resp_packet_ptr, BODYTAG);
        status += HTMLWRITE(resp_packet_ptr, H1LINE);
        status += nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

        /* generate different HTML*/
        status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);

        status += HTMLWRITE(resp_packet_ptr, TABLETAG);
        status += HTMLWRITE(resp_packet_ptr, TRTAG);
        status += HTMLWRITE(resp_packet_ptr, TDTAG);
        status += HTMLWRITE(resp_packet_ptr, "<B>ADC Channel</B>");
        status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
        status += HTMLWRITE(resp_packet_ptr, TDTAG);
        status += HTMLWRITE(resp_packet_ptr, "<B>Raw ADC Count</B>");
        status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
        status += HTMLWRITE(resp_packet_ptr, TDTAG);
        status += HTMLWRITE(resp_packet_ptr, "<B>ADC Voltage (X 100)</B>");
        status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
        status += HTMLWRITE(resp_packet_ptr, TDTAG);
        status += HTMLWRITE(resp_packet_ptr, "<B>Gain (X 100)</B>");
        status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
        status += HTMLWRITE(resp_packet_ptr, TDTAG);
        status += HTMLWRITE(resp_packet_ptr, "<B>Offset (X 100)</B>");
        status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
        status += HTMLWRITE(resp_packet_ptr, TDTAG);
        status += HTMLWRITE(resp_packet_ptr, "<B>Scaled Value (X 100)</B>");
        status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
        status += HTMLWRITE(resp_packet_ptr, TRENDTAG);
		
		/*
		 * Loop for all the ADCs Configured printing each value to each column of the table
		 */
        for(i = 0u; i < NUM_ADC_CHANNELS; i++)
        {
            status += HTMLWRITE(resp_packet_ptr, TRTAG);
            status += HTMLWRITE(resp_packet_ptr, TDTAG);
            _nx_http_server_number_convert(i, string);
            status += HTMLWRITE(resp_packet_ptr,string);
            status += HTMLWRITE(resp_packet_ptr, TDENDTAG);

            status += HTMLWRITE(resp_packet_ptr, TDTAG);
            _nx_http_server_number_convert(adc_data[i].adc_raw_count, string);
            status += HTMLWRITE(resp_packet_ptr, string);
            status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
            status += HTMLWRITE(resp_packet_ptr, TDTAG);
            _nx_http_server_number_convert((UINT)(adc_data[i].adc_voltage * 100.0f), string);
            status += HTMLWRITE(resp_packet_ptr, string);
            status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
            status += HTMLWRITE(resp_packet_ptr, TDTAG);
            _nx_http_server_number_convert((UINT)(adc_data[i].gain * 100.0f), string);
            status += HTMLWRITE(resp_packet_ptr, string);
            status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
            status += HTMLWRITE(resp_packet_ptr, TDTAG);
            _nx_http_server_number_convert((UINT)(adc_data[i].offset * 100.0f), string);
            status += HTMLWRITE(resp_packet_ptr, string);
            status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
            status += HTMLWRITE(resp_packet_ptr, TDTAG);
            _nx_http_server_number_convert((UINT)(adc_data[i].scaled_value * 100.0f), string);
            status += HTMLWRITE(resp_packet_ptr, string);
            status += HTMLWRITE(resp_packet_ptr, TDENDTAG);
            status += HTMLWRITE(resp_packet_ptr, TRENDTAG);
        }

        status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

        status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);

        status += HTMLWRITE(resp_packet_ptr, HRLINE);
        status += HTMLWRITE(resp_packet_ptr, BODYENDTAG );
        status += HTMLWRITE(resp_packet_ptr, HTMLENDTAG );

        status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

        if(status)
		{
		  error++;
		}
        to_return = NX_HTTP_CALLBACK_COMPLETED;
    }
    else
    {
        to_return = NX_SUCCESS;
    }
    return to_return;
}

/*=============================================================================*
End Of File
*=============================================================================*/


