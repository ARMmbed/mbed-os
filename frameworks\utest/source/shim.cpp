/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

#include "utest/shim.h"

#if UTEST_SHIM_SCHEDULER_USE_MINAR
#include "minar/minar.h"

static int32_t utest_minar_init()
{
    return 0;
}
static void *utest_minar_post(const utest_v1_harness_callback_t callback, const uint32_t delay_ms)
{
    void *handle = minar::Scheduler::postCallback(callback).delay(minar::milliseconds(delay_ms)).getHandle();
    return handle;
}
static int32_t utest_minar_cancel(void *handle)
{
    int32_t ret = minar::Scheduler::cancelCallback(handle);
    return ret;
}
static int32_t utest_minar_run()
{
    return 0;
}
extern "C" {
static const utest_v1_scheduler_t utest_v1_scheduler =
{
    utest_minar_init,
    utest_minar_post,
    utest_minar_cancel,
    utest_minar_run
};
utest_v1_scheduler_t utest_v1_get_scheduler()
{
    return utest_v1_scheduler;
}
}

#elif UTEST_SHIM_SCHEDULER_USE_US_TICKER
#ifdef YOTTA_MBED_HAL_VERSION_STRING
#   include "mbed-hal/us_ticker.h"
#else
#   include "us_ticker.h"
#endif
// only one callback is active at any given time
static volatile utest_v1_harness_callback_t minimal_callback;
static volatile utest_v1_harness_callback_t ticker_callback;
static const ticker_data_t *ticker_data;
static ticker_event_t ticker_event;

static void ticker_handler(uint32_t)
{
    // printf("\t\t>>> Ticker callback fired for %p.\n", ticker_callback);
    minimal_callback = ticker_callback;
}

static int32_t utest_us_ticker_init()
{
    ticker_data = get_us_ticker_data();
    ticker_set_handler(ticker_data, ticker_handler);
    return 0;
}
static void *utest_us_ticker_post(const utest_v1_harness_callback_t callback, const uint32_t delay_ms)
{
    // printf("\t\t>>> Schedule %p with %ums delay => %p.\n", callback, (unsigned int)delay_ms, (void*)1);
    if (delay_ms) {
        ticker_callback = callback;
        // fire the interrupt in 1000us * delay_ms
        ticker_insert_event(ticker_data, &ticker_event, ticker_read(ticker_data) + delay_ms * 1000, 0);
    } else {
        minimal_callback = callback;
    }

    // return a bogus handle
    return (void*)1;
}
static int32_t utest_us_ticker_cancel(void *handle)
{
    // printf("\t\t>>> Cancel %p => %u\n", handle, (unsigned int)0);
    (void) handle;
    ticker_remove_event(ticker_data, &ticker_event);
    return 0;
}
static int32_t utest_us_ticker_run()
{
    while(1)
    {
        // check if a new callback has been set
        if (minimal_callback)
        {
            // printf("\t\t>>> Firing callback %p\n", minimal_callback);
            // copy the callback
            utest_v1_harness_callback_t callback = minimal_callback;
            // reset the shared callback
            minimal_callback = NULL;
            // execute the copied callback
            callback();
        }
    }
    return 0;
}
extern "C" {
static const utest_v1_scheduler_t utest_v1_scheduler =
{
    utest_us_ticker_init,
    utest_us_ticker_post,
    utest_us_ticker_cancel,
    utest_us_ticker_run
};
utest_v1_scheduler_t utest_v1_get_scheduler()
{
    return utest_v1_scheduler;
}
}
#endif

#ifdef YOTTA_CORE_UTIL_VERSION_STRING
// their functionality is implemented using the CriticalSectionLock class
void utest_v1_enter_critical_section(void) {}
void utest_v1_leave_critical_section(void) {}
#endif
