/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PinNames.h"
#include "mbed_error.h"
#include "gpio_api.h"

#define USI_PTR(ptr) ((S5JS100_USI_UART_TypeDef *)(ptr))
#define UART_PTR(ptr)	((S5JS100_UART_TypeDef *)(ptr))

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/

static const PinMap PinMap_UART_TX[] = {
	{UART_TX0, UART_0, 0},
	{UART_TX1, UART_1, 0},
	{UART_TX2, UART_2, 0},
	{UART_TX3, UART_3, 0},
	{NC, NC, 0}
};
static const PinMap PinMap_UART_RX[] = {
	{UART_RX0, UART_0, 0},
	{UART_RX1, UART_1, 0},
	{UART_RX2, UART_2, 0},
	{UART_RX3, UART_3, 0},
	{NC, NC, 0}
};
#define UART_NUM 4

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
	if ((int)uart == NC) {
		error("Serial pinout mapping failed");
	}

	obj->uart = (void *)uart;

	// make sure every pending clear
	USI_PTR(uart)->UINTM = 0xF;
	USI_PTR(uart)->UINTP = 0xF;

	//set baud rate and enable Uart in normarl mode (RX and TX enabled)
	switch (uart) {
	case UART_0: 
		S5JS100_USI0_UART->UFCON = UART_UFCON_TX_FIFO_RESET; // reset FIFO not to break terminal
		wait_ms(10);
		S5JS100_USI0_UART->UCON = UART_UCON_RX_TOUT_32FRAMES |
								  UART_UCON_TX_INTTYPE_LEVEL | UART_UCON_RX_INTTYPE_LEVEL |
								  UART_UCON_RX_TOUTINT_ENABLE | UART_UCON_RX_ERRINT_DISABLE |
								  UART_UCON_TX_MODE_IRQPOLL | UART_UCON_RX_MODE_IRQPOLL;
		S5JS100_USI0_UART->UFCON = 0x227; // revise me
		S5JS100_USI0_UART->UINTM = UART_UINTM_MODEM_MASK |
									UART_UINTM_TXD_MASK |
								   UART_UINTM_RXD_MASK; // revise me */

		/*   S5JS100_USI0_UART->UFTL_CONF = 0x0;  */
		break;
	case UART_1: 
#if 1	
		/* temporal setting for USI_UART1 */
		////HACK///////////////////////////////////
		putreg32(1, 0x830150C4); /* Reset USI1 */
		putreg32(1, 0x85041038); /* Set USI1 as UART*/
		putreg32(0, 0x830150C4); /* Release USI1 */

		putreg32(0x7301, 0x82021130); /* Set GPI12 as USI1_RXD UART */
		putreg32(0x7301, 0x82021134); /* Set GPI13 as USI1_TXD UART */
		S5JS100_USI1_UART->UFCON =  UART_UFCON_TX_FIFO_RESET | UART_UFCON_RX_FIFO_RESET; // reset FIFO not to break terminal
		S5JS100_USI1_UART->UFCON =  UART_UFCON_FIFO_DISABLE;
#if 1
		/* USI_UART1 is used */
		*(volatile unsigned int *)(0x83015000) = 0x3;
		*(volatile unsigned int *)(0x83015004) = 0x3385;
		*(volatile unsigned int *)(0x83015008) = 0x221;
		*(volatile unsigned int *)(0x8301500C) = 0x0;

		*(volatile unsigned int *)(0x83015010) = 0x6;
		*(volatile unsigned int *)(0x83015014) = 0x0;
		*(volatile unsigned int *)(0x83015018) = 0x0;
		*(volatile unsigned int *)(0x8301501C) = 0x1;

	/*  *(volatile unsigned int *)(0x83014020) = 0x0;  */
	/*  *(volatile unsigned int *)(0x83014024) = 0xE1;  */
		*(volatile unsigned int *)(0x83015028) = 0x6A;
		*(volatile unsigned int *)(0x8301502C) = 0x6;

		*(volatile unsigned int *)(0x83015030) = 0x4;
		*(volatile unsigned int *)(0x83015034) = 0x4;
//		*(volatile unsigned int *)(0x83015038) = 0x5;
		*(volatile unsigned int *)(0x8301503C) = 0x0;

		*(volatile unsigned int *)(0x83015008) = 0x7;
		///////////////////////////////////////
		wait_ms(10);
#endif
		S5JS100_USI1_UART->UINTM = 0xF;//UART_UINTM_TXD_MASK |
								   //UART_UINTM_RXD_MASK; // revise me */


		S5JS100_USI1_UART->UCON = UART_UCON_RX_TOUT_32FRAMES |
								  UART_UCON_TX_INTTYPE_LEVEL | UART_UCON_RX_INTTYPE_LEVEL |
								  UART_UCON_RX_TOUTINT_ENABLE | UART_UCON_RX_ERRINT_DISABLE |
								  UART_UCON_TX_MODE_IRQPOLL | UART_UCON_RX_MODE_IRQPOLL;

		S5JS100_USI1_UART->UINTP = UART_UINTS_MODEM | UART_UINTS_TXD | UART_UINTS_ERROR | UART_UINTS_RXD;

		/*   S5JS100_USI0_UART->UFTL_CONF = 0x0;  */
#endif
		break;
		case UART_2 : {
			S5JS100_UART0->IBRD = 14;
			S5JS100_UART0->FBRD = 7;
			S5JS100_UART0->LCRH = 0x70;
			S5JS100_UART0->CR = 0x301;
			S5JS100_UART0->IFLS = 0x12;
			S5JS100_UART0->ICR = 0x7FF;
			S5JS100_UART0->IMSC = 0x0;
			S5JS100_UART0->MODESEL = 0x0;
		}
		break;
		case UART_3 : {
			S5JS100_UART1->IBRD = 14;
			S5JS100_UART1->FBRD = 7;
			S5JS100_UART1->LCRH = 0x70;
			S5JS100_UART1->CR = 0x301;
			S5JS100_UART1->IFLS = 0x12;
			S5JS100_UART1->ICR = 0x7FF;
			S5JS100_UART1->IMSC = 0x0;
			S5JS100_UART1->MODESEL = 0x0;
		}
		break;

	default:
		break;
	}

	// set default baud rate and format
	serial_baud(obj, 115200);
	/* for USI1 */
	//putreg32(0x508, 0x83015028);
	//putreg32(0x001, 0x8301502C);

	// pinout the chosen uart
	//pinmap_pinout(tx, PinMap_UART_TX);
	//pinmap_pinout(rx, PinMap_UART_RX);

	switch (uart) {
	case UART_0:
		obj->index = 0;
		break;
	case UART_1:
		obj->index = 1;
		break;
	case UART_2:
		obj->index = 2;
		break;
	case UART_3:
		obj->index = 3;
		break;
	}
    uart_data[obj->index].sw_rts.pin = NC;
    uart_data[obj->index].sw_cts.pin = NC;
	//serial_set_flow_control(obj, FlowControlNone, NC, NC);

	is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);

	if (is_stdio_uart) {
		stdio_uart_inited = 1;
		memcpy(&stdio_uart, obj, sizeof(serial_t));
	}
}

