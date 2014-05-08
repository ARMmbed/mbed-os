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
#include "mbed_interface.h"

void sleep(void) {
    // PCON[PD] set to sleep
    LPC_PMU->PCON = 0x0;
    
    // SRC[SLEEPDEEP] set to 0 = sleep
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    
    // wait for interrupt
    __WFI();
}

void deepsleep(void) {
    // PCON[PD] set to powerdown
    LPC_PMU->PCON = 0x2;
    
    // SRC[SLEEPDEEP] set to 1 = deep sleep
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    //If brownout detection and WDT are enabled, keep them enabled during sleep
    LPC_SYSCON->PDSLEEPCFG = LPC_SYSCON->PDRUNCFG | (~((1 << 3) | (1 << 6)));
    
    //After wakeup same stuff as currently enabled:
    LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;
    
    // wait for interrupt
    __WFI();
}
