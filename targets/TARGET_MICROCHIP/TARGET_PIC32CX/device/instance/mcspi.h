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

#ifndef _PIC32CX_MCSPI_INSTANCE_
#define _PIC32CX_MCSPI_INSTANCE_

/* ========== Register definition for MCSPI peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MCSPI_CR                     (0x48018000U) /**< \brief (MCSPI) Control Register */
  #define REG_MCSPI_MR                     (0x48018004U) /**< \brief (MCSPI) Mode Register */
  #define REG_MCSPI_RDR                    (0x48018008U) /**< \brief (MCSPI) Receive Data Register */
  #define REG_MCSPI_TDR                    (0x4801800CU) /**< \brief (MCSPI) Transmit Data Register */
  #define REG_MCSPI_SR                     (0x48018010U) /**< \brief (MCSPI) Status Register */
  #define REG_MCSPI_IER                    (0x48018014U) /**< \brief (MCSPI) Interrupt Enable Register */
  #define REG_MCSPI_IDR                    (0x48018018U) /**< \brief (MCSPI) Interrupt Disable Register */
  #define REG_MCSPI_IMR                    (0x4801801CU) /**< \brief (MCSPI) Interrupt Mask Register */
  #define REG_MCSPI_CSR                    (0x48018030U) /**< \brief (MCSPI) Chip Select Register (CS_number = 0) */
  #define REG_MCSPI_FMR                    (0x48018040U) /**< \brief (MCSPI) FIFO Mode Register */
  #define REG_MCSPI_FLR                    (0x48018044U) /**< \brief (MCSPI) FIFO Level Register */
  #define REG_MCSPI_CMPR                   (0x48018048U) /**< \brief (MCSPI) Comparison Register */
  #define REG_MCSPI_CRCR                   (0x4801804CU) /**< \brief (MCSPI) CRC Register */
  #define REG_MCSPI_TPMR                   (0x48018050U) /**< \brief (MCSPI) Two-Pin Mode Register */
  #define REG_MCSPI_TPHR                   (0x48018054U) /**< \brief (MCSPI) Two-Pin Header Register */
  #define REG_MCSPI_WPMR                   (0x480180E4U) /**< \brief (MCSPI) Write Protection Mode Register */
  #define REG_MCSPI_WPSR                   (0x480180E8U) /**< \brief (MCSPI) Write Protection Status Register */
  #define REG_MCSPI_RPR                    (0x48018100U) /**< \brief (MCSPI) Receive Pointer Register */
  #define REG_MCSPI_RCR                    (0x48018104U) /**< \brief (MCSPI) Receive Counter Register */
  #define REG_MCSPI_TPR                    (0x48018108U) /**< \brief (MCSPI) Transmit Pointer Register */
  #define REG_MCSPI_TCR                    (0x4801810CU) /**< \brief (MCSPI) Transmit Counter Register */
  #define REG_MCSPI_RNPR                   (0x48018110U) /**< \brief (MCSPI) Receive Next Pointer Register */
  #define REG_MCSPI_RNCR                   (0x48018114U) /**< \brief (MCSPI) Receive Next Counter Register */
  #define REG_MCSPI_TNPR                   (0x48018118U) /**< \brief (MCSPI) Transmit Next Pointer Register */
  #define REG_MCSPI_TNCR                   (0x4801811CU) /**< \brief (MCSPI) Transmit Next Counter Register */
  #define REG_MCSPI_PTCR                   (0x48018120U) /**< \brief (MCSPI) Transfer Control Register */
  #define REG_MCSPI_PTSR                   (0x48018124U) /**< \brief (MCSPI) Transfer Status Register */
  #define REG_MCSPI_PWPMR                  (0x48018128U) /**< \brief (MCSPI) Write Protection Mode Register */