void serial_free(serial_t *obj)
{
	uart_data[obj->index].serial_irq_id = 0;
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate)
{
	uint32_t sclk;
	float div, frac;
	switch ((int)obj->uart) {
	case UART_0:
		/* UBRDIV and UFRACVAL */
		sclk = cal_clk_getrate(d1_usi0);

		div = ((float)sclk / (float)(baudrate * 16)) - 1.0;
		frac = (uint32_t)(((div - (int32_t)div) * 16));

		S5JS100_USI0_UART->UBRDIV = (uint32_t)div;
		S5JS100_USI0_UART->UFRACVAL = (uint32_t)frac;
		break;

	case UART_1:
		/* UBRDIV and UFRACVAL */
		sclk = cal_clk_getrate(d1_usi1);

		div = ((float)sclk / (float)(baudrate * 16)) - 1.0;
		frac = (uint32_t)(((div - (int32_t)div) * 16));

		S5JS100_USI1_UART->UBRDIV = (uint32_t)div;
		S5JS100_USI1_UART->UFRACVAL = (uint32_t)frac;
		break;

	case UART_2:
		sclk = cal_clk_getrate(d1_uart0);

		div = ((float)sclk / (float)(baudrate * 16));
		frac = (uint32_t)(((div - (int32_t)div) * 64));

		UART_PTR(obj->uart)->IBRD = (uint32_t)div;
		UART_PTR(obj->uart)->FBRD = (uint32_t)frac;
		UART_PTR(obj->uart)->LCRH = 0x70;
		break;

	case UART_3:
		sclk = cal_clk_getrate(d1_uart1);

		div = ((float)sclk / (float)(baudrate * 16));
		frac = (uint32_t)(((div - (int32_t)div) * 64));

		UART_PTR(obj->uart)->IBRD = (uint32_t)div;
		UART_PTR(obj->uart)->FBRD = (uint32_t)frac;
		UART_PTR(obj->uart)->LCRH = 0x70;
		break;

	default:
		break;
	}
}

