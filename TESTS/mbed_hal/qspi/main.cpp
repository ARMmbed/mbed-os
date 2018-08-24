/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#if !DEVICE_QSPI
#error [NOT_SUPPORTED] QSPI not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "qspi_test.h"
#include "qspi_test_utils.h"

#include "mbed.h"
#include "qspi_api.h"


#if !defined(QSPI_FLASH_CHIP_STRING)
#error [NOT_SUPPORTED] QSPI test not supported for this target
#endif

using namespace utest::v1;

// uncomment to enable verbose mode
//#define QSPI_TEST_LOG_DATA
//#define QSPI_TEST_LOG_FLASH_TIME
//#define QSPI_TEST_LOG_FLASH_STATUS


// max write size is usually page size
#define DATA_SIZE_256  (QSPI_PAGE_SIZE)
#define DATA_SIZE_1024 (QSPI_PAGE_SIZE * 4)

uint8_t tx_buf[DATA_SIZE_1024];
uint8_t rx_buf[DATA_SIZE_1024];


// write address should be page aligned
#define TEST_FLASH_ADDRESS 0x0

#define TEST_REPEAT_SINGLE 1
#define TEST_REPEAT_MULTIPLE 16

// write block of data in single write operation
#define WRITE_SINGLE 1
// write block of data in adjacent locations in multiple write operations
#define WRITE_MULTIPLE 4

// read block of data in single read operation
#define READ_SINGLE 1
// read block of data in adjacent locations in multiple read operations
#define READ_MULTIPLE 4


// some target defines QSPI pins as integers thus conversion needed
#define QPIN_0 static_cast<PinName>(QSPI_FLASH1_IO0)
#define QPIN_1 static_cast<PinName>(QSPI_FLASH1_IO1)
#define QPIN_2 static_cast<PinName>(QSPI_FLASH1_IO2)
#define QPIN_3 static_cast<PinName>(QSPI_FLASH1_IO3)
#define QSCK   static_cast<PinName>(QSPI_FLASH1_SCK)
#define QCSN   static_cast<PinName>(QSPI_FLASH1_CSN)


static void log_data(const char *str, uint8_t *data, uint32_t size)
{
    utest_printf("%s: ", str);
    for (uint32_t j = 0; j < size; j++) {
        utest_printf("%02X ", data[j]);
    }
    utest_printf("\r\n");
}


