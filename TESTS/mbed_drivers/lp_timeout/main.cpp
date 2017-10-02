/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#if !DEVICE_LOWPOWERTIMER
#error [NOT_SUPPORTED] Low power timer not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "rtos.h"

using namespace utest::v1;

#define NUM_TIMEOUTS 64
const float TEST_DELAY_S = 0.01;
const uint32_t TEST_DELAY_MS = 1000.0F * TEST_DELAY_S;
const us_timestamp_t TEST_DELAY_US = 1000000.0F * TEST_DELAY_S;

void sem_callback(Semaphore *sem)
{
    sem->release();
}

void cnt_callback(volatile uint32_t *cnt)
{
    (*cnt)++;
}

class TimeoutAttachUSTester: public LowPowerTimeout {
public:
    void attach_callback(Callback<void()> func, us_timestamp_t delay_us)
    {
        attach_us(func, delay_us);
    }
};

class TimeoutAttachTester: public LowPowerTimeout {
public:
    void attach_callback(Callback<void()> func, us_timestamp_t delay_us)
    {
        attach(func, (float) delay_us / 1000000.0f);
    }
};

/** Template for tests: callback called once
 *
 * Test callback called once
 * Given a LowPowerTimeout object with a callback attached with @a attach()
 * When given time elapses
 * Then the callback is called exactly one time
 *
 * Test callback called once
 * Given a LowPowerTimeout object with a callback attached with @a attach_us()
 * When given time elapses
 * Then the callback is called exactly one time
 */
template<typename T>
void test_callback_fires_once(void)
{
    Semaphore sem(0, 1);
    T timeout;

    timeout.attach_callback(mbed::callback(sem_callback, &sem), TEST_DELAY_US);

    int32_t sem_slots = sem.wait(0);
    TEST_ASSERT_EQUAL(0, sem_slots);

    sem_slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(1, sem_slots);

    sem_slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(0, sem_slots);

    timeout.detach();
}

/** Template for tests: callback not called when cancelled
 *
 * Test callback not called when cancelled
 * Given a LowPowerTimeout object with a callback attached with @a attach()
 * When the callback is detached before being called
 * Then the callback is never called
 *
 * Test callback not called when cancelled
 * Given a LowPowerTimeout object with a callback attached with @a attach_us()
 * When the callback is detached before being called
 * Then the callback is never called
 */
