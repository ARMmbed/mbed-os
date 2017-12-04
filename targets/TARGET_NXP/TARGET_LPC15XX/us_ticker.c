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
#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "mbed_critical.h"

#define US_TICKER_TIMER_IRQn     SCT3_IRQn

int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited)
        return;

    us_ticker_inited = 1;

    // Enable the SCT3 clock
    LPC_SYSCON->SYSAHBCLKCTRL1 |= (1 << 5);

    // Clear peripheral reset the SCT3
    LPC_SYSCON->PRESETCTRL1 |= (1 << 5);
    LPC_SYSCON->PRESETCTRL1 &= ~(1 << 5);

    // Configure SCT3 as a 1MHz 32-bit counter with no auto limiting or match reload
    char sctClkDiv = ((SystemCoreClock + 1000000 - 1) / 1000000) - 1;
    LPC_SCT3->CONFIG = (1 << 7) | (1 << 0);
    LPC_SCT3->CTRL = (sctClkDiv << 5) | (1 << 3) | (1 << 2);

    // Configure SCT3 event 0 to fire on match register 0
    LPC_SCT3->EV0_STATE = (1 << 0);
    LPC_SCT3->EV0_CTRL = (0x1 << 12);

    // Start SCT3
    LPC_SCT3->CTRL &= ~(1 << 2);

    // Set SCT3 interrupt vector
    NVIC_SetVector(US_TICKER_TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
}

uint32_t us_ticker_read() {
    if (!us_ticker_inited)
        us_ticker_init();

    // Return SCT3 count value
    return LPC_SCT3->COUNT;
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    // Set SCT3 match register 0 (critical section)
    core_util_critical_section_enter();
    LPC_SCT3->CTRL |= (1 << 2);
    LPC_SCT3->MATCH0 = (uint32_t)timestamp;
    LPC_SCT3->CTRL &= ~(1 << 2);
    core_util_critical_section_exit();

    // Enable interrupt on SCT3 event 0
    LPC_SCT3->EVEN = (1 << 0);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_disable_interrupt(void) {
    // Disable interrupt on SCT3 event 0
    LPC_SCT3->EVEN = 0;
}

void us_ticker_clear_interrupt(void) {
    // Clear SCT3 event 0 interrupt flag
    LPC_SCT3->EVFLAG = (1 << 0);
}
