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

#ifndef _PIC32CX_FLEXCOM2_INSTANCE_
#define _PIC32CX_FLEXCOM2_INSTANCE_

/* ========== Register definition for FLEXCOM2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM2_MR                           (0x40008000U) /**< \brief (FLEXCOM2) FLEXCOM Mode Register */
  #define REG_FLEXCOM2_RHR                          (0x40008010U) /**< \brief (FLEXCOM2) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM2_THR                          (0x40008020U) /**< \brief (FLEXCOM2) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM2_RPR                          (0x40008100U) /**< \brief (FLEXCOM2) Receive Pointer Register */
  #define REG_FLEXCOM2_RCR                          (0x40008104U) /**< \brief (FLEXCOM2) Receive Counter Register */
  #define REG_FLEXCOM2_TPR                          (0x40008108U) /**< \brief (FLEXCOM2) Transmit Pointer Register */
  #define REG_FLEXCOM2_TCR                          (0x4000810CU) /**< \brief (FLEXCOM2) Transmit Counter Register */
  #define REG_FLEXCOM2_RNPR                         (0x40008110U) /**< \brief (FLEXCOM2) Receive Next Pointer Register */
  #define REG_FLEXCOM2_RNCR                         (0x40008114U) /**< \brief (FLEXCOM2) Receive Next Counter Register */
  #define REG_FLEXCOM2_TNPR                         (0x40008118U) /**< \brief (FLEXCOM2) Transmit Next Pointer Register */
  #define REG_FLEXCOM2_TNCR                         (0x4000811CU) /**< \brief (FLEXCOM2) Transmit Next Counter Register */
  #define REG_FLEXCOM2_PTCR                         (0x40008120U) /**< \brief (FLEXCOM2) Transfer Control Register */
  #define REG_FLEXCOM2_PTSR                         (0x40008124U) /**< \brief (FLEXCOM2) Transfer Status Register */
  #define REG_FLEXCOM2_PWPMR                        (0x40008128U) /**< \brief (FLEXCOM2) Write Protection Mode Register */
#else
  #define REG_FLEXCOM2_MR          (*(__IO uint32_t*)0x40008000U) /**< \brief (FLEXCOM2) FLEXCOM Mode Register */
  #define REG_FLEXCOM2_RHR         (*(__I  uint32_t*)0x40008010U) /**< \brief (FLEXCOM2) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM2_THR         (*(__IO uint32_t*)0x40008020U) /**< \brief (FLEXCOM2) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM2_RPR         (*(__IO uint32_t*)0x40008100U) /**< \brief (FLEXCOM2) Receive Pointer Register */
  #define REG_FLEXCOM2_RCR         (*(__IO uint32_t*)0x40008104U) /**< \brief (FLEXCOM2) Receive Counter Register */
  #define REG_FLEXCOM2_TPR         (*(__IO uint32_t*)0x40008108U) /**< \brief (FLEXCOM2) Transmit Pointer Register */
  #define REG_FLEXCOM2_TCR         (*(__IO uint32_t*)0x4000810CU) /**< \brief (FLEXCOM2) Transmit Counter Register */
  #define REG_FLEXCOM2_RNPR        (*(__IO uint32_t*)0x40008110U) /**< \brief (FLEXCOM2) Receive Next Pointer Register */
  #define REG_FLEXCOM2_RNCR        (*(__IO uint32_t*)0x40008114U) /**< \brief (FLEXCOM2) Receive Next Counter Register */
  #define REG_FLEXCOM2_TNPR        (*(__IO uint32_t*)0x40008118U) /**< \brief (FLEXCOM2) Transmit Next Pointer Register */
  #define REG_FLEXCOM2_TNCR        (*(__IO uint32_t*)0x4000811CU) /**< \brief (FLEXCOM2) Transmit Next Counter Register */
  #define REG_FLEXCOM2_PTCR        (*(__O  uint32_t*)0x40008120U) /**< \brief (FLEXCOM2) Transfer Control Register */
  #define REG_FLEXCOM2_PTSR        (*(__I  uint32_t*)0x40008124U) /**< \brief (FLEXCOM2) Transfer Status Register */
  #define REG_FLEXCOM2_PWPMR       (*(__IO uint32_t*)0x40008128U) /**< \brief (FLEXCOM2) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM2_INSTANCE_ */
