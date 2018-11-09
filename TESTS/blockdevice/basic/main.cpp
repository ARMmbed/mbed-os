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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <stdlib.h>
#include <errno.h>

using namespace utest::v1;

// test configuration
#ifndef MBED_TEST_BLOCKDEVICE
#define MBED_TEST_BLOCKDEVICE SDIOBlockDevice
#define MBED_TEST_BLOCKDEVICE_DECL SDIOBlockDevice bd;
#endif

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd
#endif

#ifndef MBED_TEST_BUFFER
#define MBED_TEST_BUFFER 8192
#endif

#ifndef MBED_TEST_TIMEOUT
#define MBED_TEST_TIMEOUT 120
#endif


// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_BLOCKDEVICE)

MBED_TEST_BLOCKDEVICE_DECL;

size_t size;
uint8_t buffer[MBED_TEST_BUFFER];
uint8_t rbuffer[MBED_TEST_BUFFER];
uint8_t wbuffer[MBED_TEST_BUFFER];


// tests

// test multiple init-deinit sequences
void test_init_deinit()
{
    for (int repeatCount=0; repeatCount < 10; repeatCount++)
    {
        int res = bd.init();
        TEST_ASSERT_EQUAL(0, res);

        res = bd.deinit();
        TEST_ASSERT_EQUAL(0, res);
    }
}

void test_init_deinit_stacked()
{
    for (int repeatCount=0; repeatCount < 10; repeatCount++)
    {
        // multiple init
        int res = bd.init();
        TEST_ASSERT_EQUAL(0, res);

        res = bd.init();
        TEST_ASSERT_EQUAL(0, res);

        res = bd.init();
        TEST_ASSERT_EQUAL(0, res);

        // same number of deinit
        res = bd.deinit();
        TEST_ASSERT_EQUAL(0, res);

        res = bd.deinit();
        TEST_ASSERT_EQUAL(0, res);

        res = bd.deinit();
        TEST_ASSERT_EQUAL(0, res);
    }
}


// test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(MBED_TEST_TIMEOUT, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Init/Deinit test", test_init_deinit),
    Case("Init/Deinit stacked test", test_init_deinit_stacked)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
