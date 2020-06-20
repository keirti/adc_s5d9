/***********************************************************************************************************************
* File Name    : sntp_client.c
* Description  : This file contains SNTP functionality. Maintain system time using SNTP protocol.
*              : Provide a routine to generate seconds since Unix Epoch (Jan 1 1970 00:00:00).
*              : This is useful for generating security tokens with an expiration time.
***********************************************************************************************************************/

/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#include "MQTT_Config.h"
#include "nxd_sntp_client.h"
#include "nx_api.h"
#include "sntp_client.h"

extern NX_SNTP_CLIENT *g_active_sntp;
extern NX_DNS *g_active_dns;
static ULONG sntp_boot_time = 0;

/* Leap seconds handler as required by the NetX Duo SNTP client. We
 * don't do anything here.
 */
UINT leap_second_handler(NX_SNTP_CLIENT *client_ptr, UINT leap_indicator)
{
    SSP_PARAMETER_NOT_USED (client_ptr);
    SSP_PARAMETER_NOT_USED (leap_indicator);
    return NX_SUCCESS;
}

/* Kiss of Death handler as required by NetX Duo SNTP client. We don't
 * do anything here.
 */
UINT kiss_of_death_handler(NX_SNTP_CLIENT *client_ptr, UINT KOD_code)
{
    SSP_PARAMETER_NOT_USED (client_ptr);
    SSP_PARAMETER_NOT_USED (KOD_code);
    return NX_SUCCESS;
}

/* Start SNTP client and update local clock */
UINT sntp_setup_and_run(void)
{
    UINT  status  = NX_SUCCESS;
    ULONG dest    = 0;
    char str[64] = {0};

    /* Lookup server name using DNS */
    status = nx_dns_host_by_name_get(g_active_dns, (UCHAR *)SNTP_SERVER, &dest, 1000);
    if (status != NX_SUCCESS)
    {
        snprintf(str,sizeof(str),"Unable to resolve, ret = 0x%x\r\n",status);
        print_to_console(str);
        print_to_console(SNTP_SERVER);
        print_to_console("\r\n");

        dest = IP_ADDRESS(8, 8, 8, 8);
    }

    /* Initialize Unicast SNTP client */
    status = nx_sntp_client_initialize_unicast(g_active_sntp, dest);
    if (status != NX_SUCCESS)
    {
        snprintf(str,sizeof(str),"Unable to initialize unicast SNTP client, ret = 0x%x\r\n",status);
        print_to_console(str);
        print_to_console("Failed to start SNTP service\r\n");
        return status;
    }

    /* Set local time to 0 */
    status = nx_sntp_client_set_local_time(g_active_sntp, 0, 0);
    if (status != NX_SUCCESS)
    {
        snprintf(str,sizeof(str),"Unable to set local time for SNTP client, ret = 0x%x\r\n",status);
        print_to_console(str);
        print_to_console("Failed to start SNTP service\r\n");
        return status;
    }

    /* Run Unicast client */
    status = nx_sntp_client_run_unicast(g_active_sntp);
    if (status != NX_SUCCESS)
    {
        snprintf(str,sizeof(str),"Unable to start unicast SNTP client, ret = 0x%x\r\n",status);
        print_to_console(str);
        return status;
    }

    /* Wait till updates are received */
    for (dest = 0; dest < 10; dest++)
    {
        if ( nx_sntp_client_receiving_updates(g_active_sntp, &status) != NX_SUCCESS)
        {
            print_to_console("Unable to get SNTP status\r\n");
            return status;
        }

        if ( NX_TRUE == status)
        {
            ULONG seconds, milliseconds;
            ULONG ticks;

            ticks = tx_time_get();
            ticks = ticks / TX_TIMER_TICKS_PER_SECOND;

            status = nx_sntp_client_get_local_time(g_active_sntp, &seconds, &milliseconds, NX_NULL);

            if (NX_SUCCESS != status)
            {
                print_to_console("Unable to get local time \r\n");
                return status;
            }

            sntp_boot_time = seconds - ticks;
            /* Convert to Unix epoch */
            sntp_boot_time -= UNIX_TO_NTP_EPOCH_SECS;

            status = nx_sntp_client_stop(g_active_sntp);

            return NX_SUCCESS;
        }
        tx_thread_sleep(50);
    }

    print_to_console("No SNTP Updates Received \r\n");
    return 1;
}


/* Return number of seconds since Unix Epoch (1/1/1970 00:00:00) */
unsigned long get_time(void)
{
    ULONG ticks = 0;

    ticks = tx_time_get();
    ticks = ticks / TX_TIMER_TICKS_PER_SECOND;

    return sntp_boot_time + ticks;
}
