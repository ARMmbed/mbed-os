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

#ifndef _SAMG55_SPI7_INSTANCE_
#define _SAMG55_SPI7_INSTANCE_

/* ========== Register definition for SPI7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI7_CR                    (0x40034400U) /**< \brief (SPI7) SPI Control Register */
#define REG_SPI7_MR                    (0x40034404U) /**< \brief (SPI7) SPI Mode Register */
#define REG_SPI7_RDR                   (0x40034408U) /**< \brief (SPI7) SPI Receive Data Register */
#define REG_SPI7_TDR                   (0x4003440CU) /**< \brief (SPI7) SPI Transmit Data Register */
#define REG_SPI7_SR                    (0x40034410U) /**< \brief (SPI7) SPI Status Register */
#define REG_SPI7_IER                   (0x40034414U) /**< \brief (SPI7) SPI Interrupt Enable Register */
#define REG_SPI7_IDR                   (0x40034418U) /**< \brief (SPI7) SPI Interrupt Disable Register */
#define REG_SPI7_IMR                   (0x4003441CU) /**< \brief (SPI7) SPI Interrupt Mask Register */
#define REG_SPI7_CSR                   (0x40034430U) /**< \brief (SPI7) SPI Chip Select Register */
#define REG_SPI7_CMPR                  (0x40034448U) /**< \brief (SPI7) SPI Comparison Register */
#define REG_SPI7_WPMR                  (0x400344E4U) /**< \brief (SPI7) SPI Write Protection Mode Register */
#define REG_SPI7_WPSR                  (0x400344E8U) /**< \brief (SPI7) SPI Write Protection Status Register */
#define REG_SPI7_RPR                   (0x40034500U) /**< \brief (SPI7) Receive Pointer Register */
#define REG_SPI7_RCR                   (0x40034504U) /**< \brief (SPI7) Receive Counter Register */
#define REG_SPI7_TPR                   (0x40034508U) /**< \brief (SPI7) Transmit Pointer Register */
#define REG_SPI7_TCR                   (0x4003450CU) /**< \brief (SPI7) Transmit Counter Register */
#define REG_SPI7_RNPR                  (0x40034510U) /**< \brief (SPI7) Receive Next Pointer Register */
#define REG_SPI7_RNCR                  (0x40034514U) /**< \brief (SPI7) Receive Next Counter Register */
#define REG_SPI7_TNPR                  (0x40034518U) /**< \brief (SPI7) Transmit Next Pointer Register */
#define REG_SPI7_TNCR                  (0x4003451CU) /**< \brief (SPI7) Transmit Next Counter Register */
#define REG_SPI7_PTCR                  (0x40034520U) /**< \brief (SPI7) Transfer Control Register */
#define REG_SPI7_PTSR                  (0x40034524U) /**< \brief (SPI7) Transfer Status Register */
#else
#define REG_SPI7_CR   (*(__O  uint32_t*)0x40034400U) /**< \brief (SPI7) SPI Control Register */
#define REG_SPI7_MR   (*(__IO uint32_t*)0x40034404U) /**< \brief (SPI7) SPI Mode Register */
#define REG_SPI7_RDR  (*(__I  uint32_t*)0x40034408U) /**< \brief (SPI7) SPI Receive Data Register */
#define REG_SPI7_TDR  (*(__O  uint32_t*)0x4003440CU) /**< \brief (SPI7) SPI Transmit Data Register */
#define REG_SPI7_SR   (*(__I  uint32_t*)0x40034410U) /**< \brief (SPI7) SPI Status Register */
#define REG_SPI7_IER  (*(__O  uint32_t*)0x40034414U) /**< \brief (SPI7) SPI Interrupt Enable Register */
#define REG_SPI7_IDR  (*(__O  uint32_t*)0x40034418U) /**< \brief (SPI7) SPI Interrupt Disable Register */
#define REG_SPI7_IMR  (*(__I  uint32_t*)0x4003441CU) /**< \brief (SPI7) SPI Interrupt Mask Register */
#define REG_SPI7_CSR  (*(__IO uint32_t*)0x40034430U) /**< \brief (SPI7) SPI Chip Select Register */
#define REG_SPI7_CMPR (*(__IO uint32_t*)0x40034448U) /**< \brief (SPI7) SPI Comparison Register */
#define REG_SPI7_WPMR (*(__IO uint32_t*)0x400344E4U) /**< \brief (SPI7) SPI Write Protection Mode Register */
#define REG_SPI7_WPSR (*(__I  uint32_t*)0x400344E8U) /**< \brief (SPI7) SPI Write Protection Status Register */
#define REG_SPI7_RPR  (*(__IO uint32_t*)0x40034500U) /**< \brief (SPI7) Receive Pointer Register */
#define REG_SPI7_RCR  (*(__IO uint32_t*)0x40034504U) /**< \brief (SPI7) Receive Counter Register */
#define REG_SPI7_TPR  (*(__IO uint32_t*)0x40034508U) /**< \brief (SPI7) Transmit Pointer Register */
#define REG_SPI7_TCR  (*(__IO uint32_t*)0x4003450CU) /**< \brief (SPI7) Transmit Counter Register */
#define REG_SPI7_RNPR (*(__IO uint32_t*)0x40034510U) /**< \brief (SPI7) Receive Next Pointer Register */
#define REG_SPI7_RNCR (*(__IO uint32_t*)0x40034514U) /**< \brief (SPI7) Receive Next Counter Register */
#define REG_SPI7_TNPR (*(__IO uint32_t*)0x40034518U) /**< \brief (SPI7) Transmit Next Pointer Register */
#define REG_SPI7_TNCR (*(__IO uint32_t*)0x4003451CU) /**< \brief (SPI7) Transmit Next Counter Register */
#define REG_SPI7_PTCR (*(__O  uint32_t*)0x40034520U) /**< \brief (SPI7) Transfer Control Register */
#define REG_SPI7_PTSR (*(__I  uint32_t*)0x40034524U) /**< \brief (SPI7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI7_INSTANCE_ */
