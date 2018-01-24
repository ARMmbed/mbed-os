/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

/* NOTE: Don't add static modifier here. These IRQ handler symbols are for linking. 
         Vector table relocation is not actually supported for low-resource target. */
void TMR2_IRQHandler(void);
void TMR3_IRQHandler(void);
/* Configure scheduled alarm */
static void arm_alarm(uint32_t cd_clk);

static uint32_t ticker_last_read_clk = 0;
static int ticker_inited = 0;

/* NOTE: To wake the system from power down mode, timer clock source must be ether LXT or LIRC. */
/* NOTE: TIMER_2 for normal counting and TIMER_3 for scheduled alarm */
static const struct nu_modinit_s timer2_modinit = {TIMER_2, TMR2_MODULE, CLK_CLKSEL2_TMR2_S_LXT, 0, TMR2_RST, TMR2_IRQn, (void *) TMR2_IRQHandler};
static const struct nu_modinit_s timer3_modinit = {TIMER_3, TMR3_MODULE, CLK_CLKSEL2_TMR3_S_LXT, 0, TMR3_RST, TMR3_IRQn, (void *) TMR3_IRQHandler};

#define TMR_CMP_MIN         2
#define TMR_CMP_MAX         0xFFFFFFu

void lp_ticker_init(void)
{
    if (ticker_inited) {
        return;
    }
    ticker_inited = 1;

    ticker_last_read_clk = 0;

    // Reset module
    SYS_ResetModule(timer2_modinit.rsetidx);
    SYS_ResetModule(timer3_modinit.rsetidx);

    // Select IP clock source
    CLK_SetModuleClock(timer2_modinit.clkidx, timer2_modinit.clksrc, timer2_modinit.clkdiv);
    CLK_SetModuleClock(timer3_modinit.clkidx, timer3_modinit.clksrc, timer3_modinit.clkdiv);
    // Enable IP clock
    CLK_EnableModuleClock(timer2_modinit.clkidx);
    CLK_EnableModuleClock(timer3_modinit.clkidx);

    // Configure clock
    uint32_t clk_timer2 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    uint32_t prescale_timer2 = clk_timer2 / NU_TMRCLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer2 != (uint32_t) -1) && prescale_timer2 <= 127);
    MBED_ASSERT((clk_timer2 % NU_TMRCLK_PER_SEC) == 0);
    uint32_t cmp_timer2 = TMR_CMP_MAX;
    MBED_ASSERT(cmp_timer2 >= TMR_CMP_MIN && cmp_timer2 <= TMR_CMP_MAX);
    // Continuous mode
    ((TIMER_T *) NU_MODBASE(timer2_modinit.modname))->CTL = TIMER_PERIODIC_MODE;
    ((TIMER_T *) NU_MODBASE(timer2_modinit.modname))->PRECNT = prescale_timer2;
    ((TIMER_T *) NU_MODBASE(timer2_modinit.modname))->CMPR = cmp_timer2;

    // Set vector
    NVIC_SetVector(timer2_modinit.irq_n, (uint32_t) timer2_modinit.var);
    NVIC_SetVector(timer3_modinit.irq_n, (uint32_t) timer3_modinit.var);

    NVIC_EnableIRQ(timer2_modinit.irq_n);
    NVIC_EnableIRQ(timer3_modinit.irq_n);

    TIMER_EnableInt((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    TIMER_EnableWakeup((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    /* NOTE: When engine is clocked by low power clock source (LXT/LIRC), we need to wait for 3 engine clocks. */
    wait_us((NU_US_PER_SEC / NU_TMRCLK_PER_SEC) * 3);
    TIMER_Start((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
}

timestamp_t lp_ticker_read()
{
    if (! ticker_inited) {
        lp_ticker_init();
    }

    TIMER_T * timer2_base = (TIMER_T *) NU_MODBASE(timer2_modinit.modname);

    ticker_last_read_clk = TIMER_GetCounter(timer2_base);
    return  (ticker_last_read_clk / NU_TMRCLK_PER_TICK);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    TIMER_Stop((TIMER_T *) NU_MODBASE(timer3_modinit.modname));

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
     
    /* ticker_last_read_clk will update in lp_ticker_read(). Keep it beforehand. */
    uint32_t last_read_clk = ticker_last_read_clk;
    uint32_t present_clk = lp_ticker_read() * NU_TMRCLK_PER_TICK;
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
        lp_ticker_fire_interrupt();
    }
}

void lp_ticker_disable_interrupt(void)
{
    TIMER_DisableInt((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
}

void lp_ticker_clear_interrupt(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
}

void lp_ticker_fire_interrupt(void)
{
    // NOTE: This event was in the past. Set the interrupt as pending, but don't process it here.
    //       This prevents a recursive loop under heavy load which can lead to a stack overflow.
    NVIC_SetPendingIRQ(timer3_modinit.irq_n);
}
void TMR2_IRQHandler(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    TIMER_ClearWakeupFlag((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
}

void TMR3_IRQHandler(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    TIMER_ClearWakeupFlag((TIMER_T *) NU_MODBASE(timer3_modinit.modname));

    // NOTE: lp_ticker_set_interrupt() may get called in lp_ticker_irq_handler();
    lp_ticker_irq_handler();
    
}

static void arm_alarm(uint32_t cd_clk)
{
    TIMER_T * timer3_base = (TIMER_T *) NU_MODBASE(timer3_modinit.modname);
    
    // Reset Timer's pre-scale counter, internal 24-bit up-counter and TMR_CTL [TMR_EN] bit
    timer3_base->CTL |= TIMER_CTL_SW_RST_Msk;
    // One-shot mode, Clock = 1 KHz 
    uint32_t clk_timer3 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    uint32_t prescale_timer3 = clk_timer3 / NU_TMRCLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer3 != (uint32_t) -1) && prescale_timer3 <= 127);
    MBED_ASSERT((clk_timer3 % NU_TMRCLK_PER_SEC) == 0);
    timer3_base->CTL &= ~TIMER_CTL_MODE_SEL_Msk;
    timer3_base->CTL |= TIMER_ONESHOT_MODE;
    timer3_base->PRECNT = prescale_timer3;

    /* NOTE: Because H/W timer requests min compare value, our implementation would have alarm delay of 
     *       (TMR_CMP_MIN - interval_clk) clocks when interval_clk is between [1, TMR_CMP_MIN). */
    uint32_t cmp_timer3 = cd_clk;
    cmp_timer3 = NU_CLAMP(cmp_timer3, TMR_CMP_MIN, TMR_CMP_MAX);
    timer3_base->CMPR = cmp_timer3;
    
    TIMER_EnableInt(timer3_base);
    TIMER_EnableWakeup((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    /* NOTE: When engine is clocked by low power clock source (LXT/LIRC), we need to wait for 3 engine clocks. */
    wait_us((NU_US_PER_SEC / NU_TMRCLK_PER_SEC) * 3);
    TIMER_Start(timer3_base);
}

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        NU_TMRCLK_PER_SEC / NU_TMRCLK_PER_TICK,
        NU_TMR_MAXCNT_BITSIZE
    };
    return &info;
}

#endif
