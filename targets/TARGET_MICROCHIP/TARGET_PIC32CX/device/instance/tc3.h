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

#ifndef _PIC32CX_TC3_INSTANCE_
#define _PIC32CX_TC3_INSTANCE_

/* ========== Register definition for TC3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TC3_CCR0                   (0x48008000U) /**< \brief (TC3) Channel Control Register (channel = 0) */
  #define REG_TC3_CMR0                   (0x48008004U) /**< \brief (TC3) Channel Mode Register (channel = 0) */
  #define REG_TC3_SMMR0                  (0x48008008U) /**< \brief (TC3) Stepper Motor Mode Register (channel = 0) */
  #define REG_TC3_CV0                    (0x48008010U) /**< \brief (TC3) Counter Value (channel = 0) */
  #define REG_TC3_RA0                    (0x48008014U) /**< \brief (TC3) Register A (channel = 0) */
  #define REG_TC3_RB0                    (0x48008018U) /**< \brief (TC3) Register B (channel = 0) */
  #define REG_TC3_RC0                    (0x4800801CU) /**< \brief (TC3) Register C (channel = 0) */
  #define REG_TC3_SR0                    (0x48008020U) /**< \brief (TC3) Interrupt Status Register (channel = 0) */
  #define REG_TC3_IER0                   (0x48008024U) /**< \brief (TC3) Interrupt Enable Register (channel = 0) */
  #define REG_TC3_IDR0                   (0x48008028U) /**< \brief (TC3) Interrupt Disable Register (channel = 0) */
  #define REG_TC3_IMR0                   (0x4800802CU) /**< \brief (TC3) Interrupt Mask Register (channel = 0) */
  #define REG_TC3_EMR0                   (0x48008030U) /**< \brief (TC3) Extended Mode Register (channel = 0) */
  #define REG_TC3_CSR0                   (0x48008034U) /**< \brief (TC3) Channel Status Register (channel = 0) */
  #define REG_TC3_SSR0                   (0x48008038U) /**< \brief (TC3) Safety Status Register (channel = 0) */
  #define REG_TC3_CCR1                   (0x48008040U) /**< \brief (TC3) Channel Control Register (channel = 1) */
  #define REG_TC3_CMR1                   (0x48008044U) /**< \brief (TC3) Channel Mode Register (channel = 1) */
  #define REG_TC3_SMMR1                  (0x48008048U) /**< \brief (TC3) Stepper Motor Mode Register (channel = 1) */
  #define REG_TC3_CV1                    (0x48008050U) /**< \brief (TC3) Counter Value (channel = 1) */
  #define REG_TC3_RA1                    (0x48008054U) /**< \brief (TC3) Register A (channel = 1) */
  #define REG_TC3_RB1                    (0x48008058U) /**< \brief (TC3) Register B (channel = 1) */
  #define REG_TC3_RC1                    (0x4800805CU) /**< \brief (TC3) Register C (channel = 1) */
  #define REG_TC3_SR1                    (0x48008060U) /**< \brief (TC3) Interrupt Status Register (channel = 1) */
  #define REG_TC3_IER1                   (0x48008064U) /**< \brief (TC3) Interrupt Enable Register (channel = 1) */
  #define REG_TC3_IDR1                   (0x48008068U) /**< \brief (TC3) Interrupt Disable Register (channel = 1) */
  #define REG_TC3_IMR1                   (0x4800806CU) /**< \brief (TC3) Interrupt Mask Register (channel = 1) */
  #define REG_TC3_EMR1                   (0x48008070U) /**< \brief (TC3) Extended Mode Register (channel = 1) */
  #define REG_TC3_CSR1                   (0x48008074U) /**< \brief (TC3) Channel Status Register (channel = 1) */
  #define REG_TC3_SSR1                   (0x48008078U) /**< \brief (TC3) Safety Status Register (channel = 1) */
  #define REG_TC3_CCR2                   (0x48008080U) /**< \brief (TC3) Channel Control Register (channel = 2) */
  #define REG_TC3_CMR2                   (0x48008084U) /**< \brief (TC3) Channel Mode Register (channel = 2) */
  #define REG_TC3_SMMR2                  (0x48008088U) /**< \brief (TC3) Stepper Motor Mode Register (channel = 2) */
  #define REG_TC3_CV2                    (0x48008090U) /**< \brief (TC3) Counter Value (channel = 2) */
  #define REG_TC3_RA2                    (0x48008094U) /**< \brief (TC3) Register A (channel = 2) */
  #define REG_TC3_RB2                    (0x48008098U) /**< \brief (TC3) Register B (channel = 2) */
  #define REG_TC3_RC2                    (0x4800809CU) /**< \brief (TC3) Register C (channel = 2) */
  #define REG_TC3_SR2                    (0x480080A0U) /**< \brief (TC3) Interrupt Status Register (channel = 2) */
  #define REG_TC3_IER2                   (0x480080A4U) /**< \brief (TC3) Interrupt Enable Register (channel = 2) */
  #define REG_TC3_IDR2                   (0x480080A8U) /**< \brief (TC3) Interrupt Disable Register (channel = 2) */
  #define REG_TC3_IMR2                   (0x480080ACU) /**< \brief (TC3) Interrupt Mask Register (channel = 2) */
  #define REG_TC3_EMR2                   (0x480080B0U) /**< \brief (TC3) Extended Mode Register (channel = 2) */
  #define REG_TC3_CSR2                   (0x480080B4U) /**< \brief (TC3) Channel Status Register (channel = 2) */
  #define REG_TC3_SSR2                   (0x480080B8U) /**< \brief (TC3) Safety Status Register (channel = 2) */
  #define REG_TC3_BCR                    (0x480080C0U) /**< \brief (TC3) Block Control Register */
  #define REG_TC3_BMR                    (0x480080C4U) /**< \brief (TC3) Block Mode Register */
  #define REG_TC3_QIER                   (0x480080C8U) /**< \brief (TC3) QDEC Interrupt Enable Register */
  #define REG_TC3_QIDR                   (0x480080CCU) /**< \brief (TC3) QDEC Interrupt Disable Register */
  #define REG_TC3_QIMR                   (0x480080D0U) /**< \brief (TC3) QDEC Interrupt Mask Register */
  #define REG_TC3_QISR                   (0x480080D4U) /**< \brief (TC3) QDEC Interrupt Status Register */
  #define REG_TC3_FMR                    (0x480080D8U) /**< \brief (TC3) Fault Mode Register */
  #define REG_TC3_QSR                    (0x480080DCU) /**< \brief (TC3) QDEC Status Register */
  #define REG_TC3_WPMR                   (0x480080E4U) /**< \brief (TC3) Write Protection Mode Register */
  #define REG_TC3_RPR                    (0x48008100U) /**< \brief (TC3) Receive Pointer Register */
  #define REG_TC3_RCR                    (0x48008104U) /**< \brief (TC3) Receive Counter Register */
  #define REG_TC3_RNPR                   (0x48008110U) /**< \brief (TC3) Receive Next Pointer Register */
  #define REG_TC3_RNCR                   (0x48008114U) /**< \brief (TC3) Receive Next Counter Register */
  #define REG_TC3_PTCR                   (0x48008120U) /**< \brief (TC3) Transfer Control Register */
  #define REG_TC3_PTSR                   (0x48008124U) /**< \brief (TC3) Transfer Status Register */
  #define REG_TC3_PWPMR                  (0x48008128U) /**< \brief (TC3) Write Protection Mode Register */
