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

#ifndef _PIC32CX_SUPC_INSTANCE_
#define _PIC32CX_SUPC_INSTANCE_

/* ========== Register definition for SUPC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SUPC_CR                    (0x400531D0U) /**< \brief (SUPC) Control Register */
  #define REG_SUPC_SMMR                  (0x400531D4U) /**< \brief (SUPC) Supply Monitor Mode Register */
  #define REG_SUPC_MR                    (0x400531D8U) /**< \brief (SUPC) Mode Register */
  #define REG_SUPC_WUMR                  (0x400531DCU) /**< \brief (SUPC) Wakeup Mode Register */
  #define REG_SUPC_WUIR                  (0x400531E0U) /**< \brief (SUPC) Wakeup Inputs Register */
  #define REG_SUPC_SR                    (0x400531E4U) /**< \brief (SUPC) Status Register */
  #define REG_SUPC_EMR                   (0x400531ECU) /**< \brief (SUPC) Extended Mode Register */
  #define REG_SUPC_BMR                   (0x400531F0U) /**< \brief (SUPC) Backup Mode Register */
  #define REG_SUPC_WUSR                  (0x400531F4U) /**< \brief (SUPC) Wakeup Status Register */
  #define REG_SUPC_IER                   (0x400531F8U) /**< \brief (SUPC) Enable Interrupt Register */
  #define REG_SUPC_IDR                   (0x400531FCU) /**< \brief (SUPC) Disable Interrupt Register */
  #define REG_SUPC_IMR                   (0x40053200U) /**< \brief (SUPC) Mask Interrupt Register */
  #define REG_SUPC_ISR                   (0x40053204U) /**< \brief (SUPC) Status Interrupt Register */
#else
  #define REG_SUPC_CR   (*(__O  uint32_t*)0x400531D0U) /**< \brief (SUPC) Control Register */
  #define REG_SUPC_SMMR (*(__IO uint32_t*)0x400531D4U) /**< \brief (SUPC) Supply Monitor Mode Register */
  #define REG_SUPC_MR   (*(__IO uint32_t*)0x400531D8U) /**< \brief (SUPC) Mode Register */
  #define REG_SUPC_WUMR (*(__IO uint32_t*)0x400531DCU) /**< \brief (SUPC) Wakeup Mode Register */
  #define REG_SUPC_WUIR (*(__IO uint32_t*)0x400531E0U) /**< \brief (SUPC) Wakeup Inputs Register */
  #define REG_SUPC_SR   (*(__I  uint32_t*)0x400531E4U) /**< \brief (SUPC) Status Register */
  #define REG_SUPC_EMR  (*(__IO uint32_t*)0x400531ECU) /**< \brief (SUPC) Extended Mode Register */
  #define REG_SUPC_BMR  (*(__IO uint32_t*)0x400531F0U) /**< \brief (SUPC) Backup Mode Register */
  #define REG_SUPC_WUSR (*(__I  uint32_t*)0x400531F4U) /**< \brief (SUPC) Wakeup Status Register */
  #define REG_SUPC_IER  (*(__O  uint32_t*)0x400531F8U) /**< \brief (SUPC) Enable Interrupt Register */
  #define REG_SUPC_IDR  (*(__O  uint32_t*)0x400531FCU) /**< \brief (SUPC) Disable Interrupt Register */
  #define REG_SUPC_IMR  (*(__I  uint32_t*)0x40053200U) /**< \brief (SUPC) Mask Interrupt Register */
  #define REG_SUPC_ISR  (*(__I  uint32_t*)0x40053204U) /**< \brief (SUPC) Status Interrupt Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SUPC_INSTANCE_ */
