/* mbed Microcontroller Library
 * Copyright (c) 2013-2017 ARM Limited
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


using namespace utest::v1;

#define ONE_MILLI_SEC 1000
volatile uint32_t callback_trigger_count = 0;
static const int test_timeout = 240;
static const int total_ticks = 10;

/* CPU clock */
extern uint32_t SystemCoreClock;

#define TOLERANCE_FACTOR 10000.0f
#define US_FACTOR 1000000.0f

/* Tolerance in microseconds

   Assume below code

   ...
   Ticker ticker;
   Timer timer;
   int32_t ret;

   timer.start();
   ticker.attach_us(callback(wait_callback_sem), 100000);
   ret = ticker_callback_sem.wait();
   timer.stop();
   int time_diff = timer.read_us();
   TEST_ASSERT_UINT32_WITHIN((uint32_t)TOLERANCE, 100000, time_diff);
   ...

   For 100000[us] callback for NUCLEO_F070RB we get time_diff: 100070[us]
   So the extra 70[us] is from time measure functions calls

   To take in to account this extra time we introduces TOLERANCE value based on CPU clock (speed)

   e.g.
   For K64F          TOLERANCE = (10000 / 120000000) * 1000000 =  83[us]
   For NUCLEO_F070RB TOLERANCE = (10000 /  48000000) * 1000000 = 208[us]
   For NRF51_DK      TOLERANCE = (10000 /  16000000) * 1000000 = 625[us]
 */
#define TOLERANCE ((TOLERANCE_FACTOR / (float)SystemCoreClock) * US_FACTOR)

#define TICKER_COUNT 16

Semaphore ticker_callback_sem(0, 1);
volatile bool ticker_callback_flag;
volatile uint32_t multi_counter;

DigitalOut led1(LED1);
DigitalOut led2(LED2);

Ticker *ticker1;
Ticker *ticker2;

volatile int ticker_count = 0;
volatile bool print_tick = false;

void ticker_callback_1_switch_to_2(void);
void ticker_callback_2_switch_to_1(void);


void ticker_callback_0(void)
{
    ++callback_trigger_count;
}

void ticker_callback_1_led(void)
{
    led1 = !led1;
}

void ticker_callback_2_led(void)
{
    led2 = !led2;
}

void ticker_callback_1_switch_to_2(void)
{
    ++callback_trigger_count;
    ticker1->detach();
    ticker1->attach_us(ticker_callback_2_switch_to_1, ONE_MILLI_SEC);
    ticker_callback_1_led();
}

void ticker_callback_2_switch_to_1(void)
{
    ++callback_trigger_count;
    ticker2->detach();
    ticker2->attach_us(ticker_callback_1_switch_to_2, ONE_MILLI_SEC);
    ticker_callback_2_led();
}

void wait_and_print()
{
    while (ticker_count <= total_ticks) {
        if (print_tick) {
            print_tick = false;
            greentea_send_kv("tick", ticker_count++);
        }
    }
}

void wait_callback_sem(void)
{
    ticker_callback_flag = true;
    ticker_callback_sem.release();
}

void wait_callback_multi(void)
{
    multi_counter++;
}

/* Tests is to measure the accuracy of Ticker over a period of time
 *
 * 1) DUT would start to update callback_trigger_count every milli sec, in 2x callback we use 2 tickers
 *    to update the count alternatively.
 * 2) Host would query what is current count base_time, Device responds by the callback_trigger_count
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

    greentea_send_kv("timing_drift_check_start", 0);
    ticker1->attach_us(&ticker_callback_0, ONE_MILLI_SEC);

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

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key,"Host side script reported a fail...");

}

void test_case_2x_callbacks()
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key =  1;

    led1 = 0;
    led2 = 0;
    callback_trigger_count = 0;

    greentea_send_kv("timing_drift_check_start", 0);
    ticker1->attach_us(ticker_callback_1_switch_to_2, ONE_MILLI_SEC);

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

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key,"Host side script reported a fail...");

}

/** Test many tickers run one after the other

    Given a 16 Tickers
    When schedule them one after the other with the same time intervals
    Then tickers properly execute callbacks
    When schedule them one after the other with the different time intervals
    Then tickers properly execute callbacks
 */
