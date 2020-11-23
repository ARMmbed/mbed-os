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

#ifndef _PIC32CX_TWI3_INSTANCE_
#define _PIC32CX_TWI3_INSTANCE_

/* ========== Register definition for TWI3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI3_CR                     (0x4000C600U) /**< \brief (TWI3) Control Register */
  #define REG_TWI3_MMR                    (0x4000C604U) /**< \brief (TWI3) Master Mode Register */
  #define REG_TWI3_SMR                    (0x4000C608U) /**< \brief (TWI3) Slave Mode Register */
  #define REG_TWI3_IADR                   (0x4000C60CU) /**< \brief (TWI3) Internal Address Register */
  #define REG_TWI3_CWGR                   (0x4000C610U) /**< \brief (TWI3) Clock Waveform Generator Register */
  #define REG_TWI3_SR                     (0x4000C620U) /**< \brief (TWI3) Status Register */
  #define REG_TWI3_IER                    (0x4000C624U) /**< \brief (TWI3) Interrupt Enable Register */
  #define REG_TWI3_IDR                    (0x4000C628U) /**< \brief (TWI3) Interrupt Disable Register */
  #define REG_TWI3_IMR                    (0x4000C62CU) /**< \brief (TWI3) Interrupt Mask Register */
  #define REG_TWI3_RHR                    (0x4000C630U) /**< \brief (TWI3) Receive Holding Register */
  #define REG_TWI3_THR                    (0x4000C634U) /**< \brief (TWI3) Transmit Holding Register */
  #define REG_TWI3_SMBTR                  (0x4000C638U) /**< \brief (TWI3) SMBus Timing Register */
  #define REG_TWI3_ACR                    (0x4000C640U) /**< \brief (TWI3) Alternative Command Register */
  #define REG_TWI3_FILTR                  (0x4000C644U) /**< \brief (TWI3) Filter Register */
  #define REG_TWI3_SWMR                   (0x4000C64CU) /**< \brief (TWI3) SleepWalking Matching Register */
  #define REG_TWI3_FMR                    (0x4000C650U) /**< \brief (TWI3) FIFO Mode Register */
  #define REG_TWI3_FLR                    (0x4000C654U) /**< \brief (TWI3) FIFO Level Register */
  #define REG_TWI3_FSR                    (0x4000C660U) /**< \brief (TWI3) FIFO Status Register */
  #define REG_TWI3_FIER                   (0x4000C664U) /**< \brief (TWI3) FIFO Interrupt Enable Register */
  #define REG_TWI3_FIDR                   (0x4000C668U) /**< \brief (TWI3) FIFO Interrupt Disable Register */
  #define REG_TWI3_FIMR                   (0x4000C66CU) /**< \brief (TWI3) FIFO Interrupt Mask Register */
  #define REG_TWI3_DR                     (0x4000C6D0U) /**< \brief (TWI3) Debug Register */
  #define REG_TWI3_WPMR                   (0x4000C6E4U) /**< \brief (TWI3) Write Protection Mode Register */
  #define REG_TWI3_WPSR                   (0x4000C6E8U) /**< \brief (TWI3) Write Protection Status Register */
  #define REG_TWI3_RPR                    (0x4000C700U) /**< \brief (TWI3) Receive Pointer Register */
  #define REG_TWI3_RCR                    (0x4000C704U) /**< \brief (TWI3) Receive Counter Register */
  #define REG_TWI3_TPR                    (0x4000C708U) /**< \brief (TWI3) Transmit Pointer Register */
  #define REG_TWI3_TCR                    (0x4000C70CU) /**< \brief (TWI3) Transmit Counter Register */
  #define REG_TWI3_RNPR                   (0x4000C710U) /**< \brief (TWI3) Receive Next Pointer Register */
  #define REG_TWI3_RNCR                   (0x4000C714U) /**< \brief (TWI3) Receive Next Counter Register */
  #define REG_TWI3_TNPR                   (0x4000C718U) /**< \brief (TWI3) Transmit Next Pointer Register */
  #define REG_TWI3_TNCR                   (0x4000C71CU) /**< \brief (TWI3) Transmit Next Counter Register */
  #define REG_TWI3_PTCR                   (0x4000C720U) /**< \brief (TWI3) Transfer Control Register */
  #define REG_TWI3_PTSR                   (0x4000C724U) /**< \brief (TWI3) Transfer Status Register */
