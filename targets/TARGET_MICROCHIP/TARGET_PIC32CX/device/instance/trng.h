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

#ifndef _PIC32CX_TRNG_INSTANCE_
#define _PIC32CX_TRNG_INSTANCE_

/* ========== Register definition for TRNG peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TRNG_CR                     (0x4400C000U) /**< \brief (TRNG) Control Register */
  #define REG_TRNG_MR                     (0x4400C004U) /**< \brief (TRNG) Mode Register */
  #define REG_TRNG_PKBCR                  (0x4400C008U) /**< \brief (TRNG) Private Key Bus Control Register */
  #define REG_TRNG_IER                    (0x4400C010U) /**< \brief (TRNG) Interrupt Enable Register */
  #define REG_TRNG_IDR                    (0x4400C014U) /**< \brief (TRNG) Interrupt Disable Register */
  #define REG_TRNG_IMR                    (0x4400C018U) /**< \brief (TRNG) Interrupt Mask Register */
  #define REG_TRNG_ISR                    (0x4400C01CU) /**< \brief (TRNG) Interrupt Status Register */
  #define REG_TRNG_ODATA                  (0x4400C050U) /**< \brief (TRNG) Output Data Register */
  #define REG_TRNG_WPMR                   (0x4400C0E4U) /**< \brief (TRNG) Write Protection Mode Register */
  #define REG_TRNG_WPSR                   (0x4400C0E8U) /**< \brief (TRNG) Write Protection Status Register */
#else
  #define REG_TRNG_CR    (*(__O  uint32_t*)0x4400C000U) /**< \brief (TRNG) Control Register */
  #define REG_TRNG_MR    (*(__IO uint32_t*)0x4400C004U) /**< \brief (TRNG) Mode Register */
  #define REG_TRNG_PKBCR (*(__O  uint32_t*)0x4400C008U) /**< \brief (TRNG) Private Key Bus Control Register */
  #define REG_TRNG_IER   (*(__O  uint32_t*)0x4400C010U) /**< \brief (TRNG) Interrupt Enable Register */
  #define REG_TRNG_IDR   (*(__O  uint32_t*)0x4400C014U) /**< \brief (TRNG) Interrupt Disable Register */
  #define REG_TRNG_IMR   (*(__I  uint32_t*)0x4400C018U) /**< \brief (TRNG) Interrupt Mask Register */
  #define REG_TRNG_ISR   (*(__I  uint32_t*)0x4400C01CU) /**< \brief (TRNG) Interrupt Status Register */
  #define REG_TRNG_ODATA (*(__I  uint32_t*)0x4400C050U) /**< \brief (TRNG) Output Data Register */
  #define REG_TRNG_WPMR  (*(__IO uint32_t*)0x4400C0E4U) /**< \brief (TRNG) Write Protection Mode Register */
  #define REG_TRNG_WPSR  (*(__I  uint32_t*)0x4400C0E8U) /**< \brief (TRNG) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TRNG_INSTANCE_ */
