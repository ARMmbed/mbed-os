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

#ifndef _PIC32CX_AESB_INSTANCE_
#define _PIC32CX_AESB_INSTANCE_

/* ========== Register definition for AESB peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_AESB_CR                      (0x44004000U) /**< \brief (AESB) Control Register */
  #define REG_AESB_MR                      (0x44004004U) /**< \brief (AESB) Mode Register */
  #define REG_AESB_IER                     (0x44004010U) /**< \brief (AESB) Interrupt Enable Register */
  #define REG_AESB_IDR                     (0x44004014U) /**< \brief (AESB) Interrupt Disable Register */
  #define REG_AESB_IMR                     (0x44004018U) /**< \brief (AESB) Interrupt Mask Register */
  #define REG_AESB_ISR                     (0x4400401CU) /**< \brief (AESB) Interrupt Status Register */
  #define REG_AESB_KEYWR                   (0x44004020U) /**< \brief (AESB) Key Word Register */
  #define REG_AESB_IDATAR                  (0x44004040U) /**< \brief (AESB) Input Data Register */
  #define REG_AESB_ODATAR                  (0x44004050U) /**< \brief (AESB) Output Data Register */
  #define REG_AESB_IVR                     (0x44004060U) /**< \brief (AESB) Initialization Vector Register */
  #define REG_AESB_EMR                     (0x440040B0U) /**< \brief (AESB) Extended Mode Register */
  #define REG_AESB_WPMR                    (0x440040E4U) /**< \brief (AESB) Write Protection Mode Register */
  #define REG_AESB_WPSR                    (0x440040E8U) /**< \brief (AESB) Write Protection Status Register */
#else
  #define REG_AESB_CR     (*(__O  uint32_t*)0x44004000U) /**< \brief (AESB) Control Register */
  #define REG_AESB_MR     (*(__IO uint32_t*)0x44004004U) /**< \brief (AESB) Mode Register */
  #define REG_AESB_IER    (*(__O  uint32_t*)0x44004010U) /**< \brief (AESB) Interrupt Enable Register */
  #define REG_AESB_IDR    (*(__O  uint32_t*)0x44004014U) /**< \brief (AESB) Interrupt Disable Register */
  #define REG_AESB_IMR    (*(__I  uint32_t*)0x44004018U) /**< \brief (AESB) Interrupt Mask Register */
  #define REG_AESB_ISR    (*(__I  uint32_t*)0x4400401CU) /**< \brief (AESB) Interrupt Status Register */
  #define REG_AESB_KEYWR  (*(__O  uint32_t*)0x44004020U) /**< \brief (AESB) Key Word Register */
  #define REG_AESB_IDATAR (*(__O  uint32_t*)0x44004040U) /**< \brief (AESB) Input Data Register */
  #define REG_AESB_ODATAR (*(__I  uint32_t*)0x44004050U) /**< \brief (AESB) Output Data Register */
  #define REG_AESB_IVR    (*(__O  uint32_t*)0x44004060U) /**< \brief (AESB) Initialization Vector Register */
  #define REG_AESB_EMR    (*(__IO uint32_t*)0x440040B0U) /**< \brief (AESB) Extended Mode Register */
  #define REG_AESB_WPMR   (*(__IO uint32_t*)0x440040E4U) /**< \brief (AESB) Write Protection Mode Register */
  #define REG_AESB_WPSR   (*(__I  uint32_t*)0x440040E8U) /**< \brief (AESB) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_AESB_INSTANCE_ */
