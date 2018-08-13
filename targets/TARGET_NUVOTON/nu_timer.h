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

void nu_countdown_init(struct nu_countdown_ctx_s *ctx, us_timestamp_t interval_us);
bool nu_countdown_expired(struct nu_countdown_ctx_s *ctx);
void nu_countdown_free(struct nu_countdown_ctx_s *ctx);


/* Replacement for wait_us when intermediary us ticker layer is disabled
 *
 * Use of wait_us directly from the low power ticker causes the system to deadlock during
 * the sleep test because the sleep test disables the intermediary us ticker layer during
 * the test.
 *
 * To prevent this lockup, nu_busy_wait_us is created to replace wait_us, which uses the us ticker
 * directly rather than go though the intermediary us ticker layer.
 *
 * During wait period through nu_busy_wait_us, CPU would be busy spinning. 
 */
void nu_busy_wait_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif
