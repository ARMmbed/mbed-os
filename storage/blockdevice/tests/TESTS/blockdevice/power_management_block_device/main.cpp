/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "BlockDevice.h"
#include <algorithm>
#include "PowerManagementBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#if COMPONENT_QSPIF
#include "QSPIFBlockDevice.h"
QSPIFBlockDevice pm_bd(MBED_CONF_QSPIF_QSPI_IO0,
                       MBED_CONF_QSPIF_QSPI_IO1,
                       MBED_CONF_QSPIF_QSPI_IO2,
                       MBED_CONF_QSPIF_QSPI_IO3,
                       MBED_CONF_QSPIF_QSPI_SCK,
                       MBED_CONF_QSPIF_QSPI_CSN,
                       MBED_CONF_QSPIF_QSPI_POLARITY_MODE,
                       MBED_CONF_QSPIF_QSPI_FREQ);
#endif

BlockDevice *_bd = &pm_bd;
static const bd_size_t read_size = 1;
static const bd_size_t prog_size = 1;
static const bd_size_t erase_size = 4096;
static const bd_size_t block_size = 65536;
static const bd_size_t num_blocks = 128;
static const bd_size_t test_buf_size = 64;
static const uint8_t   blank = 0xFF;

// Simple test for all APIs
void functionality_test()
{
    /*before the test, set the frequency to 80Mhz.*/
    PowerManagementBlockDevice bd(_bd);
    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(num_blocks * block_size, bd.size());
    TEST_ASSERT_EQUAL(read_size, bd.get_read_size());
    TEST_ASSERT_EQUAL(prog_size, bd.get_program_size());
    TEST_ASSERT_EQUAL(erase_size, bd.get_erase_size());
    TEST_ASSERT_EQUAL(blank, bd.get_erase_value());

    uint8_t read_buf[test_buf_size], write_buf[test_buf_size];

    srand(1);
    for (bd_size_t i = 0; i < test_buf_size; i++) {
        write_buf[i] = 0xff & rand();
    }

    err = bd.erase(0, erase_size);
    TEST_ASSERT_EQUAL(err, 0);

    err = bd.switch_power_management_mode(QSPIF_DEEP_DOWN_MODE);
    TEST_ASSERT_EQUAL(err, 0);

    err = bd.program(write_buf, 0, prog_size);
    if (err) {
        utest_printf("\nprogram fail, the device switch into deep down mode\n");
    }

    err = bd.switch_power_management_mode(QSPIF_STANDBY_MODE);
    TEST_ASSERT_EQUAL(err, 0);

    err = bd.program(write_buf, 0, prog_size);
    TEST_ASSERT_EQUAL(err, 0);

    if (!err) {
        utest_printf("\nprogram successfully, the device switch into standby mode\n");
    }

    for (int i, k = 0; i < 10000; i++) {
        k++;
    }

    err = bd.switch_power_management_mode(QSPIF_LOW_POWER_MODE);
    TEST_ASSERT_EQUAL(err, 0);

    err = bd.read(read_buf, 0, prog_size);
    if (memcmp(write_buf, read_buf, prog_size) != 0) {
        utest_printf("\nread fail, the device switch into low power mode\n");
    }

    err = bd.switch_power_management_mode(QSPIF_HIGH_PERFORMANCE_MODE);
    TEST_ASSERT_EQUAL(err, 0);

    err = bd.read(read_buf, 0, prog_size);
    TEST_ASSERT_EQUAL(err, 0);
    if (memcmp(write_buf, read_buf, prog_size) == 0) {
        utest_printf("\nread successfully, the device switch into high performance mode\n");
    }
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("PowerManagementBlockDevice functionality test", functionality_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