static void _qspi_write_read_test(Qspi &qspi, qspi_bus_width_t write_inst_width, qspi_bus_width_t write_addr_width,
                                  qspi_bus_width_t write_data_width, qspi_bus_width_t write_alt_width, uint32_t write_cmd,
                                  qspi_address_size_t write_addr_size, qspi_alt_size_t write_alt_size, int write_frequency,
                                  uint32_t write_count, qspi_bus_width_t read_inst_width, qspi_bus_width_t read_addr_width,
                                  qspi_bus_width_t read_data_width, qspi_bus_width_t read_alt_width, uint32_t read_cmd,
                                  int read_dummy_cycles, qspi_address_size_t read_addr_size, qspi_alt_size_t read_alt_size,
                                  int read_frequency, uint32_t read_count, uint32_t test_count, uint32_t data_size,
                                  uint32_t flash_addr)
{
    qspi_status_t ret = QSPI_STATUS_OK;

    Timer timer;
    int erase_time = 0, write_time = 0, read_time = 0;
    size_t buf_len = data_size;

    for (uint32_t tc = 0; tc < test_count; tc++) {
        qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);

        srand(ticker_read(get_us_ticker_data()));
        for (uint32_t i = 0; i < data_size; i++) {
            tx_buf[i] = (uint8_t)(rand() & 0xFF);
        }

        ret = write_enable(qspi);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

        timer.reset();
        timer.start();

        ret = erase(SECTOR_ERASE, flash_addr, qspi);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
        WAIT_FOR(SECTOR_ERASE_MAX_TIME, qspi);

        timer.stop();
        erase_time = timer.read_us();

        if (write_frequency != QSPI_NONE) {
            qspi_frequency(&qspi.handle, write_frequency);
            WAIT_FOR(WAIT_MAX_TIME, qspi);
        }

        const uint32_t write_size = data_size / write_count;
        for (uint32_t wc = 0, write_start = flash_addr; wc < write_count; wc++, write_start += write_size) {
            ret = write_enable(qspi);
            TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

            timer.reset();
            timer.start();

            buf_len = write_size;
            qspi.cmd.configure(write_inst_width, write_addr_width, write_data_width, write_alt_width, write_addr_size, write_alt_size);
            qspi.cmd.build(write_cmd, write_start);
            ret = qspi_write(&qspi.handle, qspi.cmd.get(), tx_buf + wc * write_size, &buf_len);
            TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
            TEST_ASSERT_EQUAL(write_size, buf_len);

            qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
            WAIT_FOR(PAGE_PROG_MAX_TIME, qspi);

            timer.stop();
            write_time = timer.read_us();
        }

        if (read_frequency != QSPI_NONE) {
            qspi_frequency(&qspi.handle, read_frequency);
            WAIT_FOR(WAIT_MAX_TIME, qspi);
        }

        memset(rx_buf, 0, sizeof(rx_buf));
        const uint32_t read_size = data_size / read_count;
        qspi.cmd.configure(read_inst_width, read_addr_width, read_data_width, read_alt_width, read_addr_size, read_alt_size, read_dummy_cycles);
        for (uint32_t rc = 0, read_start = flash_addr; rc < read_count; rc++, read_start += read_size) {
            timer.reset();
            timer.start();

            buf_len = read_size;
            qspi.cmd.build(read_cmd, read_start);
            ret = qspi_read(&qspi.handle, qspi.cmd.get(), rx_buf + rc * read_size, &buf_len);
            TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
            TEST_ASSERT_EQUAL(read_size, buf_len);

            timer.stop();
            read_time = timer.read_us();
        }

        for (uint32_t i = 0; i < data_size; i++) {
            if (tx_buf[i] != rx_buf[i]) {
                log_data("tx data", tx_buf, data_size);
                log_data("rx data", rx_buf, data_size);
                utest_printf("erase/write/read time: %d/%d/%d [us]\r\n", erase_time, write_time, read_time);
                TEST_ASSERT_EQUAL(tx_buf[i], rx_buf[i]);
            }
        }

#ifdef QSPI_TEST_LOG_FLASH_TIME
        utest_printf("erase/write/read time: %d/%d/%d [us]\r\n", erase_time, write_time, read_time);
#endif

#ifdef QSPI_TEST_LOG_DATA
        log_data("tx data", tx_buf, data_size);
        log_data("rx data", rx_buf, data_size);
        utest_printf("rx/tx data match\r\n");
#endif
    }
}


template <  qspi_bus_width_t write_inst_width,
            qspi_bus_width_t write_addr_width,
            qspi_bus_width_t write_data_width,
            qspi_bus_width_t write_alt_width,
            unsigned int write_cmd,
            qspi_address_size_t write_addr_size,
            qspi_alt_size_t write_alt_size,
            int write_frequency,
            uint32_t write_count,
            qspi_bus_width_t read_inst_width,
            qspi_bus_width_t read_addr_width,
            qspi_bus_width_t read_data_width,
            qspi_bus_width_t read_alt_width,
            unsigned int read_cmd,
            int read_dummy_cycles,
            qspi_address_size_t read_addr_size,
            qspi_alt_size_t read_alt_size,
            int read_frequency,
            uint32_t read_count,
            uint32_t test_count,
            uint32_t data_size,
            uint32_t flash_addr>
