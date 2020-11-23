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

#ifndef _PIC32CX_USART6_INSTANCE_
#define _PIC32CX_USART6_INSTANCE_

/* ========== Register definition for USART6 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART6_CR                        (0x40018200U) /**< \brief (USART6) USART Control Register */
  #define REG_USART6_MR                        (0x40018204U) /**< \brief (USART6) USART Mode Register */
  #define REG_USART6_IER                       (0x40018208U) /**< \brief (USART6) USART Interrupt Enable Register */
  #define REG_USART6_IDR                       (0x4001820CU) /**< \brief (USART6) USART Interrupt Disable Register */
  #define REG_USART6_IMR                       (0x40018210U) /**< \brief (USART6) USART Interrupt Mask Register */
  #define REG_USART6_CSR                       (0x40018214U) /**< \brief (USART6) USART Channel Status Register */
  #define REG_USART6_RHR                       (0x40018218U) /**< \brief (USART6) USART Receive Holding Register */
  #define REG_USART6_THR                       (0x4001821CU) /**< \brief (USART6) USART Transmit Holding Register */
  #define REG_USART6_BRGR                      (0x40018220U) /**< \brief (USART6) USART Baud Rate Generator Register */
  #define REG_USART6_RTOR                      (0x40018224U) /**< \brief (USART6) USART Receiver Timeout Register */
  #define REG_USART6_TTGR                      (0x40018228U) /**< \brief (USART6) USART Transmitter Timeguard Register */
  #define REG_USART6_FIDI                      (0x40018240U) /**< \brief (USART6) USART FI DI Ratio Register */
  #define REG_USART6_NER                       (0x40018244U) /**< \brief (USART6) USART Number of Errors Register */
  #define REG_USART6_IF                        (0x4001824CU) /**< \brief (USART6) USART IrDA Filter Register */
  #define REG_USART6_MAN                       (0x40018250U) /**< \brief (USART6) USART Manchester Configuration Register */
  #define REG_USART6_LINMR                     (0x40018254U) /**< \brief (USART6) USART LIN Mode Register */
  #define REG_USART6_LINIR                     (0x40018258U) /**< \brief (USART6) USART LIN Identifier Register */
  #define REG_USART6_LINBRR                    (0x4001825CU) /**< \brief (USART6) USART LIN Baud Rate Register */
  #define REG_USART6_CMPR                      (0x40018290U) /**< \brief (USART6) USART Comparison Register */
  #define REG_USART6_FMR                       (0x400182A0U) /**< \brief (USART6) USART FIFO Mode Register */
  #define REG_USART6_FLR                       (0x400182A4U) /**< \brief (USART6) USART FIFO Level Register */
  #define REG_USART6_FIER                      (0x400182A8U) /**< \brief (USART6) USART FIFO Interrupt Enable Register */
  #define REG_USART6_FIDR                      (0x400182ACU) /**< \brief (USART6) USART FIFO Interrupt Disable Register */
  #define REG_USART6_FIMR                      (0x400182B0U) /**< \brief (USART6) USART FIFO Interrupt Mask Register */
  #define REG_USART6_FESR                      (0x400182B4U) /**< \brief (USART6) USART FIFO Event Status Register */
  #define REG_USART6_WPMR                      (0x400182E4U) /**< \brief (USART6) USART Write Protection Mode Register */
  #define REG_USART6_WPSR                      (0x400182E8U) /**< \brief (USART6) USART Write Protection Status Register */
  #define REG_USART6_RPR                       (0x40018300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART6_RCR                       (0x40018304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART6_TPR                       (0x40018308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART6_TCR                       (0x4001830CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART6_RNPR                      (0x40018310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART6_RNCR                      (0x40018314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART6_TNPR                      (0x40018318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART6_TNCR                      (0x4001831CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART6_PTCR                      (0x40018320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART6_PTSR                      (0x40018324U) /**< \brief (USART0) Transfer Status Register */
