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

#ifndef _PIC32CX_CMCC0_INSTANCE_
#define _PIC32CX_CMCC0_INSTANCE_

/* ========== Register definition for CMCC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CMCC0_TYPE                    (0x46008000U) /**< \brief (CMCC0) Cache Controller Type Register */
  #define REG_CMCC0_CFG                     (0x46008004U) /**< \brief (CMCC0) Configuration Register */
  #define REG_CMCC0_CTRL                    (0x46008008U) /**< \brief (CMCC0) Control Register */
  #define REG_CMCC0_SR                      (0x4600800CU) /**< \brief (CMCC0) Status Register */
  #define REG_CMCC0_MAINT0                  (0x46008020U) /**< \brief (CMCC0) Maintenance Register 0 */
  #define REG_CMCC0_MAINT1                  (0x46008024U) /**< \brief (CMCC0) Maintenance Register 1 */
  #define REG_CMCC0_MCFG                    (0x46008028U) /**< \brief (CMCC0) Monitor Configuration Register */
  #define REG_CMCC0_MEN                     (0x4600802CU) /**< \brief (CMCC0) Monitor Enable Register */
  #define REG_CMCC0_MCTRL                   (0x46008030U) /**< \brief (CMCC0) Monitor Control Register */
  #define REG_CMCC0_MSR                     (0x46008034U) /**< \brief (CMCC0) Monitor Status Register */
  #define REG_CMCC0_WPMR                    (0x460080E4U) /**< \brief (CMCC0) Write Protection Mode Register */
#else
  #define REG_CMCC0_TYPE   (*(__I  uint32_t*)0x46008000U) /**< \brief (CMCC0) Cache Controller Type Register */
  #define REG_CMCC0_CFG    (*(__IO uint32_t*)0x46008004U) /**< \brief (CMCC0) Configuration Register */
  #define REG_CMCC0_CTRL   (*(__O  uint32_t*)0x46008008U) /**< \brief (CMCC0) Control Register */
  #define REG_CMCC0_SR     (*(__I  uint32_t*)0x4600800CU) /**< \brief (CMCC0) Status Register */
  #define REG_CMCC0_MAINT0 (*(__O  uint32_t*)0x46008020U) /**< \brief (CMCC0) Maintenance Register 0 */
  #define REG_CMCC0_MAINT1 (*(__O  uint32_t*)0x46008024U) /**< \brief (CMCC0) Maintenance Register 1 */
  #define REG_CMCC0_MCFG   (*(__IO uint32_t*)0x46008028U) /**< \brief (CMCC0) Monitor Configuration Register */
  #define REG_CMCC0_MEN    (*(__IO uint32_t*)0x4600802CU) /**< \brief (CMCC0) Monitor Enable Register */
  #define REG_CMCC0_MCTRL  (*(__O  uint32_t*)0x46008030U) /**< \brief (CMCC0) Monitor Control Register */
  #define REG_CMCC0_MSR    (*(__I  uint32_t*)0x46008034U) /**< \brief (CMCC0) Monitor Status Register */
  #define REG_CMCC0_WPMR   (*(__IO uint32_t*)0x460080E4U) /**< \brief (CMCC0) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_CMCC0_INSTANCE_ */
