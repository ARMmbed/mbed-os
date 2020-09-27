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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "ticker_api.h"

#include "platform/internal/SysTimer.h"

using namespace std::chrono;

#define TEST_TICKS 42
#define TEST_TICK_PERIOD std::ratio<TEST_TICKS, 1000>::type
#define TEST_TICK_DURATION duration<long long, TEST_TICK_PERIOD>
#define DELAY TEST_TICK_DURATION(1)
#define DELAY_DELTA 2500us

#define TEST_ASSERT_EQUAL_DURATION(expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(expected), decltype(actual)>; \
        TEST_ASSERT_EQUAL(ct(expected).count(), ct(actual).count()); \
    } while (0)

#define TEST_ASSERT_EQUAL_TIME_POINT(expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(expected), decltype(actual)>; \
        TEST_ASSERT_EQUAL(ct(expected).time_since_epoch().count(), ct(actual).time_since_epoch().count()); \
    } while (0)

#define TEST_ASSERT_DURATION_WITHIN(delta, expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(delta), decltype(expected), decltype(actual)>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected).count(), ct(actual).count()); \
    } while (0)


/*  Use a specific delta value for deep sleep, as entry/exit adds up extra latency.
 *  Use deep sleep latency if defined and add 1ms extra delta */
#if defined MBED_CONF_TARGET_DEEP_SLEEP_LATENCY
#define DEEP_SLEEP_DELAY_DELTA milliseconds(MBED_CONF_TARGET_DEEP_SLEEP_LATENCY + 1)
#else
#define DEEP_SLEEP_DELAY_DELTA 2500us
#endif

using namespace utest::v1;
using mbed::internal::SysTimer;

// The SysTick interrupt must not be set as pending by the test code.
template <class Period>
class SysTimerTest: public SysTimer<Period, false> {
private:
    Semaphore _sem;
    virtual void handler()
    {
        _sem.release();
        SysTimer<Period, false>::handler();
    }

public:
    SysTimerTest() :
        SysTimer<Period, false>(), _sem(0, 1)
    {
    }

    SysTimerTest(const ticker_data_t *data) :
        SysTimer<Period, false>(data), _sem(0, 1)
    {
    }

    virtual ~SysTimerTest()
    {
    }

    bool sem_try_acquire(rtos::Kernel::Clock::duration_u32 millisec)
    {
        return _sem.try_acquire_for(millisec);
    }

    void sem_acquire()
    {
        _sem.acquire();
    }
};

duration<timestamp_t, std::micro> mock_ticker_timestamp;

void mock_ticker_init()
{
}

uint32_t mock_ticker_read()
{
    return mock_ticker_timestamp.count();
}

void mock_ticker_disable_interrupt()
{
}

void mock_ticker_clear_interrupt()
{
}

void mock_ticker_set_interrupt(timestamp_t timestamp)
{
}

void mock_ticker_fire_interrupt()
{
}

void mock_ticker_free()
{
}

const ticker_info_t *mock_ticker_get_info()
{
    static const ticker_info_t mock_ticker_info = {
        .frequency = 1000000,
        .bits = 32
    };
    return &mock_ticker_info;
}

ticker_interface_t mock_ticker_interface = {
    .init = mock_ticker_init,
    .read = mock_ticker_read,
    .disable_interrupt = mock_ticker_disable_interrupt,
    .clear_interrupt = mock_ticker_clear_interrupt,
    .set_interrupt = mock_ticker_set_interrupt,
    .fire_interrupt = mock_ticker_fire_interrupt,
    .free = mock_ticker_free,
    .get_info = mock_ticker_get_info,
};

ticker_event_queue_t mock_ticker_event_queue;

const ticker_data_t mock_ticker_data = {
    .interface = &mock_ticker_interface,
    .queue = &mock_ticker_event_queue
};

void mock_ticker_reset()
{
    mock_ticker_timestamp = 0s;
    memset(&mock_ticker_event_queue, 0, sizeof mock_ticker_event_queue);
}

/** Test tick count is zero upon creation
 *
 * Given a SysTimer
 * When the timer is created
 * Then tick count is zero
 */
void test_created_with_zero_tick_count(void)
{
    SysTimerTest<std::milli> st;
    using time_point = decltype(st)::time_point;
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(0)), st.get_tick());
}

/** Test tick count is updated correctly
 *
 * Given a SysTimer
 * When the @a suspend and @a resume methods are called immediately after creation
 * Then the tick count is not updated
 * When @a suspend and @a resume methods are called again after a delay
 * Then the tick count is updated
 *     and the number of ticks incremented is equal TEST_TICKS
 * When @a suspend and @a resume methods are called again without a delay
 * Then the tick count is not updated
 */
void test_update_tick(void)
{
    mock_ticker_reset();
    SysTimerTest<std::milli> st(&mock_ticker_data);
    using time_point = decltype(st)::time_point;
    st.set_wake_time(st.get_tick() + TEST_TICK_DURATION(2));
    st.cancel_wake();
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(0)), st.get_tick());

    st.set_wake_time(st.get_tick() + TEST_TICK_DURATION(2));
    mock_ticker_timestamp = TEST_TICK_DURATION(1);
    st.cancel_wake();
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(1)), st.update_and_get_tick());
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(1)), st.get_tick());

    st.set_wake_time(st.get_tick() + TEST_TICK_DURATION(2));
    st.cancel_wake();
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(1)), st.get_tick());
}

/** Test get_time returns correct time
 *
 * Given a SysTimer
 * When @a get_time method is called before and after a delay
 * Then time difference is equal the delay
 */
