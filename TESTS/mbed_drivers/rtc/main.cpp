/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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
#include "unity.h"
#include "utest.h"
#include "rtc_api.h"

#if !DEVICE_RTC || !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#else

using namespace utest::v1;

/* On some boards RTC counter can not be
 * Initialised with 0 value in such case
 * drivers sets RTC counter to 1. */
#define CUSTOM_TIME_0  1
#define CUSTOM_TIME_1  1256729737
#define CUSTOM_TIME_2  2147483637

#define DELAY_10_SEC   10
#define MS_PER_SEC     1000
#define RTC_DELTA      1

static volatile int rtc_enabled_ret;
static volatile time_t rtc_time_val;
static volatile bool rtc_read_called;
static volatile bool rtc_write_called;
static volatile bool rtc_init_called;
static volatile bool rtc_isenabled_called;

/* Stub of RTC read function. */
static time_t read_rtc_stub(void)
{
    rtc_read_called = true;

    return rtc_time_val;
}

/* Stub of RTC write function. */
static void write_rtc_stub(time_t t)
{
    rtc_write_called = true;

    rtc_time_val = t;
}

/* Stub of RTC init function. */
static void init_rtc_stub(void)
{
    rtc_init_called = true;
}

/* Stub of RTC isenabled function. */
static int isenabled_rtc_stub(void)
{
    rtc_isenabled_called = true;

    return rtc_enabled_ret;
}

/* This test verifies if attach_rtc provides availability to
 * connect specific RTC driver functions.
 *
 * This is unit test to verify if correct functions are used
 * to support RTC.
 *
 * Given specific RTC driver functions have been attached (stubs).
 * When set_time/time functions are called.
 * Then set_time/time functions use attached RTC functions.
 */
void test_attach_RTC_stub_funtions()
{
    time_t seconds = 0;

    /* Attache RTC read/write/init/isenabled stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, init_rtc_stub, isenabled_rtc_stub);

    /* Init stub variables/set to unexpected. */
    rtc_write_called = false;
    rtc_init_called = false;

    /* Call set_time() function. We expect that init and write RTC stubs
     * will be executed.
     */
    set_time(CUSTOM_TIME_1);

    /* Verify results. */
    TEST_ASSERT_EQUAL(true, rtc_write_called);
    TEST_ASSERT_EQUAL(true, rtc_init_called);

    /* Init stub variables/set to unexpected. */
    rtc_time_val = CUSTOM_TIME_1;
    rtc_enabled_ret = true;
    rtc_isenabled_called = false;
    rtc_read_called = false;

    /* Call time() function. We expect that isenabled and read RTC stubs
     * are be executed.
     */
    time(NULL);

    /* Verify results. */
    TEST_ASSERT_EQUAL(true, rtc_isenabled_called);
    TEST_ASSERT_EQUAL(true, rtc_read_called);

    /* This part of the test can be executed only on RTC devices. */

    /* Restore env. */
    attach_rtc(rtc_read, rtc_write, rtc_init, rtc_isenabled);

    /* Set to unexpected. */
    rtc_write_called = false;
    rtc_init_called = false;
    rtc_isenabled_called = false;
    rtc_init_called = false;

    /* Set time. */
    set_time(CUSTOM_TIME_1);

    /* Get time. */
    seconds = time(NULL);

    /* Stub RTC functions should not be called now. */
    TEST_ASSERT_EQUAL(false, rtc_isenabled_called);
    TEST_ASSERT_EQUAL(false, rtc_init_called);
    TEST_ASSERT_EQUAL(false, rtc_write_called);
    TEST_ASSERT_EQUAL(false, rtc_init_called);

    /* Check if time has been successfully set and retrieved. */
    TEST_ASSERT_UINT32_WITHIN(RTC_DELTA, CUSTOM_TIME_1, seconds);
}

/* This test verifies if attach_rtc provides availability to
 * connect specific RTC driver functions.
 *
 * This is unit test to verify if correct functions are used
 * to support RTC.
 *
 * Given specific RTC driver functions have been attached (original).
 * When set_time/time functions are called.
 * Then set_time/time functions use attached RTC functions.
 */
void test_attach_RTC_org_funtions()
{
    time_t seconds = 0;

    /* Attache original driver functions. */
    attach_rtc(rtc_read, rtc_write, rtc_init, rtc_isenabled);

    /* Set to unexpected. */
    rtc_write_called = false;
    rtc_init_called = false;
    rtc_isenabled_called = false;
    rtc_init_called = false;

    /* Set time. */
    set_time(CUSTOM_TIME_1);

    /* Get time. */
    seconds = time(NULL);

    /* Stub RTC functions should not be called now. */
    TEST_ASSERT_EQUAL(false, rtc_isenabled_called);
    TEST_ASSERT_EQUAL(false, rtc_init_called);
    TEST_ASSERT_EQUAL(false, rtc_write_called);
    TEST_ASSERT_EQUAL(false, rtc_init_called);

    /* Check if time has been successfully set and retrieved. */
    TEST_ASSERT_UINT32_WITHIN(RTC_DELTA, CUSTOM_TIME_1, seconds);
}

