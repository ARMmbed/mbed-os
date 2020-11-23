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

#ifndef _PIC32CX_USART2_INSTANCE_
#define _PIC32CX_USART2_INSTANCE_

/* ========== Register definition for USART2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART2_CR                        (0x40008200U) /**< \brief (USART2) USART Control Register */
  #define REG_USART2_MR                        (0x40008204U) /**< \brief (USART2) USART Mode Register */
  #define REG_USART2_IER                       (0x40008208U) /**< \brief (USART2) USART Interrupt Enable Register */
  #define REG_USART2_IDR                       (0x4000820CU) /**< \brief (USART2) USART Interrupt Disable Register */
  #define REG_USART2_IMR                       (0x40008210U) /**< \brief (USART2) USART Interrupt Mask Register */
  #define REG_USART2_CSR                       (0x40008214U) /**< \brief (USART2) USART Channel Status Register */
  #define REG_USART2_RHR                       (0x40008218U) /**< \brief (USART2) USART Receive Holding Register */
  #define REG_USART2_THR                       (0x4000821CU) /**< \brief (USART2) USART Transmit Holding Register */
  #define REG_USART2_BRGR                      (0x40008220U) /**< \brief (USART2) USART Baud Rate Generator Register */
  #define REG_USART2_RTOR                      (0x40008224U) /**< \brief (USART2) USART Receiver Timeout Register */
  #define REG_USART2_TTGR                      (0x40008228U) /**< \brief (USART2) USART Transmitter Timeguard Register */
  #define REG_USART2_FIDI                      (0x40008240U) /**< \brief (USART2) USART FI DI Ratio Register */
  #define REG_USART2_NER                       (0x40008244U) /**< \brief (USART2) USART Number of Errors Register */
  #define REG_USART2_IF                        (0x4000824CU) /**< \brief (USART2) USART IrDA Filter Register */
  #define REG_USART2_MAN                       (0x40008250U) /**< \brief (USART2) USART Manchester Configuration Register */
  #define REG_USART2_LINMR                     (0x40008254U) /**< \brief (USART2) USART LIN Mode Register */
  #define REG_USART2_LINIR                     (0x40008258U) /**< \brief (USART2) USART LIN Identifier Register */
  #define REG_USART2_LINBRR                    (0x4000825CU) /**< \brief (USART2) USART LIN Baud Rate Register */
  #define REG_USART2_CMPR                      (0x40008290U) /**< \brief (USART2) USART Comparison Register */
  #define REG_USART2_FMR                       (0x400082A0U) /**< \brief (USART2) USART FIFO Mode Register */
  #define REG_USART2_FLR                       (0x400082A4U) /**< \brief (USART2) USART FIFO Level Register */
  #define REG_USART2_FIER                      (0x400082A8U) /**< \brief (USART2) USART FIFO Interrupt Enable Register */
  #define REG_USART2_FIDR                      (0x400082ACU) /**< \brief (USART2) USART FIFO Interrupt Disable Register */
  #define REG_USART2_FIMR                      (0x400082B0U) /**< \brief (USART2) USART FIFO Interrupt Mask Register */
  #define REG_USART2_FESR                      (0x400082B4U) /**< \brief (USART2) USART FIFO Event Status Register */
  #define REG_USART2_WPMR                      (0x400082E4U) /**< \brief (USART2) USART Write Protection Mode Register */
  #define REG_USART2_WPSR                      (0x400082E8U) /**< \brief (USART2) USART Write Protection Status Register */
  #define REG_USART2_RPR                       (0x40008300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART2_RCR                       (0x40008304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART2_TPR                       (0x40008308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART2_TCR                       (0x4000830CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART2_RNPR                      (0x40008310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART2_RNCR                      (0x40008314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART2_TNPR                      (0x40008318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART2_TNCR                      (0x4000831CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART2_PTCR                      (0x40008320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART2_PTSR                      (0x40008324U) /**< \brief (USART0) Transfer Status Register */
