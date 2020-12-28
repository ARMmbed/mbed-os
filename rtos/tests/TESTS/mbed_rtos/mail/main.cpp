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
#error [NOT_SUPPORTED] mail test cases require RTOS with multithread to run
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

#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
#define THREAD_STACK_SIZE   512
#elif defined(TARGET_ARM_FM)
#define THREAD_STACK_SIZE   512
#elif defined(TARGET_CY8CKIT_062_WIFI_BT_PSA)
#define THREAD_STACK_SIZE   512
#else
#define THREAD_STACK_SIZE   320 /* larger stack cause out of heap memory on some 16kB RAM boards in multi thread test*/
#endif
#define QUEUE_SIZE          16
#define THREAD_1_ID         1
#define THREAD_2_ID         2
#define THREAD_3_ID         3
#define QUEUE_PUT_DELAY_1   5ms
#define QUEUE_PUT_DELAY_2   50ms
#define QUEUE_PUT_DELAY_3   100ms
#define DATA_BASE           100


typedef struct {
    uint16_t data;
    uint8_t thread_id;
} mail_t;


void send_thread(Mail<mail_t, QUEUE_SIZE> *m, uint8_t thread_id, milliseconds wait, uint32_t send_count)
{
    uint32_t data = thread_id * DATA_BASE;

    for (uint32_t i = 0; i < send_count; i++) {
        mail_t *mail = m->try_alloc();
        mail->thread_id = thread_id;
        mail->data = data++;
        m->put(mail);
        ThisThread::sleep_for(wait);
    }
}

template<uint32_t queue_size>
void receive_thread(Mail<mail_t, queue_size> *m, uint8_t thread_id, milliseconds wait)
{
    int result_counter = 0;
    uint32_t data = thread_id * DATA_BASE;

    ThisThread::sleep_for(wait);
    for (uint32_t i = 0; i < queue_size; i++) {
        mail_t *mail = m->try_get_for(Kernel::wait_for_u32_forever);
        if (mail) {
            const uint8_t id = mail->thread_id;

            // verify thread id
            TEST_ASSERT_TRUE(id == thread_id);
            // verify sent data
            TEST_ASSERT_TRUE(mail->data == data++);

            m->free(mail);
            result_counter++;
        }
    }
    TEST_ASSERT_EQUAL(queue_size, result_counter);
}

/** Test single thread Mail usage and order

    Given mailbox and one additional thread
    When messages are put in to the Mail box by this thread
    Then messages are received in main thread in the same order as was sent and the data sent is valid
 */
void test_single_thread_order(void)
{
    uint16_t data = DATA_BASE;
    int result_counter = 0;
    Mail<mail_t, QUEUE_SIZE> mail_box;

    // mail send thread creation
    Thread thread(osPriorityNormal, THREAD_STACK_SIZE);
    thread.start([&] { send_thread(&mail_box, THREAD_1_ID, QUEUE_PUT_DELAY_1, QUEUE_SIZE); });

    // wait for some mail to be collected
    ThisThread::sleep_for(10ms);

    for (uint32_t i = 0; i < QUEUE_SIZE; i++) {
        // mail receive (main thread)
        mail_t *mail = mail_box.try_get_for(Kernel::wait_for_u32_forever);
        if (mail) {
            const uint8_t id = mail->thread_id;

            // verify thread id
            TEST_ASSERT_TRUE(id == THREAD_1_ID);
            // verify sent data
            TEST_ASSERT_TRUE(mail->data == data++);
            mail_box.free(mail);

            result_counter++;
        }
    }
    TEST_ASSERT_EQUAL(QUEUE_SIZE, result_counter);
}

/** Test multi thread Mail usage and order

    Given mailbox and three additional threads
    When messages are put in to the Mail box by these threads
    Then messages are received in main thread in the same per thread order as was sent and the data sent is valid
 */
void test_multi_thread_order(void)
{
    uint16_t data[4] = { 0, DATA_BASE, DATA_BASE * 2, DATA_BASE * 3 };
    int result_counter = 0;
    Mail<mail_t, QUEUE_SIZE> mail_box;

    // mail send threads creation
    Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
    thread1.start([&] { send_thread(&mail_box, THREAD_1_ID, QUEUE_PUT_DELAY_1, 7); });
    thread2.start([&] { send_thread(&mail_box, THREAD_2_ID, QUEUE_PUT_DELAY_2, 5); });
    thread3.start([&] { send_thread(&mail_box, THREAD_3_ID, QUEUE_PUT_DELAY_3, 4); });

    // wait for some mail to be collected
    ThisThread::sleep_for(10ms);

    for (uint32_t i = 0; i < QUEUE_SIZE; i++) {
        // mail receive (main thread)
        mail_t *mail = mail_box.try_get_for(Kernel::wait_for_u32_forever);
        if (mail) {
            const uint8_t id = mail->thread_id;

            // verify thread id
            TEST_ASSERT_TRUE((id == THREAD_1_ID) || (id == THREAD_2_ID) || (id == THREAD_3_ID));
            // verify sent data
            TEST_ASSERT_TRUE(mail->data == data[id]++);
            mail_box.free(mail);

            result_counter++;
        }
    }
    TEST_ASSERT_EQUAL(QUEUE_SIZE, result_counter);
}

