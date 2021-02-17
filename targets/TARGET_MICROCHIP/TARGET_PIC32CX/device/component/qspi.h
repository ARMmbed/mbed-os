/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2021, Microchip Technology Inc.                                */
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

#ifndef _PIC32CX_QSPI_COMPONENT_
#define _PIC32CX_QSPI_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Quad Serial Peripheral Interface */
/* ============================================================================= */
/** \addtogroup PIC32CX_QSPI Quad Serial Peripheral Interface */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Qspi hardware registers */
typedef struct {
  __O  uint32_t QSPI_CR;       /**< \brief (Qspi Offset: 0x0) Control Register */
  __IO uint32_t QSPI_MR;       /**< \brief (Qspi Offset: 0x4) Mode Register */
  __I  uint32_t QSPI_RDR;      /**< \brief (Qspi Offset: 0x8) Receive Data Register */
  __O  uint32_t QSPI_TDR;      /**< \brief (Qspi Offset: 0xC) Transmit Data Register */
  __I  uint32_t QSPI_ISR;      /**< \brief (Qspi Offset: 0x10) Interrupt Status Register */
  __O  uint32_t QSPI_IER;      /**< \brief (Qspi Offset: 0x14) Interrupt Enable Register */
  __O  uint32_t QSPI_IDR;      /**< \brief (Qspi Offset: 0x18) Interrupt Disable Register */
  __I  uint32_t QSPI_IMR;      /**< \brief (Qspi Offset: 0x1C) Interrupt Mask Register */
  __IO uint32_t QSPI_SCR;      /**< \brief (Qspi Offset: 0x20) Serial Clock Register */
  __IO uint32_t QSPI_SR;       /**< \brief (Qspi Offset: 0x24) Status Register */
  __I  uint32_t Reserved1[2];
  __IO uint32_t QSPI_IAR;      /**< \brief (Qspi Offset: 0x30) Instruction Address Register */
  __IO uint32_t QSPI_WICR;     /**< \brief (Qspi Offset: 0x34) Write Instruction Code Register */
  __IO uint32_t QSPI_IFR;      /**< \brief (Qspi Offset: 0x38) Instruction Frame Register */
  __IO uint32_t QSPI_RICR;     /**< \brief (Qspi Offset: 0x3C) Read Instruction Code Register */
  __IO uint32_t QSPI_SMR;      /**< \brief (Qspi Offset: 0x40) Scrambling Mode Register */
  __O  uint32_t QSPI_SKR;      /**< \brief (Qspi Offset: 0x44) Scrambling Key Register */
  __I  uint32_t Reserved2[3];
  __IO uint32_t QSPI_WRACNT;   /**< \brief (Qspi Offset: 0x54) Write Access Counter Register */
  __I  uint32_t Reserved3[3];
  __IO uint32_t QSPI_TOUT;     /**< \brief (Qspi Offset: 0x64) Timeout Register */
  __I  uint32_t Reserved4[31];
  __IO uint32_t QSPI_WPMR;     /**< \brief (Qspi Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t QSPI_WPSR;     /**< \brief (Qspi Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved5[5];
  __IO uint32_t QSPI_RPR;      /**< \brief (Qspi Offset: 0x100) Receive Pointer Register */
  __IO uint32_t QSPI_RCR;      /**< \brief (Qspi Offset: 0x104) Receive Counter Register */
  __IO uint32_t QSPI_TPR;      /**< \brief (Qspi Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t QSPI_TCR;      /**< \brief (Qspi Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t QSPI_RNPR;     /**< \brief (Qspi Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t QSPI_RNCR;     /**< \brief (Qspi Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t QSPI_TNPR;     /**< \brief (Qspi Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t QSPI_TNCR;     /**< \brief (Qspi Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t QSPI_PTCR;     /**< \brief (Qspi Offset: 0x120) Transfer Control Register */
  __I  uint32_t QSPI_PTSR;     /**< \brief (Qspi Offset: 0x124) Transfer Status Register */
  __IO uint32_t QSPI_PWPMR;    /**< \brief (Qspi Offset: 0x128) Write Protection Mode Register */
} Qspi0;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- QSPI_CR : (QSPI Offset: 0x0) Control Register -------- */
#define QSPI_CR_QSPIEN (0x1u << 0) /**< \brief (QSPI_CR) QSPI Enable */
#define QSPI_CR_QSPIDIS (0x1u << 1) /**< \brief (QSPI_CR) QSPI Disable */
#define QSPI_CR_SWRST (0x1u << 7) /**< \brief (QSPI_CR) QSPI Software Reset */
#define QSPI_CR_UPDCFG (0x1u << 8) /**< \brief (QSPI_CR) Update Configuration */
#define QSPI_CR_STTFR (0x1u << 9) /**< \brief (QSPI_CR) Start Transfer */
#define QSPI_CR_RTOUT (0x1u << 10) /**< \brief (QSPI_CR) Reset Time-out */
#define QSPI_CR_LASTXFER (0x1u << 24) /**< \brief (QSPI_CR) Last Transfer */
/* -------- QSPI_MR : (QSPI Offset: 0x4) Mode Register -------- */
#define QSPI_MR_SMM (0x1u << 0) /**< \brief (QSPI_MR) Serial Memory Mode */
#define   QSPI_MR_SMM_SPI (0x0u << 0) /**< \brief (QSPI_MR) The QSPI is in SPI mode. */
#define   QSPI_MR_SMM_MEMORY (0x1u << 0) /**< \brief (QSPI_MR) The QSPI is in Serial Memory mode. */
#define QSPI_MR_WDRBT (0x1u << 2) /**< \brief (QSPI_MR) Wait Data Read Before Transfer */
#define   QSPI_MR_WDRBT_DISABLED (0x0u << 2) /**< \brief (QSPI_MR) No effect. In SPI mode, a transfer can be initiated whatever the state of QSPI_RDR is. */
#define   QSPI_MR_WDRBT_ENABLED (0x1u << 2) /**< \brief (QSPI_MR) In SPI mode, a transfer can start only if QSPI_RDR is empty, i.e., does not contain any unread data. This mode prevents overrun error in reception. */
#define QSPI_MR_CSMODE_Pos 4
#define QSPI_MR_CSMODE_Msk (0x3u << QSPI_MR_CSMODE_Pos) /**< \brief (QSPI_MR) Chip Select Mode */
#define QSPI_MR_CSMODE(value) ((QSPI_MR_CSMODE_Msk & ((value) << QSPI_MR_CSMODE_Pos)))
#define   QSPI_MR_CSMODE_NOT_RELOADED (0x0u << 4) /**< \brief (QSPI_MR) The chip select is deasserted if QSPI_TDR.TD has not been reloaded before the end of the current transfer. */
#define   QSPI_MR_CSMODE_LASTXFER (0x1u << 4) /**< \brief (QSPI_MR) The chip select is deasserted when the bit LASTXFER is written to '1' and the character written in QSPI_TDR.TD has been transferred. */
#define   QSPI_MR_CSMODE_SYSTEMATICALLY (0x2u << 4) /**< \brief (QSPI_MR) The chip select is deasserted systematically after each transfer. */
#define QSPI_MR_TAMPCLR (0x1u << 7) /**< \brief (QSPI_MR) Tamper Clear Enable */
#define QSPI_MR_NBBITS_Pos 8
#define QSPI_MR_NBBITS_Msk (0xfu << QSPI_MR_NBBITS_Pos) /**< \brief (QSPI_MR) Number Of Bits Per Transfer */
#define QSPI_MR_NBBITS(value) ((QSPI_MR_NBBITS_Msk & ((value) << QSPI_MR_NBBITS_Pos)))
#define   QSPI_MR_NBBITS_8_BIT (0x0u << 8) /**< \brief (QSPI_MR) 8 bits for transfer */
#define   QSPI_MR_NBBITS_16_BIT (0x8u << 8) /**< \brief (QSPI_MR) 16 bits for transfer */
#define QSPI_MR_DLYBCT_Pos 16
#define QSPI_MR_DLYBCT_Msk (0xffu << QSPI_MR_DLYBCT_Pos) /**< \brief (QSPI_MR) Delay Between Consecutive Transfers */
#define QSPI_MR_DLYBCT(value) ((QSPI_MR_DLYBCT_Msk & ((value) << QSPI_MR_DLYBCT_Pos)))
#define QSPI_MR_DLYCS_Pos 24
#define QSPI_MR_DLYCS_Msk (0xffu << QSPI_MR_DLYCS_Pos) /**< \brief (QSPI_MR) Minimum Inactive QCS Delay */
#define QSPI_MR_DLYCS(value) ((QSPI_MR_DLYCS_Msk & ((value) << QSPI_MR_DLYCS_Pos)))
/* -------- QSPI_RDR : (QSPI Offset: 0x8) Receive Data Register -------- */
#define QSPI_RDR_RD_Pos 0
#define QSPI_RDR_RD_Msk (0xffffu << QSPI_RDR_RD_Pos) /**< \brief (QSPI_RDR) Receive Data */
/* -------- QSPI_TDR : (QSPI Offset: 0xC) Transmit Data Register -------- */
#define QSPI_TDR_TD_Pos 0
#define QSPI_TDR_TD_Msk (0xffffu << QSPI_TDR_TD_Pos) /**< \brief (QSPI_TDR) Transmit Data */
#define QSPI_TDR_TD(value) ((QSPI_TDR_TD_Msk & ((value) << QSPI_TDR_TD_Pos)))
/* -------- QSPI_ISR : (QSPI Offset: 0x10) Interrupt Status Register -------- */
#define QSPI_ISR_RDRF (0x1u << 0) /**< \brief (QSPI_ISR) Receive Data Register Full (cleared by reading QSPI_RDR) */
#define QSPI_ISR_TDRE (0x1u << 1) /**< \brief (QSPI_ISR) Transmit Data Register Empty (cleared by writing QSPI_TDR) */
#define QSPI_ISR_TXEMPTY (0x1u << 2) /**< \brief (QSPI_ISR) Transmission Registers Empty (cleared by writing QSPI_TDR) */
#define QSPI_ISR_OVRES (0x1u << 3) /**< \brief (QSPI_ISR) Overrun Error Status (cleared on read) */
#define QSPI_ISR_ENDRX (0x1u << 4) /**< \brief (QSPI_ISR) End of RX buffer */
#define QSPI_ISR_ENDTX (0x1u << 5) /**< \brief (QSPI_ISR) End of TX buffer */
#define QSPI_ISR_RXBUFF (0x1u << 6) /**< \brief (QSPI_ISR) RX Buffer Full */
#define QSPI_ISR_TXBUFE (0x1u << 7) /**< \brief (QSPI_ISR) TX Buffer Empty */
#define QSPI_ISR_CSR (0x1u << 8) /**< \brief (QSPI_ISR) Chip Select Rise (cleared on read) */
#define QSPI_ISR_INSTRE (0x1u << 10) /**< \brief (QSPI_ISR) Instruction End Status (cleared on read) */
#define QSPI_ISR_LWRA (0x1u << 11) /**< \brief (QSPI_ISR) Last Write Access (cleared on read) */
#define QSPI_ISR_QITF (0x1u << 12) /**< \brief (QSPI_ISR) QSPI Interrupt Fall */
#define QSPI_ISR_QITR (0x1u << 13) /**< \brief (QSPI_ISR) QSPI Interrupt Rise */
#define QSPI_ISR_CSFA (0x1u << 14) /**< \brief (QSPI_ISR) Chip Select Fall Autoclear */
#define QSPI_ISR_CSRA (0x1u << 15) /**< \brief (QSPI_ISR) Chip Select Rise Autoclear */
#define QSPI_ISR_TOUT (0x1u << 17) /**< \brief (QSPI_ISR) QSPI Time-out */
/* -------- QSPI_IER : (QSPI Offset: 0x14) Interrupt Enable Register -------- */
#define QSPI_IER_RDRF (0x1u << 0) /**< \brief (QSPI_IER) Receive Data Register Full Interrupt Enable */
#define QSPI_IER_TDRE (0x1u << 1) /**< \brief (QSPI_IER) Transmit Data Register Empty Interrupt Enable */
#define QSPI_IER_TXEMPTY (0x1u << 2) /**< \brief (QSPI_IER) Transmission Registers Empty Enable */
#define QSPI_IER_OVRES (0x1u << 3) /**< \brief (QSPI_IER) Overrun Error Interrupt Enable */
#define QSPI_IER_ENDRX (0x1u << 4) /**< \brief (QSPI_IER) End of Receive Buffer Interrupt Enable */
#define QSPI_IER_ENDTX (0x1u << 5) /**< \brief (QSPI_IER) End of Transmit Buffer Interrupt Enable */
#define QSPI_IER_RXBUFF (0x1u << 6) /**< \brief (QSPI_IER) Receive Buffer Full Interrupt Enable */
#define QSPI_IER_TXBUFE (0x1u << 7) /**< \brief (QSPI_IER) Transmit Buffer Empty Interrupt Enable */
#define QSPI_IER_CSR (0x1u << 8) /**< \brief (QSPI_IER) Chip Select Rise Interrupt Enable */
#define QSPI_IER_INSTRE (0x1u << 10) /**< \brief (QSPI_IER) Instruction End Interrupt Enable */
#define QSPI_IER_LWRA (0x1u << 11) /**< \brief (QSPI_IER) Last Write Access Interrupt Enable */
#define QSPI_IER_QITF (0x1u << 12) /**< \brief (QSPI_IER) QSPI Interrupt Fall Interrupt Enable */
#define QSPI_IER_QITR (0x1u << 13) /**< \brief (QSPI_IER) QSPI Interrupt Rise Interrupt Enable */
#define QSPI_IER_CSFA (0x1u << 14) /**< \brief (QSPI_IER) Chip Select Fall Autoclear Interrupt Enable */
#define QSPI_IER_CSRA (0x1u << 15) /**< \brief (QSPI_IER) Chip Select Rise Autoclear Interrupt Enable */
#define QSPI_IER_TOUT (0x1u << 17) /**< \brief (QSPI_IER) QSPI Time-out Interrupt Enable */
/* -------- QSPI_IDR : (QSPI Offset: 0x18) Interrupt Disable Register -------- */
#define QSPI_IDR_RDRF (0x1u << 0) /**< \brief (QSPI_IDR) Receive Data Register Full Interrupt Disable */
#define QSPI_IDR_TDRE (0x1u << 1) /**< \brief (QSPI_IDR) Transmit Data Register Empty Interrupt Disable */
#define QSPI_IDR_TXEMPTY (0x1u << 2) /**< \brief (QSPI_IDR) Transmission Registers Empty Disable */
#define QSPI_IDR_OVRES (0x1u << 3) /**< \brief (QSPI_IDR) Overrun Error Interrupt Disable */
#define QSPI_IDR_ENDRX (0x1u << 4) /**< \brief (QSPI_IDR) End of Receive Buffer Interrupt Disable */
#define QSPI_IDR_ENDTX (0x1u << 5) /**< \brief (QSPI_IDR) End of Transmit Buffer Interrupt Disable */
#define QSPI_IDR_RXBUFF (0x1u << 6) /**< \brief (QSPI_IDR) Receive Buffer Full Interrupt Disable */
#define QSPI_IDR_TXBUFE (0x1u << 7) /**< \brief (QSPI_IDR) Transmit Buffer Empty Interrupt Disable */
#define QSPI_IDR_CSR (0x1u << 8) /**< \brief (QSPI_IDR) Chip Select Rise Interrupt Disable */
#define QSPI_IDR_INSTRE (0x1u << 10) /**< \brief (QSPI_IDR) Instruction End Interrupt Disable */
#define QSPI_IDR_LWRA (0x1u << 11) /**< \brief (QSPI_IDR) Last Write Access Interrupt Disable */
#define QSPI_IDR_QITF (0x1u << 12) /**< \brief (QSPI_IDR) QSPI Interrupt Fall Interrupt Disable */
#define QSPI_IDR_QITR (0x1u << 13) /**< \brief (QSPI_IDR) QSPI Interrupt Rise Interrupt Disable */
#define QSPI_IDR_CSFA (0x1u << 14) /**< \brief (QSPI_IDR) Chip Select Fall Autoclear Interrupt Disable */
#define QSPI_IDR_CSRA (0x1u << 15) /**< \brief (QSPI_IDR) Chip Select Rise Autoclear Interrupt Disable */
#define QSPI_IDR_TOUT (0x1u << 17) /**< \brief (QSPI_IDR) QSPI Time-out Interrupt Disable */
/* -------- QSPI_IMR : (QSPI Offset: 0x1C) Interrupt Mask Register -------- */
#define QSPI_IMR_RDRF (0x1u << 0) /**< \brief (QSPI_IMR) Receive Data Register Full Interrupt Mask */
#define QSPI_IMR_TDRE (0x1u << 1) /**< \brief (QSPI_IMR) Transmit Data Register Empty Interrupt Mask */
#define QSPI_IMR_TXEMPTY (0x1u << 2) /**< \brief (QSPI_IMR) Transmission Registers Empty Mask */
#define QSPI_IMR_OVRES (0x1u << 3) /**< \brief (QSPI_IMR) Overrun Error Interrupt Mask */
#define QSPI_IMR_ENDRX (0x1u << 4) /**< \brief (QSPI_IMR) End of Receive Buffer Interrupt Mask */
#define QSPI_IMR_ENDTX (0x1u << 5) /**< \brief (QSPI_IMR) End of Transmit Buffer Interrupt Mask */
#define QSPI_IMR_RXBUFF (0x1u << 6) /**< \brief (QSPI_IMR) Receive Buffer Full Interrupt Mask */
#define QSPI_IMR_TXBUFE (0x1u << 7) /**< \brief (QSPI_IMR) Transmit Buffer Empty Interrupt Mask */
#define QSPI_IMR_CSR (0x1u << 8) /**< \brief (QSPI_IMR) Chip Select Rise Interrupt Mask */
#define QSPI_IMR_INSTRE (0x1u << 10) /**< \brief (QSPI_IMR) Instruction End Interrupt Mask */
#define QSPI_IMR_LWRA (0x1u << 11) /**< \brief (QSPI_IMR) Last Write Access Interrupt Mask */
#define QSPI_IMR_QITF (0x1u << 12) /**< \brief (QSPI_IMR) QSPI Interrupt Fall Interrupt Mask */
#define QSPI_IMR_QITR (0x1u << 13) /**< \brief (QSPI_IMR) QSPI Interrupt Rise Interrupt Mask */
#define QSPI_IMR_CSFA (0x1u << 14) /**< \brief (QSPI_IMR) Chip Select Fall Autoclear Interrupt Mask */
#define QSPI_IMR_CSRA (0x1u << 15) /**< \brief (QSPI_IMR) Chip Select Rise Autoclear Interrupt Mask */
#define QSPI_IMR_TOUT (0x1u << 17) /**< \brief (QSPI_IMR) QSPI Time-out Interrupt Mask */
/* -------- QSPI_SCR : (QSPI Offset: 0x20) Serial Clock Register -------- */
#define QSPI_SCR_CPOL (0x1u << 0) /**< \brief (QSPI_SCR) Clock Polarity */
#define QSPI_SCR_CPHA (0x1u << 1) /**< \brief (QSPI_SCR) Clock Phase */
#define QSPI_SCR_DLYBS_Pos 16
#define QSPI_SCR_DLYBS_Msk (0xffu << QSPI_SCR_DLYBS_Pos) /**< \brief (QSPI_SCR) Delay Before QSCK */
#define QSPI_SCR_DLYBS(value) ((QSPI_SCR_DLYBS_Msk & ((value) << QSPI_SCR_DLYBS_Pos)))
/* -------- QSPI_SR : (QSPI Offset: 0x24) Status Register -------- */
#define QSPI_SR_SYNCBSY (0x1u << 0) /**< \brief (QSPI_SR) Synchronization Busy */
#define QSPI_SR_QSPIENS (0x1u << 1) /**< \brief (QSPI_SR) QSPI Enable Status */
#define QSPI_SR_CSS (0x1u << 2) /**< \brief (QSPI_SR) Chip Select Status */
#define QSPI_SR_RBUSY (0x1u << 3) /**< \brief (QSPI_SR) Read Busy */
#define QSPI_SR_HIDLE (0x1u << 4) /**< \brief (QSPI_SR) QSPI Idle */
/* -------- QSPI_IAR : (QSPI Offset: 0x30) Instruction Address Register -------- */
#define QSPI_IAR_ADDR_Pos 0
#define QSPI_IAR_ADDR_Msk (0xffffffffu << QSPI_IAR_ADDR_Pos) /**< \brief (QSPI_IAR) Address */
#define QSPI_IAR_ADDR(value) ((QSPI_IAR_ADDR_Msk & ((value) << QSPI_IAR_ADDR_Pos)))
/* -------- QSPI_WICR : (QSPI Offset: 0x34) Write Instruction Code Register -------- */
#define QSPI_WICR_WRINST_Pos 0
#define QSPI_WICR_WRINST_Msk (0xffu << QSPI_WICR_WRINST_Pos) /**< \brief (QSPI_WICR) Write Instruction Code */
#define QSPI_WICR_WRINST(value) ((QSPI_WICR_WRINST_Msk & ((value) << QSPI_WICR_WRINST_Pos)))
#define QSPI_WICR_WROPT_Pos 16
#define QSPI_WICR_WROPT_Msk (0xffu << QSPI_WICR_WROPT_Pos) /**< \brief (QSPI_WICR) Write Option Code */
#define QSPI_WICR_WROPT(value) ((QSPI_WICR_WROPT_Msk & ((value) << QSPI_WICR_WROPT_Pos)))
/* -------- QSPI_IFR : (QSPI Offset: 0x38) Instruction Frame Register -------- */
#define QSPI_IFR_WIDTH_Pos 0
#define QSPI_IFR_WIDTH_Msk (0xfu << QSPI_IFR_WIDTH_Pos) /**< \brief (QSPI_IFR) Width of Instruction Code, Address, Option Code and Data */
#define QSPI_IFR_WIDTH(value) ((QSPI_IFR_WIDTH_Msk & ((value) << QSPI_IFR_WIDTH_Pos)))
#define   QSPI_IFR_WIDTH_SINGLE_BIT_SPI (0x0u << 0) /**< \brief (QSPI_IFR) Instruction: Single-bit SPI / Address-Option: Single-bit SPI / Data: Single-bit SPI */
#define   QSPI_IFR_WIDTH_DUAL_OUTPUT (0x1u << 0) /**< \brief (QSPI_IFR) Instruction: Single-bit SPI / Address-Option: Single-bit SPI / Data: Dual SPI */
#define   QSPI_IFR_WIDTH_QUAD_OUTPUT (0x2u << 0) /**< \brief (QSPI_IFR) Instruction: Single-bit SPI / Address-Option: Single-bit SPI / Data: Quad SPI */
#define   QSPI_IFR_WIDTH_DUAL_IO (0x3u << 0) /**< \brief (QSPI_IFR) Instruction: Single-bit SPI / Address-Option: Dual SPI / Data: Dual SPI */
#define   QSPI_IFR_WIDTH_QUAD_IO (0x4u << 0) /**< \brief (QSPI_IFR) Instruction: Single-bit SPI / Address-Option: Quad SPI / Data: Quad SPI */
#define   QSPI_IFR_WIDTH_DUAL_CMD (0x5u << 0) /**< \brief (QSPI_IFR) Instruction: Dual SPI / Address-Option: Dual SPI / Data: Dual SPI */
#define   QSPI_IFR_WIDTH_QUAD_CMD (0x6u << 0) /**< \brief (QSPI_IFR) Instruction: Quad SPI / Address-Option: Quad SPI / Data: Quad SPI */
#define QSPI_IFR_INSTEN (0x1u << 4) /**< \brief (QSPI_IFR) Instruction Enable */
#define QSPI_IFR_ADDREN (0x1u << 5) /**< \brief (QSPI_IFR) Address Enable */
#define QSPI_IFR_OPTEN (0x1u << 6) /**< \brief (QSPI_IFR) Option Enable */
#define QSPI_IFR_DATAEN (0x1u << 7) /**< \brief (QSPI_IFR) Data Enable */
#define QSPI_IFR_OPTL_Pos 8
#define QSPI_IFR_OPTL_Msk (0x3u << QSPI_IFR_OPTL_Pos) /**< \brief (QSPI_IFR) Option Code Length */
#define QSPI_IFR_OPTL(value) ((QSPI_IFR_OPTL_Msk & ((value) << QSPI_IFR_OPTL_Pos)))
#define   QSPI_IFR_OPTL_OPTION_1BIT (0x0u << 8) /**< \brief (QSPI_IFR) The option code is 1 bit long. */
#define   QSPI_IFR_OPTL_OPTION_2BIT (0x1u << 8) /**< \brief (QSPI_IFR) The option code is 2 bits long. */
#define   QSPI_IFR_OPTL_OPTION_4BIT (0x2u << 8) /**< \brief (QSPI_IFR) The option code is 4 bits long. */
#define   QSPI_IFR_OPTL_OPTION_8BIT (0x3u << 8) /**< \brief (QSPI_IFR) The option code is 8 bits long. */
#define QSPI_IFR_ADDRL_Pos 10
#define QSPI_IFR_ADDRL_Msk (0x3u << QSPI_IFR_ADDRL_Pos) /**< \brief (QSPI_IFR) Address Length */
#define QSPI_IFR_ADDRL(value) ((QSPI_IFR_ADDRL_Msk & ((value) << QSPI_IFR_ADDRL_Pos)))
#define   QSPI_IFR_ADDRL_8_BIT (0x0u << 10) /**< \brief (QSPI_IFR) 8-bit address size */
#define   QSPI_IFR_ADDRL_16_BIT (0x1u << 10) /**< \brief (QSPI_IFR) 16-bit address size */
#define   QSPI_IFR_ADDRL_24_BIT (0x2u << 10) /**< \brief (QSPI_IFR) 24-bit address size */
#define   QSPI_IFR_ADDRL_32_BIT (0x3u << 10) /**< \brief (QSPI_IFR) 32-bit address size */
#define QSPI_IFR_TFRTYP (0x1u << 12) /**< \brief (QSPI_IFR) Data Transfer Type */
#define   QSPI_IFR_TFRTYP_TRSFR_REGISTER (0x0u << 12) /**< \brief (QSPI_IFR) Read/Write transfer from the serial memory. Read at random location (fetch) in the serial Flash memory is not possible. */
#define   QSPI_IFR_TFRTYP_TRSFR_MEMORY (0x1u << 12) /**< \brief (QSPI_IFR) Read/Write data transfer from the serial memory. If enabled, scrambling is performed. Read at random location (fetch) in the serial Flash memory is possible. Transfers are launched only through the system bus interface. */
#define QSPI_IFR_CRM (0x1u << 14) /**< \brief (QSPI_IFR) Continuous Read Mode */
#define   QSPI_IFR_CRM_DISABLED (0x0u << 14) /**< \brief (QSPI_IFR) Continuous Read mode is disabled. */
#define   QSPI_IFR_CRM_ENABLED (0x1u << 14) /**< \brief (QSPI_IFR) Continuous Read mode is enabled. */
#define QSPI_IFR_DDREN (0x1u << 15) /**< \brief (QSPI_IFR) DDR Mode Enable */
#define   QSPI_IFR_DDREN_DISABLED (0x0u << 15) /**< \brief (QSPI_IFR) Transfers are performed in Single Data Rate mode. */
#define   QSPI_IFR_DDREN_ENABLED (0x1u << 15) /**< \brief (QSPI_IFR) Transfers are performed in Double Data Rate mode, whereas the instruction field is still transferred in Single Data Rate mode. */
#define QSPI_IFR_NBDUM_Pos 16
#define QSPI_IFR_NBDUM_Msk (0x1fu << QSPI_IFR_NBDUM_Pos) /**< \brief (QSPI_IFR) Number Of Dummy Cycles */
#define QSPI_IFR_NBDUM(value) ((QSPI_IFR_NBDUM_Msk & ((value) << QSPI_IFR_NBDUM_Pos)))
#define QSPI_IFR_SMRM (0x1u << 23) /**< \brief (QSPI_IFR) Serial Memory Register Mode */
#define QSPI_IFR_APBTFRTYP (0x1u << 24) /**< \brief (QSPI_IFR) Peripheral BusTransfer Type */
#define QSPI_IFR_DQSEN (0x1u << 25) /**< \brief (QSPI_IFR) DQS Sampling Enable */
#define QSPI_IFR_DDRCMDEN (0x1u << 26) /**< \brief (QSPI_IFR) DDR Mode Command Enable */
#define   QSPI_IFR_DDRCMDEN_DISABLED (0x0u << 26) /**< \brief (QSPI_IFR) Transfer of instruction field is performed in Single Data Rate mode even if DDREN is written to '1'. */
#define   QSPI_IFR_DDRCMDEN_ENABLED (0x1u << 26) /**< \brief (QSPI_IFR) Transfer of instruction field is performed in Double Data Rate mode if DDREN bit is written to '1'. If DDREN is written to '0', the instruction field is sent in Single Data Rate mode. */
#define QSPI_IFR_PROTTYP_Pos 28
#define QSPI_IFR_PROTTYP_Msk (0x3u << QSPI_IFR_PROTTYP_Pos) /**< \brief (QSPI_IFR) Protocol Type */
#define QSPI_IFR_PROTTYP(value) ((QSPI_IFR_PROTTYP_Msk & ((value) << QSPI_IFR_PROTTYP_Pos)))
#define   QSPI_IFR_PROTTYP_STD_SPI (0x0u << 28) /**< \brief (QSPI_IFR) Standard (Q)SPI Protocol */
/* -------- QSPI_RICR : (QSPI Offset: 0x3C) Read Instruction Code Register -------- */
#define QSPI_RICR_RDINST_Pos 0
#define QSPI_RICR_RDINST_Msk (0xffu << QSPI_RICR_RDINST_Pos) /**< \brief (QSPI_RICR) Read Instruction Code */
#define QSPI_RICR_RDINST(value) ((QSPI_RICR_RDINST_Msk & ((value) << QSPI_RICR_RDINST_Pos)))
#define QSPI_RICR_RDOPT_Pos 16
#define QSPI_RICR_RDOPT_Msk (0xffu << QSPI_RICR_RDOPT_Pos) /**< \brief (QSPI_RICR) Read Option Code */
#define QSPI_RICR_RDOPT(value) ((QSPI_RICR_RDOPT_Msk & ((value) << QSPI_RICR_RDOPT_Pos)))
/* -------- QSPI_SMR : (QSPI Offset: 0x40) Scrambling Mode Register -------- */
#define QSPI_SMR_SCREN (0x1u << 0) /**< \brief (QSPI_SMR) Scrambling/Unscrambling Enable */
#define   QSPI_SMR_SCREN_DISABLED (0x0u << 0) /**< \brief (QSPI_SMR) The scrambling/unscrambling is disabled. */
#define   QSPI_SMR_SCREN_ENABLED (0x1u << 0) /**< \brief (QSPI_SMR) The scrambling/unscrambling is enabled. */
#define QSPI_SMR_RVDIS (0x1u << 1) /**< \brief (QSPI_SMR) Scrambling/Unscrambling Random Value Disable */
#define QSPI_SMR_SCRKL (0x1u << 2) /**< \brief (QSPI_SMR) Scrambling Key Lock */
/* -------- QSPI_SKR : (QSPI Offset: 0x44) Scrambling Key Register -------- */
#define QSPI_SKR_USRK_Pos 0
#define QSPI_SKR_USRK_Msk (0xffffffffu << QSPI_SKR_USRK_Pos) /**< \brief (QSPI_SKR) User Scrambling Key */
#define QSPI_SKR_USRK(value) ((QSPI_SKR_USRK_Msk & ((value) << QSPI_SKR_USRK_Pos)))
/* -------- QSPI_WRACNT : (QSPI Offset: 0x54) Write Access Counter Register -------- */
#define QSPI_WRACNT_NBWRA_Pos 0
#define QSPI_WRACNT_NBWRA_Msk (0xffffffffu << QSPI_WRACNT_NBWRA_Pos) /**< \brief (QSPI_WRACNT) Number of Write Accesses */
#define QSPI_WRACNT_NBWRA(value) ((QSPI_WRACNT_NBWRA_Msk & ((value) << QSPI_WRACNT_NBWRA_Pos)))
/* -------- QSPI_TOUT : (QSPI Offset: 0x64) Timeout Register -------- */
#define QSPI_TOUT_TCNTM_Pos 0
#define QSPI_TOUT_TCNTM_Msk (0xffffu << QSPI_TOUT_TCNTM_Pos) /**< \brief (QSPI_TOUT) Time-out Counter Maximum Value */
#define QSPI_TOUT_TCNTM(value) ((QSPI_TOUT_TCNTM_Msk & ((value) << QSPI_TOUT_TCNTM_Pos)))
/* -------- QSPI_WPMR : (QSPI Offset: 0xE4) Write Protection Mode Register -------- */
#define QSPI_WPMR_WPEN (0x1u << 0) /**< \brief (QSPI_WPMR) Write Protection Enable */
#define QSPI_WPMR_WPITEN (0x1u << 1) /**< \brief (QSPI_WPMR) Write Protection Interrupt Enable */
#define QSPI_WPMR_WPCREN (0x1u << 2) /**< \brief (QSPI_WPMR) Write Protection Control Register Enable */
#define QSPI_WPMR_WPKEY_Pos 8
#define QSPI_WPMR_WPKEY_Msk (0xffffffu << QSPI_WPMR_WPKEY_Pos) /**< \brief (QSPI_WPMR) Write Protection Key */
#define QSPI_WPMR_WPKEY(value) ((QSPI_WPMR_WPKEY_Msk & ((value) << QSPI_WPMR_WPKEY_Pos)))
#define   QSPI_WPMR_WPKEY_PASSWD (0x515350u << 8) /**< \brief (QSPI_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- QSPI_WPSR : (QSPI Offset: 0xE8) Write Protection Status Register -------- */
#define QSPI_WPSR_WPVS (0x1u << 0) /**< \brief (QSPI_WPSR) Write Protection Violation Status */
#define QSPI_WPSR_WPVSRC_Pos 8
#define QSPI_WPSR_WPVSRC_Msk (0xffu << QSPI_WPSR_WPVSRC_Pos) /**< \brief (QSPI_WPSR) Write Protection Violation Source */
/* -------- QSPI_RPR : (QSPI Offset: 0x100) Receive Pointer Register -------- */
#define QSPI_RPR_RXPTR_Pos 0
#define QSPI_RPR_RXPTR_Msk (0xffffffffu << QSPI_RPR_RXPTR_Pos) /**< \brief (QSPI_RPR) Receive Pointer Register */
#define QSPI_RPR_RXPTR(value) ((QSPI_RPR_RXPTR_Msk & ((value) << QSPI_RPR_RXPTR_Pos)))
/* -------- QSPI_RCR : (QSPI Offset: 0x104) Receive Counter Register -------- */
#define QSPI_RCR_RXCTR_Pos 0
#define QSPI_RCR_RXCTR_Msk (0xffffu << QSPI_RCR_RXCTR_Pos) /**< \brief (QSPI_RCR) Receive Counter Register */
#define QSPI_RCR_RXCTR(value) ((QSPI_RCR_RXCTR_Msk & ((value) << QSPI_RCR_RXCTR_Pos)))
/* -------- QSPI_TPR : (QSPI Offset: 0x108) Transmit Pointer Register -------- */
#define QSPI_TPR_TXPTR_Pos 0
#define QSPI_TPR_TXPTR_Msk (0xffffffffu << QSPI_TPR_TXPTR_Pos) /**< \brief (QSPI_TPR) Transmit Counter Register */
#define QSPI_TPR_TXPTR(value) ((QSPI_TPR_TXPTR_Msk & ((value) << QSPI_TPR_TXPTR_Pos)))
/* -------- QSPI_TCR : (QSPI Offset: 0x10C) Transmit Counter Register -------- */
#define QSPI_TCR_TXCTR_Pos 0
#define QSPI_TCR_TXCTR_Msk (0xffffu << QSPI_TCR_TXCTR_Pos) /**< \brief (QSPI_TCR) Transmit Counter Register */
#define QSPI_TCR_TXCTR(value) ((QSPI_TCR_TXCTR_Msk & ((value) << QSPI_TCR_TXCTR_Pos)))
/* -------- QSPI_RNPR : (QSPI Offset: 0x110) Receive Next Pointer Register -------- */
#define QSPI_RNPR_RXNPTR_Pos 0
#define QSPI_RNPR_RXNPTR_Msk (0xffffffffu << QSPI_RNPR_RXNPTR_Pos) /**< \brief (QSPI_RNPR) Receive Next Pointer */
#define QSPI_RNPR_RXNPTR(value) ((QSPI_RNPR_RXNPTR_Msk & ((value) << QSPI_RNPR_RXNPTR_Pos)))
/* -------- QSPI_RNCR : (QSPI Offset: 0x114) Receive Next Counter Register -------- */
#define QSPI_RNCR_RXNCTR_Pos 0
#define QSPI_RNCR_RXNCTR_Msk (0xffffu << QSPI_RNCR_RXNCTR_Pos) /**< \brief (QSPI_RNCR) Receive Next Counter */
#define QSPI_RNCR_RXNCTR(value) ((QSPI_RNCR_RXNCTR_Msk & ((value) << QSPI_RNCR_RXNCTR_Pos)))
/* -------- QSPI_TNPR : (QSPI Offset: 0x118) Transmit Next Pointer Register -------- */
#define QSPI_TNPR_TXNPTR_Pos 0
#define QSPI_TNPR_TXNPTR_Msk (0xffffffffu << QSPI_TNPR_TXNPTR_Pos) /**< \brief (QSPI_TNPR) Transmit Next Pointer */
#define QSPI_TNPR_TXNPTR(value) ((QSPI_TNPR_TXNPTR_Msk & ((value) << QSPI_TNPR_TXNPTR_Pos)))
/* -------- QSPI_TNCR : (QSPI Offset: 0x11C) Transmit Next Counter Register -------- */
#define QSPI_TNCR_TXNCTR_Pos 0
#define QSPI_TNCR_TXNCTR_Msk (0xffffu << QSPI_TNCR_TXNCTR_Pos) /**< \brief (QSPI_TNCR) Transmit Counter Next */
#define QSPI_TNCR_TXNCTR(value) ((QSPI_TNCR_TXNCTR_Msk & ((value) << QSPI_TNCR_TXNCTR_Pos)))
/* -------- QSPI_PTCR : (QSPI Offset: 0x120) Transfer Control Register -------- */
#define QSPI_PTCR_RXTEN (0x1u << 0) /**< \brief (QSPI_PTCR) Receiver Transfer Enable */
#define QSPI_PTCR_RXTDIS (0x1u << 1) /**< \brief (QSPI_PTCR) Receiver Transfer Disable */
#define QSPI_PTCR_TXTEN (0x1u << 8) /**< \brief (QSPI_PTCR) Transmitter Transfer Enable */
#define QSPI_PTCR_TXTDIS (0x1u << 9) /**< \brief (QSPI_PTCR) Transmitter Transfer Disable */
#define QSPI_PTCR_RXCBEN (0x1u << 16) /**< \brief (QSPI_PTCR) Receiver Circular Buffer Enable */
#define QSPI_PTCR_RXCBDIS (0x1u << 17) /**< \brief (QSPI_PTCR) Receiver Circular Buffer Disable */
#define QSPI_PTCR_TXCBEN (0x1u << 18) /**< \brief (QSPI_PTCR) Transmitter Circular Buffer Enable */
#define QSPI_PTCR_TXCBDIS (0x1u << 19) /**< \brief (QSPI_PTCR) Transmitter Circular Buffer Disable */
#define QSPI_PTCR_ERRCLR (0x1u << 24) /**< \brief (QSPI_PTCR) Transfer Bus Error Clear */
/* -------- QSPI_PTSR : (QSPI Offset: 0x124) Transfer Status Register -------- */
#define QSPI_PTSR_RXTEN (0x1u << 0) /**< \brief (QSPI_PTSR) Receiver Transfer Enable */
#define QSPI_PTSR_TXTEN (0x1u << 8) /**< \brief (QSPI_PTSR) Transmitter Transfer Enable */
#define QSPI_PTSR_RXCBEN (0x1u << 16) /**< \brief (QSPI_PTSR) Receiver Circular Buffer Enable */
#define QSPI_PTSR_TXCBEN (0x1u << 18) /**< \brief (QSPI_PTSR) Transmitter Circular Buffer Enable */
#define QSPI_PTSR_ERR (0x1u << 24) /**< \brief (QSPI_PTSR) Transfer Bus Error */
/* -------- QSPI_PWPMR : (QSPI Offset: 0x128) Write Protection Mode Register -------- */
#define QSPI_PWPMR_WPPTREN (0x1u << 0) /**< \brief (QSPI_PWPMR) Write Protection Pointer Registers Enable */
#define QSPI_PWPMR_WPCTREN (0x1u << 1) /**< \brief (QSPI_PWPMR) Write Protection Counter Registers Enable */
#define QSPI_PWPMR_WPCREN (0x1u << 2) /**< \brief (QSPI_PWPMR) Write Protection Control Register Enable */
#define QSPI_PWPMR_WPKEY_Pos 8
#define QSPI_PWPMR_WPKEY_Msk (0xffffffu << QSPI_PWPMR_WPKEY_Pos) /**< \brief (QSPI_PWPMR) Write Protection Key */
#define QSPI_PWPMR_WPKEY(value) ((QSPI_PWPMR_WPKEY_Msk & ((value) << QSPI_PWPMR_WPKEY_Pos)))
#define   QSPI_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (QSPI_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_QSPI_COMPONENT_ */
