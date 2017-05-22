/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
#include "rtc.h"
#include "lp.h"

#define PRESCALE_VAL    RTC_PRESCALE_DIV_2_0    // Set the divider for the 4kHz clock
#define SHIFT_AMT       (RTC_PRESCALE_DIV_2_12 - PRESCALE_VAL)

#define WINDOW          1000

static int rtc_inited = 0;
static volatile uint32_t overflow_cnt = 0;

static uint64_t rtc_read64(void);

//******************************************************************************
static void overflow_handler(void)
{
    overflow_cnt++;
    RTC_ClearFlags(MXC_F_RTC_FLAGS_ASYNC_CLR_FLAGS);
}

//******************************************************************************
void rtc_init(void)
{
    if (rtc_inited) {
        return;
    }
    rtc_inited = 1;

    overflow_cnt = 0;

    /* Enable power for RTC for all LPx states */
    MXC_PWRSEQ->reg0 |= (MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN |
                         MXC_F_PWRSEQ_REG0_PWR_RTCEN_SLP);

    /* Enable clock to synchronizers */
    CLKMAN_SetClkScale(CLKMAN_CLK_SYNC, CLKMAN_SCALE_DIV_1);

    // Prepare interrupt handlers
    NVIC_SetVector(RTC0_IRQn, (uint32_t)lp_ticker_irq_handler);
    NVIC_EnableIRQ(RTC0_IRQn);
    NVIC_SetVector(RTC3_IRQn, (uint32_t)overflow_handler);
    NVIC_EnableIRQ(RTC3_IRQn);

    // Enable wakeup on RTC rollover
    LP_ConfigRTCWakeUp(0, 0, 0, 1);

    /* RTC registers are only reset on a power cycle. Do not reconfigure the RTC
     * if it is already running.
     */
    if (!RTC_IsActive()) {
        rtc_cfg_t cfg = {0};
        cfg.prescaler = PRESCALE_VAL;
        cfg.snoozeMode = RTC_SNOOZE_DISABLE;

        int retval = RTC_Init(&cfg);
        MBED_ASSERT(retval == E_NO_ERROR);

        RTC_EnableINT(MXC_F_RTC_FLAGS_OVERFLOW);
        RTC_Start();
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
    if (RTC_IsActive()) {
        // Clear and disable RTC
        MXC_RTCTMR->ctrl |= MXC_F_RTC_CTRL_CLEAR;
        RTC_Stop();
    }
}

//******************************************************************************
int rtc_isenabled(void)
{
    return RTC_IsActive();
}

//******************************************************************************
time_t rtc_read(void)
{
    uint32_t ovf_cnt_1, ovf_cnt_2, timer_cnt;
    uint32_t ovf1, ovf2;

    // Make sure RTC is setup before trying to read
    if (!rtc_inited) {
        rtc_init();
    }

    // Ensure coherency between overflow_cnt and timer
    do {
        ovf_cnt_1 = overflow_cnt;
        ovf1 = RTC_GetFlags() & MXC_F_RTC_FLAGS_OVERFLOW;
        timer_cnt = RTC_GetCount();
        ovf2 = RTC_GetFlags() & MXC_F_RTC_FLAGS_OVERFLOW;
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

    // Make sure RTC is setup before trying to read
    if (!rtc_inited) {
        rtc_init();
    }

    // Ensure coherency between overflow_cnt and timer
    do {
        ovf_cnt_1 = overflow_cnt;
        ovf1 = RTC_GetFlags() & MXC_F_RTC_FLAGS_OVERFLOW;
        timer_cnt = RTC_GetCount();
        ovf2 = RTC_GetFlags() & MXC_F_RTC_FLAGS_OVERFLOW;
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
    // Make sure RTC is setup before accessing
    if (!rtc_inited) {
        rtc_init();
    }

    RTC_Stop();
    RTC_SetCount(t << SHIFT_AMT);
    overflow_cnt = t >> (32 - SHIFT_AMT);
    RTC_Start();
}

//******************************************************************************
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t comp_value;
    uint64_t curr_ts64;
    uint64_t ts64;

    // Note: interrupts are disabled before this function is called.

    // Disable the alarm while it is prepared
    RTC_DisableINT(MXC_F_RTC_INTEN_COMP0);

    curr_ts64 = rtc_read64();
    ts64 = (uint64_t)timestamp | (curr_ts64 & 0xFFFFFFFF00000000ULL);

    // If this event is older than a recent window, it must be in the future
    if ((ts64 < (curr_ts64 - WINDOW)) && ((curr_ts64 - WINDOW) < curr_ts64)) {
        ts64 += 0x100000000ULL;
    }

    uint32_t timer = RTC_GetCount();
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
    MXC_RTCTMR->flags = MXC_F_RTC_FLAGS_ASYNC_CLR_FLAGS;
    RTC_EnableINT(MXC_F_RTC_INTEN_COMP0);

    // Enable wakeup from RTC
    LP_ConfigRTCWakeUp(1, 0, 0, 1);

    // Wait for pending transactions
    while (MXC_RTCTMR->ctrl & MXC_F_RTC_CTRL_PENDING);
}

//******************************************************************************
inline void lp_ticker_disable_interrupt(void)
{
    RTC_DisableINT(MXC_F_RTC_INTEN_COMP0);
}

//******************************************************************************
inline void lp_ticker_clear_interrupt(void)
{
    RTC_ClearFlags(MXC_F_RTC_FLAGS_ASYNC_CLR_FLAGS);
}

//******************************************************************************
inline uint32_t lp_ticker_read(void)
{
    return rtc_read64();
}