#else
  #define REG_MCSPI_CR    (*(__O  uint32_t*)0x48018000U) /**< \brief (MCSPI) Control Register */
  #define REG_MCSPI_MR    (*(__IO uint32_t*)0x48018004U) /**< \brief (MCSPI) Mode Register */
  #define REG_MCSPI_RDR   (*(__I  uint32_t*)0x48018008U) /**< \brief (MCSPI) Receive Data Register */
  #define REG_MCSPI_TDR   (*(__O  uint32_t*)0x4801800CU) /**< \brief (MCSPI) Transmit Data Register */
  #define REG_MCSPI_SR    (*(__I  uint32_t*)0x48018010U) /**< \brief (MCSPI) Status Register */
  #define REG_MCSPI_IER   (*(__O  uint32_t*)0x48018014U) /**< \brief (MCSPI) Interrupt Enable Register */
  #define REG_MCSPI_IDR   (*(__O  uint32_t*)0x48018018U) /**< \brief (MCSPI) Interrupt Disable Register */
  #define REG_MCSPI_IMR   (*(__I  uint32_t*)0x4801801CU) /**< \brief (MCSPI) Interrupt Mask Register */
  #define REG_MCSPI_CSR   (*(__IO uint32_t*)0x48018030U) /**< \brief (MCSPI) Chip Select Register (CS_number = 0) */
  #define REG_MCSPI_FMR   (*(__IO uint32_t*)0x48018040U) /**< \brief (MCSPI) FIFO Mode Register */
  #define REG_MCSPI_FLR   (*(__I  uint32_t*)0x48018044U) /**< \brief (MCSPI) FIFO Level Register */
  #define REG_MCSPI_CMPR  (*(__IO uint32_t*)0x48018048U) /**< \brief (MCSPI) Comparison Register */
  #define REG_MCSPI_CRCR  (*(__IO uint32_t*)0x4801804CU) /**< \brief (MCSPI) CRC Register */
  #define REG_MCSPI_TPMR  (*(__IO uint32_t*)0x48018050U) /**< \brief (MCSPI) Two-Pin Mode Register */
  #define REG_MCSPI_TPHR  (*(__I  uint32_t*)0x48018054U) /**< \brief (MCSPI) Two-Pin Header Register */
  #define REG_MCSPI_WPMR  (*(__IO uint32_t*)0x480180E4U) /**< \brief (MCSPI) Write Protection Mode Register */
  #define REG_MCSPI_WPSR  (*(__I  uint32_t*)0x480180E8U) /**< \brief (MCSPI) Write Protection Status Register */
  #define REG_MCSPI_RPR   (*(__IO uint32_t*)0x48018100U) /**< \brief (MCSPI) Receive Pointer Register */
  #define REG_MCSPI_RCR   (*(__IO uint32_t*)0x48018104U) /**< \brief (MCSPI) Receive Counter Register */
  #define REG_MCSPI_TPR   (*(__IO uint32_t*)0x48018108U) /**< \brief (MCSPI) Transmit Pointer Register */
  #define REG_MCSPI_TCR   (*(__IO uint32_t*)0x4801810CU) /**< \brief (MCSPI) Transmit Counter Register */
  #define REG_MCSPI_RNPR  (*(__IO uint32_t*)0x48018110U) /**< \brief (MCSPI) Receive Next Pointer Register */
  #define REG_MCSPI_RNCR  (*(__IO uint32_t*)0x48018114U) /**< \brief (MCSPI) Receive Next Counter Register */
  #define REG_MCSPI_TNPR  (*(__IO uint32_t*)0x48018118U) /**< \brief (MCSPI) Transmit Next Pointer Register */
  #define REG_MCSPI_TNCR  (*(__IO uint32_t*)0x4801811CU) /**< \brief (MCSPI) Transmit Next Counter Register */
  #define REG_MCSPI_PTCR  (*(__O  uint32_t*)0x48018120U) /**< \brief (MCSPI) Transfer Control Register */
  #define REG_MCSPI_PTSR  (*(__I  uint32_t*)0x48018124U) /**< \brief (MCSPI) Transfer Status Register */
  #define REG_MCSPI_PWPMR (*(__IO uint32_t*)0x48018128U) /**< \brief (MCSPI) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MCSPI_INSTANCE_ */
