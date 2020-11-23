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

#ifndef _PIC32CX_SPI6_INSTANCE_
#define _PIC32CX_SPI6_INSTANCE_

/* ========== Register definition for SPI6 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI6_CR                       (0x40018400U) /**< \brief (SPI6) Control Register */
  #define REG_SPI6_MR                       (0x40018404U) /**< \brief (SPI6) Mode Register */
  #define REG_SPI6_RDR                      (0x40018408U) /**< \brief (SPI6) Receive Data Register */
  #define REG_SPI6_TDR                      (0x4001840CU) /**< \brief (SPI6) Transmit Data Register */
  #define REG_SPI6_SR                       (0x40018410U) /**< \brief (SPI6) Status Register */
  #define REG_SPI6_IER                      (0x40018414U) /**< \brief (SPI6) Interrupt Enable Register */
  #define REG_SPI6_IDR                      (0x40018418U) /**< \brief (SPI6) Interrupt Disable Register */
  #define REG_SPI6_IMR                      (0x4001841CU) /**< \brief (SPI6) Interrupt Mask Register */
  #define REG_SPI6_CSR                      (0x40018430U) /**< \brief (SPI6) Chip Select Register */
  #define REG_SPI6_FMR                      (0x40018440U) /**< \brief (SPI6) FIFO Mode Register */
  #define REG_SPI6_FLR                      (0x40018444U) /**< \brief (SPI6) FIFO Level Register */
  #define REG_SPI6_CMPR                     (0x40018448U) /**< \brief (SPI6) Comparison Register */
  #define REG_SPI6_WPMR                     (0x400184E4U) /**< \brief (SPI6) Write Protection Mode Register */
  #define REG_SPI6_WPSR                     (0x400184E8U) /**< \brief (SPI6) Write Protection Status Register */
  #define REG_SPI6_RPR                      (0x40018500U) /**< \brief (SPI6) Receive Pointer Register */
  #define REG_SPI6_RCR                      (0x40018504U) /**< \brief (SPI6) Receive Counter Register */
  #define REG_SPI6_TPR                      (0x40018508U) /**< \brief (SPI6) Transmit Pointer Register */
  #define REG_SPI6_TCR                      (0x4001850CU) /**< \brief (SPI6) Transmit Counter Register */
  #define REG_SPI6_RNPR                     (0x40018510U) /**< \brief (SPI6) Receive Next Pointer Register */
  #define REG_SPI6_RNCR                     (0x40018514U) /**< \brief (SPI6) Receive Next Counter Register */
  #define REG_SPI6_TNPR                     (0x40018518U) /**< \brief (SPI6) Transmit Next Pointer Register */
  #define REG_SPI6_TNCR                     (0x4001851CU) /**< \brief (SPI6) Transmit Next Counter Register */
  #define REG_SPI6_PTCR                     (0x40018520U) /**< \brief (SPI6) Transfer Control Register */
  #define REG_SPI6_PTSR                     (0x40018524U) /**< \brief (SPI6) Transfer Status Register */
#else
  #define REG_SPI6_CR      (*(__O  uint32_t*)0x40018400U) /**< \brief (SPI6) Control Register */
  #define REG_SPI6_MR      (*(__IO uint32_t*)0x40018404U) /**< \brief (SPI6) Mode Register */
  #define REG_SPI6_RDR     (*(__I  uint32_t*)0x40018408U) /**< \brief (SPI6) Receive Data Register */
  #define REG_SPI6_TDR     (*(__O  uint32_t*)0x4001840CU) /**< \brief (SPI6) Transmit Data Register */
  #define REG_SPI6_SR      (*(__I  uint32_t*)0x40018410U) /**< \brief (SPI6) Status Register */
  #define REG_SPI6_IER     (*(__O  uint32_t*)0x40018414U) /**< \brief (SPI6) Interrupt Enable Register */
  #define REG_SPI6_IDR     (*(__O  uint32_t*)0x40018418U) /**< \brief (SPI6) Interrupt Disable Register */
  #define REG_SPI6_IMR     (*(__I  uint32_t*)0x4001841CU) /**< \brief (SPI6) Interrupt Mask Register */
  #define REG_SPI6_CSR     (*(__IO uint32_t*)0x40018430U) /**< \brief (SPI6) Chip Select Register */
  #define REG_SPI6_FMR     (*(__IO uint32_t*)0x40018440U) /**< \brief (SPI6) FIFO Mode Register */
  #define REG_SPI6_FLR     (*(__I  uint32_t*)0x40018444U) /**< \brief (SPI6) FIFO Level Register */
  #define REG_SPI6_CMPR    (*(__IO uint32_t*)0x40018448U) /**< \brief (SPI6) Comparison Register */
  #define REG_SPI6_WPMR    (*(__IO uint32_t*)0x400184E4U) /**< \brief (SPI6) Write Protection Mode Register */
  #define REG_SPI6_WPSR    (*(__I  uint32_t*)0x400184E8U) /**< \brief (SPI6) Write Protection Status Register */
  #define REG_SPI6_RPR     (*(__IO uint32_t*)0x40018500U) /**< \brief (SPI6) Receive Pointer Register */
  #define REG_SPI6_RCR     (*(__IO uint32_t*)0x40018504U) /**< \brief (SPI6) Receive Counter Register */
  #define REG_SPI6_TPR     (*(__IO uint32_t*)0x40018508U) /**< \brief (SPI6) Transmit Pointer Register */
  #define REG_SPI6_TCR     (*(__IO uint32_t*)0x4001850CU) /**< \brief (SPI6) Transmit Counter Register */
  #define REG_SPI6_RNPR    (*(__IO uint32_t*)0x40018510U) /**< \brief (SPI6) Receive Next Pointer Register */
  #define REG_SPI6_RNCR    (*(__IO uint32_t*)0x40018514U) /**< \brief (SPI6) Receive Next Counter Register */
  #define REG_SPI6_TNPR    (*(__IO uint32_t*)0x40018518U) /**< \brief (SPI6) Transmit Next Pointer Register */
  #define REG_SPI6_TNCR    (*(__IO uint32_t*)0x4001851CU) /**< \brief (SPI6) Transmit Next Counter Register */
  #define REG_SPI6_PTCR    (*(__O  uint32_t*)0x40018520U) /**< \brief (SPI6) Transfer Control Register */
  #define REG_SPI6_PTSR    (*(__I  uint32_t*)0x40018524U) /**< \brief (SPI6) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SPI6_INSTANCE_ */
