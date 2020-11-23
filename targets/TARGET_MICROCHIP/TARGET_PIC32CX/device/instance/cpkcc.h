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

#ifndef _PIC32CX_CPKCC_INSTANCE_
#define _PIC32CX_CPKCC_INSTANCE_

/* ========== Register definition for CPKCC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CPKCC_R                          (0x46000000U) /**< \brief (CPKCC) R Parameter Register */
  #define REG_CPKCC_X                          (0x46000004U) /**< \brief (CPKCC) X Parameter Register */
  #define REG_CPKCC_Y                          (0x46000008U) /**< \brief (CPKCC) Y Parameter Register */
  #define REG_CPKCC_Z                          (0x4600000CU) /**< \brief (CPKCC) Z Parameter Register */
  #define REG_CPKCC_J                          (0x46000010U) /**< \brief (CPKCC) J Parameter Register */
  #define REG_CPKCC_K                          (0x46000014U) /**< \brief (CPKCC) K Parameter Register */
  #define REG_CPKCC_N                          (0x46000018U) /**< \brief (CPKCC) N Parameter Register */
  #define REG_CPKCC_SMULA                      (0x4600001CU) /**< \brief (CPKCC) SMULA Register */
  #define REG_CPKCC_SMULB                      (0x46000020U) /**< \brief (CPKCC) SMULB Register */
  #define REG_CPKCC_SMULRL                     (0x46000024U) /**< \brief (CPKCC) SMULRL Register */
  #define REG_CPKCC_SMULRH                     (0x46000028U) /**< \brief (CPKCC) SMULRH Register */
  #define REG_CPKCC_IDLE                       (0x4600002CU) /**< \brief (CPKCC) IDLE Register */
  #define REG_CPKCC_IDLECACHE                  (0x4600002DU) /**< \brief (CPKCC) IDLECACHE Register */
  #define REG_CPKCC_CR_C                       (0x46000030U) /**< \brief (CPKCC) CR_C Register */
  #define REG_CPKCC_CR_S                       (0x46000034U) /**< \brief (CPKCC) CR_S Register */
  #define REG_CPKCC_CR                         (0x46000038U) /**< \brief (CPKCC) CR Register */
  #define REG_CPKCC_OR                         (0x4600003CU) /**< \brief (CPKCC) Operation Register */
  #define REG_CPKCC_SR                         (0x46000040U) /**< \brief (CPKCC) SR Register */
  #define REG_CPKCC_ADDRSIZE                   (0x460000ECU) /**< \brief (CPKCC) ADDRSIZE Register */
  #define REG_CPKCC_IPNAME                     (0x460000F0U) /**< \brief (CPKCC) IPNAME1 Register */
  #define REG_CPKCC_FEATURES                   (0x460000F8U) /**< \brief (CPKCC) FEATURES Register */
#else
  #define REG_CPKCC_R         (*(__IO uint32_t*)0x46000000U) /**< \brief (CPKCC) R Parameter Register */
  #define REG_CPKCC_X         (*(__IO uint32_t*)0x46000004U) /**< \brief (CPKCC) X Parameter Register */
  #define REG_CPKCC_Y         (*(__IO uint32_t*)0x46000008U) /**< \brief (CPKCC) Y Parameter Register */
  #define REG_CPKCC_Z         (*(__IO uint32_t*)0x4600000CU) /**< \brief (CPKCC) Z Parameter Register */
  #define REG_CPKCC_J         (*(__IO uint32_t*)0x46000010U) /**< \brief (CPKCC) J Parameter Register */
  #define REG_CPKCC_K         (*(__IO uint32_t*)0x46000014U) /**< \brief (CPKCC) K Parameter Register */
  #define REG_CPKCC_N         (*(__IO uint32_t*)0x46000018U) /**< \brief (CPKCC) N Parameter Register */
  #define REG_CPKCC_SMULA     (*(__IO uint32_t*)0x4600001CU) /**< \brief (CPKCC) SMULA Register */
  #define REG_CPKCC_SMULB     (*(__IO uint32_t*)0x46000020U) /**< \brief (CPKCC) SMULB Register */
  #define REG_CPKCC_SMULRL    (*(__IO uint32_t*)0x46000024U) /**< \brief (CPKCC) SMULRL Register */
  #define REG_CPKCC_SMULRH    (*(__IO uint32_t*)0x46000028U) /**< \brief (CPKCC) SMULRH Register */
  #define REG_CPKCC_IDLE      (*(__IO uint32_t*)0x4600002CU) /**< \brief (CPKCC) IDLE Register */
  #define REG_CPKCC_IDLECACHE (*(__IO uint32_t*)0x4600002DU) /**< \brief (CPKCC) IDLECACHE Register */
  #define REG_CPKCC_CR_C      (*(__IO uint32_t*)0x46000030U) /**< \brief (CPKCC) CR_C Register */
  #define REG_CPKCC_CR_S      (*(__IO uint32_t*)0x46000034U) /**< \brief (CPKCC) CR_S Register */
  #define REG_CPKCC_CR        (*(__IO uint32_t*)0x46000038U) /**< \brief (CPKCC) CR Register */
  #define REG_CPKCC_OR        (*(__IO uint32_t*)0x4600003CU) /**< \brief (CPKCC) Operation Register */
  #define REG_CPKCC_SR        (*(__IO uint32_t*)0x46000040U) /**< \brief (CPKCC) SR Register */
  #define REG_CPKCC_ADDRSIZE  (*(__IO uint32_t*)0x460000ECU) /**< \brief (CPKCC) ADDRSIZE Register */
  #define REG_CPKCC_IPNAME    (*(__IO uint32_t*)0x460000F0U) /**< \brief (CPKCC) IPNAME1 Register */
  #define REG_CPKCC_FEATURES  (*(__IO uint32_t*)0x460000F8U) /**< \brief (CPKCC) FEATURES Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_CPKCC_INSTANCE_ */
