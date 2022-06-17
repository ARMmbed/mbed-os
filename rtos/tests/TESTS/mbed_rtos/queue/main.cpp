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
#error [NOT_SUPPORTED] Queue test cases require RTOS with multithread to run
#else

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

#define TEST_ASSERT_DURATION_WITHIN(delta, expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(delta), decltype(expected), decltype(actual)>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected).count(), ct(actual).count()); \
    } while (0)

#define THREAD_STACK_SIZE 512
#define TEST_TIMEOUT 50ms

static uint32_t msg;
static uint32_t msg2;

void thread_put_uint_msg(Queue<uint32_t, 1> *q)
{
    ThisThread::sleep_for(TEST_TIMEOUT);
    bool stat = q->try_put(&msg);
    TEST_ASSERT_TRUE(stat);
}

void thread_get_uint_msg(Queue<uint32_t, 1> *q)
{
    ThisThread::sleep_for(TEST_TIMEOUT);
    uint32_t *v;
    bool stat = q->try_get_for(Kernel::wait_for_u32_forever, &v);
    TEST_ASSERT_TRUE(stat)
    TEST_ASSERT_EQUAL(&msg, v);
}

/** Test pass msg

    Given a queue for uint32_t messages with one slot
    When a uin32_t value is inserted into the queue
        and a message is extracted from the queue
    Then the extracted message is the same as previously inserted message
 */
void test_pass()
{
    Queue<uint32_t, 1> q;
    bool stat = q.try_put(&msg);
    TEST_ASSERT_TRUE(stat)

    uint32_t *v;
    stat = q.try_get_for(Kernel::wait_for_u32_forever, &v);
    TEST_ASSERT_TRUE(stat)
    TEST_ASSERT_EQUAL(&msg, v);
}

/** Test pass msg twice

    Given a queue for uint32_t messages with one slot
    When a uin32_t value is inserted into the queue
        and a message is extracted from the queue
        and the procedure is repeated with different message
    Then the extracted message is the same as previously inserted message for both iterations

 */
void test_pass_twice()
{
    Queue<uint32_t, 1> q;
    bool stat = q.try_put(&msg);
    TEST_ASSERT_TRUE(stat);

    uint32_t *v;
    stat = q.try_get_for(Kernel::wait_for_u32_forever, &v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg, v);

    stat = q.try_put(&msg2);
    TEST_ASSERT_TRUE(stat);

    stat = q.try_get_for(Kernel::wait_for_u32_forever, &v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg2, v);
}

/** Test get from empty queue

    Given an empty queue for uint32_t values
    When @a get is called on the queue with timeout of 0
    Then queue returns status of false
 */
void test_get_empty_no_timeout()
{
    Queue<uint32_t, 1> q;

    uint32_t *v;
    bool stat = q.try_get(&v);
    TEST_ASSERT_FALSE(stat);
}

/** Test get from empty queue with timeout

    Given an empty queue for uint32_t values
    When @a get is called on the queue with timeout of 50ms
    Then queue returns status of osEventTimeout after about 50ms wait
 */
void test_get_empty_timeout()
{
    Queue<uint32_t, 1> q;
    Timer timer;
    timer.start();

    uint32_t *v;
    bool stat = q.try_get_for(50ms, &v);
    TEST_ASSERT_FALSE(stat);
    TEST_ASSERT_DURATION_WITHIN(5ms, 50ms, timer.elapsed_time());
}

/** Test get empty wait forever

    Given a two threads A & B and a queue for uint32_t values
    When thread A calls @a get on an empty queue with osWaitForever
    Then the thread A waits for a message to appear in the queue
    When thread B puts a message in the queue
    Then thread A wakes up and receives it
 */
void test_get_empty_wait_forever()
{
    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Queue<uint32_t, 1> q;

    t.start(callback(thread_put_uint_msg, &q));

    Timer timer;
    timer.start();

    uint32_t *v;
    bool stat = q.try_get_for(Kernel::wait_for_u32_forever, &v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg, v);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, TEST_TIMEOUT, timer.elapsed_time());
}

/** Test put full no timeout
 *
 * Given a queue with one slot for uint32_t data
 * When a thread tries to insert two messages
 * Then first operation succeeds and second fails
 */
void test_put_full_no_timeout()
{
    Queue<uint32_t, 1> q;

    bool stat = q.try_put(&msg);
    TEST_ASSERT_TRUE(stat);

    stat = q.try_put(&msg);
    TEST_ASSERT_FALSE(stat);
}

