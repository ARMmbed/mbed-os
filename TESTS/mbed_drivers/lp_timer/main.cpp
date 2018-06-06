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
#include "rtos.h"
#include "hal/us_ticker_api.h"

#if !DEVICE_LPTICKER
#error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

extern uint32_t SystemCoreClock;

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
 *  */

#define US_PER_SEC       1000000
#define US_PER_MSEC      1000
#define MSEC_PER_SEC     1000

#define DELTA_US(delay_ms) (500 + (delay_ms) * US_PER_MSEC / 20)
#define DELTA_MS(delay_ms) (1 + ((delay_ms) * US_PER_MSEC / 20 / US_PER_MSEC))
#define DELTA_S(delay_ms) (0.000500f + (((float)(delay_ms)) / MSEC_PER_SEC / 20))

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
    TEST_ASSERT_EQUAL_FLOAT(0, lp_timer.read());
    TEST_ASSERT_EQUAL_INT32(0, lp_timer.read_ms());
    TEST_ASSERT_EQUAL_INT32(0, lp_timer.read_us());
    TEST_ASSERT_EQUAL_UINT64(0, lp_timer.read_high_resolution_us());

    /* Wait 10 ms.
     * After that operation timer read routines should still return 0. */
    wait_ms(10);

    /* Check results. */
    TEST_ASSERT_EQUAL_FLOAT(0, lp_timer.read());
    TEST_ASSERT_EQUAL_INT32(0, lp_timer.read_ms());
    TEST_ASSERT_EQUAL_INT32(0, lp_timer.read_us());
    TEST_ASSERT_EQUAL_UINT64(0, lp_timer.read_high_resolution_us());
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
    wait_ms(10);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 10 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(10), 0.010f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(10), 10, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(10), 10000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(10), 10000, lp_timer.read_high_resolution_us());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer counted time. */
    wait_ms(50);

    /* ------ */

    /* Start the timer. */
    lp_timer.start();

    /* Wait 20 ms. */
    wait_ms(20);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 30 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(30), 0.030f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(30), 30, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(30), 30000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(30), 30000, lp_timer.read_high_resolution_us());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer counted time. */

    /* ------ */

    /* Start the timer. */
    lp_timer.start();

    /* Wait 30 ms. */
    wait_ms(30);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 60 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(60), 0.060f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(60), 60, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(60), 60000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(60), 60000, lp_timer.read_high_resolution_us());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer time. */
    wait_ms(50);

    /* ------ */

    /* Start the timer. */
    lp_timer.start();

    /* Wait 1 sec. */
    wait_ms(1000);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 1060 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(1060), 1.060f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(1060), 1060, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(1060), 1060000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(1060), 1060000, lp_timer.read_high_resolution_us());
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
    wait_ms(10);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - totally 10 ms elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(10), 0.010f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(10), 10, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(10), 10000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(10), 10000, lp_timer.read_high_resolution_us());

    /* Reset the timer - previous measured time should be lost now. */
    lp_timer.reset();

    /* Now measure 20 ms delay. */
    lp_timer.start();

    /* Wait 20 ms. */
    wait_ms(20);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - 20 ms elapsed since the reset. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(20), 0.020f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(20), 20, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(20), 20000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(20), 20000, lp_timer.read_high_resolution_us());
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
    wait_ms(10);

    /* Now start timer again. */
    lp_timer.start();

    /* Wait 20 ms. */
    wait_ms(20);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - 30 ms have elapsed since the first start. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(30), 0.030f, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(30), 30, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(30), 30000, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(30), 30000, lp_timer.read_high_resolution_us());
}

/* This test verifies low power timer float operator.
 *
 * Given timer is created and a time period time is counted.
 * When timer object is casted on float type.
 * Then counted type in seconds is returned by means of
 * read() function.
 */
void test_lptimer_float_operator()
{
    LowPowerTimer lp_timer;

    /* Start the timer. */
    lp_timer.start();

    /* Wait 10 ms. */
    wait_ms(10);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check result - 10 ms elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(10), 0.010f, (float )(lp_timer));
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

    const int delta_ms = (wait_val_us / US_PER_MSEC);

    /* Start the timer. */
    lp_timer.start();

    /* Wait <wait_val_us> us. */
    wait_us(wait_val_us);

    /* Stop the timer. */
    lp_timer.stop();

    /* Check results - wait_val_us us have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(delta_ms), (float )wait_val_us / 1000000, lp_timer.read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(delta_ms), wait_val_us / 1000, lp_timer.read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(delta_ms), wait_val_us, lp_timer.read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(delta_ms), wait_val_us, lp_timer.read_high_resolution_us());
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
    Case("Test: LowPowerTimer - float operator.", test_lptimer_float_operator),
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
