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

#ifndef _PIC32CX_TWI5_INSTANCE_
#define _PIC32CX_TWI5_INSTANCE_

/* ========== Register definition for TWI5 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI5_CR                     (0x40014600U) /**< \brief (TWI5) Control Register */
  #define REG_TWI5_MMR                    (0x40014604U) /**< \brief (TWI5) Master Mode Register */
  #define REG_TWI5_SMR                    (0x40014608U) /**< \brief (TWI5) Slave Mode Register */
  #define REG_TWI5_IADR                   (0x4001460CU) /**< \brief (TWI5) Internal Address Register */
  #define REG_TWI5_CWGR                   (0x40014610U) /**< \brief (TWI5) Clock Waveform Generator Register */
  #define REG_TWI5_SR                     (0x40014620U) /**< \brief (TWI5) Status Register */
  #define REG_TWI5_IER                    (0x40014624U) /**< \brief (TWI5) Interrupt Enable Register */
  #define REG_TWI5_IDR                    (0x40014628U) /**< \brief (TWI5) Interrupt Disable Register */
  #define REG_TWI5_IMR                    (0x4001462CU) /**< \brief (TWI5) Interrupt Mask Register */
  #define REG_TWI5_RHR                    (0x40014630U) /**< \brief (TWI5) Receive Holding Register */
  #define REG_TWI5_THR                    (0x40014634U) /**< \brief (TWI5) Transmit Holding Register */
  #define REG_TWI5_SMBTR                  (0x40014638U) /**< \brief (TWI5) SMBus Timing Register */
  #define REG_TWI5_ACR                    (0x40014640U) /**< \brief (TWI5) Alternative Command Register */
  #define REG_TWI5_FILTR                  (0x40014644U) /**< \brief (TWI5) Filter Register */
  #define REG_TWI5_SWMR                   (0x4001464CU) /**< \brief (TWI5) SleepWalking Matching Register */
  #define REG_TWI5_FMR                    (0x40014650U) /**< \brief (TWI5) FIFO Mode Register */
  #define REG_TWI5_FLR                    (0x40014654U) /**< \brief (TWI5) FIFO Level Register */
  #define REG_TWI5_FSR                    (0x40014660U) /**< \brief (TWI5) FIFO Status Register */
  #define REG_TWI5_FIER                   (0x40014664U) /**< \brief (TWI5) FIFO Interrupt Enable Register */
  #define REG_TWI5_FIDR                   (0x40014668U) /**< \brief (TWI5) FIFO Interrupt Disable Register */
  #define REG_TWI5_FIMR                   (0x4001466CU) /**< \brief (TWI5) FIFO Interrupt Mask Register */
  #define REG_TWI5_DR                     (0x400146D0U) /**< \brief (TWI5) Debug Register */
  #define REG_TWI5_WPMR                   (0x400146E4U) /**< \brief (TWI5) Write Protection Mode Register */
  #define REG_TWI5_WPSR                   (0x400146E8U) /**< \brief (TWI5) Write Protection Status Register */
  #define REG_TWI5_RPR                    (0x40014700U) /**< \brief (TWI5) Receive Pointer Register */
  #define REG_TWI5_RCR                    (0x40014704U) /**< \brief (TWI5) Receive Counter Register */
  #define REG_TWI5_TPR                    (0x40014708U) /**< \brief (TWI5) Transmit Pointer Register */
  #define REG_TWI5_TCR                    (0x4001470CU) /**< \brief (TWI5) Transmit Counter Register */
  #define REG_TWI5_RNPR                   (0x40014710U) /**< \brief (TWI5) Receive Next Pointer Register */
  #define REG_TWI5_RNCR                   (0x40014714U) /**< \brief (TWI5) Receive Next Counter Register */
  #define REG_TWI5_TNPR                   (0x40014718U) /**< \brief (TWI5) Transmit Next Pointer Register */
  #define REG_TWI5_TNCR                   (0x4001471CU) /**< \brief (TWI5) Transmit Next Counter Register */
  #define REG_TWI5_PTCR                   (0x40014720U) /**< \brief (TWI5) Transfer Control Register */
  #define REG_TWI5_PTSR                   (0x40014724U) /**< \brief (TWI5) Transfer Status Register */
