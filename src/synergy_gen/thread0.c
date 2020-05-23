/* generated thread source file - do not edit */
#include "thread0.h"

TX_THREAD thread0;
void thread0_create(void);
static void thread0_func(ULONG thread_input);
static uint8_t thread0_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.thread0") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if (7) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timer0) && !defined(SSP_SUPPRESS_ISR_GPT0)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 0);
#endif
#endif
static gpt_instance_ctrl_t g_timer0_ctrl;
static const timer_on_gpt_cfg_t g_timer0_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 50, .unit = TIMER_UNIT_PERIOD_USEC, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0, .autostart = true, .p_callback = timer_user_callback, .p_context = &g_timer0, .p_extend =
          &g_timer0_extend,
  .irq_ipl = (7), };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gpt };
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer0) && !defined(SSP_SUPPRESS_ISR_DTCELC_EVENT_SCI3_TXI)
#define DTC_ACTIVATION_SRC_ELC_EVENT_SCI3_TXI
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_0) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_0);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0
#endif
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_1) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_1);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1
#endif
#endif
#endif

dtc_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info,
  .activation_source = ELC_EVENT_SCI3_TXI,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer0,
  .irq_ipl = (BSP_IRQ_DISABLED) };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
#if SCI_UART_CFG_RX_ENABLE
#if (12) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart0) && !defined(SSP_SUPPRESS_ISR_SCI3)
SSP_VECTOR_DEFINE_CHAN(sci_uart_rxi_isr, SCI, RXI, 3);
#endif
#endif
#endif
#if SCI_UART_CFG_TX_ENABLE
#if (12) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart0) && !defined(SSP_SUPPRESS_ISR_SCI3)
SSP_VECTOR_DEFINE_CHAN(sci_uart_txi_isr, SCI, TXI, 3);
#endif
#endif
#if (12) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart0) && !defined(SSP_SUPPRESS_ISR_SCI3)
SSP_VECTOR_DEFINE_CHAN(sci_uart_tei_isr, SCI, TEI, 3);
#endif
#endif
#endif
#if SCI_UART_CFG_RX_ENABLE
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_uart0) && !defined(SSP_SUPPRESS_ISR_SCI3)
SSP_VECTOR_DEFINE_CHAN(sci_uart_eri_isr, SCI, ERI, 3);
#endif
#endif
#endif
sci_uart_instance_ctrl_t g_uart0_ctrl;

/** UART extended configuration for UARTonSCI HAL driver */
const uart_on_sci_cfg_t g_uart0_cfg_extend =
{ .clk_src = SCI_CLK_SRC_INT, .baudclk_out = false, .rx_edge_start = true, .noisecancel_en = false, .p_extpin_ctrl =
          NULL,
  .bitrate_modulation = true, .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .baud_rate_error_x_1000 = (uint32_t) (
          2.0 * 1000),
  .uart_comm_mode = UART_MODE_RS485, .uart_rs485_mode = UART_RS485_FD, .rs485_de_pin = IOPORT_PORT_08_PIN_00, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 3, .baud_rate = 9600, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits =
          UART_STOP_BITS_1,
  .ctsrts_en = false, .p_callback = user_uart_callback, .p_context = &g_uart0, .p_extend = &g_uart0_cfg_extend,
#define SYNERGY_NOT_DEFINED (1)                        
#if (SYNERGY_NOT_DEFINED == g_transfer0)
  .p_transfer_tx = NULL,
#else
  .p_transfer_tx = &g_transfer0,
#endif            
#if (SYNERGY_NOT_DEFINED == SYNERGY_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
  .p_transfer_rx = &SYNERGY_NOT_DEFINED,
#endif   
#undef SYNERGY_NOT_DEFINED            
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (BSP_IRQ_DISABLED), };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };
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
adc_instance_ctrl_t g_adc0_ctrl;
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
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{ .p_ctrl = &g_adc0_ctrl, .p_cfg = &g_adc0_cfg, .p_channel_cfg = &g_adc0_channel_cfg, .p_api = &g_adc_on_adc };
TX_SEMAPHORE g_uart_semaphore0;
TX_QUEUE g_modbus_queue;
static uint8_t queue_memory_g_modbus_queue[20];
TX_QUEUE g_serial_RX_queue;
static uint8_t queue_memory_g_serial_RX_queue[20];
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void thread0_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_uart_semaphore0;
    err_g_uart_semaphore0 = tx_semaphore_create (&g_uart_semaphore0, (CHAR *) "UART semaphore", 1);
    if (TX_SUCCESS != err_g_uart_semaphore0)
    {
        tx_startup_err_callback (&g_uart_semaphore0, 0);
    }
    UINT err_g_modbus_queue;
    err_g_modbus_queue = tx_queue_create (&g_modbus_queue, (CHAR *) "Modbus Queue", 1, &queue_memory_g_modbus_queue,
                                          sizeof(queue_memory_g_modbus_queue));
    if (TX_SUCCESS != err_g_modbus_queue)
    {
        tx_startup_err_callback (&g_modbus_queue, 0);
    }
    UINT err_g_serial_RX_queue;
    err_g_serial_RX_queue = tx_queue_create (&g_serial_RX_queue, (CHAR *) "serial RX Queue", 1,
                                             &queue_memory_g_serial_RX_queue, sizeof(queue_memory_g_serial_RX_queue));
    if (TX_SUCCESS != err_g_serial_RX_queue)
    {
        tx_startup_err_callback (&g_serial_RX_queue, 0);
    }

    UINT err;
    err = tx_thread_create (&thread0, (CHAR *) "Thread", thread0_func, (ULONG) NULL, &thread0_stack, 1024, 3, 3, 1,
                            TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&thread0, 0);
    }
}

static void thread0_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    thread0_entry ();
}