void qspi_write_read_test(void)
{
    qspi_status_t ret;
    Qspi qspi;
    qspi_init(&qspi.handle, QPIN_0, QPIN_1, QPIN_2, QPIN_3, QSCK, QCSN, QSPI_COMMON_MAX_FREQUENCY, 0);

    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
    flash_init(qspi);

    if (is_dual_cmd(write_inst_width, write_addr_width, write_data_width) ||
            is_dual_cmd(read_inst_width, read_addr_width, read_data_width)) {
        ret = dual_enable(qspi);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
        WAIT_FOR(WRSR_MAX_TIME, qspi);
    }

    if (is_quad_cmd(write_inst_width, write_addr_width, write_data_width) ||
            is_quad_cmd(read_inst_width, read_addr_width, read_data_width)) {
        ret = quad_enable(qspi);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
        WAIT_FOR(WRSR_MAX_TIME, qspi);
    }

    ret = write_enable(qspi);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    WAIT_FOR(WRSR_MAX_TIME, qspi);
    ret = fast_mode_enable(qspi);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    WAIT_FOR(WRSR_MAX_TIME, qspi);

#ifdef QSPI_TEST_LOG_FLASH_STATUS
    //utest_printf("Status register:\r\n");
    log_register(STATUS_REG, QSPI_STATUS_REG_SIZE, qspi, "Status register");
    //utest_printf("Config register 0:\r\n");
    log_register(CONFIG_REG0, QSPI_CONFIG_REG_0_SIZE, qspi, "Config register 0");
#ifdef CONFIG_REG1
    //utest_printf("Config register 1:\r\n");
    log_register(CONFIG_REG1, QSPI_CONFIG_REG_1_SIZE, qspi, "Config register 1");
#endif
#ifdef CONFIG_REG2
    //utest_printf("Config register 2:\r\n");
    log_register(CONFIG_REG2, QSPI_CONFIG_REG_2_SIZE, qspi, "Config register 2");
#endif
#endif

    _qspi_write_read_test(qspi, write_inst_width, write_addr_width, write_data_width, write_alt_width, write_cmd,
                          write_addr_size, write_alt_size, write_frequency, write_count, read_inst_width,
                          read_addr_width, read_data_width, read_alt_width, read_cmd, read_dummy_cycles,
                          read_addr_size, read_alt_size, read_frequency, read_count, test_count,
                          data_size, flash_addr);

    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);

    if (is_dual_cmd(write_inst_width, write_addr_width, write_data_width) ||
            is_dual_cmd(read_inst_width, read_addr_width, read_data_width)) {
        ret = dual_disable(qspi);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
        WAIT_FOR(WRSR_MAX_TIME, qspi);
    }

    if (is_quad_cmd(write_inst_width, write_addr_width, write_data_width) ||
            is_quad_cmd(read_inst_width, read_addr_width, read_data_width)) {
        ret = quad_disable(qspi);
        TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
        WAIT_FOR(WRSR_MAX_TIME, qspi);
    }

    qspi_free(&qspi.handle);
}


void qspi_init_free_test(void)
{
    Qspi qspi;
    qspi_status_t ret;

    ret = qspi_init(&qspi.handle, QPIN_0, QPIN_1, QPIN_2, QPIN_3, QSCK, QCSN, QSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    ret = qspi_free(&qspi.handle);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);


    ret = qspi_init(&qspi.handle, QPIN_0, QPIN_1, QPIN_2, QPIN_3, QSCK, QCSN, QSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    ret = qspi_free(&qspi.handle);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);


    ret = qspi_init(&qspi.handle, QPIN_0, QPIN_1, QPIN_2, QPIN_3, QSCK, QCSN, QSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    ret = qspi_free(&qspi.handle);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);


    ret = qspi_init(&qspi.handle, QPIN_0, QPIN_1, QPIN_2, QPIN_3, QSCK, QCSN, QSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    // check if the memory is working properly
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
    flash_init(qspi);

#ifdef QSPI_TEST_LOG_FLASH_STATUS
    //utest_printf("Status register:\r\n");
    log_register(STATUS_REG, QSPI_STATUS_REG_SIZE, qspi, "Status register");
    //utest_printf("Config register 0:\r\n");
    log_register(CONFIG_REG0, QSPI_CONFIG_REG_0_SIZE, qspi, "Config register 0");
#ifdef CONFIG_REG1
    //utest_printf("Config register 1:\r\n");
    log_register(CONFIG_REG1, QSPI_CONFIG_REG_1_SIZE, qspi, "Config register 1");
#endif
#ifdef CONFIG_REG2
    //utest_printf("Config register 2:\r\n");
    log_register(CONFIG_REG2, QSPI_CONFIG_REG_2_SIZE, qspi, "Config register 2");
#endif
#endif

    _qspi_write_read_test(qspi, WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);
    qspi_free(&qspi.handle);
}


void qspi_frequency_test(void)
{
    Qspi qspi;
    qspi_status_t ret;

    ret = qspi_init(&qspi.handle, QPIN_0, QPIN_1, QPIN_2, QPIN_3, QSCK, QCSN, QSPI_COMMON_MAX_FREQUENCY, 0);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);

    ret = qspi_frequency(&qspi.handle, QSPI_COMMON_MAX_FREQUENCY);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    // check if the memory is working properly
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
    flash_init(qspi);
    _qspi_write_read_test(qspi, WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);

    ret = qspi_frequency(&qspi.handle, QSPI_COMMON_MAX_FREQUENCY / 2);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    // check if the memory is working properly
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
    flash_init(qspi);
    _qspi_write_read_test(qspi, WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);

    ret = qspi_frequency(&qspi.handle, QSPI_COMMON_MAX_FREQUENCY / 4);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    // check if the memory is working properly
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
    flash_init(qspi);
    _qspi_write_read_test(qspi, WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);

    ret = qspi_frequency(&qspi.handle, QSPI_COMMON_MAX_FREQUENCY / 8);
    TEST_ASSERT_EQUAL(QSPI_STATUS_OK, ret);
    // check if the memory is working properly
    qspi.cmd.configure(MODE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8);
    flash_init(qspi);
    _qspi_write_read_test(qspi, WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, WRITE_SINGLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_NONE, READ_SINGLE, TEST_REPEAT_SINGLE, DATA_SIZE_256, TEST_FLASH_ADDRESS);

    qspi_free(&qspi.handle);
}


