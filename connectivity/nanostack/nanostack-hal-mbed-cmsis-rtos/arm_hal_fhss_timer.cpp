/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifdef MBED_CONF_NANOSTACK_CONFIGURATION

#include "ns_types.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "mbed_trace.h"
#include "platform/SingletonPtr.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/mbed_power_mgmt.h"
#include "events/equeue.h"
#include "events/EventQueue.h"
#include "events/mbed_shared_queues.h"
#include "Timeout.h"

#define TRACE_GROUP "fhdr"
#ifndef NUMBER_OF_SIMULTANEOUS_TIMEOUTS
#define NUMBER_OF_SIMULTANEOUS_TIMEOUTS  2
#endif //NUMBER_OF_SIMULTANEOUS_TIMEOUTS

namespace {
using namespace mbed;
using namespace events;
using namespace std::chrono;
using std::micro;

static bool timer_initialized = false;
static const fhss_api_t *fhss_active_handle = NULL;
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
static EventQueue *equeue;
#endif

// All members of fhss_timeout_s must be initialized to make the structure
// constant-initialized, and hence able to be omitted by the linker,
// as SingletonPtr now relies on C++ constant-initialization. (Previously it
// worked through C++ zero-initialization). And all the constants should be zero
// to ensure it stays in the actual zero-init part of the image if used, avoiding
// an initialized-data cost.
struct fhss_timeout_s {
    void (*fhss_timer_callback)(const fhss_api_t *fhss_api, uint16_t) = nullptr;
    bool active = false;
    SingletonPtr<Timeout> timeout;
};

fhss_timeout_s fhss_timeout[NUMBER_OF_SIMULTANEOUS_TIMEOUTS];

static fhss_timeout_s *find_timeout(void (*callback)(const fhss_api_t *api, uint16_t))
{
    for (fhss_timeout_s &t : fhss_timeout) {
        if (t.fhss_timer_callback == callback) {
            return &t;
        }
    }
    return nullptr;
}

static fhss_timeout_s *allocate_timeout(void)
{
    for (fhss_timeout_s &t : fhss_timeout) {
        if (t.fhss_timer_callback == NULL) {
            return &t;
        }
    }
    return nullptr;
}

static void fhss_timeout_handler(void)
{
    for (fhss_timeout_s &t : fhss_timeout) {
        if (t.active) {
            microseconds remaining_time = t.timeout->remaining_time();
            if (remaining_time <= 0s) {
                t.active = false;
                t.fhss_timer_callback(fhss_active_handle, -remaining_time.count());
            }
        }
    }
}

static void timer_callback(void)
{
#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    fhss_timeout_handler();
#else
    equeue->call(fhss_timeout_handler);
#endif
}

static int platform_fhss_timer_start(uint32_t slots, void (*callback)(const fhss_api_t *api, uint16_t), const fhss_api_t *callback_param)
{
    int ret_val = -1;
    platform_enter_critical();
    if (timer_initialized == false) {
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
        equeue = mbed_highprio_event_queue();
        MBED_ASSERT(equeue != NULL);
#endif
        HighResClock::lock();
        timer_initialized = true;
    }
    fhss_timeout_s *fhss_tim = find_timeout(callback);
    if (!fhss_tim) {
        fhss_tim = allocate_timeout();
    }
    if (!fhss_tim) {
        platform_exit_critical();
        tr_error("Failed to allocate timeout");
        return ret_val;
    }
    fhss_tim->fhss_timer_callback = callback;
    fhss_tim->active = true;
    fhss_tim->timeout->attach(timer_callback, microseconds{slots});
    fhss_active_handle = callback_param;
    ret_val = 0;
    platform_exit_critical();
    return ret_val;
}

static int platform_fhss_timer_stop(void (*callback)(const fhss_api_t *api, uint16_t), const fhss_api_t *api)
{
    (void)api;
    platform_enter_critical();
    fhss_timeout_s *fhss_tim = find_timeout(callback);
    if (!fhss_tim) {
        platform_exit_critical();
        return -1;
    }
    fhss_tim->timeout->detach();
    fhss_tim->active = false;
    platform_exit_critical();
    return 0;
}

static uint32_t platform_fhss_get_remaining_slots(void (*callback)(const fhss_api_t *api, uint16_t), const fhss_api_t *api)
{
    (void)api;
    platform_enter_critical();
    fhss_timeout_s *fhss_tim = find_timeout(callback);
    if (!fhss_tim) {
        platform_exit_critical();
        return 0;
    }
    microseconds remaining_slots = fhss_tim->timeout->remaining_time();
    platform_exit_critical();
    return remaining_slots.count();
}

static uint32_t platform_fhss_timestamp_read(const fhss_api_t *api)
{
    (void)api;
    return HighResClock::now().time_since_epoch().count();
}
} // anonymous namespace

static_assert(std::ratio_equal<HighResClock::period, micro>::value, "HighResClock not microseconds!");
fhss_timer_t fhss_functions = {
    .fhss_timer_start = platform_fhss_timer_start,
    .fhss_timer_stop = platform_fhss_timer_stop,
    .fhss_get_remaining_slots = platform_fhss_get_remaining_slots,
    .fhss_get_timestamp = platform_fhss_timestamp_read,
    .fhss_resolution_divider = 1
};

#endif