#else
  #define REG_USART2_CR       (*(__O  uint32_t*)0x40008200U) /**< \brief (USART2) USART Control Register */
  #define REG_USART2_MR       (*(__IO uint32_t*)0x40008204U) /**< \brief (USART2) USART Mode Register */
  #define REG_USART2_IER      (*(__O  uint32_t*)0x40008208U) /**< \brief (USART2) USART Interrupt Enable Register */
  #define REG_USART2_IDR      (*(__O  uint32_t*)0x4000820CU) /**< \brief (USART2) USART Interrupt Disable Register */
  #define REG_USART2_IMR      (*(__I  uint32_t*)0x40008210U) /**< \brief (USART2) USART Interrupt Mask Register */
  #define REG_USART2_CSR      (*(__I  uint32_t*)0x40008214U) /**< \brief (USART2) USART Channel Status Register */
  #define REG_USART2_RHR      (*(__I  uint32_t*)0x40008218U) /**< \brief (USART2) USART Receive Holding Register */
  #define REG_USART2_THR      (*(__O  uint32_t*)0x4000821CU) /**< \brief (USART2) USART Transmit Holding Register */
  #define REG_USART2_BRGR     (*(__IO uint32_t*)0x40008220U) /**< \brief (USART2) USART Baud Rate Generator Register */
  #define REG_USART2_RTOR     (*(__IO uint32_t*)0x40008224U) /**< \brief (USART2) USART Receiver Timeout Register */
  #define REG_USART2_TTGR     (*(__IO uint32_t*)0x40008228U) /**< \brief (USART2) USART Transmitter Timeguard Register */
  #define REG_USART2_FIDI     (*(__IO uint32_t*)0x40008240U) /**< \brief (USART2) USART FI DI Ratio Register */
  #define REG_USART2_NER      (*(__I  uint32_t*)0x40008244U) /**< \brief (USART2) USART Number of Errors Register */
  #define REG_USART2_IF       (*(__IO uint32_t*)0x4000824CU) /**< \brief (USART2) USART IrDA Filter Register */
  #define REG_USART2_MAN      (*(__IO uint32_t*)0x40008250U) /**< \brief (USART2) USART Manchester Configuration Register */
  #define REG_USART2_LINMR    (*(__IO uint32_t*)0x40008254U) /**< \brief (USART2) USART LIN Mode Register */
  #define REG_USART2_LINIR    (*(__IO uint32_t*)0x40008258U) /**< \brief (USART2) USART LIN Identifier Register */
  #define REG_USART2_LINBRR   (*(__I  uint32_t*)0x4000825CU) /**< \brief (USART2) USART LIN Baud Rate Register */
  #define REG_USART2_CMPR     (*(__IO uint32_t*)0x40008290U) /**< \brief (USART2) USART Comparison Register */
  #define REG_USART2_FMR      (*(__IO uint32_t*)0x400082A0U) /**< \brief (USART2) USART FIFO Mode Register */
  #define REG_USART2_FLR      (*(__I  uint32_t*)0x400082A4U) /**< \brief (USART2) USART FIFO Level Register */
  #define REG_USART2_FIER     (*(__O  uint32_t*)0x400082A8U) /**< \brief (USART2) USART FIFO Interrupt Enable Register */
  #define REG_USART2_FIDR     (*(__O  uint32_t*)0x400082ACU) /**< \brief (USART2) USART FIFO Interrupt Disable Register */
  #define REG_USART2_FIMR     (*(__I  uint32_t*)0x400082B0U) /**< \brief (USART2) USART FIFO Interrupt Mask Register */
  #define REG_USART2_FESR     (*(__I  uint32_t*)0x400082B4U) /**< \brief (USART2) USART FIFO Event Status Register */
  #define REG_USART2_WPMR     (*(__IO uint32_t*)0x400082E4U) /**< \brief (USART2) USART Write Protection Mode Register */
  #define REG_USART2_WPSR     (*(__I  uint32_t*)0x400082E8U) /**< \brief (USART2) USART Write Protection Status Register */
  #define REG_USART2_RPR      (*(__IO uint32_t*)0x40008300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART2_RCR      (*(__IO uint32_t*)0x40008304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART2_TPR      (*(__IO uint32_t*)0x40008308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART2_TCR      (*(__IO uint32_t*)0x4000830CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART2_RNPR     (*(__IO uint32_t*)0x40008310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART2_RNCR     (*(__IO uint32_t*)0x40008314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART2_TNPR     (*(__IO uint32_t*)0x40008318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART2_TNCR     (*(__IO uint32_t*)0x4000831CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART2_PTCR     (*(__O  uint32_t*)0x40008320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART2_PTSR     (*(__I  uint32_t*)0x40008324U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_USART2_INSTANCE_ */
