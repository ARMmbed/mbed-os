/***************************************************************************//**
 * @file serial_api.c
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "clocking.h"

#if DEVICE_SERIAL

#include "mbed_assert.h"
#include "mbed_power_mgmt.h"
#include "serial_api.h"
#include <string.h>
#include <stdbool.h>

#include "pinmap.h"
#include "PeripheralPins.h"
#include "PeripheralNames.h"

#include "compiler.h"
#include "pic32cx.h"
#include "usart.h"
// #include "stdio_serial.h"
#include "uart_serial.h"

#define UART_COUNT    1
#define USART_COUNT   8
#define MODULES_SIZE_SERIAL (UART_COUNT + USART_COUNT)

/* Interrupt handlers */
#define UART_Handler               UART_Handler
#define USART0_Handler             FLEXCOM0_Handler
#define USART1_Handler             FLEXCOM1_Handler
#define USART2_Handler             FLEXCOM2_Handler
#define USART3_Handler             FLEXCOM3_Handler
#define USART4_Handler             FLEXCOM4_Handler
#define USART5_Handler             FLEXCOM5_Handler
#define USART6_Handler             FLEXCOM6_Handler
#define USART7_Handler             FLEXCOM7_Handler

/* Store IRQ id for each UART */
static uint32_t serial_irq_ids[MODULES_SIZE_SERIAL] = { 0 };
/* Interrupt handler from mbed common */
static uart_irq_handler irq_handler;

/* Serial interface on USBTX/USBRX retargets stdio */
int stdio_uart_inited = 0;
serial_t stdio_uart;

/**
* Get index of serial object, relating it to the physical peripheral.
*
* @param obj pointer to serial peripheral (= base address of periph)
* @return internal index of U(S)ART peripheral
*/
static inline uint8_t _serial_pointer_get_index(uint32_t serial_ptr)
{
    uint8_t index = 0;
	
	if (serial_ptr == UART_0) return index;
    index++;

    if (serial_ptr == USART_0) return index;
    index++;

    if (serial_ptr == USART_1) return index;
    index++;

    if (serial_ptr == USART_2) return index;
    index++;

    if (serial_ptr == USART_3) return index;
    index++;

    if (serial_ptr == USART_4) return index;
    index++;

    if (serial_ptr == USART_5) return index;
    index++;

	if (serial_ptr == USART_6) return index;
    index++;

    if (serial_ptr == USART_7) return index;
    index++;
	
    return 0;
}

/**
* Get index of serial object, relating it to the physical peripheral.
*
* @param obj pointer to serial object (mbed object)
* @return internal index of U(S)ART peripheral
*/
static inline uint8_t _serial_get_index(serial_t *obj)
{
    return _serial_pointer_get_index((uint32_t)obj->p_usart);
}

static void _serial_enable(serial_t *obj, uint8_t enable)
{
	if (enable) {
		/* Enable the receiver and transmitter */
		if (obj->is_usart) {
			usart_enable_tx(obj->p_usart);
			usart_enable_rx(obj->p_usart);
		} else {
			uart_enable_tx((Uart *)obj->p_usart);
			uart_enable_rx((Uart *)obj->p_usart);
		}
	} else {
		/* Disable the receiver and transmitter */
		if (obj->is_usart) {
			usart_disable_tx(obj->p_usart);
			usart_disable_rx(obj->p_usart);
		} else {
			uart_disable_tx((Uart *)obj->p_usart);
			uart_disable_rx((Uart *)obj->p_usart);
		}
	}
}

