/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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
#include "cycfg_pins.h"

#define UART_OVERSAMPLE                 12
#define UART_DEFAULT_BAUDRATE           115200
#define NUM_SERIAL_PORTS                8
#define SERIAL_DEFAULT_IRQ_PRIORITY     3

#define UART_RX_INTR_MASK   (CY_SCB_UART_RX_TRIGGER   | CY_SCB_UART_RX_OVERFLOW  | \
                             CY_SCB_UART_RX_ERR_FRAME | CY_SCB_UART_RX_ERR_PARITY)

#ifdef MBED_TICKLESS
#define SERIAL_PM_CALLBACK_ENABLED 1
#else
#define SERIAL_PM_CALLBACK_ENABLED 0
#endif

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

/* Default UART configuration */
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

    .rxFifoTriggerLevel         = 0UL,  /* Level triggers when at least one element is in FIFO */
    .rxFifoIntEnableMask        = 0x0UL,

    .txFifoTriggerLevel         = 63UL, /* Level triggers when half-fifo is half empty */
    .txFifoIntEnableMask        = 0x0UL
};

/* STDIO serial information  */
int stdio_uart_inited = 0;
serial_t stdio_uart;

int bt_uart_inited = false;
serial_t bt_uart;

typedef struct irq_info_s {
    serial_obj_t        *serial_obj;
    uart_irq_handler    handler;
    uint32_t            id_arg;
    IRQn_Type           irqn;
#if defined (TARGET_MCU_PSOC6_M0)
    cy_en_intr_t        cm0p_irq_src;
#endif
} irq_info_t;


/* Allocate interrupt information table */
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


/** Routes interrupt to proper SCB block.
 *
 * @param serial_id     The ID of serial object
 */
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

    if (Cy_SCB_GetTxInterruptStatusMasked(obj->base) & CY_SCB_UART_TX_EMPTY) {
        info->handler(info->id_arg, TxIrq);
        Cy_SCB_ClearTxInterrupt(obj->base, CY_SCB_UART_TX_EMPTY);
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

/* Interrupts table */
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
#endif /* (TARGET_MCU_PSOC6_M0) */
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

        /* Configure NVIC */
        irq_config.intrPriority = SERIAL_DEFAULT_IRQ_PRIORITY;
        irq_config.intrSrc = irqn;
#if defined (TARGET_MCU_PSOC6_M0)
        irq_config.cm0pSrc = info->cm0p_irq_src;
#endif
        if (Cy_SysInt_Init(&irq_config, irq_dispatcher_table[obj->serial_id]) != CY_SYSINT_SUCCESS) {
            return (-1);
        }

        info->irqn = irqn;
        info->serial_obj = obj;
    }

    /* Enable interrupt after successful setup */
    NVIC_EnableIRQ(info->irqn);

    return 0;
}


/** Calculates fractional divider value.
 *
 * @param frequency The desired frequency of UART clock
 * @param frac_bits The number of fractional bits in the divider
 * @return The divider value
 */
static uint32_t divider_value(uint32_t frequency, uint32_t frac_bits)
{
    /* UARTs use peripheral clock */
    return ((cy_PeriClkFreqHz * (1 << frac_bits)) + (frequency / 2)) / frequency;
}


#define FRACT_DIV_INT(divider)      (((divider) >> 5U) - 1U)
#define FRACT_DIV_FARCT(divider)    ((divider) & 0x1FU)

/** Finds clock divider, configures it and connects to SCB block.
 *
 * @param obj      The serial object
 * @param baudrate The desired UART baud rate
 * @return CY_SYSCLK_SUCCESS if operation successful and CY_SYSCLK_BAD_PARAM otherwise
 */
