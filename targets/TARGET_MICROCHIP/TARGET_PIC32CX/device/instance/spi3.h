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

#ifndef _PIC32CX_SPI3_INSTANCE_
#define _PIC32CX_SPI3_INSTANCE_

/* ========== Register definition for SPI3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI3_CR                       (0x4000C400U) /**< \brief (SPI3) Control Register */
  #define REG_SPI3_MR                       (0x4000C404U) /**< \brief (SPI3) Mode Register */
  #define REG_SPI3_RDR                      (0x4000C408U) /**< \brief (SPI3) Receive Data Register */
  #define REG_SPI3_TDR                      (0x4000C40CU) /**< \brief (SPI3) Transmit Data Register */
  #define REG_SPI3_SR                       (0x4000C410U) /**< \brief (SPI3) Status Register */
  #define REG_SPI3_IER                      (0x4000C414U) /**< \brief (SPI3) Interrupt Enable Register */
  #define REG_SPI3_IDR                      (0x4000C418U) /**< \brief (SPI3) Interrupt Disable Register */
  #define REG_SPI3_IMR                      (0x4000C41CU) /**< \brief (SPI3) Interrupt Mask Register */
  #define REG_SPI3_CSR                      (0x4000C430U) /**< \brief (SPI3) Chip Select Register */
  #define REG_SPI3_FMR                      (0x4000C440U) /**< \brief (SPI3) FIFO Mode Register */
  #define REG_SPI3_FLR                      (0x4000C444U) /**< \brief (SPI3) FIFO Level Register */
  #define REG_SPI3_CMPR                     (0x4000C448U) /**< \brief (SPI3) Comparison Register */
  #define REG_SPI3_WPMR                     (0x4000C4E4U) /**< \brief (SPI3) Write Protection Mode Register */
  #define REG_SPI3_WPSR                     (0x4000C4E8U) /**< \brief (SPI3) Write Protection Status Register */
  #define REG_SPI3_RPR                      (0x4000C500U) /**< \brief (SPI3) Receive Pointer Register */
  #define REG_SPI3_RCR                      (0x4000C504U) /**< \brief (SPI3) Receive Counter Register */
  #define REG_SPI3_TPR                      (0x4000C508U) /**< \brief (SPI3) Transmit Pointer Register */
  #define REG_SPI3_TCR                      (0x4000C50CU) /**< \brief (SPI3) Transmit Counter Register */
  #define REG_SPI3_RNPR                     (0x4000C510U) /**< \brief (SPI3) Receive Next Pointer Register */
  #define REG_SPI3_RNCR                     (0x4000C514U) /**< \brief (SPI3) Receive Next Counter Register */
  #define REG_SPI3_TNPR                     (0x4000C518U) /**< \brief (SPI3) Transmit Next Pointer Register */
  #define REG_SPI3_TNCR                     (0x4000C51CU) /**< \brief (SPI3) Transmit Next Counter Register */
  #define REG_SPI3_PTCR                     (0x4000C520U) /**< \brief (SPI3) Transfer Control Register */
  #define REG_SPI3_PTSR                     (0x4000C524U) /**< \brief (SPI3) Transfer Status Register */
#else
  #define REG_SPI3_CR      (*(__O  uint32_t*)0x4000C400U) /**< \brief (SPI3) Control Register */
  #define REG_SPI3_MR      (*(__IO uint32_t*)0x4000C404U) /**< \brief (SPI3) Mode Register */
  #define REG_SPI3_RDR     (*(__I  uint32_t*)0x4000C408U) /**< \brief (SPI3) Receive Data Register */
  #define REG_SPI3_TDR     (*(__O  uint32_t*)0x4000C40CU) /**< \brief (SPI3) Transmit Data Register */
  #define REG_SPI3_SR      (*(__I  uint32_t*)0x4000C410U) /**< \brief (SPI3) Status Register */
  #define REG_SPI3_IER     (*(__O  uint32_t*)0x4000C414U) /**< \brief (SPI3) Interrupt Enable Register */
  #define REG_SPI3_IDR     (*(__O  uint32_t*)0x4000C418U) /**< \brief (SPI3) Interrupt Disable Register */
  #define REG_SPI3_IMR     (*(__I  uint32_t*)0x4000C41CU) /**< \brief (SPI3) Interrupt Mask Register */
  #define REG_SPI3_CSR     (*(__IO uint32_t*)0x4000C430U) /**< \brief (SPI3) Chip Select Register */
  #define REG_SPI3_FMR     (*(__IO uint32_t*)0x4000C440U) /**< \brief (SPI3) FIFO Mode Register */
  #define REG_SPI3_FLR     (*(__I  uint32_t*)0x4000C444U) /**< \brief (SPI3) FIFO Level Register */
  #define REG_SPI3_CMPR    (*(__IO uint32_t*)0x4000C448U) /**< \brief (SPI3) Comparison Register */
  #define REG_SPI3_WPMR    (*(__IO uint32_t*)0x4000C4E4U) /**< \brief (SPI3) Write Protection Mode Register */
  #define REG_SPI3_WPSR    (*(__I  uint32_t*)0x4000C4E8U) /**< \brief (SPI3) Write Protection Status Register */
  #define REG_SPI3_RPR     (*(__IO uint32_t*)0x4000C500U) /**< \brief (SPI3) Receive Pointer Register */
  #define REG_SPI3_RCR     (*(__IO uint32_t*)0x4000C504U) /**< \brief (SPI3) Receive Counter Register */
  #define REG_SPI3_TPR     (*(__IO uint32_t*)0x4000C508U) /**< \brief (SPI3) Transmit Pointer Register */
  #define REG_SPI3_TCR     (*(__IO uint32_t*)0x4000C50CU) /**< \brief (SPI3) Transmit Counter Register */
  #define REG_SPI3_RNPR    (*(__IO uint32_t*)0x4000C510U) /**< \brief (SPI3) Receive Next Pointer Register */
  #define REG_SPI3_RNCR    (*(__IO uint32_t*)0x4000C514U) /**< \brief (SPI3) Receive Next Counter Register */
  #define REG_SPI3_TNPR    (*(__IO uint32_t*)0x4000C518U) /**< \brief (SPI3) Transmit Next Pointer Register */
  #define REG_SPI3_TNCR    (*(__IO uint32_t*)0x4000C51CU) /**< \brief (SPI3) Transmit Next Counter Register */
  #define REG_SPI3_PTCR    (*(__O  uint32_t*)0x4000C520U) /**< \brief (SPI3) Transfer Control Register */
  #define REG_SPI3_PTSR    (*(__I  uint32_t*)0x4000C524U) /**< \brief (SPI3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SPI3_INSTANCE_ */
