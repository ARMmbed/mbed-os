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
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "hal/us_ticker_api.h"

#if !DEVICE_LPTICKER
#error [NOT_SUPPORTED] test not supported
#else

using namespace utest::v1;

/* This test is created based on the test for Timer class.
 * Since low power timer is less accurate than regular
 * timer we need to adjust delta.
 */

/*
* Define tolerance as follows:
* tolerance = 500 us + 5% of measured time
*
* e.g.
* 1 ms delay: tolerance = 550 us
* 10 ms delay: tolerance = 1000 us
* 100 ms delay: tolerance = 5500 us
* 1000 ms delay: tolerance = 50500 us
*
*/

#define TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(expected, actual) \
    TEST_ASSERT_UINT64_WITHIN(std::chrono::duration_cast<std::chrono::microseconds>(expected).count() / 20 + 500, \
        std::chrono::duration_cast<std::chrono::microseconds>(expected).count(), \
        std::chrono::duration_cast<std::chrono::microseconds>(actual).count())

#define US_PER_MSEC      1000

void busy_wait_us(int us)
{
    const ticker_data_t *const ticker = get_us_ticker_data();
    uint32_t start = ticker_read(ticker);
    while ((ticker_read(ticker) - start) < (uint32_t)us);
}

void busy_wait_ms(int ms)
{
    busy_wait_us(ms * US_PER_MSEC);
}

/* This test verifies if low power timer is stopped after
 * creation.
 *
 * Given Timer has been successfully created.
 * When read of timer elapsed time is requested.
 * Then result is always 0.
 */
void test_lptimer_creation()
{
    LowPowerTimer lp_timer;

    /* Check results. */
    TEST_ASSERT_EQUAL_UINT64(0, lp_timer.elapsed_time().count());

    /* Wait 10 ms.
     * After that operation timer read routines should still return 0. */
    busy_wait_ms(10);

    /* Check results. */
    TEST_ASSERT_EQUAL_UINT64(0, lp_timer.elapsed_time().count());
}

/* This test verifies if read(), read_us(), read_ms(),
 * read_high_resolution_us()
 * functions return time accumulated between
 * low power timer starts and stops.
 *
 * Given Timer has been successfully created and
 * few times started and stopped after a specified period of time.
 * When timer read request is performed.
 * Then read functions return accumulated time elapsed between starts
 * and stops.
 */
void test_lptimer_time_accumulation()
{
    LowPowerTimer lp_timer;

    /* Start the timer. */
    lp_timer.start();

    /* Wait 10 ms. */
    busy_wait_ms(10);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 10 ms have elapsed. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(10ms, lp_timer.elapsed_time());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer counted time. */
    busy_wait_ms(50);

    /* ------ */

    /* Start the timer. */
    lp_timer.start();

    /* Wait 20 ms. */
    busy_wait_ms(20);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 30 ms have elapsed. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(30ms, lp_timer.elapsed_time());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer counted time. */

    /* ------ */

    /* Start the timer. */
    lp_timer.start();

    /* Wait 30 ms. */
    busy_wait_ms(30);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 60 ms have elapsed. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(60ms, lp_timer.elapsed_time());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer time. */
    busy_wait_ms(50);

    /* ------ */

    /* Start the timer. */
    lp_timer.start();

    /* Wait 1 sec. */
    busy_wait_ms(1000);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 1060 ms have elapsed. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(1060ms, lp_timer.elapsed_time());
}

/* This test verifies if reset() function resets the
 * low power timer counted time.
 *
 * Given timer has been started and stopped once, then reset
 * operation was performed.
 * When timer is started and stopped next time.
 * Then timer read functions returns only the the second
 * measured time.
 */
void test_lptimer_reset()
{
    LowPowerTimer lp_timer;

    /* First measure 10 ms delay. */
    lp_timer.start();

    /* Wait 10 ms. */
    busy_wait_ms(10);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 10 ms elapsed. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(10ms, lp_timer.elapsed_time());

    /* Reset the timer - previous measured time should be lost now. */
    lp_timer.reset();

    /* Now measure 20 ms delay. */
    lp_timer.start();

    /* Wait 20 ms. */
    busy_wait_ms(20);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - 20 ms elapsed since the reset. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(20ms, lp_timer.elapsed_time());
}

/* This test verifies if calling start() for already
 * started low power timer does nothing.
 *
 * Given timer is already started.
 * When timer is started again.
 * Then second start operation is ignored.
 */
void test_lptimer_start_started_timer()
{
    LowPowerTimer lp_timer;

    /* Start the timer. */
    lp_timer.start();

    /* Wait 10 ms. */
    busy_wait_ms(10);

    /* Now start timer again. */
    lp_timer.start();

    /* Wait 20 ms. */
    busy_wait_ms(20);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - 30 ms have elapsed since the first start. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(30ms, lp_timer.elapsed_time());
}

/* This test verifies if time counted by the low power timer is
 * valid.
 *
 * Given timer is created.
 * When timer is used to measure 1ms/10ms/100ms/1s
 * delays.
 * Then the results are valid (within acceptable range).
 */
template<int wait_val_us>
void test_lptimer_time_measurement()
{
    LowPowerTimer lp_timer;

    std::chrono::microseconds wait_val(wait_val_us);

    /* Start the timer. */
    lp_timer.start();

    /* Wait <wait_val_us> us. */
    busy_wait_us(static_cast<int>(wait_val.count()));

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - wait_val_us us have elapsed. */
    TEST_ASSERT_TIMES_EQUAL_WITHIN_TOLERANCE(wait_val, lp_timer.elapsed_time());
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test: LowPowerTimer - stopped after creation.", test_lptimer_creation),
    Case("Test: LowPowerTimer - measure time accumulation.", test_lptimer_time_accumulation),
    Case("Test: LowPowerTimer - reset.", test_lptimer_reset),
    Case("Test: LowPowerTimer - start started timer.", test_lptimer_start_started_timer),
    Case("Test: LowPowerTimer - time measurement 1 ms.", test_lptimer_time_measurement<1000>),
    Case("Test: LowPowerTimer - time measurement 10 ms.", test_lptimer_time_measurement<10000>),
    Case("Test: LowPowerTimer - time measurement 100 ms.", test_lptimer_time_measurement<100000>),
    Case("Test: LowPowerTimer - time measurement 1 s.", test_lptimer_time_measurement<1000000>)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_LPTICKER
