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

#ifndef _PIC32CX_SEFC1_INSTANCE_
#define _PIC32CX_SEFC1_INSTANCE_

/* ========== Register definition for SEFC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SEFC1_FMR                   (0x460E0200U) /**< \brief (SEFC1) SEFC Flash Mode Register */
  #define REG_SEFC1_FCR                   (0x460E0204U) /**< \brief (SEFC1) SEFC Flash Command Register */
  #define REG_SEFC1_FSR                   (0x460E0208U) /**< \brief (SEFC1) SEFC Flash Status Register */
  #define REG_SEFC1_FRR                   (0x460E020CU) /**< \brief (SEFC1) SEFC Flash Result Register */
  #define REG_SEFC1_USR                   (0x460E0210U) /**< \brief (SEFC1) SEFC User Signature Rights Register */
  #define REG_SEFC1_KBLR                  (0x460E0214U) /**< \brief (SEFC1) SEFC Key Bus Lock Register */
  #define REG_SEFC1_WPMR                  (0x460E02E4U) /**< \brief (SEFC1) Write Protection Mode Register */
#else
  #define REG_SEFC1_FMR  (*(__IO uint32_t*)0x460E0200U) /**< \brief (SEFC1) SEFC Flash Mode Register */
  #define REG_SEFC1_FCR  (*(__O  uint32_t*)0x460E0204U) /**< \brief (SEFC1) SEFC Flash Command Register */
  #define REG_SEFC1_FSR  (*(__I  uint32_t*)0x460E0208U) /**< \brief (SEFC1) SEFC Flash Status Register */
  #define REG_SEFC1_FRR  (*(__I  uint32_t*)0x460E020CU) /**< \brief (SEFC1) SEFC Flash Result Register */
  #define REG_SEFC1_USR  (*(__IO uint32_t*)0x460E0210U) /**< \brief (SEFC1) SEFC User Signature Rights Register */
  #define REG_SEFC1_KBLR (*(__IO uint32_t*)0x460E0214U) /**< \brief (SEFC1) SEFC Key Bus Lock Register */
  #define REG_SEFC1_WPMR (*(__IO uint32_t*)0x460E02E4U) /**< \brief (SEFC1) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SEFC1_INSTANCE_ */
