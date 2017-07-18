/*******************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "rtc_api.h"
#include "lp_ticker_api.h"
#include "cmsis.h"
#include "rtc_regs.h"
#include "pwrseq_regs.h"
#include "clkman_regs.h"

#define PRESCALE_VAL    MXC_E_RTC_PRESCALE_DIV_2_0    // Set the divider for the 4kHz clock
#define SHIFT_AMT       (MXC_E_RTC_PRESCALE_DIV_2_12 - PRESCALE_VAL)

#define WINDOW          1000

static int rtc_inited = 0;
static volatile uint32_t overflow_cnt = 0;

static uint64_t rtc_read64(void);

//******************************************************************************
static void overflow_handler(void)
{
    MXC_RTCTMR->flags = MXC_F_RTC_FLAGS_OVERFLOW;
    MXC_PWRSEQ->flags = MXC_F_PWRSEQ_MSK_FLAGS_RTC_ROLLOVER;
    overflow_cnt++;
}

//******************************************************************************
void rtc_init(void)
{
    if (rtc_inited) {
        return;
    }
    rtc_inited = 1;

    overflow_cnt = 0;

    // Enable the clock to the synchronizer
    MXC_CLKMAN->clk_ctrl_13_rtc_int_sync = MXC_E_CLKMAN_CLK_SCALE_ENABLED;

    // Enable the clock to the RTC
    MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;

    // Prepare interrupt handlers
    NVIC_SetVector(RTC0_IRQn, (uint32_t)lp_ticker_irq_handler);
    NVIC_EnableIRQ(RTC0_IRQn);
    NVIC_SetVector(RTC3_IRQn, (uint32_t)overflow_handler);
    NVIC_EnableIRQ(RTC3_IRQn);

    // Enable wakeup on RTC rollover
    MXC_PWRSEQ->msk_flags &= ~MXC_F_PWRSEQ_MSK_FLAGS_RTC_ROLLOVER;

    /* RTC registers are only reset on a power cycle. Do not reconfigure the RTC
     * if it is already running.
     */
    if (!(MXC_RTCTMR->ctrl & MXC_F_RTC_CTRL_ENABLE)) {
        // Set the clock divider
        MXC_RTCTMR->prescale = PRESCALE_VAL;

        // Enable the overflow interrupt
        MXC_RTCTMR->inten |= MXC_F_RTC_FLAGS_OVERFLOW;

        // Restart the timer from 0
        MXC_RTCTMR->timer = 0;

        // Enable the RTC
        MXC_RTCTMR->ctrl |= MXC_F_RTC_CTRL_ENABLE;
    }
}

//******************************************************************************
void lp_ticker_init(void)
{
    rtc_init();
}

//******************************************************************************
void rtc_free(void)
{
    if (MXC_RTCTMR->ctrl & MXC_F_RTC_CTRL_ENABLE) {
        // Clear and disable RTC
        MXC_RTCTMR->ctrl |= MXC_F_RTC_CTRL_CLEAR;
        MXC_RTCTMR->ctrl &= ~MXC_F_RTC_CTRL_ENABLE;

        // Wait for pending transactions
        while(MXC_RTCTMR->ctrl & MXC_F_RTC_CTRL_PENDING);
    }

    // Disable the clock to the RTC
    MXC_PWRSEQ->reg0 &= ~(MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RTCEN_SLP);

    // Disable the clock to the synchronizer
    MXC_CLKMAN->clk_ctrl_13_rtc_int_sync = MXC_E_CLKMAN_CLK_SCALE_DISABLED;
}

//******************************************************************************
int rtc_isenabled(void)
{
    return (MXC_RTCTMR->ctrl & MXC_F_RTC_CTRL_ENABLE);
}

//******************************************************************************
time_t rtc_read(void)
{
    uint32_t ovf_cnt_1, ovf_cnt_2, timer_cnt;
    uint32_t ovf1, ovf2;

    // Ensure coherency between overflow_cnt and timer
    do {
        ovf_cnt_1 = overflow_cnt;
        ovf1 = MXC_RTCTMR->flags & MXC_F_RTC_FLAGS_OVERFLOW;
        timer_cnt = MXC_RTCTMR->timer;
        ovf2 = MXC_RTCTMR->flags & MXC_F_RTC_FLAGS_OVERFLOW;
        ovf_cnt_2 = overflow_cnt;
    } while ((ovf_cnt_1 != ovf_cnt_2) || (ovf1 != ovf2));

    // Account for an unserviced interrupt
    if (ovf1) {
        ovf_cnt_1++;
    }

    return (timer_cnt >> SHIFT_AMT) + (ovf_cnt_1 << (32 - SHIFT_AMT));
}

