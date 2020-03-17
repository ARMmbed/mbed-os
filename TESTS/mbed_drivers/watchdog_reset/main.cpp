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
#include "utest/utest.h"
#include "unity/unity.h"
#include "drivers/Watchdog.h"
#include "Watchdog_reset_tests.h"
#include "mbed.h"

#define TIMEOUT_MS 100UL

/* This value is used to calculate the time to kick the watchdog.
 * Given the watchdog timeout is set to TIMEOUT_MS, the kick will be performed
 * with a delay of (TIMEOUT_MS - KICK_ADVANCE_MS), after the init.
 *
 * It is common for the watchdog peripheral to use a low precision clock source,
 * e.g. the LSI RC acts as a clock source for the IWDG on ST targets.
 * According to the ST spec, the 37 kHz LSI is guaranteed to have a frequency
 * around 37-38 kHz, but the actual frequency range guaranteed by the production
 * tests is 26 kHz up to 56 kHz.
 * Bearing that in mind, a 100 ms timeout value may actually last as long as 142 ms
 * and as short as 66 ms.
 * The value of 35 ms is used to cover the worst case scenario (66 ms).
 */
#define KICK_ADVANCE_MS 35UL

#define MSG_VALUE_DUMMY "0"
#define CASE_DATA_INVALID 0xffffffffUL
#define CASE_DATA_PHASE2_OK 0xfffffffeUL

#define MSG_VALUE_LEN 24
#define MSG_KEY_LEN 24

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_START_CASE "start_case"
#define MSG_KEY_DEVICE_RESET "dev_reset"

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

#define TIMEOUT_US (1000 * (TIMEOUT_MS))
#define KICK_ADVANCE_US (1000 * (KICK_ADVANCE_MS))
#define SERIAL_FLUSH_TIME_US (1000 * (SERIAL_FLUSH_TIME_MS))

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

using namespace mbed;

struct testcase_data {
    int index;
    int start_index;
    uint32_t received_data;
};

testcase_data current_case;

Ticker wdg_kicking_ticker;

bool send_reset_notification(testcase_data *tcdata, uint32_t delay_ms)
{
    char msg_value[12];
    int str_len = snprintf(msg_value, sizeof msg_value, "%02x,%08lx", tcdata->start_index + tcdata->index, delay_ms);
    if (str_len < 0) {
        utest_printf("Failed to compose a value string to be sent to host.");
        return false;
    }
    greentea_send_kv(MSG_KEY_DEVICE_RESET, msg_value);
    return true;
}

