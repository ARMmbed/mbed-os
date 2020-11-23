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

#ifndef _PIC32CX_TWI7_INSTANCE_
#define _PIC32CX_TWI7_INSTANCE_

/* ========== Register definition for TWI7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI7_CR                     (0x4001C600U) /**< \brief (TWI7) Control Register */
  #define REG_TWI7_MMR                    (0x4001C604U) /**< \brief (TWI7) Master Mode Register */
  #define REG_TWI7_SMR                    (0x4001C608U) /**< \brief (TWI7) Slave Mode Register */
  #define REG_TWI7_IADR                   (0x4001C60CU) /**< \brief (TWI7) Internal Address Register */
  #define REG_TWI7_CWGR                   (0x4001C610U) /**< \brief (TWI7) Clock Waveform Generator Register */
  #define REG_TWI7_SR                     (0x4001C620U) /**< \brief (TWI7) Status Register */
  #define REG_TWI7_IER                    (0x4001C624U) /**< \brief (TWI7) Interrupt Enable Register */
  #define REG_TWI7_IDR                    (0x4001C628U) /**< \brief (TWI7) Interrupt Disable Register */
  #define REG_TWI7_IMR                    (0x4001C62CU) /**< \brief (TWI7) Interrupt Mask Register */
  #define REG_TWI7_RHR                    (0x4001C630U) /**< \brief (TWI7) Receive Holding Register */
  #define REG_TWI7_THR                    (0x4001C634U) /**< \brief (TWI7) Transmit Holding Register */
  #define REG_TWI7_SMBTR                  (0x4001C638U) /**< \brief (TWI7) SMBus Timing Register */
  #define REG_TWI7_ACR                    (0x4001C640U) /**< \brief (TWI7) Alternative Command Register */
  #define REG_TWI7_FILTR                  (0x4001C644U) /**< \brief (TWI7) Filter Register */
  #define REG_TWI7_SWMR                   (0x4001C64CU) /**< \brief (TWI7) SleepWalking Matching Register */
  #define REG_TWI7_FMR                    (0x4001C650U) /**< \brief (TWI7) FIFO Mode Register */
  #define REG_TWI7_FLR                    (0x4001C654U) /**< \brief (TWI7) FIFO Level Register */
  #define REG_TWI7_FSR                    (0x4001C660U) /**< \brief (TWI7) FIFO Status Register */
  #define REG_TWI7_FIER                   (0x4001C664U) /**< \brief (TWI7) FIFO Interrupt Enable Register */
  #define REG_TWI7_FIDR                   (0x4001C668U) /**< \brief (TWI7) FIFO Interrupt Disable Register */
  #define REG_TWI7_FIMR                   (0x4001C66CU) /**< \brief (TWI7) FIFO Interrupt Mask Register */
  #define REG_TWI7_DR                     (0x4001C6D0U) /**< \brief (TWI7) Debug Register */
  #define REG_TWI7_WPMR                   (0x4001C6E4U) /**< \brief (TWI7) Write Protection Mode Register */
  #define REG_TWI7_WPSR                   (0x4001C6E8U) /**< \brief (TWI7) Write Protection Status Register */
  #define REG_TWI7_RPR                    (0x4001C700U) /**< \brief (TWI7) Receive Pointer Register */
  #define REG_TWI7_RCR                    (0x4001C704U) /**< \brief (TWI7) Receive Counter Register */
  #define REG_TWI7_TPR                    (0x4001C708U) /**< \brief (TWI7) Transmit Pointer Register */
  #define REG_TWI7_TCR                    (0x4001C70CU) /**< \brief (TWI7) Transmit Counter Register */
  #define REG_TWI7_RNPR                   (0x4001C710U) /**< \brief (TWI7) Receive Next Pointer Register */
  #define REG_TWI7_RNCR                   (0x4001C714U) /**< \brief (TWI7) Receive Next Counter Register */
  #define REG_TWI7_TNPR                   (0x4001C718U) /**< \brief (TWI7) Transmit Next Pointer Register */
  #define REG_TWI7_TNCR                   (0x4001C71CU) /**< \brief (TWI7) Transmit Next Counter Register */
  #define REG_TWI7_PTCR                   (0x4001C720U) /**< \brief (TWI7) Transfer Control Register */
  #define REG_TWI7_PTSR                   (0x4001C724U) /**< \brief (TWI7) Transfer Status Register */