void test_get_time(void)
{
    mock_ticker_reset();
    SysTimerTest<std::milli> st(&mock_ticker_data);
    auto t1 = st.get_time();

    mock_ticker_timestamp = TEST_TICK_DURATION(1);
    auto t2 = st.get_time();
    TEST_ASSERT_EQUAL_DURATION(TEST_TICK_DURATION(1), t2 - t1);
}

/** Test cancel_tick
 *
 * Given a SysTimer with a scheduled tick
 * When @a cancel_tick is called before the given number of ticks elapse
 * Then the handler is never called
 *     and the tick count is not incremented
 */
void test_cancel_tick(void)
{
    SysTimerTest<TEST_TICK_PERIOD> st;
    using time_point = decltype(st)::time_point;
    st.cancel_tick();
    st.start_tick();

    st.cancel_tick();
    bool acquired = st.sem_try_acquire(duration_cast<Kernel::Clock::duration>(TEST_TICK_DURATION(1) + DELAY_DELTA));
    TEST_ASSERT_FALSE(acquired);
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(0)), st.get_tick());
}

/** Test handler called twice
 *
 * Given a SysTimer with a tick scheduled with delta = TEST_TICKS
 * When the handler is called
 * Then the tick count is incremented by 1
 *     and elapsed time is equal 1000000ULL * TEST_TICKS / OS_TICK_FREQ;
 * When more time elapses
 * Repeat a second time.
 */
void test_handler_called_twice(void)
{
    SysTimerTest<TEST_TICK_PERIOD> st;
    using time_point = decltype(st)::time_point;
    auto t1 = st.get_time();
    bool acquired = st.sem_try_acquire(0s);
    TEST_ASSERT_FALSE(acquired);

    st.start_tick();
    // Wait in a busy loop to prevent entering sleep or deepsleep modes.
    do {
        acquired = st.sem_try_acquire(0s);
    } while (!acquired);
    auto t2 = st.get_time();
    TEST_ASSERT_TRUE(acquired);
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(1)), st.get_tick());
    TEST_ASSERT_DURATION_WITHIN(DELAY_DELTA, TEST_TICK_DURATION(1), t2 - t1);

    // Wait in a busy loop to prevent entering sleep or deepsleep modes.
    do {
        acquired = st.sem_try_acquire(0s);
    } while (!acquired);
    t2 = st.get_time();
    TEST_ASSERT_TRUE(acquired);
    TEST_ASSERT_EQUAL_TIME_POINT(time_point(TEST_TICK_DURATION(2)), st.get_tick());
    TEST_ASSERT_DURATION_WITHIN(DELAY_DELTA, TEST_TICK_DURATION(2), t2 - t1);
    st.cancel_tick();
}

#if DEVICE_SLEEP
/** Test wake up from sleep
 *
 * Given a SysTimer with a tick scheduled in the future
 *     and a core in sleep mode
 * When given time elapses
 * Then the uC is woken up from sleep
 *     and the tick handler is called
 *     and measured time matches requested delay
 */
void test_sleep(void)
{
    Timer timer;
    SysTimerTest<TEST_TICK_PERIOD> st;

    sleep_manager_lock_deep_sleep();
    timer.start();
    st.start_tick();

    TEST_ASSERT_FALSE_MESSAGE(sleep_manager_can_deep_sleep(), "Deep sleep should be disallowed");
    st.sem_acquire();

    timer.stop();
    st.cancel_tick();
    sleep_manager_unlock_deep_sleep();

    TEST_ASSERT_DURATION_WITHIN(DELAY_DELTA, TEST_TICK_DURATION(1), timer.elapsed_time());
}

#if DEVICE_LPTICKER && !MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER
/** Test wake up from deepsleep
 *
 * Given a SysTimer with a tick scheduled in the future
 *     and a core in deepsleep mode
 * When given time elapses
 * Then the uC is woken up from deepsleep
 *     and the tick handler is called
 *     and measured time matches requested delay
 */
void test_deepsleep(void)
{
    /*
     * Since deepsleep() may shut down the UART peripheral, we wait for 10ms
     * to allow for hardware serial buffers to completely flush.

     * This should be replaced with a better function that checks if the
     * hardware buffers are empty. However, such an API does not exist now,
     * so we'll use the ThisThread::sleep_for() function for now.
     */
    ThisThread::sleep_for(10ms);
    // Regular Timer might be disabled during deepsleep.
    LowPowerTimer lptimer;
    SysTimerTest<TEST_TICK_PERIOD> st;

    lptimer.start();
    st.start_tick();
    TEST_ASSERT_TRUE_MESSAGE(sleep_manager_can_deep_sleep_test_check(), "Deep sleep should be allowed");
    st.sem_acquire();
    lptimer.stop();
    st.cancel_tick();

    TEST_ASSERT_DURATION_WITHIN(DEEP_SLEEP_DELAY_DELTA, TEST_TICK_DURATION(1), lptimer.elapsed_time());
}
#endif
#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Tick count is zero upon creation", test_created_with_zero_tick_count),
    Case("Tick count is updated correctly", test_update_tick),
    Case("Time is updated correctly", test_get_time),
    Case("Tick can be cancelled", test_cancel_tick),
    Case("Handler called twice", test_handler_called_twice),
#if DEVICE_SLEEP
    Case("Wake up from sleep", test_sleep),
#if DEVICE_LPTICKER && !MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER
    Case("Wake up from deep sleep", test_deepsleep),
#endif
#endif

};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
