/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef CELLULAR_TESTS_COMMON_H_
#define CELLULAR_TESTS_COMMON_H_

#include "CellularLog.h"

#if MBED_CONF_MBED_TRACE_ENABLE

static rtos::Mutex trace_mutex;

void trace_wait()
{
    trace_mutex.lock();
}

void trace_release()
{
    trace_mutex.unlock();
}

static char time_st[sizeof("[12345678]") + 1];

static char *trace_time(size_t ss)
{
    snprintf(time_st, sizeof("[12345678]"), "[%08llu]", rtos::Kernel::get_ms_count());
    return time_st;
}

static void trace_open()
{
    mbed_trace_init();
    mbed_trace_prefix_function_set(&trace_time);
    mbed_trace_mutex_wait_function_set(trace_wait);
    mbed_trace_mutex_release_function_set(trace_release);

    mbed_cellular_trace::mutex_wait_function_set(trace_wait);
    mbed_cellular_trace::mutex_release_function_set(trace_release);
}

static void trace_close()
{
    mbed_cellular_trace::mutex_wait_function_set(NULL);
    mbed_cellular_trace::mutex_release_function_set(NULL);

    mbed_trace_free();
}

#endif // MBED_CONF_MBED_TRACE_ENABLE


#endif /* CELLULAR_TESTS_COMMON_H_ */
