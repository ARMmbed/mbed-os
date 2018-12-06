/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "gd32f30x.h"
#include "cmsis.h"
#include "hal_tick.h"

int mbed_sdk_inited = 0;

/*!
    \brief      configure the system clock to 120M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if TICKER_TIMER_WIDTH_BIT == 16
extern void ticker_16bits_timer_init(void);
#else
extern void ticker_32bits_timer_init(void);
#endif

/*!
    \brief      configure the system clock to 120M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_120m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;

    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    } while ((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if (0U == (RCU_CTL & RCU_CTL_HXTALSTB)) {
        while (1) {
        }
    }

    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/1 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB/2 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV2;

#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
    /* select HXTAL/2 as clock source */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PREDV0);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL_IRC48M | RCU_CFG0_PREDV0);

    /* CK_PLL = (CK_HXTAL/2) * 30 = 120 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4 | RCU_CFG0_PLLMF_5);
    RCU_CFG0 |= RCU_PLL_MUL30;

#elif defined(GD32F30X_CL)
    /* CK_PLL = (CK_PREDIV0) * 30 = 120 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4 | RCU_CFG0_PLLMF_5);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL_IRC48M | RCU_PLL_MUL30);

    /* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
    RCU_CFG1 &= ~(RCU_CFG1_PLLPRESEL | RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF | RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
    RCU_CFG1 |= (RCU_PLLPRESRC_HXTAL | RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 | RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);

    /* enable PLL1 */
    RCU_CTL |= RCU_CTL_PLL1EN;
    /* wait till PLL1 is ready */
    while ((RCU_CTL & RCU_CTL_PLL1STB) == 0U) {
    }
#endif /* GD32F30X_HD and GD32F30X_XD */

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while (0U == (RCU_CTL & RCU_CTL_PLLSTB)) {
    }

    /* enable the high-drive to extend the clock frequency to 120 MHz */
    PMU_CTL |= PMU_CTL_HDEN;
    while (0U == (PMU_CS & PMU_CS_HDRF)) {
    }

    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while (0U == (PMU_CS & PMU_CS_HDSRF)) {
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while (0U == (RCU_CFG0 & RCU_SCSS_PLL)) {
    }
}

/**
 * SDK hook for running code before ctors or OS
 *
 * This is a weak function which can be overridden by a target's
 * SDK to allow code to run after ram is initialized but before
 * the OS has been started or constructors have run.
 *
 * Preconditions:
 * - Ram is initialized
 * - NVIC is setup
 */
/**
 * This function is called after RAM initialization and before main.
 */
void mbed_sdk_init()
{
    /* Update the SystemCoreClock */
    SystemCoreClockUpdate();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    /* configure 1ms tick */
#if TICKER_TIMER_WIDTH_BIT == 16
    ticker_16bits_timer_init();
#else
    ticker_32bits_timer_init();
#endif

    system_clock_120m_hxtal();

    SystemCoreClockUpdate();

    mbed_sdk_inited = 1;
}
