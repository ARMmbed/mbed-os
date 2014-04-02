/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "sleep_api.h"
#include "cmsis.h"
#include "fsl_mcg_hal.h"
#include "fsl_smc_hal.h"

void sleep(void) {
    smc_power_mode_protection_config_t sleep_config = {true};
    smc_hal_config_power_mode_protection(&sleep_config);

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}

void deepsleep(void) {
    mcg_clock_select_t mcg_clock = clock_get_clks();

    smc_power_mode_protection_config_t sleep_config = {true};
    smc_hal_config_power_mode_protection(&sleep_config);
    SMC->PMCTRL = SMC_PMCTRL_STOPM(2);

    //Deep sleep for ARM core:
    SCB->SCR = 1 << SCB_SCR_SLEEPDEEP_Pos;

    __WFI();

    //Switch back to PLL as clock source if needed
    //The interrupt that woke up the device will run at reduced speed
    if (mcg_clock == kMcgClockSelectOut) {
        if (clock_get_plls() == kMcgPllSelectPllcs) {
            while (clock_get_lock0() == kMcgLockUnlocked);
        }
        clock_set_clks(kMcgClockSelectOut);
    }
}
