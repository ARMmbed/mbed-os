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

#ifndef _PIC32CX_SFRBU_INSTANCE_
#define _PIC32CX_SFRBU_INSTANCE_

/* ========== Register definition for SFRBU peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SFRBU_XTAL_TRIM                     (0x40050610U) /**< \brief (SFRBU) XTAL Oscillator Trimming Register */
  #define REG_SFRBU_TRIM                          (0x40050614U) /**< \brief (SFRBU) Trimming Bits Register */
  #define REG_SFRBU_BOOT                          (0x40050620U) /**< \brief (SFRBU) Boot Register */
  #define REG_SFRBU_IO_RETENTION                  (0x40050630U) /**< \brief (SFRBU) IO Retention Register */
  #define REG_SFRBU_BODCORE                       (0x40050640U) /**< \brief (SFRBU) BOD Core Register */
  #define REG_SFRBU_PWS_CNTRL                     (0x40050650U) /**< \brief (SFRBU) Power Switch Control Register */
  #define REG_SFRBU_WPMR                          (0x400506E4U) /**< \brief (SFRBU) Write Protection Mode Register */
  #define REG_SFRBU_WPSR                          (0x400506E8U) /**< \brief (SFRBU) Write Protection Status Register */
  #define REG_SFRBU_SPARE                         (0x400506F0U) /**< \brief (SFRBU) Spare Register */
#else
  #define REG_SFRBU_XTAL_TRIM    (*(__IO uint32_t*)0x40050610U) /**< \brief (SFRBU) XTAL Oscillator Trimming Register */
  #define REG_SFRBU_TRIM         (*(__I  uint32_t*)0x40050614U) /**< \brief (SFRBU) Trimming Bits Register */
  #define REG_SFRBU_BOOT         (*(__IO uint32_t*)0x40050620U) /**< \brief (SFRBU) Boot Register */
  #define REG_SFRBU_IO_RETENTION (*(__IO uint32_t*)0x40050630U) /**< \brief (SFRBU) IO Retention Register */
  #define REG_SFRBU_BODCORE      (*(__IO uint32_t*)0x40050640U) /**< \brief (SFRBU) BOD Core Register */
  #define REG_SFRBU_PWS_CNTRL    (*(__IO uint32_t*)0x40050650U) /**< \brief (SFRBU) Power Switch Control Register */
  #define REG_SFRBU_WPMR         (*(__IO uint32_t*)0x400506E4U) /**< \brief (SFRBU) Write Protection Mode Register */
  #define REG_SFRBU_WPSR         (*(__IO uint32_t*)0x400506E8U) /**< \brief (SFRBU) Write Protection Status Register */
  #define REG_SFRBU_SPARE        (*(__IO uint32_t*)0x400506F0U) /**< \brief (SFRBU) Spare Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SFRBU_INSTANCE_ */
