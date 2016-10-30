/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

#include <stdint.h>
#include <stdbool.h>

#include "system_hi2110.h"
#include "cmsis.h"

/*lint ++flb "Enter library region" */

#define __SYSTEM_CLOCK      (48000000UL)

#if defined ( __CC_ARM )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;  
#elif defined ( __ICCARM__ )
    __root uint32_t SystemCoreClock = __SYSTEM_CLOCK;
#elif defined   ( __GNUC__ )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;
#endif

void SystemCoreClockUpdate(void)
{
    SystemCoreClock = __SYSTEM_CLOCK;
}

/* Restart the core if we're in an interrupt context to ensure a known state.
 * Also reset any PIOs and ensure interrupts are disabled. */
void SystemInit(void)
{
    uint32_t x = __get_xPSR();

    /* Check for interrupt context and reboot needed. */
    if (x & 0x3f) {
        /* Processor is in an interrupt context, reset the core by triggering
         * the reset vector using the SYSRESETREQ bit in the AIRCR register */
        x =  SCB->AIRCR;
        x &= 0xffff;            /* Mask out the top 16 bits */
        x |= 0x05fa0000;        /* Must write with this value for the write to take effect */
        x |= 0x04;              /* Set the SYSRESETREQ bit */
        SCB->AIRCR  = x;        /* Reset the core */
    }

    /* Release claim on any pins */
    PIO_FUNC0_BITCLR = 0xFFFFFFFF;
    PIO_FUNC1_BITCLR = 0xFFFFFFFF;
    PIO_FUNC2_BITCLR = 0xFFFFFFFF;
    PIO_FUNC3_BITCLR = 0xFFFFFFFF;
    PIO_FUNC4_BITCLR = 0xFFFFFFFF;

    /* Disable all IRQ interrupts */
    NVIC->ICER[0] = 0xffffffff;

    /* Ensure interrupts are enabled */
    __set_PRIMASK(0);
    
    /* Allow sleep */
    SystemAllowSleep(true);
}

void SystemAllowSleep(bool sleepAllowed)
{
    if (sleepAllowed) {
        /* Set deep sleep, though not on exit */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    } else {
        /* Unset deep sleep */
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    }
}

/*lint --flb "Leave library region" */
