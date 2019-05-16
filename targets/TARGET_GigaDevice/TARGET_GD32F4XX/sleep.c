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

#if DEVICE_SLEEP

#include "sleep_api.h"
#include "us_ticker_api.h"
#include "mbed_critical.h"
#include "mbed_error.h"

extern void ticker_timer_data_save(void);
extern void ticker_timer_data_restore(void);
extern int serial_busy_state_check(void);
extern int mbed_sdk_inited;

/*!
    \brief      configure the system clock to 200M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_200m_25m_hxtal(void)
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
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 25, PLL_N = 400, PLL_P = 2, PLL_Q = 9 */
    RCU_PLL = (25U | (400U << 6U) | (((2U >> 1U) - 1U) << 16U) |
               (RCU_PLLSRC_HXTAL) | (9U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while (0U == (RCU_CTL & RCU_CTL_PLLSTB)) {
    }

    /* Enable the high-drive to extend the clock frequency to 200 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while (0U == (PMU_CS & PMU_CS_HDRF)) {
    }

    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while (0U == (PMU_CS & PMU_CS_HDSRF)) {
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while (0U == (RCU_CFG0 & RCU_SCSS_PLLP)) {
    }
}


/** Send the microcontroller to sleep
 *
 * The processor is setup ready for sleep, and sent to sleep. In this mode, the
 * system clock to the core is stopped until a reset or an interrupt occurs. This eliminates
 * dynamic power used by the processor, memory systems and buses. The processor, peripheral and
 * memory state are maintained, and the peripherals continue to work and can generate interrupts.
 *
 * The processor can be woken up by any internal peripheral interrupt or external pin interrupt.
 *
 * The wake-up time shall be less than 10 us.
 *
 */
void hal_sleep(void)
{
    /* disable interrupts */
    core_util_critical_section_enter();

    /* enter SLEEP mode */
    pmu_to_sleepmode(WFI_CMD);

    /* enable interrupts */
    core_util_critical_section_exit();
}


/** Send the microcontroller to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and high frequency clocks.
 * All state is still maintained.
 *
 * The processor can only be woken up by low power ticker, RTC, an external interrupt on a pin or a watchdog timer.
 *
 * The wake-up time shall be less than 10 ms.
 */
void hal_deepsleep(void)
{
    if (0 != serial_busy_state_check()) {
        return;
    }

    /* disable interrupts */
    core_util_critical_section_enter();

    ticker_timer_data_save();

    /* enter DEEP-SLEEP mode */
    rcu_periph_clock_enable(RCU_PMU);
    pmu_to_deepsleepmode(PMU_LDO_NORMAL, WFI_CMD);

    mbed_sdk_inited = 0;

    /* reconfigure the PLL and set the system clock to the highest frequence after weak up */
    system_clock_200m_25m_hxtal();

    ticker_timer_data_restore();
    mbed_sdk_inited = 1;

    /* enable interrupts */
    core_util_critical_section_exit();
}

#endif /* DEVICE_SLEEP */
