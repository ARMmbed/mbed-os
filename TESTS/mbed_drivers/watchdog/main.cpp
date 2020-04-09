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
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbed.h"
#include "drivers/Watchdog.h"
#include "hal/watchdog_api.h"
#include "Watchdog_tests.h"
#include <stdlib.h>

/* The shortest timeout value, this test suite is able to handle correctly. */
#define WDG_MIN_TIMEOUT_MS 50UL

// Do not set watchdog timeout shorter than WDG_MIN_TIMEOUT_MS, as it may
// cause the host-test-runner return 'TIMEOUT' instead of 'FAIL' / 'PASS'
// if watchdog performs reset during test suite teardown.
#define WDG_TIMEOUT_MS 100UL

#define MSG_VALUE_DUMMY "0"
#define MSG_VALUE_LEN 24
#define MSG_KEY_LEN 24

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_START_CASE "start_case"
#define MSG_KEY_DEVICE_RESET "reset_on_case_teardown"

/* To prevent a loss of Greentea data, the serial buffers have to be flushed
 * before the UART peripheral shutdown. The UART shutdown happens when the
 * device is entering the deepsleep mode or performing a reset.
 *
 * With the current API, it is not possible to check if the hardware buffers
 * are empty. However, it is possible to determine the time required for the
 * buffers to flush.
 *
 * Assuming the biggest Tx FIFO of 128 bytes (as for CY8CPROTO_062_4343W)
 * and a default UART config (9600, 8N1), flushing the Tx FIFO wold take:
 * (1 start_bit + 8 data_bits + 1 stop_bit) * 128 * 1000 / 9600 = 133.3 ms.
 * To be on the safe side, set the wait time to 150 ms.
 */
#define SERIAL_FLUSH_TIME_MS 150

int CASE_INDEX_START;
int CASE_INDEX_CURRENT;
bool CASE_IGNORED = false;

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

using namespace mbed;

void test_max_timeout_is_valid()
{
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT(watchdog.get_max_timeout() > 1UL);
}

