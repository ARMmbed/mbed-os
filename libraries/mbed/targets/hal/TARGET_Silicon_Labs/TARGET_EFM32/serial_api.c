/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "device.h"
#if DEVICE_SERIAL

#include "mbed_assert.h"
#include "serial_api.h"
#include <string.h>

#include "pinmap.h"
#include "pinmap_function.h"
#include "PeripheralPins.h"

#include "em_usart.h"
#include "em_cmu.h"

#define SERIAL_NUM_UARTS (5)

/* Store IRQ id for each UART */
static uint32_t serial_irq_ids[SERIAL_NUM_UARTS] = { 0 };
/* Interrupt handler from mbed common */
static uart_irq_handler irq_handler;

/* Serial interface on USBTX/USBRX retargets stdio */
int stdio_uart_inited = 0;
serial_t stdio_uart;

static void uart_irq(UARTName, int, SerialIrq);
/* ISRs for RX and TX events */
#ifdef UART0
static void uart0_rx_irq() { uart_irq(UART_0, 0, RxIrq); }
static void uart0_tx_irq() { uart_irq(UART_0, 0, TxIrq); }
#endif
#ifdef UART1
static void uart1_rx_irq() { uart_irq(UART_1, 1, RxIrq); }
static void uart1_tx_irq() { uart_irq(UART_1, 1, TxIrq); }
#endif
#ifdef USART0
static void usart0_rx_irq() { uart_irq(USART_0, 2, RxIrq); }
static void usart0_tx_irq() { uart_irq(USART_0, 2, TxIrq); }
#endif
#ifdef USART1
static void usart1_rx_irq() { uart_irq(USART_1, 3, RxIrq); }
static void usart1_tx_irq() { uart_irq(USART_1, 3, TxIrq); }
#endif
#ifdef USART2
static void usart2_rx_irq() { uart_irq(USART_2, 4, RxIrq); }
static void usart2_tx_irq() { uart_irq(USART_2, 4, TxIrq); }
#endif

/**
 * Initialize the UART using default settings, overridden by settings from serial object
 */
