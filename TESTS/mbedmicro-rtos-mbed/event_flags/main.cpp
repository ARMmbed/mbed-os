/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

using utest::v1::Case;

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#if !DEVICE_USTICKER
  #error [NOT_SUPPORTED] test not supported
#endif

#define THREAD_STACK_SIZE   320 /* 512B stack on GCC_ARM compiler cause out of memory on some 16kB RAM boards e.g. NUCLEO_F070RB */

#define MAX_FLAG_POS 30
#define PROHIBITED_FLAG_POS 31

/* flags */
#define FLAG01          0x1FFF       /* 00000000000000000001111111111111 */
#define FLAG02          0x3FFE000    /* 00000011111111111110000000000000 */
#define FLAG03          0x7C000000   /* 01111100000000000000000000000000 */
#define PROHIBITED_FLAG 0x80000000   /* 10000000000000000000000000000000 */
#define NO_FLAGS 0x0

Semaphore sync_sem(0, 1);

/* In order to successfully run this test suite when compiled with --profile=debug
 * error() has to be redefined as noop.
 *
 * EventFlags calls RTX API which uses Event Recorder functionality. When compiled
 * with MBED_TRAP_ERRORS_ENABLED=1 (set in debug profile) EvrRtxEventFlagsError() calls error()
 * which aborts test program.
 */
#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED
void error(const char* format, ...) {
    (void) format;
}

//Override the set_error function to trap the errors 
mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number) 
{
    return MBED_SUCCESS;
}
#endif

template<uint32_t flags, uint32_t wait_ms>
void send_thread(EventFlags *ef)
{
    for (uint32_t i = 0; i <= MAX_FLAG_POS; i++) {
        const uint32_t flag = flags & (1 << i);
        if (flag) {
            ef->set(flag);
            Thread::wait(wait_ms);
        }
    }
}

template<uint32_t flags, uint32_t wait_ms>
void send_thread_sync(EventFlags *ef)
{
    for (uint32_t i = 0; i <= MAX_FLAG_POS; i++) {
        const uint32_t flag = flags & (1 << i);
        if (flag) {
            sync_sem.wait();
            ef->set(flag);
            Thread::wait(wait_ms);
        }
    }
}

template<uint32_t flags>
void wait_thread_all(EventFlags *ef)
{
    uint32_t ret, flags_after_clear;
    ret = ef->wait_all(flags);
    flags_after_clear = ef->get();
    TEST_ASSERT(flags | ret);
    TEST_ASSERT(flags | ~flags_after_clear);
}


/** Test if get on empty EventFlags object return NO_FLAGS

    Given a empty EventFlags object
    When call @a get
    Then @a get return status is NO_FLAGS
 */
void test_empty_get(void)
{
    EventFlags ev;
    uint32_t flags;

    flags = ev.get();
    TEST_ASSERT_EQUAL(NO_FLAGS, flags);
}

/** Test if clear on empty EventFlags object return NO_FLAGS

    Given a empty EventFlags object
    When call @a clear(NO_FLAGS)
    Then @a clear return status is NO_FLAGS
 */
void test_empty_clear(void)
{
    EventFlags ev;
    uint32_t flags;

    flags = ev.clear(NO_FLAGS);
    TEST_ASSERT_EQUAL(NO_FLAGS, flags);
}

/** Test if set on empty EventFlags object return NO_FLAGS

    Given a empty EventFlags object
    When call @a set(NO_FLAGS)
    Then @a set return status is NO_FLAGS
 */
void test_empty_set(void)
{
    EventFlags ev;
    uint32_t flags;

    flags = ev.set(NO_FLAGS);
    TEST_ASSERT_EQUAL(NO_FLAGS, flags);
}

