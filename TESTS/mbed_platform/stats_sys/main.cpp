
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include "mbed.h"

#if !defined(MBED_SYS_STATS_ENABLED)
#error [NOT_SUPPORTED] test not supported
#else

using namespace utest::v1;

void test_sys_info()
{
    mbed_stats_sys_t stats;
    mbed_stats_sys_get(&stats);

    TEST_ASSERT_NOT_EQUAL(0, stats.os_version);
#if defined(__CORTEX_M)
    TEST_ASSERT_NOT_EQUAL(0, stats.cpu_id);
#endif

#if defined(__IAR_SYSTEMS_ICC__)
    TEST_ASSERT_EQUAL(IAR, stats.compiler_id);
#elif defined(__ARMCC_VERSION)
    TEST_ASSERT_EQUAL(ARM, stats.compiler_id);
#elif defined(__GNUC__)
    TEST_ASSERT_EQUAL(GCC_ARM, stats.compiler_id);
#endif
    TEST_ASSERT_NOT_EQUAL(0, stats.compiler_version);

    // RAM / ROM sizes should not be zero and should match the define
#if defined(MBED_ROM_START) && defined(MBED_ROM_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.rom_size[0]);
    TEST_ASSERT_EQUAL(MBED_ROM_SIZE, stats.rom_size[0]);
    TEST_ASSERT_EQUAL(MBED_ROM_START, stats.rom_start[0]);
#endif
#if defined(MBED_RAM_START) && defined(MBED_RAM_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.ram_size[0]);
    TEST_ASSERT_EQUAL(MBED_RAM_START, stats.ram_start[0]);
    TEST_ASSERT_EQUAL(MBED_RAM_SIZE, stats.ram_size[0]);
#endif
#if defined(MBED_RAM1_START) && defined(MBED_RAM1_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.ram_size[1]);
    TEST_ASSERT_EQUAL(MBED_RAM1_SIZE, stats.ram_size[1]);
    TEST_ASSERT_EQUAL(MBED_RAM1_START, stats.ram_start[1]);
#endif
#if defined(MBED_RAM2_START) && defined(MBED_RAM2_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.ram_size[2]);
    TEST_ASSERT_EQUAL(MBED_RAM2_SIZE, stats.ram_size[2]);
    TEST_ASSERT_EQUAL(MBED_RAM2_START, stats.ram_start[2]);
#endif
#if defined(MBED_RAM3_START) && defined(MBED_RAM3_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.ram_size[3]);
    TEST_ASSERT_EQUAL(MBED_RAM3_SIZE, stats.ram_size[3]);
    TEST_ASSERT_EQUAL(MBED_RAM3_START, stats.ram_start[3]);
#endif
#if defined(MBED_ROM1_START) && defined(MBED_ROM1_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.rom_size[1]);
    TEST_ASSERT_EQUAL(MBED_ROM1_SIZE, stats.rom_size[1]);
    TEST_ASSERT_EQUAL(MBED_ROM1_START, stats.rom_start[1]);
#endif
#if defined(MBED_ROM2_START) && defined(MBED_ROM2_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.rom_size[2]);
    TEST_ASSERT_EQUAL(MBED_ROM2_SIZE, stats.rom_size[2]);
    TEST_ASSERT_EQUAL(MBED_ROM2_START, stats.rom_start[2]);
#endif
#if defined(MBED_ROM3_START) && defined(MBED_ROM3_SIZE)
    TEST_ASSERT_NOT_EQUAL(0, stats.rom_size[3]);
    TEST_ASSERT_EQUAL(MBED_ROM3_SIZE, stats.rom_size[3]);
    TEST_ASSERT_EQUAL(MBED_ROM3_START, stats.rom_start[3]);
#endif
}

Case cases[] = {
    Case("Test Sys Info", test_sys_info)
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

#endif // !defined(MBED_SYS_STATS_ENABLED)
