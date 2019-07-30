/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if DEVICE_SERIAL

#include <string.h>

#include "cmsis.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "serial_api.h"
#include "psoc6_utils.h"

#include "cy_sysclk.h"
#include "cy_gpio.h"
#include "cy_scb_uart.h"
#include "cy_sysint.h"

#define UART_OVERSAMPLE                 12
#define UART_DEFAULT_BAUDRATE           115200
#define NUM_SERIAL_PORTS                8
#define SERIAL_DEFAULT_IRQ_PRIORITY     3

typedef struct serial_s serial_obj_t;
#if DEVICE_SERIAL_ASYNCH
#define OBJ_P(in)     (&(in->serial))
#else
#define OBJ_P(in)     (in)
#endif

/*
 * NOTE: Cypress PDL high level API implementation of USART doe not
 * align well with Mbed interface for interrupt-driven serial I/O.
 * For this reason only low level PDL API is used here.
 */


static const cy_stc_scb_uart_config_t default_uart_config = {
    .uartMode                   = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode   = false,
    .smartCardRetryOnNack       = false,
    .irdaInvertRx               = false,
    .irdaEnableLowPowerReceiver = false,

    .oversample                 = UART_OVERSAMPLE,

    .enableMsbFirst             = false,
    .dataWidth                  = 8UL,
    .parity                     = CY_SCB_UART_PARITY_NONE,
    .stopBits                   = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter          = false,
    .breakWidth                 = 11UL,
    .dropOnFrameError           = false,
    .dropOnParityError          = false,

    .receiverAddress            = 0x0UL,
    .receiverAddressMask        = 0x0UL,
    .acceptAddrInFifo           = false,

    .enableCts                  = false,
    .ctsPolarity                = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel             = 20UL,
    .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,

    .rxFifoTriggerLevel         = 0UL,
    .rxFifoIntEnableMask        = 0x0UL,

    .txFifoTriggerLevel         = 0UL,
    .txFifoIntEnableMask        = 0x0UL
};

int stdio_uart_inited = false;
serial_t stdio_uart;

typedef struct irq_info_s {
    serial_obj_t        *serial_obj;
    uart_irq_handler    handler;
    uint32_t            id_arg;
    IRQn_Type           irqn;
#if defined (TARGET_MCU_PSOC6_M0)
    cy_en_intr_t        cm0p_irq_src;
#endif
} irq_info_t;

static irq_info_t irq_info[NUM_SERIAL_PORTS] = {
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn},
    {NULL, NULL, 0, unconnected_IRQn}
};


static void serial_irq_dispatcher(uint32_t serial_id)
{
    MBED_ASSERT(serial_id < NUM_SERIAL_PORTS);
    irq_info_t *info = &irq_info[serial_id];
    serial_obj_t *obj = info->serial_obj;
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    if (obj->async_handler) {
        obj->async_handler();
        return;
    }
#endif
    if (Cy_SCB_GetRxInterruptStatusMasked(obj->base) & CY_SCB_RX_INTR_NOT_EMPTY) {
        info->handler(info->id_arg, RxIrq);
        Cy_SCB_ClearRxInterrupt(obj->base, CY_SCB_RX_INTR_NOT_EMPTY);
    }

    if (Cy_SCB_GetTxInterruptStatusMasked(obj->base) & (CY_SCB_TX_INTR_LEVEL | CY_SCB_UART_TX_DONE)) {
        info->handler(info->id_arg, TxIrq);
    }
}

static void serial_irq_dispatcher_uart0(void)
{
    serial_irq_dispatcher(0);
}

static void serial_irq_dispatcher_uart1(void)
{
    serial_irq_dispatcher(1);
}

static void serial_irq_dispatcher_uart2(void)
{
    serial_irq_dispatcher(2);
}

static void serial_irq_dispatcher_uart3(void)
{
    serial_irq_dispatcher(3);
}

static void serial_irq_dispatcher_uart4(void)
{
    serial_irq_dispatcher(4);
}

static void serial_irq_dispatcher_uart5(void)
{
    serial_irq_dispatcher(5);
}

void serial_irq_dispatcher_uart6(void)
{
    serial_irq_dispatcher(6);
}

