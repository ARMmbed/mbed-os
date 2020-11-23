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

#ifndef _PIC32CX_RTC_INSTANCE_
#define _PIC32CX_RTC_INSTANCE_

/* ========== Register definition for RTC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_RTC_CR                      (0x40053100U) /**< \brief (RTC) Control Register */
  #define REG_RTC_MR                      (0x40053104U) /**< \brief (RTC) Mode Register */
  #define REG_RTC_TIMR                    (0x40053108U) /**< \brief (RTC) Time Register */
  #define REG_RTC_CALR                    (0x4005310CU) /**< \brief (RTC) Calendar Register */
  #define REG_RTC_TIMALR                  (0x40053110U) /**< \brief (RTC) Time Alarm Register */
  #define REG_RTC_CALALR                  (0x40053114U) /**< \brief (RTC) Calendar Alarm Register */
  #define REG_RTC_SR                      (0x40053118U) /**< \brief (RTC) Status Register */
  #define REG_RTC_SCCR                    (0x4005311CU) /**< \brief (RTC) Status Clear Command Register */
  #define REG_RTC_IER                     (0x40053120U) /**< \brief (RTC) Interrupt Enable Register */
  #define REG_RTC_IDR                     (0x40053124U) /**< \brief (RTC) Interrupt Disable Register */
  #define REG_RTC_IMR                     (0x40053128U) /**< \brief (RTC) Interrupt Mask Register */
  #define REG_RTC_VER                     (0x4005312CU) /**< \brief (RTC) Valid Entry Register */
  #define REG_RTC_TCR                     (0x40053134U) /**< \brief (RTC) Tamper Control Register */
  #define REG_RTC_TISR                    (0x40053138U) /**< \brief (RTC) Tamper Input Status Register */
  #define REG_RTC_FSTR0                   (0x40053140U) /**< \brief (RTC) First Stamping Time Register of Source 0 */
  #define REG_RTC_FSDR0                   (0x40053144U) /**< \brief (RTC) First Stamping Date Register of Source 0 */
  #define REG_RTC_LSTR0                   (0x40053148U) /**< \brief (RTC) Last Stamping Time Register of Source 0 */
  #define REG_RTC_LSDR0                   (0x4005314CU) /**< \brief (RTC) Last Stamping Date Register of Source 0 */
  #define REG_RTC_FSTR1                   (0x40053150U) /**< \brief (RTC) First Stamping Time Register of Source 1 */
  #define REG_RTC_FSDR1                   (0x40053154U) /**< \brief (RTC) First Stamping Date Register of Source 1 */
  #define REG_RTC_LSTR1                   (0x40053158U) /**< \brief (RTC) Last Stamping Time Register of Source 1 */
  #define REG_RTC_LSDR1                   (0x4005315CU) /**< \brief (RTC) Last Stamping Date Register of Source 1 */
  #define REG_RTC_FSTR2                   (0x40053160U) /**< \brief (RTC) First Stamping Time Register of Source 2 */
  #define REG_RTC_FSDR2                   (0x40053164U) /**< \brief (RTC) First Stamping Date Register of Source 2 */
  #define REG_RTC_LSTR2                   (0x40053168U) /**< \brief (RTC) Last Stamping Time Register of Source 2 */
  #define REG_RTC_LSDR2                   (0x4005316CU) /**< \brief (RTC) Last Stamping Date Register of Source 2 */
  #define REG_RTC_FSTR3                   (0x40053170U) /**< \brief (RTC) First Stamping Time Register of Source 3 */
  #define REG_RTC_FSDR3                   (0x40053174U) /**< \brief (RTC) First Stamping Date Register of Source 3 */
  #define REG_RTC_LSTR3                   (0x40053178U) /**< \brief (RTC) Last Stamping Time Register of Source 3 */
  #define REG_RTC_LSDR3                   (0x4005317CU) /**< \brief (RTC) Last Stamping Date Register of Source 3 */
  #define REG_RTC_FSTR4                   (0x40053180U) /**< \brief (RTC) First Stamping Time Register of Source 4 */
  #define REG_RTC_FSDR4                   (0x40053184U) /**< \brief (RTC) First Stamping Date Register of Source 4 */
  #define REG_RTC_LSTR4                   (0x40053188U) /**< \brief (RTC) Last Stamping Time Register of Source 4 */
  #define REG_RTC_LSDR4                   (0x4005318CU) /**< \brief (RTC) Last Stamping Date Register of Source 4 */
