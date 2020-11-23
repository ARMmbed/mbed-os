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

#ifndef _PIC32CX_FLEXCOM7_INSTANCE_
#define _PIC32CX_FLEXCOM7_INSTANCE_

/* ========== Register definition for FLEXCOM7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM7_MR                           (0x4001C000U) /**< \brief (FLEXCOM7) FLEXCOM Mode Register */
  #define REG_FLEXCOM7_RHR                          (0x4001C010U) /**< \brief (FLEXCOM7) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM7_THR                          (0x4001C020U) /**< \brief (FLEXCOM7) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM7_RPR                          (0x4001C100U) /**< \brief (FLEXCOM7) Receive Pointer Register */
  #define REG_FLEXCOM7_RCR                          (0x4001C104U) /**< \brief (FLEXCOM7) Receive Counter Register */
  #define REG_FLEXCOM7_TPR                          (0x4001C108U) /**< \brief (FLEXCOM7) Transmit Pointer Register */
  #define REG_FLEXCOM7_TCR                          (0x4001C10CU) /**< \brief (FLEXCOM7) Transmit Counter Register */
  #define REG_FLEXCOM7_RNPR                         (0x4001C110U) /**< \brief (FLEXCOM7) Receive Next Pointer Register */
  #define REG_FLEXCOM7_RNCR                         (0x4001C114U) /**< \brief (FLEXCOM7) Receive Next Counter Register */
  #define REG_FLEXCOM7_TNPR                         (0x4001C118U) /**< \brief (FLEXCOM7) Transmit Next Pointer Register */
  #define REG_FLEXCOM7_TNCR                         (0x4001C11CU) /**< \brief (FLEXCOM7) Transmit Next Counter Register */
  #define REG_FLEXCOM7_PTCR                         (0x4001C120U) /**< \brief (FLEXCOM7) Transfer Control Register */
  #define REG_FLEXCOM7_PTSR                         (0x4001C124U) /**< \brief (FLEXCOM7) Transfer Status Register */
  #define REG_FLEXCOM7_PWPMR                        (0x4001C128U) /**< \brief (FLEXCOM7) Write Protection Mode Register */
#else
  #define REG_FLEXCOM7_MR          (*(__IO uint32_t*)0x4001C000U) /**< \brief (FLEXCOM7) FLEXCOM Mode Register */
  #define REG_FLEXCOM7_RHR         (*(__I  uint32_t*)0x4001C010U) /**< \brief (FLEXCOM7) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM7_THR         (*(__IO uint32_t*)0x4001C020U) /**< \brief (FLEXCOM7) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM7_RPR         (*(__IO uint32_t*)0x4001C100U) /**< \brief (FLEXCOM7) Receive Pointer Register */
  #define REG_FLEXCOM7_RCR         (*(__IO uint32_t*)0x4001C104U) /**< \brief (FLEXCOM7) Receive Counter Register */
  #define REG_FLEXCOM7_TPR         (*(__IO uint32_t*)0x4001C108U) /**< \brief (FLEXCOM7) Transmit Pointer Register */
  #define REG_FLEXCOM7_TCR         (*(__IO uint32_t*)0x4001C10CU) /**< \brief (FLEXCOM7) Transmit Counter Register */
  #define REG_FLEXCOM7_RNPR        (*(__IO uint32_t*)0x4001C110U) /**< \brief (FLEXCOM7) Receive Next Pointer Register */
  #define REG_FLEXCOM7_RNCR        (*(__IO uint32_t*)0x4001C114U) /**< \brief (FLEXCOM7) Receive Next Counter Register */
  #define REG_FLEXCOM7_TNPR        (*(__IO uint32_t*)0x4001C118U) /**< \brief (FLEXCOM7) Transmit Next Pointer Register */
  #define REG_FLEXCOM7_TNCR        (*(__IO uint32_t*)0x4001C11CU) /**< \brief (FLEXCOM7) Transmit Next Counter Register */
  #define REG_FLEXCOM7_PTCR        (*(__O  uint32_t*)0x4001C120U) /**< \brief (FLEXCOM7) Transfer Control Register */
  #define REG_FLEXCOM7_PTSR        (*(__I  uint32_t*)0x4001C124U) /**< \brief (FLEXCOM7) Transfer Status Register */
  #define REG_FLEXCOM7_PWPMR       (*(__IO uint32_t*)0x4001C128U) /**< \brief (FLEXCOM7) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM7_INSTANCE_ */
