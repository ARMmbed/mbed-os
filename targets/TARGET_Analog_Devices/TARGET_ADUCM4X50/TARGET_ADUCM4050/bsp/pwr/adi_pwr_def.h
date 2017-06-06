/*
 *****************************************************************************
 * @file:    adi_pwr_def.h
 * @brief:   Definitions for the system clock and power management.
 *-----------------------------------------------------------------------------
 *
 * Copyright (c) 2016 Analog Devices, Inc.
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

#ifndef ADI_PWR_DEF_H
#define ADI_PWR_DEF_H

 /*Power control register access key */
#define ADI_PMG_KEY             (0x4859u)

 /*Osc control register access key */
#define ADI_OSC_KEY             (0xCB14u)

 /*HCLK/PCLK minimum Divider value */
#define CLOCK_MIN_DIV_VALUE     (0x1u)

 /*HCLK/PCLK maximum Divider value */
#define CLOCK_MAX_DIV_VALUE     (32u)

 /*ADC Clock minimum Divider value */
#define ACLK_MIN_DIV_VALUE      (0x1u)

 /*ADC Clock maximum Divider value */
#define ACLK_MAX_DIV_VALUE      (511u)

/* Minimum divider for PLL */
#define MINIMUM_PLL_DIVIDER     (0x02u)

/* Minimum multiplier for PLL */
#define MINIMUM_PLL_MULTIPLIER  (0x08u)

/* Maximum external clock */
#define MAXIMUM_EXT_CLOCK       (26000000u)

 /* Default osc  control register value */
#define OSCCTRL_CONFIG_VALUE                                                                                              \
          ( (uint32_t) ADI_PWR_LF_CLOCK_MUX                             << BITP_CLKG_OSC_CTL_LFCLK_MUX                  | \
            (uint32_t) ADI_PWR_HFOSC_CLOCK_ENABLE                       << BITP_CLKG_OSC_CTL_HFOSC_EN                   | \
            (uint32_t) ADI_PWR_LFXTAL_CLOCK_ENABLE                      << BITP_CLKG_OSC_CTL_LFX_EN                     | \
            (uint32_t) ADI_PWR_HFXTAL_CLOCK_ENABLE                      << BITP_CLKG_OSC_CTL_HFX_EN                     | \
            (uint32_t) ADI_PWR_LFXTAL_CLOCK_MON_ENABLE                  << BITP_CLKG_OSC_CTL_LFX_MON_EN                 | \
            (uint32_t) ADI_PWR_LFXTAL_FAIL_AUTO_SWITCH_ENABLE           << BITP_CLKG_OSC_CTL_LFX_AUTSW_EN               | \
            (uint32_t) ADI_PWR_LFXTAL_ROBUST_MODE_ENABLE                << BITP_CLKG_OSC_CTL_LFX_ROBUST_EN              | \
            (uint32_t) ADI_PWR_LFXTAL_ROBUST_LOAD_SELECT                << BITP_CLKG_OSC_CTL_LFX_ROBUST_LD              | \
            (uint32_t) ADI_PWR_ROOT_CLOCK_MON_INT_ENABLE                << BITP_CLKG_OSC_CTL_ROOT_MON_EN                | \
            (uint32_t) ADI_PWR_ROOT_CLOCK_FAIL_AUTOSWITCH_ENABLE        << BITP_CLKG_OSC_CTL_ROOT_AUTSW_EN )

 /* Default clock control register-0 value */
#define CLOCK_CTL0_CONFIG_VALUE                                                                                           \
          ( (uint32_t) ADI_PWR_INPUT_TO_ROOT_CLOCK_MUX                  << BITP_CLKG_CLK_CTL0_CLKMUX                    | \
            (uint32_t) ADI_PWR_GPIO_CLOCK_OUT_SELECT                    << BITP_CLKG_CLK_CTL0_CLKOUT                    | \
            (uint32_t) ADI_PWR_INPUT_TO_RCLK_MUX                        << BITP_CLKG_CLK_CTL0_RCLKMUX                   | \
            (uint32_t) ADI_PWR_INPUT_TO_SPLL_MUX                        << BITP_CLKG_CLK_CTL0_PLL_IPSEL                 | \
            (uint32_t) ADI_PWR_LFXTAL_CLOCK_INTERRUPT_ENABLE            << BITP_CLKG_CLK_CTL0_LFXTALIE                  | \
            (uint32_t) ADI_PWR_HFXTAL_CLOCK_INTERRUPT_ENABLE             << BITP_CLKG_CLK_CTL0_HFXTALIE )

 /* Default clock control register-1 value */
#define CLOCK_CTL1_CONFIG_VALUE                                                                                           \
          ( (uint32_t) ADI_PWR_HCLK_DIVIDE_COUNT                        << BITP_CLKG_CLK_CTL1_HCLKDIVCNT                | \
            (uint32_t) ADI_PWR_PCLK_DIVIDE_COUNT                        << BITP_CLKG_CLK_CTL1_PCLKDIVCNT                | \
            (uint32_t) ADI_PWR_ACLK_DIVIDE_COUNT                        << BITP_CLKG_CLK_CTL1_ACLKDIVCNT )

/* Default clock control register-2 value */
#define CLOCK_CTL2_CONFIG_VALUE                                                                                           \
          ( (uint32_t) ADI_PWR_HFOSC_AUTO_DIV_BY_1                      << BITP_CLKG_CLK_CTL2_HFOSCAUTODIV_EN           | \
            (uint32_t) ADI_PWR_HFOSC_DIVIDE_SELECT                      << BITP_CLKG_CLK_CTL2_HFOSCDIVCLKSEL )

 /* Default clock control register-3 value */
