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

#ifndef _PIC32CX_FLEXCOM0_INSTANCE_
#define _PIC32CX_FLEXCOM0_INSTANCE_

/* ========== Register definition for FLEXCOM0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM0_MR                           (0x40000000U) /**< \brief (FLEXCOM0) FLEXCOM Mode Register */
  #define REG_FLEXCOM0_RHR                          (0x40000010U) /**< \brief (FLEXCOM0) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM0_THR                          (0x40000020U) /**< \brief (FLEXCOM0) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM0_RPR                          (0x40000100U) /**< \brief (FLEXCOM0) Receive Pointer Register */
  #define REG_FLEXCOM0_RCR                          (0x40000104U) /**< \brief (FLEXCOM0) Receive Counter Register */
  #define REG_FLEXCOM0_TPR                          (0x40000108U) /**< \brief (FLEXCOM0) Transmit Pointer Register */
  #define REG_FLEXCOM0_TCR                          (0x4000010CU) /**< \brief (FLEXCOM0) Transmit Counter Register */
  #define REG_FLEXCOM0_RNPR                         (0x40000110U) /**< \brief (FLEXCOM0) Receive Next Pointer Register */
  #define REG_FLEXCOM0_RNCR                         (0x40000114U) /**< \brief (FLEXCOM0) Receive Next Counter Register */
  #define REG_FLEXCOM0_TNPR                         (0x40000118U) /**< \brief (FLEXCOM0) Transmit Next Pointer Register */
  #define REG_FLEXCOM0_TNCR                         (0x4000011CU) /**< \brief (FLEXCOM0) Transmit Next Counter Register */
  #define REG_FLEXCOM0_PTCR                         (0x40000120U) /**< \brief (FLEXCOM0) Transfer Control Register */
  #define REG_FLEXCOM0_PTSR                         (0x40000124U) /**< \brief (FLEXCOM0) Transfer Status Register */
  #define REG_FLEXCOM0_PWPMR                        (0x40000128U) /**< \brief (FLEXCOM0) Write Protection Mode Register */
#else
  #define REG_FLEXCOM0_MR          (*(__IO uint32_t*)0x40000000U) /**< \brief (FLEXCOM0) FLEXCOM Mode Register */
  #define REG_FLEXCOM0_RHR         (*(__I  uint32_t*)0x40000010U) /**< \brief (FLEXCOM0) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM0_THR         (*(__IO uint32_t*)0x40000020U) /**< \brief (FLEXCOM0) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM0_RPR         (*(__IO uint32_t*)0x40000100U) /**< \brief (FLEXCOM0) Receive Pointer Register */
  #define REG_FLEXCOM0_RCR         (*(__IO uint32_t*)0x40000104U) /**< \brief (FLEXCOM0) Receive Counter Register */
  #define REG_FLEXCOM0_TPR         (*(__IO uint32_t*)0x40000108U) /**< \brief (FLEXCOM0) Transmit Pointer Register */
  #define REG_FLEXCOM0_TCR         (*(__IO uint32_t*)0x4000010CU) /**< \brief (FLEXCOM0) Transmit Counter Register */
  #define REG_FLEXCOM0_RNPR        (*(__IO uint32_t*)0x40000110U) /**< \brief (FLEXCOM0) Receive Next Pointer Register */
  #define REG_FLEXCOM0_RNCR        (*(__IO uint32_t*)0x40000114U) /**< \brief (FLEXCOM0) Receive Next Counter Register */
  #define REG_FLEXCOM0_TNPR        (*(__IO uint32_t*)0x40000118U) /**< \brief (FLEXCOM0) Transmit Next Pointer Register */
  #define REG_FLEXCOM0_TNCR        (*(__IO uint32_t*)0x4000011CU) /**< \brief (FLEXCOM0) Transmit Next Counter Register */
  #define REG_FLEXCOM0_PTCR        (*(__O  uint32_t*)0x40000120U) /**< \brief (FLEXCOM0) Transfer Control Register */
  #define REG_FLEXCOM0_PTSR        (*(__I  uint32_t*)0x40000124U) /**< \brief (FLEXCOM0) Transfer Status Register */
  #define REG_FLEXCOM0_PWPMR       (*(__IO uint32_t*)0x40000128U) /**< \brief (FLEXCOM0) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM0_INSTANCE_ */
