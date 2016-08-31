/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include <string.h>
#include "mbed_assert.h"
#include "cmsis.h"
#include "serial_api.h"
#include "gpio_api.h"
#include "uart.h"
#include "uart_regs.h"
#include "ioman_regs.h"
#include "PeripheralPins.h"

#define DEFAULT_BAUD    9600

#define UART_ERRORS     (MXC_F_UART_INTFL_RX_FRAMING_ERR | \
                         MXC_F_UART_INTFL_RX_PARITY_ERR | \
                         MXC_F_UART_INTFL_RX_FIFO_OVERFLOW)

// Variables for managing the stdio UART
int stdio_uart_inited;
serial_t stdio_uart;

// Variables for interrupt driven
static uart_irq_handler irq_handler;
static serial_t *objs[MXC_CFG_UART_INSTANCES];

//******************************************************************************
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    // Determine which uart is associated with each pin
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    // Make sure that both pins are pointing to the same uart
    MBED_ASSERT(uart != (UARTName)NC);

    // Set the obj pointer to the proper uart
    obj->uart = (mxc_uart_regs_t*)uart;

    // Set the uart index
    obj->index = MXC_UART_GET_IDX(obj->uart);
    obj->fifo = (mxc_uart_fifo_regs_t*)MXC_UART_GET_BASE_FIFO(obj->index);

    // Manage stdio UART
    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }

    // Merge pin function requests for use with CMSIS init func
    uint32_t req_val;
    pin_function_t *pin_func;
    pin_func = (pin_function_t *)pinmap_find_function(tx, PinMap_UART_TX);
    req_val  = pin_func->req_val;
    pin_func = (pin_function_t *)pinmap_find_function(rx, PinMap_UART_RX);
    req_val |= pin_func->req_val;

    // Using req and ack pointers of last pin function lookup
    sys_cfg_uart_t sys_cfg;
    sys_cfg.io_cfg.req_reg = pin_func->reg_req;
    sys_cfg.io_cfg.ack_reg = pin_func->reg_ack;
    sys_cfg.io_cfg.req_val = (ioman_req_t)req_val;
    sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;

    // Configure the UART with default parameters
    obj->cfg.extra_stop = 0;
    obj->cfg.cts = 0;
    obj->cfg.rts = 0;
    obj->cfg.baud = DEFAULT_BAUD;
    obj->cfg.size = UART_DATA_SIZE_8_BITS;
    obj->cfg.parity = UART_PARITY_DISABLE;

    int retval = UART_Init(obj->uart, &obj->cfg, &sys_cfg);
    MBED_ASSERT(retval == E_NO_ERROR);
}

//******************************************************************************
void serial_baud(serial_t *obj, int baudrate)
{
    obj->cfg.baud = baudrate;
    int retval = UART_Init(obj->uart, &obj->cfg, NULL);
    MBED_ASSERT(retval == E_NO_ERROR);
}

//******************************************************************************
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    switch (data_bits) {
        case 5:
            obj->cfg.size = UART_DATA_SIZE_5_BITS;
            break;
        case 6:
            obj->cfg.size = UART_DATA_SIZE_6_BITS;
            break;
        case 7:
            obj->cfg.size = UART_DATA_SIZE_7_BITS;
            break;
        case 8:
            obj->cfg.size = UART_DATA_SIZE_8_BITS;
            break;
        default:
            MBED_ASSERT(0);
            break;
    }

    switch (parity) {
        case ParityNone:
            obj->cfg.parity = UART_PARITY_DISABLE;
            break;
        case ParityOdd :
            obj->cfg.parity = UART_PARITY_ODD;
            break;
        case ParityEven:
            obj->cfg.parity = UART_PARITY_EVEN;
            break;
        case ParityForced1:
        case ParityForced0:
        default:
            MBED_ASSERT(0);
            break;
    }

    switch (stop_bits) {
        case 1:
            obj->cfg.extra_stop = 0;
            break;
        case 2:
            obj->cfg.extra_stop = 1;
            break;
        default:
            MBED_ASSERT(0);
            break;
    }

    int retval = UART_Init(obj->uart, &obj->cfg, NULL);
    MBED_ASSERT(retval == E_NO_ERROR);
}

//******************************************************************************
void uart_handler(serial_t *obj)
{
    if (obj && obj->id) {
        irq_handler(obj->id, RxIrq);
    }
}

void uart0_handler(void) { uart_handler(objs[0]); }
void uart1_handler(void) { uart_handler(objs[1]); }
void uart2_handler(void) { uart_handler(objs[2]); }

//******************************************************************************
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    obj->id = id;
}

