/*!
 *****************************************************************************
 * @file:    adi_pwr_def_v1.h
 * @brief:   CMSIS Cortex-M3 Device clock and power management function 
 * @version: $Revision: 33449 $
 * @date:    $Date: 2016-02-08 14:17:03 +0000 (Mon, 08 Feb 2016) $
 *-----------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2014 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Modified versions of the software must be conspicuously marked as such.
 * - This software is licensed solely and exclusively for use with processors
 *   manufactured by or for Analog Devices, Inc.
 * - This software may not be combined or merged with other code in any manner
 *   that would cause the software to become subject to terms and conditions
 *   which differ from those listed here.
 * - Neither the name of Analog Devices, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 * - The use of this software may or may not infringe the patent rights of one
 *   or more patent holders.  This license does not release you from the
 *   requirement that you obtain separate licenses from these patent holders
 *   to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
 * PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef __ADI_PWR_DEF_V1_H__
#define __ADI_PWR_DEF_V1_H__
/* System clock constant */
#define __HFOSC     26000000u  

/* System clock constant (may also be 16000000) */
#define __HFXTAL    26000000u  

 /*System clock constant (same whether internal osc or external xtal) */
#define __LFCLK        32768u 


 /*Power control register access key */
#define ADI_PMG_KEY 0x4859u

 /*Osc control register access key */
#define OSCKEY_UNLOCK                (0xCB14u)

 /*ACLK/HCLK/PCLK minimum Divider  value */
#define CLOCK_MIN_DIV_VALUE        (0x1u)

 /*ACLK/HCLK/PCLK maximum Divider  value */
#define CLOCK_MAX_DIV_VALUE                (32u)

/*! Minimum divider for PLL */
#define MINIMUM_PLL_DIVIDER     0x02u

/*! Minimum multiplier for PLL */
#define MINIMUM_PLL_MULTIPLIER  0x08u

#define MAXIMUM_EXT_CLOCK 26000000u

 /*Default osc  control register value*/
#define OSCCTRL_CONFIG_VALUE ((uint32_t)ADI_PWR_HFOSC_CLOCK_ENABLE << BITP_CLKG_OSC_CTL_HFOSCEN  | \
                              (uint32_t)ADI_PWR_LFXTL_CLOCK_ENABLE << BITP_CLKG_OSC_CTL_LFXTALEN | \
                              (uint32_t)ADI_PWR_HFXTL_CLOCK_ENABLE << BITP_CLKG_OSC_CTL_HFXTALEN)

 /*Default clock  control register-0 value*/
#define CLOCKN0_CONFIG_VALUE ((uint32_t)ADI_PWR_INPUT_TO_ROOT_CLOCK_MUX      << BITP_CLKG_CLK_CTL0_CLKMUX    | \
                              (uint32_t)ADI_PWR_INPUT_TO_OUTPUT_RCLK_MUX     << BITP_CLKG_CLK_CTL0_RCLKMUX   | \
                              (uint32_t)ADI_PWR_INPUT_TO_SPLL_MUX            << BITP_CLKG_CLK_CTL0_SPLLIPSEL | \
                              (uint32_t)ADI_PWR_LFXTL_CLOCK_INTERRUPT_ENABLE << BITP_CLKG_CLK_CTL0_LFXTALIE  | \
                              (uint32_t)ADI_PWR_HFXTL_CLOCK_INTERRUPT_ENABLE << BITP_CLKG_CLK_CTL0_HFXTALIE)

 /*Default clock  control register-1 value*/
#define CLOCKN1_CONFIG_VALUE ((uint32_t)ADI_PWR_HCLK_DIVIDE_COUNT << BITP_CLKG_CLK_CTL1_HCLKDIVCNT | \
                              (uint32_t)ADI_PWR_PCLK_DIVIDE_COUNT << BITP_CLKG_CLK_CTL1_PCLKDIVCNT | \
                              (uint32_t)ADI_PWR_ACLK_DIVIDE_COUNT << BITP_CLKG_CLK_CTL1_ACLKDIVCNT)


 /*Default clock  control register-3 value*/
