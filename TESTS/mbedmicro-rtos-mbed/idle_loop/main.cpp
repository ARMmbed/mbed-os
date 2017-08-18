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

using namespace utest::v1;


void idle_loop_test_only_sleep()
{
    LowPowerTimer timer;
    timestamp_t start = timer.read_us();
    sleep_manager_lock_deep_sleep();
    TEST_ASSERT_FALSE(sleep_manager_can_deep_sleep());
    // idle loop for 5 seconds to test if we wake up
    wait(5);
    timestamp_t end = timer.read_us();
    TEST_ASSERT_UINT32_WITHIN(100, 5000000, end - start);
    sleep_manager_unlock_deep_sleep();
}

void idle_loop_test_deep_sleep()
{
    LowPowerTimer timer;
    timestamp_t start = timer.read_us();
    TEST_ASSERT_TRUE(sleep_manager_can_deep_sleep());
    // idle loop for 5 seconds to test if we wake up
    wait(5);
    timestamp_t end = timer.read_us();
    TEST_ASSERT_UINT32_WITHIN(10000, 5000000, end - start);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) 
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Idle loop - sleep only", idle_loop_test_only_sleep, greentea_failure_handler),
    Case("Idle loop - deep sleep", idle_loop_test_deep_sleep, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) 
{
    GREENTEA_SETUP(25, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}

