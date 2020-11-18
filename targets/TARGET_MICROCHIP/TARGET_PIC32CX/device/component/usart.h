/**
 * \file
 *
 * Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _PIC32CX_USART_COMPONENT_
#define _PIC32CX_USART_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Universal Synchronous Asynchronous Receiver Transmitter */
/* ============================================================================= */
/** \addtogroup PIC32CX_USART Universal Synchronous Asynchronous Receiver Transmitter */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Usart hardware registers */
typedef struct {
  __O  uint32_t US_CR;         /**< \brief (Usart Offset: 0x00) USART Control Register */
  __IO uint32_t US_MR;         /**< \brief (Usart Offset: 0x04) USART Mode Register */
  __O  uint32_t US_IER;        /**< \brief (Usart Offset: 0x08) USART Interrupt Enable Register */
  __O  uint32_t US_IDR;        /**< \brief (Usart Offset: 0x0C) USART Interrupt Disable Register */
  __I  uint32_t US_IMR;        /**< \brief (Usart Offset: 0x10) USART Interrupt Mask Register */
  __I  uint32_t US_CSR;        /**< \brief (Usart Offset: 0x14) USART Channel Status Register */
  __I  uint32_t US_RHR;        /**< \brief (Usart Offset: 0x18) USART Receive Holding Register */
  __O  uint32_t US_THR;        /**< \brief (Usart Offset: 0x1C) USART Transmit Holding Register */
  __IO uint32_t US_BRGR;       /**< \brief (Usart Offset: 0x20) USART Baud Rate Generator Register */
  __IO uint32_t US_RTOR;       /**< \brief (Usart Offset: 0x24) USART Receiver Timeout Register */
  __IO uint32_t US_TTGR;       /**< \brief (Usart Offset: 0x28) USART Transmitter Timeguard Register */
  __I  uint32_t Reserved1[5];
  __IO uint32_t US_FIDI;       /**< \brief (Usart Offset: 0x40) USART FI DI Ratio Register */
  __I  uint32_t US_NER;        /**< \brief (Usart Offset: 0x44) USART Number of Errors Register */
  __I  uint32_t Reserved2[1];
  __IO uint32_t US_IF;         /**< \brief (Usart Offset: 0x4C) USART IrDA Filter Register */
  __IO uint32_t US_MAN;        /**< \brief (Usart Offset: 0x50) USART Manchester Configuration Register */
  __IO uint32_t US_LINMR;      /**< \brief (Usart Offset: 0x54) USART LIN Mode Register */
  __IO uint32_t US_LINIR;      /**< \brief (Usart Offset: 0x58) USART LIN Identifier Register */
  __I  uint32_t US_LINBRR;     /**< \brief (Usart Offset: 0x5C) USART LIN Baud Rate Register */
  __I  uint32_t Reserved3[12];
  __IO uint32_t US_CMPR;       /**< \brief (Usart Offset: 0x90) USART Comparison Register */
  __I  uint32_t Reserved4[3];
  __IO uint32_t US_FMR;        /**< \brief (Usart Offset: 0xA0) USART FIFO Mode Register */
  __I  uint32_t US_FLR;        /**< \brief (Usart Offset: 0xA4) USART FIFO Level Register */
  __O  uint32_t US_FIER;       /**< \brief (Usart Offset: 0xA8) USART FIFO Interrupt Enable Register */
  __O  uint32_t US_FIDR;       /**< \brief (Usart Offset: 0xAC) USART FIFO Interrupt Disable Register */
  __I  uint32_t US_FIMR;       /**< \brief (Usart Offset: 0xB0) USART FIFO Interrupt Mask Register */
  __I  uint32_t US_FESR;       /**< \brief (Usart Offset: 0xB4) USART FIFO Event Status Register */
  __I  uint32_t Reserved5[11];
  __IO uint32_t US_WPMR;       /**< \brief (Usart Offset: 0xE4) USART Write Protection Mode Register */
  __I  uint32_t US_WPSR;       /**< \brief (Usart Offset: 0xE8) USART Write Protection Status Register */
  __I  uint32_t Reserved6[5];
  __IO uint32_t US_RPR;        /**< \brief (Usart Offset: 0x100) Receive Pointer Register */
  __IO uint32_t US_RCR;        /**< \brief (Usart Offset: 0x104) Receive Counter Register */
  __IO uint32_t US_TPR;        /**< \brief (Usart Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t US_TCR;        /**< \brief (Usart Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t US_RNPR;       /**< \brief (Usart Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t US_RNCR;       /**< \brief (Usart Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t US_TNPR;       /**< \brief (Usart Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t US_TNCR;       /**< \brief (Usart Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t US_PTCR;       /**< \brief (Usart Offset: 0x120) Transfer Control Register */
  __I  uint32_t US_PTSR;       /**< \brief (Usart Offset: 0x124) Transfer Status Register */
} Usart;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- US_CR : (USART Offset: 0x00) USART Control Register -------- */
#define US_CR_RSTRX (0x1u << 2) /**< \brief (US_CR) Reset Receiver */
#define US_CR_RSTTX (0x1u << 3) /**< \brief (US_CR) Reset Transmitter */
#define US_CR_RXEN (0x1u << 4) /**< \brief (US_CR) Receiver Enable */
#define US_CR_RXDIS (0x1u << 5) /**< \brief (US_CR) Receiver Disable */
#define US_CR_TXEN (0x1u << 6) /**< \brief (US_CR) Transmitter Enable */
#define US_CR_TXDIS (0x1u << 7) /**< \brief (US_CR) Transmitter Disable */
#define US_CR_RSTSTA (0x1u << 8) /**< \brief (US_CR) Reset Status Bits */
#define US_CR_STTBRK (0x1u << 9) /**< \brief (US_CR) Start Break */
#define US_CR_STPBRK (0x1u << 10) /**< \brief (US_CR) Stop Break */
#define US_CR_STTTO (0x1u << 11) /**< \brief (US_CR) Clear TIMEOUT Flag and Start Timeout After Next Character Received */
#define US_CR_SENDA (0x1u << 12) /**< \brief (US_CR) Send Address */
#define US_CR_RSTIT (0x1u << 13) /**< \brief (US_CR) Reset Iterations */
#define US_CR_RSTNACK (0x1u << 14) /**< \brief (US_CR) Reset Non Acknowledge */
#define US_CR_RETTO (0x1u << 15) /**< \brief (US_CR) Start Timeout Immediately */
#define US_CR_RTSEN (0x1u << 18) /**< \brief (US_CR) Request to Send Enable */
#define US_CR_RTSDIS (0x1u << 19) /**< \brief (US_CR) Request to Send Disable */
#define US_CR_LINABT (0x1u << 20) /**< \brief (US_CR) Abort LIN Transmission */
#define US_CR_LINWKUP (0x1u << 21) /**< \brief (US_CR) Send LIN Wakeup Signal */
#define US_CR_TXFCLR (0x1u << 24) /**< \brief (US_CR) Transmit FIFO Clear */
#define US_CR_RXFCLR (0x1u << 25) /**< \brief (US_CR) Receive FIFO Clear */
#define US_CR_TXFLCLR (0x1u << 26) /**< \brief (US_CR) Transmit FIFO Lock CLEAR */
#define US_CR_REQCLR (0x1u << 28) /**< \brief (US_CR) Request to Clear the Comparison Trigger */
#define US_CR_FIFOEN (0x1u << 30) /**< \brief (US_CR) FIFO Enable */
#define US_CR_FIFODIS (0x1u << 31) /**< \brief (US_CR) FIFO Disable */
/* -------- US_MR : (USART Offset: 0x04) USART Mode Register -------- */
#define US_MR_USART_MODE_Pos 0
#define US_MR_USART_MODE_Msk (0xfu << US_MR_USART_MODE_Pos) /**< \brief (US_MR) USART Mode of Operation */
#define US_MR_USART_MODE(value) ((US_MR_USART_MODE_Msk & ((value) << US_MR_USART_MODE_Pos)))
#define   US_MR_USART_MODE_NORMAL (0x0u << 0) /**< \brief (US_MR) Normal mode */
#define   US_MR_USART_MODE_RS485 (0x1u << 0) /**< \brief (US_MR) RS485 */
#define   US_MR_USART_MODE_HW_HANDSHAKING (0x2u << 0) /**< \brief (US_MR) Hardware handshaking */
#define   US_MR_USART_MODE_IS07816_T_0 (0x4u << 0) /**< \brief (US_MR) IS07816 Protocol: T = 0 */
#define   US_MR_USART_MODE_IS07816_T_1 (0x6u << 0) /**< \brief (US_MR) IS07816 Protocol: T = 1 */
#define   US_MR_USART_MODE_IRDA (0x8u << 0) /**< \brief (US_MR) IrDA */
#define   US_MR_USART_MODE_LIN_MASTER (0xAu << 0) /**< \brief (US_MR) LIN Master mode */
#define   US_MR_USART_MODE_LIN_SLAVE (0xBu << 0) /**< \brief (US_MR) LIN Slave mode */
#define   US_MR_USART_MODE_DATA16BIT_MASTER (0xCu << 0) /**< \brief (US_MR) 16-bit data master */
#define   US_MR_USART_MODE_DATA16BIT_SLAVE (0xDu << 0) /**< \brief (US_MR) 16-bit data slave */
#define US_MR_USCLKS_Pos 4
#define US_MR_USCLKS_Msk (0x3u << US_MR_USCLKS_Pos) /**< \brief (US_MR) Clock Selection */
#define US_MR_USCLKS(value) ((US_MR_USCLKS_Msk & ((value) << US_MR_USCLKS_Pos)))
#define   US_MR_USCLKS_MCK (0x0u << 4) /**< \brief (US_MR) Peripheral clock is selected */
#define   US_MR_USCLKS_DIV (0x1u << 4) /**< \brief (US_MR) Peripheral clock divided (DIV = 8) is selected */
#define   US_MR_USCLKS_GCLK (0x2u << 4) /**< \brief (US_MR) PMC generic clock is selected. If the SCK pin is driven (CLKO = 1), the CD field must be greater than 1. */
#define   US_MR_USCLKS_SCK (0x3u << 4) /**< \brief (US_MR) External pin SCK is selected */
#define US_MR_CHRL_Pos 6
#define US_MR_CHRL_Msk (0x3u << US_MR_CHRL_Pos) /**< \brief (US_MR) Character Length */
#define US_MR_CHRL(value) ((US_MR_CHRL_Msk & ((value) << US_MR_CHRL_Pos)))
#define   US_MR_CHRL_5_BIT (0x0u << 6) /**< \brief (US_MR) Character length is 5 bits */
#define   US_MR_CHRL_6_BIT (0x1u << 6) /**< \brief (US_MR) Character length is 6 bits */
#define   US_MR_CHRL_7_BIT (0x2u << 6) /**< \brief (US_MR) Character length is 7 bits */
#define   US_MR_CHRL_8_BIT (0x3u << 6) /**< \brief (US_MR) Character length is 8 bits */
#define US_MR_SYNC (0x1u << 8) /**< \brief (US_MR) Synchronous Mode Select */
#define US_MR_PAR_Pos 9
#define US_MR_PAR_Msk (0x7u << US_MR_PAR_Pos) /**< \brief (US_MR) Parity Type */
#define US_MR_PAR(value) ((US_MR_PAR_Msk & ((value) << US_MR_PAR_Pos)))
#define   US_MR_PAR_EVEN (0x0u << 9) /**< \brief (US_MR) Even parity */
#define   US_MR_PAR_ODD (0x1u << 9) /**< \brief (US_MR) Odd parity */
#define   US_MR_PAR_SPACE (0x2u << 9) /**< \brief (US_MR) Parity forced to 0 (Space) */
#define   US_MR_PAR_MARK (0x3u << 9) /**< \brief (US_MR) Parity forced to 1 (Mark) */
#define   US_MR_PAR_NO (0x4u << 9) /**< \brief (US_MR) No parity */
#define   US_MR_PAR_MULTIDROP (0x6u << 9) /**< \brief (US_MR) Multidrop mode */
#define US_MR_NBSTOP_Pos 12
#define US_MR_NBSTOP_Msk (0x3u << US_MR_NBSTOP_Pos) /**< \brief (US_MR) Number of Stop Bits */
#define US_MR_NBSTOP(value) ((US_MR_NBSTOP_Msk & ((value) << US_MR_NBSTOP_Pos)))
#define   US_MR_NBSTOP_1_BIT (0x0u << 12) /**< \brief (US_MR) 1 stop bit */
#define   US_MR_NBSTOP_1_5_BIT (0x1u << 12) /**< \brief (US_MR) 1.5 stop bit (SYNC = 0) or reserved (SYNC = 1) */
#define   US_MR_NBSTOP_2_BIT (0x2u << 12) /**< \brief (US_MR) 2 stop bits */
#define US_MR_CHMODE_Pos 14
#define US_MR_CHMODE_Msk (0x3u << US_MR_CHMODE_Pos) /**< \brief (US_MR) Channel Mode */
#define US_MR_CHMODE(value) ((US_MR_CHMODE_Msk & ((value) << US_MR_CHMODE_Pos)))
#define   US_MR_CHMODE_NORMAL (0x0u << 14) /**< \brief (US_MR) Normal mode */
#define   US_MR_CHMODE_AUTOMATIC (0x1u << 14) /**< \brief (US_MR) Automatic Echo. Receiver input is connected to the TXD pin. */
#define   US_MR_CHMODE_LOCAL_LOOPBACK (0x2u << 14) /**< \brief (US_MR) Local Loopback. Transmitter output is connected to the Receiver Input. */
#define   US_MR_CHMODE_REMOTE_LOOPBACK (0x3u << 14) /**< \brief (US_MR) Remote Loopback. RXD pin is internally connected to the TXD pin. */
#define US_MR_MSBF (0x1u << 16) /**< \brief (US_MR) Bit Order */
#define US_MR_MODE9 (0x1u << 17) /**< \brief (US_MR) 9-bit Character Length */
#define US_MR_CLKO (0x1u << 18) /**< \brief (US_MR) Clock Output Select */
#define US_MR_OVER (0x1u << 19) /**< \brief (US_MR) Oversampling Mode */
#define US_MR_INACK (0x1u << 20) /**< \brief (US_MR) Inhibit Non Acknowledge */
#define US_MR_DSNACK (0x1u << 21) /**< \brief (US_MR) Disable Successive NACK */
#define US_MR_VAR_SYNC (0x1u << 22) /**< \brief (US_MR) Variable Synchronization of Command/Data Sync Start Frame Delimiter */
#define US_MR_INVDATA (0x1u << 23) /**< \brief (US_MR) Inverted Data */
#define US_MR_MAX_ITERATION_Pos 24
#define US_MR_MAX_ITERATION_Msk (0x7u << US_MR_MAX_ITERATION_Pos) /**< \brief (US_MR) Maximum Number of Automatic Iteration */
#define US_MR_MAX_ITERATION(value) ((US_MR_MAX_ITERATION_Msk & ((value) << US_MR_MAX_ITERATION_Pos)))
#define US_MR_FILTER (0x1u << 28) /**< \brief (US_MR) Receive Line Filter */
#define US_MR_MAN (0x1u << 29) /**< \brief (US_MR) Manchester Encoder/Decoder Enable */
#define US_MR_MODSYNC (0x1u << 30) /**< \brief (US_MR) Manchester Synchronization Mode */
#define US_MR_ONEBIT (0x1u << 31) /**< \brief (US_MR) Start Frame Delimiter Selector */
#define US_MR_OOKRXD (0x1u << 20) /**< \brief (US_MR) OOK Demodulation Input Selection */
#define US_MR_OOKEN (0x1u << 21) /**< \brief (US_MR) OOK Modulation/Demodulation Enabled */
/* -------- US_IER : (USART Offset: 0x08) USART Interrupt Enable Register -------- */
#define US_IER_RXRDY (0x1u << 0) /**< \brief (US_IER) RXRDY Interrupt Enable */
#define US_IER_TXRDY (0x1u << 1) /**< \brief (US_IER) TXRDY Interrupt Enable */
#define US_IER_RXBRK (0x1u << 2) /**< \brief (US_IER) Receiver Break Interrupt Enable */
#define US_IER_ENDRX (0x1u << 3) /**< \brief (US_IER) End of Receive Transfer Interrupt Enable (available in all USART modes of operation) */
#define US_IER_ENDTX (0x1u << 4) /**< \brief (US_IER) End of Transmit Interrupt Enable (available in all USART modes of operation) */
#define US_IER_OVRE (0x1u << 5) /**< \brief (US_IER) Overrun Error Interrupt Enable */
#define US_IER_FRAME (0x1u << 6) /**< \brief (US_IER) Framing Error Interrupt Enable */
#define US_IER_PARE (0x1u << 7) /**< \brief (US_IER) Parity Error Interrupt Enable */
#define US_IER_TIMEOUT (0x1u << 8) /**< \brief (US_IER) Timeout Interrupt Enable */
#define US_IER_TXEMPTY (0x1u << 9) /**< \brief (US_IER) TXEMPTY Interrupt Enable */
#define US_IER_ITER (0x1u << 10) /**< \brief (US_IER) Max number of Repetitions Reached Interrupt Enable */
#define US_IER_TXBUFE (0x1u << 11) /**< \brief (US_IER) Buffer Empty Interrupt Enable (available in all USART modes of operation) */
#define US_IER_RXBUFF (0x1u << 12) /**< \brief (US_IER) Buffer Full Interrupt Enable (available in all USART modes of operation) */
#define US_IER_NACK (0x1u << 13) /**< \brief (US_IER) Non Acknowledge Interrupt Enable */
#define US_IER_CTSIC (0x1u << 19) /**< \brief (US_IER) Clear to Send Input Change Interrupt Enable */
#define US_IER_CMP (0x1u << 22) /**< \brief (US_IER) Comparison Interrupt Enable */
#define US_IER_MANE (0x1u << 24) /**< \brief (US_IER) Manchester Error Interrupt Enable */
#define US_IER_LINBK (0x1u << 13) /**< \brief (US_IER) LIN Break Sent or LIN Break Received Interrupt Enable */
#define US_IER_LINID (0x1u << 14) /**< \brief (US_IER) LIN Identifier Sent or LIN Identifier Received Interrupt Enable */
#define US_IER_LINTC (0x1u << 15) /**< \brief (US_IER) LIN Transfer Completed Interrupt Enable */
#define US_IER_LINBE (0x1u << 25) /**< \brief (US_IER) LIN Bus Error Interrupt Enable */
#define US_IER_LINISFE (0x1u << 26) /**< \brief (US_IER) LIN Inconsistent Synch Field Error Interrupt Enable */
#define US_IER_LINIPE (0x1u << 27) /**< \brief (US_IER) LIN Identifier Parity Interrupt Enable */
#define US_IER_LINCE (0x1u << 28) /**< \brief (US_IER) LIN Checksum Error Interrupt Enable */
#define US_IER_LINSNRE (0x1u << 29) /**< \brief (US_IER) LIN Slave Not Responding Error Interrupt Enable */
#define US_IER_LINSTE (0x1u << 30) /**< \brief (US_IER) LIN Synch Tolerance Error Interrupt Enable */
#define US_IER_LINHTE (0x1u << 31) /**< \brief (US_IER) LIN Header Timeout Error Interrupt Enable */
/* -------- US_IDR : (USART Offset: 0x0C) USART Interrupt Disable Register -------- */
#define US_IDR_RXRDY (0x1u << 0) /**< \brief (US_IDR) RXRDY Interrupt Disable */
#define US_IDR_TXRDY (0x1u << 1) /**< \brief (US_IDR) TXRDY Interrupt Disable */
#define US_IDR_RXBRK (0x1u << 2) /**< \brief (US_IDR) Receiver Break Interrupt Disable */
#define US_IDR_ENDRX (0x1u << 3) /**< \brief (US_IDR) End of Receive Transfer Interrupt Disable (available in all USART modes of operation) */
#define US_IDR_ENDTX (0x1u << 4) /**< \brief (US_IDR) End of Transmit Interrupt Disable (available in all USART modes of operation) */
#define US_IDR_OVRE (0x1u << 5) /**< \brief (US_IDR) Overrun Error Interrupt Disable */
#define US_IDR_FRAME (0x1u << 6) /**< \brief (US_IDR) Framing Error Interrupt Disable */
#define US_IDR_PARE (0x1u << 7) /**< \brief (US_IDR) Parity Error Interrupt Disable */
#define US_IDR_TIMEOUT (0x1u << 8) /**< \brief (US_IDR) Timeout Interrupt Disable */
#define US_IDR_TXEMPTY (0x1u << 9) /**< \brief (US_IDR) TXEMPTY Interrupt Disable */
#define US_IDR_ITER (0x1u << 10) /**< \brief (US_IDR) Max Number of Repetitions Reached Interrupt Disable */
#define US_IDR_TXBUFE (0x1u << 11) /**< \brief (US_IDR) Buffer Empty Interrupt Disable (available in all USART modes of operation) */
#define US_IDR_RXBUFF (0x1u << 12) /**< \brief (US_IDR) Buffer Full Interrupt Disable (available in all USART modes of operation) */
#define US_IDR_NACK (0x1u << 13) /**< \brief (US_IDR) Non Acknowledge Interrupt Disable */
#define US_IDR_CTSIC (0x1u << 19) /**< \brief (US_IDR) Clear to Send Input Change Interrupt Disable */
#define US_IDR_CMP (0x1u << 22) /**< \brief (US_IDR) Comparison Interrupt Disable */
#define US_IDR_MANE (0x1u << 24) /**< \brief (US_IDR) Manchester Error Interrupt Disable */
#define US_IDR_LINBK (0x1u << 13) /**< \brief (US_IDR) LIN Break Sent or LIN Break Received Interrupt Disable */
#define US_IDR_LINID (0x1u << 14) /**< \brief (US_IDR) LIN Identifier Sent or LIN Identifier Received Interrupt Disable */
#define US_IDR_LINTC (0x1u << 15) /**< \brief (US_IDR) LIN Transfer Completed Interrupt Disable */
#define US_IDR_LINBE (0x1u << 25) /**< \brief (US_IDR) LIN Bus Error Interrupt Disable */
#define US_IDR_LINISFE (0x1u << 26) /**< \brief (US_IDR) LIN Inconsistent Synch Field Error Interrupt Disable */
#define US_IDR_LINIPE (0x1u << 27) /**< \brief (US_IDR) LIN Identifier Parity Interrupt Disable */
#define US_IDR_LINCE (0x1u << 28) /**< \brief (US_IDR) LIN Checksum Error Interrupt Disable */
#define US_IDR_LINSNRE (0x1u << 29) /**< \brief (US_IDR) LIN Slave Not Responding Error Interrupt Disable */
#define US_IDR_LINSTE (0x1u << 30) /**< \brief (US_IDR) LIN Synch Tolerance Error Interrupt Disable */
#define US_IDR_LINHTE (0x1u << 31) /**< \brief (US_IDR) LIN Header Timeout Error Interrupt Disable */
/* -------- US_IMR : (USART Offset: 0x10) USART Interrupt Mask Register -------- */
#define US_IMR_RXRDY (0x1u << 0) /**< \brief (US_IMR) RXRDY Interrupt Mask */
#define US_IMR_TXRDY (0x1u << 1) /**< \brief (US_IMR) TXRDY Interrupt Mask */
#define US_IMR_RXBRK (0x1u << 2) /**< \brief (US_IMR) Receiver Break Interrupt Mask */
#define US_IMR_ENDRX (0x1u << 3) /**< \brief (US_IMR) End of Receive Transfer Interrupt Mask (available in all USART modes of operation) */
#define US_IMR_ENDTX (0x1u << 4) /**< \brief (US_IMR) End of Transmit Interrupt Mask (available in all USART modes of operation) */
#define US_IMR_OVRE (0x1u << 5) /**< \brief (US_IMR) Overrun Error Interrupt Mask */
#define US_IMR_FRAME (0x1u << 6) /**< \brief (US_IMR) Framing Error Interrupt Mask */
#define US_IMR_PARE (0x1u << 7) /**< \brief (US_IMR) Parity Error Interrupt Mask */
#define US_IMR_TIMEOUT (0x1u << 8) /**< \brief (US_IMR) Timeout Interrupt Mask */
#define US_IMR_TXEMPTY (0x1u << 9) /**< \brief (US_IMR) TXEMPTY Interrupt Mask */
#define US_IMR_ITER (0x1u << 10) /**< \brief (US_IMR) Max Number of Repetitions Reached Interrupt Mask */
#define US_IMR_TXBUFE (0x1u << 11) /**< \brief (US_IMR) Buffer Empty Interrupt Mask (available in all USART modes of operation) */
#define US_IMR_RXBUFF (0x1u << 12) /**< \brief (US_IMR) Buffer Full Interrupt Mask (available in all USART modes of operation) */
#define US_IMR_NACK (0x1u << 13) /**< \brief (US_IMR) Non Acknowledge Interrupt Mask */
#define US_IMR_CTSIC (0x1u << 19) /**< \brief (US_IMR) Clear to Send Input Change Interrupt Mask */
#define US_IMR_CMP (0x1u << 22) /**< \brief (US_IMR) Comparison Interrupt Mask */
#define US_IMR_MANE (0x1u << 24) /**< \brief (US_IMR) Manchester Error Interrupt Mask */
#define US_IMR_LINBK (0x1u << 13) /**< \brief (US_IMR) LIN Break Sent or LIN Break Received Interrupt Mask */
#define US_IMR_LINID (0x1u << 14) /**< \brief (US_IMR) LIN Identifier Sent or LIN Identifier Received Interrupt Mask */
#define US_IMR_LINTC (0x1u << 15) /**< \brief (US_IMR) LIN Transfer Completed Interrupt Mask */
#define US_IMR_LINBE (0x1u << 25) /**< \brief (US_IMR) LIN Bus Error Interrupt Mask */
#define US_IMR_LINISFE (0x1u << 26) /**< \brief (US_IMR) LIN Inconsistent Synch Field Error Interrupt Mask */
#define US_IMR_LINIPE (0x1u << 27) /**< \brief (US_IMR) LIN Identifier Parity Interrupt Mask */
#define US_IMR_LINCE (0x1u << 28) /**< \brief (US_IMR) LIN Checksum Error Interrupt Mask */
#define US_IMR_LINSNRE (0x1u << 29) /**< \brief (US_IMR) LIN Slave Not Responding Error Interrupt Mask */
#define US_IMR_LINSTE (0x1u << 30) /**< \brief (US_IMR) LIN Synch Tolerance Error Interrupt Mask */
#define US_IMR_LINHTE (0x1u << 31) /**< \brief (US_IMR) LIN Header Timeout Error Interrupt Mask */
/* -------- US_CSR : (USART Offset: 0x14) USART Channel Status Register -------- */
#define US_CSR_RXRDY (0x1u << 0) /**< \brief (US_CSR) Receiver Ready (cleared by reading US_RHR) */
#define US_CSR_TXRDY (0x1u << 1) /**< \brief (US_CSR) Transmitter Ready (cleared by writing US_THR) */
#define US_CSR_RXBRK (0x1u << 2) /**< \brief (US_CSR) Break Received/End of Break */
#define US_CSR_ENDRX (0x1u << 3) /**< \brief (US_CSR) End of RX Buffer (cleared by writing US_RCR or US_RNCR) */
#define US_CSR_ENDTX (0x1u << 4) /**< \brief (US_CSR) End of TX Buffer (cleared by writing US_TCR or US_TNCR) */
#define US_CSR_OVRE (0x1u << 5) /**< \brief (US_CSR) Overrun Error */
#define US_CSR_FRAME (0x1u << 6) /**< \brief (US_CSR) Framing Error */
#define US_CSR_PARE (0x1u << 7) /**< \brief (US_CSR) Parity Error */
#define US_CSR_TIMEOUT (0x1u << 8) /**< \brief (US_CSR) Receiver Timeout */
#define US_CSR_TXEMPTY (0x1u << 9) /**< \brief (US_CSR) Transmitter Empty (cleared by writing US_THR) */
#define US_CSR_ITER (0x1u << 10) /**< \brief (US_CSR) Max Number of Repetitions Reached */
#define US_CSR_TXBUFE (0x1u << 11) /**< \brief (US_CSR) TX Buffer Empty (cleared by writing US_TCR or US_TNCR) */
#define US_CSR_RXBUFF (0x1u << 12) /**< \brief (US_CSR) RX Buffer Full (cleared by writing US_RCR or US_RNCR) */
#define US_CSR_NACK (0x1u << 13) /**< \brief (US_CSR) Non Acknowledge Interrupt */
#define US_CSR_CTSIC (0x1u << 19) /**< \brief (US_CSR) Clear to Send Input Change Flag */
#define US_CSR_CMP (0x1u << 22) /**< \brief (US_CSR) Comparison Status */
#define US_CSR_CTS (0x1u << 23) /**< \brief (US_CSR) Image of CTS Input */
#define US_CSR_MANE (0x1u << 24) /**< \brief (US_CSR) Manchester Error */
#define US_CSR_LINBK (0x1u << 13) /**< \brief (US_CSR) LIN Break Sent or LIN Break Received */
#define US_CSR_LINID (0x1u << 14) /**< \brief (US_CSR) LIN Identifier Sent or LIN Identifier Received */
#define US_CSR_LINTC (0x1u << 15) /**< \brief (US_CSR) LIN Transfer Completed */
#define US_CSR_LINBLS (0x1u << 23) /**< \brief (US_CSR) LIN Bus Line Status */
#define US_CSR_LINBE (0x1u << 25) /**< \brief (US_CSR) LIN Bit Error */
#define US_CSR_LINISFE (0x1u << 26) /**< \brief (US_CSR) LIN Inconsistent Synch Field Error */
#define US_CSR_LINIPE (0x1u << 27) /**< \brief (US_CSR) LIN Identifier Parity Error */
#define US_CSR_LINCE (0x1u << 28) /**< \brief (US_CSR) LIN Checksum Error */
#define US_CSR_LINSNRE (0x1u << 29) /**< \brief (US_CSR) LIN Slave Not Responding Error */
#define US_CSR_LINSTE (0x1u << 30) /**< \brief (US_CSR) LIN Synch Tolerance Error */
#define US_CSR_LINHTE (0x1u << 31) /**< \brief (US_CSR) LIN Header Timeout Error */
/* -------- US_RHR : (USART Offset: 0x18) USART Receive Holding Register -------- */
#define US_RHR_RXCHR_Pos 0
#define US_RHR_RXCHR_Msk (0x1ffu << US_RHR_RXCHR_Pos) /**< \brief (US_RHR) Received Character */
#define US_RHR_RXSYNH (0x1u << 15) /**< \brief (US_RHR) Received Sync */
#define US_RHR_RXCHR0_Pos 0
#define US_RHR_RXCHR0_Msk (0xffu << US_RHR_RXCHR0_Pos) /**< \brief (US_RHR) Received Character */
#define US_RHR_RXCHR1_Pos 8
#define US_RHR_RXCHR1_Msk (0xffu << US_RHR_RXCHR1_Pos) /**< \brief (US_RHR) Received Character */
#define US_RHR_RXCHR2_Pos 16
#define US_RHR_RXCHR2_Msk (0xffu << US_RHR_RXCHR2_Pos) /**< \brief (US_RHR) Received Character */
#define US_RHR_RXCHR3_Pos 24
#define US_RHR_RXCHR3_Msk (0xffu << US_RHR_RXCHR3_Pos) /**< \brief (US_RHR) Received Character */
/* -------- US_THR : (USART Offset: 0x1C) USART Transmit Holding Register -------- */
#define US_THR_TXCHR_Pos 0
#define US_THR_TXCHR_Msk (0x1ffu << US_THR_TXCHR_Pos) /**< \brief (US_THR) Character to be Transmitted */
#define US_THR_TXCHR(value) ((US_THR_TXCHR_Msk & ((value) << US_THR_TXCHR_Pos)))
#define US_THR_TXSYNH (0x1u << 15) /**< \brief (US_THR) Sync Field to be Transmitted */
#define US_THR_TXCHR0_Pos 0
#define US_THR_TXCHR0_Msk (0xffu << US_THR_TXCHR0_Pos) /**< \brief (US_THR) Character to be Transmitted */
#define US_THR_TXCHR0(value) ((US_THR_TXCHR0_Msk & ((value) << US_THR_TXCHR0_Pos)))
#define US_THR_TXCHR1_Pos 8
#define US_THR_TXCHR1_Msk (0xffu << US_THR_TXCHR1_Pos) /**< \brief (US_THR) Character to be Transmitted */
#define US_THR_TXCHR1(value) ((US_THR_TXCHR1_Msk & ((value) << US_THR_TXCHR1_Pos)))
#define US_THR_TXCHR2_Pos 16
#define US_THR_TXCHR2_Msk (0xffu << US_THR_TXCHR2_Pos) /**< \brief (US_THR) Character to be Transmitted */
#define US_THR_TXCHR2(value) ((US_THR_TXCHR2_Msk & ((value) << US_THR_TXCHR2_Pos)))
#define US_THR_TXCHR3_Pos 24
#define US_THR_TXCHR3_Msk (0xffu << US_THR_TXCHR3_Pos) /**< \brief (US_THR) Character to be Transmitted */
#define US_THR_TXCHR3(value) ((US_THR_TXCHR3_Msk & ((value) << US_THR_TXCHR3_Pos)))
/* -------- US_BRGR : (USART Offset: 0x20) USART Baud Rate Generator Register -------- */
#define US_BRGR_CD_Pos 0
#define US_BRGR_CD_Msk (0xffffu << US_BRGR_CD_Pos) /**< \brief (US_BRGR) Clock Divider */
#define US_BRGR_CD(value) ((US_BRGR_CD_Msk & ((value) << US_BRGR_CD_Pos)))
#define US_BRGR_FP_Pos 16
#define US_BRGR_FP_Msk (0x7u << US_BRGR_FP_Pos) /**< \brief (US_BRGR) Fractional Part */
#define US_BRGR_FP(value) ((US_BRGR_FP_Msk & ((value) << US_BRGR_FP_Pos)))
/* -------- US_RTOR : (USART Offset: 0x24) USART Receiver Timeout Register -------- */
#define US_RTOR_TO_Pos 0
#define US_RTOR_TO_Msk (0x1ffffu << US_RTOR_TO_Pos) /**< \brief (US_RTOR) Timeout Value */
#define US_RTOR_TO(value) ((US_RTOR_TO_Msk & ((value) << US_RTOR_TO_Pos)))
/* -------- US_TTGR : (USART Offset: 0x28) USART Transmitter Timeguard Register -------- */
#define US_TTGR_TG_Pos 0
#define US_TTGR_TG_Msk (0xffu << US_TTGR_TG_Pos) /**< \brief (US_TTGR) Timeguard Value */
#define US_TTGR_TG(value) ((US_TTGR_TG_Msk & ((value) << US_TTGR_TG_Pos)))
/* -------- US_FIDI : (USART Offset: 0x40) USART FI DI Ratio Register -------- */
#define US_FIDI_FI_DI_RATIO_Pos 0
#define US_FIDI_FI_DI_RATIO_Msk (0xffffu << US_FIDI_FI_DI_RATIO_Pos) /**< \brief (US_FIDI) FI Over DI Ratio Value */
#define US_FIDI_FI_DI_RATIO(value) ((US_FIDI_FI_DI_RATIO_Msk & ((value) << US_FIDI_FI_DI_RATIO_Pos)))
/* -------- US_NER : (USART Offset: 0x44) USART Number of Errors Register -------- */
#define US_NER_NB_ERRORS_Pos 0
#define US_NER_NB_ERRORS_Msk (0xffu << US_NER_NB_ERRORS_Pos) /**< \brief (US_NER) Number of Errors */
/* -------- US_IF : (USART Offset: 0x4C) USART IrDA Filter Register -------- */
#define US_IF_IRDA_FILTER_Pos 0
#define US_IF_IRDA_FILTER_Msk (0xffu << US_IF_IRDA_FILTER_Pos) /**< \brief (US_IF) IrDA Filter */
#define US_IF_IRDA_FILTER(value) ((US_IF_IRDA_FILTER_Msk & ((value) << US_IF_IRDA_FILTER_Pos)))
/* -------- US_MAN : (USART Offset: 0x50) USART Manchester Configuration Register -------- */
#define US_MAN_TX_PL_Pos 0
#define US_MAN_TX_PL_Msk (0xfu << US_MAN_TX_PL_Pos) /**< \brief (US_MAN) Transmitter Preamble Length */
#define US_MAN_TX_PL(value) ((US_MAN_TX_PL_Msk & ((value) << US_MAN_TX_PL_Pos)))
#define US_MAN_TX_PP_Pos 8
#define US_MAN_TX_PP_Msk (0x3u << US_MAN_TX_PP_Pos) /**< \brief (US_MAN) Transmitter Preamble Pattern */
#define US_MAN_TX_PP(value) ((US_MAN_TX_PP_Msk & ((value) << US_MAN_TX_PP_Pos)))
#define   US_MAN_TX_PP_ALL_ONE (0x0u << 8) /**< \brief (US_MAN) The preamble is composed of '1's */
#define   US_MAN_TX_PP_ALL_ZERO (0x1u << 8) /**< \brief (US_MAN) The preamble is composed of '0's */
#define   US_MAN_TX_PP_ZERO_ONE (0x2u << 8) /**< \brief (US_MAN) The preamble is composed of '01's */
#define   US_MAN_TX_PP_ONE_ZERO (0x3u << 8) /**< \brief (US_MAN) The preamble is composed of '10's */
#define US_MAN_TX_MPOL (0x1u << 12) /**< \brief (US_MAN) Transmitter Manchester Polarity */
#define US_MAN_RX_PL_Pos 16
#define US_MAN_RX_PL_Msk (0xfu << US_MAN_RX_PL_Pos) /**< \brief (US_MAN) Receiver Preamble Length */
#define US_MAN_RX_PL(value) ((US_MAN_RX_PL_Msk & ((value) << US_MAN_RX_PL_Pos)))
#define US_MAN_RX_PP_Pos 24
#define US_MAN_RX_PP_Msk (0x3u << US_MAN_RX_PP_Pos) /**< \brief (US_MAN) Receiver Preamble Pattern detected */
#define US_MAN_RX_PP(value) ((US_MAN_RX_PP_Msk & ((value) << US_MAN_RX_PP_Pos)))
#define   US_MAN_RX_PP_ALL_ONE (0x0u << 24) /**< \brief (US_MAN) The preamble is composed of '1's */
#define   US_MAN_RX_PP_ALL_ZERO (0x1u << 24) /**< \brief (US_MAN) The preamble is composed of '0's */
#define   US_MAN_RX_PP_ZERO_ONE (0x2u << 24) /**< \brief (US_MAN) The preamble is composed of '01's */
#define   US_MAN_RX_PP_ONE_ZERO (0x3u << 24) /**< \brief (US_MAN) The preamble is composed of '10's */
#define US_MAN_RX_MPOL (0x1u << 28) /**< \brief (US_MAN) Receiver Manchester Polarity */
#define US_MAN_ONE (0x1u << 29) /**< \brief (US_MAN) Must Be Set to 1 */
#define US_MAN_DRIFT (0x1u << 30) /**< \brief (US_MAN) Drift Compensation */
#define US_MAN_RXIDLEV (0x1u << 31) /**< \brief (US_MAN) Receiver Idle Value */
/* -------- US_LINMR : (USART Offset: 0x54) USART LIN Mode Register -------- */
#define US_LINMR_NACT_Pos 0
#define US_LINMR_NACT_Msk (0x3u << US_LINMR_NACT_Pos) /**< \brief (US_LINMR) LIN Node Action */
#define US_LINMR_NACT(value) ((US_LINMR_NACT_Msk & ((value) << US_LINMR_NACT_Pos)))
#define   US_LINMR_NACT_PUBLISH (0x0u << 0) /**< \brief (US_LINMR) The USART transmits the response. */
#define   US_LINMR_NACT_SUBSCRIBE (0x1u << 0) /**< \brief (US_LINMR) The USART receives the response. */
#define   US_LINMR_NACT_IGNORE (0x2u << 0) /**< \brief (US_LINMR) The USART does not transmit and does not receive the response. */
#define US_LINMR_PARDIS (0x1u << 2) /**< \brief (US_LINMR) Parity Disable */
#define US_LINMR_CHKDIS (0x1u << 3) /**< \brief (US_LINMR) Checksum Disable */
#define US_LINMR_CHKTYP (0x1u << 4) /**< \brief (US_LINMR) Checksum Type */
#define US_LINMR_DLM (0x1u << 5) /**< \brief (US_LINMR) Data Length Mode */
#define US_LINMR_FSDIS (0x1u << 6) /**< \brief (US_LINMR) Frame Slot Mode Disable */
#define US_LINMR_WKUPTYP (0x1u << 7) /**< \brief (US_LINMR) Wakeup Signal Type */
#define US_LINMR_DLC_Pos 8
#define US_LINMR_DLC_Msk (0xffu << US_LINMR_DLC_Pos) /**< \brief (US_LINMR) Data Length Control */
#define US_LINMR_DLC(value) ((US_LINMR_DLC_Msk & ((value) << US_LINMR_DLC_Pos)))
#define US_LINMR_PDCM (0x1u << 16) /**< \brief (US_LINMR) PDC Mode */
#define US_LINMR_SYNCDIS (0x1u << 17) /**< \brief (US_LINMR) Synchronization Disable */
/* -------- US_LINIR : (USART Offset: 0x58) USART LIN Identifier Register -------- */
#define US_LINIR_IDCHR_Pos 0
#define US_LINIR_IDCHR_Msk (0xffu << US_LINIR_IDCHR_Pos) /**< \brief (US_LINIR) Identifier Character */
#define US_LINIR_IDCHR(value) ((US_LINIR_IDCHR_Msk & ((value) << US_LINIR_IDCHR_Pos)))
/* -------- US_LINBRR : (USART Offset: 0x5C) USART LIN Baud Rate Register -------- */
#define US_LINBRR_LINCD_Pos 0
#define US_LINBRR_LINCD_Msk (0xffffu << US_LINBRR_LINCD_Pos) /**< \brief (US_LINBRR) Clock Divider after Synchronization */
#define US_LINBRR_LINFP_Pos 16
#define US_LINBRR_LINFP_Msk (0x7u << US_LINBRR_LINFP_Pos) /**< \brief (US_LINBRR) Fractional Part after Synchronization */
/* -------- US_CMPR : (USART Offset: 0x90) USART Comparison Register -------- */
#define US_CMPR_VAL1_Pos 0
#define US_CMPR_VAL1_Msk (0x1ffu << US_CMPR_VAL1_Pos) /**< \brief (US_CMPR) First Comparison Value for Received Character */
#define US_CMPR_VAL1(value) ((US_CMPR_VAL1_Msk & ((value) << US_CMPR_VAL1_Pos)))
#define US_CMPR_CMPMODE_Pos 12
#define US_CMPR_CMPMODE_Msk (0x3u << US_CMPR_CMPMODE_Pos) /**< \brief (US_CMPR) Comparison Mode */
#define US_CMPR_CMPMODE(value) ((US_CMPR_CMPMODE_Msk & ((value) << US_CMPR_CMPMODE_Pos)))
#define   US_CMPR_CMPMODE_FLAG_ONLY (0x0u << 12) /**< \brief (US_CMPR) Any character is received and comparison function drives CMP flag. */
#define   US_CMPR_CMPMODE_START_CONDITION (0x1u << 12) /**< \brief (US_CMPR) Comparison condition must be met to start reception. */
#define   US_CMPR_CMPMODE_FILTER (0x2u << 12) /**< \brief (US_CMPR) Comparison must be met to receive the current data only */
#define US_CMPR_CMPPAR (0x1u << 14) /**< \brief (US_CMPR) Compare Parity */
#define US_CMPR_VAL2_Pos 16
#define US_CMPR_VAL2_Msk (0x1ffu << US_CMPR_VAL2_Pos) /**< \brief (US_CMPR) Second Comparison Value for Received Character */
#define US_CMPR_VAL2(value) ((US_CMPR_VAL2_Msk & ((value) << US_CMPR_VAL2_Pos)))
/* -------- US_FMR : (USART Offset: 0xA0) USART FIFO Mode Register -------- */
#define US_FMR_TXRDYM_Pos 0
#define US_FMR_TXRDYM_Msk (0x3u << US_FMR_TXRDYM_Pos) /**< \brief (US_FMR) Transmitter Ready Mode */
#define US_FMR_TXRDYM(value) ((US_FMR_TXRDYM_Msk & ((value) << US_FMR_TXRDYM_Pos)))
#define   US_FMR_TXRDYM_ONE_DATA (0x0u << 0) /**< \brief (US_FMR) TXRDY will be at level '1' when at least one data can be written in the Transmit FIFO */
#define   US_FMR_TXRDYM_TWO_DATA (0x1u << 0) /**< \brief (US_FMR) TXRDY will be at level '1' when at least two data can be written in the Transmit FIFO */
#define   US_FMR_TXRDYM_FOUR_DATA (0x2u << 0) /**< \brief (US_FMR) TXRDY will be at level '1' when at least four data can be written in the Transmit FIFO */
#define US_FMR_RXRDYM_Pos 4
#define US_FMR_RXRDYM_Msk (0x3u << US_FMR_RXRDYM_Pos) /**< \brief (US_FMR) Receiver Ready Mode */
#define US_FMR_RXRDYM(value) ((US_FMR_RXRDYM_Msk & ((value) << US_FMR_RXRDYM_Pos)))
#define   US_FMR_RXRDYM_ONE_DATA (0x0u << 4) /**< \brief (US_FMR) RXRDY will be at level '1' when at least one unread data is in the Receive FIFO */
#define   US_FMR_RXRDYM_TWO_DATA (0x1u << 4) /**< \brief (US_FMR) RXRDY will be at level '1' when at least two unread data are in the Receive FIFO */
#define   US_FMR_RXRDYM_FOUR_DATA (0x2u << 4) /**< \brief (US_FMR) RXRDY will be at level '1' when at least four unread data are in the Receive FIFO */
#define US_FMR_FRTSC (0x1u << 7) /**< \brief (US_FMR) FIFO RTS Pin Control enable (Hardware Handshaking mode only) */
#define US_FMR_TXFTHRES_Pos 8
#define US_FMR_TXFTHRES_Msk (0x3fu << US_FMR_TXFTHRES_Pos) /**< \brief (US_FMR) Transmit FIFO Threshold */
#define US_FMR_TXFTHRES(value) ((US_FMR_TXFTHRES_Msk & ((value) << US_FMR_TXFTHRES_Pos)))
#define US_FMR_RXFTHRES_Pos 16
#define US_FMR_RXFTHRES_Msk (0x3fu << US_FMR_RXFTHRES_Pos) /**< \brief (US_FMR) Receive FIFO Threshold */
#define US_FMR_RXFTHRES(value) ((US_FMR_RXFTHRES_Msk & ((value) << US_FMR_RXFTHRES_Pos)))
#define US_FMR_RXFTHRES2_Pos 24
#define US_FMR_RXFTHRES2_Msk (0x3fu << US_FMR_RXFTHRES2_Pos) /**< \brief (US_FMR) Receive FIFO Threshold 2 */
#define US_FMR_RXFTHRES2(value) ((US_FMR_RXFTHRES2_Msk & ((value) << US_FMR_RXFTHRES2_Pos)))
/* -------- US_FLR : (USART Offset: 0xA4) USART FIFO Level Register -------- */
#define US_FLR_TXFL_Pos 0
#define US_FLR_TXFL_Msk (0x3fu << US_FLR_TXFL_Pos) /**< \brief (US_FLR) Transmit FIFO Level */
#define US_FLR_RXFL_Pos 16
#define US_FLR_RXFL_Msk (0x3fu << US_FLR_RXFL_Pos) /**< \brief (US_FLR) Receive FIFO Level */
/* -------- US_FIER : (USART Offset: 0xA8) USART FIFO Interrupt Enable Register -------- */
#define US_FIER_TXFEF (0x1u << 0) /**< \brief (US_FIER) TXFEF Interrupt Enable */
#define US_FIER_TXFFF (0x1u << 1) /**< \brief (US_FIER) TXFFF Interrupt Enable */
#define US_FIER_TXFTHF (0x1u << 2) /**< \brief (US_FIER) TXFTHF Interrupt Enable */
#define US_FIER_RXFEF (0x1u << 3) /**< \brief (US_FIER) RXFEF Interrupt Enable */
#define US_FIER_RXFFF (0x1u << 4) /**< \brief (US_FIER) RXFFF Interrupt Enable */
#define US_FIER_RXFTHF (0x1u << 5) /**< \brief (US_FIER) RXFTHF Interrupt Enable */
#define US_FIER_TXFPTEF (0x1u << 6) /**< \brief (US_FIER) TXFPTEF Interrupt Enable */
#define US_FIER_RXFPTEF (0x1u << 7) /**< \brief (US_FIER) RXFPTEF Interrupt Enable */
#define US_FIER_RXFTHF2 (0x1u << 9) /**< \brief (US_FIER) RXFTHF2 Interrupt Enable */
/* -------- US_FIDR : (USART Offset: 0xAC) USART FIFO Interrupt Disable Register -------- */
#define US_FIDR_TXFEF (0x1u << 0) /**< \brief (US_FIDR) TXFEF Interrupt Disable */
#define US_FIDR_TXFFF (0x1u << 1) /**< \brief (US_FIDR) TXFFF Interrupt Disable */
#define US_FIDR_TXFTHF (0x1u << 2) /**< \brief (US_FIDR) TXFTHF Interrupt Disable */
#define US_FIDR_RXFEF (0x1u << 3) /**< \brief (US_FIDR) RXFEF Interrupt Disable */
#define US_FIDR_RXFFF (0x1u << 4) /**< \brief (US_FIDR) RXFFF Interrupt Disable */
#define US_FIDR_RXFTHF (0x1u << 5) /**< \brief (US_FIDR) RXFTHF Interrupt Disable */
#define US_FIDR_TXFPTEF (0x1u << 6) /**< \brief (US_FIDR) TXFPTEF Interrupt Disable */
#define US_FIDR_RXFPTEF (0x1u << 7) /**< \brief (US_FIDR) RXFPTEF Interrupt Disable */
#define US_FIDR_RXFTHF2 (0x1u << 9) /**< \brief (US_FIDR) RXFTHF2 Interrupt Disable */
/* -------- US_FIMR : (USART Offset: 0xB0) USART FIFO Interrupt Mask Register -------- */
#define US_FIMR_TXFEF (0x1u << 0) /**< \brief (US_FIMR) TXFEF Interrupt Mask */
#define US_FIMR_TXFFF (0x1u << 1) /**< \brief (US_FIMR) TXFFF Interrupt Mask */
#define US_FIMR_TXFTHF (0x1u << 2) /**< \brief (US_FIMR) TXFTHF Interrupt Mask */
#define US_FIMR_RXFEF (0x1u << 3) /**< \brief (US_FIMR) RXFEF Interrupt Mask */
#define US_FIMR_RXFFF (0x1u << 4) /**< \brief (US_FIMR) RXFFF Interrupt Mask */
#define US_FIMR_RXFTHF (0x1u << 5) /**< \brief (US_FIMR) RXFTHF Interrupt Mask */
#define US_FIMR_TXFPTEF (0x1u << 6) /**< \brief (US_FIMR) TXFPTEF Interrupt Mask */
#define US_FIMR_RXFPTEF (0x1u << 7) /**< \brief (US_FIMR) RXFPTEF Interrupt Mask */
#define US_FIMR_RXFTHF2 (0x1u << 9) /**< \brief (US_FIMR) RXFTHF2 Interrupt Mask */
/* -------- US_FESR : (USART Offset: 0xB4) USART FIFO Event Status Register -------- */
#define US_FESR_TXFEF (0x1u << 0) /**< \brief (US_FESR) Transmit FIFO Empty Flag (cleared by writing the US_CR.RSTSTA bit) */
#define US_FESR_TXFFF (0x1u << 1) /**< \brief (US_FESR) Transmit FIFO Full Flag (cleared by writing the US_CR.RSTSTA bit) */
#define US_FESR_TXFTHF (0x1u << 2) /**< \brief (US_FESR) Transmit FIFO Threshold Flag (cleared by writing the US_CR.RSTSTA bit) */
#define US_FESR_RXFEF (0x1u << 3) /**< \brief (US_FESR) Receive FIFO Empty Flag (cleared by writing the US_CR.RSTSTA bit) */
#define US_FESR_RXFFF (0x1u << 4) /**< \brief (US_FESR) Receive FIFO Full Flag (cleared by writing the US_CR.RSTSTA bit) */
#define US_FESR_RXFTHF (0x1u << 5) /**< \brief (US_FESR) Receive FIFO Threshold Flag (cleared by writing the US_CR.RSTSTA bit) */
#define US_FESR_TXFPTEF (0x1u << 6) /**< \brief (US_FESR) Transmit FIFO Pointer Error Flag */
#define US_FESR_RXFPTEF (0x1u << 7) /**< \brief (US_FESR) Receive FIFO Pointer Error Flag */
#define US_FESR_TXFLOCK (0x1u << 8) /**< \brief (US_FESR) Transmit FIFO Lock */
#define US_FESR_RXFTHF2 (0x1u << 9) /**< \brief (US_FESR) Receive FIFO Threshold Flag 2 (cleared by writing the US_CR.RSTSTA bit) */
/* -------- US_WPMR : (USART Offset: 0xE4) USART Write Protection Mode Register -------- */
#define US_WPMR_WPEN (0x1u << 0) /**< \brief (US_WPMR) Write Protection Enable */
#define US_WPMR_WPITEN (0x1u << 1) /**< \brief (US_WPMR) Write Protection Interrupt Enable */
#define US_WPMR_WPCREN (0x1u << 2) /**< \brief (US_WPMR) Write Protection Control Enable */
#define US_WPMR_WPKEY_Pos 8
#define US_WPMR_WPKEY_Msk (0xffffffu << US_WPMR_WPKEY_Pos) /**< \brief (US_WPMR) Write Protection Key */
#define US_WPMR_WPKEY(value) ((US_WPMR_WPKEY_Msk & ((value) << US_WPMR_WPKEY_Pos)))
#define   US_WPMR_WPKEY_PASSWD (0x555341u << 8) /**< \brief (US_WPMR) Writing any other value in this field aborts the write operation of bits WPEN, WPITEN and WPCREN. Always reads as 0. */
/* -------- US_WPSR : (USART Offset: 0xE8) USART Write Protection Status Register -------- */
#define US_WPSR_WPVS (0x1u << 0) /**< \brief (US_WPSR) Write Protection Violation Status */
#define US_WPSR_WPVSRC_Pos 8
#define US_WPSR_WPVSRC_Msk (0xffffu << US_WPSR_WPVSRC_Pos) /**< \brief (US_WPSR) Write Protection Violation Source */
/* -------- US_RPR : (USART Offset: 0x100) Receive Pointer Register -------- */
#define US_RPR_RXPTR_Pos 0
#define US_RPR_RXPTR_Msk (0xffffffffu << US_RPR_RXPTR_Pos) /**< \brief (US_RPR) Receive Pointer Register */
#define US_RPR_RXPTR(value) ((US_RPR_RXPTR_Msk & ((value) << US_RPR_RXPTR_Pos)))
/* -------- US_RCR : (USART Offset: 0x104) Receive Counter Register -------- */
#define US_RCR_RXCTR_Pos 0
#define US_RCR_RXCTR_Msk (0xffffu << US_RCR_RXCTR_Pos) /**< \brief (US_RCR) Receive Counter Register */
#define US_RCR_RXCTR(value) ((US_RCR_RXCTR_Msk & ((value) << US_RCR_RXCTR_Pos)))
/* -------- US_TPR : (USART Offset: 0x108) Transmit Pointer Register -------- */
#define US_TPR_TXPTR_Pos 0
#define US_TPR_TXPTR_Msk (0xffffffffu << US_TPR_TXPTR_Pos) /**< \brief (US_TPR) Transmit Counter Register */
#define US_TPR_TXPTR(value) ((US_TPR_TXPTR_Msk & ((value) << US_TPR_TXPTR_Pos)))
/* -------- US_TCR : (USART Offset: 0x10C) Transmit Counter Register -------- */
#define US_TCR_TXCTR_Pos 0
#define US_TCR_TXCTR_Msk (0xffffu << US_TCR_TXCTR_Pos) /**< \brief (US_TCR) Transmit Counter Register */
#define US_TCR_TXCTR(value) ((US_TCR_TXCTR_Msk & ((value) << US_TCR_TXCTR_Pos)))
/* -------- US_RNPR : (USART Offset: 0x110) Receive Next Pointer Register -------- */
#define US_RNPR_RXNPTR_Pos 0
#define US_RNPR_RXNPTR_Msk (0xffffffffu << US_RNPR_RXNPTR_Pos) /**< \brief (US_RNPR) Receive Next Pointer */
#define US_RNPR_RXNPTR(value) ((US_RNPR_RXNPTR_Msk & ((value) << US_RNPR_RXNPTR_Pos)))
/* -------- US_RNCR : (USART Offset: 0x114) Receive Next Counter Register -------- */
#define US_RNCR_RXNCTR_Pos 0
#define US_RNCR_RXNCTR_Msk (0xffffu << US_RNCR_RXNCTR_Pos) /**< \brief (US_RNCR) Receive Next Counter */
#define US_RNCR_RXNCTR(value) ((US_RNCR_RXNCTR_Msk & ((value) << US_RNCR_RXNCTR_Pos)))
/* -------- US_TNPR : (USART Offset: 0x118) Transmit Next Pointer Register -------- */
#define US_TNPR_TXNPTR_Pos 0
#define US_TNPR_TXNPTR_Msk (0xffffffffu << US_TNPR_TXNPTR_Pos) /**< \brief (US_TNPR) Transmit Next Pointer */
#define US_TNPR_TXNPTR(value) ((US_TNPR_TXNPTR_Msk & ((value) << US_TNPR_TXNPTR_Pos)))
/* -------- US_TNCR : (USART Offset: 0x11C) Transmit Next Counter Register -------- */
#define US_TNCR_TXNCTR_Pos 0
#define US_TNCR_TXNCTR_Msk (0xffffu << US_TNCR_TXNCTR_Pos) /**< \brief (US_TNCR) Transmit Counter Next */
#define US_TNCR_TXNCTR(value) ((US_TNCR_TXNCTR_Msk & ((value) << US_TNCR_TXNCTR_Pos)))
/* -------- US_PTCR : (USART Offset: 0x120) Transfer Control Register -------- */
#define US_PTCR_RXTEN (0x1u << 0) /**< \brief (US_PTCR) Receiver Transfer Enable */
#define US_PTCR_RXTDIS (0x1u << 1) /**< \brief (US_PTCR) Receiver Transfer Disable */
#define US_PTCR_TXTEN (0x1u << 8) /**< \brief (US_PTCR) Transmitter Transfer Enable */
#define US_PTCR_TXTDIS (0x1u << 9) /**< \brief (US_PTCR) Transmitter Transfer Disable */
#define US_PTCR_RXCBEN (0x1u << 16) /**< \brief (US_PTCR) Receiver Circular Buffer Enable */
#define US_PTCR_RXCBDIS (0x1u << 17) /**< \brief (US_PTCR) Receiver Circular Buffer Disable */
#define US_PTCR_TXCBEN (0x1u << 18) /**< \brief (US_PTCR) Transmitter Circular Buffer Enable */
#define US_PTCR_TXCBDIS (0x1u << 19) /**< \brief (US_PTCR) Transmitter Circular Buffer Disable */
#define US_PTCR_ERRCLR (0x1u << 24) /**< \brief (US_PTCR) Transfer Bus Error Clear */
/* -------- US_PTSR : (USART Offset: 0x124) Transfer Status Register -------- */
#define US_PTSR_RXTEN (0x1u << 0) /**< \brief (US_PTSR) Receiver Transfer Enable */
#define US_PTSR_TXTEN (0x1u << 8) /**< \brief (US_PTSR) Transmitter Transfer Enable */
#define US_PTSR_RXCBEN (0x1u << 16) /**< \brief (US_PTSR) Receiver Transfer Enable */
#define US_PTSR_TXCBEN (0x1u << 18) /**< \brief (US_PTSR) Transmitter Transfer Enable */
#define US_PTSR_ERR (0x1u << 24) /**< \brief (US_PTSR) Transfer Bus Error (clear on read) */

/*@}*/


#endif /* _PIC32CX_USART_COMPONENT_ */