/** Test if call of set/clean with PROHIBITED_FLAG doesn't invalidates object flags

    Given a EventFlags object with all flags already set
    When call @a clear(PROHIBITED_FLAG) with prohibited flag
    Then @a clear return status is osFlagsErrorParameter and object flags stays unchanged
    When call @a set(PROHIBITED_FLAG) with prohibited flag
    Then @a set return status is osFlagsErrorParameter and object flags stays unchanged

    @note Each signal has up to 31 event flags 0x1, 0x2, 0x4, 0x8, ...,  0x40000000
          Most significant bit is reserved and thereby flag 0x80000000 is prohibited
 */
void test_prohibited(void)
{
    EventFlags ev;
    uint32_t flags;

    ev.set(FLAG01 | FLAG02 | FLAG03);

    flags = ev.clear(PROHIBITED_FLAG);
    TEST_ASSERT_EQUAL(osFlagsErrorParameter, flags);

    flags = ev.get();
    TEST_ASSERT_EQUAL(FLAG01 | FLAG02 | FLAG03, flags);

    flags = ev.set(PROHIBITED_FLAG);
    TEST_ASSERT_EQUAL(osFlagsErrorParameter, flags);

    flags = ev.get();
    TEST_ASSERT_EQUAL(FLAG01 | FLAG02 | FLAG03, flags);
}

/** Test set/get/clear for full flag range

    Given a EventFlags object
    When call @a clear
    Then @a clear return status is already set flags
    When call @a set with specified flag
    Then @a set return status is flags after setting
    When call @a get
    Then @a get return status is set flags
 */
void test_set_get_clear_full_flag_range(void)
{
    EventFlags ev;
    uint32_t flag, flags, ret;

    flags = NO_FLAGS;
    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        ret = ev.clear();
        TEST_ASSERT_EQUAL(flags, ret);
        flags = 1 << i;
        ret = ev.set(flags);
        TEST_ASSERT_EQUAL(flags, ret);
        ret = ev.get();
        TEST_ASSERT_EQUAL(flags, ret);
    }

    ev.clear();
    flags = NO_FLAGS;
    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        ret = ev.clear(NO_FLAGS);
        TEST_ASSERT_EQUAL(flags, ret);
        flag = 1 << i;
        flags |= flag;
        ret = ev.set(flag);
        TEST_ASSERT_EQUAL(flags, ret);
        ret = ev.get();
        TEST_ASSERT_EQUAL(flags, ret);
    }
}

/** Test if multi-threaded flag set cause wait_all to return

    Given a EventFlags object and three threads are started in parallel
    When threads set specified flags
    Then main thread waits until receive all of them
 */
void test_multi_thread_all(void)
{
    EventFlags ef;
    Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
    thread1.start(callback(send_thread<FLAG01, 1>, &ef));
    thread2.start(callback(send_thread<FLAG02, 2>, &ef));
    thread3.start(callback(send_thread<FLAG03, 3>, &ef));

    uint32_t ret = ef.wait_all(FLAG01 | FLAG02 | FLAG03);
    TEST_ASSERT_EQUAL(FLAG01 | FLAG02 | FLAG03, ret);
}

/** Test if multi-threaded flag set cause wait_any to return

    Given a EventFlags object and three threads are started in parallel
    When threads set specified flags
    Then main thread waits until receive all of them
 */
void test_multi_thread_any(void)
{
    EventFlags ef;
    uint32_t ret;
    Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
    thread1.start(callback(send_thread<FLAG01, 1>, &ef));
    thread2.start(callback(send_thread<FLAG02, 1>, &ef));
    thread3.start(callback(send_thread<FLAG03, 1>, &ef));

    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        uint32_t flag = 1 << i;
        ret = ef.wait_any(flag);
        TEST_ASSERT(flag | ret);
    }
    ret = ef.get();
    TEST_ASSERT_EQUAL(NO_FLAGS, ret);
}

/** Test if multi-threaded flag set cause wait_any(with timeout) to return

    Given a EventFlags object and thread is running
    When main thread call @ wait_any with timeout
    Then when timeout expires @ wait_any return status is osFlagsErrorTimeout
    When main thread call @ wait_any with timeout and thread set specified flags
    Then main thread waits until receive all of them and @ wait_any return status is wait flag
 */
void test_multi_thread_any_timeout(void)
{
    EventFlags ef;
    uint32_t ret;
    Thread thread(osPriorityNormal, THREAD_STACK_SIZE);
    thread.start(callback(send_thread_sync<FLAG01 | FLAG02 | FLAG03, 1>, &ef));

    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        uint32_t flag = 1 << i;

        ret = ef.wait_any(flag, 10);
        TEST_ASSERT_EQUAL(osFlagsErrorTimeout, ret);

        sync_sem.release();
        ret = ef.wait_any(flag, 10);
        TEST_ASSERT_EQUAL(flag, ret);
    }
    ret = ef.get();
    TEST_ASSERT_EQUAL(NO_FLAGS, ret);
}

/** Test if multi-threaded flag set cause wait_any(without clear) to return

    Given a EventFlags object and three threads are started in parallel
    When threads set specified flags
    Then main thread waits until receive all of them
 */
void test_multi_thread_any_no_clear(void)
{
    EventFlags ef;
    uint32_t ret;
    Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
    thread1.start(callback(send_thread<FLAG01, 1>, &ef));
    thread2.start(callback(send_thread<FLAG02, 1>, &ef));
    thread3.start(callback(send_thread<FLAG03, 1>, &ef));

    for (int i = 0; i <= MAX_FLAG_POS; i++) {
        uint32_t flag = 1 << i;
        ret = ef.wait_any(flag, osWaitForever, false);
        TEST_ASSERT(flag | ret);
        ret = ef.clear(flag);
        TEST_ASSERT(ret < osFlagsError);
    }
    ret = ef.get();
    TEST_ASSERT_EQUAL(NO_FLAGS, ret);
}

/** Test multi-threaded wait_any

    Given a EventFlags object and three threads are started in parallel
    When flags are set in main thread
    Then other threads waits until receive all of them
 */
void test_multi_thread_all_many_wait(void)
{
    EventFlags ef;
    {
        Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
        Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
        Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
        thread1.start(callback(wait_thread_all<FLAG01>, &ef));
        thread2.start(callback(wait_thread_all<FLAG02>, &ef));
        thread3.start(callback(wait_thread_all<FLAG03>, &ef));

        ef.set(FLAG01 | FLAG02 | FLAG03);
        thread1.join();
        thread2.join();
        thread3.join();
        TEST_ASSERT_EQUAL(NO_FLAGS, ef.get());
    }

    {
        Thread thread1(osPriorityNormal, THREAD_STACK_SIZE);
        Thread thread2(osPriorityNormal, THREAD_STACK_SIZE);
        Thread thread3(osPriorityNormal, THREAD_STACK_SIZE);
        thread1.start(callback(wait_thread_all<FLAG01>, &ef));
        thread2.start(callback(wait_thread_all<FLAG02>, &ef));
        thread3.start(callback(wait_thread_all<FLAG03>, &ef));

        ef.set(FLAG01);
        thread1.join();
        ef.set(FLAG02);
        thread2.join();
        ef.set(FLAG03);
        thread3.join();
        TEST_ASSERT_EQUAL(NO_FLAGS, ef.get());
    }
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test empty clear", test_empty_clear),
    Case("Test empty get", test_empty_get),
    Case("Test empty set", test_empty_set),
    Case("Test clear/set with prohibited flag", test_prohibited),
    Case("Test set/get/clear for full flag range", test_set_get_clear_full_flag_range),
    Case("Test multi-threaded wait_all", test_multi_thread_all),
    Case("Test multi-threaded wait_any", test_multi_thread_any),
    Case("Test multi-threaded wait_all many wait", test_multi_thread_all_many_wait),
    Case("Test multi-threaded wait_any timeout", test_multi_thread_any_timeout),
    Case("Test multi-threaded wait_any no clear", test_multi_thread_any_no_clear)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
