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

#ifndef _PIC32CX_TWI0_INSTANCE_
#define _PIC32CX_TWI0_INSTANCE_

/* ========== Register definition for TWI0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI0_CR                     (0x40000600U) /**< \brief (TWI0) Control Register */
  #define REG_TWI0_MMR                    (0x40000604U) /**< \brief (TWI0) Master Mode Register */
  #define REG_TWI0_SMR                    (0x40000608U) /**< \brief (TWI0) Slave Mode Register */
  #define REG_TWI0_IADR                   (0x4000060CU) /**< \brief (TWI0) Internal Address Register */
  #define REG_TWI0_CWGR                   (0x40000610U) /**< \brief (TWI0) Clock Waveform Generator Register */
  #define REG_TWI0_SR                     (0x40000620U) /**< \brief (TWI0) Status Register */
  #define REG_TWI0_IER                    (0x40000624U) /**< \brief (TWI0) Interrupt Enable Register */
  #define REG_TWI0_IDR                    (0x40000628U) /**< \brief (TWI0) Interrupt Disable Register */
  #define REG_TWI0_IMR                    (0x4000062CU) /**< \brief (TWI0) Interrupt Mask Register */
  #define REG_TWI0_RHR                    (0x40000630U) /**< \brief (TWI0) Receive Holding Register */
  #define REG_TWI0_THR                    (0x40000634U) /**< \brief (TWI0) Transmit Holding Register */
  #define REG_TWI0_SMBTR                  (0x40000638U) /**< \brief (TWI0) SMBus Timing Register */
  #define REG_TWI0_ACR                    (0x40000640U) /**< \brief (TWI0) Alternative Command Register */
  #define REG_TWI0_FILTR                  (0x40000644U) /**< \brief (TWI0) Filter Register */
  #define REG_TWI0_SWMR                   (0x4000064CU) /**< \brief (TWI0) SleepWalking Matching Register */
  #define REG_TWI0_FMR                    (0x40000650U) /**< \brief (TWI0) FIFO Mode Register */
  #define REG_TWI0_FLR                    (0x40000654U) /**< \brief (TWI0) FIFO Level Register */
  #define REG_TWI0_FSR                    (0x40000660U) /**< \brief (TWI0) FIFO Status Register */
  #define REG_TWI0_FIER                   (0x40000664U) /**< \brief (TWI0) FIFO Interrupt Enable Register */
  #define REG_TWI0_FIDR                   (0x40000668U) /**< \brief (TWI0) FIFO Interrupt Disable Register */
  #define REG_TWI0_FIMR                   (0x4000066CU) /**< \brief (TWI0) FIFO Interrupt Mask Register */
  #define REG_TWI0_DR                     (0x400006D0U) /**< \brief (TWI0) Debug Register */
  #define REG_TWI0_WPMR                   (0x400006E4U) /**< \brief (TWI0) Write Protection Mode Register */
  #define REG_TWI0_WPSR                   (0x400006E8U) /**< \brief (TWI0) Write Protection Status Register */
  #define REG_TWI0_RPR                    (0x40000700U) /**< \brief (TWI0) Receive Pointer Register */
  #define REG_TWI0_RCR                    (0x40000704U) /**< \brief (TWI0) Receive Counter Register */
  #define REG_TWI0_TPR                    (0x40000708U) /**< \brief (TWI0) Transmit Pointer Register */
  #define REG_TWI0_TCR                    (0x4000070CU) /**< \brief (TWI0) Transmit Counter Register */
  #define REG_TWI0_RNPR                   (0x40000710U) /**< \brief (TWI0) Receive Next Pointer Register */
  #define REG_TWI0_RNCR                   (0x40000714U) /**< \brief (TWI0) Receive Next Counter Register */
  #define REG_TWI0_TNPR                   (0x40000718U) /**< \brief (TWI0) Transmit Next Pointer Register */
  #define REG_TWI0_TNCR                   (0x4000071CU) /**< \brief (TWI0) Transmit Next Counter Register */
  #define REG_TWI0_PTCR                   (0x40000720U) /**< \brief (TWI0) Transfer Control Register */
  #define REG_TWI0_PTSR                   (0x40000724U) /**< \brief (TWI0) Transfer Status Register */
