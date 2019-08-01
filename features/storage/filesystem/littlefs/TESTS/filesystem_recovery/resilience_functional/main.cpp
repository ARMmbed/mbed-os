/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed.h"
#include "unity.h"
#include "utest.h"
#include "test_env.h"

#include "atomic_usage.h"
#include "ObservingBlockDevice.h"
#include "LittleFileSystem.h"

#include <string.h>

using namespace utest::v1;


// test configuration
#ifndef MBED_TEST_BLOCKDEVICE
#error [NOT_SUPPORTED] Non-volatile block device required for resilience_functional tests
#else

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd
#endif

#ifndef MBED_TEST_BLOCK_COUNT
#define MBED_TEST_BLOCK_COUNT 64
#endif

#ifndef MBED_TEST_CYCLES
#define MBED_TEST_CYCLES 10
#endif

#ifndef MBED_TEST_TIMEOUT
#define MBED_TEST_TIMEOUT 480
#endif

// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_BLOCKDEVICE)

MBED_TEST_BLOCKDEVICE_DECL;

typedef enum {
    CMD_STATUS_PASS,
    CMD_STATUS_FAIL,
    CMD_STATUS_CONTINUE,
    CMD_STATUS_ERROR
} cmd_status_t;

void use_filesystem()
{
    // Perform operations
    while (true) {
        int64_t ret = perform_atomic_operations(&bd);
        TEST_ASSERT(ret > 0);
    }
}

static cmd_status_t handle_command(const char *key, const char *value)
{
    if (strcmp(key, "format") == 0) {
        setup_atomic_operations(&bd, true);
        greentea_send_kv("format_done", 1);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "run") == 0) {
        use_filesystem();
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "exit") == 0) {
        if (strcmp(value, "pass") != 0) {
            return CMD_STATUS_FAIL;
        }
        check_atomic_operations(&bd);
        return CMD_STATUS_PASS;

    } else {
        return CMD_STATUS_ERROR;

    }
}

int main()
{
    GREENTEA_SETUP(MBED_TEST_TIMEOUT, "unexpected_reset");

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

#endif // MBED_TEST_BLOCKDEVICE
