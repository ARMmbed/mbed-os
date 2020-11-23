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

#ifndef _PIC32CX_DWDT_INSTANCE_
#define _PIC32CX_DWDT_INSTANCE_

/* ========== Register definition for DWDT peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_WDT1_CR                      (0x40052000U) /**< \brief (DWDT) Watchdog 1 Control Register */
  #define REG_WDT1_MR                      (0x40052004U) /**< \brief (DWDT) Watchdog 1 Mode Register */
  #define REG_WDT1_VR                      (0x40052008U) /**< \brief (DWDT) Watchdog 1 Value Register */
  #define REG_WDT1_WL                      (0x4005200CU) /**< \brief (DWDT) Watchdog 1 Window Level Register */
  #define REG_WDT1_IL                      (0x40052010U) /**< \brief (DWDT) Watchdog 1 Interrupt Level Register */
  #define REG_WDT1_IER                     (0x40052014U) /**< \brief (DWDT) Watchdog 1 Interrupt Enable Register */
  #define REG_WDT1_IDR                     (0x40052018U) /**< \brief (DWDT) Watchdog 1 Interrupt Disable Register */
  #define REG_WDT1_ISR                     (0x4005201CU) /**< \brief (DWDT) Watchdog 1 Interrupt Status Register */
  #define REG_WDT1_IMR                     (0x40052020U) /**< \brief (DWDT) Watchdog 1 Interrupt Mask Register */
  #define REG_WDT0_CR                      (0x40053210U) /**< \brief (DWDT) Watchdog 0 Control Register */
  #define REG_WDT0_MR                      (0x40053214U) /**< \brief (DWDT) Watchdog 0 Mode Register */
  #define REG_WDT0_VR                      (0x40053218U) /**< \brief (DWDT) Watchdog 0 Value Register */
  #define REG_WDT0_WL                      (0x4005321CU) /**< \brief (DWDT) Watchdog 0 Window Level */
  #define REG_WDT0_IL                      (0x40053220U) /**< \brief (DWDT) Watchdog 0 Interrupt Level */
  #define REG_WDT0_IER                     (0x40053224U) /**< \brief (DWDT) Watchdog 0 Interrupt Enable Register */
  #define REG_WDT0_IDR                     (0x40053228U) /**< \brief (DWDT) Watchdog 0 Interrupt Disable Register */
  #define REG_WDT0_ISR                     (0x4005322CU) /**< \brief (DWDT) Watchdog 0 Interrupt Status Register */
  #define REG_WDT0_IMR                     (0x40053230U) /**< \brief (DWDT) Watchdog 0 Interrupt Mask Register */
  #define REG_WDT1_LVLLIM                  (0x40053234U) /**< \brief (DWDT) Watchdog 1 Level Limits Register */
  #define REG_WDT1_RLIM                    (0x40053238U) /**< \brief (DWDT) Watchdog 1 Repeat Limits Register */
  #define REG_WDT1_PLIM                    (0x4005323CU) /**< \brief (DWDT) Watchdog 1 Period Limits Register */
#else
  #define REG_WDT1_CR     (*(__O  uint32_t*)0x40052000U) /**< \brief (DWDT) Watchdog 1 Control Register */
  #define REG_WDT1_MR     (*(__IO uint32_t*)0x40052004U) /**< \brief (DWDT) Watchdog 1 Mode Register */
  #define REG_WDT1_VR     (*(__I  uint32_t*)0x40052008U) /**< \brief (DWDT) Watchdog 1 Value Register */
  #define REG_WDT1_WL     (*(__IO uint32_t*)0x4005200CU) /**< \brief (DWDT) Watchdog 1 Window Level Register */
  #define REG_WDT1_IL     (*(__IO uint32_t*)0x40052010U) /**< \brief (DWDT) Watchdog 1 Interrupt Level Register */
  #define REG_WDT1_IER    (*(__O  uint32_t*)0x40052014U) /**< \brief (DWDT) Watchdog 1 Interrupt Enable Register */
  #define REG_WDT1_IDR    (*(__O  uint32_t*)0x40052018U) /**< \brief (DWDT) Watchdog 1 Interrupt Disable Register */
  #define REG_WDT1_ISR    (*(__I  uint32_t*)0x4005201CU) /**< \brief (DWDT) Watchdog 1 Interrupt Status Register */
  #define REG_WDT1_IMR    (*(__I  uint32_t*)0x40052020U) /**< \brief (DWDT) Watchdog 1 Interrupt Mask Register */
  #define REG_WDT0_CR     (*(__O  uint32_t*)0x40053210U) /**< \brief (DWDT) Watchdog 0 Control Register */
  #define REG_WDT0_MR     (*(__IO uint32_t*)0x40053214U) /**< \brief (DWDT) Watchdog 0 Mode Register */
  #define REG_WDT0_VR     (*(__I  uint32_t*)0x40053218U) /**< \brief (DWDT) Watchdog 0 Value Register */
  #define REG_WDT0_WL     (*(__IO uint32_t*)0x4005321CU) /**< \brief (DWDT) Watchdog 0 Window Level */
  #define REG_WDT0_IL     (*(__IO uint32_t*)0x40053220U) /**< \brief (DWDT) Watchdog 0 Interrupt Level */
  #define REG_WDT0_IER    (*(__O  uint32_t*)0x40053224U) /**< \brief (DWDT) Watchdog 0 Interrupt Enable Register */
  #define REG_WDT0_IDR    (*(__O  uint32_t*)0x40053228U) /**< \brief (DWDT) Watchdog 0 Interrupt Disable Register */
  #define REG_WDT0_ISR    (*(__I  uint32_t*)0x4005322CU) /**< \brief (DWDT) Watchdog 0 Interrupt Status Register */
  #define REG_WDT0_IMR    (*(__I  uint32_t*)0x40053230U) /**< \brief (DWDT) Watchdog 0 Interrupt Mask Register */
  #define REG_WDT1_LVLLIM (*(__IO uint32_t*)0x40053234U) /**< \brief (DWDT) Watchdog 1 Level Limits Register */
  #define REG_WDT1_RLIM   (*(__IO uint32_t*)0x40053238U) /**< \brief (DWDT) Watchdog 1 Repeat Limits Register */
  #define REG_WDT1_PLIM   (*(__IO uint32_t*)0x4005323CU) /**< \brief (DWDT) Watchdog 1 Period Limits Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_DWDT_INSTANCE_ */
