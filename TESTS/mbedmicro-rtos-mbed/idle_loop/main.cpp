/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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
 */

#include "mbed.h"
#include "rtos.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#if !DEVICE_LOWPOWERTIMER
    #error [NOT_SUPPORTED] Low power timer not supported for this target
#endif

using namespace utest::v1;

void idle_loop_test_sleep_ms_callback(timestamp_t *time_ms)
{
    LowPowerTimer timer;
    timer.start();
    const uint64_t kernel_tick_start = osKernelGetTickCount();
    Thread::wait(*time_ms);
    const timestamp_t end = timer.read_ms();
    const uint64_t kernel_tick_end = osKernelGetTickCount();

    // this does not test accurancy for waking up
    // just that we are with some margin awake (10ms)
    // sleep/low power ticker will test this
    // Note: if this does not work, we wake up with
    // default freq - 1ms
    TEST_ASSERT_UINT32_WITHIN(3, *time_ms, end);
    uint64_t kernel_ticks_expected = (uint64_t)*time_ms * osKernelGetTickFreq() * 1000UL;
    TEST_ASSERT_UINT32_WITHIN(3, kernel_ticks_expected, kernel_tick_end - kernel_tick_start);
}

template<timestamp_t time_ms>
void idle_loop_test_sleep_ms()
{
    Thread t;
    timestamp_t delay = time_ms;
    t.start(callback(idle_loop_test_sleep_ms_callback, &delay));

    Thread::yield();
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) 
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Idle loop - sleep for 10 ms", idle_loop_test_sleep_ms<10>, greentea_failure_handler),
    Case("Idle loop - sleep for 100 ms", idle_loop_test_sleep_ms<100>, greentea_failure_handler),
    Case("Idle loop - sleep for 500 ms", idle_loop_test_sleep_ms<500>, greentea_failure_handler),
    Case("Idle loop - sleep for 1 s", idle_loop_test_sleep_ms<1000>, greentea_failure_handler),
    Case("Idle loop - sleep for 5 s", idle_loop_test_sleep_ms<5000>, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) 
{
    GREENTEA_SETUP(25, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() 
{
    Harness::run(specification);
}

