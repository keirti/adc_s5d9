/*******************************************************************************
(C) COPYRIGHT Smart Power Systems - 2020

FILE
    adc_interface.h

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    File to provide access to the ADC values to each module that needs it

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

#ifndef _ADC_INTERFACE_H_
#define _ADC_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================*
 ANSI C & System-wide Header Files
*=============================================================================*/
#include "stdint.h"

/*=============================================================================*
 Interface Header Files
*=============================================================================*/
#include "r_adc_api.h"

/*=============================================================================*
 Pragmas
*=============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
*=============================================================================*/
#define NUM_ADC_CHANNELS    ADC_REG_CHANNEL_7
#define MAX_ADC_COUNT       4095.0f
#define ADC_VREF            3.3f

/*=============================================================================*
 Public Data
*=============================================================================*/
typedef struct
{
    uint16_t adc_raw_count;
    float adc_voltage;
    float gain;
    float offset;
    float scaled_value;
}adc_data_t;

/*=============================================================================*
 Public Function Definitions
*=============================================================================*/
/*-------------------------------------------------------------------*

  NAME
    get_adc_arr

  DESCRIPTION
    Accessor function for the ADC data array
  PARAM
    None

  RETURNS
    adc_data_t* - pointer to the adc data array

*--------------------------------------------------------------------*/
adc_data_t* get_adc_arr(void);

/*=============================================================================*
End Of File
*=============================================================================*/
#endif  /* _ADC_INTERFACE_H_ */
