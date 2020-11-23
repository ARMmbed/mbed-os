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

#ifndef _PIC32CX_FLEXCOM6_INSTANCE_
#define _PIC32CX_FLEXCOM6_INSTANCE_

/* ========== Register definition for FLEXCOM6 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM6_MR                           (0x40018000U) /**< \brief (FLEXCOM6) FLEXCOM Mode Register */
  #define REG_FLEXCOM6_RHR                          (0x40018010U) /**< \brief (FLEXCOM6) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM6_THR                          (0x40018020U) /**< \brief (FLEXCOM6) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM6_RPR                          (0x40018100U) /**< \brief (FLEXCOM6) Receive Pointer Register */
  #define REG_FLEXCOM6_RCR                          (0x40018104U) /**< \brief (FLEXCOM6) Receive Counter Register */
  #define REG_FLEXCOM6_TPR                          (0x40018108U) /**< \brief (FLEXCOM6) Transmit Pointer Register */
  #define REG_FLEXCOM6_TCR                          (0x4001810CU) /**< \brief (FLEXCOM6) Transmit Counter Register */
  #define REG_FLEXCOM6_RNPR                         (0x40018110U) /**< \brief (FLEXCOM6) Receive Next Pointer Register */
  #define REG_FLEXCOM6_RNCR                         (0x40018114U) /**< \brief (FLEXCOM6) Receive Next Counter Register */
  #define REG_FLEXCOM6_TNPR                         (0x40018118U) /**< \brief (FLEXCOM6) Transmit Next Pointer Register */
  #define REG_FLEXCOM6_TNCR                         (0x4001811CU) /**< \brief (FLEXCOM6) Transmit Next Counter Register */
  #define REG_FLEXCOM6_PTCR                         (0x40018120U) /**< \brief (FLEXCOM6) Transfer Control Register */
  #define REG_FLEXCOM6_PTSR                         (0x40018124U) /**< \brief (FLEXCOM6) Transfer Status Register */
  #define REG_FLEXCOM6_PWPMR                        (0x40018128U) /**< \brief (FLEXCOM6) Write Protection Mode Register */
#else
  #define REG_FLEXCOM6_MR          (*(__IO uint32_t*)0x40018000U) /**< \brief (FLEXCOM6) FLEXCOM Mode Register */
  #define REG_FLEXCOM6_RHR         (*(__I  uint32_t*)0x40018010U) /**< \brief (FLEXCOM6) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM6_THR         (*(__IO uint32_t*)0x40018020U) /**< \brief (FLEXCOM6) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM6_RPR         (*(__IO uint32_t*)0x40018100U) /**< \brief (FLEXCOM6) Receive Pointer Register */
  #define REG_FLEXCOM6_RCR         (*(__IO uint32_t*)0x40018104U) /**< \brief (FLEXCOM6) Receive Counter Register */
  #define REG_FLEXCOM6_TPR         (*(__IO uint32_t*)0x40018108U) /**< \brief (FLEXCOM6) Transmit Pointer Register */
  #define REG_FLEXCOM6_TCR         (*(__IO uint32_t*)0x4001810CU) /**< \brief (FLEXCOM6) Transmit Counter Register */
  #define REG_FLEXCOM6_RNPR        (*(__IO uint32_t*)0x40018110U) /**< \brief (FLEXCOM6) Receive Next Pointer Register */
  #define REG_FLEXCOM6_RNCR        (*(__IO uint32_t*)0x40018114U) /**< \brief (FLEXCOM6) Receive Next Counter Register */
  #define REG_FLEXCOM6_TNPR        (*(__IO uint32_t*)0x40018118U) /**< \brief (FLEXCOM6) Transmit Next Pointer Register */
  #define REG_FLEXCOM6_TNCR        (*(__IO uint32_t*)0x4001811CU) /**< \brief (FLEXCOM6) Transmit Next Counter Register */
  #define REG_FLEXCOM6_PTCR        (*(__O  uint32_t*)0x40018120U) /**< \brief (FLEXCOM6) Transfer Control Register */
  #define REG_FLEXCOM6_PTSR        (*(__I  uint32_t*)0x40018124U) /**< \brief (FLEXCOM6) Transfer Status Register */
  #define REG_FLEXCOM6_PWPMR       (*(__IO uint32_t*)0x40018128U) /**< \brief (FLEXCOM6) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM6_INSTANCE_ */