void test_stop()
{
    watchdog_features_t features = hal_watchdog_get_platform_features();
    Watchdog &watchdog = Watchdog::get_instance();
    if (!features.disable_watchdog) {
        TEST_ASSERT_FALSE(watchdog.stop());
        CASE_IGNORED = true;
        TEST_IGNORE_MESSAGE("Disabling watchdog not supported for this platform");
        return;
    }

    TEST_ASSERT_FALSE(watchdog.is_running());
    TEST_ASSERT_FALSE(watchdog.stop());
    TEST_ASSERT_FALSE(watchdog.is_running());

    TEST_ASSERT_TRUE(watchdog.start(WDG_TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    TEST_ASSERT_TRUE(watchdog.stop());
    TEST_ASSERT_FALSE(watchdog.is_running());
    // Make sure that a disabled watchdog does not reset the core.
    ThisThread::sleep_for(2 * WDG_TIMEOUT_MS); // Watchdog should fire before twice the timeout value.

    TEST_ASSERT_FALSE(watchdog.stop());
}


void test_restart()
{
    watchdog_features_t features = hal_watchdog_get_platform_features();
    if (!features.update_config) {
        CASE_IGNORED = true;
        TEST_IGNORE_MESSAGE("Updating watchdog config not supported for this platform");
        return;
    }
    if (!features.disable_watchdog) {
        CASE_IGNORED = true;
        TEST_IGNORE_MESSAGE("Disabling watchdog not supported for this platform");
        return;
    }

    Watchdog &watchdog = Watchdog::get_instance();
    uint32_t max_timeout = watchdog.get_max_timeout();
    uint32_t timeouts[] = {
        max_timeout / 4,
        max_timeout / 8,
        max_timeout / 16
    };
    int num_timeouts = sizeof timeouts / sizeof timeouts[0];

    for (size_t i = 0; i < num_timeouts; i++) {
        if (timeouts[i] < WDG_MIN_TIMEOUT_MS) {
            CASE_IGNORED = true;
            TEST_IGNORE_MESSAGE("Requested timeout value is too short -- ignoring test case.");
            return;
        }

        TEST_ASSERT_TRUE(watchdog.start(timeouts[i]));
        TEST_ASSERT_TRUE(watchdog.is_running());
        uint32_t actual_timeout = watchdog.get_timeout();
        TEST_ASSERT_TRUE(watchdog.stop());
        TEST_ASSERT_FALSE(watchdog.is_running());
        // The watchdog should trigger at, or after the timeout value.
        TEST_ASSERT(actual_timeout >= timeouts[i]);
        // The watchdog should trigger before twice the timeout value.
        TEST_ASSERT(actual_timeout < 2 * timeouts[i]);
    }
}

utest::v1::status_t case_setup_sync_on_reset(const Case *const source, const size_t index_of_case)
{
    CASE_INDEX_CURRENT = index_of_case;
    CASE_IGNORED = false;
    return utest::v1::greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t case_teardown_sync_on_reset(const Case *const source, const size_t passed, const size_t failed,
                                                const utest::v1::failure_t failure)
{
    if (CASE_IGNORED) {
        return utest::v1::greentea_case_teardown_handler(source, passed, failed, failure);
    }
    // Start kicking the watchdog during teardown.
    hal_watchdog_kick();
    Ticker wdg_kicking_ticker;
    wdg_kicking_ticker.attach_us(mbed::callback(hal_watchdog_kick), 20000);
    utest::v1::status_t status = utest::v1::greentea_case_teardown_handler(source, passed, failed, failure);
    if (failed) {
        /* Return immediately and skip the device reset, if the test case failed.
         * Provided that the device won't be restarted by other means (i.e. watchdog timer),
         * this should allow the test suite to finish in a defined manner
         * and report failure to host.
         * In case of watchdog reset during test suite teardown, the loss of serial
         * connection is possible, so the host-test-runner may return 'TIMEOUT'
         * instead of 'FAIL'.
         */
        return status;
    }
    greentea_send_kv(MSG_KEY_DEVICE_RESET, CASE_INDEX_START + CASE_INDEX_CURRENT);
    utest_printf("The device will now restart.\n");
    ThisThread::sleep_for(SERIAL_FLUSH_TIME_MS); // Wait for the serial buffers to flush.
    NVIC_SystemReset();
    return status; // Reset is instant so this line won't be reached.
}

utest::v1::status_t case_teardown_wdg_stop_or_reset(const Case *const source, const size_t passed, const size_t failed,
                                                    const utest::v1::failure_t failure)
{
    if (CASE_IGNORED) {
        return utest::v1::greentea_case_teardown_handler(source, passed, failed, failure);
    }
    watchdog_features_t features = hal_watchdog_get_platform_features();
    if (features.disable_watchdog) {
        hal_watchdog_stop();
        return utest::v1::greentea_case_teardown_handler(source, passed, failed, failure);
    }

    return case_teardown_sync_on_reset(source, passed, failed, failure);
}

template<uint32_t timeout_ms>
void test_start()
{
    if (timeout_ms < WDG_MIN_TIMEOUT_MS) {
        CASE_IGNORED = true;
        TEST_IGNORE_MESSAGE("Requested timeout value is too short -- ignoring test case.");
        return;
    }
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT_TRUE(watchdog.start(timeout_ms));
    uint32_t actual_timeout = watchdog.get_timeout();
    // The watchdog should trigger at, or after the timeout value.
    TEST_ASSERT(actual_timeout >= timeout_ms);
    // The watchdog should trigger before twice the timeout value.
    TEST_ASSERT(actual_timeout < 2 * timeout_ms);
}

void test_start_max_timeout()
{
    Watchdog &watchdog = Watchdog::get_instance();
    uint32_t max_timeout = watchdog.get_max_timeout();
    TEST_ASSERT_TRUE(watchdog.start(max_timeout));
    // The watchdog should trigger at, or after the timeout value.
    TEST_ASSERT(watchdog.get_timeout() >= max_timeout);
}

int testsuite_setup_sync_on_reset(const size_t number_of_cases)
{
    GREENTEA_SETUP(45, "sync_on_reset");
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

    char *tailptr = NULL;
    CASE_INDEX_START = (int) strtol(value, &tailptr, 10);
    if (*tailptr != '\0' || CASE_INDEX_START < 0) {
        utest_printf("Invalid start case index received from host\n");
        return utest::v1::STATUS_ABORT;
    }

    utest_printf("Starting with test case index %i of all %i defined test cases.\n", CASE_INDEX_START, number_of_cases);
    return CASE_INDEX_START;
}

Case cases[] = {
    Case("max_timeout is valid", test_max_timeout_is_valid),
    Case("Stop", test_stop),
    Case("Restart multiple times",
         (utest::v1::case_setup_handler_t) case_setup_sync_on_reset,
         test_restart, (utest::v1::case_teardown_handler_t) case_teardown_wdg_stop_or_reset),
    Case("Start, 100 ms", (utest::v1::case_setup_handler_t) case_setup_sync_on_reset,
         test_start<100UL>, (utest::v1::case_teardown_handler_t) case_teardown_sync_on_reset),
    Case("Start, max_timeout", (utest::v1::case_setup_handler_t) case_setup_sync_on_reset,
         test_start_max_timeout, (utest::v1::case_teardown_handler_t) case_teardown_sync_on_reset),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup_sync_on_reset, cases);

int main()
{
    // Harness will start with a test case index provided by host script.
    return !Harness::run(specification);
}

#endif // !DEVICE_WATCHDOG
