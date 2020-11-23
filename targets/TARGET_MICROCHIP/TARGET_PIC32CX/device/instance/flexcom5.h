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

#ifndef _PIC32CX_FLEXCOM5_INSTANCE_
#define _PIC32CX_FLEXCOM5_INSTANCE_

/* ========== Register definition for FLEXCOM5 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM5_MR                           (0x40014000U) /**< \brief (FLEXCOM5) FLEXCOM Mode Register */
  #define REG_FLEXCOM5_RHR                          (0x40014010U) /**< \brief (FLEXCOM5) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM5_THR                          (0x40014020U) /**< \brief (FLEXCOM5) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM5_RPR                          (0x40014100U) /**< \brief (FLEXCOM5) Receive Pointer Register */
  #define REG_FLEXCOM5_RCR                          (0x40014104U) /**< \brief (FLEXCOM5) Receive Counter Register */
  #define REG_FLEXCOM5_TPR                          (0x40014108U) /**< \brief (FLEXCOM5) Transmit Pointer Register */
  #define REG_FLEXCOM5_TCR                          (0x4001410CU) /**< \brief (FLEXCOM5) Transmit Counter Register */
  #define REG_FLEXCOM5_RNPR                         (0x40014110U) /**< \brief (FLEXCOM5) Receive Next Pointer Register */
  #define REG_FLEXCOM5_RNCR                         (0x40014114U) /**< \brief (FLEXCOM5) Receive Next Counter Register */
  #define REG_FLEXCOM5_TNPR                         (0x40014118U) /**< \brief (FLEXCOM5) Transmit Next Pointer Register */
  #define REG_FLEXCOM5_TNCR                         (0x4001411CU) /**< \brief (FLEXCOM5) Transmit Next Counter Register */
  #define REG_FLEXCOM5_PTCR                         (0x40014120U) /**< \brief (FLEXCOM5) Transfer Control Register */
  #define REG_FLEXCOM5_PTSR                         (0x40014124U) /**< \brief (FLEXCOM5) Transfer Status Register */
  #define REG_FLEXCOM5_PWPMR                        (0x40014128U) /**< \brief (FLEXCOM5) Write Protection Mode Register */
#else
  #define REG_FLEXCOM5_MR          (*(__IO uint32_t*)0x40014000U) /**< \brief (FLEXCOM5) FLEXCOM Mode Register */
  #define REG_FLEXCOM5_RHR         (*(__I  uint32_t*)0x40014010U) /**< \brief (FLEXCOM5) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM5_THR         (*(__IO uint32_t*)0x40014020U) /**< \brief (FLEXCOM5) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM5_RPR         (*(__IO uint32_t*)0x40014100U) /**< \brief (FLEXCOM5) Receive Pointer Register */
  #define REG_FLEXCOM5_RCR         (*(__IO uint32_t*)0x40014104U) /**< \brief (FLEXCOM5) Receive Counter Register */
  #define REG_FLEXCOM5_TPR         (*(__IO uint32_t*)0x40014108U) /**< \brief (FLEXCOM5) Transmit Pointer Register */
  #define REG_FLEXCOM5_TCR         (*(__IO uint32_t*)0x4001410CU) /**< \brief (FLEXCOM5) Transmit Counter Register */
  #define REG_FLEXCOM5_RNPR        (*(__IO uint32_t*)0x40014110U) /**< \brief (FLEXCOM5) Receive Next Pointer Register */
  #define REG_FLEXCOM5_RNCR        (*(__IO uint32_t*)0x40014114U) /**< \brief (FLEXCOM5) Receive Next Counter Register */
  #define REG_FLEXCOM5_TNPR        (*(__IO uint32_t*)0x40014118U) /**< \brief (FLEXCOM5) Transmit Next Pointer Register */
  #define REG_FLEXCOM5_TNCR        (*(__IO uint32_t*)0x4001411CU) /**< \brief (FLEXCOM5) Transmit Next Counter Register */
  #define REG_FLEXCOM5_PTCR        (*(__O  uint32_t*)0x40014120U) /**< \brief (FLEXCOM5) Transfer Control Register */
  #define REG_FLEXCOM5_PTSR        (*(__I  uint32_t*)0x40014124U) /**< \brief (FLEXCOM5) Transfer Status Register */
  #define REG_FLEXCOM5_PWPMR       (*(__IO uint32_t*)0x40014128U) /**< \brief (FLEXCOM5) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM5_INSTANCE_ */
