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

#ifndef _SAMG55_SPI2_INSTANCE_
#define _SAMG55_SPI2_INSTANCE_

/* ========== Register definition for SPI2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI2_CR                    (0x40024400U) /**< \brief (SPI2) SPI Control Register */
#define REG_SPI2_MR                    (0x40024404U) /**< \brief (SPI2) SPI Mode Register */
#define REG_SPI2_RDR                   (0x40024408U) /**< \brief (SPI2) SPI Receive Data Register */
#define REG_SPI2_TDR                   (0x4002440CU) /**< \brief (SPI2) SPI Transmit Data Register */
#define REG_SPI2_SR                    (0x40024410U) /**< \brief (SPI2) SPI Status Register */
#define REG_SPI2_IER                   (0x40024414U) /**< \brief (SPI2) SPI Interrupt Enable Register */
#define REG_SPI2_IDR                   (0x40024418U) /**< \brief (SPI2) SPI Interrupt Disable Register */
#define REG_SPI2_IMR                   (0x4002441CU) /**< \brief (SPI2) SPI Interrupt Mask Register */
#define REG_SPI2_CSR                   (0x40024430U) /**< \brief (SPI2) SPI Chip Select Register */
#define REG_SPI2_CMPR                  (0x40024448U) /**< \brief (SPI2) SPI Comparison Register */
#define REG_SPI2_WPMR                  (0x400244E4U) /**< \brief (SPI2) SPI Write Protection Mode Register */
#define REG_SPI2_WPSR                  (0x400244E8U) /**< \brief (SPI2) SPI Write Protection Status Register */
#define REG_SPI2_RPR                   (0x40024500U) /**< \brief (SPI2) Receive Pointer Register */
#define REG_SPI2_RCR                   (0x40024504U) /**< \brief (SPI2) Receive Counter Register */
#define REG_SPI2_TPR                   (0x40024508U) /**< \brief (SPI2) Transmit Pointer Register */
#define REG_SPI2_TCR                   (0x4002450CU) /**< \brief (SPI2) Transmit Counter Register */
#define REG_SPI2_RNPR                  (0x40024510U) /**< \brief (SPI2) Receive Next Pointer Register */
#define REG_SPI2_RNCR                  (0x40024514U) /**< \brief (SPI2) Receive Next Counter Register */
#define REG_SPI2_TNPR                  (0x40024518U) /**< \brief (SPI2) Transmit Next Pointer Register */
#define REG_SPI2_TNCR                  (0x4002451CU) /**< \brief (SPI2) Transmit Next Counter Register */
#define REG_SPI2_PTCR                  (0x40024520U) /**< \brief (SPI2) Transfer Control Register */
#define REG_SPI2_PTSR                  (0x40024524U) /**< \brief (SPI2) Transfer Status Register */
#else
#define REG_SPI2_CR   (*(__O  uint32_t*)0x40024400U) /**< \brief (SPI2) SPI Control Register */
#define REG_SPI2_MR   (*(__IO uint32_t*)0x40024404U) /**< \brief (SPI2) SPI Mode Register */
#define REG_SPI2_RDR  (*(__I  uint32_t*)0x40024408U) /**< \brief (SPI2) SPI Receive Data Register */
#define REG_SPI2_TDR  (*(__O  uint32_t*)0x4002440CU) /**< \brief (SPI2) SPI Transmit Data Register */
#define REG_SPI2_SR   (*(__I  uint32_t*)0x40024410U) /**< \brief (SPI2) SPI Status Register */
#define REG_SPI2_IER  (*(__O  uint32_t*)0x40024414U) /**< \brief (SPI2) SPI Interrupt Enable Register */
#define REG_SPI2_IDR  (*(__O  uint32_t*)0x40024418U) /**< \brief (SPI2) SPI Interrupt Disable Register */
#define REG_SPI2_IMR  (*(__I  uint32_t*)0x4002441CU) /**< \brief (SPI2) SPI Interrupt Mask Register */
#define REG_SPI2_CSR  (*(__IO uint32_t*)0x40024430U) /**< \brief (SPI2) SPI Chip Select Register */
#define REG_SPI2_CMPR (*(__IO uint32_t*)0x40024448U) /**< \brief (SPI2) SPI Comparison Register */
#define REG_SPI2_WPMR (*(__IO uint32_t*)0x400244E4U) /**< \brief (SPI2) SPI Write Protection Mode Register */
#define REG_SPI2_WPSR (*(__I  uint32_t*)0x400244E8U) /**< \brief (SPI2) SPI Write Protection Status Register */
#define REG_SPI2_RPR  (*(__IO uint32_t*)0x40024500U) /**< \brief (SPI2) Receive Pointer Register */
#define REG_SPI2_RCR  (*(__IO uint32_t*)0x40024504U) /**< \brief (SPI2) Receive Counter Register */
#define REG_SPI2_TPR  (*(__IO uint32_t*)0x40024508U) /**< \brief (SPI2) Transmit Pointer Register */
#define REG_SPI2_TCR  (*(__IO uint32_t*)0x4002450CU) /**< \brief (SPI2) Transmit Counter Register */
#define REG_SPI2_RNPR (*(__IO uint32_t*)0x40024510U) /**< \brief (SPI2) Receive Next Pointer Register */
#define REG_SPI2_RNCR (*(__IO uint32_t*)0x40024514U) /**< \brief (SPI2) Receive Next Counter Register */
#define REG_SPI2_TNPR (*(__IO uint32_t*)0x40024518U) /**< \brief (SPI2) Transmit Next Pointer Register */
#define REG_SPI2_TNCR (*(__IO uint32_t*)0x4002451CU) /**< \brief (SPI2) Transmit Next Counter Register */
#define REG_SPI2_PTCR (*(__O  uint32_t*)0x40024520U) /**< \brief (SPI2) Transfer Control Register */
#define REG_SPI2_PTSR (*(__I  uint32_t*)0x40024524U) /**< \brief (SPI2) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI2_INSTANCE_ */
