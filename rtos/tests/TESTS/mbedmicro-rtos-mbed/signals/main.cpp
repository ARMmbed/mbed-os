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
#include "utest/utest.h"
#include "unity/unity.h"

using utest::v1::Case;

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] UsTicker need to be enabled for this test.
#else

#define ALL_SIGNALS       0x7fffffff
#define NO_SIGNALS        0x0

#define SIGNAL1 0x1
#define SIGNAL2 0x2
#define SIGNAL3 0x4

template <int32_t signals, uint32_t timeout, uint32_t test_val>
void run_signal_wait(void)
{
    uint32_t ret = ThisThread::flags_wait_all_for(signals, timeout);
    TEST_ASSERT_EQUAL(test_val, ret);
}

template <int32_t signals, int32_t test_val>
void run_clear(void)
{
    int32_t ret = ThisThread::flags_clear(signals);
    TEST_ASSERT_EQUAL(test_val, ret);
}

void run_multiple_wait_clear(int32_t signals1, int32_t signals2, int32_t test_val1, int32_t test_val2)
{
    int32_t ret;
    ret = ThisThread::flags_clear(signals1);
    TEST_ASSERT_EQUAL(test_val1, ret);
    ret = ThisThread::flags_clear(signals2);
    TEST_ASSERT_EQUAL(test_val2, ret);
}
/** Validate that ticker callback to flags_clr(NO_SIGNALS) doesn't change main thread signals and return actual signals

    Given main thread and ticker instance with callback registered
    When callback calls @a flags_clear(NO_SIGNALS)
    then callback @a flags_clear return status should be ALL_SIGNALS indicating that the signal is unchanged
 */
void test_clear_no_signals_with_ticker(void)
{
    Ticker t;
    osThreadFlagsSet(ThisThread::get_id(), ALL_SIGNALS);
    t.attach_us([&] { run_multiple_wait_clear(NO_SIGNALS, NO_SIGNALS, ALL_SIGNALS, ALL_SIGNALS); }, 3000);
}

/** Validate all flags_clr clears the signal in one shot

    Given main thread and ticker instance with callback registered
    When callback calls @a flags_clear(ALL_SIGNALS) with all possible signals
    then callback @a flags_clear(NO_SIGNALS) return status should be NO_SIGNALS(0) indicating all signals cleared correctly
 */
void test_clear_all_with_ticker(void)
{
    Ticker t;
    osThreadFlagsSet(ThisThread::get_id(), ALL_SIGNALS);
    t.attach_us([&] { run_multiple_wait_clear(ALL_SIGNALS, NO_SIGNALS, ALL_SIGNALS, NO_SIGNALS); }, 3000);
}

/** Validate if any signals are set on ticker callback

    Given main thread and ticker instance with callback registered
    When callback calls @a flags_clear(NO_SIGNALS)
    then callback @a flags_clear return status should be NO_SIGNALS(0) indicating no signals set
 */
void test_init_state_with_ticker(void)
{
    Ticker t;
    t.attach_us(callback(run_clear<NO_SIGNALS, NO_SIGNALS>), 3000);
}

/** Validate signal_wait return status if timeout specified

    Given main thread and ticker instance with callback registered
    When callback calls @a flags_wait_all_for(signals, timeout) with specified signals and timeout
    then callback @a flags_wait_all_for timeout and return 0 indicating no signals set
 */
template <int32_t signals, uint32_t timeout, uint32_t status>
void test_wait_timeout_with_ticker(void)
{
    Ticker t;
    t.attach_us(callback(run_signal_wait<signals, timeout, status>), 3000);
}

void run_release_wait_signal_wait_callback()
{
    osThreadFlagsSet(ThisThread::get_id(), ALL_SIGNALS);
}

/** Validate that call of signal_wait return correctly when thread has all signals already set

    Given main thread and ticker instance with callback registered
    When main thread @a flags_wait_all_for(ALL_SIGNALS, osWaitForever),
    then main thread is blocked
    when a callback calls @a osThreadFlagsSet set ALL_SIGNALS
    then the main thread is unblocked from @ flags_wait_all_for with the return of ALL_SIGNALS set
 */
