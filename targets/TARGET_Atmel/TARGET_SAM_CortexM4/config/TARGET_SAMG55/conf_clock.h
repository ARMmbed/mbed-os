/**
 * \file
 *
 * \brief SAMG55 clock configuration.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

/*
 * ===== System Clock (MCK) Source Options
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_RC
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_XTAL
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_BYPASS
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_8M_RC
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_16M_RC
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_24M_RC
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_XTAL
 * #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_BYPASS
 */
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK

/*
 *  ===== System Clock (MCK) Prescaler Options   (Fmck = Fsys / (SYSCLK_PRES))
 */
#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
/*
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_2
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_4
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_8
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_16
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_32
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_64
 * #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_3
 */

/*
 *  ===== PLL0 (A) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
 *  Use mul and div effective values here.
 */
#define CONFIG_PLL0_SOURCE          PLL_SRC_SLCK_XTAL
#define CONFIG_PLL0_MUL             3662
#define CONFIG_PLL0_DIV             1

/*
 *  ===== Target frequency (System clock)
 *  - External XTAL frequency: 32768Hz
 *  - System clock source: SLCK XTAL
 *  - System clock prescaler: 1 (divided by 1)
 *  - PLLA source: SLCK_XTAL
 *  - PLLA output: SLCK_XTAL * 3662 / 1
 *  - System clock: SLCK_XTAL * 3662 / 1 / 1 = 120MHz
 */

#endif /* CONF_CLOCK_H_INCLUDED */
