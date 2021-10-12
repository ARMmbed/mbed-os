/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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

#include <stdio.h>
#include <string.h>
#include "mbed_assert.h"
#include "cmsis.h"
#include "serial_api.h"
#include "gpio_api.h"
#include "uart.h"
#include "uart_regs.h"
#include "uart_reva_regs.h"
#include "PeripheralPins.h"

#define DEFAULT_BAUD    9600

// Variables for managing the stdio UART
int stdio_uart_inited = 0;
serial_t stdio_uart = {0};

// Variables for interrupt driven
static uart_irq_handler irq_handler = NULL;
static serial_t *objs[MXC_UART_INSTANCES];


#define UART_ER_IE      (MXC_F_UART_REVA_INT_EN_RX_FRAME_ERROR | \
                         MXC_F_UART_REVA_INT_EN_RX_PARITY_ERROR | \
                         MXC_F_UART_REVA_INT_EN_RX_OVERRUN)

#define UART_RX_IE (MXC_F_UART_INT_EN_RX_FIFO_LVL)

#define UART_TX_IE (MXC_F_UART_INT_EN_TX_FIFO_AE | \
                    MXC_F_UART_INT_EN_TX_FIFO_LVL)

#define MXC_F_UART_THRESH_CTRL_TX_FIFO_DEFAULT_THRESH_VAL  (1 << MXC_F_UART_CTRL1_TX_FIFO_LVL_POS)
#define MXC_F_UART_THRESH_CTRL_RX_FIFO_DEFAULT_THRESH_VAL  (1 << MXC_F_UART_CTRL1_RX_FIFO_LVL_POS)

static void usurp_pin(PinName, int);

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

    // Record the pins requested
    obj->tx = tx;
    obj->rx = rx;

    obj->map = MAP_A;

    if (uart == UART_1) {
        if ( (tx == UART1B_TX) && (rx == UART1B_RX) ) { 
            obj->map = MAP_B;
        } else if ( (tx == UART1C_TX) && (rx == UART1C_RX) ) { 
            obj->map = MAP_C;
        }
    }

    // Manage stdio UART
    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        stdio_uart = *obj;
    }

    MXC_UART_Init (obj->uart, DEFAULT_BAUD, obj->map);
    //MBED_ASSERT(retval == E_NO_ERROR);
}

void serial_free(serial_t *obj)
{
    MXC_UART_Shutdown(obj->uart);
    objs[obj->index] = 0;
    obj->index = 0xff;
}

//******************************************************************************
void serial_baud(serial_t *obj, int baudrate)
{
    MXC_UART_SetFrequency(obj->uart, baudrate);
    //MBED_ASSERT(retval == E_NO_ERROR);
}

//******************************************************************************
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
 
    MXC_UART_SetDataSize(obj->uart, data_bits);

    switch (parity) {
        case ParityNone:
            MXC_UART_SetParity (obj->uart, MXC_UART_PARITY_DISABLE);
            break;
        case ParityOdd :
            MXC_UART_SetParity (obj->uart, MXC_UART_PARITY_ODD);
            break;
        case ParityEven:
            MXC_UART_SetParity (obj->uart, MXC_UART_PARITY_EVEN);
            break;
        case ParityForced1:
        case ParityForced0:
        default:
            MBED_ASSERT(0);
            break;
    }

    switch (stop_bits) {
        case 1:
            MXC_UART_SetStopBits (obj->uart, MXC_UART_STOP_1);
            break;
        case 2:
            MXC_UART_SetStopBits (obj->uart, MXC_UART_STOP_2);
            break;
        default:
            MBED_ASSERT(0);
            break;
    }
}

//******************************************************************************
void uart_handler(serial_t *obj)
{
    // clear interrupts
    volatile uint32_t flags = obj->uart->int_fl;
    
    if (flags & UART_ER_IE) {
        // clear error flags
        obj->uart->int_fl = UART_ER_IE;
    }
    
    if (flags & UART_RX_IE) {
        obj->uart->int_fl = UART_RX_IE;
        if (obj && irq_handler) {
            irq_handler(obj->id, RxIrq);
        }
    }
    
    if (flags & UART_TX_IE) {
        obj->uart->int_fl = UART_TX_IE;
        if (obj && irq_handler) {
            irq_handler(obj->id, TxIrq);
        }
    }
}

void uart0_handler(void) { uart_handler(objs[0]); }
void uart1_handler(void) { uart_handler(objs[1]); }

//******************************************************************************
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    obj->id = id;
}

