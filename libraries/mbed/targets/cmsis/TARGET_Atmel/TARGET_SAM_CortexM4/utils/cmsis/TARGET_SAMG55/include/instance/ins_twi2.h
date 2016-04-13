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

#ifndef _SAMG55_TWI2_INSTANCE_
#define _SAMG55_TWI2_INSTANCE_

/* ========== Register definition for TWI2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI2_CR                     (0x40024600U) /**< \brief (TWI2) TWI Control Register */
#define REG_TWI2_MMR                    (0x40024604U) /**< \brief (TWI2) TWI Master Mode Register */
#define REG_TWI2_SMR                    (0x40024608U) /**< \brief (TWI2) TWI Slave Mode Register */
#define REG_TWI2_IADR                   (0x4002460CU) /**< \brief (TWI2) TWI Internal Address Register */
#define REG_TWI2_CWGR                   (0x40024610U) /**< \brief (TWI2) TWI Clock Waveform Generator Register */
#define REG_TWI2_SR                     (0x40024620U) /**< \brief (TWI2) TWI Status Register */
#define REG_TWI2_IER                    (0x40024624U) /**< \brief (TWI2) TWI Interrupt Enable Register */
#define REG_TWI2_IDR                    (0x40024628U) /**< \brief (TWI2) TWI Interrupt Disable Register */
#define REG_TWI2_IMR                    (0x4002462CU) /**< \brief (TWI2) TWI Interrupt Mask Register */
#define REG_TWI2_RHR                    (0x40024630U) /**< \brief (TWI2) TWI Receive Holding Register */
#define REG_TWI2_THR                    (0x40024634U) /**< \brief (TWI2) TWI Transmit Holding Register */
#define REG_TWI2_SMBTR                  (0x40024638U) /**< \brief (TWI2) TWI SMBus Timing Register */
#define REG_TWI2_ACR                    (0x40024640U) /**< \brief (TWI2) TWI Alternative Command Register */
#define REG_TWI2_FILTR                  (0x40024644U) /**< \brief (TWI2) TWI Filter Register */
#define REG_TWI2_SWMR                   (0x4002464CU) /**< \brief (TWI2) TWI SleepWalking Matching Register */
#define REG_TWI2_WPMR                   (0x400246E4U) /**< \brief (TWI2) TWI Write Protection Mode Register */
#define REG_TWI2_WPSR                   (0x400246E8U) /**< \brief (TWI2) TWI Write Protection Status Register */
#define REG_TWI2_RPR                    (0x40024700U) /**< \brief (TWI2) Receive Pointer Register */
#define REG_TWI2_RCR                    (0x40024704U) /**< \brief (TWI2) Receive Counter Register */
#define REG_TWI2_TPR                    (0x40024708U) /**< \brief (TWI2) Transmit Pointer Register */
#define REG_TWI2_TCR                    (0x4002470CU) /**< \brief (TWI2) Transmit Counter Register */
#define REG_TWI2_RNPR                   (0x40024710U) /**< \brief (TWI2) Receive Next Pointer Register */
#define REG_TWI2_RNCR                   (0x40024714U) /**< \brief (TWI2) Receive Next Counter Register */
#define REG_TWI2_TNPR                   (0x40024718U) /**< \brief (TWI2) Transmit Next Pointer Register */
#define REG_TWI2_TNCR                   (0x4002471CU) /**< \brief (TWI2) Transmit Next Counter Register */
#define REG_TWI2_PTCR                   (0x40024720U) /**< \brief (TWI2) Transfer Control Register */
#define REG_TWI2_PTSR                   (0x40024724U) /**< \brief (TWI2) Transfer Status Register */
#else
#define REG_TWI2_CR    (*(__O  uint32_t*)0x40024600U) /**< \brief (TWI2) TWI Control Register */
#define REG_TWI2_MMR   (*(__IO uint32_t*)0x40024604U) /**< \brief (TWI2) TWI Master Mode Register */
#define REG_TWI2_SMR   (*(__IO uint32_t*)0x40024608U) /**< \brief (TWI2) TWI Slave Mode Register */
#define REG_TWI2_IADR  (*(__IO uint32_t*)0x4002460CU) /**< \brief (TWI2) TWI Internal Address Register */
#define REG_TWI2_CWGR  (*(__IO uint32_t*)0x40024610U) /**< \brief (TWI2) TWI Clock Waveform Generator Register */
#define REG_TWI2_SR    (*(__I  uint32_t*)0x40024620U) /**< \brief (TWI2) TWI Status Register */
#define REG_TWI2_IER   (*(__O  uint32_t*)0x40024624U) /**< \brief (TWI2) TWI Interrupt Enable Register */
#define REG_TWI2_IDR   (*(__O  uint32_t*)0x40024628U) /**< \brief (TWI2) TWI Interrupt Disable Register */
#define REG_TWI2_IMR   (*(__I  uint32_t*)0x4002462CU) /**< \brief (TWI2) TWI Interrupt Mask Register */
#define REG_TWI2_RHR   (*(__I  uint32_t*)0x40024630U) /**< \brief (TWI2) TWI Receive Holding Register */
#define REG_TWI2_THR   (*(__O  uint32_t*)0x40024634U) /**< \brief (TWI2) TWI Transmit Holding Register */
#define REG_TWI2_SMBTR (*(__IO uint32_t*)0x40024638U) /**< \brief (TWI2) TWI SMBus Timing Register */
#define REG_TWI2_ACR   (*(__IO uint32_t*)0x40024640U) /**< \brief (TWI2) TWI Alternative Command Register */
#define REG_TWI2_FILTR (*(__IO uint32_t*)0x40024644U) /**< \brief (TWI2) TWI Filter Register */
#define REG_TWI2_SWMR  (*(__IO uint32_t*)0x4002464CU) /**< \brief (TWI2) TWI SleepWalking Matching Register */
#define REG_TWI2_WPMR  (*(__IO uint32_t*)0x400246E4U) /**< \brief (TWI2) TWI Write Protection Mode Register */
#define REG_TWI2_WPSR  (*(__I  uint32_t*)0x400246E8U) /**< \brief (TWI2) TWI Write Protection Status Register */
#define REG_TWI2_RPR   (*(__IO uint32_t*)0x40024700U) /**< \brief (TWI2) Receive Pointer Register */
#define REG_TWI2_RCR   (*(__IO uint32_t*)0x40024704U) /**< \brief (TWI2) Receive Counter Register */
#define REG_TWI2_TPR   (*(__IO uint32_t*)0x40024708U) /**< \brief (TWI2) Transmit Pointer Register */
#define REG_TWI2_TCR   (*(__IO uint32_t*)0x4002470CU) /**< \brief (TWI2) Transmit Counter Register */
#define REG_TWI2_RNPR  (*(__IO uint32_t*)0x40024710U) /**< \brief (TWI2) Receive Next Pointer Register */
#define REG_TWI2_RNCR  (*(__IO uint32_t*)0x40024714U) /**< \brief (TWI2) Receive Next Counter Register */
#define REG_TWI2_TNPR  (*(__IO uint32_t*)0x40024718U) /**< \brief (TWI2) Transmit Next Pointer Register */
#define REG_TWI2_TNCR  (*(__IO uint32_t*)0x4002471CU) /**< \brief (TWI2) Transmit Next Counter Register */
#define REG_TWI2_PTCR  (*(__O  uint32_t*)0x40024720U) /**< \brief (TWI2) Transfer Control Register */
#define REG_TWI2_PTSR  (*(__I  uint32_t*)0x40024724U) /**< \brief (TWI2) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI2_INSTANCE_ */