static cy_en_sysclk_status_t serial_init_clock(serial_obj_t *obj, uint32_t baudrate)
{
    cy_en_sysclk_status_t status = CY_SYSCLK_BAD_PARAM;

    if (obj->div_num == CY_INVALID_DIVIDER) {
        uint32_t divider_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_16_5_BIT);

        if (divider_num < PERI_DIV_16_5_NR) {
            /* Assign fractional divider */
            status = Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_16_5_BIT, divider_num);
            if (status == CY_SYSCLK_SUCCESS) {
                obj->div_type = CY_SYSCLK_DIV_16_5_BIT;
                obj->div_num  = divider_num;
            }
        } else {
            // Try 16-bit divider.
            divider_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_16_BIT);
            if (divider_num < PERI_DIV_16_NR) {
                /* Assign 16-bit divider */
                status = Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_16_BIT, divider_num);
                if (status == CY_SYSCLK_SUCCESS) {
                    obj->div_type = CY_SYSCLK_DIV_16_BIT;
                    obj->div_num  = divider_num;
                }
            } else {
                error("Serial: cannot assign clock divider.");
            }
        }
    } else {
        /* Divider already allocated and connected to the SCB block */
        status = CY_SYSCLK_SUCCESS;
    }

    if (status == CY_SYSCLK_SUCCESS) {
        Cy_SysClk_PeriphDisableDivider(obj->div_type, obj->div_num);

        /* Set baud rate */
        if ((obj->div_type == CY_SYSCLK_DIV_16_5_BIT) || (obj->div_type == CY_SYSCLK_DIV_24_5_BIT)) {
            /* Get fractional divider */
            uint32_t divider = divider_value(baudrate * UART_OVERSAMPLE, 5U);

            status = Cy_SysClk_PeriphSetFracDivider(obj->div_type,
                                                    obj->div_num,
                                                    FRACT_DIV_INT(divider),
                                                    FRACT_DIV_FARCT(divider));
        } else {
            /* Get integer divider */
            status = Cy_SysClk_PeriphSetDivider(obj->div_type,
                                                obj->div_num,
                                                divider_value(baudrate * UART_OVERSAMPLE, 0));
        }

        Cy_SysClk_PeriphEnableDivider(obj->div_type, obj->div_num);
    }

    return status;
}


/** Initializes i/o pins for UART tx/rx.
 *
 * @param obj The serial object
 */
static void serial_init_pins(serial_obj_t *obj)
{
    if ((cy_reserve_io_pin(obj->pin_tx) || cy_reserve_io_pin(obj->pin_rx))
            && !obj->already_reserved) {
        error("Serial TX/RX pin reservation conflict.");
    }

    pin_function(obj->pin_tx, pinmap_function(obj->pin_tx, PinMap_UART_TX));
    pin_function(obj->pin_rx, pinmap_function(obj->pin_rx, PinMap_UART_RX));
}


/** Initializes i/o pins for UART flow control
 *
 * @param obj The serial object
 */
static void serial_init_flow_pins(serial_obj_t *obj)
{
    if (obj->pin_rts != NC) {
        if ((0 != cy_reserve_io_pin(obj->pin_rts)) && !obj->already_reserved) {
            error("Serial RTS pin reservation conflict.");
        }
        pin_function(obj->pin_rts, pinmap_function(obj->pin_rts, PinMap_UART_RTS));
    }

    if (obj->pin_cts != NC) {
        if ((0 != cy_reserve_io_pin(obj->pin_cts)) && !obj->already_reserved) {
            error("Serial CTS pin reservation conflict.");
        }
        pin_function(obj->pin_cts, pinmap_function(obj->pin_cts, PinMap_UART_CTS));
    }
}


/** Initializes and enables UART/SCB.
 *
 * @param obj The serial object
 */
static void serial_init_peripheral(serial_obj_t *obj)
{
    cy_stc_scb_uart_config_t uart_config = default_uart_config;

    uart_config.dataWidth = obj->data_width;
    uart_config.parity    = obj->parity;
    uart_config.stopBits  = obj->stop_bits;
    uart_config.enableCts = (obj->pin_cts != NC);

    Cy_SCB_UART_Init(obj->base, &uart_config, NULL);
    Cy_SCB_UART_Enable(obj->base);
}

/*
 * Callback function to handle into and out of deep sleep state transitions.
 */
