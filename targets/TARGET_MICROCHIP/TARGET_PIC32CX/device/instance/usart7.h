/**
 * \file
 *
 * Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _PIC32CX_USART7_INSTANCE_
#define _PIC32CX_USART7_INSTANCE_

/* ========== Register definition for USART7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART7_CR                        (0x4001C200U) /**< \brief (USART7) USART Control Register */
  #define REG_USART7_MR                        (0x4001C204U) /**< \brief (USART7) USART Mode Register */
  #define REG_USART7_IER                       (0x4001C208U) /**< \brief (USART7) USART Interrupt Enable Register */
  #define REG_USART7_IDR                       (0x4001C20CU) /**< \brief (USART7) USART Interrupt Disable Register */
  #define REG_USART7_IMR                       (0x4001C210U) /**< \brief (USART7) USART Interrupt Mask Register */
  #define REG_USART7_CSR                       (0x4001C214U) /**< \brief (USART7) USART Channel Status Register */
  #define REG_USART7_RHR                       (0x4001C218U) /**< \brief (USART7) USART Receive Holding Register */
  #define REG_USART7_THR                       (0x4001C21CU) /**< \brief (USART7) USART Transmit Holding Register */
  #define REG_USART7_BRGR                      (0x4001C220U) /**< \brief (USART7) USART Baud Rate Generator Register */
  #define REG_USART7_RTOR                      (0x4001C224U) /**< \brief (USART7) USART Receiver Timeout Register */
  #define REG_USART7_TTGR                      (0x4001C228U) /**< \brief (USART7) USART Transmitter Timeguard Register */
  #define REG_USART7_FIDI                      (0x4001C240U) /**< \brief (USART7) USART FI DI Ratio Register */
  #define REG_USART7_NER                       (0x4001C244U) /**< \brief (USART7) USART Number of Errors Register */
  #define REG_USART7_IF                        (0x4001C24CU) /**< \brief (USART7) USART IrDA Filter Register */
  #define REG_USART7_MAN                       (0x4001C250U) /**< \brief (USART7) USART Manchester Configuration Register */
  #define REG_USART7_LINMR                     (0x4001C254U) /**< \brief (USART7) USART LIN Mode Register */
  #define REG_USART7_LINIR                     (0x4001C258U) /**< \brief (USART7) USART LIN Identifier Register */
  #define REG_USART7_LINBRR                    (0x4001C25CU) /**< \brief (USART7) USART LIN Baud Rate Register */
  #define REG_USART7_CMPR                      (0x4001C290U) /**< \brief (USART7) USART Comparison Register */
  #define REG_USART7_FMR                       (0x4001C2A0U) /**< \brief (USART7) USART FIFO Mode Register */
  #define REG_USART7_FLR                       (0x4001C2A4U) /**< \brief (USART7) USART FIFO Level Register */
  #define REG_USART7_FIER                      (0x4001C2A8U) /**< \brief (USART7) USART FIFO Interrupt Enable Register */
  #define REG_USART7_FIDR                      (0x4001C2ACU) /**< \brief (USART7) USART FIFO Interrupt Disable Register */
  #define REG_USART7_FIMR                      (0x4001C2B0U) /**< \brief (USART7) USART FIFO Interrupt Mask Register */
  #define REG_USART7_FESR                      (0x4001C2B4U) /**< \brief (USART7) USART FIFO Event Status Register */
  #define REG_USART7_WPMR                      (0x4001C2E4U) /**< \brief (USART7) USART Write Protection Mode Register */
  #define REG_USART7_WPSR                      (0x4001C2E8U) /**< \brief (USART7) USART Write Protection Status Register */
  #define REG_USART7_RPR                       (0x4001C300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART7_RCR                       (0x4001C304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART7_TPR                       (0x4001C308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART7_TCR                       (0x4001C30CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART7_RNPR                      (0x4001C310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART7_RNCR                      (0x4001C314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART7_TNPR                      (0x4001C318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART7_TNCR                      (0x4001C31CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART7_PTCR                      (0x4001C320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART7_PTSR                      (0x4001C324U) /**< \brief (USART0) Transfer Status Register */
