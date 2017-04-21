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

#ifndef _SAMG55_MEM2MEM_INSTANCE_
#define _SAMG55_MEM2MEM_INSTANCE_

/* ========== Register definition for MEM2MEM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_MEM2MEM_THR                   (0x40028000U) /**< \brief (MEM2MEM) Memory to Memory Transfer Holding Register */
#define REG_MEM2MEM_MR                    (0x40028004U) /**< \brief (MEM2MEM) Memory to Memory Mode Register */
#define REG_MEM2MEM_IER                   (0x40028008U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Enable Register */
#define REG_MEM2MEM_IDR                   (0x4002800CU) /**< \brief (MEM2MEM) Memory to Memory Interrupt Disable Register */
#define REG_MEM2MEM_IMR                   (0x40028010U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Mask Register */
#define REG_MEM2MEM_ISR                   (0x40028014U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Status Register */
#define REG_MEM2MEM_RPR                   (0x40028100U) /**< \brief (MEM2MEM) Receive Pointer Register */
#define REG_MEM2MEM_RCR                   (0x40028104U) /**< \brief (MEM2MEM) Receive Counter Register */
#define REG_MEM2MEM_TPR                   (0x40028108U) /**< \brief (MEM2MEM) Transmit Pointer Register */
#define REG_MEM2MEM_TCR                   (0x4002810CU) /**< \brief (MEM2MEM) Transmit Counter Register */
#define REG_MEM2MEM_RNPR                  (0x40028110U) /**< \brief (MEM2MEM) Receive Next Pointer Register */
#define REG_MEM2MEM_RNCR                  (0x40028114U) /**< \brief (MEM2MEM) Receive Next Counter Register */
#define REG_MEM2MEM_TNPR                  (0x40028118U) /**< \brief (MEM2MEM) Transmit Next Pointer Register */
#define REG_MEM2MEM_TNCR                  (0x4002811CU) /**< \brief (MEM2MEM) Transmit Next Counter Register */
#define REG_MEM2MEM_PTCR                  (0x40028120U) /**< \brief (MEM2MEM) Transfer Control Register */
#define REG_MEM2MEM_PTSR                  (0x40028124U) /**< \brief (MEM2MEM) Transfer Status Register */
#else
#define REG_MEM2MEM_THR  (*(__IO uint32_t*)0x40028000U) /**< \brief (MEM2MEM) Memory to Memory Transfer Holding Register */
#define REG_MEM2MEM_MR   (*(__IO uint32_t*)0x40028004U) /**< \brief (MEM2MEM) Memory to Memory Mode Register */
#define REG_MEM2MEM_IER  (*(__O  uint32_t*)0x40028008U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Enable Register */
#define REG_MEM2MEM_IDR  (*(__O  uint32_t*)0x4002800CU) /**< \brief (MEM2MEM) Memory to Memory Interrupt Disable Register */
#define REG_MEM2MEM_IMR  (*(__I  uint32_t*)0x40028010U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Mask Register */
#define REG_MEM2MEM_ISR  (*(__I  uint32_t*)0x40028014U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Status Register */
#define REG_MEM2MEM_RPR  (*(__IO uint32_t*)0x40028100U) /**< \brief (MEM2MEM) Receive Pointer Register */
#define REG_MEM2MEM_RCR  (*(__IO uint32_t*)0x40028104U) /**< \brief (MEM2MEM) Receive Counter Register */
#define REG_MEM2MEM_TPR  (*(__IO uint32_t*)0x40028108U) /**< \brief (MEM2MEM) Transmit Pointer Register */
#define REG_MEM2MEM_TCR  (*(__IO uint32_t*)0x4002810CU) /**< \brief (MEM2MEM) Transmit Counter Register */
#define REG_MEM2MEM_RNPR (*(__IO uint32_t*)0x40028110U) /**< \brief (MEM2MEM) Receive Next Pointer Register */
#define REG_MEM2MEM_RNCR (*(__IO uint32_t*)0x40028114U) /**< \brief (MEM2MEM) Receive Next Counter Register */
#define REG_MEM2MEM_TNPR (*(__IO uint32_t*)0x40028118U) /**< \brief (MEM2MEM) Transmit Next Pointer Register */
#define REG_MEM2MEM_TNCR (*(__IO uint32_t*)0x4002811CU) /**< \brief (MEM2MEM) Transmit Next Counter Register */
#define REG_MEM2MEM_PTCR (*(__O  uint32_t*)0x40028120U) /**< \brief (MEM2MEM) Transfer Control Register */
#define REG_MEM2MEM_PTSR (*(__I  uint32_t*)0x40028124U) /**< \brief (MEM2MEM) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_MEM2MEM_INSTANCE_ */
