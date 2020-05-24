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

/**********************************************************************************************************************
 * File Name    : sf_thread_monitor.h
 * Description  : Thread Monitor interface
 **********************************************************************************************************************/


#ifndef SF_THREAD_MONITOR_H
#define SF_THREAD_MONITOR_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_THREAD_MONITOR Thread Monitor Framework
 * @brief Framework module providing monitoring of threads.
 *
 * Any misbehaving threads result in the device being reset.
 * Both the WDT and IWDT HAL modules are supported by this framework module.
 *
 * @section SF_THREAD_MONITOR_INTERFACE Interface Used
 * @see WDT_API
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_thread_monitor_api.h"

//#include "sf_thread_monitor_private_api.h"
#include "sf_thread_monitor_cfg.h"

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Version of code that implements the API defined in this file */
#define SF_THREAD_MONITOR_CODE_VERSION_MAJOR (1U)
#define SF_THREAD_MONITOR_CODE_VERSION_MINOR (10U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Thread monitor control block. DO NOT INITIALIZE.  Initialization occurs when
 * sf_thread_monitor_api_t::open is called. */
typedef struct st_sf_thread_monitor_instance_ctrl
{
    /** Used by driver to check if the control structure is valid */
    uint32_t  open;

    /** Pointer to interface structure for the watchdog peripheral */
    wdt_instance_t const * p_lower_lvl_wdt;

    /** Time in milliseconds of the watchdog timeout period. Used to calculate the period of the monitoring thread. */
    uint32_t  timeout_period_msec;

    /** Maximum count value of the watchdog. Used to synchronise to the count. */
    uint32_t  timeout_period_watchdog_clocks;

    /** Used by the driver to check if profiling mode is enabled.*/
    bool      profiling_mode_enabled;

    TX_MUTEX  mutex;                            ///< Mutex to protect access to the thread counters

    /** Value used to verify profiling mode is enabled when prfiling_mode_enabled == true.*/
    uint32_t  profiling_mode_check;

    /** Data storage for the thread counter information.*/
    sf_thread_monitor_thread_counter_t  thread_counters[THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS];

    TX_THREAD                           thread;     ///< Thread monitor thread.

    void const                          * p_extend; ///< Extended configuration data

    /** Stack for thread monitor thread.  */
    uint8_t  stack[THREAD_MONITOR_THREAD_STACK_SIZE] BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
} sf_thread_monitor_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const sf_thread_monitor_api_t g_sf_thread_monitor_on_sf_thread_monitor;
/** @endcond */

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif //SF_WATCHDOG_API_H

/*******************************************************************************************************************//**
 * @} (end SF_THREAD_MONITOR)
 **********************************************************************************************************************/