void test_wait_all_already_set_with_ticker(void)
{
    Ticker t;
    t.attach_us([&] { run_release_wait_signal_wait_callback(); }, 3000);
    uint32_t ret = ThisThread::flags_wait_all_for(ALL_SIGNALS, osWaitForever);
    TEST_ASSERT_EQUAL(ALL_SIGNALS, ret);
}

void run_release_wait_signal_set_callback(int32_t signal, osThreadId_t id)
{
    int32_t ret;
    if (signal == 0) {
        for (int i = 0; i < 16; i++) {
            int32_t signal = 1 << i;
            ret = osThreadFlagsSet(id, signal);
        }
    } else {
        ret = osThreadFlagsSet(id, signal);
    }
}

/** Validate if wait_signal accumulate signals and return correctly when all signals set

    Given the main thread and ticker instance with callback registered
    When main thread @a flags_wait_all_for,
    then main thread is blocked
    when a callback calls @a osThreadFlagsSet in a loop to set 16 different signal
    then the main thread is unblocked from @ flags_wait_all_for with the return of expected different signals set
 */
void test_wait_all_loop_with_ticker(void)
{
    int32_t ret;
    Semaphore sem(0, 1);
    Ticker t;
    osThreadId_t id = ThisThread::get_id();
    t.attach_us([&] { run_release_wait_signal_set_callback(0, id); }, 4000);
    ret = ThisThread::flags_wait_all_for((ALL_SIGNALS & 0xFFFF), osWaitForever, true);
    TEST_ASSERT_EQUAL((ALL_SIGNALS & 0xFFFF), ret);
}

/** Validate if setting same signal twice cause any unwanted behaviour

    Given the main thread and two ticker instance with callback registered
    When main thread @a flags_wait_all_for,
    then main thread is blocked
    when a first callback calls @a osThreadFlagsSet set SIGNAL2 and
    second callback calls @a osThreadFlagsSet set SIGNAL1 | SIGNAL2 | SIGNAL3 with SIGNAL2 set again
    then the main thread is unblocked from @ flags_wait_all_for with return of expected signals set
 */
void test_set_double_with_ticker(void)
{
    int32_t ret;
    Ticker t1, t2;
    osThreadId_t id = ThisThread::get_id();
    t1.attach_us([&] { run_release_wait_signal_set_callback(SIGNAL2, id); }, 3000);
    t2.attach_us([&] { run_release_wait_signal_set_callback(SIGNAL1 | SIGNAL2 | SIGNAL3, id); }, 4000);

    ret = ThisThread::flags_wait_all_for((SIGNAL1 | SIGNAL2 | SIGNAL3), osWaitForever, true);
    TEST_ASSERT_EQUAL(SIGNAL1 | SIGNAL2 | SIGNAL3, ret);
}

#if defined(MBED_CONF_RTOS_PRESENT)

#define TEST_STACK_SIZE   512
#define PROHIBITED_SIGNAL 0x80000000
#define MAX_FLAG_POS 30

struct Sync {
    Sync(Semaphore &parent, Semaphore &child): sem_parent(parent), sem_child(child)
    {}

    Semaphore &sem_parent;
    Semaphore &sem_child;
};

template <int32_t signals, uint32_t timeout, uint32_t test_val>
void run_release_signal_wait(Semaphore *sem)
{
    sem->release();
    uint32_t ret = ThisThread::flags_wait_all_for(signals, timeout);
    TEST_ASSERT_EQUAL(test_val, ret);
}

template <int32_t signals, uint32_t timeout, uint32_t test_val>
void run_release_wait_signal_wait(Sync *sync)
{
    sync->sem_parent.release();
    sync->sem_child.acquire();
    uint32_t ret = ThisThread::flags_wait_all_for(signals, timeout);
    TEST_ASSERT_EQUAL(test_val, ret);
}

template <int32_t signals, int32_t test_val>
void run_wait_clear(Sync *sync)
{
    sync->sem_parent.release();
    sync->sem_child.acquire();
    int32_t ret = ThisThread::flags_clear(signals);
    TEST_ASSERT_EQUAL(test_val, ret);
}

