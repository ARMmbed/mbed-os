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
// math.h required for floating point operations for baud rate calculation
//#include <math.h>
#include <string.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/
#define UART_NUM    1

static const PinMap PinMap_UART_TX[] = {
    {TX_PIN_NUMBER, UART_0, 1},
    { NC  , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {RX_PIN_NUMBER, UART_0, 1},
    {NC   , NC    , 0}
};

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;



void serial_init(serial_t *obj, PinName tx, PinName rx) {
    // determine the UART to use -- for mcu's with multiple uart connections
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    if ((int)uart == NC) {
        error("Serial pinout mapping failed");	   
    }
	
    obj->uart = (NRF_UART_Type *)uart;
	
	//pin configurations -- 
	//outputs	
	NRF_GPIO->DIR |= (1<<tx);//TX_PIN_NUMBER);
	NRF_GPIO->DIR |= (1<<RTS_PIN_NUMBER);

	NRF_GPIO->DIR &= ~(1<<rx);//RX_PIN_NUMBER);
	NRF_GPIO->DIR &= ~(1<<CTS_PIN_NUMBER);
	
	obj->uart->PSELRTS = RTS_PIN_NUMBER;
	obj->uart->PSELTXD = tx;//TX_PIN_NUMBER;
	//inputs
	obj->uart->PSELCTS = CTS_PIN_NUMBER;
	obj->uart->PSELRXD = rx;//RX_PIN_NUMBER;
    
    
    // set default baud rate and format
    serial_baud  (obj, 9600);
    serial_format(obj, 8, ParityNone, 1);
    
	obj->uart->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);;
	obj->uart->TASKS_STARTTX = 1;
	obj->uart->TASKS_STARTRX = 1;
	obj->uart->EVENTS_RXDRDY =0;
    
    // set rx/tx pins in PullUp mode
    pin_mode(tx, PullUp);
    pin_mode(rx, PullUp);
    
    switch (uart) {
        case UART_0: obj->index = 0; break;
    }
    
    
    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj) {
    serial_irq_ids[obj->index] = 0;
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate) {
	switch(baudrate)
	{
		case 1200:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud1200; break;
		case 2400:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud2400; break;
		case 4800:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud4800; break;
		case 9600:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud9600; break;
		case 14400:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud14400; break;
		case 19200:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud19200; break;
		case 28800:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud28800; break;
		case 38400:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud38400; break;
		case 57600:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud57600; break;
		case 76800:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud76800; break;
		case 115200:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud115200; break;
		case 230400:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud230400; break;
		case 250000:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud250000; break;
		case 460800:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud460800; break;
		case 921600:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud921600; break;
		case 1000000:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud1M; break;
		default:obj->uart->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud9600; break;
	}
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    // 0: 1 stop bits, 1: 2 stop bits
   // int parity_enable, parity_select;
    switch (parity) {
        case ParityNone: obj->uart->CONFIG  = 0;break;
        default:
			obj->uart->CONFIG  = (UART_CONFIG_PARITY_Included<<UART_CONFIG_PARITY_Pos);
            return;
    }
	//no Flow Control
}

//******************************************************************************
// * INTERRUPTS HANDLING
//******************************************************************************
static inline void uart_irq(uint32_t iir, uint32_t index) {
    // [Chapter 14] LPC17xx UART0/2/3: UARTn Interrupt Handling
    SerialIrq irq_type;
    switch (iir) {
        case 1: irq_type = TxIrq; break;
        case 2: irq_type = RxIrq; break;
        default: return;
    }
    
    if (serial_irq_ids[index] != 0)
        irq_handler(serial_irq_ids[index], irq_type);
}

void UART0_IRQHandler() 
{
	uint32_t irtype =0;
	if(NRF_UART0->EVENTS_TXDRDY)
		irtype =1;
	else if(NRF_UART0->EVENTS_RXDRDY)
		irtype =2;
	uart_irq(irtype, 0);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    IRQn_Type irq_n = (IRQn_Type)0;
   // uint32_t vector = 0;
    switch ((int)obj->uart) {
        case UART_0: irq_n=UART0_IRQn ; //vector = (uint32_t)&uart0_irq; 
			break;
    }
    
    if (enable) {
		switch (irq) {
			case RxIrq: obj->uart->INTENSET |= (UART_INTENSET_RXDRDY_Msk);break;
			case TxIrq: obj->uart->INTENSET |= (UART_INTENSET_TXDRDY_Msk);break;
		}
        NVIC_EnableIRQ(irq_n);
    }
	else { // disable
		int all_disabled = 0;
		switch (irq) {
            case RxIrq: obj->uart->INTENSET &= ~(UART_INTENSET_RXDRDY_Msk);
						all_disabled = (obj->uart->INTENSET& (UART_INTENSET_TXDRDY_Msk))==0;
			break;
            case TxIrq: obj->uart->INTENSET &= ~(UART_INTENSET_TXDRDY_Msk);
						all_disabled = (obj->uart->INTENSET& (UART_INTENSET_RXDRDY_Msk))==0;
			break;
		}
        if (all_disabled)
            NVIC_DisableIRQ(irq_n);
    }
}

//******************************************************************************
//* READ/WRITE
//******************************************************************************
int serial_getc(serial_t *obj) {
    while (!serial_readable(obj));
    obj->uart->EVENTS_RXDRDY = 0;
	return (uint8_t)obj->uart->RXD;
}

void serial_putc(serial_t *obj, int c) {
    obj->uart->TXD = (uint8_t)c;
	while (!serial_writable(obj));
	obj->uart->EVENTS_TXDRDY =0;
}

int serial_readable(serial_t *obj) {
    return (obj->uart->EVENTS_RXDRDY == 1);
}

int serial_writable(serial_t *obj) {
    return (obj->uart->EVENTS_TXDRDY ==1);
}

void serial_break_set(serial_t *obj) {
    obj->uart->TASKS_SUSPEND = 1;
}

void serial_break_clear(serial_t *obj) {
    obj->uart->TASKS_STARTTX = 1;
	obj->uart->TASKS_STARTRX = 1;
}