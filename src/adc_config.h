/*******************************************************************************
(C) COPYRIGHT Smart Power Solutions Limited - 2020

FILE
    adc_cfg.h

ORIGINAL AUTHOR
    Chris Goodwin

DESCRIPTION
    Enter file description

REFERENCES
    None

REVIEWS
    None
*******************************************************************************/

#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_

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
/* None */
	
/*=============================================================================*
 Public Data
*=============================================================================*/
#if (4) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_adc0) && !defined(SSP_SUPPRESS_ISR_ADC0)
SSP_VECTOR_DEFINE_CHAN(adc_scan_end_isr, ADC, SCAN_END, 0);
#endif
#endif
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_adc0) && !defined(SSP_SUPPRESS_ISR_ADC0)
SSP_VECTOR_DEFINE_CHAN(adc_scan_end_b_isr, ADC, SCAN_END_B, 0);
#endif
#endif

const adc_cfg_t g_adc0_cfg =
{ .unit = 0,
  .mode = ADC_MODE_CONTINUOUS_SCAN,
  .resolution = ADC_RESOLUTION_12_BIT,
  .alignment = ADC_ALIGNMENT_RIGHT,
  .add_average_count = ADC_ADD_OFF,
  .clearing = ADC_CLEAR_AFTER_READ_ON,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC,
  .p_callback = NULL,
  .p_context = &g_adc0,
  .scan_end_ipl = (4),
  .scan_end_b_ipl = (BSP_IRQ_DISABLED),
  .calib_adc_skip = false,
  .voltage_ref = ADC_EXTERNAL_VOLTAGE,
  .over_current = OVER_CURRENT_DETECTION_ENABLE,
  .pga0 = SINGLE_INPUT_GAIN_1,
  .pga1 = PGA_DISABLE,
  .pga2 = PGA_DISABLE, };

const adc_channel_cfg_t g_adc0_channel_cfg =
        { .scan_mask = (uint32_t) (
                ((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) ADC_MASK_CHANNEL_1) | ((uint64_t) ADC_MASK_CHANNEL_2)
                        | ((uint64_t) ADC_MASK_CHANNEL_3) | ((uint64_t) ADC_MASK_CHANNEL_4)
                        | ((uint64_t) ADC_MASK_CHANNEL_5) | ((uint64_t) ADC_MASK_CHANNEL_6)
                        | ((uint64_t) ADC_MASK_CHANNEL_7) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | (0)),
          /** Group B channel mask is right shifted by 32 at the end to form the proper mask */
          .scan_mask_group_b = (uint32_t) (
                  (((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) ADC_MASK_CHANNEL_1) | ((uint64_t) ADC_MASK_CHANNEL_2)
                          | ((uint64_t) ADC_MASK_CHANNEL_3) | ((uint64_t) ADC_MASK_CHANNEL_4)
                          | ((uint64_t) ADC_MASK_CHANNEL_5) | ((uint64_t) ADC_MASK_CHANNEL_6)
                          | ((uint64_t) ADC_MASK_CHANNEL_7) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | (0)) >> 32),
          .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask = (uint32_t) (
                  (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)
                          | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)),
          .sample_hold_mask = (uint32_t) ((ADC_MASK_CHANNEL_0) | (ADC_MASK_CHANNEL_1) | (ADC_MASK_CHANNEL_2)), .sample_hold_states =
                  24, };

/*=============================================================================*
 Public Function Definitions
*=============================================================================*/
/*-------------------------------------------------------------------* 

  NAME
	_ENTER_FUNCTION_NAME_HERE_
  
  DESCRIPTION
	_ENTER_FUNCTION_DESCRIPTION_HERE_
  PARAM
	_PARAM_NAME_ - _DESCRIPTION_
  
  RETURNS
	_RETURN_DESCRIPTION_

*--------------------------------------------------------------------*/
/* None */ 

/*=============================================================================*
End Of File
*=============================================================================*/
#endif  /* ENTER NAMESPACE HERE */
