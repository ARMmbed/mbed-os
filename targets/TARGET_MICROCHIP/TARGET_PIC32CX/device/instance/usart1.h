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

#ifndef _PIC32CX_USART1_INSTANCE_
#define _PIC32CX_USART1_INSTANCE_

/* ========== Register definition for USART1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART1_CR                        (0x40004200U) /**< \brief (USART1) USART Control Register */
  #define REG_USART1_MR                        (0x40004204U) /**< \brief (USART1) USART Mode Register */
  #define REG_USART1_IER                       (0x40004208U) /**< \brief (USART1) USART Interrupt Enable Register */
  #define REG_USART1_IDR                       (0x4000420CU) /**< \brief (USART1) USART Interrupt Disable Register */
  #define REG_USART1_IMR                       (0x40004210U) /**< \brief (USART1) USART Interrupt Mask Register */
  #define REG_USART1_CSR                       (0x40004214U) /**< \brief (USART1) USART Channel Status Register */
  #define REG_USART1_RHR                       (0x40004218U) /**< \brief (USART1) USART Receive Holding Register */
  #define REG_USART1_THR                       (0x4000421CU) /**< \brief (USART1) USART Transmit Holding Register */
  #define REG_USART1_BRGR                      (0x40004220U) /**< \brief (USART1) USART Baud Rate Generator Register */
  #define REG_USART1_RTOR                      (0x40004224U) /**< \brief (USART1) USART Receiver Timeout Register */
  #define REG_USART1_TTGR                      (0x40004228U) /**< \brief (USART1) USART Transmitter Timeguard Register */
  #define REG_USART1_FIDI                      (0x40004240U) /**< \brief (USART1) USART FI DI Ratio Register */
  #define REG_USART1_NER                       (0x40004244U) /**< \brief (USART1) USART Number of Errors Register */
  #define REG_USART1_IF                        (0x4000424CU) /**< \brief (USART1) USART IrDA Filter Register */
  #define REG_USART1_MAN                       (0x40004250U) /**< \brief (USART1) USART Manchester Configuration Register */
  #define REG_USART1_LINMR                     (0x40004254U) /**< \brief (USART1) USART LIN Mode Register */
  #define REG_USART1_LINIR                     (0x40004258U) /**< \brief (USART1) USART LIN Identifier Register */
  #define REG_USART1_LINBRR                    (0x4000425CU) /**< \brief (USART1) USART LIN Baud Rate Register */
  #define REG_USART1_CMPR                      (0x40004290U) /**< \brief (USART1) USART Comparison Register */
  #define REG_USART1_FMR                       (0x400042A0U) /**< \brief (USART1) USART FIFO Mode Register */
  #define REG_USART1_FLR                       (0x400042A4U) /**< \brief (USART1) USART FIFO Level Register */
  #define REG_USART1_FIER                      (0x400042A8U) /**< \brief (USART1) USART FIFO Interrupt Enable Register */
  #define REG_USART1_FIDR                      (0x400042ACU) /**< \brief (USART1) USART FIFO Interrupt Disable Register */
  #define REG_USART1_FIMR                      (0x400042B0U) /**< \brief (USART1) USART FIFO Interrupt Mask Register */
  #define REG_USART1_FESR                      (0x400042B4U) /**< \brief (USART1) USART FIFO Event Status Register */
  #define REG_USART1_WPMR                      (0x400042E4U) /**< \brief (USART1) USART Write Protection Mode Register */
  #define REG_USART1_WPSR                      (0x400042E8U) /**< \brief (USART1) USART Write Protection Status Register */
  #define REG_USART1_RPR                       (0x40004300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART1_RCR                       (0x40004304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART1_TPR                       (0x40004308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART1_TCR                       (0x4000430CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART1_RNPR                      (0x40004310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART1_RNCR                      (0x40004314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART1_TNPR                      (0x40004318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART1_TNCR                      (0x4000431CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART1_PTCR                      (0x40004320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART1_PTSR                      (0x40004324U) /**< \brief (USART0) Transfer Status Register */
