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

#ifndef _SAMG55_SPI1_INSTANCE_
#define _SAMG55_SPI1_INSTANCE_

/* ========== Register definition for SPI1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI1_CR                    (0x40020400U) /**< \brief (SPI1) SPI Control Register */
#define REG_SPI1_MR                    (0x40020404U) /**< \brief (SPI1) SPI Mode Register */
#define REG_SPI1_RDR                   (0x40020408U) /**< \brief (SPI1) SPI Receive Data Register */
#define REG_SPI1_TDR                   (0x4002040CU) /**< \brief (SPI1) SPI Transmit Data Register */
#define REG_SPI1_SR                    (0x40020410U) /**< \brief (SPI1) SPI Status Register */
#define REG_SPI1_IER                   (0x40020414U) /**< \brief (SPI1) SPI Interrupt Enable Register */
#define REG_SPI1_IDR                   (0x40020418U) /**< \brief (SPI1) SPI Interrupt Disable Register */
#define REG_SPI1_IMR                   (0x4002041CU) /**< \brief (SPI1) SPI Interrupt Mask Register */
#define REG_SPI1_CSR                   (0x40020430U) /**< \brief (SPI1) SPI Chip Select Register */
#define REG_SPI1_CMPR                  (0x40020448U) /**< \brief (SPI1) SPI Comparison Register */
#define REG_SPI1_WPMR                  (0x400204E4U) /**< \brief (SPI1) SPI Write Protection Mode Register */
#define REG_SPI1_WPSR                  (0x400204E8U) /**< \brief (SPI1) SPI Write Protection Status Register */
#define REG_SPI1_RPR                   (0x40020500U) /**< \brief (SPI1) Receive Pointer Register */
#define REG_SPI1_RCR                   (0x40020504U) /**< \brief (SPI1) Receive Counter Register */
#define REG_SPI1_TPR                   (0x40020508U) /**< \brief (SPI1) Transmit Pointer Register */
#define REG_SPI1_TCR                   (0x4002050CU) /**< \brief (SPI1) Transmit Counter Register */
#define REG_SPI1_RNPR                  (0x40020510U) /**< \brief (SPI1) Receive Next Pointer Register */
#define REG_SPI1_RNCR                  (0x40020514U) /**< \brief (SPI1) Receive Next Counter Register */
#define REG_SPI1_TNPR                  (0x40020518U) /**< \brief (SPI1) Transmit Next Pointer Register */
#define REG_SPI1_TNCR                  (0x4002051CU) /**< \brief (SPI1) Transmit Next Counter Register */
#define REG_SPI1_PTCR                  (0x40020520U) /**< \brief (SPI1) Transfer Control Register */
#define REG_SPI1_PTSR                  (0x40020524U) /**< \brief (SPI1) Transfer Status Register */
#else
#define REG_SPI1_CR   (*(__O  uint32_t*)0x40020400U) /**< \brief (SPI1) SPI Control Register */
#define REG_SPI1_MR   (*(__IO uint32_t*)0x40020404U) /**< \brief (SPI1) SPI Mode Register */
#define REG_SPI1_RDR  (*(__I  uint32_t*)0x40020408U) /**< \brief (SPI1) SPI Receive Data Register */
#define REG_SPI1_TDR  (*(__O  uint32_t*)0x4002040CU) /**< \brief (SPI1) SPI Transmit Data Register */
#define REG_SPI1_SR   (*(__I  uint32_t*)0x40020410U) /**< \brief (SPI1) SPI Status Register */
#define REG_SPI1_IER  (*(__O  uint32_t*)0x40020414U) /**< \brief (SPI1) SPI Interrupt Enable Register */
#define REG_SPI1_IDR  (*(__O  uint32_t*)0x40020418U) /**< \brief (SPI1) SPI Interrupt Disable Register */
#define REG_SPI1_IMR  (*(__I  uint32_t*)0x4002041CU) /**< \brief (SPI1) SPI Interrupt Mask Register */
#define REG_SPI1_CSR  (*(__IO uint32_t*)0x40020430U) /**< \brief (SPI1) SPI Chip Select Register */
#define REG_SPI1_CMPR (*(__IO uint32_t*)0x40020448U) /**< \brief (SPI1) SPI Comparison Register */
#define REG_SPI1_WPMR (*(__IO uint32_t*)0x400204E4U) /**< \brief (SPI1) SPI Write Protection Mode Register */
#define REG_SPI1_WPSR (*(__I  uint32_t*)0x400204E8U) /**< \brief (SPI1) SPI Write Protection Status Register */
#define REG_SPI1_RPR  (*(__IO uint32_t*)0x40020500U) /**< \brief (SPI1) Receive Pointer Register */
#define REG_SPI1_RCR  (*(__IO uint32_t*)0x40020504U) /**< \brief (SPI1) Receive Counter Register */
#define REG_SPI1_TPR  (*(__IO uint32_t*)0x40020508U) /**< \brief (SPI1) Transmit Pointer Register */
#define REG_SPI1_TCR  (*(__IO uint32_t*)0x4002050CU) /**< \brief (SPI1) Transmit Counter Register */
#define REG_SPI1_RNPR (*(__IO uint32_t*)0x40020510U) /**< \brief (SPI1) Receive Next Pointer Register */
#define REG_SPI1_RNCR (*(__IO uint32_t*)0x40020514U) /**< \brief (SPI1) Receive Next Counter Register */
#define REG_SPI1_TNPR (*(__IO uint32_t*)0x40020518U) /**< \brief (SPI1) Transmit Next Pointer Register */
#define REG_SPI1_TNCR (*(__IO uint32_t*)0x4002051CU) /**< \brief (SPI1) Transmit Next Counter Register */
#define REG_SPI1_PTCR (*(__O  uint32_t*)0x40020520U) /**< \brief (SPI1) Transfer Control Register */
#define REG_SPI1_PTSR (*(__I  uint32_t*)0x40020524U) /**< \brief (SPI1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI1_INSTANCE_ */
