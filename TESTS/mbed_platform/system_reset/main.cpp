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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"

#define MSG_VALUE_DUMMY "0"
#define MSG_VALUE_LEN 16
#define MSG_KEY_LEN 16

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_DEVICE_RESET "reset"

void test_system_reset()
{
    // Report readiness
    greentea_send_kv(MSG_KEY_DEVICE_READY, MSG_VALUE_DUMMY);

    static char _key[MSG_KEY_LEN + 1] = { };
    static char _value[MSG_VALUE_LEN + 1] = { };

    greentea_parse_kv(_key, _value, MSG_KEY_LEN, MSG_VALUE_LEN);
    if (strcmp(_key, MSG_KEY_DEVICE_RESET) == 0) {
        system_reset();
        TEST_ASSERT_MESSAGE(0, "system_reset() did not reset the device as expected.");
    }

    TEST_ASSERT_MESSAGE(0, "Unexpected message key.");
}

int main(void)
{
    GREENTEA_SETUP(2, "system_reset");
    test_system_reset();
    GREENTEA_TESTSUITE_RESULT(0); // Fail on any error.

    return 0;
}