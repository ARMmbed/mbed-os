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

#ifndef NU_TIMER_H
#define NU_TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis.h"
#include "mbed_power_mgmt.h"
#include "mbed_critical.h"
#include "ticker_api.h"
#include "us_ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A simple count-down timer used for Nuvoton ported drivers
 *
 * NOTE: nu_countdown_init must be paired with nu_countdown_free.
 *
 * Example:
 * nu_countdown_ctx_s ctx;
 * 
 * // Set up 2 ms timeout
 * nu_countdown_init(&ctx, 2000);
 *
 * // Timed-wait for a task
 * while (true) {
 *     // Poll the task
 *
 *     if (nu_countdown_expired(&ctx)) {
 *         // Timeout
 *     }
 * }
 *
 * // Must pair nu_countdown_init with nu_countdown_free in the end
 * nu_countdown_free(&ctx);
 */

struct nu_countdown_ctx_s {
    const ticker_data_t *   _ticker_data;       // Hold ticker_data_t
    us_timestamp_t          _interval_end_us;   // End of interval in us
    bool                    _expired;           // Expired or not
};

__STATIC_INLINE void nu_countdown_init(struct nu_countdown_ctx_s *ctx, us_timestamp_t interval_us)
{
    core_util_critical_section_enter();
    sleep_manager_lock_deep_sleep();
    ctx->_ticker_data = get_us_ticker_data();
    ctx->_interval_end_us = ticker_read_us(ctx->_ticker_data) + interval_us;
    ctx->_expired = false;
    core_util_critical_section_exit();
}

__STATIC_INLINE bool nu_countdown_expired(struct nu_countdown_ctx_s *ctx)
{
    core_util_critical_section_enter();
    if (! ctx->_expired) {
        ctx->_expired = ticker_read_us(ctx->_ticker_data) >= ctx->_interval_end_us;
    }
    core_util_critical_section_exit();
    
    return ctx->_expired;
}

__STATIC_INLINE void nu_countdown_free(struct nu_countdown_ctx_s *ctx)
{
    core_util_critical_section_enter();
    sleep_manager_unlock_deep_sleep();
    core_util_critical_section_exit();
}

#ifdef __cplusplus
}
#endif

#endif
