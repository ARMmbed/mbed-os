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

#ifndef _PIC32CX_TWI4_INSTANCE_
#define _PIC32CX_TWI4_INSTANCE_

/* ========== Register definition for TWI4 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI4_CR                     (0x40010600U) /**< \brief (TWI4) Control Register */
  #define REG_TWI4_MMR                    (0x40010604U) /**< \brief (TWI4) Master Mode Register */
  #define REG_TWI4_SMR                    (0x40010608U) /**< \brief (TWI4) Slave Mode Register */
  #define REG_TWI4_IADR                   (0x4001060CU) /**< \brief (TWI4) Internal Address Register */
  #define REG_TWI4_CWGR                   (0x40010610U) /**< \brief (TWI4) Clock Waveform Generator Register */
  #define REG_TWI4_SR                     (0x40010620U) /**< \brief (TWI4) Status Register */
  #define REG_TWI4_IER                    (0x40010624U) /**< \brief (TWI4) Interrupt Enable Register */
  #define REG_TWI4_IDR                    (0x40010628U) /**< \brief (TWI4) Interrupt Disable Register */
  #define REG_TWI4_IMR                    (0x4001062CU) /**< \brief (TWI4) Interrupt Mask Register */
  #define REG_TWI4_RHR                    (0x40010630U) /**< \brief (TWI4) Receive Holding Register */
  #define REG_TWI4_THR                    (0x40010634U) /**< \brief (TWI4) Transmit Holding Register */
  #define REG_TWI4_SMBTR                  (0x40010638U) /**< \brief (TWI4) SMBus Timing Register */
  #define REG_TWI4_ACR                    (0x40010640U) /**< \brief (TWI4) Alternative Command Register */
  #define REG_TWI4_FILTR                  (0x40010644U) /**< \brief (TWI4) Filter Register */
  #define REG_TWI4_SWMR                   (0x4001064CU) /**< \brief (TWI4) SleepWalking Matching Register */
  #define REG_TWI4_FMR                    (0x40010650U) /**< \brief (TWI4) FIFO Mode Register */
  #define REG_TWI4_FLR                    (0x40010654U) /**< \brief (TWI4) FIFO Level Register */
  #define REG_TWI4_FSR                    (0x40010660U) /**< \brief (TWI4) FIFO Status Register */
  #define REG_TWI4_FIER                   (0x40010664U) /**< \brief (TWI4) FIFO Interrupt Enable Register */
  #define REG_TWI4_FIDR                   (0x40010668U) /**< \brief (TWI4) FIFO Interrupt Disable Register */
  #define REG_TWI4_FIMR                   (0x4001066CU) /**< \brief (TWI4) FIFO Interrupt Mask Register */
  #define REG_TWI4_DR                     (0x400106D0U) /**< \brief (TWI4) Debug Register */
  #define REG_TWI4_WPMR                   (0x400106E4U) /**< \brief (TWI4) Write Protection Mode Register */
  #define REG_TWI4_WPSR                   (0x400106E8U) /**< \brief (TWI4) Write Protection Status Register */
  #define REG_TWI4_RPR                    (0x40010700U) /**< \brief (TWI4) Receive Pointer Register */
  #define REG_TWI4_RCR                    (0x40010704U) /**< \brief (TWI4) Receive Counter Register */
  #define REG_TWI4_TPR                    (0x40010708U) /**< \brief (TWI4) Transmit Pointer Register */
  #define REG_TWI4_TCR                    (0x4001070CU) /**< \brief (TWI4) Transmit Counter Register */
  #define REG_TWI4_RNPR                   (0x40010710U) /**< \brief (TWI4) Receive Next Pointer Register */
  #define REG_TWI4_RNCR                   (0x40010714U) /**< \brief (TWI4) Receive Next Counter Register */
  #define REG_TWI4_TNPR                   (0x40010718U) /**< \brief (TWI4) Transmit Next Pointer Register */
  #define REG_TWI4_TNCR                   (0x4001071CU) /**< \brief (TWI4) Transmit Next Counter Register */
  #define REG_TWI4_PTCR                   (0x40010720U) /**< \brief (TWI4) Transfer Control Register */
  #define REG_TWI4_PTSR                   (0x40010724U) /**< \brief (TWI4) Transfer Status Register */
