/**
 * \file
 *
 * \brief PIC32CX clock configuration.
 *
 * Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

/* ===== System Clock (MCK) Source Options */
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_RC */
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_XTAL */
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_BYPASS */
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_12M_RC */
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_XTAL */
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_BYPASS */
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK1
/* #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK0 */

/* ===== System Clock (MCK) Prescaler Options   (Fmck = Fsys / (SYSCLK_PRES)) */
#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_2 */
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_4 */
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_8 */
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_16 */
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_32 */
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_64 */
/* #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_3 */

/* ===== PLLACK1 (A) Options   (Fpll = (Fclk * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) = 400MHz) */
/* ===== PLLACK0 (A) Options   (Fpll = (Fclk * (MUL + 1 + (FRACR / 2^22))) / (DIV0 + 1) = 400MHz) */
#define CONFIG_PLL0_SOURCE          PLLA_SRC_SLCK_32K_XTAL
#define CONFIG_PLL0_MUL             12206
#define CONFIG_PLL0_FRACR           131072
#define CONFIG_PLL0_DIV1            1
#define CONFIG_PLL0_DIV0            19

/* ===== Main processor frequency (MCK0) */
/* - System clock source: PLLACK1 */
/* - System clock prescaler: 1 */
/* - PLLA source: SLCK_32K_XTAL */
/* - PLLACK1 output: (SLCK_32K_XTAL * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) */
/* - PLLACK1 output: (32768 * (12206 + 1 + (131072 / 2^22))) / ( 1 + 1) = 200 MHz */
/* - PLLACK0 output: (SLCK_32K_XTAL * (MUL + 1 + (FRACR / 2^22))) / (19 + 1) */
/* - PLLACK0 output: (32768 * (12206 + 1 + (131072 / 2^22))) / ( 19 + 1) = 20 MHz */
/* - System clock: (PLLA output 1) / (CONFIG_SYSCLK_PRES) = 200 MHz */

/* ===== Coprocessor System Clock (CPMCK) Options */
/* Note: */
/* CONFIG_CPCLK_ENABLE  MUST be defined if using peripherals on bus matrix 1. */
#define CONFIG_CPCLK_ENABLE

/* Coprocessor System Clock Source Options */
/* #define CONFIG_CPCLK_SOURCE         CPCLK_SRC_MDSLCK */
/* #define CONFIG_CPCLK_SOURCE         CPCLK_SRC_MAINCK */
/* #define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLACK1 */
#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLBCK0

/* Fcpmck = Fcpclk_source / (CPCLK_PRES + 1) */
/* Coprocessor System Clock Prescaler Options (CPCLK_PRES may be 0 to 15). */
#define CONFIG_CPCLK_PRES           0

/* ===== PLLBCK0 (C) Options   Fpll = (Fclk * (MUL + 1 + (FRACR / 2^22))) / (DIV + 1) = 480MHz) */
#define CONFIG_PLL1_SOURCE          PLLB_SRC_PLLACK0
#define CONFIG_PLL1_MUL             23
#define CONFIG_PLL1_FRACR           0
#define CONFIG_PLL1_DIV0            1

/* ===== Coprocessor frequency (CPMCK) */
/* - PLLC source: PLLACK0 (20MHz) */
/* - PLLCCK0 output: (PLLACK0 * (MUL + 1 + (FRACR / 2^22))) / (DIV0 + 1) */
/* - PLLCCK0 output: (20000000 * (23 + 1 + (0 / 2^22))) / ( 1 + 1) = 240 MHz */
/* - Coprocessor system clock: (PLLC output) / (CONFIG_CPCLK_PRES + 1) = 240 MHz */

#endif /* CONF_CLOCK_H_INCLUDED */
