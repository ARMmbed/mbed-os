/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_api.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/

static const PinMap PinMap_UART_TX[] = {
    {MCC_TX  , UART_0, 0},
    {USBTX   , UART_1, 0},
    {SH0_TX  , UART_2, ALTERNATE_FUNC},
    {SH1_TX  , UART_3, ALTERNATE_FUNC},
    {XB_TX   , UART_4, ALTERNATE_FUNC},
    {NC      , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {MCC_RX  , UART_0, 0},
    {USBRX   , UART_1, 0},
    {SH0_RX  , UART_2, ALTERNATE_FUNC},
    {SH1_RX  , UART_3, ALTERNATE_FUNC},
    {XB_RX   , UART_4, ALTERNATE_FUNC},
    {NC      , NC    , 0}
};

#define UART_NUM    5

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_global_data_s {
    uint32_t serial_irq_id;
    gpio_t sw_rts, sw_cts;
    uint8_t count, rx_irq_set_flow, rx_irq_set_api;
};

static struct serial_global_data_s uart_data[UART_NUM];

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    uint32_t uart_ctrl = 0;

    /* Determine the UART to use */
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    if ((int)uart == NC) {
        error("Serial pinout mapping failed");
        return;
    }

    obj->uart = (CMSDK_UART_TypeDef *)uart;

    if (tx != NC) {
        uart_ctrl = 0x01;   /* TX enable */
    }
    if (rx != NC) {
        uart_ctrl |= 0x02;  /* RX enable */
    }

    switch (uart) {
        case UART_0:
            CMSDK_UART0->CTRL = uart_ctrl;
            obj->index = 0;
            break;
        case UART_1:
            CMSDK_UART1->CTRL = uart_ctrl;
            obj->index = 1;
            break;
        case UART_2:
            CMSDK_UART2->CTRL = 0;
            obj->index = 2;
            pin_function(tx, ALTERNATE_FUNC);
            pin_function(rx, ALTERNATE_FUNC);
            CMSDK_UART2->CTRL = uart_ctrl;
            break;
        case UART_3:
            CMSDK_UART3->CTRL = 0;
            obj->index = 3;
            pin_function(tx, ALTERNATE_FUNC);
            pin_function(rx, ALTERNATE_FUNC);
            CMSDK_UART3->CTRL = uart_ctrl;
            break;
        case UART_4:
            CMSDK_UART4->CTRL = 0;
            obj->index = 4;
            pin_function(tx, ALTERNATE_FUNC);
            pin_function(rx, ALTERNATE_FUNC);
            CMSDK_UART4->CTRL = uart_ctrl;
            break;
    }

    /* Set default baud rate and format */
    serial_baud(obj, 9600);

    /*
     *  The CMSDK APB UART doesn't have support for flow control.
     *  Ref. DDI0479C_cortex_m_system_design_kit_r1p0_trm.pdf
     */
    uart_data[obj->index].sw_rts.pin = NC;
    uart_data[obj->index].sw_cts.pin = NC;

    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }

    /* Clear UART */
    serial_clear(obj);
}

