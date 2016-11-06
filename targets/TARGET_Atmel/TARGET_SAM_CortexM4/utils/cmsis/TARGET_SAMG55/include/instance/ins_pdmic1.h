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

#ifndef _SAMG55_PDMIC1_INSTANCE_
#define _SAMG55_PDMIC1_INSTANCE_

/* ========== Register definition for PDMIC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PDMIC1_CR                     (0x40030000U) /**< \brief (PDMIC1) Control Register */
#define REG_PDMIC1_MR                     (0x40030004U) /**< \brief (PDMIC1) Mode Register */
#define REG_PDMIC1_CDR                    (0x40030014U) /**< \brief (PDMIC1) Converted Data Register */
#define REG_PDMIC1_IER                    (0x40030018U) /**< \brief (PDMIC1) Interrupt Enable Register */
#define REG_PDMIC1_IDR                    (0x4003001CU) /**< \brief (PDMIC1) Interrupt Disable Register */
#define REG_PDMIC1_IMR                    (0x40030020U) /**< \brief (PDMIC1) Interrupt Mask Register */
#define REG_PDMIC1_ISR                    (0x40030024U) /**< \brief (PDMIC1) Interrupt Status Register */
#define REG_PDMIC1_DSPR0                  (0x40030058U) /**< \brief (PDMIC1) DSP Configuration Register 0 */
#define REG_PDMIC1_DSPR1                  (0x4003005CU) /**< \brief (PDMIC1) DSP Configuration Register 1 */
#define REG_PDMIC1_WPMR                   (0x400300E4U) /**< \brief (PDMIC1) Write Protection Mode Register */
#define REG_PDMIC1_WPSR                   (0x400300E8U) /**< \brief (PDMIC1) Write Protection Status Register */
#define REG_PDMIC1_RPR                    (0x40030100U) /**< \brief (PDMIC1) Receive Pointer Register */
#define REG_PDMIC1_RCR                    (0x40030104U) /**< \brief (PDMIC1) Receive Counter Register */
#define REG_PDMIC1_RNPR                   (0x40030110U) /**< \brief (PDMIC1) Receive Next Pointer Register */
#define REG_PDMIC1_RNCR                   (0x40030114U) /**< \brief (PDMIC1) Receive Next Counter Register */
#define REG_PDMIC1_PTCR                   (0x40030120U) /**< \brief (PDMIC1) Transfer Control Register */
#define REG_PDMIC1_PTSR                   (0x40030124U) /**< \brief (PDMIC1) Transfer Status Register */
#else
#define REG_PDMIC1_CR    (*(__IO uint32_t*)0x40030000U) /**< \brief (PDMIC1) Control Register */
#define REG_PDMIC1_MR    (*(__IO uint32_t*)0x40030004U) /**< \brief (PDMIC1) Mode Register */
#define REG_PDMIC1_CDR   (*(__I  uint32_t*)0x40030014U) /**< \brief (PDMIC1) Converted Data Register */
#define REG_PDMIC1_IER   (*(__O  uint32_t*)0x40030018U) /**< \brief (PDMIC1) Interrupt Enable Register */
#define REG_PDMIC1_IDR   (*(__O  uint32_t*)0x4003001CU) /**< \brief (PDMIC1) Interrupt Disable Register */
#define REG_PDMIC1_IMR   (*(__I  uint32_t*)0x40030020U) /**< \brief (PDMIC1) Interrupt Mask Register */
#define REG_PDMIC1_ISR   (*(__I  uint32_t*)0x40030024U) /**< \brief (PDMIC1) Interrupt Status Register */
#define REG_PDMIC1_DSPR0 (*(__IO uint32_t*)0x40030058U) /**< \brief (PDMIC1) DSP Configuration Register 0 */
#define REG_PDMIC1_DSPR1 (*(__IO uint32_t*)0x4003005CU) /**< \brief (PDMIC1) DSP Configuration Register 1 */
#define REG_PDMIC1_WPMR  (*(__IO uint32_t*)0x400300E4U) /**< \brief (PDMIC1) Write Protection Mode Register */
#define REG_PDMIC1_WPSR  (*(__I  uint32_t*)0x400300E8U) /**< \brief (PDMIC1) Write Protection Status Register */
#define REG_PDMIC1_RPR   (*(__IO uint32_t*)0x40030100U) /**< \brief (PDMIC1) Receive Pointer Register */
#define REG_PDMIC1_RCR   (*(__IO uint32_t*)0x40030104U) /**< \brief (PDMIC1) Receive Counter Register */
#define REG_PDMIC1_RNPR  (*(__IO uint32_t*)0x40030110U) /**< \brief (PDMIC1) Receive Next Pointer Register */
#define REG_PDMIC1_RNCR  (*(__IO uint32_t*)0x40030114U) /**< \brief (PDMIC1) Receive Next Counter Register */
#define REG_PDMIC1_PTCR  (*(__O  uint32_t*)0x40030120U) /**< \brief (PDMIC1) Transfer Control Register */
#define REG_PDMIC1_PTSR  (*(__I  uint32_t*)0x40030124U) /**< \brief (PDMIC1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_PDMIC1_INSTANCE_ */
