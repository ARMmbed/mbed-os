/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Microchip Technology Inc.                    */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

#ifndef _PIC32CX_MCSPI_COMPONENT_
#define _PIC32CX_MCSPI_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Multi Channel Serial Peripheral Interface */
/* ============================================================================= */
/** \addtogroup PIC32CX_MCSPI Multi Channel Serial Peripheral Interface */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Mcspi hardware registers */
typedef struct {
  __O  uint32_t MCSPI_CR;      /**< \brief (Mcspi Offset: 0x0) Control Register */
  __IO uint32_t MCSPI_MR;      /**< \brief (Mcspi Offset: 0x4) Mode Register */
  __I  uint32_t MCSPI_RDR;     /**< \brief (Mcspi Offset: 0x8) Receive Data Register */
  __O  uint32_t MCSPI_TDR;     /**< \brief (Mcspi Offset: 0xC) Transmit Data Register */
  __I  uint32_t MCSPI_SR;      /**< \brief (Mcspi Offset: 0x10) Status Register */
  __O  uint32_t MCSPI_IER;     /**< \brief (Mcspi Offset: 0x14) Interrupt Enable Register */
  __O  uint32_t MCSPI_IDR;     /**< \brief (Mcspi Offset: 0x18) Interrupt Disable Register */
  __I  uint32_t MCSPI_IMR;     /**< \brief (Mcspi Offset: 0x1C) Interrupt Mask Register */
  __I  uint32_t Reserved1[4];
  __IO uint32_t MCSPI_CSR[4];  /**< \brief (Mcspi Offset: 0x30) Chip Select Register (CS_number = 0) */
  __IO uint32_t MCSPI_FMR;     /**< \brief (Mcspi Offset: 0x40) FIFO Mode Register */
  __I  uint32_t MCSPI_FLR;     /**< \brief (Mcspi Offset: 0x44) FIFO Level Register */
  __IO uint32_t MCSPI_CMPR;    /**< \brief (Mcspi Offset: 0x48) Comparison Register */
  __IO uint32_t MCSPI_CRCR;    /**< \brief (Mcspi Offset: 0x4C) CRC Register */
  __IO uint32_t MCSPI_TPMR;    /**< \brief (Mcspi Offset: 0x50) Two-Pin Mode Register */
  __I  uint32_t MCSPI_TPHR;    /**< \brief (Mcspi Offset: 0x54) Two-Pin Header Register */
  __I  uint32_t Reserved2[35];
  __IO uint32_t MCSPI_WPMR;    /**< \brief (Mcspi Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t MCSPI_WPSR;    /**< \brief (Mcspi Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved3[5];
  __IO uint32_t MCSPI_RPR;     /**< \brief (Mcspi Offset: 0x100) Receive Pointer Register */
  __IO uint32_t MCSPI_RCR;     /**< \brief (Mcspi Offset: 0x104) Receive Counter Register */
  __IO uint32_t MCSPI_TPR;     /**< \brief (Mcspi Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t MCSPI_TCR;     /**< \brief (Mcspi Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t MCSPI_RNPR;    /**< \brief (Mcspi Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t MCSPI_RNCR;    /**< \brief (Mcspi Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t MCSPI_TNPR;    /**< \brief (Mcspi Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t MCSPI_TNCR;    /**< \brief (Mcspi Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t MCSPI_PTCR;    /**< \brief (Mcspi Offset: 0x120) Transfer Control Register */
  __I  uint32_t MCSPI_PTSR;    /**< \brief (Mcspi Offset: 0x124) Transfer Status Register */
  __IO uint32_t MCSPI_PWPMR;   /**< \brief (Mcspi Offset: 0x128) Write Protection Mode Register */
} Mcspi;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- MCSPI_CR : (MCSPI Offset: 0x0) Control Register -------- */
#define MCSPI_CR_SPIEN (0x1u << 0) /**< \brief (MCSPI_CR) MCSPI Enable */
#define MCSPI_CR_SPIDIS (0x1u << 1) /**< \brief (MCSPI_CR) MCSPI Disable */
#define MCSPI_CR_SWRST (0x1u << 7) /**< \brief (MCSPI_CR) MCSPI Software Reset */
#define MCSPI_CR_REQCLR (0x1u << 12) /**< \brief (MCSPI_CR) Request to Clear the Comparison Trigger */
#define MCSPI_CR_TXFCLR (0x1u << 16) /**< \brief (MCSPI_CR) Transmit FIFO Clear */
#define MCSPI_CR_RXFCLR (0x1u << 17) /**< \brief (MCSPI_CR) Receive FIFO Clear */
#define MCSPI_CR_LASTXFER (0x1u << 24) /**< \brief (MCSPI_CR) Last Transfer */
#define MCSPI_CR_FIFOEN (0x1u << 30) /**< \brief (MCSPI_CR) FIFO Enable */
#define MCSPI_CR_FIFODIS (0x1u << 31) /**< \brief (MCSPI_CR) FIFO Disable */
/* -------- MCSPI_MR : (MCSPI Offset: 0x4) Mode Register -------- */
#define MCSPI_MR_MSTR (0x1u << 0) /**< \brief (MCSPI_MR) Master/Slave Mode */
#define MCSPI_MR_PS (0x1u << 1) /**< \brief (MCSPI_MR) Peripheral Select */
#define MCSPI_MR_PCSDEC (0x1u << 2) /**< \brief (MCSPI_MR) Chip Select Decode */
#define MCSPI_MR_BRSRCCLK (0x1u << 3) /**< \brief (MCSPI_MR) Bit Rate Source Clock */
#define   MCSPI_MR_BRSRCCLK_PERIPH_CLK (0x0u << 3) /**< \brief (MCSPI_MR) The peripheral clock is the source clock for the bit rate generation. */
#define   MCSPI_MR_BRSRCCLK_GCLK (0x1u << 3) /**< \brief (MCSPI_MR) PMC GCLK is the source clock for the bit rate generation, thus the bit rate can be independent of the core/peripheral clock. */
#define MCSPI_MR_MODFDIS (0x1u << 4) /**< \brief (MCSPI_MR) Mode Fault Detection */
#define MCSPI_MR_WDRBT (0x1u << 5) /**< \brief (MCSPI_MR) Wait for Data Read Before Transfer */
#define MCSPI_MR_CRCEN (0x1u << 6) /**< \brief (MCSPI_MR) CRC Enable */
#define MCSPI_MR_LLB (0x1u << 7) /**< \brief (MCSPI_MR) Local Loopback Enable */
#define MCSPI_MR_LSBHALF (0x1u << 8) /**< \brief (MCSPI_MR) LSB Timing Selection */
#define MCSPI_MR_CMPMODE (0x1u << 12) /**< \brief (MCSPI_MR) Comparison Mode */
#define   MCSPI_MR_CMPMODE_FLAG_ONLY (0x0u << 12) /**< \brief (MCSPI_MR) Any character is received and comparison function drives CMP flag. */
#define   MCSPI_MR_CMPMODE_START_CONDITION (0x1u << 12) /**< \brief (MCSPI_MR) Comparison condition must be met to start reception of all incoming characters until REQCLR is set. */
#define MCSPI_MR_TPMEN (0x1u << 13) /**< \brief (MCSPI_MR) Two-Pin Mode Enable */
#define MCSPI_MR_CSIE (0x1u << 14) /**< \brief (MCSPI_MR) Chip Select Inversion Enable */
#define MCSPI_MR_MOSIIE (0x1u << 15) /**< \brief (MCSPI_MR) MOSI Inversion Enable */
#define MCSPI_MR_PCS_Pos 16
#define MCSPI_MR_PCS_Msk (0xfu << MCSPI_MR_PCS_Pos) /**< \brief (MCSPI_MR) Peripheral Chip Select */
#define MCSPI_MR_PCS(value) ((MCSPI_MR_PCS_Msk & ((value) << MCSPI_MR_PCS_Pos)))
#define MCSPI_MR_DLYBCS_Pos 24
#define MCSPI_MR_DLYBCS_Msk (0xffu << MCSPI_MR_DLYBCS_Pos) /**< \brief (MCSPI_MR) Delay Between Chip Selects */
#define MCSPI_MR_DLYBCS(value) ((MCSPI_MR_DLYBCS_Msk & ((value) << MCSPI_MR_DLYBCS_Pos)))
/* -------- MCSPI_RDR : (MCSPI Offset: 0x8) Receive Data Register -------- */
#define MCSPI_RDR_RD_Pos 0
#define MCSPI_RDR_RD_Msk (0xffffu << MCSPI_RDR_RD_Pos) /**< \brief (MCSPI_RDR) Receive Data */
#define MCSPI_RDR_PCS_Pos 16
#define MCSPI_RDR_PCS_Msk (0xfu << MCSPI_RDR_PCS_Pos) /**< \brief (MCSPI_RDR) Peripheral Chip Select */
#define MCSPI_RDR_RD0_Pos 0
#define MCSPI_RDR_RD0_Msk (0xffu << MCSPI_RDR_RD0_Pos) /**< \brief (MCSPI_RDR) Receive Data */
#define MCSPI_RDR_RD1_Pos 8
#define MCSPI_RDR_RD1_Msk (0xffu << MCSPI_RDR_RD1_Pos) /**< \brief (MCSPI_RDR) Receive Data */
#define MCSPI_RDR_RD2_Pos 16
#define MCSPI_RDR_RD2_Msk (0xffu << MCSPI_RDR_RD2_Pos) /**< \brief (MCSPI_RDR) Receive Data */
#define MCSPI_RDR_RD3_Pos 24
#define MCSPI_RDR_RD3_Msk (0xffu << MCSPI_RDR_RD3_Pos) /**< \brief (MCSPI_RDR) Receive Data */
#define MCSPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos 0
#define MCSPI_RDR_RD0_FIFO_MULTI_DATA_16_Msk (0xffffu << MCSPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos) /**< \brief (MCSPI_RDR) Receive Data */
#define MCSPI_RDR_RD1_FIFO_MULTI_DATA_16_Pos 16
#define MCSPI_RDR_RD1_FIFO_MULTI_DATA_16_Msk (0xffffu << MCSPI_RDR_RD1_FIFO_MULTI_DATA_16_Pos) /**< \brief (MCSPI_RDR) Receive Data */
/* -------- MCSPI_TDR : (MCSPI Offset: 0xC) Transmit Data Register -------- */
#define MCSPI_TDR_TD_Pos 0
#define MCSPI_TDR_TD_Msk (0xffffu << MCSPI_TDR_TD_Pos) /**< \brief (MCSPI_TDR) Transmit Data */
#define MCSPI_TDR_TD(value) ((MCSPI_TDR_TD_Msk & ((value) << MCSPI_TDR_TD_Pos)))
#define MCSPI_TDR_PCS_Pos 16
#define MCSPI_TDR_PCS_Msk (0xfu << MCSPI_TDR_PCS_Pos) /**< \brief (MCSPI_TDR) Peripheral Chip Select */
#define MCSPI_TDR_PCS(value) ((MCSPI_TDR_PCS_Msk & ((value) << MCSPI_TDR_PCS_Pos)))
#define MCSPI_TDR_LASTXFER (0x1u << 24) /**< \brief (MCSPI_TDR) Last Transfer */
#define MCSPI_TDR_TD0_Pos 0
#define MCSPI_TDR_TD0_Msk (0xffffu << MCSPI_TDR_TD0_Pos) /**< \brief (MCSPI_TDR) Transmit Data */
#define MCSPI_TDR_TD0(value) ((MCSPI_TDR_TD0_Msk & ((value) << MCSPI_TDR_TD0_Pos)))
#define MCSPI_TDR_TD1_Pos 16
#define MCSPI_TDR_TD1_Msk (0xffffu << MCSPI_TDR_TD1_Pos) /**< \brief (MCSPI_TDR) Transmit Data */
#define MCSPI_TDR_TD1(value) ((MCSPI_TDR_TD1_Msk & ((value) << MCSPI_TDR_TD1_Pos)))
/* -------- MCSPI_SR : (MCSPI Offset: 0x10) Status Register -------- */
#define MCSPI_SR_RDRF (0x1u << 0) /**< \brief (MCSPI_SR) Receive Data Register Full (cleared by reading MCSPI_RDR) */
#define MCSPI_SR_TDRE (0x1u << 1) /**< \brief (MCSPI_SR) Transmit Data Register Empty (cleared by writing MCSPI_TDR) */
#define MCSPI_SR_MODF (0x1u << 2) /**< \brief (MCSPI_SR) Mode Fault Error (cleared on read) */
#define MCSPI_SR_OVRES (0x1u << 3) /**< \brief (MCSPI_SR) Overrun Error Status (cleared on read) */
#define MCSPI_SR_ENDRX (0x1u << 4) /**< \brief (MCSPI_SR) End of RX Buffer (cleared by writing MCSPI_RCR or MCSPI_RNCR) */
#define MCSPI_SR_ENDTX (0x1u << 5) /**< \brief (MCSPI_SR) End of TX Buffer (cleared by writing MCSPI_TCR or MCSPI_TNCR) */
#define MCSPI_SR_RXBUFF (0x1u << 6) /**< \brief (MCSPI_SR) RX Buffer Full (cleared by writing MCSPI_RCR or MCSPI_RNCR) */
#define MCSPI_SR_TXBUFE (0x1u << 7) /**< \brief (MCSPI_SR) TX Buffer Empty (cleared by writing MCSPI_TCR or MCSPI_TNCR) */
#define MCSPI_SR_NSSR (0x1u << 8) /**< \brief (MCSPI_SR) NSS Rising (cleared on read) */
#define MCSPI_SR_TXEMPTY (0x1u << 9) /**< \brief (MCSPI_SR) Transmission Registers Empty (cleared by writing MCSPI_TDR) */
#define MCSPI_SR_UNDES (0x1u << 10) /**< \brief (MCSPI_SR) Underrun Error Status (Slave mode only) (cleared on read) */
#define MCSPI_SR_CMP (0x1u << 11) /**< \brief (MCSPI_SR) Comparison Status (cleared on read) */
#define MCSPI_SR_SFERR (0x1u << 12) /**< \brief (MCSPI_SR) Slave Frame Error (cleared on read) */
#define MCSPI_SR_CRCERR (0x1u << 13) /**< \brief (MCSPI_SR) CRC Error (cleared on read) */
#define MCSPI_SR_SPIENS (0x1u << 16) /**< \brief (MCSPI_SR) MCSPI Enable Status */
#define MCSPI_SR_TXFEF (0x1u << 24) /**< \brief (MCSPI_SR) Transmit FIFO Empty Flag (cleared on read) */
#define MCSPI_SR_TXFFF (0x1u << 25) /**< \brief (MCSPI_SR) Transmit FIFO Full Flag (cleared on read) */
#define MCSPI_SR_TXFTHF (0x1u << 26) /**< \brief (MCSPI_SR) Transmit FIFO Threshold Flag (cleared on read) */
#define MCSPI_SR_RXFEF (0x1u << 27) /**< \brief (MCSPI_SR) Receive FIFO Empty Flag */
#define MCSPI_SR_RXFFF (0x1u << 28) /**< \brief (MCSPI_SR) Receive FIFO Full Flag */
#define MCSPI_SR_RXFTHF (0x1u << 29) /**< \brief (MCSPI_SR) Receive FIFO Threshold Flag */
#define MCSPI_SR_TXFPTEF (0x1u << 30) /**< \brief (MCSPI_SR) Transmit FIFO Pointer Error Flag */
#define MCSPI_SR_RXFPTEF (0x1u << 31) /**< \brief (MCSPI_SR) Receive FIFO Pointer Error Flag */
/* -------- MCSPI_IER : (MCSPI Offset: 0x14) Interrupt Enable Register -------- */
#define MCSPI_IER_RDRF (0x1u << 0) /**< \brief (MCSPI_IER) Receive Data Register Full Interrupt Enable */
#define MCSPI_IER_TDRE (0x1u << 1) /**< \brief (MCSPI_IER) MCSPI Transmit Data Register Empty Interrupt Enable */
#define MCSPI_IER_MODF (0x1u << 2) /**< \brief (MCSPI_IER) Mode Fault Error Interrupt Enable */
#define MCSPI_IER_OVRES (0x1u << 3) /**< \brief (MCSPI_IER) Overrun Error Interrupt Enable */
#define MCSPI_IER_ENDRX (0x1u << 4) /**< \brief (MCSPI_IER) End of Receive Buffer Interrupt Enable */
#define MCSPI_IER_ENDTX (0x1u << 5) /**< \brief (MCSPI_IER) End of Transmit Buffer Interrupt Enable */
#define MCSPI_IER_RXBUFF (0x1u << 6) /**< \brief (MCSPI_IER) Receive Buffer Full Interrupt Enable */
#define MCSPI_IER_TXBUFE (0x1u << 7) /**< \brief (MCSPI_IER) Transmit Buffer Empty Interrupt Enable */
#define MCSPI_IER_NSSR (0x1u << 8) /**< \brief (MCSPI_IER) NSS Rising Interrupt Enable */
#define MCSPI_IER_TXEMPTY (0x1u << 9) /**< \brief (MCSPI_IER) Transmission Registers Empty Enable */
#define MCSPI_IER_UNDES (0x1u << 10) /**< \brief (MCSPI_IER) Underrun Error Interrupt Enable */
#define MCSPI_IER_CMP (0x1u << 11) /**< \brief (MCSPI_IER) Comparison Interrupt Enable */
#define MCSPI_IER_CRCERR (0x1u << 13) /**< \brief (MCSPI_IER) CRC Error Interrupt Enable */
#define MCSPI_IER_TXFEF (0x1u << 24) /**< \brief (MCSPI_IER) TXFEF Interrupt Enable */
#define MCSPI_IER_TXFFF (0x1u << 25) /**< \brief (MCSPI_IER) TXFFF Interrupt Enable */
#define MCSPI_IER_TXFTHF (0x1u << 26) /**< \brief (MCSPI_IER) TXFTHF Interrupt Enable */
#define MCSPI_IER_RXFEF (0x1u << 27) /**< \brief (MCSPI_IER) RXFEF Interrupt Enable */
#define MCSPI_IER_RXFFF (0x1u << 28) /**< \brief (MCSPI_IER) RXFFF Interrupt Enable */
#define MCSPI_IER_RXFTHF (0x1u << 29) /**< \brief (MCSPI_IER) RXFTHF Interrupt Enable */
#define MCSPI_IER_TXFPTEF (0x1u << 30) /**< \brief (MCSPI_IER) TXFPTEF Interrupt Enable */
#define MCSPI_IER_RXFPTEF (0x1u << 31) /**< \brief (MCSPI_IER) RXFPTEF Interrupt Enable */
/* -------- MCSPI_IDR : (MCSPI Offset: 0x18) Interrupt Disable Register -------- */
#define MCSPI_IDR_RDRF (0x1u << 0) /**< \brief (MCSPI_IDR) Receive Data Register Full Interrupt Disable */
#define MCSPI_IDR_TDRE (0x1u << 1) /**< \brief (MCSPI_IDR) MCSPI Transmit Data Register Empty Interrupt Disable */
#define MCSPI_IDR_MODF (0x1u << 2) /**< \brief (MCSPI_IDR) Mode Fault Error Interrupt Disable */
#define MCSPI_IDR_OVRES (0x1u << 3) /**< \brief (MCSPI_IDR) Overrun Error Interrupt Disable */
#define MCSPI_IDR_ENDRX (0x1u << 4) /**< \brief (MCSPI_IDR) End of Receive Buffer Interrupt Disable */
#define MCSPI_IDR_ENDTX (0x1u << 5) /**< \brief (MCSPI_IDR) End of Transmit Buffer Interrupt Disable */
#define MCSPI_IDR_RXBUFF (0x1u << 6) /**< \brief (MCSPI_IDR) Receive Buffer Full Interrupt Disable */
#define MCSPI_IDR_TXBUFE (0x1u << 7) /**< \brief (MCSPI_IDR) Transmit Buffer Empty Interrupt Disable */
#define MCSPI_IDR_NSSR (0x1u << 8) /**< \brief (MCSPI_IDR) NSS Rising Interrupt Disable */
#define MCSPI_IDR_TXEMPTY (0x1u << 9) /**< \brief (MCSPI_IDR) Transmission Registers Empty Disable */
#define MCSPI_IDR_UNDES (0x1u << 10) /**< \brief (MCSPI_IDR) Underrun Error Interrupt Disable */
#define MCSPI_IDR_CMP (0x1u << 11) /**< \brief (MCSPI_IDR) Comparison Interrupt Disable */
#define MCSPI_IDR_CRCERR (0x1u << 13) /**< \brief (MCSPI_IDR) CRC Error Interrupt Disable */
#define MCSPI_IDR_TXFEF (0x1u << 24) /**< \brief (MCSPI_IDR) TXFEF Interrupt Disable */
#define MCSPI_IDR_TXFFF (0x1u << 25) /**< \brief (MCSPI_IDR) TXFFF Interrupt Disable */
#define MCSPI_IDR_TXFTHF (0x1u << 26) /**< \brief (MCSPI_IDR) TXFTHF Interrupt Disable */
#define MCSPI_IDR_RXFEF (0x1u << 27) /**< \brief (MCSPI_IDR) RXFEF Interrupt Disable */
#define MCSPI_IDR_RXFFF (0x1u << 28) /**< \brief (MCSPI_IDR) RXFFF Interrupt Disable */
#define MCSPI_IDR_RXFTHF (0x1u << 29) /**< \brief (MCSPI_IDR) RXFTHF Interrupt Disable */
#define MCSPI_IDR_TXFPTEF (0x1u << 30) /**< \brief (MCSPI_IDR) TXFPTEF Interrupt Disable */
#define MCSPI_IDR_RXFPTEF (0x1u << 31) /**< \brief (MCSPI_IDR) RXFPTEF Interrupt Disable */
/* -------- MCSPI_IMR : (MCSPI Offset: 0x1C) Interrupt Mask Register -------- */
#define MCSPI_IMR_RDRF (0x1u << 0) /**< \brief (MCSPI_IMR) Receive Data Register Full Interrupt Mask */
#define MCSPI_IMR_TDRE (0x1u << 1) /**< \brief (MCSPI_IMR) MCSPI Transmit Data Register Empty Interrupt Mask */
#define MCSPI_IMR_MODF (0x1u << 2) /**< \brief (MCSPI_IMR) Mode Fault Error Interrupt Mask */
#define MCSPI_IMR_OVRES (0x1u << 3) /**< \brief (MCSPI_IMR) Overrun Error Interrupt Mask */
#define MCSPI_IMR_ENDRX (0x1u << 4) /**< \brief (MCSPI_IMR) End of Receive Buffer Interrupt Mask */
#define MCSPI_IMR_ENDTX (0x1u << 5) /**< \brief (MCSPI_IMR) End of Transmit Buffer Interrupt Mask */
#define MCSPI_IMR_RXBUFF (0x1u << 6) /**< \brief (MCSPI_IMR) Receive Buffer Full Interrupt Mask */
#define MCSPI_IMR_TXBUFE (0x1u << 7) /**< \brief (MCSPI_IMR) Transmit Buffer Empty Interrupt Mask */
#define MCSPI_IMR_NSSR (0x1u << 8) /**< \brief (MCSPI_IMR) NSS Rising Interrupt Mask */
#define MCSPI_IMR_TXEMPTY (0x1u << 9) /**< \brief (MCSPI_IMR) Transmission Registers Empty Mask */
#define MCSPI_IMR_UNDES (0x1u << 10) /**< \brief (MCSPI_IMR) Underrun Error Interrupt Mask */
#define MCSPI_IMR_CMP (0x1u << 11) /**< \brief (MCSPI_IMR) Comparison Interrupt Mask */
#define MCSPI_IMR_CRCERR (0x1u << 13) /**< \brief (MCSPI_IMR) CRC Error Interrupt Mask */
#define MCSPI_IMR_TXFEF (0x1u << 24) /**< \brief (MCSPI_IMR) TXFEF Interrupt Mask */
#define MCSPI_IMR_TXFFF (0x1u << 25) /**< \brief (MCSPI_IMR) TXFFF Interrupt Mask */
#define MCSPI_IMR_TXFTHF (0x1u << 26) /**< \brief (MCSPI_IMR) TXFTHF Interrupt Mask */
#define MCSPI_IMR_RXFEF (0x1u << 27) /**< \brief (MCSPI_IMR) RXFEF Interrupt Mask */
#define MCSPI_IMR_RXFFF (0x1u << 28) /**< \brief (MCSPI_IMR) RXFFF Interrupt Mask */
#define MCSPI_IMR_RXFTHF (0x1u << 29) /**< \brief (MCSPI_IMR) RXFTHF Interrupt Mask */
#define MCSPI_IMR_TXFPTEF (0x1u << 30) /**< \brief (MCSPI_IMR) TXFPTEF Interrupt Mask */
#define MCSPI_IMR_RXFPTEF (0x1u << 31) /**< \brief (MCSPI_IMR) RXFPTEF Interrupt Mask */
/* -------- MCSPI_CSR[4] : (MCSPI Offset: 0x30) Chip Select Register (CS_number = 0) -------- */
#define MCSPI_CSR_CPOL (0x1u << 0) /**< \brief (MCSPI_CSR[4]) Clock Polarity */
#define MCSPI_CSR_NCPHA (0x1u << 1) /**< \brief (MCSPI_CSR[4]) Clock Phase */
#define MCSPI_CSR_CSNAAT (0x1u << 2) /**< \brief (MCSPI_CSR[4]) Chip Select Not Active After Transfer (ignored if CSAAT = 1) */
#define MCSPI_CSR_CSAAT (0x1u << 3) /**< \brief (MCSPI_CSR[4]) Chip Select Active After Transfer */
#define MCSPI_CSR_BITS_Pos 4
#define MCSPI_CSR_BITS_Msk (0xfu << MCSPI_CSR_BITS_Pos) /**< \brief (MCSPI_CSR[4]) Bits Per Transfer */
#define MCSPI_CSR_BITS(value) ((MCSPI_CSR_BITS_Msk & ((value) << MCSPI_CSR_BITS_Pos)))
#define   MCSPI_CSR_BITS_8_BIT (0x0u << 4) /**< \brief (MCSPI_CSR[4]) 8 bits for transfer */
#define   MCSPI_CSR_BITS_9_BIT (0x1u << 4) /**< \brief (MCSPI_CSR[4]) 9 bits for transfer */
#define   MCSPI_CSR_BITS_10_BIT (0x2u << 4) /**< \brief (MCSPI_CSR[4]) 10 bits for transfer */
#define   MCSPI_CSR_BITS_11_BIT (0x3u << 4) /**< \brief (MCSPI_CSR[4]) 11 bits for transfer */
#define   MCSPI_CSR_BITS_12_BIT (0x4u << 4) /**< \brief (MCSPI_CSR[4]) 12 bits for transfer */
#define   MCSPI_CSR_BITS_13_BIT (0x5u << 4) /**< \brief (MCSPI_CSR[4]) 13 bits for transfer */
#define   MCSPI_CSR_BITS_14_BIT (0x6u << 4) /**< \brief (MCSPI_CSR[4]) 14 bits for transfer */
#define   MCSPI_CSR_BITS_15_BIT (0x7u << 4) /**< \brief (MCSPI_CSR[4]) 15 bits for transfer */
#define   MCSPI_CSR_BITS_16_BIT (0x8u << 4) /**< \brief (MCSPI_CSR[4]) 16 bits for transfer */
#define MCSPI_CSR_SCBR_Pos 8
#define MCSPI_CSR_SCBR_Msk (0xffu << MCSPI_CSR_SCBR_Pos) /**< \brief (MCSPI_CSR[4]) Serial Clock Bit Rate */
#define MCSPI_CSR_SCBR(value) ((MCSPI_CSR_SCBR_Msk & ((value) << MCSPI_CSR_SCBR_Pos)))
#define MCSPI_CSR_DLYBS_Pos 16
#define MCSPI_CSR_DLYBS_Msk (0xffu << MCSPI_CSR_DLYBS_Pos) /**< \brief (MCSPI_CSR[4]) Delay Before SPCK */
#define MCSPI_CSR_DLYBS(value) ((MCSPI_CSR_DLYBS_Msk & ((value) << MCSPI_CSR_DLYBS_Pos)))
#define MCSPI_CSR_DLYBCT_Pos 24
#define MCSPI_CSR_DLYBCT_Msk (0xffu << MCSPI_CSR_DLYBCT_Pos) /**< \brief (MCSPI_CSR[4]) Delay Between Consecutive Transfers */
#define MCSPI_CSR_DLYBCT(value) ((MCSPI_CSR_DLYBCT_Msk & ((value) << MCSPI_CSR_DLYBCT_Pos)))
/* -------- MCSPI_FMR : (MCSPI Offset: 0x40) FIFO Mode Register -------- */
#define MCSPI_FMR_TXRDYM_Pos 0
#define MCSPI_FMR_TXRDYM_Msk (0x3u << MCSPI_FMR_TXRDYM_Pos) /**< \brief (MCSPI_FMR) Transmit Data Register Empty Mode */
#define MCSPI_FMR_TXRDYM(value) ((MCSPI_FMR_TXRDYM_Msk & ((value) << MCSPI_FMR_TXRDYM_Pos)))
#define   MCSPI_FMR_TXRDYM_ONE_DATA (0x0u << 0) /**< \brief (MCSPI_FMR) TDRE will be at level '1' when at least one data can be written in the Transmit FIFO. */
#define   MCSPI_FMR_TXRDYM_TWO_DATA (0x1u << 0) /**< \brief (MCSPI_FMR) TDRE will be at level '1' when at least two data can be written in the Transmit FIFO. Cannot be used if MCSPI_MR.PS =1. */
#define MCSPI_FMR_RXRDYM_Pos 4
#define MCSPI_FMR_RXRDYM_Msk (0x3u << MCSPI_FMR_RXRDYM_Pos) /**< \brief (MCSPI_FMR) Receive Data Register Full Mode */
#define MCSPI_FMR_RXRDYM(value) ((MCSPI_FMR_RXRDYM_Msk & ((value) << MCSPI_FMR_RXRDYM_Pos)))
#define   MCSPI_FMR_RXRDYM_ONE_DATA (0x0u << 4) /**< \brief (MCSPI_FMR) RDRF will be at level '1' when at least one unread data is in the Receive FIFO. */
#define   MCSPI_FMR_RXRDYM_TWO_DATA (0x1u << 4) /**< \brief (MCSPI_FMR) RDRF will be at level '1' when at least two unread data are in the Receive FIFO. Cannot be used when MCSPI_MR.MSTR =1, or if MCSPI_MR.PS =1. */
#define   MCSPI_FMR_RXRDYM_FOUR_DATA (0x2u << 4) /**< \brief (MCSPI_FMR) RDRF will be at level '1' when at least four unread data are in the Receive FIFO. Cannot be used when MCSPI_CSRx.BITS is greater than 0, or if MCSPI_MR.MSTR = 1, or if MCSPI_MR.PS = 1. */
#define MCSPI_FMR_TXFTHRES_Pos 16
#define MCSPI_FMR_TXFTHRES_Msk (0x3fu << MCSPI_FMR_TXFTHRES_Pos) /**< \brief (MCSPI_FMR) Transmit FIFO Threshold */
#define MCSPI_FMR_TXFTHRES(value) ((MCSPI_FMR_TXFTHRES_Msk & ((value) << MCSPI_FMR_TXFTHRES_Pos)))
#define MCSPI_FMR_RXFTHRES_Pos 24
#define MCSPI_FMR_RXFTHRES_Msk (0x3fu << MCSPI_FMR_RXFTHRES_Pos) /**< \brief (MCSPI_FMR) Receive FIFO Threshold */
#define MCSPI_FMR_RXFTHRES(value) ((MCSPI_FMR_RXFTHRES_Msk & ((value) << MCSPI_FMR_RXFTHRES_Pos)))
/* -------- MCSPI_FLR : (MCSPI Offset: 0x44) FIFO Level Register -------- */
#define MCSPI_FLR_TXFL_Pos 0
#define MCSPI_FLR_TXFL_Msk (0x3fu << MCSPI_FLR_TXFL_Pos) /**< \brief (MCSPI_FLR) Transmit FIFO Level */
#define MCSPI_FLR_RXFL_Pos 16
#define MCSPI_FLR_RXFL_Msk (0x3fu << MCSPI_FLR_RXFL_Pos) /**< \brief (MCSPI_FLR) Receive FIFO Level */
/* -------- MCSPI_CMPR : (MCSPI Offset: 0x48) Comparison Register -------- */
#define MCSPI_CMPR_VAL1_Pos 0
#define MCSPI_CMPR_VAL1_Msk (0xffffu << MCSPI_CMPR_VAL1_Pos) /**< \brief (MCSPI_CMPR) First Comparison Value for Received Character */
#define MCSPI_CMPR_VAL1(value) ((MCSPI_CMPR_VAL1_Msk & ((value) << MCSPI_CMPR_VAL1_Pos)))
#define MCSPI_CMPR_VAL2_Pos 16
#define MCSPI_CMPR_VAL2_Msk (0xffffu << MCSPI_CMPR_VAL2_Pos) /**< \brief (MCSPI_CMPR) Second Comparison Value for Received Character */
#define MCSPI_CMPR_VAL2(value) ((MCSPI_CMPR_VAL2_Msk & ((value) << MCSPI_CMPR_VAL2_Pos)))
/* -------- MCSPI_CRCR : (MCSPI Offset: 0x4C) CRC Register -------- */
#define MCSPI_CRCR_FRL_Pos 0
#define MCSPI_CRCR_FRL_Msk (0xffu << MCSPI_CRCR_FRL_Pos) /**< \brief (MCSPI_CRCR) Frame Length */
#define MCSPI_CRCR_FRL(value) ((MCSPI_CRCR_FRL_Msk & ((value) << MCSPI_CRCR_FRL_Pos)))
#define MCSPI_CRCR_CRCS (0x1u << 16) /**< \brief (MCSPI_CRCR) CRC Size */
#define   MCSPI_CRCR_CRCS_16B_CRC (0x0u << 16) /**< \brief (MCSPI_CRCR) CRC size is 16 bits. */
#define   MCSPI_CRCR_CRCS_32B_CRC (0x1u << 16) /**< \brief (MCSPI_CRCR) CRC size is 32 bits. */
#define MCSPI_CRCR_FRHL_Pos 20
#define MCSPI_CRCR_FRHL_Msk (0xfu << MCSPI_CRCR_FRHL_Pos) /**< \brief (MCSPI_CRCR) Frame Header Length */
#define MCSPI_CRCR_FRHL(value) ((MCSPI_CRCR_FRHL_Msk & ((value) << MCSPI_CRCR_FRHL_Pos)))
#define MCSPI_CRCR_CRM (0x1u << 24) /**< \brief (MCSPI_CRCR) Continuous Read Mode */
#define MCSPI_CRCR_FHE (0x1u << 25) /**< \brief (MCSPI_CRCR) Frame Header Excluded */
#define MCSPI_CRCR_DCRX (0x1u << 26) /**< \brief (MCSPI_CRCR) Disable CRC Receiving */
#define MCSPI_CRCR_DHRX (0x1u << 27) /**< \brief (MCSPI_CRCR) Disable Header Receiving */
/* -------- MCSPI_TPMR : (MCSPI Offset: 0x50) Two-Pin Mode Register -------- */
#define MCSPI_TPMR_CSM (0x1u << 0) /**< \brief (MCSPI_TPMR) Chip Select Mode */
#define MCSPI_TPMR_MIL (0x1u << 1) /**< \brief (MCSPI_TPMR) Multiple Input Lines */
#define MCSPI_TPMR_OSR_Pos 2
#define MCSPI_TPMR_OSR_Msk (0x3u << MCSPI_TPMR_OSR_Pos) /**< \brief (MCSPI_TPMR) OverSampling Rate */
#define MCSPI_TPMR_OSR(value) ((MCSPI_TPMR_OSR_Msk & ((value) << MCSPI_TPMR_OSR_Pos)))
/* -------- MCSPI_TPHR : (MCSPI Offset: 0x54) Two-Pin Header Register -------- */
#define MCSPI_TPHR_CNT_Pos 0
#define MCSPI_TPHR_CNT_Msk (0x3u << MCSPI_TPHR_CNT_Pos) /**< \brief (MCSPI_TPHR) Frame Counter */
#define MCSPI_TPHR_BOOST (0x1u << 2) /**< \brief (MCSPI_TPHR) Current Boost */
#define MCSPI_TPHR_GAIN_Pos 3
#define MCSPI_TPHR_GAIN_Msk (0x3u << MCSPI_TPHR_GAIN_Pos) /**< \brief (MCSPI_TPHR) Gain */
#define MCSPI_TPHR_OSR_Pos 5
#define MCSPI_TPHR_OSR_Msk (0x3u << MCSPI_TPHR_OSR_Pos) /**< \brief (MCSPI_TPHR) OverSampling Rate */
/* -------- MCSPI_WPMR : (MCSPI Offset: 0xE4) Write Protection Mode Register -------- */
#define MCSPI_WPMR_WPEN (0x1u << 0) /**< \brief (MCSPI_WPMR) Write Protection Enable */
#define MCSPI_WPMR_WPITEN (0x1u << 1) /**< \brief (MCSPI_WPMR) Write Protection Interrupt Enable */
#define MCSPI_WPMR_WPCREN (0x1u << 2) /**< \brief (MCSPI_WPMR) Write Protection Control Register Enable */
#define MCSPI_WPMR_WPKEY_Pos 8
#define MCSPI_WPMR_WPKEY_Msk (0xffffffu << MCSPI_WPMR_WPKEY_Pos) /**< \brief (MCSPI_WPMR) Write Protection Key */
#define MCSPI_WPMR_WPKEY(value) ((MCSPI_WPMR_WPKEY_Msk & ((value) << MCSPI_WPMR_WPKEY_Pos)))
#define   MCSPI_WPMR_WPKEY_PASSWD (0x535049u << 8) /**< \brief (MCSPI_WPMR) Writing any other value in this field aborts the write operation of WPEN. Always reads as 0. */
/* -------- MCSPI_WPSR : (MCSPI Offset: 0xE8) Write Protection Status Register -------- */
#define MCSPI_WPSR_WPVS (0x1u << 0) /**< \brief (MCSPI_WPSR) Write Protection Violation Status */
#define MCSPI_WPSR_WPVSRC_Pos 8
#define MCSPI_WPSR_WPVSRC_Msk (0xffu << MCSPI_WPSR_WPVSRC_Pos) /**< \brief (MCSPI_WPSR) Write Protection Violation Source */
/* -------- MCSPI_RPR : (MCSPI Offset: 0x100) Receive Pointer Register -------- */
#define MCSPI_RPR_RXPTR_Pos 0
#define MCSPI_RPR_RXPTR_Msk (0xffffffffu << MCSPI_RPR_RXPTR_Pos) /**< \brief (MCSPI_RPR) Receive Pointer Register */
#define MCSPI_RPR_RXPTR(value) ((MCSPI_RPR_RXPTR_Msk & ((value) << MCSPI_RPR_RXPTR_Pos)))
/* -------- MCSPI_RCR : (MCSPI Offset: 0x104) Receive Counter Register -------- */
#define MCSPI_RCR_RXCTR_Pos 0
#define MCSPI_RCR_RXCTR_Msk (0xffffu << MCSPI_RCR_RXCTR_Pos) /**< \brief (MCSPI_RCR) Receive Counter Register */
#define MCSPI_RCR_RXCTR(value) ((MCSPI_RCR_RXCTR_Msk & ((value) << MCSPI_RCR_RXCTR_Pos)))
/* -------- MCSPI_TPR : (MCSPI Offset: 0x108) Transmit Pointer Register -------- */
#define MCSPI_TPR_TXPTR_Pos 0
#define MCSPI_TPR_TXPTR_Msk (0xffffffffu << MCSPI_TPR_TXPTR_Pos) /**< \brief (MCSPI_TPR) Transmit Counter Register */
#define MCSPI_TPR_TXPTR(value) ((MCSPI_TPR_TXPTR_Msk & ((value) << MCSPI_TPR_TXPTR_Pos)))
/* -------- MCSPI_TCR : (MCSPI Offset: 0x10C) Transmit Counter Register -------- */
#define MCSPI_TCR_TXCTR_Pos 0
#define MCSPI_TCR_TXCTR_Msk (0xffffu << MCSPI_TCR_TXCTR_Pos) /**< \brief (MCSPI_TCR) Transmit Counter Register */
#define MCSPI_TCR_TXCTR(value) ((MCSPI_TCR_TXCTR_Msk & ((value) << MCSPI_TCR_TXCTR_Pos)))
/* -------- MCSPI_RNPR : (MCSPI Offset: 0x110) Receive Next Pointer Register -------- */
#define MCSPI_RNPR_RXNPTR_Pos 0
#define MCSPI_RNPR_RXNPTR_Msk (0xffffffffu << MCSPI_RNPR_RXNPTR_Pos) /**< \brief (MCSPI_RNPR) Receive Next Pointer */
#define MCSPI_RNPR_RXNPTR(value) ((MCSPI_RNPR_RXNPTR_Msk & ((value) << MCSPI_RNPR_RXNPTR_Pos)))
/* -------- MCSPI_RNCR : (MCSPI Offset: 0x114) Receive Next Counter Register -------- */
#define MCSPI_RNCR_RXNCTR_Pos 0
#define MCSPI_RNCR_RXNCTR_Msk (0xffffu << MCSPI_RNCR_RXNCTR_Pos) /**< \brief (MCSPI_RNCR) Receive Next Counter */
#define MCSPI_RNCR_RXNCTR(value) ((MCSPI_RNCR_RXNCTR_Msk & ((value) << MCSPI_RNCR_RXNCTR_Pos)))
/* -------- MCSPI_TNPR : (MCSPI Offset: 0x118) Transmit Next Pointer Register -------- */
#define MCSPI_TNPR_TXNPTR_Pos 0
#define MCSPI_TNPR_TXNPTR_Msk (0xffffffffu << MCSPI_TNPR_TXNPTR_Pos) /**< \brief (MCSPI_TNPR) Transmit Next Pointer */
#define MCSPI_TNPR_TXNPTR(value) ((MCSPI_TNPR_TXNPTR_Msk & ((value) << MCSPI_TNPR_TXNPTR_Pos)))
/* -------- MCSPI_TNCR : (MCSPI Offset: 0x11C) Transmit Next Counter Register -------- */
#define MCSPI_TNCR_TXNCTR_Pos 0
#define MCSPI_TNCR_TXNCTR_Msk (0xffffu << MCSPI_TNCR_TXNCTR_Pos) /**< \brief (MCSPI_TNCR) Transmit Counter Next */
#define MCSPI_TNCR_TXNCTR(value) ((MCSPI_TNCR_TXNCTR_Msk & ((value) << MCSPI_TNCR_TXNCTR_Pos)))
/* -------- MCSPI_PTCR : (MCSPI Offset: 0x120) Transfer Control Register -------- */
#define MCSPI_PTCR_RXTEN (0x1u << 0) /**< \brief (MCSPI_PTCR) Receiver Transfer Enable */
#define MCSPI_PTCR_RXTDIS (0x1u << 1) /**< \brief (MCSPI_PTCR) Receiver Transfer Disable */
#define MCSPI_PTCR_TXTEN (0x1u << 8) /**< \brief (MCSPI_PTCR) Transmitter Transfer Enable */
#define MCSPI_PTCR_TXTDIS (0x1u << 9) /**< \brief (MCSPI_PTCR) Transmitter Transfer Disable */
#define MCSPI_PTCR_RXCBEN (0x1u << 16) /**< \brief (MCSPI_PTCR) Receiver Circular Buffer Enable */
#define MCSPI_PTCR_RXCBDIS (0x1u << 17) /**< \brief (MCSPI_PTCR) Receiver Circular Buffer Disable */
#define MCSPI_PTCR_TXCBEN (0x1u << 18) /**< \brief (MCSPI_PTCR) Transmitter Circular Buffer Enable */
#define MCSPI_PTCR_TXCBDIS (0x1u << 19) /**< \brief (MCSPI_PTCR) Transmitter Circular Buffer Disable */
#define MCSPI_PTCR_ERRCLR (0x1u << 24) /**< \brief (MCSPI_PTCR) Transfer Bus Error Clear */
/* -------- MCSPI_PTSR : (MCSPI Offset: 0x124) Transfer Status Register -------- */
#define MCSPI_PTSR_RXTEN (0x1u << 0) /**< \brief (MCSPI_PTSR) Receiver Transfer Enable */
#define MCSPI_PTSR_TXTEN (0x1u << 8) /**< \brief (MCSPI_PTSR) Transmitter Transfer Enable */
#define MCSPI_PTSR_RXCBEN (0x1u << 16) /**< \brief (MCSPI_PTSR) Receiver Circular Buffer Enable */
#define MCSPI_PTSR_TXCBEN (0x1u << 18) /**< \brief (MCSPI_PTSR) Transmitter Circular Buffer Enable */
#define MCSPI_PTSR_ERR (0x1u << 24) /**< \brief (MCSPI_PTSR) Transfer Bus Error */
/* -------- MCSPI_PWPMR : (MCSPI Offset: 0x128) Write Protection Mode Register -------- */
#define MCSPI_PWPMR_WPPTREN (0x1u << 0) /**< \brief (MCSPI_PWPMR) Write Protection Pointer Registers Enable */
#define MCSPI_PWPMR_WPCTREN (0x1u << 1) /**< \brief (MCSPI_PWPMR) Write Protection Counter Registers Enable */
#define MCSPI_PWPMR_WPCREN (0x1u << 2) /**< \brief (MCSPI_PWPMR) Write Protection Control Register Enable */
#define MCSPI_PWPMR_WPKEY_Pos 8
#define MCSPI_PWPMR_WPKEY_Msk (0xffffffu << MCSPI_PWPMR_WPKEY_Pos) /**< \brief (MCSPI_PWPMR) Write Protection Key */
#define MCSPI_PWPMR_WPKEY(value) ((MCSPI_PWPMR_WPKEY_Msk & ((value) << MCSPI_PWPMR_WPKEY_Pos)))
#define   MCSPI_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (MCSPI_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_MCSPI_COMPONENT_ */