/* This test verifies if time() function returns
 * current time when all RTC functions are
 * defined and RTC is enabled.
 *
 * Note: Stubs are used instead of original RTC functions.
 *
 * Given environment has RTC functions defined and RTC is enabled.
 * When time() functions is called.
 * Then current time is returned.
 */
void test_time_RTC_func_defined_RTC_is_enabled()
{
    time_t seconds = 0;

    /* Attache RTC read/write/init/isenabled stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, init_rtc_stub, isenabled_rtc_stub);

    /* Simulate that RTC is enabled. */
    rtc_enabled_ret = true;

    /* Simulate current time. */
    rtc_time_val = CUSTOM_TIME_1;

    /* Try to get current time. */
    seconds = time(NULL);

    /* Check if expected value has been returned. */
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, seconds);
}

/* This test verifies if time() function resets time
 * when RTC functions are defined and RTC is disabled.
 *
 * Note: Stubs are used instead of original RTC functions.
 *
 * Given environment has RTC functions defined and RTC is disabled.
 * When time() functions is called.
 * Then function result is 0.
 */
void test_time_RTC_func_defined_RTC_is_disabled()
{
    time_t seconds = 0;

    /* Attache RTC read/write/init/isenabled stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, init_rtc_stub, isenabled_rtc_stub);

    /* Simulate that RTC is disabled. */
    rtc_enabled_ret = false;

    /* Simulate current time. */
    rtc_time_val = CUSTOM_TIME_1;

    /* Try to get current time. */
    seconds = time(NULL);

    /* Check if expected value has been returned. */
    TEST_ASSERT_EQUAL(0, seconds);
}

/* This test verifies if time() function can be successfully
 * executed when isenabled RTC function is undefined.
 *
 * Note: Stubs are used instead of original RTC functions.
 *
 * Given environment has isenabled RTC function undefined.
 * When time() functions is called.
 * Then current time is returned.
 */
void test_time_isenabled_RTC_func_undefined()
{
    time_t seconds = 0;

    /* Attache RTC read/write/init stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, init_rtc_stub, NULL);

    /* Simulate current time. */
    rtc_time_val = CUSTOM_TIME_1;

    /* Try to get current time. */
    seconds = time(NULL);

    /* Check if expected value has been returned. */
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, seconds);
}

/* This test verifies if time() function returns -1 if
 * read RTC function is undefined.
 *
 * Note: Stubs are used instead of original RTC functions.
 *
 * Given environment has read RTC function undefined.
 * When time() functions is called.
 * Then -1 is returned.
 */
void test_time_read_RTC_func_undefined()
{
    time_t seconds = 0;

    /* Attache RTC write/init/isenabled stubs. */
    attach_rtc(NULL, write_rtc_stub, init_rtc_stub, isenabled_rtc_stub);

    /* Simulate current time. */
    rtc_time_val = CUSTOM_TIME_1;

    /* Try to get current time. */
    seconds = time(NULL);

    /* Check if expected value has been returned. */
    TEST_ASSERT_EQUAL((time_t) -1, seconds);
}

/* This test verifies if time() function stores
 * the result in given time buffer (if specified).
 *
 * Note: Stubs are used instead original RTC functions.
 *       Other test cases calls time() routine with
 *       undefined time buffer.
 *
 * Given environment has all RTC function defined, RTC is enabled and time buffer is passed to time() function.
 * When time() functions is called.
 * Then current time is stored in the specified buffer.
 */
void test_time_called_with_param()
{
    time_t seconds = 0;
    time_t buffer = 0;

    /* Attache RTC read/write/init/isenabled stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, init_rtc_stub, isenabled_rtc_stub);

    /* Simulate that RTC is enabled. */
    rtc_enabled_ret = true;

    /* Simulate current time. */
    rtc_time_val = CUSTOM_TIME_1;

    /* Try to get current time. */
    seconds = time(&buffer);

    /* Check if expected value has been returned. */
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, seconds);
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, buffer);
}

/* This test verifies if set_time() function inits the RTC
 * and writes current time if RTC functions are defined.
 *
 * Note: Stubs are used instead of original RTC functions.
 *
 * Given environment has RTC functions defined.
 * When set_time() functions is called.
 * Then function initialises RTC and sets RTC time.
 */
void test_set_time_RTC_func_defined()
{
    /* Attache RTC read/write/init/isenabled stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, init_rtc_stub, isenabled_rtc_stub);

    /* Set to unexpected. */
    rtc_time_val = 123;
    rtc_init_called = false;

    /* Set current time. */
    rtc_time_val = 123;

    /* Set new RTC time. */
    set_time(CUSTOM_TIME_1);

    /* Check if RTC init has been performed and RTC time has been updated. */
    TEST_ASSERT_EQUAL(true, rtc_init_called);
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, time(NULL));
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, rtc_time_val);
}