static void serial_irq_dispatcher_uart7(void)
{
    serial_irq_dispatcher(7);
}


static void (*irq_dispatcher_table[])(void) = {
    serial_irq_dispatcher_uart0,
    serial_irq_dispatcher_uart1,
    serial_irq_dispatcher_uart2,
    serial_irq_dispatcher_uart3,
    serial_irq_dispatcher_uart4,
    serial_irq_dispatcher_uart5,
    serial_irq_dispatcher_uart6,
    serial_irq_dispatcher_uart7
};


static IRQn_Type serial_irq_allocate_channel(serial_obj_t *obj)
{
#if defined (TARGET_MCU_PSOC6_M0)
    irq_info[obj->serial_id].cm0p_irq_src = scb_0_interrupt_IRQn + obj->serial_id;
    return cy_m0_nvic_allocate_channel(CY_SERIAL_IRQN_ID + obj->serial_id);
#else
    return (IRQn_Type)(scb_0_interrupt_IRQn + obj->serial_id);
#endif // M0
}

static int serial_irq_setup_channel(serial_obj_t *obj)
{
    cy_stc_sysint_t irq_config;
    irq_info_t      *info = &irq_info[obj->serial_id];

    if (info->irqn == unconnected_IRQn) {
        IRQn_Type irqn = serial_irq_allocate_channel(obj);
        if (irqn < 0) {
            return (-1);
        }
        // Configure NVIC
        irq_config.intrPriority = SERIAL_DEFAULT_IRQ_PRIORITY;
        irq_config.intrSrc = irqn;
#if defined (TARGET_MCU_PSOC6_M0)
        irq_config.cm0pSrc = info->cm0p_irq_src;
#endif
        if (Cy_SysInt_Init(&irq_config, irq_dispatcher_table[obj->serial_id]) != CY_SYSINT_SUCCESS) {
            return(-1);
        }

        info->irqn = irqn;
        info->serial_obj = obj;
        NVIC_EnableIRQ(irqn);
    }
    return 0;
}

static void serial_irq_release_channel(serial_obj_t *obj)
{
    irq_info_t *info = &irq_info[obj->serial_id];

    if (info->irqn != unconnected_IRQn) {
        MBED_ASSERT(info->serial_obj == obj);
        NVIC_DisableIRQ(info->irqn);
    
#if defined (TARGET_MCU_PSOC6_M0)
        cy_m0_nvic_release_channel(info->irqn, CY_SERIAL_IRQN_ID + obj->serial_id);
#endif //M0
        info->irqn = unconnected_IRQn;
        info->serial_obj = NULL;
        info->handler = NULL;
    }
}

/*
 * Calculates fractional divider value.
 */
static uint32_t divider_value(uint32_t frequency, uint32_t frac_bits)
{
    /* UARTs use peripheral clock */
    return ((CY_CLK_PERICLK_FREQ_HZ * (1 << frac_bits)) + (frequency / 2)) / frequency;
}

static cy_en_sysclk_status_t serial_init_clock(serial_obj_t *obj, uint32_t baudrate)
{
    cy_en_sysclk_status_t status = CY_SYSCLK_BAD_PARAM;

    if (obj->div_num == CY_INVALID_DIVIDER) {
        uint32_t divider_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_16_5_BIT);

        if (divider_num < PERI_DIV_16_5_NR) {
            /* Assign fractional divider. */
            status = Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_16_5_BIT, divider_num);
            if (status == CY_SYSCLK_SUCCESS) {
                obj->div_type = CY_SYSCLK_DIV_16_5_BIT;
                obj->div_num = divider_num;
            }
        } else {
            // Try 16-bit divider.
            divider_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_16_BIT);
            if (divider_num < PERI_DIV_16_NR) {
                /* Assign 16-bit divider. */
                status = Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_16_BIT, divider_num);
                if (status == CY_SYSCLK_SUCCESS) {
                    obj->div_type = CY_SYSCLK_DIV_16_BIT;
                    obj->div_num = divider_num;
                }
            } else {
                error("Serial: cannot assign clock divider.");
            }
        }
    } else {
        status = CY_SYSCLK_SUCCESS;
    }

    if (status == CY_SYSCLK_SUCCESS) {
        /* Set baud rate */
        if (obj->div_type == CY_SYSCLK_DIV_16_5_BIT) {
            Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_5_BIT, obj->div_num);
            uint32_t divider = divider_value(baudrate * UART_OVERSAMPLE, 5);
            status = Cy_SysClk_PeriphSetFracDivider(CY_SYSCLK_DIV_16_5_BIT,
                                                    obj->div_num,
                                                    (divider >> 5) - 1, // integral part
                                                    divider & 0x1F);    // fractional part
            Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_5_BIT, obj->div_num);
        } else if (obj->div_type == CY_SYSCLK_DIV_16_BIT) {
            Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, obj->div_num);
            status = Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT,
                                                obj->div_num,
                                                divider_value(baudrate * UART_OVERSAMPLE, 0));
            Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, obj->div_num);
        }
    }
    return status;
}

