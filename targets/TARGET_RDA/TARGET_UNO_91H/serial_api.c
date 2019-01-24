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
#include "mbed_assert.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "gpio_api.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/
#define UART_NUM    2

#define UART_CLKGATE_REG        (RDA_SCU->CLKGATE0)

#define UART1_CLKEN_MASK        (0x01UL << 21)
#define RXFIFO_EMPTY_MASK       (0x01UL << 0)
#define TXFIFO_FULL_MASK        (0x01UL << 19)
#define AFCE_MASK               (0x01UL << 5)

static const PinMap PinMap_UART_TX[] = {
    {PA_1,  UART_0, 0},
    {PB_2,  UART_1, 5},
    {PD_3,  UART_1, 2},
    {NC   , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {PA_0,  UART_0, 0},
    {PB_1,  UART_1, 5},
    {PD_2,  UART_1, 2},
    {NC   , NC    , 0}
};

static const PinMap PinMap_UART_RTS[] = {
    {PD_1,  UART_1, 2},
    {NC,    NC,     0}
};

static const PinMap PinMap_UART_CTS[] = {
    {PD_0,  UART_1, 2},
    {NC,    NC,     0}
};

static uart_irq_handler irq_handler[UART_NUM];

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
    int is_stdio_uart = 0;

    // determine the UART to use
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)uart != NC);

    switch (uart) {
        case UART_0:
            obj->index = 0;
            break;
        case UART_1:
            obj->index = 1;
            /* Enable clock-gating */
            UART_CLKGATE_REG |= UART1_CLKEN_MASK;
            break;
    }

    obj->uart = (RDA_UART_TypeDef *)uart;

    // enable fifos and default rx trigger level
    obj->uart->FCR = 0 << 0  //FIFO Enable - 0 = Disables, 1 = Enabled
                   | 0 << 1  // Rx Fifo Reset
                   | 0 << 2  // Tx Fifo Reset
                   | 0 << 6; // Rx irq trigger level - 0 = 1 char, 1 = 4 chars, 2 = 8 chars, 3 = 14 chars

    // disable irqs
    obj->uart->IER = 0 << 0  // Rx Data available irq enable
                   | 0 << 1  // Tx Fifo empty irq enable
                   | 0 << 2; // Rx Line Status irq enable

    obj->uart->MCR =  1 << 8; //select clock
    obj->uart->FRR = 0x2001; //tx_trigger = 0x10, rx_trigger = 0x01

    serial_format(obj, 8, ParityNone, 1);

    // pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    // set rx/tx pins in PullUp mode
    if (tx != NC) {
        pin_mode(tx, PullUp);
    }
    if (rx != NC) {
        pin_mode(rx, PullUp);
    }

    if ((rx != NC) && (tx != NC)) {
        obj->uart->FCR |= 1 << 0;  //enable fifo
    }

    uart_data[obj->index].sw_rts.pin = NC;
    uart_data[obj->index].sw_cts.pin = NC;
    serial_set_flow_control(obj, FlowControlNone, NC, NC);

    is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);

    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
    serial_clear(obj);
}

void serial_free(serial_t *obj)
{
    uart_data[obj->index].serial_irq_id = 0;
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate)
{
    MBED_ASSERT((int)obj->uart <= UART_1);

    uint32_t baud_divisor;
    uint32_t baud_mod;

    baud_divisor  = (AHBBusClock / baudrate) >> 4;
    baud_mod      = (AHBBusClock / baudrate) & 0x0F;

    obj->uart->LCR |= (1 << 7); //enable load devisor register

    obj->uart->DLL = (baud_divisor >> 0) & 0xFF;
    obj->uart->DLH = (baud_divisor >> 8) & 0xFF;
    obj->uart->DL2 = (baud_mod>>1) + ((baud_mod - (baud_mod>>1))<<4);

    obj->uart->LCR &= ~(1 << 7);// after loading, disable load devisor register

}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2)); // 0: 1 stop bits, 1: 2 stop bits
    MBED_ASSERT((data_bits > 4) && (data_bits < 9)); // 0: 5 data bits ... 3: 8 data bits
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven) ||
           (parity == ParityForced1) || (parity == ParityForced0));

    stop_bits -= 1;
    data_bits -= 5;

    int parity_enable, parity_select;
    switch (parity) {
        case ParityNone:
            parity_enable = 0;
            parity_select = 0;
            break;
        case ParityOdd:
            parity_enable = 1;
            parity_select = 0;
            break;
        case ParityEven:
            parity_enable = 1;
            parity_select = 1;
            break;
        case ParityForced1:
            parity_enable = 1;
            parity_select = 2;
            break;
        case ParityForced0:
            parity_enable = 1;
            parity_select = 3;
            break;
        default:
            parity_enable = 0;
            parity_select = 0;
            break;
    }

    obj->uart->LCR = (obj->uart->LCR) &
                     ((~0x3FUL) |
                     (data_bits << 0) |
                     (stop_bits << 2) |
                     (parity_enable << 3) |
                     (parity_select << 4));
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(uint32_t iir, uint32_t index, RDA_UART_TypeDef *puart)
{
    SerialIrq irq_type;
    switch (iir) {
        case 0x02UL: irq_type = TxIrq; break;
        case 0x04UL: irq_type = RxIrq; break;
        case 0x00UL: iir = puart->MSR;
        default: return;
    }

    if ((RxIrq == irq_type) && ((NC != uart_data[index].sw_rts.pin) && ((puart->MCR & AFCE_MASK) == 0x00UL))) {
        gpio_write(&uart_data[index].sw_rts, 1);
        // Disable interrupt if it wasn't enabled by other part of the application
        if (!uart_data[index].rx_irq_set_api)
            puart->IER &= ~(1 << RxIrq);
    }

    if (uart_data[index].serial_irq_id != 0)
        if ((irq_type != RxIrq) || (uart_data[index].rx_irq_set_api))
            (irq_handler[index])(uart_data[index].serial_irq_id, irq_type);
}

