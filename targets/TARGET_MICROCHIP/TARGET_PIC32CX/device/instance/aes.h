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

#ifndef _PIC32CX_AES_INSTANCE_
#define _PIC32CX_AES_INSTANCE_

/* ========== Register definition for AES peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_AES_CR                      (0x44000000U) /**< \brief (AES) Control Register */
  #define REG_AES_MR                      (0x44000004U) /**< \brief (AES) Mode Register */
  #define REG_AES_IER                     (0x44000010U) /**< \brief (AES) Interrupt Enable Register */
  #define REG_AES_IDR                     (0x44000014U) /**< \brief (AES) Interrupt Disable Register */
  #define REG_AES_IMR                     (0x44000018U) /**< \brief (AES) Interrupt Mask Register */
  #define REG_AES_ISR                     (0x4400001CU) /**< \brief (AES) Interrupt Status Register */
  #define REG_AES_KEYWR                   (0x44000020U) /**< \brief (AES) Key Word Register */
  #define REG_AES_IDATAR                  (0x44000040U) /**< \brief (AES) Input Data Register */
  #define REG_AES_ODATAR                  (0x44000050U) /**< \brief (AES) Output Data Register */
  #define REG_AES_IVR                     (0x44000060U) /**< \brief (AES) Initialization Vector Register */
  #define REG_AES_EMR                     (0x440000B0U) /**< \brief (AES) Extended Mode Register */
  #define REG_AES_WPMR                    (0x440000E4U) /**< \brief (AES) Write Protection Mode Register */
  #define REG_AES_WPSR                    (0x440000E8U) /**< \brief (AES) Write Protection Status Register */
  #define REG_AES_RPR                     (0x44000100U) /**< \brief (AES) Receive Pointer Register */
  #define REG_AES_RCR                     (0x44000104U) /**< \brief (AES) Receive Counter Register */
  #define REG_AES_TPR                     (0x44000108U) /**< \brief (AES) Transmit Pointer Register */
  #define REG_AES_TCR                     (0x4400010CU) /**< \brief (AES) Transmit Counter Register */
  #define REG_AES_RNPR                    (0x44000110U) /**< \brief (AES) Receive Next Pointer Register */
  #define REG_AES_RNCR                    (0x44000114U) /**< \brief (AES) Receive Next Counter Register */
  #define REG_AES_TNPR                    (0x44000118U) /**< \brief (AES) Transmit Next Pointer Register */
  #define REG_AES_TNCR                    (0x4400011CU) /**< \brief (AES) Transmit Next Counter Register */
  #define REG_AES_PTCR                    (0x44000120U) /**< \brief (AES) Transfer Control Register */
  #define REG_AES_PTSR                    (0x44000124U) /**< \brief (AES) Transfer Status Register */
  #define REG_AES_PWPMR                   (0x44000128U) /**< \brief (AES) Write Protection Mode Register */
#else
  #define REG_AES_CR     (*(__O  uint32_t*)0x44000000U) /**< \brief (AES) Control Register */
  #define REG_AES_MR     (*(__IO uint32_t*)0x44000004U) /**< \brief (AES) Mode Register */
  #define REG_AES_IER    (*(__O  uint32_t*)0x44000010U) /**< \brief (AES) Interrupt Enable Register */
  #define REG_AES_IDR    (*(__O  uint32_t*)0x44000014U) /**< \brief (AES) Interrupt Disable Register */
  #define REG_AES_IMR    (*(__I  uint32_t*)0x44000018U) /**< \brief (AES) Interrupt Mask Register */
  #define REG_AES_ISR    (*(__I  uint32_t*)0x4400001CU) /**< \brief (AES) Interrupt Status Register */
  #define REG_AES_KEYWR  (*(__O  uint32_t*)0x44000020U) /**< \brief (AES) Key Word Register */
  #define REG_AES_IDATAR (*(__O  uint32_t*)0x44000040U) /**< \brief (AES) Input Data Register */
  #define REG_AES_ODATAR (*(__I  uint32_t*)0x44000050U) /**< \brief (AES) Output Data Register */
  #define REG_AES_IVR    (*(__O  uint32_t*)0x44000060U) /**< \brief (AES) Initialization Vector Register */
  #define REG_AES_EMR    (*(__IO uint32_t*)0x440000B0U) /**< \brief (AES) Extended Mode Register */
  #define REG_AES_WPMR   (*(__IO uint32_t*)0x440000E4U) /**< \brief (AES) Write Protection Mode Register */
  #define REG_AES_WPSR   (*(__I  uint32_t*)0x440000E8U) /**< \brief (AES) Write Protection Status Register */
  #define REG_AES_RPR    (*(__IO uint32_t*)0x44000100U) /**< \brief (AES) Receive Pointer Register */
  #define REG_AES_RCR    (*(__IO uint32_t*)0x44000104U) /**< \brief (AES) Receive Counter Register */
  #define REG_AES_TPR    (*(__IO uint32_t*)0x44000108U) /**< \brief (AES) Transmit Pointer Register */
  #define REG_AES_TCR    (*(__IO uint32_t*)0x4400010CU) /**< \brief (AES) Transmit Counter Register */
  #define REG_AES_RNPR   (*(__IO uint32_t*)0x44000110U) /**< \brief (AES) Receive Next Pointer Register */
  #define REG_AES_RNCR   (*(__IO uint32_t*)0x44000114U) /**< \brief (AES) Receive Next Counter Register */
  #define REG_AES_TNPR   (*(__IO uint32_t*)0x44000118U) /**< \brief (AES) Transmit Next Pointer Register */
  #define REG_AES_TNCR   (*(__IO uint32_t*)0x4400011CU) /**< \brief (AES) Transmit Next Counter Register */
  #define REG_AES_PTCR   (*(__O  uint32_t*)0x44000120U) /**< \brief (AES) Transfer Control Register */
  #define REG_AES_PTSR   (*(__I  uint32_t*)0x44000124U) /**< \brief (AES) Transfer Status Register */
  #define REG_AES_PWPMR  (*(__IO uint32_t*)0x44000128U) /**< \brief (AES) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_AES_INSTANCE_ */
