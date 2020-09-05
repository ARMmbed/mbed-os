/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "platform/mbed_wait_api.h"
#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"

//FastModels not support timing test
#if defined(TARGET_ARM_FM)
#error [NOT_SUPPORTED] test not supported
#else

using namespace utest::v1;

/* This test is created based on the test for Timer class.
 * Since low power timer is less accurate than regular
 * timer we need to adjust delta.
 */

/*
 * Define tolerance as follows:
 * Timer might be +/-5% out; wait_ns is permitted 40% slow, but not fast.
 * Therefore minimum measured time should be 95% of requested, maximum should
 * be 145%. Unity doesn't let us specify an asymmetric error though.
 *
 * Would be nice to have tighter upper tolerance, but in practice we've seen
 * a few devices unable to sustain theoretical throughput - flash wait states?
 */
#define TOLERANCE_MIN 0.95f
#define TOLERANCE_MAX 1.45f
#define MIDPOINT ((TOLERANCE_MIN+TOLERANCE_MAX)/2)
#define DELTA (MIDPOINT-TOLERANCE_MIN)

/* This test verifies if wait_ns's wait time
 * is accurate, according to a timer.
 *
 * Given timer is created.
 * When timer is used to measure delay.
 * Then the results are valid (within acceptable range).
 */
template<int wait_val_ms, class CompareTimer>
void test_wait_ns_time_measurement()
{
    CompareTimer timer;

    float wait_val_s = (float)wait_val_ms / 1000;

    /* Start the timer. */
    timer.start();

    /* Wait <wait_val_ms> ms - arithmetic inside wait_ns will overflow if
     * asked for too large a delay, so break it up.
     */
    for (int i = 0; i < wait_val_ms; i++) {
        wait_ns(1000000);
    }

    /* Stop the timer. */
    timer.stop();

    /* Check results - wait_val_us us have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA * wait_val_s, MIDPOINT * wait_val_s, timer.read());
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
#if DEVICE_LPTICKER
    Case("Test: wait_ns - compare with lp_timer 1s", test_wait_ns_time_measurement<1000, LowPowerTimer>),
#endif
    Case("Test: wait_ns - compare with us_timer 1s", test_wait_ns_time_measurement<1000, Timer>)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // defined(TARGET_ARM_FM)