static void uart_init(serial_t *obj)
{
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

    init.enable = obj->enable;
    init.baudrate = obj->baudrate;
    init.oversampling = obj->oversampling;
    init.databits = obj->databits;
    init.parity = obj->parity;
    init.stopbits = obj->stopbits;

    USART_InitAsync(obj->uart, &init);
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    /* Get UART object connected to the given pins */
    UARTName uart_tx = (UARTName) pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName) pinmap_peripheral(rx, PinMap_UART_RX);
    /* Check that pins are connected to same UART */
    UARTName uart = (UARTName) pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int) uart != NC);

    obj->uart = (USART_TypeDef *) uart;

    /* Get location */
    uint32_t uart_tx_loc = pin_location(tx, PinMap_UART_TX);
    uint32_t uart_rx_loc = pin_location(rx, PinMap_UART_RX);
    /* Check that pins are used by same location for the given UART */
    uint32_t location = pinmap_merge(uart_tx_loc, uart_rx_loc);
    MBED_ASSERT(location != (uint32_t)NC);

    /* Enable GPIO clock */
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Configure GPIO pins*/
    pin_mode(rx, Input);
    /* 0x10 sets DOUT. Prevents false start. */
    pin_mode(tx, PushPull | 0x10);
    /* Store pins in object for easy disabling in serial_free() */
    obj->rx_pin = rx;
    obj->tx_pin = tx;

    /* Enable peripheral clocks */
    CMU_ClockEnable(cmuClock_HFPER, true);

    /* Select interrupt and enable clock */
    switch (uart) {
#ifdef UART0
        case UART_0:
            obj->index = 0;
            obj->clock = cmuClock_UART0;
            obj->rx_irq = UART0_RX_IRQn;
            obj->tx_irq = UART0_TX_IRQn;
            obj->rx_vector = (uint32_t) &uart0_rx_irq;
            obj->tx_vector = (uint32_t) &uart0_tx_irq;
            break;
#endif
#ifdef UART1
        case UART_1:
            obj->index = 1;
            obj->clock = cmuClock_UART1;
            obj->rx_irq = UART1_RX_IRQn;
            obj->tx_irq = UART1_TX_IRQn;
            obj->rx_vector = (uint32_t) &uart1_rx_irq;
            obj->tx_vector = (uint32_t) &uart1_tx_irq;
            break;
#endif
#ifdef USART0
        case USART_0:
            obj->index = 2;
            obj->clock = cmuClock_USART0;
            obj->rx_irq = USART0_RX_IRQn;
            obj->tx_irq = USART0_TX_IRQn;
            obj->rx_vector = (uint32_t) &usart0_rx_irq;
            obj->tx_vector = (uint32_t) &usart0_tx_irq;
            break;
#endif
#ifdef USART1
        case USART_1:
            obj->index = 3;
            obj->clock = cmuClock_USART1;
            obj->rx_irq = USART1_RX_IRQn;
            obj->tx_irq = USART1_TX_IRQn;
            obj->rx_vector = (uint32_t) &usart1_rx_irq;
            obj->tx_vector = (uint32_t) &usart1_tx_irq;
            break;
#endif
#ifdef USART2
        case USART_2:
            obj->index = 4;
            obj->clock = cmuClock_USART2;
            obj->rx_irq = USART2_RX_IRQn;
            obj->tx_irq = USART2_TX_IRQn;
            obj->rx_vector = (uint32_t) &usart2_rx_irq;
            obj->tx_vector = (uint32_t) &usart2_tx_irq;
            break;
#endif
    }
    CMU_ClockEnable(obj->clock, true);

    /* Configure UART for async operation */
    obj->enable = usartDisable;
    obj->baudrate = 9600;
    obj->oversampling = usartOVS16;
    obj->databits = usartDatabits8;
    obj->parity = usartNoParity;
    obj->stopbits = usartStopbits1;
    uart_init(obj);

    /* Enable pins for UART at correct location */
    obj->uart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | (location << _USART_ROUTE_LOCATION_SHIFT);

    /* Finally enable UART */
    obj->enable = usartEnable;
    USART_Enable(obj->uart, usartEnable);

    /* If this is the UART to be used for stdio, copy it to the stdio_uart struct */
    if (obj->uart == STDIO_UART ) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

/**
 * Disable UART and clean up
 */
void serial_free(serial_t *obj)
{
    USART_Enable(obj->uart, usartDisable);
    USART_Reset(obj->uart);
    CMU_ClockEnable(obj->clock, false);

    pin_mode(obj->tx_pin, Disabled);
    pin_mode(obj->rx_pin, Disabled);

    serial_irq_ids[obj->index] = 0;
}

/**
 * Set UART baud rate
 */
void serial_baud(serial_t *obj, int baudrate)
{
    obj->baudrate = baudrate;
    uart_init(obj);
}

/**
 * Set UART format
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    /* We support 4 to 9 data bits */
    MBED_ASSERT(data_bits >= 4 && data_bits <= 9);
    obj->databits = (USART_Databits_TypeDef)((data_bits - 3) << _USART_FRAME_DATABITS_SHIFT);

    if (stop_bits == 2) {
        obj->stopbits = usartStopbits2;
    } else {
        obj->stopbits = usartStopbits1;
    }

    switch (parity) {
        case ParityOdd:
        case ParityForced0:
            obj->parity = usartOddParity;
            break;
        case ParityEven:
        case ParityForced1:
            obj->parity = usartEvenParity;
            break;
        default: /* ParityNone */
            obj->parity = usartNoParity;
            break;
    }

    uart_init(obj);
}

/******************************************************************************
 *                               INTERRUPTS                                   *
 ******************************************************************************/

