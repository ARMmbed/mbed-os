/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"


using utest::v1::Case;


bool callback_called;

void tiemout_callback(void)
{
    core_util_atomic_store(&callback_called, true);
}

template<int N>
void critical_section_raii_recursive(Timeout &timeout)
{
    static uint32_t depth = 0;
    CriticalSectionLock cs;

    depth++;
    TEST_ASSERT_TRUE(core_util_in_critical_section());

    if (depth < N) {
        critical_section_raii_recursive<N>(timeout);
    } else {
        // max depth reached - do the test
        const us_timestamp_t timeout_time_us = 1;
        const int wait_time_us = timeout_time_us * 100;

        timeout.attach_us(callback(tiemout_callback), timeout_time_us);
        wait_us(wait_time_us);
    }
    TEST_ASSERT_TRUE(core_util_in_critical_section());
    TEST_ASSERT_FALSE(core_util_atomic_load(&callback_called));
}


/** Template for tests

    Test C API of critical section
    Given a Timeout with callback attached
    When before critical section
    Then interrupts are enabled and timeout callback is fired
    When inside critical section
    Then interrupts are disabled and timeout callback is blocked
    When after critical section
    Then interrupts are enabled and timeout callback is fired

    Test C API of critical section - nested lock
    Given a Timeout with callback attached
    When before critical section
    Then interrupts are enabled and timeout callback is fired
    When inside nested critical section
    Then interrupts are disabled and timeout callback is blocked
    When after critical section
    Then interrupts are enabled and timeout callback is fired
 */
template<int N>
void test_C_API(void)
{
    Timeout timeout;
    const us_timestamp_t timeout_time_us = 1;
    const int wait_time_us = timeout_time_us * 100;

    TEST_ASSERT_FALSE(core_util_in_critical_section());

    core_util_atomic_store(&callback_called, false);
    timeout.attach_us(callback(tiemout_callback), timeout_time_us);
    wait_us(wait_time_us);
    TEST_ASSERT_TRUE(core_util_atomic_exchange(&callback_called, false));

    for (int i = 0; i < N; i++) {
        core_util_critical_section_enter();
        TEST_ASSERT_TRUE(core_util_in_critical_section());
    }

    timeout.attach_us(callback(tiemout_callback), timeout_time_us);
    wait_us(wait_time_us);
    TEST_ASSERT_FALSE(core_util_atomic_load(&callback_called));
    TEST_ASSERT_TRUE(core_util_in_critical_section());

    for (int i = 0; i < N - 1; i++) {
        core_util_critical_section_exit();
        TEST_ASSERT_TRUE(core_util_in_critical_section());
        TEST_ASSERT_FALSE(core_util_atomic_load(&callback_called));
    }

    core_util_critical_section_exit();
    TEST_ASSERT_FALSE(core_util_in_critical_section());
    TEST_ASSERT_TRUE(core_util_atomic_load(&callback_called));
}

/** Template for tests

    Test C++ API of critical section constructor/destructor
    Given a Timeout with callback attached
    When before critical section
    Then interrupts are enabled and timeout callback is fired
    When inside critical section
    Then interrupts are disabled and timeout callback is blocked
    When after critical section
    Then interrupts are enabled and timeout callback is fired

    Test C++ API of critical section constructor/destructor - nested lock
    Given a Timeout with callback attached
    When before critical section
    Then interrupts are enabled and timeout callback is fired
    When inside nested critical section
    Then interrupts are disabled and timeout callback is blocked
    When after critical section
    Then interrupts are enabled and timeout callback is fired
 */
template<int N>
void test_CPP_API_constructor_destructor(void)
{
    Timeout timeout;
    const us_timestamp_t timeout_time_us = 1;
    const int wait_time_us = timeout_time_us * 100;

    TEST_ASSERT_FALSE(core_util_in_critical_section());

    core_util_atomic_store(&callback_called, false);
    timeout.attach_us(callback(tiemout_callback), timeout_time_us);
    wait_us(wait_time_us);
    TEST_ASSERT_TRUE(core_util_atomic_exchange(&callback_called, false));

    critical_section_raii_recursive<N>(timeout);

    TEST_ASSERT_FALSE(core_util_in_critical_section());
    TEST_ASSERT_TRUE(core_util_atomic_load(&callback_called));
}

/** Template for tests

    Test C++ API of critical section enable/disable
    Given a Timeout with attached callback
    When before critical section
    Then interrupts are enabled and timeout callback is fired
    When inside critical section
    Then interrupts are disabled and timeout callback is blocked
    When after critical section
    Then interrupts are enabled and timeout callback is fired

    Test C++ API of critical section enable/disable - nested lock
    Given a Timeout with attached callback
    When before critical section
    Then interrupts are enabled and timeout callback is fired
    When inside nested critical section
    Then interrupts are disabled and timeout callback is blocked
    When after critical section
    Then interrupts are enabled and timeout callback is fired
 */
template<int N>
void test_CPP_API_enable_disable(void)
{
    Timeout timeout;
    const us_timestamp_t timeout_time_us = 1;
    const int wait_time_us = timeout_time_us * 100;

    TEST_ASSERT_FALSE(core_util_in_critical_section());

    core_util_atomic_store(&callback_called, false);
    timeout.attach_us(callback(tiemout_callback), timeout_time_us);
    wait_us(wait_time_us);
    TEST_ASSERT_TRUE(core_util_atomic_exchange(&callback_called, false));

    for (int i = 0; i < N; i++) {
        CriticalSectionLock::enable();
        TEST_ASSERT_TRUE(core_util_in_critical_section());
    }

    timeout.attach_us(callback(tiemout_callback), timeout_time_us);
    wait_us(wait_time_us);
    TEST_ASSERT_FALSE(core_util_atomic_load(&callback_called));
    TEST_ASSERT_TRUE(core_util_in_critical_section());

    for (int i = 0; i < N - 1; i++) {
        CriticalSectionLock::disable();
        TEST_ASSERT_TRUE(core_util_in_critical_section());
        TEST_ASSERT_FALSE(core_util_atomic_load(&callback_called));
    }

    CriticalSectionLock::disable();
    TEST_ASSERT_FALSE(core_util_in_critical_section());
    TEST_ASSERT_TRUE(core_util_atomic_load(&callback_called));
}


utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test critical section C API", test_C_API<1>),
    Case("Test critical section C API nested lock", test_C_API<10>),
    Case("Test critical section C++ API constructor/destructor", test_CPP_API_constructor_destructor<1>),
    Case("Test critical section C++ API constructor/destructor nested lock", test_CPP_API_constructor_destructor<10>),
    Case("Test critical section C++ API enable/disable", test_CPP_API_enable_disable<1>),
    Case("Test critical section C++ API enable/disable nested lock", test_CPP_API_enable_disable<10>)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
