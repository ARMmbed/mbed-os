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

#include "nu_timer.h"
#include "mbed_power_mgmt.h"
#include "mbed_critical.h"
#include "us_ticker_api.h"
#include "mbed_assert.h"

void nu_countdown_init(struct nu_countdown_ctx_s *ctx, us_timestamp_t interval_us)
{
    core_util_critical_section_enter();
    sleep_manager_lock_deep_sleep();
    ctx->_ticker_data = get_us_ticker_data();
    ctx->_interval_end_us = ticker_read_us(ctx->_ticker_data) + interval_us;
    ctx->_expired = false;
    core_util_critical_section_exit();
}

bool nu_countdown_expired(struct nu_countdown_ctx_s *ctx)
{
    core_util_critical_section_enter();
    if (! ctx->_expired) {
        ctx->_expired = ticker_read_us(ctx->_ticker_data) >= ctx->_interval_end_us;
    }
    core_util_critical_section_exit();
    
    return ctx->_expired;
}

void nu_countdown_free(struct nu_countdown_ctx_s *ctx)
{
    core_util_critical_section_enter();
    sleep_manager_unlock_deep_sleep();
    core_util_critical_section_exit();
}


void nu_busy_wait_us(uint32_t us)
{
    const uint32_t bits = us_ticker_get_info()->bits;
    const uint32_t mask = (1 << bits) - 1;
    MBED_ASSERT(us_ticker_get_info()->frequency == 1000000);
    uint32_t prev = us_ticker_read();
    while (1) {
        const uint32_t cur = us_ticker_read();
        const uint32_t elapsed = (cur - prev) & mask;
        if (elapsed > us) {
            break;
        }
        us -= elapsed;
        prev = cur;
    }
}