/** Test multi thread multi Mail usage and order

    Given 3 mailbox and three additional threads
    When messages are put in to the mail boxes by main thread
    Then messages are received by threads in the same per mail box order as was sent and the data sent is valid
 */
void test_multi_thread_multi_mail_order(void)
{
    Mail<mail_t, 4> mail_box[4]; /* mail_box[0] not used */
    uint16_t data[4] = { 0, DATA_BASE, DATA_BASE * 2, DATA_BASE * 3 };
    mail_t *mail;
    uint8_t id;

    // mail receive threads creation
    Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
    thread1.start([&] { receive_thread<4>(mail_box + 1, THREAD_1_ID, 0ms); });
    thread2.start([&] { receive_thread<4>(mail_box + 2, THREAD_2_ID, 10ms); });
    thread3.start([&] { receive_thread<4>(mail_box + 3, THREAD_3_ID, 100ms); });

    for (uint32_t i = 0; i < 4; i++) {
        id = THREAD_1_ID;
        mail = mail_box[id].try_alloc();
        mail->thread_id = id;
        mail->data = data[id]++;
        mail_box[id].put(mail);

        id = THREAD_2_ID;
        mail = mail_box[id].try_alloc();
        mail->thread_id = id;
        mail->data = data[id]++;
        mail_box[id].put(mail);

        id = THREAD_3_ID;
        mail = mail_box[id].try_alloc();
        mail->thread_id = id;
        mail->data = data[id]++;
        mail_box[id].put(mail);

        ThisThread::sleep_for(i * 10ms);
    }

    thread1.join();
    thread2.join();
    thread3.join();
}

/** Test message memory deallocation with block out of the scope

    Given an empty mailbox
    When try to free out of the scope memory block
    Then it return appropriate error code
 */
void test_free_wrong()
{
    osStatus status;
    Mail<uint32_t, 4> mail_box;
    uint32_t *mail, data;

    mail = &data;
    status = mail_box.free(mail);
    TEST_ASSERT_EQUAL(osErrorParameter, status);

    mail = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail);

    mail = &data;
    status = mail_box.free(mail);
    TEST_ASSERT_EQUAL(osErrorParameter, status);
}

/** Test message memory deallocation with null block

    Given an empty mailbox
    When try to free null ptr
    Then it return appropriate error code
 */
void test_free_null()
{
    osStatus status;
    Mail<uint32_t, 4> mail_box;
    uint32_t *mail;

    mail = NULL;
    status = mail_box.free(mail);
    TEST_ASSERT_EQUAL(osErrorParameter, status);

    mail = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail);

    mail = NULL;
    status = mail_box.free(mail);
    TEST_ASSERT_EQUAL(osErrorParameter, status);
}

/** Test get from empty mailbox with timeout set

    Given an empty mailbox
    When @a try_get_for is called on the mailbox with timeout of 50ms
    Then mailbox returns no data
 */
void test_get_empty_timeout()
{
    Mail<uint32_t, 4> mail_box;
    Timer timer;

    timer.start();
    uint32_t *mail = mail_box.try_get_for(50ms);
    TEST_ASSERT_DURATION_WITHIN(5ms, 50ms, timer.elapsed_time());
    TEST_ASSERT_NULL(mail);
}

/** Test get from empty mailbox with 0 timeout

    Given an empty mailbox
    When @a try_get is called on the mailbox
    Then mailbox returns no data
 */
void test_get_empty_no_timeout()
{
    Mail<uint32_t, 4> mail_box;

    uint32_t *mail = mail_box.try_get();
    TEST_ASSERT_NULL(mail);
}

/** Test mail order

    Given an mailbox for uint32_t values
    Then allocate two mails and put them in to mailbox
    When call @a get it returns previously put mails
    Then mails should be in the same order as put
 */
