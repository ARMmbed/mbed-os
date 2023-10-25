/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2023, MultiTech Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of MultiTech nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "max32670.h"
#include "gcr_regs.h"
#include "mxc_sys.h"
#include "pwrseq_regs.h"

#define XDOT_ERFO_FREQ      24000000    // Change to 24000000 for xDot 1.5 Rev A

void SystemCoreClockUpdateXdot(void)
{
    uint32_t base_freq, div, clk_src;

    // Get the clock source and frequency
    clk_src = (MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_SYSCLK_SEL);
    switch (clk_src)
    {
        case MXC_S_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK:
            base_freq = EXTCLK_FREQ;
            break;
        case MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERFO:
            base_freq = XDOT_ERFO_FREQ;
            break;
        case MXC_S_GCR_CLKCTRL_SYSCLK_SEL_INRO:
            base_freq = INRO_FREQ;
            break;
        case MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IPO:
        base_freq = IPO_FREQ;
            break;
        case MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IBRO:
        base_freq = IBRO_FREQ;
            break;
        case MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERTCO:
            base_freq = ERTCO_FREQ;
            break;
        default:
            // Codes 001 and 111 are reserved.
            // This code should never execute, however, initialize to safe value.
            base_freq = HIRC_FREQ;
            break;
    }
    // Get the clock divider
    if (clk_src == MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IPO)
    {
        base_freq = base_freq >> ((MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_IPO_DIV)>> MXC_F_GCR_CLKCTRL_IPO_DIV_POS);
    }
    div = (MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_SYSCLK_DIV) >> MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS;

    SystemCoreClock = base_freq >> div;
}

int PreInit(void)
{
    return 0;
}

void SystemInit(void)
{
    /* Make sure interrupts are enabled. */
    __enable_irq();

#if (__FPU_PRESENT == 1)
    /* Enable FPU on Cortex-M4, which occupies coprocessor slots 10 & 11 */
    /* Grant full access, per "Table B3-24 CPACR bit assignments". */
    /* DDI0403D "ARMv7-M Architecture Reference Manual" */
    SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
    __DSB();
    __ISB();
#endif

    MXC_PWRSEQ->lpcn &= ~(1 << 31); // Ensure ERTCO is on

    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdateXdot();

    // Increase drive strength of I2C_SE bus and Mem Pwr En.
    // Note: Mem Pwr En doesn't help, higher drive strength on se i2c pins seems to though
    MXC_GPIO0->ds0 |= (1 << 6) | (1 << 7) | (1 << 24);
    MXC_GPIO0->ds1 |= (1 << 6) | (1 << 7) | (1 << 24);

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO0); 
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO1); 
}