#else
  #define REG_TWI4_CR    (*(__O  uint32_t*)0x40010600U) /**< \brief (TWI4) Control Register */
  #define REG_TWI4_MMR   (*(__IO uint32_t*)0x40010604U) /**< \brief (TWI4) Master Mode Register */
  #define REG_TWI4_SMR   (*(__IO uint32_t*)0x40010608U) /**< \brief (TWI4) Slave Mode Register */
  #define REG_TWI4_IADR  (*(__IO uint32_t*)0x4001060CU) /**< \brief (TWI4) Internal Address Register */
  #define REG_TWI4_CWGR  (*(__IO uint32_t*)0x40010610U) /**< \brief (TWI4) Clock Waveform Generator Register */
  #define REG_TWI4_SR    (*(__I  uint32_t*)0x40010620U) /**< \brief (TWI4) Status Register */
  #define REG_TWI4_IER   (*(__O  uint32_t*)0x40010624U) /**< \brief (TWI4) Interrupt Enable Register */
  #define REG_TWI4_IDR   (*(__O  uint32_t*)0x40010628U) /**< \brief (TWI4) Interrupt Disable Register */
  #define REG_TWI4_IMR   (*(__I  uint32_t*)0x4001062CU) /**< \brief (TWI4) Interrupt Mask Register */
  #define REG_TWI4_RHR   (*(__I  uint32_t*)0x40010630U) /**< \brief (TWI4) Receive Holding Register */
  #define REG_TWI4_THR   (*(__O  uint32_t*)0x40010634U) /**< \brief (TWI4) Transmit Holding Register */
  #define REG_TWI4_SMBTR (*(__IO uint32_t*)0x40010638U) /**< \brief (TWI4) SMBus Timing Register */
  #define REG_TWI4_ACR   (*(__IO uint32_t*)0x40010640U) /**< \brief (TWI4) Alternative Command Register */
  #define REG_TWI4_FILTR (*(__IO uint32_t*)0x40010644U) /**< \brief (TWI4) Filter Register */
  #define REG_TWI4_SWMR  (*(__IO uint32_t*)0x4001064CU) /**< \brief (TWI4) SleepWalking Matching Register */
  #define REG_TWI4_FMR   (*(__IO uint32_t*)0x40010650U) /**< \brief (TWI4) FIFO Mode Register */
  #define REG_TWI4_FLR   (*(__I  uint32_t*)0x40010654U) /**< \brief (TWI4) FIFO Level Register */
  #define REG_TWI4_FSR   (*(__I  uint32_t*)0x40010660U) /**< \brief (TWI4) FIFO Status Register */
  #define REG_TWI4_FIER  (*(__O  uint32_t*)0x40010664U) /**< \brief (TWI4) FIFO Interrupt Enable Register */
  #define REG_TWI4_FIDR  (*(__O  uint32_t*)0x40010668U) /**< \brief (TWI4) FIFO Interrupt Disable Register */
  #define REG_TWI4_FIMR  (*(__I  uint32_t*)0x4001066CU) /**< \brief (TWI4) FIFO Interrupt Mask Register */
  #define REG_TWI4_DR    (*(__I  uint32_t*)0x400106D0U) /**< \brief (TWI4) Debug Register */
  #define REG_TWI4_WPMR  (*(__IO uint32_t*)0x400106E4U) /**< \brief (TWI4) Write Protection Mode Register */
  #define REG_TWI4_WPSR  (*(__I  uint32_t*)0x400106E8U) /**< \brief (TWI4) Write Protection Status Register */
  #define REG_TWI4_RPR   (*(__IO uint32_t*)0x40010700U) /**< \brief (TWI4) Receive Pointer Register */
  #define REG_TWI4_RCR   (*(__IO uint32_t*)0x40010704U) /**< \brief (TWI4) Receive Counter Register */
  #define REG_TWI4_TPR   (*(__IO uint32_t*)0x40010708U) /**< \brief (TWI4) Transmit Pointer Register */
  #define REG_TWI4_TCR   (*(__IO uint32_t*)0x4001070CU) /**< \brief (TWI4) Transmit Counter Register */
  #define REG_TWI4_RNPR  (*(__IO uint32_t*)0x40010710U) /**< \brief (TWI4) Receive Next Pointer Register */
  #define REG_TWI4_RNCR  (*(__IO uint32_t*)0x40010714U) /**< \brief (TWI4) Receive Next Counter Register */
  #define REG_TWI4_TNPR  (*(__IO uint32_t*)0x40010718U) /**< \brief (TWI4) Transmit Next Pointer Register */
  #define REG_TWI4_TNCR  (*(__IO uint32_t*)0x4001071CU) /**< \brief (TWI4) Transmit Next Counter Register */
  #define REG_TWI4_PTCR  (*(__O  uint32_t*)0x40010720U) /**< \brief (TWI4) Transfer Control Register */
  #define REG_TWI4_PTSR  (*(__I  uint32_t*)0x40010724U) /**< \brief (TWI4) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TWI4_INSTANCE_ */
