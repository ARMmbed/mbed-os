/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
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

#include "us_ticker_api.h"

#if DEVICE_USTICKER

#include "sleep_api.h"
#include "mbed_assert.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"

/* We have the following policy for configuring security state of TIMER for us_ticer/lp_ticker:
 *
 * TIMER0: Hard-wired to secure for TF-M SPE use
 * TIMER1: Hard-wired to secure for TF-M SPE use
 * TIMER2: Configured to non-secure for NSPE use, including TF-M NSPE test
 * TIMER3: Configured to non-secure for NSPE use
 * TIMER4: Configured to non-secure for Mbed NSPE us_ticer exclusively
 * TIMER5: Configured to non-secure for Mbed NSPE lp_ticer exclusively
 */

/* Micro seconds per second */
#define NU_US_PER_SEC               1000000
/* Timer clock per us_ticker tick */
#define NU_TMRCLK_PER_TICK          1
/* Timer clock per second */
#define NU_TMRCLK_PER_SEC           (1000 * 1000)
/* Timer max counter bit size */
#define NU_TMR_MAXCNT_BITSIZE       24
/* Timer max counter */
#define NU_TMR_MAXCNT               ((1 << NU_TMR_MAXCNT_BITSIZE) - 1)

static void tmr4_vec(void);

static const struct nu_modinit_s timer4_modinit = {TIMER_4, TMR4_MODULE, CLK_CLKSEL3_TMR4SEL_PCLK0, 0, TMR4_RST, TMR4_IRQn, (void *) tmr4_vec};

#define TIMER_MODINIT       timer4_modinit

/* Track ticker status */
static volatile uint16_t ticker_inited = 0;

#define TMR_CMP_MIN         2
#define TMR_CMP_MAX         0xFFFFFFu

void us_ticker_init(void)
{
    if (ticker_inited) {
        /* By HAL spec, ticker_init allows the ticker to keep counting and disables the
         * ticker interrupt. */
        us_ticker_disable_interrupt();
        return;
    }
    ticker_inited = 1;

    /* Select IP clock source
     *
     * NOTE: We must call secure version (from non-secure domain) because SYS/CLK regions are secure.
     */
    CLK_SetModuleClock_S(TIMER_MODINIT.clkidx, TIMER_MODINIT.clksrc, TIMER_MODINIT.clkdiv);

    /* Enable IP clock
     *
     * NOTE: We must call secure version (from non-secure domain) because SYS/CLK regions are secure.
     */
    CLK_EnableModuleClock_S(TIMER_MODINIT.clkidx);

    /* Reset module
     *
     * NOTE: We must call secure version (from non-secure domain) because SYS/CLK regions are secure.
     */
    SYS_ResetModule_S(TIMER_MODINIT.rsetidx);

    TIMER_T *timer_base = (TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname);

    // Timer for normal counter
    uint32_t clk_timer = TIMER_GetModuleClock(timer_base);
    uint32_t prescale_timer = clk_timer / NU_TMRCLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer != (uint32_t) -1) && prescale_timer <= 127);
    MBED_ASSERT((clk_timer % NU_TMRCLK_PER_SEC) == 0);
    uint32_t cmp_timer = TMR_CMP_MAX;
    MBED_ASSERT(cmp_timer >= TMR_CMP_MIN && cmp_timer <= TMR_CMP_MAX);
    // NOTE: TIMER_CTL_CNTDATEN_Msk exists in NUC472, but not in M451/M480/M2351. In M451/M480/M2351, TIMER_CNT is updated continuously by default.
    timer_base->CTL = TIMER_CONTINUOUS_MODE | prescale_timer/* | TIMER_CTL_CNTDATEN_Msk*/;
    timer_base->CMP = cmp_timer;

    NVIC_SetVector(TIMER_MODINIT.irq_n, (uint32_t) TIMER_MODINIT.var);

    NVIC_DisableIRQ(TIMER_MODINIT.irq_n);

    TIMER_EnableInt(timer_base);

    TIMER_Start(timer_base);
    /* Wait for timer to start counting and raise active flag */
    while(! (timer_base->CTL & TIMER_CTL_ACTSTS_Msk));
}

void us_ticker_free(void)
{
    /* Disable interrupt */
    NVIC_DisableIRQ(TIMER_MODINIT.irq_n);

    /* Disable IP clock
     *
     * NOTE: We must call secure version (from non-secure domain) because SYS/CLK regions are secure.
     */
    CLK_DisableModuleClock_S(TIMER_MODINIT.clkidx);

    ticker_inited = 0;
}

uint32_t us_ticker_read()
{
    if (! ticker_inited) {
        us_ticker_init();
    }

    TIMER_T *timer_base = (TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname);

    return  (TIMER_GetCounter(timer_base) / NU_TMRCLK_PER_TICK);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    /* Clear any previously pending interrupts */
    us_ticker_clear_interrupt();
    NVIC_ClearPendingIRQ(TIMER_MODINIT.irq_n);

    /* In continuous mode, counter will be reset to zero with the following sequence: 
     * 1. Stop counting
     * 2. Configure new CMP value
     * 3. Restart counting
     *
     * This behavior is not what we want. To fix it, we could configure new CMP value
     * without stopping counting first.
     */
    TIMER_T *timer_base = (TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname);

    /* NOTE: Because H/W timer requests min compare value, our implementation would have alarm delay of 
     *       (TMR_CMP_MIN - interval_clk) clocks when interval_clk is between [1, TMR_CMP_MIN). */
    uint32_t cmp_timer = timestamp * NU_TMRCLK_PER_TICK;
    cmp_timer = NU_CLAMP(cmp_timer, TMR_CMP_MIN, TMR_CMP_MAX);
    timer_base->CMP = cmp_timer;

    /* We can call ticker_irq_handler now. */
    NVIC_EnableIRQ(TIMER_MODINIT.irq_n);
}

void us_ticker_disable_interrupt(void)
{
    /* We cannot call ticker_irq_handler now. */
    NVIC_DisableIRQ(TIMER_MODINIT.irq_n);
}

void us_ticker_clear_interrupt(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname));
}

void us_ticker_fire_interrupt(void)
{
    // NOTE: This event was in the past. Set the interrupt as pending, but don't process it here.
    //       This prevents a recursive loop under heavy load which can lead to a stack overflow.
    NVIC_SetPendingIRQ(TIMER_MODINIT.irq_n);

    /* We can call ticker_irq_handler now. */
    NVIC_EnableIRQ(TIMER_MODINIT.irq_n);
}

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        NU_TMRCLK_PER_SEC / NU_TMRCLK_PER_TICK,
        NU_TMR_MAXCNT_BITSIZE
    };
    return &info;
}

static void tmr4_vec(void)
{
    us_ticker_clear_interrupt();

    // NOTE: us_ticker_set_interrupt() may get called in us_ticker_irq_handler();
    us_ticker_irq_handler();
}

#endif
