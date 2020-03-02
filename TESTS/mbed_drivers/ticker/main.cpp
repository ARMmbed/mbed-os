/* mbed Microcontroller Library
 * Copyright (c) 2013-2017 ARM Limited
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

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#else

using utest::v1::Case;

#define ONE_MILLI_SEC 1000
#define TICKER_COUNT 16
#define MULTI_TICKER_TIME_MS 100
volatile uint32_t callback_trigger_count = 0;
static const int test_timeout = 240;
static const int total_ticks = 10;


/* Tolerance is quite arbitrary due to large number of boards with varying level of accuracy */
#define TOLERANCE_US 1000

volatile uint32_t ticker_callback_flag;
volatile uint32_t multi_counter;

DigitalOut led1(LED1);
DigitalOut led2(LED2);

Timer gtimer;
volatile int ticker_count = 0;


void switch_led1_state(void)
{
    // blink 3 times per second
    if ((callback_trigger_count % 333) == 0) {
        led1 = !led1;
    }
}

void switch_led2_state(void)
{
    // blink 3 times per second
    // make led2 blink at the same callback_trigger_count value as led1
    if (((callback_trigger_count - 1) % 333) == 0) {
        led2 = !led2;
    }
}

void ticker_callback_1(void)
{
    ++callback_trigger_count;
    switch_led1_state();
}

void ticker_callback_2(void)
{
    ++callback_trigger_count;
    if (LED2 != NC) {
        switch_led2_state();
    }
}


void sem_release(Semaphore *sem)
{
    sem->release();
}


void stop_gtimer_set_flag(void)
{
    gtimer.stop();
    core_util_atomic_incr_u32((uint32_t *)&ticker_callback_flag, 1);
}

void increment_multi_counter(void)
{
    core_util_atomic_incr_u32((uint32_t *)&multi_counter, 1);
}


/* Tests is to measure the accuracy of Ticker over a period of time
 *
 * 1) DUT would start to update callback_trigger_count every milli sec
 * 2) Host would query what is current count base_time, Device responds by the callback_trigger_count.
 * 3) Host after waiting for measurement stretch. It will query for device time again final_time.
 * 4) Host computes the drift considering base_time, final_time, transport delay and measurement stretch
 * 5) Finally host send the results back to device pass/fail based on tolerance.
 * 6) More details on tests can be found in timing_drift_auto.py
 */
void test_case_1x_ticker()
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key = 1;
    Ticker ticker;

    led1 = 1;
    if (LED2 != NC) {
        led2 = 1;
    }
    callback_trigger_count = 0;

    greentea_send_kv("timing_drift_check_start", 0);
    ticker.attach_us(&ticker_callback_1, ONE_MILLI_SEC);

    // wait for 1st signal from host
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);
    greentea_send_kv(_key, callback_trigger_count * ONE_MILLI_SEC);

    // wait for 2nd signal from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    greentea_send_kv(_key, callback_trigger_count * ONE_MILLI_SEC);

    //get the results from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key, "Host side script reported a fail...");
}

/* Tests is to measure the accuracy of Ticker over a period of time
 *
 * 1) DUT would start to update callback_trigger_count every milli sec, we use 2 tickers
 *    to update the count alternatively.
 * 2) Host would query what is current count base_time, Device responds by the callback_trigger_count
 * 3) Host after waiting for measurement stretch. It will query for device time again final_time.
 * 4) Host computes the drift considering base_time, final_time, transport delay and measurement stretch
 * 5) Finally host send the results back to device pass/fail based on tolerance.
 * 6) More details on tests can be found in timing_drift_auto.py
 */
void test_case_2x_ticker()
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key =  1;
    Ticker ticker1, ticker2;

    led1 = 0;
    if (LED2 != NC) {
        led2 = 1;
    }
    callback_trigger_count = 0;

    ticker1.attach_us(ticker_callback_1, 2 * ONE_MILLI_SEC);
    // delay second ticker to have a pair of tickers tick every one millisecond
    wait_us(ONE_MILLI_SEC);
    greentea_send_kv("timing_drift_check_start", 0);
    ticker2.attach_us(ticker_callback_2, 2 * ONE_MILLI_SEC);

    // wait for 1st signal from host
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);
    greentea_send_kv(_key, callback_trigger_count * ONE_MILLI_SEC);

    // wait for 2nd signal from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    greentea_send_kv(_key, callback_trigger_count * ONE_MILLI_SEC);

    //get the results from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key, "Host side script reported a fail...");
}

/** Test many tickers run one after the other

    Given many Tickers
    When schedule them one after the other with the same time intervals
    Then tickers properly execute callbacks
    When schedule them one after the other with the different time intervals
    Then tickers properly execute callbacks
 */