void run_loop_wait_clear(Sync *sync)
{
    int32_t signals = NO_SIGNALS;
    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        int32_t signal = 1 << i;
        signals |= signal;
        sync->sem_child.acquire();
        int32_t ret = ThisThread::flags_clear(NO_SIGNALS);
        TEST_ASSERT_EQUAL(signals, ret);
        sync->sem_parent.release();
    }
}

template <int32_t signals1, int32_t signals2, int32_t test_val1, int32_t test_val2>
void run_double_wait_clear(Sync *sync)
{
    int32_t ret;

    sync->sem_parent.release();
    sync->sem_child.acquire();
    ret = ThisThread::flags_clear(signals1);
    TEST_ASSERT_EQUAL(test_val1, ret);

    ret = ThisThread::flags_clear(signals2);
    TEST_ASSERT_EQUAL(test_val2, ret);
}

/** Validate that call signal_clr(NO_SIGNALS) doesn't change thread signals and return actual signals

    Given two threads A & B are started, B with all signals already set
    When thread B calls @a signal_clr(NO_SIGNALS)
    Then thread B @a signal_clr status should be ALL_SIGNALS indicating that thread B state is unchanged
 */
void test_clear_no_signals(void)
{
    Semaphore sem_parent(0, 1);
    Semaphore sem_child(0, 1);
    Sync sync(sem_parent, sem_child);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_double_wait_clear<NO_SIGNALS, NO_SIGNALS, ALL_SIGNALS, ALL_SIGNALS>, &sync));
    sem_parent.acquire();
    t.flags_set(ALL_SIGNALS);
    sem_child.release();
    t.join();
}

/** Validate if any signals are set on just created thread

    Given the thread is running
    When thread execute @a signal_clr(NO_SIGNALS)
    Then thread @a signal_clr return status should be NO_SIGNALS(0) indicating no signals set
 */
void test_init_state(void)
{
    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_clear<NO_SIGNALS, NO_SIGNALS>));
    t.join();
}

/** Validate all signals set in one shot

    Given two threads A & B are started
    When thread A call @a flags_set(ALL_SIGNALS) with all possible signals
    Then thread B @a signal_clr(NO_SIGNALS) status should be ALL_SIGNALS indicating all signals set correctly
 */
void test_set_all(void)
{
    int32_t ret;
    Semaphore sem_parent(0, 1);
    Semaphore sem_child(0, 1);
    Sync sync(sem_parent, sem_child);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_wait_clear<NO_SIGNALS, ALL_SIGNALS>, &sync));

    sem_parent.acquire();
    ret = t.flags_set(ALL_SIGNALS);
    TEST_ASSERT_EQUAL(ALL_SIGNALS, ret);

    sem_child.release();
    t.join();
}

/** Validate that call flags_set with prohibited signal doesn't change thread signals

    Given two threads A & B are started, B with all signals set
    When thread A executes @a flags_set(PROHIBITED_SIGNAL) with prohibited signal
    Then thread B @a signal_clr(NO_SIGNALS) status should be ALL_SIGNALS indicating that thread B signals are unchanged

    @note Each signal has up to 31 event flags 0x1, 0x2, 0x4, 0x8, ...,  0x40000000
          Most significant bit is reserved and thereby flag 0x80000000 is prohibited
 */
void test_set_prohibited(void)
{
    int32_t ret;
    Semaphore sem_parent(0, 1);
    Semaphore sem_child(0, 1);
    Sync sync(sem_parent, sem_child);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_wait_clear<NO_SIGNALS, ALL_SIGNALS>, &sync));

    sem_parent.acquire();
    t.flags_set(ALL_SIGNALS);

#if !MBED_TRAP_ERRORS_ENABLED
    ret = t.flags_set(PROHIBITED_SIGNAL);
    TEST_ASSERT_EQUAL(osErrorParameter, ret);
#endif

    sem_child.release();
    t.join();
}

/** Validate all signals clear in one shot

    Given two threads A & B are started, B with all signals set
    When thread B execute @a signal_clr(ALL_SIGNALS) with all possible signals
    Then thread B @a signal_clr(NO_SIGNALS) status should be NO_SIGNALS(0) indicating all signals cleared correctly
 */