#else
  #define REG_TWI5_CR    (*(__O  uint32_t*)0x40014600U) /**< \brief (TWI5) Control Register */
  #define REG_TWI5_MMR   (*(__IO uint32_t*)0x40014604U) /**< \brief (TWI5) Master Mode Register */
  #define REG_TWI5_SMR   (*(__IO uint32_t*)0x40014608U) /**< \brief (TWI5) Slave Mode Register */
  #define REG_TWI5_IADR  (*(__IO uint32_t*)0x4001460CU) /**< \brief (TWI5) Internal Address Register */
  #define REG_TWI5_CWGR  (*(__IO uint32_t*)0x40014610U) /**< \brief (TWI5) Clock Waveform Generator Register */
  #define REG_TWI5_SR    (*(__I  uint32_t*)0x40014620U) /**< \brief (TWI5) Status Register */
  #define REG_TWI5_IER   (*(__O  uint32_t*)0x40014624U) /**< \brief (TWI5) Interrupt Enable Register */
  #define REG_TWI5_IDR   (*(__O  uint32_t*)0x40014628U) /**< \brief (TWI5) Interrupt Disable Register */
  #define REG_TWI5_IMR   (*(__I  uint32_t*)0x4001462CU) /**< \brief (TWI5) Interrupt Mask Register */
  #define REG_TWI5_RHR   (*(__I  uint32_t*)0x40014630U) /**< \brief (TWI5) Receive Holding Register */
  #define REG_TWI5_THR   (*(__O  uint32_t*)0x40014634U) /**< \brief (TWI5) Transmit Holding Register */
  #define REG_TWI5_SMBTR (*(__IO uint32_t*)0x40014638U) /**< \brief (TWI5) SMBus Timing Register */
  #define REG_TWI5_ACR   (*(__IO uint32_t*)0x40014640U) /**< \brief (TWI5) Alternative Command Register */
  #define REG_TWI5_FILTR (*(__IO uint32_t*)0x40014644U) /**< \brief (TWI5) Filter Register */
  #define REG_TWI5_SWMR  (*(__IO uint32_t*)0x4001464CU) /**< \brief (TWI5) SleepWalking Matching Register */
  #define REG_TWI5_FMR   (*(__IO uint32_t*)0x40014650U) /**< \brief (TWI5) FIFO Mode Register */
  #define REG_TWI5_FLR   (*(__I  uint32_t*)0x40014654U) /**< \brief (TWI5) FIFO Level Register */
  #define REG_TWI5_FSR   (*(__I  uint32_t*)0x40014660U) /**< \brief (TWI5) FIFO Status Register */
  #define REG_TWI5_FIER  (*(__O  uint32_t*)0x40014664U) /**< \brief (TWI5) FIFO Interrupt Enable Register */
  #define REG_TWI5_FIDR  (*(__O  uint32_t*)0x40014668U) /**< \brief (TWI5) FIFO Interrupt Disable Register */
  #define REG_TWI5_FIMR  (*(__I  uint32_t*)0x4001466CU) /**< \brief (TWI5) FIFO Interrupt Mask Register */
  #define REG_TWI5_DR    (*(__I  uint32_t*)0x400146D0U) /**< \brief (TWI5) Debug Register */
  #define REG_TWI5_WPMR  (*(__IO uint32_t*)0x400146E4U) /**< \brief (TWI5) Write Protection Mode Register */
  #define REG_TWI5_WPSR  (*(__I  uint32_t*)0x400146E8U) /**< \brief (TWI5) Write Protection Status Register */
  #define REG_TWI5_RPR   (*(__IO uint32_t*)0x40014700U) /**< \brief (TWI5) Receive Pointer Register */
  #define REG_TWI5_RCR   (*(__IO uint32_t*)0x40014704U) /**< \brief (TWI5) Receive Counter Register */
  #define REG_TWI5_TPR   (*(__IO uint32_t*)0x40014708U) /**< \brief (TWI5) Transmit Pointer Register */
  #define REG_TWI5_TCR   (*(__IO uint32_t*)0x4001470CU) /**< \brief (TWI5) Transmit Counter Register */
  #define REG_TWI5_RNPR  (*(__IO uint32_t*)0x40014710U) /**< \brief (TWI5) Receive Next Pointer Register */
  #define REG_TWI5_RNCR  (*(__IO uint32_t*)0x40014714U) /**< \brief (TWI5) Receive Next Counter Register */
  #define REG_TWI5_TNPR  (*(__IO uint32_t*)0x40014718U) /**< \brief (TWI5) Transmit Next Pointer Register */
  #define REG_TWI5_TNCR  (*(__IO uint32_t*)0x4001471CU) /**< \brief (TWI5) Transmit Next Counter Register */
  #define REG_TWI5_PTCR  (*(__O  uint32_t*)0x40014720U) /**< \brief (TWI5) Transfer Control Register */
  #define REG_TWI5_PTSR  (*(__I  uint32_t*)0x40014724U) /**< \brief (TWI5) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TWI5_INSTANCE_ */
