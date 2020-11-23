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

#ifndef _PIC32CX_MEM2MEM1_INSTANCE_
#define _PIC32CX_MEM2MEM1_INSTANCE_

/* ========== Register definition for MEM2MEM1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MEM2MEM1_THR                    (0x48004000U) /**< \brief (MEM2MEM1) Transfer Holding Register */
  #define REG_MEM2MEM1_MR                     (0x48004004U) /**< \brief (MEM2MEM1) Mode Register */
  #define REG_MEM2MEM1_IER                    (0x48004008U) /**< \brief (MEM2MEM1) Interrupt Enable Register */
  #define REG_MEM2MEM1_IDR                    (0x4800400CU) /**< \brief (MEM2MEM1) Interrupt Disable Register */
  #define REG_MEM2MEM1_IMR                    (0x48004010U) /**< \brief (MEM2MEM1) Interrupt Mask Register */
  #define REG_MEM2MEM1_ISR                    (0x48004014U) /**< \brief (MEM2MEM1) Interrupt Status Register */
  #define REG_MEM2MEM1_WPMR                   (0x48004018U) /**< \brief (MEM2MEM1) Write Protection Mode Register */
  #define REG_MEM2MEM1_RPR                    (0x48004100U) /**< \brief (MEM2MEM1) Receive Pointer Register */
  #define REG_MEM2MEM1_RCR                    (0x48004104U) /**< \brief (MEM2MEM1) Receive Counter Register */
  #define REG_MEM2MEM1_TPR                    (0x48004108U) /**< \brief (MEM2MEM1) Transmit Pointer Register */
  #define REG_MEM2MEM1_TCR                    (0x4800410CU) /**< \brief (MEM2MEM1) Transmit Counter Register */
  #define REG_MEM2MEM1_RNPR                   (0x48004110U) /**< \brief (MEM2MEM1) Receive Next Pointer Register */
  #define REG_MEM2MEM1_RNCR                   (0x48004114U) /**< \brief (MEM2MEM1) Receive Next Counter Register */
  #define REG_MEM2MEM1_TNPR                   (0x48004118U) /**< \brief (MEM2MEM1) Transmit Next Pointer Register */
  #define REG_MEM2MEM1_TNCR                   (0x4800411CU) /**< \brief (MEM2MEM1) Transmit Next Counter Register */
  #define REG_MEM2MEM1_PTCR                   (0x48004120U) /**< \brief (MEM2MEM1) Transfer Control Register */
  #define REG_MEM2MEM1_PTSR                   (0x48004124U) /**< \brief (MEM2MEM1) Transfer Status Register */
  #define REG_MEM2MEM1_PWPMR                  (0x48004128U) /**< \brief (MEM2MEM1) Write Protection Mode Register */
#else
  #define REG_MEM2MEM1_THR   (*(__IO uint32_t*)0x48004000U) /**< \brief (MEM2MEM1) Transfer Holding Register */
  #define REG_MEM2MEM1_MR    (*(__IO uint32_t*)0x48004004U) /**< \brief (MEM2MEM1) Mode Register */
  #define REG_MEM2MEM1_IER   (*(__O  uint32_t*)0x48004008U) /**< \brief (MEM2MEM1) Interrupt Enable Register */
  #define REG_MEM2MEM1_IDR   (*(__O  uint32_t*)0x4800400CU) /**< \brief (MEM2MEM1) Interrupt Disable Register */
  #define REG_MEM2MEM1_IMR   (*(__I  uint32_t*)0x48004010U) /**< \brief (MEM2MEM1) Interrupt Mask Register */
  #define REG_MEM2MEM1_ISR   (*(__I  uint32_t*)0x48004014U) /**< \brief (MEM2MEM1) Interrupt Status Register */
  #define REG_MEM2MEM1_WPMR  (*(__IO uint32_t*)0x48004018U) /**< \brief (MEM2MEM1) Write Protection Mode Register */
  #define REG_MEM2MEM1_RPR   (*(__IO uint32_t*)0x48004100U) /**< \brief (MEM2MEM1) Receive Pointer Register */
  #define REG_MEM2MEM1_RCR   (*(__IO uint32_t*)0x48004104U) /**< \brief (MEM2MEM1) Receive Counter Register */
  #define REG_MEM2MEM1_TPR   (*(__IO uint32_t*)0x48004108U) /**< \brief (MEM2MEM1) Transmit Pointer Register */
  #define REG_MEM2MEM1_TCR   (*(__IO uint32_t*)0x4800410CU) /**< \brief (MEM2MEM1) Transmit Counter Register */
  #define REG_MEM2MEM1_RNPR  (*(__IO uint32_t*)0x48004110U) /**< \brief (MEM2MEM1) Receive Next Pointer Register */
  #define REG_MEM2MEM1_RNCR  (*(__IO uint32_t*)0x48004114U) /**< \brief (MEM2MEM1) Receive Next Counter Register */
  #define REG_MEM2MEM1_TNPR  (*(__IO uint32_t*)0x48004118U) /**< \brief (MEM2MEM1) Transmit Next Pointer Register */
  #define REG_MEM2MEM1_TNCR  (*(__IO uint32_t*)0x4800411CU) /**< \brief (MEM2MEM1) Transmit Next Counter Register */
  #define REG_MEM2MEM1_PTCR  (*(__O  uint32_t*)0x48004120U) /**< \brief (MEM2MEM1) Transfer Control Register */
  #define REG_MEM2MEM1_PTSR  (*(__I  uint32_t*)0x48004124U) /**< \brief (MEM2MEM1) Transfer Status Register */
  #define REG_MEM2MEM1_PWPMR (*(__IO uint32_t*)0x48004128U) /**< \brief (MEM2MEM1) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MEM2MEM1_INSTANCE_ */