static void serial_deinit_clock(serial_obj_t *obj)
{
    Cy_SysClk_PeriphDisableDivider(obj->div_type, obj->div_num);
    cy_clk_free_divider(obj->div_type, obj->div_num);
}

/*
 * Initializes i/o pins for UART tx/rx.
 */
static void serial_init_pins(serial_obj_t *obj)
{
    int tx_function = pinmap_function(obj->pin_tx, PinMap_UART_TX);
    int rx_function = pinmap_function(obj->pin_rx, PinMap_UART_RX);
    if (cy_reserve_io_pin(obj->pin_tx) || cy_reserve_io_pin(obj->pin_rx)) {
        error("Serial TX/RX pin reservation conflict.");
    }
    pin_function(obj->pin_tx, tx_function);
    pin_function(obj->pin_rx, rx_function);
}

/*
 * Initializes i/o pins for UART flow control.
 */
static void serial_init_flow_pins(serial_obj_t *obj)
{
    if (obj->pin_rts != NC) {
        int rts_function = pinmap_function(obj->pin_rts, PinMap_UART_RTS);
        if (cy_reserve_io_pin(obj->pin_rts)) {
            error("Serial RTS pin reservation conflict.");
        }
        pin_function(obj->pin_rts, rts_function);
    }

    if (obj->pin_cts != NC) {
        int cts_function = pinmap_function(obj->pin_cts, PinMap_UART_CTS);
        if (cy_reserve_io_pin(obj->pin_cts)) {
            error("Serial CTS pin reservation conflict.");
        }
        pin_function(obj->pin_cts, cts_function);
    }
}

static void serial_deinit_pins(serial_obj_t *obj)
{
    if (obj->pin_tx != NC) {
        pin_function(obj->pin_tx, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullDown, PIN_INPUT));
    }
    if (obj->pin_rx != NC) {
        pin_function(obj->pin_rx, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullDown, PIN_INPUT));
    }
    if (obj->pin_rts != NC) {
        pin_function(obj->pin_rts, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullDown, PIN_INPUT));
    }
    if (obj->pin_cts != NC) {
        pin_function(obj->pin_cts, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullDown, PIN_INPUT));
    }
}

/*
 * Initializes and enables UART/SCB.
 */
static void serial_init_peripheral(serial_obj_t *obj)
{
    cy_stc_scb_uart_config_t uart_config = default_uart_config;

    uart_config.dataWidth   = obj->data_width;
    uart_config.parity      = obj->parity;
    uart_config.stopBits    = obj->stop_bits;
    uart_config.enableCts   = (obj->pin_cts != NC);

    Cy_SCB_UART_Init(obj->base, &uart_config, NULL);
    Cy_SCB_UART_Enable(obj->base);
}

