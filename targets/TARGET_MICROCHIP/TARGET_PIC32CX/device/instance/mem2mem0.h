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

#ifndef _PIC32CX_MEM2MEM0_INSTANCE_
#define _PIC32CX_MEM2MEM0_INSTANCE_

/* ========== Register definition for MEM2MEM0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MEM2MEM0_THR                    (0x40034000U) /**< \brief (MEM2MEM0) Transfer Holding Register */
  #define REG_MEM2MEM0_MR                     (0x40034004U) /**< \brief (MEM2MEM0) Mode Register */
  #define REG_MEM2MEM0_IER                    (0x40034008U) /**< \brief (MEM2MEM0) Interrupt Enable Register */
  #define REG_MEM2MEM0_IDR                    (0x4003400CU) /**< \brief (MEM2MEM0) Interrupt Disable Register */
  #define REG_MEM2MEM0_IMR                    (0x40034010U) /**< \brief (MEM2MEM0) Interrupt Mask Register */
  #define REG_MEM2MEM0_ISR                    (0x40034014U) /**< \brief (MEM2MEM0) Interrupt Status Register */
  #define REG_MEM2MEM0_WPMR                   (0x40034018U) /**< \brief (MEM2MEM0) Write Protection Mode Register */
  #define REG_MEM2MEM0_RPR                    (0x40034100U) /**< \brief (MEM2MEM0) Receive Pointer Register */
  #define REG_MEM2MEM0_RCR                    (0x40034104U) /**< \brief (MEM2MEM0) Receive Counter Register */
  #define REG_MEM2MEM0_TPR                    (0x40034108U) /**< \brief (MEM2MEM0) Transmit Pointer Register */
  #define REG_MEM2MEM0_TCR                    (0x4003410CU) /**< \brief (MEM2MEM0) Transmit Counter Register */
  #define REG_MEM2MEM0_RNPR                   (0x40034110U) /**< \brief (MEM2MEM0) Receive Next Pointer Register */
  #define REG_MEM2MEM0_RNCR                   (0x40034114U) /**< \brief (MEM2MEM0) Receive Next Counter Register */
  #define REG_MEM2MEM0_TNPR                   (0x40034118U) /**< \brief (MEM2MEM0) Transmit Next Pointer Register */
  #define REG_MEM2MEM0_TNCR                   (0x4003411CU) /**< \brief (MEM2MEM0) Transmit Next Counter Register */
  #define REG_MEM2MEM0_PTCR                   (0x40034120U) /**< \brief (MEM2MEM0) Transfer Control Register */
  #define REG_MEM2MEM0_PTSR                   (0x40034124U) /**< \brief (MEM2MEM0) Transfer Status Register */
  #define REG_MEM2MEM0_PWPMR                  (0x40034128U) /**< \brief (MEM2MEM0) Write Protection Mode Register */
#else
  #define REG_MEM2MEM0_THR   (*(__IO uint32_t*)0x40034000U) /**< \brief (MEM2MEM0) Transfer Holding Register */
  #define REG_MEM2MEM0_MR    (*(__IO uint32_t*)0x40034004U) /**< \brief (MEM2MEM0) Mode Register */
  #define REG_MEM2MEM0_IER   (*(__O  uint32_t*)0x40034008U) /**< \brief (MEM2MEM0) Interrupt Enable Register */
  #define REG_MEM2MEM0_IDR   (*(__O  uint32_t*)0x4003400CU) /**< \brief (MEM2MEM0) Interrupt Disable Register */
  #define REG_MEM2MEM0_IMR   (*(__I  uint32_t*)0x40034010U) /**< \brief (MEM2MEM0) Interrupt Mask Register */
  #define REG_MEM2MEM0_ISR   (*(__I  uint32_t*)0x40034014U) /**< \brief (MEM2MEM0) Interrupt Status Register */
  #define REG_MEM2MEM0_WPMR  (*(__IO uint32_t*)0x40034018U) /**< \brief (MEM2MEM0) Write Protection Mode Register */
  #define REG_MEM2MEM0_RPR   (*(__IO uint32_t*)0x40034100U) /**< \brief (MEM2MEM0) Receive Pointer Register */
  #define REG_MEM2MEM0_RCR   (*(__IO uint32_t*)0x40034104U) /**< \brief (MEM2MEM0) Receive Counter Register */
  #define REG_MEM2MEM0_TPR   (*(__IO uint32_t*)0x40034108U) /**< \brief (MEM2MEM0) Transmit Pointer Register */
  #define REG_MEM2MEM0_TCR   (*(__IO uint32_t*)0x4003410CU) /**< \brief (MEM2MEM0) Transmit Counter Register */
  #define REG_MEM2MEM0_RNPR  (*(__IO uint32_t*)0x40034110U) /**< \brief (MEM2MEM0) Receive Next Pointer Register */
  #define REG_MEM2MEM0_RNCR  (*(__IO uint32_t*)0x40034114U) /**< \brief (MEM2MEM0) Receive Next Counter Register */
  #define REG_MEM2MEM0_TNPR  (*(__IO uint32_t*)0x40034118U) /**< \brief (MEM2MEM0) Transmit Next Pointer Register */
  #define REG_MEM2MEM0_TNCR  (*(__IO uint32_t*)0x4003411CU) /**< \brief (MEM2MEM0) Transmit Next Counter Register */
  #define REG_MEM2MEM0_PTCR  (*(__O  uint32_t*)0x40034120U) /**< \brief (MEM2MEM0) Transfer Control Register */
  #define REG_MEM2MEM0_PTSR  (*(__I  uint32_t*)0x40034124U) /**< \brief (MEM2MEM0) Transfer Status Register */
  #define REG_MEM2MEM0_PWPMR (*(__IO uint32_t*)0x40034128U) /**< \brief (MEM2MEM0) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MEM2MEM0_INSTANCE_ */