#define CLOCKN3_CONFIG_VALUE ((uint32_t)ADI_PWR_SPLL_MUL_FACTOR       << BITP_CLKG_CLK_CTL3_SPLLNSEL | \
                              (uint32_t)ADI_PWR_SPLL_DIV_FACTOR       << BITP_CLKG_CLK_CTL3_SPLLMSEL | \
                              (uint32_t)ADI_PWR_SPLL_ENABLE_DIV2      << BITP_CLKG_CLK_CTL3_SPLLDIV2 | \
                              (uint32_t)ADI_PWR_SPLL_ENABLE_MUL2      << BITP_CLKG_CLK_CTL3_SPLLMUL2 | \
                              (uint32_t)ADI_PWR_SPLL_INTERRUPT_ENABLE << BITP_CLKG_CLK_CTL3_SPLLIE)

 /*Default clock  control register-3 value*/
#define CLOCKN5_CONFIG_VALUE ((uint32_t)ADI_PWR_GPT2_CLOCK_ENABLE << BITP_CLKG_CLK_CTL5_GPTCLK0OFF | \
                              (uint32_t)ADI_PWR_GPT1_CLOCK_ENABLE << BITP_CLKG_CLK_CTL5_GPTCLK1OFF | \
                              (uint32_t)ADI_PWR_GPT0_CLOCK_ENABLE << BITP_CLKG_CLK_CTL5_GPTCLK2OFF | \
                              (uint32_t)ADI_PWR_I2C_CLOCK_ENABLE  << BITP_CLKG_CLK_CTL5_UCLKI2COFF | \
                              (uint32_t)ADI_PWR_GPIO_CLOCK_ENABLE << BITP_CLKG_CLK_CTL5_GPIOCLKOFF | \
                              (uint32_t)ADI_PWR_PCLK_ENABLE       << BITP_CLKG_CLK_CTL5_PERCLKOFF)

#define PWM_INTERRUPT_CONFIG ((uint32_t)ADI_PWR_ENABLE_VBAT_INTERRUPT                  << BITP_PMG_IEN_VBAT     | \
                              (uint32_t)ADI_PWR_ENABLE_VREG_UNDER_VOLTAGE_INTERRUPT    << BITP_PMG_IEN_VREGUNDR | \
                              (uint32_t)ADI_PWR_ENABLE_VREG_OVER_VOLTAGE_INTERRUPT     << BITP_PMG_IEN_VREGOVR  | \
                              (uint32_t)ADI_PWR_ENABLE_BATTERY_VOLTAGE_RANGE_INTERRUPT << BITP_PMG_IEN_IENBAT   | \
                              (uint32_t)ADI_PWR_BATTERY_VOLTAGE_RANGE_FOR_INTERRUPT    << BITP_PMG_IEN_RANGEBAT)
 
 /*Selecting HFOSC as input for generating root clock*/
#define HFMUX_INTERNAL_OSC_VAL      (0u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting HFXTL as input for generating root clock*/
#define HFMUX_EXTERNAL_XTAL_VAL     (1u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting SPLL as input for generating root clock*/
#define HFMUX_SYSTEM_SPLL_VAL       (2u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting GPIO as input for generating root clock*/
#define HFMUX_GPIO_VAL              (3u << BITP_CLKG_CLK_CTL0_CLKMUX)

/* SCR Deep Sleep (SLEEPDEEP) */
#define SLEEPDEEP_BIT                    (1u << 2)  
/* SCR Sleep On Exit (SLEEPONEXIT) */
#define SLEEPONEXIT_BIT                  (1u << 1)  

/* Interrupt handler for the battery voltage interrupt */
ADI_INT_HANDLER(Battery_Voltage_Int_Handler);
/* Interrupt handler for the VREG under/over voltage interrupt */
ADI_INT_HANDLER(Vreg_over_Int_Handler);
/* Interrupt handler for PLL interrupts. */
ADI_INT_HANDLER(PLL_Int_Handler);     
/*Interrupt handler for oscillator interrupts.*/                                 
ADI_INT_HANDLER(Crystal_osc_Int_Handler);
#endif /* __ADI_PWR_DEF_V1_H__ */


/*@}*//*
** EOF
*/