#if DEVICE_SLEEP && DEVICE_LPTICKER && SERIAL_PM_CALLBACK_ENABLED
static cy_en_syspm_status_t serial_pm_callback(cy_stc_syspm_callback_params_t *params)
{
    serial_obj_t *obj = (serial_obj_t *)params->context;
    cy_en_syspm_status_t status = CY_SYSPM_FAIL;

    switch (params->mode) {
        case CY_SYSPM_CHECK_READY:
            /* If all data elements are transmitted from the TX FIFO and
            * shifter and the RX FIFO is empty: the UART is ready to enter
            * Deep Sleep mode.
            */
            if (Cy_SCB_UART_IsTxComplete(obj->base)) {
                if (0UL == Cy_SCB_UART_GetNumInRxFifo(obj->base)) {
                    /* Disable the UART. The transmitter stops driving the
                    * lines and the receiver stops receiving data until
                    * the UART is enabled.
                    * This happens when the device failed to enter Deep
                    * Sleep or it is awaken from Deep Sleep mode.
                    */
                    Cy_SCB_UART_Disable(obj->base, NULL);
                    status = CY_SYSPM_SUCCESS;
                }
            }
            break;


        case CY_SYSPM_CHECK_FAIL:
            /* Enable the UART to operate */
            Cy_SCB_UART_Enable(obj->base);
            status = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_BEFORE_TRANSITION:
            status = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_AFTER_TRANSITION:
            /* Enable the UART to operate */
            Cy_SCB_UART_Enable(obj->base);
            status = CY_SYSPM_SUCCESS;
            break;

        default:
            break;
    }
    return status;
}
#endif // DEVICE_SLEEP && DEVICE_LPTICKER

void serial_init(serial_t *obj_in, PinName tx, PinName rx)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    bool is_stdio = (tx == CY_STDIO_UART_TX) || (rx == CY_STDIO_UART_RX);

    if (is_stdio && stdio_uart_inited) {
        memcpy(obj_in, &stdio_uart, sizeof(serial_t));
        return;
    }
    {
        uint32_t uart = pinmap_peripheral(tx, PinMap_UART_TX);
        uart = pinmap_merge(uart, pinmap_peripheral(rx, PinMap_UART_RX));
        if (uart != (uint32_t)NC) {
            obj->base = (CySCB_Type*)uart;
            obj->serial_id = ((UARTName)uart - UART_0) / (UART_1 - UART_0);
            obj->pin_tx = tx;
            obj->pin_rx = rx;
            obj->clock = CY_PIN_CLOCK(pinmap_function(tx, PinMap_UART_TX));
            obj->div_num = CY_INVALID_DIVIDER;
            obj->data_width = 8;
            obj->stop_bits = CY_SCB_UART_STOP_BITS_1;
            obj->parity = CY_SCB_UART_PARITY_NONE;
            obj->pin_rts = NC;
            obj->pin_cts = NC;

            serial_init_clock(obj, UART_DEFAULT_BAUDRATE);
            serial_init_peripheral(obj);
            //Cy_GPIO_Write(Cy_GPIO_PortToAddr(CY_PORT(P13_6)), CY_PIN(P13_6), 1);
            serial_init_pins(obj);
            //Cy_GPIO_Write(Cy_GPIO_PortToAddr(CY_PORT(P13_6)), CY_PIN(P13_6), 0);
#if DEVICE_SLEEP && DEVICE_LPTICKER && SERIAL_PM_CALLBACK_ENABLED
            obj->pm_callback_handler.callback = serial_pm_callback;
            obj->pm_callback_handler.type = CY_SYSPM_DEEPSLEEP;
            obj->pm_callback_handler.skipMode = 0;
            obj->pm_callback_handler.callbackParams = &obj->pm_callback_params;
            obj->pm_callback_params.base = obj->base;
            obj->pm_callback_params.context = obj;
            if (!Cy_SysPm_RegisterCallback(&obj->pm_callback_handler)) {
                error("PM callback registration failed!");
            }
#endif // DEVICE_SLEEP && DEVICE_LPTICKER
            if (is_stdio) {
                memcpy(&stdio_uart, obj_in, sizeof(serial_t));
                stdio_uart_inited = true;
            }
        } else {
            error("Serial pinout mismatch. Requested pins Rx and Tx can't be used for the same Serial communication.");
        }
    }
}

void serial_free(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    bool is_stdio = (obj->pin_tx == CY_STDIO_UART_TX) || (obj->pin_rx == CY_STDIO_UART_RX);

    if (is_stdio && stdio_uart_inited) {
        /* stdio_uart just can't be released */
        return;
    }

    Cy_SCB_UART_Disable(obj->base, NULL);
    Cy_SysPm_UnregisterCallback(&obj->pm_callback_handler);
    serial_irq_release_channel(obj);
    serial_deinit_pins(obj);
    Cy_SCB_UART_DeInit(obj->base);
    serial_deinit_clock(obj);
}

