/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_api.h"
#include "platform_devices.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/

#define STDIO_UART_NOT_INITED 0
#define STDIO_UART_INITED     1
#define UART_NUMBER           5

struct uart_irq_t {
    uart_irq_handler handler;
    uint32_t id;
};

static const PinMap PinMap_UART_TX[] = {
    {MCC_TX, UART_0, 0},
    {USBTX,  UART_1, 0},
    {SH0_TX, UART_2, ALTERNATE_FUNC},
    {SH1_TX, UART_3, ALTERNATE_FUNC},
    {XB_TX,  UART_4, ALTERNATE_FUNC},
    {NC,     NC,     0}
};

static const PinMap PinMap_UART_RX[] = {
    {MCC_RX, UART_0, 0},
    {USBRX,  UART_1, 0},
    {SH0_RX, UART_2, ALTERNATE_FUNC},
    {SH1_RX, UART_3, ALTERNATE_FUNC},
    {XB_RX,  UART_4, ALTERNATE_FUNC},
    {NC,     NC,     0}
};

/* Handlers registered */
static struct uart_irq_t uart_irq[UART_NUMBER];

/* Global variables needed for mbed */
int stdio_uart_inited = STDIO_UART_NOT_INITED;
serial_t stdio_uart;

/*
 * Fill the serial_obj structure with good elements.
 */
static uint32_t fill_serial_object(struct serial_s *serial_obj, PinName tx,
                                   PinName rx)
{
    UARTName uart_peripheral;

    if (serial_obj == NULL) {
        error("serial_s structure is NULL");
        return 1;
    }

    uart_peripheral = (UARTName)pinmap_merge(
                                         pinmap_peripheral(tx, PinMap_UART_TX),
                                         pinmap_peripheral(rx, PinMap_UART_RX));

    switch (uart_peripheral) {
#ifdef ARM_UART0
    case UART_0:
        serial_obj->uart = &ARM_UART0_DEV;
        serial_obj->index = UART_0;
        serial_obj->irq_number = UART0_IRQn;
        /* Fill stdio_uart global variable with these settings */
        memcpy(&stdio_uart, serial_obj, sizeof(struct serial_s));
        stdio_uart_inited = STDIO_UART_INITED;
        return 0;
#endif /* ARM_UART0 */
#ifdef ARM_UART1
    case UART_1:
        serial_obj->uart = &ARM_UART1_DEV;
        serial_obj->index = UART_1;
        serial_obj->irq_number = UART1_IRQn;
        return 0;
#endif /* ARM_UART1 */
#ifdef ARM_UART2
    case UART_2:
        serial_obj->uart = &ARM_UART2_DEV;
        serial_obj->index = UART_2;
        serial_obj->irq_number = UART2_IRQn;
        return 0;
#endif /* ARM_UART2 */
#ifdef ARM_UART3
    case UART_3:
        serial_obj->uart = &ARM_UART3_DEV;
        serial_obj->index = UART_3;
        serial_obj->irq_number = UART3_IRQn;
        return 0;
#endif /* ARM_UART3 */
#ifdef ARM_UART4
    case UART_4:
        serial_obj->uart = &ARM_UART4_DEV;
        serial_obj->index = UART_4;
        serial_obj->irq_number = UART4_IRQn;
        return 0;
#endif /* ARM_UART4 */
    default:
        error("can not assign a valid UART peripheral to TX and RX pins given");
        return 1;
    }
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    if (fill_serial_object(obj, tx, rx) != 0) {
        return;
    }

    (void)arm_uart_init(obj->uart, SystemCoreClock);

    /*
     * If tx and rx pins are not linked to a GPIO (like for UART0),
     * pin_function will have no effect.
     */
    pin_function(tx, pinmap_function(tx, PinMap_UART_TX));
    pin_function(rx, pinmap_function(rx, PinMap_UART_RX));
}

void serial_free(serial_t *obj)
{
    uart_irq[obj->index].id = 0;
    uart_irq[obj->index].handler = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    if (arm_uart_set_baudrate(obj->uart, (uint32_t)baudrate) !=
        ARM_UART_ERR_NONE) {
        error("Invalid baudrate value or uart not initialized");
    }
}