#else
  #define REG_TWI7_CR    (*(__O  uint32_t*)0x4001C600U) /**< \brief (TWI7) Control Register */
  #define REG_TWI7_MMR   (*(__IO uint32_t*)0x4001C604U) /**< \brief (TWI7) Master Mode Register */
  #define REG_TWI7_SMR   (*(__IO uint32_t*)0x4001C608U) /**< \brief (TWI7) Slave Mode Register */
  #define REG_TWI7_IADR  (*(__IO uint32_t*)0x4001C60CU) /**< \brief (TWI7) Internal Address Register */
  #define REG_TWI7_CWGR  (*(__IO uint32_t*)0x4001C610U) /**< \brief (TWI7) Clock Waveform Generator Register */
  #define REG_TWI7_SR    (*(__I  uint32_t*)0x4001C620U) /**< \brief (TWI7) Status Register */
  #define REG_TWI7_IER   (*(__O  uint32_t*)0x4001C624U) /**< \brief (TWI7) Interrupt Enable Register */
  #define REG_TWI7_IDR   (*(__O  uint32_t*)0x4001C628U) /**< \brief (TWI7) Interrupt Disable Register */
  #define REG_TWI7_IMR   (*(__I  uint32_t*)0x4001C62CU) /**< \brief (TWI7) Interrupt Mask Register */
  #define REG_TWI7_RHR   (*(__I  uint32_t*)0x4001C630U) /**< \brief (TWI7) Receive Holding Register */
  #define REG_TWI7_THR   (*(__O  uint32_t*)0x4001C634U) /**< \brief (TWI7) Transmit Holding Register */
  #define REG_TWI7_SMBTR (*(__IO uint32_t*)0x4001C638U) /**< \brief (TWI7) SMBus Timing Register */
  #define REG_TWI7_ACR   (*(__IO uint32_t*)0x4001C640U) /**< \brief (TWI7) Alternative Command Register */
  #define REG_TWI7_FILTR (*(__IO uint32_t*)0x4001C644U) /**< \brief (TWI7) Filter Register */
  #define REG_TWI7_SWMR  (*(__IO uint32_t*)0x4001C64CU) /**< \brief (TWI7) SleepWalking Matching Register */
  #define REG_TWI7_FMR   (*(__IO uint32_t*)0x4001C650U) /**< \brief (TWI7) FIFO Mode Register */
  #define REG_TWI7_FLR   (*(__I  uint32_t*)0x4001C654U) /**< \brief (TWI7) FIFO Level Register */
  #define REG_TWI7_FSR   (*(__I  uint32_t*)0x4001C660U) /**< \brief (TWI7) FIFO Status Register */
  #define REG_TWI7_FIER  (*(__O  uint32_t*)0x4001C664U) /**< \brief (TWI7) FIFO Interrupt Enable Register */
  #define REG_TWI7_FIDR  (*(__O  uint32_t*)0x4001C668U) /**< \brief (TWI7) FIFO Interrupt Disable Register */
  #define REG_TWI7_FIMR  (*(__I  uint32_t*)0x4001C66CU) /**< \brief (TWI7) FIFO Interrupt Mask Register */
  #define REG_TWI7_DR    (*(__I  uint32_t*)0x4001C6D0U) /**< \brief (TWI7) Debug Register */
  #define REG_TWI7_WPMR  (*(__IO uint32_t*)0x4001C6E4U) /**< \brief (TWI7) Write Protection Mode Register */
  #define REG_TWI7_WPSR  (*(__I  uint32_t*)0x4001C6E8U) /**< \brief (TWI7) Write Protection Status Register */
  #define REG_TWI7_RPR   (*(__IO uint32_t*)0x4001C700U) /**< \brief (TWI7) Receive Pointer Register */
  #define REG_TWI7_RCR   (*(__IO uint32_t*)0x4001C704U) /**< \brief (TWI7) Receive Counter Register */
  #define REG_TWI7_TPR   (*(__IO uint32_t*)0x4001C708U) /**< \brief (TWI7) Transmit Pointer Register */
  #define REG_TWI7_TCR   (*(__IO uint32_t*)0x4001C70CU) /**< \brief (TWI7) Transmit Counter Register */
  #define REG_TWI7_RNPR  (*(__IO uint32_t*)0x4001C710U) /**< \brief (TWI7) Receive Next Pointer Register */
  #define REG_TWI7_RNCR  (*(__IO uint32_t*)0x4001C714U) /**< \brief (TWI7) Receive Next Counter Register */
  #define REG_TWI7_TNPR  (*(__IO uint32_t*)0x4001C718U) /**< \brief (TWI7) Transmit Next Pointer Register */
  #define REG_TWI7_TNCR  (*(__IO uint32_t*)0x4001C71CU) /**< \brief (TWI7) Transmit Next Counter Register */
  #define REG_TWI7_PTCR  (*(__O  uint32_t*)0x4001C720U) /**< \brief (TWI7) Transfer Control Register */
  #define REG_TWI7_PTSR  (*(__I  uint32_t*)0x4001C724U) /**< \brief (TWI7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TWI7_INSTANCE_ */
