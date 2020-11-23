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

#ifndef _PIC32CX_TWI1_INSTANCE_
#define _PIC32CX_TWI1_INSTANCE_

/* ========== Register definition for TWI1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI1_CR                     (0x40004600U) /**< \brief (TWI1) Control Register */
  #define REG_TWI1_MMR                    (0x40004604U) /**< \brief (TWI1) Master Mode Register */
  #define REG_TWI1_SMR                    (0x40004608U) /**< \brief (TWI1) Slave Mode Register */
  #define REG_TWI1_IADR                   (0x4000460CU) /**< \brief (TWI1) Internal Address Register */
  #define REG_TWI1_CWGR                   (0x40004610U) /**< \brief (TWI1) Clock Waveform Generator Register */
  #define REG_TWI1_SR                     (0x40004620U) /**< \brief (TWI1) Status Register */
  #define REG_TWI1_IER                    (0x40004624U) /**< \brief (TWI1) Interrupt Enable Register */
  #define REG_TWI1_IDR                    (0x40004628U) /**< \brief (TWI1) Interrupt Disable Register */
  #define REG_TWI1_IMR                    (0x4000462CU) /**< \brief (TWI1) Interrupt Mask Register */
  #define REG_TWI1_RHR                    (0x40004630U) /**< \brief (TWI1) Receive Holding Register */
  #define REG_TWI1_THR                    (0x40004634U) /**< \brief (TWI1) Transmit Holding Register */
  #define REG_TWI1_SMBTR                  (0x40004638U) /**< \brief (TWI1) SMBus Timing Register */
  #define REG_TWI1_ACR                    (0x40004640U) /**< \brief (TWI1) Alternative Command Register */
  #define REG_TWI1_FILTR                  (0x40004644U) /**< \brief (TWI1) Filter Register */
  #define REG_TWI1_SWMR                   (0x4000464CU) /**< \brief (TWI1) SleepWalking Matching Register */
  #define REG_TWI1_FMR                    (0x40004650U) /**< \brief (TWI1) FIFO Mode Register */
  #define REG_TWI1_FLR                    (0x40004654U) /**< \brief (TWI1) FIFO Level Register */
  #define REG_TWI1_FSR                    (0x40004660U) /**< \brief (TWI1) FIFO Status Register */
  #define REG_TWI1_FIER                   (0x40004664U) /**< \brief (TWI1) FIFO Interrupt Enable Register */
  #define REG_TWI1_FIDR                   (0x40004668U) /**< \brief (TWI1) FIFO Interrupt Disable Register */
  #define REG_TWI1_FIMR                   (0x4000466CU) /**< \brief (TWI1) FIFO Interrupt Mask Register */
  #define REG_TWI1_DR                     (0x400046D0U) /**< \brief (TWI1) Debug Register */
  #define REG_TWI1_WPMR                   (0x400046E4U) /**< \brief (TWI1) Write Protection Mode Register */
  #define REG_TWI1_WPSR                   (0x400046E8U) /**< \brief (TWI1) Write Protection Status Register */
  #define REG_TWI1_RPR                    (0x40004700U) /**< \brief (TWI1) Receive Pointer Register */
  #define REG_TWI1_RCR                    (0x40004704U) /**< \brief (TWI1) Receive Counter Register */
  #define REG_TWI1_TPR                    (0x40004708U) /**< \brief (TWI1) Transmit Pointer Register */
  #define REG_TWI1_TCR                    (0x4000470CU) /**< \brief (TWI1) Transmit Counter Register */
  #define REG_TWI1_RNPR                   (0x40004710U) /**< \brief (TWI1) Receive Next Pointer Register */
  #define REG_TWI1_RNCR                   (0x40004714U) /**< \brief (TWI1) Receive Next Counter Register */
  #define REG_TWI1_TNPR                   (0x40004718U) /**< \brief (TWI1) Transmit Next Pointer Register */
  #define REG_TWI1_TNCR                   (0x4000471CU) /**< \brief (TWI1) Transmit Next Counter Register */
  #define REG_TWI1_PTCR                   (0x40004720U) /**< \brief (TWI1) Transfer Control Register */
  #define REG_TWI1_PTSR                   (0x40004724U) /**< \brief (TWI1) Transfer Status Register */
