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

#ifndef _PIC32CX_PMC_INSTANCE_
#define _PIC32CX_PMC_INSTANCE_

/* ========== Register definition for PMC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_PMC_SCER                        (0x46800000U) /**< \brief (PMC) System Clock Enable Register */
  #define REG_PMC_SCDR                        (0x46800004U) /**< \brief (PMC) System Clock Disable Register */
  #define REG_PMC_SCSR                        (0x46800008U) /**< \brief (PMC) System Clock Status Register */
  #define REG_PMC_PLL_CTRL0                   (0x4680000CU) /**< \brief (PMC) PLL Control Register 0 */
  #define REG_PMC_PLL_CTRL1                   (0x46800010U) /**< \brief (PMC) PLL Control Register 1 */
  #define REG_PMC_PLL_CTRL2                   (0x46800014U) /**< \brief (PMC) PLL Control Register 2 */
  #define REG_PMC_PLL_SSR                     (0x46800018U) /**< \brief (PMC) PLL Spread Spectrum Register */
  #define REG_PMC_PLL_UPDT                    (0x46800020U) /**< \brief (PMC) PLL Update Register */
  #define REG_CKGR_MOR                        (0x46800024U) /**< \brief (PMC) Main Oscillator Register */
  #define REG_CKGR_MCFR                       (0x46800028U) /**< \brief (PMC) Main Clock Frequency Register */
  #define REG_PMC_CPU_CKR                     (0x4680002CU) /**< \brief (PMC) CPU Clock Register */
  #define REG_PMC_CPU_RATIO                   (0x46800030U) /**< \brief (PMC) CPU Clock Ratio Register */
  #define REG_PMC_PCK                         (0x46800044U) /**< \brief (PMC) Programmable Clock Register (chid = 0) */
  #define REG_PMC_IER                         (0x46800064U) /**< \brief (PMC) Interrupt Enable Register */
  #define REG_PMC_IDR                         (0x46800068U) /**< \brief (PMC) Interrupt Disable Register */
  #define REG_PMC_SR                          (0x4680006CU) /**< \brief (PMC) Status Register */
  #define REG_PMC_IMR                         (0x46800070U) /**< \brief (PMC) Interrupt Mask Register */
  #define REG_PMC_FSMR                        (0x46800074U) /**< \brief (PMC) Fast Start-up Mode Register */
  #define REG_PMC_WCR                         (0x46800078U) /**< \brief (PMC) Wake-up Control Register */
  #define REG_PMC_FOCR                        (0x4680007CU) /**< \brief (PMC) Fault Output Clear Register */
  #define REG_PMC_CPFSMR                      (0x46800080U) /**< \brief (PMC) Coprocessor Fast Start-up Mode Register */
  #define REG_PMC_WPMR                        (0x46800084U) /**< \brief (PMC) Write Protection Mode Register */
  #define REG_PMC_WPSR                        (0x46800088U) /**< \brief (PMC) Write Protection Status Register */
  #define REG_PMC_PCR                         (0x4680008CU) /**< \brief (PMC) Peripheral Control Register */
  #define REG_PMC_OCR                         (0x46800090U) /**< \brief (PMC) Oscillator Calibration Register */
  #define REG_PMC_SLPWK_AIPR                  (0x46800094U) /**< \brief (PMC) SleepWalking Activity In Progress Register */
  #define REG_PMC_SLPWKCR                     (0x46800098U) /**< \brief (PMC) SleepWalking Control Register */
  #define REG_PMC_MCKLIM                      (0x468000A0U) /**< \brief (PMC) MCK0 Monitor Limits Register */
  #define REG_PMC_CSR0                        (0x468000A4U) /**< \brief (PMC) Peripheral Clock Status Register 0 */
  #define REG_PMC_CSR1                        (0x468000A8U) /**< \brief (PMC) Peripheral Clock Status Register 1 */
  #define REG_PMC_CSR2                        (0x468000ACU) /**< \brief (PMC) Peripheral Clock Status Register 2 */
  #define REG_PMC_CSR3                        (0x468000B0U) /**< \brief (PMC) Peripheral Clock Status Register 3 */
  #define REG_PMC_GCSR0                       (0x468000C4U) /**< \brief (PMC) Generic Clock Status Register 0 */
  #define REG_PMC_GCSR1                       (0x468000C8U) /**< \brief (PMC) Generic Clock Status Register 1 */
  #define REG_PMC_GCSR2                       (0x468000CCU) /**< \brief (PMC) Generic Clock Status Register 2 */
  #define REG_PMC_GCSR3                       (0x468000D0U) /**< \brief (PMC) Generic Clock Status Register 3 */
  #define REG_PMC_PLL_IER                     (0x468000E4U) /**< \brief (PMC) PLL Interrupt Enable Register */
  #define REG_PMC_PLL_IDR                     (0x468000E8U) /**< \brief (PMC) PLL Interrupt Disable Register */
  #define REG_PMC_PLL_IMR                     (0x468000ECU) /**< \brief (PMC) PLL Interrupt Mask Register */
  #define REG_PMC_PLL_ISR0                    (0x468000F0U) /**< \brief (PMC) PLL Interrupt Status Register 0 */
  #define REG_PMC_PLL_ISR1                    (0x468000F4U) /**< \brief (PMC) PLL Interrupt Status Register 1 */