/**
 * Set handler for all serial interrupts (is probably SerialBase::_handler())
 * and store IRQ ID to be returned to the handler upon interrupt. ID is
 * probably a pointer to the calling Serial object.
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

/**
 * Generic ISR for all UARTs, both TX and RX
 */
static void uart_irq(UARTName name, int index, SerialIrq irq)
{
    if (serial_irq_ids[index] != 0) {
        /* Pass interrupt on to mbed common handler */
        irq_handler(serial_irq_ids[index], irq);
        /* Clearing interrupt not necessary */
    }
}

/**
 * Set ISR for a given UART and interrupt event (TX or RX)
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    /* Enable or disable interrupt */
    if (enable) {
        if (irq == RxIrq) { /* RX */
            USART_IntEnable(obj->uart, USART_IEN_RXDATAV);
            NVIC_ClearPendingIRQ(obj->rx_irq);
            NVIC_SetVector(obj->rx_irq, obj->rx_vector);
            NVIC_EnableIRQ(obj->rx_irq);
        } else { /* TX */
            USART_IntEnable(obj->uart, USART_IEN_TXBL);
            NVIC_ClearPendingIRQ(obj->tx_irq);
            NVIC_SetVector(obj->tx_irq, obj->tx_vector);
            NVIC_EnableIRQ(obj->tx_irq);
        }
    } else {
        if (irq == RxIrq) { /* RX */
            USART_IntDisable(obj->uart, USART_IEN_RXDATAV);
            NVIC_DisableIRQ(obj->rx_irq);
        } else { /* TX */
            USART_IntDisable(obj->uart, USART_IEN_TXBL);
            NVIC_DisableIRQ(obj->tx_irq);
        }
    }
}

/******************************************************************************
 *                               READ/WRITE                                   *
 ******************************************************************************/

/**
 *  Get one char from serial link
 */
int serial_getc(serial_t *obj)
{
    /* Emlib USART_Rx blocks until data is available, so we don't need to use
     * serial_readable(). Use USART_RxDataGet() to read register directly.
     *
     * Use Rx with extended control in order to read up to 9 bits.
     * Only return the lower 9 bits, higher bits may contain control data. */
    return USART_RxExt(obj->uart) & 0x1FF;
}

/*
 * Send one char over serial link
 */
void serial_putc(serial_t *obj, int c)
{
    /* Emlib USART_Tx blocks until buffer is writable (non-full), so we don't
     * need to use serial_writable().
     * 
     * Use Tx with extended control in order to write up to 9 bits.
     * Only write the lower 9 bits, higher bits are for control data. */
    USART_TxExt(obj->uart, (uint16_t)(c) & 0x1FF);
}

/**
 * Check if data is available in RX data vector
 */
int serial_readable(serial_t *obj)
{
    return obj->uart->STATUS & USART_STATUS_RXDATAV;
}

/**
 * Check if TX buffer is empty
 */
int serial_writable(serial_t *obj)
{
    return obj->uart->STATUS & USART_STATUS_TXBL;
}

/**
 * Clear UART interrupts
 */
void serial_clear(serial_t *obj)
{
    /* Interrupts automatically clear when condition is not met anymore */
}

void serial_break_set(serial_t *obj)
{
    /* Send transmission break */
    obj->uart->TXDATAX |= USART_TXDATAX_TXBREAK;
}

void serial_break_clear(serial_t *obj)
{
    /* No need to clear break, it is automatically cleared after one frame.
     * From the reference manual:
     *
     * By setting TXBREAK, the output will be held low during the stop-bit
     * period to generate a framing error. A receiver that supports break
     * detection detects this state, allowing it to be used e.g. for framing
     * of larger data packets. The line is driven high before the next frame
     * is transmitted so the next start condition can be identified correctly
     * by the recipient. Continuous breaks lasting longer than a USART frame
     * are thus not supported by the USART. GPIO can be used for this.
     */
}

void serial_pinout_tx(PinName tx)
{
    /* 0x10 sets DOUT high. Prevents false start. */
    pin_mode(tx, PushPull | 0x10);
}

#endif
