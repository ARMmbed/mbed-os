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
#include "utest/utest.h"
#include "unity/unity.h"
#include "timeout_tests.h"

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] usticker not supported for this target.
#else

using namespace utest::v1;

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Callback called once", test_single_call<Timeout>),

    Case("Callback not called when cancelled", test_cancel<Timeout>),

    Case("Callback override", test_override<Timeout>),

    Case("Multiple timeouts running in parallel", test_multiple<Timeout>),

    Case("Zero delay", test_no_wait<Timeout>),

    Case("Reschedule in callback",  test_reschedule<Timeout>),

    Case("10 ms delay accuracy", test_delay_accuracy<Timeout, 10000, SHORT_DELTA_US>,
         greentea_failure_handler),

    Case("1 s delay accuracy", test_delay_accuracy<Timeout, 1000000, LONG_DELTA_US>,
         greentea_failure_handler),

    Case("5 s delay accuracy", test_delay_accuracy<Timeout, 5000000, LONG_DELTA_US>,
         greentea_failure_handler),

#if DEVICE_SLEEP
    Case("1 s delay during sleep", test_sleep<Timeout, 1000000, LONG_DELTA_US>,
         greentea_failure_handler),
#endif

#if !defined(SKIP_TIME_DRIFT_TESTS)
    Case("Timing drift", test_drift<Timeout>),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(240, "timing_drift_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // !DEVICE_USTICKER