//******************************************************************************
static uint64_t rtc_read64(void)
{
    uint32_t ovf_cnt_1, ovf_cnt_2, timer_cnt;
    uint32_t ovf1, ovf2;
    uint64_t current_us;

    // Ensure coherency between overflow_cnt and timer
    do {
        ovf_cnt_1 = overflow_cnt;
        ovf1 = MXC_RTCTMR->flags & MXC_F_RTC_FLAGS_OVERFLOW;
        timer_cnt = MXC_RTCTMR->timer;
        ovf2 = MXC_RTCTMR->flags & MXC_F_RTC_FLAGS_OVERFLOW;
        ovf_cnt_2 = overflow_cnt;
    } while ((ovf_cnt_1 != ovf_cnt_2) || (ovf1 != ovf2));

    // Account for an unserviced interrupt
    if (ovf1) {
        ovf_cnt_1++;
    }

    current_us = (((uint64_t)timer_cnt * 1000000) >> SHIFT_AMT) + (((uint64_t)ovf_cnt_1 * 1000000) << (32 - SHIFT_AMT));

    return current_us;
}

//******************************************************************************
void rtc_write(time_t t)
{
    MXC_RTCTMR->ctrl &= ~MXC_F_RTC_CTRL_ENABLE; // disable the timer while updating
    MXC_RTCTMR->timer = t << SHIFT_AMT;
    overflow_cnt = t >> (32 - SHIFT_AMT);
    MXC_RTCTMR->ctrl |= MXC_F_RTC_CTRL_ENABLE;  // enable the timer while updating
}

//******************************************************************************
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t comp_value;
    uint64_t curr_ts64;
    uint64_t ts64;

    // Note: interrupts are disabled before this function is called.

    // Disable the alarm while it is prepared
    MXC_RTCTMR->inten &= ~MXC_F_RTC_INTEN_COMP0;

    curr_ts64 = rtc_read64();
    ts64 = (uint64_t)timestamp | (curr_ts64 & 0xFFFFFFFF00000000ULL);

    // If this event is older than a recent window, it must be in the future
    if ((ts64 < (curr_ts64 - WINDOW)) && ((curr_ts64 - WINDOW) < curr_ts64)) {
        ts64 += 0x100000000ULL;
    }

    uint32_t timer = MXC_RTCTMR->timer;
    if (ts64 <= curr_ts64) {
        // This event has already occurred. Set the alarm to expire immediately.
        comp_value = timer + 1;
    } else {
        comp_value = (ts64 << SHIFT_AMT) / 1000000;
    }

    // Ensure that the compare value is far enough in the future to guarantee the interrupt occurs.
    if ((comp_value < (timer + 2)) && (comp_value > (timer - 10))) {
        comp_value = timer + 2;
    }

    MXC_RTCTMR->comp[0] = comp_value;
    MXC_RTCTMR->flags = MXC_F_RTC_FLAGS_COMP0;  // clear interrupt
    MXC_RTCTMR->inten |= MXC_F_RTC_INTEN_COMP0; // enable the interrupt

    // Enable wakeup from RTC
    MXC_PWRSEQ->msk_flags &= ~MXC_F_PWRSEQ_MSK_FLAGS_RTC_CMPR0;
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(RTC0_IRQn);
}

//******************************************************************************
inline void lp_ticker_disable_interrupt(void)
{
    MXC_RTCTMR->inten &= ~MXC_F_RTC_INTEN_COMP0;
}

//******************************************************************************
inline void lp_ticker_clear_interrupt(void)
{
    MXC_RTCTMR->flags = MXC_F_RTC_FLAGS_ASYNC_CLR_FLAGS;
    MXC_PWRSEQ->flags = MXC_F_PWRSEQ_MSK_FLAGS_RTC_CMPR0;
}

//******************************************************************************
inline uint32_t lp_ticker_read(void)
{
    return rtc_read64();
}