#else
  #define REG_TC3_CCR0  (*(__O  uint32_t*)0x48008000U) /**< \brief (TC3) Channel Control Register (channel = 0) */
  #define REG_TC3_CMR0  (*(__IO uint32_t*)0x48008004U) /**< \brief (TC3) Channel Mode Register (channel = 0) */
  #define REG_TC3_SMMR0 (*(__IO uint32_t*)0x48008008U) /**< \brief (TC3) Stepper Motor Mode Register (channel = 0) */
  #define REG_TC3_CV0   (*(__I  uint32_t*)0x48008010U) /**< \brief (TC3) Counter Value (channel = 0) */
  #define REG_TC3_RA0   (*(__IO uint32_t*)0x48008014U) /**< \brief (TC3) Register A (channel = 0) */
  #define REG_TC3_RB0   (*(__IO uint32_t*)0x48008018U) /**< \brief (TC3) Register B (channel = 0) */
  #define REG_TC3_RC0   (*(__IO uint32_t*)0x4800801CU) /**< \brief (TC3) Register C (channel = 0) */
  #define REG_TC3_SR0   (*(__I  uint32_t*)0x48008020U) /**< \brief (TC3) Interrupt Status Register (channel = 0) */
  #define REG_TC3_IER0  (*(__O  uint32_t*)0x48008024U) /**< \brief (TC3) Interrupt Enable Register (channel = 0) */
  #define REG_TC3_IDR0  (*(__O  uint32_t*)0x48008028U) /**< \brief (TC3) Interrupt Disable Register (channel = 0) */
  #define REG_TC3_IMR0  (*(__I  uint32_t*)0x4800802CU) /**< \brief (TC3) Interrupt Mask Register (channel = 0) */
  #define REG_TC3_EMR0  (*(__IO uint32_t*)0x48008030U) /**< \brief (TC3) Extended Mode Register (channel = 0) */
  #define REG_TC3_CSR0  (*(__I  uint32_t*)0x48008034U) /**< \brief (TC3) Channel Status Register (channel = 0) */
  #define REG_TC3_SSR0  (*(__I  uint32_t*)0x48008038U) /**< \brief (TC3) Safety Status Register (channel = 0) */
  #define REG_TC3_CCR1  (*(__O  uint32_t*)0x48008040U) /**< \brief (TC3) Channel Control Register (channel = 1) */
  #define REG_TC3_CMR1  (*(__IO uint32_t*)0x48008044U) /**< \brief (TC3) Channel Mode Register (channel = 1) */
  #define REG_TC3_SMMR1 (*(__IO uint32_t*)0x48008048U) /**< \brief (TC3) Stepper Motor Mode Register (channel = 1) */
  #define REG_TC3_CV1   (*(__I  uint32_t*)0x48008050U) /**< \brief (TC3) Counter Value (channel = 1) */
  #define REG_TC3_RA1   (*(__IO uint32_t*)0x48008054U) /**< \brief (TC3) Register A (channel = 1) */
  #define REG_TC3_RB1   (*(__IO uint32_t*)0x48008058U) /**< \brief (TC3) Register B (channel = 1) */
  #define REG_TC3_RC1   (*(__IO uint32_t*)0x4800805CU) /**< \brief (TC3) Register C (channel = 1) */
  #define REG_TC3_SR1   (*(__I  uint32_t*)0x48008060U) /**< \brief (TC3) Interrupt Status Register (channel = 1) */
  #define REG_TC3_IER1  (*(__O  uint32_t*)0x48008064U) /**< \brief (TC3) Interrupt Enable Register (channel = 1) */
  #define REG_TC3_IDR1  (*(__O  uint32_t*)0x48008068U) /**< \brief (TC3) Interrupt Disable Register (channel = 1) */
  #define REG_TC3_IMR1  (*(__I  uint32_t*)0x4800806CU) /**< \brief (TC3) Interrupt Mask Register (channel = 1) */
  #define REG_TC3_EMR1  (*(__IO uint32_t*)0x48008070U) /**< \brief (TC3) Extended Mode Register (channel = 1) */
  #define REG_TC3_CSR1  (*(__I  uint32_t*)0x48008074U) /**< \brief (TC3) Channel Status Register (channel = 1) */
  #define REG_TC3_SSR1  (*(__I  uint32_t*)0x48008078U) /**< \brief (TC3) Safety Status Register (channel = 1) */
  #define REG_TC3_CCR2  (*(__O  uint32_t*)0x48008080U) /**< \brief (TC3) Channel Control Register (channel = 2) */
  #define REG_TC3_CMR2  (*(__IO uint32_t*)0x48008084U) /**< \brief (TC3) Channel Mode Register (channel = 2) */
  #define REG_TC3_SMMR2 (*(__IO uint32_t*)0x48008088U) /**< \brief (TC3) Stepper Motor Mode Register (channel = 2) */
  #define REG_TC3_CV2   (*(__I  uint32_t*)0x48008090U) /**< \brief (TC3) Counter Value (channel = 2) */
  #define REG_TC3_RA2   (*(__IO uint32_t*)0x48008094U) /**< \brief (TC3) Register A (channel = 2) */
  #define REG_TC3_RB2   (*(__IO uint32_t*)0x48008098U) /**< \brief (TC3) Register B (channel = 2) */
  #define REG_TC3_RC2   (*(__IO uint32_t*)0x4800809CU) /**< \brief (TC3) Register C (channel = 2) */
  #define REG_TC3_SR2   (*(__I  uint32_t*)0x480080A0U) /**< \brief (TC3) Interrupt Status Register (channel = 2) */
  #define REG_TC3_IER2  (*(__O  uint32_t*)0x480080A4U) /**< \brief (TC3) Interrupt Enable Register (channel = 2) */
  #define REG_TC3_IDR2  (*(__O  uint32_t*)0x480080A8U) /**< \brief (TC3) Interrupt Disable Register (channel = 2) */
  #define REG_TC3_IMR2  (*(__I  uint32_t*)0x480080ACU) /**< \brief (TC3) Interrupt Mask Register (channel = 2) */
  #define REG_TC3_EMR2  (*(__IO uint32_t*)0x480080B0U) /**< \brief (TC3) Extended Mode Register (channel = 2) */
  #define REG_TC3_CSR2  (*(__I  uint32_t*)0x480080B4U) /**< \brief (TC3) Channel Status Register (channel = 2) */
  #define REG_TC3_SSR2  (*(__I  uint32_t*)0x480080B8U) /**< \brief (TC3) Safety Status Register (channel = 2) */
  #define REG_TC3_BCR   (*(__O  uint32_t*)0x480080C0U) /**< \brief (TC3) Block Control Register */
  #define REG_TC3_BMR   (*(__IO uint32_t*)0x480080C4U) /**< \brief (TC3) Block Mode Register */
  #define REG_TC3_QIER  (*(__O  uint32_t*)0x480080C8U) /**< \brief (TC3) QDEC Interrupt Enable Register */
  #define REG_TC3_QIDR  (*(__O  uint32_t*)0x480080CCU) /**< \brief (TC3) QDEC Interrupt Disable Register */
  #define REG_TC3_QIMR  (*(__I  uint32_t*)0x480080D0U) /**< \brief (TC3) QDEC Interrupt Mask Register */
  #define REG_TC3_QISR  (*(__I  uint32_t*)0x480080D4U) /**< \brief (TC3) QDEC Interrupt Status Register */
  #define REG_TC3_FMR   (*(__IO uint32_t*)0x480080D8U) /**< \brief (TC3) Fault Mode Register */
  #define REG_TC3_QSR   (*(__I  uint32_t*)0x480080DCU) /**< \brief (TC3) QDEC Status Register */
  #define REG_TC3_WPMR  (*(__IO uint32_t*)0x480080E4U) /**< \brief (TC3) Write Protection Mode Register */
  #define REG_TC3_RPR   (*(__IO uint32_t*)0x48008100U) /**< \brief (TC3) Receive Pointer Register */
  #define REG_TC3_RCR   (*(__IO uint32_t*)0x48008104U) /**< \brief (TC3) Receive Counter Register */
  #define REG_TC3_RNPR  (*(__IO uint32_t*)0x48008110U) /**< \brief (TC3) Receive Next Pointer Register */
  #define REG_TC3_RNCR  (*(__IO uint32_t*)0x48008114U) /**< \brief (TC3) Receive Next Counter Register */
  #define REG_TC3_PTCR  (*(__O  uint32_t*)0x48008120U) /**< \brief (TC3) Transfer Control Register */
  #define REG_TC3_PTSR  (*(__I  uint32_t*)0x48008124U) /**< \brief (TC3) Transfer Status Register */
  #define REG_TC3_PWPMR (*(__IO uint32_t*)0x48008128U) /**< \brief (TC3) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TC3_INSTANCE_ */
