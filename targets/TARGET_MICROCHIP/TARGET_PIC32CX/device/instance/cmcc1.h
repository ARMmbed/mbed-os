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

#ifndef _PIC32CX_CMCC1_INSTANCE_
#define _PIC32CX_CMCC1_INSTANCE_

/* ========== Register definition for CMCC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CMCC1_TYPE                    (0x4600C000U) /**< \brief (CMCC1) Cache Controller Type Register */
  #define REG_CMCC1_CFG                     (0x4600C004U) /**< \brief (CMCC1) Configuration Register */
  #define REG_CMCC1_CTRL                    (0x4600C008U) /**< \brief (CMCC1) Control Register */
  #define REG_CMCC1_SR                      (0x4600C00CU) /**< \brief (CMCC1) Status Register */
  #define REG_CMCC1_MAINT0                  (0x4600C020U) /**< \brief (CMCC1) Maintenance Register 0 */
  #define REG_CMCC1_MAINT1                  (0x4600C024U) /**< \brief (CMCC1) Maintenance Register 1 */
  #define REG_CMCC1_MCFG                    (0x4600C028U) /**< \brief (CMCC1) Monitor Configuration Register */
  #define REG_CMCC1_MEN                     (0x4600C02CU) /**< \brief (CMCC1) Monitor Enable Register */
  #define REG_CMCC1_MCTRL                   (0x4600C030U) /**< \brief (CMCC1) Monitor Control Register */
  #define REG_CMCC1_MSR                     (0x4600C034U) /**< \brief (CMCC1) Monitor Status Register */
  #define REG_CMCC1_WPMR                    (0x4600C0E4U) /**< \brief (CMCC1) Write Protection Mode Register */
#else
  #define REG_CMCC1_TYPE   (*(__I  uint32_t*)0x4600C000U) /**< \brief (CMCC1) Cache Controller Type Register */
  #define REG_CMCC1_CFG    (*(__IO uint32_t*)0x4600C004U) /**< \brief (CMCC1) Configuration Register */
  #define REG_CMCC1_CTRL   (*(__O  uint32_t*)0x4600C008U) /**< \brief (CMCC1) Control Register */
  #define REG_CMCC1_SR     (*(__I  uint32_t*)0x4600C00CU) /**< \brief (CMCC1) Status Register */
  #define REG_CMCC1_MAINT0 (*(__O  uint32_t*)0x4600C020U) /**< \brief (CMCC1) Maintenance Register 0 */
  #define REG_CMCC1_MAINT1 (*(__O  uint32_t*)0x4600C024U) /**< \brief (CMCC1) Maintenance Register 1 */
  #define REG_CMCC1_MCFG   (*(__IO uint32_t*)0x4600C028U) /**< \brief (CMCC1) Monitor Configuration Register */
  #define REG_CMCC1_MEN    (*(__IO uint32_t*)0x4600C02CU) /**< \brief (CMCC1) Monitor Enable Register */
  #define REG_CMCC1_MCTRL  (*(__O  uint32_t*)0x4600C030U) /**< \brief (CMCC1) Monitor Control Register */
  #define REG_CMCC1_MSR    (*(__I  uint32_t*)0x4600C034U) /**< \brief (CMCC1) Monitor Status Register */
  #define REG_CMCC1_WPMR   (*(__IO uint32_t*)0x4600C0E4U) /**< \brief (CMCC1) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_CMCC1_INSTANCE_ */
