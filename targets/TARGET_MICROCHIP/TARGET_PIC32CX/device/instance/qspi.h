/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Microchip Technology Inc.                    */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

#ifndef _PIC32CX_QSPI_INSTANCE_
#define _PIC32CX_QSPI_INSTANCE_

/* ========== Register definition for QSPI peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_QSPI_CR                      (0x40020000U) /**< \brief (QSPI) Control Register */
  #define REG_QSPI_MR                      (0x40020004U) /**< \brief (QSPI) Mode Register */
  #define REG_QSPI_RDR                     (0x40020008U) /**< \brief (QSPI) Receive Data Register */
  #define REG_QSPI_TDR                     (0x4002000CU) /**< \brief (QSPI) Transmit Data Register */
  #define REG_QSPI_ISR                     (0x40020010U) /**< \brief (QSPI) Interrupt Status Register */
  #define REG_QSPI_IER                     (0x40020014U) /**< \brief (QSPI) Interrupt Enable Register */
  #define REG_QSPI_IDR                     (0x40020018U) /**< \brief (QSPI) Interrupt Disable Register */
  #define REG_QSPI_IMR                     (0x4002001CU) /**< \brief (QSPI) Interrupt Mask Register */
  #define REG_QSPI_SCR                     (0x40020020U) /**< \brief (QSPI) Serial Clock Register */
  #define REG_QSPI_SR                      (0x40020024U) /**< \brief (QSPI) Status Register */
  #define REG_QSPI_IAR                     (0x40020030U) /**< \brief (QSPI) Instruction Address Register */
  #define REG_QSPI_WICR                    (0x40020034U) /**< \brief (QSPI) Write Instruction Code Register */
  #define REG_QSPI_IFR                     (0x40020038U) /**< \brief (QSPI) Instruction Frame Register */
  #define REG_QSPI_RICR                    (0x4002003CU) /**< \brief (QSPI) Read Instruction Code Register */
  #define REG_QSPI_SMR                     (0x40020040U) /**< \brief (QSPI) Scrambling Mode Register */
  #define REG_QSPI_SKR                     (0x40020044U) /**< \brief (QSPI) Scrambling Key Register */
  #define REG_QSPI_WRACNT                  (0x40020054U) /**< \brief (QSPI) Write Access Counter Register */
  #define REG_QSPI_TOUT                    (0x40020064U) /**< \brief (QSPI) Timeout Register */
  #define REG_QSPI_WPMR                    (0x400200E4U) /**< \brief (QSPI) Write Protection Mode Register */
  #define REG_QSPI_WPSR                    (0x400200E8U) /**< \brief (QSPI) Write Protection Status Register */
  #define REG_QSPI_RPR                     (0x40020100U) /**< \brief (QSPI) Receive Pointer Register */
  #define REG_QSPI_RCR                     (0x40020104U) /**< \brief (QSPI) Receive Counter Register */
  #define REG_QSPI_TPR                     (0x40020108U) /**< \brief (QSPI) Transmit Pointer Register */
  #define REG_QSPI_TCR                     (0x4002010CU) /**< \brief (QSPI) Transmit Counter Register */
  #define REG_QSPI_RNPR                    (0x40020110U) /**< \brief (QSPI) Receive Next Pointer Register */
  #define REG_QSPI_RNCR                    (0x40020114U) /**< \brief (QSPI) Receive Next Counter Register */
  #define REG_QSPI_TNPR                    (0x40020118U) /**< \brief (QSPI) Transmit Next Pointer Register */
  #define REG_QSPI_TNCR                    (0x4002011CU) /**< \brief (QSPI) Transmit Next Counter Register */
  #define REG_QSPI_PTCR                    (0x40020120U) /**< \brief (QSPI) Transfer Control Register */
  #define REG_QSPI_PTSR                    (0x40020124U) /**< \brief (QSPI) Transfer Status Register */
  #define REG_QSPI_PWPMR                   (0x40020128U) /**< \brief (QSPI) Write Protection Mode Register */
