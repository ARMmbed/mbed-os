/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "sleep_api.h"
#include "xmc_scu.h"

/**************************************************************** Functions **/

void hal_sleep(void)
{

//    // Configure system during SLEEP state
//    XMC_SCU_CLOCK_SetSleepConfig(XMC_SCU_CLOCK_SLEEP_MODE_CONFIG_SYSCLK_FPLL); //TODO: why pll clock is divided by 2
//
//    // Make sure that SLEEPDEEP bit is cleared
//    SCB->SCR &= ~ SCB_SCR_SLEEPONEXIT_Msk;
//
//    // Put system in SLEEP state
//    __WFI();
}

void hal_deepsleep(void)
{

    // Configure system during SLEEP state
//    XMC_SCU_CLOCK_SetDeepSleepConfig(XMC_SCU_CLOCK_SLEEP_MODE_CONFIG_SYSCLK_FOFI |
//            XMC_SCU_CLOCK_DEEPSLEEP_MODE_CONFIG_FLASH_POWERDOWN |
//            XMC_SCU_CLOCK_DEEPSLEEP_MODE_CONFIG_PLL_POWERDOWN);
//
//    // Make sure that SLEEPDEEP bit is set
//    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//
//    // Return to SLEEP mode after handling the wakeup event
//    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
//
//    // Put system in DEEPSLEEP state
//    __WFI();
}

/*EOF*/