template<typename T>
void test_cancel(void)
{
    Semaphore sem(0, 1);
    T timeout;

    timeout.attach_callback(mbed::callback(sem_callback, &sem), 2.0f * TEST_DELAY_US);

    int32_t sem_slots = sem.wait(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(0, sem_slots);
    timeout.detach();

    sem_slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(0, sem_slots);
}

/** Template for tests: callback override
 *
 * Test callback override
 * Given a LowPowerTimeout object with a callback attached with @a attach()
 * When another callback is attached before first one is called
 *     and second callback's delay elapses
 * Then the second callback is called
 *     and the first callback is never called
 *
 * Test callback override
 * Given a LowPowerTimeout object with a callback attached with @a attach_us()
 * When another callback is attached before first one is called
 *     and second callback's delay elapses
 * Then the second callback is called
 *     and the first callback is never called
 */
template<typename T>
void test_override(void)
{
    Semaphore sem1(0, 1);
    Semaphore sem2(0, 1);
    T timeout;

    timeout.attach_callback(mbed::callback(sem_callback, &sem1), 2.0f * TEST_DELAY_US);

    int32_t sem_slots = sem1.wait(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(0, sem_slots);
    timeout.attach_callback(mbed::callback(sem_callback, &sem2), 2.0f * TEST_DELAY_US);

    sem_slots = sem2.wait(2 * TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(1, sem_slots);
    sem_slots = sem1.wait(0);
    TEST_ASSERT_EQUAL(0, sem_slots);

    timeout.detach();
}

/** Template for tests: multiple LowPowerTimeouts
 *
 * Test multiple LowPowerTimeouts
 * Given multiple separate LowPowerTimeout objects
 * When a callback is attached to all of these LowPowerTimeout objects with @a attach()
 *     and delay for every LowPowerTimeout elapses
 * Then all callbacks are called
 *
 * Test multiple LowPowerTimeouts
 * Given multiple separate LowPowerTimeout objects
 * When a callback is attached to all of these LowPowerTimeout objects with @a attach_us()
 *     and delay for every LowPowerTimeout elapses
 * Then all callbacks are called
 */
template<typename T>
void test_multiple(void)
{
    volatile uint32_t callback_count = 0;
    T timeouts[NUM_TIMEOUTS];
    for (size_t i = 0; i < NUM_TIMEOUTS; i++) {
        timeouts[i].attach_callback(mbed::callback(cnt_callback, &callback_count), TEST_DELAY_US);
    }
    Thread::wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(NUM_TIMEOUTS, callback_count);
}

/** Template for tests: zero delay
 *
 * Test zero delay
 * Given a LowPowerTimeout object
 * When a callback is attached with 0.0 s delay, with @a attach()
 * Then the callback is called instantly
 *
 * Test zero delay
 * Given a LowPowerTimeout object
 * When a callback is attached with 0.0 s delay, with @a attach_us()
 * Then the callback is called instantly
 */
template<typename T>
void test_no_wait(void)
{
    Semaphore sem(0, 1);
    T timeout;
    timeout.attach_callback(mbed::callback(sem_callback, &sem), 0ULL);

    int32_t sem_slots = sem.wait(0);
    TEST_ASSERT_EQUAL(1, sem_slots);
    timeout.detach();
}

volatile static bool complete;
static LowPowerTimeout lpt;

/* Timeouts are quite arbitrary due to large number of boards with varying level of accuracy */
#define LONG_TIMEOUT (100000)
#define SHORT_TIMEOUT (600)

void cb_done()
{
    complete = true;
}

#if DEVICE_SLEEP
void lp_timeout_1s_deepsleep(void)
{
    complete = false;
    LowPowerTimer timer;

    /*
     * Since deepsleep() may shut down the UART peripheral, we wait for 10ms
     * to allow for hardware serial buffers to completely flush.

     * This should be replaced with a better function that checks if the
     * hardware buffers are empty. However, such an API does not exist now,
     * so we'll use the wait_ms() function for now.
     */
    wait_ms(10);

    /*
     * We use here the low power timer instead of microsecond timer for start and
     * end because the microseconds timer might be disable during deepsleep.
     */
    timer.start();
    lpt.attach(&cb_done, 1);
    deepsleep();
    while (!complete);

    /* It takes longer to wake up from deep sleep */
    TEST_ASSERT_UINT32_WITHIN(LONG_TIMEOUT, 1000000, timer.read_us());
    TEST_ASSERT_TRUE(complete);
}

void lp_timeout_1s_sleep(void)
{
    complete = false;
    Timer timer;
    timer.start();

    sleep_manager_lock_deep_sleep();
    lpt.attach(&cb_done, 1);
    sleep();
    while (!complete);
    sleep_manager_unlock_deep_sleep();

    TEST_ASSERT_UINT32_WITHIN(LONG_TIMEOUT, 1000000, timer.read_us());
    TEST_ASSERT_TRUE(complete);
}
#endif /* DEVICE_SLEEP */

void lp_timeout_us(uint32_t delay_us, uint32_t tolerance)
{
    complete = false;
    Timer timer;
    timer.start();

    lpt.attach_us(&cb_done, delay_us);
    while (!complete);

    /* Using RTC which is less accurate */
    TEST_ASSERT_UINT32_WITHIN(tolerance, delay_us, timer.read_us());
    TEST_ASSERT_TRUE(complete);
}

void lp_timeout_5s(void)
{
    lp_timeout_us(5000000, LONG_TIMEOUT);
}

void lp_timeout_1s(void)
{
    lp_timeout_us(1000000, LONG_TIMEOUT);
}

void lp_timeout_1ms(void)
{
    lp_timeout_us(1000, SHORT_TIMEOUT);
}

void lp_timeout_500us(void)
{
    lp_timeout_us(500, SHORT_TIMEOUT);

}

utest::v1::status_t greentea_failure_handler(const Case * const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Test callback called once (with attach)", test_callback_fires_once<TimeoutAttachTester>),
    Case("Test callback called once (with attach_us)", test_callback_fires_once<TimeoutAttachUSTester>),

    Case("Test callback not called when cancelled (with attach)", test_cancel<TimeoutAttachTester>),
    Case("Test callback not called when cancelled (with attach_us)", test_cancel<TimeoutAttachUSTester>),

    Case("Test callback override (with attach)", test_override<TimeoutAttachTester>),
    Case("Test callback override (with attach_us)", test_override<TimeoutAttachUSTester>),

    Case("Test multiple timeouts running in parallel (with attach)", test_multiple<TimeoutAttachTester>),
    Case("Test multiple timeouts running in parallel (with attach_us)", test_multiple<TimeoutAttachUSTester>),

    Case("Test zero delay (with attach)", test_no_wait<TimeoutAttachTester>),
    Case("Test zero delay (with attach_us)", test_no_wait<TimeoutAttachUSTester>),

    Case("500us LowPowerTimeout", lp_timeout_500us, greentea_failure_handler),
    Case("1ms LowPowerTimeout", lp_timeout_1ms, greentea_failure_handler),
    Case("1sec LowPowerTimeout", lp_timeout_1s, greentea_failure_handler),
    Case("5sec LowPowerTimeout", lp_timeout_5s, greentea_failure_handler),

#if DEVICE_SLEEP
    Case("1sec LowPowerTimeout from sleep", lp_timeout_1s_sleep, greentea_failure_handler),
    Case("1sec LowPowerTimeout from deepsleep", lp_timeout_1s_deepsleep, greentea_failure_handler),
#endif /* DEVICE_SLEEP */
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
