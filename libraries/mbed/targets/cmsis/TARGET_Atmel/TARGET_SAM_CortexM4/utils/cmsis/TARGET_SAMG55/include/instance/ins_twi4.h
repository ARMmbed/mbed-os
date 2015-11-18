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

#ifndef _SAMG55_TWI4_INSTANCE_
#define _SAMG55_TWI4_INSTANCE_

/* ========== Register definition for TWI4 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI4_CR                     (0x4001C600U) /**< \brief (TWI4) TWI Control Register */
#define REG_TWI4_MMR                    (0x4001C604U) /**< \brief (TWI4) TWI Master Mode Register */
#define REG_TWI4_SMR                    (0x4001C608U) /**< \brief (TWI4) TWI Slave Mode Register */
#define REG_TWI4_IADR                   (0x4001C60CU) /**< \brief (TWI4) TWI Internal Address Register */
#define REG_TWI4_CWGR                   (0x4001C610U) /**< \brief (TWI4) TWI Clock Waveform Generator Register */
#define REG_TWI4_SR                     (0x4001C620U) /**< \brief (TWI4) TWI Status Register */
#define REG_TWI4_IER                    (0x4001C624U) /**< \brief (TWI4) TWI Interrupt Enable Register */
#define REG_TWI4_IDR                    (0x4001C628U) /**< \brief (TWI4) TWI Interrupt Disable Register */
#define REG_TWI4_IMR                    (0x4001C62CU) /**< \brief (TWI4) TWI Interrupt Mask Register */
#define REG_TWI4_RHR                    (0x4001C630U) /**< \brief (TWI4) TWI Receive Holding Register */
#define REG_TWI4_THR                    (0x4001C634U) /**< \brief (TWI4) TWI Transmit Holding Register */
#define REG_TWI4_SMBTR                  (0x4001C638U) /**< \brief (TWI4) TWI SMBus Timing Register */
#define REG_TWI4_ACR                    (0x4001C640U) /**< \brief (TWI4) TWI Alternative Command Register */
#define REG_TWI4_FILTR                  (0x4001C644U) /**< \brief (TWI4) TWI Filter Register */
#define REG_TWI4_SWMR                   (0x4001C64CU) /**< \brief (TWI4) TWI SleepWalking Matching Register */
#define REG_TWI4_WPMR                   (0x4001C6E4U) /**< \brief (TWI4) TWI Write Protection Mode Register */
#define REG_TWI4_WPSR                   (0x4001C6E8U) /**< \brief (TWI4) TWI Write Protection Status Register */
#define REG_TWI4_RPR                    (0x4001C700U) /**< \brief (TWI4) Receive Pointer Register */
#define REG_TWI4_RCR                    (0x4001C704U) /**< \brief (TWI4) Receive Counter Register */
#define REG_TWI4_TPR                    (0x4001C708U) /**< \brief (TWI4) Transmit Pointer Register */
#define REG_TWI4_TCR                    (0x4001C70CU) /**< \brief (TWI4) Transmit Counter Register */
#define REG_TWI4_RNPR                   (0x4001C710U) /**< \brief (TWI4) Receive Next Pointer Register */
#define REG_TWI4_RNCR                   (0x4001C714U) /**< \brief (TWI4) Receive Next Counter Register */
#define REG_TWI4_TNPR                   (0x4001C718U) /**< \brief (TWI4) Transmit Next Pointer Register */
#define REG_TWI4_TNCR                   (0x4001C71CU) /**< \brief (TWI4) Transmit Next Counter Register */
#define REG_TWI4_PTCR                   (0x4001C720U) /**< \brief (TWI4) Transfer Control Register */
#define REG_TWI4_PTSR                   (0x4001C724U) /**< \brief (TWI4) Transfer Status Register */
#else
#define REG_TWI4_CR    (*(__O  uint32_t*)0x4001C600U) /**< \brief (TWI4) TWI Control Register */
#define REG_TWI4_MMR   (*(__IO uint32_t*)0x4001C604U) /**< \brief (TWI4) TWI Master Mode Register */
#define REG_TWI4_SMR   (*(__IO uint32_t*)0x4001C608U) /**< \brief (TWI4) TWI Slave Mode Register */
#define REG_TWI4_IADR  (*(__IO uint32_t*)0x4001C60CU) /**< \brief (TWI4) TWI Internal Address Register */
#define REG_TWI4_CWGR  (*(__IO uint32_t*)0x4001C610U) /**< \brief (TWI4) TWI Clock Waveform Generator Register */
#define REG_TWI4_SR    (*(__I  uint32_t*)0x4001C620U) /**< \brief (TWI4) TWI Status Register */
#define REG_TWI4_IER   (*(__O  uint32_t*)0x4001C624U) /**< \brief (TWI4) TWI Interrupt Enable Register */
#define REG_TWI4_IDR   (*(__O  uint32_t*)0x4001C628U) /**< \brief (TWI4) TWI Interrupt Disable Register */
#define REG_TWI4_IMR   (*(__I  uint32_t*)0x4001C62CU) /**< \brief (TWI4) TWI Interrupt Mask Register */
#define REG_TWI4_RHR   (*(__I  uint32_t*)0x4001C630U) /**< \brief (TWI4) TWI Receive Holding Register */
#define REG_TWI4_THR   (*(__O  uint32_t*)0x4001C634U) /**< \brief (TWI4) TWI Transmit Holding Register */
#define REG_TWI4_SMBTR (*(__IO uint32_t*)0x4001C638U) /**< \brief (TWI4) TWI SMBus Timing Register */
#define REG_TWI4_ACR   (*(__IO uint32_t*)0x4001C640U) /**< \brief (TWI4) TWI Alternative Command Register */
#define REG_TWI4_FILTR (*(__IO uint32_t*)0x4001C644U) /**< \brief (TWI4) TWI Filter Register */
#define REG_TWI4_SWMR  (*(__IO uint32_t*)0x4001C64CU) /**< \brief (TWI4) TWI SleepWalking Matching Register */
#define REG_TWI4_WPMR  (*(__IO uint32_t*)0x4001C6E4U) /**< \brief (TWI4) TWI Write Protection Mode Register */
#define REG_TWI4_WPSR  (*(__I  uint32_t*)0x4001C6E8U) /**< \brief (TWI4) TWI Write Protection Status Register */
#define REG_TWI4_RPR   (*(__IO uint32_t*)0x4001C700U) /**< \brief (TWI4) Receive Pointer Register */
#define REG_TWI4_RCR   (*(__IO uint32_t*)0x4001C704U) /**< \brief (TWI4) Receive Counter Register */
#define REG_TWI4_TPR   (*(__IO uint32_t*)0x4001C708U) /**< \brief (TWI4) Transmit Pointer Register */
#define REG_TWI4_TCR   (*(__IO uint32_t*)0x4001C70CU) /**< \brief (TWI4) Transmit Counter Register */
#define REG_TWI4_RNPR  (*(__IO uint32_t*)0x4001C710U) /**< \brief (TWI4) Receive Next Pointer Register */
#define REG_TWI4_RNCR  (*(__IO uint32_t*)0x4001C714U) /**< \brief (TWI4) Receive Next Counter Register */
#define REG_TWI4_TNPR  (*(__IO uint32_t*)0x4001C718U) /**< \brief (TWI4) Transmit Next Pointer Register */
#define REG_TWI4_TNCR  (*(__IO uint32_t*)0x4001C71CU) /**< \brief (TWI4) Transmit Next Counter Register */
#define REG_TWI4_PTCR  (*(__O  uint32_t*)0x4001C720U) /**< \brief (TWI4) Transfer Control Register */
#define REG_TWI4_PTSR  (*(__I  uint32_t*)0x4001C724U) /**< \brief (TWI4) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI4_INSTANCE_ */
