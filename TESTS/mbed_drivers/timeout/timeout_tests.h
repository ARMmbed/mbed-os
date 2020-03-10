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
#ifndef MBED_TIMEOUT_TESTS_H
#define MBED_TIMEOUT_TESTS_H

#include "mbed.h"
#include "unity/unity.h"

#define NUM_TIMEOUTS 16
#define DRIFT_TEST_PERIOD_US 10000

const float TEST_DELAY_S = 0.01;
const uint32_t TEST_DELAY_MS = 1000.0F * TEST_DELAY_S;
const us_timestamp_t TEST_DELAY_US = 1000000.0F * TEST_DELAY_S;

/* Timeouts are quite arbitrary due to large number of boards with varying level of accuracy */
#define LONG_DELTA_US (100000)
#define SHORT_DELTA_US (2000)

void sem_callback(Semaphore *sem)
{
    sem->release();
}

void cnt_callback(volatile uint32_t *cnt)
{
    (*cnt)++;
}

template<typename TimeoutType>
class AttachTester: public TimeoutType {
public:
    void attach_callback(Callback<void()> func, us_timestamp_t delay_us)
    {
        TimeoutType::attach(func, (float) delay_us / 1000000.0f);
    }
};

template<typename TimeoutType>
class AttachUSTester: public TimeoutType {
public:
    void attach_callback(Callback<void()> func, us_timestamp_t delay_us)
    {
        TimeoutType::attach_us(func, delay_us);
    }
};

/** Template for tests: callback called once
 *
 * Test callback called once
 * Given a Timeout object with a callback attached with @a attach()
 * When given time elapses
 * Then the callback is called exactly one time
 *
 * Test callback called once
 * Given a Timeout object with a callback attached with @a attach_us()
 * When given time elapses
 * Then the callback is called exactly one time
 */
template<typename T>
void test_single_call(void)
{
    Semaphore sem(0, 1);
    T timeout;

    timeout.attach_callback(mbed::callback(sem_callback, &sem), TEST_DELAY_US);

    bool acquired = sem.try_acquire();
    TEST_ASSERT_FALSE(acquired);

    acquired = sem.try_acquire_for(TEST_DELAY_MS + 2);
    TEST_ASSERT_TRUE(acquired);

    acquired = sem.try_acquire_for(TEST_DELAY_MS + 2);
    TEST_ASSERT_FALSE(acquired);

    timeout.detach();
}

/** Template for tests: callback not called when cancelled
 *
 * Test callback not called when cancelled
 * Given a Timeout object with a callback attached with @a attach()
 * When the callback is detached before being called
 * Then the callback is never called
 *
 * Test callback not called when cancelled
 * Given a Timeout object with a callback attached with @a attach_us()
 * When the callback is detached before being called
 * Then the callback is never called
 */
template<typename T>
void test_cancel(void)
{
    Semaphore sem(0, 1);
    T timeout;

    timeout.attach_callback(mbed::callback(sem_callback, &sem), 2.0f * TEST_DELAY_US);

    bool acquired = sem.try_acquire_for(TEST_DELAY_MS);
    TEST_ASSERT_FALSE(acquired);
    timeout.detach();

    acquired = sem.try_acquire_for(TEST_DELAY_MS + 2);
    TEST_ASSERT_FALSE(acquired);
}

