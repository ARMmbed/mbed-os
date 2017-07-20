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


/*
 * Tests is to measure the accuracy of Thread::wait() over a period of time
 *
 *
 * 1) DUT would start to update callback_trigger_count every milli sec
 * 2) Host would query what is current count base_time, Device responds by the callback_trigger_count
 * 3) Host after waiting for measurement stretch. It will query for device time again final_time.
 * 4) Host computes the drift considering base_time, final_time, transport delay and measurement stretch
 * 5) Finally host send the results back to device pass/fail based on tolerance.
 * 6) More details on tests can be found in timing_drift_auto.py
 *
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"
#include "unity/unity.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
#error [NOT_SUPPORTED] test not supported
#endif

#define TEST_STACK_SIZE 1024
#define ONE_MILLI_SEC 1000

volatile uint32_t callback_trigger_count = 0;

static const int test_timeout = 240;
bool test_result = false;

void update_tick_thread() {
    while (true) {
        Thread::wait(1);
        ++callback_trigger_count;
    }
}

void gt_comm_wait_thread() {
    char _key[11] = { };
    char _value[128] = { };
    int expected_key = 1;

    greentea_send_kv("timing_drift_check_start", 0);

    // wait for 1st signal from host
    do {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        expected_key = strcmp(_key, "base_time");
    } while (expected_key);
    greentea_send_kv(_key, callback_trigger_count * ONE_MILLI_SEC);

    // wait for 2nd signal from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    greentea_send_kv(_key, callback_trigger_count * ONE_MILLI_SEC);

    //get the results from host
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

    if (strcmp("pass", _key) == 0) {
        test_result = true;
    }
}

int main() {
    GREENTEA_SETUP(test_timeout, "timing_drift_auto");
    Thread tick_thread(osPriorityHigh, TEST_STACK_SIZE);
    Thread gt_conn_thread(osPriorityNormal, TEST_STACK_SIZE);

    tick_thread.start(update_tick_thread);
    gt_conn_thread.start(gt_comm_wait_thread);
    gt_conn_thread.join();

    GREENTEA_TESTSUITE_RESULT(test_result);
}
