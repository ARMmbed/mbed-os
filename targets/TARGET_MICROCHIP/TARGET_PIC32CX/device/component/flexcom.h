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

#ifndef _PIC32CX_FLEXCOM_COMPONENT_
#define _PIC32CX_FLEXCOM_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Flexible Serial Communication */
/* ============================================================================= */
/** \addtogroup PIC32CX_FLEXCOM Flexible Serial Communication */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Flexcom hardware registers */
typedef struct {
  __IO uint32_t FLEXCOM_MR;          /**< \brief (Flexcom Offset: 0x0) FLEXCOM Mode Register */
  __I  uint32_t Reserved1[3];
  __I  uint32_t FLEXCOM_RHR;         /**< \brief (Flexcom Offset: 0x10) FLEXCOM Receive Holding Register */
  __I  uint32_t Reserved2[3];
  __IO uint32_t FLEXCOM_THR;         /**< \brief (Flexcom Offset: 0x20) FLEXCOM Transmit Holding Register */
  __I  uint32_t Reserved3[55];
  __IO uint32_t FLEXCOM_RPR;         /**< \brief (Flexcom Offset: 0x100) Receive Pointer Register */
  __IO uint32_t FLEXCOM_RCR;         /**< \brief (Flexcom Offset: 0x104) Receive Counter Register */
  __IO uint32_t FLEXCOM_TPR;         /**< \brief (Flexcom Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t FLEXCOM_TCR;         /**< \brief (Flexcom Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t FLEXCOM_RNPR;        /**< \brief (Flexcom Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t FLEXCOM_RNCR;        /**< \brief (Flexcom Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t FLEXCOM_TNPR;        /**< \brief (Flexcom Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t FLEXCOM_TNCR;        /**< \brief (Flexcom Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t FLEXCOM_PTCR;        /**< \brief (Flexcom Offset: 0x120) Transfer Control Register */
  __I  uint32_t FLEXCOM_PTSR;        /**< \brief (Flexcom Offset: 0x124) Transfer Status Register */
  __IO uint32_t FLEXCOM_PWPMR;       /**< \brief (Flexcom Offset: 0x128) Write Protection Mode Register */
} Flexcom;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- FLEXCOM_MR : (FLEXCOM Offset: 0x0) FLEXCOM Mode Register -------- */
#define FLEXCOM_MR_OPMODE_Pos 0
#define FLEXCOM_MR_OPMODE_Msk (0x3u << FLEXCOM_MR_OPMODE_Pos) /**< \brief (FLEXCOM_MR) FLEXCOM Operating Mode */
#define FLEXCOM_MR_OPMODE(value) ((FLEXCOM_MR_OPMODE_Msk & ((value) << FLEXCOM_MR_OPMODE_Pos)))
#define   FLEXCOM_MR_OPMODE_NO_COM (0x0u << 0) /**< \brief (FLEXCOM_MR) No communication */
#define   FLEXCOM_MR_OPMODE_USART (0x1u << 0) /**< \brief (FLEXCOM_MR) All UART related protocols are selected (RS232, RS485, IrDA, ISO7816, LIN,) SPI/TWI related registers are not accessible and have no impact on IOs. */
#define   FLEXCOM_MR_OPMODE_SPI (0x2u << 0) /**< \brief (FLEXCOM_MR) SPI operating mode is selected. USART/TWI related registers are not accessible and have no impact on IOs. */
#define   FLEXCOM_MR_OPMODE_TWI (0x3u << 0) /**< \brief (FLEXCOM_MR) All TWI related protocols are selected (TWI, SMBus). USART/SPI related registers are not accessible and have no impact on IOs. */
/* -------- FLEXCOM_RHR : (FLEXCOM Offset: 0x10) FLEXCOM Receive Holding Register -------- */
#define FLEXCOM_RHR_RXDATA_Pos 0
#define FLEXCOM_RHR_RXDATA_Msk (0xffffu << FLEXCOM_RHR_RXDATA_Pos) /**< \brief (FLEXCOM_RHR) Receive Data */
/* -------- FLEXCOM_THR : (FLEXCOM Offset: 0x20) FLEXCOM Transmit Holding Register -------- */
#define FLEXCOM_THR_TXDATA_Pos 0
#define FLEXCOM_THR_TXDATA_Msk (0xffffu << FLEXCOM_THR_TXDATA_Pos) /**< \brief (FLEXCOM_THR) Transmit Data */
#define FLEXCOM_THR_TXDATA(value) ((FLEXCOM_THR_TXDATA_Msk & ((value) << FLEXCOM_THR_TXDATA_Pos)))
/* -------- FLEXCOM_RPR : (FLEXCOM Offset: 0x100) Receive Pointer Register -------- */
#define FLEXCOM_RPR_RXPTR_Pos 0
#define FLEXCOM_RPR_RXPTR_Msk (0xffffffffu << FLEXCOM_RPR_RXPTR_Pos) /**< \brief (FLEXCOM_RPR) Receive Pointer Register */
#define FLEXCOM_RPR_RXPTR(value) ((FLEXCOM_RPR_RXPTR_Msk & ((value) << FLEXCOM_RPR_RXPTR_Pos)))
/* -------- FLEXCOM_RCR : (FLEXCOM Offset: 0x104) Receive Counter Register -------- */
#define FLEXCOM_RCR_RXCTR_Pos 0
#define FLEXCOM_RCR_RXCTR_Msk (0xffffu << FLEXCOM_RCR_RXCTR_Pos) /**< \brief (FLEXCOM_RCR) Receive Counter Register */
#define FLEXCOM_RCR_RXCTR(value) ((FLEXCOM_RCR_RXCTR_Msk & ((value) << FLEXCOM_RCR_RXCTR_Pos)))
/* -------- FLEXCOM_TPR : (FLEXCOM Offset: 0x108) Transmit Pointer Register -------- */
#define FLEXCOM_TPR_TXPTR_Pos 0
#define FLEXCOM_TPR_TXPTR_Msk (0xffffffffu << FLEXCOM_TPR_TXPTR_Pos) /**< \brief (FLEXCOM_TPR) Transmit Counter Register */
#define FLEXCOM_TPR_TXPTR(value) ((FLEXCOM_TPR_TXPTR_Msk & ((value) << FLEXCOM_TPR_TXPTR_Pos)))
/* -------- FLEXCOM_TCR : (FLEXCOM Offset: 0x10C) Transmit Counter Register -------- */
#define FLEXCOM_TCR_TXCTR_Pos 0
#define FLEXCOM_TCR_TXCTR_Msk (0xffffu << FLEXCOM_TCR_TXCTR_Pos) /**< \brief (FLEXCOM_TCR) Transmit Counter Register */
#define FLEXCOM_TCR_TXCTR(value) ((FLEXCOM_TCR_TXCTR_Msk & ((value) << FLEXCOM_TCR_TXCTR_Pos)))
/* -------- FLEXCOM_RNPR : (FLEXCOM Offset: 0x110) Receive Next Pointer Register -------- */
#define FLEXCOM_RNPR_RXNPTR_Pos 0
#define FLEXCOM_RNPR_RXNPTR_Msk (0xffffffffu << FLEXCOM_RNPR_RXNPTR_Pos) /**< \brief (FLEXCOM_RNPR) Receive Next Pointer */
#define FLEXCOM_RNPR_RXNPTR(value) ((FLEXCOM_RNPR_RXNPTR_Msk & ((value) << FLEXCOM_RNPR_RXNPTR_Pos)))
/* -------- FLEXCOM_RNCR : (FLEXCOM Offset: 0x114) Receive Next Counter Register -------- */
#define FLEXCOM_RNCR_RXNCTR_Pos 0
#define FLEXCOM_RNCR_RXNCTR_Msk (0xffffu << FLEXCOM_RNCR_RXNCTR_Pos) /**< \brief (FLEXCOM_RNCR) Receive Next Counter */
#define FLEXCOM_RNCR_RXNCTR(value) ((FLEXCOM_RNCR_RXNCTR_Msk & ((value) << FLEXCOM_RNCR_RXNCTR_Pos)))
/* -------- FLEXCOM_TNPR : (FLEXCOM Offset: 0x118) Transmit Next Pointer Register -------- */
#define FLEXCOM_TNPR_TXNPTR_Pos 0
#define FLEXCOM_TNPR_TXNPTR_Msk (0xffffffffu << FLEXCOM_TNPR_TXNPTR_Pos) /**< \brief (FLEXCOM_TNPR) Transmit Next Pointer */
#define FLEXCOM_TNPR_TXNPTR(value) ((FLEXCOM_TNPR_TXNPTR_Msk & ((value) << FLEXCOM_TNPR_TXNPTR_Pos)))
/* -------- FLEXCOM_TNCR : (FLEXCOM Offset: 0x11C) Transmit Next Counter Register -------- */
#define FLEXCOM_TNCR_TXNCTR_Pos 0
#define FLEXCOM_TNCR_TXNCTR_Msk (0xffffu << FLEXCOM_TNCR_TXNCTR_Pos) /**< \brief (FLEXCOM_TNCR) Transmit Counter Next */
#define FLEXCOM_TNCR_TXNCTR(value) ((FLEXCOM_TNCR_TXNCTR_Msk & ((value) << FLEXCOM_TNCR_TXNCTR_Pos)))
/* -------- FLEXCOM_PTCR : (FLEXCOM Offset: 0x120) Transfer Control Register -------- */
#define FLEXCOM_PTCR_RXTEN (0x1u << 0) /**< \brief (FLEXCOM_PTCR) Receiver Transfer Enable */
#define FLEXCOM_PTCR_RXTDIS (0x1u << 1) /**< \brief (FLEXCOM_PTCR) Receiver Transfer Disable */
#define FLEXCOM_PTCR_TXTEN (0x1u << 8) /**< \brief (FLEXCOM_PTCR) Transmitter Transfer Enable */
#define FLEXCOM_PTCR_TXTDIS (0x1u << 9) /**< \brief (FLEXCOM_PTCR) Transmitter Transfer Disable */
#define FLEXCOM_PTCR_RXCBEN (0x1u << 16) /**< \brief (FLEXCOM_PTCR) Receiver Circular Buffer Enable */
#define FLEXCOM_PTCR_RXCBDIS (0x1u << 17) /**< \brief (FLEXCOM_PTCR) Receiver Circular Buffer Disable */
#define FLEXCOM_PTCR_TXCBEN (0x1u << 18) /**< \brief (FLEXCOM_PTCR) Transmitter Circular Buffer Enable */
#define FLEXCOM_PTCR_TXCBDIS (0x1u << 19) /**< \brief (FLEXCOM_PTCR) Transmitter Circular Buffer Disable */
#define FLEXCOM_PTCR_ERRCLR (0x1u << 24) /**< \brief (FLEXCOM_PTCR) Transfer Bus Error Clear */
/* -------- FLEXCOM_PTSR : (FLEXCOM Offset: 0x124) Transfer Status Register -------- */
#define FLEXCOM_PTSR_RXTEN (0x1u << 0) /**< \brief (FLEXCOM_PTSR) Receiver Transfer Enable */
#define FLEXCOM_PTSR_TXTEN (0x1u << 8) /**< \brief (FLEXCOM_PTSR) Transmitter Transfer Enable */
#define FLEXCOM_PTSR_RXCBEN (0x1u << 16) /**< \brief (FLEXCOM_PTSR) Receiver Circular Buffer Enable */
#define FLEXCOM_PTSR_TXCBEN (0x1u << 18) /**< \brief (FLEXCOM_PTSR) Transmitter Circular Buffer Enable */
#define FLEXCOM_PTSR_ERR (0x1u << 24) /**< \brief (FLEXCOM_PTSR) Transfer Bus Error */
/* -------- FLEXCOM_PWPMR : (FLEXCOM Offset: 0x128) Write Protection Mode Register -------- */
#define FLEXCOM_PWPMR_WPPTREN (0x1u << 0) /**< \brief (FLEXCOM_PWPMR) Write Protection Pointer Registers Enable */
#define FLEXCOM_PWPMR_WPCTREN (0x1u << 1) /**< \brief (FLEXCOM_PWPMR) Write Protection Counter Registers Enable */
#define FLEXCOM_PWPMR_WPCREN (0x1u << 2) /**< \brief (FLEXCOM_PWPMR) Write Protection Control Register Enable */
#define FLEXCOM_PWPMR_WPKEY_Pos 8
#define FLEXCOM_PWPMR_WPKEY_Msk (0xffffffu << FLEXCOM_PWPMR_WPKEY_Pos) /**< \brief (FLEXCOM_PWPMR) Write Protection Key */
#define FLEXCOM_PWPMR_WPKEY(value) ((FLEXCOM_PWPMR_WPKEY_Msk & ((value) << FLEXCOM_PWPMR_WPKEY_Pos)))
#define   FLEXCOM_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (FLEXCOM_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_FLEXCOM_COMPONENT_ */