#else
  #define REG_TWI3_CR    (*(__O  uint32_t*)0x4000C600U) /**< \brief (TWI3) Control Register */
  #define REG_TWI3_MMR   (*(__IO uint32_t*)0x4000C604U) /**< \brief (TWI3) Master Mode Register */
  #define REG_TWI3_SMR   (*(__IO uint32_t*)0x4000C608U) /**< \brief (TWI3) Slave Mode Register */
  #define REG_TWI3_IADR  (*(__IO uint32_t*)0x4000C60CU) /**< \brief (TWI3) Internal Address Register */
  #define REG_TWI3_CWGR  (*(__IO uint32_t*)0x4000C610U) /**< \brief (TWI3) Clock Waveform Generator Register */
  #define REG_TWI3_SR    (*(__I  uint32_t*)0x4000C620U) /**< \brief (TWI3) Status Register */
  #define REG_TWI3_IER   (*(__O  uint32_t*)0x4000C624U) /**< \brief (TWI3) Interrupt Enable Register */
  #define REG_TWI3_IDR   (*(__O  uint32_t*)0x4000C628U) /**< \brief (TWI3) Interrupt Disable Register */
  #define REG_TWI3_IMR   (*(__I  uint32_t*)0x4000C62CU) /**< \brief (TWI3) Interrupt Mask Register */
  #define REG_TWI3_RHR   (*(__I  uint32_t*)0x4000C630U) /**< \brief (TWI3) Receive Holding Register */
  #define REG_TWI3_THR   (*(__O  uint32_t*)0x4000C634U) /**< \brief (TWI3) Transmit Holding Register */
  #define REG_TWI3_SMBTR (*(__IO uint32_t*)0x4000C638U) /**< \brief (TWI3) SMBus Timing Register */
  #define REG_TWI3_ACR   (*(__IO uint32_t*)0x4000C640U) /**< \brief (TWI3) Alternative Command Register */
  #define REG_TWI3_FILTR (*(__IO uint32_t*)0x4000C644U) /**< \brief (TWI3) Filter Register */
  #define REG_TWI3_SWMR  (*(__IO uint32_t*)0x4000C64CU) /**< \brief (TWI3) SleepWalking Matching Register */
  #define REG_TWI3_FMR   (*(__IO uint32_t*)0x4000C650U) /**< \brief (TWI3) FIFO Mode Register */
  #define REG_TWI3_FLR   (*(__I  uint32_t*)0x4000C654U) /**< \brief (TWI3) FIFO Level Register */
  #define REG_TWI3_FSR   (*(__I  uint32_t*)0x4000C660U) /**< \brief (TWI3) FIFO Status Register */
  #define REG_TWI3_FIER  (*(__O  uint32_t*)0x4000C664U) /**< \brief (TWI3) FIFO Interrupt Enable Register */
  #define REG_TWI3_FIDR  (*(__O  uint32_t*)0x4000C668U) /**< \brief (TWI3) FIFO Interrupt Disable Register */
  #define REG_TWI3_FIMR  (*(__I  uint32_t*)0x4000C66CU) /**< \brief (TWI3) FIFO Interrupt Mask Register */
  #define REG_TWI3_DR    (*(__I  uint32_t*)0x4000C6D0U) /**< \brief (TWI3) Debug Register */
  #define REG_TWI3_WPMR  (*(__IO uint32_t*)0x4000C6E4U) /**< \brief (TWI3) Write Protection Mode Register */
  #define REG_TWI3_WPSR  (*(__I  uint32_t*)0x4000C6E8U) /**< \brief (TWI3) Write Protection Status Register */
  #define REG_TWI3_RPR   (*(__IO uint32_t*)0x4000C700U) /**< \brief (TWI3) Receive Pointer Register */
  #define REG_TWI3_RCR   (*(__IO uint32_t*)0x4000C704U) /**< \brief (TWI3) Receive Counter Register */
  #define REG_TWI3_TPR   (*(__IO uint32_t*)0x4000C708U) /**< \brief (TWI3) Transmit Pointer Register */
  #define REG_TWI3_TCR   (*(__IO uint32_t*)0x4000C70CU) /**< \brief (TWI3) Transmit Counter Register */
  #define REG_TWI3_RNPR  (*(__IO uint32_t*)0x4000C710U) /**< \brief (TWI3) Receive Next Pointer Register */
  #define REG_TWI3_RNCR  (*(__IO uint32_t*)0x4000C714U) /**< \brief (TWI3) Receive Next Counter Register */
  #define REG_TWI3_TNPR  (*(__IO uint32_t*)0x4000C718U) /**< \brief (TWI3) Transmit Next Pointer Register */
  #define REG_TWI3_TNCR  (*(__IO uint32_t*)0x4000C71CU) /**< \brief (TWI3) Transmit Next Counter Register */
  #define REG_TWI3_PTCR  (*(__O  uint32_t*)0x4000C720U) /**< \brief (TWI3) Transfer Control Register */
  #define REG_TWI3_PTSR  (*(__I  uint32_t*)0x4000C724U) /**< \brief (TWI3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TWI3_INSTANCE_ */