#else
  #define REG_QSPI_CR     (*(__O  uint32_t*)0x40020000U) /**< \brief (QSPI) Control Register */
  #define REG_QSPI_MR     (*(__IO uint32_t*)0x40020004U) /**< \brief (QSPI) Mode Register */
  #define REG_QSPI_RDR    (*(__I  uint32_t*)0x40020008U) /**< \brief (QSPI) Receive Data Register */
  #define REG_QSPI_TDR    (*(__O  uint32_t*)0x4002000CU) /**< \brief (QSPI) Transmit Data Register */
  #define REG_QSPI_ISR    (*(__I  uint32_t*)0x40020010U) /**< \brief (QSPI) Interrupt Status Register */
  #define REG_QSPI_IER    (*(__O  uint32_t*)0x40020014U) /**< \brief (QSPI) Interrupt Enable Register */
  #define REG_QSPI_IDR    (*(__O  uint32_t*)0x40020018U) /**< \brief (QSPI) Interrupt Disable Register */
  #define REG_QSPI_IMR    (*(__I  uint32_t*)0x4002001CU) /**< \brief (QSPI) Interrupt Mask Register */
  #define REG_QSPI_SCR    (*(__IO uint32_t*)0x40020020U) /**< \brief (QSPI) Serial Clock Register */
  #define REG_QSPI_SR     (*(__IO uint32_t*)0x40020024U) /**< \brief (QSPI) Status Register */
  #define REG_QSPI_IAR    (*(__IO uint32_t*)0x40020030U) /**< \brief (QSPI) Instruction Address Register */
  #define REG_QSPI_WICR   (*(__IO uint32_t*)0x40020034U) /**< \brief (QSPI) Write Instruction Code Register */
  #define REG_QSPI_IFR    (*(__IO uint32_t*)0x40020038U) /**< \brief (QSPI) Instruction Frame Register */
  #define REG_QSPI_RICR   (*(__IO uint32_t*)0x4002003CU) /**< \brief (QSPI) Read Instruction Code Register */
  #define REG_QSPI_SMR    (*(__IO uint32_t*)0x40020040U) /**< \brief (QSPI) Scrambling Mode Register */
  #define REG_QSPI_SKR    (*(__O  uint32_t*)0x40020044U) /**< \brief (QSPI) Scrambling Key Register */
  #define REG_QSPI_WRACNT (*(__IO uint32_t*)0x40020054U) /**< \brief (QSPI) Write Access Counter Register */
  #define REG_QSPI_TOUT   (*(__IO uint32_t*)0x40020064U) /**< \brief (QSPI) Timeout Register */
  #define REG_QSPI_WPMR   (*(__IO uint32_t*)0x400200E4U) /**< \brief (QSPI) Write Protection Mode Register */
  #define REG_QSPI_WPSR   (*(__I  uint32_t*)0x400200E8U) /**< \brief (QSPI) Write Protection Status Register */
  #define REG_QSPI_RPR    (*(__IO uint32_t*)0x40020100U) /**< \brief (QSPI) Receive Pointer Register */
  #define REG_QSPI_RCR    (*(__IO uint32_t*)0x40020104U) /**< \brief (QSPI) Receive Counter Register */
  #define REG_QSPI_TPR    (*(__IO uint32_t*)0x40020108U) /**< \brief (QSPI) Transmit Pointer Register */
  #define REG_QSPI_TCR    (*(__IO uint32_t*)0x4002010CU) /**< \brief (QSPI) Transmit Counter Register */
  #define REG_QSPI_RNPR   (*(__IO uint32_t*)0x40020110U) /**< \brief (QSPI) Receive Next Pointer Register */
  #define REG_QSPI_RNCR   (*(__IO uint32_t*)0x40020114U) /**< \brief (QSPI) Receive Next Counter Register */
  #define REG_QSPI_TNPR   (*(__IO uint32_t*)0x40020118U) /**< \brief (QSPI) Transmit Next Pointer Register */
  #define REG_QSPI_TNCR   (*(__IO uint32_t*)0x4002011CU) /**< \brief (QSPI) Transmit Next Counter Register */
  #define REG_QSPI_PTCR   (*(__O  uint32_t*)0x40020120U) /**< \brief (QSPI) Transfer Control Register */
  #define REG_QSPI_PTSR   (*(__I  uint32_t*)0x40020124U) /**< \brief (QSPI) Transfer Status Register */
  #define REG_QSPI_PWPMR  (*(__IO uint32_t*)0x40020128U) /**< \brief (QSPI) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_QSPI_INSTANCE_ */
