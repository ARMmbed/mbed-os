/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

#ifdef APM32F411
#define APB1_PSC 0x04
#define APB2_PSC 0x00
/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_B) * PLL_A */
#define PLL_B      4
/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLL_D */
#define PLL_D      7
#define PLL_A      100
/* SYSCLK = PLL_VCO / PLL_C */
#define PLL_C      2
#else
#define APB1_PSC 0x05
#define APB2_PSC 0x04
/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_B) * PLL_A */
#define PLL_B      8
/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLL_D */
#define PLL_D      7
#define PLL_A      336
/* SYSCLK = PLL_VCO / PLL_C */
#define PLL_C      2
#endif /* APM32F411 */

extern int mbed_sdk_inited;
extern int serial_busy_state_check(void);
extern void ticker_timer_data_save(void);
extern void ticker_timer_data_restore(void);

/*!
 * @brief     configure the system clock to 168M by PLL which selects HXTAL(8M) as its clock source
 *
 * @param     None
 *
 * @retval    None
 */
static void system_clock_168m_8m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint8_t stab_flag = 0U;

    /* enable HXTAL */
    RCM->CTRL_B.HSEEN = BIT_SET;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    stab_flag = RCM_WaitHSEReady();
    /* if fail */
    if (stab_flag == 0U)
    {
        while (1)
        {
        }
    }

    RCM->APB1CLKEN_B.PMUEN = BIT_SET;
    PMU->CTRL_B.VOSSEL = BIT_SET;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCM_ConfigAHB(RCM_AHB_DIV_1);
    /* APB2 = AHB/2 */
    RCM_ConfigAPB2(RCM_APB_DIV_2);
    /* APB1 = AHB/4 */
    RCM_ConfigAPB1(RCM_APB_DIV_4);

    /* Configure the main PLL, PSC = 25, PLL_N = 400, PLL_P = 2, PLL_Q = 9 */
    RCM->PLL1CFG = PLL_B | (PLL_A << 6) | (((PLL_C >> 1) -1) << 16) |(PLL_D << 24);
    RCM->PLL1CFG_B.PLL1CLKS = 0x01;

    /* enable PLL */
    RCM_EnablePLL1();

    /* wait until PLL is stable */
    while (RCM->CTRL_B.PLL1RDYFLG == 0U)
    {
    }

    /* select PLL as system clock */
    RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_PLL);

    /* wait until PLL is selected as system clock */
    while (1)
    {
        if (RCM->CFG_B.SCLKSWSTS == 2U)
        {
            break;
        }
    }
}

/*!
 * @brief     Send the microcontroller to sleep
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
 * @param     None
 *
 * @retval    None
 */
void hal_sleep(void)
{
    /* disable interrupts */
    core_util_critical_section_enter();

    /* enter SLEEP mode */
    /* clear sleepdeep bit of Cortex-M4 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

    /* select WFI or WFE command to enter sleep mode */
    if (PMU_STOP_ENTRY_WFI == BIT_SET)
    {
        __WFI();
        __NOP();
        __NOP();
        __NOP();
    } 
    else
    {
        __SEV();
        __WFE();
        __WFE();
    }

    /* enable interrupts */
    core_util_critical_section_exit();
}

/*!
 * @brief     Send the microcontroller to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and high frequency clocks.
 * All state is still maintained.
 *
 * The processor can only be woken up by low power ticker, RTC, an external interrupt on a pin or a watchdog timer.
 *
 * The wake-up time shall be less than 10 ms.
 *
 * @param     None
 *
 * @retval    None
 */
void hal_deepsleep(void)
{
    if (serial_busy_state_check() != 0U)
    {
        return ;
    }

    /* disable interrupts */
    core_util_critical_section_enter();

    ticker_timer_data_save();

    /* enter DEEP-SLEEP mode */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
    PMU_EnterSTOPMode(PMU_REGULATOR_ON, PMU_STOP_ENTRY_WFI);

    mbed_sdk_inited = 0;

    /* reconfigure the PLL and set the system clock to the highest frequence after weak up */
    system_clock_168m_8m_hxtal();

    ticker_timer_data_restore();
    mbed_sdk_inited = 1;

    /* enable interrupts */
    core_util_critical_section_exit();
}

#endif /* DEVICE_SLEEP */