void serial_format(serial_t *obj, int data_bits,
				   SerialParity parity, int stop_bits)
{

}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(uint32_t intstatus, uint32_t index,
							UARTName uart)
{
	int irq_type = 0xFFFF; /* type none */

	if (index < 2) {
		if (intstatus & (1 << 0)) {
			irq_type = RxIrq;
		} else if (intstatus & (1 << 1)) {
			// RTS Int
		} else if (intstatus & (1 << 2)) {
			irq_type = TxIrq;
		} else if (intstatus & (1 << 3)) {
			// CTS Int
		} else {
		}
	} else {
		if(!(UART_PTR(uart)->FR & 1u << 5)) {
			if(UART_PTR(uart)->IMSC & 1u << 5) {
				irq_type = TxIrq;
			}
		}

		if(UART_PTR(uart)->MIS & (1u << 4) || UART_PTR(uart)->MIS & (1u << 6)) {
			/* 
				Rx Interrupt & Rx Timeout Interrupt 
				The receive timeout interrupt is asserted when the receive FIFO is not empty, 
				and no further data is received over a 32-bit period. 
			*/
			irq_type = RxIrq;
		}
	}

	if (irq_type  == RxIrq) {
		if (uart_data[index].rx_irq_set_api) {
			(irq_handler[index])(uart_data[index].serial_irq_id, irq_type);
		}
	}

	if (irq_type == TxIrq) {
		/* Clear the TX interrupt Flag */
		/* UART TX */
	} else {
		/* Clear the Rx interupt Flag */
		/* UART RX */
	}

	if (index < 2)
		USI_PTR(uart)->UINTP = intstatus;
}

void uart0_irq()
{
	uart_irq(S5JS100_USI0_UART->UINTP & 0xF, 0, UART_0);
}

void uart1_irq()
{
	uart_irq(S5JS100_USI1_UART->UINTP & 0xF, 1, UART_1);
}

void uart2_irq()
{
	uart_irq(S5JS100_UART0->IMSC, 3, UART_2);
}

void uart3_irq()
{
	uart_irq(S5JS100_UART1->IMSC, 3, UART_3);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
//	irq_handler = handler;
	irq_handler[obj->index] = handler;
	uart_data[obj->index].serial_irq_id = id;
}

void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable)
{
	/* Declare a variable of type IRQn, initialise to 0 */
	IRQn_Type irq_n = (IRQn_Type)0;
	uint32_t vector = 0;
	switch ((int)obj->uart) {
	/*********************************************************************
	 *                  SIDK SOC BOARD                                 *
	 *********************************************************************/
	case UART_0: {
		irq_n = S5JS100_IRQ_USI0;
		vector = (uint32_t)&uart0_irq;
		break;
	}
	case UART_1: {
		irq_n = S5JS100_IRQ_USI1;
		vector = (uint32_t)&uart1_irq;
		break;
	}
	case UART_2: {
		irq_n = S5JS100_IRQ_UART0;
		vector = (uint32_t)&uart2_irq;
		break;
	}
	case UART_3: {
		irq_n = S5JS100_IRQ_UART1;
		vector = (uint32_t)&uart3_irq;
		break;
	}
	default:
		break;
	}

	if (enable) {
		if (irq == TxIrq) {
			/* Transmit IRQ, set appripriate enable */

			/* set TX interrupt enable in CTRL REG */
			//revise me
			//obj->uart->CTRL |= CMSDK_UART_CTRL_TXIRQEN_Msk;
		} else {
			/* set Rx interrupt on in CTRL REG */
			//revise me
			//obj->uart->CTRL |= CMSDK_UART_CTRL_RXIRQEN_Msk;
		}
		NVIC_SetVector(irq_n, vector);
#if defined (__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
		SCB_InvalidateICache();
#endif
		NVIC_EnableIRQ(irq_n);

		/* for external device via UART */
		if((int)obj->uart == UART_1) {
			S5JS100_USI1_UART->UFCON =  UART_UFCON_FIFO_DISABLE;
			USI_PTR(obj->uart)->UINTM &= ~(UART_UINTM_RXD_MASK);
			S5JS100_USI1_UART->UFCON =  0x227;//UART_UFCON_FIFO_DISABLE;
		}
		/* for external device via UART */
		if((int)obj->uart == UART_3) {
		//	UART_PTR(obj->uart)->UINTM &= ~(UART_UINTM_RXD_MASK);
			UART_PTR(obj->uart)->IMSC = 0x250; //interrupt by Rx Timeout & Rx (for fifo mode)
		}

	} else if ((irq == TxIrq) || (uart_data[obj->index].rx_irq_set_api
								  + uart_data[obj->index].rx_irq_set_flow == 0)) {
		/*      Disable   IRQ   */
		int all_disabled = 0;
#if 0//revise me
		SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);

		obj->uart->CTRL &= ~(1 << (irq + 2));

		all_disabled = (obj->uart->CTRL & (1 << (other_irq + 2))) == 0;
#endif
		if (all_disabled) {
			NVIC_DisableIRQ(irq_n);
		}
	}
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
//	 mbed_error_printf("%s\n", __func__);
	if (RxIrq == irq)
		uart_data[obj->index].rx_irq_set_api = enable;
	serial_irq_set_internal(obj, irq, enable);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int c_cnt = 0;