/** Test put full timeout
 *
 * Given a queue with one slot for uint32_t data
 * When a thread tries to insert two messages with @ TEST_TIMEOUT timeout
 * Then first operation succeeds and second fails
 */
void test_put_full_timeout()
{
    Queue<uint32_t, 1> q;

    bool stat = q.try_put_for(TEST_TIMEOUT, &msg);
    TEST_ASSERT_TRUE(stat);

    Timer timer;
    timer.start();

    stat = q.try_put_for(TEST_TIMEOUT, &msg);
    TEST_ASSERT_FALSE(stat);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, TEST_TIMEOUT, timer.elapsed_time());
}

/** Test put full wait forever
 *
 * Given two threads A & B and a queue with one slot for uint32_t data
 * When thread A puts a message to the queue and tries to put second one with @a Kernel::wait_for_u32_forever timeout
 * Then thread waits for a slot to become empty in the queue
 * When thread B takes one message out of the queue
 * Then thread A successfully inserts message into the queue
 */
void test_put_full_waitforever()
{
    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Queue<uint32_t, 1> q;

    t.start(callback(thread_get_uint_msg, &q));

    bool stat = q.try_put(&msg);
    TEST_ASSERT_TRUE(stat);

    Timer timer;
    timer.start();
    stat = q.try_put_for(Kernel::wait_for_u32_forever, &msg);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_DURATION_WITHIN(TEST_TIMEOUT / 10, TEST_TIMEOUT, timer.elapsed_time());

    t.join();
}

/** Test message ordering

    Given a queue of uint32_t data
    When two messages are inserted with equal priority
    Then messages should be returned in the exact order they were inserted
 */
void test_msg_order()
{
    Queue<uint32_t, 2> q;

    bool stat = q.try_put_for(TEST_TIMEOUT, &msg);
    TEST_ASSERT_TRUE(stat);

    stat = q.try_put_for(TEST_TIMEOUT, &msg2);
    TEST_ASSERT_TRUE(stat);

    uint32_t *v;
    stat = q.try_get(&v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg, v);

    stat = q.try_get(&v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg2, v);
}

/** Test message priority

    Given a queue of uint32_t data
    When two messages are inserted with ascending priority
    Then messages should be returned in descending priority order
 */
void test_msg_prio()
{
    Queue<uint32_t, 2> q;

    bool stat = q.try_put_for(TEST_TIMEOUT, &msg, 0);
    TEST_ASSERT_TRUE(stat);

    stat = q.try_put_for(TEST_TIMEOUT, &msg2, 1);
    TEST_ASSERT_TRUE(stat);

    uint32_t *v;
    stat = q.try_get(&v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg2, v);

    stat = q.try_get(&v);
    TEST_ASSERT_TRUE(stat);
    TEST_ASSERT_EQUAL(&msg, v);
}

/** Test queue empty

    Given a queue of uint32_t data
    before data is inserted the queue should be empty
    after data is inserted the queue shouldn't be empty
 */
void test_queue_empty()
{
    Queue<uint32_t, 1> q;

    TEST_ASSERT_TRUE(q.empty());

    q.try_put_for(TEST_TIMEOUT, &msg, 1);

    TEST_ASSERT_FALSE(q.empty());
}

/** Test queue empty

    Given a queue of uint32_t data with size of 1
    before data is inserted the queue shouldn't be full
    after data is inserted the queue should be full
 */
void test_queue_full()
{
    Queue<uint32_t, 1> q;

    TEST_ASSERT_FALSE(q.full());

    q.try_put_for(TEST_TIMEOUT, &msg, 1);

    TEST_ASSERT_TRUE(q.full());
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test pass msg", test_pass),
    Case("Test pass msg twice", test_pass_twice),
    Case("Test get from empty queue no timeout", test_get_empty_no_timeout),
    Case("Test get from empty queue timeout", test_get_empty_timeout),
    Case("Test get empty wait forever", test_get_empty_wait_forever),
    Case("Test put full no timeout", test_put_full_no_timeout),
    Case("Test put full timeout", test_put_full_timeout),
    Case("Test put full wait forever", test_put_full_waitforever),
    Case("Test message ordering", test_msg_order),
    Case("Test message priority", test_msg_prio),
    Case("Test queue empty", test_queue_empty),
    Case("Test queue full", test_queue_full)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
#endif // defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