void test_clear_all(void)
{
    Semaphore sem_parent(0, 1);
    Semaphore sem_child(0, 1);
    Sync sync(sem_parent, sem_child);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_double_wait_clear<ALL_SIGNALS, NO_SIGNALS, ALL_SIGNALS, NO_SIGNALS>, &sync));
    sem_parent.acquire();
    t.flags_set(ALL_SIGNALS);
    sem_child.release();
    t.join();
}

/** Validate all signals set one by one in loop

    Given two threads A & B are started
    When thread A executes @a flags_set(signal) in loop with all possible signals
 */
void test_set_all_loop(void)
{
    int32_t ret;
    Semaphore sem_parent(0, 1);
    Semaphore sem_child(0, 1);
    Sync sync(sem_parent, sem_child);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_loop_wait_clear, &sync));

    int32_t signals = 0;
    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        int32_t signal = 1 << i;

        ret = t.flags_set(signal);
        signals |= signal;
        TEST_ASSERT_EQUAL(signals, ret);
        sem_child.release();
        sem_parent.acquire();
    }
    t.join();
}

/** Validate signal_wait return status if timeout specified

    Given the thread is running
    When thread executes @a flags_wait_all_for(signals, timeout) with specified signals and timeout
    Then thread @a flags_wait_all_for return should be 0 indicating no flags set
 */
template <int32_t signals, uint32_t timeout, uint32_t status>
void test_wait_timeout(void)
{
    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_signal_wait<signals, timeout, status>));
    t.join();
}

/** Validate that call of signal_wait return correctly when thread has all signals already set

    Given two threads A & B are started, B with all signals already set
    When thread B executes @a signal_wait(ALL_SIGNALS, osWaitForever),
    Then thread B @a flags_wait_all_for return immediately with ALL_SIGNALS indicating all wait signals was already set
 */
void test_wait_all_already_set(void)
{
    Semaphore sem_parent(0, 1);
    Semaphore sem_child(0, 1);
    Sync sync(sem_parent, sem_child);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_release_wait_signal_wait<ALL_SIGNALS, osWaitForever, ALL_SIGNALS>, &sync));

    sem_parent.acquire();
    TEST_ASSERT_EQUAL(Thread::WaitingSemaphore, t.get_state());
    t.flags_set(ALL_SIGNALS);
    sem_child.release();
    t.join();
}

/** Validate if signal_wait return correctly when all signals set

    Given two threads A & B are started and B waiting for a thread flag to be set
    When thread A executes @a flags_set(ALL_SIGNALS) with all possible signals
    Then thread B @a flags_wait_all_for return is ALL_SIGNALS indicating all wait signals was set
 */
void test_wait_all(void)
{
    Semaphore sem(0, 1);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_release_signal_wait<ALL_SIGNALS, osWaitForever, ALL_SIGNALS>, &sem));

    sem.acquire();
    TEST_ASSERT_EQUAL(Thread::WaitingThreadFlag, t.get_state());

    t.flags_set(ALL_SIGNALS);
    t.join();
}

/** Validate if signal_wait accumulate signals and return correctly when all signals set

    Given two threads A & B are started and B waiting for a thread signals to be set
    When thread A executes @a flags_set setting all signals in loop
    Then thread B @a flags_wait_all_for return is ALL_SIGNALS indicating that all wait signals was set
 */
void test_wait_all_loop(void)
{
    int32_t ret;
    Semaphore sem(0, 1);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_release_signal_wait<ALL_SIGNALS, osWaitForever, ALL_SIGNALS>, &sem));

    sem.acquire();
    TEST_ASSERT_EQUAL(Thread::WaitingThreadFlag, t.get_state());

    for (int i = 0; i < MAX_FLAG_POS; i++) {
        int32_t signal = 1 << i;
        ret = t.flags_set(signal);
    }
    ret = t.flags_set(1 << MAX_FLAG_POS);
    TEST_ASSERT_EQUAL(NO_SIGNALS, ret);
    t.join();
}

/** Validate if setting same signal twice cause any unwanted behaviour

    Given two threads A & B are started and B waiting for a thread signals to be set
    When thread A executes @a flags_set twice for the same signal
    Then thread A @a flags_set status is current signal set
         thread B @a flags_wait_all_for return indicates that all wait signals was set
 */
