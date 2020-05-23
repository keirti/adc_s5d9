/*
 * FreeModbus Libary: Atmel AT91SAM3S Demo Application
 * Copyright (C) 2010 Christian Walter <cwalter@embedded-solutions.at>
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id$
 */

/* ----------------------- System includes ----------------------------------*/
#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "stdio.h"

#include "sf_thread_monitor_api.h"
#include "watchdog.h"
/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START                 ( 1000 )
#define REG_INPUT_NREGS                 ( 64 )

#define REG_HOLDING_START               ( 1 )
#define REG_HOLDING_NREGS               ( 32 )

#define TASK_MODBUS_STACK_SIZE          ( 256 )
#define TASK_MODBUS_PRIORITY            ( tskIDLE_PRIORITY + 1 )

#define TASK_APPL_STACK_SIZE            ( 256 )
#define TASK_APPL_PRIORITY              ( tskIDLE_PRIORITY + 1 )

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];
extern uint16_t adc_data[7];
/* ----------------------- Start implementation -----------------------------*/

void
vTaskMODBUS( void );

void
vTaskMODBUS( void )
{
    const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
    eMBErrorCode    eStatus;

        if( MB_ENOERR != ( eStatus = eMBInit( MB_RTU, 0x04, 3, 9600, MB_PAR_EVEN ) ) )
        {
            printf("modbus not initialized : %d\n",eStatus);/* Can not initialize. Add error handling code here. */
        }
        else
        {
            if( MB_ENOERR != ( eStatus = eMBSetSlaveID( 0x34, TRUE, ucSlaveID, 3 ) ) )
            {
                printf("modbus setid : %d\n",eStatus);  /* Can not set slave id. Check arguments */
            }
            else if( MB_ENOERR != ( eStatus = eMBEnable(  ) ) )
            {
                printf("protocol stack : %d\n",eStatus);  /* Enable failed. */
            }
            else
            {
                usRegHoldingBuf[0] = 1;
                do
                {
                    ( void )eMBPoll(  );
                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_0, &adc_data[0]);

                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_1, &adc_data[1]);

                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_2, &adc_data[2]);

                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_3, &adc_data[3]);

                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_4, &adc_data[4]);

                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_5, &adc_data[5]);

                    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_6, &adc_data[6]);

                    /* Here we simply count the number of poll cycles. */
                    usRegInputBuf[0]++;
                }
                while( usRegHoldingBuf[0] );
            }
            ( void )eMBDisable(  );
            ( void )eMBClose(  );
        }

        /*
         *  Increment the thread_counter
         */
        if(SSP_SUCCESS != g_sf_thread_monitor0.p_api->countIncrement(g_sf_thread_monitor0.p_ctrl))
        {
            __BKPT(0);
        }
        tx_thread_sleep (5);
}


eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START ) && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        printf("read input register");
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            if(iRegIndex <7)
            usRegInputBuf[iRegIndex] = adc_data[iRegIndex];
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
   // printf("holding register\n");
    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
/*
        printf("pucRegBuffer : %d\n",*pucRegBuffer);
        printf("usAddress : %d\n",usAddress);
        printf("usNRegs : %d\n",usNRegs);
        printf("iRegIndex :%d\n",iRegIndex);
*/

        switch ( eMode )
        {
        case MB_REG_READ:
            printf("executing callback fn read holding register\n");
            while( usNRegs > 0 )
            {
                /*sending adc data to first 7 references */

              /*  if(iRegIndex < 7)
                usRegHoldingBuf[iRegIndex] = adc_data[iRegIndex];*/
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
           printf("executing write holding register\n");
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}
