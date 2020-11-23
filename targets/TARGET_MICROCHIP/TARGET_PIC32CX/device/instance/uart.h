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

#ifndef _PIC32CX_UART_INSTANCE_
#define _PIC32CX_UART_INSTANCE_

/* ========== Register definition for UART peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_UART_CR                     (0x48010000U) /**< \brief (UART) Control Register */
  #define REG_UART_MR                     (0x48010004U) /**< \brief (UART) Mode Register */
  #define REG_UART_IER                    (0x48010008U) /**< \brief (UART) Interrupt Enable Register */
  #define REG_UART_IDR                    (0x4801000CU) /**< \brief (UART) Interrupt Disable Register */
  #define REG_UART_IMR                    (0x48010010U) /**< \brief (UART) Interrupt Mask Register */
  #define REG_UART_SR                     (0x48010014U) /**< \brief (UART) Interrupt Status Register */
  #define REG_UART_RHR                    (0x48010018U) /**< \brief (UART) Receive Holding Register */
  #define REG_UART_THR                    (0x4801001CU) /**< \brief (UART) Transmit Holding Register */
  #define REG_UART_BRGR                   (0x48010020U) /**< \brief (UART) Baud Rate Generator Register */
  #define REG_UART_CMPR                   (0x48010024U) /**< \brief (UART) Comparison Register */
  #define REG_UART_RTOR                   (0x48010028U) /**< \brief (UART) Receiver Time-out Register */
  #define REG_UART_CSR                    (0x4801002CU) /**< \brief (UART) Current Status Register */
  #define REG_UART_WPMR                   (0x480100E4U) /**< \brief (UART) Write Protection Mode Register */
  #define REG_UART_RPR                    (0x48010100U) /**< \brief (UART) Receive Pointer Register */
  #define REG_UART_RCR                    (0x48010104U) /**< \brief (UART) Receive Counter Register */
  #define REG_UART_TPR                    (0x48010108U) /**< \brief (UART) Transmit Pointer Register */
  #define REG_UART_TCR                    (0x4801010CU) /**< \brief (UART) Transmit Counter Register */
  #define REG_UART_RNPR                   (0x48010110U) /**< \brief (UART) Receive Next Pointer Register */
  #define REG_UART_RNCR                   (0x48010114U) /**< \brief (UART) Receive Next Counter Register */
  #define REG_UART_TNPR                   (0x48010118U) /**< \brief (UART) Transmit Next Pointer Register */
  #define REG_UART_TNCR                   (0x4801011CU) /**< \brief (UART) Transmit Next Counter Register */
  #define REG_UART_PTCR                   (0x48010120U) /**< \brief (UART) Transfer Control Register */
  #define REG_UART_PTSR                   (0x48010124U) /**< \brief (UART) Transfer Status Register */
  #define REG_UART_PWPMR                  (0x48010128U) /**< \brief (UART) Write Protection Mode Register */
#else
  #define REG_UART_CR    (*(__O  uint32_t*)0x48010000U) /**< \brief (UART) Control Register */
  #define REG_UART_MR    (*(__IO uint32_t*)0x48010004U) /**< \brief (UART) Mode Register */
  #define REG_UART_IER   (*(__O  uint32_t*)0x48010008U) /**< \brief (UART) Interrupt Enable Register */
  #define REG_UART_IDR   (*(__O  uint32_t*)0x4801000CU) /**< \brief (UART) Interrupt Disable Register */
  #define REG_UART_IMR   (*(__I  uint32_t*)0x48010010U) /**< \brief (UART) Interrupt Mask Register */
  #define REG_UART_SR    (*(__I  uint32_t*)0x48010014U) /**< \brief (UART) Interrupt Status Register */
  #define REG_UART_RHR   (*(__I  uint32_t*)0x48010018U) /**< \brief (UART) Receive Holding Register */
  #define REG_UART_THR   (*(__O  uint32_t*)0x4801001CU) /**< \brief (UART) Transmit Holding Register */
  #define REG_UART_BRGR  (*(__IO uint32_t*)0x48010020U) /**< \brief (UART) Baud Rate Generator Register */
  #define REG_UART_CMPR  (*(__IO uint32_t*)0x48010024U) /**< \brief (UART) Comparison Register */
  #define REG_UART_RTOR  (*(__IO uint32_t*)0x48010028U) /**< \brief (UART) Receiver Time-out Register */
  #define REG_UART_CSR   (*(__I  uint32_t*)0x4801002CU) /**< \brief (UART) Current Status Register */
  #define REG_UART_WPMR  (*(__IO uint32_t*)0x480100E4U) /**< \brief (UART) Write Protection Mode Register */
  #define REG_UART_RPR   (*(__IO uint32_t*)0x48010100U) /**< \brief (UART) Receive Pointer Register */
  #define REG_UART_RCR   (*(__IO uint32_t*)0x48010104U) /**< \brief (UART) Receive Counter Register */
  #define REG_UART_TPR   (*(__IO uint32_t*)0x48010108U) /**< \brief (UART) Transmit Pointer Register */
  #define REG_UART_TCR   (*(__IO uint32_t*)0x4801010CU) /**< \brief (UART) Transmit Counter Register */
  #define REG_UART_RNPR  (*(__IO uint32_t*)0x48010110U) /**< \brief (UART) Receive Next Pointer Register */
  #define REG_UART_RNCR  (*(__IO uint32_t*)0x48010114U) /**< \brief (UART) Receive Next Counter Register */
  #define REG_UART_TNPR  (*(__IO uint32_t*)0x48010118U) /**< \brief (UART) Transmit Next Pointer Register */
  #define REG_UART_TNCR  (*(__IO uint32_t*)0x4801011CU) /**< \brief (UART) Transmit Next Counter Register */
  #define REG_UART_PTCR  (*(__O  uint32_t*)0x48010120U) /**< \brief (UART) Transfer Control Register */
  #define REG_UART_PTSR  (*(__I  uint32_t*)0x48010124U) /**< \brief (UART) Transfer Status Register */
  #define REG_UART_PWPMR (*(__IO uint32_t*)0x48010128U) /**< \brief (UART) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_UART_INSTANCE_ */
