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

#ifndef _PIC32CX_SHA_INSTANCE_
#define _PIC32CX_SHA_INSTANCE_

/* ========== Register definition for SHA peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SHA_CR                       (0x44008000U) /**< \brief (SHA) Control Register */
  #define REG_SHA_MR                       (0x44008004U) /**< \brief (SHA) Mode Register */
  #define REG_SHA_IER                      (0x44008010U) /**< \brief (SHA) Interrupt Enable Register */
  #define REG_SHA_IDR                      (0x44008014U) /**< \brief (SHA) Interrupt Disable Register */
  #define REG_SHA_IMR                      (0x44008018U) /**< \brief (SHA) Interrupt Mask Register */
  #define REG_SHA_ISR                      (0x4400801CU) /**< \brief (SHA) Interrupt Status Register */
  #define REG_SHA_MSR                      (0x44008020U) /**< \brief (SHA) Message Size Register */
  #define REG_SHA_BCR                      (0x44008030U) /**< \brief (SHA) Bytes Count Register */
  #define REG_SHA_IDATAR                   (0x44008040U) /**< \brief (SHA) Input Data 0 Register */
  #define REG_SHA_IODATAR                  (0x44008080U) /**< \brief (SHA) Input/Output Data 0 Register */
  #define REG_SHA_WPMR                     (0x440080E4U) /**< \brief (SHA) Write Protection Mode Register */
  #define REG_SHA_WPSR                     (0x440080E8U) /**< \brief (SHA) Write Protection Status Register */
  #define REG_SHA_TPR                      (0x44008108U) /**< \brief (SHA) Transmit Pointer Register */
  #define REG_SHA_TCR                      (0x4400810CU) /**< \brief (SHA) Transmit Counter Register */
  #define REG_SHA_TNPR                     (0x44008118U) /**< \brief (SHA) Transmit Next Pointer Register */
  #define REG_SHA_TNCR                     (0x4400811CU) /**< \brief (SHA) Transmit Next Counter Register */
  #define REG_SHA_PTCR                     (0x44008120U) /**< \brief (SHA) Transfer Control Register */
  #define REG_SHA_PTSR                     (0x44008124U) /**< \brief (SHA) Transfer Status Register */
  #define REG_SHA_PWPMR                    (0x44008128U) /**< \brief (SHA) Write Protection Mode Register */
#else
  #define REG_SHA_CR      (*(__O  uint32_t*)0x44008000U) /**< \brief (SHA) Control Register */
  #define REG_SHA_MR      (*(__IO uint32_t*)0x44008004U) /**< \brief (SHA) Mode Register */
  #define REG_SHA_IER     (*(__O  uint32_t*)0x44008010U) /**< \brief (SHA) Interrupt Enable Register */
  #define REG_SHA_IDR     (*(__O  uint32_t*)0x44008014U) /**< \brief (SHA) Interrupt Disable Register */
  #define REG_SHA_IMR     (*(__I  uint32_t*)0x44008018U) /**< \brief (SHA) Interrupt Mask Register */
  #define REG_SHA_ISR     (*(__I  uint32_t*)0x4400801CU) /**< \brief (SHA) Interrupt Status Register */
  #define REG_SHA_MSR     (*(__IO uint32_t*)0x44008020U) /**< \brief (SHA) Message Size Register */
  #define REG_SHA_BCR     (*(__IO uint32_t*)0x44008030U) /**< \brief (SHA) Bytes Count Register */
  #define REG_SHA_IDATAR  (*(__O  uint32_t*)0x44008040U) /**< \brief (SHA) Input Data 0 Register */
  #define REG_SHA_IODATAR (*(__IO uint32_t*)0x44008080U) /**< \brief (SHA) Input/Output Data 0 Register */
  #define REG_SHA_WPMR    (*(__IO uint32_t*)0x440080E4U) /**< \brief (SHA) Write Protection Mode Register */
  #define REG_SHA_WPSR    (*(__I  uint32_t*)0x440080E8U) /**< \brief (SHA) Write Protection Status Register */
  #define REG_SHA_TPR     (*(__IO uint32_t*)0x44008108U) /**< \brief (SHA) Transmit Pointer Register */
  #define REG_SHA_TCR     (*(__IO uint32_t*)0x4400810CU) /**< \brief (SHA) Transmit Counter Register */
  #define REG_SHA_TNPR    (*(__IO uint32_t*)0x44008118U) /**< \brief (SHA) Transmit Next Pointer Register */
  #define REG_SHA_TNCR    (*(__IO uint32_t*)0x4400811CU) /**< \brief (SHA) Transmit Next Counter Register */
  #define REG_SHA_PTCR    (*(__O  uint32_t*)0x44008120U) /**< \brief (SHA) Transfer Control Register */
  #define REG_SHA_PTSR    (*(__I  uint32_t*)0x44008124U) /**< \brief (SHA) Transfer Status Register */
  #define REG_SHA_PWPMR   (*(__IO uint32_t*)0x44008128U) /**< \brief (SHA) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SHA_INSTANCE_ */
