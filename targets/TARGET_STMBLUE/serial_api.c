/*
 * serial_device.h
 *
 *  Created on: 30 ott 2018
 *      Author: Antonio O.
 */
#ifdef DEVICE_SERIAL

#include "serial_api.h"
#include "serial_device.h"
#include "pin_device.h"
#include "objects.h"


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
	  ------------ USART configuration -------------------
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

/* Function used to protect deep sleep while a serial transmission is on-going.
.* Returns 1 if there is at least 1 serial instance with an on-going transfer
 * and 0 otherwise.
*/
int serial_is_tx_ongoing(void) {
    int TxOngoing = 0;

    if (UART_IsEnabled() && UART_GetFlagStatus(UART_FLAG_BUSY)) {
        TxOngoing |= 1;
    }
    return TxOngoing;
}

#endif //DEVICE_SERIAL