void test_simple_reset()
{
    // Phase 2. -- verify the test results.
    // Verify if this test case passed based on data received from host.
    if (current_case.received_data != CASE_DATA_INVALID) {
        TEST_ASSERT_EQUAL(CASE_DATA_PHASE2_OK, current_case.received_data);
        current_case.received_data = CASE_DATA_INVALID;
        return;
    }

    // Phase 1. -- run the test code.
    // Init the watchdog and wait for a device reset.
    if (send_reset_notification(&current_case, 2 * TIMEOUT_MS + SERIAL_FLUSH_TIME_MS) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    wait_us(SERIAL_FLUSH_TIME_US); // Wait for the serial buffers to flush.
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT_FALSE(watchdog.is_running());
    TEST_ASSERT_TRUE(watchdog.start(TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    // Watchdog should fire before twice the timeout value.
    wait_us(2 * TIMEOUT_US); // Device reset expected.

    // Watchdog reset should have occurred during a wait above.

    hal_watchdog_kick();
    wdg_kicking_ticker.attach_us(mbed::callback(hal_watchdog_kick), 20000); // For testsuite failure handling.
    TEST_ASSERT_MESSAGE(0, "Watchdog did not reset the device as expected.");
}

#if DEVICE_SLEEP
void test_sleep_reset()
{
    // Phase 2. -- verify the test results.
    if (current_case.received_data != CASE_DATA_INVALID) {
        TEST_ASSERT_EQUAL(CASE_DATA_PHASE2_OK, current_case.received_data);
        current_case.received_data = CASE_DATA_INVALID;
        return;
    }

    // Phase 1. -- run the test code.
    if (send_reset_notification(&current_case, 2 * TIMEOUT_MS + SERIAL_FLUSH_TIME_MS) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    wait_us(SERIAL_FLUSH_TIME_US); // Wait for the serial buffers to flush.
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT_FALSE(watchdog.is_running());
    TEST_ASSERT_TRUE(watchdog.start(TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    sleep_manager_lock_deep_sleep();
    if (sleep_manager_can_deep_sleep()) {
        TEST_ASSERT_MESSAGE(0, "Deepsleep should be disallowed.");
        return;
    }
    // Watchdog should fire before twice the timeout value.
    ThisThread::sleep_for(2 * TIMEOUT_MS); // Device reset expected.
    sleep_manager_unlock_deep_sleep();

    // Watchdog reset should have occurred during the sleep above.

    hal_watchdog_kick();
    wdg_kicking_ticker.attach_us(mbed::callback(hal_watchdog_kick), 20000); // For testsuite failure handling.
    TEST_ASSERT_MESSAGE(0, "Watchdog did not reset the device as expected.");
}

#if DEVICE_LPTICKER
void test_deepsleep_reset()
{
    // Phase 2. -- verify the test results.
    if (current_case.received_data != CASE_DATA_INVALID) {
        TEST_ASSERT_EQUAL(CASE_DATA_PHASE2_OK, current_case.received_data);
        current_case.received_data = CASE_DATA_INVALID;
        return;
    }

    // Phase 1. -- run the test code.
    if (send_reset_notification(&current_case, 2 * TIMEOUT_MS + SERIAL_FLUSH_TIME_MS) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    wait_us(SERIAL_FLUSH_TIME_US); // Wait for the serial buffers to flush.
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT_FALSE(watchdog.is_running());
    TEST_ASSERT_TRUE(watchdog.start(TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    if (!sleep_manager_can_deep_sleep()) {
        TEST_ASSERT_MESSAGE(0, "Deepsleep should be allowed.");
    }

    // The Watchdog reset is allowed to be delayed up to twice the timeout
    // value when the deepsleep mode is active.
    // To make the test less sensitive to clock/wait accuracy, add 20% extra
    // (making tha whole deepsleep wait equal to 2.2 * timeout).
    ThisThread::sleep_for(220 * TIMEOUT_MS / 100); // Device reset expected.

    // Watchdog reset should have occurred during the deepsleep above.

    hal_watchdog_kick();
    wdg_kicking_ticker.attach_us(mbed::callback(hal_watchdog_kick), 20000); // For testsuite failure handling.
    TEST_ASSERT_MESSAGE(0, "Watchdog did not reset the device as expected.");
}
#endif
#endif

void test_restart_reset()
{
    watchdog_features_t features = hal_watchdog_get_platform_features();
    if (!features.disable_watchdog) {
        TEST_IGNORE_MESSAGE("Disabling Watchdog not supported for this platform");
        return;
    }

    // Phase 2. -- verify the test results.
    if (current_case.received_data != CASE_DATA_INVALID) {
        TEST_ASSERT_EQUAL(CASE_DATA_PHASE2_OK, current_case.received_data);
        current_case.received_data = CASE_DATA_INVALID;
        return;
    }

    // Phase 1. -- run the test code.
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT_FALSE(watchdog.is_running());
    TEST_ASSERT_TRUE(watchdog.start(TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    wait_us(TIMEOUT_US / 2);
    TEST_ASSERT_TRUE(watchdog.stop());
    TEST_ASSERT_FALSE(watchdog.is_running());
    // Check that stopping the Watchdog prevents a device reset.
    // The watchdog should trigger at, or after the timeout value.
    // The watchdog should trigger before twice the timeout value.
    wait_us(TIMEOUT_US / 2 + TIMEOUT_US);

    if (send_reset_notification(&current_case, 2 * TIMEOUT_MS + SERIAL_FLUSH_TIME_MS) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    wait_us(SERIAL_FLUSH_TIME_US); // Wait for the serial buffers to flush.
    TEST_ASSERT_TRUE(watchdog.start(TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    // Watchdog should fire before twice the timeout value.
    wait_us(2 * TIMEOUT_US); // Device reset expected.

    // Watchdog reset should have occurred during a wait above.

    hal_watchdog_kick();
    wdg_kicking_ticker.attach_us(mbed::callback(hal_watchdog_kick), 20000); // For testsuite failure handling.
    TEST_ASSERT_MESSAGE(0, "Watchdog did not reset the device as expected.");
}

void test_kick_reset()
{
    // Phase 2. -- verify the test results.
    if (current_case.received_data != CASE_DATA_INVALID) {
        TEST_ASSERT_EQUAL(CASE_DATA_PHASE2_OK, current_case.received_data);
        current_case.received_data = CASE_DATA_INVALID;
        return;
    }

    // Phase 1. -- run the test code.
    Watchdog &watchdog = Watchdog::get_instance();
    TEST_ASSERT_FALSE(watchdog.is_running());
    TEST_ASSERT_TRUE(watchdog.start(TIMEOUT_MS));
    TEST_ASSERT_TRUE(watchdog.is_running());
    for (int i = 3; i; i--) {
        // The reset is prevented as long as the watchdog is kicked
        // anytime before the timeout.
        wait_us(TIMEOUT_US - KICK_ADVANCE_US);
        watchdog.kick();
    }
    if (send_reset_notification(&current_case, 2 * TIMEOUT_MS + SERIAL_FLUSH_TIME_MS) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        return;
    }
    wait_us(SERIAL_FLUSH_TIME_US); // Wait for the serial buffers to flush.
    // Watchdog should fire before twice the timeout value.
    wait_us(2 * TIMEOUT_US); // Device reset expected.

    // Watchdog reset should have occurred during a wait above.

    hal_watchdog_kick();
    wdg_kicking_ticker.attach_us(mbed::callback(hal_watchdog_kick), 20000); // For testsuite failure handling.
    TEST_ASSERT_MESSAGE(0, "Watchdog did not reset the device as expected.");
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
    Case("Watchdog reset", case_setup, test_simple_reset),
#if DEVICE_SLEEP
    Case("Watchdog reset in sleep mode", case_setup, test_sleep_reset),
#if DEVICE_LPTICKER
    Case("Watchdog reset in deepsleep mode", case_setup, test_deepsleep_reset),
#endif
#endif
    Case("Watchdog started again", case_setup, test_restart_reset),
    Case("Kicking the Watchdog prevents reset", case_setup, test_kick_reset),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup, cases);

int main()
{
    // Harness will start with a test case index provided by host script.
    return !Harness::run(specification);
}
#endif // !DEVICE_WATCHDOG
