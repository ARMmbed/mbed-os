/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

#include "analogin_api.h"
#include "analogout_api.h"
#include "can_api.h"
#include "i2c_api.h"
#include "pwmout_api.h"
#include "qspi_api.h"
#include "serial_api.h"
#include "spi_api.h"

#define PINMAP_TEST_ENTRY(function)     {function, #function}

typedef const PinMap *(*get_pinmap_func_t)(void);
typedef struct {
    get_pinmap_func_t function;
    const char *name;
} pinmap_info_t;

const pinmap_info_t pinmap_functions[] = {
#if DEVICE_ANALOGIN
    PINMAP_TEST_ENTRY(analogin_pinmap),
#endif
#if DEVICE_ANALOGOUT
    PINMAP_TEST_ENTRY(analogout_pinmap),
#endif
#if DEVICE_CAN
    PINMAP_TEST_ENTRY(can_rd_pinmap),
    PINMAP_TEST_ENTRY(can_td_pinmap),
#endif
#if DEVICE_I2C
    PINMAP_TEST_ENTRY(i2c_master_sda_pinmap),
    PINMAP_TEST_ENTRY(i2c_master_scl_pinmap),
#if DEVICE_I2CSLAVE
    PINMAP_TEST_ENTRY(i2c_slave_sda_pinmap),
    PINMAP_TEST_ENTRY(i2c_slave_scl_pinmap),
#endif
#endif
#if DEVICE_PWMOUT
    PINMAP_TEST_ENTRY(pwmout_pinmap),
#endif
#if DEVICE_QSPI
    PINMAP_TEST_ENTRY(qspi_master_sclk_pinmap),
    PINMAP_TEST_ENTRY(qspi_master_ssel_pinmap),
    PINMAP_TEST_ENTRY(qspi_master_data0_pinmap),
    PINMAP_TEST_ENTRY(qspi_master_data1_pinmap),
    PINMAP_TEST_ENTRY(qspi_master_data2_pinmap),
    PINMAP_TEST_ENTRY(qspi_master_data3_pinmap),
#endif
#if DEVICE_SERIAL
    PINMAP_TEST_ENTRY(serial_tx_pinmap),
    PINMAP_TEST_ENTRY(serial_rx_pinmap),
#if DEVICE_SERIAL_FC
    PINMAP_TEST_ENTRY(serial_cts_pinmap),
    PINMAP_TEST_ENTRY(serial_rts_pinmap),
#endif
#endif
#if DEVICE_SPI
    PINMAP_TEST_ENTRY(spi_master_mosi_pinmap),
    PINMAP_TEST_ENTRY(spi_master_miso_pinmap),
    PINMAP_TEST_ENTRY(spi_master_clk_pinmap),
    PINMAP_TEST_ENTRY(spi_master_cs_pinmap),
#if DEVICE_SPISLAVE
    PINMAP_TEST_ENTRY(spi_slave_mosi_pinmap),
    PINMAP_TEST_ENTRY(spi_slave_miso_pinmap),
    PINMAP_TEST_ENTRY(spi_slave_clk_pinmap),
    PINMAP_TEST_ENTRY(spi_slave_cs_pinmap),
#endif
#endif
    {NULL, NULL}
};

void pinmap_validation()
{
    for (size_t i = 0; i < sizeof(pinmap_functions) / sizeof(pinmap_functions[0]) - 1; i++) {
        printf("Testing pinmap %s\r\n", pinmap_functions[i].name);

        get_pinmap_func_t function = pinmap_functions[i].function;
        TEST_ASSERT_NOT_EQUAL(NULL, function);

        const PinMap *map = function();
        TEST_ASSERT_NOT_EQUAL(NULL, map);

        while (map->pin != NC) {
            map++;
        }

        TEST_ASSERT_EQUAL(NC, map->peripheral);
    }
}

Case cases[] = {
    Case("pinmap - validation", pinmap_validation)
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
