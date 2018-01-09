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

using namespace utest::v1;

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#if !DEVICE_USTICKER
  #error [NOT_SUPPORTED] test not supported
#endif

#define THREAD_DELAY     30
#define SEMAPHORE_SLOTS  2
#define SEM_CHANGES      100
#define SHORT_WAIT       5

#define THREAD_STACK_SIZE 320 /* larger stack cause out of heap memory on some 16kB RAM boards in multi thread test*/

Semaphore two_slots(SEMAPHORE_SLOTS);

volatile int change_counter = 0;
volatile int sem_counter = 0;
volatile bool sem_defect = false;

void test_thread(int const *delay)
{
    const int thread_delay = *delay;
    while (true) {
        two_slots.wait();
        sem_counter++;
        const bool sem_lock_failed = sem_counter > SEMAPHORE_SLOTS;
        if (sem_lock_failed) {
            sem_defect = true;
        }
        Thread::wait(thread_delay);
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
    const int t1_delay = THREAD_DELAY * 1;
    const int t2_delay = THREAD_DELAY * 2;
    const int t3_delay = THREAD_DELAY * 3;

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

struct thread_data {
    Semaphore *sem;
    uint32_t data;
};

void single_thread(struct thread_data *data)
{
    int32_t cnt = data->sem->wait();
    TEST_ASSERT_EQUAL(1, cnt);
    data->data++;
}

/** Test single thread

    Given a two threads A & B and a semaphore (with count of 0) and a counter (equals to 0)
    when thread B calls @a wait
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
    struct thread_data data;
    osStatus res;

    data.sem = &sem;
    data.data = 0;

    res = t.start(callback(single_thread, &data));
    TEST_ASSERT_EQUAL(osOK, res);
    Thread::wait(SHORT_WAIT);

    TEST_ASSERT_EQUAL(Thread::WaitingSemaphore, t.get_state());
    TEST_ASSERT_EQUAL(0, data.data);

    res = sem.release();
    TEST_ASSERT_EQUAL(osOK, res);

    Thread::wait(SHORT_WAIT);

    TEST_ASSERT_EQUAL(1, data.data);

    t.join();
}

void timeout_thread(Semaphore *sem)
{
    int32_t cnt = sem->wait(30);
    TEST_ASSERT_EQUAL(0, cnt);
}

/** Test timeout

    Given thread and a semaphore with no tokens available
    when thread calls @a wait on the semaphore with timeout of 10ms
    then the thread waits for 10ms and timeouts after
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
    Thread::wait(SHORT_WAIT);

    TEST_ASSERT_EQUAL(Thread::WaitingSemaphore, t.get_state());

    t.join();
    TEST_ASSERT_UINT32_WITHIN(5000, 30000, timer.read_us());
}

/** Test no timeouts

Test 1 token no timeout
Given thread and a semaphore with one token available
when thread calls @a wait on the semaphore with timeout of 0ms
then the thread acquires the token immediately

Test 0 tokens no timeout
Given thread and a semaphore with no tokens available
when thread calls @a wait on the semaphore with timeout of 0ms
then the thread returns immediately without acquiring a token
 */
template<int T>
void test_no_timeout()
{
    Semaphore sem(T);

    Timer timer;
    timer.start();

    int32_t cnt = sem.wait(0);
    TEST_ASSERT_EQUAL(T, cnt);

    TEST_ASSERT_UINT32_WITHIN(5000, 0, timer.read_us());
}

/** Test multiple tokens wait

    Given a thread and a semaphore initialized with 5 tokens
    when thread calls @a wait 6 times on the semaphore
    then the token counts goes to zero
 */
void test_multiple_tokens_wait()
{
    Semaphore sem(5);

    for(int i = 5; i >= 0; i--) {
        int32_t cnt = sem.wait(0);
        TEST_ASSERT_EQUAL(i, cnt);
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

    for(int i = 5; i > 0; i--) {
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
    Case("Test single thread", test_single_thread),
    Case("Test timeout", test_timeout),
    Case("Test 1 token no timeout", test_no_timeout<1>),
    Case("Test 0 tokens no timeout", test_no_timeout<0>),
    Case("Test multiple tokens wait", test_multiple_tokens_wait),
    Case("Test multiple tokens release", test_multiple_tokens_release),
    Case("Test multiple threads", test_multi)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
