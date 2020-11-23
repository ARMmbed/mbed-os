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

#ifndef _PIC32CX_ICM_INSTANCE_
#define _PIC32CX_ICM_INSTANCE_

/* ========== Register definition for ICM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_ICM_CFG                     (0x44010000U) /**< \brief (ICM) Configuration Register */
  #define REG_ICM_CTRL                    (0x44010004U) /**< \brief (ICM) Control Register */
  #define REG_ICM_SR                      (0x44010008U) /**< \brief (ICM) Status Register */
  #define REG_ICM_IER                     (0x44010010U) /**< \brief (ICM) Interrupt Enable Register */
  #define REG_ICM_IDR                     (0x44010014U) /**< \brief (ICM) Interrupt Disable Register */
  #define REG_ICM_IMR                     (0x44010018U) /**< \brief (ICM) Interrupt Mask Register */
  #define REG_ICM_ISR                     (0x4401001CU) /**< \brief (ICM) Interrupt Status Register */
  #define REG_ICM_UASR                    (0x44010020U) /**< \brief (ICM) Undefined Access Status Register */
  #define REG_ICM_DSCR                    (0x44010030U) /**< \brief (ICM) Region Descriptor Area Start Address Register */
  #define REG_ICM_HASH                    (0x44010034U) /**< \brief (ICM) Region Hash Area Start Address Register */
  #define REG_ICM_UIHVAL                  (0x44010038U) /**< \brief (ICM) User Initial Hash Value 0 Register */
  #define REG_ICM_WPMR                    (0x440100E4U) /**< \brief (ICM) Write Protection Mode Register */
  #define REG_ICM_WPSR                    (0x440100E8U) /**< \brief (ICM) Write Protection Status Register */
#else
  #define REG_ICM_CFG    (*(__IO uint32_t*)0x44010000U) /**< \brief (ICM) Configuration Register */
  #define REG_ICM_CTRL   (*(__O  uint32_t*)0x44010004U) /**< \brief (ICM) Control Register */
  #define REG_ICM_SR     (*(__I  uint32_t*)0x44010008U) /**< \brief (ICM) Status Register */
  #define REG_ICM_IER    (*(__O  uint32_t*)0x44010010U) /**< \brief (ICM) Interrupt Enable Register */
  #define REG_ICM_IDR    (*(__O  uint32_t*)0x44010014U) /**< \brief (ICM) Interrupt Disable Register */
  #define REG_ICM_IMR    (*(__I  uint32_t*)0x44010018U) /**< \brief (ICM) Interrupt Mask Register */
  #define REG_ICM_ISR    (*(__I  uint32_t*)0x4401001CU) /**< \brief (ICM) Interrupt Status Register */
  #define REG_ICM_UASR   (*(__I  uint32_t*)0x44010020U) /**< \brief (ICM) Undefined Access Status Register */
  #define REG_ICM_DSCR   (*(__IO uint32_t*)0x44010030U) /**< \brief (ICM) Region Descriptor Area Start Address Register */
  #define REG_ICM_HASH   (*(__IO uint32_t*)0x44010034U) /**< \brief (ICM) Region Hash Area Start Address Register */
  #define REG_ICM_UIHVAL (*(__O  uint32_t*)0x44010038U) /**< \brief (ICM) User Initial Hash Value 0 Register */
  #define REG_ICM_WPMR   (*(__IO uint32_t*)0x440100E4U) /**< \brief (ICM) Write Protection Mode Register */
  #define REG_ICM_WPSR   (*(__I  uint32_t*)0x440100E8U) /**< \brief (ICM) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_ICM_INSTANCE_ */
