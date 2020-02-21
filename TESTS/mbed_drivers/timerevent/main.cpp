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
#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] usticker not supported for this target.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "drivers/TimerEvent.h"
#include "hal/ticker_api.h"
#include "rtos.h"

using namespace utest::v1;

#define TEST_DELAY_US 50000ULL
#define DELTA         2

class TestTimerEvent: public TimerEvent {
private:
    Semaphore sem;
    virtual void handler()
    {
        sem.release();
    }

public:
    TestTimerEvent() :
        TimerEvent(), sem(0, 1)
    {

        sleep_manager_lock_deep_sleep();
    }

    TestTimerEvent(const ticker_data_t *data) :
        TimerEvent(data), sem(0, 1)
    {
    }

    virtual ~TestTimerEvent()
    {
        sleep_manager_unlock_deep_sleep();
    }

    // Make these methods publicly accessible
    using TimerEvent::insert;
    using TimerEvent::insert_absolute;
    using TimerEvent::remove;

    bool sem_try_acquire(uint32_t millisec)
    {
        return sem.try_acquire_for(millisec);
    }
};

class TestTimerEventRelative: public TestTimerEvent {
public:
    static const bool SEM_ACQUIRED_AFTER_PAST_TS_INSERTED = false;
    TestTimerEventRelative() :
        TestTimerEvent()
    {
    }

    TestTimerEventRelative(const ticker_data_t *data) :
        TestTimerEvent(data)
    {
    }

    // Set relative timestamp of internal event to present_time + ts
    void set_future_timestamp(timestamp_t ts)
    {
        insert(::ticker_read(_ticker_data) + ts);
    }

    void set_past_timestamp(void)
    {
        insert(::ticker_read(_ticker_data) - 1UL);
    }
};

class TestTimerEventAbsolute: public TestTimerEvent {
public:
    static const bool SEM_ACQUIRED_AFTER_PAST_TS_INSERTED = true;
    TestTimerEventAbsolute() :
        TestTimerEvent()
    {
    }

    TestTimerEventAbsolute(const ticker_data_t *data) :
        TestTimerEvent(data)
    {
    }

    // Set absolute timestamp of internal event to present_time + ts
    void set_future_timestamp(us_timestamp_t ts)
    {
        insert_absolute(::ticker_read_us(_ticker_data) + ts);
    }

    void set_past_timestamp(void)
    {
        insert_absolute(::ticker_read_us(_ticker_data) - 1ULL);
    }
};

/** Template for tests: insert, insert_absolute
 *
 * Test insert
 * Given an instance of @a TimerEvent subclass
 * When a tiestamp is set with @a insert()
 *     and given time elapses
 * Then an event handler is called
 *
 * Test insert_absolute
 * Given an instance of @a TimerEvent subclass
 * When a tiestamp is set with @a insert_absolute()
 *     and given time elapses
 * Then an event handler is called
 */
template<typename T>
void test_insert(void)
{
    T tte;

    tte.set_future_timestamp(TEST_DELAY_US);
    bool acquired = tte.sem_try_acquire(0);
    TEST_ASSERT_FALSE(acquired);

    acquired = tte.sem_try_acquire(TEST_DELAY_US / 1000 + DELTA);
    TEST_ASSERT_TRUE(acquired);

    tte.remove();
}

/** Template for tests: remove
 *
 * Test remove after insert
 * Given an instance of @a TimerEvent subclass
 * When a tiestamp is set with @a insert()
 *     and timestamp is removed before being reached
 * Then the event handler is never called
 *
 * Test remove after insert_absolute
 * Given an instance of @a TimerEvent subclass
 * When a tiestamp is set with @a insert_absolute()
 *     and timestamp is removed before being reached
 * Then the event handler is never called
 */
template<typename T>
void test_remove(void)
{
    T tte;

    tte.set_future_timestamp(TEST_DELAY_US * 2);
    bool acquired = tte.sem_try_acquire(TEST_DELAY_US / 1000);
    TEST_ASSERT_FALSE(acquired);
    tte.remove();

    acquired = tte.sem_try_acquire(TEST_DELAY_US * 2 / 1000 + DELTA);
    TEST_ASSERT_FALSE(acquired);
}

/** Test insert_absolute zero
 * Given an instance of @a TimerEvent subclass
 * When a timestamp of 0 us is set with @a insert_absolute()
 * Then an event handler is called instantly
 */
void test_insert_zero(void)
{
    TestTimerEvent tte;

    tte.insert_absolute(0ULL);
    bool acquired = tte.sem_try_acquire(0);
    TEST_ASSERT_TRUE(acquired);

    tte.remove();
}

/** Template for tests: insert, insert_absolute past timestamp
 *
 * Test insert timestamp from the past
 * Given an instance of @a TimerEvent subclass
 * When a timestamp of X us is set with @a insert()
 *     and X is less than present_time
 * Then an event handler is **not** called instantly
 *     (the event is scheduled after the ticker's overflow)
 *
 * Test insert_absolute timestamp from the past
 * Given an instance of @a TimerEvent subclass
 * When a timestamp of X us is set with @a insert_absolute()
 *     and X is less than present_time
 * Then an event handler is called instantly
 */
template<typename T>
void test_insert_past(void)
{
    T tte;

    tte.set_past_timestamp();
    bool acquired = tte.sem_try_acquire(0);
    TEST_ASSERT_EQUAL(tte.SEM_ACQUIRED_AFTER_PAST_TS_INSERTED, acquired);

    tte.remove();
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test insert", test_insert<TestTimerEventRelative>),
    Case("Test insert_absolute", test_insert<TestTimerEventAbsolute>),

    Case("Test remove after insert", test_remove<TestTimerEventRelative>),
    Case("Test remove after insert_absolute", test_remove<TestTimerEventAbsolute>),

    Case("Test insert_absolute zero", test_insert_zero),

    Case("Test insert timestamp from the past", test_insert_past<TestTimerEventRelative>),
    Case("Test insert_absolute timestamp from the past", test_insert_past<TestTimerEventAbsolute>),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
