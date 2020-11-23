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

#ifndef _PIC32CX_FLEXCOM4_INSTANCE_
#define _PIC32CX_FLEXCOM4_INSTANCE_

/* ========== Register definition for FLEXCOM4 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM4_MR                           (0x40010000U) /**< \brief (FLEXCOM4) FLEXCOM Mode Register */
  #define REG_FLEXCOM4_RHR                          (0x40010010U) /**< \brief (FLEXCOM4) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM4_THR                          (0x40010020U) /**< \brief (FLEXCOM4) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM4_RPR                          (0x40010100U) /**< \brief (FLEXCOM4) Receive Pointer Register */
  #define REG_FLEXCOM4_RCR                          (0x40010104U) /**< \brief (FLEXCOM4) Receive Counter Register */
  #define REG_FLEXCOM4_TPR                          (0x40010108U) /**< \brief (FLEXCOM4) Transmit Pointer Register */
  #define REG_FLEXCOM4_TCR                          (0x4001010CU) /**< \brief (FLEXCOM4) Transmit Counter Register */
  #define REG_FLEXCOM4_RNPR                         (0x40010110U) /**< \brief (FLEXCOM4) Receive Next Pointer Register */
  #define REG_FLEXCOM4_RNCR                         (0x40010114U) /**< \brief (FLEXCOM4) Receive Next Counter Register */
  #define REG_FLEXCOM4_TNPR                         (0x40010118U) /**< \brief (FLEXCOM4) Transmit Next Pointer Register */
  #define REG_FLEXCOM4_TNCR                         (0x4001011CU) /**< \brief (FLEXCOM4) Transmit Next Counter Register */
  #define REG_FLEXCOM4_PTCR                         (0x40010120U) /**< \brief (FLEXCOM4) Transfer Control Register */
  #define REG_FLEXCOM4_PTSR                         (0x40010124U) /**< \brief (FLEXCOM4) Transfer Status Register */
  #define REG_FLEXCOM4_PWPMR                        (0x40010128U) /**< \brief (FLEXCOM4) Write Protection Mode Register */
#else
  #define REG_FLEXCOM4_MR          (*(__IO uint32_t*)0x40010000U) /**< \brief (FLEXCOM4) FLEXCOM Mode Register */
  #define REG_FLEXCOM4_RHR         (*(__I  uint32_t*)0x40010010U) /**< \brief (FLEXCOM4) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM4_THR         (*(__IO uint32_t*)0x40010020U) /**< \brief (FLEXCOM4) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM4_RPR         (*(__IO uint32_t*)0x40010100U) /**< \brief (FLEXCOM4) Receive Pointer Register */
  #define REG_FLEXCOM4_RCR         (*(__IO uint32_t*)0x40010104U) /**< \brief (FLEXCOM4) Receive Counter Register */
  #define REG_FLEXCOM4_TPR         (*(__IO uint32_t*)0x40010108U) /**< \brief (FLEXCOM4) Transmit Pointer Register */
  #define REG_FLEXCOM4_TCR         (*(__IO uint32_t*)0x4001010CU) /**< \brief (FLEXCOM4) Transmit Counter Register */
  #define REG_FLEXCOM4_RNPR        (*(__IO uint32_t*)0x40010110U) /**< \brief (FLEXCOM4) Receive Next Pointer Register */
  #define REG_FLEXCOM4_RNCR        (*(__IO uint32_t*)0x40010114U) /**< \brief (FLEXCOM4) Receive Next Counter Register */
  #define REG_FLEXCOM4_TNPR        (*(__IO uint32_t*)0x40010118U) /**< \brief (FLEXCOM4) Transmit Next Pointer Register */
  #define REG_FLEXCOM4_TNCR        (*(__IO uint32_t*)0x4001011CU) /**< \brief (FLEXCOM4) Transmit Next Counter Register */
  #define REG_FLEXCOM4_PTCR        (*(__O  uint32_t*)0x40010120U) /**< \brief (FLEXCOM4) Transfer Control Register */
  #define REG_FLEXCOM4_PTSR        (*(__I  uint32_t*)0x40010124U) /**< \brief (FLEXCOM4) Transfer Status Register */
  #define REG_FLEXCOM4_PWPMR       (*(__IO uint32_t*)0x40010128U) /**< \brief (FLEXCOM4) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM4_INSTANCE_ */
