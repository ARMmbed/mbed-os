/**
 ******************************************************************************
 * @file uart_16c550_map.h
 * @brief UART module hardware register map.
 * @internal
 * @author ON Semiconductor.
 * $Rev: 2615 $
 * $Date: 2013-12-13 13:17:21 +0530 (Fri, 13 Dec 2013) $
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
 * <h1> Reference document(s) </h1>
 * <p>
 * <a href="../pdf/IPC7202_UART_APB_DS_v1P4.pdf" target="_blank">
 * IPC7202 APB UART Design Specification v1.4 </a>
 * </p>
 */

#ifndef UART_16C550_MAP_H_
#define UART_16C550_MAP_H_

#include "architecture.h"

#if defined ( __CC_ARM )
#pragma anon_unions
#endif

#define DCTS (uint8_t)0x01
#define DDSR (uint8_t)0x02
#define TERI (uint8_t)0x04
#define DDCD (uint8_t)0x08
//#define CTS   (uint8_t)0x10
#define DSR     (uint8_t)0x20
#define RI   (uint8_t)0x40
#define DCD  (uint8_t)0x80
#define IER_PWRDNENACTIVE          ((uint8_t)(1<<5))
#define IER_MSI                    ((uint8_t)(1<<3))
#define IER_RLSI                   ((uint8_t)(1<<2))
#define IER_THRI                   ((uint8_t)(1<<1))
#define IER_RDAI                   ((uint8_t)(1<<0))
#define FCR_RXFIFOTRIGGERLEVEL_1   ((uint8_t)(0x00))
#define FCR_RXFIFOTRIGGERLEVEL_4   ((uint8_t)(0x40))
#define FCR_RXFIFOTRIGGERLEVEL_8   ((uint8_t)(0x80))
#define FCR_RXFIFOTRIGGERLEVEL_14  ((uint8_t)(0xC0))
#define FCR_DMA_MODE_0             ((uint8_t)(0<<3))
#define FCR_DMA_MODE_1             ((uint8_t)(1<<3))
#define FCR_TXFIFO_RESET           ((uint8_t)(1<<2))
#define FCR_RXFIFO_RESET           ((uint8_t)(1<<1))
#define FCR_FIFO_ENABLE            ((uint8_t)(1<<0))

