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

#ifndef _PIC32CX_SPI2_INSTANCE_
#define _PIC32CX_SPI2_INSTANCE_

/* ========== Register definition for SPI2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI2_CR                       (0x40008400U) /**< \brief (SPI2) Control Register */
  #define REG_SPI2_MR                       (0x40008404U) /**< \brief (SPI2) Mode Register */
  #define REG_SPI2_RDR                      (0x40008408U) /**< \brief (SPI2) Receive Data Register */
  #define REG_SPI2_TDR                      (0x4000840CU) /**< \brief (SPI2) Transmit Data Register */
  #define REG_SPI2_SR                       (0x40008410U) /**< \brief (SPI2) Status Register */
  #define REG_SPI2_IER                      (0x40008414U) /**< \brief (SPI2) Interrupt Enable Register */
  #define REG_SPI2_IDR                      (0x40008418U) /**< \brief (SPI2) Interrupt Disable Register */
  #define REG_SPI2_IMR                      (0x4000841CU) /**< \brief (SPI2) Interrupt Mask Register */
  #define REG_SPI2_CSR                      (0x40008430U) /**< \brief (SPI2) Chip Select Register */
  #define REG_SPI2_FMR                      (0x40008440U) /**< \brief (SPI2) FIFO Mode Register */
  #define REG_SPI2_FLR                      (0x40008444U) /**< \brief (SPI2) FIFO Level Register */
  #define REG_SPI2_CMPR                     (0x40008448U) /**< \brief (SPI2) Comparison Register */
  #define REG_SPI2_WPMR                     (0x400084E4U) /**< \brief (SPI2) Write Protection Mode Register */
  #define REG_SPI2_WPSR                     (0x400084E8U) /**< \brief (SPI2) Write Protection Status Register */
  #define REG_SPI2_RPR                      (0x40008500U) /**< \brief (SPI2) Receive Pointer Register */
  #define REG_SPI2_RCR                      (0x40008504U) /**< \brief (SPI2) Receive Counter Register */
  #define REG_SPI2_TPR                      (0x40008508U) /**< \brief (SPI2) Transmit Pointer Register */
  #define REG_SPI2_TCR                      (0x4000850CU) /**< \brief (SPI2) Transmit Counter Register */
  #define REG_SPI2_RNPR                     (0x40008510U) /**< \brief (SPI2) Receive Next Pointer Register */
  #define REG_SPI2_RNCR                     (0x40008514U) /**< \brief (SPI2) Receive Next Counter Register */
  #define REG_SPI2_TNPR                     (0x40008518U) /**< \brief (SPI2) Transmit Next Pointer Register */
  #define REG_SPI2_TNCR                     (0x4000851CU) /**< \brief (SPI2) Transmit Next Counter Register */
  #define REG_SPI2_PTCR                     (0x40008520U) /**< \brief (SPI2) Transfer Control Register */
  #define REG_SPI2_PTSR                     (0x40008524U) /**< \brief (SPI2) Transfer Status Register */
#else
  #define REG_SPI2_CR      (*(__O  uint32_t*)0x40008400U) /**< \brief (SPI2) Control Register */
  #define REG_SPI2_MR      (*(__IO uint32_t*)0x40008404U) /**< \brief (SPI2) Mode Register */
  #define REG_SPI2_RDR     (*(__I  uint32_t*)0x40008408U) /**< \brief (SPI2) Receive Data Register */
  #define REG_SPI2_TDR     (*(__O  uint32_t*)0x4000840CU) /**< \brief (SPI2) Transmit Data Register */
  #define REG_SPI2_SR      (*(__I  uint32_t*)0x40008410U) /**< \brief (SPI2) Status Register */
  #define REG_SPI2_IER     (*(__O  uint32_t*)0x40008414U) /**< \brief (SPI2) Interrupt Enable Register */
  #define REG_SPI2_IDR     (*(__O  uint32_t*)0x40008418U) /**< \brief (SPI2) Interrupt Disable Register */
  #define REG_SPI2_IMR     (*(__I  uint32_t*)0x4000841CU) /**< \brief (SPI2) Interrupt Mask Register */
  #define REG_SPI2_CSR     (*(__IO uint32_t*)0x40008430U) /**< \brief (SPI2) Chip Select Register */
  #define REG_SPI2_FMR     (*(__IO uint32_t*)0x40008440U) /**< \brief (SPI2) FIFO Mode Register */
  #define REG_SPI2_FLR     (*(__I  uint32_t*)0x40008444U) /**< \brief (SPI2) FIFO Level Register */
  #define REG_SPI2_CMPR    (*(__IO uint32_t*)0x40008448U) /**< \brief (SPI2) Comparison Register */
  #define REG_SPI2_WPMR    (*(__IO uint32_t*)0x400084E4U) /**< \brief (SPI2) Write Protection Mode Register */
  #define REG_SPI2_WPSR    (*(__I  uint32_t*)0x400084E8U) /**< \brief (SPI2) Write Protection Status Register */
  #define REG_SPI2_RPR     (*(__IO uint32_t*)0x40008500U) /**< \brief (SPI2) Receive Pointer Register */
  #define REG_SPI2_RCR     (*(__IO uint32_t*)0x40008504U) /**< \brief (SPI2) Receive Counter Register */
  #define REG_SPI2_TPR     (*(__IO uint32_t*)0x40008508U) /**< \brief (SPI2) Transmit Pointer Register */
  #define REG_SPI2_TCR     (*(__IO uint32_t*)0x4000850CU) /**< \brief (SPI2) Transmit Counter Register */
  #define REG_SPI2_RNPR    (*(__IO uint32_t*)0x40008510U) /**< \brief (SPI2) Receive Next Pointer Register */
  #define REG_SPI2_RNCR    (*(__IO uint32_t*)0x40008514U) /**< \brief (SPI2) Receive Next Counter Register */
  #define REG_SPI2_TNPR    (*(__IO uint32_t*)0x40008518U) /**< \brief (SPI2) Transmit Next Pointer Register */
  #define REG_SPI2_TNCR    (*(__IO uint32_t*)0x4000851CU) /**< \brief (SPI2) Transmit Next Counter Register */
  #define REG_SPI2_PTCR    (*(__O  uint32_t*)0x40008520U) /**< \brief (SPI2) Transfer Control Register */
  #define REG_SPI2_PTSR    (*(__I  uint32_t*)0x40008524U) /**< \brief (SPI2) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SPI2_INSTANCE_ */
