/*******************************************************************************
(C) COPYRIGHT Smart Power Systems - 2020

FILE
    htmlstrings.h

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    Header file to house all the #defines used when sending html text

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

#ifndef _HTML_STRINGS_H_
#define _HTML_STRINGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================*
 ANSI C & System-wide Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
/* None */

/*=============================================================================*
 Pragmas
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
*=============================================================================*/
#define HTMLWRITE(p,s)  (nx_packet_data_append(p,s,(ULONG)strlen((const char*)s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER))
#define HTMLRESPONSE "HTTP/1.0 200 \r\nContent-Type: text/html\r\n\r\n"
#define HTMLTAG "<HTML>"
#define HTMLENDTAG "</HTML>"
#define BODYTAG "<body bgcolor=\"#FFFF00\" >"
#define BODYENDTAG "</body>"
#define HRLINE "<HR SIZE=6 WIDTH=\"90%\" NOSHADE COLOR=\"black\"  >"
#define TITLELINE "<HEAD><TITLE>SPS_EMS ADC Information</TITLE></HEAD>\r\n"
#define H1LINE " <H1 ALIGN=\"center\">ADC Information</H1>\r\n"
#define TABLETAG "<TABLE BORDER=\"1\" ALIGN=\"center\" >"
#define TABLEENDTAG "</TABLE>"
#define CAPTIONTAG "<CAPTION ALIGN=\"top\" >"
#define CAPTIONENDTAG "</CAPTION>"
#define TRTAG "<TR>"
#define TRENDTAG "</TR>"
#define TDTAG "<TD>"
#define TDENDTAG "</TD>"
#define ANCHORTAG "<A href=\"/test.htm?Display="
#define GTCHAR "\">"
#define ANCHORENDTAG "</A>"
#define REFRESH "<H1 ALIGN=\"center\">"   "<meta http-equiv=\"refresh\" content=\"2\">"   "</H1>\r\n"

/*=============================================================================*
 Public Data
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Function Definitions
*=============================================================================*/
/* None */

/*=============================================================================*
End Of File
*=============================================================================*/
#endif  /* _HTML_STRINGS_H_ */