#if DEVICE_SLEEP && DEVICE_LPTICKER && SERIAL_PM_CALLBACK_ENABLED
static cy_en_syspm_status_t serial_pm_callback(cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode)
{
    serial_obj_t *obj = (serial_obj_t *) callbackParams->context;
    cy_en_syspm_status_t status = CY_SYSPM_FAIL;

    switch (mode) {
        case CY_SYSPM_CHECK_READY:
            /* If all data elements are transmitted from the TX FIFO and
            * shifter and the RX FIFO is empty: the UART is ready to enter
            * Deep Sleep mode.
            */
            if (Cy_SCB_UART_IsTxComplete(obj->base)) {
                if (0 == Cy_SCB_UART_GetNumInRxFifo(obj->base)) {
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
#endif /* DEVICE_SLEEP && DEVICE_LPTICKER && SERIAL_PM_CALLBACK_ENABLED */

void serial_init(serial_t *obj_in, PinName tx, PinName rx)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    bool is_stdio = (tx == CY_STDIO_UART_TX) || (rx == CY_STDIO_UART_RX);
#if !defined(TARGET_CY8CKIT_062_BLE)
    bool is_bt = (tx == CY_BT_UART_TX) || (rx == CY_BT_UART_RX);
#else
    bool is_bt = false;
#endif

    if (is_stdio && stdio_uart_inited) {
        memcpy(obj_in, &stdio_uart, sizeof(serial_t));
        return;
    } else if (is_bt && bt_uart_inited) {
        memcpy(obj_in, &bt_uart, sizeof(serial_t));
        return;
    } else {
        uint32_t uart = pinmap_peripheral(tx, PinMap_UART_TX);
        uart = pinmap_merge(uart, pinmap_peripheral(rx, PinMap_UART_RX));

        if (uart != (uint32_t)NC) {
            uint32_t  serial_id = ((UARTName)uart - UART_0) / (UART_1 - UART_0);

            /* Initialize configuration */
            obj->base      = (CySCB_Type *)uart;
            obj->serial_id = serial_id;
            obj->clock     = CY_PIN_CLOCK(pinmap_function(tx, PinMap_UART_TX));
            obj->div_num   = CY_INVALID_DIVIDER;
            obj->already_reserved = (0 != cy_reserve_scb(obj->serial_id));

            obj->pin_tx    = tx;
            obj->pin_rx    = rx;
            obj->pin_rts   = NC;
            obj->pin_cts   = NC;

            obj->data_width = 8;
            obj->stop_bits  = CY_SCB_UART_STOP_BITS_1;
            obj->parity     = CY_SCB_UART_PARITY_NONE;

            /* Check if resource severed */
            if (obj->already_reserved) {
                uint32_t map;

                /* SCB pins and clocks are connected */

                /* Disable block and get it into the default state */
                Cy_SCB_UART_Disable(obj->base, NULL);
                Cy_SCB_UART_DeInit(obj->base);

                /* Get connected clock */
                map = Cy_SysClk_PeriphGetAssignedDivider(obj->clock);
                obj->div_num  = _FLD2VAL(CY_PERI_CLOCK_CTL_DIV_SEL,  map);
                obj->div_type = (cy_en_divider_types_t) _FLD2VAL(CY_PERI_CLOCK_CTL_TYPE_SEL, map);
            } else {
#if DEVICE_SLEEP && DEVICE_LPTICKER && SERIAL_PM_CALLBACK_ENABLED
                /* Register callback once */
                obj->pm_callback_handler.callback = serial_pm_callback;
                obj->pm_callback_handler.type     = CY_SYSPM_DEEPSLEEP;
                obj->pm_callback_handler.skipMode = 0;
                obj->pm_callback_handler.callbackParams = &obj->pm_callback_params;
                obj->pm_callback_params.base    = obj->base;
                obj->pm_callback_params.context = obj;

                if (!Cy_SysPm_RegisterCallback(&obj->pm_callback_handler)) {
                    error("PM callback registration failed!");
                }
#endif /* DEVICE_SLEEP && DEVICE_LPTICKER && SERIAL_PM_CALLBACK_ENABLED */
            }

            /* Configure hardware resources */
            serial_init_clock(obj, UART_DEFAULT_BAUDRATE);
            serial_init_pins(obj);
            serial_init_peripheral(obj);

            if (is_stdio) {
                memcpy(&stdio_uart, obj_in, sizeof(serial_t));
                stdio_uart_inited = 1;
            } else if (is_bt) {
                memcpy(&bt_uart, obj_in, sizeof(serial_t));
                bt_uart_inited = true;
            }
        } else {
            error("Serial pinout mismatch. Requested pins Rx and Tx can't be used for the same Serial communication.");
        }
    }
}


void serial_free(serial_t *obj)
{
    error("This function is not supported.");
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
            error("Serial parity mode not supported!");
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
    /* SCB is enabled at the and of serial_init_peripheral() call */
}


void serial_putc(serial_t *obj_in, int c)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    while (0 == serial_writable(obj_in)) {
        /* There is an entry to be written */
    }

    Cy_SCB_WriteTxFifo(obj->base, (uint32_t) c);
}

int serial_getc(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    while (0 == serial_readable(obj_in)) {
        /* There is an item to be read */
    }

    return Cy_SCB_UART_Get(obj->base);
}


int serial_readable(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    return Cy_SCB_GetNumInRxFifo(obj->base);
}


int serial_writable(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);
    return (Cy_SCB_GetFifoSize(obj->base) != Cy_SCB_GetNumInTxFifo(obj->base));
}


void serial_clear(serial_t *obj_in)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    /* The hardware FIFOs and statuses are cleared when SCB is disabled */
    Cy_SCB_UART_Disable(obj->base, NULL);
    serial_init_peripheral(obj);
    /* SCB is enabled at the and of serial_init_peripheral() call */
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


void serial_pinout_tx(PinName tx)
{
    error("This function is not supported.");
}


void serial_set_flow_control(serial_t *obj_in, FlowControl type, PinName rxflow, PinName txflow)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    /* Do not perform pins reservation second time for the same pins */
    if ((obj->pin_rts == rxflow) && (obj->pin_cts == txflow)) {
        return;
    }

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

    serial_init_flow_pins(obj);
    serial_init_peripheral(obj);
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
        /* Seccessful serial_irq_setup_channel enables interrupt in NVIC */
        NVIC_DisableIRQ(info->irqn);
    }

    info->handler = handler;
    info->id_arg = id;

    if (0 != serial_irq_setup_channel(obj)) {
        error("Interrupt setup failed.");
    }
}


