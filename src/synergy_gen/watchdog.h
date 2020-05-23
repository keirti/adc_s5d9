/* generated thread header file - do not edit */
#ifndef WATCHDOG_H_
#define WATCHDOG_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
extern "C" void watchdog_entry(void);
#else
extern void watchdog_entry(void);
#endif
#include "r_wdt.h"
#include "r_wdt_api.h"
#include "sf_thread_monitor.h"
#ifdef __cplusplus
extern "C"
{
#endif
/** WDT on WDT Instance. */
extern const wdt_instance_t g_wdt0;
#ifndef NULL
void NULL(wdt_callback_args_t *p_args);
#endif
extern const sf_thread_monitor_instance_t g_sf_thread_monitor0;
void g_sf_thread_monitor0_err_callback(void *p_instance, void *p_data);
void sf_thread_monitor_init0(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* WATCHDOG_H_ */
