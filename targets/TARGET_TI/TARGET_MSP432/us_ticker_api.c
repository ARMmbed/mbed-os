/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/* Low-level implementation of the us ticker for MSP432. The Timer32
 * modules can not be used for this purpose, because they do not
 * generate irqs in free-running mode. So the only remaing choice
 * are the Timer A modules, which are only 16 bit, but support irqs
 * when operated in continuous mode. Timer A3 is choosen here, which
 * could be made configurable in the future.
 *
 * See hal/us_ticker_api.h for a detailed description of the following
 * functions.
 */

#if DEVICE_USTICKER

#include <stdint.h>
#include "us_ticker_api.h"
#include "mbed_assert.h"

extern void us_ticker_irq_handler(void);

void us_ticker_init(void)
{
    // Calculate the Timer A divisors (shift and remainder)
    uint32_t prescale = SubsystemMasterClock / 1000000;
    int  shifts = 0;
    for (shifts = 0; shifts < 3; shifts++) {
        if (prescale & 0x1) {
            break;
        } else {
            prescale >>= 1;
        }
    }

    // Check that we have at least a 1MHz clock
    MBED_ASSERT(prescale && (prescale < 9));

    // initialize the registers and set prescalers
    TIMER_A3->CTL     = (shifts << TIMER_A_CTL_ID_OFS) |
                        TIMER_A_CTL_SSEL__SMCLK |
                        TIMER_A_CTL_MC__CONTINUOUS;
    TIMER_A3->CCTL[0] = 0;
    TIMER_A3->EX0     = prescale - 1;

    // enable IRQ in NVIC
    NVIC_EnableIRQ(TA3_0_IRQn);
}

void us_ticker_free(void)
{
    // Reset the ctrl registers to default value
    TIMER_A3->CTL     = 0;
    TIMER_A3->CCTL[0] = 0;

    // Disable timer interrupt
    NVIC_DisableIRQ(TA3_0_IRQn);
}

uint32_t us_ticker_read(void)
{
    return TIMER_A3->R;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    TIMER_A3->CCR [0] = timestamp;
    TIMER_A3->CCTL[0] = TIMER_A_CCTLN_CCIE;
}

void us_ticker_disable_interrupt(void)
{
    TIMER_A3->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
}

void us_ticker_clear_interrupt(void)
{
    TIMER_A3->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(TA3_0_IRQn);
}

const ticker_info_t *us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        1000000,    // 1 MHz
        16          // 16 bit counter
    };
    return &info;
}

/* Timer_A3 interrupt handler */
void TA3_0_IRQHandler()
{
    us_ticker_irq_handler();
}

#endif /* DEVICE_USTICKER */
