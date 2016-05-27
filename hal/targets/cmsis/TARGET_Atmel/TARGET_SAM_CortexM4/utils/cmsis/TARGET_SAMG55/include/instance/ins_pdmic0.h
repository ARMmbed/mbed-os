/**
 * \file
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAMG55_PDMIC0_INSTANCE_
#define _SAMG55_PDMIC0_INSTANCE_

/* ========== Register definition for PDMIC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PDMIC0_CR                     (0x4002C000U) /**< \brief (PDMIC0) Control Register */
#define REG_PDMIC0_MR                     (0x4002C004U) /**< \brief (PDMIC0) Mode Register */
#define REG_PDMIC0_CDR                    (0x4002C014U) /**< \brief (PDMIC0) Converted Data Register */
#define REG_PDMIC0_IER                    (0x4002C018U) /**< \brief (PDMIC0) Interrupt Enable Register */
#define REG_PDMIC0_IDR                    (0x4002C01CU) /**< \brief (PDMIC0) Interrupt Disable Register */
#define REG_PDMIC0_IMR                    (0x4002C020U) /**< \brief (PDMIC0) Interrupt Mask Register */
#define REG_PDMIC0_ISR                    (0x4002C024U) /**< \brief (PDMIC0) Interrupt Status Register */
#define REG_PDMIC0_DSPR0                  (0x4002C058U) /**< \brief (PDMIC0) DSP Configuration Register 0 */
#define REG_PDMIC0_DSPR1                  (0x4002C05CU) /**< \brief (PDMIC0) DSP Configuration Register 1 */
#define REG_PDMIC0_WPMR                   (0x4002C0E4U) /**< \brief (PDMIC0) Write Protection Mode Register */
#define REG_PDMIC0_WPSR                   (0x4002C0E8U) /**< \brief (PDMIC0) Write Protection Status Register */
#define REG_PDMIC0_RPR                    (0x4002C100U) /**< \brief (PDMIC0) Receive Pointer Register */
#define REG_PDMIC0_RCR                    (0x4002C104U) /**< \brief (PDMIC0) Receive Counter Register */
#define REG_PDMIC0_RNPR                   (0x4002C110U) /**< \brief (PDMIC0) Receive Next Pointer Register */
#define REG_PDMIC0_RNCR                   (0x4002C114U) /**< \brief (PDMIC0) Receive Next Counter Register */
#define REG_PDMIC0_PTCR                   (0x4002C120U) /**< \brief (PDMIC0) Transfer Control Register */
#define REG_PDMIC0_PTSR                   (0x4002C124U) /**< \brief (PDMIC0) Transfer Status Register */
#else
#define REG_PDMIC0_CR    (*(__IO uint32_t*)0x4002C000U) /**< \brief (PDMIC0) Control Register */
#define REG_PDMIC0_MR    (*(__IO uint32_t*)0x4002C004U) /**< \brief (PDMIC0) Mode Register */
#define REG_PDMIC0_CDR   (*(__I  uint32_t*)0x4002C014U) /**< \brief (PDMIC0) Converted Data Register */
#define REG_PDMIC0_IER   (*(__O  uint32_t*)0x4002C018U) /**< \brief (PDMIC0) Interrupt Enable Register */
#define REG_PDMIC0_IDR   (*(__O  uint32_t*)0x4002C01CU) /**< \brief (PDMIC0) Interrupt Disable Register */
#define REG_PDMIC0_IMR   (*(__I  uint32_t*)0x4002C020U) /**< \brief (PDMIC0) Interrupt Mask Register */
#define REG_PDMIC0_ISR   (*(__I  uint32_t*)0x4002C024U) /**< \brief (PDMIC0) Interrupt Status Register */
#define REG_PDMIC0_DSPR0 (*(__IO uint32_t*)0x4002C058U) /**< \brief (PDMIC0) DSP Configuration Register 0 */
#define REG_PDMIC0_DSPR1 (*(__IO uint32_t*)0x4002C05CU) /**< \brief (PDMIC0) DSP Configuration Register 1 */
#define REG_PDMIC0_WPMR  (*(__IO uint32_t*)0x4002C0E4U) /**< \brief (PDMIC0) Write Protection Mode Register */
#define REG_PDMIC0_WPSR  (*(__I  uint32_t*)0x4002C0E8U) /**< \brief (PDMIC0) Write Protection Status Register */
#define REG_PDMIC0_RPR   (*(__IO uint32_t*)0x4002C100U) /**< \brief (PDMIC0) Receive Pointer Register */
#define REG_PDMIC0_RCR   (*(__IO uint32_t*)0x4002C104U) /**< \brief (PDMIC0) Receive Counter Register */
#define REG_PDMIC0_RNPR  (*(__IO uint32_t*)0x4002C110U) /**< \brief (PDMIC0) Receive Next Pointer Register */
#define REG_PDMIC0_RNCR  (*(__IO uint32_t*)0x4002C114U) /**< \brief (PDMIC0) Receive Next Counter Register */
#define REG_PDMIC0_PTCR  (*(__O  uint32_t*)0x4002C120U) /**< \brief (PDMIC0) Transfer Control Register */
#define REG_PDMIC0_PTSR  (*(__I  uint32_t*)0x4002C124U) /**< \brief (PDMIC0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_PDMIC0_INSTANCE_ */
