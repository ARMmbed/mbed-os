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

#ifndef _SAMG55_USART7_INSTANCE_
#define _SAMG55_USART7_INSTANCE_

/* ========== Register definition for USART7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART7_CR                      (0x40034200U) /**< \brief (USART7) USART Control Register */
#define REG_USART7_MR                      (0x40034204U) /**< \brief (USART7) USART Mode Register */
#define REG_USART7_IER                     (0x40034208U) /**< \brief (USART7) USART Interrupt Enable Register */
#define REG_USART7_IDR                     (0x4003420CU) /**< \brief (USART7) USART Interrupt Disable Register */
#define REG_USART7_IMR                     (0x40034210U) /**< \brief (USART7) USART Interrupt Mask Register */
#define REG_USART7_CSR                     (0x40034214U) /**< \brief (USART7) USART Channel Status Register */
#define REG_USART7_RHR                     (0x40034218U) /**< \brief (USART7) USART Receive Holding Register */
#define REG_USART7_THR                     (0x4003421CU) /**< \brief (USART7) USART Transmit Holding Register */
#define REG_USART7_BRGR                    (0x40034220U) /**< \brief (USART7) USART Baud Rate Generator Register */
#define REG_USART7_RTOR                    (0x40034224U) /**< \brief (USART7) USART Receiver Time-out Register */
#define REG_USART7_TTGR                    (0x40034228U) /**< \brief (USART7) USART Transmitter Timeguard Register */
#define REG_USART7_FIDI                    (0x40034240U) /**< \brief (USART7) USART FI DI Ratio Register */
#define REG_USART7_NER                     (0x40034244U) /**< \brief (USART7) USART Number of Errors Register */
#define REG_USART7_LINMR                   (0x40034254U) /**< \brief (USART7) USART LIN Mode Register */
#define REG_USART7_LINIR                   (0x40034258U) /**< \brief (USART7) USART LIN Identifier Register */
#define REG_USART7_LINBRR                  (0x4003425CU) /**< \brief (USART7) USART LIN Baud Rate Register */
#define REG_USART7_CMPR                    (0x40034290U) /**< \brief (USART7) USART Comparison Register */
#define REG_USART7_WPMR                    (0x400342E4U) /**< \brief (USART7) USART Write Protection Mode Register */
#define REG_USART7_WPSR                    (0x400342E8U) /**< \brief (USART7) USART Write Protection Status Register */
#define REG_USART7_RPR                     (0x40034300U) /**< \brief (USART7) Receive Pointer Register */
#define REG_USART7_RCR                     (0x40034304U) /**< \brief (USART7) Receive Counter Register */
#define REG_USART7_TPR                     (0x40034308U) /**< \brief (USART7) Transmit Pointer Register */
#define REG_USART7_TCR                     (0x4003430CU) /**< \brief (USART7) Transmit Counter Register */
#define REG_USART7_RNPR                    (0x40034310U) /**< \brief (USART7) Receive Next Pointer Register */
#define REG_USART7_RNCR                    (0x40034314U) /**< \brief (USART7) Receive Next Counter Register */
#define REG_USART7_TNPR                    (0x40034318U) /**< \brief (USART7) Transmit Next Pointer Register */
#define REG_USART7_TNCR                    (0x4003431CU) /**< \brief (USART7) Transmit Next Counter Register */
#define REG_USART7_PTCR                    (0x40034320U) /**< \brief (USART7) Transfer Control Register */
#define REG_USART7_PTSR                    (0x40034324U) /**< \brief (USART7) Transfer Status Register */
#else
#define REG_USART7_CR     (*(__O  uint32_t*)0x40034200U) /**< \brief (USART7) USART Control Register */
#define REG_USART7_MR     (*(__IO uint32_t*)0x40034204U) /**< \brief (USART7) USART Mode Register */
#define REG_USART7_IER    (*(__O  uint32_t*)0x40034208U) /**< \brief (USART7) USART Interrupt Enable Register */
#define REG_USART7_IDR    (*(__O  uint32_t*)0x4003420CU) /**< \brief (USART7) USART Interrupt Disable Register */
#define REG_USART7_IMR    (*(__I  uint32_t*)0x40034210U) /**< \brief (USART7) USART Interrupt Mask Register */
#define REG_USART7_CSR    (*(__I  uint32_t*)0x40034214U) /**< \brief (USART7) USART Channel Status Register */
#define REG_USART7_RHR    (*(__I  uint32_t*)0x40034218U) /**< \brief (USART7) USART Receive Holding Register */
#define REG_USART7_THR    (*(__O  uint32_t*)0x4003421CU) /**< \brief (USART7) USART Transmit Holding Register */
#define REG_USART7_BRGR   (*(__IO uint32_t*)0x40034220U) /**< \brief (USART7) USART Baud Rate Generator Register */
#define REG_USART7_RTOR   (*(__IO uint32_t*)0x40034224U) /**< \brief (USART7) USART Receiver Time-out Register */
#define REG_USART7_TTGR   (*(__IO uint32_t*)0x40034228U) /**< \brief (USART7) USART Transmitter Timeguard Register */
#define REG_USART7_FIDI   (*(__IO uint32_t*)0x40034240U) /**< \brief (USART7) USART FI DI Ratio Register */
#define REG_USART7_NER    (*(__I  uint32_t*)0x40034244U) /**< \brief (USART7) USART Number of Errors Register */
#define REG_USART7_LINMR  (*(__IO uint32_t*)0x40034254U) /**< \brief (USART7) USART LIN Mode Register */
#define REG_USART7_LINIR  (*(__IO uint32_t*)0x40034258U) /**< \brief (USART7) USART LIN Identifier Register */
#define REG_USART7_LINBRR (*(__I  uint32_t*)0x4003425CU) /**< \brief (USART7) USART LIN Baud Rate Register */
#define REG_USART7_CMPR   (*(__IO uint32_t*)0x40034290U) /**< \brief (USART7) USART Comparison Register */
#define REG_USART7_WPMR   (*(__IO uint32_t*)0x400342E4U) /**< \brief (USART7) USART Write Protection Mode Register */
#define REG_USART7_WPSR   (*(__I  uint32_t*)0x400342E8U) /**< \brief (USART7) USART Write Protection Status Register */
#define REG_USART7_RPR    (*(__IO uint32_t*)0x40034300U) /**< \brief (USART7) Receive Pointer Register */
#define REG_USART7_RCR    (*(__IO uint32_t*)0x40034304U) /**< \brief (USART7) Receive Counter Register */
#define REG_USART7_TPR    (*(__IO uint32_t*)0x40034308U) /**< \brief (USART7) Transmit Pointer Register */
#define REG_USART7_TCR    (*(__IO uint32_t*)0x4003430CU) /**< \brief (USART7) Transmit Counter Register */
#define REG_USART7_RNPR   (*(__IO uint32_t*)0x40034310U) /**< \brief (USART7) Receive Next Pointer Register */
#define REG_USART7_RNCR   (*(__IO uint32_t*)0x40034314U) /**< \brief (USART7) Receive Next Counter Register */
#define REG_USART7_TNPR   (*(__IO uint32_t*)0x40034318U) /**< \brief (USART7) Transmit Next Pointer Register */
#define REG_USART7_TNCR   (*(__IO uint32_t*)0x4003431CU) /**< \brief (USART7) Transmit Next Counter Register */
#define REG_USART7_PTCR   (*(__O  uint32_t*)0x40034320U) /**< \brief (USART7) Transfer Control Register */
#define REG_USART7_PTSR   (*(__I  uint32_t*)0x40034324U) /**< \brief (USART7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART7_INSTANCE_ */