static IRQn_Type _serial_get_irq_index(serial_t *obj)
{
    UARTName uart = (UARTName)obj->p_usart;
    
    switch (uart) {
	case UART_0:
		return UART_IRQn;
		break;
	case USART_0:
		return FLEXCOM0_IRQn;
		break;
	case USART_1:
		return FLEXCOM1_IRQn;
		break;
	case USART_2:
		return FLEXCOM2_IRQn;
		break;
	case USART_3:
		return FLEXCOM3_IRQn;
		break;
	case USART_4:
		return FLEXCOM4_IRQn;
		break;
	case USART_5:
		return FLEXCOM5_IRQn;
		break;
	case USART_6:
		return FLEXCOM6_IRQn;
		break;
	case USART_7:
		return FLEXCOM7_IRQn;
		break;
   }

   return (IRQn_Type)0;
}

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The serial object
 * @param tx  The TX pin name
 * @param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    /* Get UART object connected to the given pins */
    UARTName uart_tx = (UARTName) pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName) pinmap_peripheral(rx, PinMap_UART_RX);
    /* Check that pins are connected to same UART */
    UARTName uart = (UARTName) pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((unsigned int) uart != NC);
    
    /* Set Pin Mode */
    ioport_mode_t mode = (ioport_mode_t)pinmap_function(tx, PinMap_UART_TX);
    ioport_set_pin_mode(tx, mode);
    mode = (ioport_mode_t)pinmap_function(rx, PinMap_UART_RX);
    ioport_set_pin_mode(rx, mode);

	/* Set default values */
	obj->p_usart = (Usart *) uart;
	obj->serial_options.charlength = US_MR_CHRL_8_BIT;
	obj->serial_options.paritytype = US_MR_PAR_NO;
	obj->serial_options.stopbits = US_MR_NBSTOP_1_BIT;
	obj->serial_options.baudrate  = 115200;

    if (uart == STDIO_UART) {
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE
        obj->serial_options.baudrate  = MBED_CONF_PLATFORM_STDIO_BAUD_RATE; // baudrate takes value from platform/mbed_lib.json
#endif /* MBED_CONF_PLATFORM_STDIO_BAUD_RATE */
    } else {
#if MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
        obj->serial_options.baudrate  = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE; // baudrate takes value from platform/mbed_lib.json
#endif /* MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE */
    }

	/* Check if UART or USART */
	if (UART == (Uart *)obj->p_usart) {
		obj->is_usart = false;
	} else {
		/* Flexcomm USART */
		if ((USART0 == (Usart *)obj->p_usart) || (USART1 == (Usart *)obj->p_usart) ||
				(USART2 == (Usart *)obj->p_usart) || (USART3 == (Usart *)obj->p_usart) ||
				(USART4 == (Usart *)obj->p_usart) || (USART5 == (Usart *)obj->p_usart) ||
				(USART6 == (Usart *)obj->p_usart) || (USART7 == (Usart *)obj->p_usart)) {
			obj->is_usart = true;
		}
	}

	/* If this is the UART to be used for stdio, copy it to the stdio_uart struct */
	if (uart == STDIO_UART) {
	    stdio_uart_inited = 1;
	    memcpy(&stdio_uart, obj, sizeof(serial_t));
	}

	/* Configure serial mode */
	usart_serial_init((usart_if)obj->p_usart, &obj->serial_options);

	/* Disable all the interrupts */
	if (obj->is_usart) {
		usart_disable_interrupt(obj->p_usart, 0xFFFFFFFF);
	} else {
		uart_disable_interrupt((Uart *)obj->p_usart, 0xFFFFFFFF);
	}

	/* Configure and enable interrupt of U(S)ART */
	NVIC_EnableIRQ(_serial_get_irq_index(obj));
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The serial object
 */
void serial_free(serial_t *obj)
{
	/* Disable the receiver and transmitter */
	if (obj->is_usart) {
		usart_disable_tx(obj->p_usart);
		usart_disable_rx(obj->p_usart);
	} else {
		uart_disable_tx((Uart *)obj->p_usart);
		uart_disable_rx((Uart *)obj->p_usart);
	}
}

/** Configure the baud rate
 *
 * @param obj      The serial object
 * @param baudrate The baud rate to be configured
 */
void serial_baud(serial_t *obj, int baudrate)
{
	/* Only in Flexcomm USART */
	if (obj->is_usart) {
		usart_set_async_baudrate(obj->p_usart, (uint32_t)baudrate, sysclk_get_peripheral_bus_hz(obj->p_usart));
		obj->serial_options.baudrate = baudrate;
	}
}

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param obj       The serial object
 * @param data_bits The number of data bits
 * @param parity    The parity
 * @param stop_bits The number of stop bits
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
	/* Disable the receiver and transmitter */
	_serial_enable(obj, false);
		
	/* We support 5 to 8 data bits */
    MBED_ASSERT(data_bits >= 5 && data_bits <= 8);
	
	/* Set new values */
	obj->serial_options.charlength = (data_bits - 5) << 6;
	obj->serial_options.paritytype = US_MR_PAR_NO;
	obj->serial_options.stopbits = (stop_bits - 1) << 9;
	switch (parity) {
    case ParityNone:
		obj->serial_options.paritytype = US_MR_PAR_NO;
		break;
    case ParityOdd:
		obj->serial_options.paritytype = US_MR_PAR_ODD;
		break;
    case ParityEven:
		obj->serial_options.paritytype = US_MR_PAR_EVEN;
		break;
    case ParityForced1:
		obj->serial_options.paritytype = US_MR_PAR_MARK;
		break;
    case ParityForced0:
		obj->serial_options.paritytype = US_MR_PAR_SPACE;
		break;
	}

	/* Configure USART in serial mode */
	usart_serial_init((usart_if)obj->p_usart, &obj->serial_options);
}

/******************************************************************************
 *                               INTERRUPTS                                   *
 ******************************************************************************/

static inline void _uart_irq_handler(uint32_t index, SerialIrq irq_type)
{
    if (serial_irq_ids[index] != 0) {
        irq_handler(serial_irq_ids[index], irq_type);
    }
}

void UART_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = uart_get_status(UART);

	if (ul_status & UART_SR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(UART_0), RxIrq);
	} else if (ul_status & UART_SR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(UART_0), TxIrq);
	}
}

void USART0_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART0);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_0), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_0), TxIrq);
	}
}

void USART1_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART1);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_1), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_1), TxIrq);
	}
}

void USART2_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART2);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_2), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_2), TxIrq);
	}
}

