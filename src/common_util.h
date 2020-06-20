/***********************************************************************************************************************
* File Name    : common_util.h
* Description  : This file contains utility functions and its Macro definitions.
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

#ifndef UTIL_H_
#define UTIL_H_

#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"

void print_to_console(const char* msg);

#define APP_ERR_TRAP(a)             if(a) {print_to_console("Fatal Error Detected!!! Please power cycle the board or attach debugger for debugging.\r\n");\
		                                   __asm("BKPT #0\n");} /* trap the error location */
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof(x[0]))
#define BACK_SPACE_CHAR_CTRL_H          (0x08)
#define BACK_SPACE_CHAR_CTRL_QUESTION   (0x7F)
#define CARRIAGE_RETURN                 (0x0D)
#define TIME_OUT                        (0xFFF)

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

void GetUserInput(char *str);
void print_float(char *buffer, size_t buflen, double value);
int32_t convertTimeToEpoch(const UCHAR* dateTime);
void led_on_off(ioport_port_pin_t pin, char *state);
char *ftoa(float f, char *buf);
#endif /* UTIL_H_ */
