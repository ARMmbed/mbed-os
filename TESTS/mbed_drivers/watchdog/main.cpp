/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#endif

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "drivers/Watchdog.h"
#include "Watchdog_tests.h"

/* This is platform specific and depends on the watchdog timer implementation,
 * e.g. STM32F4 uses 32kHz internal RC oscillator to clock the IWDG, so
 * when the prescaler divider is set to max value of 256 the resolution
 * drops to 8 ms.
 */
#define WORST_TIMEOUT_RESOLUTION_MS 8UL

#define TIMEOUT_DELTA_MS (WORST_TIMEOUT_RESOLUTION_MS)
#define WDG_TIMEOUT_MS 500UL

#define MSG_VALUE_DUMMY "0"
#define MSG_VALUE_LEN 24
#define MSG_KEY_LEN 24

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_START_CASE "start_case"
#define MSG_KEY_DEVICE_RESET "reset_on_case_teardown"

int CASE_INDEX_START;
int CASE_INDEX_CURRENT;

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

void test_max_timeout_is_valid()
{
    Watchdog watchdog;
    TEST_ASSERT(watchdog.max_timeout() > 1UL);
}

void test_stop()
{
    Watchdog watchdog;
    if (watchdog.stop() == WATCHDOG_STATUS_NOT_SUPPORTED) {
        TEST_IGNORE_MESSAGE("Disabling watchdog not supported for this platform");
        return;
    }

    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.stop());

    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.start(WDG_TIMEOUT_MS));
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.stop());
    // Make sure that a disabled watchdog does not reset the core.
    wait_ms(WDG_TIMEOUT_MS + TIMEOUT_DELTA_MS);

    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.stop());
}

void test_restart()
{
    Watchdog watchdog;
    watchdog.start(watchdog.max_timeout());
    uint64_t timeout = watchdog.max_timeout() - 2ULL * WORST_TIMEOUT_RESOLUTION_MS;
    watchdog_status_t status = watchdog.start(timeout);

    if (status == WATCHDOG_STATUS_NOT_SUPPORTED) {
        TEST_IGNORE_MESSAGE("Updating watchdog config not supported for this platform");
        return;
    }

    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, status);
    TEST_ASSERT_UINT32_WITHIN(WORST_TIMEOUT_RESOLUTION_MS, timeout, watchdog.reload_value());

    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.start(watchdog.max_timeout()));
    TEST_ASSERT_UINT32_WITHIN(WORST_TIMEOUT_RESOLUTION_MS, watchdog.max_timeout(), watchdog.reload_value());

    timeout = watchdog.max_timeout() + 2ULL * WORST_TIMEOUT_RESOLUTION_MS;
    // Make sure requested timeout does not overflow uint32_t.
    if (timeout <= UINT32_MAX) {
        TEST_ASSERT_EQUAL(WATCHDOG_STATUS_INVALID_ARGUMENT, watchdog.start(timeout));
        TEST_ASSERT_UINT32_WITHIN(WORST_TIMEOUT_RESOLUTION_MS, watchdog.max_timeout(), watchdog.reload_value());
    }

    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_INVALID_ARGUMENT, watchdog.start(0UL));
    TEST_ASSERT_UINT32_WITHIN(WORST_TIMEOUT_RESOLUTION_MS, watchdog.max_timeout(), watchdog.reload_value());
}

utest::v1::status_t case_setup_sync_on_reset(const Case * const source, const size_t index_of_case)
{
    CASE_INDEX_CURRENT = index_of_case;
    return utest::v1::greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t case_teardown_sync_on_reset(const Case * const source, const size_t passed, const size_t failed,
        const utest::v1::failure_t failure)
{
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
    wait_ms(10); // Wait for the serial buffers to flush.
    NVIC_SystemReset();
    return status; // Reset is instant so this line won't be reached.
}

void test_start_zero()
{
    Watchdog watchdog;
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_INVALID_ARGUMENT, watchdog.start(0UL));
}

template<uint32_t timeout_ms>
void test_start()
{
    Watchdog watchdog;
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.start(timeout_ms));
    TEST_ASSERT_UINT32_WITHIN(WORST_TIMEOUT_RESOLUTION_MS, timeout_ms, watchdog.reload_value());
}

void test_start_max_timeout()
{
    Watchdog watchdog;
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, watchdog.start(watchdog.max_timeout()));
    TEST_ASSERT_UINT32_WITHIN(WORST_TIMEOUT_RESOLUTION_MS, watchdog.max_timeout(), watchdog.reload_value());
}

void test_start_max_timeout_exceeded()
{
    Watchdog watchdog;
    uint64_t timeout = watchdog.max_timeout() + 2ULL * WORST_TIMEOUT_RESOLUTION_MS;
    // Make sure requested timeout does not overflow uint32_t.
    if (timeout > UINT32_MAX) {
        TEST_IGNORE_MESSAGE("Requested timeout overflows uint32_t -- ignoring test case.");
        return;
    }
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_INVALID_ARGUMENT, watchdog.start(timeout));
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
    Case("Restart multiple times", (utest::v1::case_setup_handler_t) case_setup_sync_on_reset,
        test_restart, (utest::v1::case_teardown_handler_t) case_teardown_sync_on_reset),

    // Do not set watchdog timeout shorter than 500 ms as it may cause the
    // host-test-runner return 'TIMEOUT' instead of 'FAIL' / 'PASS' if watchdog
    // performs reset during test suite teardown.
    Case("Start, 500 ms", (utest::v1::case_setup_handler_t) case_setup_sync_on_reset,
        test_start<500UL>, (utest::v1::case_teardown_handler_t) case_teardown_sync_on_reset),
    Case("Start, max_timeout", (utest::v1::case_setup_handler_t) case_setup_sync_on_reset,
        test_start_max_timeout, (utest::v1::case_teardown_handler_t) case_teardown_sync_on_reset),

    Case("Start, 0 ms", test_start_zero),
    Case("Start, max_timeout exceeded", test_start_max_timeout_exceeded),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup_sync_on_reset, cases);

int main()
{
    // Harness will start with a test case index provided by host script.
    return !Harness::run(specification);
}