void serial_baud(serial_t *obj_in, int baudrate)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    Cy_SCB_UART_Disable(obj->base, NULL);
    serial_init_clock(obj, baudrate);
    Cy_SCB_UART_Enable(obj->base);
}

void serial_format(serial_t *obj_in, int data_bits, SerialParity parity, int stop_bits)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    if ((data_bits >= 5) && (data_bits <= 9)) {
        obj->data_width = data_bits;
    }

    switch (parity) {
        case ParityNone:
            obj->parity = CY_SCB_UART_PARITY_NONE;
            break;
        case ParityOdd:
            obj->parity = CY_SCB_UART_PARITY_ODD;
            break;
        case ParityEven:
            obj->parity = CY_SCB_UART_PARITY_EVEN;
            break;
        case ParityForced1:
        case ParityForced0:
            MBED_ASSERT("Serial parity mode not supported!");
            break;
    }

    switch (stop_bits) {
        case 1:
            obj->stop_bits = CY_SCB_UART_STOP_BITS_1;
            break;
        case 2:
            obj->stop_bits = CY_SCB_UART_STOP_BITS_2;
            break;
        case 3:
            obj->stop_bits = CY_SCB_UART_STOP_BITS_3;
            break;
        case 4:
            obj->stop_bits = CY_SCB_UART_STOP_BITS_4;
            break;
    }

    Cy_SCB_UART_Disable(obj->base, NULL);
    serial_init_peripheral(obj);
}

void serial_putc(serial_t *obj_in, int c)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    while (!serial_writable(obj_in)) {
        // empty
    }
    Cy_SCB_UART_Put(obj->base, c);
}

int serial_getc(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    while (!serial_readable(obj_in)) {
        // empty
    }
    return Cy_SCB_UART_Get(obj->base);
}

int serial_readable(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    return Cy_SCB_GetNumInRxFifo(obj->base) != 0;
}

int serial_writable(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    return Cy_SCB_GetNumInTxFifo(obj->base) != Cy_SCB_GetFifoSize(obj->base);
}

void serial_clear(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    Cy_SCB_UART_Disable(obj->base, NULL);
    Cy_SCB_ClearTxFifo(obj->base);
    Cy_SCB_ClearRxFifo(obj->base);
    serial_init_peripheral(obj);
}

void serial_break_set(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    /* Cypress SCB does not support transmitting break directly.
     * We emulate functionality by switching TX pin to GPIO mode.
     */
    GPIO_PRT_Type *port_tx = Cy_GPIO_PortToAddr(CY_PORT(obj->pin_tx));
    Cy_GPIO_Pin_FastInit(port_tx, CY_PIN(obj->pin_tx), CY_GPIO_DM_STRONG_IN_OFF, 0, HSIOM_SEL_GPIO);
    Cy_GPIO_Write(port_tx, CY_PIN(obj->pin_tx), 0);
}

void serial_break_clear(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    /* Connect TX pin back to SCB, see a comment in serial_break_set() above */
    GPIO_PRT_Type *port_tx = Cy_GPIO_PortToAddr(CY_PORT(obj->pin_tx));
    int tx_function = pinmap_function(obj->pin_tx, PinMap_UART_TX);
    Cy_GPIO_Pin_FastInit(port_tx, CY_PIN(obj->pin_tx), CY_GPIO_DM_STRONG_IN_OFF, 0, CY_PIN_HSIOM(tx_function));
}

void serial_set_flow_control(serial_t *obj_in, FlowControl type, PinName rxflow, PinName txflow)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    Cy_SCB_UART_Disable(obj->base, NULL);

    switch (type) {
        case FlowControlNone:
            obj->pin_rts = NC;
            obj->pin_cts = NC;
            break;
        case FlowControlRTS:
            obj->pin_rts = rxflow;
            obj->pin_cts = NC;
            break;
        case FlowControlCTS:
            obj->pin_rts = NC;
            obj->pin_cts = txflow;
            break;
        case FlowControlRTSCTS:
            obj->pin_rts = rxflow;
            obj->pin_cts = txflow;
            break;
    }

    serial_init_peripheral(obj);
    serial_init_flow_pins(obj);
}

