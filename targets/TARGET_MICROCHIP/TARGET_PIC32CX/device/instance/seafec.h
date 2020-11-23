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

#ifndef _PIC32CX_SEAFEC_INSTANCE_
#define _PIC32CX_SEAFEC_INSTANCE_

/* ========== Register definition for SEAFEC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SEAFEC_CR                        (0x48000000U) /**< \brief (SEAFEC) Control Register */
  #define REG_SEAFEC_MR                        (0x48000004U) /**< \brief (SEAFEC) Mode Register */
  #define REG_SEAFEC_CDCR                      (0x48000008U) /**< \brief (SEAFEC) Channels Difference Comparison Register */
  #define REG_SEAFEC_EMR                       (0x4800000CU) /**< \brief (SEAFEC) Extended Mode Register */
  #define REG_SEAFEC_DMA_DATA                  (0x48000010U) /**< \brief (SEAFEC) DMA Data Register */
  #define REG_SEAFEC_DATA                      (0x48000014U) /**< \brief (SEAFEC) Data Channel 0 Register */
  #define REG_SEAFEC_CHA_CFG1                  (0x48000040U) /**< \brief (SEAFEC) Channel A Comparison Configuration 1 */
  #define REG_SEAFEC_CHB_CFG1                  (0x48000044U) /**< \brief (SEAFEC) Channel B Comparison Configuration 1 */
  #define REG_SEAFEC_CHA_CFG2                  (0x48000048U) /**< \brief (SEAFEC) Channel A Comparison Configuration 2 */
  #define REG_SEAFEC_CHB_CFG2                  (0x4800004CU) /**< \brief (SEAFEC) Channel B Comparison Configuration 2 */
  #define REG_SEAFEC_IER                       (0x48000050U) /**< \brief (SEAFEC) Interrupt Enable Register */
  #define REG_SEAFEC_IDR                       (0x48000054U) /**< \brief (SEAFEC) Interrupt Disable Register */
  #define REG_SEAFEC_IMR                       (0x48000058U) /**< \brief (SEAFEC) Interrupt Mask Register */
  #define REG_SEAFEC_ISR                       (0x4800005CU) /**< \brief (SEAFEC) Interrupt Status Register */
  #define REG_SEAFEC_SR                        (0x48000060U) /**< \brief (SEAFEC) Status Register */
  #define REG_SEAFEC_WPMR                      (0x480000E4U) /**< \brief (SEAFEC) Write Protection Mode Register */
  #define REG_SEAFEC_WPSR                      (0x480000E8U) /**< \brief (SEAFEC) Write Protection Status Register */
  #define REG_SEAFEC_VERSION                   (0x480000FCU) /**< \brief (SEAFEC) Version Register */
  #define REG_SEAFEC_RPR                       (0x48000100U) /**< \brief (SEAFEC) Receive Pointer Register */
  #define REG_SEAFEC_RCR                       (0x48000104U) /**< \brief (SEAFEC) Receive Counter Register */
  #define REG_SEAFEC_RNPR                      (0x48000110U) /**< \brief (SEAFEC) Receive Next Pointer Register */
  #define REG_SEAFEC_RNCR                      (0x48000114U) /**< \brief (SEAFEC) Receive Next Counter Register */
  #define REG_SEAFEC_PTCR                      (0x48000120U) /**< \brief (SEAFEC) Transfer Control Register */
  #define REG_SEAFEC_PTSR                      (0x48000124U) /**< \brief (SEAFEC) Transfer Status Register */
  #define REG_SEAFEC_PWPMR                     (0x48000128U) /**< \brief (SEAFEC) Write Protection Mode Register */
#else
  #define REG_SEAFEC_CR       (*(__O  uint32_t*)0x48000000U) /**< \brief (SEAFEC) Control Register */
  #define REG_SEAFEC_MR       (*(__IO uint32_t*)0x48000004U) /**< \brief (SEAFEC) Mode Register */
  #define REG_SEAFEC_CDCR     (*(__IO uint32_t*)0x48000008U) /**< \brief (SEAFEC) Channels Difference Comparison Register */
  #define REG_SEAFEC_EMR      (*(__IO uint32_t*)0x4800000CU) /**< \brief (SEAFEC) Extended Mode Register */
  #define REG_SEAFEC_DMA_DATA (*(__I  uint32_t*)0x48000010U) /**< \brief (SEAFEC) DMA Data Register */
  #define REG_SEAFEC_DATA     (*(__I  uint32_t*)0x48000014U) /**< \brief (SEAFEC) Data Channel 0 Register */
  #define REG_SEAFEC_CHA_CFG1 (*(__IO uint32_t*)0x48000040U) /**< \brief (SEAFEC) Channel A Comparison Configuration 1 */
  #define REG_SEAFEC_CHB_CFG1 (*(__IO uint32_t*)0x48000044U) /**< \brief (SEAFEC) Channel B Comparison Configuration 1 */
  #define REG_SEAFEC_CHA_CFG2 (*(__IO uint32_t*)0x48000048U) /**< \brief (SEAFEC) Channel A Comparison Configuration 2 */
  #define REG_SEAFEC_CHB_CFG2 (*(__IO uint32_t*)0x4800004CU) /**< \brief (SEAFEC) Channel B Comparison Configuration 2 */
  #define REG_SEAFEC_IER      (*(__O  uint32_t*)0x48000050U) /**< \brief (SEAFEC) Interrupt Enable Register */
  #define REG_SEAFEC_IDR      (*(__O  uint32_t*)0x48000054U) /**< \brief (SEAFEC) Interrupt Disable Register */
  #define REG_SEAFEC_IMR      (*(__I  uint32_t*)0x48000058U) /**< \brief (SEAFEC) Interrupt Mask Register */
  #define REG_SEAFEC_ISR      (*(__I  uint32_t*)0x4800005CU) /**< \brief (SEAFEC) Interrupt Status Register */
  #define REG_SEAFEC_SR       (*(__I  uint32_t*)0x48000060U) /**< \brief (SEAFEC) Status Register */
  #define REG_SEAFEC_WPMR     (*(__IO uint32_t*)0x480000E4U) /**< \brief (SEAFEC) Write Protection Mode Register */
  #define REG_SEAFEC_WPSR     (*(__I  uint32_t*)0x480000E8U) /**< \brief (SEAFEC) Write Protection Status Register */
  #define REG_SEAFEC_VERSION  (*(__I  uint32_t*)0x480000FCU) /**< \brief (SEAFEC) Version Register */
  #define REG_SEAFEC_RPR      (*(__IO uint32_t*)0x48000100U) /**< \brief (SEAFEC) Receive Pointer Register */
  #define REG_SEAFEC_RCR      (*(__IO uint32_t*)0x48000104U) /**< \brief (SEAFEC) Receive Counter Register */
  #define REG_SEAFEC_RNPR     (*(__IO uint32_t*)0x48000110U) /**< \brief (SEAFEC) Receive Next Pointer Register */
  #define REG_SEAFEC_RNCR     (*(__IO uint32_t*)0x48000114U) /**< \brief (SEAFEC) Receive Next Counter Register */
  #define REG_SEAFEC_PTCR     (*(__O  uint32_t*)0x48000120U) /**< \brief (SEAFEC) Transfer Control Register */
  #define REG_SEAFEC_PTSR     (*(__I  uint32_t*)0x48000124U) /**< \brief (SEAFEC) Transfer Status Register */
  #define REG_SEAFEC_PWPMR    (*(__IO uint32_t*)0x48000128U) /**< \brief (SEAFEC) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SEAFEC_INSTANCE_ */
