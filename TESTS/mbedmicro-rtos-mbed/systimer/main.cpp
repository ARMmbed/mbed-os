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
#if !MBED_TICKLESS
#error [NOT_SUPPORTED] Tickless mode not supported for this target.
#endif

#if !DEVICE_LOWPOWERTIMER
#error [NOT_SUPPORTED] Current SysTimer implementation requires lp ticker support.
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

extern "C" {
#include "rtx_lib.h"
}
#include "rtos/TARGET_CORTEX/SysTimer.h"

#define TEST_TICKS 42UL
#define DELAY_DELTA_US 2500ULL

using namespace utest::v1;

const us_timestamp_t DELAY_US = 1000000ULL * TEST_TICKS / OS_TICK_FREQ;

// Override the handler() -- the SysTick interrupt must not be set as pending by the test code.
class SysTimerTest: public rtos::internal::SysTimer {
private:
    Semaphore _sem;
    virtual void handler()
    {
        core_util_critical_section_enter();
        _increment_tick();
        core_util_critical_section_exit();
        _sem.release();
    }

public:
    SysTimerTest() :
            SysTimer(), _sem(0, 1)
    {
    }

    virtual ~SysTimerTest()
    {
    }

    int32_t sem_wait(uint32_t millisec)
    {
        return _sem.wait(millisec);
    }
};

/** Test tick count is zero upon creation
 *
 * Given a SysTimer
 * When the timer is created
 * Then tick count is zero
 */
void test_created_with_zero_tick_count(void)
{
    SysTimerTest st;
    TEST_ASSERT_EQUAL_UINT32(0, st.get_tick());
}

/** Test tick count is updated correctly
 *
 * Given a SysTimer
 * When the @a suspend and @a resume methods are called immediately after creation
 * Then the tick count is not updated
 * When @a suspend and @a resume methods are called again after a delay
 * Then the tick count is updated
 *     and the number of ticks incremented is equal TEST_TICKS - 1
 * When @a suspend and @a resume methods are called again without a delay
 * Then the tick count is not updated
 */
void test_update_tick(void)
{
    SysTimerTest st;
    st.suspend(TEST_TICKS * 2);
    TEST_ASSERT_EQUAL_UINT32(0, st.resume());
    TEST_ASSERT_EQUAL_UINT32(0, st.get_tick());
    us_timestamp_t test_ticks_elapsed_ts = st.get_time() + DELAY_US;

    st.suspend(TEST_TICKS * 2);
    while (st.get_time() <= test_ticks_elapsed_ts) {}
    TEST_ASSERT_EQUAL_UINT32(TEST_TICKS - 1, st.resume());
    TEST_ASSERT_EQUAL_UINT32(TEST_TICKS - 1, st.get_tick());

    st.suspend(TEST_TICKS * 2);
    TEST_ASSERT_EQUAL_UINT32(0, st.resume());
    TEST_ASSERT_EQUAL_UINT32(TEST_TICKS - 1, st.get_tick());
}

/** Test get_time returns correct time
 *
 * Given a SysTimer
 * When @a get_time method is called before and after a delay
 * Then time difference is equal the delay
 */
void test_get_time(void)
{
    SysTimerTest st;
    us_timestamp_t t1 = st.get_time();

    wait_us(DELAY_US);
    us_timestamp_t t2 = st.get_time();
    TEST_ASSERT_UINT64_WITHIN(DELAY_DELTA_US, DELAY_US, t2 - t1);
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
    SysTimerTest st;
    st.cancel_tick();
    st.schedule_tick(TEST_TICKS);

    st.cancel_tick();
    int32_t sem_slots = st.sem_wait((DELAY_US + DELAY_DELTA_US) / 1000ULL);
    TEST_ASSERT_EQUAL_INT32(0, sem_slots);
    TEST_ASSERT_EQUAL_UINT32(0, st.get_tick());
}

/** Test schedule zero
 *
 * Given a SysTimer
 * When a tick is scheduled with delta = 0 ticks
 * Then the handler is called instantly
 */
void test_schedule_zero(void)
{
    SysTimerTest st;

    st.schedule_tick(0UL);
    int32_t sem_slots = st.sem_wait(0UL);
    TEST_ASSERT_EQUAL_INT32(1, sem_slots);
}

/** Test handler called once
 *
 * Given a SysTimer with a tick scheduled with delta = TEST_TICKS
 * When the handler is called
 * Then the tick count is incremented by 1
 *     and elapsed time is equal 1000000ULL * TEST_TICKS / OS_TICK_FREQ;
 * When more time elapses
 * Then the handler is not called again
 */
void test_handler_called_once(void)
{
    SysTimerTest st;
    st.schedule_tick(TEST_TICKS);
    us_timestamp_t t1 = st.get_time();
    int32_t sem_slots = st.sem_wait(0);
    TEST_ASSERT_EQUAL_INT32(0, sem_slots);

    sem_slots = st.sem_wait(osWaitForever);
    us_timestamp_t t2 = st.get_time();
    TEST_ASSERT_EQUAL_INT32(1, sem_slots);
    TEST_ASSERT_EQUAL_UINT32(1, st.get_tick());
    TEST_ASSERT_UINT64_WITHIN(DELAY_DELTA_US, DELAY_US, t2 - t1);

    sem_slots = st.sem_wait((DELAY_US + DELAY_DELTA_US) / 1000ULL);
    TEST_ASSERT_EQUAL_INT32(0, sem_slots);
    TEST_ASSERT_EQUAL_UINT32(1, st.get_tick());
}

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
    SysTimerTest st;

    sleep_manager_lock_deep_sleep();
    timer.start();
    st.schedule_tick(TEST_TICKS);

    TEST_ASSERT_FALSE_MESSAGE(sleep_manager_can_deep_sleep(), "Deep sleep should be disallowed");
    while (st.sem_wait(0) != 1) {
        sleep();
    }
    timer.stop();
    sleep_manager_unlock_deep_sleep();

    TEST_ASSERT_UINT64_WITHIN(DELAY_DELTA_US, DELAY_US, timer.read_high_resolution_us());
}

#if DEVICE_LOWPOWERTIMER
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
     * so we'll use the wait_ms() function for now.
     */
    wait_ms(10);

    // Regular Timer might be disabled during deepsleep.
    LowPowerTimer lptimer;
    SysTimerTest st;

    lptimer.start();
    st.schedule_tick(TEST_TICKS);
    TEST_ASSERT_TRUE_MESSAGE(sleep_manager_can_deep_sleep(), "Deep sleep should be allowed");
    while (st.sem_wait(0) != 1) {
        sleep();
    }
    lptimer.stop();

    TEST_ASSERT_UINT64_WITHIN(DELAY_DELTA_US, DELAY_US, lptimer.read_high_resolution_us());
}
#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Tick count is zero upon creation", test_created_with_zero_tick_count),
    Case("Tick count is updated correctly", test_update_tick),
    Case("Time is updated correctly", test_get_time),
    Case("Tick can be cancelled", test_cancel_tick),
    Case("Schedule zero ticks", test_schedule_zero),
    Case("Handler called once", test_handler_called_once),
    Case("Wake up from sleep", test_sleep),
#if DEVICE_LOWPOWERTIMER
    Case("Wake up from deep sleep", test_deepsleep),
#endif

};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
