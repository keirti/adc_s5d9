/***********************************************************************************************************************
 * Copyright [2015-2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

#ifndef SF_THREAD_MONITOR_PRIVATE_API_H
#define SF_THREAD_MONITOR_PRIVATE_API_H

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t SF_THREAD_MONITOR_Open (sf_thread_monitor_ctrl_t * const p_ctrl,
                                  sf_thread_monitor_cfg_t const * const p_cfg);

ssp_err_t SF_THREAD_MONITOR_Close (sf_thread_monitor_ctrl_t * const p_ctrl);

ssp_err_t SF_THREAD_MONITOR_ThreadRegister (sf_thread_monitor_ctrl_t * const    p_ctrl,
                                            sf_thread_monitor_counter_min_max_t const * p_counter_min_max);

ssp_err_t SF_THREAD_MONITOR_ThreadUnregister (sf_thread_monitor_ctrl_t * const p_ctrl);

ssp_err_t SF_THREAD_MONITOR_CountIncrement (sf_thread_monitor_ctrl_t * const p_ctrl);

ssp_err_t SF_THREAD_MONITOR_VersionGet (ssp_version_t * const p_version);

void      SF_THREAD_MONITOR_Thread (ULONG thread_input);

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif /* SF_THREAD_MONITOR_PRIVATE_API_H */
