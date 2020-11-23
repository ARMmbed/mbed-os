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

#ifndef _PIC32CX_FLEXCOM3_INSTANCE_
#define _PIC32CX_FLEXCOM3_INSTANCE_

/* ========== Register definition for FLEXCOM3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM3_MR                           (0x4000C000U) /**< \brief (FLEXCOM3) FLEXCOM Mode Register */
  #define REG_FLEXCOM3_RHR                          (0x4000C010U) /**< \brief (FLEXCOM3) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM3_THR                          (0x4000C020U) /**< \brief (FLEXCOM3) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM3_RPR                          (0x4000C100U) /**< \brief (FLEXCOM3) Receive Pointer Register */
  #define REG_FLEXCOM3_RCR                          (0x4000C104U) /**< \brief (FLEXCOM3) Receive Counter Register */
  #define REG_FLEXCOM3_TPR                          (0x4000C108U) /**< \brief (FLEXCOM3) Transmit Pointer Register */
  #define REG_FLEXCOM3_TCR                          (0x4000C10CU) /**< \brief (FLEXCOM3) Transmit Counter Register */
  #define REG_FLEXCOM3_RNPR                         (0x4000C110U) /**< \brief (FLEXCOM3) Receive Next Pointer Register */
  #define REG_FLEXCOM3_RNCR                         (0x4000C114U) /**< \brief (FLEXCOM3) Receive Next Counter Register */
  #define REG_FLEXCOM3_TNPR                         (0x4000C118U) /**< \brief (FLEXCOM3) Transmit Next Pointer Register */
  #define REG_FLEXCOM3_TNCR                         (0x4000C11CU) /**< \brief (FLEXCOM3) Transmit Next Counter Register */
  #define REG_FLEXCOM3_PTCR                         (0x4000C120U) /**< \brief (FLEXCOM3) Transfer Control Register */
  #define REG_FLEXCOM3_PTSR                         (0x4000C124U) /**< \brief (FLEXCOM3) Transfer Status Register */
  #define REG_FLEXCOM3_PWPMR                        (0x4000C128U) /**< \brief (FLEXCOM3) Write Protection Mode Register */
#else
  #define REG_FLEXCOM3_MR          (*(__IO uint32_t*)0x4000C000U) /**< \brief (FLEXCOM3) FLEXCOM Mode Register */
  #define REG_FLEXCOM3_RHR         (*(__I  uint32_t*)0x4000C010U) /**< \brief (FLEXCOM3) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM3_THR         (*(__IO uint32_t*)0x4000C020U) /**< \brief (FLEXCOM3) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM3_RPR         (*(__IO uint32_t*)0x4000C100U) /**< \brief (FLEXCOM3) Receive Pointer Register */
  #define REG_FLEXCOM3_RCR         (*(__IO uint32_t*)0x4000C104U) /**< \brief (FLEXCOM3) Receive Counter Register */
  #define REG_FLEXCOM3_TPR         (*(__IO uint32_t*)0x4000C108U) /**< \brief (FLEXCOM3) Transmit Pointer Register */
  #define REG_FLEXCOM3_TCR         (*(__IO uint32_t*)0x4000C10CU) /**< \brief (FLEXCOM3) Transmit Counter Register */
  #define REG_FLEXCOM3_RNPR        (*(__IO uint32_t*)0x4000C110U) /**< \brief (FLEXCOM3) Receive Next Pointer Register */
  #define REG_FLEXCOM3_RNCR        (*(__IO uint32_t*)0x4000C114U) /**< \brief (FLEXCOM3) Receive Next Counter Register */
  #define REG_FLEXCOM3_TNPR        (*(__IO uint32_t*)0x4000C118U) /**< \brief (FLEXCOM3) Transmit Next Pointer Register */
  #define REG_FLEXCOM3_TNCR        (*(__IO uint32_t*)0x4000C11CU) /**< \brief (FLEXCOM3) Transmit Next Counter Register */
  #define REG_FLEXCOM3_PTCR        (*(__O  uint32_t*)0x4000C120U) /**< \brief (FLEXCOM3) Transfer Control Register */
  #define REG_FLEXCOM3_PTSR        (*(__I  uint32_t*)0x4000C124U) /**< \brief (FLEXCOM3) Transfer Status Register */
  #define REG_FLEXCOM3_PWPMR       (*(__IO uint32_t*)0x4000C128U) /**< \brief (FLEXCOM3) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM3_INSTANCE_ */