#else
  #define REG_USART6_CR       (*(__O  uint32_t*)0x40018200U) /**< \brief (USART6) USART Control Register */
  #define REG_USART6_MR       (*(__IO uint32_t*)0x40018204U) /**< \brief (USART6) USART Mode Register */
  #define REG_USART6_IER      (*(__O  uint32_t*)0x40018208U) /**< \brief (USART6) USART Interrupt Enable Register */
  #define REG_USART6_IDR      (*(__O  uint32_t*)0x4001820CU) /**< \brief (USART6) USART Interrupt Disable Register */
  #define REG_USART6_IMR      (*(__I  uint32_t*)0x40018210U) /**< \brief (USART6) USART Interrupt Mask Register */
  #define REG_USART6_CSR      (*(__I  uint32_t*)0x40018214U) /**< \brief (USART6) USART Channel Status Register */
  #define REG_USART6_RHR      (*(__I  uint32_t*)0x40018218U) /**< \brief (USART6) USART Receive Holding Register */
  #define REG_USART6_THR      (*(__O  uint32_t*)0x4001821CU) /**< \brief (USART6) USART Transmit Holding Register */
  #define REG_USART6_BRGR     (*(__IO uint32_t*)0x40018220U) /**< \brief (USART6) USART Baud Rate Generator Register */
  #define REG_USART6_RTOR     (*(__IO uint32_t*)0x40018224U) /**< \brief (USART6) USART Receiver Timeout Register */
  #define REG_USART6_TTGR     (*(__IO uint32_t*)0x40018228U) /**< \brief (USART6) USART Transmitter Timeguard Register */
  #define REG_USART6_FIDI     (*(__IO uint32_t*)0x40018240U) /**< \brief (USART6) USART FI DI Ratio Register */
  #define REG_USART6_NER      (*(__I  uint32_t*)0x40018244U) /**< \brief (USART6) USART Number of Errors Register */
  #define REG_USART6_IF       (*(__IO uint32_t*)0x4001824CU) /**< \brief (USART6) USART IrDA Filter Register */
  #define REG_USART6_MAN      (*(__IO uint32_t*)0x40018250U) /**< \brief (USART6) USART Manchester Configuration Register */
  #define REG_USART6_LINMR    (*(__IO uint32_t*)0x40018254U) /**< \brief (USART6) USART LIN Mode Register */
  #define REG_USART6_LINIR    (*(__IO uint32_t*)0x40018258U) /**< \brief (USART6) USART LIN Identifier Register */
  #define REG_USART6_LINBRR   (*(__I  uint32_t*)0x4001825CU) /**< \brief (USART6) USART LIN Baud Rate Register */
  #define REG_USART6_CMPR     (*(__IO uint32_t*)0x40018290U) /**< \brief (USART6) USART Comparison Register */
  #define REG_USART6_FMR      (*(__IO uint32_t*)0x400182A0U) /**< \brief (USART6) USART FIFO Mode Register */
  #define REG_USART6_FLR      (*(__I  uint32_t*)0x400182A4U) /**< \brief (USART6) USART FIFO Level Register */
  #define REG_USART6_FIER     (*(__O  uint32_t*)0x400182A8U) /**< \brief (USART6) USART FIFO Interrupt Enable Register */
  #define REG_USART6_FIDR     (*(__O  uint32_t*)0x400182ACU) /**< \brief (USART6) USART FIFO Interrupt Disable Register */
  #define REG_USART6_FIMR     (*(__I  uint32_t*)0x400182B0U) /**< \brief (USART6) USART FIFO Interrupt Mask Register */
  #define REG_USART6_FESR     (*(__I  uint32_t*)0x400182B4U) /**< \brief (USART6) USART FIFO Event Status Register */
  #define REG_USART6_WPMR     (*(__IO uint32_t*)0x400182E4U) /**< \brief (USART6) USART Write Protection Mode Register */
  #define REG_USART6_WPSR     (*(__I  uint32_t*)0x400182E8U) /**< \brief (USART6) USART Write Protection Status Register */
  #define REG_USART6_RPR      (*(__IO uint32_t*)0x40018300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART6_RCR      (*(__IO uint32_t*)0x40018304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART6_TPR      (*(__IO uint32_t*)0x40018308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART6_TCR      (*(__IO uint32_t*)0x4001830CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART6_RNPR     (*(__IO uint32_t*)0x40018310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART6_RNCR     (*(__IO uint32_t*)0x40018314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART6_TNPR     (*(__IO uint32_t*)0x40018318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART6_TNCR     (*(__IO uint32_t*)0x4001831CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART6_PTCR     (*(__O  uint32_t*)0x40018320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART6_PTSR     (*(__I  uint32_t*)0x40018324U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_USART6_INSTANCE_ */
