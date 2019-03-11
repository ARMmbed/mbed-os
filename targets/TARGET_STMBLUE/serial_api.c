/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if DEVICE_SERIAL

#include "serial_api.h"
#include "device.h"
#include "BlueNRG1_sysCtrl.h"
#include "misc.h"


int stdio_uart_inited = 0;// used in platform/mbed_board.c and platform/mbed_retarget.cpp
serial_t stdio_uart;

static uart_irq_handler irq_handler;
static uint32_t serial_irq_ids = 0;

void serial_init(serial_t *obj, PinName tx, PinName rx){

	//GPIO and UART Peripherals clock enable
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_UART | CLOCK_PERIPH_GPIO, ENABLE);

	//This statement is valid for both BlueNRG1-2, developed under DK 3.0.0
	//GPIO TX config
	switch(tx){
		case USBTX:
			GPIO_InitUartTxPin8();
			break;
		case IO_5:
			GPIO_InitUartTxPin5();
			break;
		default:
			error("The selected is not UART_TX capable. Choose the correct pin.");
			break;
	}
	//GPIO RX config
	switch(rx){
		case USBRX:
			GPIO_InitUartRxPin11();
			break;
		case IO_4:
			GPIO_InitUartRxPin4();
			break;
		default:
			error("The selected is not UART_RX capable. Choose the correct pin.");
			break;
		}
	/*
	  ------------ UART configuration -------------------
	  - BaudRate = 115200 baud
	  - Word Length = 8 Bits
	  - One Stop Bit
	  - No parity
	  - Hardware flow control disabled (RTS and CTS signals)
	  - Receive and transmit enabled
	 */
	UART_StructInit(&obj->serial.init);
	UART_Init(&obj->serial.init);

	obj->serial.uart = UART_1;

	//obj->init = &UART_InitStructure;
	obj->serial.pin_tx = tx;
	obj->serial.pin_rx = rx;

	/* Interrupt as soon as data is received. */
	UART_RxFifoIrqLevelConfig(FIFO_LEV_1_64);

	/* Enable UART */
	UART_Cmd(ENABLE);

	// For stdio management in platform/mbed_board.c and platform/mbed_retarget.cpp
	if (tx==USBTX && rx==USBRX) {
	    stdio_uart_inited = 1;
	    memcpy(&stdio_uart, obj, sizeof(serial_t));
	}
}

void serial_free(serial_t *obj){
	UART_DeInit();
	UART_Cmd(DISABLE);
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_UART, DISABLE);
	obj->serial.index_irq = 0;
	obj->serial.init.UART_BaudRate = 0;
	/**
	 * One has to release also the GPIO assigned to the UART. In this case the
	 * GPIO release is not performed since it has been initialized with pull and
	 * high power mode disable.
	 */
}

void serial_baud(serial_t *obj, int baudrate){
	obj->serial.init.UART_BaudRate = baudrate;
	UART_Init(&obj->serial.init);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits){
	obj->serial.init.UART_WordLengthTransmit = (uint8_t)data_bits;
	obj->serial.init.UART_WordLengthReceive = (uint8_t)data_bits;
	obj->serial.init.UART_Parity = (uint8_t)parity;
	obj->serial.init.UART_StopBits= (uint8_t)stop_bits;
	UART_Init(&obj->serial.init);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id){
    irq_handler = handler;
    serial_irq_ids = id;
    //obj->index_irq = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable){
	/* NVIC configuration */
	NVIC_InitType NVIC_InitStructure;
	/* Enable the UART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
	NVIC_Init(&NVIC_InitStructure);
	//UART_ITConfig(UART_IT_RX, enable);
	if (irq == RxIrq)
		UART_ITConfig(UART_IT_RX, enable);
	else// TxIrq
		UART_ITConfig(UART_IT_TX, enable);
}

int serial_getc(serial_t *obj){
	/* Loop until the UART Receive Data Register is not empty */
	while (UART_GetFlagStatus(UART_FLAG_RXFE) == SET);
	/* Store the received byte in RxBuffer */
	return (int) UART_ReceiveData();
}

void serial_putc(serial_t *obj, int c){
	  /* Wait if TX fifo is full. */
	  while (UART_GetFlagStatus(UART_FLAG_TXFF) == SET);
	  /* send the data */
	  UART_SendData((uint16_t)c);
}

int serial_readable(serial_t *obj){
	// To avoid a target blocking case, let's check for possible OVERRUN error and discard it
	if(UART_GetFlagStatus(UART_FLAG_OE)){
		UART_ClearFlag(UART_FLAG_OE);
	}
	return !UART_GetFlagStatus(UART_FLAG_RXFE);
}

int  serial_writable(serial_t *obj){
	return UART_GetFlagStatus(UART_FLAG_TXFF);
}

void serial_clear(serial_t *obj){}

void serial_break_set(serial_t *obj){}

void serial_break_clear(serial_t *obj){}

void serial_pinout_tx(PinName tx){}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow){}

const PinMap *serial_tx_pinmap(void){}

const PinMap *serial_rx_pinmap(void){}

const PinMap *serial_cts_pinmap(void){}

const PinMap *serial_rts_pinmap(void){}

#if DEVICE_SERIAL_ASYNCH

int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint){}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint){}

uint8_t serial_tx_active(serial_t *obj){}

uint8_t serial_rx_active(serial_t *obj){}

int serial_irq_handler_asynch(serial_t *obj){}

void serial_tx_abort_asynch(serial_t *obj){}

void serial_rx_abort_asynch(serial_t *obj){}

#endif

/* Function used to protect deep sleep while a serial transmission is on-going.
 * Returns 1 if there is at least 1 serial instance with an on-going transfer
 * and 0 otherwise.
 * HANDLED IN sleep_api.c
*/
uint8_t serialTxActive(void) {
	return(UART_GetFlagStatus(UART_FLAG_TXFE) == RESET);
}

#endif //DEVICE_SERIAL