//******************************************************************************
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    MBED_ASSERT(obj->index < MXC_UART_INSTANCES);
    objs[obj->index] = obj;

    switch (obj->index) {
        case 0:
            NVIC_SetVector(UART0_IRQn, (uint32_t)uart0_handler);
            NVIC_EnableIRQ(UART0_IRQn);
            break;
        case 1:
            NVIC_SetVector(UART1_IRQn, (uint32_t)uart1_handler);
            NVIC_EnableIRQ(UART1_IRQn);
            break;
        default:
            MBED_ASSERT(0);
    }

    // Clear pending interrupts
    obj->uart->int_fl = obj->uart->int_fl;

    // Set TX Almost Empty level to interrupt when empty
    obj->uart->ctrl1 = (MXC_F_UART_THRESH_CTRL_RX_FIFO_DEFAULT_THRESH_VAL | MXC_F_UART_THRESH_CTRL_TX_FIFO_DEFAULT_THRESH_VAL);
    
    if (irq == RxIrq) {
        // Enable RX FIFO Threshold Interrupt
        if (enable) {
            obj->uart->int_en |= UART_RX_IE | UART_ER_IE;
        } else {
            obj->uart->int_en &= ~(UART_RX_IE | UART_ER_IE);
        }
    } else if (irq == TxIrq) {
        // Enable TX Almost Empty Interrupt
        if (enable) {
            //obj->uart->int_en |= MXC_F_UART_INT_EN_TX_FIFO_ALMOST_EMPTY;
            obj->uart->int_en |= UART_TX_IE | UART_ER_IE;
        } else {
            //obj->uart->int_en &= ~MXC_F_UART_INT_EN_TX_FIFO_ALMOST_EMPTY;
            obj->uart->int_en &= ~(UART_TX_IE | UART_ER_IE);
        }
    } else {
        MBED_ASSERT(0);
    }
}

//******************************************************************************
int serial_getc(serial_t *obj)
{
    int c = -1;

    if (obj->rx != NC) {
        // Wait for data to be available
        while( (obj->uart->stat & MXC_F_UART_STAT_RX_NUM) == 0);
        c = obj->uart->fifo;
    }

    return c;
}

//******************************************************************************
void serial_putc(serial_t *obj, int c)
{
    if (obj->tx != NC) {
        // Wait for room in the FIFO without blocking interrupts.
        while (MXC_UART_GetTXFIFOAvailable(obj->uart) == 0);

        //obj->uart->int_fl |= MXC_F_UART_INT_FL_TX_FIFO_ALMOST_EMPTY;
        obj->uart->fifo = (uint8_t)c;
    }
}

//******************************************************************************
int serial_readable(serial_t *obj)
{
	unsigned int ret;
	ret = MXC_UART_GetRXFIFOAvailable(obj->uart);
    return ret;
}

//******************************************************************************
int serial_writable(serial_t *obj)
{
    return MXC_UART_GetTXFIFOAvailable(obj->uart);
}

//******************************************************************************
void serial_clear(serial_t *obj)
{
    // Clear the RX and TX FIFOs
    MXC_UART_ClearRXFIFO(obj->uart);
    MXC_UART_ClearTXFIFO(obj->uart);
}

//******************************************************************************
void serial_break_set(serial_t *obj)
{
    // Make sure that nothing is being sent
    while ( (obj->uart->stat & MXC_F_UART_REVA_STATUS_TX_BUSY) );

    // Configure TX to output 0
    usurp_pin(obj->tx, 0);
}

//******************************************************************************
void serial_break_clear(serial_t *obj)
{
    // Configure TX to output 1
    usurp_pin(obj->tx, 1);
    // Return TX to UART control
    serial_pinout_tx(obj->tx);
}

//******************************************************************************
void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

//******************************************************************************
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{

    if ((FlowControlCTS == type) || (FlowControlRTSCTS == type) || (FlowControlRTS == type) || (FlowControlRTSCTS == type)) {
        UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);
        UARTName uart = (UARTName)pinmap_merge(uart_cts, (UARTName)obj->uart);
        // Assert pin is usable with existing uart
        MBED_ASSERT(uart != (UARTName)NC);
    }

    MXC_UART_SetFlowCtrl (obj->uart, MXC_UART_FLOW_EN_LOW, 1);
    //MBED_ASSERT(retval == E_NO_ERROR);
}

//******************************************************************************
static void usurp_pin(PinName pin, int state)
{
    gpio_t gpio;
    gpio_init_out(&gpio, pin);
    gpio_write(&gpio, state);
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
