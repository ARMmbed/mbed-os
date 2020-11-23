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

#ifndef _PIC32CX_SPI5_INSTANCE_
#define _PIC32CX_SPI5_INSTANCE_

/* ========== Register definition for SPI5 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI5_CR                       (0x40014400U) /**< \brief (SPI5) Control Register */
  #define REG_SPI5_MR                       (0x40014404U) /**< \brief (SPI5) Mode Register */
  #define REG_SPI5_RDR                      (0x40014408U) /**< \brief (SPI5) Receive Data Register */
  #define REG_SPI5_TDR                      (0x4001440CU) /**< \brief (SPI5) Transmit Data Register */
  #define REG_SPI5_SR                       (0x40014410U) /**< \brief (SPI5) Status Register */
  #define REG_SPI5_IER                      (0x40014414U) /**< \brief (SPI5) Interrupt Enable Register */
  #define REG_SPI5_IDR                      (0x40014418U) /**< \brief (SPI5) Interrupt Disable Register */
  #define REG_SPI5_IMR                      (0x4001441CU) /**< \brief (SPI5) Interrupt Mask Register */
  #define REG_SPI5_CSR                      (0x40014430U) /**< \brief (SPI5) Chip Select Register */
  #define REG_SPI5_FMR                      (0x40014440U) /**< \brief (SPI5) FIFO Mode Register */
  #define REG_SPI5_FLR                      (0x40014444U) /**< \brief (SPI5) FIFO Level Register */
  #define REG_SPI5_CMPR                     (0x40014448U) /**< \brief (SPI5) Comparison Register */
  #define REG_SPI5_WPMR                     (0x400144E4U) /**< \brief (SPI5) Write Protection Mode Register */
  #define REG_SPI5_WPSR                     (0x400144E8U) /**< \brief (SPI5) Write Protection Status Register */
  #define REG_SPI5_RPR                      (0x40014500U) /**< \brief (SPI5) Receive Pointer Register */
  #define REG_SPI5_RCR                      (0x40014504U) /**< \brief (SPI5) Receive Counter Register */
  #define REG_SPI5_TPR                      (0x40014508U) /**< \brief (SPI5) Transmit Pointer Register */
  #define REG_SPI5_TCR                      (0x4001450CU) /**< \brief (SPI5) Transmit Counter Register */
  #define REG_SPI5_RNPR                     (0x40014510U) /**< \brief (SPI5) Receive Next Pointer Register */
  #define REG_SPI5_RNCR                     (0x40014514U) /**< \brief (SPI5) Receive Next Counter Register */
  #define REG_SPI5_TNPR                     (0x40014518U) /**< \brief (SPI5) Transmit Next Pointer Register */
  #define REG_SPI5_TNCR                     (0x4001451CU) /**< \brief (SPI5) Transmit Next Counter Register */
  #define REG_SPI5_PTCR                     (0x40014520U) /**< \brief (SPI5) Transfer Control Register */
  #define REG_SPI5_PTSR                     (0x40014524U) /**< \brief (SPI5) Transfer Status Register */
#else
  #define REG_SPI5_CR      (*(__O  uint32_t*)0x40014400U) /**< \brief (SPI5) Control Register */
  #define REG_SPI5_MR      (*(__IO uint32_t*)0x40014404U) /**< \brief (SPI5) Mode Register */
  #define REG_SPI5_RDR     (*(__I  uint32_t*)0x40014408U) /**< \brief (SPI5) Receive Data Register */
  #define REG_SPI5_TDR     (*(__O  uint32_t*)0x4001440CU) /**< \brief (SPI5) Transmit Data Register */
  #define REG_SPI5_SR      (*(__I  uint32_t*)0x40014410U) /**< \brief (SPI5) Status Register */
  #define REG_SPI5_IER     (*(__O  uint32_t*)0x40014414U) /**< \brief (SPI5) Interrupt Enable Register */
  #define REG_SPI5_IDR     (*(__O  uint32_t*)0x40014418U) /**< \brief (SPI5) Interrupt Disable Register */
  #define REG_SPI5_IMR     (*(__I  uint32_t*)0x4001441CU) /**< \brief (SPI5) Interrupt Mask Register */
  #define REG_SPI5_CSR     (*(__IO uint32_t*)0x40014430U) /**< \brief (SPI5) Chip Select Register */
  #define REG_SPI5_FMR     (*(__IO uint32_t*)0x40014440U) /**< \brief (SPI5) FIFO Mode Register */
  #define REG_SPI5_FLR     (*(__I  uint32_t*)0x40014444U) /**< \brief (SPI5) FIFO Level Register */
  #define REG_SPI5_CMPR    (*(__IO uint32_t*)0x40014448U) /**< \brief (SPI5) Comparison Register */
  #define REG_SPI5_WPMR    (*(__IO uint32_t*)0x400144E4U) /**< \brief (SPI5) Write Protection Mode Register */
  #define REG_SPI5_WPSR    (*(__I  uint32_t*)0x400144E8U) /**< \brief (SPI5) Write Protection Status Register */
  #define REG_SPI5_RPR     (*(__IO uint32_t*)0x40014500U) /**< \brief (SPI5) Receive Pointer Register */
  #define REG_SPI5_RCR     (*(__IO uint32_t*)0x40014504U) /**< \brief (SPI5) Receive Counter Register */
  #define REG_SPI5_TPR     (*(__IO uint32_t*)0x40014508U) /**< \brief (SPI5) Transmit Pointer Register */
  #define REG_SPI5_TCR     (*(__IO uint32_t*)0x4001450CU) /**< \brief (SPI5) Transmit Counter Register */
  #define REG_SPI5_RNPR    (*(__IO uint32_t*)0x40014510U) /**< \brief (SPI5) Receive Next Pointer Register */
  #define REG_SPI5_RNCR    (*(__IO uint32_t*)0x40014514U) /**< \brief (SPI5) Receive Next Counter Register */
  #define REG_SPI5_TNPR    (*(__IO uint32_t*)0x40014518U) /**< \brief (SPI5) Transmit Next Pointer Register */
  #define REG_SPI5_TNCR    (*(__IO uint32_t*)0x4001451CU) /**< \brief (SPI5) Transmit Next Counter Register */
  #define REG_SPI5_PTCR    (*(__O  uint32_t*)0x40014520U) /**< \brief (SPI5) Transfer Control Register */
  #define REG_SPI5_PTSR    (*(__I  uint32_t*)0x40014524U) /**< \brief (SPI5) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SPI5_INSTANCE_ */
