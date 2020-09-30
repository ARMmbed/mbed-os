/* mbed Microcontroller Library
 * Copyright (c) 2018-2020 ARM Limited
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

#if !DEVICE_OSPI
#error [NOT_SUPPORTED] OSPI not supported for this target
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "ospi_test.h"
#include "ospi_test_utils.h"

#include "mbed.h"
#include "ospi_api.h"
#include "hal/us_ticker_api.h"


#if !defined(OSPI_FLASH_CHIP_STRING)
#error [NOT_SUPPORTED] OSPI test not supported for this target
#else

using namespace utest::v1;

// uncomment to enable verbose mode
//#define OSPI_TEST_LOG_DATA
//#define OSPI_TEST_LOG_FLASH_TIME
//#define OSPI_TEST_LOG_FLASH_STATUS



#ifndef OSPI_MIN_FREQUENCY
#define OSPI_MIN_FREQUENCY  1000000
#endif

// max write size is usually page size
#define DATA_SIZE_256  (OSPI_PAGE_SIZE)
#define DATA_SIZE_1024 (OSPI_PAGE_SIZE * 4)

uint8_t tx_buf[DATA_SIZE_1024];
uint8_t rx_buf[DATA_SIZE_1024];


// write address should be page aligned
#define TEST_FLASH_ADDRESS 0x0

#define TEST_REPEAT_SINGLE 1
#define TEST_REPEAT_MULTIPLE 4

// write block of data in single write operation
#define WRITE_SINGLE 1
// write block of data in adjacent locations in multiple write operations
#define WRITE_MULTIPLE 4

// read block of data in single read operation
#define READ_SINGLE 1
// read block of data in adjacent locations in multiple read operations
#define READ_MULTIPLE 4


// some target defines OSPI pins as integers thus conversion needed
#define OPIN_0 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO0)
#define OPIN_1 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO1)
#define OPIN_2 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO2)
#define OPIN_3 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO3)
#define OPIN_4 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO4)
#define OPIN_5 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO5)
#define OPIN_6 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO6)
#define OPIN_7 static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_IO7)
#define QSCK   static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_SCK)
#define QCSN   static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_CSN)
#define DQS    static_cast<PinName>(MBED_CONF_DRIVERS_OSPI_DQS)

static uint32_t gen_flash_address()
{
    srand(ticker_read(get_us_ticker_data()));
    uint32_t address = (((uint32_t)rand()) % OSPI_SECTOR_COUNT) * OSPI_SECTOR_SIZE;
    address &= 0xFFFFFF; // Ensure address is within 24 bits so as to not have to deal with 4-byte addressing
    return address;
}

static void log_data(const char *str, uint8_t *data, uint32_t size)
{
    utest_printf("%s: ", str);
    for (uint32_t j = 0; j < size; j++) {
        utest_printf("%02X ", data[j]);
    }
    utest_printf("\r\n");
}


static void _ospi_write_read_test(Ospi &ospi, ospi_bus_width_t write_inst_width, ospi_bus_width_t write_addr_width,
                                  ospi_bus_width_t write_data_width, ospi_bus_width_t write_alt_width, uint32_t write_cmd,
                                  ospi_address_size_t write_addr_size, ospi_alt_size_t write_alt_size,
                                  uint32_t write_count, ospi_bus_width_t read_inst_width, ospi_bus_width_t read_addr_width,
                                  ospi_bus_width_t read_data_width, ospi_bus_width_t read_alt_width, uint32_t read_cmd,
                                  int read_dummy_cycles, ospi_address_size_t read_addr_size, ospi_alt_size_t read_alt_size,
                                  uint32_t read_count, uint32_t test_count, uint32_t data_size,
                                  uint32_t flash_addr)
{
    ospi_status_t ret = OSPI_STATUS_OK;

    Timer timer;
    int erase_time = 0, write_time = 0, read_time = 0;
    size_t buf_len = data_size;

    for (uint32_t tc = 0; tc < test_count; tc++) {

        srand(ticker_read(get_us_ticker_data()));
        for (uint32_t i = 0; i < data_size; i++) {
            tx_buf[i] = (uint8_t)(rand() & 0xFF);
        }

        ret = write_enable(ospi);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

        timer.reset();
        timer.start();

        ret = erase(SECTOR_ERASE, flash_addr, ospi);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);
        WAIT_FOR(SECTOR_ERASE_MAX_TIME, ospi);

        timer.stop();
        erase_time = timer.read_us();

        // switching to extended-SPI/DPI/QPI mode here for write operation
        // for DPI/QPI ospi.cmd is automatically switched to 2_2_2/4_4_4 mode
        ret = mode_enable(ospi, write_inst_width, write_addr_width, write_data_width);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

        const uint32_t write_size = data_size / write_count;
        for (uint32_t wc = 0, write_start = flash_addr; wc < write_count; wc++, write_start += write_size) {
            ret = write_enable(ospi);
            TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

            timer.reset();
            timer.start();

            buf_len = write_size;
            ospi.cmd.configure(write_inst_width, write_addr_width, write_data_width, write_alt_width, write_addr_size, write_alt_size);
            ospi.cmd.build(write_cmd, write_start);
            ret = ospi_write(&ospi.handle, ospi.cmd.get(), tx_buf + wc * write_size, &buf_len);
            TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);
            TEST_ASSERT_EQUAL(write_size, buf_len);

            if (is_extended_mode(write_inst_width, write_addr_width, write_data_width)) {
                // on some flash chips in extended-SPI mode, control commands works only in 1-1-1 mode
                // so switching back to 1-1-1 mode
                ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);
            }

            WAIT_FOR(PAGE_PROG_MAX_TIME, ospi);

            timer.stop();
            write_time = timer.read_us();
        }

        // switching back to single channel SPI
        ret = mode_disable(ospi, write_inst_width, write_addr_width, write_data_width);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

        // switching to extended-SPI/DPI/QPI mode here for read operation
        // for DPI/QPI ospi.cmd is automatically switched to 2_2_2/4_4_4 mode
        ret = mode_enable(ospi, read_inst_width, read_addr_width, read_data_width);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

        memset(rx_buf, 0, sizeof(rx_buf));
        const uint32_t read_size = data_size / read_count;
        ospi.cmd.configure(read_inst_width, read_addr_width, read_data_width, read_alt_width, read_addr_size, read_alt_size, read_dummy_cycles);
        for (uint32_t rc = 0, read_start = flash_addr; rc < read_count; rc++, read_start += read_size) {
            timer.reset();
            timer.start();

            buf_len = read_size;
            ospi.cmd.build(read_cmd, read_start);
            ret = ospi_read(&ospi.handle, ospi.cmd.get(), rx_buf + rc * read_size, &buf_len);
            TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);
            TEST_ASSERT_EQUAL(read_size, buf_len);

            timer.stop();
            read_time = timer.read_us();
        }
        ospi.cmd.set_dummy_cycles(0);

        if (is_extended_mode(read_inst_width, read_addr_width, read_data_width)) {
            // on some flash chips in extended-SPI mode, control commands works only in 1-1-1 mode
            // so switching back to 1-1-1 mode
            ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);
        }

        // switching back to single channel SPI
        ret = mode_disable(ospi, read_inst_width, read_addr_width, read_data_width);
        TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

        for (uint32_t i = 0; i < data_size; i++) {
            if (tx_buf[i] != rx_buf[i]) {
                log_data("tx data", tx_buf, data_size);
                log_data("rx data", rx_buf, data_size);
                utest_printf("erase/write/read time: %d/%d/%d [us]\r\n", erase_time, write_time, read_time);
                TEST_ASSERT_EQUAL(tx_buf[i], rx_buf[i]);
            }
        }

#ifdef OSPI_TEST_LOG_FLASH_TIME
        utest_printf("erase/write/read time: %d/%d/%d [us]\r\n", erase_time, write_time, read_time);
#endif

#ifdef OSPI_TEST_LOG_DATA
        log_data("tx data", tx_buf, data_size);
        log_data("rx data", rx_buf, data_size);
        utest_printf("rx/tx data match\r\n");
#endif
    }
}


template <  ospi_bus_width_t write_inst_width,
            ospi_bus_width_t write_addr_width,
            ospi_bus_width_t write_data_width,
            ospi_bus_width_t write_alt_width,
            unsigned int write_cmd,
            ospi_address_size_t write_addr_size,
            ospi_alt_size_t write_alt_size,
            uint32_t write_count,
            ospi_bus_width_t read_inst_width,
            ospi_bus_width_t read_addr_width,
            ospi_bus_width_t read_data_width,
            ospi_bus_width_t read_alt_width,
            unsigned int read_cmd,
            int read_dummy_cycles,
            ospi_address_size_t read_addr_size,
            ospi_alt_size_t read_alt_size,
            int frequency,
            uint32_t read_count,
            uint32_t test_count,
            uint32_t data_size,
            uint32_t flash_addr>
void ospi_write_read_test(void)
{
    ospi_status_t ret;
    Ospi ospi;

    uint32_t addr = flash_addr;
    if (addr == 0) {
        // if no specified address selected, use random one to extend flash life
        addr = gen_flash_address();
    }

    ospi_init(&ospi.handle, OPIN_0, OPIN_1, OPIN_2, OPIN_3, OPIN_4, OPIN_5, OPIN_6, OPIN_7, QSCK, QCSN, DQS, OSPI_COMMON_MAX_FREQUENCY, 0);

    ret = ospi_frequency(&ospi.handle, frequency);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);
    flash_init(ospi);

    // switch memory to high performance mode (if available)
    ret = fast_mode_enable(ospi);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

#ifdef OSPI_TEST_LOG_FLASH_STATUS
    log_register(STATUS_REG, OSPI_STATUS_REG_SIZE, ospi, "Status register");
    log_register(CONFIG_REG0, OSPI_CONFIG_REG_0_SIZE, ospi, "Config register 0");
#ifdef CONFIG_REG1
    log_register(CONFIG_REG1, OSPI_CONFIG_REG_1_SIZE, ospi, "Config register 1");
#endif
#ifdef CONFIG_REG2
    log_register(CONFIG_REG2, OSPI_CONFIG_REG_2_SIZE, ospi, "Config register 2");
#endif
#endif

    _ospi_write_read_test(ospi, write_inst_width, write_addr_width, write_data_width, write_alt_width, write_cmd,
                          write_addr_size, write_alt_size, write_count, read_inst_width,
                          read_addr_width, read_data_width, read_alt_width, read_cmd, read_dummy_cycles,
                          read_addr_size, read_alt_size, read_count, test_count,
                          data_size, addr);

    ret = fast_mode_disable(ospi);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    ospi_free(&ospi.handle);
}


void ospi_init_free_test(void)
{
    Ospi ospi;
    ospi_status_t ret;

    ret = ospi_init(&ospi.handle, OPIN_0, OPIN_1, OPIN_2, OPIN_3, OPIN_4, OPIN_5, OPIN_6, OPIN_7, QSCK, QCSN, DQS, OSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);
    ret = ospi_free(&ospi.handle);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);


    ret = ospi_init(&ospi.handle, OPIN_0, OPIN_1, OPIN_2, OPIN_3, OPIN_4, OPIN_5, OPIN_6, OPIN_7, QSCK, QCSN, DQS, OSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);
    ret = ospi_free(&ospi.handle);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);


    ret = ospi_init(&ospi.handle, OPIN_0, OPIN_1, OPIN_2, OPIN_3, OPIN_4, OPIN_5, OPIN_6, OPIN_7, QSCK, QCSN, DQS, OSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);
    ret = ospi_free(&ospi.handle);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);


    ret = ospi_init(&ospi.handle, OPIN_0, OPIN_1, OPIN_2, OPIN_3, OPIN_4, OPIN_5, OPIN_6, OPIN_7, QSCK, QCSN, DQS, OSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    // check if the memory is working properly
    ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);
    flash_init(ospi);

#ifdef OSPI_TEST_LOG_FLASH_STATUS
    log_register(STATUS_REG, OSPI_STATUS_REG_SIZE, ospi, "Status register");
    log_register(CONFIG_REG0, OSPI_CONFIG_REG_0_SIZE, ospi, "Config register 0");
#ifdef CONFIG_REG1
    log_register(CONFIG_REG1, OSPI_CONFIG_REG_1_SIZE, ospi, "Config register 1");
#endif
#ifdef CONFIG_REG2
    log_register(CONFIG_REG2, OSPI_CONFIG_REG_2_SIZE, ospi, "Config register 2");
#endif
#endif

    _ospi_write_read_test(ospi, WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);
    ospi_free(&ospi.handle);
}


void ospi_frequency_test(void)
{
    Ospi ospi;
    ospi_status_t ret;
    int freq = OSPI_COMMON_MAX_FREQUENCY;

    ret = ospi_init(&ospi.handle, OPIN_0, OPIN_1, OPIN_2, OPIN_3, OPIN_4, OPIN_5, OPIN_6, OPIN_7, QSCK, QCSN, DQS, freq, 0);
    TEST_ASSERT_EQUAL(OSPI_STATUS_OK, ret);

    while (ret == OSPI_STATUS_OK && freq >= OSPI_MIN_FREQUENCY) {
        // check if the memory is working properly
        ospi.cmd.configure(MODE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8);
        ret = ospi_frequency(&ospi.handle, freq);
        flash_init(ospi);
        _ospi_write_read_test(ospi, WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);

        utest_printf("frequency setting %d [Hz] - OK\r\n", freq);

        freq /= 2;
    }

    ospi_free(&ospi.handle);
}


void ospi_memory_id_test()
{
    utest_printf("*** %s memory config loaded ***\r\n", OSPI_FLASH_CHIP_STRING);
}


Case cases[] = {
    Case("ospi memory id test", ospi_memory_id_test),
    Case("ospi init/free test", ospi_init_free_test),
    Case("ospi frequency setting test", ospi_frequency_test),
    //   read/x1 write/x1 - read/write block of data in single write/read operation
    //   read/x4 write/x4 - read/write block of data in adjacent locations in multiple write/read operations
    //   repeat/xN        - test repeat count (new data pattern each time)
    //   1-1-1            - single channel SPI
    //   1-1-2            - Dual data (extended SPI)
    //   1-2-2            - Dual I/O  (extended SPI)
    //   1-1-4            - Quad data (extended SPI)
    //   1-4-4            - Quad I/O  (extended SPI)
    //   2-2-2            - DPI (multi-channel SPI)
    //   4-4-4            - QPI (multi-channel SPI)
    Case("ospi write(1-1-1)/x1  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-1-1)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-1-1)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_1_1_2
    Case("ospi write(1-1-1)/x1  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-1-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-1-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_2_2
    Case("ospi write(1-1-1)/x1  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_2_2_2
    Case("ospi write(1-1-1)/x1  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(2-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(2-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_1_4
    Case("ospi write(1-1-1)/x1  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-1-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-1-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_4_4
    Case("ospi write(1-1-1)/x1  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(1-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_4_4_4
    Case("ospi write(1-1-1)/x1  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x4  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(4-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-1)/x1  read(4-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif

#ifdef WRITE_1_2_2
    Case("ospi write(1-2-2)/x1  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-1)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-1)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_2_2_2
    Case("ospi write(1-2-2)/x1  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(2-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(2-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_1_4
    Case("ospi write(1-2-2)/x1  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-1-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
    Case("ospi write(1-2-2)/x1  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(1-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_4_4_4
    Case("ospi write(1-2-2)/x1  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x4  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(4-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-2-2)/x1  read(4-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#endif

#ifdef WRITE_2_2_2
    Case("ospi write(2-2-2)/x1  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-1)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-1)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_2_2_2
    Case("ospi write(2-2-2)/x1  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(2-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(2-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_1_4
    Case("ospi write(2-2-2)/x1  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-1-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
    Case("ospi write(2-2-2)/x1  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(1-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_4_4_4
    Case("ospi write(2-2-2)/x1  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x4  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(4-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(2-2-2)/x1  read(4-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#endif

#ifdef WRITE_1_1_4
    Case("ospi write(1-1-4)/x1  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-1)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-1)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_2_2_2
    Case("ospi write(1-1-4)/x1  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(2-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(2-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_1_4
    Case("ospi write(1-1-4)/x1  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-1-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
    Case("ospi write(1-1-4)/x1  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(1-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_4_4_4
    Case("ospi write(1-1-4)/x1  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x4  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(4-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-1-4)/x1  read(4-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#endif

#ifdef WRITE_1_4_4
    Case("ospi write(1-4-4)/x1  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-1)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-1)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_2_2_2
    Case("ospi write(1-4-4)/x1  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(2-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(2-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_1_4
    Case("ospi write(1-4-4)/x1  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-1-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
    Case("ospi write(1-4-4)/x1  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(1-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_4_4_4
    Case("ospi write(1-4-4)/x1  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x4  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(4-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(1-4-4)/x1  read(4-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#endif

#ifdef WRITE_4_4_4
    Case("ospi write(4-4-4)/x1  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(1-1-1)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-1)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-1)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(1-1-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(1-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_2_2_2
    Case("ospi write(4-4-4)/x1  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(2-2-2)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(2-2-2)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(2-2-2)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_2_2_2, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef READ_1_1_4
    Case("ospi write(4-4-4)/x1  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(1-1-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-1-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
    Case("ospi write(4-4-4)/x1  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(1-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(1-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef READ_4_4_4
    Case("ospi write(4-4-4)/x1  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x4  read(4-4-4)/x1  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(4-4-4)/x4  repeat/x1  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(4-4-4)/x1  read(4-4-4)/x1  repeat/x4  test", ospi_write_read_test<WRITE_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_4_4_4, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#endif
#ifdef READ_8_8_8
    Case("ospi write(8_8_8)/x1  read(8_8_8)/x1  repeat/x1  test", ospi_write_read_test<WRITE_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(8_8_8)/x4  read(8_8_8)/x1  repeat/x1  test", ospi_write_read_test<WRITE_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(8_8_8)/x1  read(8_8_8)/x4  repeat/x1  test", ospi_write_read_test<WRITE_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(8_8_8)/x1  read(8_8_8)/x1  repeat/x4  test", ospi_write_read_test<WRITE_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_8_8_8, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif

#ifdef READ_8D_8D_8D
    Case("ospi write(8D_8D_8D)/x1  read(8D_8D_8D)/x1  repeat/x1  test", ospi_write_read_test<WRITE_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(8D_8D_8D)/x4  read(8D_8D_8D)/x1  repeat/x1  test", ospi_write_read_test<WRITE_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, WRITE_MULTIPLE, READ_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("ospi write(8D_8D_8D)/x1  read(8D_8D_8D)/x4  repeat/x1  test", ospi_write_read_test<WRITE_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("ospi write(8D_8D_8D)/x1  read(8D_8D_8D)/x1  repeat/x4  test", ospi_write_read_test<WRITE_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, WRITE_SINGLE,   READ_8D_8D_8D, ADDR_SIZE_32, ALT_SIZE_8, OSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(180, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // !defined(OSPI_FLASH_CHIP_STRING)
#endif // !DEVICE_OSPI