void serial_format(serial_t *obj, int data_bits,
                   SerialParity parity, int stop_bits)
{
    /*
     *  The CMSDK APB UART is a simple design that supports 8-bit communication
     *  without parity, and is fixed at one stop bit per configuration.
     *  Ref. DDI0479C_cortex_m_system_design_kit_r1p0_trm.pdf
     */
    error("serial format function not supported");
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
#ifdef ARM_UART0
void UART0_IRQHandler()
{
    enum arm_uart_irq_t irq = arm_uart_get_interrupt_status(&ARM_UART0_DEV);
    arm_uart_clear_interrupt(&ARM_UART0_DEV, irq);
    if(uart_irq[UART_0].handler) {
        switch(irq) {
        case ARM_UART_IRQ_RX:
            uart_irq[UART_0].handler(uart_irq[UART_0].id, RxIrq);
            break;
        case ARM_UART_IRQ_TX:
            uart_irq[UART_0].handler(uart_irq[UART_0].id, TxIrq);
            break;
        case ARM_UART_IRQ_COMBINED:
            uart_irq[UART_0].handler(uart_irq[UART_0].id, RxIrq);
            uart_irq[UART_0].handler(uart_irq[UART_0].id, TxIrq);
            break;
        case ARM_UART_IRQ_NONE:
        default:
            break;
        }
    }
}
#endif /* ARM_UART0 */

#ifdef ARM_UART1
void UART1_IRQHandler()
{
    enum arm_uart_irq_t irq = arm_uart_get_interrupt_status(&ARM_UART1_DEV);
    arm_uart_clear_interrupt(&ARM_UART1_DEV, irq);
    if(uart_irq[UART_1].handler) {
        switch(irq) {
        case ARM_UART_IRQ_RX:
            uart_irq[UART_1].handler(uart_irq[UART_1].id, RxIrq);
            break;
        case ARM_UART_IRQ_TX:
            uart_irq[UART_1].handler(uart_irq[UART_1].id, TxIrq);
            break;
        case ARM_UART_IRQ_COMBINED:
            uart_irq[UART_1].handler(uart_irq[UART_1].id, RxIrq);
            uart_irq[UART_1].handler(uart_irq[UART_1].id, TxIrq);
            break;
        case ARM_UART_IRQ_NONE:
        default:
            break;
        }
    }
}
#endif /* ARM_UART1 */

#ifdef ARM_UART2
void UART2_IRQHandler()
{
    enum arm_uart_irq_t irq = arm_uart_get_interrupt_status(&ARM_UART2_DEV);
    arm_uart_clear_interrupt(&ARM_UART2_DEV, irq);
    if(uart_irq[UART_2].handler) {
        switch(irq) {
        case ARM_UART_IRQ_RX:
            uart_irq[UART_2].handler(uart_irq[UART_2].id, RxIrq);
            break;
        case ARM_UART_IRQ_TX:
            uart_irq[UART_2].handler(uart_irq[UART_2].id, TxIrq);
            break;
        case ARM_UART_IRQ_COMBINED:
            uart_irq[UART_2].handler(uart_irq[UART_2].id, RxIrq);
            uart_irq[UART_2].handler(uart_irq[UART_2].id, TxIrq);
            break;
        case ARM_UART_IRQ_NONE:
        default:
            break;
        }
    }
}
#endif /* ARM_UART2 */

#ifdef ARM_UART3
void UART3_IRQHandler()
{
    enum arm_uart_irq_t irq = arm_uart_get_interrupt_status(&ARM_UART3_DEV);
    arm_uart_clear_interrupt(&ARM_UART3_DEV, irq);
    if(uart_irq[UART_3].handler) {
        switch(irq) {
        case ARM_UART_IRQ_RX:
            uart_irq[UART_3].handler(uart_irq[UART_3].id, RxIrq);
            break;
        case ARM_UART_IRQ_TX:
            uart_irq[UART_3].handler(uart_irq[UART_3].id, TxIrq);
            break;
        case ARM_UART_IRQ_COMBINED:
            uart_irq[UART_3].handler(uart_irq[UART_3].id, RxIrq);
            uart_irq[UART_3].handler(uart_irq[UART_3].id, TxIrq);
            break;
        case ARM_UART_IRQ_NONE:
        default:
            break;
        }
    }
}
#endif /* ARM_UART3 */

#ifdef ARM_UART4
void UART4_IRQHandler()
{
    enum arm_uart_irq_t irq = arm_uart_get_interrupt_status(&ARM_UART4_DEV);
    arm_uart_clear_interrupt(&ARM_UART4_DEV, irq);
    if(uart_irq[UART_4].handler) {
        switch(irq) {
        case ARM_UART_IRQ_RX:
            uart_irq[UART_4].handler(uart_irq[UART_4].id, RxIrq);
            break;
        case ARM_UART_IRQ_TX:
            uart_irq[UART_4].handler(uart_irq[UART_4].id, TxIrq);
            break;
        case ARM_UART_IRQ_COMBINED:
            uart_irq[UART_4].handler(uart_irq[UART_4].id, RxIrq);
            uart_irq[UART_4].handler(uart_irq[UART_4].id, TxIrq);
            break;
        case ARM_UART_IRQ_NONE:
        default:
            break;
        }
    }
}
#endif /* ARM_UART4 */

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    uart_irq[obj->index].handler = handler;
    uart_irq[obj->index].id = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    switch (irq) {
    case RxIrq:
        if (enable) {
            NVIC_EnableIRQ(obj->irq_number);
            (void)arm_uart_irq_rx_enable(obj->uart);
        } else {
            arm_uart_irq_rx_disable(obj->uart);
            NVIC_DisableIRQ(obj->irq_number);
        }
        break;
    case TxIrq:
        if (enable) {
            NVIC_EnableIRQ(obj->irq_number);
            (void)arm_uart_irq_tx_enable(obj->uart);
        } else {
            arm_uart_irq_tx_disable(obj->uart);
            NVIC_DisableIRQ(obj->irq_number);
        }
        break;
    }
    /* default: not added to force to cover all enumeration cases */
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    uint8_t byte = 0;

    while (!serial_readable(obj)){};
    (void)arm_uart_read(obj->uart, &byte);

    return (int)byte;
}