/** UART HW Structure Overlay */
typedef struct {
    /** Base address + 0x0: Receive, transmit and divisor_LSB offset */
    union {
        __I uint32_t RBR; /**< Received data (8 bits wide) / read only */
        __O uint32_t THR; /**< Data to be transmitted (8 bits wide) / write only */
        __IO uint32_t DLL; /**< If DLAB = 1. LS byte for input to baud rate generator */
    };
    /** Base address + 0x4: Interrupt enable and divisor_MSB offset */
    union {
        union {
            struct {
                __IO uint32_t RX_DATA_INT :1; /**< Enables the received data interrupt, write 1 to enable */
                __IO uint32_t TX_HOLD_INT :1; /**< Enables the transmitter holding interrupt, write 1 to enable */
                __IO uint32_t RX_STATUS_INT :1; /**< Enables the receiver line status interrupt, write 1 to enable */
                __IO uint32_t MODEM_STATUS_INT :1; /**< Enables the modem status interrupt, write 1 to enable */
                __IO uint32_t PAD0 :1;
                __IO uint32_t PD_EN :1; /**< Power down enable active bit, write 1 to enable.  Only enabled if PD_EN in MCR is set also */
                __IO uint32_t PAD1 :2;
            } BITS;
            __IO uint32_t WORD;
        } IER; /** Interrupt enable offset 0x04 */
        __IO uint32_t DLM; /**< If DLAB = 1. MS byte for input to baud rate generator */
    };
    /** Base address + 0x8: Interrupt status and fifo control offset*/
    union {
        union {
            struct {
                __I uint32_t INT_PEND :1; /**< Interrupt is pending if 1 */
                __I uint32_t INT_ID :3; /**< Interrupt identification: 011-RX Line, 010-Rx Data, 110-char TO, 001-TX empty, 000-Modem status*/
                __I uint32_t PAD0 :2;
                __I uint32_t FIFO_EN :2; /**< Fifos enabled: 00-disabled, 01/10-undefined, 11-enabled */
            } BITS;
            __I uint32_t WORD;
        } IIR; /** Interrupt status and fifo status offset 0x08 */
        union {
            struct {
                __O uint32_t FIFO_EN :1; /**< FIFO enable, write 1 to enable */
                __O uint32_t RX_FIFO_RST :1; /**< RX FIFO reset, write 1 to reset */
                __O uint32_t TX_FIFO_RST :1; /**< TX FIFO reset, write 1 to reset */
                __O uint32_t DMA_SEL :1; /**< DMA mode select */
                __O uint32_t PAD0 :2;
                __O uint32_t RX_FIFO_TRIG :2; /**< Receiver FIFO trigger level:00-1byte, 01-4bytes, 10-8bytes, 11-14bytes */
            } BITS;
            __O uint32_t WORD;
        } FCR; /** Fifo control offset 0x08 */
    };
    /** Base address + 0xC: Line control offset */
    union {
        struct {
            __IO uint32_t CHAR_LEN :2; /**< Number of bits per character: 00-5bits, 01-6bits, 10:7bits, 11:8bits */
            __IO uint32_t NUM_STOP :1; /**< Number of stop bits: 0-1bit, 1-2bits */
            __IO uint32_t PARITY :3; /**< Parity: xx0-disable, 001-odd, 011-even, 101-stick generated/checked as 1, 111-stick generated/checked as 0 */
            __IO uint32_t BREAK :1; /**< Set to 1 to force output to 0, set to 0 to return to normal operation */
            __IO uint32_t DLAB :1; /**< Set to 1 to enable the DLL, DLM registers at 0x00 and 0x04 */
        } BITS;
        __IO uint32_t WORD;
    } LCR; /** Line control offset 0x0C */
    /** Base address + 0x10: Modem control offset */
    union {
        struct {
            __IO uint32_t DTR :1; /**< Data terminal ready. Write 1 to set DTR high (de-asserted), or read DTR */
            __IO uint32_t RTS :1; /**< Request to send. Write 1 to set RTS high (de-asserted), or read RTS */
            __IO uint32_t OUTN_CTRL :2; /**< Direct control of out2N and out1N */
            __IO uint32_t LOOPBACK :1; /**< Write 1 to enable loop back */
            __IO uint32_t PAD0 :3;
            __IO uint32_t PD_EN :1; /**< Power down enable active bit, write 1 to enable.  Only enabled if PD_EN in IER is set also */
        } BITS;
        __IO uint32_t WORD;
    } MCR; /**< Modem control offset 0x10 */
    /** Base address + 0x14: Line status offset */
    union {
        struct {
            __O uint32_t READY :1; /**< Rx data available */
            __O uint32_t OVERRUN_ERR :1; /**< Overrun error */
            __O uint32_t PARITY_ERR :1; /**< Parity error */
            __O uint32_t FRAME_ERR :1; /**< Framing error */
            __O uint32_t BREAK_INT :1; /**< Break interrupt is set when output is kept to 0 for more than 1 bit time */
            __O uint32_t TX_HOLD_EMPTY :1; /**< Transmit holding register empty */
            __O uint32_t TX_EMPTY :1; /**< Transmitter empty */
            __O uint32_t FIFO_ERR :1; /**< Receive fifo error */
        } BITS;
        __O uint32_t WORD;
    } LSR; /**< Line status offset 0x14 */
    /** Base address + 0x18: Modem status offset */
    union {
        struct {
            __O uint32_t CHG_CTSN :1; /**< CTS change since last MSR read */
            __O uint32_t CHG_DSRN :1; /**< DSR change since last MSR read */
            __O uint32_t CHG_RIN :1; /**< RI change since last MSR read */
            __O uint32_t CHG_DCDN :1; /**< DCD change since last MSR read */
            __O uint32_t CURR_CTSN :1; /**< CTS current state, 0 = asserted, 1 = de-asserted */
            __O uint32_t CURR_DSRN :1; /**< DSR current state */
            __O uint32_t CURR_RIN :1; /**< RI current state */
            __O uint32_t CURR_DCDN :1; /**< DCD current state */
        } BITS;
        __O uint32_t WORD;
    } MSR; /**< Modem status offset 0x18 */
    /** Base address + 0x1C: Scratch offset*/
    __IO uint32_t SCR; /**< Scratch pad register */
} Uart16C550Reg_t, *Uart16C550Reg_pt;

#endif /* UART_16C550_MAP_H_ */
