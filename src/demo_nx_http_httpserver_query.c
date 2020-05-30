#include   "common_data.h"
#include   "nx_api.h"
#include   "demo_nx_http_htmlstrings.h"
#include   "nx_http_server.h"
#include   "http_thread.h"
#include    "adc_interface.h"
#include    "stdbool.h"

extern UINT _nx_http_server_number_convert(UINT number, CHAR *string);
/* graphics are located in nx_http_demo_graphics.c */

extern const char nxlogo[];
extern const char txlogo[];
extern UINT nxlogosize, txlogosize;

/* Define the counters used in the demo application...  */

static ULONG   thread_0_counter = 100;
static ULONG   thread_1_counter = 100;
static ULONG   thread_2_counter = 100;
static ULONG   thread_3_counter = 100;
static ULONG   thread_4_counter = 100;
static ULONG   thread_5_counter = 100;
static ULONG   thread_6_counter = 100;
static ULONG   thread_7_counter = 100;
static ULONG   trial_counter	 = 100;

static adc_data_t* adc_data = NULL;
static bool pointer_init = false;

UINT    authentication_check(struct NX_HTTP_SERVER_STRUCT *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm)
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

UINT    request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
CHAR    string[30];
UINT    status=0;
UINT    error=0;
NX_PACKET *resp_packet_ptr;
  
    SSP_PARAMETER_NOT_USED(request_type);
    SSP_PARAMETER_NOT_USED(packet_ptr);

    if(!pointer_init)
    {
        adc_data = get_adc_arr();
        if(adc_data != NULL)
        {
            pointer_init = true;
        }
    }

    /* send the graphics data */
    if(strcmp((const char*)resource,(const char*)"/nxlogo.gif")==0)
    {
        nx_http_server_callback_data_send(server_ptr, (void *)nxlogo, nxlogosize);
        return(NX_HTTP_CALLBACK_COMPLETED);
    }
	
    if(strcmp((const char*)resource,(const char*)"/txlogo.gif")==0)
    {
        nx_http_server_callback_data_send(server_ptr, (void *)txlogo, txlogosize);
        return(NX_HTTP_CALLBACK_COMPLETED);
    }
	
    /* return requested resource or query */
    if(strcmp((const char*)resource,(const char*)"/")==0)
    {
        /* obtain a packet for our html code to be sent to the client */
        status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                     &resp_packet_ptr,
                                     NX_TCP_PACKET,
                                     NX_WAIT_FOREVER);
	
        /* write HTML code into the packet */
        /* htmlwrite(p,s)  (nx_packet_data_append(p,s,strlen(s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER)) */
        status += htmlwrite(resp_packet_ptr, htmlresponse);
        status += htmlwrite(resp_packet_ptr, htmltag);
        status += htmlwrite(resp_packet_ptr, refresh);
        status += htmlwrite(resp_packet_ptr, titleline);
        status += htmlwrite(resp_packet_ptr, bodytag);
        status += htmlwrite(resp_packet_ptr, h1line);
        status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);
	
        /* generate different HTML*/
        status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                     &resp_packet_ptr,
                                     NX_TCP_PACKET,
                                     NX_WAIT_FOREVER);
	
        status += htmlwrite(resp_packet_ptr, hrline);
        status += htmlwrite(resp_packet_ptr, tabletag);
        status += htmlwrite(resp_packet_ptr, captiontag);
        status += htmlwrite(resp_packet_ptr, sysvars_anchor_tag);
        status += htmlwrite(resp_packet_ptr, captionendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 0 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[0].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 1 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[1].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 2 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[2].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 3 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[3].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 4 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[4].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 5 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[5].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 6 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[6].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 7 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(adc_data[7].adc_raw_count, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
        status += htmlwrite(resp_packet_ptr, tableendtag);
	
        status += htmlwrite(resp_packet_ptr, hrline);
        status += htmlwrite(resp_packet_ptr, tabletag);
        status += htmlwrite(resp_packet_ptr, captiontag);
        status += htmlwrite(resp_packet_ptr, sysvars_anchor_tag);
        status += htmlwrite(resp_packet_ptr, captionendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 0 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_0_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 1 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_1_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 2 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_2_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 3 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_3_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 4 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_4_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 5 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_5_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 6 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_6_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Trial Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(trial_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
	
        status += htmlwrite(resp_packet_ptr, trtag);
        status += htmlwrite(resp_packet_ptr, tdtag);
        status += htmlwrite(resp_packet_ptr, "<B>Thread 7 Counter </B> ");
        status += htmlwrite(resp_packet_ptr, tdendtag);
	
        status += htmlwrite(resp_packet_ptr, tdtag);
        _nx_http_server_number_convert(thread_7_counter, string);
        status += htmlwrite(resp_packet_ptr, string);
        status += htmlwrite(resp_packet_ptr, tdendtag);
        status += htmlwrite(resp_packet_ptr, trendtag);
        status += htmlwrite(resp_packet_ptr, tableendtag);
	
        status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);
	
        status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                     &resp_packet_ptr,
                                     NX_TCP_PACKET,
                                     NX_WAIT_FOREVER);
	
        status += htmlwrite(resp_packet_ptr, hrline);
        status += htmlwrite(resp_packet_ptr, bodyendtag );
        status += htmlwrite(resp_packet_ptr, htmlendtag );
	
        status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);
	
        if(status)
          error++;
	
        return(NX_HTTP_CALLBACK_COMPLETED);
    }
    return(NX_SUCCESS);
}

