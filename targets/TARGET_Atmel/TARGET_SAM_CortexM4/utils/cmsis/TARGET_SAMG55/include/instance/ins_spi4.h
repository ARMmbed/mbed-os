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

#ifndef _SAMG55_SPI4_INSTANCE_
#define _SAMG55_SPI4_INSTANCE_

/* ========== Register definition for SPI4 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI4_CR                    (0x4001C400U) /**< \brief (SPI4) SPI Control Register */
#define REG_SPI4_MR                    (0x4001C404U) /**< \brief (SPI4) SPI Mode Register */
#define REG_SPI4_RDR                   (0x4001C408U) /**< \brief (SPI4) SPI Receive Data Register */
#define REG_SPI4_TDR                   (0x4001C40CU) /**< \brief (SPI4) SPI Transmit Data Register */
#define REG_SPI4_SR                    (0x4001C410U) /**< \brief (SPI4) SPI Status Register */
#define REG_SPI4_IER                   (0x4001C414U) /**< \brief (SPI4) SPI Interrupt Enable Register */
#define REG_SPI4_IDR                   (0x4001C418U) /**< \brief (SPI4) SPI Interrupt Disable Register */
#define REG_SPI4_IMR                   (0x4001C41CU) /**< \brief (SPI4) SPI Interrupt Mask Register */
#define REG_SPI4_CSR                   (0x4001C430U) /**< \brief (SPI4) SPI Chip Select Register */
#define REG_SPI4_CMPR                  (0x4001C448U) /**< \brief (SPI4) SPI Comparison Register */
#define REG_SPI4_WPMR                  (0x4001C4E4U) /**< \brief (SPI4) SPI Write Protection Mode Register */
#define REG_SPI4_WPSR                  (0x4001C4E8U) /**< \brief (SPI4) SPI Write Protection Status Register */
#define REG_SPI4_RPR                   (0x4001C500U) /**< \brief (SPI4) Receive Pointer Register */
#define REG_SPI4_RCR                   (0x4001C504U) /**< \brief (SPI4) Receive Counter Register */
#define REG_SPI4_TPR                   (0x4001C508U) /**< \brief (SPI4) Transmit Pointer Register */
#define REG_SPI4_TCR                   (0x4001C50CU) /**< \brief (SPI4) Transmit Counter Register */
#define REG_SPI4_RNPR                  (0x4001C510U) /**< \brief (SPI4) Receive Next Pointer Register */
#define REG_SPI4_RNCR                  (0x4001C514U) /**< \brief (SPI4) Receive Next Counter Register */
#define REG_SPI4_TNPR                  (0x4001C518U) /**< \brief (SPI4) Transmit Next Pointer Register */
#define REG_SPI4_TNCR                  (0x4001C51CU) /**< \brief (SPI4) Transmit Next Counter Register */
#define REG_SPI4_PTCR                  (0x4001C520U) /**< \brief (SPI4) Transfer Control Register */
#define REG_SPI4_PTSR                  (0x4001C524U) /**< \brief (SPI4) Transfer Status Register */
#else
#define REG_SPI4_CR   (*(__O  uint32_t*)0x4001C400U) /**< \brief (SPI4) SPI Control Register */
#define REG_SPI4_MR   (*(__IO uint32_t*)0x4001C404U) /**< \brief (SPI4) SPI Mode Register */
#define REG_SPI4_RDR  (*(__I  uint32_t*)0x4001C408U) /**< \brief (SPI4) SPI Receive Data Register */
#define REG_SPI4_TDR  (*(__O  uint32_t*)0x4001C40CU) /**< \brief (SPI4) SPI Transmit Data Register */
#define REG_SPI4_SR   (*(__I  uint32_t*)0x4001C410U) /**< \brief (SPI4) SPI Status Register */
#define REG_SPI4_IER  (*(__O  uint32_t*)0x4001C414U) /**< \brief (SPI4) SPI Interrupt Enable Register */
#define REG_SPI4_IDR  (*(__O  uint32_t*)0x4001C418U) /**< \brief (SPI4) SPI Interrupt Disable Register */
#define REG_SPI4_IMR  (*(__I  uint32_t*)0x4001C41CU) /**< \brief (SPI4) SPI Interrupt Mask Register */
#define REG_SPI4_CSR  (*(__IO uint32_t*)0x4001C430U) /**< \brief (SPI4) SPI Chip Select Register */
#define REG_SPI4_CMPR (*(__IO uint32_t*)0x4001C448U) /**< \brief (SPI4) SPI Comparison Register */
#define REG_SPI4_WPMR (*(__IO uint32_t*)0x4001C4E4U) /**< \brief (SPI4) SPI Write Protection Mode Register */
#define REG_SPI4_WPSR (*(__I  uint32_t*)0x4001C4E8U) /**< \brief (SPI4) SPI Write Protection Status Register */
#define REG_SPI4_RPR  (*(__IO uint32_t*)0x4001C500U) /**< \brief (SPI4) Receive Pointer Register */
#define REG_SPI4_RCR  (*(__IO uint32_t*)0x4001C504U) /**< \brief (SPI4) Receive Counter Register */
#define REG_SPI4_TPR  (*(__IO uint32_t*)0x4001C508U) /**< \brief (SPI4) Transmit Pointer Register */
#define REG_SPI4_TCR  (*(__IO uint32_t*)0x4001C50CU) /**< \brief (SPI4) Transmit Counter Register */
#define REG_SPI4_RNPR (*(__IO uint32_t*)0x4001C510U) /**< \brief (SPI4) Receive Next Pointer Register */
#define REG_SPI4_RNCR (*(__IO uint32_t*)0x4001C514U) /**< \brief (SPI4) Receive Next Counter Register */
#define REG_SPI4_TNPR (*(__IO uint32_t*)0x4001C518U) /**< \brief (SPI4) Transmit Next Pointer Register */
#define REG_SPI4_TNCR (*(__IO uint32_t*)0x4001C51CU) /**< \brief (SPI4) Transmit Next Counter Register */
#define REG_SPI4_PTCR (*(__O  uint32_t*)0x4001C520U) /**< \brief (SPI4) Transfer Control Register */
#define REG_SPI4_PTSR (*(__I  uint32_t*)0x4001C524U) /**< \brief (SPI4) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI4_INSTANCE_ */