/** Template for tests: callback override
 *
 * Test callback override
 * Given a Timeout object with a callback attached with @a attach()
 * When another callback is attached before first one is called
 *     and second callback's delay elapses
 * Then the second callback is called
 *     and the first callback is never called
 *
 * Test callback override
 * Given a Timeout object with a callback attached with @a attach_us()
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

    bool acquired = sem1.try_acquire_for(TEST_DELAY_MS);
    TEST_ASSERT_FALSE(acquired);
    timeout.attach_callback(mbed::callback(sem_callback, &sem2), 2.0f * TEST_DELAY_US);

    acquired = sem2.try_acquire_for(2 * TEST_DELAY_MS + 2);
    TEST_ASSERT_TRUE(acquired);
    acquired = sem1.try_acquire();
    TEST_ASSERT_FALSE(acquired);

    timeout.detach();
}

/** Template for tests: multiple Timeouts
 *
 * Test multiple Timeouts
 * Given multiple separate Timeout objects
 * When a callback is attached to all of these Timeout objects with @a attach()
 *     and delay for every Timeout elapses
 * Then all callbacks are called
 *
 * Test multiple Timeouts
 * Given multiple separate Timeout objects
 * When a callback is attached to all of these Timeout objects with @a attach_us()
 *     and delay for every Timeout elapses
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
    ThisThread::sleep_for(TEST_DELAY_MS + 2);
    TEST_ASSERT_EQUAL(NUM_TIMEOUTS, callback_count);
}

/** Template for tests: zero delay
 *
 * Test zero delay
 * Given a Timeout object
 * When a callback is attached with 0.0 s delay, with @a attach()
 * Then the callback is called instantly
 *
 * Test zero delay
 * Given a Timeout object
 * When a callback is attached with 0.0 s delay, with @a attach_us()
 * Then the callback is called instantly
 */
template<typename T>
void test_no_wait(void)
{
    for (int i = 0; i < 100; i++) {
        Semaphore sem(0, 1);
        T timeout;
        timeout.attach_callback(mbed::callback(sem_callback, &sem), 0ULL);
        bool sem_acquired = sem.try_acquire();
        TEST_ASSERT_EQUAL(true, sem_acquired);
        timeout.detach();
    }
}

/** Template for tests: accuracy of timeout delay
 *
 * Test delay accuracy
 * Given a Timeout object with a callback attached with @a attach()
 * When the callback is called
 * Then elapsed time matches given delay
 *
 * Test delay accuracy
 * Given a Timeout object with a callback attached with @a attach_us()
 * When the callback is called
 * Then elapsed time matches given delay
 */
template<typename T, us_timestamp_t delay_us, us_timestamp_t delta_us>
void test_delay_accuracy(void)
{
    Semaphore sem(0, 1);
    T timeout;
    Timer timer;

    timer.start();
    timeout.attach_callback(mbed::callback(sem_callback, &sem), delay_us);

    sem.acquire();
    timer.stop();
    TEST_ASSERT_UINT64_WITHIN(delta_us, delay_us, timer.read_high_resolution_us());

    timeout.detach();
}

#if DEVICE_SLEEP
/** Template for tests: timeout during sleep
 *
 * Test timeout during sleep
 * Given a Timeout object with a callback attached with @a attach()
 *     and the uC in a sleep mode
 * When given delay elapses
 * Then the callback is called
 *     and elapsed time matches given delay
 *
 * Test timeout during sleep
 * Given a Timeout object with a callback attached with @a attach_us()
 *     and the uC in a sleep mode
 * When given delay elapses
 * Then the callback is called
 *     and elapsed time matches given delay
 */
template<typename T, us_timestamp_t delay_us, us_timestamp_t delta_us>
void test_sleep(void)
{
    Semaphore sem(0, 1);
    T timeout;
    Timer timer;

    sleep_manager_lock_deep_sleep();
    timer.start();
    timeout.attach_callback(mbed::callback(sem_callback, &sem), delay_us);

    bool deep_sleep_allowed = sleep_manager_can_deep_sleep_test_check();
    TEST_ASSERT_FALSE_MESSAGE(deep_sleep_allowed, "Deep sleep should be disallowed");
    sem.acquire();
    timer.stop();

    sleep_manager_unlock_deep_sleep();
    TEST_ASSERT_UINT64_WITHIN(delta_us, delay_us, timer.read_high_resolution_us());

    timeout.detach();
}

#if DEVICE_LPTICKER
/** Template for tests: timeout during deepsleep
 *
 * Test timeout during deepsleep
 * Given a LowPowerTimeout object with a callback attached with @a attach()
 *     and the uC in a deepsleep mode
 * When given delay elapses
 * Then the callback is called
 *     and elapsed time matches given delay
 *
 * Test timeout during deepsleep
 * Given a LowPowerTimeout object with a callback attached with @a attach_us()
 *     and the uC in a deepsleep mode
 * When given delay elapses
 * Then the callback is called
 *     and elapsed time matches given delay
 */