void uart0_irq(void)
{
    uart_irq((RDA_UART0->IIR & 0x0FUL), 0, (RDA_UART_TypeDef*)RDA_UART0);
}

void uart1_irq(void)
{
    uart_irq((RDA_UART1->IIR & 0x0FUL), 1, (RDA_UART_TypeDef*)RDA_UART1);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler[obj->index] = handler;
    uart_data[obj->index].serial_irq_id = id;
}

static void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
    switch ((int)obj->uart) {
        case UART_0:
            irq_n=UART0_IRQn;
            vector = (uint32_t)&uart0_irq;
            break;
        case UART_1:
            irq_n=UART1_IRQn;
            vector = (uint32_t)&uart1_irq;
            break;
        default:
            break;
    }

    if (enable) {
        obj->uart->IER |= 1 << irq;
        NVIC_SetVector(irq_n, vector);
        NVIC_SetPriority(irq_n, 0x1FUL);
        NVIC_EnableIRQ(irq_n);
    }
    else if ((TxIrq == irq) ||
        (uart_data[obj->index].rx_irq_set_api + uart_data[obj->index].rx_irq_set_flow == 0)) { // disable
        int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        obj->uart->IER &= ~(1 << irq);
        all_disabled = (obj->uart->IER & (1 << other_irq)) == 0;
        if (all_disabled)
            NVIC_DisableIRQ(irq_n);
    }

}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    if (RxIrq == irq)
        uart_data[obj->index].rx_irq_set_api = enable;
    serial_irq_set_internal(obj, irq, enable);
}

static void serial_flow_irq_set(serial_t *obj, uint32_t enable)
{
    uart_data[obj->index].rx_irq_set_flow = enable;
    serial_irq_set_internal(obj, RxIrq, enable);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    int data = 0;
    while (!serial_readable(obj));
    data = (int)(obj->uart->RBR & 0x00FFUL);

    if (((obj->uart->MCR & AFCE_MASK) == 0x00UL) && (NC != uart_data[obj->index].sw_rts.pin)) {  //enable flow control rx
        gpio_write(&uart_data[obj->index].sw_rts, 0);
        obj->uart->IER |= 1 << RxIrq;
    }
    return data;
}

void serial_putc(serial_t *obj, int c)
{
    while (serial_writable(obj));
    obj->uart->THR = c;
}

int serial_readable(serial_t *obj)
{
    return (obj->uart->LSR & RXFIFO_EMPTY_MASK);
}

int serial_writable(serial_t *obj)
{
    int isWritable = 1;
    if (obj->index == 0) {
        return (obj->uart->FSR & TXFIFO_FULL_MASK);  // uart0 not have flow control
    } else {
        if (((obj->uart->MCR & AFCE_MASK) == 0x00UL) && (NC != uart_data[obj->index].sw_cts.pin))  //If flow control: writable if CTS low + UART done
            isWritable = (gpio_read(&uart_data[obj->index].sw_cts) == 0) && (obj->uart->FSR & TXFIFO_FULL_MASK);
        else
            isWritable = (obj->uart->FSR & TXFIFO_FULL_MASK);
        return isWritable;
    }
}

void serial_clear(serial_t *obj)
{
    obj->uart->FCR = 1 << 0  // FIFO Enable - 0 = Disables, 1 = Enabled
                   | 1 << 1  // rx FIFO reset
                   | 1 << 2; // tx FIFO reset
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    obj->uart->LCR |= (1 << 6);
}

void serial_break_clear(serial_t *obj)
{
    obj->uart->LCR &= ~(1 << 6);
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    // Only UART1 has hardware flow control on RDA5991H
    MBED_ASSERT((rxflow != UART0_RX) && (txflow != UART0_TX));

    RDA_UART_TypeDef *uart1 = (uint32_t)obj->uart == (uint32_t)RDA_UART1 ? RDA_UART1 : NULL;
    int index = obj->index;

    // First, disable flow control completely
    uart_data[index].sw_rts.pin = uart_data[index].sw_cts.pin = NC;
    serial_flow_irq_set(obj, 0);
    if (FlowControlNone == type) {
        RDA_GPIO->IFCTRL &= ~(0x01UL << 2); //disable flow control
        return;
    }

    // Check type(s) of flow control to use
    UARTName uart_rts = (UARTName)pinmap_find_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_find_peripheral(txflow, PinMap_UART_CTS);

    if ((UART_1 == uart_cts) && (NULL != uart1)) {
        pinmap_pinout(txflow, PinMap_UART_CTS);
        gpio_init_in(&uart_data[index].sw_cts, txflow);
    }

    if ((UART_1 == uart_rts) && (NULL != uart1)) {
        pinmap_pinout(rxflow, PinMap_UART_RTS);
        gpio_init_out(&uart_data[index].sw_rts, rxflow);
        serial_flow_irq_set(obj, 1);
    }

    uart1->MCR = uart1->MCR | AFCE_MASK; //enable auto flow control, in this case we don't have to read and write sw_cts & sw_rts
    uart1->FRR = (0x3EUL << 0) | (0x3EUL << 9); //rts/cts fifo trigger
    RDA_GPIO->IFCTRL |= 0x01UL << 2; //enable flow control
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