int serial_getc(serial_t *obj)
{
	int data;
	while (!serial_readable(obj));
	if (obj->index < 2)
		data = (USI_PTR(obj->uart)->URXH & 0xFF);
	else
		data = (UART_PTR(obj->uart)->DR & 0xFF);

#ifdef STDIO_UART
	if((int)obj->uart == (int)STDIO_UART) {
		while (!serial_writable(obj));
		if (obj->index < 2)
			USI_PTR(obj->uart)->UTXH = data;
		else
			UART_PTR(obj->uart)->DR = data;
	}
#endif
	return data;
}

void serial_putc(serial_t *obj, int c)
{
	while (!serial_writable(obj));
	if (obj->index < 2)
		USI_PTR(obj->uart)->UTXH = c;
	else
		UART_PTR(obj->uart)->DR = c;
}

int serial_readable(serial_t *obj)
{
	if (obj->index < 2)
		return (USI_PTR(obj->uart)->UFSTAT & UART_UFSTAT_RX_FIFO_COUNT_MASK);
	else
		return !(UART_PTR(obj->uart)->FR & (1u <<4));
}

int serial_writable(serial_t *obj)
{
	if (obj->index < 2)
		while (USI_PTR(obj->uart)->UFSTAT & UART_UFSTAT_TX_FIFO_FULL);
	else
		while (UART_PTR(obj->uart)->FR & (1u << 5));

	return 1;
}

void serial_clear(serial_t *obj)
{
	if (obj->index < 2) {
		USI_PTR(obj->uart)->URXH = 0x00;
		while ((getreg32(0x83015018) & UART_UFSTAT_TX_FIFO_FULL));
		while ((getreg32(0x83015018) & UART_UFSTAT_TX_FIFO_COUNT_MASK));
		putreg8('C', 0x83015020);
		while ((getreg32(0x83015018) & UART_UFSTAT_TX_FIFO_FULL));
		while ((getreg32(0x83015018) & UART_UFSTAT_TX_FIFO_COUNT_MASK));
		putreg8('C', 0x83015020);
		putreg8('C', 0x83015020);
		putreg8('C', 0x83015020);
		putreg8('C', 0x83015020);
		putreg8('C', 0x83015020);
		putreg8('C', 0x83015020);
	} else {
		UART_PTR(obj->uart)->DR = 0x00;
		while (UART_PTR(obj->uart)->FR & (1u << 5));
		while (UART_PTR(obj->uart)->FR & (1u << 7));
		UART_PTR(obj->uart)->DR = 'C';
		while (UART_PTR(obj->uart)->FR & (1u << 5));
		while (UART_PTR(obj->uart)->FR & (1u << 7));
		UART_PTR(obj->uart)->DR = 'C';
		UART_PTR(obj->uart)->DR = 'C';
		UART_PTR(obj->uart)->DR = 'C';
		UART_PTR(obj->uart)->DR = 'C';
		UART_PTR(obj->uart)->DR = 'C';
		UART_PTR(obj->uart)->DR = 'C';
	}
}

void serial_pinout_tx(PinName tx)
{
	pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
}

void serial_break_clear(serial_t *obj)
{
}
void serial_set_flow_control(serial_t *obj, FlowControl type,
							 PinName rxflow, PinName txflow)
{
}

void serial_low_putc(char c)
{
	S5JS100_USI0_UART->UTXH = c; // revise me
}


