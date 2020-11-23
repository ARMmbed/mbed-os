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

#ifndef _PIC32CX_FLEXCOM1_INSTANCE_
#define _PIC32CX_FLEXCOM1_INSTANCE_

/* ========== Register definition for FLEXCOM1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_FLEXCOM1_MR                           (0x40004000U) /**< \brief (FLEXCOM1) FLEXCOM Mode Register */
  #define REG_FLEXCOM1_RHR                          (0x40004010U) /**< \brief (FLEXCOM1) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM1_THR                          (0x40004020U) /**< \brief (FLEXCOM1) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM1_RPR                          (0x40004100U) /**< \brief (FLEXCOM1) Receive Pointer Register */
  #define REG_FLEXCOM1_RCR                          (0x40004104U) /**< \brief (FLEXCOM1) Receive Counter Register */
  #define REG_FLEXCOM1_TPR                          (0x40004108U) /**< \brief (FLEXCOM1) Transmit Pointer Register */
  #define REG_FLEXCOM1_TCR                          (0x4000410CU) /**< \brief (FLEXCOM1) Transmit Counter Register */
  #define REG_FLEXCOM1_RNPR                         (0x40004110U) /**< \brief (FLEXCOM1) Receive Next Pointer Register */
  #define REG_FLEXCOM1_RNCR                         (0x40004114U) /**< \brief (FLEXCOM1) Receive Next Counter Register */
  #define REG_FLEXCOM1_TNPR                         (0x40004118U) /**< \brief (FLEXCOM1) Transmit Next Pointer Register */
  #define REG_FLEXCOM1_TNCR                         (0x4000411CU) /**< \brief (FLEXCOM1) Transmit Next Counter Register */
  #define REG_FLEXCOM1_PTCR                         (0x40004120U) /**< \brief (FLEXCOM1) Transfer Control Register */
  #define REG_FLEXCOM1_PTSR                         (0x40004124U) /**< \brief (FLEXCOM1) Transfer Status Register */
  #define REG_FLEXCOM1_PWPMR                        (0x40004128U) /**< \brief (FLEXCOM1) Write Protection Mode Register */
#else
  #define REG_FLEXCOM1_MR          (*(__IO uint32_t*)0x40004000U) /**< \brief (FLEXCOM1) FLEXCOM Mode Register */
  #define REG_FLEXCOM1_RHR         (*(__I  uint32_t*)0x40004010U) /**< \brief (FLEXCOM1) FLEXCOM Receive Holding Register */
  #define REG_FLEXCOM1_THR         (*(__IO uint32_t*)0x40004020U) /**< \brief (FLEXCOM1) FLEXCOM Transmit Holding Register */
  #define REG_FLEXCOM1_RPR         (*(__IO uint32_t*)0x40004100U) /**< \brief (FLEXCOM1) Receive Pointer Register */
  #define REG_FLEXCOM1_RCR         (*(__IO uint32_t*)0x40004104U) /**< \brief (FLEXCOM1) Receive Counter Register */
  #define REG_FLEXCOM1_TPR         (*(__IO uint32_t*)0x40004108U) /**< \brief (FLEXCOM1) Transmit Pointer Register */
  #define REG_FLEXCOM1_TCR         (*(__IO uint32_t*)0x4000410CU) /**< \brief (FLEXCOM1) Transmit Counter Register */
  #define REG_FLEXCOM1_RNPR        (*(__IO uint32_t*)0x40004110U) /**< \brief (FLEXCOM1) Receive Next Pointer Register */
  #define REG_FLEXCOM1_RNCR        (*(__IO uint32_t*)0x40004114U) /**< \brief (FLEXCOM1) Receive Next Counter Register */
  #define REG_FLEXCOM1_TNPR        (*(__IO uint32_t*)0x40004118U) /**< \brief (FLEXCOM1) Transmit Next Pointer Register */
  #define REG_FLEXCOM1_TNCR        (*(__IO uint32_t*)0x4000411CU) /**< \brief (FLEXCOM1) Transmit Next Counter Register */
  #define REG_FLEXCOM1_PTCR        (*(__O  uint32_t*)0x40004120U) /**< \brief (FLEXCOM1) Transfer Control Register */
  #define REG_FLEXCOM1_PTSR        (*(__I  uint32_t*)0x40004124U) /**< \brief (FLEXCOM1) Transfer Status Register */
  #define REG_FLEXCOM1_PWPMR       (*(__IO uint32_t*)0x40004128U) /**< \brief (FLEXCOM1) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_FLEXCOM1_INSTANCE_ */
