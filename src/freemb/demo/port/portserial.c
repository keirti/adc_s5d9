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
#include "mbport.h"
#include "stdio.h"
/* ----------------------- Defines ------------------------------------------*/

#define NUMBER_SCI_CHANNELS 10 //S5D9 has 10 channels 0 - 9

#define SEMPAHORE_GET_TIMEOUT   (10)

/* ----------------------- Static variables ---------------------------------*/

uart_cfg_t g_uart0_cfg_runtime;

void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    sci_uart_instance_ctrl_t * p_ctrl = (sci_uart_instance_ctrl_t *) g_uart0.p_ctrl;
    R_SCI0_Type * p_sci_reg = (R_SCI0_Type *) p_ctrl->p_reg;

    if( xRxEnable )
    {
        p_sci_reg->SCR_b.RE = 1;
    }
    else
    {
        p_sci_reg->SCR_b.RE = 0;
    }

    /* TX is enabled by the SSP UART driver internally when the write API id called */
    /* and disable when all the characters have been transmitted */
    if( xTxEnable )
    {
        pxMBFrameCBTransmitterEmpty(  );
    }
    else
    {

    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bStatus = FALSE;
    ssp_err_t err;

    memcpy(&g_uart0_cfg_runtime, g_uart0.p_cfg, sizeof(uart_cfg_t));

    if( ( ucPORT <= NUMBER_SCI_CHANNELS ) )
    {
        g_uart0_cfg_runtime.channel   = ucPORT;
        g_uart0_cfg_runtime.baud_rate = ulBaudRate;

        bStatus = TRUE;

        switch ( eParity )
        {
        case MB_PAR_NONE:
            g_uart0_cfg_runtime.parity = UART_PARITY_OFF;
            break;
        case MB_PAR_ODD:
            g_uart0_cfg_runtime.parity = UART_PARITY_ODD;
            break;
        case MB_PAR_EVEN:
            g_uart0_cfg_runtime.parity = UART_PARITY_EVEN;
            break;
        default:
            bStatus = FALSE;
            break;
        }

        switch ( ucDataBits )
        {

        case 8:
            g_uart0_cfg_runtime.data_bits = UART_DATA_BITS_8;
            break;
        case 7:
            g_uart0_cfg_runtime.data_bits = UART_DATA_BITS_7;
            break;
        default:
            bStatus = FALSE;
        }

        if( TRUE == bStatus )
        {
            err = g_uart0.p_api->open(g_uart0.p_ctrl, &g_uart0_cfg_runtime);

           // printf("err in uart open  : %d\n",err);

            if (SSP_SUCCESS != err)
            {
                bStatus = FALSE;
            }
/*            printf("bstatus inside portserialinit: %d\n",bStatus);*/
        }
    }
    return bStatus;
}

void
vMBPortSerialClose( void )
{
    ssp_err_t err;

    err = g_uart0.p_api->close(g_uart0.p_ctrl);
    if (SSP_SUCCESS != err)
    {
        __NOP();
    }
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    ssp_err_t err;
    BOOL            bStatus = TRUE;
    ULONG status;

    status = tx_semaphore_get(&g_uart_semaphore0, TX_NO_WAIT);
    if (TX_SUCCESS != status)
    {
        bStatus = FALSE;
    }
    else
    {
        err = g_uart0.p_api->write(g_uart0.p_ctrl, (uint8_t *)&ucByte, 1);
        if (SSP_SUCCESS != err)
        {
            bStatus = FALSE;
        }
    }
    return bStatus;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    BOOL  bStatus = TRUE;
    ULONG status;
    int32_t data;

    status = tx_queue_receive(&g_serial_RX_queue, &data , TX_NO_WAIT);
    if (TX_SUCCESS != status)
    {
        bStatus = FALSE;
    }
    else
    {
        *pucByte = (CHAR)data;

/*        printf("data  %d\n",data);*/
//        printf("data port serial get byte = %d\n",*pucByte);

    }

    return bStatus;
}

#if 0
UART_EVENT_RX_COMPLETE        = (1UL << 0),         ///< Receive complete event
UART_EVENT_TX_COMPLETE        = (1UL << 1),         ///< Transmit complete event
UART_EVENT_ERR_PARITY         = (1UL << 2),         ///< Parity error event
UART_EVENT_ERR_FRAMING        = (1UL << 3),         ///< Mode fault error event
UART_EVENT_BREAK_DETECT       = (1UL << 4),         ///< Break detect error event
UART_EVENT_ERR_OVERFLOW       = (1UL << 5),         ///< FIFO Overflow error event
UART_EVENT_ERR_RXBUF_OVERFLOW = (1UL << 6),         ///< DEPRECATED: Receive buffer overflow error event
UART_EVENT_RX_CHAR            = (1UL << 7),         ///< Character received
UART_EVENT_TX_DATA_EMPTY      = (1UL << 8),         ///< Last byte is transmitting, ready for more data
#endif

void user_uart_callback(uart_callback_args_t *p_args)
{
    ULONG status;

    if (UART_EVENT_RX_CHAR == p_args->event)
    {
        status = tx_queue_send(&g_serial_RX_queue, &p_args->data , TX_NO_WAIT);
        if (TX_SUCCESS != status)
        {
            __NOP();
        }
        pxMBFrameCBByteReceived(  );

    }
    else if (UART_EVENT_TX_COMPLETE == p_args->event)
    {
        status = tx_semaphore_ceiling_put(&g_uart_semaphore0, 1);
        if (TX_SUCCESS != status)
        {
            __NOP();
        }

        pxMBFrameCBTransmitterEmpty(  );
    }
    else
    {

    }
}
