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

#ifndef _SAMG55_TWI7_INSTANCE_
#define _SAMG55_TWI7_INSTANCE_

/* ========== Register definition for TWI7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI7_CR                     (0x40034600U) /**< \brief (TWI7) TWI Control Register */
#define REG_TWI7_MMR                    (0x40034604U) /**< \brief (TWI7) TWI Master Mode Register */
#define REG_TWI7_SMR                    (0x40034608U) /**< \brief (TWI7) TWI Slave Mode Register */
#define REG_TWI7_IADR                   (0x4003460CU) /**< \brief (TWI7) TWI Internal Address Register */
#define REG_TWI7_CWGR                   (0x40034610U) /**< \brief (TWI7) TWI Clock Waveform Generator Register */
#define REG_TWI7_SR                     (0x40034620U) /**< \brief (TWI7) TWI Status Register */
#define REG_TWI7_IER                    (0x40034624U) /**< \brief (TWI7) TWI Interrupt Enable Register */
#define REG_TWI7_IDR                    (0x40034628U) /**< \brief (TWI7) TWI Interrupt Disable Register */
#define REG_TWI7_IMR                    (0x4003462CU) /**< \brief (TWI7) TWI Interrupt Mask Register */
#define REG_TWI7_RHR                    (0x40034630U) /**< \brief (TWI7) TWI Receive Holding Register */
#define REG_TWI7_THR                    (0x40034634U) /**< \brief (TWI7) TWI Transmit Holding Register */
#define REG_TWI7_SMBTR                  (0x40034638U) /**< \brief (TWI7) TWI SMBus Timing Register */
#define REG_TWI7_ACR                    (0x40034640U) /**< \brief (TWI7) TWI Alternative Command Register */
#define REG_TWI7_FILTR                  (0x40034644U) /**< \brief (TWI7) TWI Filter Register */
#define REG_TWI7_SWMR                   (0x4003464CU) /**< \brief (TWI7) TWI SleepWalking Matching Register */
#define REG_TWI7_WPMR                   (0x400346E4U) /**< \brief (TWI7) TWI Write Protection Mode Register */
#define REG_TWI7_WPSR                   (0x400346E8U) /**< \brief (TWI7) TWI Write Protection Status Register */
#define REG_TWI7_RPR                    (0x40034700U) /**< \brief (TWI7) Receive Pointer Register */
#define REG_TWI7_RCR                    (0x40034704U) /**< \brief (TWI7) Receive Counter Register */
#define REG_TWI7_TPR                    (0x40034708U) /**< \brief (TWI7) Transmit Pointer Register */
#define REG_TWI7_TCR                    (0x4003470CU) /**< \brief (TWI7) Transmit Counter Register */
#define REG_TWI7_RNPR                   (0x40034710U) /**< \brief (TWI7) Receive Next Pointer Register */
#define REG_TWI7_RNCR                   (0x40034714U) /**< \brief (TWI7) Receive Next Counter Register */
#define REG_TWI7_TNPR                   (0x40034718U) /**< \brief (TWI7) Transmit Next Pointer Register */
#define REG_TWI7_TNCR                   (0x4003471CU) /**< \brief (TWI7) Transmit Next Counter Register */
#define REG_TWI7_PTCR                   (0x40034720U) /**< \brief (TWI7) Transfer Control Register */
#define REG_TWI7_PTSR                   (0x40034724U) /**< \brief (TWI7) Transfer Status Register */
#else
#define REG_TWI7_CR    (*(__O  uint32_t*)0x40034600U) /**< \brief (TWI7) TWI Control Register */
#define REG_TWI7_MMR   (*(__IO uint32_t*)0x40034604U) /**< \brief (TWI7) TWI Master Mode Register */
#define REG_TWI7_SMR   (*(__IO uint32_t*)0x40034608U) /**< \brief (TWI7) TWI Slave Mode Register */
#define REG_TWI7_IADR  (*(__IO uint32_t*)0x4003460CU) /**< \brief (TWI7) TWI Internal Address Register */
#define REG_TWI7_CWGR  (*(__IO uint32_t*)0x40034610U) /**< \brief (TWI7) TWI Clock Waveform Generator Register */
#define REG_TWI7_SR    (*(__I  uint32_t*)0x40034620U) /**< \brief (TWI7) TWI Status Register */
#define REG_TWI7_IER   (*(__O  uint32_t*)0x40034624U) /**< \brief (TWI7) TWI Interrupt Enable Register */
#define REG_TWI7_IDR   (*(__O  uint32_t*)0x40034628U) /**< \brief (TWI7) TWI Interrupt Disable Register */
#define REG_TWI7_IMR   (*(__I  uint32_t*)0x4003462CU) /**< \brief (TWI7) TWI Interrupt Mask Register */
#define REG_TWI7_RHR   (*(__I  uint32_t*)0x40034630U) /**< \brief (TWI7) TWI Receive Holding Register */
#define REG_TWI7_THR   (*(__O  uint32_t*)0x40034634U) /**< \brief (TWI7) TWI Transmit Holding Register */
#define REG_TWI7_SMBTR (*(__IO uint32_t*)0x40034638U) /**< \brief (TWI7) TWI SMBus Timing Register */
#define REG_TWI7_ACR   (*(__IO uint32_t*)0x40034640U) /**< \brief (TWI7) TWI Alternative Command Register */
#define REG_TWI7_FILTR (*(__IO uint32_t*)0x40034644U) /**< \brief (TWI7) TWI Filter Register */
#define REG_TWI7_SWMR  (*(__IO uint32_t*)0x4003464CU) /**< \brief (TWI7) TWI SleepWalking Matching Register */
#define REG_TWI7_WPMR  (*(__IO uint32_t*)0x400346E4U) /**< \brief (TWI7) TWI Write Protection Mode Register */
#define REG_TWI7_WPSR  (*(__I  uint32_t*)0x400346E8U) /**< \brief (TWI7) TWI Write Protection Status Register */
#define REG_TWI7_RPR   (*(__IO uint32_t*)0x40034700U) /**< \brief (TWI7) Receive Pointer Register */
#define REG_TWI7_RCR   (*(__IO uint32_t*)0x40034704U) /**< \brief (TWI7) Receive Counter Register */
#define REG_TWI7_TPR   (*(__IO uint32_t*)0x40034708U) /**< \brief (TWI7) Transmit Pointer Register */
#define REG_TWI7_TCR   (*(__IO uint32_t*)0x4003470CU) /**< \brief (TWI7) Transmit Counter Register */
#define REG_TWI7_RNPR  (*(__IO uint32_t*)0x40034710U) /**< \brief (TWI7) Receive Next Pointer Register */
#define REG_TWI7_RNCR  (*(__IO uint32_t*)0x40034714U) /**< \brief (TWI7) Receive Next Counter Register */
#define REG_TWI7_TNPR  (*(__IO uint32_t*)0x40034718U) /**< \brief (TWI7) Transmit Next Pointer Register */
#define REG_TWI7_TNCR  (*(__IO uint32_t*)0x4003471CU) /**< \brief (TWI7) Transmit Next Counter Register */
#define REG_TWI7_PTCR  (*(__O  uint32_t*)0x40034720U) /**< \brief (TWI7) Transfer Control Register */
#define REG_TWI7_PTSR  (*(__I  uint32_t*)0x40034724U) /**< \brief (TWI7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI7_INSTANCE_ */