#define CLOCK_CTL3_CONFIG_VALUE                                                                                           \
          ( (uint32_t) ADI_PWR_SPLL_MUL_FACTOR                          << BITP_CLKG_CLK_CTL3_SPLLNSEL                  | \
            (uint32_t) ADI_PWR_SPLL_ENABLE_DIV2                         << BITP_CLKG_CLK_CTL3_SPLLDIV2                  | \
            (uint32_t) ADI_PWR_SPLL_ENABLE                              << BITP_CLKG_CLK_CTL3_SPLLEN                    | \
            (uint32_t) ADI_PWR_SPLL_INTERRUPT_ENABLE                    << BITP_CLKG_CLK_CTL3_SPLLIE                    | \
            (uint32_t) ADI_PWR_SPLL_DIV_FACTOR                          << BITP_CLKG_CLK_CTL3_SPLLMSEL                  | \
            (uint32_t) ADI_PWR_SPLL_ENABLE_MUL2                         << BITP_CLKG_CLK_CTL3_SPLLMUL2 )

 /* Default clock control register-5 value */
#define CLOCK_CTL5_CONFIG_VALUE                                                                                           \
          ( (uint32_t) ADI_PWR_GPT0_CLOCK_ENABLE                        << BITP_CLKG_CLK_CTL5_GPTCLK0OFF                | \
            (uint32_t) ADI_PWR_GPT1_CLOCK_ENABLE                        << BITP_CLKG_CLK_CTL5_GPTCLK1OFF                | \
            (uint32_t) ADI_PWR_GPT2_CLOCK_ENABLE                        << BITP_CLKG_CLK_CTL5_GPTCLK2OFF                | \
            (uint32_t) ADI_PWR_I2C_CLOCK_ENABLE                         << BITP_CLKG_CLK_CTL5_UCLKI2COFF                | \
            (uint32_t) ADI_PWR_GPIO_CLOCK_ENABLE                        << BITP_CLKG_CLK_CTL5_GPIOCLKOFF                | \
            (uint32_t) ADI_PWR_PCLK_ENABLE                              << BITP_CLKG_CLK_CTL5_PERCLKOFF                 | \
            (uint32_t) ADI_PWR_TIMER_RGB_ENABLE                         << BITP_CLKG_CLK_CTL5_TMRRGBCLKOFF )
            
/* Default configuration for Power supply monitor Interrupt Enable Register */            
#define PWM_INTERRUPT_CONFIG                                                                                              \
          ( (uint32_t) ADI_PWR_ENABLE_VBAT_INTERRUPT                    << BITP_PMG_IEN_VBAT                            | \
            (uint32_t) ADI_PWR_ENABLE_VREG_UNDER_VOLTAGE_INTERRUPT      << BITP_PMG_IEN_VREGUNDR                        | \
            (uint32_t) ADI_PWR_ENABLE_VREG_OVER_VOLTAGE_INTERRUPT       << BITP_PMG_IEN_VREGOVR                         | \
            (uint32_t) ADI_PWR_ENABLE_BATTERY_VOLTAGE_RANGE_INTERRUPT   << BITP_PMG_IEN_IENBAT                          | \
            (uint32_t) ADI_PWR_BATTERY_VOLTAGE_RANGE_FOR_INTERRUPT      << BITP_PMG_IEN_RANGEBAT )
            
 /* Default configuration for Power Mode Register */            
 #define PWM_PWRMOD_CONFIG                                                                                                \
         (  (uint32_t) ADI_PWR_ENABLE_BATTERY_VOLTAGE_MONITORING        << BITP_PMG_PWRMOD_MONVBATN ) 
 
/* Default configuration for HP Buck Control register */
#define PWM_HPBUCK_CONTROL                                                                                                \
         (  (uint32_t) ADI_PWR_HP_BUCK_ENABLE                           << BITP_PMG_CTL1_HPBUCKEN                       | \
            (uint32_t) ADI_PWR_HP_BUCK_LOAD_MODE                        << BITP_PMG_CTL1_HPBUCK_LD_MODE                 | \
            (uint32_t) ADI_PWR_HP_BUCK_LOW_POWER_MODE                   << BITP_PMG_CTL1_HPBUCK_LOWPWR_MODE )  
           
 /*Selecting HFOSC as input for generating root clock*/
#define HFMUX_INTERNAL_OSC_VAL      (0u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting HFXTAL as input for generating root clock*/
#define HFMUX_EXTERNAL_XTAL_VAL     (1u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting SPLL as input for generating root clock*/
#define HFMUX_SYSTEM_SPLL_VAL       (2u << BITP_CLKG_CLK_CTL0_CLKMUX)

 /*Selecting GPIO as input for generating root clock*/
#define HFMUX_GPIO_VAL              (3u << BITP_CLKG_CLK_CTL0_CLKMUX)

/* Interrupt handler for the battery voltage interrupt */
void Battery_Voltage_Int_Handler(void);
/* Interrupt handler for the VREG under/over voltage interrupt */
void Vreg_over_Int_Handler(void);
/* Interrupt handler for PLL interrupts. */
void PLL_Int_Handler(void);     
/*Interrupt handler for oscillator interrupts.*/                                 
void Crystal_osc_Int_Handler(void);

#endif /* ADI_PWR_DEF_H */


/*
** EOF
*/