void USART3_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART3);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_3), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_3), TxIrq);
	}
}

void USART4_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART4);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_4), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_4), TxIrq);
	}
}

void USART5_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART5);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_5), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_5), TxIrq);
	}
}

void USART6_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART6);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_6), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_6), TxIrq);
	}
}

void USART7_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status */
	ul_status = usart_get_status(USART7);

	if (ul_status & US_CSR_RXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_7), RxIrq);
	} else if (ul_status & US_CSR_TXRDY) {
		_uart_irq_handler(_serial_pointer_get_index(USART_7), TxIrq);
	}
}

/** The serial interrupt handler registration
 *
 * @param obj     The serial object
 * @param handler The interrupt handler which will be invoked when the interrupt fires
 * @param id      The SerialBase object
 */
/**
 * Set handler for all serial interrupts (is probably SerialBase::_handler())
 * and store IRQ ID to be returned to the handler upon interrupt. ID is
 * probably a pointer to the calling Serial object.
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[_serial_get_index(obj)] = id;
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * @param obj    The serial object
 * @param irq    The serial IRQ type (RX or TX)
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
/**
 * Set ISR for a given UART and interrupt event (TX or RX)
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
	/* Enable or disable interrupt */
	if (enable) {
		if (irq == RxIrq) { /* RX */
			if (!obj->is_usart) {
				uart_enable_interrupt((Uart *)obj->p_usart, UART_IER_RXRDY);
			} else { /* Flexcomm USART */
				usart_enable_interrupt(obj->p_usart, US_IER_RXRDY);
			}
		} else { /* TX */
			if (!obj->is_usart) {
				uart_enable_interrupt((Uart *)obj->p_usart, UART_IER_TXRDY);
			} else { /* Flexcomm USART */
				usart_enable_interrupt(obj->p_usart, US_IER_TXRDY);
			}
		}
		
		NVIC_ClearPendingIRQ(_serial_get_irq_index(obj));
		NVIC_SetPriority(_serial_get_irq_index(obj), 1);
		NVIC_EnableIRQ(_serial_get_irq_index(obj));
	} else { /* Disable */
		if (irq == RxIrq) { /* RX */
			if (!obj->is_usart) {
				uart_disable_interrupt((Uart *)obj->p_usart, UART_IDR_RXRDY);
			} else { /* Flexcomm USART */
				usart_disable_interrupt(obj->p_usart, US_IDR_RXRDY);
			}
		} else { /* TX */
			if (!obj->is_usart) {
				uart_disable_interrupt((Uart *)obj->p_usart, UART_IDR_TXRDY);
			} else { /* Flexcomm USART */
				usart_disable_interrupt(obj->p_usart, US_IDR_TXRDY);
			}
		}

		NVIC_DisableIRQ(_serial_get_irq_index(obj));
	}
}

/******************************************************************************
 *                               READ/WRITE                                   *
 ******************************************************************************/

/** Get character. This is a blocking call, waiting for a character
 *
 * @param obj The serial object
 */
int serial_getc(serial_t *obj)
{
	uint8_t data;
	usart_serial_getchar((usart_if)obj->p_usart, &data);
	return data;
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param obj The serial object
 * @param c   The character to be sent
 */
void serial_putc(serial_t *obj, int c)
{
	usart_serial_putchar((usart_if)obj->p_usart, (uint8_t)c);
}

/** Check if the serial peripheral is readable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be read, 0 if nothing to read
 */
int serial_readable(serial_t *obj)
{
	return usart_serial_is_rx_ready((usart_if)obj->p_usart);
}

/** Check if the serial peripheral is writable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be written, 0 otherwise.
 */
int serial_writable(serial_t *obj)
{
	return usart_serial_is_tx_ready((usart_if)obj->p_usart);
}

/** Clear the serial peripheral
 *
 * @param obj The serial object
 */
void serial_clear(serial_t *obj)
{
    /* Interrupts automatically clear when condition is not met anymore */
}

/** Set the break
 *
 * @param obj The serial object
 */
void serial_break_set(serial_t *obj)
{
	/* Only in Flexcomm USART */
	if (obj->is_usart) {
		usart_start_tx_break((usart_if)obj->p_usart);
	}
}

/** Clear the break
 *
 * @param obj The serial object
 */
void serial_break_clear(serial_t *obj)
{
	/* Only in Flexcomm USART */
	if (obj->is_usart) {
		usart_stop_tx_break((usart_if)obj->p_usart);
	}
}

/** Configure the TX pin for UART function.
 *
 * @param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx)
{
    ioport_mode_t mode = (ioport_mode_t)pinmap_function(tx, PinMap_UART_TX);
    ioport_set_pin_mode(tx, mode);
}

/** Get the pins that support Serial TX
 *
 * Return a PinMap array of pins that support Serial TX. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}

/** Get the pins that support Serial RX
 *
 * Return a PinMap array of pins that support Serial RX. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_rx_pinmap()
{
    return PinMap_UART_RX;
}

#endif //DEVICE_SERIAL