void test_set_double(void)
{
    int32_t ret;
    Semaphore sem(0, 1);

    Thread t(osPriorityNormal, TEST_STACK_SIZE);
    t.start(callback(run_release_signal_wait < SIGNAL1 | SIGNAL2 | SIGNAL3, osWaitForever, SIGNAL1 | SIGNAL2 | SIGNAL3 >, &sem));

    sem.acquire();
    TEST_ASSERT_EQUAL(Thread::WaitingThreadFlag, t.get_state());

    ret = t.flags_set(SIGNAL1);
    TEST_ASSERT_EQUAL(SIGNAL1, ret);

    ret = t.flags_set(SIGNAL2);
    TEST_ASSERT_EQUAL(SIGNAL1 | SIGNAL2, ret);

    ret = t.flags_set(SIGNAL2);
    TEST_ASSERT_EQUAL(SIGNAL1 | SIGNAL2, ret);
    TEST_ASSERT_EQUAL(Thread::WaitingThreadFlag, t.get_state());

    ret = t.flags_set(SIGNAL3);
    TEST_ASSERT_EQUAL(NO_SIGNALS, ret);
    t.join();
}
#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Validate that ticker callback flags_clear(NO_SIGNALS) doesn't change main thread flags and return actual flags", test_clear_no_signals_with_ticker),
    Case("Validate if any flags are set on ticker callback", test_init_state_with_ticker),
    Case("Validate all flags clear in one shot using ticker callback", test_clear_all_with_ticker),
    Case("Validate ticker callback flags_wait return status if timeout specified: 0[ms] no flags", test_wait_timeout_with_ticker<0, 0, 0>),
    Case("Validate ticker callback flags_wait return status if timeout specified: 0[ms] all flags", test_wait_timeout_with_ticker<ALL_SIGNALS, 0, 0>),
    Case("Validate ticker callback flags_wait return status if timeout specified: 1[ms] no flags", test_wait_timeout_with_ticker<0, 1, 0>),
    Case("Validate ticker callback flags_wait return status if timeout specified: 1[ms] all flags", test_wait_timeout_with_ticker<ALL_SIGNALS, 1, 0>),
    Case("Validate that main thread call of flags_wait_all_for return correctly when ticker callback set all flags", test_wait_all_already_set_with_ticker),
    Case("Validate if setting same flag twice cause any unwanted behaviour when ticker callbacks set", test_set_double_with_ticker),
    Case("Validate if main thread flags_wait_all_for accumulate flags and return correctly when all flags set by ticker callback", test_wait_all_loop_with_ticker),
#if defined(MBED_CONF_RTOS_PRESENT)
    Case("Validate that call flags_clear(NO_SIGNALS) doesn't change thread flags and return actual flags", test_clear_no_signals),
    Case("Validate if any flags are set on just created thread", test_init_state),
    Case("Validate all flags set in one shot", test_set_all),
    Case("Validate that call flags_set with prohibited flag doesn't change thread flags", test_set_prohibited),
    Case("Validate all flags clear in one shot", test_clear_all),
    Case("Validate all flags set one by one in loop", test_set_all_loop),
    Case("Validate flags_wait return status if timeout specified: 0[ms] no flags", test_wait_timeout<0, 0, 0>),
    Case("Validate flags_wait return status if timeout specified: 0[ms] all flags", test_wait_timeout<ALL_SIGNALS, 0, 0>),
    Case("Validate flags_wait return status if timeout specified: 1[ms] no flags", test_wait_timeout<0, 1, 0>),
    Case("Validate flags_wait return status if timeout specified: 1[ms] all flags", test_wait_timeout<ALL_SIGNALS, 1, 0>),
    Case("Validate that call of flags_wait_all_for return correctly when thread has all flags already set", test_wait_all_already_set),
    Case("Validate if flags_wait_all_for return correctly when all flags set", test_wait_all),
    Case("Validate if flags_wait_all_for accumulate flags and return correctly when all flags set", test_wait_all_loop),
    Case("Validate if setting same flag twice cause any unwanted behaviour", test_set_double),

#endif
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}

#endif // !DEVICE_USTICKER
