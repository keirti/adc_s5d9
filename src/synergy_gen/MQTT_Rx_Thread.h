/* generated thread header file - do not edit */
#ifndef MQTT_RX_THREAD_H_
#define MQTT_RX_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
extern "C" void MQTT_Rx_Thread_entry(void);
#else
extern void MQTT_Rx_Thread_entry(void);
#endif
#ifdef __cplusplus
extern "C"
{
#endif
extern TX_SEMAPHORE g_mqtt_notify_sem;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* MQTT_RX_THREAD_H_ */