void serial_irq_set(serial_t *obj_in, SerialIrq irq, uint32_t enable)
{
    serial_obj_t *obj = OBJ_P(obj_in);

    switch (irq) {
        case RxIrq: /* RxIrq for receive (buffer is not empty) */
            Cy_SCB_SetRxInterruptMask(obj->base, (0 != enable) ? CY_SCB_RX_INTR_NOT_EMPTY : 0);
            break;

        case TxIrq: /* TxIrq for transmit (buffer is empty) */
            Cy_SCB_SetTxInterruptMask(obj->base, (0 != enable) ? CY_SCB_UART_TX_EMPTY : 0);
            break;
    }
}


int serial_tx_asynch(serial_t *obj_in, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void) tx_width; /* Deprecated argument */
    (void) hint;     /* At the moment we do not support DMA transfers, so this parameter gets ignored. */

    serial_obj_t *obj = OBJ_P(obj_in);
    const uint8_t *p_buf = tx;

    if (obj->tx_pending || (tx_length == 0)) {
        return 0;
    }

    /* Configure interrupt handler */
    obj->async_handler = (cy_israddress)handler;
    if (serial_irq_setup_channel(obj) < 0) {
        return 0;
    }

    /* Clear TX_DONE interrupt it might remain set from previous call */
    Cy_SCB_ClearTxInterrupt(obj->base, CY_SCB_UART_TX_DONE | CY_SCB_TX_INTR_LEVEL);

    /* Write as much as possible into the FIFO first */
    while ((tx_length > 0) && (0 != Cy_SCB_UART_Put(obj->base, *p_buf))) {
        ++p_buf;
        --tx_length;
    }

    if (tx_length > 0) {
        obj->tx_pending = true;
        obj->tx_events  = event;
        obj_in->tx_buff.pos    = 0;
        obj_in->tx_buff.buffer = (void *)p_buf;
        obj_in->tx_buff.length = tx_length;

        /* Enable LEVEL interrupt to complete transmission */
        Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_TX_INTR_LEVEL);
    } else {
        /* Enable DONE interrupt to signal completing of the transmission */
        Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_UART_TX_DONE);
    }

    return tx_length;
}


void serial_rx_asynch(serial_t *obj_in, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    (void) rx_width; /* Deprecated argument */
    (void) hint;     /* At the moment we do not support DMA transfers, so this parameter gets ignored. */

    serial_obj_t *obj = OBJ_P(obj_in);

    if (obj->rx_pending || (rx_length == 0)) {
        return;
    }

    /* Configure interrupt handler */
    obj->async_handler = (cy_israddress)handler;
    if (serial_irq_setup_channel(obj) < 0) {
        return;
    }

    obj->rx_pending = true;
    obj->rx_events  = event;
    obj_in->char_match = char_match;
    obj_in->char_found = false;
    obj_in->rx_buff.pos    = 0;
    obj_in->rx_buff.buffer = rx;
    obj_in->rx_buff.length = rx_length;

    /* Enable interrupts to start receiving */
    Cy_SCB_SetRxInterruptMask(obj->base, UART_RX_INTR_MASK);
}


uint8_t serial_tx_active(serial_t *obj)
{
    return obj->serial.tx_pending;
}


uint8_t serial_rx_active(serial_t *obj)
{
    return obj->serial.rx_pending;
}


/** Finishes TX asynchronous transfer: disable TX interrupts and clear
 * pending state.
 *
 * @param obj The serial object
 */
static void serial_finish_tx_asynch(serial_obj_t *obj)
{
    Cy_SCB_SetTxInterruptMask(obj->base, 0);
    obj->tx_pending = false;
}


/** Finishes TX asynchronous transfer: disable TX interrupts and clear
 * pending state.
 *
 * @param obj The serial object
 */
