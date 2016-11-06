/**
 ******************************************************************************
 * @file uart_16c550.h
 * @brief Definitions and API for the 16c550 driver.
 * @internal
 * @author ON Semiconductor
 * $Rev: 2607 $
 * $Date: 2013-12-06 18:02:43 +0530 (Fri, 06 Dec 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup uart_16c550
 *
 * @details
 * The driver for the 16c550 UART is a character driver (see char_driver.h).
 * The driver can be accessed via the uart_16c550_driver variable.
 *
 * This file defines the options structure to be passed to the driver when
 * opening a device. The structure depends on the generic uart_options_t
 * options structure for any UART implementation (see uart.h).
 *
 * Any application that uses this driver must define an interrupt handler
 * for the 16C550 interrupt and call the fUart16C550Handler() function from
 * that ISR.
 */

#ifndef UART_16C550_H_
#define UART_16C550_H_

#include "architecture.h"
#include "uart.h"
#include "uart_16c550_map.h"
#include "memory_map.h"
#include "crossbar.h"
#include "types.h"
#include "clock.h"
#include "pad.h"
#include "serial_api.h"

/** A set of options to be passed when opening a 16C550 UART device. */
typedef struct uart_16c550_options {
    uart_options_t uartOptions; /**< The generic UART options. */
    Uart16C550Reg_pt membase; /**< The memory base for the device's registers. */
    uint8_t irq; /**< The IRQ number of the IRQ associated to the device. */
} uart_16c550_options_t, *uart_16c550_options_pt;

#define UART_NUM        2

#define CTS_ASSERT      1
#define CTS_UNASSERT    0
#define RTS_ASSERT      1
#define RTS_UNASSERT    0

#define UART_ERROR_INSUFFICIENT_SPACE   ((uint8_t)0xF0)
#define UART_ERROR_TOO_BIG              ((uint8_t)0xF1)

/** The depth of the hardware FIFOs. */
#define UART_HW_FIFO_DEPTH        16

/** The length of the receive buffer in software. */
#define UART_RX_BUFFER_LENGTH        (1<<8)
#define UART_TX_BUFFER_LENGTH        (1<<8)

#define STATUS_INVALID_PARAMETER    0x1
#define STATUS_SUCCESS              0x1

#define UART_LCR_DATALEN_BIT_POS    0
#define UART_LCR_STPBIT_BIT_POS     2
#define UART_LCR_PARITY_BIT_POS     3

#define UART_FCS_RX_FIFO_RST_BIT_POS 1
#define UART_FCS_TX_FIFO_RST_BIT_POS 2

#define UART_RX_IRQ        0x0
#define UART_TX_IRQ        0x1

#define UART_RX_BUFFER_LEN_MAX    16

#define UART_LSR_TX_EMPTY_MASK        0x40
#define UART_LSR_RX_DATA_READY_MASK   0x01

#define UART_IER_TX_EMPTY_MASK        0x02
#define UART_IER_RX_DATA_READY_MASK   0x01

#define UART_DEFAULT_BAUD    9600

/** Interrupt handler for 16C550 UART devices; to be called from an actual ISR.
 * @param membase The memory base for the device that corresponds to the IRQ.
 */
void fUart16C550Handler(Uart16C550Reg_pt membase);

/** An externally accessible instance of the UART driver implementation. */
//extern char_driver_t uart_16c550_driver;
typedef void (*fUartCallBack)(void);
//void serial_init(serial_t *obj, PinName tx, PinName rx);
//extern void fSerialInit(Uart16C550Reg_pt UartRegBase, flow_control_t FlowControl);
extern void fSerialFree(void);
extern void fSerialBaud(Uart16C550Reg_pt UartRegBase, uint32_t BaudRate);
extern void fSerialFormat(Uart16C550Reg_pt UartRegBase, uint8_t DataLen, uint8_t Parity, uint8_t StopBit);
extern void fSerialIrqSet(Uart16C550Reg_pt UartRegBase, fUartCallBack PtrUartCallBack, uint8_t IrqType, boolean Enable);
extern uint8_t fSerialGetc(Uart16C550Reg_pt UartRegBase);
extern void fSerialPutc(Uart16C550Reg_pt UartRegBase, uint8_t c);
extern boolean fSerialReadable(Uart16C550Reg_pt UartRegBase);
extern boolean fSerialWritable(Uart16C550Reg_pt UartRegBase);
extern void fSerialClear(Uart16C550Reg_pt UartRegBase);
extern void fSerialBreakSet(Uart16C550Reg_pt UartRegBase);
extern void fSerialBreakClear(Uart16C550Reg_pt UartRegBase);
extern void fSerialPinoutTx(uint8_t PinNo);

extern void Uart1_Irq(void);
extern void Uart2_Irq(void);

#endif /* UART_16C550_H_ */