#else
  #define REG_USART1_CR       (*(__O  uint32_t*)0x40004200U) /**< \brief (USART1) USART Control Register */
  #define REG_USART1_MR       (*(__IO uint32_t*)0x40004204U) /**< \brief (USART1) USART Mode Register */
  #define REG_USART1_IER      (*(__O  uint32_t*)0x40004208U) /**< \brief (USART1) USART Interrupt Enable Register */
  #define REG_USART1_IDR      (*(__O  uint32_t*)0x4000420CU) /**< \brief (USART1) USART Interrupt Disable Register */
  #define REG_USART1_IMR      (*(__I  uint32_t*)0x40004210U) /**< \brief (USART1) USART Interrupt Mask Register */
  #define REG_USART1_CSR      (*(__I  uint32_t*)0x40004214U) /**< \brief (USART1) USART Channel Status Register */
  #define REG_USART1_RHR      (*(__I  uint32_t*)0x40004218U) /**< \brief (USART1) USART Receive Holding Register */
  #define REG_USART1_THR      (*(__O  uint32_t*)0x4000421CU) /**< \brief (USART1) USART Transmit Holding Register */
  #define REG_USART1_BRGR     (*(__IO uint32_t*)0x40004220U) /**< \brief (USART1) USART Baud Rate Generator Register */
  #define REG_USART1_RTOR     (*(__IO uint32_t*)0x40004224U) /**< \brief (USART1) USART Receiver Timeout Register */
  #define REG_USART1_TTGR     (*(__IO uint32_t*)0x40004228U) /**< \brief (USART1) USART Transmitter Timeguard Register */
  #define REG_USART1_FIDI     (*(__IO uint32_t*)0x40004240U) /**< \brief (USART1) USART FI DI Ratio Register */
  #define REG_USART1_NER      (*(__I  uint32_t*)0x40004244U) /**< \brief (USART1) USART Number of Errors Register */
  #define REG_USART1_IF       (*(__IO uint32_t*)0x4000424CU) /**< \brief (USART1) USART IrDA Filter Register */
  #define REG_USART1_MAN      (*(__IO uint32_t*)0x40004250U) /**< \brief (USART1) USART Manchester Configuration Register */
  #define REG_USART1_LINMR    (*(__IO uint32_t*)0x40004254U) /**< \brief (USART1) USART LIN Mode Register */
  #define REG_USART1_LINIR    (*(__IO uint32_t*)0x40004258U) /**< \brief (USART1) USART LIN Identifier Register */
  #define REG_USART1_LINBRR   (*(__I  uint32_t*)0x4000425CU) /**< \brief (USART1) USART LIN Baud Rate Register */
  #define REG_USART1_CMPR     (*(__IO uint32_t*)0x40004290U) /**< \brief (USART1) USART Comparison Register */
  #define REG_USART1_FMR      (*(__IO uint32_t*)0x400042A0U) /**< \brief (USART1) USART FIFO Mode Register */
  #define REG_USART1_FLR      (*(__I  uint32_t*)0x400042A4U) /**< \brief (USART1) USART FIFO Level Register */
  #define REG_USART1_FIER     (*(__O  uint32_t*)0x400042A8U) /**< \brief (USART1) USART FIFO Interrupt Enable Register */
  #define REG_USART1_FIDR     (*(__O  uint32_t*)0x400042ACU) /**< \brief (USART1) USART FIFO Interrupt Disable Register */
  #define REG_USART1_FIMR     (*(__I  uint32_t*)0x400042B0U) /**< \brief (USART1) USART FIFO Interrupt Mask Register */
  #define REG_USART1_FESR     (*(__I  uint32_t*)0x400042B4U) /**< \brief (USART1) USART FIFO Event Status Register */
  #define REG_USART1_WPMR     (*(__IO uint32_t*)0x400042E4U) /**< \brief (USART1) USART Write Protection Mode Register */
  #define REG_USART1_WPSR     (*(__I  uint32_t*)0x400042E8U) /**< \brief (USART1) USART Write Protection Status Register */
  #define REG_USART1_RPR      (*(__IO uint32_t*)0x40004300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART1_RCR      (*(__IO uint32_t*)0x40004304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART1_TPR      (*(__IO uint32_t*)0x40004308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART1_TCR      (*(__IO uint32_t*)0x4000430CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART1_RNPR     (*(__IO uint32_t*)0x40004310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART1_RNCR     (*(__IO uint32_t*)0x40004314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART1_TNPR     (*(__IO uint32_t*)0x40004318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART1_TNCR     (*(__IO uint32_t*)0x4000431CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART1_PTCR     (*(__O  uint32_t*)0x40004320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART1_PTSR     (*(__I  uint32_t*)0x40004324U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_USART1_INSTANCE_ */
