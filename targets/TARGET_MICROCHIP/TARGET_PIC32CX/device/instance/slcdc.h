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

#ifndef _PIC32CX_SLCDC_INSTANCE_
#define _PIC32CX_SLCDC_INSTANCE_

/* ========== Register definition for SLCDC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SLCDC_CR                       (0x40030000U) /**< \brief (SLCDC) Control Register */
  #define REG_SLCDC_MR                       (0x40030004U) /**< \brief (SLCDC) Mode Register */
  #define REG_SLCDC_FRR                      (0x40030008U) /**< \brief (SLCDC) Frame Rate Register */
  #define REG_SLCDC_DR                       (0x4003000CU) /**< \brief (SLCDC) Display Register */
  #define REG_SLCDC_SR                       (0x40030010U) /**< \brief (SLCDC) Status Register */
  #define REG_SLCDC_IER                      (0x40030020U) /**< \brief (SLCDC) Interrupt Enable Register */
  #define REG_SLCDC_IDR                      (0x40030024U) /**< \brief (SLCDC) Interrupt Disable Register */
  #define REG_SLCDC_IMR                      (0x40030028U) /**< \brief (SLCDC) Interrupt Mask Register */
  #define REG_SLCDC_ISR                      (0x4003002CU) /**< \brief (SLCDC) Interrupt Status Register */
  #define REG_SLCDC_SMR0                     (0x40030030U) /**< \brief (SLCDC) Segment Map Register 0 */
  #define REG_SLCDC_WPMR                     (0x400300E4U) /**< \brief (SLCDC) Write Protection Mode Register */
  #define REG_SLCDC_LMEMR0                   (0x40030200U) /**< \brief (SLCDC) LSB Memory Register (com = 0) */
  #define REG_SLCDC_LMEMR1                   (0x40030208U) /**< \brief (SLCDC) LSB Memory Register (com = 1) */
  #define REG_SLCDC_LMEMR2                   (0x40030210U) /**< \brief (SLCDC) LSB Memory Register (com = 2) */
  #define REG_SLCDC_LMEMR3                   (0x40030218U) /**< \brief (SLCDC) LSB Memory Register (com = 3) */
  #define REG_SLCDC_LMEMR4                   (0x40030220U) /**< \brief (SLCDC) LSB Memory Register (com = 4) */
  #define REG_SLCDC_LMEMR5                   (0x40030228U) /**< \brief (SLCDC) LSB Memory Register (com = 5) */
  #define REG_SLCDC_LMEMR6                   (0x40030230U) /**< \brief (SLCDC) LSB Memory Register (com = 6) */
  #define REG_SLCDC_LMEMR7                   (0x40030238U) /**< \brief (SLCDC) LSB Memory Register (com = 7) */
#else
  #define REG_SLCDC_CR      (*(__O  uint32_t*)0x40030000U) /**< \brief (SLCDC) Control Register */
  #define REG_SLCDC_MR      (*(__IO uint32_t*)0x40030004U) /**< \brief (SLCDC) Mode Register */
  #define REG_SLCDC_FRR     (*(__IO uint32_t*)0x40030008U) /**< \brief (SLCDC) Frame Rate Register */
  #define REG_SLCDC_DR      (*(__IO uint32_t*)0x4003000CU) /**< \brief (SLCDC) Display Register */
  #define REG_SLCDC_SR      (*(__I  uint32_t*)0x40030010U) /**< \brief (SLCDC) Status Register */
  #define REG_SLCDC_IER     (*(__O  uint32_t*)0x40030020U) /**< \brief (SLCDC) Interrupt Enable Register */
  #define REG_SLCDC_IDR     (*(__O  uint32_t*)0x40030024U) /**< \brief (SLCDC) Interrupt Disable Register */
  #define REG_SLCDC_IMR     (*(__I  uint32_t*)0x40030028U) /**< \brief (SLCDC) Interrupt Mask Register */
  #define REG_SLCDC_ISR     (*(__I  uint32_t*)0x4003002CU) /**< \brief (SLCDC) Interrupt Status Register */
  #define REG_SLCDC_SMR0    (*(__IO uint32_t*)0x40030030U) /**< \brief (SLCDC) Segment Map Register 0 */
  #define REG_SLCDC_WPMR    (*(__IO uint32_t*)0x400300E4U) /**< \brief (SLCDC) Write Protection Mode Register */
  #define REG_SLCDC_LMEMR0  (*(__IO uint32_t*)0x40030200U) /**< \brief (SLCDC) LSB Memory Register (com = 0) */
  #define REG_SLCDC_LMEMR1  (*(__IO uint32_t*)0x40030208U) /**< \brief (SLCDC) LSB Memory Register (com = 1) */
  #define REG_SLCDC_LMEMR2  (*(__IO uint32_t*)0x40030210U) /**< \brief (SLCDC) LSB Memory Register (com = 2) */
  #define REG_SLCDC_LMEMR3  (*(__IO uint32_t*)0x40030218U) /**< \brief (SLCDC) LSB Memory Register (com = 3) */
  #define REG_SLCDC_LMEMR4  (*(__IO uint32_t*)0x40030220U) /**< \brief (SLCDC) LSB Memory Register (com = 4) */
  #define REG_SLCDC_LMEMR5  (*(__IO uint32_t*)0x40030228U) /**< \brief (SLCDC) LSB Memory Register (com = 5) */
  #define REG_SLCDC_LMEMR6  (*(__IO uint32_t*)0x40030230U) /**< \brief (SLCDC) LSB Memory Register (com = 6) */
  #define REG_SLCDC_LMEMR7  (*(__IO uint32_t*)0x40030238U) /**< \brief (SLCDC) LSB Memory Register (com = 7) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SLCDC_INSTANCE_ */
