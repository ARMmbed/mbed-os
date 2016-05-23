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

#ifndef _SAMG55_TWI6_INSTANCE_
#define _SAMG55_TWI6_INSTANCE_

/* ========== Register definition for TWI6 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI6_CR                     (0x40040600U) /**< \brief (TWI6) TWI Control Register */
#define REG_TWI6_MMR                    (0x40040604U) /**< \brief (TWI6) TWI Master Mode Register */
#define REG_TWI6_SMR                    (0x40040608U) /**< \brief (TWI6) TWI Slave Mode Register */
#define REG_TWI6_IADR                   (0x4004060CU) /**< \brief (TWI6) TWI Internal Address Register */
#define REG_TWI6_CWGR                   (0x40040610U) /**< \brief (TWI6) TWI Clock Waveform Generator Register */
#define REG_TWI6_SR                     (0x40040620U) /**< \brief (TWI6) TWI Status Register */
#define REG_TWI6_IER                    (0x40040624U) /**< \brief (TWI6) TWI Interrupt Enable Register */
#define REG_TWI6_IDR                    (0x40040628U) /**< \brief (TWI6) TWI Interrupt Disable Register */
#define REG_TWI6_IMR                    (0x4004062CU) /**< \brief (TWI6) TWI Interrupt Mask Register */
#define REG_TWI6_RHR                    (0x40040630U) /**< \brief (TWI6) TWI Receive Holding Register */
#define REG_TWI6_THR                    (0x40040634U) /**< \brief (TWI6) TWI Transmit Holding Register */
#define REG_TWI6_SMBTR                  (0x40040638U) /**< \brief (TWI6) TWI SMBus Timing Register */
#define REG_TWI6_ACR                    (0x40040640U) /**< \brief (TWI6) TWI Alternative Command Register */
#define REG_TWI6_FILTR                  (0x40040644U) /**< \brief (TWI6) TWI Filter Register */
#define REG_TWI6_SWMR                   (0x4004064CU) /**< \brief (TWI6) TWI SleepWalking Matching Register */
#define REG_TWI6_WPMR                   (0x400406E4U) /**< \brief (TWI6) TWI Write Protection Mode Register */
#define REG_TWI6_WPSR                   (0x400406E8U) /**< \brief (TWI6) TWI Write Protection Status Register */
#define REG_TWI6_RPR                    (0x40040700U) /**< \brief (TWI6) Receive Pointer Register */
#define REG_TWI6_RCR                    (0x40040704U) /**< \brief (TWI6) Receive Counter Register */
#define REG_TWI6_TPR                    (0x40040708U) /**< \brief (TWI6) Transmit Pointer Register */
#define REG_TWI6_TCR                    (0x4004070CU) /**< \brief (TWI6) Transmit Counter Register */
#define REG_TWI6_RNPR                   (0x40040710U) /**< \brief (TWI6) Receive Next Pointer Register */
#define REG_TWI6_RNCR                   (0x40040714U) /**< \brief (TWI6) Receive Next Counter Register */
#define REG_TWI6_TNPR                   (0x40040718U) /**< \brief (TWI6) Transmit Next Pointer Register */
#define REG_TWI6_TNCR                   (0x4004071CU) /**< \brief (TWI6) Transmit Next Counter Register */
#define REG_TWI6_PTCR                   (0x40040720U) /**< \brief (TWI6) Transfer Control Register */
#define REG_TWI6_PTSR                   (0x40040724U) /**< \brief (TWI6) Transfer Status Register */
#else
#define REG_TWI6_CR    (*(__O  uint32_t*)0x40040600U) /**< \brief (TWI6) TWI Control Register */
#define REG_TWI6_MMR   (*(__IO uint32_t*)0x40040604U) /**< \brief (TWI6) TWI Master Mode Register */
#define REG_TWI6_SMR   (*(__IO uint32_t*)0x40040608U) /**< \brief (TWI6) TWI Slave Mode Register */
#define REG_TWI6_IADR  (*(__IO uint32_t*)0x4004060CU) /**< \brief (TWI6) TWI Internal Address Register */
#define REG_TWI6_CWGR  (*(__IO uint32_t*)0x40040610U) /**< \brief (TWI6) TWI Clock Waveform Generator Register */
#define REG_TWI6_SR    (*(__I  uint32_t*)0x40040620U) /**< \brief (TWI6) TWI Status Register */
#define REG_TWI6_IER   (*(__O  uint32_t*)0x40040624U) /**< \brief (TWI6) TWI Interrupt Enable Register */
#define REG_TWI6_IDR   (*(__O  uint32_t*)0x40040628U) /**< \brief (TWI6) TWI Interrupt Disable Register */
#define REG_TWI6_IMR   (*(__I  uint32_t*)0x4004062CU) /**< \brief (TWI6) TWI Interrupt Mask Register */
#define REG_TWI6_RHR   (*(__I  uint32_t*)0x40040630U) /**< \brief (TWI6) TWI Receive Holding Register */
#define REG_TWI6_THR   (*(__O  uint32_t*)0x40040634U) /**< \brief (TWI6) TWI Transmit Holding Register */
#define REG_TWI6_SMBTR (*(__IO uint32_t*)0x40040638U) /**< \brief (TWI6) TWI SMBus Timing Register */
#define REG_TWI6_ACR   (*(__IO uint32_t*)0x40040640U) /**< \brief (TWI6) TWI Alternative Command Register */
#define REG_TWI6_FILTR (*(__IO uint32_t*)0x40040644U) /**< \brief (TWI6) TWI Filter Register */
#define REG_TWI6_SWMR  (*(__IO uint32_t*)0x4004064CU) /**< \brief (TWI6) TWI SleepWalking Matching Register */
#define REG_TWI6_WPMR  (*(__IO uint32_t*)0x400406E4U) /**< \brief (TWI6) TWI Write Protection Mode Register */
#define REG_TWI6_WPSR  (*(__I  uint32_t*)0x400406E8U) /**< \brief (TWI6) TWI Write Protection Status Register */
#define REG_TWI6_RPR   (*(__IO uint32_t*)0x40040700U) /**< \brief (TWI6) Receive Pointer Register */
#define REG_TWI6_RCR   (*(__IO uint32_t*)0x40040704U) /**< \brief (TWI6) Receive Counter Register */
#define REG_TWI6_TPR   (*(__IO uint32_t*)0x40040708U) /**< \brief (TWI6) Transmit Pointer Register */
#define REG_TWI6_TCR   (*(__IO uint32_t*)0x4004070CU) /**< \brief (TWI6) Transmit Counter Register */
#define REG_TWI6_RNPR  (*(__IO uint32_t*)0x40040710U) /**< \brief (TWI6) Receive Next Pointer Register */
#define REG_TWI6_RNCR  (*(__IO uint32_t*)0x40040714U) /**< \brief (TWI6) Receive Next Counter Register */
#define REG_TWI6_TNPR  (*(__IO uint32_t*)0x40040718U) /**< \brief (TWI6) Transmit Next Pointer Register */
#define REG_TWI6_TNCR  (*(__IO uint32_t*)0x4004071CU) /**< \brief (TWI6) Transmit Next Counter Register */
#define REG_TWI6_PTCR  (*(__O  uint32_t*)0x40040720U) /**< \brief (TWI6) Transfer Control Register */
#define REG_TWI6_PTSR  (*(__I  uint32_t*)0x40040724U) /**< \brief (TWI6) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI6_INSTANCE_ */
