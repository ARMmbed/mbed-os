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
#if !DEVICE_RESET_REASON
#error [NOT_SUPPORTED] Reset reason API not supported for this target
#else

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "drivers/ResetReason.h"
#include "ResetReason_tests.h"
#include "mbed.h"

#if DEVICE_WATCHDOG
#   include "hal/watchdog_api.h"
#   define MSG_VALUE_WATCHDOG_STATUS 1
#   define WDG_TIMEOUT_MS 50UL
#else
#   define MSG_VALUE_WATCHDOG_STATUS 0
#endif

#define MSG_VALUE_DUMMY "0"
#define MSG_VALUE_RESET_REASON_GET "get"
#define MSG_VALUE_RESET_REASON_CLEAR "clear"
#define MSG_VALUE_RESET_REASON_CLEAR_ACK "cleared"
#define MSG_VALUE_DEVICE_RESET_ACK "ack"
#define MSG_VALUE_DEVICE_RESET_NVIC "nvic"
#define MSG_VALUE_DEVICE_RESET_WATCHDOG "watchdog"
#define MSG_VALUE_LEN 16
#define MSG_KEY_LEN 16

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_RESET_REASON_RAW "reason_raw"
#define MSG_KEY_RESET_REASON "reason"
#define MSG_KEY_DEVICE_RESET "reset"

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

typedef enum {
    CMD_STATUS_CONTINUE,
    CMD_STATUS_ERROR
} cmd_status_t;

static cmd_status_t handle_command(const char *key, const char *value)
{
    if (strcmp(key, MSG_KEY_RESET_REASON_RAW) == 0) {
        uint32_t raw_reason = ResetReason::get_raw();
        char raw_reason_hex_str[9] = { };
        int raw_reason_hex_str_len = snprintf(raw_reason_hex_str,
                                              sizeof raw_reason_hex_str, "%08lx", raw_reason);

        if (raw_reason_hex_str_len < 0) {
            TEST_ASSERT_MESSAGE(0, "Failed to compose raw reset reason hex string.");
            return CMD_STATUS_ERROR;
        }

        greentea_send_kv(MSG_KEY_RESET_REASON_RAW, raw_reason_hex_str);
        return CMD_STATUS_CONTINUE;
    }

    if (strcmp(key, MSG_KEY_RESET_REASON) == 0 && strcmp(value, MSG_VALUE_RESET_REASON_GET) == 0) {
        int reason = (int) ResetReason::get();
        greentea_send_kv(MSG_KEY_RESET_REASON, reason);
        return CMD_STATUS_CONTINUE;
    }

    if (strcmp(key, MSG_KEY_RESET_REASON) == 0 && strcmp(value, MSG_VALUE_RESET_REASON_CLEAR) == 0) {
        /* In order to keep this code compatible with a host script common for
         * both HAL API tests and driver API tests, ignore the 'clear' command
         * received from host.
         *
         * The driver API does not provide clear() function directly.
         */
        greentea_send_kv(MSG_KEY_RESET_REASON, MSG_VALUE_RESET_REASON_CLEAR_ACK);
        return CMD_STATUS_CONTINUE;
    }

    if (strcmp(key, MSG_KEY_DEVICE_RESET) == 0 && strcmp(value, MSG_VALUE_DEVICE_RESET_NVIC) == 0) {
        greentea_send_kv(MSG_KEY_DEVICE_RESET, MSG_VALUE_DEVICE_RESET_ACK);
        ThisThread::sleep_for(SERIAL_FLUSH_TIME_MS); // Wait for the serial buffers to flush.
        NVIC_SystemReset();
        TEST_ASSERT_MESSAGE(0, "NVIC_SystemReset did not reset the device as expected.");
        return CMD_STATUS_ERROR;
    }

#if DEVICE_WATCHDOG
    if (strcmp(key, MSG_KEY_DEVICE_RESET) == 0 && strcmp(value, MSG_VALUE_DEVICE_RESET_WATCHDOG) == 0) {
        greentea_send_kv(MSG_KEY_DEVICE_RESET, MSG_VALUE_DEVICE_RESET_ACK);
        ThisThread::sleep_for(SERIAL_FLUSH_TIME_MS); // Wait for the serial buffers to flush.
        watchdog_config_t config = { .timeout_ms = WDG_TIMEOUT_MS };
        if (hal_watchdog_init(&config) != WATCHDOG_STATUS_OK) {
            TEST_ASSERT_MESSAGE(0, "hal_watchdog_init() error.");
            return CMD_STATUS_ERROR;
        }
        ThisThread::sleep_for(2 * WDG_TIMEOUT_MS); // Watchdog should fire before twice the timeout value.
        TEST_ASSERT_MESSAGE(0, "Watchdog did not reset the device as expected.");
        return CMD_STATUS_ERROR;
    }
#endif

    TEST_ASSERT_MESSAGE(0, "Invalid message key.");
    return CMD_STATUS_ERROR;
}

void test_reset_reason()
{
    reset_reason_capabilities_t rrcap = {};
    hal_reset_reason_get_capabilities(&rrcap);
    char msg_value[11];
    int str_len = snprintf(msg_value, sizeof msg_value, "%08lx,%01x", rrcap.reasons, MSG_VALUE_WATCHDOG_STATUS);
    if (str_len < 0) {
        printf("Failed to compose a value string to be sent to host.");
        GREENTEA_TESTSUITE_RESULT(0);
        return;
    }

    // Report readiness, capabilities and watchdog status.
    greentea_send_kv(MSG_KEY_DEVICE_READY, msg_value);

    cmd_status_t cmd_status = CMD_STATUS_CONTINUE;
    static char _key[MSG_KEY_LEN + 1] = { };
    static char _value[MSG_VALUE_LEN + 1] = { };

    // Let the host side decide what to do and just handle the commands.
    while (CMD_STATUS_CONTINUE == cmd_status) {
        memset(_key, 0, sizeof _key);
        memset(_value, 0, sizeof _value);
        greentea_parse_kv(_key, _value, MSG_KEY_LEN, MSG_VALUE_LEN);
        cmd_status = handle_command(_key, _value);
    }
}

int main()
{
    GREENTEA_SETUP(90, "reset_reason");
    test_reset_reason(); // The result of this test suite is reported by the host side.
    GREENTEA_TESTSUITE_RESULT(0); // Fail on any error.
}

#endif // !DEVICE_RESET_REASON
