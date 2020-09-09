/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "lp_ticker_api.h"
#include "lp_ticker_defines.h"
#include "platform/mbed_critical.h"

#if DEVICE_LPTICKER

static bool lp_ticker_initialized = false;

/* LP ticker is driven by 32kHz clock and counter length is 24 bits. */
const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
        LP_TICKER_FREQ,
        LP_TICKER_BITS
    };
    return &info;
}

void lp_ticker_init(void)
{
    if (lp_ticker_initialized) {
        lp_ticker_disable_interrupt();
        return;
    }
    am_hal_ctimer_int_register(LP_TICKER_AM_HAL_CTIMER_CMPR_INT, lp_ticker_irq_handler);
    am_hal_ctimer_config_single(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                                LP_TICKER_AM_HAL_CTIMER_SEGMENT_TIME_KEEPER,
                                (LP_TICKER_AM_HAL_CTIMER_TIME_KEEPER_FN | LP_TICKER_AM_HAL_CTIMER_SRC));
    am_hal_ctimer_config_single(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                                LP_TICKER_AM_HAL_CTIMER_SEGMENT_INT_COUNTER,
                                (LP_TICKER_AM_HAL_CTIMER_INT_COUNTER_FN | LP_TICKER_AM_HAL_CTIMER_SRC | AM_HAL_CTIMER_INT_ENABLE));
    am_hal_ctimer_int_enable(LP_TICKER_AM_HAL_CTIMER_CMPR_INT);
    NVIC_EnableIRQ(CTIMER_IRQn);
    am_hal_ctimer_start(LP_TICKER_AM_HAL_CTIMER_NUMBER, LP_TICKER_AM_HAL_CTIMER_SEGMENT_TIME_KEEPER);
    lp_ticker_initialized = true;
}

void lp_ticker_free(void)
{
    am_hal_ctimer_stop(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                       LP_TICKER_AM_HAL_CTIMER_SEGMENT_TIME_KEEPER);

    am_hal_ctimer_clear(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                        LP_TICKER_AM_HAL_CTIMER_SEGMENT_TIME_KEEPER);
    lp_ticker_initialized = false;
}

uint32_t lp_ticker_read()
{
    return am_hal_ctimer_read(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                              LP_TICKER_AM_HAL_CTIMER_SEGMENT_TIME_KEEPER);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    am_hal_ctimer_int_enable(LP_TICKER_AM_HAL_CTIMER_CMPR_INT);
    am_hal_ctimer_clear(LP_TICKER_AM_HAL_CTIMER_NUMBER, LP_TICKER_AM_HAL_CTIMER_SEGMENT_INT_COUNTER);
    // am_hal_ctimer_config_single(LP_TICKER_AM_HAL_CTIMER_NUMBER,
    //                             LP_TICKER_AM_HAL_CTIMER_SEGMENT_INT_COUNTER,
    //                             (LP_TICKER_AM_HAL_CTIMER_INT_COUNTER_FN | LP_TICKER_AM_HAL_CTIMER_SRC | AM_HAL_CTIMER_INT_ENABLE | CTIMER_CTRL0_TMRA0IE1_Msk));
    am_hal_ctimer_start(LP_TICKER_AM_HAL_CTIMER_NUMBER, LP_TICKER_AM_HAL_CTIMER_SEGMENT_INT_COUNTER);
    uint32_t delta = (uint32_t)timestamp - lp_ticker_read();
    am_hal_ctimer_compare_set(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                              LP_TICKER_AM_HAL_CTIMER_SEGMENT_INT_COUNTER,
                              LP_TICKER_AM_HAL_CTIMER_CMPR_REG,
                              (uint32_t)delta);
}

void lp_ticker_fire_interrupt(void)
{
    am_hal_ctimer_int_enable(LP_TICKER_AM_HAL_CTIMER_CMPR_INT);
    am_hal_ctimer_int_set(LP_TICKER_AM_HAL_CTIMER_CMPR_INT);
}

void lp_ticker_disable_interrupt(void)
{
    am_hal_ctimer_int_disable(LP_TICKER_AM_HAL_CTIMER_CMPR_INT);
}

void lp_ticker_clear_interrupt(void)
{
    am_hal_ctimer_int_clear(LP_TICKER_AM_HAL_CTIMER_CMPR_INT);
}

#endif // DEVICE_LPTICKER
