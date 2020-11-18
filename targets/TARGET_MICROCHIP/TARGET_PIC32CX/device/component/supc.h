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

#ifndef _PIC32CX_SUPC_COMPONENT_
#define _PIC32CX_SUPC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Supply Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_SUPC Supply Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Supc hardware registers */
typedef struct {
  __O  uint32_t SUPC_CR;      /**< \brief (Supc Offset: 0x0) Control Register */
  __IO uint32_t SUPC_SMMR;    /**< \brief (Supc Offset: 0x4) Supply Monitor Mode Register */
  __IO uint32_t SUPC_MR;      /**< \brief (Supc Offset: 0x8) Mode Register */
  __IO uint32_t SUPC_WUMR;    /**< \brief (Supc Offset: 0xC) Wakeup Mode Register */
  __IO uint32_t SUPC_WUIR;    /**< \brief (Supc Offset: 0x10) Wakeup Inputs Register */
  __I  uint32_t SUPC_SR;      /**< \brief (Supc Offset: 0x14) Status Register */
  __I  uint32_t Reserved1[1];
  __IO uint32_t SUPC_EMR;     /**< \brief (Supc Offset: 0x1C) Extended Mode Register */
  __IO uint32_t SUPC_BMR;     /**< \brief (Supc Offset: 0x20) Backup Mode Register */
  __I  uint32_t SUPC_WUSR;    /**< \brief (Supc Offset: 0x24) Wakeup Status Register */
  __O  uint32_t SUPC_IER;     /**< \brief (Supc Offset: 0x28) Enable Interrupt Register */
  __O  uint32_t SUPC_IDR;     /**< \brief (Supc Offset: 0x2C) Disable Interrupt Register */
  __I  uint32_t SUPC_IMR;     /**< \brief (Supc Offset: 0x30) Mask Interrupt Register */
  __I  uint32_t SUPC_ISR;     /**< \brief (Supc Offset: 0x34) Status Interrupt Register */
} Supc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SUPC_CR : (SUPC Offset: 0x0) Control Register -------- */
#define SUPC_CR_SHDW (0x1u << 0) /**< \brief (SUPC_CR) Shutdown */
#define SUPC_CR_SHDWEOF (0x1u << 1) /**< \brief (SUPC_CR) Shutdown End Of Frame */
#define SUPC_CR_VROFF (0x1u << 2) /**< \brief (SUPC_CR) Voltage Regulator Off */
#define   SUPC_CR_VROFF_NO_EFFECT (0x0u << 2) /**< \brief (SUPC_CR) No effect. */
#define   SUPC_CR_VROFF_STOP_VREG (0x1u << 2) /**< \brief (SUPC_CR) If KEY=0xA5, VROFF asserts the VDDCORE domain reset and stops the voltage regulator. */
#define SUPC_CR_TDXTALSEL (0x1u << 3) /**< \brief (SUPC_CR) Timing Domain Clock on Slow Crystal Oscillator */
#define   SUPC_CR_TDXTALSEL_NO_EFFECT (0x0u << 3) /**< \brief (SUPC_CR) No effect. */
#define   SUPC_CR_TDXTALSEL_XTAL32K (0x1u << 3) /**< \brief (SUPC_CR) If KEY=0xA5, TDXTALSEL switches the slow clock of the timing domain (TD_SLCK) on the 32.768 kHz crystal oscillator output. */
#define SUPC_CR_KEY_Pos 24
#define SUPC_CR_KEY_Msk (0xffu << SUPC_CR_KEY_Pos) /**< \brief (SUPC_CR) Password */
#define SUPC_CR_KEY(value) ((SUPC_CR_KEY_Msk & ((value) << SUPC_CR_KEY_Pos)))
#define   SUPC_CR_KEY_PASSWD (0xA5u << 24) /**< \brief (SUPC_CR) Writing any other value in this field aborts the write operation. */
/* -------- SUPC_SMMR : (SUPC Offset: 0x4) Supply Monitor Mode Register -------- */
#define SUPC_SMMR_IOSMTH_Pos 0
#define SUPC_SMMR_IOSMTH_Msk (0xfu << SUPC_SMMR_IOSMTH_Pos) /**< \brief (SUPC_SMMR) VDDIO Supply Monitor Threshold */
#define SUPC_SMMR_IOSMTH(value) ((SUPC_SMMR_IOSMTH_Msk & ((value) << SUPC_SMMR_IOSMTH_Pos)))
#define SUPC_SMMR_IOSMSMPL_Pos 8
#define SUPC_SMMR_IOSMSMPL_Msk (0x7u << SUPC_SMMR_IOSMSMPL_Pos) /**< \brief (SUPC_SMMR) VDDIO Supply Monitor Sampling Period */
#define SUPC_SMMR_IOSMSMPL(value) ((SUPC_SMMR_IOSMSMPL_Msk & ((value) << SUPC_SMMR_IOSMSMPL_Pos)))
#define   SUPC_SMMR_IOSMSMPL_DISABLED (0x0u << 8) /**< \brief (SUPC_SMMR) VDDIO supply monitor is disabled */
#define   SUPC_SMMR_IOSMSMPL_ALWAYS_ON (0x1u << 8) /**< \brief (SUPC_SMMR) Continuous VDDIO supply monitoring */
#define   SUPC_SMMR_IOSMSMPL_32SLCK (0x2u << 8) /**< \brief (SUPC_SMMR) VDDIO supply monitor is enabled for 1 period every 32 MD_SLCK periods Energy optimization in Backup mode with VDDIO supplied. */
#define   SUPC_SMMR_IOSMSMPL_256SLCK (0x3u << 8) /**< \brief (SUPC_SMMR) VDDIO supply monitor is enabled for 1 period every 256 MD_SLCK periods Energy optimization in Backup mode with VDDIO supplied. */
#define   SUPC_SMMR_IOSMSMPL_2048SLCK (0x4u << 8) /**< \brief (SUPC_SMMR) VDDIO supply monitor is enabled for 1 period every 2048 MD_SLCK periods Energy optimization in Backup mode with VDDIO supplied. */
#define SUPC_SMMR_IOSMRSTEN (0x1u << 12) /**< \brief (SUPC_SMMR) VDDIO Supply Monitor Reset Enable */
#define SUPC_SMMR_IOSMPWRM (0x1u << 13) /**< \brief (SUPC_SMMR) VDDIO Supply Monitor Power Supply Mode */
#define   SUPC_SMMR_IOSMPWRM_MANUAL (0x0u << 13) /**< \brief (SUPC_SMMR) The VDDBU power source selection is controlled by configuring the bit RSTC_MR.PWRSW. */
#define   SUPC_SMMR_IOSMPWRM_AUTO_IOSM (0x1u << 13) /**< \brief (SUPC_SMMR) The VDDBU power source is VBAT when a VDDIO under voltage is detected by VDDIO Supply Monitor. */
/* -------- SUPC_MR : (SUPC Offset: 0x8) Mode Register -------- */
#define SUPC_MR_LCDOUT_Pos 0
#define SUPC_MR_LCDOUT_Msk (0xfu << SUPC_MR_LCDOUT_Pos) /**< \brief (SUPC_MR) LCD Voltage Regulator Output */
#define SUPC_MR_LCDOUT(value) ((SUPC_MR_LCDOUT_Msk & ((value) << SUPC_MR_LCDOUT_Pos)))
#define SUPC_MR_LCDMODE_Pos 4
#define SUPC_MR_LCDMODE_Msk (0x3u << SUPC_MR_LCDMODE_Pos) /**< \brief (SUPC_MR) LCD Controller Mode of Operation */
#define SUPC_MR_LCDMODE(value) ((SUPC_MR_LCDMODE_Msk & ((value) << SUPC_MR_LCDMODE_Pos)))
#define   SUPC_MR_LCDMODE_LCDOFF (0x0u << 4) /**< \brief (SUPC_MR) The internal supply source and the external supply source are both deselected (OFF Mode). */
#define   SUPC_MR_LCDMODE_LCDON_EXTVR (0x2u << 4) /**< \brief (SUPC_MR) The external supply source for LCD (VDDLCD) is selected (the embedded LCD voltage regulator is in High-impedance mode). */
#define   SUPC_MR_LCDMODE_LCDON_INVR (0x3u << 4) /**< \brief (SUPC_MR) The internal voltage regulator for VDDLCD is selected (Active mode). */
#define SUPC_MR_CORSMM (0x1u << 6) /**< \brief (SUPC_MR) VDDCORE Supply Monitor Output Mode */
#define   SUPC_MR_CORSMM_NO_EFFECT (0x0u << 6) /**< \brief (SUPC_MR) VDDCORE supply monitor output value has no effect. Power-on is performed whatever the value of the supply monitor output. */
#define   SUPC_MR_CORSMM_VALID_VDD (0x1u << 6) /**< \brief (SUPC_MR) VDDCORE supply monitor output value is checked to validate the VDDCORE domain power-on. */
#define SUPC_MR_VREGDIS (0x1u << 7) /**< \brief (SUPC_MR) Internal VDDCORE Voltage Regulator Disable */
#define   SUPC_MR_VREGDIS_INT_VREG (0x0u << 7) /**< \brief (SUPC_MR) Internal VDDCORE voltage regulator is enabled. */
#define   SUPC_MR_VREGDIS_EXT_VREG (0x1u << 7) /**< \brief (SUPC_MR) Internal VDDCORE voltage regulator is disabled (external power supply is used). */
#define SUPC_MR_CORSMRSTEN (0x1u << 12) /**< \brief (SUPC_MR) VDDCORE Supply Monitor Reset Enable */
#define SUPC_MR_CORSMDIS (0x1u << 13) /**< \brief (SUPC_MR) VDDCORE Supply Monitor Disable */
#define SUPC_MR_VDDIOSMEN (0x1u << 14) /**< \brief (SUPC_MR) VDDIO Supply Monitor Enable */
#define SUPC_MR_OSCBYPASS (0x1u << 20) /**< \brief (SUPC_MR) Slow Crystal Oscillator Bypass */
#define   SUPC_MR_OSCBYPASS_NO_EFFECT (0x0u << 20) /**< \brief (SUPC_MR) No effect. Clock selection depends on the value of the bit SUPC_CR.TDXTALSEL. */
#define   SUPC_MR_OSCBYPASS_BYPASS (0x1u << 20) /**< \brief (SUPC_MR) The slow crystal oscillator is bypassed if SUPC_CR.TDXTALSEL=1. The bit OSCBYPASS must be set prior to setting the bit TDXTALSEL. */
#define SUPC_MR_FXTALSTUP (0x1u << 22) /**< \brief (SUPC_MR) Fast Start-up 32.768 kHz Crystal Oscillator */
#define SUPC_MR_KEY_Pos 24
#define SUPC_MR_KEY_Msk (0xffu << SUPC_MR_KEY_Pos) /**< \brief (SUPC_MR) Password Key */
#define SUPC_MR_KEY(value) ((SUPC_MR_KEY_Msk & ((value) << SUPC_MR_KEY_Pos)))
#define   SUPC_MR_KEY_PASSWD1 (0x59u << 24) /**< \brief (SUPC_MR) Modifies the VREGDIS bit. Other bits are not affected. */
#define   SUPC_MR_KEY_PASSWD (0xA5u << 24) /**< \brief (SUPC_MR) Writing any other value in this field aborts the write operation. */
/* -------- SUPC_WUMR : (SUPC Offset: 0xC) Wakeup Mode Register -------- */
#define SUPC_WUMR_LPDBCEN0 (0x1u << 0) /**< \brief (SUPC_WUMR) Tamper Enable for WKUPx Pin */
#define SUPC_WUMR_LPDBCEN1 (0x1u << 1) /**< \brief (SUPC_WUMR) Tamper Enable for WKUPx Pin */
#define SUPC_WUMR_LPDBCEN2 (0x1u << 2) /**< \brief (SUPC_WUMR) Tamper Enable for WKUPx Pin */
#define SUPC_WUMR_LPDBCEN3 (0x1u << 3) /**< \brief (SUPC_WUMR) Tamper Enable for WKUPx Pin */
#define SUPC_WUMR_LPDBCEN4 (0x1u << 4) /**< \brief (SUPC_WUMR) Tamper Enable for WKUPx Pin */
#define SUPC_WUMR_FWUPDBC_Pos 8
#define SUPC_WUMR_FWUPDBC_Msk (0x7u << SUPC_WUMR_FWUPDBC_Pos) /**< \brief (SUPC_WUMR) Force Wake-up Inputs Debouncer Period */
#define SUPC_WUMR_FWUPDBC(value) ((SUPC_WUMR_FWUPDBC_Msk & ((value) << SUPC_WUMR_FWUPDBC_Pos)))
#define   SUPC_WUMR_FWUPDBC_IMMEDIATE (0x0u << 8) /**< \brief (SUPC_WUMR) Immediate, no debouncing, detected active at least on one Slow Clock edge. */
#define   SUPC_WUMR_FWUPDBC_3_SK (0x1u << 8) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 3 MD_SLCK periods */
#define   SUPC_WUMR_FWUPDBC_32_SK (0x2u << 8) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 32 MD_SLCK periods */
#define   SUPC_WUMR_FWUPDBC_512_SK (0x3u << 8) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 512 MD_SLCK periods */
#define   SUPC_WUMR_FWUPDBC_4096_SK (0x4u << 8) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 4,096 MD_SLCK periods */
#define   SUPC_WUMR_FWUPDBC_32768_SK (0x5u << 8) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 32,768 MD_SLCK periods */
#define SUPC_WUMR_WKUPDBC_Pos 12
#define SUPC_WUMR_WKUPDBC_Msk (0x7u << SUPC_WUMR_WKUPDBC_Pos) /**< \brief (SUPC_WUMR) Wake-up Inputs Debouncer Period */
#define SUPC_WUMR_WKUPDBC(value) ((SUPC_WUMR_WKUPDBC_Msk & ((value) << SUPC_WUMR_WKUPDBC_Pos)))
#define   SUPC_WUMR_WKUPDBC_IMMEDIATE (0x0u << 12) /**< \brief (SUPC_WUMR) Immediate, no debouncing, detected active at least on one Slow Clock edge. */
#define   SUPC_WUMR_WKUPDBC_3_SK (0x1u << 12) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 3 MD_SLCK periods */
#define   SUPC_WUMR_WKUPDBC_32_SK (0x2u << 12) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 32 MD_SLCK periods */
#define   SUPC_WUMR_WKUPDBC_512_SK (0x3u << 12) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 512 MD_SLCK periods */
#define   SUPC_WUMR_WKUPDBC_4096_SK (0x4u << 12) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 4,096 MD_SLCK periods */
#define   SUPC_WUMR_WKUPDBC_32768_SK (0x5u << 12) /**< \brief (SUPC_WUMR) WKUPx shall be in its active state for at least 32,768 MD_SLCK periods */
#define SUPC_WUMR_LPDBC0_Pos 16
#define SUPC_WUMR_LPDBC0_Msk (0x7u << SUPC_WUMR_LPDBC0_Pos) /**< \brief (SUPC_WUMR) Low-power Debouncer Period of WKUPx */
#define SUPC_WUMR_LPDBC0(value) ((SUPC_WUMR_LPDBC0_Msk & ((value) << SUPC_WUMR_LPDBC0_Pos)))
#define   SUPC_WUMR_LPDBC0_DISABLE (0x0u << 16) /**< \brief (SUPC_WUMR) Disables the low-power debouncers. */
#define   SUPC_WUMR_LPDBC0_2_RTCOUT (0x1u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 2 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC0_3_RTCOUT (0x2u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 3 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC0_4_RTCOUT (0x3u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 4 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC0_5_RTCOUT (0x4u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 5 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC0_6_RTCOUT (0x5u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 6 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC0_7_RTCOUT (0x6u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 7 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC0_8_RTCOUT (0x7u << 16) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 8 RTCOUTx clock periods */
#define SUPC_WUMR_LPDBC1_Pos 19
#define SUPC_WUMR_LPDBC1_Msk (0x7u << SUPC_WUMR_LPDBC1_Pos) /**< \brief (SUPC_WUMR) Low-power Debouncer Period of WKUPx */
#define SUPC_WUMR_LPDBC1(value) ((SUPC_WUMR_LPDBC1_Msk & ((value) << SUPC_WUMR_LPDBC1_Pos)))
#define   SUPC_WUMR_LPDBC1_DISABLE (0x0u << 19) /**< \brief (SUPC_WUMR) Disables the low-power debouncers. */
#define   SUPC_WUMR_LPDBC1_2_RTCOUT (0x1u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 2 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC1_3_RTCOUT (0x2u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 3 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC1_4_RTCOUT (0x3u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 4 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC1_5_RTCOUT (0x4u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 5 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC1_6_RTCOUT (0x5u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 6 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC1_7_RTCOUT (0x6u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 7 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC1_8_RTCOUT (0x7u << 19) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 8 RTCOUTx clock periods */
#define SUPC_WUMR_LPDBC2_Pos 22
#define SUPC_WUMR_LPDBC2_Msk (0x7u << SUPC_WUMR_LPDBC2_Pos) /**< \brief (SUPC_WUMR) Low-power Debouncer Period of WKUPx */
#define SUPC_WUMR_LPDBC2(value) ((SUPC_WUMR_LPDBC2_Msk & ((value) << SUPC_WUMR_LPDBC2_Pos)))
#define   SUPC_WUMR_LPDBC2_DISABLE (0x0u << 22) /**< \brief (SUPC_WUMR) Disables the low-power debouncers. */
#define   SUPC_WUMR_LPDBC2_2_RTCOUT (0x1u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 2 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC2_3_RTCOUT (0x2u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 3 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC2_4_RTCOUT (0x3u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 4 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC2_5_RTCOUT (0x4u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 5 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC2_6_RTCOUT (0x5u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 6 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC2_7_RTCOUT (0x6u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 7 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC2_8_RTCOUT (0x7u << 22) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 8 RTCOUTx clock periods */
#define SUPC_WUMR_LPDBC3_Pos 25
#define SUPC_WUMR_LPDBC3_Msk (0x7u << SUPC_WUMR_LPDBC3_Pos) /**< \brief (SUPC_WUMR) Low-power Debouncer Period of WKUPx */
#define SUPC_WUMR_LPDBC3(value) ((SUPC_WUMR_LPDBC3_Msk & ((value) << SUPC_WUMR_LPDBC3_Pos)))
#define   SUPC_WUMR_LPDBC3_DISABLE (0x0u << 25) /**< \brief (SUPC_WUMR) Disables the low-power debouncers. */
#define   SUPC_WUMR_LPDBC3_2_RTCOUT (0x1u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 2 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC3_3_RTCOUT (0x2u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 3 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC3_4_RTCOUT (0x3u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 4 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC3_5_RTCOUT (0x4u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 5 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC3_6_RTCOUT (0x5u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 6 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC3_7_RTCOUT (0x6u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 7 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC3_8_RTCOUT (0x7u << 25) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 8 RTCOUTx clock periods */
#define SUPC_WUMR_LPDBC4_Pos 28
#define SUPC_WUMR_LPDBC4_Msk (0x7u << SUPC_WUMR_LPDBC4_Pos) /**< \brief (SUPC_WUMR) Low-power Debouncer Period of WKUPx */
#define SUPC_WUMR_LPDBC4(value) ((SUPC_WUMR_LPDBC4_Msk & ((value) << SUPC_WUMR_LPDBC4_Pos)))
#define   SUPC_WUMR_LPDBC4_DISABLE (0x0u << 28) /**< \brief (SUPC_WUMR) Disables the low-power debouncers. */
#define   SUPC_WUMR_LPDBC4_2_RTCOUT (0x1u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 2 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC4_3_RTCOUT (0x2u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 3 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC4_4_RTCOUT (0x3u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 4 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC4_5_RTCOUT (0x4u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 5 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC4_6_RTCOUT (0x5u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 6 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC4_7_RTCOUT (0x6u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 7 RTCOUTx clock periods */
#define   SUPC_WUMR_LPDBC4_8_RTCOUT (0x7u << 28) /**< \brief (SUPC_WUMR) WKUPx in active state for at least 8 RTCOUTx clock periods */
/* -------- SUPC_WUIR : (SUPC Offset: 0x10) Wakeup Inputs Register -------- */
#define SUPC_WUIR_WKUPEN0 (0x1u << 0) /**< \brief (SUPC_WUIR) Wake-up Input Enable 0 */
#define SUPC_WUIR_WKUPEN1 (0x1u << 1) /**< \brief (SUPC_WUIR) Wake-up Input Enable 1 */
#define SUPC_WUIR_WKUPEN2 (0x1u << 2) /**< \brief (SUPC_WUIR) Wake-up Input Enable 2 */
#define SUPC_WUIR_WKUPEN3 (0x1u << 3) /**< \brief (SUPC_WUIR) Wake-up Input Enable 3 */
#define SUPC_WUIR_WKUPEN4 (0x1u << 4) /**< \brief (SUPC_WUIR) Wake-up Input Enable 4 */
#define SUPC_WUIR_WKUPEN5 (0x1u << 5) /**< \brief (SUPC_WUIR) Wake-up Input Enable 5 */
#define SUPC_WUIR_WKUPEN6 (0x1u << 6) /**< \brief (SUPC_WUIR) Wake-up Input Enable 6 */
#define SUPC_WUIR_WKUPEN7 (0x1u << 7) /**< \brief (SUPC_WUIR) Wake-up Input Enable 7 */
#define SUPC_WUIR_WKUPEN8 (0x1u << 8) /**< \brief (SUPC_WUIR) Wake-up Input Enable 8 */
#define SUPC_WUIR_WKUPEN9 (0x1u << 9) /**< \brief (SUPC_WUIR) Wake-up Input Enable 9 */
#define SUPC_WUIR_WKUPEN10 (0x1u << 10) /**< \brief (SUPC_WUIR) Wake-up Input Enable 10 */
#define SUPC_WUIR_WKUPEN11 (0x1u << 11) /**< \brief (SUPC_WUIR) Wake-up Input Enable 11 */
#define SUPC_WUIR_WKUPEN12 (0x1u << 12) /**< \brief (SUPC_WUIR) Wake-up Input Enable 12 */
#define SUPC_WUIR_WKUPEN13 (0x1u << 13) /**< \brief (SUPC_WUIR) Wake-up Input Enable 13 */
#define SUPC_WUIR_WKUPEN14 (0x1u << 14) /**< \brief (SUPC_WUIR) Wake-up Input Enable 14 */
#define SUPC_WUIR_WKUPT0 (0x1u << 16) /**< \brief (SUPC_WUIR) Wake-up Input Type 0 */
#define   SUPC_WUIR_WKUPT0_LOW (0x0u << 16) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT0_HIGH (0x1u << 16) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT1 (0x1u << 17) /**< \brief (SUPC_WUIR) Wake-up Input Type 1 */
#define   SUPC_WUIR_WKUPT1_LOW (0x0u << 17) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT1_HIGH (0x1u << 17) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT2 (0x1u << 18) /**< \brief (SUPC_WUIR) Wake-up Input Type 2 */
#define   SUPC_WUIR_WKUPT2_LOW (0x0u << 18) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT2_HIGH (0x1u << 18) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT3 (0x1u << 19) /**< \brief (SUPC_WUIR) Wake-up Input Type 3 */
#define   SUPC_WUIR_WKUPT3_LOW (0x0u << 19) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT3_HIGH (0x1u << 19) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT4 (0x1u << 20) /**< \brief (SUPC_WUIR) Wake-up Input Type 4 */
#define   SUPC_WUIR_WKUPT4_LOW (0x0u << 20) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT4_HIGH (0x1u << 20) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT5 (0x1u << 21) /**< \brief (SUPC_WUIR) Wake-up Input Type 5 */
#define   SUPC_WUIR_WKUPT5_LOW (0x0u << 21) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT5_HIGH (0x1u << 21) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT6 (0x1u << 22) /**< \brief (SUPC_WUIR) Wake-up Input Type 6 */
#define   SUPC_WUIR_WKUPT6_LOW (0x0u << 22) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT6_HIGH (0x1u << 22) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT7 (0x1u << 23) /**< \brief (SUPC_WUIR) Wake-up Input Type 7 */
#define   SUPC_WUIR_WKUPT7_LOW (0x0u << 23) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT7_HIGH (0x1u << 23) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT8 (0x1u << 24) /**< \brief (SUPC_WUIR) Wake-up Input Type 8 */
#define   SUPC_WUIR_WKUPT8_LOW (0x0u << 24) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT8_HIGH (0x1u << 24) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT9 (0x1u << 25) /**< \brief (SUPC_WUIR) Wake-up Input Type 9 */
#define   SUPC_WUIR_WKUPT9_LOW (0x0u << 25) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT9_HIGH (0x1u << 25) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT10 (0x1u << 26) /**< \brief (SUPC_WUIR) Wake-up Input Type 10 */
#define   SUPC_WUIR_WKUPT10_LOW (0x0u << 26) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT10_HIGH (0x1u << 26) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT11 (0x1u << 27) /**< \brief (SUPC_WUIR) Wake-up Input Type 11 */
#define   SUPC_WUIR_WKUPT11_LOW (0x0u << 27) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT11_HIGH (0x1u << 27) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT12 (0x1u << 28) /**< \brief (SUPC_WUIR) Wake-up Input Type 12 */
#define   SUPC_WUIR_WKUPT12_LOW (0x0u << 28) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT12_HIGH (0x1u << 28) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT13 (0x1u << 29) /**< \brief (SUPC_WUIR) Wake-up Input Type 13 */
#define   SUPC_WUIR_WKUPT13_LOW (0x0u << 29) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT13_HIGH (0x1u << 29) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define SUPC_WUIR_WKUPT14 (0x1u << 30) /**< \brief (SUPC_WUIR) Wake-up Input Type 14 */
#define   SUPC_WUIR_WKUPT14_LOW (0x0u << 30) /**< \brief (SUPC_WUIR) A falling edge followed by a low level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
#define   SUPC_WUIR_WKUPT14_HIGH (0x1u << 30) /**< \brief (SUPC_WUIR) A rising edge followed by a high level for a period defined by WKUPDBC on the corresponding wake-up input forces the wake-up of the core power supply. */
/* -------- SUPC_SR : (SUPC Offset: 0x14) Status Register -------- */
#define SUPC_SR_TDOSCSEL (0x1u << 0) /**< \brief (SUPC_SR) Timing Domain 32 kHz Oscillator Selection Status */
#define   SUPC_SR_TDOSCSEL_RC (0x0u << 0) /**< \brief (SUPC_SR) The timing domain slow clock (TD_SLCK) source is the slow RC oscillator output. */
#define   SUPC_SR_TDOSCSEL_XTAL (0x1u << 0) /**< \brief (SUPC_SR) The timing domain slow clock source is the 32.768 kHz crystal oscillator output. */
#define SUPC_SR_IOSMWS (0x1u << 2) /**< \brief (SUPC_SR) VDDIO Supply Monitor Wake-up Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_CORSMRSTS (0x1u << 3) /**< \brief (SUPC_SR) VDDCORE Supply Monitor Reset Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_IOSMRSTS (0x1u << 4) /**< \brief (SUPC_SR) VDDIO Supply Monitor Reset Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_IOSMIS (0x1u << 5) /**< \brief (SUPC_SR) VDDIO Supply Monitor Interrupt Status (cleared by reading SUPC_ISR) */
#define SUPC_SR_IOSMS (0x1u << 6) /**< \brief (SUPC_SR) VDDIO Supply Monitor Output Status */
#define SUPC_SR_LCDS (0x1u << 8) /**< \brief (SUPC_SR) LCD Power Domain Status */
#define SUPC_SR_FWUPS (0x1u << 12) /**< \brief (SUPC_SR) Force Wake-up Pin Status */
#define SUPC_SR_LPDBCS0 (0x1u << 16) /**< \brief (SUPC_SR) Tamper Detection Wake-up Status (cleared by reading SUPC_ISR) */
#define SUPC_SR_LPDBCS1 (0x1u << 17) /**< \brief (SUPC_SR) Tamper Detection Wake-up Status (cleared by reading SUPC_ISR) */
#define SUPC_SR_LPDBCS2 (0x1u << 18) /**< \brief (SUPC_SR) Tamper Detection Wake-up Status (cleared by reading SUPC_ISR) */
#define SUPC_SR_LPDBCS3 (0x1u << 19) /**< \brief (SUPC_SR) Tamper Detection Wake-up Status (cleared by reading SUPC_ISR) */
#define SUPC_SR_LPDBCS4 (0x1u << 20) /**< \brief (SUPC_SR) Tamper Detection Wake-up Status (cleared by reading SUPC_ISR) */
#define SUPC_SR_SXFMS_Pos 24
#define SUPC_SR_SXFMS_Msk (0x3u << SUPC_SR_SXFMS_Pos) /**< \brief (SUPC_SR) Slow Crystal Oscillator Frequency Monitor Status (cleared on read) */
#define   SUPC_SR_SXFMS_GOOD (0x0u << 24) /**< \brief (SUPC_SR) No frequency error detected. */
#define   SUPC_SR_SXFMS_FREQ_ERROR (0x1u << 24) /**< \brief (SUPC_SR) The frequency has not been correct over 4 consecutive monitoring periods but there are still edges detected on the slow crystal oscillator output. */
#define   SUPC_SR_SXFMS_FAIL (0x2u << 24) /**< \brief (SUPC_SR) No edge detected in the slow crystal oscillator output for at least 2 consecutive monitoring periods. */
#define SUPC_SR_SXFME (0x1u << 26) /**< \brief (SUPC_SR) Slow Crystal Oscillator Frequency Monitor Error (cleared on read) */
#define SUPC_SR_WKUPS (0x1u << 27) /**< \brief (SUPC_SR) WKUP Wake-up Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_BADXTS (0x1u << 28) /**< \brief (SUPC_SR) Slow Crystal Oscillator Wake-up Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_FWKUPS (0x1u << 29) /**< \brief (SUPC_SR) FWUP Wake-up Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_RTTS (0x1u << 30) /**< \brief (SUPC_SR) RTT Wake-up Status (cleared by reading SUPC_WUSR) */
#define SUPC_SR_RTCS (0x1u << 31) /**< \brief (SUPC_SR) RTC Wake-up Status (cleared by reading SUPC_WUSR) */
/* -------- SUPC_EMR : (SUPC Offset: 0x1C) Extended Mode Register -------- */
#define SUPC_EMR_FLRSGPBR (0x1u << 16) /**< \brief (SUPC_EMR) Flash Erase GPBR */
#define SUPC_EMR_FULLGPBRC (0x1u << 17) /**< \brief (SUPC_EMR) Full GPBR Clean */
#define SUPC_EMR_COREBGEN (0x1u << 18) /**< \brief (SUPC_EMR) VDDCORE Voltage Regulator Bandgap Enable */
/* -------- SUPC_BMR : (SUPC Offset: 0x20) Backup Mode Register -------- */
#define SUPC_BMR_RTTWKEN (0x1u << 0) /**< \brief (SUPC_BMR) Real-time Timer Wake-up Enable */
#define   SUPC_BMR_RTTWKEN_DISABLE (0x0u << 0) /**< \brief (SUPC_BMR) The RTT alarm signal has no wake-up effect. */
#define   SUPC_BMR_RTTWKEN_ENABLE (0x1u << 0) /**< \brief (SUPC_BMR) The RTT alarm signal forces the wake-up of the core power supply. */
#define SUPC_BMR_RTCWKEN (0x1u << 1) /**< \brief (SUPC_BMR) Real-time Clock Wake-up Enable */
#define   SUPC_BMR_RTCWKEN_DISABLE (0x0u << 1) /**< \brief (SUPC_BMR) The RTC alarm signal has no wake-up effect. */
#define   SUPC_BMR_RTCWKEN_ENABLE (0x1u << 1) /**< \brief (SUPC_BMR) The RTC alarm signal forces the wake-up of the core power supply. */
#define SUPC_BMR_VBATWKEN (0x1u << 2) /**< \brief (SUPC_BMR) VBAT Supply Monitor Wake-up Enable */
#define   SUPC_BMR_VBATWKEN_DISABLE (0x0u << 2) /**< \brief (SUPC_BMR) Wake-up on VBAT supply monitor under voltage detection is disabled. */
#define   SUPC_BMR_VBATWKEN_ENABLE (0x1u << 2) /**< \brief (SUPC_BMR) Wake-up on VBAT supply monitor under voltage detection is enabled. */
#define SUPC_BMR_FWUPEN (0x1u << 4) /**< \brief (SUPC_BMR) Force Wake-up Pin Wake-up Enable */
#define   SUPC_BMR_FWUPEN_DISABLE (0x0u << 4) /**< \brief (SUPC_BMR) The fwup signal has no wake-up effect. */
#define   SUPC_BMR_FWUPEN_ENABLE (0x1u << 4) /**< \brief (SUPC_BMR) The fwup signal forces the wake-up of the core power supply. */
#define SUPC_BMR_CORPORWKEN (0x1u << 5) /**< \brief (SUPC_BMR) VDDCORE POR Wake-up Enable */
#define   SUPC_BMR_CORPORWKEN_DISABLE (0x0u << 5) /**< \brief (SUPC_BMR) Wake-up on VDDCORE Power-On Reset Event is disabled. */
#define   SUPC_BMR_CORPORWKEN_ENABLE (0x1u << 5) /**< \brief (SUPC_BMR) Wake-up on VDDCORE Power-On Reset Event is enabled. */
#define SUPC_BMR_IOSMWKEN (0x1u << 6) /**< \brief (SUPC_BMR) VDDIO Supply Monitor Wake-up Enable */
#define   SUPC_BMR_IOSMWKEN_DISABLE (0x0u << 6) /**< \brief (SUPC_BMR) Wake-up on VDDIO supply monitor under voltage detection is disabled. */
#define   SUPC_BMR_IOSMWKEN_ENABLE (0x1u << 6) /**< \brief (SUPC_BMR) Wake-up on VDDIO supply monitor under voltage detection is enabled. */
#define SUPC_BMR_VBATREN (0x1u << 8) /**< \brief (SUPC_BMR) Battery Voltage Event Report Enable */
#define   SUPC_BMR_VBATREN_DISABLE (0x0u << 8) /**< \brief (SUPC_BMR) Disables the report of event on VBAT voltage. */
#define   SUPC_BMR_VBATREN_ENABLE (0x1u << 8) /**< \brief (SUPC_BMR) Enables the report of event on VBAT voltage. */
#define SUPC_BMR_MRTCOUT (0x1u << 9) /**< \brief (SUPC_BMR) RTCOUT0 Outputs Drive Mode */
#define   SUPC_BMR_MRTCOUT_USERDEF (0x0u << 9) /**< \brief (SUPC_BMR) RTCOUT0 output is driven according to the configuration of the field RTC_MR.OUT0. */
#define SUPC_BMR_BADXTWKEN (0x1u << 10) /**< \brief (SUPC_BMR) Slow Crystal Oscillator Frequency Error Wake-up Enable */
#define SUPC_BMR_KEY_Pos 24
#define SUPC_BMR_KEY_Msk (0xffu << SUPC_BMR_KEY_Pos) /**< \brief (SUPC_BMR) Password Key */
#define SUPC_BMR_KEY(value) ((SUPC_BMR_KEY_Msk & ((value) << SUPC_BMR_KEY_Pos)))
#define   SUPC_BMR_KEY_PASSWD (0xA5u << 24) /**< \brief (SUPC_BMR) Writing any other value in this field aborts the write operation. */
/* -------- SUPC_WUSR : (SUPC Offset: 0x24) Wakeup Status Register -------- */
#define SUPC_WUSR_WKUPS (0x1u << 0) /**< \brief (SUPC_WUSR) WKUP Wake-up Status (cleared on read) */
#define SUPC_WUSR_BADXTWKS (0x1u << 1) /**< \brief (SUPC_WUSR) Slow Crystal Oscillator Frequency Error Wake-up Status (cleared on read) */
#define SUPC_WUSR_FWUPS (0x1u << 2) /**< \brief (SUPC_WUSR) FWUP Wake-up Status (cleared on read) */
#define SUPC_WUSR_IOSMWS (0x1u << 3) /**< \brief (SUPC_WUSR) VDDIO Supply Monitor Wake-up Status (cleared on read) */
#define SUPC_WUSR_RTTS (0x1u << 4) /**< \brief (SUPC_WUSR) RTT Wake-up Status (cleared on read) */
#define SUPC_WUSR_RTCS (0x1u << 5) /**< \brief (SUPC_WUSR) RTC Wake-up Status (cleared on read) */
#define SUPC_WUSR_IOSMRSTS (0x1u << 8) /**< \brief (SUPC_WUSR) VDDIO Supply Monitor Reset Status (cleared on read) */
#define SUPC_WUSR_WKUPIS0 (0x1u << 16) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS1 (0x1u << 17) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS2 (0x1u << 18) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS3 (0x1u << 19) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS4 (0x1u << 20) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS5 (0x1u << 21) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS6 (0x1u << 22) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS7 (0x1u << 23) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS8 (0x1u << 24) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS9 (0x1u << 25) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS10 (0x1u << 26) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS11 (0x1u << 27) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS12 (0x1u << 28) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS13 (0x1u << 29) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS14 (0x1u << 30) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
#define SUPC_WUSR_WKUPIS15 (0x1u << 31) /**< \brief (SUPC_WUSR) WKUPx Input Wake-up Status (cleared on read) */
/* -------- SUPC_IER : (SUPC Offset: 0x28) Enable Interrupt Register -------- */
#define SUPC_IER_LPDBC0 (0x1u << 0) /**< \brief (SUPC_IER) WKUPx Pin Tamper Detection Interrupt Enable */
#define SUPC_IER_LPDBC1 (0x1u << 1) /**< \brief (SUPC_IER) WKUPx Pin Tamper Detection Interrupt Enable */
#define SUPC_IER_LPDBC2 (0x1u << 2) /**< \brief (SUPC_IER) WKUPx Pin Tamper Detection Interrupt Enable */
#define SUPC_IER_LPDBC3 (0x1u << 3) /**< \brief (SUPC_IER) WKUPx Pin Tamper Detection Interrupt Enable */
#define SUPC_IER_LPDBC4 (0x1u << 4) /**< \brief (SUPC_IER) WKUPx Pin Tamper Detection Interrupt Enable */
#define SUPC_IER_IOSMEV (0x1u << 16) /**< \brief (SUPC_IER) VDDIO Supply Monitor Event Interrupt Enable */
#define SUPC_IER_VBATSMEV (0x1u << 17) /**< \brief (SUPC_IER) VBAT Supply Monitor Event Interrupt Enable */
/* -------- SUPC_IDR : (SUPC Offset: 0x2C) Disable Interrupt Register -------- */
#define SUPC_IDR_LPDBC0 (0x1u << 0) /**< \brief (SUPC_IDR) WKUPx Pin Tamper Detection Interrupt Disable */
#define SUPC_IDR_LPDBC1 (0x1u << 1) /**< \brief (SUPC_IDR) WKUPx Pin Tamper Detection Interrupt Disable */
#define SUPC_IDR_LPDBC2 (0x1u << 2) /**< \brief (SUPC_IDR) WKUPx Pin Tamper Detection Interrupt Disable */
#define SUPC_IDR_LPDBC3 (0x1u << 3) /**< \brief (SUPC_IDR) WKUPx Pin Tamper Detection Interrupt Disable */
#define SUPC_IDR_LPDBC4 (0x1u << 4) /**< \brief (SUPC_IDR) WKUPx Pin Tamper Detection Interrupt Disable */
#define SUPC_IDR_IOSMEV (0x1u << 16) /**< \brief (SUPC_IDR) VDDIO Supply Monitor Event Interrupt Disable */
#define SUPC_IDR_VBATSMEV (0x1u << 17) /**< \brief (SUPC_IDR) VBAT Supply Monitor Event Interrupt Disable */
/* -------- SUPC_IMR : (SUPC Offset: 0x30) Mask Interrupt Register -------- */
#define SUPC_IMR_LPDBC0 (0x1u << 0) /**< \brief (SUPC_IMR) WKUPx Pin Tamper Detection Interrupt Mask */
#define SUPC_IMR_LPDBC1 (0x1u << 1) /**< \brief (SUPC_IMR) WKUPx Pin Tamper Detection Interrupt Mask */
#define SUPC_IMR_LPDBC2 (0x1u << 2) /**< \brief (SUPC_IMR) WKUPx Pin Tamper Detection Interrupt Mask */
#define SUPC_IMR_LPDBC3 (0x1u << 3) /**< \brief (SUPC_IMR) WKUPx Pin Tamper Detection Interrupt Mask */
#define SUPC_IMR_LPDBC4 (0x1u << 4) /**< \brief (SUPC_IMR) WKUPx Pin Tamper Detection Interrupt Mask */
#define SUPC_IMR_IOSMEV (0x1u << 16) /**< \brief (SUPC_IMR) VDDIO Supply Monitor Event Interrupt Mask */
#define SUPC_IMR_VBATSMEV (0x1u << 17) /**< \brief (SUPC_IMR) VBAT Supply Monitor Event Interrupt Mask */
/* -------- SUPC_ISR : (SUPC Offset: 0x34) Status Interrupt Register -------- */
#define SUPC_ISR_LPDBC0 (0x1u << 0) /**< \brief (SUPC_ISR) WKUPx Pin Tamper Detection Interrupt Status (cleared on read) */
#define SUPC_ISR_LPDBC1 (0x1u << 1) /**< \brief (SUPC_ISR) WKUPx Pin Tamper Detection Interrupt Status (cleared on read) */
#define SUPC_ISR_LPDBC2 (0x1u << 2) /**< \brief (SUPC_ISR) WKUPx Pin Tamper Detection Interrupt Status (cleared on read) */
#define SUPC_ISR_LPDBC3 (0x1u << 3) /**< \brief (SUPC_ISR) WKUPx Pin Tamper Detection Interrupt Status (cleared on read) */
#define SUPC_ISR_LPDBC4 (0x1u << 4) /**< \brief (SUPC_ISR) WKUPx Pin Tamper Detection Interrupt Status (cleared on read) */
#define SUPC_ISR_IOSMEV (0x1u << 16) /**< \brief (SUPC_ISR) VDDIO Supply Monitor Event Interrupt Status (cleared on read) */
#define SUPC_ISR_VBATSMEV (0x1u << 17) /**< \brief (SUPC_ISR) VBAT Supply Monitor Event Interrupt Status (cleared on read) */

/*@}*/


#endif /* _PIC32CX_SUPC_COMPONENT_ */
