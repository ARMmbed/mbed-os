/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#endif

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd
#endif

#ifndef MBED_TEST_BLOCK_COUNT
#define MBED_TEST_BLOCK_COUNT 64
#endif

#ifndef MBED_TEST_CYCLES
#define MBED_TEST_CYCLES 10
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
    GREENTEA_SETUP(120, "unexpected_reset");

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
