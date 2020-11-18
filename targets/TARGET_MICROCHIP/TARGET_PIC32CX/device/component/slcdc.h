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

#ifndef _PIC32CX_SLCDC_COMPONENT_
#define _PIC32CX_SLCDC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Segment LCD Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_SLCDC Segment LCD Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief SlcdcCom hardware registers */
typedef struct {
  __IO uint32_t SLCDC_LMEMR; /**< \brief (SlcdcCom Offset: 0x0) SLCDC LSB Memory Register */
  __I  uint32_t Reserved;
} SlcdcCom;
/** \brief Slcdc hardware registers */
#define SLCDCCOM_NUMBER 8
typedef struct {
  __O  uint32_t SLCDC_CR;      /**< \brief (Slcdc Offset: 0x0) Control Register */
  __IO uint32_t SLCDC_MR;      /**< \brief (Slcdc Offset: 0x4) Mode Register */
  __IO uint32_t SLCDC_FRR;     /**< \brief (Slcdc Offset: 0x8) Frame Rate Register */
  __IO uint32_t SLCDC_DR;      /**< \brief (Slcdc Offset: 0xC) Display Register */
  __I  uint32_t SLCDC_SR;      /**< \brief (Slcdc Offset: 0x10) Status Register */
  __I  uint32_t Reserved1[3];
  __O  uint32_t SLCDC_IER;     /**< \brief (Slcdc Offset: 0x20) Interrupt Enable Register */
  __O  uint32_t SLCDC_IDR;     /**< \brief (Slcdc Offset: 0x24) Interrupt Disable Register */
  __I  uint32_t SLCDC_IMR;     /**< \brief (Slcdc Offset: 0x28) Interrupt Mask Register */
  __I  uint32_t SLCDC_ISR;     /**< \brief (Slcdc Offset: 0x2C) Interrupt Status Register */
  __IO uint32_t SLCDC_SMR0;    /**< \brief (Slcdc Offset: 0x30) Segment Map Register 0 */
  __I  uint32_t Reserved2[44];
  __IO uint32_t SLCDC_WPMR;    /**< \brief (Slcdc Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t Reserved3[70];
  SlcdcCom SLCDC_COM[SLCDCCOM_NUMBER]; /**< \brief (Slcdc Offset: 0x200) com = 0 .. 7 */
} Slcdc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SLCDC_CR : (SLCDC Offset: 0x0) Control Register -------- */
#define SLCDC_CR_LCDEN (0x1u << 0) /**< \brief (SLCDC_CR) Enable the LCDC */
#define SLCDC_CR_LCDDIS (0x1u << 1) /**< \brief (SLCDC_CR) Disable LCDC */
#define SLCDC_CR_SWRST (0x1u << 3) /**< \brief (SLCDC_CR) Software Reset */
#define SLCDC_CR_FRZDF (0x1u << 6) /**< \brief (SLCDC_CR) Freeze Display Panel Features Configuration */
#define SLCDC_CR_FRZMAP (0x1u << 7) /**< \brief (SLCDC_CR) Freeze Remap Configuration */
#define SLCDC_CR_FRZKEY_Pos 8
#define SLCDC_CR_FRZKEY_Msk (0xffu << SLCDC_CR_FRZKEY_Pos) /**< \brief (SLCDC_CR) Freeze Key (write-only) */
#define SLCDC_CR_FRZKEY(value) ((SLCDC_CR_FRZKEY_Msk & ((value) << SLCDC_CR_FRZKEY_Pos)))
#define   SLCDC_CR_FRZKEY_PASSWD (0x4Eu << 8) /**< \brief (SLCDC_CR) Writing any other value in this field aborts the write operation of the FRZDF bit or FRZMAP bit. Always reads as 0. */
/* -------- SLCDC_MR : (SLCDC Offset: 0x4) Mode Register -------- */
#define SLCDC_MR_COMSEL_Pos 0
#define SLCDC_MR_COMSEL_Msk (0x7u << SLCDC_MR_COMSEL_Pos) /**< \brief (SLCDC_MR) Selection of the Number of Commons */
#define SLCDC_MR_COMSEL(value) ((SLCDC_MR_COMSEL_Msk & ((value) << SLCDC_MR_COMSEL_Pos)))
#define   SLCDC_MR_COMSEL_COM_0 (0x0u << 0) /**< \brief (SLCDC_MR) COM0 is driven by SLCDC, COM1:7 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO1 (0x1u << 0) /**< \brief (SLCDC_MR) COM0:1 are driven by SLCDC, COM2:7 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO2 (0x2u << 0) /**< \brief (SLCDC_MR) COM0:2 are driven by SLCDC, COM3:7 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO3 (0x3u << 0) /**< \brief (SLCDC_MR) COM0:3 are driven by SLCDC, COM4:7 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO4 (0x4u << 0) /**< \brief (SLCDC_MR) COM0:4 are driven by SLCDC, COM5:7 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO5 (0x5u << 0) /**< \brief (SLCDC_MR) COM0:5 are driven by SLCDC, COM6:7 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO6 (0x6u << 0) /**< \brief (SLCDC_MR) COM0:6 are driven by SLCDC, COM7 driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO7 (0x7u << 0) /**< \brief (SLCDC_MR) COM0:7 are driven by SLCDC, no COM pin driven by digital function */
#define SLCDC_MR_SEGSEL_Pos 8
#define SLCDC_MR_SEGSEL_Msk (0x3fu << SLCDC_MR_SEGSEL_Pos) /**< \brief (SLCDC_MR) Selection of the Number of Segments */
#define SLCDC_MR_SEGSEL(value) ((SLCDC_MR_SEGSEL_Msk & ((value) << SLCDC_MR_SEGSEL_Pos)))
#define SLCDC_MR_BUFTIME_Pos 16
#define SLCDC_MR_BUFTIME_Msk (0xfu << SLCDC_MR_BUFTIME_Pos) /**< \brief (SLCDC_MR) Buffer On-Time */
#define SLCDC_MR_BUFTIME(value) ((SLCDC_MR_BUFTIME_Msk & ((value) << SLCDC_MR_BUFTIME_Pos)))
#define   SLCDC_MR_BUFTIME_OFF (0x0u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 0% of SLCK period */
#define   SLCDC_MR_BUFTIME_X2_SLCK_PERIOD (0x1u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 2 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_X4_SLCK_PERIOD (0x2u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 4 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_X8_SLCK_PERIOD (0x3u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 8 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_X16_SLCK_PERIOD (0x4u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 16 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_X32_SLCK_PERIOD (0x5u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 32 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_X64_SLCK_PERIOD (0x6u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 64 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_X128_SLCK_PERIOD (0x7u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 128 periods of SLCK clock */
#define   SLCDC_MR_BUFTIME_PERCENT_50 (0x8u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 50% of SLCK period */
#define   SLCDC_MR_BUFTIME_PERCENT_100 (0x9u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 100% of SLCK period */
#define SLCDC_MR_BIAS_Pos 20
#define SLCDC_MR_BIAS_Msk (0x3u << SLCDC_MR_BIAS_Pos) /**< \brief (SLCDC_MR) LCD Display Configuration */
#define SLCDC_MR_BIAS(value) ((SLCDC_MR_BIAS_Msk & ((value) << SLCDC_MR_BIAS_Pos)))
#define   SLCDC_MR_BIAS_STATIC (0x0u << 20) /**< \brief (SLCDC_MR) Static */
#define   SLCDC_MR_BIAS_BIAS_1_2 (0x1u << 20) /**< \brief (SLCDC_MR) Bias 1/2 */
#define   SLCDC_MR_BIAS_BIAS_1_3 (0x2u << 20) /**< \brief (SLCDC_MR) Bias 1/3 */
#define   SLCDC_MR_BIAS_BIAS_1_4 (0x3u << 20) /**< \brief (SLCDC_MR) Bias 1/4 */
#define SLCDC_MR_LPMODE (0x1u << 24) /**< \brief (SLCDC_MR) Low-Power Mode */
/* -------- SLCDC_FRR : (SLCDC Offset: 0x8) Frame Rate Register -------- */
#define SLCDC_FRR_PRESC_Pos 0
#define SLCDC_FRR_PRESC_Msk (0x7u << SLCDC_FRR_PRESC_Pos) /**< \brief (SLCDC_FRR) Clock Prescaler */
#define SLCDC_FRR_PRESC(value) ((SLCDC_FRR_PRESC_Msk & ((value) << SLCDC_FRR_PRESC_Pos)))
#define   SLCDC_FRR_PRESC_SLCK_DIV8 (0x0u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 8 */
#define   SLCDC_FRR_PRESC_SLCK_DIV16 (0x1u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 16 */
#define   SLCDC_FRR_PRESC_SLCK_DIV32 (0x2u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 32 */
#define   SLCDC_FRR_PRESC_SLCK_DIV64 (0x3u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 64 */
#define   SLCDC_FRR_PRESC_SLCK_DIV128 (0x4u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 128 */
#define   SLCDC_FRR_PRESC_SLCK_DIV256 (0x5u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 256 */
#define   SLCDC_FRR_PRESC_SLCK_DIV512 (0x6u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 512 */
#define   SLCDC_FRR_PRESC_SLCK_DIV1024 (0x7u << 0) /**< \brief (SLCDC_FRR) Slow clock is divided by 1024 */
#define SLCDC_FRR_DIV_Pos 8
#define SLCDC_FRR_DIV_Msk (0x7u << SLCDC_FRR_DIV_Pos) /**< \brief (SLCDC_FRR) Clock Division */
#define SLCDC_FRR_DIV(value) ((SLCDC_FRR_DIV_Msk & ((value) << SLCDC_FRR_DIV_Pos)))
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV1 (0x0u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 1 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV2 (0x1u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 2 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV3 (0x2u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 3 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV4 (0x3u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 4 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV5 (0x4u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 5 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV6 (0x5u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 6 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV7 (0x6u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 7 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV8 (0x7u << 8) /**< \brief (SLCDC_FRR) Clock output from prescaler is divided by 8 */
/* -------- SLCDC_DR : (SLCDC Offset: 0xC) Display Register -------- */
#define SLCDC_DR_DISPMODE_Pos 0
#define SLCDC_DR_DISPMODE_Msk (0x7u << SLCDC_DR_DISPMODE_Pos) /**< \brief (SLCDC_DR) Display Mode Register */
#define SLCDC_DR_DISPMODE(value) ((SLCDC_DR_DISPMODE_Msk & ((value) << SLCDC_DR_DISPMODE_Pos)))
#define   SLCDC_DR_DISPMODE_NORMAL (0x0u << 0) /**< \brief (SLCDC_DR) Normal Mode-Latched data are displayed. */
#define   SLCDC_DR_DISPMODE_FORCE_OFF (0x1u << 0) /**< \brief (SLCDC_DR) Force Off Mode-All pixels are invisible. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_FORCE_ON (0x2u << 0) /**< \brief (SLCDC_DR) Force On Mode-All pixels are visible. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_BLINKING (0x3u << 0) /**< \brief (SLCDC_DR) Blinking Mode-All pixels are alternately turned off to the predefined state in SLCDC memory at LCDBLKFREQ frequency. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_INVERTED (0x4u << 0) /**< \brief (SLCDC_DR) Inverted Mode-All pixels are set in the inverted state as defined in SLCDC memory. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_INVERTED_BLINK (0x5u << 0) /**< \brief (SLCDC_DR) Inverted Blinking Mode-All pixels are alternately turned off to the predefined opposite state in SLCDC memory at LCDBLKFREQ frequency. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_USER_BUFFER_LOAD (0x6u << 0) /**< \brief (SLCDC_DR) User Buffer Only Load Mode-Blocks the automatic transfer from User Buffer to Display Buffer. */
#define   SLCDC_DR_DISPMODE_BUFFERS_SWAP (0x7u << 0) /**< \brief (SLCDC_DR) Buffer Swap Mode-All pixels are alternatively assigned to the state defined in the User Buffer, then to the state defined in the Display Buffer at LCDBLKFREQ frequency. */
#define SLCDC_DR_LCDBLKFREQ_Pos 8
#define SLCDC_DR_LCDBLKFREQ_Msk (0xffu << SLCDC_DR_LCDBLKFREQ_Pos) /**< \brief (SLCDC_DR) LCD Blinking Frequency Selection */
#define SLCDC_DR_LCDBLKFREQ(value) ((SLCDC_DR_LCDBLKFREQ_Msk & ((value) << SLCDC_DR_LCDBLKFREQ_Pos)))
/* -------- SLCDC_SR : (SLCDC Offset: 0x10) Status Register -------- */
#define SLCDC_SR_ENA (0x1u << 0) /**< \brief (SLCDC_SR) Enable Status (Automatically Set/Reset) */
#define SLCDC_SR_DFFRZS (0x1u << 6) /**< \brief (SLCDC_SR) Display Panel Features Configuration Freeze Status */
#define SLCDC_SR_MAPFRZS (0x1u << 7) /**< \brief (SLCDC_SR) Remapping Configuration Freeze Status */
/* -------- SLCDC_IER : (SLCDC Offset: 0x20) Interrupt Enable Register -------- */
#define SLCDC_IER_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_IER) End of Frame Interrupt Enable */
#define SLCDC_IER_DIS (0x1u << 2) /**< \brief (SLCDC_IER) Disable Completion Interrupt Enable */
/* -------- SLCDC_IDR : (SLCDC Offset: 0x24) Interrupt Disable Register -------- */
#define SLCDC_IDR_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_IDR) End of Frame Interrupt Disable */
#define SLCDC_IDR_DIS (0x1u << 2) /**< \brief (SLCDC_IDR) Disable Completion Interrupt Disable */
/* -------- SLCDC_IMR : (SLCDC Offset: 0x28) Interrupt Mask Register -------- */
#define SLCDC_IMR_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_IMR) End of Frame Interrupt Mask */
#define SLCDC_IMR_DIS (0x1u << 2) /**< \brief (SLCDC_IMR) Disable Completion Interrupt Mask */
/* -------- SLCDC_ISR : (SLCDC Offset: 0x2C) Interrupt Status Register -------- */
#define SLCDC_ISR_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_ISR) End of Frame Interrupt Status */
#define SLCDC_ISR_DIS (0x1u << 2) /**< \brief (SLCDC_ISR) Disable Completion Interrupt Status */
/* -------- SLCDC_SMR0 : (SLCDC Offset: 0x30) Segment Map Register 0 -------- */
#define SLCDC_SMR0_LCD0 (0x1u << 0) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD1 (0x1u << 1) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD2 (0x1u << 2) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD3 (0x1u << 3) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD4 (0x1u << 4) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD5 (0x1u << 5) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD6 (0x1u << 6) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD7 (0x1u << 7) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD8 (0x1u << 8) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD9 (0x1u << 9) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD10 (0x1u << 10) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD11 (0x1u << 11) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD12 (0x1u << 12) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD13 (0x1u << 13) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD14 (0x1u << 14) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD15 (0x1u << 15) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD16 (0x1u << 16) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD17 (0x1u << 17) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD18 (0x1u << 18) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD19 (0x1u << 19) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD20 (0x1u << 20) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD21 (0x1u << 21) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD22 (0x1u << 22) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD23 (0x1u << 23) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD24 (0x1u << 24) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD25 (0x1u << 25) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD26 (0x1u << 26) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD27 (0x1u << 27) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD28 (0x1u << 28) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD29 (0x1u << 29) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD30 (0x1u << 30) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
#define SLCDC_SMR0_LCD31 (0x1u << 31) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O Pin */
/* -------- SLCDC_WPMR : (SLCDC Offset: 0xE4) Write Protection Mode Register -------- */
#define SLCDC_WPMR_WPEN (0x1u << 0) /**< \brief (SLCDC_WPMR) Write Protection Enable */
#define SLCDC_WPMR_WPITEN (0x1u << 1) /**< \brief (SLCDC_WPMR) Write Protection Interrupt Enable */
#define SLCDC_WPMR_WPCREN (0x1u << 2) /**< \brief (SLCDC_WPMR) Write Protection Control Enable */
#define SLCDC_WPMR_WPKEY_Pos 8
#define SLCDC_WPMR_WPKEY_Msk (0xffffffu << SLCDC_WPMR_WPKEY_Pos) /**< \brief (SLCDC_WPMR) Write Protection Key */
#define SLCDC_WPMR_WPKEY(value) ((SLCDC_WPMR_WPKEY_Msk & ((value) << SLCDC_WPMR_WPKEY_Pos)))
#define   SLCDC_WPMR_WPKEY_PASSWD (0x534C43u << 8) /**< \brief (SLCDC_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- SLCDC_LMEMR : (SLCDC Offset: 0x200) SLCDC LSB Memory Register -------- */
#define SLCDC_LMEMR_LPIXEL_Pos 0
#define SLCDC_LMEMR_LPIXEL_Msk (0xffffffffu << SLCDC_LMEMR_LPIXEL_Pos) /**< \brief (SLCDC_LMEMR) LSB Pixels pattern associated to COMx terminal */
#define SLCDC_LMEMR_LPIXEL(value) ((SLCDC_LMEMR_LPIXEL_Msk & ((value) << SLCDC_LMEMR_LPIXEL_Pos)))

/*@}*/


#endif /* _PIC32CX_SLCDC_COMPONENT_ */
