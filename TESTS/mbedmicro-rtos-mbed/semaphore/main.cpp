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
#error [NOT_SUPPORTED] UsTicker need to be enabled for this test.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"

using namespace utest::v1;
using namespace std::chrono;

struct test_data {
    Semaphore *sem;
    uint32_t data;
};

#define TEST_ASSERT_DURATION_WITHIN(delta, expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(delta), decltype(expected), decltype(actual)>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected).count(), ct(actual).count()); \
    } while (0)

#if defined(MBED_CONF_RTOS_PRESENT)
#define THREAD_DELAY     30ms
#define SEMAPHORE_SLOTS  2
#define SEM_CHANGES      100
#define SHORT_WAIT       5ms

#define THREAD_STACK_SIZE 320 /* larger stack cause out of heap memory on some 16kB RAM boards in multi thread test*/

Semaphore two_slots(SEMAPHORE_SLOTS);

volatile int change_counter = 0;
volatile int sem_counter = 0;
volatile bool sem_defect = false;

void test_thread(rtos::Kernel::Clock::duration const *delay)
{
    const auto thread_delay = *delay;
    while (true) {
        two_slots.acquire();
        sem_counter++;
        const bool sem_lock_failed = sem_counter > SEMAPHORE_SLOTS;
        if (sem_lock_failed) {
            sem_defect = true;
        }
        ThisThread::sleep_for(thread_delay);
        sem_counter--;
        change_counter++;
        two_slots.release();
    }
}

/* Test multiple threads

    Given 3 threads started with different delays and a semaphore with 2 tokens
    when each thread runs it tries to acquire a token
    then no more than two threads should be able to access protected region
*/
void test_multi()
{
    const rtos::Kernel::Clock::duration t1_delay = THREAD_DELAY * 1;
    const rtos::Kernel::Clock::duration t2_delay = THREAD_DELAY * 2;
    const rtos::Kernel::Clock::duration t3_delay = THREAD_DELAY * 3;

    Thread t1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread t2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread t3(osPriorityNormal, THREAD_STACK_SIZE);

    t1.start(callback(test_thread, &t1_delay));
    t2.start(callback(test_thread, &t2_delay));
    t3.start(callback(test_thread, &t3_delay));

    while (true) {
        if (change_counter >= SEM_CHANGES or sem_defect == true) {
            t1.terminate();
            t2.terminate();
            t3.terminate();
            break;
        }
    }
}

void single_thread(struct test_data *data)
{
    data->sem->acquire();
    data->data++;
}

/** Test single thread

    Given a two threads A & B and a semaphore (with count of 0) and a counter (equals to 0)
    when thread B calls @a acquire
    then thread B waits for a token to become available
    then the counter is equal to 0
    when thread A calls @a release on the semaphore
    then thread B acquires a token and increments the counter
    then the counter equals to 1
 */
void test_single_thread()
{
    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Semaphore sem(0);
    struct test_data data;
    osStatus res;

    data.sem = &sem;
    data.data = 0;

    res = t.start(callback(single_thread, &data));
    TEST_ASSERT_EQUAL(osOK, res);
    ThisThread::sleep_for(SHORT_WAIT);

    TEST_ASSERT_EQUAL(Thread::WaitingSemaphore, t.get_state());
    TEST_ASSERT_EQUAL(0, data.data);

    res = sem.release();
    TEST_ASSERT_EQUAL(osOK, res);

    ThisThread::sleep_for(SHORT_WAIT);

    TEST_ASSERT_EQUAL(1, data.data);

    t.join();
}

void timeout_thread(Semaphore *sem)
{
    bool acquired = sem->try_acquire_for(30ms);
    TEST_ASSERT_FALSE(acquired);
}

/** Test timeout

    Given thread and a semaphore with no tokens available
    when a thread calls @a try_acquire_for with a timeout of 30ms
    then the thread is blocked for up to 30ms and timeouts after.
 */
void test_timeout()
{
    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Semaphore sem(0);
    osStatus res;

    Timer timer;
    timer.start();
    res = t.start(callback(timeout_thread, &sem));
    TEST_ASSERT_EQUAL(osOK, res);
    ThisThread::sleep_for(SHORT_WAIT);

    TEST_ASSERT_EQUAL(Thread::WaitingSemaphore, t.get_state());

    t.join();
    TEST_ASSERT_DURATION_WITHIN(5ms, 30ms, timer.elapsed_time());
}
#endif

void test_ticker_release(struct test_data *data)
{
    osStatus res;
    data->data++;
    res = data->sem->release();
    TEST_ASSERT_EQUAL(osOK, res);
}

