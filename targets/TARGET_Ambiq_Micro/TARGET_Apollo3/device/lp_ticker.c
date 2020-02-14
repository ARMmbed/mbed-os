/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include "lp_ticker_defines.h"
#include "platform/mbed_critical.h"

#if DEVICE_LPTICKER

/* LP ticker is driven by 32kHz clock and counter length is 24 bits. */
const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
        LP_TICKER_FREQ,
        LP_TICKER_BITS};
    return &info;
}

void lp_ticker_init(void)
{
    am_hal_ctimer_config_single(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                                LP_TICKER_AM_HAL_CTIMER_SEGMENT,
                                (AM_HAL_CTIMER_FN_CONTINUOUS | LP_TICKER_AM_HAL_CTIMER_SRC | AM_HAL_CTIMER_INT_ENABLE));

    am_hal_ctimer_int_register(LP_TICKER_AM_HAL_CTIMER_CMPR_INT, lp_ticker_irq_handler);
}

void lp_ticker_free(void)
{
    am_hal_ctimer_stop(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                       LP_TICKER_AM_HAL_CTIMER_SEGMENT);

    am_hal_ctimer_clear(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                        LP_TICKER_AM_HAL_CTIMER_SEGMENT);
}

uint32_t lp_ticker_read()
{
    return am_hal_ctimer_read(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                              LP_TICKER_AM_HAL_CTIMER_SEGMENT);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    am_hal_ctimer_compare_set(LP_TICKER_AM_HAL_CTIMER_NUMBER,
                              LP_TICKER_AM_HAL_CTIMER_SEGMENT,
                              LP_TICKER_AM_HAL_CTIMER_CMPR_REG,
                              (uint32_t)timestamp);
}

void lp_ticker_fire_interrupt(void)
{
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
