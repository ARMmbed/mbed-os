/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2021, Microchip Technology Inc.                                */
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

#ifndef _PIC32CX_ADC_COMPONENT_
#define _PIC32CX_ADC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Analog-to-Digital Converter */
/* ============================================================================= */
/** \addtogroup PIC32CX_ADC Analog-to-Digital Converter */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Adc hardware registers */
typedef struct {
  __O  uint32_t ADC_CR;        /**< \brief (Adc Offset: 0x0) Control Register */
  __IO uint32_t ADC_MR;        /**< \brief (Adc Offset: 0x4) Mode Register */
  __IO uint32_t ADC_SEQR1;     /**< \brief (Adc Offset: 0x8) Channel Sequence Register 1 */
  __IO uint32_t ADC_SEQR2;     /**< \brief (Adc Offset: 0xC) Channel Sequence Register 2 */
  __O  uint32_t ADC_CHER;      /**< \brief (Adc Offset: 0x10) Channel Enable Register */
  __O  uint32_t ADC_CHDR;      /**< \brief (Adc Offset: 0x14) Channel Disable Register */
  __I  uint32_t ADC_CHSR;      /**< \brief (Adc Offset: 0x18) Channel Status Register */
  __I  uint32_t Reserved1[1];
  __I  uint32_t ADC_LCDR;      /**< \brief (Adc Offset: 0x20) Last Converted Data Register */
  __O  uint32_t ADC_IER;       /**< \brief (Adc Offset: 0x24) Interrupt Enable Register */
  __O  uint32_t ADC_IDR;       /**< \brief (Adc Offset: 0x28) Interrupt Disable Register */
  __I  uint32_t ADC_IMR;       /**< \brief (Adc Offset: 0x2C) Interrupt Mask Register */
  __I  uint32_t ADC_ISR;       /**< \brief (Adc Offset: 0x30) Interrupt Status Register */
  __O  uint32_t ADC_EOC_IER;   /**< \brief (Adc Offset: 0x34) End Of Conversion Interrupt Enable Register */
  __O  uint32_t ADC_EOC_IDR;   /**< \brief (Adc Offset: 0x38) End Of Conversion Interrupt Disable Register */
  __I  uint32_t ADC_EOC_IMR;   /**< \brief (Adc Offset: 0x3C) End Of Conversion Interrupt Mask Register */
  __I  uint32_t ADC_EOC_ISR;   /**< \brief (Adc Offset: 0x40) End Of Conversion Interrupt Status Register */
  __IO uint32_t ADC_TEMPMR;    /**< \brief (Adc Offset: 0x44) Temperature Sensor Mode Register */
  __IO uint32_t ADC_TEMPCWR;   /**< \brief (Adc Offset: 0x48) Temperature Compare Window Register */
  __I  uint32_t ADC_OVER;      /**< \brief (Adc Offset: 0x4C) Overrun Status Register */
  __IO uint32_t ADC_EMR;       /**< \brief (Adc Offset: 0x50) Extended Mode Register */
  __IO uint32_t ADC_CWR;       /**< \brief (Adc Offset: 0x54) Compare Window Register */
  __I  uint32_t Reserved2[1];
  __IO uint32_t ADC_CCR;       /**< \brief (Adc Offset: 0x5C) Channel Configuration Register */
  __I  uint32_t ADC_CDR[8];    /**< \brief (Adc Offset: 0x60) Channel Data Register */
  __I  uint32_t Reserved3[24];
  __IO uint32_t ADC_ACR;       /**< \brief (Adc Offset: 0xE0) Analog Control Register */
  __IO uint32_t ADC_FMR;       /**< \brief (Adc Offset: 0xE4) FIFO Mode Register */
  __I  uint32_t Reserved4[6];
  __IO uint32_t ADC_RPR;       /**< \brief (Adc Offset: 0x100) Receive Pointer Register */
  __IO uint32_t ADC_RCR;       /**< \brief (Adc Offset: 0x104) Receive Counter Register */
  __I  uint32_t Reserved5[2];
  __IO uint32_t ADC_RNPR;      /**< \brief (Adc Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t ADC_RNCR;      /**< \brief (Adc Offset: 0x114) Receive Next Counter Register */
  __I  uint32_t Reserved6[2];
  __O  uint32_t ADC_PTCR;      /**< \brief (Adc Offset: 0x120) Transfer Control Register */
  __I  uint32_t ADC_PTSR;      /**< \brief (Adc Offset: 0x124) Transfer Status Register */
  __IO uint32_t ADC_PWPMR;     /**< \brief (Adc Offset: 0x128) Write Protection Mode Register */
  __I  uint32_t Reserved7[1];
  __IO uint32_t ADC_TRGR;      /**< \brief (Adc Offset: 0x130) Trigger Register */
  __IO uint32_t ADC_COSR;      /**< \brief (Adc Offset: 0x134) Correction Select Register */
  __IO uint32_t ADC_CVR;       /**< \brief (Adc Offset: 0x138) Correction Values Register */
  __IO uint32_t ADC_CECR;      /**< \brief (Adc Offset: 0x13C) Channel Error Correction Register */
  __I  uint32_t Reserved8[1];
  __I  uint32_t ADC_SR;        /**< \brief (Adc Offset: 0x144) Status Register */
  __I  uint32_t Reserved9[1];
  __IO uint32_t ADC_WPMR;      /**< \brief (Adc Offset: 0x14C) Write Protection Mode Register */
  __I  uint32_t ADC_WPSR;      /**< \brief (Adc Offset: 0x150) Write Protection Status Register */
} Adc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- ADC_CR : (ADC Offset: 0x0) Control Register -------- */
#define ADC_CR_SWRST (0x1u << 0) /**< \brief (ADC_CR) Software Reset */
#define ADC_CR_START (0x1u << 1) /**< \brief (ADC_CR) Start Conversion */
#define ADC_CR_SWFIFO (0x1u << 3) /**< \brief (ADC_CR) Software FIFO Reset */
#define ADC_CR_CMPRST (0x1u << 4) /**< \brief (ADC_CR) Comparison Restart */
/* -------- ADC_MR : (ADC Offset: 0x4) Mode Register -------- */
#define ADC_MR_TRGSEL_Pos 1
#define ADC_MR_TRGSEL_Msk (0x7u << ADC_MR_TRGSEL_Pos) /**< \brief (ADC_MR) Trigger Selection */
#define ADC_MR_TRGSEL(value) ((ADC_MR_TRGSEL_Msk & ((value) << ADC_MR_TRGSEL_Pos)))
#define   ADC_MR_TRGSEL_ADC_TRIG0 (0x0u << 1) /**< \brief (ADC_MR) PWM event line 0 */
#define   ADC_MR_TRGSEL_ADC_TRIG1 (0x1u << 1) /**< \brief (ADC_MR) TIOA0 TC0 */
#define   ADC_MR_TRGSEL_ADC_TRIG2 (0x2u << 1) /**< \brief (ADC_MR) TIOA1 TC0 */
#define   ADC_MR_TRGSEL_ADC_TRIG3 (0x3u << 1) /**< \brief (ADC_MR) TIOA2 TC0 */
#define   ADC_MR_TRGSEL_ADC_TRIG4 (0x4u << 1) /**< \brief (ADC_MR) TIOA0 TC1 */
#define   ADC_MR_TRGSEL_ADC_TRIG5 (0x5u << 1) /**< \brief (ADC_MR) TIOA1 TC1 */
#define   ADC_MR_TRGSEL_ADC_TRIG6 (0x6u << 1) /**< \brief (ADC_MR) RTCOUT0 */
#define ADC_MR_SLEEP (0x1u << 5) /**< \brief (ADC_MR) Sleep Mode */
#define   ADC_MR_SLEEP_NORMAL (0x0u << 5) /**< \brief (ADC_MR) Normal mode: The ADC core and reference voltage circuitry are kept on between conversions. */
#define   ADC_MR_SLEEP_SLEEP (0x1u << 5) /**< \brief (ADC_MR) Sleep mode: The wakeup time can be modified by programming the FWUP bit. */
#define ADC_MR_FWUP (0x1u << 6) /**< \brief (ADC_MR) Fast Wakeup */
#define   ADC_MR_FWUP_OFF (0x0u << 6) /**< \brief (ADC_MR) If SLEEP is 1, then both ADC core and reference voltage circuitry are off between conversions. */
#define   ADC_MR_FWUP_ON (0x1u << 6) /**< \brief (ADC_MR) If SLEEP is 1, then Fast Wakeup Sleep mode: The voltage reference is on between conversions and ADC core is off. */
#define ADC_MR_PRESCAL_Pos 8
#define ADC_MR_PRESCAL_Msk (0xffu << ADC_MR_PRESCAL_Pos) /**< \brief (ADC_MR) Prescaler Rate Selection */
#define ADC_MR_PRESCAL(value) ((ADC_MR_PRESCAL_Msk & ((value) << ADC_MR_PRESCAL_Pos)))
#define ADC_MR_STARTUP_Pos 16
#define ADC_MR_STARTUP_Msk (0xfu << ADC_MR_STARTUP_Pos) /**< \brief (ADC_MR) Startup Time */
#define ADC_MR_STARTUP(value) ((ADC_MR_STARTUP_Msk & ((value) << ADC_MR_STARTUP_Pos)))
#define   ADC_MR_STARTUP_SUT0 (0x0u << 16) /**< \brief (ADC_MR) 0 period of ADCCLK */
#define   ADC_MR_STARTUP_SUT8 (0x1u << 16) /**< \brief (ADC_MR) 8 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT16 (0x2u << 16) /**< \brief (ADC_MR) 16 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT24 (0x3u << 16) /**< \brief (ADC_MR) 24 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT64 (0x4u << 16) /**< \brief (ADC_MR) 64 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT80 (0x5u << 16) /**< \brief (ADC_MR) 80 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT96 (0x6u << 16) /**< \brief (ADC_MR) 96 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT112 (0x7u << 16) /**< \brief (ADC_MR) 112 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT512 (0x8u << 16) /**< \brief (ADC_MR) 512 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT576 (0x9u << 16) /**< \brief (ADC_MR) 576 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT640 (0xAu << 16) /**< \brief (ADC_MR) 640 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT704 (0xBu << 16) /**< \brief (ADC_MR) 704 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT768 (0xCu << 16) /**< \brief (ADC_MR) 768 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT832 (0xDu << 16) /**< \brief (ADC_MR) 832 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT896 (0xEu << 16) /**< \brief (ADC_MR) 896 periods of ADCCLK */
#define   ADC_MR_STARTUP_SUT960 (0xFu << 16) /**< \brief (ADC_MR) 960 periods of ADCCLK */
#define ADC_MR_ANACH (0x1u << 23) /**< \brief (ADC_MR) Analog Change */
#define   ADC_MR_ANACH_NONE (0x0u << 23) /**< \brief (ADC_MR) No analog change on channel switching: DIFF0 is used for all channels. */
#define   ADC_MR_ANACH_ALLOWED (0x1u << 23) /**< \brief (ADC_MR) Allows different analog settings for each channel. See ADC Channel Configuration Register. */
#define ADC_MR_TRACKTIM_Pos 24
#define ADC_MR_TRACKTIM_Msk (0xfu << ADC_MR_TRACKTIM_Pos) /**< \brief (ADC_MR) Tracking Time */
#define ADC_MR_TRACKTIM(value) ((ADC_MR_TRACKTIM_Msk & ((value) << ADC_MR_TRACKTIM_Pos)))
#define ADC_MR_TRANSFER_Pos 28
#define ADC_MR_TRANSFER_Msk (0x3u << ADC_MR_TRANSFER_Pos) /**< \brief (ADC_MR) Transfer Time */
#define ADC_MR_TRANSFER(value) ((ADC_MR_TRANSFER_Msk & ((value) << ADC_MR_TRANSFER_Pos)))
#define ADC_MR_USEQ (0x1u << 31) /**< \brief (ADC_MR) User Sequence Enable */
#define   ADC_MR_USEQ_NUM_ORDER (0x0u << 31) /**< \brief (ADC_MR) Normal mode: The controller converts channels in a simple numeric order depending only on the channel index. */
#define   ADC_MR_USEQ_REG_ORDER (0x1u << 31) /**< \brief (ADC_MR) User Sequence mode: The sequence respects what is defined in the ADC_SEQR1 and ADC_SEQR2 registers and can be used to convert the same channel several times. */
/* -------- ADC_SEQR1 : (ADC Offset: 0x8) Channel Sequence Register 1 -------- */
#define ADC_SEQR1_USCH1_Pos 0
#define ADC_SEQR1_USCH1_Msk (0xfu << ADC_SEQR1_USCH1_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 1 */
#define ADC_SEQR1_USCH1(value) ((ADC_SEQR1_USCH1_Msk & ((value) << ADC_SEQR1_USCH1_Pos)))
#define ADC_SEQR1_USCH2_Pos 4
#define ADC_SEQR1_USCH2_Msk (0xfu << ADC_SEQR1_USCH2_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 2 */
#define ADC_SEQR1_USCH2(value) ((ADC_SEQR1_USCH2_Msk & ((value) << ADC_SEQR1_USCH2_Pos)))
#define ADC_SEQR1_USCH3_Pos 8
#define ADC_SEQR1_USCH3_Msk (0xfu << ADC_SEQR1_USCH3_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 3 */
#define ADC_SEQR1_USCH3(value) ((ADC_SEQR1_USCH3_Msk & ((value) << ADC_SEQR1_USCH3_Pos)))
#define ADC_SEQR1_USCH4_Pos 12
#define ADC_SEQR1_USCH4_Msk (0xfu << ADC_SEQR1_USCH4_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 4 */
#define ADC_SEQR1_USCH4(value) ((ADC_SEQR1_USCH4_Msk & ((value) << ADC_SEQR1_USCH4_Pos)))
#define ADC_SEQR1_USCH5_Pos 16
#define ADC_SEQR1_USCH5_Msk (0xfu << ADC_SEQR1_USCH5_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 5 */
#define ADC_SEQR1_USCH5(value) ((ADC_SEQR1_USCH5_Msk & ((value) << ADC_SEQR1_USCH5_Pos)))
#define ADC_SEQR1_USCH6_Pos 20
#define ADC_SEQR1_USCH6_Msk (0xfu << ADC_SEQR1_USCH6_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 6 */
#define ADC_SEQR1_USCH6(value) ((ADC_SEQR1_USCH6_Msk & ((value) << ADC_SEQR1_USCH6_Pos)))
#define ADC_SEQR1_USCH7_Pos 24
#define ADC_SEQR1_USCH7_Msk (0xfu << ADC_SEQR1_USCH7_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 7 */
#define ADC_SEQR1_USCH7(value) ((ADC_SEQR1_USCH7_Msk & ((value) << ADC_SEQR1_USCH7_Pos)))
#define ADC_SEQR1_USCH8_Pos 28
#define ADC_SEQR1_USCH8_Msk (0xfu << ADC_SEQR1_USCH8_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 8 */
#define ADC_SEQR1_USCH8(value) ((ADC_SEQR1_USCH8_Msk & ((value) << ADC_SEQR1_USCH8_Pos)))
/* -------- ADC_SEQR2 : (ADC Offset: 0xC) Channel Sequence Register 2 -------- */
#define ADC_SEQR2_USCH9_Pos 0
#define ADC_SEQR2_USCH9_Msk (0xfu << ADC_SEQR2_USCH9_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 9 */
#define ADC_SEQR2_USCH9(value) ((ADC_SEQR2_USCH9_Msk & ((value) << ADC_SEQR2_USCH9_Pos)))
#define ADC_SEQR2_USCH10_Pos 4
#define ADC_SEQR2_USCH10_Msk (0xfu << ADC_SEQR2_USCH10_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 10 */
#define ADC_SEQR2_USCH10(value) ((ADC_SEQR2_USCH10_Msk & ((value) << ADC_SEQR2_USCH10_Pos)))
#define ADC_SEQR2_USCH11_Pos 8
#define ADC_SEQR2_USCH11_Msk (0xfu << ADC_SEQR2_USCH11_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 11 */
#define ADC_SEQR2_USCH11(value) ((ADC_SEQR2_USCH11_Msk & ((value) << ADC_SEQR2_USCH11_Pos)))
#define ADC_SEQR2_USCH12_Pos 12
#define ADC_SEQR2_USCH12_Msk (0xfu << ADC_SEQR2_USCH12_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 12 */
#define ADC_SEQR2_USCH12(value) ((ADC_SEQR2_USCH12_Msk & ((value) << ADC_SEQR2_USCH12_Pos)))
#define ADC_SEQR2_USCH13_Pos 16
#define ADC_SEQR2_USCH13_Msk (0xfu << ADC_SEQR2_USCH13_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 13 */
#define ADC_SEQR2_USCH13(value) ((ADC_SEQR2_USCH13_Msk & ((value) << ADC_SEQR2_USCH13_Pos)))
#define ADC_SEQR2_USCH14_Pos 20
#define ADC_SEQR2_USCH14_Msk (0xfu << ADC_SEQR2_USCH14_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 14 */
#define ADC_SEQR2_USCH14(value) ((ADC_SEQR2_USCH14_Msk & ((value) << ADC_SEQR2_USCH14_Pos)))
#define ADC_SEQR2_USCH15_Pos 24
#define ADC_SEQR2_USCH15_Msk (0xfu << ADC_SEQR2_USCH15_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 15 */
#define ADC_SEQR2_USCH15(value) ((ADC_SEQR2_USCH15_Msk & ((value) << ADC_SEQR2_USCH15_Pos)))
#define ADC_SEQR2_USCH16_Pos 28
#define ADC_SEQR2_USCH16_Msk (0xfu << ADC_SEQR2_USCH16_Pos) /**< \brief (ADC_SEQR2) User Sequence Number 16 */
#define ADC_SEQR2_USCH16(value) ((ADC_SEQR2_USCH16_Msk & ((value) << ADC_SEQR2_USCH16_Pos)))
/* -------- ADC_CHER : (ADC Offset: 0x10) Channel Enable Register -------- */
#define ADC_CHER_CH0 (0x1u << 0) /**< \brief (ADC_CHER) Channel 0 Enable */
#define ADC_CHER_CH1 (0x1u << 1) /**< \brief (ADC_CHER) Channel 1 Enable */
#define ADC_CHER_CH2 (0x1u << 2) /**< \brief (ADC_CHER) Channel 2 Enable */
#define ADC_CHER_CH3 (0x1u << 3) /**< \brief (ADC_CHER) Channel 3 Enable */
#define ADC_CHER_CH4 (0x1u << 4) /**< \brief (ADC_CHER) Channel 4 Enable */
#define ADC_CHER_CH5 (0x1u << 5) /**< \brief (ADC_CHER) Channel 5 Enable */
#define ADC_CHER_CH6 (0x1u << 6) /**< \brief (ADC_CHER) Channel 6 Enable */
#define ADC_CHER_CH7 (0x1u << 7) /**< \brief (ADC_CHER) Channel 7 Enable */
/* -------- ADC_CHDR : (ADC Offset: 0x14) Channel Disable Register -------- */
#define ADC_CHDR_CH0 (0x1u << 0) /**< \brief (ADC_CHDR) Channel 0 Disable */
#define ADC_CHDR_CH1 (0x1u << 1) /**< \brief (ADC_CHDR) Channel 1 Disable */
#define ADC_CHDR_CH2 (0x1u << 2) /**< \brief (ADC_CHDR) Channel 2 Disable */
#define ADC_CHDR_CH3 (0x1u << 3) /**< \brief (ADC_CHDR) Channel 3 Disable */
#define ADC_CHDR_CH4 (0x1u << 4) /**< \brief (ADC_CHDR) Channel 4 Disable */
#define ADC_CHDR_CH5 (0x1u << 5) /**< \brief (ADC_CHDR) Channel 5 Disable */
#define ADC_CHDR_CH6 (0x1u << 6) /**< \brief (ADC_CHDR) Channel 6 Disable */
#define ADC_CHDR_CH7 (0x1u << 7) /**< \brief (ADC_CHDR) Channel 7 Disable */
/* -------- ADC_CHSR : (ADC Offset: 0x18) Channel Status Register -------- */
#define ADC_CHSR_CH0 (0x1u << 0) /**< \brief (ADC_CHSR) Channel 0 Status */
#define ADC_CHSR_CH1 (0x1u << 1) /**< \brief (ADC_CHSR) Channel 1 Status */
#define ADC_CHSR_CH2 (0x1u << 2) /**< \brief (ADC_CHSR) Channel 2 Status */
#define ADC_CHSR_CH3 (0x1u << 3) /**< \brief (ADC_CHSR) Channel 3 Status */
#define ADC_CHSR_CH4 (0x1u << 4) /**< \brief (ADC_CHSR) Channel 4 Status */
#define ADC_CHSR_CH5 (0x1u << 5) /**< \brief (ADC_CHSR) Channel 5 Status */
#define ADC_CHSR_CH6 (0x1u << 6) /**< \brief (ADC_CHSR) Channel 6 Status */
#define ADC_CHSR_CH7 (0x1u << 7) /**< \brief (ADC_CHSR) Channel 7 Status */
/* -------- ADC_LCDR : (ADC Offset: 0x20) Last Converted Data Register -------- */
#define ADC_LCDR_LDATA_Pos 0
#define ADC_LCDR_LDATA_Msk (0xffffu << ADC_LCDR_LDATA_Pos) /**< \brief (ADC_LCDR) Last Data Converted */
#define ADC_LCDR_CHNBOSR_Pos 24
#define ADC_LCDR_CHNBOSR_Msk (0x1fu << ADC_LCDR_CHNBOSR_Pos) /**< \brief (ADC_LCDR) Channel Number in Oversampling Mode */
#define ADC_LCDR_NO_OSR_LDATA_Pos 0
#define ADC_LCDR_NO_OSR_LDATA_Msk (0xfffu << ADC_LCDR_NO_OSR_LDATA_Pos) /**< \brief (ADC_LCDR) Last Data Converted when No Oversampling */
#define ADC_LCDR_NO_OSR_CHNB_Pos 12
#define ADC_LCDR_NO_OSR_CHNB_Msk (0xfu << ADC_LCDR_NO_OSR_CHNB_Pos) /**< \brief (ADC_LCDR) Channel Number when No Oversampling */
/* -------- ADC_IER : (ADC Offset: 0x24) Interrupt Enable Register -------- */
#define ADC_IER_RXRDY (0x1u << 0) /**< \brief (ADC_IER) Receive Ready Interrupt Enable */
#define ADC_IER_RXEMPTY (0x1u << 1) /**< \brief (ADC_IER) Receive FIFO Empty Interrupt Enable */
#define ADC_IER_RXFULL (0x1u << 2) /**< \brief (ADC_IER) Receive FIFO Full Interrupt Enable */
#define ADC_IER_RXCHUNK (0x1u << 3) /**< \brief (ADC_IER) Receive FIFO Chunk Interrupt Enable */
#define ADC_IER_RXUDR (0x1u << 4) /**< \brief (ADC_IER) Receive Under Flow Interrupt Enable */
#define ADC_IER_RXOVR (0x1u << 5) /**< \brief (ADC_IER) Receive Over Flow Interrupt Enable */
#define ADC_IER_VADCSMEV (0x1u << 6) /**< \brief (ADC_IER) VDD ADC Supply Monitor Event Interrupt Enable */
#define ADC_IER_EOS (0x1u << 18) /**< \brief (ADC_IER) End Of Sequence Interrupt Enable */
#define ADC_IER_TEMPCHG (0x1u << 19) /**< \brief (ADC_IER) Temperature Change Interrupt Enable */
#define ADC_IER_DRDY (0x1u << 24) /**< \brief (ADC_IER) Data Ready Interrupt Enable */
#define ADC_IER_GOVRE (0x1u << 25) /**< \brief (ADC_IER) General Overrun Error Interrupt Enable */
#define ADC_IER_COMPE (0x1u << 26) /**< \brief (ADC_IER) Comparison Event Interrupt Enable */
#define ADC_IER_ENDRX (0x1u << 27) /**< \brief (ADC_IER) End of Receive Transfer Interrupt Enable */
#define ADC_IER_RXBUFF (0x1u << 28) /**< \brief (ADC_IER) Receive Buffer Full Interrupt Enable */
/* -------- ADC_IDR : (ADC Offset: 0x28) Interrupt Disable Register -------- */
#define ADC_IDR_RXRDY (0x1u << 0) /**< \brief (ADC_IDR) Receive Ready Interrupt Disable */
#define ADC_IDR_RXEMPTY (0x1u << 1) /**< \brief (ADC_IDR) Receive FIFO Empty Interrupt Disable */
#define ADC_IDR_RXFULL (0x1u << 2) /**< \brief (ADC_IDR) Receive FIFO Full Interrupt Disable */
#define ADC_IDR_RXCHUNK (0x1u << 3) /**< \brief (ADC_IDR) Receive FIFO Chunk Interrupt Disable */
#define ADC_IDR_RXUDR (0x1u << 4) /**< \brief (ADC_IDR) Receive Under Flow Interrupt Disable */
#define ADC_IDR_RXOVR (0x1u << 5) /**< \brief (ADC_IDR) Receive Over Flow Interrupt Disable */
#define ADC_IDR_VADCSMEV (0x1u << 6) /**< \brief (ADC_IDR) VDD ADC Supply Monitor Event Interrupt Disable */
#define ADC_IDR_EOS (0x1u << 18) /**< \brief (ADC_IDR) End Of Sequence Interrupt Disable */
#define ADC_IDR_TEMPCHG (0x1u << 19) /**< \brief (ADC_IDR) Temperature Change Interrupt Disable */
#define ADC_IDR_DRDY (0x1u << 24) /**< \brief (ADC_IDR) Data Ready Interrupt Disable */
#define ADC_IDR_GOVRE (0x1u << 25) /**< \brief (ADC_IDR) General Overrun Error Interrupt Disable */
#define ADC_IDR_COMPE (0x1u << 26) /**< \brief (ADC_IDR) Comparison Event Interrupt Disable */
#define ADC_IDR_ENDRX (0x1u << 27) /**< \brief (ADC_IDR) End of Receive Transfer Interrupt Disable */
#define ADC_IDR_RXBUFF (0x1u << 28) /**< \brief (ADC_IDR) Receive Buffer Full Interrupt Disable */
/* -------- ADC_IMR : (ADC Offset: 0x2C) Interrupt Mask Register -------- */
#define ADC_IMR_RXRDY (0x1u << 0) /**< \brief (ADC_IMR) Receive Ready Interrupt Mask */
#define ADC_IMR_RXEMPTY (0x1u << 1) /**< \brief (ADC_IMR) Receive FIFO Empty Interrupt Mask */
#define ADC_IMR_RXFULL (0x1u << 2) /**< \brief (ADC_IMR) Receive FIFO Full Interrupt Mask */
#define ADC_IMR_RXCHUNK (0x1u << 3) /**< \brief (ADC_IMR) Receive FIFO Chunk Interrupt Mask */
#define ADC_IMR_RXUDR (0x1u << 4) /**< \brief (ADC_IMR) Receive Under Flow Interrupt Mask */
#define ADC_IMR_RXOVR (0x1u << 5) /**< \brief (ADC_IMR) Receive Over Flow Interrupt Mask */
#define ADC_IMR_VADCSMEV (0x1u << 6) /**< \brief (ADC_IMR) VDD ADC Supply Monitor Event Interrupt Mask */
#define ADC_IMR_EOS (0x1u << 18) /**< \brief (ADC_IMR) End Of Sequence Interrupt Mask */
#define ADC_IMR_TEMPCHG (0x1u << 19) /**< \brief (ADC_IMR) Temperature Change Interrupt Mask */
#define ADC_IMR_DRDY (0x1u << 24) /**< \brief (ADC_IMR) Data Ready Interrupt Mask */
#define ADC_IMR_GOVRE (0x1u << 25) /**< \brief (ADC_IMR) General Overrun Error Interrupt Mask */
#define ADC_IMR_COMPE (0x1u << 26) /**< \brief (ADC_IMR) Comparison Event Interrupt Mask */
#define ADC_IMR_ENDRX (0x1u << 27) /**< \brief (ADC_IMR) End of Receive Transfer Interrupt Mask */
#define ADC_IMR_RXBUFF (0x1u << 28) /**< \brief (ADC_IMR) Receive Buffer Full Interrupt Mask */
/* -------- ADC_ISR : (ADC Offset: 0x30) Interrupt Status Register -------- */
#define ADC_ISR_RXRDY (0x1u << 0) /**< \brief (ADC_ISR) Receive Ready (cleared on read) */
#define ADC_ISR_RXEMPTY (0x1u << 1) /**< \brief (ADC_ISR) Receive FIFO Empty (cleared on read) */
#define ADC_ISR_RXFULL (0x1u << 2) /**< \brief (ADC_ISR) Receive FIFO Full (cleared on read) */
#define ADC_ISR_RXCHUNK (0x1u << 3) /**< \brief (ADC_ISR) Receive FIFO Chunk (cleared on read) */
#define ADC_ISR_RXUDR (0x1u << 4) /**< \brief (ADC_ISR) Receive Under Flow (cleared on read) */
#define ADC_ISR_RXOVR (0x1u << 5) /**< \brief (ADC_ISR) Receive Over Flow (cleared on read) */
#define ADC_ISR_VADCSMEV (0x1u << 6) /**< \brief (ADC_ISR) VDD ADC Supply Monitor Event (cleared on read) */
#define ADC_ISR_EOS (0x1u << 18) /**< \brief (ADC_ISR) End Of Sequence (cleared on read) */
#define ADC_ISR_TEMPCHG (0x1u << 19) /**< \brief (ADC_ISR) Temperature Change (cleared on read) */
#define ADC_ISR_DRDY (0x1u << 24) /**< \brief (ADC_ISR) Data Ready (automatically set / cleared) */
#define ADC_ISR_GOVRE (0x1u << 25) /**< \brief (ADC_ISR) General Overrun Error (cleared on read) */
#define ADC_ISR_COMPE (0x1u << 26) /**< \brief (ADC_ISR) Comparison Event (cleared on read) */
#define ADC_ISR_ENDRX (0x1u << 27) /**< \brief (ADC_ISR) End of Receive Transfer (cleared by writing ADC_RCR or ADC_RNCR) */
#define ADC_ISR_RXBUFF (0x1u << 28) /**< \brief (ADC_ISR) Receive Buffer Full (cleared by writing ADC_RCR or ADC_RNCR) */
/* -------- ADC_EOC_IER : (ADC Offset: 0x34) End Of Conversion Interrupt Enable Register -------- */
#define ADC_EOC_IER_EOC0 (0x1u << 0) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 0 */
#define ADC_EOC_IER_EOC1 (0x1u << 1) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 1 */
#define ADC_EOC_IER_EOC2 (0x1u << 2) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 2 */
#define ADC_EOC_IER_EOC3 (0x1u << 3) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 3 */
#define ADC_EOC_IER_EOC4 (0x1u << 4) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 4 */
#define ADC_EOC_IER_EOC5 (0x1u << 5) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 5 */
#define ADC_EOC_IER_EOC6 (0x1u << 6) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 6 */
#define ADC_EOC_IER_EOC7 (0x1u << 7) /**< \brief (ADC_EOC_IER) End of Conversion Interrupt Enable 7 */
/* -------- ADC_EOC_IDR : (ADC Offset: 0x38) End Of Conversion Interrupt Disable Register -------- */
#define ADC_EOC_IDR_EOC0 (0x1u << 0) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 0 */
#define ADC_EOC_IDR_EOC1 (0x1u << 1) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 1 */
#define ADC_EOC_IDR_EOC2 (0x1u << 2) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 2 */
#define ADC_EOC_IDR_EOC3 (0x1u << 3) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 3 */
#define ADC_EOC_IDR_EOC4 (0x1u << 4) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 4 */
#define ADC_EOC_IDR_EOC5 (0x1u << 5) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 5 */
#define ADC_EOC_IDR_EOC6 (0x1u << 6) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 6 */
#define ADC_EOC_IDR_EOC7 (0x1u << 7) /**< \brief (ADC_EOC_IDR) End of Conversion Interrupt Disable 7 */
/* -------- ADC_EOC_IMR : (ADC Offset: 0x3C) End Of Conversion Interrupt Mask Register -------- */
#define ADC_EOC_IMR_EOC0 (0x1u << 0) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 0 */
#define ADC_EOC_IMR_EOC1 (0x1u << 1) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 1 */
#define ADC_EOC_IMR_EOC2 (0x1u << 2) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 2 */
#define ADC_EOC_IMR_EOC3 (0x1u << 3) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 3 */
#define ADC_EOC_IMR_EOC4 (0x1u << 4) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 4 */
#define ADC_EOC_IMR_EOC5 (0x1u << 5) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 5 */
#define ADC_EOC_IMR_EOC6 (0x1u << 6) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 6 */
#define ADC_EOC_IMR_EOC7 (0x1u << 7) /**< \brief (ADC_EOC_IMR) End of Conversion Interrupt Mask 7 */
/* -------- ADC_EOC_ISR : (ADC Offset: 0x40) End Of Conversion Interrupt Status Register -------- */
#define ADC_EOC_ISR_EOC0 (0x1u << 0) /**< \brief (ADC_EOC_ISR) End of Conversion 0 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC1 (0x1u << 1) /**< \brief (ADC_EOC_ISR) End of Conversion 1 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC2 (0x1u << 2) /**< \brief (ADC_EOC_ISR) End of Conversion 2 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC3 (0x1u << 3) /**< \brief (ADC_EOC_ISR) End of Conversion 3 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC4 (0x1u << 4) /**< \brief (ADC_EOC_ISR) End of Conversion 4 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC5 (0x1u << 5) /**< \brief (ADC_EOC_ISR) End of Conversion 5 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC6 (0x1u << 6) /**< \brief (ADC_EOC_ISR) End of Conversion 6 (automatically set / cleared) */
#define ADC_EOC_ISR_EOC7 (0x1u << 7) /**< \brief (ADC_EOC_ISR) End of Conversion 7 (automatically set / cleared) */
/* -------- ADC_TEMPMR : (ADC Offset: 0x44) Temperature Sensor Mode Register -------- */
#define ADC_TEMPMR_TEMPON (0x1u << 0) /**< \brief (ADC_TEMPMR) Temperature Sensor On */
#define ADC_TEMPMR_TEMPCMPMOD_Pos 4
#define ADC_TEMPMR_TEMPCMPMOD_Msk (0x3u << ADC_TEMPMR_TEMPCMPMOD_Pos) /**< \brief (ADC_TEMPMR) Temperature Comparison Mode */
#define ADC_TEMPMR_TEMPCMPMOD(value) ((ADC_TEMPMR_TEMPCMPMOD_Msk & ((value) << ADC_TEMPMR_TEMPCMPMOD_Pos)))
#define   ADC_TEMPMR_TEMPCMPMOD_LOW (0x0u << 4) /**< \brief (ADC_TEMPMR) Generates the TEMPCHG flag in ADC_ISR when the converted data is lower than the low threshold of the window. */
#define   ADC_TEMPMR_TEMPCMPMOD_HIGH (0x1u << 4) /**< \brief (ADC_TEMPMR) Generates the TEMPCHG flag in ADC_ISR when the converted data is higher than the high threshold of the window. */
#define   ADC_TEMPMR_TEMPCMPMOD_IN (0x2u << 4) /**< \brief (ADC_TEMPMR) Generates the TEMPCHG flag in ADC_ISR when the converted data is in the comparison window. */
#define   ADC_TEMPMR_TEMPCMPMOD_OUT (0x3u << 4) /**< \brief (ADC_TEMPMR) Generates the TEMPCHG flag in ADC_ISR when the converted data is out of the comparison window. */
/* -------- ADC_TEMPCWR : (ADC Offset: 0x48) Temperature Compare Window Register -------- */
#define ADC_TEMPCWR_TLOWTHRES_Pos 0
#define ADC_TEMPCWR_TLOWTHRES_Msk (0xfffu << ADC_TEMPCWR_TLOWTHRES_Pos) /**< \brief (ADC_TEMPCWR) Temperature Low Threshold */
#define ADC_TEMPCWR_TLOWTHRES(value) ((ADC_TEMPCWR_TLOWTHRES_Msk & ((value) << ADC_TEMPCWR_TLOWTHRES_Pos)))
#define ADC_TEMPCWR_THIGHTHRES_Pos 16
#define ADC_TEMPCWR_THIGHTHRES_Msk (0xfffu << ADC_TEMPCWR_THIGHTHRES_Pos) /**< \brief (ADC_TEMPCWR) Temperature High Threshold */
#define ADC_TEMPCWR_THIGHTHRES(value) ((ADC_TEMPCWR_THIGHTHRES_Msk & ((value) << ADC_TEMPCWR_THIGHTHRES_Pos)))
/* -------- ADC_OVER : (ADC Offset: 0x4C) Overrun Status Register -------- */
#define ADC_OVER_OVRE0 (0x1u << 0) /**< \brief (ADC_OVER) Overrun Error 0 */
#define ADC_OVER_OVRE1 (0x1u << 1) /**< \brief (ADC_OVER) Overrun Error 1 */
#define ADC_OVER_OVRE2 (0x1u << 2) /**< \brief (ADC_OVER) Overrun Error 2 */
#define ADC_OVER_OVRE3 (0x1u << 3) /**< \brief (ADC_OVER) Overrun Error 3 */
#define ADC_OVER_OVRE4 (0x1u << 4) /**< \brief (ADC_OVER) Overrun Error 4 */
#define ADC_OVER_OVRE5 (0x1u << 5) /**< \brief (ADC_OVER) Overrun Error 5 */
#define ADC_OVER_OVRE6 (0x1u << 6) /**< \brief (ADC_OVER) Overrun Error 6 */
#define ADC_OVER_OVRE7 (0x1u << 7) /**< \brief (ADC_OVER) Overrun Error 7 */
/* -------- ADC_EMR : (ADC Offset: 0x50) Extended Mode Register -------- */
#define ADC_EMR_CMPMODE_Pos 0
#define ADC_EMR_CMPMODE_Msk (0x3u << ADC_EMR_CMPMODE_Pos) /**< \brief (ADC_EMR) Comparison Mode */
#define ADC_EMR_CMPMODE(value) ((ADC_EMR_CMPMODE_Msk & ((value) << ADC_EMR_CMPMODE_Pos)))
#define   ADC_EMR_CMPMODE_LOW (0x0u << 0) /**< \brief (ADC_EMR) When the converted data is lower than the low threshold of the window, generates the COMPE flag in ADC_ISR or, in Partial Wakeup mode, defines the conditions to exit the system from Wait mode. */
#define   ADC_EMR_CMPMODE_HIGH (0x1u << 0) /**< \brief (ADC_EMR) When the converted data is higher than the high threshold of the window, generates the COMPE flag in ADC_ISR or, in Partial Wakeup mode, defines the conditions to exit the system from Wait mode. */
#define   ADC_EMR_CMPMODE_IN (0x2u << 0) /**< \brief (ADC_EMR) When the converted data is in the comparison window, generates the COMPE flag in ADC_ISR or, in Partial Wakeup mode, defines the conditions to exit the system from Wait mode. */
#define   ADC_EMR_CMPMODE_OUT (0x3u << 0) /**< \brief (ADC_EMR) When the converted data is out of the comparison window, generates the COMPE flag in ADC_ISR or, in Partial Wakeup mode, defines the conditions to exit the system from Wait mode. */
#define ADC_EMR_CMPTYPE (0x1u << 2) /**< \brief (ADC_EMR) Comparison Type */
#define   ADC_EMR_CMPTYPE_FLAG_ONLY (0x0u << 2) /**< \brief (ADC_EMR) Any conversion is performed and comparison function drives the COMPE flag. */
#define   ADC_EMR_CMPTYPE_START_CONDITION (0x1u << 2) /**< \brief (ADC_EMR) Comparison conditions must be met to start the storage of all conversions until ADC_CR.CMPRST is set. */
#define ADC_EMR_CMPSEL_Pos 4
#define ADC_EMR_CMPSEL_Msk (0x1fu << ADC_EMR_CMPSEL_Pos) /**< \brief (ADC_EMR) Comparison Selected Channel */
#define ADC_EMR_CMPSEL(value) ((ADC_EMR_CMPSEL_Msk & ((value) << ADC_EMR_CMPSEL_Pos)))
#define ADC_EMR_CMPALL (0x1u << 9) /**< \brief (ADC_EMR) Compare All Channels */
#define ADC_EMR_CMPFILTER_Pos 12
#define ADC_EMR_CMPFILTER_Msk (0x3u << ADC_EMR_CMPFILTER_Pos) /**< \brief (ADC_EMR) Compare Event Filtering */
#define ADC_EMR_CMPFILTER(value) ((ADC_EMR_CMPFILTER_Msk & ((value) << ADC_EMR_CMPFILTER_Pos)))
#define ADC_EMR_OSR_Pos 16
#define ADC_EMR_OSR_Msk (0x7u << ADC_EMR_OSR_Pos) /**< \brief (ADC_EMR) Over Sampling Rate */
#define ADC_EMR_OSR(value) ((ADC_EMR_OSR_Msk & ((value) << ADC_EMR_OSR_Pos)))
#define   ADC_EMR_OSR_NO_AVERAGE (0x0u << 16) /**< \brief (ADC_EMR) No averaging. ADC sample rate is maximum. */
#define   ADC_EMR_OSR_OSR4 (0x1u << 16) /**< \brief (ADC_EMR) 1-bit enhanced resolution by averaging. ADC sample rate divided by 4. */
#define   ADC_EMR_OSR_OSR16 (0x2u << 16) /**< \brief (ADC_EMR) 2-bit enhanced resolution by averaging. ADC sample rate divided by 16. */
#define   ADC_EMR_OSR_OSR64 (0x3u << 16) /**< \brief (ADC_EMR) 1-bit enhanced resolution by averaging. ADC sample rate divided by 64 */
#define   ADC_EMR_OSR_OSR256 (0x4u << 16) /**< \brief (ADC_EMR) 2-bit enhanced resolution by averaging. ADC sample rate divided by 256. */
#define ADC_EMR_ASTE (0x1u << 20) /**< \brief (ADC_EMR) Averaging on Single Trigger Event */
#define   ADC_EMR_ASTE_MULTI_TRIG_AVERAGE (0x0u << 20) /**< \brief (ADC_EMR) The average requests several trigger events. */
#define   ADC_EMR_ASTE_SINGLE_TRIG_AVERAGE (0x1u << 20) /**< \brief (ADC_EMR) The average requests only one trigger event. */
#define ADC_EMR_SRCCLK (0x1u << 21) /**< \brief (ADC_EMR) External Clock Selection */
#define   ADC_EMR_SRCCLK_PERIPH_CLK (0x0u << 21) /**< \brief (ADC_EMR) The peripheral clock is the source for the ADC prescaler. */
#define   ADC_EMR_SRCCLK_GCLK (0x1u << 21) /**< \brief (ADC_EMR) GCLK is the source clock for the ADC prescaler, thus the ADC clock can be independent of the core/peripheral clock. */
#define ADC_EMR_TRACKX_Pos 22
#define ADC_EMR_TRACKX_Msk (0x3u << ADC_EMR_TRACKX_Pos) /**< \brief (ADC_EMR) Tracking Time x4, x8 or x16 */
#define ADC_EMR_TRACKX(value) ((ADC_EMR_TRACKX_Msk & ((value) << ADC_EMR_TRACKX_Pos)))
#define   ADC_EMR_TRACKX_TRACKTIMX1 (0x0u << 22) /**< \brief (ADC_EMR) ADC_MR.TRACKTIM effect is multiplied by 1. */
#define   ADC_EMR_TRACKX_TRACKTIMX4 (0x1u << 22) /**< \brief (ADC_EMR) ADC_MR.TRACKTIM effect is multiplied by 4. */
#define   ADC_EMR_TRACKX_TRACKTIMX8 (0x2u << 22) /**< \brief (ADC_EMR) ADC_MR.TRACKTIM effect is multiplied by 8 */
#define   ADC_EMR_TRACKX_TRACKTIMX16 (0x3u << 22) /**< \brief (ADC_EMR) ADC_MR.TRACKTIM effect is multiplied by 16. */
#define ADC_EMR_TAG (0x1u << 24) /**< \brief (ADC_EMR) ADC_LCDR Tag */
#define ADC_EMR_SIGNMODE_Pos 25
#define ADC_EMR_SIGNMODE_Msk (0x3u << ADC_EMR_SIGNMODE_Pos) /**< \brief (ADC_EMR) Sign Mode */
#define ADC_EMR_SIGNMODE(value) ((ADC_EMR_SIGNMODE_Msk & ((value) << ADC_EMR_SIGNMODE_Pos)))
#define   ADC_EMR_SIGNMODE_SE_UNSG_DF_SIGN (0x0u << 25) /**< \brief (ADC_EMR) Single-ended channels: unsigned conversions Differential channels: signed conversions */
#define   ADC_EMR_SIGNMODE_SE_SIGN_DF_UNSG (0x1u << 25) /**< \brief (ADC_EMR) Single-ended channels: signed conversions Differential channels: unsigned conversions */
#define   ADC_EMR_SIGNMODE_ALL_UNSIGNED (0x2u << 25) /**< \brief (ADC_EMR) All channels: unsigned conversions */
#define   ADC_EMR_SIGNMODE_ALL_SIGNED (0x3u << 25) /**< \brief (ADC_EMR) All channels: signed conversions */
#define ADC_EMR_ADCMODE_Pos 28
#define ADC_EMR_ADCMODE_Msk (0x3u << ADC_EMR_ADCMODE_Pos) /**< \brief (ADC_EMR) ADC Running Mode */
#define ADC_EMR_ADCMODE(value) ((ADC_EMR_ADCMODE_Msk & ((value) << ADC_EMR_ADCMODE_Pos)))
#define   ADC_EMR_ADCMODE_NORMAL (0x0u << 28) /**< \brief (ADC_EMR) Normal mode of operation. */
#define   ADC_EMR_ADCMODE_OFFSET_ERROR (0x1u << 28) /**< \brief (ADC_EMR) Offset Error mode to measure the offset error. See Table 6-6 "ADC Running Modes". */
#define   ADC_EMR_ADCMODE_GAIN_ERROR_HIGH (0x2u << 28) /**< \brief (ADC_EMR) Gain Error mode to measure the gain error. See Table 6-6 "ADC Running Modes". */
#define   ADC_EMR_ADCMODE_GAIN_ERROR_LOW (0x3u << 28) /**< \brief (ADC_EMR) Gain Error mode to measure the gain error. See Table 6-6 "ADC Running Modes". */
/* -------- ADC_CWR : (ADC Offset: 0x54) Compare Window Register -------- */
#define ADC_CWR_LOWTHRES_Pos 0
#define ADC_CWR_LOWTHRES_Msk (0xffffu << ADC_CWR_LOWTHRES_Pos) /**< \brief (ADC_CWR) Low Threshold */
#define ADC_CWR_LOWTHRES(value) ((ADC_CWR_LOWTHRES_Msk & ((value) << ADC_CWR_LOWTHRES_Pos)))
#define ADC_CWR_HIGHTHRES_Pos 16
#define ADC_CWR_HIGHTHRES_Msk (0xffffu << ADC_CWR_HIGHTHRES_Pos) /**< \brief (ADC_CWR) High Threshold */
#define ADC_CWR_HIGHTHRES(value) ((ADC_CWR_HIGHTHRES_Msk & ((value) << ADC_CWR_HIGHTHRES_Pos)))
/* -------- ADC_CCR : (ADC Offset: 0x5C) Channel Configuration Register -------- */
#define ADC_CCR_DIFF0 (0x1u << 0) /**< \brief (ADC_CCR) Differential Inputs for Channel 0 */
#define ADC_CCR_DIFF1 (0x1u << 1) /**< \brief (ADC_CCR) Differential Inputs for Channel 1 */
#define ADC_CCR_DIFF2 (0x1u << 2) /**< \brief (ADC_CCR) Differential Inputs for Channel 2 */
#define ADC_CCR_DIFF3 (0x1u << 3) /**< \brief (ADC_CCR) Differential Inputs for Channel 3 */
#define ADC_CCR_DIFF4 (0x1u << 4) /**< \brief (ADC_CCR) Differential Inputs for Channel 4 */
#define ADC_CCR_DIFF5 (0x1u << 5) /**< \brief (ADC_CCR) Differential Inputs for Channel 5 */
#define ADC_CCR_DIFF6 (0x1u << 6) /**< \brief (ADC_CCR) Differential Inputs for Channel 6 */
#define ADC_CCR_DIFF7 (0x1u << 7) /**< \brief (ADC_CCR) Differential Inputs for Channel 7 */
/* -------- ADC_CDR[8] : (ADC Offset: 0x60) Channel Data Register -------- */
#define ADC_CDR_DATA_Pos 0
#define ADC_CDR_DATA_Msk (0xffffu << ADC_CDR_DATA_Pos) /**< \brief (ADC_CDR[8]) Converted Data */
/* -------- ADC_ACR : (ADC Offset: 0xE0) Analog Control Register -------- */
#define ADC_ACR_ZBAT (0x1u << 2) /**< \brief (ADC_ACR) VBAT Load Enable */
#define ADC_ACR_INTVREFEN (0x1u << 20) /**< \brief (ADC_ACR) ADC Internal Positive Voltage Reference Enable */
#define ADC_ACR_SMEN (0x1u << 21) /**< \brief (ADC_ACR) Supply Monitor Enable */
#define ADC_ACR_SMVT (0x1u << 22) /**< \brief (ADC_ACR) Supply Monitor Voltage Threshold */
/* -------- ADC_FMR : (ADC Offset: 0xE4) FIFO Mode Register -------- */
#define ADC_FMR_ENFIFO (0x1u << 0) /**< \brief (ADC_FMR) Enable FIFO */
#define ADC_FMR_ENLEVEL (0x1u << 1) /**< \brief (ADC_FMR) Enable Level */
#define ADC_FMR_CHUNK_Pos 4
#define ADC_FMR_CHUNK_Msk (0xfu << ADC_FMR_CHUNK_Pos) /**< \brief (ADC_FMR) Chunk Size */
#define ADC_FMR_CHUNK(value) ((ADC_FMR_CHUNK_Msk & ((value) << ADC_FMR_CHUNK_Pos)))
#define ADC_FMR_FIFOCNT_Pos 16
#define ADC_FMR_FIFOCNT_Msk (0xffu << ADC_FMR_FIFOCNT_Pos) /**< \brief (ADC_FMR) FIFO Count (read-only) */
#define ADC_FMR_FIFOCNT(value) ((ADC_FMR_FIFOCNT_Msk & ((value) << ADC_FMR_FIFOCNT_Pos)))
/* -------- ADC_RPR : (ADC Offset: 0x100) Receive Pointer Register -------- */
#define ADC_RPR_RXPTR_Pos 0
#define ADC_RPR_RXPTR_Msk (0xffffffffu << ADC_RPR_RXPTR_Pos) /**< \brief (ADC_RPR) Receive Pointer Register */
#define ADC_RPR_RXPTR(value) ((ADC_RPR_RXPTR_Msk & ((value) << ADC_RPR_RXPTR_Pos)))
/* -------- ADC_RCR : (ADC Offset: 0x104) Receive Counter Register -------- */
#define ADC_RCR_RXCTR_Pos 0
#define ADC_RCR_RXCTR_Msk (0xffffu << ADC_RCR_RXCTR_Pos) /**< \brief (ADC_RCR) Receive Counter Register */
#define ADC_RCR_RXCTR(value) ((ADC_RCR_RXCTR_Msk & ((value) << ADC_RCR_RXCTR_Pos)))
/* -------- ADC_RNPR : (ADC Offset: 0x110) Receive Next Pointer Register -------- */
#define ADC_RNPR_RXNPTR_Pos 0
#define ADC_RNPR_RXNPTR_Msk (0xffffffffu << ADC_RNPR_RXNPTR_Pos) /**< \brief (ADC_RNPR) Receive Next Pointer */
#define ADC_RNPR_RXNPTR(value) ((ADC_RNPR_RXNPTR_Msk & ((value) << ADC_RNPR_RXNPTR_Pos)))
/* -------- ADC_RNCR : (ADC Offset: 0x114) Receive Next Counter Register -------- */
#define ADC_RNCR_RXNCTR_Pos 0
#define ADC_RNCR_RXNCTR_Msk (0xffffu << ADC_RNCR_RXNCTR_Pos) /**< \brief (ADC_RNCR) Receive Next Counter */
#define ADC_RNCR_RXNCTR(value) ((ADC_RNCR_RXNCTR_Msk & ((value) << ADC_RNCR_RXNCTR_Pos)))
/* -------- ADC_PTCR : (ADC Offset: 0x120) Transfer Control Register -------- */
#define ADC_PTCR_RXTEN (0x1u << 0) /**< \brief (ADC_PTCR) Receiver Transfer Enable */
#define ADC_PTCR_RXTDIS (0x1u << 1) /**< \brief (ADC_PTCR) Receiver Transfer Disable */
#define ADC_PTCR_TXTEN (0x1u << 8) /**< \brief (ADC_PTCR) Transmitter Transfer Enable */
#define ADC_PTCR_TXTDIS (0x1u << 9) /**< \brief (ADC_PTCR) Transmitter Transfer Disable */
#define ADC_PTCR_RXCBEN (0x1u << 16) /**< \brief (ADC_PTCR) Receiver Circular Buffer Enable */
#define ADC_PTCR_RXCBDIS (0x1u << 17) /**< \brief (ADC_PTCR) Receiver Circular Buffer Disable */
#define ADC_PTCR_TXCBEN (0x1u << 18) /**< \brief (ADC_PTCR) Transmitter Circular Buffer Enable */
#define ADC_PTCR_TXCBDIS (0x1u << 19) /**< \brief (ADC_PTCR) Transmitter Circular Buffer Disable */
#define ADC_PTCR_ERRCLR (0x1u << 24) /**< \brief (ADC_PTCR) Transfer Bus Error Clear */
/* -------- ADC_PTSR : (ADC Offset: 0x124) Transfer Status Register -------- */
#define ADC_PTSR_RXTEN (0x1u << 0) /**< \brief (ADC_PTSR) Receiver Transfer Enable */
#define ADC_PTSR_TXTEN (0x1u << 8) /**< \brief (ADC_PTSR) Transmitter Transfer Enable */
#define ADC_PTSR_RXCBEN (0x1u << 16) /**< \brief (ADC_PTSR) Receiver Circular Buffer Enable */
#define ADC_PTSR_TXCBEN (0x1u << 18) /**< \brief (ADC_PTSR) Transmitter Circular Buffer Enable */
#define ADC_PTSR_ERR (0x1u << 24) /**< \brief (ADC_PTSR) Transfer Bus Error */
/* -------- ADC_PWPMR : (ADC Offset: 0x128) Write Protection Mode Register -------- */
#define ADC_PWPMR_WPPTREN (0x1u << 0) /**< \brief (ADC_PWPMR) Write Protection Pointer Registers Enable */
#define ADC_PWPMR_WPCTREN (0x1u << 1) /**< \brief (ADC_PWPMR) Write Protection Counter Registers Enable */
#define ADC_PWPMR_WPCREN (0x1u << 2) /**< \brief (ADC_PWPMR) Write Protection Control Register Enable */
#define ADC_PWPMR_WPKEY_Pos 8
#define ADC_PWPMR_WPKEY_Msk (0xffffffu << ADC_PWPMR_WPKEY_Pos) /**< \brief (ADC_PWPMR) Write Protection Key */
#define ADC_PWPMR_WPKEY(value) ((ADC_PWPMR_WPKEY_Msk & ((value) << ADC_PWPMR_WPKEY_Pos)))
#define   ADC_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (ADC_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */
/* -------- ADC_TRGR : (ADC Offset: 0x130) Trigger Register -------- */
#define ADC_TRGR_TRGMOD_Pos 0
#define ADC_TRGR_TRGMOD_Msk (0x7u << ADC_TRGR_TRGMOD_Pos) /**< \brief (ADC_TRGR) Trigger Mode */
#define ADC_TRGR_TRGMOD(value) ((ADC_TRGR_TRGMOD_Msk & ((value) << ADC_TRGR_TRGMOD_Pos)))
#define   ADC_TRGR_TRGMOD_NO_TRIGGER (0x0u << 0) /**< \brief (ADC_TRGR) No hardware trigger, only software trigger can start conversions */
#define   ADC_TRGR_TRGMOD_EXT_TRIG_RISE (0x1u << 0) /**< \brief (ADC_TRGR) Rising edge of the selected trigger event, defined in ADC_MR.TRGSEL */
#define   ADC_TRGR_TRGMOD_EXT_TRIG_FALL (0x2u << 0) /**< \brief (ADC_TRGR) Falling edge of the selected trigger event */
#define   ADC_TRGR_TRGMOD_EXT_TRIG_ANY (0x3u << 0) /**< \brief (ADC_TRGR) Any edge of the selected trigger event */
#define   ADC_TRGR_TRGMOD_PERIOD_TRIG (0x5u << 0) /**< \brief (ADC_TRGR) ADC internal periodic trigger (see TRGPER) */
#define   ADC_TRGR_TRGMOD_CONTINUOUS (0x6u << 0) /**< \brief (ADC_TRGR) Continuous mode, free run mode */
#define ADC_TRGR_TRGPER_Pos 8
#define ADC_TRGR_TRGPER_Msk (0xffffffu << ADC_TRGR_TRGPER_Pos) /**< \brief (ADC_TRGR) Trigger Period */
#define ADC_TRGR_TRGPER(value) ((ADC_TRGR_TRGPER_Msk & ((value) << ADC_TRGR_TRGPER_Pos)))
/* -------- ADC_COSR : (ADC Offset: 0x134) Correction Select Register -------- */
#define ADC_COSR_CSEL_Pos 0
#define ADC_COSR_CSEL_Msk (0x1fu << ADC_COSR_CSEL_Pos) /**< \brief (ADC_COSR) Channel Correction Select */
#define ADC_COSR_CSEL(value) ((ADC_COSR_CSEL_Msk & ((value) << ADC_COSR_CSEL_Pos)))
/* -------- ADC_CVR : (ADC Offset: 0x138) Correction Values Register -------- */
#define ADC_CVR_OFFSETCORR_Pos 0
#define ADC_CVR_OFFSETCORR_Msk (0xffffu << ADC_CVR_OFFSETCORR_Pos) /**< \brief (ADC_CVR) Offset Correction */
#define ADC_CVR_OFFSETCORR(value) ((ADC_CVR_OFFSETCORR_Msk & ((value) << ADC_CVR_OFFSETCORR_Pos)))
#define ADC_CVR_GAINCORR_Pos 16
#define ADC_CVR_GAINCORR_Msk (0xffffu << ADC_CVR_GAINCORR_Pos) /**< \brief (ADC_CVR) Gain Correction */
#define ADC_CVR_GAINCORR(value) ((ADC_CVR_GAINCORR_Msk & ((value) << ADC_CVR_GAINCORR_Pos)))
/* -------- ADC_CECR : (ADC Offset: 0x13C) Channel Error Correction Register -------- */
#define ADC_CECR_ECORR0 (0x1u << 0) /**< \brief (ADC_CECR) Error Correction Enable for Channel 0 */
#define ADC_CECR_ECORR1 (0x1u << 1) /**< \brief (ADC_CECR) Error Correction Enable for Channel 1 */
#define ADC_CECR_ECORR2 (0x1u << 2) /**< \brief (ADC_CECR) Error Correction Enable for Channel 2 */
#define ADC_CECR_ECORR3 (0x1u << 3) /**< \brief (ADC_CECR) Error Correction Enable for Channel 3 */
#define ADC_CECR_ECORR4 (0x1u << 4) /**< \brief (ADC_CECR) Error Correction Enable for Channel 4 */
#define ADC_CECR_ECORR5 (0x1u << 5) /**< \brief (ADC_CECR) Error Correction Enable for Channel 5 */
#define ADC_CECR_ECORR6 (0x1u << 6) /**< \brief (ADC_CECR) Error Correction Enable for Channel 6 */
#define ADC_CECR_ECORR7 (0x1u << 7) /**< \brief (ADC_CECR) Error Correction Enable for Channel 7 */
/* -------- ADC_SR : (ADC Offset: 0x144) Status Register -------- */
#define ADC_SR_VADCSM (0x1u << 0) /**< \brief (ADC_SR) VDD ADC Supply Monitor Output */
/* -------- ADC_WPMR : (ADC Offset: 0x14C) Write Protection Mode Register -------- */
#define ADC_WPMR_WPEN (0x1u << 0) /**< \brief (ADC_WPMR) Write Protection Enable */
#define ADC_WPMR_WPITEN (0x1u << 1) /**< \brief (ADC_WPMR) Write Protection Interrupt Enable */
#define ADC_WPMR_WPCREN (0x1u << 2) /**< \brief (ADC_WPMR) Write Protection Control Register Enable */
#define ADC_WPMR_WPKEY_Pos 8
#define ADC_WPMR_WPKEY_Msk (0xffffffu << ADC_WPMR_WPKEY_Pos) /**< \brief (ADC_WPMR) Write Protection Key */
#define ADC_WPMR_WPKEY(value) ((ADC_WPMR_WPKEY_Msk & ((value) << ADC_WPMR_WPKEY_Pos)))
#define   ADC_WPMR_WPKEY_PASSWD (0x414443u << 8) /**< \brief (ADC_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- ADC_WPSR : (ADC Offset: 0x150) Write Protection Status Register -------- */
#define ADC_WPSR_WPVS (0x1u << 0) /**< \brief (ADC_WPSR) Write Protection Violation Status */
#define ADC_WPSR_WPVSRC_Pos 8
#define ADC_WPSR_WPVSRC_Msk (0xffffu << ADC_WPSR_WPVSRC_Pos) /**< \brief (ADC_WPSR) Write Protection Violation Source */

/*@}*/


#endif /* _PIC32CX_ADC_COMPONENT_ */
