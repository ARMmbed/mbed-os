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

#ifndef _PIC32CX_DWDT_COMPONENT_
#define _PIC32CX_DWDT_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Dual Watchdog Timer */
/* ============================================================================= */
/** \addtogroup PIC32CX_DWDT Dual Watchdog Timer */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Dwdt hardware registers */
typedef struct {
  __O  uint32_t WDT1_CR;         /**< \brief (Dwdt Offset: 0x0) Watchdog 1 Control Register */
  __IO uint32_t WDT1_MR;         /**< \brief (Dwdt Offset: 0x4) Watchdog 1 Mode Register */
  __I  uint32_t WDT1_VR;         /**< \brief (Dwdt Offset: 0x8) Watchdog 1 Value Register */
  __IO uint32_t WDT1_WL;         /**< \brief (Dwdt Offset: 0xC) Watchdog 1 Window Level Register */
  __IO uint32_t WDT1_IL;         /**< \brief (Dwdt Offset: 0x10) Watchdog 1 Interrupt Level Register */
  __O  uint32_t WDT1_IER;        /**< \brief (Dwdt Offset: 0x14) Watchdog 1 Interrupt Enable Register */
  __O  uint32_t WDT1_IDR;        /**< \brief (Dwdt Offset: 0x18) Watchdog 1 Interrupt Disable Register */
  __I  uint32_t WDT1_ISR;        /**< \brief (Dwdt Offset: 0x1C) Watchdog 1 Interrupt Status Register */
  __I  uint32_t WDT1_IMR;        /**< \brief (Dwdt Offset: 0x20) Watchdog 1 Interrupt Mask Register */
  __I  uint32_t Reserved1[1147];
  __O  uint32_t WDT0_CR;         /**< \brief (Dwdt Offset: 0x1210) Watchdog 0 Control Register */
  __IO uint32_t WDT0_MR;         /**< \brief (Dwdt Offset: 0x1214) Watchdog 0 Mode Register */
  __I  uint32_t WDT0_VR;         /**< \brief (Dwdt Offset: 0x1218) Watchdog 0 Value Register */
  __IO uint32_t WDT0_WL;         /**< \brief (Dwdt Offset: 0x121C) Watchdog 0 Window Level */
  __IO uint32_t WDT0_IL;         /**< \brief (Dwdt Offset: 0x1220) Watchdog 0 Interrupt Level */
  __O  uint32_t WDT0_IER;        /**< \brief (Dwdt Offset: 0x1224) Watchdog 0 Interrupt Enable Register */
  __O  uint32_t WDT0_IDR;        /**< \brief (Dwdt Offset: 0x1228) Watchdog 0 Interrupt Disable Register */
  __I  uint32_t WDT0_ISR;        /**< \brief (Dwdt Offset: 0x122C) Watchdog 0 Interrupt Status Register */
  __I  uint32_t WDT0_IMR;        /**< \brief (Dwdt Offset: 0x1230) Watchdog 0 Interrupt Mask Register */
  __IO uint32_t WDT1_LVLLIM;     /**< \brief (Dwdt Offset: 0x1234) Watchdog 1 Level Limits Register */
  __IO uint32_t WDT1_RLIM;       /**< \brief (Dwdt Offset: 0x1238) Watchdog 1 Repeat Limits Register */
  __IO uint32_t WDT1_PLIM;       /**< \brief (Dwdt Offset: 0x123C) Watchdog 1 Period Limits Register */
} Dwdt;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- WDT1_CR : (DWDT Offset: 0x0) Watchdog 1 Control Register -------- */
#define WDT1_CR_WDRSTT (0x1u << 0) /**< \brief (WDT1_CR) Watchdog Restart */
#define WDT1_CR_LOCKMR (0x1u << 4) /**< \brief (WDT1_CR) Lock Mode Register Write Access */
#define WDT1_CR_KEY_Pos 24
#define WDT1_CR_KEY_Msk (0xffu << WDT1_CR_KEY_Pos) /**< \brief (WDT1_CR) Password */
#define WDT1_CR_KEY(value) ((WDT1_CR_KEY_Msk & ((value) << WDT1_CR_KEY_Pos)))
#define   WDT1_CR_KEY_PASSWD (0xA5u << 24) /**< \brief (WDT1_CR) Writing any other value in this field aborts the write operation. */
/* -------- WDT1_MR : (DWDT Offset: 0x4) Watchdog 1 Mode Register -------- */
#define WDT1_MR_PERIODRST (0x1u << 4) /**< \brief (WDT1_MR) Watchdog Overflow Period Reset Enable */
#define WDT1_MR_RPTHRST (0x1u << 5) /**< \brief (WDT1_MR) Repeat Threshold Reset Enable */
#define WDT1_MR_WDRPROC (0x1u << 8) /**< \brief (WDT1_MR) Watchdog Reset Processor */
#define WDT1_MR_WDNRSTDIS (0x1u << 9) /**< \brief (WDT1_MR) Watchdog Reset NRST Pin Disable */
#define WDT1_MR_WDDIS (0x1u << 12) /**< \brief (WDT1_MR) Watchdog Disable */
#define WDT1_MR_WDIDLEHLT (0x1u << 28) /**< \brief (WDT1_MR) Watchdog Idle Halt */
#define WDT1_MR_WDDBG0HLT (0x1u << 29) /**< \brief (WDT1_MR) Watchdog Core 0 Debug Halt */
#define WDT1_MR_WDDBG1HLT (0x1u << 30) /**< \brief (WDT1_MR) Watchdog Core 1 Debug Halt */
/* -------- WDT1_VR : (DWDT Offset: 0x8) Watchdog 1 Value Register -------- */
#define WDT1_VR_COUNTER_Pos 0
#define WDT1_VR_COUNTER_Msk (0xfffu << WDT1_VR_COUNTER_Pos) /**< \brief (WDT1_VR) Watchdog Down Counter Value */
/* -------- WDT1_WL : (DWDT Offset: 0xC) Watchdog 1 Window Level Register -------- */
#define WDT1_WL_PERIOD_Pos 0
#define WDT1_WL_PERIOD_Msk (0xfffu << WDT1_WL_PERIOD_Pos) /**< \brief (WDT1_WL) Watchdog Period */
#define WDT1_WL_PERIOD(value) ((WDT1_WL_PERIOD_Msk & ((value) << WDT1_WL_PERIOD_Pos)))
#define WDT1_WL_RPTH_Pos 16
#define WDT1_WL_RPTH_Msk (0xfffu << WDT1_WL_RPTH_Pos) /**< \brief (WDT1_WL) Repeat Threshold */
#define WDT1_WL_RPTH(value) ((WDT1_WL_RPTH_Msk & ((value) << WDT1_WL_RPTH_Pos)))
/* -------- WDT1_IL : (DWDT Offset: 0x10) Watchdog 1 Interrupt Level Register -------- */
#define WDT1_IL_LVLTH_Pos 0
#define WDT1_IL_LVLTH_Msk (0xfffu << WDT1_IL_LVLTH_Pos) /**< \brief (WDT1_IL) Level Threshold */
#define WDT1_IL_LVLTH(value) ((WDT1_IL_LVLTH_Msk & ((value) << WDT1_IL_LVLTH_Pos)))
#define WDT1_IL_PRESC_Pos 16
#define WDT1_IL_PRESC_Msk (0x7u << WDT1_IL_PRESC_Pos) /**< \brief (WDT1_IL) Prescaler Ratio */
#define WDT1_IL_PRESC(value) ((WDT1_IL_PRESC_Msk & ((value) << WDT1_IL_PRESC_Pos)))
#define   WDT1_IL_PRESC_RATIO128 (0x0u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 128. */
#define   WDT1_IL_PRESC_RATIO64 (0x1u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 64. */
#define   WDT1_IL_PRESC_RATIO32 (0x2u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 32. */
#define   WDT1_IL_PRESC_RATIO16 (0x3u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 16. */
#define   WDT1_IL_PRESC_RATIO8 (0x4u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 8. */
#define   WDT1_IL_PRESC_RATIO4 (0x5u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 4. */
#define   WDT1_IL_PRESC_RATIO2 (0x6u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 2. */
#define   WDT1_IL_PRESC_RATIO1 (0x7u << 16) /**< \brief (WDT1_IL) The watchdog counter decreased when the prescaler reaches 1. */
/* -------- WDT1_IER : (DWDT Offset: 0x14) Watchdog 1 Interrupt Enable Register -------- */
#define WDT1_IER_PERINT (0x1u << 0) /**< \brief (WDT1_IER) Overflow Period Interrupt Enable */
#define WDT1_IER_RPTHINT (0x1u << 1) /**< \brief (WDT1_IER) Reload Repeat Period Interrupt Enable */
#define WDT1_IER_LVLINT (0x1u << 2) /**< \brief (WDT1_IER) Interrupt Level Threshold Interrupt Enable */
#define WDT1_IER_RLDERR (0x1u << 5) /**< \brief (WDT1_IER) Reload Command Error Interrupt Enable */
/* -------- WDT1_IDR : (DWDT Offset: 0x18) Watchdog 1 Interrupt Disable Register -------- */
#define WDT1_IDR_PERINT (0x1u << 0) /**< \brief (WDT1_IDR) Overflow Period Interrupt Disable */
#define WDT1_IDR_RPTHINT (0x1u << 1) /**< \brief (WDT1_IDR) Reload Repeat Period Interrupt Disable */
#define WDT1_IDR_LVLINT (0x1u << 2) /**< \brief (WDT1_IDR) Interrupt Level Threshold Interrupt Disable */
#define WDT1_IDR_RLDERR (0x1u << 5) /**< \brief (WDT1_IDR) Reload Command Error Interrupt Disable */
/* -------- WDT1_ISR : (DWDT Offset: 0x1C) Watchdog 1 Interrupt Status Register -------- */
#define WDT1_ISR_PERINT (0x1u << 0) /**< \brief (WDT1_ISR) Overflow Period Status (cleared on read) */
#define WDT1_ISR_RPTHINT (0x1u << 1) /**< \brief (WDT1_ISR) Reload Repeat Period Status (cleared on read) */
#define WDT1_ISR_LVLINT (0x1u << 2) /**< \brief (WDT1_ISR) Interrupt Level Threshold Status (cleared on read) */
#define WDT1_ISR_RLDERR (0x1u << 5) /**< \brief (WDT1_ISR) Reload Command Error Status (cleared on read) */
/* -------- WDT1_IMR : (DWDT Offset: 0x20) Watchdog 1 Interrupt Mask Register -------- */
#define WDT1_IMR_PERINT (0x1u << 0) /**< \brief (WDT1_IMR) Overflow Period Interrupt Mask */
#define WDT1_IMR_RPTHINT (0x1u << 1) /**< \brief (WDT1_IMR) Reload Repeat Period Interrupt Mask */
#define WDT1_IMR_LVLINT (0x1u << 2) /**< \brief (WDT1_IMR) Interrupt Level Threshold Interrupt Mask */
#define WDT1_IMR_RLDERR (0x1u << 5) /**< \brief (WDT1_IMR) Reload Command Error Interrupt Mask */
/* -------- WDT0_CR : (DWDT Offset: 0x1210) Watchdog 0 Control Register -------- */
#define WDT0_CR_WDRSTT (0x1u << 0) /**< \brief (WDT0_CR) Watchdog Restart */
#define WDT0_CR_LOCKMR (0x1u << 4) /**< \brief (WDT0_CR) Lock Mode Register Write Access */
#define WDT0_CR_KEY_Pos 24
#define WDT0_CR_KEY_Msk (0xffu << WDT0_CR_KEY_Pos) /**< \brief (WDT0_CR) Password */
#define WDT0_CR_KEY(value) ((WDT0_CR_KEY_Msk & ((value) << WDT0_CR_KEY_Pos)))
#define   WDT0_CR_KEY_PASSWD (0xA5u << 24) /**< \brief (WDT0_CR) Writing any other value in this field aborts the write operation. */
/* -------- WDT0_MR : (DWDT Offset: 0x1214) Watchdog 0 Mode Register -------- */
#define WDT0_MR_PERIODRST (0x1u << 4) /**< \brief (WDT0_MR) Period Reset */
#define WDT0_MR_RPTHRST (0x1u << 5) /**< \brief (WDT0_MR) Repeat Threshold Reset */
#define WDT0_MR_WDNRSTDIS (0x1u << 9) /**< \brief (WDT0_MR) Watchdog NRST Disable */
#define WDT0_MR_WDDIS (0x1u << 12) /**< \brief (WDT0_MR) Watchdog Disable */
#define WDT0_MR_WDIDLEHLT (0x1u << 28) /**< \brief (WDT0_MR) Watchdog Idle Halt */
#define WDT0_MR_WDDBG0HLT (0x1u << 29) /**< \brief (WDT0_MR) Watchdog Core 0 Debug Halt */
#define WDT0_MR_WDDBG1HLT (0x1u << 30) /**< \brief (WDT0_MR) Watchdog Core 1 Debug Halt */
/* -------- WDT0_VR : (DWDT Offset: 0x1218) Watchdog 0 Value Register -------- */
#define WDT0_VR_COUNTER_Pos 0
#define WDT0_VR_COUNTER_Msk (0xfffu << WDT0_VR_COUNTER_Pos) /**< \brief (WDT0_VR) Watchdog Down Counter Value */
/* -------- WDT0_WL : (DWDT Offset: 0x121C) Watchdog 0 Window Level -------- */
#define WDT0_WL_PERIOD_Pos 0
#define WDT0_WL_PERIOD_Msk (0xfffu << WDT0_WL_PERIOD_Pos) /**< \brief (WDT0_WL) Watchdog Period */
#define WDT0_WL_PERIOD(value) ((WDT0_WL_PERIOD_Msk & ((value) << WDT0_WL_PERIOD_Pos)))
#define WDT0_WL_RPTH_Pos 16
#define WDT0_WL_RPTH_Msk (0xfffu << WDT0_WL_RPTH_Pos) /**< \brief (WDT0_WL) Repeat Threshold */
#define WDT0_WL_RPTH(value) ((WDT0_WL_RPTH_Msk & ((value) << WDT0_WL_RPTH_Pos)))
/* -------- WDT0_IL : (DWDT Offset: 0x1220) Watchdog 0 Interrupt Level -------- */
#define WDT0_IL_LVLTH_Pos 0
#define WDT0_IL_LVLTH_Msk (0xfffu << WDT0_IL_LVLTH_Pos) /**< \brief (WDT0_IL) Level Threshold */
#define WDT0_IL_LVLTH(value) ((WDT0_IL_LVLTH_Msk & ((value) << WDT0_IL_LVLTH_Pos)))
#define WDT0_IL_PRESC_Pos 16
#define WDT0_IL_PRESC_Msk (0x7u << WDT0_IL_PRESC_Pos) /**< \brief (WDT0_IL) Prescaler Ratio */
#define WDT0_IL_PRESC(value) ((WDT0_IL_PRESC_Msk & ((value) << WDT0_IL_PRESC_Pos)))
#define   WDT0_IL_PRESC_RATIO128 (0x0u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 128. */
#define   WDT0_IL_PRESC_RATIO64 (0x1u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 64. */
#define   WDT0_IL_PRESC_RATIO32 (0x2u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 32. */
#define   WDT0_IL_PRESC_RATIO16 (0x3u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 16. */
#define   WDT0_IL_PRESC_RATIO8 (0x4u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 8. */
#define   WDT0_IL_PRESC_RATIO4 (0x5u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 4. */
#define   WDT0_IL_PRESC_RATIO2 (0x6u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 2. */
#define   WDT0_IL_PRESC_RATIO1 (0x7u << 16) /**< \brief (WDT0_IL) The watchdog counter decreased when the prescaler reaches 1. */
/* -------- WDT0_IER : (DWDT Offset: 0x1224) Watchdog 0 Interrupt Enable Register -------- */
#define WDT0_IER_PERINT (0x1u << 0) /**< \brief (WDT0_IER) Overflow Period Interrupt Enable */
#define WDT0_IER_RPTHINT (0x1u << 1) /**< \brief (WDT0_IER) Reload Repeat Period Interrupt Enable */
#define WDT0_IER_LVLINT (0x1u << 2) /**< \brief (WDT0_IER) Interrupt Level Threshold Interrupt Enable */
#define WDT0_IER_W1PERINT (0x1u << 3) /**< \brief (WDT0_IER) Watchdog 1 Overflow Interrupt Enable */
#define WDT0_IER_W1RPTHINT (0x1u << 4) /**< \brief (WDT0_IER) Watchdog 1 Repeat Threshold Interrupt Enable */
#define WDT0_IER_RLDERR (0x1u << 5) /**< \brief (WDT0_IER) Reload Command Error Interrupt Enable */
/* -------- WDT0_IDR : (DWDT Offset: 0x1228) Watchdog 0 Interrupt Disable Register -------- */
#define WDT0_IDR_PERINT (0x1u << 0) /**< \brief (WDT0_IDR) Overflow Period Interrupt Disable */
#define WDT0_IDR_RPTHINT (0x1u << 1) /**< \brief (WDT0_IDR) Reload Repeat Period Interrupt Disable */
#define WDT0_IDR_LVLINT (0x1u << 2) /**< \brief (WDT0_IDR) Interrupt Level Threshold Interrupt Disable */
#define WDT0_IDR_W1PERINT (0x1u << 3) /**< \brief (WDT0_IDR) Watchdog 1 Overflow Interrupt Disable */
#define WDT0_IDR_W1RPTHINT (0x1u << 4) /**< \brief (WDT0_IDR) Watchdog 1 Repeat Threshold Interrupt Disable */
#define WDT0_IDR_RLDERR (0x1u << 5) /**< \brief (WDT0_IDR) Reload Command Error Interrupt Disable */
/* -------- WDT0_ISR : (DWDT Offset: 0x122C) Watchdog 0 Interrupt Status Register -------- */
#define WDT0_ISR_PERINT (0x1u << 0) /**< \brief (WDT0_ISR) Overflow Period Status (cleared on read) */
#define WDT0_ISR_RPTHINT (0x1u << 1) /**< \brief (WDT0_ISR) Reload Repeat Period Status (cleared on read) */
#define WDT0_ISR_LVLINT (0x1u << 2) /**< \brief (WDT0_ISR) Interrupt Level Threshold Status (cleared on read) */
#define WDT0_ISR_W1PERINT (0x1u << 3) /**< \brief (WDT0_ISR) Watchdog 1 Overflow Interrupt Status */
#define WDT0_ISR_W1RPTHINT (0x1u << 4) /**< \brief (WDT0_ISR) Watchdog 1 Repeat Threshold Interrupt Status */
#define WDT0_ISR_RLDERR (0x1u << 5) /**< \brief (WDT0_ISR) Reload Command Error Status (cleared on read) */
/* -------- WDT0_IMR : (DWDT Offset: 0x1230) Watchdog 0 Interrupt Mask Register -------- */
#define WDT0_IMR_PERINT (0x1u << 0) /**< \brief (WDT0_IMR) Overflow Period Interrupt Mask */
#define WDT0_IMR_RPTHINT (0x1u << 1) /**< \brief (WDT0_IMR) Reload Repeat Period Interrupt Mask */
#define WDT0_IMR_LVLINT (0x1u << 2) /**< \brief (WDT0_IMR) Interrupt Level Threshold Interrupt Mask */
#define WDT0_IMR_W1PERINT (0x1u << 3) /**< \brief (WDT0_IMR) Watchdog 1 Overflow Interrupt Mask */
#define WDT0_IMR_W1RPTHINT (0x1u << 4) /**< \brief (WDT0_IMR) Watchdog 1 Repeat Threshold Interrupt Mask */
#define WDT0_IMR_RLDERR (0x1u << 5) /**< \brief (WDT0_IMR) Reload Command Error Interrupt Mask */
/* -------- WDT1_LVLLIM : (DWDT Offset: 0x1234) Watchdog 1 Level Limits Register -------- */
#define WDT1_LVLLIM_LVLMIN_Pos 0
#define WDT1_LVLLIM_LVLMIN_Msk (0xfffu << WDT1_LVLLIM_LVLMIN_Pos) /**< \brief (WDT1_LVLLIM) Minimum Level */
#define WDT1_LVLLIM_LVLMIN(value) ((WDT1_LVLLIM_LVLMIN_Msk & ((value) << WDT1_LVLLIM_LVLMIN_Pos)))
#define WDT1_LVLLIM_LVLMAX_Pos 16
#define WDT1_LVLLIM_LVLMAX_Msk (0xfffu << WDT1_LVLLIM_LVLMAX_Pos) /**< \brief (WDT1_LVLLIM) Maximum Level */
#define WDT1_LVLLIM_LVLMAX(value) ((WDT1_LVLLIM_LVLMAX_Msk & ((value) << WDT1_LVLLIM_LVLMAX_Pos)))
/* -------- WDT1_RLIM : (DWDT Offset: 0x1238) Watchdog 1 Repeat Limits Register -------- */
#define WDT1_RLIM_RPTHMIN_Pos 0
#define WDT1_RLIM_RPTHMIN_Msk (0xfffu << WDT1_RLIM_RPTHMIN_Pos) /**< \brief (WDT1_RLIM) Minimum Repeat Threshold */
#define WDT1_RLIM_RPTHMIN(value) ((WDT1_RLIM_RPTHMIN_Msk & ((value) << WDT1_RLIM_RPTHMIN_Pos)))
#define WDT1_RLIM_RPTHMAX_Pos 16
#define WDT1_RLIM_RPTHMAX_Msk (0xfffu << WDT1_RLIM_RPTHMAX_Pos) /**< \brief (WDT1_RLIM) Maximum Repeat Threshold */
#define WDT1_RLIM_RPTHMAX(value) ((WDT1_RLIM_RPTHMAX_Msk & ((value) << WDT1_RLIM_RPTHMAX_Pos)))
/* -------- WDT1_PLIM : (DWDT Offset: 0x123C) Watchdog 1 Period Limits Register -------- */
#define WDT1_PLIM_PERMIN_Pos 0
#define WDT1_PLIM_PERMIN_Msk (0xfffu << WDT1_PLIM_PERMIN_Pos) /**< \brief (WDT1_PLIM) Minimum Period */
#define WDT1_PLIM_PERMIN(value) ((WDT1_PLIM_PERMIN_Msk & ((value) << WDT1_PLIM_PERMIN_Pos)))
#define WDT1_PLIM_PERMAX_Pos 16
#define WDT1_PLIM_PERMAX_Msk (0xfffu << WDT1_PLIM_PERMAX_Pos) /**< \brief (WDT1_PLIM) Maximum Period */
#define WDT1_PLIM_PERMAX(value) ((WDT1_PLIM_PERMAX_Msk & ((value) << WDT1_PLIM_PERMAX_Pos)))

/*@}*/


#endif /* _PIC32CX_DWDT_COMPONENT_ */
