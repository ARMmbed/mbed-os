/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 Nuvoton
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

#include "lp_ticker_api.h"

#if DEVICE_LOWPOWERTIMER

#include "sleep_api.h"
#include "mbed_wait_api.h"
#include "mbed_assert.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "partition_M2351.h"

/* We have the following policy for configuring security state of TIMER for us_ticer/lp_ticker:
 *
 * TIMER0: Hard-wired to secure and used for secure us_ticer
 * TIMER1: Hard-wired to secure and used for secure lp_ticer
 * TIMER2: Configured to non-secure and used for non-secure us_ticer
 * TIMER3: Configured to non-secure and used for non-secure lp_ticer
 */
#if (! defined(SCU_INIT_PNSSET2_VAL)) || (! (SCU_INIT_PNSSET2_VAL & (1 << 17)))
#error("TIMER2/3 must be configured to non-secure for non-secure us_ticker/lp_ticker.")
#endif

/* Micro seconds per second */
#define NU_US_PER_SEC               1000000
/* Timer clock per lp_ticker tick */
#define NU_TMRCLK_PER_TICK          1
/* Timer clock per second */
#define NU_TMRCLK_PER_SEC           (__LXT)
/* Timer max counter bit size */
#define NU_TMR_MAXCNT_BITSIZE       24
/* Timer max counter */
#define NU_TMR_MAXCNT               ((1 << NU_TMR_MAXCNT_BITSIZE) - 1)

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

static void tmr1_vec(void);

/* NOTE: To wake the system from power down mode, timer clock source must be ether LXT or LIRC. */
static const struct nu_modinit_s timer1_modinit = {TIMER_1, TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_LXT, 0, TMR1_RST, TMR1_IRQn, (void *) tmr1_vec};

#define TIMER_MODINIT       timer1_modinit

#else

static void tmr3_vec(void);

/* NOTE: To wake the system from power down mode, timer clock source must be ether LXT or LIRC. */
static const struct nu_modinit_s timer3_modinit = {TIMER_3, TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_LXT, 0, TMR3_RST, TMR3_IRQn, (void *) tmr3_vec};

#define TIMER_MODINIT       timer3_modinit

#endif

static int ticker_inited = 0;

#define TMR_CMP_MIN         2
#define TMR_CMP_MAX         0xFFFFFFu

void lp_ticker_init(void)
{
    if (ticker_inited) {
        return;
    }
    ticker_inited = 1;

    /* Reset module
     *
     * NOTE: We must call secure version (from non-secure domain) because SYS/CLK regions are secure.
     */
    SYS_ResetModule_S(TIMER_MODINIT.rsetidx);

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

    TIMER_T *timer_base = (TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname);

    // Configure clock
    uint32_t clk_timer = TIMER_GetModuleClock(timer_base);
    uint32_t prescale_timer = clk_timer / NU_TMRCLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer != (uint32_t) -1) && prescale_timer <= 127);
    MBED_ASSERT((clk_timer % NU_TMRCLK_PER_SEC) == 0);
    uint32_t cmp_timer = TMR_CMP_MAX;
    MBED_ASSERT(cmp_timer >= TMR_CMP_MIN && cmp_timer <= TMR_CMP_MAX);
    // Continuous mode
    // NOTE: TIMER_CTL_CNTDATEN_Msk exists in NUC472, but not in M451/M480/M2351. In M451/M480/M2351, TIMER_CNT is updated continuously by default.
    timer_base->CTL = TIMER_CONTINUOUS_MODE | prescale_timer/* | TIMER_CTL_CNTDATEN_Msk*/;
    timer_base->CMP = cmp_timer;

    // Set vector
    NVIC_SetVector(TIMER_MODINIT.irq_n, (uint32_t) TIMER_MODINIT.var);

    NVIC_EnableIRQ(TIMER_MODINIT.irq_n);

    TIMER_EnableInt(timer_base);
    TIMER_EnableWakeup(timer_base);

    /* NOTE: When system clock is higher than timer clock, we need to add 3 engine clock
     *       (recommended by designer) delay to wait for above timer control to take effect. */

    /* Add delay to wait for above timer control to take effect before enabling timer counting */
    wait_us((NU_US_PER_SEC / NU_TMRCLK_PER_SEC) * 3);
    TIMER_Start(timer_base);

    /* Add delay to wait for timer to start counting and raise active flag
     * 
     * It is possible timer active bit cannot be set in time while we check it, and the while loop
     * below would return immediately. */
    wait_us((NU_US_PER_SEC / NU_TMRCLK_PER_SEC) * 3);
    while(! (timer_base->CTL & TIMER_CTL_ACTSTS_Msk));
}

timestamp_t lp_ticker_read()
{
    if (! ticker_inited) {
        lp_ticker_init();
    }

    TIMER_T *timer_base = (TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname);

    return  (TIMER_GetCounter(timer_base) / NU_TMRCLK_PER_TICK);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
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
}

void lp_ticker_disable_interrupt(void)
{
    TIMER_DisableInt((TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname));
}

void lp_ticker_clear_interrupt(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname));
}

void lp_ticker_fire_interrupt(void)
{
    // NOTE: This event was in the past. Set the interrupt as pending, but don't process it here.
    //       This prevents a recursive loop under heavy load which can lead to a stack overflow.
    NVIC_SetPendingIRQ(TIMER_MODINIT.irq_n);
}

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        NU_TMRCLK_PER_SEC / NU_TMRCLK_PER_TICK,
        NU_TMR_MAXCNT_BITSIZE
    };
    return &info;
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
static void tmr1_vec(void)
#else
static void tmr3_vec(void)
#endif
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname));
    TIMER_ClearWakeupFlag((TIMER_T *) NU_MODBASE(TIMER_MODINIT.modname));
    
    // NOTE: lp_ticker_set_interrupt() may get called in lp_ticker_irq_handler();
    lp_ticker_irq_handler();
}

#endif
