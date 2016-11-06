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

#ifndef _SAMG55_USART3_INSTANCE_
#define _SAMG55_USART3_INSTANCE_

/* ========== Register definition for USART3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART3_CR                      (0x40018200U) /**< \brief (USART3) USART Control Register */
#define REG_USART3_MR                      (0x40018204U) /**< \brief (USART3) USART Mode Register */
#define REG_USART3_IER                     (0x40018208U) /**< \brief (USART3) USART Interrupt Enable Register */
#define REG_USART3_IDR                     (0x4001820CU) /**< \brief (USART3) USART Interrupt Disable Register */
#define REG_USART3_IMR                     (0x40018210U) /**< \brief (USART3) USART Interrupt Mask Register */
#define REG_USART3_CSR                     (0x40018214U) /**< \brief (USART3) USART Channel Status Register */
#define REG_USART3_RHR                     (0x40018218U) /**< \brief (USART3) USART Receive Holding Register */
#define REG_USART3_THR                     (0x4001821CU) /**< \brief (USART3) USART Transmit Holding Register */
#define REG_USART3_BRGR                    (0x40018220U) /**< \brief (USART3) USART Baud Rate Generator Register */
#define REG_USART3_RTOR                    (0x40018224U) /**< \brief (USART3) USART Receiver Time-out Register */
#define REG_USART3_TTGR                    (0x40018228U) /**< \brief (USART3) USART Transmitter Timeguard Register */
#define REG_USART3_FIDI                    (0x40018240U) /**< \brief (USART3) USART FI DI Ratio Register */
#define REG_USART3_NER                     (0x40018244U) /**< \brief (USART3) USART Number of Errors Register */
#define REG_USART3_LINMR                   (0x40018254U) /**< \brief (USART3) USART LIN Mode Register */
#define REG_USART3_LINIR                   (0x40018258U) /**< \brief (USART3) USART LIN Identifier Register */
#define REG_USART3_LINBRR                  (0x4001825CU) /**< \brief (USART3) USART LIN Baud Rate Register */
#define REG_USART3_CMPR                    (0x40018290U) /**< \brief (USART3) USART Comparison Register */
#define REG_USART3_WPMR                    (0x400182E4U) /**< \brief (USART3) USART Write Protection Mode Register */
#define REG_USART3_WPSR                    (0x400182E8U) /**< \brief (USART3) USART Write Protection Status Register */
#define REG_USART3_RPR                     (0x40018300U) /**< \brief (USART3) Receive Pointer Register */
#define REG_USART3_RCR                     (0x40018304U) /**< \brief (USART3) Receive Counter Register */
#define REG_USART3_TPR                     (0x40018308U) /**< \brief (USART3) Transmit Pointer Register */
#define REG_USART3_TCR                     (0x4001830CU) /**< \brief (USART3) Transmit Counter Register */
#define REG_USART3_RNPR                    (0x40018310U) /**< \brief (USART3) Receive Next Pointer Register */
#define REG_USART3_RNCR                    (0x40018314U) /**< \brief (USART3) Receive Next Counter Register */
#define REG_USART3_TNPR                    (0x40018318U) /**< \brief (USART3) Transmit Next Pointer Register */
#define REG_USART3_TNCR                    (0x4001831CU) /**< \brief (USART3) Transmit Next Counter Register */
#define REG_USART3_PTCR                    (0x40018320U) /**< \brief (USART3) Transfer Control Register */
#define REG_USART3_PTSR                    (0x40018324U) /**< \brief (USART3) Transfer Status Register */
#else
#define REG_USART3_CR     (*(__O  uint32_t*)0x40018200U) /**< \brief (USART3) USART Control Register */
#define REG_USART3_MR     (*(__IO uint32_t*)0x40018204U) /**< \brief (USART3) USART Mode Register */
#define REG_USART3_IER    (*(__O  uint32_t*)0x40018208U) /**< \brief (USART3) USART Interrupt Enable Register */
#define REG_USART3_IDR    (*(__O  uint32_t*)0x4001820CU) /**< \brief (USART3) USART Interrupt Disable Register */
#define REG_USART3_IMR    (*(__I  uint32_t*)0x40018210U) /**< \brief (USART3) USART Interrupt Mask Register */
#define REG_USART3_CSR    (*(__I  uint32_t*)0x40018214U) /**< \brief (USART3) USART Channel Status Register */
#define REG_USART3_RHR    (*(__I  uint32_t*)0x40018218U) /**< \brief (USART3) USART Receive Holding Register */
#define REG_USART3_THR    (*(__O  uint32_t*)0x4001821CU) /**< \brief (USART3) USART Transmit Holding Register */
#define REG_USART3_BRGR   (*(__IO uint32_t*)0x40018220U) /**< \brief (USART3) USART Baud Rate Generator Register */
#define REG_USART3_RTOR   (*(__IO uint32_t*)0x40018224U) /**< \brief (USART3) USART Receiver Time-out Register */
#define REG_USART3_TTGR   (*(__IO uint32_t*)0x40018228U) /**< \brief (USART3) USART Transmitter Timeguard Register */
#define REG_USART3_FIDI   (*(__IO uint32_t*)0x40018240U) /**< \brief (USART3) USART FI DI Ratio Register */
#define REG_USART3_NER    (*(__I  uint32_t*)0x40018244U) /**< \brief (USART3) USART Number of Errors Register */
#define REG_USART3_LINMR  (*(__IO uint32_t*)0x40018254U) /**< \brief (USART3) USART LIN Mode Register */
#define REG_USART3_LINIR  (*(__IO uint32_t*)0x40018258U) /**< \brief (USART3) USART LIN Identifier Register */
#define REG_USART3_LINBRR (*(__I  uint32_t*)0x4001825CU) /**< \brief (USART3) USART LIN Baud Rate Register */
#define REG_USART3_CMPR   (*(__IO uint32_t*)0x40018290U) /**< \brief (USART3) USART Comparison Register */
#define REG_USART3_WPMR   (*(__IO uint32_t*)0x400182E4U) /**< \brief (USART3) USART Write Protection Mode Register */
#define REG_USART3_WPSR   (*(__I  uint32_t*)0x400182E8U) /**< \brief (USART3) USART Write Protection Status Register */
#define REG_USART3_RPR    (*(__IO uint32_t*)0x40018300U) /**< \brief (USART3) Receive Pointer Register */
#define REG_USART3_RCR    (*(__IO uint32_t*)0x40018304U) /**< \brief (USART3) Receive Counter Register */
#define REG_USART3_TPR    (*(__IO uint32_t*)0x40018308U) /**< \brief (USART3) Transmit Pointer Register */
#define REG_USART3_TCR    (*(__IO uint32_t*)0x4001830CU) /**< \brief (USART3) Transmit Counter Register */
#define REG_USART3_RNPR   (*(__IO uint32_t*)0x40018310U) /**< \brief (USART3) Receive Next Pointer Register */
#define REG_USART3_RNCR   (*(__IO uint32_t*)0x40018314U) /**< \brief (USART3) Receive Next Counter Register */
#define REG_USART3_TNPR   (*(__IO uint32_t*)0x40018318U) /**< \brief (USART3) Transmit Next Pointer Register */
#define REG_USART3_TNCR   (*(__IO uint32_t*)0x4001831CU) /**< \brief (USART3) Transmit Next Counter Register */
#define REG_USART3_PTCR   (*(__O  uint32_t*)0x40018320U) /**< \brief (USART3) Transfer Control Register */
#define REG_USART3_PTSR   (*(__I  uint32_t*)0x40018324U) /**< \brief (USART3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART3_INSTANCE_ */