//******************************************************************************
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    switch (obj->index) {
        case 0:
            NVIC_SetVector(UART0_IRQn, (uint32_t)uart0_handler);
            NVIC_EnableIRQ(UART0_IRQn);
            break;
        case 1:
            NVIC_SetVector(UART1_IRQn, (uint32_t)uart1_handler);
            NVIC_EnableIRQ(UART1_IRQn);
            break;
        case 2:
            NVIC_SetVector(UART2_IRQn, (uint32_t)uart2_handler);
            NVIC_EnableIRQ(UART2_IRQn);
            break;
        default:
            MBED_ASSERT(0);
    }

    if (irq == RxIrq) {
        // Enable RX FIFO Threshold Interrupt
        if (enable) {
            // Clear pending interrupts
            obj->uart->intfl = obj->uart->intfl;
            obj->uart->inten |= (MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY | UART_ERRORS);
        } else {
            // Clear pending interrupts
            obj->uart->intfl = obj->uart->intfl;
            obj->uart->inten &= ~(MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY | UART_ERRORS);
        }
    } else if (irq == TxIrq) {
        // Set TX Almost Empty level to interrupt when empty
        MXC_SET_FIELD(&obj->uart->tx_fifo_ctrl, MXC_F_UART_RX_FIFO_CTRL_FIFO_AF_LVL,
                      (MXC_UART_FIFO_DEPTH - 1) << MXC_F_UART_TX_FIFO_CTRL_FIFO_AE_LVL_POS);

        // Enable TX Almost Empty Interrupt
        if (enable) {
            // Clear pending interrupts
            obj->uart->intfl = obj->uart->intfl;
            obj->uart->inten |= MXC_F_UART_INTFL_TX_FIFO_AE;
        } else {
            // Clear pending interrupts
            obj->uart->intfl = obj->uart->intfl;
            obj->uart->inten &= ~MXC_F_UART_INTFL_TX_FIFO_AE;
        }
    } else {
        MBED_ASSERT(0);
    }
}

//******************************************************************************
int serial_getc(serial_t *obj)
{
    int c;

    // Wait for data to be available
    while ((obj->uart->rx_fifo_ctrl & MXC_F_UART_RX_FIFO_CTRL_FIFO_ENTRY) == 0);

    c = obj->fifo->rx;

    return c;
}

//******************************************************************************
void serial_putc(serial_t *obj, int c)
{
    // Wait for room in the FIFO without blocking interrupts.
    while (UART_NumWriteAvail(obj->uart) == 0);

    // Must clear before every write to the buffer to know that the FIFO
    // is empty when the TX DONE bit is set
    obj->uart->intfl = MXC_F_UART_INTFL_TX_DONE;
    obj->fifo->tx = (uint8_t)c;
}

//******************************************************************************
int serial_readable(serial_t *obj)
{
    return UART_NumReadAvail(obj->uart);
}

//******************************************************************************
int serial_writable(serial_t *obj)
{
    return UART_NumWriteAvail(obj->uart);
}

//******************************************************************************
void serial_clear(serial_t *obj)
{
    // Clear the RX and TX FIFOs
    UART_DrainRX(obj->uart);
    UART_DrainTX(obj->uart);
}

//******************************************************************************
void serial_break_set(serial_t *obj)
{
    // Make sure that nothing is being sent
    while ( ((obj->uart->tx_fifo_ctrl & MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY)
             >> MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY_POS) > 0);
    while (!(obj->uart->intfl & MXC_F_UART_INTFL_TX_DONE));

    // Configure the GPIO to output 0
    gpio_t tx_gpio;
    switch (((UARTName)(obj->uart))) {
        case UART_0:
            gpio_init_out(&tx_gpio, UART0_TX);
            break;
        case UART_1:
            gpio_init_out(&tx_gpio, UART1_TX);
            break;
        case UART_2:
            gpio_init_out(&tx_gpio, UART2_TX);
            break;
        default:
            gpio_init_out(&tx_gpio, (PinName)NC);
            break;
    }

    gpio_write(&tx_gpio, 0);

    // GPIO is setup now, but we need to map GPIO to the pin
    switch (((UARTName)(obj->uart))) {
        case UART_0:
            MXC_IOMAN->uart0_req &= ~MXC_F_IOMAN_UART_REQ_IO_REQ;
            MBED_ASSERT((MXC_IOMAN->uart0_ack & (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)) == 0);
            break;
        case UART_1:
            MXC_IOMAN->uart1_req &= ~MXC_F_IOMAN_UART_REQ_IO_REQ;
            MBED_ASSERT((MXC_IOMAN->uart1_ack & (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)) == 0);
            break;
        case UART_2:
            MXC_IOMAN->uart2_req &= ~MXC_F_IOMAN_UART_REQ_IO_REQ;
            MBED_ASSERT((MXC_IOMAN->uart2_ack & (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)) == 0);
            break;
        default:
            break;
    }
}

//******************************************************************************
void serial_break_clear(serial_t *obj)
{
    // Configure the GPIO to output 1
    gpio_t tx_gpio;
    switch (((UARTName)(obj->uart))) {
        case UART_0:
            gpio_init_out(&tx_gpio, UART0_TX);
            break;
        case UART_1:
            gpio_init_out(&tx_gpio, UART1_TX);
            break;
        case UART_2:
            gpio_init_out(&tx_gpio, UART2_TX);
            break;
        default:
            gpio_init_out(&tx_gpio, (PinName)NC);
            break;
    }

    gpio_write(&tx_gpio, 1);

    // Renable UART
    switch (((UARTName)(obj->uart))) {
        case UART_0:
            serial_pinout_tx(UART0_TX);
            break;
        case UART_1:
            serial_pinout_tx(UART1_TX);
            break;
        case UART_2:
            serial_pinout_tx(UART2_TX);
            break;
        default:
            serial_pinout_tx((PinName)NC);
            break;
    }
}

//******************************************************************************
void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

//******************************************************************************
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
}
