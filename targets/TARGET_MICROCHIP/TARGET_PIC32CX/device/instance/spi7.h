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

#ifndef _PIC32CX_SPI7_INSTANCE_
#define _PIC32CX_SPI7_INSTANCE_

/* ========== Register definition for SPI7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI7_CR                       (0x4001C400U) /**< \brief (SPI7) Control Register */
  #define REG_SPI7_MR                       (0x4001C404U) /**< \brief (SPI7) Mode Register */
  #define REG_SPI7_RDR                      (0x4001C408U) /**< \brief (SPI7) Receive Data Register */
  #define REG_SPI7_TDR                      (0x4001C40CU) /**< \brief (SPI7) Transmit Data Register */
  #define REG_SPI7_SR                       (0x4001C410U) /**< \brief (SPI7) Status Register */
  #define REG_SPI7_IER                      (0x4001C414U) /**< \brief (SPI7) Interrupt Enable Register */
  #define REG_SPI7_IDR                      (0x4001C418U) /**< \brief (SPI7) Interrupt Disable Register */
  #define REG_SPI7_IMR                      (0x4001C41CU) /**< \brief (SPI7) Interrupt Mask Register */
  #define REG_SPI7_CSR                      (0x4001C430U) /**< \brief (SPI7) Chip Select Register */
  #define REG_SPI7_FMR                      (0x4001C440U) /**< \brief (SPI7) FIFO Mode Register */
  #define REG_SPI7_FLR                      (0x4001C444U) /**< \brief (SPI7) FIFO Level Register */
  #define REG_SPI7_CMPR                     (0x4001C448U) /**< \brief (SPI7) Comparison Register */
  #define REG_SPI7_WPMR                     (0x4001C4E4U) /**< \brief (SPI7) Write Protection Mode Register */
  #define REG_SPI7_WPSR                     (0x4001C4E8U) /**< \brief (SPI7) Write Protection Status Register */
  #define REG_SPI7_RPR                      (0x4001C500U) /**< \brief (SPI7) Receive Pointer Register */
  #define REG_SPI7_RCR                      (0x4001C504U) /**< \brief (SPI7) Receive Counter Register */
  #define REG_SPI7_TPR                      (0x4001C508U) /**< \brief (SPI7) Transmit Pointer Register */
  #define REG_SPI7_TCR                      (0x4001C50CU) /**< \brief (SPI7) Transmit Counter Register */
  #define REG_SPI7_RNPR                     (0x4001C510U) /**< \brief (SPI7) Receive Next Pointer Register */
  #define REG_SPI7_RNCR                     (0x4001C514U) /**< \brief (SPI7) Receive Next Counter Register */
  #define REG_SPI7_TNPR                     (0x4001C518U) /**< \brief (SPI7) Transmit Next Pointer Register */
  #define REG_SPI7_TNCR                     (0x4001C51CU) /**< \brief (SPI7) Transmit Next Counter Register */
  #define REG_SPI7_PTCR                     (0x4001C520U) /**< \brief (SPI7) Transfer Control Register */
  #define REG_SPI7_PTSR                     (0x4001C524U) /**< \brief (SPI7) Transfer Status Register */
#else
  #define REG_SPI7_CR      (*(__O  uint32_t*)0x4001C400U) /**< \brief (SPI7) Control Register */
  #define REG_SPI7_MR      (*(__IO uint32_t*)0x4001C404U) /**< \brief (SPI7) Mode Register */
  #define REG_SPI7_RDR     (*(__I  uint32_t*)0x4001C408U) /**< \brief (SPI7) Receive Data Register */
  #define REG_SPI7_TDR     (*(__O  uint32_t*)0x4001C40CU) /**< \brief (SPI7) Transmit Data Register */
  #define REG_SPI7_SR      (*(__I  uint32_t*)0x4001C410U) /**< \brief (SPI7) Status Register */
  #define REG_SPI7_IER     (*(__O  uint32_t*)0x4001C414U) /**< \brief (SPI7) Interrupt Enable Register */
  #define REG_SPI7_IDR     (*(__O  uint32_t*)0x4001C418U) /**< \brief (SPI7) Interrupt Disable Register */
  #define REG_SPI7_IMR     (*(__I  uint32_t*)0x4001C41CU) /**< \brief (SPI7) Interrupt Mask Register */
  #define REG_SPI7_CSR     (*(__IO uint32_t*)0x4001C430U) /**< \brief (SPI7) Chip Select Register */
  #define REG_SPI7_FMR     (*(__IO uint32_t*)0x4001C440U) /**< \brief (SPI7) FIFO Mode Register */
  #define REG_SPI7_FLR     (*(__I  uint32_t*)0x4001C444U) /**< \brief (SPI7) FIFO Level Register */
  #define REG_SPI7_CMPR    (*(__IO uint32_t*)0x4001C448U) /**< \brief (SPI7) Comparison Register */
  #define REG_SPI7_WPMR    (*(__IO uint32_t*)0x4001C4E4U) /**< \brief (SPI7) Write Protection Mode Register */
  #define REG_SPI7_WPSR    (*(__I  uint32_t*)0x4001C4E8U) /**< \brief (SPI7) Write Protection Status Register */
  #define REG_SPI7_RPR     (*(__IO uint32_t*)0x4001C500U) /**< \brief (SPI7) Receive Pointer Register */
  #define REG_SPI7_RCR     (*(__IO uint32_t*)0x4001C504U) /**< \brief (SPI7) Receive Counter Register */
  #define REG_SPI7_TPR     (*(__IO uint32_t*)0x4001C508U) /**< \brief (SPI7) Transmit Pointer Register */
  #define REG_SPI7_TCR     (*(__IO uint32_t*)0x4001C50CU) /**< \brief (SPI7) Transmit Counter Register */
  #define REG_SPI7_RNPR    (*(__IO uint32_t*)0x4001C510U) /**< \brief (SPI7) Receive Next Pointer Register */
  #define REG_SPI7_RNCR    (*(__IO uint32_t*)0x4001C514U) /**< \brief (SPI7) Receive Next Counter Register */
  #define REG_SPI7_TNPR    (*(__IO uint32_t*)0x4001C518U) /**< \brief (SPI7) Transmit Next Pointer Register */
  #define REG_SPI7_TNCR    (*(__IO uint32_t*)0x4001C51CU) /**< \brief (SPI7) Transmit Next Counter Register */
  #define REG_SPI7_PTCR    (*(__O  uint32_t*)0x4001C520U) /**< \brief (SPI7) Transfer Control Register */
  #define REG_SPI7_PTSR    (*(__I  uint32_t*)0x4001C524U) /**< \brief (SPI7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SPI7_INSTANCE_ */
