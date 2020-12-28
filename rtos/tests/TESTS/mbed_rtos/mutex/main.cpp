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
#if defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] Mutex test cases require RTOS with multithread to run
#else

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] UsTicker need to be enabled for this test.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include <type_traits>

#define TEST_ASSERT_DURATION_WITHIN(delta, expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(delta), decltype(expected), decltype(actual)>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected).count(), ct(actual).count()); \
    } while (0)


using namespace utest::v1;
using namespace std::chrono;

#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
#define TEST_STACK_SIZE 768
#else
#define TEST_STACK_SIZE 512
#endif

#define TEST_LONG_DELAY 20ms
#define TEST_DELAY 10ms
#define SIGNALS_TO_EMIT 100

Mutex stdio_mutex;

volatile int change_counter = 0;
volatile bool changing_counter = false;
volatile bool mutex_defect = false;

bool manipulate_protected_zone(const Kernel::Clock::duration thread_delay)
{
    bool result = true;
    osStatus stat;

    stdio_mutex.lock();

    core_util_critical_section_enter();
    if (changing_counter == true) {
        result = false;
        mutex_defect = true;
    }
    changing_counter = true;

    change_counter++;
    core_util_critical_section_exit();

    ThisThread::sleep_for(thread_delay);

    core_util_critical_section_enter();
    changing_counter = false;
    core_util_critical_section_exit();

    stdio_mutex.unlock();

    return result;
}

void test_thread(Kernel::Clock::duration const *thread_delay)
{
    while (true) {
        manipulate_protected_zone(*thread_delay);
    }
}

/** Test multiple thread

    Given 3 threads started with different delays and a section protected with a mutex
    when each thread runs it tries to lock the mutex
    then no more than one thread should be able to access protected region
*/
void test_multiple_threads(void)
{
    const Kernel::Clock::duration t1_delay = TEST_DELAY * 1;
    const Kernel::Clock::duration t2_delay = TEST_DELAY * 2;
    const Kernel::Clock::duration t3_delay = TEST_DELAY * 3;

    Thread t2(osPriorityNormal, TEST_STACK_SIZE);
    Thread t3(osPriorityNormal, TEST_STACK_SIZE);

    t2.start(callback(test_thread, &t2_delay));
    t3.start(callback(test_thread, &t3_delay));

    while (true) {
        // Thread 1 action
        ThisThread::sleep_for(t1_delay);
        manipulate_protected_zone(t1_delay);

        core_util_critical_section_enter();
        if (change_counter >= SIGNALS_TO_EMIT or mutex_defect == true) {
            core_util_critical_section_exit();
            t2.terminate();
            t3.terminate();
            break;
        }
        core_util_critical_section_exit();
    }

    TEST_ASSERT_EQUAL(false, mutex_defect);
}

void test_dual_thread_nolock_lock_thread(Mutex *mutex)
{
    osStatus stat;
    mutex->lock();

    mutex->unlock();
}

void test_dual_thread_nolock_trylock_thread(Mutex *mutex)
{
    bool stat_b = mutex->trylock();
    TEST_ASSERT_EQUAL(true, stat_b);

    mutex->unlock();
}

/** Test dual thread no-lock

    Test dual thread second thread lock
    Given two threads A & B and a mutex
    When thread A creates a mutex and starts thread B
        and thread B calls @a lock and @a unlock
    Then @a lock and @a unlock operations are successfully performed.

    Test dual thread second thread trylock
    Given two threads A & B and a mutex
    When thread A creates a mutex and starts thread B
        and thread B calls @a trylock and @a unlock
    Then @a trylock and @a unlock operations are successfully performed.
*/
template <void (*F)(Mutex *)>
void test_dual_thread_nolock(void)
{
    Mutex mutex;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE);

    thread.start(callback(F, &mutex));

    ThisThread::sleep_for(TEST_DELAY);
}

