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

#ifndef _SAMG55_USART0_INSTANCE_
#define _SAMG55_USART0_INSTANCE_

/* ========== Register definition for USART0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART0_CR                      (0x4000C200U) /**< \brief (USART0) USART Control Register */
#define REG_USART0_MR                      (0x4000C204U) /**< \brief (USART0) USART Mode Register */
#define REG_USART0_IER                     (0x4000C208U) /**< \brief (USART0) USART Interrupt Enable Register */
#define REG_USART0_IDR                     (0x4000C20CU) /**< \brief (USART0) USART Interrupt Disable Register */
#define REG_USART0_IMR                     (0x4000C210U) /**< \brief (USART0) USART Interrupt Mask Register */
#define REG_USART0_CSR                     (0x4000C214U) /**< \brief (USART0) USART Channel Status Register */
#define REG_USART0_RHR                     (0x4000C218U) /**< \brief (USART0) USART Receive Holding Register */
#define REG_USART0_THR                     (0x4000C21CU) /**< \brief (USART0) USART Transmit Holding Register */
#define REG_USART0_BRGR                    (0x4000C220U) /**< \brief (USART0) USART Baud Rate Generator Register */
#define REG_USART0_RTOR                    (0x4000C224U) /**< \brief (USART0) USART Receiver Time-out Register */
#define REG_USART0_TTGR                    (0x4000C228U) /**< \brief (USART0) USART Transmitter Timeguard Register */
#define REG_USART0_FIDI                    (0x4000C240U) /**< \brief (USART0) USART FI DI Ratio Register */
#define REG_USART0_NER                     (0x4000C244U) /**< \brief (USART0) USART Number of Errors Register */
#define REG_USART0_LINMR                   (0x4000C254U) /**< \brief (USART0) USART LIN Mode Register */
#define REG_USART0_LINIR                   (0x4000C258U) /**< \brief (USART0) USART LIN Identifier Register */
#define REG_USART0_LINBRR                  (0x4000C25CU) /**< \brief (USART0) USART LIN Baud Rate Register */
#define REG_USART0_CMPR                    (0x4000C290U) /**< \brief (USART0) USART Comparison Register */
#define REG_USART0_WPMR                    (0x4000C2E4U) /**< \brief (USART0) USART Write Protection Mode Register */
#define REG_USART0_WPSR                    (0x4000C2E8U) /**< \brief (USART0) USART Write Protection Status Register */
#define REG_USART0_RPR                     (0x4000C300U) /**< \brief (USART0) Receive Pointer Register */
#define REG_USART0_RCR                     (0x4000C304U) /**< \brief (USART0) Receive Counter Register */
#define REG_USART0_TPR                     (0x4000C308U) /**< \brief (USART0) Transmit Pointer Register */
#define REG_USART0_TCR                     (0x4000C30CU) /**< \brief (USART0) Transmit Counter Register */
#define REG_USART0_RNPR                    (0x4000C310U) /**< \brief (USART0) Receive Next Pointer Register */
#define REG_USART0_RNCR                    (0x4000C314U) /**< \brief (USART0) Receive Next Counter Register */
#define REG_USART0_TNPR                    (0x4000C318U) /**< \brief (USART0) Transmit Next Pointer Register */
#define REG_USART0_TNCR                    (0x4000C31CU) /**< \brief (USART0) Transmit Next Counter Register */
#define REG_USART0_PTCR                    (0x4000C320U) /**< \brief (USART0) Transfer Control Register */
#define REG_USART0_PTSR                    (0x4000C324U) /**< \brief (USART0) Transfer Status Register */
#else
#define REG_USART0_CR     (*(__O  uint32_t*)0x4000C200U) /**< \brief (USART0) USART Control Register */
#define REG_USART0_MR     (*(__IO uint32_t*)0x4000C204U) /**< \brief (USART0) USART Mode Register */
#define REG_USART0_IER    (*(__O  uint32_t*)0x4000C208U) /**< \brief (USART0) USART Interrupt Enable Register */
#define REG_USART0_IDR    (*(__O  uint32_t*)0x4000C20CU) /**< \brief (USART0) USART Interrupt Disable Register */
#define REG_USART0_IMR    (*(__I  uint32_t*)0x4000C210U) /**< \brief (USART0) USART Interrupt Mask Register */
#define REG_USART0_CSR    (*(__I  uint32_t*)0x4000C214U) /**< \brief (USART0) USART Channel Status Register */
#define REG_USART0_RHR    (*(__I  uint32_t*)0x4000C218U) /**< \brief (USART0) USART Receive Holding Register */
#define REG_USART0_THR    (*(__O  uint32_t*)0x4000C21CU) /**< \brief (USART0) USART Transmit Holding Register */
#define REG_USART0_BRGR   (*(__IO uint32_t*)0x4000C220U) /**< \brief (USART0) USART Baud Rate Generator Register */
#define REG_USART0_RTOR   (*(__IO uint32_t*)0x4000C224U) /**< \brief (USART0) USART Receiver Time-out Register */
#define REG_USART0_TTGR   (*(__IO uint32_t*)0x4000C228U) /**< \brief (USART0) USART Transmitter Timeguard Register */
#define REG_USART0_FIDI   (*(__IO uint32_t*)0x4000C240U) /**< \brief (USART0) USART FI DI Ratio Register */
#define REG_USART0_NER    (*(__I  uint32_t*)0x4000C244U) /**< \brief (USART0) USART Number of Errors Register */
#define REG_USART0_LINMR  (*(__IO uint32_t*)0x4000C254U) /**< \brief (USART0) USART LIN Mode Register */
#define REG_USART0_LINIR  (*(__IO uint32_t*)0x4000C258U) /**< \brief (USART0) USART LIN Identifier Register */
#define REG_USART0_LINBRR (*(__I  uint32_t*)0x4000C25CU) /**< \brief (USART0) USART LIN Baud Rate Register */
#define REG_USART0_CMPR   (*(__IO uint32_t*)0x4000C290U) /**< \brief (USART0) USART Comparison Register */
#define REG_USART0_WPMR   (*(__IO uint32_t*)0x4000C2E4U) /**< \brief (USART0) USART Write Protection Mode Register */
#define REG_USART0_WPSR   (*(__I  uint32_t*)0x4000C2E8U) /**< \brief (USART0) USART Write Protection Status Register */
#define REG_USART0_RPR    (*(__IO uint32_t*)0x4000C300U) /**< \brief (USART0) Receive Pointer Register */
#define REG_USART0_RCR    (*(__IO uint32_t*)0x4000C304U) /**< \brief (USART0) Receive Counter Register */
#define REG_USART0_TPR    (*(__IO uint32_t*)0x4000C308U) /**< \brief (USART0) Transmit Pointer Register */
#define REG_USART0_TCR    (*(__IO uint32_t*)0x4000C30CU) /**< \brief (USART0) Transmit Counter Register */
#define REG_USART0_RNPR   (*(__IO uint32_t*)0x4000C310U) /**< \brief (USART0) Receive Next Pointer Register */
#define REG_USART0_RNCR   (*(__IO uint32_t*)0x4000C314U) /**< \brief (USART0) Receive Next Counter Register */
#define REG_USART0_TNPR   (*(__IO uint32_t*)0x4000C318U) /**< \brief (USART0) Transmit Next Pointer Register */
#define REG_USART0_TNCR   (*(__IO uint32_t*)0x4000C31CU) /**< \brief (USART0) Transmit Next Counter Register */
#define REG_USART0_PTCR   (*(__O  uint32_t*)0x4000C320U) /**< \brief (USART0) Transfer Control Register */
#define REG_USART0_PTSR   (*(__I  uint32_t*)0x4000C324U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART0_INSTANCE_ */
