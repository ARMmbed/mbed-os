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

#ifndef _SAMG55_TWI0_INSTANCE_
#define _SAMG55_TWI0_INSTANCE_

/* ========== Register definition for TWI0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI0_CR                     (0x4000C600U) /**< \brief (TWI0) TWI Control Register */
#define REG_TWI0_MMR                    (0x4000C604U) /**< \brief (TWI0) TWI Master Mode Register */
#define REG_TWI0_SMR                    (0x4000C608U) /**< \brief (TWI0) TWI Slave Mode Register */
#define REG_TWI0_IADR                   (0x4000C60CU) /**< \brief (TWI0) TWI Internal Address Register */
#define REG_TWI0_CWGR                   (0x4000C610U) /**< \brief (TWI0) TWI Clock Waveform Generator Register */
#define REG_TWI0_SR                     (0x4000C620U) /**< \brief (TWI0) TWI Status Register */
#define REG_TWI0_IER                    (0x4000C624U) /**< \brief (TWI0) TWI Interrupt Enable Register */
#define REG_TWI0_IDR                    (0x4000C628U) /**< \brief (TWI0) TWI Interrupt Disable Register */
#define REG_TWI0_IMR                    (0x4000C62CU) /**< \brief (TWI0) TWI Interrupt Mask Register */
#define REG_TWI0_RHR                    (0x4000C630U) /**< \brief (TWI0) TWI Receive Holding Register */
#define REG_TWI0_THR                    (0x4000C634U) /**< \brief (TWI0) TWI Transmit Holding Register */
#define REG_TWI0_SMBTR                  (0x4000C638U) /**< \brief (TWI0) TWI SMBus Timing Register */
#define REG_TWI0_ACR                    (0x4000C640U) /**< \brief (TWI0) TWI Alternative Command Register */
#define REG_TWI0_FILTR                  (0x4000C644U) /**< \brief (TWI0) TWI Filter Register */
#define REG_TWI0_SWMR                   (0x4000C64CU) /**< \brief (TWI0) TWI SleepWalking Matching Register */
#define REG_TWI0_WPMR                   (0x4000C6E4U) /**< \brief (TWI0) TWI Write Protection Mode Register */
#define REG_TWI0_WPSR                   (0x4000C6E8U) /**< \brief (TWI0) TWI Write Protection Status Register */
#define REG_TWI0_RPR                    (0x4000C700U) /**< \brief (TWI0) Receive Pointer Register */
#define REG_TWI0_RCR                    (0x4000C704U) /**< \brief (TWI0) Receive Counter Register */
#define REG_TWI0_TPR                    (0x4000C708U) /**< \brief (TWI0) Transmit Pointer Register */
#define REG_TWI0_TCR                    (0x4000C70CU) /**< \brief (TWI0) Transmit Counter Register */
#define REG_TWI0_RNPR                   (0x4000C710U) /**< \brief (TWI0) Receive Next Pointer Register */
#define REG_TWI0_RNCR                   (0x4000C714U) /**< \brief (TWI0) Receive Next Counter Register */
#define REG_TWI0_TNPR                   (0x4000C718U) /**< \brief (TWI0) Transmit Next Pointer Register */
#define REG_TWI0_TNCR                   (0x4000C71CU) /**< \brief (TWI0) Transmit Next Counter Register */
#define REG_TWI0_PTCR                   (0x4000C720U) /**< \brief (TWI0) Transfer Control Register */
#define REG_TWI0_PTSR                   (0x4000C724U) /**< \brief (TWI0) Transfer Status Register */
#else
#define REG_TWI0_CR    (*(__O  uint32_t*)0x4000C600U) /**< \brief (TWI0) TWI Control Register */
#define REG_TWI0_MMR   (*(__IO uint32_t*)0x4000C604U) /**< \brief (TWI0) TWI Master Mode Register */
#define REG_TWI0_SMR   (*(__IO uint32_t*)0x4000C608U) /**< \brief (TWI0) TWI Slave Mode Register */
#define REG_TWI0_IADR  (*(__IO uint32_t*)0x4000C60CU) /**< \brief (TWI0) TWI Internal Address Register */
#define REG_TWI0_CWGR  (*(__IO uint32_t*)0x4000C610U) /**< \brief (TWI0) TWI Clock Waveform Generator Register */
#define REG_TWI0_SR    (*(__I  uint32_t*)0x4000C620U) /**< \brief (TWI0) TWI Status Register */
#define REG_TWI0_IER   (*(__O  uint32_t*)0x4000C624U) /**< \brief (TWI0) TWI Interrupt Enable Register */
#define REG_TWI0_IDR   (*(__O  uint32_t*)0x4000C628U) /**< \brief (TWI0) TWI Interrupt Disable Register */
#define REG_TWI0_IMR   (*(__I  uint32_t*)0x4000C62CU) /**< \brief (TWI0) TWI Interrupt Mask Register */
#define REG_TWI0_RHR   (*(__I  uint32_t*)0x4000C630U) /**< \brief (TWI0) TWI Receive Holding Register */
#define REG_TWI0_THR   (*(__O  uint32_t*)0x4000C634U) /**< \brief (TWI0) TWI Transmit Holding Register */
#define REG_TWI0_SMBTR (*(__IO uint32_t*)0x4000C638U) /**< \brief (TWI0) TWI SMBus Timing Register */
#define REG_TWI0_ACR   (*(__IO uint32_t*)0x4000C640U) /**< \brief (TWI0) TWI Alternative Command Register */
#define REG_TWI0_FILTR (*(__IO uint32_t*)0x4000C644U) /**< \brief (TWI0) TWI Filter Register */
#define REG_TWI0_SWMR  (*(__IO uint32_t*)0x4000C64CU) /**< \brief (TWI0) TWI SleepWalking Matching Register */
#define REG_TWI0_WPMR  (*(__IO uint32_t*)0x4000C6E4U) /**< \brief (TWI0) TWI Write Protection Mode Register */
#define REG_TWI0_WPSR  (*(__I  uint32_t*)0x4000C6E8U) /**< \brief (TWI0) TWI Write Protection Status Register */
#define REG_TWI0_RPR   (*(__IO uint32_t*)0x4000C700U) /**< \brief (TWI0) Receive Pointer Register */
#define REG_TWI0_RCR   (*(__IO uint32_t*)0x4000C704U) /**< \brief (TWI0) Receive Counter Register */
#define REG_TWI0_TPR   (*(__IO uint32_t*)0x4000C708U) /**< \brief (TWI0) Transmit Pointer Register */
#define REG_TWI0_TCR   (*(__IO uint32_t*)0x4000C70CU) /**< \brief (TWI0) Transmit Counter Register */
#define REG_TWI0_RNPR  (*(__IO uint32_t*)0x4000C710U) /**< \brief (TWI0) Receive Next Pointer Register */
#define REG_TWI0_RNCR  (*(__IO uint32_t*)0x4000C714U) /**< \brief (TWI0) Receive Next Counter Register */
#define REG_TWI0_TNPR  (*(__IO uint32_t*)0x4000C718U) /**< \brief (TWI0) Transmit Next Pointer Register */
#define REG_TWI0_TNCR  (*(__IO uint32_t*)0x4000C71CU) /**< \brief (TWI0) Transmit Next Counter Register */
#define REG_TWI0_PTCR  (*(__O  uint32_t*)0x4000C720U) /**< \brief (TWI0) Transfer Control Register */
#define REG_TWI0_PTSR  (*(__I  uint32_t*)0x4000C724U) /**< \brief (TWI0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI0_INSTANCE_ */
