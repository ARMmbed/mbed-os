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
#include "uart_regs.h"
#include "ioman_regs.h"
#include "gpio_api.h"
#include "clkman_regs.h"
#include "PeripheralPins.h"

#define DEFAULT_BAUD    9600
#define DEFAULT_STOP    1
#define DEFAULT_PARITY  ParityNone

#define UART_ERRORS     (MXC_F_UART_INTFL_RX_FRAMING_ERR | \
                         MXC_F_UART_INTFL_RX_PARITY_ERR | \
                         MXC_F_UART_INTFL_RX_FIFO_OVERFLOW)

// Variables for managing the stdio UART
int stdio_uart_inited;
serial_t stdio_uart;

// Variables for interrupt driven
static uart_irq_handler irq_handler;
static uint32_t serial_irq_ids[MXC_CFG_UART_INSTANCES];

//******************************************************************************
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    // Determine which uart is associated with each pin
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    // Make sure that both pins are pointing to the same uart
    MBED_ASSERT(uart != (UARTName)NC);

    // Ensure that the UART clock is enabled
    switch (uart) {
        case UART_0:
            MXC_CLKMAN->clk_gate_ctrl1 |= MXC_F_CLKMAN_CLK_GATE_CTRL1_UART0_CLK_GATER;
            break;
        case UART_1:
            MXC_CLKMAN->clk_gate_ctrl1 |= MXC_F_CLKMAN_CLK_GATE_CTRL1_UART1_CLK_GATER;
            break;
        case UART_2:
            MXC_CLKMAN->clk_gate_ctrl1 |= MXC_F_CLKMAN_CLK_GATE_CTRL1_UART2_CLK_GATER;
            break;
        case UART_3:
            MXC_CLKMAN->clk_gate_ctrl1 |= MXC_F_CLKMAN_CLK_GATE_CTRL1_UART3_CLK_GATER;
            break;
        default:
            break;
    }

    // Ensure that the UART clock is enabled
    // But don't override the scaler
    //
    // To support the most common baud rates, 9600 and 115200, we need to
    // scale down the uart input clock.
    if (!(MXC_CLKMAN->sys_clk_ctrl_8_uart & MXC_F_CLKMAN_SYS_CLK_CTRL_8_UART_UART_CLK_SCALE)) {

        switch (SystemCoreClock) {
            case RO_FREQ:
                MXC_CLKMAN->sys_clk_ctrl_8_uart = MXC_S_CLKMAN_CLK_SCALE_DIV_4;
                break;
            case (RO_FREQ / 2):
                MXC_CLKMAN->sys_clk_ctrl_8_uart = MXC_S_CLKMAN_CLK_SCALE_DIV_2;
                break;
            default:
                MXC_CLKMAN->sys_clk_ctrl_8_uart = MXC_S_CLKMAN_CLK_SCALE_DIV_4;
                break;
        }
    }

    // Set the obj pointer to the proper uart
    obj->uart = (mxc_uart_regs_t*)uart;

    // Set the uart index
    obj->index = MXC_UART_GET_IDX(obj->uart);
    obj->fifo = (mxc_uart_fifo_regs_t*)MXC_UART_GET_BASE_FIFO(obj->index);

    // Configure the pins
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    // Flush the RX and TX FIFOs, clear the settings
    obj->uart->ctrl &= ~(MXC_F_UART_CTRL_RX_FIFO_EN | MXC_F_UART_CTRL_TX_FIFO_EN);
    obj->uart->ctrl |= (MXC_F_UART_CTRL_RX_FIFO_EN | MXC_F_UART_CTRL_TX_FIFO_EN);

    // Disable interrupts
    obj->uart->inten = 0;
    obj->uart->intfl = obj->uart->intfl;

    // Configure to default settings
    serial_baud(obj, DEFAULT_BAUD);
    serial_format(obj, 8, ParityNone, 1);

    // Manage stdio UART
    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }

    // Enable UART
    obj->uart->ctrl |= MXC_F_UART_CTRL_UART_EN;
}

//******************************************************************************
void serial_baud(serial_t *obj, int baudrate)
{
    uint32_t baud_setting = 0;

    MBED_ASSERT(MXC_CLKMAN->sys_clk_ctrl_8_uart > MXC_S_CLKMAN_CLK_SCALE_DISABLED);

    // Calculate the integer and decimal portions
    baud_setting = SystemCoreClock / (1<<(MXC_CLKMAN->sys_clk_ctrl_8_uart-1));
    baud_setting = baud_setting / (baudrate * 16);

    // If the result doesn't fit in the register
    MBED_ASSERT(baud_setting <= UINT8_MAX);

    obj->uart->baud = baud_setting;
}

//******************************************************************************
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    // Check the validity of the inputs
    MBED_ASSERT((data_bits > 4) && (data_bits < 9));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) ||
                (parity == ParityEven) || (parity == ParityForced1) ||
                (parity == ParityForced0));
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));

    // Adjust the stop and data bits
    stop_bits -= 1;
    data_bits -= 5;

    // Adjust the parity setting
    int mode = 0;
    switch (parity) {
        case ParityNone:
            mode = 0;
            break;
        case ParityOdd :
            mode = 1;
            break;
        case ParityEven:
            mode = 2;
            break;
        case ParityForced1:
            // Hardware does not support forced parity
            MBED_ASSERT(0);
            break;
        case ParityForced0:
            // Hardware does not support forced parity
            MBED_ASSERT(0);
            break;
        default:
            mode = 0;
            break;
    }

    int temp = obj->uart->ctrl;
    temp &= ~(MXC_F_UART_CTRL_DATA_SIZE | MXC_F_UART_CTRL_EXTRA_STOP | MXC_F_UART_CTRL_PARITY);
    temp |= (data_bits << MXC_F_UART_CTRL_DATA_SIZE_POS);
    temp |= (stop_bits << MXC_F_UART_CTRL_EXTRA_STOP_POS);
    temp |= (mode << MXC_F_UART_CTRL_PARITY_POS);
    obj->uart->ctrl = temp;
}