template<typename T, us_timestamp_t delay_us, us_timestamp_t delta_us>
void test_deepsleep(void)
{
    Semaphore sem(0, 1);
    T timeout;
    /*
     * We use here the low power timer instead of microsecond timer for start and
     * end because the microseconds timer might be disabled during deepsleep.
     */
    LowPowerTimer timer;

    /*
     * Since deepsleep() may shut down the UART peripheral, we wait for 20ms
     * to allow for hardware serial buffers to completely flush.
     *
     * Take NUMAKER_PFM_NUC472 as an example:
     * Its UART peripheral has 16-byte Tx FIFO. With baud rate set to 9600, flush
     * Tx FIFO would take: 16 * 8 * 1000 / 9600 = 13.3 (ms). So set wait time to
     * 20ms here for safe.

     * This should be replaced with a better function that checks if the
     * hardware buffers are empty. However, such an API does not exist now,
     * so we'll use the ThisThread::sleep_for() function for now.
     */
    ThisThread::sleep_for(20);

    timer.start();
    timeout.attach_callback(mbed::callback(sem_callback, &sem), delay_us);

    bool deep_sleep_allowed = sleep_manager_can_deep_sleep_test_check();
    TEST_ASSERT_TRUE_MESSAGE(deep_sleep_allowed, "Deep sleep should be allowed");
    sem.acquire();
    timer.stop();

    TEST_ASSERT_UINT64_WITHIN(delta_us, delay_us, timer.read_high_resolution_us());

    timeout.detach();
}
#endif
#endif

template<typename TimeoutTesterType>
class TimeoutDriftTester {
public:
    TimeoutDriftTester(us_timestamp_t period = 1000) :
        _callback_count(0), _period(period), _timeout()
    {
    }

    void reschedule_callback(void)
    {
        _timeout.attach_callback(mbed::callback(this, &TimeoutDriftTester::reschedule_callback), _period);
        _callback_count++;
    }

    void detach_callback(void)
    {
        _timeout.detach();
    }

    uint32_t get_callback_count(void)
    {
        return _callback_count;
    }

private:
    volatile uint32_t _callback_count;
    us_timestamp_t _period;
    TimeoutTesterType _timeout;
};

/** Template for tests: accuracy of timeout delay scheduled repeatedly
 *
 * Test time drift -- device part
 * Given a Timeout object with a callback repeatedly attached with @a attach()
 * When the testing host computes test duration based on values received from uC
 * Then computed time and actual time measured by host are equal within given tolerance
 *
 * Test time drift -- device part
 * Given a Timeout object with a callback repeatedly attached with @a attach_us()
 * When the testing host computes test duration based on values received from uC
 * Then computed time and actual time measured by host are equal within given tolerance
 *
 * Original description:
 * 1) DUT would start to update callback_trigger_count every milli sec
 * 2) Host would query what is current count base_time, Device responds by the callback_trigger_count
 * 3) Host after waiting for measurement stretch. It will query for device time again final_time.
 * 4) Host computes the drift considering base_time, final_time, transport delay and measurement stretch
 * 5) Finally host send the results back to device pass/fail based on tolerance.
 * 6) More details on tests can be found in timing_drift_auto.py
 */
template<typename T>
void test_drift(void)
{
    char _key[11] = { };
    char _value[128] = { };
    int expected_key = 1;
    TimeoutDriftTester<T> timeout(DRIFT_TEST_PERIOD_US);

    greentea_send_kv("timing_drift_check_start", 0);
    timeout.reschedule_callback();

    // wait for 1st signal from host
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);

    greentea_send_kv(_key, timeout.get_callback_count() * DRIFT_TEST_PERIOD_US);

    // wait for 2nd signal from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    greentea_send_kv(_key, timeout.get_callback_count() * DRIFT_TEST_PERIOD_US);

    timeout.detach_callback();

    //get the results from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    TEST_ASSERT_EQUAL_STRING_MESSAGE("pass", _key, "Host script reported a failure");
}

#endif
