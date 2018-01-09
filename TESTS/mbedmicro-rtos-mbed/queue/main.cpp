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

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

#define THREAD_STACK_SIZE 512
#define TEST_UINT_MSG 0xDEADBEEF
#define TEST_UINT_MSG2 0xE1EE7
#define TEST_TIMEOUT 50

template <uint32_t ms>
void thread_put_uint_msg(Queue<uint32_t, 1> *q)
{
    Thread::wait(ms);
    osStatus stat = q->put((uint32_t*) TEST_UINT_MSG);
    TEST_ASSERT_EQUAL(osOK, stat);
}

template <uint32_t ms, uint32_t val>
void thread_get_uint_msg(Queue<uint32_t, 1> *q)
{
    Thread::wait(ms);
    osEvent evt = q->get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(val, evt.value.v);
}

/** Test pass uint msg

    Given a queue for uint32_t messages with one slot
    When a uin32_t value is inserted into the queue
        and a message is extracted from the queue
    Then the extracted message is the same as previously inserted message
 */
void test_pass_uint()
{
    Queue<uint32_t, 1> q;
    osStatus stat = q.put((uint32_t*)TEST_UINT_MSG);
    TEST_ASSERT_EQUAL(osOK, stat);

    osEvent evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG, evt.value.v);
}

/** Test pass uint msg twice

    Given a queue for uint32_t messages with one slot
    When a uin32_t value is inserted into the queue
        and a message is extracted from the queue
        and the procedure is repeated with different message
    Then the extracted message is the same as previously inserted message for both iterations

 */
void test_pass_uint_twice()
{
    Queue<uint32_t, 1> q;
    osStatus stat = q.put((uint32_t*)TEST_UINT_MSG);
    TEST_ASSERT_EQUAL(osOK, stat);

    osEvent evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG, evt.value.v);

    stat = q.put((uint32_t*)TEST_UINT_MSG2);
    TEST_ASSERT_EQUAL(osOK, stat);

    evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG2, evt.value.v);
}

/** Test pass ptr msg

    Given a queue for pointers to uint32_t messages with one slot
    When a pointer to an uint32_t is inserted into the queue
        and a message is extracted from the queue
    Then the extracted message is the same as previously inserted message
 */
void test_pass_ptr()
{
    Queue<uint32_t, 1> q;
    uint32_t msg = TEST_UINT_MSG;

    osStatus stat = q.put(&msg);
    TEST_ASSERT_EQUAL(osOK, stat);

    osEvent evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(&msg, evt.value.p);
}

/** Test get from empty queue

    Given an empty queue for uint32_t values
    When @a get is called on the queue with timeout of 0
    Then queue returns status of osOK, but no data
 */
void test_get_empty_no_timeout()
{
    Queue<uint32_t, 1> q;

    osEvent evt = q.get(0);
    TEST_ASSERT_EQUAL(osOK, evt.status);
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

    osEvent evt = q.get(50);
    TEST_ASSERT_EQUAL(osEventTimeout, evt.status);
    TEST_ASSERT_UINT32_WITHIN(5000, 50000, timer.read_us());
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

    t.start(callback(thread_put_uint_msg<TEST_TIMEOUT>, &q));

    Timer timer;
    timer.start();

    osEvent evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG, evt.value.v);
    TEST_ASSERT_UINT32_WITHIN(TEST_TIMEOUT * 100, TEST_TIMEOUT * 1000, timer.read_us());
}

/** Test put full no timeout
 *
 * Given a queue with one slot for uint32_t data
 * When a thread tries to insert two messages
 * Then first operation succeeds and second fails with @a osErrorResource
 */