#else
  #define REG_RTC_CR     (*(__IO uint32_t*)0x40053100U) /**< \brief (RTC) Control Register */
  #define REG_RTC_MR     (*(__IO uint32_t*)0x40053104U) /**< \brief (RTC) Mode Register */
  #define REG_RTC_TIMR   (*(__IO uint32_t*)0x40053108U) /**< \brief (RTC) Time Register */
  #define REG_RTC_CALR   (*(__IO uint32_t*)0x4005310CU) /**< \brief (RTC) Calendar Register */
  #define REG_RTC_TIMALR (*(__IO uint32_t*)0x40053110U) /**< \brief (RTC) Time Alarm Register */
  #define REG_RTC_CALALR (*(__IO uint32_t*)0x40053114U) /**< \brief (RTC) Calendar Alarm Register */
  #define REG_RTC_SR     (*(__I  uint32_t*)0x40053118U) /**< \brief (RTC) Status Register */
  #define REG_RTC_SCCR   (*(__O  uint32_t*)0x4005311CU) /**< \brief (RTC) Status Clear Command Register */
  #define REG_RTC_IER    (*(__O  uint32_t*)0x40053120U) /**< \brief (RTC) Interrupt Enable Register */
  #define REG_RTC_IDR    (*(__O  uint32_t*)0x40053124U) /**< \brief (RTC) Interrupt Disable Register */
  #define REG_RTC_IMR    (*(__I  uint32_t*)0x40053128U) /**< \brief (RTC) Interrupt Mask Register */
  #define REG_RTC_VER    (*(__I  uint32_t*)0x4005312CU) /**< \brief (RTC) Valid Entry Register */
  #define REG_RTC_TCR    (*(__IO uint32_t*)0x40053134U) /**< \brief (RTC) Tamper Control Register */
  #define REG_RTC_TISR   (*(__I  uint32_t*)0x40053138U) /**< \brief (RTC) Tamper Input Status Register */
  #define REG_RTC_FSTR0  (*(__I  uint32_t*)0x40053140U) /**< \brief (RTC) First Stamping Time Register of Source 0 */
  #define REG_RTC_FSDR0  (*(__I  uint32_t*)0x40053144U) /**< \brief (RTC) First Stamping Date Register of Source 0 */
  #define REG_RTC_LSTR0  (*(__I  uint32_t*)0x40053148U) /**< \brief (RTC) Last Stamping Time Register of Source 0 */
  #define REG_RTC_LSDR0  (*(__I  uint32_t*)0x4005314CU) /**< \brief (RTC) Last Stamping Date Register of Source 0 */
  #define REG_RTC_FSTR1  (*(__I  uint32_t*)0x40053150U) /**< \brief (RTC) First Stamping Time Register of Source 1 */
  #define REG_RTC_FSDR1  (*(__I  uint32_t*)0x40053154U) /**< \brief (RTC) First Stamping Date Register of Source 1 */
  #define REG_RTC_LSTR1  (*(__I  uint32_t*)0x40053158U) /**< \brief (RTC) Last Stamping Time Register of Source 1 */
  #define REG_RTC_LSDR1  (*(__I  uint32_t*)0x4005315CU) /**< \brief (RTC) Last Stamping Date Register of Source 1 */
  #define REG_RTC_FSTR2  (*(__I  uint32_t*)0x40053160U) /**< \brief (RTC) First Stamping Time Register of Source 2 */
  #define REG_RTC_FSDR2  (*(__I  uint32_t*)0x40053164U) /**< \brief (RTC) First Stamping Date Register of Source 2 */
  #define REG_RTC_LSTR2  (*(__I  uint32_t*)0x40053168U) /**< \brief (RTC) Last Stamping Time Register of Source 2 */
  #define REG_RTC_LSDR2  (*(__I  uint32_t*)0x4005316CU) /**< \brief (RTC) Last Stamping Date Register of Source 2 */
  #define REG_RTC_FSTR3  (*(__I  uint32_t*)0x40053170U) /**< \brief (RTC) First Stamping Time Register of Source 3 */
  #define REG_RTC_FSDR3  (*(__I  uint32_t*)0x40053174U) /**< \brief (RTC) First Stamping Date Register of Source 3 */
  #define REG_RTC_LSTR3  (*(__I  uint32_t*)0x40053178U) /**< \brief (RTC) Last Stamping Time Register of Source 3 */
  #define REG_RTC_LSDR3  (*(__I  uint32_t*)0x4005317CU) /**< \brief (RTC) Last Stamping Date Register of Source 3 */
  #define REG_RTC_FSTR4  (*(__I  uint32_t*)0x40053180U) /**< \brief (RTC) First Stamping Time Register of Source 4 */
  #define REG_RTC_FSDR4  (*(__I  uint32_t*)0x40053184U) /**< \brief (RTC) First Stamping Date Register of Source 4 */
  #define REG_RTC_LSTR4  (*(__I  uint32_t*)0x40053188U) /**< \brief (RTC) Last Stamping Time Register of Source 4 */
  #define REG_RTC_LSDR4  (*(__I  uint32_t*)0x4005318CU) /**< \brief (RTC) Last Stamping Date Register of Source 4 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_RTC_INSTANCE_ */
