/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#if !DEVICE_SLEEP
#error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

void sleep_manager_deepsleep_counter_test()
{
    bool deep_sleep_allowed = sleep_manager_can_deep_sleep();
    TEST_ASSERT_TRUE(deep_sleep_allowed);
    
    sleep_manager_lock_deep_sleep();
    deep_sleep_allowed = sleep_manager_can_deep_sleep();
    TEST_ASSERT_FALSE(deep_sleep_allowed);

    sleep_manager_unlock_deep_sleep();
    deep_sleep_allowed = sleep_manager_can_deep_sleep();
    TEST_ASSERT_TRUE(deep_sleep_allowed);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) 
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) 
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("sleep manager -  deep sleep counter", sleep_manager_deepsleep_counter_test, greentea_failure_handler),
};

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