void test_order(void)
{
    osStatus status;
    Mail<int32_t, 4> mail_box;
    const int32_t TEST_VAL1 = 123;
    const int32_t TEST_VAL2 = 456;

    int32_t *mail1 = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail1);

    *mail1 = TEST_VAL1;
    mail_box.put(mail1);

    int32_t *mail2 = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail2);

    *mail2 = TEST_VAL2;
    mail_box.put(mail2);


    mail1 = mail_box.try_get_for(Kernel::wait_for_u32_forever);
    TEST_ASSERT_NOT_NULL(mail1);
    TEST_ASSERT_EQUAL(TEST_VAL1, *mail1);

    mail2 = mail_box.try_get_for(Kernel::wait_for_u32_forever);
    TEST_ASSERT_NOT_NULL(mail2);
    TEST_ASSERT_EQUAL(TEST_VAL2, *mail2);


    status = mail_box.free(mail1);
    TEST_ASSERT_EQUAL(osOK, status);

    status = mail_box.free(mail2);
    TEST_ASSERT_EQUAL(osOK, status);
}

/** Test Mail box max size limit

    Given an Mail box with max size of 4 elements
    When call @a try_alloc four times it returns memory blocks
    Then the memory blocks should be valid
    When call @a try_alloc one more time it returns memory blocks
    Then the memory blocks should be not valid (NULL - no memory available)
 */
void test_max_size()
{
    osStatus status;
    Mail<uint32_t, 4> mail_box;

    // 1 OK
    uint32_t *mail1 = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail1);

    // 2 OK
    uint32_t *mail2 = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail2);

    // 3 OK
    uint32_t *mail3 = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail3);

    // 4 OK
    uint32_t *mail4 = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail4);

    // 5 KO
    uint32_t *mail5 = mail_box.try_alloc();
    TEST_ASSERT_EQUAL(NULL, mail5);


    status = mail_box.free(mail1);
    TEST_ASSERT_EQUAL(osOK, status);

    status = mail_box.free(mail2);
    TEST_ASSERT_EQUAL(osOK, status);

    status = mail_box.free(mail3);
    TEST_ASSERT_EQUAL(osOK, status);

    status = mail_box.free(mail4);
    TEST_ASSERT_EQUAL(osOK, status);
}

/** Test mailbox of T type data

    Given an mailbox with T memory block type
    When allocate/put/get/free memory block
    Then all operations should succeed
 */
template<typename T>
void test_data_type(void)
{
    osStatus status;
    Mail<T, 4> mail_box;
    const T TEST_VAL = 123;

    T *mail = mail_box.try_alloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail);

    *mail = TEST_VAL;
    mail_box.put(mail);

    mail = mail_box.try_get_for(Kernel::wait_for_u32_forever);
    TEST_ASSERT_NOT_NULL(mail);
    TEST_ASSERT_EQUAL(TEST_VAL, *mail);


    status = mail_box.free(mail);
    TEST_ASSERT_EQUAL(osOK, status);
}

/** Test try_calloc - memory block allocation with resetting

    Given an empty Mail box
    When call @a try_calloc it returns allocated memory block
    Then the memory block should be valid and filled with zeros
 */
void test_calloc()
{
    Mail<uint32_t, 1> mail_box;

    uint32_t *mail = mail_box.try_calloc();
    TEST_ASSERT_NOT_EQUAL(NULL, mail);
    TEST_ASSERT_EQUAL(0, *mail);
}

/** Test mail empty

    Given a mail of uint32_t data
    before data is inserted the mail should be empty
    after data is inserted the mail shouldn't be empty
 */
void test_mail_empty()
{
    Mail<mail_t, 1> m;

    mail_t *mail = m.try_alloc();

    TEST_ASSERT_EQUAL(true,  m.empty());

    m.put(mail);

    TEST_ASSERT_EQUAL(false, m.empty());
}

/** Test mail empty

    Given a mail of uint32_t data with size of 1
    before data is inserted the mail shouldn't be full
    after data is inserted the mail should be full
 */
void test_mail_full()
{
    Mail<mail_t, 1> m;

    mail_t *mail = m.try_alloc();

    TEST_ASSERT_EQUAL(false,  m.full());

    m.put(mail);

    TEST_ASSERT_EQUAL(true, m.full());
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test try_calloc", test_calloc),
    Case("Test message type uint8", test_data_type<uint8_t>),
    Case("Test message type uint16", test_data_type<uint16_t>),
    Case("Test message type uint32", test_data_type<uint32_t>),
    Case("Test mailbox max size", test_max_size),
    Case("Test message send order", test_order),
    Case("Test get with timeout on empty mailbox", test_get_empty_timeout),
    Case("Test get without timeout on empty mailbox", test_get_empty_no_timeout),
    Case("Test null message free", test_free_null),
    Case("Test invalid message free", test_free_wrong),
    Case("Test message send/receive single thread and order", test_single_thread_order),
    Case("Test message send/receive multi-thread and per thread order", test_multi_thread_order),
    Case("Test message send/receive multi-thread, multi-Mail and per thread order", test_multi_thread_multi_mail_order),
    Case("Test mail empty", test_mail_empty),
    Case("Test mail full", test_mail_full)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
#endif // defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
