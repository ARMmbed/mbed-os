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

#ifndef _PIC32CX_EMAFELPFIF_COMPONENT_
#define _PIC32CX_EMAFELPFIF_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Energy Metering Analog Front End (EMAFE) */
/* ============================================================================= */
/** \addtogroup PIC32CX_EMAFELPFIF Energy Metering Analog Front End (EMAFE) */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Emafelpfif hardware registers */
typedef struct {
  __O  uint32_t EMAFE_CR;       /**< \brief (Emafelpfif Offset: 0x0) Control Register */
  __IO uint32_t EMAFE_MR;       /**< \brief (Emafelpfif Offset: 0x4) Mode Register */
  __IO uint32_t EMAFE_CDCR;     /**< \brief (Emafelpfif Offset: 0x8) Channels Difference Comparison Register */
  __IO uint32_t EMAFE_EMR;      /**< \brief (Emafelpfif Offset: 0xC) Extended Mode Register */
  __I  uint32_t EMAFE_DMA_DATA; /**< \brief (Emafelpfif Offset: 0x10) DMA Data Register */
  __I  uint32_t EMAFE_DATA[5];  /**< \brief (Emafelpfif Offset: 0x14) Data Channel 0 Register */
  __I  uint32_t Reserved1[6];
  __IO uint32_t EMAFE_CHA_CFG1; /**< \brief (Emafelpfif Offset: 0x40) Channel A Comparison Configuration 1 */
  __IO uint32_t EMAFE_CHB_CFG1; /**< \brief (Emafelpfif Offset: 0x44) Channel B Comparison Configuration 1 */
  __IO uint32_t EMAFE_CHA_CFG2; /**< \brief (Emafelpfif Offset: 0x48) Channel A Comparison Configuration 2 */
  __IO uint32_t EMAFE_CHB_CFG2; /**< \brief (Emafelpfif Offset: 0x4C) Channel B Comparison Configuration 2 */
  __O  uint32_t EMAFE_IER;      /**< \brief (Emafelpfif Offset: 0x50) Interrupt Enable Register */
  __O  uint32_t EMAFE_IDR;      /**< \brief (Emafelpfif Offset: 0x54) Interrupt Disable Register */
  __I  uint32_t EMAFE_IMR;      /**< \brief (Emafelpfif Offset: 0x58) Interrupt Mask Register */
  __I  uint32_t EMAFE_ISR;      /**< \brief (Emafelpfif Offset: 0x5C) Interrupt Status Register */
  __I  uint32_t EMAFE_SR;       /**< \brief (Emafelpfif Offset: 0x60) Status Register */
  __I  uint32_t Reserved2[32];
  __IO uint32_t EMAFE_WPMR;     /**< \brief (Emafelpfif Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t EMAFE_WPSR;     /**< \brief (Emafelpfif Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved3[4];
  __I  uint32_t EMAFE_VERSION;  /**< \brief (Emafelpfif Offset: 0xFC) Version Register */
  __IO uint32_t EMAFE_RPR;      /**< \brief (Emafelpfif Offset: 0x100) Receive Pointer Register */
  __IO uint32_t EMAFE_RCR;      /**< \brief (Emafelpfif Offset: 0x104) Receive Counter Register */
  __I  uint32_t Reserved4[2];
  __IO uint32_t EMAFE_RNPR;     /**< \brief (Emafelpfif Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t EMAFE_RNCR;     /**< \brief (Emafelpfif Offset: 0x114) Receive Next Counter Register */
  __I  uint32_t Reserved5[2];
  __O  uint32_t EMAFE_PTCR;     /**< \brief (Emafelpfif Offset: 0x120) Transfer Control Register */
  __I  uint32_t EMAFE_PTSR;     /**< \brief (Emafelpfif Offset: 0x124) Transfer Status Register */
  __IO uint32_t EMAFE_PWPMR;    /**< \brief (Emafelpfif Offset: 0x128) Write Protection Mode Register */
} Emafelpfif;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- EMAFE_CR : (EMAFELPFIF Offset: 0x0) Control Register -------- */
#define EMAFE_CR_SWRST (0x1u << 0) /**< \brief (EMAFE_CR) Software Reset */
#define EMAFE_CR_SWRST_DECIM (0x1u << 1) /**< \brief (EMAFE_CR) Sinc Decimation Filters Reset */
/* -------- EMAFE_MR : (EMAFELPFIF Offset: 0x4) Mode Register -------- */
#define EMAFE_MR_LPF_ON0 (0x1u << 0) /**< \brief (EMAFE_MR) Low-Pass Filter 0 On */
#define EMAFE_MR_LPF_ON1 (0x1u << 1) /**< \brief (EMAFE_MR) Low-Pass Filter 1 On */
#define EMAFE_MR_LPF_ON2 (0x1u << 2) /**< \brief (EMAFE_MR) Low-Pass Filter 2 On */
#define EMAFE_MR_LPF_ON3 (0x1u << 3) /**< \brief (EMAFE_MR) Low-Pass Filter 3 On */
#define EMAFE_MR_LPF_ON4 (0x1u << 4) /**< \brief (EMAFE_MR) Low-Pass Filter 4 On */
#define EMAFE_MR_DMA_ON (0x1u << 7) /**< \brief (EMAFE_MR) Direct Memory Access On */
#define EMAFE_MR_DMACH0 (0x1u << 8) /**< \brief (EMAFE_MR) Channel 0 Enable for DMA transfer */
#define EMAFE_MR_DMACH1 (0x1u << 9) /**< \brief (EMAFE_MR) Channel 1 Enable for DMA transfer */
#define EMAFE_MR_DMACH2 (0x1u << 10) /**< \brief (EMAFE_MR) Channel 2 Enable for DMA transfer */
#define EMAFE_MR_DMACH3 (0x1u << 11) /**< \brief (EMAFE_MR) Channel 3 Enable for DMA transfer */
#define EMAFE_MR_DMACH4 (0x1u << 12) /**< \brief (EMAFE_MR) Channel 4 Enable for DMA transfer */
#define EMAFE_MR_VAR_ON (0x1u << 15) /**< \brief (EMAFE_MR) Variation Detection On */
#define EMAFE_MR_VAR_CH_Pos 16
#define EMAFE_MR_VAR_CH_Msk (0x7u << EMAFE_MR_VAR_CH_Pos) /**< \brief (EMAFE_MR) Variation Detection Channel Selection */
#define EMAFE_MR_VAR_CH(value) ((EMAFE_MR_VAR_CH_Msk & ((value) << EMAFE_MR_VAR_CH_Pos)))
#define   EMAFE_MR_VAR_CH_CH0 (0x0u << 16) /**< \brief (EMAFE_MR) Channel 0 is selected. */
#define   EMAFE_MR_VAR_CH_CH1 (0x1u << 16) /**< \brief (EMAFE_MR) Channel 1 is selected. */
#define   EMAFE_MR_VAR_CH_CH2 (0x2u << 16) /**< \brief (EMAFE_MR) Channel 2 is selected. */
#define   EMAFE_MR_VAR_CH_CH3 (0x3u << 16) /**< \brief (EMAFE_MR) Channel 3 is selected. */
#define   EMAFE_MR_VAR_CH_CH4 (0x4u << 16) /**< \brief (EMAFE_MR) Channel 4 is selected. */
#define   EMAFE_MR_VAR_CH_CH5 (0x5u << 16) /**< \brief (EMAFE_MR) Channel 5 is selected. */
#define   EMAFE_MR_VAR_CH_CH6 (0x6u << 16) /**< \brief (EMAFE_MR) Channel 6 is selected. */
#define EMAFE_MR_VAR_LIMIT_Pos 20
#define EMAFE_MR_VAR_LIMIT_Msk (0x3ffu << EMAFE_MR_VAR_LIMIT_Pos) /**< \brief (EMAFE_MR) Variation Detection Limit for Event Triggering */
#define EMAFE_MR_VAR_LIMIT(value) ((EMAFE_MR_VAR_LIMIT_Msk & ((value) << EMAFE_MR_VAR_LIMIT_Pos)))
/* -------- EMAFE_CDCR : (EMAFELPFIF Offset: 0x8) Channels Difference Comparison Register -------- */
#define EMAFE_CDCR_CHA_IDX_Pos 0
#define EMAFE_CDCR_CHA_IDX_Msk (0x7u << EMAFE_CDCR_CHA_IDX_Pos) /**< \brief (EMAFE_CDCR) First Channel to Compare */
#define EMAFE_CDCR_CHA_IDX(value) ((EMAFE_CDCR_CHA_IDX_Msk & ((value) << EMAFE_CDCR_CHA_IDX_Pos)))
#define   EMAFE_CDCR_CHA_IDX_CH0 (0x0u << 0) /**< \brief (EMAFE_CDCR) Channel 0 is used to perform comparison. */
#define   EMAFE_CDCR_CHA_IDX_CH1 (0x1u << 0) /**< \brief (EMAFE_CDCR) Channel 1 is used to perform comparison. */
#define   EMAFE_CDCR_CHA_IDX_CH2 (0x2u << 0) /**< \brief (EMAFE_CDCR) Channel 2 is used to perform comparison. */
#define   EMAFE_CDCR_CHA_IDX_CH3 (0x3u << 0) /**< \brief (EMAFE_CDCR) Channel 3 is used to perform comparison. */
#define   EMAFE_CDCR_CHA_IDX_CH4 (0x4u << 0) /**< \brief (EMAFE_CDCR) Channel 4 is used to perform comparison. */
#define   EMAFE_CDCR_CHA_IDX_CH5 (0x5u << 0) /**< \brief (EMAFE_CDCR) Channel 5 is used to perform comparison. */
#define   EMAFE_CDCR_CHA_IDX_CH6 (0x6u << 0) /**< \brief (EMAFE_CDCR) Channel 6 is used to perform comparison. */
#define EMAFE_CDCR_CHB_IDX_Pos 4
#define EMAFE_CDCR_CHB_IDX_Msk (0x7u << EMAFE_CDCR_CHB_IDX_Pos) /**< \brief (EMAFE_CDCR) Second Channel to Compare */
#define EMAFE_CDCR_CHB_IDX(value) ((EMAFE_CDCR_CHB_IDX_Msk & ((value) << EMAFE_CDCR_CHB_IDX_Pos)))
#define   EMAFE_CDCR_CHB_IDX_CH0 (0x0u << 4) /**< \brief (EMAFE_CDCR) Channel 0 is used to perform comparison */
#define   EMAFE_CDCR_CHB_IDX_CH1 (0x1u << 4) /**< \brief (EMAFE_CDCR) Channel 1 is used to perform comparison */
#define   EMAFE_CDCR_CHB_IDX_CH2 (0x2u << 4) /**< \brief (EMAFE_CDCR) Channel 2 is used to perform comparison */
#define   EMAFE_CDCR_CHB_IDX_CH3 (0x3u << 4) /**< \brief (EMAFE_CDCR) Channel 3 is used to perform comparison */
#define   EMAFE_CDCR_CHB_IDX_CH4 (0x4u << 4) /**< \brief (EMAFE_CDCR) Channel 4 is used to perform comparison */
#define   EMAFE_CDCR_CHB_IDX_CH5 (0x5u << 4) /**< \brief (EMAFE_CDCR) Channel 5 is used to perform comparison. */
#define   EMAFE_CDCR_CHB_IDX_CH6 (0x6u << 4) /**< \brief (EMAFE_CDCR) Channel 6 is used to perform comparison. */
#define EMAFE_CDCR_DIFFTYPE (0x1u << 8) /**< \brief (EMAFE_CDCR) Type of Comparison */
#define EMAFE_CDCR_DIFFCOUNT_Pos 12
#define EMAFE_CDCR_DIFFCOUNT_Msk (0xfu << EMAFE_CDCR_DIFFCOUNT_Pos) /**< \brief (EMAFE_CDCR) Successive Comparison Event Count */
#define EMAFE_CDCR_DIFFCOUNT(value) ((EMAFE_CDCR_DIFFCOUNT_Msk & ((value) << EMAFE_CDCR_DIFFCOUNT_Pos)))
#define EMAFE_CDCR_DIFFTHRES_Pos 16
#define EMAFE_CDCR_DIFFTHRES_Msk (0xfffu << EMAFE_CDCR_DIFFTHRES_Pos) /**< \brief (EMAFE_CDCR) Threshold */
#define EMAFE_CDCR_DIFFTHRES(value) ((EMAFE_CDCR_DIFFTHRES_Msk & ((value) << EMAFE_CDCR_DIFFTHRES_Pos)))
/* -------- EMAFE_EMR : (EMAFELPFIF Offset: 0xC) Extended Mode Register -------- */
#define EMAFE_EMR_FORMAT_Pos 0
#define EMAFE_EMR_FORMAT_Msk (0x3u << EMAFE_EMR_FORMAT_Pos) /**< \brief (EMAFE_EMR) Data Format */
#define EMAFE_EMR_FORMAT(value) ((EMAFE_EMR_FORMAT_Msk & ((value) << EMAFE_EMR_FORMAT_Pos)))
#define   EMAFE_EMR_FORMAT_NB_CHANNEL (0x0u << 0) /**< \brief (EMAFE_EMR) Index of channel is added in register EMAFE_DMA_DATA. 24-bit dynamic range. (default). */
#define   EMAFE_EMR_FORMAT_SIGNEXTEND (0x1u << 0) /**< \brief (EMAFE_EMR) 24-bit value with sign extension up to 32-bit applied to registers EMAFE_DMA_DATA and EMAFE _DATAx. 24-bit dynamic range */
#define   EMAFE_EMR_FORMAT_FULLRANGE (0x2u << 0) /**< \brief (EMAFE_EMR) 32-bit full range signed extension is applied in EMAFE_DMA_DATA and EMAFE _DATAx. 32-bit dynamic range. */
#define EMAFE_EMR_MCLKDIV_LPF_Pos 4
#define EMAFE_EMR_MCLKDIV_LPF_Msk (0xfu << EMAFE_EMR_MCLKDIV_LPF_Pos) /**< \brief (EMAFE_EMR) LPF Clock Prescaler Ratio */
#define EMAFE_EMR_MCLKDIV_LPF(value) ((EMAFE_EMR_MCLKDIV_LPF_Msk & ((value) << EMAFE_EMR_MCLKDIV_LPF_Pos)))
#define   EMAFE_EMR_MCLKDIV_LPF_MCLK (0x0u << 4) /**< \brief (EMAFE_EMR) LPFIF clock is MCLK/1. */
#define   EMAFE_EMR_MCLKDIV_LPF_MCLKDIV2 (0x1u << 4) /**< \brief (EMAFE_EMR) LPFIF clock is MCLK/2. */
#define   EMAFE_EMR_MCLKDIV_LPF_MCLKDIV4 (0x2u << 4) /**< \brief (EMAFE_EMR) LPFIF clock is MCLK/4. */
#define   EMAFE_EMR_MCLKDIV_LPF_MCLKDIV8 (0x3u << 4) /**< \brief (EMAFE_EMR) LPFIF clock is MCLK/8. (Default) */
#define   EMAFE_EMR_MCLKDIV_LPF_MCLKDIV16 (0x4u << 4) /**< \brief (EMAFE_EMR) LPFIF clock is MCLK/16. */
#define   EMAFE_EMR_MCLKDIV_LPF_MCLKDIV32 (0x5u << 4) /**< \brief (EMAFE_EMR) LPFIF clock is MCLK/32. */
#define EMAFE_EMR_OSR_Pos 8
#define EMAFE_EMR_OSR_Msk (0x7u << EMAFE_EMR_OSR_Pos) /**< \brief (EMAFE_EMR) OSR of the Decimation Filters */
#define EMAFE_EMR_OSR(value) ((EMAFE_EMR_OSR_Msk & ((value) << EMAFE_EMR_OSR_Pos)))
#define   EMAFE_EMR_OSR_OSR8 (0x0u << 8) /**< \brief (EMAFE_EMR) OSR of the system is 8. */
#define   EMAFE_EMR_OSR_OSR16 (0x1u << 8) /**< \brief (EMAFE_EMR) OSR of the system is 16. */
#define   EMAFE_EMR_OSR_OSR32 (0x2u << 8) /**< \brief (EMAFE_EMR) OSR of the system is 32. */
#define   EMAFE_EMR_OSR_OSR64 (0x3u << 8) /**< \brief (EMAFE_EMR) OSR of the system is 64 (Default). */
/* -------- EMAFE_DMA_DATA : (EMAFELPFIF Offset: 0x10) DMA Data Register -------- */
#define EMAFE_DMA_DATA_DATA24_Pos 0
#define EMAFE_DMA_DATA_DATA24_Msk (0xffffffu << EMAFE_DMA_DATA_DATA24_Pos) /**< \brief (EMAFE_DMA_DATA) 24-bit Data read by DMA */
#define EMAFE_DMA_DATA_CHIDX_Pos 24
#define EMAFE_DMA_DATA_CHIDX_Msk (0xffu << EMAFE_DMA_DATA_CHIDX_Pos) /**< \brief (EMAFE_DMA_DATA) Channel Index */
#define EMAFE_DMA_DATA_DATA24SE_Pos 0
#define EMAFE_DMA_DATA_DATA24SE_Msk (0xffffffffu << EMAFE_DMA_DATA_DATA24SE_Pos) /**< \brief (EMAFE_DMA_DATA) Low-pass Filter 24-bit Data with 32-bit Sign Extension read by DMA */
#define EMAFE_DMA_DATA_DATA32_Pos 0
#define EMAFE_DMA_DATA_DATA32_Msk (0xffffffffu << EMAFE_DMA_DATA_DATA32_Pos) /**< \brief (EMAFE_DMA_DATA) 32-bit Data read by DMA */
/* -------- EMAFE_DATA[5] : (EMAFELPFIF Offset: 0x14) Data Channel 0 Register -------- */
#define EMAFE_DATA_DATA24_Pos 0
#define EMAFE_DATA_DATA24_Msk (0xffffffu << EMAFE_DATA_DATA24_Pos) /**< \brief (EMAFE_DATA[5]) Low-pass Filter 24-bit Signed Data */
#define EMAFE_DATA_DATA24SE_Pos 0
#define EMAFE_DATA_DATA24SE_Msk (0xffffffffu << EMAFE_DATA_DATA24SE_Pos) /**< \brief (EMAFE_DATA[5]) Low-pass Filter 24-bit Data with 32-bit Sign Extension */
#define EMAFE_DATA_DATA32_Pos 0
#define EMAFE_DATA_DATA32_Msk (0xffffffffu << EMAFE_DATA_DATA32_Pos) /**< \brief (EMAFE_DATA[5]) Low-pass Filter 32-bit Signed Data */
/* -------- EMAFE_CHA_CFG1 : (EMAFELPFIF Offset: 0x40) Channel A Comparison Configuration 1 -------- */
#define EMAFE_CHA_CFG1_THRESH1_Pos 0
#define EMAFE_CHA_CFG1_THRESH1_Msk (0xffffffu << EMAFE_CHA_CFG1_THRESH1_Pos) /**< \brief (EMAFE_CHA_CFG1) Threshold 1 for Channel A */
#define EMAFE_CHA_CFG1_THRESH1(value) ((EMAFE_CHA_CFG1_THRESH1_Msk & ((value) << EMAFE_CHA_CFG1_THRESH1_Pos)))
#define EMAFE_CHA_CFG1_IDX_Pos 24
#define EMAFE_CHA_CFG1_IDX_Msk (0x7u << EMAFE_CHA_CFG1_IDX_Pos) /**< \brief (EMAFE_CHA_CFG1) Channel A Index */
#define EMAFE_CHA_CFG1_IDX(value) ((EMAFE_CHA_CFG1_IDX_Msk & ((value) << EMAFE_CHA_CFG1_IDX_Pos)))
#define   EMAFE_CHA_CFG1_IDX_CH0 (0x0u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 0 is selected. */
#define   EMAFE_CHA_CFG1_IDX_CH1 (0x1u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 1 is selected. */
#define   EMAFE_CHA_CFG1_IDX_CH2 (0x2u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 2 is selected. */
#define   EMAFE_CHA_CFG1_IDX_CH3 (0x3u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 3 is selected. */
#define   EMAFE_CHA_CFG1_IDX_CH4 (0x4u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 4 is selected. */
#define   EMAFE_CHA_CFG1_IDX_CH5 (0x5u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 5 is selected. */
#define   EMAFE_CHA_CFG1_IDX_CH6 (0x6u << 24) /**< \brief (EMAFE_CHA_CFG1) Channel 6 is selected. */
#define EMAFE_CHA_CFG1_COND (0x1u << 29) /**< \brief (EMAFE_CHA_CFG1) Condition Applied for Detection on Channel A */
#define   EMAFE_CHA_CFG1_COND_OR (0x0u << 29) /**< \brief (EMAFE_CHA_CFG1) Selected channel data is lower than THRESH1 OR higher than THRESH2. */
#define   EMAFE_CHA_CFG1_COND_AND (0x1u << 29) /**< \brief (EMAFE_CHA_CFG1) Selected channel data is lower than THRESH1 AND higher than THRESH2. */
/* -------- EMAFE_CHB_CFG1 : (EMAFELPFIF Offset: 0x44) Channel B Comparison Configuration 1 -------- */
#define EMAFE_CHB_CFG1_THRESH1_Pos 0
#define EMAFE_CHB_CFG1_THRESH1_Msk (0xffffffu << EMAFE_CHB_CFG1_THRESH1_Pos) /**< \brief (EMAFE_CHB_CFG1) Threshold 1 for Channel B */
#define EMAFE_CHB_CFG1_THRESH1(value) ((EMAFE_CHB_CFG1_THRESH1_Msk & ((value) << EMAFE_CHB_CFG1_THRESH1_Pos)))
#define EMAFE_CHB_CFG1_IDX_Pos 24
#define EMAFE_CHB_CFG1_IDX_Msk (0x7u << EMAFE_CHB_CFG1_IDX_Pos) /**< \brief (EMAFE_CHB_CFG1) Channel B Index */
#define EMAFE_CHB_CFG1_IDX(value) ((EMAFE_CHB_CFG1_IDX_Msk & ((value) << EMAFE_CHB_CFG1_IDX_Pos)))
#define   EMAFE_CHB_CFG1_IDX_CH0 (0x0u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 0 is selected. */
#define   EMAFE_CHB_CFG1_IDX_CH1 (0x1u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 1 is selected. */
#define   EMAFE_CHB_CFG1_IDX_CH2 (0x2u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 2 is selected. */
#define   EMAFE_CHB_CFG1_IDX_CH3 (0x3u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 3 is selected. */
#define   EMAFE_CHB_CFG1_IDX_CH4 (0x4u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 4 is selected. */
#define   EMAFE_CHB_CFG1_IDX_CH5 (0x5u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 5 is selected. */
#define   EMAFE_CHB_CFG1_IDX_CH6 (0x6u << 24) /**< \brief (EMAFE_CHB_CFG1) Channel 6 is selected. */
#define EMAFE_CHB_CFG1_COND (0x1u << 29) /**< \brief (EMAFE_CHB_CFG1) Condition Applied for Detection on Channel B */
#define   EMAFE_CHB_CFG1_COND_OR (0x0u << 29) /**< \brief (EMAFE_CHB_CFG1) Selected channel data is lower than THRESH1 OR higher than THRESH2. */
#define   EMAFE_CHB_CFG1_COND_AND (0x1u << 29) /**< \brief (EMAFE_CHB_CFG1) Selected channel data is lower than THRESH1 AND higher than THRESH2. */
/* -------- EMAFE_CHA_CFG2 : (EMAFELPFIF Offset: 0x48) Channel A Comparison Configuration 2 -------- */
#define EMAFE_CHA_CFG2_THRESH2_Pos 0
#define EMAFE_CHA_CFG2_THRESH2_Msk (0xffffffu << EMAFE_CHA_CFG2_THRESH2_Pos) /**< \brief (EMAFE_CHA_CFG2) Threshold 2 for Channel A */
#define EMAFE_CHA_CFG2_THRESH2(value) ((EMAFE_CHA_CFG2_THRESH2_Msk & ((value) << EMAFE_CHA_CFG2_THRESH2_Pos)))
/* -------- EMAFE_CHB_CFG2 : (EMAFELPFIF Offset: 0x4C) Channel B Comparison Configuration 2 -------- */
#define EMAFE_CHB_CFG2_THRESH2_Pos 0
#define EMAFE_CHB_CFG2_THRESH2_Msk (0xffffffu << EMAFE_CHB_CFG2_THRESH2_Pos) /**< \brief (EMAFE_CHB_CFG2) Threshold 2 for Channel B */
#define EMAFE_CHB_CFG2_THRESH2(value) ((EMAFE_CHB_CFG2_THRESH2_Msk & ((value) << EMAFE_CHB_CFG2_THRESH2_Pos)))
/* -------- EMAFE_IER : (EMAFELPFIF Offset: 0x50) Interrupt Enable Register -------- */
#define EMAFE_IER_DRDY (0x1u << 0) /**< \brief (EMAFE_IER) Data Ready Interrupt Enable */
#define EMAFE_IER_RXUDR (0x1u << 1) /**< \brief (EMAFE_IER) Receive Underflow Flow Interrupt Enable */
#define EMAFE_IER_RXFULL (0x1u << 2) /**< \brief (EMAFE_IER) Receive FIFO Full Interrupt Enable */
#define EMAFE_IER_RXEMPTY (0x1u << 3) /**< \brief (EMAFE_IER) Receive FIFO Empty Interrupt Enable */
#define EMAFE_IER_CHCHG (0x1u << 4) /**< \brief (EMAFE_IER) Channel Data Change Interrupt Enable */
#define EMAFE_IER_CHDIFF (0x1u << 5) /**< \brief (EMAFE_IER) Comparison Flag Interrupt Enable */
#define EMAFE_IER_PARERROR (0x1u << 6) /**< \brief (EMAFE_IER) Parity Error Interrupt Enable */
#define EMAFE_IER_ENDRX (0x1u << 7) /**< \brief (EMAFE_IER) PDC Receive Transfer End Interrupt Enable */
#define EMAFE_IER_RXBUFF (0x1u << 8) /**< \brief (EMAFE_IER) PDC Receive Buffer Full Interrupt Enable */
#define EMAFE_IER_CHAEVT (0x1u << 9) /**< \brief (EMAFE_IER) Channel A Event Interrupt Enable */
#define EMAFE_IER_CHBEVT (0x1u << 10) /**< \brief (EMAFE_IER) Channel B Event Interrupt Enable */
/* -------- EMAFE_IDR : (EMAFELPFIF Offset: 0x54) Interrupt Disable Register -------- */
#define EMAFE_IDR_DRDY (0x1u << 0) /**< \brief (EMAFE_IDR) Data Ready Interrupt Disable */
#define EMAFE_IDR_RXUDR (0x1u << 1) /**< \brief (EMAFE_IDR) Receive Underflow Flow Interrupt Disable */
#define EMAFE_IDR_RXFULL (0x1u << 2) /**< \brief (EMAFE_IDR) Receive FIFO Full Interrupt Disable */
#define EMAFE_IDR_RXEMPTY (0x1u << 3) /**< \brief (EMAFE_IDR) Receive FIFO Empty Interrupt Disable */
#define EMAFE_IDR_CHCHG (0x1u << 4) /**< \brief (EMAFE_IDR) Channel Data Change Interrupt Disable */
#define EMAFE_IDR_CHDIFF (0x1u << 5) /**< \brief (EMAFE_IDR) Comparison Flag Interrupt Disable */
#define EMAFE_IDR_PARERROR (0x1u << 6) /**< \brief (EMAFE_IDR) Parity Error Interrupt Disable */
#define EMAFE_IDR_ENDRX (0x1u << 7) /**< \brief (EMAFE_IDR) PDC Receive Transfer End Interrupt Disable */
#define EMAFE_IDR_RXBUFF (0x1u << 8) /**< \brief (EMAFE_IDR) PDC Receive Buffer Full Interrupt Disable */
#define EMAFE_IDR_CHAEVT (0x1u << 9) /**< \brief (EMAFE_IDR) Threshold Channel A Interrupt Disable */
#define EMAFE_IDR_CHBEVT (0x1u << 10) /**< \brief (EMAFE_IDR) Threshold Channel B Interrupt Disable */
/* -------- EMAFE_IMR : (EMAFELPFIF Offset: 0x58) Interrupt Mask Register -------- */
#define EMAFE_IMR_DRDY (0x1u << 0) /**< \brief (EMAFE_IMR) Data Ready Interrupt Mask */
#define EMAFE_IMR_RXUDR (0x1u << 1) /**< \brief (EMAFE_IMR) Receive Underflow Flow Interrupt Mask */
#define EMAFE_IMR_RXFULL (0x1u << 2) /**< \brief (EMAFE_IMR) Receive FIFO Full Interrupt Mask */
#define EMAFE_IMR_RXEMPTY (0x1u << 3) /**< \brief (EMAFE_IMR) Receive FIFO Empty Interrupt Mask */
#define EMAFE_IMR_CHCHG (0x1u << 4) /**< \brief (EMAFE_IMR) Channel Data Change Interrupt Mask */
#define EMAFE_IMR_CHDIFF (0x1u << 5) /**< \brief (EMAFE_IMR) Comparison Flag Interrupt Mask */
#define EMAFE_IMR_PARERROR (0x1u << 6) /**< \brief (EMAFE_IMR) Parity Error Interrupt Mask */
#define EMAFE_IMR_ENDRX (0x1u << 7) /**< \brief (EMAFE_IMR) PDC Receive Transfer End Interrupt Mask */
#define EMAFE_IMR_RXBUFF (0x1u << 8) /**< \brief (EMAFE_IMR) PDC Receive Buffer Full Interrupt Mask */
#define EMAFE_IMR_CHAEVT (0x1u << 9) /**< \brief (EMAFE_IMR) Channel A Interrupt Mask */
#define EMAFE_IMR_CHBEVT (0x1u << 10) /**< \brief (EMAFE_IMR) Channel B Interrupt Mask */
/* -------- EMAFE_ISR : (EMAFELPFIF Offset: 0x5C) Interrupt Status Register -------- */
#define EMAFE_ISR_DRDY (0x1u << 0) /**< \brief (EMAFE_ISR) Data Ready (cleared on read) */
#define EMAFE_ISR_RXUDR (0x1u << 1) /**< \brief (EMAFE_ISR) Receive Under Flow (cleared on read) */
#define EMAFE_ISR_RXFULL (0x1u << 2) /**< \brief (EMAFE_ISR) Receive FIFO Full (cleared on read) */
#define EMAFE_ISR_RXEMPTY (0x1u << 3) /**< \brief (EMAFE_ISR) Receive FIFO Empty (cleared on read) */
#define EMAFE_ISR_CHCHG (0x1u << 4) /**< \brief (EMAFE_ISR) Channel Data Variation Detection Event (cleared on read) */
#define EMAFE_ISR_CHDIFF (0x1u << 5) /**< \brief (EMAFE_ISR) Channel Difference Comparison Event (cleared on read) */
#define EMAFE_ISR_PARERROR (0x1u << 6) /**< \brief (EMAFE_ISR) Parity Error Event (cleared on read) */
#define EMAFE_ISR_ENDRX (0x1u << 7) /**< \brief (EMAFE_ISR) PDC Receive Transfer End Event (cleared by writing EMAFE_RCR or EMAFE_RNCR) */
#define EMAFE_ISR_RXBUFF (0x1u << 8) /**< \brief (EMAFE_ISR) PDC Receive Buffer Full Event (cleared by writing EMAFE_RCR or EMAFE_RNCR) */
#define EMAFE_ISR_CHAEVT (0x1u << 9) /**< \brief (EMAFE_ISR) Channel A Event (cleared on read) */
#define EMAFE_ISR_CHBEVT (0x1u << 10) /**< \brief (EMAFE_ISR) Channel B Event (cleared on read) */
/* -------- EMAFE_SR : (EMAFELPFIF Offset: 0x60) Status Register -------- */
#define EMAFE_SR_CHAEVT (0x1u << 0) /**< \brief (EMAFE_SR) Event on Channel A */
#define EMAFE_SR_CHBEVT (0x1u << 1) /**< \brief (EMAFE_SR) Event on Channel B */
/* -------- EMAFE_WPMR : (EMAFELPFIF Offset: 0xE4) Write Protection Mode Register -------- */
#define EMAFE_WPMR_WPCFEN (0x1u << 0) /**< \brief (EMAFE_WPMR) Write Protection Configuration Enable */
#define EMAFE_WPMR_WPITEN (0x1u << 1) /**< \brief (EMAFE_WPMR) Write Protection Interrupt Enable */
#define EMAFE_WPMR_WPCTRL (0x1u << 2) /**< \brief (EMAFE_WPMR) Write Protection Control */
#define EMAFE_WPMR_WPKEY_Pos 8
#define EMAFE_WPMR_WPKEY_Msk (0xffffffu << EMAFE_WPMR_WPKEY_Pos) /**< \brief (EMAFE_WPMR) Write Protection Key */
#define EMAFE_WPMR_WPKEY(value) ((EMAFE_WPMR_WPKEY_Msk & ((value) << EMAFE_WPMR_WPKEY_Pos)))
#define   EMAFE_WPMR_WPKEY_PASSWD (0x768070u << 8) /**< \brief (EMAFE_WPMR) Writing any other value in this field aborts the write operation of the WPCFEN and WPITEN bit. Always reads as 0. */
/* -------- EMAFE_WPSR : (EMAFELPFIF Offset: 0xE8) Write Protection Status Register -------- */
#define EMAFE_WPSR_WPVS (0x1u << 0) /**< \brief (EMAFE_WPSR) Write Protection Violation Status */
#define EMAFE_WPSR_WPVSRC_Pos 8
#define EMAFE_WPSR_WPVSRC_Msk (0xffffu << EMAFE_WPSR_WPVSRC_Pos) /**< \brief (EMAFE_WPSR) Write Protection Violation Source */
/* -------- EMAFE_VERSION : (EMAFELPFIF Offset: 0xFC) Version Register -------- */
#define EMAFE_VERSION_VERSION_Pos 0
#define EMAFE_VERSION_VERSION_Msk (0xffffu << EMAFE_VERSION_VERSION_Pos) /**< \brief (EMAFE_VERSION) Version of the Hardware Module */
#define EMAFE_VERSION_MFN_Pos 16
#define EMAFE_VERSION_MFN_Msk (0xfu << EMAFE_VERSION_MFN_Pos) /**< \brief (EMAFE_VERSION) Metal Fix Number */
/* -------- EMAFE_RPR : (EMAFELPFIF Offset: 0x100) Receive Pointer Register -------- */
#define EMAFE_RPR_RXPTR_Pos 0
#define EMAFE_RPR_RXPTR_Msk (0xffffffffu << EMAFE_RPR_RXPTR_Pos) /**< \brief (EMAFE_RPR) Receive Pointer Register */
#define EMAFE_RPR_RXPTR(value) ((EMAFE_RPR_RXPTR_Msk & ((value) << EMAFE_RPR_RXPTR_Pos)))
/* -------- EMAFE_RCR : (EMAFELPFIF Offset: 0x104) Receive Counter Register -------- */
#define EMAFE_RCR_RXCTR_Pos 0
#define EMAFE_RCR_RXCTR_Msk (0xffffu << EMAFE_RCR_RXCTR_Pos) /**< \brief (EMAFE_RCR) Receive Counter Register */
#define EMAFE_RCR_RXCTR(value) ((EMAFE_RCR_RXCTR_Msk & ((value) << EMAFE_RCR_RXCTR_Pos)))
/* -------- EMAFE_RNPR : (EMAFELPFIF Offset: 0x110) Receive Next Pointer Register -------- */
#define EMAFE_RNPR_RXNPTR_Pos 0
#define EMAFE_RNPR_RXNPTR_Msk (0xffffffffu << EMAFE_RNPR_RXNPTR_Pos) /**< \brief (EMAFE_RNPR) Receive Next Pointer */
#define EMAFE_RNPR_RXNPTR(value) ((EMAFE_RNPR_RXNPTR_Msk & ((value) << EMAFE_RNPR_RXNPTR_Pos)))
/* -------- EMAFE_RNCR : (EMAFELPFIF Offset: 0x114) Receive Next Counter Register -------- */
#define EMAFE_RNCR_RXNCTR_Pos 0
#define EMAFE_RNCR_RXNCTR_Msk (0xffffu << EMAFE_RNCR_RXNCTR_Pos) /**< \brief (EMAFE_RNCR) Receive Next Counter */
#define EMAFE_RNCR_RXNCTR(value) ((EMAFE_RNCR_RXNCTR_Msk & ((value) << EMAFE_RNCR_RXNCTR_Pos)))
/* -------- EMAFE_PTCR : (EMAFELPFIF Offset: 0x120) Transfer Control Register -------- */
#define EMAFE_PTCR_RXTEN (0x1u << 0) /**< \brief (EMAFE_PTCR) Receiver Transfer Enable */
#define EMAFE_PTCR_RXTDIS (0x1u << 1) /**< \brief (EMAFE_PTCR) Receiver Transfer Disable */
#define EMAFE_PTCR_TXTEN (0x1u << 8) /**< \brief (EMAFE_PTCR) Transmitter Transfer Enable */
#define EMAFE_PTCR_TXTDIS (0x1u << 9) /**< \brief (EMAFE_PTCR) Transmitter Transfer Disable */
#define EMAFE_PTCR_RXCBEN (0x1u << 16) /**< \brief (EMAFE_PTCR) Receiver Circular Buffer Enable */
#define EMAFE_PTCR_RXCBDIS (0x1u << 17) /**< \brief (EMAFE_PTCR) Receiver Circular Buffer Disable */
#define EMAFE_PTCR_TXCBEN (0x1u << 18) /**< \brief (EMAFE_PTCR) Transmitter Circular Buffer Enable */
#define EMAFE_PTCR_TXCBDIS (0x1u << 19) /**< \brief (EMAFE_PTCR) Transmitter Circular Buffer Disable */
#define EMAFE_PTCR_ERRCLR (0x1u << 24) /**< \brief (EMAFE_PTCR) Transfer Bus Error Clear */
/* -------- EMAFE_PTSR : (EMAFELPFIF Offset: 0x124) Transfer Status Register -------- */
#define EMAFE_PTSR_RXTEN (0x1u << 0) /**< \brief (EMAFE_PTSR) Receiver Transfer Enable */
#define EMAFE_PTSR_TXTEN (0x1u << 8) /**< \brief (EMAFE_PTSR) Transmitter Transfer Enable */
#define EMAFE_PTSR_RXCBEN (0x1u << 16) /**< \brief (EMAFE_PTSR) Receiver Circular Buffer Enable */
#define EMAFE_PTSR_TXCBEN (0x1u << 18) /**< \brief (EMAFE_PTSR) Transmitter Circular Buffer Enable */
#define EMAFE_PTSR_ERR (0x1u << 24) /**< \brief (EMAFE_PTSR) Transfer Bus Error */
/* -------- EMAFE_PWPMR : (EMAFELPFIF Offset: 0x128) Write Protection Mode Register -------- */
#define EMAFE_PWPMR_WPPTREN (0x1u << 0) /**< \brief (EMAFE_PWPMR) Write Protection Pointer Registers Enable */
#define EMAFE_PWPMR_WPCTREN (0x1u << 1) /**< \brief (EMAFE_PWPMR) Write Protection Counter Registers Enable */
#define EMAFE_PWPMR_WPCREN (0x1u << 2) /**< \brief (EMAFE_PWPMR) Write Protection Control Register Enable */
#define EMAFE_PWPMR_WPKEY_Pos 8
#define EMAFE_PWPMR_WPKEY_Msk (0xffffffu << EMAFE_PWPMR_WPKEY_Pos) /**< \brief (EMAFE_PWPMR) Write Protection Key */
#define EMAFE_PWPMR_WPKEY(value) ((EMAFE_PWPMR_WPKEY_Msk & ((value) << EMAFE_PWPMR_WPKEY_Pos)))
#define   EMAFE_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (EMAFE_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_EMAFELPFIF_COMPONENT_ */