#else
  #define REG_USART7_CR       (*(__O  uint32_t*)0x4001C200U) /**< \brief (USART7) USART Control Register */
  #define REG_USART7_MR       (*(__IO uint32_t*)0x4001C204U) /**< \brief (USART7) USART Mode Register */
  #define REG_USART7_IER      (*(__O  uint32_t*)0x4001C208U) /**< \brief (USART7) USART Interrupt Enable Register */
  #define REG_USART7_IDR      (*(__O  uint32_t*)0x4001C20CU) /**< \brief (USART7) USART Interrupt Disable Register */
  #define REG_USART7_IMR      (*(__I  uint32_t*)0x4001C210U) /**< \brief (USART7) USART Interrupt Mask Register */
  #define REG_USART7_CSR      (*(__I  uint32_t*)0x4001C214U) /**< \brief (USART7) USART Channel Status Register */
  #define REG_USART7_RHR      (*(__I  uint32_t*)0x4001C218U) /**< \brief (USART7) USART Receive Holding Register */
  #define REG_USART7_THR      (*(__O  uint32_t*)0x4001C21CU) /**< \brief (USART7) USART Transmit Holding Register */
  #define REG_USART7_BRGR     (*(__IO uint32_t*)0x4001C220U) /**< \brief (USART7) USART Baud Rate Generator Register */
  #define REG_USART7_RTOR     (*(__IO uint32_t*)0x4001C224U) /**< \brief (USART7) USART Receiver Timeout Register */
  #define REG_USART7_TTGR     (*(__IO uint32_t*)0x4001C228U) /**< \brief (USART7) USART Transmitter Timeguard Register */
  #define REG_USART7_FIDI     (*(__IO uint32_t*)0x4001C240U) /**< \brief (USART7) USART FI DI Ratio Register */
  #define REG_USART7_NER      (*(__I  uint32_t*)0x4001C244U) /**< \brief (USART7) USART Number of Errors Register */
  #define REG_USART7_IF       (*(__IO uint32_t*)0x4001C24CU) /**< \brief (USART7) USART IrDA Filter Register */
  #define REG_USART7_MAN      (*(__IO uint32_t*)0x4001C250U) /**< \brief (USART7) USART Manchester Configuration Register */
  #define REG_USART7_LINMR    (*(__IO uint32_t*)0x4001C254U) /**< \brief (USART7) USART LIN Mode Register */
  #define REG_USART7_LINIR    (*(__IO uint32_t*)0x4001C258U) /**< \brief (USART7) USART LIN Identifier Register */
  #define REG_USART7_LINBRR   (*(__I  uint32_t*)0x4001C25CU) /**< \brief (USART7) USART LIN Baud Rate Register */
  #define REG_USART7_CMPR     (*(__IO uint32_t*)0x4001C290U) /**< \brief (USART7) USART Comparison Register */
  #define REG_USART7_FMR      (*(__IO uint32_t*)0x4001C2A0U) /**< \brief (USART7) USART FIFO Mode Register */
  #define REG_USART7_FLR      (*(__I  uint32_t*)0x4001C2A4U) /**< \brief (USART7) USART FIFO Level Register */
  #define REG_USART7_FIER     (*(__O  uint32_t*)0x4001C2A8U) /**< \brief (USART7) USART FIFO Interrupt Enable Register */
  #define REG_USART7_FIDR     (*(__O  uint32_t*)0x4001C2ACU) /**< \brief (USART7) USART FIFO Interrupt Disable Register */
  #define REG_USART7_FIMR     (*(__I  uint32_t*)0x4001C2B0U) /**< \brief (USART7) USART FIFO Interrupt Mask Register */
  #define REG_USART7_FESR     (*(__I  uint32_t*)0x4001C2B4U) /**< \brief (USART7) USART FIFO Event Status Register */
  #define REG_USART7_WPMR     (*(__IO uint32_t*)0x4001C2E4U) /**< \brief (USART7) USART Write Protection Mode Register */
  #define REG_USART7_WPSR     (*(__I  uint32_t*)0x4001C2E8U) /**< \brief (USART7) USART Write Protection Status Register */
  #define REG_USART7_RPR      (*(__IO uint32_t*)0x4001C300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART7_RCR      (*(__IO uint32_t*)0x4001C304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART7_TPR      (*(__IO uint32_t*)0x4001C308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART7_TCR      (*(__IO uint32_t*)0x4001C30CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART7_RNPR     (*(__IO uint32_t*)0x4001C310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART7_RNCR     (*(__IO uint32_t*)0x4001C314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART7_TNPR     (*(__IO uint32_t*)0x4001C318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART7_TNCR     (*(__IO uint32_t*)0x4001C31CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART7_PTCR     (*(__O  uint32_t*)0x4001C320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART7_PTSR     (*(__I  uint32_t*)0x4001C324U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_USART7_INSTANCE_ */
