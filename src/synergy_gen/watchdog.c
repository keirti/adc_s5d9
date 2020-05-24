/* generated thread source file - do not edit */
#include "watchdog.h"

TX_THREAD watchdog;
void watchdog_create(void);
static void watchdog_func(ULONG thread_input);
static uint8_t watchdog_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.watchdog") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
static wdt_instance_ctrl_t g_wdt0_ctrl;

static const wdt_cfg_t g_wdt0_cfg =
{ .start_mode = WDT_START_MODE_REGISTER,
  .autostart = true,
  .timeout = WDT_TIMEOUT_16384,
  .clock_division = WDT_CLOCK_DIVISION_8192,
  .window_start = WDT_WINDOW_START_100,
  .window_end = WDT_WINDOW_END_0,
  .reset_control = WDT_RESET_CONTROL_RESET,
  .stop_control = WDT_STOP_CONTROL_ENABLE,
  .p_callback = NULL, };

/* Instance structure to use this module. */
const wdt_instance_t g_wdt0 =
{ .p_ctrl = &g_wdt0_ctrl, .p_cfg = &g_wdt0_cfg, .p_api = &g_wdt_on_wdt };
#if defined(__ICCARM__)
#define g_sf_thread_monitor0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_sf_thread_monitor0_err_callback  = g_sf_thread_monitor0_err_callback_internal
#elif defined(__GNUC__)
#define g_sf_thread_monitor0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_sf_thread_monitor0_err_callback_internal")))
#endif
void g_sf_thread_monitor0_err_callback(void *p_instance, void *p_data)
g_sf_thread_monitor0_err_callback_WEAK_ATTRIBUTE;
static sf_thread_monitor_instance_ctrl_t g_sf_thread_monitor0_ctrl;
static sf_thread_monitor_cfg_t g_sf_thread_monitor0_cfg =
{ .p_lower_lvl_wdt = &g_wdt0, .priority = 1, .profiling_mode_enabled = false };
/* Instance structure to use this module. */
const sf_thread_monitor_instance_t g_sf_thread_monitor0 =
{ .p_ctrl = &g_sf_thread_monitor0_ctrl, .p_cfg = &g_sf_thread_monitor0_cfg, .p_api =
          &g_sf_thread_monitor_on_sf_thread_monitor };
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void g_sf_thread_monitor0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_sf_thread_monitor0_err_callback_internal(void *p_instance, void *p_data);
void g_sf_thread_monitor0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.

 *            - void sf_thread_monitor_init0(void)
 **********************************************************************************************************************/
void sf_thread_monitor_init0(void)
{
    ssp_err_t ssp_err_g_sf_thread_monitor0;
    ssp_err_g_sf_thread_monitor0 = g_sf_thread_monitor0.p_api->open (g_sf_thread_monitor0.p_ctrl,
                                                                     g_sf_thread_monitor0.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_thread_monitor0)
    {
        g_sf_thread_monitor0_err_callback ((void *) &g_sf_thread_monitor0, &ssp_err_g_sf_thread_monitor0);
    }
}

extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void watchdog_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&watchdog, (CHAR *) "WD", watchdog_func, (ULONG) NULL, &watchdog_stack, 1024, 1, 1, 1,
                            TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&watchdog, 0);
    }
}

static void watchdog_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */
    /** Call initialization function if user has selected to do so. */
#if (1)
    sf_thread_monitor_init0 ();
#endif

    /* Enter user code for this thread. */
    watchdog_entry ();
}