void test_multi_ticker(void)
{
    Ticker ticker[TICKER_COUNT];

    multi_counter = 0;
    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].attach_us(callback(wait_callback_multi), 100 * 1000);
    }

    Thread::wait(105);
    for (int i = 0; i < TICKER_COUNT; i++) {
            ticker[i].detach();
    }

    TEST_ASSERT_EQUAL(TICKER_COUNT, multi_counter);

    multi_counter = 0;
    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].attach_us(callback(wait_callback_multi), (100 + i) * 1000);
    }

    Thread::wait(120);
    for (int i = 0; i < TICKER_COUNT; i++) {
        ticker[i].detach();
    }
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
    int32_t ret;
    Timer timer;
    int time_diff;

    for (int i = 0; i < 10; i++) {
        ticker_callback_flag = false;
        timer.reset();

        timer.start();
        ticker.attach_us(callback(wait_callback_sem), 100000);
        ret = ticker_callback_sem.wait();
        timer.stop();
        time_diff = timer.read_us();

        TEST_ASSERT_TRUE(ret > 0);
        TEST_ASSERT_EQUAL(true, ticker_callback_flag);
        TEST_ASSERT_UINT32_WITHIN(TOLERANCE, 100000, time_diff);
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
    int32_t ret;
    ticker_callback_flag = false;

    ticker.attach(callback(wait_callback_sem), 0.1f);

    ret = ticker_callback_sem.wait();
    TEST_ASSERT_TRUE(ret > 0);
    TEST_ASSERT_EQUAL(true, ticker_callback_flag);
    ticker_callback_flag = false;

    ret = ticker_callback_sem.wait();
    ticker.detach(); /* cancel */
    TEST_ASSERT_TRUE(ret > 0);
    TEST_ASSERT_EQUAL(true, ticker_callback_flag);
    ticker_callback_flag = false;

    ret = ticker_callback_sem.wait(500);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(false, ticker_callback_flag);
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
    int32_t ret;
    Timer timer;
    ticker_callback_flag = false;

    timer.start();
    ticker.attach(callback(wait_callback_sem), ((float)DELAY_US) / 1000000.0f);
    ret = ticker_callback_sem.wait(1000 + (DELAY_US / 1000)); /* prevents infinite wait */
    timer.stop();
    ticker.detach();
    int time_diff = timer.read_us();

    TEST_ASSERT_TRUE(ret > 0);
    TEST_ASSERT_EQUAL(true, ticker_callback_flag);
    TEST_ASSERT_UINT64_WITHIN((us_timestamp_t)TOLERANCE, DELAY_US, time_diff);
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
    int32_t ret;
    Timer timer;
    ticker_callback_flag = false;

    timer.start();
    ticker.attach_us(callback(wait_callback_sem), DELAY_US);
    ret = ticker_callback_sem.wait(1000 + (DELAY_US / 1000)); /* prevents infinite wait */
    timer.stop();
    ticker.detach();
    int time_diff = timer.read_us();

    TEST_ASSERT_TRUE(ret > 0);
    TEST_ASSERT_EQUAL(true, ticker_callback_flag);
    TEST_ASSERT_UINT64_WITHIN((us_timestamp_t)TOLERANCE, DELAY_US, time_diff);
}


utest::v1::status_t one_ticker_case_setup_handler_t(const Case *const source, const size_t index_of_case) {
    ticker1 = new Ticker();
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t two_ticker_case_setup_handler_t(const Case *const source, const size_t index_of_case)
{
    ticker1 = new Ticker();
    ticker2 = new Ticker();
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t one_ticker_case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    delete ticker1;
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t two_ticker_case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    delete ticker1;
    delete ticker2;
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

// Test cases
Case cases[] = {
    Case("Test attach for 0.001s and time measure", test_attach_time<1000>),
    Case("Test attach_us for 1ms and time measure", test_attach_us_time<1000>),
    Case("Test attach for 0.01s and time measure", test_attach_time<10000>),
    Case("Test attach_us for 10ms and time measure", test_attach_us_time<10000>),
    Case("Test attach for 0.1s and time measure", test_attach_time<100000>),
    Case("Test attach_us for 100ms and time measure", test_attach_us_time<100000>),
    Case("Test attach for 0.5s and time measure", test_attach_time<500000>),
    Case("Test attach_us for 500ms and time measure", test_attach_us_time<500000>),
    Case("Test detach", test_detach),
    Case("Test multi call and time measure", test_multi_call_time),
    Case("Test multi ticker", test_multi_ticker),
    Case("Test timers: 1x ticker", one_ticker_case_setup_handler_t,test_case_1x_ticker, one_ticker_case_teardown_handler_t),
    Case("Test timers: 2x callbacks", two_ticker_case_setup_handler_t,test_case_2x_callbacks, two_ticker_case_teardown_handler_t),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(test_timeout, "timing_drift_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
