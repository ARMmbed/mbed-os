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

#ifndef _PIC32CX_SFR_INSTANCE_
#define _PIC32CX_SFR_INSTANCE_

/* ========== Register definition for SFR peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SFR_SRAM0_SW_CFG                    (0x40050410U) /**< \brief (SFR) SRAM0 Software Config Register */
  #define REG_SFR_SRAM0_HW_CFG                    (0x40050414U) /**< \brief (SFR) SRAM0 Hardware Status Register */
  #define REG_SFR_SRAM0_CH                        (0x40050418U) /**< \brief (SFR) SRAM0 Read Margin Register Channel */
  #define REG_SFR_SRAM                            (0x40050428U) /**< \brief (SFR) SRAM Configuration Register */
  #define REG_SFR_CPKCC                           (0x40050430U) /**< \brief (SFR) CPKCC Memory Configuration Register */
  #define REG_SFR_HROMC                           (0x40050440U) /**< \brief (SFR) HROMC Memory Configuration Register */
  #define REG_SFR_HCACHEI_VALID                   (0x40050450U) /**< \brief (SFR) HCACHEI Valid Memory Configuration Register */
  #define REG_SFR_HCACHEI_DATA                    (0x40050454U) /**< \brief (SFR) HCACHEI Data Memory Configuration Register */
  #define REG_SFR_HCACHEI_TAG                     (0x40050458U) /**< \brief (SFR) HCACHEI Tag Memory Configuration Register */
  #define REG_SFR_HCACHED_VALID                   (0x40050460U) /**< \brief (SFR) HCACHED Valid Memory Configuration Register */
  #define REG_SFR_HCACHED_DATA                    (0x40050464U) /**< \brief (SFR) HCACHED Data Memory Configuration Register */
  #define REG_SFR_HCACHED_TAG                     (0x40050468U) /**< \brief (SFR) HCACHEDTag Memory Configuration Register */
  #define REG_SFR_FLASH                           (0x40050470U) /**< \brief (SFR) Flash Memory Configuration Register */
  #define REG_SFR_OPT_LINK                        (0x40050480U) /**< \brief (SFR) Optical Link Register */
  #define REG_SFR_JTAG                            (0x40050490U) /**< \brief (SFR) JTAG Register */
  #define REG_SFR_CORE_DEBUG_CFG                  (0x400504A0U) /**< \brief (SFR) Core Debug Configuration Register */
  #define REG_SFR_EMAHB2AHB                       (0x400504A4U) /**< \brief (SFR) AHB2AHB Configuration Register */
  #define REG_SFR_SECURE                          (0x400504A8U) /**< \brief (SFR) Secure Register */
  #define REG_SFR_SECURE_BIT                      (0x400504ACU) /**< \brief (SFR) Secure Bit Register */
  #define REG_SFR_ERASE_FLASH                     (0x400504B0U) /**< \brief (SFR) Erase Flash Register */
  #define REG_SFR_PWM_DEBUG                       (0x400504B4U) /**< \brief (SFR) PWM Debug Register */
  #define REG_SFR_FFPI                            (0x400504B8U) /**< \brief (SFR) FFPI Register */
  #define REG_SFR_WAIT_MODE                       (0x400504BCU) /**< \brief (SFR) Improved Wait Mode Register */
  #define REG_SFR_ROM_CODE                        (0x400504C0U) /**< \brief (SFR) ROM Code Register */
  #define REG_SFR_WPMR                            (0x400504E4U) /**< \brief (SFR) Write Protection Mode Register */
  #define REG_SFR_WPSR                            (0x400504E8U) /**< \brief (SFR) Write Protection Status Register */
  #define REG_SFR_SPARE                           (0x400504F0U) /**< \brief (SFR) Spare Register */
  #define REG_SFR_VERSION                         (0x400504FCU) /**< \brief (SFR) Version Register */