const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}

const PinMap *serial_rx_pinmap()
{
    return PinMap_UART_RX;
}

const PinMap *serial_cts_pinmap()
{
    return PinMap_UART_CTS;
}

const PinMap *serial_rts_pinmap()
{
    return PinMap_UART_RTS;
}

#if DEVICE_SERIAL_ASYNCH

void serial_irq_handler(serial_t *obj_in, uart_irq_handler handler, uint32_t id)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    irq_info_t *info = &irq_info[obj->serial_id];

    if (info->irqn != unconnected_IRQn) {
        NVIC_DisableIRQ(info->irqn);
    }
    info->handler = handler;
    info->id_arg = id;
    serial_irq_setup_channel(obj);
}

void serial_irq_set(serial_t *obj_in, SerialIrq irq, uint32_t enable)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    switch (irq) {
        case RxIrq:
            if (enable) {
                Cy_SCB_SetRxInterruptMask(obj->base,  CY_SCB_RX_INTR_NOT_EMPTY);
            } else {
                Cy_SCB_SetRxInterruptMask(obj->base, 0);
            }
            break;
        case TxIrq:
            if (enable) {
                Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_TX_INTR_LEVEL | CY_SCB_UART_TX_DONE);
            } else {
                Cy_SCB_SetTxInterruptMask(obj->base, 0);
            }
            break;
    }
}

static void serial_finish_tx_asynch(serial_obj_t *obj)
{
    Cy_SCB_SetTxInterruptMask(obj->base, 0);
    obj->tx_pending = false;
}

static void serial_finish_rx_asynch(serial_obj_t *obj)
{
    Cy_SCB_SetRxInterruptMask(obj->base, 0);
    obj->rx_pending = false;
}

int serial_tx_asynch(serial_t *obj_in, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    const uint8_t *p_buf = tx;

    (void)tx_width; // Obsolete argument
    (void)hint; // At the moment we do not support DAM transfers, so this parameter gets ignored.

    if (obj->tx_pending) {
        return 0;
    }

    obj->tx_events = event;
    obj->async_handler = (cy_israddress)handler;
    if (serial_irq_setup_channel(obj) < 0) {
        return 0;
    }

    // Write as much as possible into the FIFO first.
    while ((tx_length > 0) && Cy_SCB_UART_Put(obj->base, *p_buf)) {
        ++p_buf;
        --tx_length;
    }

    if (tx_length > 0) {
        obj_in->tx_buff.buffer = (void *)p_buf;
        obj_in->tx_buff.length = tx_length;
        obj_in->tx_buff.pos = 0;
        obj->tx_pending = true;
        // Enable interrupts to complete transmission.
        Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_TX_INTR_LEVEL | CY_SCB_UART_TX_DONE);

    } else {
        // Enable interrupt to signal completing of the transmission.
        Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_UART_TX_DONE);
    }
    return tx_length;
}

void serial_rx_asynch(serial_t *obj_in, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    (void)rx_width; // Obsolete argument
    (void)hint; // At the moment we do not support DAM transfers, so this parameter gets ignored.

    if (obj->rx_pending || (rx_length == 0)) {
        return;
    }

    obj_in->char_match = char_match;
    obj_in->char_found = false;
    obj->rx_events = event;
    obj_in->rx_buff.buffer = rx;
    obj_in->rx_buff.length = rx_length;
    obj_in->rx_buff.pos = 0;
    obj->async_handler = (cy_israddress)handler;
    if (serial_irq_setup_channel(obj) < 0) {
        return;
    }
    obj->rx_pending = true;
    // Enable interrupts to start receiving.
    Cy_SCB_SetRxInterruptMask(obj->base, CY_SCB_UART_RX_INTR_MASK & ~CY_SCB_RX_INTR_UART_BREAK_DETECT);
}

uint8_t serial_tx_active(serial_t *obj)
{
    return obj->serial.tx_pending;
}

uint8_t serial_rx_active(serial_t *obj)
{
    return obj->serial.rx_pending;
}