void qspi_memory_id_test()
{
    utest_printf("*** %s memory config loaded ***\r\n", QSPI_FLASH_CHIP_STRING);
}


Case cases[] = {
    Case("qspi memory id test", qspi_memory_id_test),
    Case("qspi init/free test", qspi_init_free_test),
    Case("qspi frequency setting test", qspi_frequency_test),
    //   read/x1 write/x1 - read/write block of data in single write/read operation
    //   read/x4 write/x4 - read/write block of data in adjacent locations in multiple write/read operations
    //   repeat/xN        - test repeat count (new data pattern each time)
    Case("qspi write(1-1-1)/x1  read(1-1-1)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x4  read(1-1-1)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-1)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-1)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x4  read(1-1-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-2)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-2)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-2-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x4  read(1-2-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-2-2)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-2-2)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x4  read(1-1-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-4)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-1-4)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-4-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x4  read(1-4-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-4-4)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-1-1)/x1  read(1-4-4)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#ifdef QSPI_CMD_WRITE_2IO
    Case("qspi write(1-2-2)/x1  read(1-1-1)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x4  read(1-1-1)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-1)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-1)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x4  read(1-1-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-2)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-2)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-2-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x4  read(1-2-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-2-2)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-2-2)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x4  read(1-1-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-4)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-1-4)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-4-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x4  read(1-4-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-4-4)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-2-2)/x1  read(1-4-4)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
#endif
#ifdef QSPI_CMD_WRITE_4IO
    Case("qspi write(1-4-4)/x1  read(1-1-1)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x4  read(1-1-1)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-1)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-1)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_1, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x4  read(1-1-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-2)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-2)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-2-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x4  read(1-2-2)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-2-2)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-2-2)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_2_2, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x4  read(1-1-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-4)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-1-4)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_1_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-4-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x4  read(1-4-4)/x1  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_MULTIPLE, READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_SINGLE,   DATA_SIZE_1024, TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-4-4)/x4  repeat/x1  test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_MULTIPLE, TEST_REPEAT_SINGLE,   DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
    Case("qspi write(1-4-4)/x1  read(1-4-4)/x1  repeat/x16 test", qspi_write_read_test<WRITE_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, WRITE_SINGLE,   READ_1_4_4, ADDR_SIZE_24, ALT_SIZE_8, QSPI_COMMON_MAX_FREQUENCY, READ_SINGLE,   TEST_REPEAT_MULTIPLE, DATA_SIZE_256,  TEST_FLASH_ADDRESS>),
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

