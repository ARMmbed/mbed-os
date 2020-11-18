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

#ifndef _PIC32CX_SPI_COMPONENT_
#define _PIC32CX_SPI_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Serial Peripheral Interface */
/* ============================================================================= */
/** \addtogroup PIC32CX_SPI Serial Peripheral Interface */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Spi hardware registers */
typedef struct {
  __O  uint32_t SPI_CR;        /**< \brief (Spi Offset: 0x000) SPI Control Register */
  __IO uint32_t SPI_MR;        /**< \brief (Spi Offset: 0x004) SPI Mode Register */
  __I  uint32_t SPI_RDR;       /**< \brief (Spi Offset: 0x008) SPI Receive Data Register */
  __O  uint32_t SPI_TDR;       /**< \brief (Spi Offset: 0x00C) SPI Transmit Data Register */
  __I  uint32_t SPI_SR;        /**< \brief (Spi Offset: 0x010) SPI Status Register */
  __O  uint32_t SPI_IER;       /**< \brief (Spi Offset: 0x014) SPI Interrupt Enable Register */
  __O  uint32_t SPI_IDR;       /**< \brief (Spi Offset: 0x018) SPI Interrupt Disable Register */
  __I  uint32_t SPI_IMR;       /**< \brief (Spi Offset: 0x01C) SPI Interrupt Mask Register */
  __I  uint32_t Reserved1[4];
  __IO uint32_t SPI_CSR[2];    /**< \brief (Spi Offset: 0x030) SPI Chip Select Register */
  __I  uint32_t Reserved2[2];
  __IO uint32_t SPI_FMR;       /**< \brief (Spi Offset: 0x040) SPI FIFO Mode Register */
  __I  uint32_t SPI_FLR;       /**< \brief (Spi Offset: 0x044) SPI FIFO Level Register */
  __IO uint32_t SPI_CMPR;      /**< \brief (Spi Offset: 0x048) SPI Comparison Register */
  __I  uint32_t Reserved3[38];
  __IO uint32_t SPI_WPMR;      /**< \brief (Spi Offset: 0x0E4) SPI Write Protection Mode Register */
  __I  uint32_t SPI_WPSR;      /**< \brief (Spi Offset: 0x0E8) SPI Write Protection Status Register */
  __I  uint32_t Reserved4[4];
  __I  uint32_t SPI_VERSION;   /**< \brief (Spi Offset: 0x0FC) SPI Version Register */
  __IO uint32_t SPI_RPR;       /**< \brief (Spi Offset: 0x100) Receive Pointer Register */
  __IO uint32_t SPI_RCR;       /**< \brief (Spi Offset: 0x104) Receive Counter Register */
  __IO uint32_t SPI_TPR;       /**< \brief (Spi Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t SPI_TCR;       /**< \brief (Spi Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t SPI_RNPR;      /**< \brief (Spi Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t SPI_RNCR;      /**< \brief (Spi Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t SPI_TNPR;      /**< \brief (Spi Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t SPI_TNCR;      /**< \brief (Spi Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t SPI_PTCR;      /**< \brief (Spi Offset: 0x120) Transfer Control Register */
  __I  uint32_t SPI_PTSR;      /**< \brief (Spi Offset: 0x124) Transfer Status Register */
} Spi;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SPI_CR : (SPI Offset: 0x000) SPI Control Register -------- */
#define SPI_CR_SPIEN (0x1u << 0) /**< \brief (SPI_CR) SPI Enable */
#define SPI_CR_SPIDIS (0x1u << 1) /**< \brief (SPI_CR) SPI Disable */
#define SPI_CR_SWRST (0x1u << 7) /**< \brief (SPI_CR) SPI Software Reset */
#define SPI_CR_REQCLR (0x1u << 12) /**< \brief (SPI_CR) Request to Clear the Comparison Trigger */
#define SPI_CR_TXFCLR (0x1u << 16) /**< \brief (SPI_CR) Transmit FIFO Clear */
#define SPI_CR_RXFCLR (0x1u << 17) /**< \brief (SPI_CR) Receive FIFO Clear */
#define SPI_CR_LASTXFER (0x1u << 24) /**< \brief (SPI_CR) Last Transfer */
#define SPI_CR_FIFOEN (0x1u << 30) /**< \brief (SPI_CR) FIFO Enable */
#define SPI_CR_FIFODIS (0x1u << 31) /**< \brief (SPI_CR) FIFO Disable */
/* -------- SPI_MR : (SPI Offset: 0x004) SPI Mode Register -------- */
#define SPI_MR_MSTR (0x1u << 0) /**< \brief (SPI_MR) Master/Slave Mode */
#define SPI_MR_PS (0x1u << 1) /**< \brief (SPI_MR) Peripheral Select */
#define SPI_MR_PCSDEC (0x1u << 2) /**< \brief (SPI_MR) Chip Select Decode */
#define SPI_MR_BRSRCCLK (0x1u << 3) /**< \brief (SPI_MR) Bit Rate Source Clock */
#define   SPI_MR_BRSRCCLK_PERIPH_CLK (0x0u << 3) /**< \brief (SPI_MR) The peripheral clock is the source clock for the bit rate generation. */
#define   SPI_MR_BRSRCCLK_GCLK (0x1u << 3) /**< \brief (SPI_MR) GCLK is the source clock for the bit rate generation, thus the bit rate can be independent of the core/peripheral clock. */
#define SPI_MR_MODFDIS (0x1u << 4) /**< \brief (SPI_MR) Mode Fault Detection */
#define SPI_MR_WDRBT (0x1u << 5) /**< \brief (SPI_MR) Wait Data Read Before Transfer */
#define SPI_MR_LLB (0x1u << 7) /**< \brief (SPI_MR) Local Loopback Enable */
#define SPI_MR_LBHPC (0x1u << 8) /**< \brief (SPI_MR) Last Bit Half Period Compatibility */
#define SPI_MR_CMPMODE (0x1u << 12) /**< \brief (SPI_MR) Comparison Mode */
#define   SPI_MR_CMPMODE_FLAG_ONLY (0x0u << 12) /**< \brief (SPI_MR) Any character is received and comparison function drives CMP flag. */
#define   SPI_MR_CMPMODE_START_CONDITION (0x1u << 12) /**< \brief (SPI_MR) Comparison condition must be met to start reception of all incoming characters until REQCLR is set. */
#define SPI_MR_PCS_Pos 16
#define SPI_MR_PCS_Msk (0x3u << SPI_MR_PCS_Pos) /**< \brief (SPI_MR) Peripheral Chip Select */
#define SPI_MR_PCS(value) ((SPI_MR_PCS_Msk & ((value) << SPI_MR_PCS_Pos)))
#define SPI_MR_DLYBCS_Pos 24
#define SPI_MR_DLYBCS_Msk (0xffu << SPI_MR_DLYBCS_Pos) /**< \brief (SPI_MR) Delay Between Chip Selects */
#define SPI_MR_DLYBCS(value) ((SPI_MR_DLYBCS_Msk & ((value) << SPI_MR_DLYBCS_Pos)))
/* -------- SPI_RDR : (SPI Offset: 0x008) SPI Receive Data Register -------- */
#define SPI_RDR_RD_Pos 0
#define SPI_RDR_RD_Msk (0xffffu << SPI_RDR_RD_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_PCS_Pos 16
#define SPI_RDR_PCS_Msk (0xfu << SPI_RDR_PCS_Pos) /**< \brief (SPI_RDR) Peripheral Chip Select */
#define SPI_RDR_RD0_Pos 0
#define SPI_RDR_RD0_Msk (0xffu << SPI_RDR_RD0_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_RD1_Pos 8
#define SPI_RDR_RD1_Msk (0xffu << SPI_RDR_RD1_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_RD2_Pos 16
#define SPI_RDR_RD2_Msk (0xffu << SPI_RDR_RD2_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_RD3_Pos 24
#define SPI_RDR_RD3_Msk (0xffu << SPI_RDR_RD3_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos 0
#define SPI_RDR_RD0_FIFO_MULTI_DATA_16_Msk (0xffffu << SPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_RD1_FIFO_MULTI_DATA_16_Pos 16
#define SPI_RDR_RD1_FIFO_MULTI_DATA_16_Msk (0xffffu << SPI_RDR_RD1_FIFO_MULTI_DATA_16_Pos) /**< \brief (SPI_RDR) Receive Data */
/* -------- SPI_TDR : (SPI Offset: 0x00C) SPI Transmit Data Register -------- */
#define SPI_TDR_TD_Pos 0
#define SPI_TDR_TD_Msk (0xffffu << SPI_TDR_TD_Pos) /**< \brief (SPI_TDR) Transmit Data */
#define SPI_TDR_TD(value) ((SPI_TDR_TD_Msk & ((value) << SPI_TDR_TD_Pos)))
#define SPI_TDR_PCS_Pos 16
#define SPI_TDR_PCS_Msk (0xfu << SPI_TDR_PCS_Pos) /**< \brief (SPI_TDR) Peripheral Chip Select */
#define SPI_TDR_PCS(value) ((SPI_TDR_PCS_Msk & ((value) << SPI_TDR_PCS_Pos)))
#define SPI_TDR_LASTXFER (0x1u << 24) /**< \brief (SPI_TDR) Last Transfer */
#define SPI_TDR_TD0_Pos 0
#define SPI_TDR_TD0_Msk (0xffffu << SPI_TDR_TD0_Pos) /**< \brief (SPI_TDR) Transmit Data */
#define SPI_TDR_TD0(value) ((SPI_TDR_TD0_Msk & ((value) << SPI_TDR_TD0_Pos)))
#define SPI_TDR_TD1_Pos 16
#define SPI_TDR_TD1_Msk (0xffffu << SPI_TDR_TD1_Pos) /**< \brief (SPI_TDR) Transmit Data */
#define SPI_TDR_TD1(value) ((SPI_TDR_TD1_Msk & ((value) << SPI_TDR_TD1_Pos)))
/* -------- SPI_SR : (SPI Offset: 0x010) SPI Status Register -------- */
#define SPI_SR_RDRF (0x1u << 0) /**< \brief (SPI_SR) Receive Data Register Full (cleared by reading SPI_RDR) */
#define SPI_SR_TDRE (0x1u << 1) /**< \brief (SPI_SR) Transmit Data Register Empty (cleared by writing SPI_TDR) */
#define SPI_SR_MODF (0x1u << 2) /**< \brief (SPI_SR) Mode Fault Error (cleared on read) */
#define SPI_SR_OVRES (0x1u << 3) /**< \brief (SPI_SR) Overrun Error Status (cleared on read) */
#define SPI_SR_ENDRX (0x1u << 4) /**< \brief (SPI_SR) End of RX Buffer (cleared by writing SPI_RCR or SPI_RNCR) */
#define SPI_SR_ENDTX (0x1u << 5) /**< \brief (SPI_SR) End of TX Buffer (cleared by writing SPI_TCR or SPI_TNCR) */
#define SPI_SR_RXBUFF (0x1u << 6) /**< \brief (SPI_SR) RX Buffer Full (cleared by writing SPI_RCR or SPI_RNCR) */
#define SPI_SR_TXBUFE (0x1u << 7) /**< \brief (SPI_SR) TX Buffer Empty (cleared by writing SPI_TCR or SPI_TNCR) */
#define SPI_SR_NSSR (0x1u << 8) /**< \brief (SPI_SR) NSS Rising (cleared on read) */
#define SPI_SR_TXEMPTY (0x1u << 9) /**< \brief (SPI_SR) Transmission Registers Empty (cleared by writing SPI_TDR) */
#define SPI_SR_UNDES (0x1u << 10) /**< \brief (SPI_SR) Underrun Error Status (Slave mode only) (cleared on read) */
#define SPI_SR_CMP (0x1u << 11) /**< \brief (SPI_SR) Comparison Status (cleared on read) */
#define SPI_SR_SFERR (0x1u << 12) /**< \brief (SPI_SR) Slave Frame Error (cleared on read) */
#define SPI_SR_SPIENS (0x1u << 16) /**< \brief (SPI_SR) SPI Enable Status */
#define SPI_SR_TXFEF (0x1u << 24) /**< \brief (SPI_SR) Transmit FIFO Empty Flag (cleared on read) */
#define SPI_SR_TXFFF (0x1u << 25) /**< \brief (SPI_SR) Transmit FIFO Full Flag (cleared on read) */
#define SPI_SR_TXFTHF (0x1u << 26) /**< \brief (SPI_SR) Transmit FIFO Threshold Flag (cleared on read) */
#define SPI_SR_RXFEF (0x1u << 27) /**< \brief (SPI_SR) Receive FIFO Empty Flag */
#define SPI_SR_RXFFF (0x1u << 28) /**< \brief (SPI_SR) Receive FIFO Full Flag */
#define SPI_SR_RXFTHF (0x1u << 29) /**< \brief (SPI_SR) Receive FIFO Threshold Flag */
#define SPI_SR_TXFPTEF (0x1u << 30) /**< \brief (SPI_SR) Transmit FIFO Pointer Error Flag */
#define SPI_SR_RXFPTEF (0x1u << 31) /**< \brief (SPI_SR) Receive FIFO Pointer Error Flag */
/* -------- SPI_IER : (SPI Offset: 0x014) SPI Interrupt Enable Register -------- */
#define SPI_IER_RDRF (0x1u << 0) /**< \brief (SPI_IER) Receive Data Register Full Interrupt Enable */
#define SPI_IER_TDRE (0x1u << 1) /**< \brief (SPI_IER) SPI Transmit Data Register Empty Interrupt Enable */
#define SPI_IER_MODF (0x1u << 2) /**< \brief (SPI_IER) Mode Fault Error Interrupt Enable */
#define SPI_IER_OVRES (0x1u << 3) /**< \brief (SPI_IER) Overrun Error Interrupt Enable */
#define SPI_IER_ENDRX (0x1u << 4) /**< \brief (SPI_IER) End of Receive Buffer Interrupt Enable */
#define SPI_IER_ENDTX (0x1u << 5) /**< \brief (SPI_IER) End of Transmit Buffer Interrupt Enable */
#define SPI_IER_RXBUFF (0x1u << 6) /**< \brief (SPI_IER) Receive Buffer Full Interrupt Enable */
#define SPI_IER_TXBUFE (0x1u << 7) /**< \brief (SPI_IER) Transmit Buffer Empty Interrupt Enable */
#define SPI_IER_NSSR (0x1u << 8) /**< \brief (SPI_IER) NSS Rising Interrupt Enable */
#define SPI_IER_TXEMPTY (0x1u << 9) /**< \brief (SPI_IER) Transmission Registers Empty Enable */
#define SPI_IER_UNDES (0x1u << 10) /**< \brief (SPI_IER) Underrun Error Interrupt Enable */
#define SPI_IER_CMP (0x1u << 11) /**< \brief (SPI_IER) Comparison Interrupt Enable */
#define SPI_IER_TXFEF (0x1u << 24) /**< \brief (SPI_IER) TXFEF Interrupt Enable */
#define SPI_IER_TXFFF (0x1u << 25) /**< \brief (SPI_IER) TXFFF Interrupt Enable */
#define SPI_IER_TXFTHF (0x1u << 26) /**< \brief (SPI_IER) TXFTHF Interrupt Enable */
#define SPI_IER_RXFEF (0x1u << 27) /**< \brief (SPI_IER) RXFEF Interrupt Enable */
#define SPI_IER_RXFFF (0x1u << 28) /**< \brief (SPI_IER) RXFFF Interrupt Enable */
#define SPI_IER_RXFTHF (0x1u << 29) /**< \brief (SPI_IER) RXFTHF Interrupt Enable */
#define SPI_IER_TXFPTEF (0x1u << 30) /**< \brief (SPI_IER) TXFPTEF Interrupt Enable */
#define SPI_IER_RXFPTEF (0x1u << 31) /**< \brief (SPI_IER) RXFPTEF Interrupt Enable */
/* -------- SPI_IDR : (SPI Offset: 0x018) SPI Interrupt Disable Register -------- */
#define SPI_IDR_RDRF (0x1u << 0) /**< \brief (SPI_IDR) Receive Data Register Full Interrupt Disable */
#define SPI_IDR_TDRE (0x1u << 1) /**< \brief (SPI_IDR) SPI Transmit Data Register Empty Interrupt Disable */
#define SPI_IDR_MODF (0x1u << 2) /**< \brief (SPI_IDR) Mode Fault Error Interrupt Disable */
#define SPI_IDR_OVRES (0x1u << 3) /**< \brief (SPI_IDR) Overrun Error Interrupt Disable */
#define SPI_IDR_ENDRX (0x1u << 4) /**< \brief (SPI_IDR) End of Receive Buffer Interrupt Disable */
#define SPI_IDR_ENDTX (0x1u << 5) /**< \brief (SPI_IDR) End of Transmit Buffer Interrupt Disable */
#define SPI_IDR_RXBUFF (0x1u << 6) /**< \brief (SPI_IDR) Receive Buffer Full Interrupt Disable */
#define SPI_IDR_TXBUFE (0x1u << 7) /**< \brief (SPI_IDR) Transmit Buffer Empty Interrupt Disable */
#define SPI_IDR_NSSR (0x1u << 8) /**< \brief (SPI_IDR) NSS Rising Interrupt Disable */
#define SPI_IDR_TXEMPTY (0x1u << 9) /**< \brief (SPI_IDR) Transmission Registers Empty Disable */
#define SPI_IDR_UNDES (0x1u << 10) /**< \brief (SPI_IDR) Underrun Error Interrupt Disable */
#define SPI_IDR_CMP (0x1u << 11) /**< \brief (SPI_IDR) Comparison Interrupt Disable */
#define SPI_IDR_TXFEF (0x1u << 24) /**< \brief (SPI_IDR) TXFEF Interrupt Disable */
#define SPI_IDR_TXFFF (0x1u << 25) /**< \brief (SPI_IDR) TXFFF Interrupt Disable */
#define SPI_IDR_TXFTHF (0x1u << 26) /**< \brief (SPI_IDR) TXFTHF Interrupt Disable */
#define SPI_IDR_RXFEF (0x1u << 27) /**< \brief (SPI_IDR) RXFEF Interrupt Disable */
#define SPI_IDR_RXFFF (0x1u << 28) /**< \brief (SPI_IDR) RXFFF Interrupt Disable */
#define SPI_IDR_RXFTHF (0x1u << 29) /**< \brief (SPI_IDR) RXFTHF Interrupt Disable */
#define SPI_IDR_TXFPTEF (0x1u << 30) /**< \brief (SPI_IDR) TXFPTEF Interrupt Disable */
#define SPI_IDR_RXFPTEF (0x1u << 31) /**< \brief (SPI_IDR) RXFPTEF Interrupt Disable */
/* -------- SPI_IMR : (SPI Offset: 0x01C) SPI Interrupt Mask Register -------- */
#define SPI_IMR_RDRF (0x1u << 0) /**< \brief (SPI_IMR) Receive Data Register Full Interrupt Mask */
#define SPI_IMR_TDRE (0x1u << 1) /**< \brief (SPI_IMR) SPI Transmit Data Register Empty Interrupt Mask */
#define SPI_IMR_MODF (0x1u << 2) /**< \brief (SPI_IMR) Mode Fault Error Interrupt Mask */
#define SPI_IMR_OVRES (0x1u << 3) /**< \brief (SPI_IMR) Overrun Error Interrupt Mask */
#define SPI_IMR_ENDRX (0x1u << 4) /**< \brief (SPI_IMR) End of Receive Buffer Interrupt Mask */
#define SPI_IMR_ENDTX (0x1u << 5) /**< \brief (SPI_IMR) End of Transmit Buffer Interrupt Mask */
#define SPI_IMR_RXBUFF (0x1u << 6) /**< \brief (SPI_IMR) Receive Buffer Full Interrupt Mask */
#define SPI_IMR_TXBUFE (0x1u << 7) /**< \brief (SPI_IMR) Transmit Buffer Empty Interrupt Mask */
#define SPI_IMR_NSSR (0x1u << 8) /**< \brief (SPI_IMR) NSS Rising Interrupt Mask */
#define SPI_IMR_TXEMPTY (0x1u << 9) /**< \brief (SPI_IMR) Transmission Registers Empty Mask */
#define SPI_IMR_UNDES (0x1u << 10) /**< \brief (SPI_IMR) Underrun Error Interrupt Mask */
#define SPI_IMR_CMP (0x1u << 11) /**< \brief (SPI_IMR) Comparison Interrupt Mask */
#define SPI_IMR_TXFEF (0x1u << 24) /**< \brief (SPI_IMR) TXFEF Interrupt Mask */
#define SPI_IMR_TXFFF (0x1u << 25) /**< \brief (SPI_IMR) TXFFF Interrupt Mask */
#define SPI_IMR_TXFTHF (0x1u << 26) /**< \brief (SPI_IMR) TXFTHF Interrupt Mask */
#define SPI_IMR_RXFEF (0x1u << 27) /**< \brief (SPI_IMR) RXFEF Interrupt Mask */
#define SPI_IMR_RXFFF (0x1u << 28) /**< \brief (SPI_IMR) RXFFF Interrupt Mask */
#define SPI_IMR_RXFTHF (0x1u << 29) /**< \brief (SPI_IMR) RXFTHF Interrupt Mask */
#define SPI_IMR_TXFPTEF (0x1u << 30) /**< \brief (SPI_IMR) TXFPTEF Interrupt Mask */
#define SPI_IMR_RXFPTEF (0x1u << 31) /**< \brief (SPI_IMR) RXFPTEF Interrupt Mask */
/* -------- SPI_CSR[2] : (SPI Offset: 0x030) SPI Chip Select Register -------- */
#define SPI_CSR_CPOL (0x1u << 0) /**< \brief (SPI_CSR[2]) Clock Polarity */
#define SPI_CSR_NCPHA (0x1u << 1) /**< \brief (SPI_CSR[2]) Clock Phase */
#define SPI_CSR_CSNAAT (0x1u << 2) /**< \brief (SPI_CSR[2]) Chip Select Not Active After Transfer (Ignored if CSAAT = 1) */
#define SPI_CSR_CSAAT (0x1u << 3) /**< \brief (SPI_CSR[2]) Chip Select Active After Transfer */
#define SPI_CSR_BITS_Pos 4
#define SPI_CSR_BITS_Msk (0xfu << SPI_CSR_BITS_Pos) /**< \brief (SPI_CSR[2]) Bits Per Transfer */
#define SPI_CSR_BITS(value) ((SPI_CSR_BITS_Msk & ((value) << SPI_CSR_BITS_Pos)))
#define   SPI_CSR_BITS_8_BIT (0x0u << 4) /**< \brief (SPI_CSR[2]) 8 bits for transfer */
#define   SPI_CSR_BITS_9_BIT (0x1u << 4) /**< \brief (SPI_CSR[2]) 9 bits for transfer */
#define   SPI_CSR_BITS_10_BIT (0x2u << 4) /**< \brief (SPI_CSR[2]) 10 bits for transfer */
#define   SPI_CSR_BITS_11_BIT (0x3u << 4) /**< \brief (SPI_CSR[2]) 11 bits for transfer */
#define   SPI_CSR_BITS_12_BIT (0x4u << 4) /**< \brief (SPI_CSR[2]) 12 bits for transfer */
#define   SPI_CSR_BITS_13_BIT (0x5u << 4) /**< \brief (SPI_CSR[2]) 13 bits for transfer */
#define   SPI_CSR_BITS_14_BIT (0x6u << 4) /**< \brief (SPI_CSR[2]) 14 bits for transfer */
#define   SPI_CSR_BITS_15_BIT (0x7u << 4) /**< \brief (SPI_CSR[2]) 15 bits for transfer */
#define   SPI_CSR_BITS_16_BIT (0x8u << 4) /**< \brief (SPI_CSR[2]) 16 bits for transfer */
#define SPI_CSR_SCBR_Pos 8
#define SPI_CSR_SCBR_Msk (0xffu << SPI_CSR_SCBR_Pos) /**< \brief (SPI_CSR[2]) Serial Clock Bit Rate */
#define SPI_CSR_SCBR(value) ((SPI_CSR_SCBR_Msk & ((value) << SPI_CSR_SCBR_Pos)))
#define SPI_CSR_DLYBS_Pos 16
#define SPI_CSR_DLYBS_Msk (0xffu << SPI_CSR_DLYBS_Pos) /**< \brief (SPI_CSR[2]) Delay Before SPCK */
#define SPI_CSR_DLYBS(value) ((SPI_CSR_DLYBS_Msk & ((value) << SPI_CSR_DLYBS_Pos)))
#define SPI_CSR_DLYBCT_Pos 24
#define SPI_CSR_DLYBCT_Msk (0xffu << SPI_CSR_DLYBCT_Pos) /**< \brief (SPI_CSR[2]) Delay Between Consecutive Transfers */
#define SPI_CSR_DLYBCT(value) ((SPI_CSR_DLYBCT_Msk & ((value) << SPI_CSR_DLYBCT_Pos)))
/* -------- SPI_FMR : (SPI Offset: 0x040) SPI FIFO Mode Register -------- */
#define SPI_FMR_TXRDYM_Pos 0
#define SPI_FMR_TXRDYM_Msk (0x3u << SPI_FMR_TXRDYM_Pos) /**< \brief (SPI_FMR) Transmit Data Register Empty Mode */
#define SPI_FMR_TXRDYM(value) ((SPI_FMR_TXRDYM_Msk & ((value) << SPI_FMR_TXRDYM_Pos)))
#define   SPI_FMR_TXRDYM_ONE_DATA (0x0u << 0) /**< \brief (SPI_FMR) TDRE will be at level '1' when at least one data can be written in the Transmit FIFO. */
#define   SPI_FMR_TXRDYM_TWO_DATA (0x1u << 0) /**< \brief (SPI_FMR) TDRE will be at level '1' when at least two data can be written in the Transmit FIFO. Cannot be used if SPI_MR.PS =1. */
#define SPI_FMR_RXRDYM_Pos 4
#define SPI_FMR_RXRDYM_Msk (0x3u << SPI_FMR_RXRDYM_Pos) /**< \brief (SPI_FMR) Receive Data Register Full Mode */
#define SPI_FMR_RXRDYM(value) ((SPI_FMR_RXRDYM_Msk & ((value) << SPI_FMR_RXRDYM_Pos)))
#define   SPI_FMR_RXRDYM_ONE_DATA (0x0u << 4) /**< \brief (SPI_FMR) RDRF will be at level '1' when at least one unread data is in the Receive FIFO. */
#define   SPI_FMR_RXRDYM_TWO_DATA (0x1u << 4) /**< \brief (SPI_FMR) RDRF will be at level '1' when at least two unread data are in the Receive FIFO. Cannot be used when SPI_MR.MSTR =1, or if SPI_MR.PS =1. */
#define   SPI_FMR_RXRDYM_FOUR_DATA (0x2u << 4) /**< \brief (SPI_FMR) RDRF will be at level '1' when at least four unread data are in the Receive FIFO. Cannot be used when SPI_CSRx.BITS is greater than 0, or if SPI_MR.MSTR =1, or if SPI_MR.PS =1. */
#define SPI_FMR_TXFTHRES_Pos 16
#define SPI_FMR_TXFTHRES_Msk (0x3fu << SPI_FMR_TXFTHRES_Pos) /**< \brief (SPI_FMR) Transmit FIFO Threshold */
#define SPI_FMR_TXFTHRES(value) ((SPI_FMR_TXFTHRES_Msk & ((value) << SPI_FMR_TXFTHRES_Pos)))
#define SPI_FMR_RXFTHRES_Pos 24
#define SPI_FMR_RXFTHRES_Msk (0x3fu << SPI_FMR_RXFTHRES_Pos) /**< \brief (SPI_FMR) Receive FIFO Threshold */
#define SPI_FMR_RXFTHRES(value) ((SPI_FMR_RXFTHRES_Msk & ((value) << SPI_FMR_RXFTHRES_Pos)))
/* -------- SPI_FLR : (SPI Offset: 0x044) SPI FIFO Level Register -------- */
#define SPI_FLR_TXFL_Pos 0
#define SPI_FLR_TXFL_Msk (0x3fu << SPI_FLR_TXFL_Pos) /**< \brief (SPI_FLR) Transmit FIFO Level */
#define SPI_FLR_RXFL_Pos 16
#define SPI_FLR_RXFL_Msk (0x3fu << SPI_FLR_RXFL_Pos) /**< \brief (SPI_FLR) Receive FIFO Level */
/* -------- SPI_CMPR : (SPI Offset: 0x048) SPI Comparison Register -------- */
#define SPI_CMPR_VAL1_Pos 0
#define SPI_CMPR_VAL1_Msk (0xffffu << SPI_CMPR_VAL1_Pos) /**< \brief (SPI_CMPR) First Comparison Value for Received Character */
#define SPI_CMPR_VAL1(value) ((SPI_CMPR_VAL1_Msk & ((value) << SPI_CMPR_VAL1_Pos)))
#define SPI_CMPR_VAL2_Pos 16
#define SPI_CMPR_VAL2_Msk (0xffffu << SPI_CMPR_VAL2_Pos) /**< \brief (SPI_CMPR) Second Comparison Value for Received Character */
#define SPI_CMPR_VAL2(value) ((SPI_CMPR_VAL2_Msk & ((value) << SPI_CMPR_VAL2_Pos)))
/* -------- SPI_WPMR : (SPI Offset: 0x0E4) SPI Write Protection Mode Register -------- */
#define SPI_WPMR_WPEN (0x1u << 0) /**< \brief (SPI_WPMR) Write Protection Enable */
#define SPI_WPMR_WPITEN (0x1u << 1) /**< \brief (SPI_WPMR) Write Protection Interrupt Enable */
#define SPI_WPMR_WPCREN (0x1u << 2) /**< \brief (SPI_WPMR) Write Protection Control Enable */
#define SPI_WPMR_WPKEY_Pos 8
#define SPI_WPMR_WPKEY_Msk (0xffffffu << SPI_WPMR_WPKEY_Pos) /**< \brief (SPI_WPMR) Write Protection Key */
#define SPI_WPMR_WPKEY(value) ((SPI_WPMR_WPKEY_Msk & ((value) << SPI_WPMR_WPKEY_Pos)))
#define   SPI_WPMR_WPKEY_PASSWD (0x535049u << 8) /**< \brief (SPI_WPMR) Writing any other value in this field aborts the write operation of bits WPEN, WPITEN and WPCREN. Always reads as 0 */
/* -------- SPI_WPSR : (SPI Offset: 0x0E8) SPI Write Protection Status Register -------- */
#define SPI_WPSR_WPVS (0x1u << 0) /**< \brief (SPI_WPSR) Write Protection Violation Status */
#define SPI_WPSR_WPVSRC_Pos 8
#define SPI_WPSR_WPVSRC_Msk (0xffu << SPI_WPSR_WPVSRC_Pos) /**< \brief (SPI_WPSR) Write Protection Violation Source */
/* -------- SPI_VERSION : (SPI Offset: 0x0FC) SPI Version Register -------- */
#define SPI_VERSION_VERSION_Pos 0
#define SPI_VERSION_VERSION_Msk (0xfffu << SPI_VERSION_VERSION_Pos) /**< \brief (SPI_VERSION) Version of the Hardware Module */
#define SPI_VERSION_MFN_Pos 16
#define SPI_VERSION_MFN_Msk (0x7u << SPI_VERSION_MFN_Pos) /**< \brief (SPI_VERSION) Metal Fix Number */
/* -------- SPI_RPR : (SPI Offset: 0x100) Receive Pointer Register -------- */
#define SPI_RPR_RXPTR_Pos 0
#define SPI_RPR_RXPTR_Msk (0xffffffffu << SPI_RPR_RXPTR_Pos) /**< \brief (SPI_RPR) Receive Pointer Register */
#define SPI_RPR_RXPTR(value) ((SPI_RPR_RXPTR_Msk & ((value) << SPI_RPR_RXPTR_Pos)))
/* -------- SPI_RCR : (SPI Offset: 0x104) Receive Counter Register -------- */
#define SPI_RCR_RXCTR_Pos 0
#define SPI_RCR_RXCTR_Msk (0xffffu << SPI_RCR_RXCTR_Pos) /**< \brief (SPI_RCR) Receive Counter Register */
#define SPI_RCR_RXCTR(value) ((SPI_RCR_RXCTR_Msk & ((value) << SPI_RCR_RXCTR_Pos)))
/* -------- SPI_TPR : (SPI Offset: 0x108) Transmit Pointer Register -------- */
#define SPI_TPR_TXPTR_Pos 0
#define SPI_TPR_TXPTR_Msk (0xffffffffu << SPI_TPR_TXPTR_Pos) /**< \brief (SPI_TPR) Transmit Counter Register */
#define SPI_TPR_TXPTR(value) ((SPI_TPR_TXPTR_Msk & ((value) << SPI_TPR_TXPTR_Pos)))
/* -------- SPI_TCR : (SPI Offset: 0x10C) Transmit Counter Register -------- */
#define SPI_TCR_TXCTR_Pos 0
#define SPI_TCR_TXCTR_Msk (0xffffu << SPI_TCR_TXCTR_Pos) /**< \brief (SPI_TCR) Transmit Counter Register */
#define SPI_TCR_TXCTR(value) ((SPI_TCR_TXCTR_Msk & ((value) << SPI_TCR_TXCTR_Pos)))
/* -------- SPI_RNPR : (SPI Offset: 0x110) Receive Next Pointer Register -------- */
#define SPI_RNPR_RXNPTR_Pos 0
#define SPI_RNPR_RXNPTR_Msk (0xffffffffu << SPI_RNPR_RXNPTR_Pos) /**< \brief (SPI_RNPR) Receive Next Pointer */
#define SPI_RNPR_RXNPTR(value) ((SPI_RNPR_RXNPTR_Msk & ((value) << SPI_RNPR_RXNPTR_Pos)))
/* -------- SPI_RNCR : (SPI Offset: 0x114) Receive Next Counter Register -------- */
#define SPI_RNCR_RXNCTR_Pos 0
#define SPI_RNCR_RXNCTR_Msk (0xffffu << SPI_RNCR_RXNCTR_Pos) /**< \brief (SPI_RNCR) Receive Next Counter */
#define SPI_RNCR_RXNCTR(value) ((SPI_RNCR_RXNCTR_Msk & ((value) << SPI_RNCR_RXNCTR_Pos)))
/* -------- SPI_TNPR : (SPI Offset: 0x118) Transmit Next Pointer Register -------- */
#define SPI_TNPR_TXNPTR_Pos 0
#define SPI_TNPR_TXNPTR_Msk (0xffffffffu << SPI_TNPR_TXNPTR_Pos) /**< \brief (SPI_TNPR) Transmit Next Pointer */
#define SPI_TNPR_TXNPTR(value) ((SPI_TNPR_TXNPTR_Msk & ((value) << SPI_TNPR_TXNPTR_Pos)))
/* -------- SPI_TNCR : (SPI Offset: 0x11C) Transmit Next Counter Register -------- */
#define SPI_TNCR_TXNCTR_Pos 0
#define SPI_TNCR_TXNCTR_Msk (0xffffu << SPI_TNCR_TXNCTR_Pos) /**< \brief (SPI_TNCR) Transmit Counter Next */
#define SPI_TNCR_TXNCTR(value) ((SPI_TNCR_TXNCTR_Msk & ((value) << SPI_TNCR_TXNCTR_Pos)))
/* -------- SPI_PTCR : (SPI Offset: 0x120) Transfer Control Register -------- */
#define SPI_PTCR_RXTEN (0x1u << 0) /**< \brief (SPI_PTCR) Receiver Transfer Enable */
#define SPI_PTCR_RXTDIS (0x1u << 1) /**< \brief (SPI_PTCR) Receiver Transfer Disable */
#define SPI_PTCR_TXTEN (0x1u << 8) /**< \brief (SPI_PTCR) Transmitter Transfer Enable */
#define SPI_PTCR_TXTDIS (0x1u << 9) /**< \brief (SPI_PTCR) Transmitter Transfer Disable */
#define SPI_PTCR_RXCBEN (0x1u << 16) /**< \brief (SPI_PTCR) Receiver Circular Buffer Enable */
#define SPI_PTCR_RXCBDIS (0x1u << 17) /**< \brief (SPI_PTCR) Receiver Circular Buffer Disable */
#define SPI_PTCR_TXCBEN (0x1u << 18) /**< \brief (SPI_PTCR) Transmitter Circular Buffer Enable */
#define SPI_PTCR_TXCBDIS (0x1u << 19) /**< \brief (SPI_PTCR) Transmitter Circular Buffer Disable */
#define SPI_PTCR_ERRCLR (0x1u << 24) /**< \brief (SPI_PTCR) Transfer Bus Error Clear */
/* -------- SPI_PTSR : (SPI Offset: 0x124) Transfer Status Register -------- */
#define SPI_PTSR_RXTEN (0x1u << 0) /**< \brief (SPI_PTSR) Receiver Transfer Enable */
#define SPI_PTSR_TXTEN (0x1u << 8) /**< \brief (SPI_PTSR) Transmitter Transfer Enable */
#define SPI_PTSR_RXCBEN (0x1u << 16) /**< \brief (SPI_PTSR) Receiver Transfer Enable */
#define SPI_PTSR_TXCBEN (0x1u << 18) /**< \brief (SPI_PTSR) Transmitter Transfer Enable */
#define SPI_PTSR_ERR (0x1u << 24) /**< \brief (SPI_PTSR) Transfer Bus Error (clear on read) */

/*@}*/


#endif /* _PIC32CX_SPI_COMPONENT_ */
