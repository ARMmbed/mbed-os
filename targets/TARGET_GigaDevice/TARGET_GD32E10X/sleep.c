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

/*!
    \brief      configure the system clock to 120M by PLL which selects HXTAL(8M) as its clock source
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

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | FMC_WAIT_STATE_3;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/1 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB/2 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV2;

    /* CK_PLL = (CK_PREDIV0) * 30 = 120 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL_IRC48M | RCU_PLL_MUL30);

    RCU_CFG1 &= ~(RCU_CFG1_PLLPRESEL | RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF | RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
#ifdef HXTAL_VALUE_8M
    /* CK_PREDIV0 = (CK_HXTAL)/2 *10 /10 = 4 MHz */
    RCU_CFG1 |= (RCU_PLLPRESRC_HXTAL | RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL10 | RCU_PREDV1_DIV2 | RCU_PREDV0_DIV10);
#elif defined (HXTAL_VALUE_25M)
    /* CK_PREDIV0 = (CK_HXTAL)/5 *8/10 = 4 MHz */
    RCU_CFG1 |= (RCU_PLLPRESRC_HXTAL | RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 | RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);
#endif

    /* enable PLL1 */
    RCU_CTL |= RCU_CTL_PLL1EN;
    /* wait till PLL1 is ready */
    while ((RCU_CTL & RCU_CTL_PLL1STB) == 0U) {
    }

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while (0U == (RCU_CTL & RCU_CTL_PLLSTB)) {
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while (0U == (RCU_CFG0 & RCU_SCSS_PLL)) {
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
    /* Disable Interrupts */
    core_util_critical_section_enter();

    /* Enter SLEEP mode */
    pmu_to_sleepmode(WFI_CMD);

    /* Enable Interrupts */
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

    /* Disable Interrupts */
    core_util_critical_section_enter();

    ticker_timer_data_save();

    /* Enter DEEP SLEEP mode */
    rcu_periph_clock_enable(RCU_PMU);
    pmu_to_deepsleepmode(PMU_LDO_NORMAL, WFI_CMD);

    /* Reconfigure the PLL after weak up */
    system_clock_120m_hxtal();

    ticker_timer_data_restore();

    /* Enable Interrupts */
    core_util_critical_section_exit();
}

#endif /* DEVICE_SLEEP */
