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

static int us_ticker_inited = 0;
static int ticker_expired = 0;

#define US_TICKER_TIMER_IRQn     MRT_IRQn
#define MRT_CLOCK_MHZ            30

void us_ticker_init(void)
{
    if (us_ticker_inited)
        return;

    us_ticker_inited = 1;

    // Enable the MRT clock
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);

    // Clear peripheral reset the MRT
    LPC_SYSCON->PRESETCTRL |= (1 << 7);

    // Force load interval value
    LPC_MRT->INTVAL0 = 0xFFFFFFFFUL;
    // Enable ch0 interrupt
    LPC_MRT->CTRL0 = 1;

    // Force load interval value
    LPC_MRT->INTVAL1 = 0x80000000UL;
    // Disable ch1 interrupt
    LPC_MRT->CTRL1 = 0;

    // Set MRT interrupt vector
    NVIC_SetVector(US_TICKER_TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited)
        us_ticker_init();

    // Generate ticker value
    // MRT source clock is SystemCoreClock (30MHz) and 31-bit down count timer
    // Calculate expected value using number of expired times
    return (0x7FFFFFFFUL - LPC_MRT->TIMER0)/MRT_CLOCK_MHZ + (ticker_expired * (0x80000000UL/MRT_CLOCK_MHZ));
}


void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // Force load interval value
    LPC_MRT->INTVAL1 = (((timestamp - us_ticker_read()) * MRT_CLOCK_MHZ) | 0x80000000UL);

    // Enable interrupt 
    LPC_MRT->CTRL1 |= 1;
}

void us_ticker_disable_interrupt()
{
    LPC_MRT->CTRL1 &= ~1;
}

void us_ticker_clear_interrupt()
{
    if (LPC_MRT->STAT1 & 1)
        LPC_MRT->STAT1 = 1;
    
    if (LPC_MRT->STAT0 & 1) {
        LPC_MRT->STAT0 = 1;
        ticker_expired++;
    }
}