void test_dual_thread_lock_unlock_thread(Mutex *mutex)
{
    mutex->lock();
}

/** Test dual thread lock unlock

    Given two threads and a lock
    When thread A locks the lock and starts thread B
        and thread B calls @a lock on the mutex
    Then thread B waits for thread A to unlock the lock
    When thread A calls @a unlock on the mutex
    Then thread B acquires the lock
*/
void test_dual_thread_lock_unlock(void)
{
    Mutex mutex;
    osStatus stat;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE);

    mutex.lock();

    thread.start(callback(test_dual_thread_lock_unlock_thread, &mutex));

    mutex.unlock();

    ThisThread::sleep_for(TEST_DELAY);
}

void test_dual_thread_lock_trylock_thread(Mutex *mutex)
{
    bool stat = mutex->trylock();
    TEST_ASSERT_EQUAL(false, stat);
}

void test_dual_thread_lock_lock_thread(Mutex *mutex)
{
    Timer timer;
    timer.start();

    bool stat = mutex->trylock_for(TEST_DELAY);
    TEST_ASSERT_EQUAL(false, stat);
    TEST_ASSERT_DURATION_WITHIN(5ms, TEST_DELAY, timer.elapsed_time());
}

/** Test dual thread lock

    Test dual thread lock locked
    Given a mutex and two threads A & B
    When thread A calls @a lock and starts thread B
        and thread B calls @a lock with 500ms timeout
    Then thread B waits 500ms and timeouts

    Test dual thread trylock locked
    Given a mutex and two threads A & B
    When thread A calls @a lock and starts thread B
    Then thread B calls @a trylock
        and thread B fails to acquire the lock
*/
template <void (*F)(Mutex *)>
void test_dual_thread_lock(void)
{
    Mutex mutex;
    osStatus stat;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE);

    mutex.lock();

    thread.start(callback(F, &mutex));

    ThisThread::sleep_for(TEST_LONG_DELAY);

    mutex.unlock();
}

/** Test single thread lock recursive

    Given a mutex and a single running thread
    When thread calls @a lock twice and @a unlock twice on the mutex
    Then @a lock and @a unlock operations are successfully performed.
*/
void test_single_thread_lock_recursive(void)
{
    Mutex mutex;
    osStatus stat;

    mutex.lock();

    mutex.lock();

    mutex.unlock();

    mutex.unlock();
}

/** Test single thread trylock

    Given a mutex and a single running thread
    When thread calls @a trylock and @a unlock on the mutex
    Then @a trylock and @a unlock operations are successfully performed.
*/
void test_single_thread_trylock(void)
{
    Mutex mutex;

    bool stat_b = mutex.trylock();
    TEST_ASSERT_EQUAL(true, stat_b);

    mutex.unlock();
}

/** Test single thread lock

    Given a mutex and a single running thread
    When thread calls @a lock and @a unlock on the mutex
    Then @a lock and @a unlock operations are successfully performed.
*/
void test_single_thread_lock(void)
{
    Mutex mutex;
    osStatus stat;

    mutex.lock();

    mutex.unlock();
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test single thread lock", test_single_thread_lock),
    Case("Test single thread trylock", test_single_thread_trylock),
    Case("Test single thread lock recursive", test_single_thread_lock_recursive),
    Case("Test dual thread lock locked", test_dual_thread_lock<test_dual_thread_lock_lock_thread>),
    Case("Test dual thread trylock locked", test_dual_thread_lock<test_dual_thread_lock_trylock_thread>),
    Case("Test dual thread lock unlock", test_dual_thread_lock_unlock),
    Case("Test dual thread second thread lock", test_dual_thread_nolock<test_dual_thread_nolock_lock_thread>),
    Case("Test dual thread second thread trylock", test_dual_thread_nolock<test_dual_thread_nolock_trylock_thread>),
    Case("Test multiple thread", test_multiple_threads),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
#endif // defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
