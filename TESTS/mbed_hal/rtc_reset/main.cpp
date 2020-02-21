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

#if !DEVICE_RTC
#error [NOT_SUPPORTED] RTC API not supported for this target
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "rtc_reset_test.h"

#include "mbed.h"
#include "rtc_api.h"


typedef enum {
    CMD_STATUS_PASS,
    CMD_STATUS_FAIL,
    CMD_STATUS_CONTINUE,
    CMD_STATUS_ERROR
} cmd_status_t;

static cmd_status_t handle_command(const char *key, const char *value)
{
    if (strcmp(key, "init") == 0) {
        rtc_init();
        greentea_send_kv("ack", 0);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "free") == 0) {
        rtc_free();
        greentea_send_kv("ack", 0);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "read") == 0) {
        static char time_buf[64];
        memset(time_buf, 0, sizeof(time_buf));
        sprintf(time_buf, "%lu", (uint32_t)rtc_read());
        greentea_send_kv("read", time_buf);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "write") == 0) {
        uint32_t time;
        sscanf(value, "%lu", &time);
        rtc_write(time);
        greentea_send_kv("ack", 0);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "reset") == 0) {
        NVIC_SystemReset();
        // Code shouldn't read this due to the reset
        return CMD_STATUS_ERROR;

    } else if (strcmp(key, "exit") == 0) {
        return strcmp(value, "pass") == 0 ? CMD_STATUS_PASS : CMD_STATUS_FAIL;

    } else {
        return CMD_STATUS_ERROR;

    }
}

/* Test that software reset doesn't stop RTC from counting. */
void rtc_reset_test()
{
    GREENTEA_SETUP(100, "rtc_reset");

    static char _key[10 + 1] = {};
    static char _value[128 + 1] = {};

    greentea_send_kv("start", 1);

    // Handshake with host
    cmd_status_t cmd_status = CMD_STATUS_CONTINUE;
    while (CMD_STATUS_CONTINUE == cmd_status) {
        memset(_key, 0, sizeof(_key));
        memset(_value, 0, sizeof(_value));
        greentea_parse_kv(_key, _value, sizeof(_key) - 1, sizeof(_value) - 1);
        cmd_status = handle_command(_key, _value);
    }

    GREENTEA_TESTSUITE_RESULT(CMD_STATUS_PASS == cmd_status);
}

int main()
{
    rtc_reset_test();
}

#endif // !DEVICE_RTC