//******************************************************************************
void uart_handler(mxc_uart_regs_t* uart, int id)
{
    // Check for errors or RX Threshold
    if (uart->intfl & (MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY | UART_ERRORS)) {
        if (serial_irq_ids[id]) {
            irq_handler(serial_irq_ids[id], RxIrq);
        }
        uart->intfl = (MXC_F_UART_INTFL_RX_FIFO_NOT_EMPTY | UART_ERRORS);
    }

    // Check for TX Threshold
    if (uart->intfl & MXC_F_UART_INTFL_TX_FIFO_AE) {
        if (serial_irq_ids[id]) {
            irq_handler(serial_irq_ids[id], TxIrq);
        }
        uart->intfl = MXC_F_UART_INTFL_TX_FIFO_AE;
    }
}

void uart0_handler(void) { uart_handler(MXC_UART0, 0); }
void uart1_handler(void) { uart_handler(MXC_UART1, 1); }
void uart2_handler(void) { uart_handler(MXC_UART2, 2); }
void uart3_handler(void) { uart_handler(MXC_UART3, 3); }

//******************************************************************************
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
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
        case 3:
            NVIC_SetVector(UART3_IRQn, (uint32_t)uart3_handler);
            NVIC_EnableIRQ(UART3_IRQn);
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

    c = *obj->fifo->rx_8;

    return c;
}

//******************************************************************************
void serial_putc(serial_t *obj, int c)
{
    // Wait for TXFIFO to not be full
    while ( ((obj->uart->tx_fifo_ctrl & MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY)
            >> MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY_POS)
            >= MXC_UART_FIFO_DEPTH );

    // Must clear before every write to the buffer to know that the fifo
    // is empty when the TX DONE bit is set
    obj->uart->intfl = MXC_F_UART_INTFL_TX_DONE;
    *obj->fifo->tx_8 = (uint8_t)c;
}

//******************************************************************************
int serial_readable(serial_t *obj)
{
    return (obj->uart->rx_fifo_ctrl & MXC_F_UART_RX_FIFO_CTRL_FIFO_ENTRY);
}

//******************************************************************************
int serial_writable(serial_t *obj)
{
    return ( ((obj->uart->tx_fifo_ctrl & MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY) 
             >> MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY_POS)
             < MXC_UART_FIFO_DEPTH );
}

//******************************************************************************
void serial_clear(serial_t *obj)
{
    // Clear the rx and tx fifos
    obj->uart->ctrl &= ~(MXC_F_UART_CTRL_RX_FIFO_EN | MXC_F_UART_CTRL_TX_FIFO_EN);
    obj->uart->ctrl |= (MXC_F_UART_CTRL_RX_FIFO_EN | MXC_F_UART_CTRL_TX_FIFO_EN);
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
        case UART_3:
            gpio_init_out(&tx_gpio, UART3_TX);
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
        case UART_3:
            MXC_IOMAN->uart3_req &= ~MXC_F_IOMAN_UART_REQ_IO_REQ;
            MBED_ASSERT((MXC_IOMAN->uart3_ack & (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)) == 0);
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
        case UART_3:
            gpio_init_out(&tx_gpio, UART3_TX);
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
        case UART_3:
            serial_pinout_tx(UART3_TX);
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
    uint32_t ctrl = obj->uart->ctrl;

    // Disable hardware flow control
    ctrl &= ~(MXC_F_UART_CTRL_RTS_EN | MXC_F_UART_CTRL_CTS_EN);

    if (FlowControlNone != type) {
        // Check to see if we can use HW flow control
        UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);
        UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
        UARTName uart = (UARTName)pinmap_merge(uart_cts, uart_rts);

        // Make sure that the pins are pointing to the same UART
        MBED_ASSERT(uart != (UARTName)NC);

        if ((FlowControlCTS == type) || (FlowControlRTSCTS == type)) {
            // Make sure pin is in the PinMap
            MBED_ASSERT(uart_cts != (UARTName)NC);

            // Enable the pin for CTS function
            pinmap_pinout(txflow, PinMap_UART_CTS);

            // Enable active-low hardware flow control
            ctrl |= (MXC_F_UART_CTRL_CTS_EN | MXC_F_UART_CTRL_CTS_POLARITY);
        }

        if ((FlowControlRTS == type) || (FlowControlRTSCTS == type)) {
            // Make sure pin is in the PinMap
            MBED_ASSERT(uart_rts != (UARTName)NC);

            // Enable the pin for RTS function
            pinmap_pinout(rxflow, PinMap_UART_RTS);

            // Enable active-low hardware flow control
            ctrl |= (MXC_F_UART_CTRL_RTS_EN | MXC_F_UART_CTRL_RTS_POLARITY);
        }
    }

    obj->uart->ctrl = ctrl;
}
