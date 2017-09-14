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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

#define TEST_STACK_SIZE 512

#define TEST_LONG_DELAY 20
#define TEST_DELAY 10
#define SIGNALS_TO_EMIT 100

Mutex stdio_mutex;

volatile int change_counter = 0;
volatile bool changing_counter = false;
volatile bool mutex_defect = false;

bool manipulate_protected_zone(const int thread_delay)
{
    bool result = true;

    osStatus stat = stdio_mutex.lock();
    TEST_ASSERT_EQUAL(osOK, stat);

    core_util_critical_section_enter();
    if (changing_counter == true) {
        result = false;
        mutex_defect = true;
    }
    changing_counter = true;

    change_counter++;
    core_util_critical_section_exit();

    Thread::wait(thread_delay);

    core_util_critical_section_enter();
    changing_counter = false;
    core_util_critical_section_exit();

    stat = stdio_mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
    return result;
}

void test_thread(int const *thread_delay)
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
    const int t1_delay = TEST_DELAY * 1;
    const int t2_delay = TEST_DELAY * 2;
    const int t3_delay = TEST_DELAY * 3;

    Thread t2(osPriorityNormal, TEST_STACK_SIZE);
    Thread t3(osPriorityNormal, TEST_STACK_SIZE);

    t2.start(callback(test_thread, &t2_delay));
    t3.start(callback(test_thread, &t3_delay));

    while (true) {
        // Thread 1 action
        Thread::wait(t1_delay);
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
    osStatus stat = mutex->lock(osWaitForever);
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = mutex->unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
}

void test_dual_thread_nolock_trylock_thread(Mutex *mutex)
{
    bool stat_b = mutex->trylock();
    TEST_ASSERT_EQUAL(true, stat_b);

    osStatus stat = mutex->unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
}

/** Test dual thread no-lock

    Test dual thread second thread lock
    Given two threads A & B and a mutex
    When thread A creates a mutex and starts thread B
        and thread B calls @a lock and @a unlock
    Then returned statuses are osOK

    Test dual thread second thread trylock
    Given two threads A & B and a mutex
    When thread A creates a mutex and starts thread B
        and thread B calls @a trylock and @a unlock
    Then returned statuses are true and osOK
*/
template <void (*F)(Mutex *)>
void test_dual_thread_nolock(void)
{
    Mutex mutex;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE);

    thread.start(callback(F, &mutex));

    wait_ms(TEST_DELAY);
}

void test_dual_thread_lock_unlock_thread(Mutex *mutex)
{
    osStatus stat = mutex->lock(osWaitForever);
    TEST_ASSERT_EQUAL(osOK, stat);
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

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(osOK, stat);

    thread.start(callback(test_dual_thread_lock_unlock_thread, &mutex));

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);

    wait_ms(TEST_DELAY);
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

    osStatus stat = mutex->lock(TEST_DELAY);
    TEST_ASSERT_EQUAL(osErrorTimeout, stat);
    TEST_ASSERT_UINT32_WITHIN(5000, TEST_DELAY*1000, timer.read_us());
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

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(osOK, stat);

    thread.start(callback(F, &mutex));

    wait_ms(TEST_LONG_DELAY);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
}

/** Test single thread lock recursive

    Given a mutex and a single running thread
    When thread calls @a lock twice and @a unlock twice on the mutex
    Then the returned statuses are osOK
*/
void test_single_thread_lock_recursive(void)
{
    Mutex mutex;
    osStatus stat;

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
}

/** Test single thread trylock

    Given a mutex and a single running thread
    When thread calls @a trylock and @a unlock on the mutex
    Then the returned statuses are osOK
*/
void test_single_thread_trylock(void)
{
    Mutex mutex;

    bool stat_b = mutex.trylock();
    TEST_ASSERT_EQUAL(true, stat_b);

    osStatus stat = mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
}

/** Test single thread lock

    Given a mutex and a single running thread
    When thread calls @a lock and @a unlock on the mutex
    Then the returned statuses are osOK
*/
void test_single_thread_lock(void)
{
    Mutex mutex;
    osStatus stat;

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(osOK, stat);
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