#else
  #define REG_TWI1_CR    (*(__O  uint32_t*)0x40004600U) /**< \brief (TWI1) Control Register */
  #define REG_TWI1_MMR   (*(__IO uint32_t*)0x40004604U) /**< \brief (TWI1) Master Mode Register */
  #define REG_TWI1_SMR   (*(__IO uint32_t*)0x40004608U) /**< \brief (TWI1) Slave Mode Register */
  #define REG_TWI1_IADR  (*(__IO uint32_t*)0x4000460CU) /**< \brief (TWI1) Internal Address Register */
  #define REG_TWI1_CWGR  (*(__IO uint32_t*)0x40004610U) /**< \brief (TWI1) Clock Waveform Generator Register */
  #define REG_TWI1_SR    (*(__I  uint32_t*)0x40004620U) /**< \brief (TWI1) Status Register */
  #define REG_TWI1_IER   (*(__O  uint32_t*)0x40004624U) /**< \brief (TWI1) Interrupt Enable Register */
  #define REG_TWI1_IDR   (*(__O  uint32_t*)0x40004628U) /**< \brief (TWI1) Interrupt Disable Register */
  #define REG_TWI1_IMR   (*(__I  uint32_t*)0x4000462CU) /**< \brief (TWI1) Interrupt Mask Register */
  #define REG_TWI1_RHR   (*(__I  uint32_t*)0x40004630U) /**< \brief (TWI1) Receive Holding Register */
  #define REG_TWI1_THR   (*(__O  uint32_t*)0x40004634U) /**< \brief (TWI1) Transmit Holding Register */
  #define REG_TWI1_SMBTR (*(__IO uint32_t*)0x40004638U) /**< \brief (TWI1) SMBus Timing Register */
  #define REG_TWI1_ACR   (*(__IO uint32_t*)0x40004640U) /**< \brief (TWI1) Alternative Command Register */
  #define REG_TWI1_FILTR (*(__IO uint32_t*)0x40004644U) /**< \brief (TWI1) Filter Register */
  #define REG_TWI1_SWMR  (*(__IO uint32_t*)0x4000464CU) /**< \brief (TWI1) SleepWalking Matching Register */
  #define REG_TWI1_FMR   (*(__IO uint32_t*)0x40004650U) /**< \brief (TWI1) FIFO Mode Register */
  #define REG_TWI1_FLR   (*(__I  uint32_t*)0x40004654U) /**< \brief (TWI1) FIFO Level Register */
  #define REG_TWI1_FSR   (*(__I  uint32_t*)0x40004660U) /**< \brief (TWI1) FIFO Status Register */
  #define REG_TWI1_FIER  (*(__O  uint32_t*)0x40004664U) /**< \brief (TWI1) FIFO Interrupt Enable Register */
  #define REG_TWI1_FIDR  (*(__O  uint32_t*)0x40004668U) /**< \brief (TWI1) FIFO Interrupt Disable Register */
  #define REG_TWI1_FIMR  (*(__I  uint32_t*)0x4000466CU) /**< \brief (TWI1) FIFO Interrupt Mask Register */
  #define REG_TWI1_DR    (*(__I  uint32_t*)0x400046D0U) /**< \brief (TWI1) Debug Register */
  #define REG_TWI1_WPMR  (*(__IO uint32_t*)0x400046E4U) /**< \brief (TWI1) Write Protection Mode Register */
  #define REG_TWI1_WPSR  (*(__I  uint32_t*)0x400046E8U) /**< \brief (TWI1) Write Protection Status Register */
  #define REG_TWI1_RPR   (*(__IO uint32_t*)0x40004700U) /**< \brief (TWI1) Receive Pointer Register */
  #define REG_TWI1_RCR   (*(__IO uint32_t*)0x40004704U) /**< \brief (TWI1) Receive Counter Register */
  #define REG_TWI1_TPR   (*(__IO uint32_t*)0x40004708U) /**< \brief (TWI1) Transmit Pointer Register */
  #define REG_TWI1_TCR   (*(__IO uint32_t*)0x4000470CU) /**< \brief (TWI1) Transmit Counter Register */
  #define REG_TWI1_RNPR  (*(__IO uint32_t*)0x40004710U) /**< \brief (TWI1) Receive Next Pointer Register */
  #define REG_TWI1_RNCR  (*(__IO uint32_t*)0x40004714U) /**< \brief (TWI1) Receive Next Counter Register */
  #define REG_TWI1_TNPR  (*(__IO uint32_t*)0x40004718U) /**< \brief (TWI1) Transmit Next Pointer Register */
  #define REG_TWI1_TNCR  (*(__IO uint32_t*)0x4000471CU) /**< \brief (TWI1) Transmit Next Counter Register */
  #define REG_TWI1_PTCR  (*(__O  uint32_t*)0x40004720U) /**< \brief (TWI1) Transfer Control Register */
  #define REG_TWI1_PTSR  (*(__I  uint32_t*)0x40004724U) /**< \brief (TWI1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_TWI1_INSTANCE_ */