#else
  #define REG_TWI0_CR    (*(__O  uint32_t*)0x40000600U) /**< \brief (TWI0) Control Register */
  #define REG_TWI0_MMR   (*(__IO uint32_t*)0x40000604U) /**< \brief (TWI0) Master Mode Register */
  #define REG_TWI0_SMR   (*(__IO uint32_t*)0x40000608U) /**< \brief (TWI0) Slave Mode Register */
  #define REG_TWI0_IADR  (*(__IO uint32_t*)0x4000060CU) /**< \brief (TWI0) Internal Address Register */
  #define REG_TWI0_CWGR  (*(__IO uint32_t*)0x40000610U) /**< \brief (TWI0) Clock Waveform Generator Register */
  #define REG_TWI0_SR    (*(__I  uint32_t*)0x40000620U) /**< \brief (TWI0) Status Register */
  #define REG_TWI0_IER   (*(__O  uint32_t*)0x40000624U) /**< \brief (TWI0) Interrupt Enable Register */
  #define REG_TWI0_IDR   (*(__O  uint32_t*)0x40000628U) /**< \brief (TWI0) Interrupt Disable Register */
  #define REG_TWI0_IMR   (*(__I  uint32_t*)0x4000062CU) /**< \brief (TWI0) Interrupt Mask Register */
  #define REG_TWI0_RHR   (*(__I  uint32_t*)0x40000630U) /**< \brief (TWI0) Receive Holding Register */
  #define REG_TWI0_THR   (*(__O  uint32_t*)0x40000634U) /**< \brief (TWI0) Transmit Holding Register */
  #define REG_TWI0_SMBTR (*(__IO uint32_t*)0x40000638U) /**< \brief (TWI0) SMBus Timing Register */
  #define REG_TWI0_ACR   (*(__IO uint32_t*)0x40000640U) /**< \brief (TWI0) Alternative Command Register */
  #define REG_TWI0_FILTR (*(__IO uint32_t*)0x40000644U) /**< \brief (TWI0) Filter Register */
  #define REG_TWI0_SWMR  (*(__IO uint32_t*)0x4000064CU) /**< \brief (TWI0) SleepWalking Matching Register */
  #define REG_TWI0_FMR   (*(__IO uint32_t*)0x40000650U) /**< \brief (TWI0) FIFO Mode Register */
  #define REG_TWI0_FLR   (*(__I  uint32_t*)0x40000654U) /**< \brief (TWI0) FIFO Level Register */
  #define REG_TWI0_FSR   (*(__I  uint32_t*)0x40000660U) /**< \brief (TWI0) FIFO Status Register */
  #define REG_TWI0_FIER  (*(__O  uint32_t*)0x40000664U) /**< \brief (TWI0) FIFO Interrupt Enable Register */
  #define REG_TWI0_FIDR  (*(__O  uint32_t*)0x40000668U) /**< \brief (TWI0) FIFO Interrupt Disable Register */
  #define REG_TWI0_FIMR  (*(__I  uint32_t*)0x4000066CU) /**< \brief (TWI0) FIFO Interrupt Mask Register */
  #define REG_TWI0_DR    (*(__I  uint32_t*)0x400006D0U) /**< \brief (TWI0) Debug Register */
  #define REG_TWI0_WPMR  (*(__IO uint32_t*)0x400006E4U) /**< \brief (TWI0) Write Protection Mode Register */
  #define REG_TWI0_WPSR  (*(__I  uint32_t*)0x400006E8U) /**< \brief (TWI0) Write Protection Status Register */
  #define REG_TWI0_RPR   (*(__IO uint32_t*)0x40000700U) /**< \brief (TWI0) Receive Pointer Register */
  #define REG_TWI0_RCR   (*(__IO uint32_t*)0x40000704U) /**< \brief (TWI0) Receive Counter Register */
  #define REG_TWI0_TPR   (*(__IO uint32_t*)0x40000708U) /**< \brief (TWI0) Transmit Pointer Register */
  #define REG_TWI0_TCR   (*(__IO uint32_t*)0x4000070CU) /**< \brief (TWI0) Transmit Counter Register */
  #define REG_TWI0_RNPR  (*(__IO uint32_t*)0x40000710U) /**< \brief (TWI0) Receive Next Pointer Register */
  #define REG_TWI0_RNCR  (*(__IO uint32_t*)0x40000714U) /**< \brief (TWI0) Receive Next Counter Register */
  #define REG_TWI0_TNPR  (*(__IO uint32_t*)0x40000718U) /**< \brief (TWI0) Transmit Next Pointer Register */
  #define REG_TWI0_TNCR  (*(__IO uint32_t*)0x4000071CU) /**< \brief (TWI0) Transmit Next Counter Register */
  #define REG_TWI0_PTCR  (*(__O  uint32_t*)0x40000720U) /**< \brief (TWI0) Transfer Control Register */
  #define REG_TWI0_PTSR  (*(__I  uint32_t*)0x40000724U) /**< \brief (TWI0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TWI0_INSTANCE_ */