/* This test verifies if set_time() function can be
 * successfully executed when init RTC function is undefined.
 *
 * Note: Stubs are used instead of original RTC functions.
 *
 * Given environment has init RTC function undefined.
 * When set_time() functions is called.
 * Then function sets RTC time.
 */
void test_set_time_init_RTC_func_undefined()
{
    /* Attache RTC read/write/isenabled stubs. */
    attach_rtc(read_rtc_stub, write_rtc_stub, NULL, isenabled_rtc_stub);

    /* Set to unexpected. */
    rtc_time_val = 123;

    /* Set new RTC time. */
    set_time(CUSTOM_TIME_1);

    /* Check if RTC time has been updated. */
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, time(NULL));
    TEST_ASSERT_EQUAL(CUSTOM_TIME_1, rtc_time_val);
}

/* This test verifies if set_time() function can be
 * successfully executed when write RTC function is undefined.
 *
 * Note: Stubs are used instead original RTC functions.
 *
 * Given environemt has write RTC function undefined.
 * When set_time() function is called.
 * Then function inits RTC and does not modify RTC time.
 */
void test_set_time_write_RTC_func_undefined()
{
    /* Attache RTC read/write/init/isenabled stubs. */
    attach_rtc(read_rtc_stub, NULL, init_rtc_stub, isenabled_rtc_stub);

    /* Set to unexpected. */
    rtc_time_val = 123;
    rtc_init_called = false;

    /* Set new RTC time. */
    set_time(CUSTOM_TIME_1);

    /* Check if RTC has been initialized and RTC time has not been updated. */
    TEST_ASSERT_EQUAL(true, rtc_init_called);
    TEST_ASSERT_EQUAL(123, time(NULL));
    TEST_ASSERT_EQUAL(123, rtc_time_val);
}

/* This test verifies if RTC time can be successfully set.
 *
 * Note: Original RTC functions are used in this test.
 *
 * Given environment has RTC available.
 * When set_time() functions is called.
 * Then RTC time is retrieved.
 */
template<uint32_t timeValue>
void test_functional_set()
{
    /* Set original RTC functions. */
    attach_rtc(rtc_read, rtc_write, rtc_init, rtc_isenabled);

    /* Set new RTC time. */
    set_time(timeValue);

    /* Get current time and verify that new value has been set. */
    TEST_ASSERT_UINT32_WITHIN(1, timeValue, time(NULL));
}

/* This test verifies if RTC counts seconds.
 *
 * Note: Original RTC functions are used in this test.
 *
 * Given RTC has time set.
 * When some time has passed (seconds).
 * Then RTC time is updated.
 */
void test_functional_count()
{
    time_t seconds = 0;

    /* Set original RTC functions. */
    attach_rtc(rtc_read, rtc_write, rtc_init, rtc_isenabled);

    /* Set new RTC time. */
    set_time(CUSTOM_TIME_2);

    /* Wait 10 sec. */
    ThisThread::sleep_for(DELAY_10_SEC * MS_PER_SEC);

    /* Get time. */
    seconds = time(NULL);

    /* Verify that RTC counts seconds. */
    TEST_ASSERT_UINT_WITHIN(RTC_DELTA, (unsigned int)seconds, CUSTOM_TIME_2 + DELAY_10_SEC);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Unit Test: attach stub RTC functions.", test_attach_RTC_stub_funtions),
    Case("Unit Test: attach original RTC functions.", test_attach_RTC_org_funtions),

    Case("Unit Test: time() - RTC functions are defined, RTC is enabled.", test_time_RTC_func_defined_RTC_is_enabled),
    Case("Unit Test: time() - RTC functions are defined, RTC is disabled.", test_time_RTC_func_defined_RTC_is_disabled),
    Case("Unit Test: time() - isenabled RTC function is undefined.", test_time_isenabled_RTC_func_undefined),
    Case("Unit Test: time() - read RTC function is undefined.", test_time_read_RTC_func_undefined),
    Case("Unit Test: time() - result is stored in given buffer.", test_time_called_with_param),

    Case("Unit Test: set_time() - RTC functions are defined.", test_set_time_RTC_func_defined),
    Case("Unit Test: set_time() - init RTC function is undefined.", test_set_time_init_RTC_func_undefined),
    Case("Unit Test: set_time() - write RTC function is undefined.", test_set_time_write_RTC_func_undefined),

    Case("Functional Test: set time - CUSTOM_TIME_0.", test_functional_set<CUSTOM_TIME_0>),
    Case("Functional Test: set time - CUSTOM_TIME_1.", test_functional_set<CUSTOM_TIME_1>),
    Case("Functional Test: set time - CUSTOM_TIME_2.", test_functional_set<CUSTOM_TIME_2>),

    Case("Functional Test: RTC counts seconds.", test_functional_count),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_RTC || !DEVICE_USTICKER
