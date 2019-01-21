/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "25AA02E48_EEPROM.h"

using namespace utest::v1;

void eeprom_read_mac(void)
{

    uint8_t buf[6];
    memset(buf, 0, 6);

    int result = eeprom_read_eui48_node_address(buf);

    // test if read was successful
    //TEST_ASSERT_TRUE(result == 0);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, result, "result");

    // validate mac address range -> D8:80:39:00:00:00 - D8:80:39:FF:FF:FF
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0xD8, buf[0], " byte 1");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0x80, buf[1], " byte 2");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0x39, buf[2], " byte 3");
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("EEPROM read and validate MAC address", eeprom_read_mac, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