void serial_putc(serial_t *obj, int c)
{
    while (!serial_writable(obj)){};
    (void)arm_uart_write(obj->uart, (int)c);
}

int serial_readable(serial_t *obj)
{
    return arm_uart_rx_ready(obj->uart);
}

int serial_writable(serial_t *obj)
{
    return arm_uart_tx_ready(obj->uart);
}

void serial_clear(serial_t *obj)
{
    (void)arm_uart_write(obj->uart, 0x00);
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    /*
     *  The CMSDK APB UART doesn't support serial break.
     *  Ref. DDI0479C_cortex_m_system_design_kit_r1p0_trm.pdf
     */
    error("serial_break_set function not supported");
}

void serial_break_clear(serial_t *obj)
{
    /*
     *  The CMSDK APB UART doesn't support serial break.
     *  Ref. DDI0479C_cortex_m_system_design_kit_r1p0_trm.pdf
     */
    error("serial_break_clear function not supported");
}
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow,
                                PinName txflow)
{
    /*
     *  The CMSDK APB UART doesn't have support for flow control.
     *  Ref. DDI0479C_cortex_m_system_design_kit_r1p0_trm.pdf
     */
    error("serial_set_flow_control function not supported");
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
#if !DEVICE_SERIAL_FC
    static const PinMap PinMap_UART_CTS[] = {
        {NC, NC, 0}
    };
#endif

    return PinMap_UART_CTS;
}

const PinMap *serial_rts_pinmap()
{
#if !DEVICE_SERIAL_FC
    static const PinMap PinMap_UART_RTS[] = {
        {NC, NC, 0}
    };
#endif

    return PinMap_UART_RTS;
}
