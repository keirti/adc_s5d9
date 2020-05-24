/* generated thread header file - do not edit */
#ifndef THREAD0_H_
#define THREAD0_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
extern "C" void thread0_entry(void);
#else
extern void thread0_entry(void);
#endif
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;
#ifndef timer_user_callback
void timer_user_callback(timer_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;
#ifdef NULL
#else
extern void NULL(uint32_t channel, uint32_t level);
#endif
#ifndef user_uart_callback
void user_uart_callback(uart_callback_args_t *p_args);
#endif
extern TX_SEMAPHORE g_uart_semaphore0;
extern TX_QUEUE g_modbus_queue;
extern TX_QUEUE g_serial_RX_queue;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* THREAD0_H_ */
