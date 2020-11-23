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

#ifndef _PIC32CX_ADC_INSTANCE_
#define _PIC32CX_ADC_INSTANCE_

/* ========== Register definition for ADC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_ADC_CR                       (0x40024000U) /**< \brief (ADC) Control Register */
  #define REG_ADC_MR                       (0x40024004U) /**< \brief (ADC) Mode Register */
  #define REG_ADC_SEQR1                    (0x40024008U) /**< \brief (ADC) Channel Sequence Register 1 */
  #define REG_ADC_SEQR2                    (0x4002400CU) /**< \brief (ADC) Channel Sequence Register 2 */
  #define REG_ADC_CHER                     (0x40024010U) /**< \brief (ADC) Channel Enable Register */
  #define REG_ADC_CHDR                     (0x40024014U) /**< \brief (ADC) Channel Disable Register */
  #define REG_ADC_CHSR                     (0x40024018U) /**< \brief (ADC) Channel Status Register */
  #define REG_ADC_LCDR                     (0x40024020U) /**< \brief (ADC) Last Converted Data Register */
  #define REG_ADC_IER                      (0x40024024U) /**< \brief (ADC) Interrupt Enable Register */
  #define REG_ADC_IDR                      (0x40024028U) /**< \brief (ADC) Interrupt Disable Register */
  #define REG_ADC_IMR                      (0x4002402CU) /**< \brief (ADC) Interrupt Mask Register */
  #define REG_ADC_ISR                      (0x40024030U) /**< \brief (ADC) Interrupt Status Register */
  #define REG_ADC_EOC_IER                  (0x40024034U) /**< \brief (ADC) End Of Conversion Interrupt Enable Register */
  #define REG_ADC_EOC_IDR                  (0x40024038U) /**< \brief (ADC) End Of Conversion Interrupt Disable Register */
  #define REG_ADC_EOC_IMR                  (0x4002403CU) /**< \brief (ADC) End Of Conversion Interrupt Mask Register */
  #define REG_ADC_EOC_ISR                  (0x40024040U) /**< \brief (ADC) End Of Conversion Interrupt Status Register */
  #define REG_ADC_TEMPMR                   (0x40024044U) /**< \brief (ADC) Temperature Sensor Mode Register */
  #define REG_ADC_TEMPCWR                  (0x40024048U) /**< \brief (ADC) Temperature Compare Window Register */
  #define REG_ADC_OVER                     (0x4002404CU) /**< \brief (ADC) Overrun Status Register */
  #define REG_ADC_EMR                      (0x40024050U) /**< \brief (ADC) Extended Mode Register */
  #define REG_ADC_CWR                      (0x40024054U) /**< \brief (ADC) Compare Window Register */
  #define REG_ADC_CCR                      (0x4002405CU) /**< \brief (ADC) Channel Configuration Register */
  #define REG_ADC_CDR                      (0x40024060U) /**< \brief (ADC) Channel Data Register */
  #define REG_ADC_ACR                      (0x400240E0U) /**< \brief (ADC) Analog Control Register */
  #define REG_ADC_FMR                      (0x400240E4U) /**< \brief (ADC) FIFO Mode Register */
  #define REG_ADC_RPR                      (0x40024100U) /**< \brief (ADC) Receive Pointer Register */
  #define REG_ADC_RCR                      (0x40024104U) /**< \brief (ADC) Receive Counter Register */
  #define REG_ADC_RNPR                     (0x40024110U) /**< \brief (ADC) Receive Next Pointer Register */
  #define REG_ADC_RNCR                     (0x40024114U) /**< \brief (ADC) Receive Next Counter Register */
  #define REG_ADC_PTCR                     (0x40024120U) /**< \brief (ADC) Transfer Control Register */
  #define REG_ADC_PTSR                     (0x40024124U) /**< \brief (ADC) Transfer Status Register */
  #define REG_ADC_PWPMR                    (0x40024128U) /**< \brief (ADC) Write Protection Mode Register */
  #define REG_ADC_TRGR                     (0x40024130U) /**< \brief (ADC) Trigger Register */
  #define REG_ADC_COSR                     (0x40024134U) /**< \brief (ADC) Correction Select Register */
  #define REG_ADC_CVR                      (0x40024138U) /**< \brief (ADC) Correction Values Register */
  #define REG_ADC_CECR                     (0x4002413CU) /**< \brief (ADC) Channel Error Correction Register */
  #define REG_ADC_SR                       (0x40024144U) /**< \brief (ADC) Status Register */
  #define REG_ADC_WPMR                     (0x4002414CU) /**< \brief (ADC) Write Protection Mode Register */
  #define REG_ADC_WPSR                     (0x40024150U) /**< \brief (ADC) Write Protection Status Register */
