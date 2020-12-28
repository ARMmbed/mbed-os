/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] Low power timer test cases require a RTOS to run.
#else

#if !DEVICE_LPTICKER
#error [NOT_SUPPORTED] Low power timer not supported for this target
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "../timeout/timeout_tests.h"

using namespace utest::v1;

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Callback called once", test_single_call<LowPowerTimeout>),

    Case("Callback not called when cancelled", test_cancel<LowPowerTimeout>),

    Case("Callback override", test_override<LowPowerTimeout>),

    Case("Multiple timeouts running in parallel", test_multiple<LowPowerTimeout>),

    Case("Zero delay", test_no_wait<LowPowerTimeout>),

    Case("Reschedule in callback",  test_reschedule<LowPowerTimeout>),

    Case("10 ms delay accuracy", test_delay_accuracy<LowPowerTimeout, 10000, SHORT_DELTA_US>,
         greentea_failure_handler),

    Case("1 s delay accuracy (attach)", test_delay_accuracy<LowPowerTimeout, 1000000, LONG_DELTA_US>,
         greentea_failure_handler),

    Case("5 s delay accuracy (attach)", test_delay_accuracy<LowPowerTimeout, 5000000, LONG_DELTA_US>,
         greentea_failure_handler),

#if DEVICE_SLEEP
    Case("1 s delay during sleep (attach)", test_sleep<LowPowerTimeout, 1000000, LONG_DELTA_US>,
         greentea_failure_handler),

    Case("1 s delay during deepsleep (attach)", test_deepsleep<LowPowerTimeout, 1000000, LONG_DELTA_US>,
         greentea_failure_handler),
#endif

#if !defined(SKIP_TIME_DRIFT_TESTS)
    Case("Timing drift (attach)", test_drift<LowPowerTimeout>),
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

#endif // !DEVICE_LPTICKER
#endif //!defined(MBED_CONF_RTOS_PRESENT)
