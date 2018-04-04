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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "timeout_tests.h"

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] usticker not supported for this target.
#endif

using namespace utest::v1;

utest::v1::status_t greentea_failure_handler(const Case * const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Callback called once (attach)", test_single_call<AttachTester<Timeout> >),
    Case("Callback called once (attach_us)", test_single_call<AttachUSTester<Timeout> >),

    Case("Callback not called when cancelled (attach)", test_cancel<AttachTester<Timeout> >),
    Case("Callback not called when cancelled (attach_us)", test_cancel<AttachUSTester<Timeout> >),

    Case("Callback override (attach)", test_override<AttachTester<Timeout> >),
    Case("Callback override (attach_us)", test_override<AttachUSTester<Timeout> >),

    Case("Multiple timeouts running in parallel (attach)", test_multiple<AttachTester<Timeout> >),
    Case("Multiple timeouts running in parallel (attach_us)", test_multiple<AttachUSTester<Timeout> >),

    Case("Zero delay (attach)", test_no_wait<AttachTester<Timeout> >),
    Case("Zero delay (attach_us)", test_no_wait<AttachUSTester<Timeout> >),

    Case("10 ms delay accuracy (attach)", test_delay_accuracy<AttachTester<Timeout>, 10000, SHORT_DELTA_US>,
            greentea_failure_handler),
    Case("10 ms delay accuracy (attach_us)", test_delay_accuracy<AttachUSTester<Timeout>, 10000, SHORT_DELTA_US>,
            greentea_failure_handler),

    Case("1 s delay accuracy (attach)", test_delay_accuracy<AttachTester<Timeout>, 1000000, LONG_DELTA_US>,
            greentea_failure_handler),
    Case("1 s delay accuracy (attach_us)", test_delay_accuracy<AttachUSTester<Timeout>, 1000000, LONG_DELTA_US>,
            greentea_failure_handler),

    Case("5 s delay accuracy (attach)", test_delay_accuracy<AttachTester<Timeout>, 5000000, LONG_DELTA_US>,
            greentea_failure_handler),
    Case("5 s delay accuracy (attach_us)", test_delay_accuracy<AttachUSTester<Timeout>, 5000000, LONG_DELTA_US>,
            greentea_failure_handler),

#if DEVICE_SLEEP
    Case("1 s delay during sleep (attach)", test_sleep<AttachTester<Timeout>, 1000000, LONG_DELTA_US>,
            greentea_failure_handler),
    Case("1 s delay during sleep (attach_us)", test_sleep<AttachUSTester<Timeout>, 1000000, LONG_DELTA_US>,
            greentea_failure_handler),
#endif

    Case("Timing drift (attach)", test_drift<AttachTester<Timeout> >),
    Case("Timing drift (attach_us)", test_drift<AttachUSTester<Timeout> >),
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
