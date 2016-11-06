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

#ifndef _SAMG55_USART6_INSTANCE_
#define _SAMG55_USART6_INSTANCE_

/* ========== Register definition for USART6 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART6_CR                      (0x40040200U) /**< \brief (USART6) USART Control Register */
#define REG_USART6_MR                      (0x40040204U) /**< \brief (USART6) USART Mode Register */
#define REG_USART6_IER                     (0x40040208U) /**< \brief (USART6) USART Interrupt Enable Register */
#define REG_USART6_IDR                     (0x4004020CU) /**< \brief (USART6) USART Interrupt Disable Register */
#define REG_USART6_IMR                     (0x40040210U) /**< \brief (USART6) USART Interrupt Mask Register */
#define REG_USART6_CSR                     (0x40040214U) /**< \brief (USART6) USART Channel Status Register */
#define REG_USART6_RHR                     (0x40040218U) /**< \brief (USART6) USART Receive Holding Register */
#define REG_USART6_THR                     (0x4004021CU) /**< \brief (USART6) USART Transmit Holding Register */
#define REG_USART6_BRGR                    (0x40040220U) /**< \brief (USART6) USART Baud Rate Generator Register */
#define REG_USART6_RTOR                    (0x40040224U) /**< \brief (USART6) USART Receiver Time-out Register */
#define REG_USART6_TTGR                    (0x40040228U) /**< \brief (USART6) USART Transmitter Timeguard Register */
#define REG_USART6_FIDI                    (0x40040240U) /**< \brief (USART6) USART FI DI Ratio Register */
#define REG_USART6_NER                     (0x40040244U) /**< \brief (USART6) USART Number of Errors Register */
#define REG_USART6_LINMR                   (0x40040254U) /**< \brief (USART6) USART LIN Mode Register */
#define REG_USART6_LINIR                   (0x40040258U) /**< \brief (USART6) USART LIN Identifier Register */
#define REG_USART6_LINBRR                  (0x4004025CU) /**< \brief (USART6) USART LIN Baud Rate Register */
#define REG_USART6_CMPR                    (0x40040290U) /**< \brief (USART6) USART Comparison Register */
#define REG_USART6_WPMR                    (0x400402E4U) /**< \brief (USART6) USART Write Protection Mode Register */
#define REG_USART6_WPSR                    (0x400402E8U) /**< \brief (USART6) USART Write Protection Status Register */
#define REG_USART6_RPR                     (0x40040300U) /**< \brief (USART6) Receive Pointer Register */
#define REG_USART6_RCR                     (0x40040304U) /**< \brief (USART6) Receive Counter Register */
#define REG_USART6_TPR                     (0x40040308U) /**< \brief (USART6) Transmit Pointer Register */
#define REG_USART6_TCR                     (0x4004030CU) /**< \brief (USART6) Transmit Counter Register */
#define REG_USART6_RNPR                    (0x40040310U) /**< \brief (USART6) Receive Next Pointer Register */
#define REG_USART6_RNCR                    (0x40040314U) /**< \brief (USART6) Receive Next Counter Register */
#define REG_USART6_TNPR                    (0x40040318U) /**< \brief (USART6) Transmit Next Pointer Register */
#define REG_USART6_TNCR                    (0x4004031CU) /**< \brief (USART6) Transmit Next Counter Register */
#define REG_USART6_PTCR                    (0x40040320U) /**< \brief (USART6) Transfer Control Register */
#define REG_USART6_PTSR                    (0x40040324U) /**< \brief (USART6) Transfer Status Register */
#else
#define REG_USART6_CR     (*(__O  uint32_t*)0x40040200U) /**< \brief (USART6) USART Control Register */
#define REG_USART6_MR     (*(__IO uint32_t*)0x40040204U) /**< \brief (USART6) USART Mode Register */
#define REG_USART6_IER    (*(__O  uint32_t*)0x40040208U) /**< \brief (USART6) USART Interrupt Enable Register */
#define REG_USART6_IDR    (*(__O  uint32_t*)0x4004020CU) /**< \brief (USART6) USART Interrupt Disable Register */
#define REG_USART6_IMR    (*(__I  uint32_t*)0x40040210U) /**< \brief (USART6) USART Interrupt Mask Register */
#define REG_USART6_CSR    (*(__I  uint32_t*)0x40040214U) /**< \brief (USART6) USART Channel Status Register */
#define REG_USART6_RHR    (*(__I  uint32_t*)0x40040218U) /**< \brief (USART6) USART Receive Holding Register */
#define REG_USART6_THR    (*(__O  uint32_t*)0x4004021CU) /**< \brief (USART6) USART Transmit Holding Register */
#define REG_USART6_BRGR   (*(__IO uint32_t*)0x40040220U) /**< \brief (USART6) USART Baud Rate Generator Register */
#define REG_USART6_RTOR   (*(__IO uint32_t*)0x40040224U) /**< \brief (USART6) USART Receiver Time-out Register */
#define REG_USART6_TTGR   (*(__IO uint32_t*)0x40040228U) /**< \brief (USART6) USART Transmitter Timeguard Register */
#define REG_USART6_FIDI   (*(__IO uint32_t*)0x40040240U) /**< \brief (USART6) USART FI DI Ratio Register */
#define REG_USART6_NER    (*(__I  uint32_t*)0x40040244U) /**< \brief (USART6) USART Number of Errors Register */
#define REG_USART6_LINMR  (*(__IO uint32_t*)0x40040254U) /**< \brief (USART6) USART LIN Mode Register */
#define REG_USART6_LINIR  (*(__IO uint32_t*)0x40040258U) /**< \brief (USART6) USART LIN Identifier Register */
#define REG_USART6_LINBRR (*(__I  uint32_t*)0x4004025CU) /**< \brief (USART6) USART LIN Baud Rate Register */
#define REG_USART6_CMPR   (*(__IO uint32_t*)0x40040290U) /**< \brief (USART6) USART Comparison Register */
#define REG_USART6_WPMR   (*(__IO uint32_t*)0x400402E4U) /**< \brief (USART6) USART Write Protection Mode Register */
#define REG_USART6_WPSR   (*(__I  uint32_t*)0x400402E8U) /**< \brief (USART6) USART Write Protection Status Register */
#define REG_USART6_RPR    (*(__IO uint32_t*)0x40040300U) /**< \brief (USART6) Receive Pointer Register */
#define REG_USART6_RCR    (*(__IO uint32_t*)0x40040304U) /**< \brief (USART6) Receive Counter Register */
#define REG_USART6_TPR    (*(__IO uint32_t*)0x40040308U) /**< \brief (USART6) Transmit Pointer Register */
#define REG_USART6_TCR    (*(__IO uint32_t*)0x4004030CU) /**< \brief (USART6) Transmit Counter Register */
#define REG_USART6_RNPR   (*(__IO uint32_t*)0x40040310U) /**< \brief (USART6) Receive Next Pointer Register */
#define REG_USART6_RNCR   (*(__IO uint32_t*)0x40040314U) /**< \brief (USART6) Receive Next Counter Register */
#define REG_USART6_TNPR   (*(__IO uint32_t*)0x40040318U) /**< \brief (USART6) Transmit Next Pointer Register */
#define REG_USART6_TNCR   (*(__IO uint32_t*)0x4004031CU) /**< \brief (USART6) Transmit Next Counter Register */
#define REG_USART6_PTCR   (*(__O  uint32_t*)0x40040320U) /**< \brief (USART6) Transfer Control Register */
#define REG_USART6_PTSR   (*(__I  uint32_t*)0x40040324U) /**< \brief (USART6) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART6_INSTANCE_ */