void serial_free(serial_t *obj)
{
      uart_data[obj->index].serial_irq_id = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    /*
     * The MPS2 has a simple divider to control the baud rate.
     * The formula is:
     * Baudrate = PCLK / BAUDDIV  where  PCLK = SystemCoreClock and
     *                                   BAUDDIV is the desire baudrate
     *
     * So, if the desired baud rate is 9600 the calculation will be:
     * Baudrate = SystemCoreClock / 9600;
     */

    /* Check to see if minimum baud value entered */
    int baudrate_div = 0;

    if (baudrate == 0) {
        error("Invalid baudrate value");
        return;
    }

    baudrate_div = SystemCoreClock / baudrate;

    if (baudrate >= 16) {
        switch ((int)obj->uart) {
            case UART_0:
                CMSDK_UART0->BAUDDIV = baudrate_div;
                break;
            case UART_1:
                CMSDK_UART1->BAUDDIV = baudrate_div;
                break;
            case UART_2:
                CMSDK_UART2->BAUDDIV = baudrate_div;
                break;
            case UART_3:
                CMSDK_UART3->BAUDDIV = baudrate_div;
                break;
            case UART_4:
                CMSDK_UART4->BAUDDIV = baudrate_div;
                break;
            default:
                error("Invalid uart object");
                break;
        }
    } else {
        error("Invalid baudrate value");
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
static inline void uart_irq(uint32_t intstatus, uint32_t index,
                            CMSDK_UART_TypeDef *puart)
{
    SerialIrq irq_type;

    switch (intstatus) {
        case 1:
            irq_type = TxIrq;
            break;
        case 2:
            irq_type = RxIrq;
            break;
        default:
            return;
    }

    if ((RxIrq == irq_type) && (NC != uart_data[index].sw_rts.pin)) {
        gpio_write(&uart_data[index].sw_rts, 1);
        /* Disable interrupt if it wasn't enabled by the application */
        if (!uart_data[index].rx_irq_set_api) {
            /* Disable Rx interrupt */
            puart->CTRL &= ~(CMSDK_UART_CTRL_RXIRQEN_Msk);
        }
    }

    if (uart_data[index].serial_irq_id != 0) {
        if ((irq_type != RxIrq) || (uart_data[index].rx_irq_set_api)) {
            irq_handler(uart_data[index].serial_irq_id, irq_type);
        }
    }

    if (irq_type == TxIrq) {
        /* Clear the TX interrupt Flag */
        puart->INTCLEAR |= 0x01;
    } else {
        /* Clear the Rx interupt Flag */
        puart->INTCLEAR |= 0x02;
    }
}

void uart0_irq()
{
    uart_irq(CMSDK_UART0->INTSTATUS & 0x3, 0, (CMSDK_UART_TypeDef*)CMSDK_UART0);
}

void uart1_irq()
{
    uart_irq(CMSDK_UART1->INTSTATUS & 0x3, 1, (CMSDK_UART_TypeDef*)CMSDK_UART1);
}

void uart2_irq()
{
    uart_irq(CMSDK_UART2->INTSTATUS & 0x3, 2, (CMSDK_UART_TypeDef*)CMSDK_UART2);
}

void uart3_irq() {
    uart_irq(CMSDK_UART3->INTSTATUS & 0x3, 3, (CMSDK_UART_TypeDef*)CMSDK_UART3);
}

void uart4_irq() {
    uart_irq(CMSDK_UART4->INTSTATUS & 0x3, 4, (CMSDK_UART_TypeDef*)CMSDK_UART4);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    uart_data[obj->index].serial_irq_id = id;
}

static void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable)
{

    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    switch ((int)obj->uart) {
        case UART_0:
            irq_n = UART0_IRQn;
            vector = (uint32_t)&uart0_irq;
            break;
        case UART_1:
            irq_n = UART1_IRQn;
            vector = (uint32_t)&uart1_irq;
            break;
        case UART_2:
            irq_n = UART2_IRQn;
            vector  = (uint32_t)&uart2_irq;
            break;
        case UART_3:
            irq_n = UART3_IRQn;
            vector  = (uint32_t)&uart3_irq;
            break;
        case UART_4:
            irq_n = UART4_IRQn;
            vector  = (uint32_t)&uart4_irq;
            break;
    }

    if (enable) {
        if (irq == TxIrq) {
            /* Set TX interrupt enable in CTRL REG */
            obj->uart->CTRL |= CMSDK_UART_CTRL_TXIRQEN_Msk;
        }  else {
            /* Set Rx interrupt on in CTRL REG */
            obj->uart->CTRL |= CMSDK_UART_CTRL_RXIRQEN_Msk;
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    } else if ((irq == TxIrq) ||
               (uart_data[obj->index].rx_irq_set_api
                + uart_data[obj->index].rx_irq_set_flow == 0)) {
         /* Disable IRQ */
        int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);

        obj->uart->CTRL &= ~(1 << (irq + 2));

        all_disabled = (obj->uart->CTRL & (1 << (other_irq + 2))) == 0;

        if (all_disabled) {
            NVIC_DisableIRQ(irq_n);
        }
    }
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    if (RxIrq == irq) {
        uart_data[obj->index].rx_irq_set_api = enable;
    }

    serial_irq_set_internal(obj, irq, enable);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    while (serial_readable(obj) == 0) {
        /* NOP */
    }

    return obj->uart->DATA;
}

void serial_putc(serial_t *obj, int c)
{
    while (serial_writable(obj)) {
        /* NOP */
    }
    obj->uart->DATA = c;
}

int serial_readable(serial_t *obj)
{
    return obj->uart->STATE & 0x2;
}

int serial_writable(serial_t *obj)
{
    return obj->uart->STATE & 0x1;
}

void serial_clear(serial_t *obj)
{
    obj->uart->DATA = 0x00;
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
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    /*
     *  The CMSDK APB UART doesn't have support for flow control.
     *  Ref. DDI0479C_cortex_m_system_design_kit_r1p0_trm.pdf
     */
    error("serial_set_flow_control function not supported");
}

