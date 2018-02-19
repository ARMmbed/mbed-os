/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
#include "sleep_api.h"
#include "mbed_assert.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"

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

static void tmr0_vec(void);
static void tmr1_vec(void);
/* Configure alarm exactly after scheduled clocks */
static void arm_alarm(uint32_t cd_clk);

static int ticker_inited = 0;
static uint32_t ticker_last_read_clk = 0;

/* NOTE: TIMER_0 for normal counting and TIMER_1 for scheduled alarm. */
static const struct nu_modinit_s timer0hires_modinit = {TIMER_0, TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK, 0, TMR0_RST, TMR0_IRQn, (void *) tmr0_vec};
static const struct nu_modinit_s timer1hires_modinit = {TIMER_1, TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_PCLK, 0, TMR1_RST, TMR1_IRQn, (void *) tmr1_vec};

#define TMR_CMP_MIN         2
#define TMR_CMP_MAX         0xFFFFFFu

void us_ticker_init(void)
{
    if (ticker_inited) {
        return;
    }
    ticker_inited = 1;

    ticker_last_read_clk = 0;

    // Reset IP
    SYS_ResetModule(timer0hires_modinit.rsetidx);
    SYS_ResetModule(timer1hires_modinit.rsetidx);

    // Select IP clock source
    CLK_SetModuleClock(timer0hires_modinit.clkidx, timer0hires_modinit.clksrc, timer0hires_modinit.clkdiv);
    CLK_SetModuleClock(timer1hires_modinit.clkidx, timer1hires_modinit.clksrc, timer1hires_modinit.clkdiv);
    // Enable IP clock
    CLK_EnableModuleClock(timer0hires_modinit.clkidx);
    CLK_EnableModuleClock(timer1hires_modinit.clkidx);

    // Timer for normal counter
    uint32_t clk_timer0 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
    uint32_t prescale_timer0 = clk_timer0 / NU_TMRCLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer0 != (uint32_t) -1) && prescale_timer0 <= 127);
    MBED_ASSERT((clk_timer0 % NU_TMRCLK_PER_SEC) == 0);
    uint32_t cmp_timer0 = TMR_CMP_MAX;
    MBED_ASSERT(cmp_timer0 >= TMR_CMP_MIN && cmp_timer0 <= TMR_CMP_MAX);
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->CTL = TIMER_PERIODIC_MODE | prescale_timer0 | TIMER_CTL_CNTDATEN_Msk;
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->CMP = cmp_timer0;

    NVIC_SetVector(timer0hires_modinit.irq_n, (uint32_t) timer0hires_modinit.var);
    NVIC_SetVector(timer1hires_modinit.irq_n, (uint32_t) timer1hires_modinit.var);

    NVIC_EnableIRQ(timer0hires_modinit.irq_n);
    NVIC_EnableIRQ(timer1hires_modinit.irq_n);

    TIMER_EnableInt((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
    TIMER_Start((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
}

uint32_t us_ticker_read()
{
    if (! ticker_inited) {
        us_ticker_init();
    }

    TIMER_T * timer0_base = (TIMER_T *) NU_MODBASE(timer0hires_modinit.modname);

    ticker_last_read_clk = TIMER_GetCounter(timer0_base);
    return  (ticker_last_read_clk / NU_TMRCLK_PER_TICK);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    TIMER_Stop((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));

    /* We need to get alarm interval from alarm timestamp `timestamp` to configure H/W timer.
     *  
     * Because both `timestamp` and xx_ticker_read() would wrap around, we have difficulties in distinguishing
     * long future event and past event. To distinguish them, we need `tick_last_read` against which 
     * `timestamp` is calculated out. In timeline, we would always have below after fixing wrap-around:
     * (1) tick_last_read <= present_clk
     * (2) tick_last_read <= alarm_ts_clk
     *
     *
     * 1. Future event case:
     *
     * tick_last_read     present_clk                         alarm_ts_clk
     * |                  |                                   |
     * --------------------------------------------------------
     * |-alarm_intvl1_clk-|
     * |-------------------alarm_intvl2_clk-------------------|
     *
     * 2. Past event case:
     *
     * tick_last_read     alarm_ts_clk                        present_clk
     * |                  |                                   |
     * --------------------------------------------------------
     * |-------------------alarm_intvl1_clk-------------------|
     * |-alarm_intvl2_clk-|
     *
     * Unfortunately, `tick_last_read` is not passed along the xx_ticker_set_interrupt() call. To solve it, we
     * assume that `tick_last_read` tick is exactly the one returned by the last xx_ticker_read() call before
     * xx_ticker_set_interrupt() is invoked. With this assumption, we can hold it via `xx_ticker_last_read_clk`
     * in xx_ticker_read().
     */
    
    /* ticker_last_read_clk will update in us_ticker_read(). Keep it beforehand. */
    uint32_t last_read_clk = ticker_last_read_clk;
    uint32_t present_clk = us_ticker_read() * NU_TMRCLK_PER_TICK;
    uint32_t alarm_ts_clk = timestamp * NU_TMRCLK_PER_TICK;
    uint32_t alarm_intvl1_clk, alarm_intvl2_clk;
    
    /* alarm_intvl1_clk = present_clk - last_read_clk
     *
     * NOTE: Don't miss the `=` sign here. Otherwise, we would get the wrong result.
     */
    if (present_clk >= last_read_clk) {
        alarm_intvl1_clk = present_clk - last_read_clk;
    } else {
        alarm_intvl1_clk = (uint32_t) (((uint64_t) NU_TMR_MAXCNT) + 1 + present_clk - last_read_clk);
    }

    /* alarm_intvl2_clk = alarm_ts_clk - last_read_clk
     * 
     * NOTE: Don't miss the `=` sign here. Otherwise, we would get the wrong result.
     */
    if (alarm_ts_clk >= last_read_clk) {
        alarm_intvl2_clk = alarm_ts_clk - last_read_clk;
    } else {
        alarm_intvl2_clk = (uint32_t) (((uint64_t) NU_TMR_MAXCNT) + 1 + alarm_ts_clk - last_read_clk);
    }

    /* Distinguish (long) future event and past event
     * 
     * NOTE: No '=' sign here. Alarm should go off immediately in equal case.
     */
    if (alarm_intvl2_clk > alarm_intvl1_clk) {
        /* Schedule for future event */
        arm_alarm(alarm_intvl2_clk - alarm_intvl1_clk);
    } else {
        /* Go off immediately for past event, including equal case */
        us_ticker_fire_interrupt();
    }
}

void us_ticker_disable_interrupt(void)
{
    TIMER_DisableInt((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
}

void us_ticker_clear_interrupt(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
}

void us_ticker_fire_interrupt(void)
{
    // NOTE: This event was in the past. Set the interrupt as pending, but don't process it here.
    //       This prevents a recursive loop under heavy load which can lead to a stack overflow.
    NVIC_SetPendingIRQ(timer1hires_modinit.irq_n);
}

static void tmr0_vec(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
}

static void tmr1_vec(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));

    // NOTE: us_ticker_set_interrupt() may get called in us_ticker_irq_handler();
    us_ticker_irq_handler();
}

static void arm_alarm(uint32_t cd_clk)
{
    TIMER_T * timer1_base = (TIMER_T *) NU_MODBASE(timer1hires_modinit.modname);

    // Reset 8-bit PSC counter, 24-bit up counter value and CNTEN bit
    timer1_base->CTL |= TIMER_CTL_RSTCNT_Msk;
    // One-shot mode, Clock = 1 MHz 
    uint32_t clk_timer1 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
    uint32_t prescale_timer1 = clk_timer1 / NU_TMRCLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer1 != (uint32_t) -1) && prescale_timer1 <= 127);
    MBED_ASSERT((clk_timer1 % NU_TMRCLK_PER_SEC) == 0);
    timer1_base->CTL &= ~(TIMER_CTL_OPMODE_Msk | TIMER_CTL_PSC_Msk | TIMER_CTL_CNTDATEN_Msk);
    timer1_base->CTL |= TIMER_ONESHOT_MODE | prescale_timer1 | TIMER_CTL_CNTDATEN_Msk;

    /* NOTE: Because H/W timer requests min compare value, our implementation would have alarm delay of 
     *       (TMR_CMP_MIN - interval_clk) clocks when interval_clk is between [1, TMR_CMP_MIN). */
    uint32_t cmp_timer1 = cd_clk;
    cmp_timer1 = NU_CLAMP(cmp_timer1, TMR_CMP_MIN, TMR_CMP_MAX);
    timer1_base->CMP = cmp_timer1;

    TIMER_EnableInt(timer1_base);
    TIMER_Start(timer1_base);
}

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        NU_TMRCLK_PER_SEC / NU_TMRCLK_PER_TICK,
        NU_TMR_MAXCNT_BITSIZE
    };
    return &info;
}
