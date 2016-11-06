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

#ifndef _SAMG55_SPI0_INSTANCE_
#define _SAMG55_SPI0_INSTANCE_

/* ========== Register definition for SPI0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI0_CR                    (0x4000C400U) /**< \brief (SPI0) SPI Control Register */
#define REG_SPI0_MR                    (0x4000C404U) /**< \brief (SPI0) SPI Mode Register */
#define REG_SPI0_RDR                   (0x4000C408U) /**< \brief (SPI0) SPI Receive Data Register */
#define REG_SPI0_TDR                   (0x4000C40CU) /**< \brief (SPI0) SPI Transmit Data Register */
#define REG_SPI0_SR                    (0x4000C410U) /**< \brief (SPI0) SPI Status Register */
#define REG_SPI0_IER                   (0x4000C414U) /**< \brief (SPI0) SPI Interrupt Enable Register */
#define REG_SPI0_IDR                   (0x4000C418U) /**< \brief (SPI0) SPI Interrupt Disable Register */
#define REG_SPI0_IMR                   (0x4000C41CU) /**< \brief (SPI0) SPI Interrupt Mask Register */
#define REG_SPI0_CSR                   (0x4000C430U) /**< \brief (SPI0) SPI Chip Select Register */
#define REG_SPI0_CMPR                  (0x4000C448U) /**< \brief (SPI0) SPI Comparison Register */
#define REG_SPI0_WPMR                  (0x4000C4E4U) /**< \brief (SPI0) SPI Write Protection Mode Register */
#define REG_SPI0_WPSR                  (0x4000C4E8U) /**< \brief (SPI0) SPI Write Protection Status Register */
#define REG_SPI0_RPR                   (0x4000C500U) /**< \brief (SPI0) Receive Pointer Register */
#define REG_SPI0_RCR                   (0x4000C504U) /**< \brief (SPI0) Receive Counter Register */
#define REG_SPI0_TPR                   (0x4000C508U) /**< \brief (SPI0) Transmit Pointer Register */
#define REG_SPI0_TCR                   (0x4000C50CU) /**< \brief (SPI0) Transmit Counter Register */
#define REG_SPI0_RNPR                  (0x4000C510U) /**< \brief (SPI0) Receive Next Pointer Register */
#define REG_SPI0_RNCR                  (0x4000C514U) /**< \brief (SPI0) Receive Next Counter Register */
#define REG_SPI0_TNPR                  (0x4000C518U) /**< \brief (SPI0) Transmit Next Pointer Register */
#define REG_SPI0_TNCR                  (0x4000C51CU) /**< \brief (SPI0) Transmit Next Counter Register */
#define REG_SPI0_PTCR                  (0x4000C520U) /**< \brief (SPI0) Transfer Control Register */
#define REG_SPI0_PTSR                  (0x4000C524U) /**< \brief (SPI0) Transfer Status Register */
#else
#define REG_SPI0_CR   (*(__O  uint32_t*)0x4000C400U) /**< \brief (SPI0) SPI Control Register */
#define REG_SPI0_MR   (*(__IO uint32_t*)0x4000C404U) /**< \brief (SPI0) SPI Mode Register */
#define REG_SPI0_RDR  (*(__I  uint32_t*)0x4000C408U) /**< \brief (SPI0) SPI Receive Data Register */
#define REG_SPI0_TDR  (*(__O  uint32_t*)0x4000C40CU) /**< \brief (SPI0) SPI Transmit Data Register */
#define REG_SPI0_SR   (*(__I  uint32_t*)0x4000C410U) /**< \brief (SPI0) SPI Status Register */
#define REG_SPI0_IER  (*(__O  uint32_t*)0x4000C414U) /**< \brief (SPI0) SPI Interrupt Enable Register */
#define REG_SPI0_IDR  (*(__O  uint32_t*)0x4000C418U) /**< \brief (SPI0) SPI Interrupt Disable Register */
#define REG_SPI0_IMR  (*(__I  uint32_t*)0x4000C41CU) /**< \brief (SPI0) SPI Interrupt Mask Register */
#define REG_SPI0_CSR  (*(__IO uint32_t*)0x4000C430U) /**< \brief (SPI0) SPI Chip Select Register */
#define REG_SPI0_CMPR (*(__IO uint32_t*)0x4000C448U) /**< \brief (SPI0) SPI Comparison Register */
#define REG_SPI0_WPMR (*(__IO uint32_t*)0x4000C4E4U) /**< \brief (SPI0) SPI Write Protection Mode Register */
#define REG_SPI0_WPSR (*(__I  uint32_t*)0x4000C4E8U) /**< \brief (SPI0) SPI Write Protection Status Register */
#define REG_SPI0_RPR  (*(__IO uint32_t*)0x4000C500U) /**< \brief (SPI0) Receive Pointer Register */
#define REG_SPI0_RCR  (*(__IO uint32_t*)0x4000C504U) /**< \brief (SPI0) Receive Counter Register */
#define REG_SPI0_TPR  (*(__IO uint32_t*)0x4000C508U) /**< \brief (SPI0) Transmit Pointer Register */
#define REG_SPI0_TCR  (*(__IO uint32_t*)0x4000C50CU) /**< \brief (SPI0) Transmit Counter Register */
#define REG_SPI0_RNPR (*(__IO uint32_t*)0x4000C510U) /**< \brief (SPI0) Receive Next Pointer Register */
#define REG_SPI0_RNCR (*(__IO uint32_t*)0x4000C514U) /**< \brief (SPI0) Receive Next Counter Register */
#define REG_SPI0_TNPR (*(__IO uint32_t*)0x4000C518U) /**< \brief (SPI0) Transmit Next Pointer Register */
#define REG_SPI0_TNCR (*(__IO uint32_t*)0x4000C51CU) /**< \brief (SPI0) Transmit Next Counter Register */
#define REG_SPI0_PTCR (*(__O  uint32_t*)0x4000C520U) /**< \brief (SPI0) Transfer Control Register */
#define REG_SPI0_PTSR (*(__I  uint32_t*)0x4000C524U) /**< \brief (SPI0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI0_INSTANCE_ */