void test_put_full_no_timeout()
{
    Queue<uint32_t, 1> q;

    osStatus stat = q.put((uint32_t*) TEST_UINT_MSG);
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = q.put((uint32_t*) TEST_UINT_MSG);
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test put full timeout
 *
 * Given a queue with one slot for uint32_t data
 * When a thread tries to insert two messages with @ TEST_TIMEOUT timeout
 * Then first operation succeeds and second fails with @a osErrorTimeout
 */
void test_put_full_timeout()
{
    Queue<uint32_t, 1> q;

    osStatus stat = q.put((uint32_t*) TEST_UINT_MSG, TEST_TIMEOUT);
    TEST_ASSERT_EQUAL(osOK, stat);

    Timer timer;
    timer.start();

    stat = q.put((uint32_t*) TEST_UINT_MSG, TEST_TIMEOUT);
    TEST_ASSERT_EQUAL(osErrorTimeout, stat);
    TEST_ASSERT_UINT32_WITHIN(TEST_TIMEOUT * 100, TEST_TIMEOUT * 1000, timer.read_us());
}

/** Test put full wait forever
 *
 * Given two threads A & B and a queue with one slot for uint32_t data
 * When thread A puts a message to the queue and tries to put second one with @a osWaitForever timeout
 * Then thread waits for a slot to become empty in the queue
 * When thread B takes one message out of the queue
 * Then thread A successfully inserts message into the queue
 */
void test_put_full_waitforever()
{
    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Queue<uint32_t, 1> q;

    t.start(callback(thread_get_uint_msg<TEST_TIMEOUT, TEST_UINT_MSG>, &q));

    osStatus stat = q.put((uint32_t*) TEST_UINT_MSG);
    TEST_ASSERT_EQUAL(osOK, stat);

    Timer timer;
    timer.start();
    stat = q.put((uint32_t*) TEST_UINT_MSG, osWaitForever);
    TEST_ASSERT_EQUAL(osOK, stat);
    TEST_ASSERT_UINT32_WITHIN(TEST_TIMEOUT * 100, TEST_TIMEOUT * 1000, timer.read_us());

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

    osStatus stat = q.put((uint32_t*) TEST_UINT_MSG, TEST_TIMEOUT);
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = q.put((uint32_t*) TEST_UINT_MSG2, TEST_TIMEOUT);
    TEST_ASSERT_EQUAL(osOK, stat);

    osEvent evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG, evt.value.v);

    evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG2, evt.value.v);
}

/** Test message priority

    Given a queue of uint32_t data
    When two messages are inserted with ascending priority
    Then messages should be returned in descending priority order
 */
void test_msg_prio()
{
    Queue<uint32_t, 2> q;

    osStatus stat = q.put((uint32_t*) TEST_UINT_MSG, TEST_TIMEOUT, 0);
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = q.put((uint32_t*) TEST_UINT_MSG2, TEST_TIMEOUT, 1);
    TEST_ASSERT_EQUAL(osOK, stat);

    osEvent evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG2, evt.value.v);

    evt = q.get();
    TEST_ASSERT_EQUAL(osEventMessage, evt.status);
    TEST_ASSERT_EQUAL(TEST_UINT_MSG, evt.value.v);
}

/** Test queue empty

    Given a queue of uint32_t data
    before data is inserted the queue should be empty
    after data is inserted the queue shouldn't be empty
 */
void test_queue_empty()
{
    Queue<uint32_t, 1> q;

    TEST_ASSERT_EQUAL(true, q.empty());

    q.put((uint32_t*) TEST_UINT_MSG, TEST_TIMEOUT, 1);

    TEST_ASSERT_EQUAL(false, q.empty());
}

/** Test queue empty

    Given a queue of uint32_t data with size of 1
    before data is inserted the queue shouldn't be full
    after data is inserted the queue should be full
 */
void test_queue_full()
{
    Queue<uint32_t, 1> q;

    TEST_ASSERT_EQUAL(false, q.full());

    q.put((uint32_t*) TEST_UINT_MSG, TEST_TIMEOUT, 1);

    TEST_ASSERT_EQUAL(true, q.full());
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test pass uint msg", test_pass_uint),
    Case("Test pass uint msg twice", test_pass_uint_twice),
    Case("Test pass ptr msg", test_pass_ptr),
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
