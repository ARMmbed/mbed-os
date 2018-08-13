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

/* Delay 4 cycles per round by hand-counting instruction cycles
 *
 * The delay function here is implemented by just hand-counting instruction cycles rather than preferred
 * H/W timer since it is to use in cases where H/W timer is not available. Usually, it can delay at least
 * 4-cycles per round.
 *
 * In modern pipeline core, plus flash performance and other factors, we cannot rely accurately on hand-
 * counting instruction cycles for expected delay cycles.
 */
#if defined(__CC_ARM)
MBED_NOINLINE
__asm void nu_delay_cycle_x4(uint32_t rounds)
{
// AStyle should not format inline assembly
// *INDENT-OFF*
1
#if !defined(__CORTEX_M0)
    NOP                         // 1 cycle
#endif
    SUBS a1, a1, #1             // 1 cycle
    BCS  %BT1                   // 3 cycles(M0)/2 cycles(non-M0)
    BX   lr
// *INDENT-ON*
}
#elif defined (__ICCARM__)
MBED_NOINLINE
void nu_delay_cycle_x4(uint32_t rounds)
{
    __asm volatile(
        "loop: \n"
#if !defined(__CORTEX_M0)
        " NOP \n"               // 1 cycle
#endif
        " SUBS %0, %0, #1 \n"   // 1 cycle
        " BCS.n  loop\n"        // 3 cycles(M0)/2 cycles(non-M0)
        : "+r"(rounds)
        :
        : "cc"
    );
}
#elif  defined ( __GNUC__ ) ||  (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
MBED_NOINLINE
void nu_delay_cycle_x4(uint32_t rounds)
{
    __asm__ volatile(
        "%=:\n\t"
#if !defined(__CORTEX_M0)
        "NOP\n\t"               // 1 cycle
#endif
#if defined(__thumb__) && !defined(__thumb2__) && !defined(__ARMCC_VERSION)
        "SUB  %0, #1\n\t"       // 1 cycle
#else
        "SUBS %0, %0, #1\n\t"   // 1 cycle
#endif
        "BCS  %=b\n\t"          // 3 cycles(M0)/2 cycles(non-M0)
        : "+l"(rounds)
        :
        : "cc"
    );
}
#endif