#else
  #define REG_SFR_SRAM0_SW_CFG   (*(__IO uint32_t*)0x40050410U) /**< \brief (SFR) SRAM0 Software Config Register */
  #define REG_SFR_SRAM0_HW_CFG   (*(__I  uint32_t*)0x40050414U) /**< \brief (SFR) SRAM0 Hardware Status Register */
  #define REG_SFR_SRAM0_CH       (*(__IO uint32_t*)0x40050418U) /**< \brief (SFR) SRAM0 Read Margin Register Channel */
  #define REG_SFR_SRAM           (*(__IO uint32_t*)0x40050428U) /**< \brief (SFR) SRAM Configuration Register */
  #define REG_SFR_CPKCC          (*(__IO uint32_t*)0x40050430U) /**< \brief (SFR) CPKCC Memory Configuration Register */
  #define REG_SFR_HROMC          (*(__IO uint32_t*)0x40050440U) /**< \brief (SFR) HROMC Memory Configuration Register */
  #define REG_SFR_HCACHEI_VALID  (*(__IO uint32_t*)0x40050450U) /**< \brief (SFR) HCACHEI Valid Memory Configuration Register */
  #define REG_SFR_HCACHEI_DATA   (*(__IO uint32_t*)0x40050454U) /**< \brief (SFR) HCACHEI Data Memory Configuration Register */
  #define REG_SFR_HCACHEI_TAG    (*(__IO uint32_t*)0x40050458U) /**< \brief (SFR) HCACHEI Tag Memory Configuration Register */
  #define REG_SFR_HCACHED_VALID  (*(__IO uint32_t*)0x40050460U) /**< \brief (SFR) HCACHED Valid Memory Configuration Register */
  #define REG_SFR_HCACHED_DATA   (*(__IO uint32_t*)0x40050464U) /**< \brief (SFR) HCACHED Data Memory Configuration Register */
  #define REG_SFR_HCACHED_TAG    (*(__IO uint32_t*)0x40050468U) /**< \brief (SFR) HCACHEDTag Memory Configuration Register */
  #define REG_SFR_FLASH          (*(__IO uint32_t*)0x40050470U) /**< \brief (SFR) Flash Memory Configuration Register */
  #define REG_SFR_OPT_LINK       (*(__IO uint32_t*)0x40050480U) /**< \brief (SFR) Optical Link Register */
  #define REG_SFR_JTAG           (*(__IO uint32_t*)0x40050490U) /**< \brief (SFR) JTAG Register */
  #define REG_SFR_CORE_DEBUG_CFG (*(__IO uint32_t*)0x400504A0U) /**< \brief (SFR) Core Debug Configuration Register */
  #define REG_SFR_EMAHB2AHB      (*(__IO uint32_t*)0x400504A4U) /**< \brief (SFR) AHB2AHB Configuration Register */
  #define REG_SFR_SECURE         (*(__IO uint32_t*)0x400504A8U) /**< \brief (SFR) Secure Register */
  #define REG_SFR_SECURE_BIT     (*(__I  uint32_t*)0x400504ACU) /**< \brief (SFR) Secure Bit Register */
  #define REG_SFR_ERASE_FLASH    (*(__IO uint32_t*)0x400504B0U) /**< \brief (SFR) Erase Flash Register */
  #define REG_SFR_PWM_DEBUG      (*(__IO uint32_t*)0x400504B4U) /**< \brief (SFR) PWM Debug Register */
  #define REG_SFR_FFPI           (*(__I  uint32_t*)0x400504B8U) /**< \brief (SFR) FFPI Register */
  #define REG_SFR_WAIT_MODE      (*(__IO uint32_t*)0x400504BCU) /**< \brief (SFR) Improved Wait Mode Register */
  #define REG_SFR_ROM_CODE       (*(__I  uint32_t*)0x400504C0U) /**< \brief (SFR) ROM Code Register */
  #define REG_SFR_WPMR           (*(__IO uint32_t*)0x400504E4U) /**< \brief (SFR) Write Protection Mode Register */
  #define REG_SFR_WPSR           (*(__IO uint32_t*)0x400504E8U) /**< \brief (SFR) Write Protection Status Register */
  #define REG_SFR_SPARE          (*(__IO uint32_t*)0x400504F0U) /**< \brief (SFR) Spare Register */
  #define REG_SFR_VERSION        (*(__I  uint32_t*)0x400504FCU) /**< \brief (SFR) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_SFR_INSTANCE_ */
