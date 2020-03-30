/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 *
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

/*
 * Based on mbed-stress-test by Marcus Chang @ Arm Mbed - http://github.com/ARMmbed/mbed-stress-test
*/

#if !INTEGRATION_TESTS
#error [NOT_SUPPORTED] integration tests not enabled for this target
#elif !MBED_CONF_RTOS_PRESENT
#error [NOT_SUPPORTED] integration tests require RTOS
#else

#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "common_defines_net_test.h"
#include "download_test.h"
#include <string>

#ifdef MBED_CONF_APP_BASICS_TEST_FILENAME
#include MBED_CONF_APP_BASICS_TEST_FILENAME
#else
#include "sample.h"
#endif

#ifndef MBED_CONF_APP_TESTS_FS_SIZE
#define MBED_CONF_APP_TESTS_FS_SIZE (2*1024*1024)
#endif

using namespace utest::v1;

#if !defined(MBED_CONF_APP_NO_LED)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
void led_thread()
{
    led1 = !led1;
    led2 = !led1;
}
#endif

#define MAX_RETRIES 3
NetworkInterface *net = NULL;
static control_t setup_network(const size_t call_count)
{
    net = NetworkInterface::get_default_instance();
    TEST_ASSERT_NOT_NULL_MESSAGE(net, "failed to initialize network");

    nsapi_error_t err = -1;
    for (int tries = 0; tries < MAX_RETRIES; tries++) {
        err = net->connect();
        if (err == NSAPI_ERROR_OK) {
            break;
        } else {
            tr_error("[ERROR] Connecting to network. Retrying %d of %d.", tries, MAX_RETRIES);
        }
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

    return CaseNext;
}

static uint32_t thread_counter = 0;
void download_fn()
{
    uint32_t thread_id = core_util_atomic_incr_u32(&thread_counter, 1);
    download_test(net, story, sizeof(story), 1024, thread_id);
}

static control_t download_1_thread(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    t1.start(download_fn);
    t1.join();

    return CaseNext;
}

static control_t download_2_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    t1.start(download_fn);
    ThisThread::sleep_for(1);
    t2.start(download_fn);
    t2.join();
    t1.join();

    return CaseNext;
}

static control_t download_3_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    Thread t3;
    t1.start(download_fn);
    t2.start(download_fn);
    t3.start(download_fn);
    t1.join();
    t2.join();
    t3.join();

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(12 * 60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case(TEST_NETWORK_TYPE " network setup", setup_network),
    Case(TEST_NETWORK_TYPE " 1 thread", download_1_thread),
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != CELLULAR
    Case(TEST_NETWORK_TYPE " 2 threads", download_2_threads),
#endif
};

Specification specification(greentea_setup, cases);

int main()
{
    //Create a thread to blink an LED and signal that the device is alive
#if !defined(MBED_CONF_APP_NO_LED)
    Ticker t;
    t.attach(led_thread, 0.5);
#endif

    return !Harness::run(specification);
}
#endif // !INTEGRATION_TESTS
