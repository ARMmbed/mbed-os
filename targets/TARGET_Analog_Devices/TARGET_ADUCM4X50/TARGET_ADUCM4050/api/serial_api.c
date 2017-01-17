/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "serial_api.h"

#if DEVICE_SERIAL

// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include "mbed_assert.h"

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "drivers/uart/adi_uart.h"
#define ADI_UART_MEMORY_SIZE   (ADI_UART_BIDIR_MEMORY_SIZE)

static ADI_UART_HANDLE          hDevice;
static uint8_t UartDeviceMem[(ADI_UART_MEMORY_SIZE + 3)/4];
static uint32_t serial_irq_ids[2] = {0};
static uart_irq_handler irq_handler = NULL;
int stdio_uart_inited = 0;
serial_t stdio_uart;


static void uart_callback(void *pCBParam, uint32_t Event, void *pArg)
{
    MBED_ASSERT(irq_handler);

	if (Event == ADI_UART_EVENT_TX_BUFFER_PROCESSED)
        irq_handler(serial_irq_ids[0], TxIrq);
	else
	if (Event == ADI_UART_EVENT_RX_BUFFER_PROCESSED)
		irq_handler(serial_irq_ids[0], RxIrq);
}


void serial_free(serial_t *obj)
{
    adi_uart_Close(hDevice);
}

void serial_baud(serial_t *obj, int baudrate)
{
	uint32_t uartdivc,uartdivm,uartdivn,uartosr;

    // figures based on PCLK of 26MHz
    switch (baudrate)
    {
		case    9600: uartdivc= 28; uartdivm= 3; uartdivn= 46; uartosr= 3; break;
        case   19200: uartdivc= 14; uartdivm= 3; uartdivn= 46; uartosr= 3; break;
        case   38400: uartdivc= 07; uartdivm= 3; uartdivn= 46; uartosr= 3; break;
        case   57600: uartdivc= 14; uartdivm= 1; uartdivn= 15; uartosr= 3; break;
        case  115200: uartdivc= 03; uartdivm= 2; uartdivn= 719; uartosr= 3; break;
        case  230400: uartdivc= 03; uartdivm= 1; uartdivn= 359; uartosr= 3; break;
        default:    // default of 9600kbps
            uartdivc= 28; uartdivm= 3; uartdivn= 46; uartosr= 3;
            break;
	}

    adi_uart_ConfigBaudRate(hDevice,uartdivc,uartdivm,uartdivn,uartosr);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
	int convertedparity   = ADI_UART_NO_PARITY;
	int convertedstopbits = ADI_UART_ONE_STOPBIT;

	if (stop_bits)
	    convertedstopbits = ADI_UART_ONE_AND_HALF_TWO_STOPBITS;

	if (parity == ParityOdd)
	    convertedparity = ADI_UART_ODD_PARITY;
	else
	if (parity == ParityEven)
	    convertedparity = ADI_UART_EVEN_PARITY;
	else
	if (parity == ParityForced1)
	    convertedparity = ADI_UART_ODD_PARITY_STICKY;
	else
	if (parity == ParityForced0)
	    convertedparity = ADI_UART_EVEN_PARITY_STICKY;

	adi_uart_SetConfiguration(hDevice, convertedparity, convertedstopbits, (data_bits - 5));
}

void serial_init(serial_t *obj, PinName tx, PinName rx) {
	uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);

    obj->index = pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)obj->index != NC);

	adi_uart_Open(0, ADI_UART_DIR_BIDIRECTION, UartDeviceMem, ADI_UART_MEMORY_SIZE, &hDevice);

	serial_baud(obj, 9600);
	serial_format(obj, 8, ParityNone, 1);

	pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    if (tx != NC)
    {
        pin_mode(tx, PullUp);
    }

    if (rx != NC)
    {
        pin_mode(rx, PullUp);
    }

    if (obj->index == STDIO_UART)
    {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }

    // set maximum FIFO depth
    adi_uart_SetRxFifoTriggerLevel(hDevice, ADI_UART_RX_FIFO_TRIG_LEVEL_14BYTE);

    // enable FIFO
	adi_uart_EnableFifo(hDevice, true);
}

int serial_getc(serial_t *obj)
{
	int c;
	uint32_t hwErr;

    adi_uart_Read(hDevice, (void *) &c, 1, false, &hwErr);
    return (c);
}

void serial_putc(serial_t *obj, int c)
{
    uint32_t hwErr;

	adi_uart_Write(hDevice, &c, 1, false, &hwErr);
	return;
}

int serial_readable(serial_t *obj)
{
	return 0;
}

int serial_writable(serial_t *obj)
{
	return 0;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    MBED_ASSERT(obj);

    adi_uart_RegisterCallback(hDevice, &uart_callback, obj);
    if (enable)
    {
    }
    else
    {
    }
}

void serial_pinout_tx(PinName tx)
{

}

void serial_break_set(serial_t *obj)
{

}

void serial_break_clear(serial_t *obj)
{

}


uint8_t serial_tx_active(serial_t *obj)
{
	return 0;
}

uint8_t serial_rx_active(serial_t *obj)
{
	return 0;
}

int serial_irq_handler_asynch(serial_t *obj)
{
	return 0;
}



int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
	return 0;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{

}

void serial_tx_abort_asynch(serial_t *obj)
{

}

void serial_rx_abort_asynch(serial_t *obj)
{

}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{

    MBED_ASSERT(obj);

    irq_handler = handler;
	serial_irq_ids[0] = id;
}

#endif
