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

#ifndef _SAMG55_SPI3_INSTANCE_
#define _SAMG55_SPI3_INSTANCE_

/* ========== Register definition for SPI3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI3_CR                    (0x40018400U) /**< \brief (SPI3) SPI Control Register */
#define REG_SPI3_MR                    (0x40018404U) /**< \brief (SPI3) SPI Mode Register */
#define REG_SPI3_RDR                   (0x40018408U) /**< \brief (SPI3) SPI Receive Data Register */
#define REG_SPI3_TDR                   (0x4001840CU) /**< \brief (SPI3) SPI Transmit Data Register */
#define REG_SPI3_SR                    (0x40018410U) /**< \brief (SPI3) SPI Status Register */
#define REG_SPI3_IER                   (0x40018414U) /**< \brief (SPI3) SPI Interrupt Enable Register */
#define REG_SPI3_IDR                   (0x40018418U) /**< \brief (SPI3) SPI Interrupt Disable Register */
#define REG_SPI3_IMR                   (0x4001841CU) /**< \brief (SPI3) SPI Interrupt Mask Register */
#define REG_SPI3_CSR                   (0x40018430U) /**< \brief (SPI3) SPI Chip Select Register */
#define REG_SPI3_CMPR                  (0x40018448U) /**< \brief (SPI3) SPI Comparison Register */
#define REG_SPI3_WPMR                  (0x400184E4U) /**< \brief (SPI3) SPI Write Protection Mode Register */
#define REG_SPI3_WPSR                  (0x400184E8U) /**< \brief (SPI3) SPI Write Protection Status Register */
#define REG_SPI3_RPR                   (0x40018500U) /**< \brief (SPI3) Receive Pointer Register */
#define REG_SPI3_RCR                   (0x40018504U) /**< \brief (SPI3) Receive Counter Register */
#define REG_SPI3_TPR                   (0x40018508U) /**< \brief (SPI3) Transmit Pointer Register */
#define REG_SPI3_TCR                   (0x4001850CU) /**< \brief (SPI3) Transmit Counter Register */
#define REG_SPI3_RNPR                  (0x40018510U) /**< \brief (SPI3) Receive Next Pointer Register */
#define REG_SPI3_RNCR                  (0x40018514U) /**< \brief (SPI3) Receive Next Counter Register */
#define REG_SPI3_TNPR                  (0x40018518U) /**< \brief (SPI3) Transmit Next Pointer Register */
#define REG_SPI3_TNCR                  (0x4001851CU) /**< \brief (SPI3) Transmit Next Counter Register */
#define REG_SPI3_PTCR                  (0x40018520U) /**< \brief (SPI3) Transfer Control Register */
#define REG_SPI3_PTSR                  (0x40018524U) /**< \brief (SPI3) Transfer Status Register */
#else
#define REG_SPI3_CR   (*(__O  uint32_t*)0x40018400U) /**< \brief (SPI3) SPI Control Register */
#define REG_SPI3_MR   (*(__IO uint32_t*)0x40018404U) /**< \brief (SPI3) SPI Mode Register */
#define REG_SPI3_RDR  (*(__I  uint32_t*)0x40018408U) /**< \brief (SPI3) SPI Receive Data Register */
#define REG_SPI3_TDR  (*(__O  uint32_t*)0x4001840CU) /**< \brief (SPI3) SPI Transmit Data Register */
#define REG_SPI3_SR   (*(__I  uint32_t*)0x40018410U) /**< \brief (SPI3) SPI Status Register */
#define REG_SPI3_IER  (*(__O  uint32_t*)0x40018414U) /**< \brief (SPI3) SPI Interrupt Enable Register */
#define REG_SPI3_IDR  (*(__O  uint32_t*)0x40018418U) /**< \brief (SPI3) SPI Interrupt Disable Register */
#define REG_SPI3_IMR  (*(__I  uint32_t*)0x4001841CU) /**< \brief (SPI3) SPI Interrupt Mask Register */
#define REG_SPI3_CSR  (*(__IO uint32_t*)0x40018430U) /**< \brief (SPI3) SPI Chip Select Register */
#define REG_SPI3_CMPR (*(__IO uint32_t*)0x40018448U) /**< \brief (SPI3) SPI Comparison Register */
#define REG_SPI3_WPMR (*(__IO uint32_t*)0x400184E4U) /**< \brief (SPI3) SPI Write Protection Mode Register */
#define REG_SPI3_WPSR (*(__I  uint32_t*)0x400184E8U) /**< \brief (SPI3) SPI Write Protection Status Register */
#define REG_SPI3_RPR  (*(__IO uint32_t*)0x40018500U) /**< \brief (SPI3) Receive Pointer Register */
#define REG_SPI3_RCR  (*(__IO uint32_t*)0x40018504U) /**< \brief (SPI3) Receive Counter Register */
#define REG_SPI3_TPR  (*(__IO uint32_t*)0x40018508U) /**< \brief (SPI3) Transmit Pointer Register */
#define REG_SPI3_TCR  (*(__IO uint32_t*)0x4001850CU) /**< \brief (SPI3) Transmit Counter Register */
#define REG_SPI3_RNPR (*(__IO uint32_t*)0x40018510U) /**< \brief (SPI3) Receive Next Pointer Register */
#define REG_SPI3_RNCR (*(__IO uint32_t*)0x40018514U) /**< \brief (SPI3) Receive Next Counter Register */
#define REG_SPI3_TNPR (*(__IO uint32_t*)0x40018518U) /**< \brief (SPI3) Transmit Next Pointer Register */
#define REG_SPI3_TNCR (*(__IO uint32_t*)0x4001851CU) /**< \brief (SPI3) Transmit Next Counter Register */
#define REG_SPI3_PTCR (*(__O  uint32_t*)0x40018520U) /**< \brief (SPI3) Transfer Control Register */
#define REG_SPI3_PTSR (*(__I  uint32_t*)0x40018524U) /**< \brief (SPI3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI3_INSTANCE_ */
