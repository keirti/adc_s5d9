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

/* ----------------------- Defines ------------------------------------------*/

#define TIMER_50_USECONDS          (50)
#define MILLISECOND_PER_TIMER_TICK (1000/TX_TIMER_TICKS_PER_SECOND)

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    ssp_err_t err;
    bool ret = true;

    /* Auto start is disabled for the timer */
    err = g_timer0.p_api->open(g_timer0.p_ctrl, g_timer0.p_cfg);
    if (SSP_SUCCESS != err)
    {
        ret = false;
    }
    else
    {
        err = g_timer0.p_api->periodSet(g_timer0.p_ctrl, (timer_size_t)(usTim1Timerout50us * TIMER_50_USECONDS), TIMER_UNIT_PERIOD_USEC);
        if (SSP_SUCCESS != err)
        {
            ret = false;
        }
    }

    return ret;
}

void
vMBPortTimerClose( void )
{
    ssp_err_t err;

    err = g_timer0.p_api->close(g_timer0.p_ctrl);
    if (SSP_SUCCESS != err)
    {
        __NOP();
    }

}

void
vMBPortTimersEnable( void )
{
    ssp_err_t err;

    /* Auto start is disabled for the timer */
    err = g_timer0.p_api->start(g_timer0.p_ctrl);
    if (SSP_SUCCESS != err)
    {
        __NOP();
    }
}

void
vMBPortTimersDisable( void )
{
    ssp_err_t err;

    /* Auto start is disabled for the timer */
    err = g_timer0.p_api->stop(g_timer0.p_ctrl);
    if (SSP_SUCCESS != err)
    {
        __NOP();
    }
}

void  vMBPortTimersDelay( USHORT usTimeOutMS )
{
    ULONG sleep;
    UINT status;

    sleep = usTimeOutMS/MILLISECOND_PER_TIMER_TICK;

    if ((0 == sleep) && (0!= usTimeOutMS))
    {
        sleep = 1;
    }

    status = tx_thread_sleep(sleep);
    if (TX_SUCCESS != status)
    {
        __NOP();
    }
}

void timer_user_callback(timer_callback_args_t *p_args)
{
    if (TIMER_EVENT_EXPIRED  == p_args->event)
    {
       pxMBPortCBTimerExpired(  );
    }
}