#else
  #define REG_PMC_SCER       (*(__O  uint32_t*)0x46800000U) /**< \brief (PMC) System Clock Enable Register */
  #define REG_PMC_SCDR       (*(__O  uint32_t*)0x46800004U) /**< \brief (PMC) System Clock Disable Register */
  #define REG_PMC_SCSR       (*(__I  uint32_t*)0x46800008U) /**< \brief (PMC) System Clock Status Register */
  #define REG_PMC_PLL_CTRL0  (*(__IO uint32_t*)0x4680000CU) /**< \brief (PMC) PLL Control Register 0 */
  #define REG_PMC_PLL_CTRL1  (*(__IO uint32_t*)0x46800010U) /**< \brief (PMC) PLL Control Register 1 */
  #define REG_PMC_PLL_CTRL2  (*(__IO uint32_t*)0x46800014U) /**< \brief (PMC) PLL Control Register 2 */
  #define REG_PMC_PLL_SSR    (*(__IO uint32_t*)0x46800018U) /**< \brief (PMC) PLL Spread Spectrum Register */
  #define REG_PMC_PLL_UPDT   (*(__IO uint32_t*)0x46800020U) /**< \brief (PMC) PLL Update Register */
  #define REG_CKGR_MOR       (*(__IO uint32_t*)0x46800024U) /**< \brief (PMC) Main Oscillator Register */
  #define REG_CKGR_MCFR      (*(__IO uint32_t*)0x46800028U) /**< \brief (PMC) Main Clock Frequency Register */
  #define REG_PMC_CPU_CKR    (*(__IO uint32_t*)0x4680002CU) /**< \brief (PMC) CPU Clock Register */
  #define REG_PMC_CPU_RATIO  (*(__IO uint32_t*)0x46800030U) /**< \brief (PMC) CPU Clock Ratio Register */
  #define REG_PMC_PCK        (*(__IO uint32_t*)0x46800044U) /**< \brief (PMC) Programmable Clock Register (chid = 0) */
  #define REG_PMC_IER        (*(__O  uint32_t*)0x46800064U) /**< \brief (PMC) Interrupt Enable Register */
  #define REG_PMC_IDR        (*(__O  uint32_t*)0x46800068U) /**< \brief (PMC) Interrupt Disable Register */
  #define REG_PMC_SR         (*(__I  uint32_t*)0x4680006CU) /**< \brief (PMC) Status Register */
  #define REG_PMC_IMR        (*(__I  uint32_t*)0x46800070U) /**< \brief (PMC) Interrupt Mask Register */
  #define REG_PMC_FSMR       (*(__IO uint32_t*)0x46800074U) /**< \brief (PMC) Fast Start-up Mode Register */
  #define REG_PMC_WCR        (*(__IO uint32_t*)0x46800078U) /**< \brief (PMC) Wake-up Control Register */
  #define REG_PMC_FOCR       (*(__O  uint32_t*)0x4680007CU) /**< \brief (PMC) Fault Output Clear Register */
  #define REG_PMC_CPFSMR     (*(__IO uint32_t*)0x46800080U) /**< \brief (PMC) Coprocessor Fast Start-up Mode Register */
  #define REG_PMC_WPMR       (*(__IO uint32_t*)0x46800084U) /**< \brief (PMC) Write Protection Mode Register */
  #define REG_PMC_WPSR       (*(__I  uint32_t*)0x46800088U) /**< \brief (PMC) Write Protection Status Register */
  #define REG_PMC_PCR        (*(__IO uint32_t*)0x4680008CU) /**< \brief (PMC) Peripheral Control Register */
  #define REG_PMC_OCR        (*(__IO uint32_t*)0x46800090U) /**< \brief (PMC) Oscillator Calibration Register */
  #define REG_PMC_SLPWK_AIPR (*(__I  uint32_t*)0x46800094U) /**< \brief (PMC) SleepWalking Activity In Progress Register */
  #define REG_PMC_SLPWKCR    (*(__IO uint32_t*)0x46800098U) /**< \brief (PMC) SleepWalking Control Register */
  #define REG_PMC_MCKLIM     (*(__IO uint32_t*)0x468000A0U) /**< \brief (PMC) MCK0 Monitor Limits Register */
  #define REG_PMC_CSR0       (*(__I  uint32_t*)0x468000A4U) /**< \brief (PMC) Peripheral Clock Status Register 0 */
  #define REG_PMC_CSR1       (*(__I  uint32_t*)0x468000A8U) /**< \brief (PMC) Peripheral Clock Status Register 1 */
  #define REG_PMC_CSR2       (*(__I  uint32_t*)0x468000ACU) /**< \brief (PMC) Peripheral Clock Status Register 2 */
  #define REG_PMC_CSR3       (*(__I  uint32_t*)0x468000B0U) /**< \brief (PMC) Peripheral Clock Status Register 3 */
  #define REG_PMC_GCSR0      (*(__I  uint32_t*)0x468000C4U) /**< \brief (PMC) Generic Clock Status Register 0 */
  #define REG_PMC_GCSR1      (*(__I  uint32_t*)0x468000C8U) /**< \brief (PMC) Generic Clock Status Register 1 */
  #define REG_PMC_GCSR2      (*(__I  uint32_t*)0x468000CCU) /**< \brief (PMC) Generic Clock Status Register 2 */
  #define REG_PMC_GCSR3      (*(__I  uint32_t*)0x468000D0U) /**< \brief (PMC) Generic Clock Status Register 3 */
  #define REG_PMC_PLL_IER    (*(__O  uint32_t*)0x468000E4U) /**< \brief (PMC) PLL Interrupt Enable Register */
  #define REG_PMC_PLL_IDR    (*(__O  uint32_t*)0x468000E8U) /**< \brief (PMC) PLL Interrupt Disable Register */
  #define REG_PMC_PLL_IMR    (*(__I  uint32_t*)0x468000ECU) /**< \brief (PMC) PLL Interrupt Mask Register */
  #define REG_PMC_PLL_ISR0   (*(__I  uint32_t*)0x468000F0U) /**< \brief (PMC) PLL Interrupt Status Register 0 */
  #define REG_PMC_PLL_ISR1   (*(__I  uint32_t*)0x468000F4U) /**< \brief (PMC) PLL Interrupt Status Register 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_PMC_INSTANCE_ */
