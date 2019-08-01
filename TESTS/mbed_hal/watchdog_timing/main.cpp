/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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
#if !DEVICE_WATCHDOG
#error [NOT_SUPPORTED] Watchdog not supported for this target
#else

#include "greentea-client/test_env.h"
#include "hal/watchdog_api.h"
#include "unity/unity.h"
#include "us_ticker_api.h"
#include "utest/utest.h"
#include "watchdog_timing_tests.h"

#define MSG_VALUE_DUMMY "0"
#define CASE_DATA_INVALID 0xffffffffUL

#define MSG_VALUE_LEN 24
#define MSG_KEY_LEN 24

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_START_CASE "start_case"
#define MSG_KEY_HEARTBEAT "hb"

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

struct testcase_data {
    int index;
    int start_index;
    uint32_t received_data;
};

testcase_data current_case;

template<uint32_t timeout_ms>
void test_timing()
{
    watchdog_features_t features = hal_watchdog_get_platform_features();
    if (timeout_ms > features.max_timeout) {
        TEST_IGNORE_MESSAGE("Requested timeout value not supported for this target -- ignoring test case.");
        return;
    }

    // Phase 2. -- verify the test results.
    // Verify the heartbeat time span sent by host is within given range:
    // 1. The watchdog should trigger at, or after the timeout value.
    // 2. The watchdog should trigger before twice the timeout value.
    if (current_case.received_data != CASE_DATA_INVALID) {
        // Provided the watchdog works as expected, the last timestamp received
        // by the host will always be before the expected reset time. Because
        // of that, the constraint no 1. is not verified.
        TEST_ASSERT(current_case.received_data > 0);
        TEST_ASSERT(current_case.received_data < 2 * timeout_ms);
        current_case.received_data = CASE_DATA_INVALID;
        return;
    }

    // Phase 1. -- run the test code.
    // Send heartbeat messages to host until the watchdeg resets the device.
    const ticker_data_t *const us_ticker = get_us_ticker_data();
    us_timestamp_t current_ts, next_ts, expected_reset_ts, divider, ts_increment;
    char msg_value[12];

    watchdog_config_t config = { timeout_ms };
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, hal_watchdog_init(&config));
    next_ts = ticker_read_us(us_ticker);
    expected_reset_ts = next_ts + 1000ULL * timeout_ms;

    divider = 0x2ULL;
    while (1) {
        current_ts = ticker_read_us(us_ticker);
        if (current_ts < next_ts) {
            continue;
        }

        int str_len = snprintf(msg_value, sizeof msg_value, "%02x,%08lx", current_case.start_index + current_case.index,
                               (uint32_t) current_ts);
        if (str_len != (sizeof msg_value) - 1) {
            utest_printf("Failed to compose a value string to be sent to host.");
            return;
        }
        greentea_send_kv(MSG_KEY_HEARTBEAT, msg_value);

        // The closer to expected reset, the smaller heartbeat time difference.
        // This should reduce measurement error present for heartbeat with
        // equal periods.
        ts_increment = (1000ULL * timeout_ms / divider);
        next_ts += ts_increment;

        if (current_ts <= expected_reset_ts) {
            divider <<= 1;
        } else if (divider > 0x2ULL) {
            divider >>= 1;
        }
    }
}

utest::v1::status_t case_setup(const Case *const source, const size_t index_of_case)
{
    current_case.index = index_of_case;
    return utest::v1::greentea_case_setup_handler(source, index_of_case);
}

int testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(90, "watchdog_reset");
    utest::v1::status_t status = utest::v1::greentea_test_setup_handler(number_of_cases);
    if (status != utest::v1::STATUS_CONTINUE) {
        return status;
    }

    char key[MSG_KEY_LEN + 1] = { };
    char value[MSG_VALUE_LEN + 1] = { };

    greentea_send_kv(MSG_KEY_DEVICE_READY, MSG_VALUE_DUMMY);
    greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);

    if (strcmp(key, MSG_KEY_START_CASE) != 0) {
        utest_printf("Invalid message key.\n");
        return utest::v1::STATUS_ABORT;
    }

    int num_args = sscanf(value, "%02x,%08lx", &(current_case.start_index), &(current_case.received_data));
    if (num_args == 0 || num_args == EOF) {
        utest_printf("Invalid data received from host\n");
        return utest::v1::STATUS_ABORT;
    }

    utest_printf("This test suite is composed of %i test cases. Starting at index %i.\n", number_of_cases,
                 current_case.start_index);
    return current_case.start_index;
}

Case cases[] = {
    Case("Timing, 200 ms", case_setup, test_timing<200UL>),
    Case("Timing, 500 ms", case_setup, test_timing<500UL>),
    Case("Timing, 1000 ms", case_setup, test_timing<1000UL>),
    Case("Timing, 3000 ms", case_setup, test_timing<3000UL>),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup, cases);

int main()
{
    // Harness will start with a test case index provided by host script.
    return !Harness::run(specification);
}

#endif // !DEVICE_WATCHDOG