static void serial_finish_rx_asynch(serial_obj_t *obj)
{
    Cy_SCB_SetRxInterruptMask(obj->base, 0);
    obj->rx_pending = false;
}


int serial_irq_handler_asynch(serial_t *obj_in)
{
    uint32_t cur_events = 0;
    uint32_t tx_status;
    uint32_t rx_status;

    serial_obj_t *obj = OBJ_P(obj_in);

    /* Interrupt cause is TX */
    if (0 != (CY_SCB_TX_INTR & Cy_SCB_GetInterruptCause(obj->base))) {

        /* Get interrupt sources and clear them */
        tx_status = Cy_SCB_GetTxInterruptStatusMasked(obj->base);
        Cy_SCB_ClearTxInterrupt(obj->base, tx_status);

        if (0 != (tx_status & CY_SCB_TX_INTR_LEVEL)) {

            /* Get current buffer position */
            uint8_t *ptr = obj_in->tx_buff.buffer;
            ptr += obj_in->tx_buff.pos;

            /* FIFO has space available for more TX */
            while ((obj_in->tx_buff.pos < obj_in->tx_buff.length) &&
                    (0 != Cy_SCB_UART_Put(obj->base, *ptr))) {
                ++ptr;
                ++(obj_in->tx_buff.pos);
            }

            if (obj_in->tx_buff.pos == obj_in->tx_buff.length) {
                /* No more bytes to follow; check to see if we need to signal completion */
                if (0 != (obj->tx_events & SERIAL_EVENT_TX_COMPLETE)) {
                    /* Disable TX_LEVEL and enable TX_DONE to get completion event */
                    Cy_SCB_SetTxInterruptMask(obj->base, CY_SCB_UART_TX_DONE);
                } else {
                    /* Nothing more to do, mark end of transmission */
                    serial_finish_tx_asynch(obj);
                }
            }
        }

        if (0 != (tx_status & CY_SCB_TX_INTR_UART_DONE)) {
            /* Mark end of the transmission */
            serial_finish_tx_asynch(obj);
            cur_events |= SERIAL_EVENT_TX_COMPLETE;
        }
    }

    /* Interrupt cause is RX */
    if (0 != (CY_SCB_RX_INTR & Cy_SCB_GetInterruptCause(obj->base))) {

        /* Get interrupt sources and clear them */
        rx_status = Cy_SCB_GetRxInterruptStatusMasked(obj->base);
        Cy_SCB_ClearRxInterrupt(obj->base, rx_status);

        if (0 != (rx_status & CY_SCB_RX_INTR_OVERFLOW)) {
            cur_events |= SERIAL_EVENT_RX_OVERRUN_ERROR;
        }

        if (0 != (rx_status & CY_SCB_RX_INTR_UART_FRAME_ERROR)) {
            cur_events |= SERIAL_EVENT_RX_FRAMING_ERROR;
        }

        if (0 != (rx_status & CY_SCB_RX_INTR_UART_PARITY_ERROR)) {
            cur_events |= SERIAL_EVENT_RX_PARITY_ERROR;
        }

        if (0 != (rx_status & CY_SCB_RX_INTR_LEVEL)) {

            /* Get current buffer position */
            uint8_t *ptr = obj_in->rx_buff.buffer;
            ptr += obj_in->rx_buff.pos;

            /* Get data from the RX FIFO */
            while (obj_in->rx_buff.pos < obj_in->rx_buff.length) {
                uint32_t c = Cy_SCB_UART_Get(obj->base);

                if (c == CY_SCB_UART_RX_NO_DATA) {
                    break;
                }

                *ptr++ = (uint8_t) c;
                ++(obj_in->rx_buff.pos);

                /* Check for character match condition */
                if (obj_in->char_match != SERIAL_RESERVED_CHAR_MATCH) {
                    if (c == obj_in->char_match)  {
                        obj_in->char_found = true;
                        cur_events |= SERIAL_EVENT_RX_CHARACTER_MATCH;

                        /* Clamp RX buffer */
                        obj_in->rx_buff.length = obj_in->rx_buff.pos;
                        break;
                    }
                }
            }
        }

        if (obj_in->rx_buff.pos == obj_in->rx_buff.length) {
            serial_finish_rx_asynch(obj);
            cur_events |= SERIAL_EVENT_RX_COMPLETE;
        }
    }

    return (cur_events & (obj->tx_events | obj->rx_events));
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
    Cy_SCB_ClearRxInterrupt(obj->base, CY_SCB_UART_RX_INTR_MASK);
}

#endif /* DEVICE_SERIAL_ASYNCH */

#endif /* DEVICE_SERIAL */
