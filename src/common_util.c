/***********************************************************************************************************************
* File Name    : common_util.c
* Description  : This file contains utility functions.
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

#include "common_util.h"
#include "console_thread.h"

/** Prints a string to the console.
     * @pre None
     * @param[in] msg NULL terminated character array
     * @return None
 */
void print_to_console(const char *msg);

static bool isleapyear(int year);
static uint32_t month(int a, int yy);

/*********************************************************************************************************************
 * @brief  led_on_off function
 *
 * This function turns ON/OFF the user LED based on incoming MQTT message
 *********************************************************************************************************************/
void led_on_off(ioport_port_pin_t pin, char *state)
{
    int on = 0;

    if (strcmp(state, "OFF") == 0)
        on = 1;
    else if (strcmp(state, "ON") == 0)
        on = 0;
    else
        return;

    g_ioport.p_api->pinWrite(pin, on ? IOPORT_LEVEL_LOW : IOPORT_LEVEL_HIGH);
}

static char GetChar(UINT timeout)
{
    uint8_t   data    = 0;
    ssp_err_t ssp_err = SSP_SUCCESS;

    ssp_err = g_sf_comms0.p_api->read(g_sf_comms0.p_ctrl, &data, 1, timeout);
    APP_ERR_TRAP(ssp_err);

    /* Type casting to char to fix warning.
     * Also the data will not be signed and entered character will not be extended ascii */
    return((char)data);
}

static void PutChar(const uint8_t *data, size_t dataLen)
{
    ssp_err_t ssp_err = SSP_SUCCESS;
    ssp_err = g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl, data, dataLen, TX_WAIT_FOREVER);
    APP_ERR_TRAP(ssp_err);
}

void GetUserInput(char *str)
{
    char *ptr = str;
    char temp = 0;

    while(1)
    {
        temp  = GetChar(TIME_OUT);
        if(temp == '\r')
            break;

        /* If backspace (0x08 or 0x7F) is received then move pointer back by one */
        if((temp == BACK_SPACE_CHAR_CTRL_H) || (temp == BACK_SPACE_CHAR_CTRL_QUESTION))
        {
            /*check to see that str pointer doesn't roll back*/
            if(str > ptr)
            {
                str--;

                /* Type casting temp to unsigned 8 bit integer is safe as the
                 * character received from terminal is not going to be negative.
                 */
                PutChar((uint8_t*)&temp, 1);
                temp = ' ';

                /* Type casting temp to unsigned 8 bit integer is safe as the
                 * character received from terminal is not going to be negative.
                 */
                PutChar((uint8_t*)&temp, 1);
                temp = BACK_SPACE_CHAR_CTRL_H;
            }
            else
            {
                /* Do not delete/overwrite characters before command prompt */
                continue;
            }
        }
        else
        {
            *str++ = temp;
        }

        PutChar((uint8_t*)&temp, 1);
    }

    *str = '\0';
    str = ptr;
}

/*
 * convert float to string format and store it in buffer
 */
void print_float(char *buffer, size_t buflen, double value)
{
    int  integer  = (int) value;
    int  fraction = (int) ((value - integer) * 100.0);
    char sign     = 0;

    if(buffer == NULL)
    {
        print_to_console("\r\n NULL pointer in print_float fn\r\n");
        return;
    }
    if(value < 0.0)
    {
        sign = '-';
        snprintf (buffer, buflen, "%c%d.%02d", sign, abs(integer), abs (fraction));
    }
    else
        snprintf (buffer, buflen, "%d.%02d", abs(integer), abs (fraction));
}



/*
 * Helper function for convertTimeToEpoch()
 */
static int32_t days(int y1, int y2, int m1, int m2, int d1, int d2)
{
    int32_t count = 0, i = 0;
    for (i = y1; i < y2; i++)
    {
        if (isleapyear (i))
            count += 366;
        else
            count += 365;
    }
    count -= (int32_t) month (m1, y1);
    count -= d1;
    count += (int32_t) month (m2, y2);
    count += d2;
    if (count < 0)
        count = count * -1;
    return count;
}

/*
 * Helper function for convertTimeToEpoch()
 */
static uint32_t month(int a, int yy)
{
    uint32_t x = 0, c = 0;
    uint32_t mon[12] =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    for (c = 0; c < (uint32_t)(a - 1); c++)
    {
        if (c == 1)
        {
            if (isleapyear (yy))
                x += 29;
            else
                x += 28;
        }
        else
            x += mon[c];
    }
    return (x);
}

/*
 * Helper function for convertTimeToEpoch()
 */
static bool isleapyear(int year)
{
    bool i = true;
    if (year % 400 == 0)
        i = true;
    else if (year % 100 == 0)
        i = false;
    else if (year % 4 == 0)
        i = true;
    else
        i = false;
    return i;
}

/*
 * Helper function for convertTimeToEpoch()
 */
static int char2num(UCHAR c)
{
    int i = 0;
    i = (int) c - 48;
    return i;
}

/* Converts UTC Date and Time to epoch
 * @input:- Date and Time in UTC
 * @output:- unix epoch time
 */
int32_t convertTimeToEpoch(const UCHAR* dateTime)
{
    int32_t y = 0, m = 0, d = 0, h = 0, min = 0, sec = 0, day = 0;
    int32_t time = 0;
    y = (char2num (dateTime[0]) * 10 + char2num (dateTime[1]) + 2000);
    m = (char2num (dateTime[2]) * 10 + char2num (dateTime[3]));
    d = (char2num (dateTime[4]) * 10 + char2num (dateTime[5]));
    h = (char2num (dateTime[6]) * 10 + char2num (dateTime[7]));
    min = (char2num (dateTime[8]) * 10 + char2num (dateTime[9]));
    sec = (char2num (dateTime[10]) * 10 + char2num (dateTime[11]));
    day = (days (1970, y, 1, m, 1, d));
    time = ((day * 24 + h) * 60 + min) * 60 + sec;
    return time;
}

char *ftoa(float f, char *buf)
{
    char *cp = buf;
    unsigned long l = 0, rem = 0;

    if (f < 0)
    {
        *cp++ = '-';
        f = -f;
    }
    l = (unsigned long) f;
    f -= (float) l;
    rem = (unsigned long) (f * 1e6);
    sprintf (cp, "%lu.%6.6lu", l, rem);
    return buf;
}