/** Test semaphore acquire

    Given a semaphore with no tokens available and ticker with the callback registered
    when the main thread calls @a acquire
    then the main thread is blocked
    when callback calls @a release on the semaphore
    then the main thread is unblocked
 */
void test_semaphore_acquire()
{
    Semaphore sem(0);
    struct test_data data;

    data.sem = &sem;
    data.data = 0;
    Ticker t1;
    t1.attach(callback(test_ticker_release, &data), 3ms);
    sem.acquire();
    t1.detach();

    TEST_ASSERT_EQUAL(1, data.data);
}

void test_ticker_try_acquire(Semaphore *sem)
{
    osStatus res;
    res = sem->try_acquire();
    TEST_ASSERT_FALSE(res);
}

/** Test semaphore try acquire

    Given a semaphore with no tokens available and ticker with the callback registered
    when callback tries to acquire the semaphore, it fails.
 */
void test_semaphore_try_acquire()
{
    Semaphore sem(0);
    Ticker t1;
    t1.attach(callback(test_ticker_try_acquire, &sem), 3ms);
    ThisThread::sleep_for(4ms);
    t1.detach();
}


/** Test semaphore try timeout

    Given a semaphore with no tokens available
    when the main thread calls @a try_acquire_for with 3ms timeout
    then the main thread is blocked for 3ms and timeouts after
 */
void test_semaphore_try_timeout()
{
    Semaphore sem(0);
    bool res;
    res = sem.try_acquire_for(3ms);
    TEST_ASSERT_FALSE(res);
}


void test_ticker_semaphore_release(struct Semaphore *sem)
{
    osStatus res;
    res = sem->release();
    TEST_ASSERT_EQUAL(osOK, res);
}

/** Test semaphore try acquire timeout

    Given a semaphore with no tokens available and ticker with the callback registered
    when the main thread calls @a try_acquire_for with 10ms timeout
    then the main thread is blocked for up to 10ms
    when callback call @a release on the semaphore after 3ms
    then the main thread is unblocked.
 */
void test_semaphore_try_acquire_timeout()
{
    Semaphore sem(0);
    bool res;
    Ticker t1;
    t1.attach(callback(test_ticker_semaphore_release, &sem), 3ms);
    res = sem.try_acquire_for(10ms);
    t1.detach();
    TEST_ASSERT_TRUE(res);
}

/** Test no timeouts

Test 1 token no timeout
Given thread and a semaphore with one token available
when thread calls @a try_acquire with timeout of 0ms
then the thread acquires the token immediately

Test 0 tokens no timeout
Given thread and a semaphore with no tokens available
when thread calls @a try_acquire with timeout of 0ms
then the thread returns immediately without acquiring a token
 */
template<int T>
void test_no_timeout()
{
    Semaphore sem(T);

    Timer timer;
    timer.start();

    bool acquired = sem.try_acquire();
    TEST_ASSERT_EQUAL(T > 0, acquired);

    TEST_ASSERT_DURATION_WITHIN(5ms, 0ms, timer.elapsed_time());
}

/** Test multiple tokens wait

    Given a thread and a semaphore initialized with 5 tokens
    when thread calls @a try_acquire 6 times in a loop
    then the token counts goes to zero
 */
void test_multiple_tokens_wait()
{
    Semaphore sem(5);

    for (int i = 5; i >= 0; i--) {
        bool acquired = sem.try_acquire();
        TEST_ASSERT_EQUAL(i > 0, acquired);
    }
}

/** Test multiple tokens release

    Given a thread and a semaphore initialized with zero tokens and max of 5
    when thread calls @a release 6 times on the semaphore
    then the token count should be equal to 5 and last release call should fail
 */
void test_multiple_tokens_release()
{
    Semaphore sem(0, 5);

    for (int i = 5; i > 0; i--) {
        osStatus stat = sem.release();
        TEST_ASSERT_EQUAL(osOK, stat);
    }
    osStatus stat = sem.release();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test 1 token no timeout", test_no_timeout<1>),
    Case("Test 0 tokens no timeout", test_no_timeout<0>),
    Case("Test multiple tokens wait", test_multiple_tokens_wait),
    Case("Test multiple tokens release", test_multiple_tokens_release),
    Case("Test semaphore acquire", test_semaphore_acquire),
    Case("Test semaphore try acquire", test_semaphore_try_acquire),
    Case("Test semaphore try timeout", test_semaphore_try_timeout),
    Case("Test semaphore try acquire timeout", test_semaphore_try_acquire_timeout),
#if defined(MBED_CONF_RTOS_PRESENT)
    Case("Test single thread", test_single_thread),
    Case("Test timeout", test_timeout),
    Case("Test multiple threads", test_multi)
#endif
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