void test_multi_ticker(void)
{
    Ticker ticker[TICKER_COUNT];
    const uint32_t extra_wait = 5; // extra 5ms wait time

    multi_counter = 0;
    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].attach_us(callback(increment_multi_counter), MULTI_TICKER_TIME_MS * 1000);
    }

    ThisThread::sleep_for(MULTI_TICKER_TIME_MS + extra_wait);
    TEST_ASSERT_EQUAL(TICKER_COUNT, multi_counter);

    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].detach();
    }
    // Because detach calls schedule_interrupt in some circumstances
    // (e.g. when head event is removed), it's good to check if
    // no more callbacks were triggered during detaching.
    TEST_ASSERT_EQUAL(TICKER_COUNT, multi_counter);

    multi_counter = 0;
    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].attach_us(callback(increment_multi_counter), (MULTI_TICKER_TIME_MS + i) * 1000);
    }

    ThisThread::sleep_for(MULTI_TICKER_TIME_MS + TICKER_COUNT + extra_wait);
    TEST_ASSERT_EQUAL(TICKER_COUNT, multi_counter);

    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].detach();
    }
    // Because detach calls schedule_interrupt in some circumstances
    // (e.g. when head event is removed), it's good to check if
    // no more callbacks were triggered during detaching.
    TEST_ASSERT_EQUAL(TICKER_COUNT, multi_counter);
}

/** Test multi callback time

    Given a Ticker
    When the callback is attached multiple times
    Then ticker properly execute callback multiple times
 */
void test_multi_call_time(void)
{
    Ticker ticker;
    int time_diff;
    const int attach_count = 10;

    for (int i = 0; i < attach_count; i++) {
        ticker_callback_flag = 0;
        gtimer.reset();

        gtimer.start();
        ticker.attach_us(callback(stop_gtimer_set_flag), MULTI_TICKER_TIME_MS * 1000);
        while (!ticker_callback_flag);
        time_diff = gtimer.read_us();

        TEST_ASSERT_UINT32_WITHIN(TOLERANCE_US, MULTI_TICKER_TIME_MS * 1000, time_diff);
    }
}

/** Test if detach cancel scheduled callback event

    Given a Ticker with callback attached
    When the callback is detached
    Then the callback is not being called
 */
void test_detach(void)
{
    Ticker ticker;
    bool ret;
    const float ticker_time_s = 0.1f;
    const uint32_t wait_time_ms = 500;
    Semaphore sem(0, 1);

    ticker.attach(callback(sem_release, &sem), ticker_time_s);

    sem.acquire();

    sem.acquire();
    ticker.detach(); /* cancel */

    ret = sem.try_acquire_for(wait_time_ms);
    TEST_ASSERT_FALSE(ret);
}

/** Test single callback time via attach

    Given a Ticker
    When callback attached with time interval specified
    Then ticker properly executes callback within a specified time interval
 */
template<us_timestamp_t DELAY_US>
void test_attach_time(void)
{
    Ticker ticker;
    ticker_callback_flag = 0;

    gtimer.reset();
    gtimer.start();
    ticker.attach(callback(stop_gtimer_set_flag), ((float)DELAY_US) / 1000000.0f);
    while (!ticker_callback_flag);
    ticker.detach();
    const int time_diff = gtimer.read_us();

    TEST_ASSERT_UINT64_WITHIN(TOLERANCE_US, DELAY_US, time_diff);
}

/** Test single callback time via attach_us

    Given a Ticker
    When callback attached with time interval specified
    Then ticker properly executes callback within a specified time interval
 */
template<us_timestamp_t DELAY_US>
void test_attach_us_time(void)
{
    Ticker ticker;
    ticker_callback_flag = 0;

    gtimer.reset();
    gtimer.start();
    ticker.attach_us(callback(stop_gtimer_set_flag), DELAY_US);
    while (!ticker_callback_flag);
    ticker.detach();
    const int time_diff = gtimer.read_us();

    TEST_ASSERT_UINT64_WITHIN(TOLERANCE_US, DELAY_US, time_diff);
}


// Test cases
Case cases[] = {
    Case("Test attach for 0.01s and time measure", test_attach_time<10000>),
    Case("Test attach_us for 10ms and time measure", test_attach_us_time<10000>),
    Case("Test attach for 0.1s and time measure", test_attach_time<100000>),
    Case("Test attach_us for 100ms and time measure", test_attach_us_time<100000>),
    Case("Test attach for 0.5s and time measure", test_attach_time<500000>),
    Case("Test attach_us for 500ms and time measure", test_attach_us_time<500000>),
    Case("Test detach", test_detach),
    Case("Test multi call and time measure", test_multi_call_time),
    Case("Test multi ticker", test_multi_ticker),

#if !defined(SKIP_TIME_DRIFT_TESTS)
    Case("Test timers: 1x ticker", test_case_1x_ticker),
    Case("Test timers: 2x ticker", test_case_2x_ticker)
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(test_timeout, "timing_drift_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases, utest::v1::greentea_test_teardown_handler);

int main()
{
    utest::v1::Harness::run(specification);
}

#endif // !DEVICE_USTICKER
