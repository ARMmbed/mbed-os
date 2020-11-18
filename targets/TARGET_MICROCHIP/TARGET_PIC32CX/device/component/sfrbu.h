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

#ifndef _PIC32CX_SFRBU_COMPONENT_
#define _PIC32CX_SFRBU_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Special Function Register Backup */
/* ============================================================================= */
/** \addtogroup PIC32CX_SFRBU Special Function Register Backup */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Sfrbu hardware registers */
typedef struct {
  __I  uint32_t Reserved1[4];
  __IO uint32_t SFRBU_XTAL_TRIM;    /**< \brief (Sfrbu Offset: 0x10) XTAL Oscillator Trimming Register */
  __I  uint32_t SFRBU_TRIM;         /**< \brief (Sfrbu Offset: 0x14) Trimming Bits Register */
  __I  uint32_t Reserved2[2];
  __IO uint32_t SFRBU_BOOT;         /**< \brief (Sfrbu Offset: 0x20) Boot Register */
  __I  uint32_t Reserved3[3];
  __IO uint32_t SFRBU_IO_RETENTION; /**< \brief (Sfrbu Offset: 0x30) IO Retention Register */
  __I  uint32_t Reserved4[3];
  __IO uint32_t SFRBU_BODCORE;      /**< \brief (Sfrbu Offset: 0x40) BOD Core Register */
  __I  uint32_t Reserved5[3];
  __IO uint32_t SFRBU_PWS_CNTRL;    /**< \brief (Sfrbu Offset: 0x50) Power Switch Control Register */
  __I  uint32_t Reserved6[36];
  __IO uint32_t SFRBU_WPMR;         /**< \brief (Sfrbu Offset: 0xE4) Write Protection Mode Register */
  __IO uint32_t SFRBU_WPSR;         /**< \brief (Sfrbu Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved7[1];
  __IO uint32_t SFRBU_SPARE;        /**< \brief (Sfrbu Offset: 0xF0) Spare Register */
} Sfrbu;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SFRBU_XTAL_TRIM : (SFRBU Offset: 0x10) XTAL Oscillator Trimming Register -------- */
#define SFRBU_XTAL_TRIM_XTAL_TRIM_Pos 0
#define SFRBU_XTAL_TRIM_XTAL_TRIM_Msk (0x3u << SFRBU_XTAL_TRIM_XTAL_TRIM_Pos) /**< \brief (SFRBU_XTAL_TRIM) 32 kHz Crystal Oscillator Trimming Value */
#define SFRBU_XTAL_TRIM_XTAL_TRIM(value) ((SFRBU_XTAL_TRIM_XTAL_TRIM_Msk & ((value) << SFRBU_XTAL_TRIM_XTAL_TRIM_Pos)))
/* -------- SFRBU_TRIM : (SFRBU Offset: 0x14) Trimming Bits Register -------- */
#define SFRBU_TRIM_RC32_Pos 0
#define SFRBU_TRIM_RC32_Msk (0x3fu << SFRBU_TRIM_RC32_Pos) /**< \brief (SFRBU_TRIM) RC Oscillator Trimming Value */
#define SFRBU_TRIM_CONVBG_Pos 8
#define SFRBU_TRIM_CONVBG_Msk (0xfu << SFRBU_TRIM_CONVBG_Pos) /**< \brief (SFRBU_TRIM) Conversion IP Bandgap Trimming Value */
#define SFRBU_TRIM_LDO_Pos 16
#define SFRBU_TRIM_LDO_Msk (0xfu << SFRBU_TRIM_LDO_Pos) /**< \brief (SFRBU_TRIM) Power Management IP LDO Trimming Value */
#define SFRBU_TRIM_LCD_Pos 24
#define SFRBU_TRIM_LCD_Msk (0xfu << SFRBU_TRIM_LCD_Pos) /**< \brief (SFRBU_TRIM) LDO Internal Bandgap Trimming for LCD Driver */
/* -------- SFRBU_BOOT : (SFRBU Offset: 0x20) Boot Register -------- */
#define SFRBU_BOOT_SOURCE (0x1u << 0) /**< \brief (SFRBU_BOOT) Source for Booting Sequence */
/* -------- SFRBU_IO_RETENTION : (SFRBU Offset: 0x30) IO Retention Register -------- */
#define SFRBU_IO_RETENTION_PA_7_0 (0x1u << 0) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PA0 to PA7 */
#define SFRBU_IO_RETENTION_PA_15_8 (0x1u << 1) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PA8 to PA15 */
#define SFRBU_IO_RETENTION_PA_23_16 (0x1u << 2) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PA16 to PA23 */
#define SFRBU_IO_RETENTION_PA_31_24 (0x1u << 3) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PA24 to PA31 */
#define SFRBU_IO_RETENTION_PB_7_0 (0x1u << 4) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PB0 to PB7 */
#define SFRBU_IO_RETENTION_PB_15_8 (0x1u << 5) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PB8 to PB15 */
#define SFRBU_IO_RETENTION_PB_23_16 (0x1u << 6) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PB216 to PB23 */
#define SFRBU_IO_RETENTION_PB_26_24 (0x1u << 7) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PB24 to PB26 */
#define SFRBU_IO_RETENTION_PC_7_0 (0x1u << 8) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PC0 to PC7 */
#define SFRBU_IO_RETENTION_PC_15_8 (0x1u << 9) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PC8 to PC15 */
#define SFRBU_IO_RETENTION_PC_22_16 (0x1u << 10) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PC16 to PC22 */
#define SFRBU_IO_RETENTION_PD_7_0 (0x1u << 12) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PD0 to PD7 */
#define SFRBU_IO_RETENTION_PD_15_8 (0x1u << 13) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PD8 to PD15 */
#define SFRBU_IO_RETENTION_PD_23_16 (0x1u << 14) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PD16 to PD23 */
#define SFRBU_IO_RETENTION_PD_30_24 (0x1u << 15) /**< \brief (SFRBU_IO_RETENTION) Keep Function on PD24 to PD30 */
#define SFRBU_IO_RETENTION_NRST (0x1u << 16) /**< \brief (SFRBU_IO_RETENTION) Keep Function on NRST */
/* -------- SFRBU_BODCORE : (SFRBU Offset: 0x40) BOD Core Register -------- */
#define SFRBU_BODCORE_STATUS (0x1u << 0) /**< \brief (SFRBU_BODCORE) Core Brownout Detector */
/* -------- SFRBU_PWS_CNTRL : (SFRBU Offset: 0x50) Power Switch Control Register -------- */
#define SFRBU_PWS_CNTRL_UNMASK (0x1u << 0) /**< \brief (SFRBU_PWS_CNTRL) Unmask Power Switch Control Signals */
/* -------- SFRBU_WPMR : (SFRBU Offset: 0xE4) Write Protection Mode Register -------- */
#define SFRBU_WPMR_WPEN (0x1u << 0) /**< \brief (SFRBU_WPMR) Write Protection Enable */
#define SFRBU_WPMR_WPKEY_Pos 8
#define SFRBU_WPMR_WPKEY_Msk (0xffffffu << SFRBU_WPMR_WPKEY_Pos) /**< \brief (SFRBU_WPMR) Write Protection Key */
#define SFRBU_WPMR_WPKEY(value) ((SFRBU_WPMR_WPKEY_Msk & ((value) << SFRBU_WPMR_WPKEY_Pos)))
#define   SFRBU_WPMR_WPKEY_PASSWD (0x534652u << 8) /**< \brief (SFRBU_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- SFRBU_WPSR : (SFRBU Offset: 0xE8) Write Protection Status Register -------- */
#define SFRBU_WPSR_WPVS (0x1u << 0) /**< \brief (SFRBU_WPSR) Write Protection Violation Status */
#define SFRBU_WPSR_WPSRC_Pos 8
#define SFRBU_WPSR_WPSRC_Msk (0xffffu << SFRBU_WPSR_WPSRC_Pos) /**< \brief (SFRBU_WPSR) Write Protection Source */
#define SFRBU_WPSR_WPSRC(value) ((SFRBU_WPSR_WPSRC_Msk & ((value) << SFRBU_WPSR_WPSRC_Pos)))
/* -------- SFRBU_SPARE : (SFRBU Offset: 0xF0) Spare Register -------- */
#define SFRBU_SPARE_VALUE_Pos 0
#define SFRBU_SPARE_VALUE_Msk (0xffffffffu << SFRBU_SPARE_VALUE_Pos) /**< \brief (SFRBU_SPARE) Spare */
#define SFRBU_SPARE_VALUE(value) ((SFRBU_SPARE_VALUE_Msk & ((value) << SFRBU_SPARE_VALUE_Pos)))

/*@}*/


#endif /* _PIC32CX_SFRBU_COMPONENT_ */