int serial_irq_handler_asynch(serial_t *obj_in)
{
    uint32_t cur_events = 0;
    uint32_t tx_status;
    uint32_t rx_status;

    serial_obj_t *obj = OBJ_P(obj_in);

    rx_status = Cy_SCB_GetRxInterruptStatusMasked(obj->base);

    tx_status = Cy_SCB_GetTxInterruptStatusMasked(obj->base);


    if (tx_status & CY_SCB_TX_INTR_LEVEL) {
        // FIFO has space available for more TX
        uint8_t *ptr = obj_in->tx_buff.buffer;
        ptr += obj_in->tx_buff.pos;
        while ((obj_in->tx_buff.pos < obj_in->tx_buff.length) &&
                Cy_SCB_UART_Put(obj->base, *ptr)) {
            ++ptr;
            ++(obj_in->tx_buff.pos);
        }
        if (obj_in->tx_buff.pos == obj_in->tx_buff.length) {
            // No more bytes to follow; check to see if we need to signal completion.
            if (obj->tx_events & SERIAL_EVENT_TX_COMPLETE) {
                // Disable FIFO interrupt as there are no more bytes to follow.
                Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_UART_TX_DONE);
            } else {
                // Nothing more to do, mark end of transmission.
                serial_finish_tx_asynch(obj);
            }
        }
    }

    if (tx_status & CY_SCB_TX_INTR_UART_DONE) {
        // Mark end of the transmission.
        serial_finish_tx_asynch(obj);
        cur_events |= SERIAL_EVENT_TX_COMPLETE & obj->tx_events;
    }

    Cy_SCB_ClearTxInterrupt(obj->base, tx_status);

    if (rx_status & CY_SCB_RX_INTR_OVERFLOW) {
        cur_events |= SERIAL_EVENT_RX_OVERRUN_ERROR & obj->rx_events;
    }

    if (rx_status & CY_SCB_RX_INTR_UART_FRAME_ERROR) {
        cur_events |= SERIAL_EVENT_RX_FRAMING_ERROR & obj->rx_events;
    }

    if (rx_status & CY_SCB_RX_INTR_UART_PARITY_ERROR) {
        cur_events |= SERIAL_EVENT_RX_PARITY_ERROR & obj->rx_events;
    }

    if (rx_status & CY_SCB_RX_INTR_LEVEL) {
        uint8_t *ptr = obj_in->rx_buff.buffer;
        ptr += obj_in->rx_buff.pos;
        uint32_t fifo_cnt = Cy_SCB_UART_GetNumInRxFifo(obj->base);
        while ((obj_in->rx_buff.pos < obj_in->rx_buff.length) && fifo_cnt) {
            uint32_t c = Cy_SCB_UART_Get(obj->base);
            *ptr++ = (uint8_t)c;
            ++(obj_in->rx_buff.pos);
            --fifo_cnt;
            // Check for character match condition.
            if (obj_in->char_match != SERIAL_RESERVED_CHAR_MATCH) {
                if (c == obj_in->char_match) {
                    obj_in->char_found = true;
                    cur_events |= SERIAL_EVENT_RX_CHARACTER_MATCH & obj->rx_events;
                    // Clamp RX.
                    obj_in->rx_buff.length = obj_in->rx_buff.pos;
                    break;
                }
            }
        }
        if (obj_in->rx_buff.pos == obj_in->rx_buff.length) {
            cur_events |= SERIAL_EVENT_RX_COMPLETE & obj->rx_events;
        }
    }

    // Any event should end operation.
    if (cur_events & SERIAL_EVENT_RX_ALL) {
        serial_finish_rx_asynch(obj);
    }

    Cy_SCB_ClearRxInterrupt(obj->base, rx_status);

    return cur_events;
}

void serial_tx_abort_asynch(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    serial_finish_tx_asynch(obj);
    Cy_SCB_UART_ClearTxFifo(obj->base);
}

void serial_rx_abort_asynch(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    serial_finish_rx_asynch(obj);
    Cy_SCB_UART_ClearRxFifo(obj->base);
}

#endif // DEVICE_SERIAL_ASYNCH

#endif // DEVICE_SERIAL