#else
  #define REG_ADC_CR      (*(__O  uint32_t*)0x40024000U) /**< \brief (ADC) Control Register */
  #define REG_ADC_MR      (*(__IO uint32_t*)0x40024004U) /**< \brief (ADC) Mode Register */
  #define REG_ADC_SEQR1   (*(__IO uint32_t*)0x40024008U) /**< \brief (ADC) Channel Sequence Register 1 */
  #define REG_ADC_SEQR2   (*(__IO uint32_t*)0x4002400CU) /**< \brief (ADC) Channel Sequence Register 2 */
  #define REG_ADC_CHER    (*(__O  uint32_t*)0x40024010U) /**< \brief (ADC) Channel Enable Register */
  #define REG_ADC_CHDR    (*(__O  uint32_t*)0x40024014U) /**< \brief (ADC) Channel Disable Register */
  #define REG_ADC_CHSR    (*(__I  uint32_t*)0x40024018U) /**< \brief (ADC) Channel Status Register */
  #define REG_ADC_LCDR    (*(__I  uint32_t*)0x40024020U) /**< \brief (ADC) Last Converted Data Register */
  #define REG_ADC_IER     (*(__O  uint32_t*)0x40024024U) /**< \brief (ADC) Interrupt Enable Register */
  #define REG_ADC_IDR     (*(__O  uint32_t*)0x40024028U) /**< \brief (ADC) Interrupt Disable Register */
  #define REG_ADC_IMR     (*(__I  uint32_t*)0x4002402CU) /**< \brief (ADC) Interrupt Mask Register */
  #define REG_ADC_ISR     (*(__I  uint32_t*)0x40024030U) /**< \brief (ADC) Interrupt Status Register */
  #define REG_ADC_EOC_IER (*(__O  uint32_t*)0x40024034U) /**< \brief (ADC) End Of Conversion Interrupt Enable Register */
  #define REG_ADC_EOC_IDR (*(__O  uint32_t*)0x40024038U) /**< \brief (ADC) End Of Conversion Interrupt Disable Register */
  #define REG_ADC_EOC_IMR (*(__I  uint32_t*)0x4002403CU) /**< \brief (ADC) End Of Conversion Interrupt Mask Register */
  #define REG_ADC_EOC_ISR (*(__I  uint32_t*)0x40024040U) /**< \brief (ADC) End Of Conversion Interrupt Status Register */
  #define REG_ADC_TEMPMR  (*(__IO uint32_t*)0x40024044U) /**< \brief (ADC) Temperature Sensor Mode Register */
  #define REG_ADC_TEMPCWR (*(__IO uint32_t*)0x40024048U) /**< \brief (ADC) Temperature Compare Window Register */
  #define REG_ADC_OVER    (*(__I  uint32_t*)0x4002404CU) /**< \brief (ADC) Overrun Status Register */
  #define REG_ADC_EMR     (*(__IO uint32_t*)0x40024050U) /**< \brief (ADC) Extended Mode Register */
  #define REG_ADC_CWR     (*(__IO uint32_t*)0x40024054U) /**< \brief (ADC) Compare Window Register */
  #define REG_ADC_CCR     (*(__IO uint32_t*)0x4002405CU) /**< \brief (ADC) Channel Configuration Register */
  #define REG_ADC_CDR     (*(__I  uint32_t*)0x40024060U) /**< \brief (ADC) Channel Data Register */
  #define REG_ADC_ACR     (*(__IO uint32_t*)0x400240E0U) /**< \brief (ADC) Analog Control Register */
  #define REG_ADC_FMR     (*(__IO uint32_t*)0x400240E4U) /**< \brief (ADC) FIFO Mode Register */
  #define REG_ADC_RPR     (*(__IO uint32_t*)0x40024100U) /**< \brief (ADC) Receive Pointer Register */
  #define REG_ADC_RCR     (*(__IO uint32_t*)0x40024104U) /**< \brief (ADC) Receive Counter Register */
  #define REG_ADC_RNPR    (*(__IO uint32_t*)0x40024110U) /**< \brief (ADC) Receive Next Pointer Register */
  #define REG_ADC_RNCR    (*(__IO uint32_t*)0x40024114U) /**< \brief (ADC) Receive Next Counter Register */
  #define REG_ADC_PTCR    (*(__O  uint32_t*)0x40024120U) /**< \brief (ADC) Transfer Control Register */
  #define REG_ADC_PTSR    (*(__I  uint32_t*)0x40024124U) /**< \brief (ADC) Transfer Status Register */
  #define REG_ADC_PWPMR   (*(__IO uint32_t*)0x40024128U) /**< \brief (ADC) Write Protection Mode Register */
  #define REG_ADC_TRGR    (*(__IO uint32_t*)0x40024130U) /**< \brief (ADC) Trigger Register */
  #define REG_ADC_COSR    (*(__IO uint32_t*)0x40024134U) /**< \brief (ADC) Correction Select Register */
  #define REG_ADC_CVR     (*(__IO uint32_t*)0x40024138U) /**< \brief (ADC) Correction Values Register */
  #define REG_ADC_CECR    (*(__IO uint32_t*)0x4002413CU) /**< \brief (ADC) Channel Error Correction Register */
  #define REG_ADC_SR      (*(__I  uint32_t*)0x40024144U) /**< \brief (ADC) Status Register */
  #define REG_ADC_WPMR    (*(__IO uint32_t*)0x4002414CU) /**< \brief (ADC) Write Protection Mode Register */
  #define REG_ADC_WPSR    (*(__I  uint32_t*)0x40024150U) /**< \brief (ADC) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_ADC_INSTANCE_ */
