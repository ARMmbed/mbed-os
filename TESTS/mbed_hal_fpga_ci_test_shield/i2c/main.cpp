/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !DEVICE_I2C
#error [NOT_SUPPORTED] I2C not supported for this target
#elif !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test

#elif !defined(TARGET_FF_ARDUINO) && !defined(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#error [NOT_SUPPORTED] Test not supported for this form factor
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "i2c_api.h"
#include "pinmap.h"
#include "test_utils.h"
#include "I2CTester.h"
#include "i2c_test.h"
#include "hal/us_ticker_api.h"

using namespace utest::v1;

#define DEVICE_AS_MASTER   false
#define DEVICE_AS_SLAVE    true

// generate stop condition at each transfer end
#define STOP_EACH       true
// generate stop condition at the test end
#define STOP_AT_END     false

#define STANDARD_MODE_FREQ_100kHz   100000
#define FAST_MODE_FREQ_400kHz       400000
#define FAST_MODE_PLUS_FREQ_1MHz    1000000

#define NACK    0
#define ACK     1
#define TIMEOUT 2


#define I2C_ADDRESS_MASK_7BIT   (0x7F)
#define I2C_ADDRESS_MASK_10BIT  (0x3FF)
#define I2C_10BIT_FIRST_BYTE    (0xF0)

// 7bit addressing, allowed values from 0x8 to 0x77
#define I2C_7BIT_ADDRESS_MIN    (0x08)
#define I2C_7BIT_ADDRESS_MAX    (0x77)
#define I2C_7BIT_ADDRESS        (0x55)

#define MAKE_7BIT_SLAVE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)
#define MAKE_7BIT_READ_ADDRESS(addr)    (((I2C_ADDRESS_MASK_7BIT & addr) << 1) | 1)
#define MAKE_7BIT_WRITE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)


I2CTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

#if DEVICE_I2C_ASYNCH
struct  async_status {
    volatile i2c_async_event_t event;
    volatile bool done;
};

void async_callback(i2c_t *obj, i2c_async_event_t *event, void *ctx)
{
    async_status *s = ((async_status *)ctx);
    s->event.sent_bytes = event->sent_bytes;
    s->event.received_bytes = event->received_bytes;
    s->event.error = event->error;
    s->event.error_status = event->error_status;
    s->done = true;
}
#endif //DEVICE_I2C_ASYNCH


// Test that i2c_get_capabilities fills given i2c_capabilities_t instance with capabilities
void fpga_i2c_test_get_capabilities()
{
    i2c_capabilities_t cap;

    i2c_get_capabilities(&cap);

    TEST_ASSERT_TRUE(cap.minimum_frequency > 0);
    TEST_ASSERT_TRUE(cap.minimum_frequency <= cap.maximum_frequency);
    // at least Standard and Fast mode is supported
    TEST_ASSERT_TRUE(cap.minimum_frequency <= STANDARD_MODE_FREQ_100kHz && cap.maximum_frequency >= STANDARD_MODE_FREQ_100kHz);
    TEST_ASSERT_TRUE(cap.minimum_frequency <= FAST_MODE_FREQ_400kHz && cap.maximum_frequency >= FAST_MODE_FREQ_400kHz);

#if DEVICE_I2CSLAVE
    TEST_ASSERT_TRUE(cap.supports_slave_mode);
#else
    TEST_ASSERT_FALSE(cap.supports_slave_mode);
#endif
}

// Test that i2c_init successfully initializes and i2c_free successfully frees i2c interface
void fpga_i2c_test_init_free(PinName sda, PinName scl)
{
    i2c_t obj = {};

    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);
#endif

    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);
#endif

    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);
#endif

    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);

    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_frequency(&obj, STANDARD_MODE_FREQ_100kHz);
    i2c_free(&obj);
#endif
}

//  Test that i2c_frequency successfully sets given frequency
void fpga_i2c_test_frequency(PinName sda, PinName scl)
{
    i2c_t obj = {};
    uint32_t set_frequency;
    i2c_init(&obj, sda, scl, DEVICE_AS_MASTER);

    i2c_capabilities_t cap;
    i2c_get_capabilities(&cap);

    set_frequency = i2c_frequency(&obj, cap.minimum_frequency);
    TEST_ASSERT_EQUAL(cap.minimum_frequency, set_frequency);

    set_frequency = i2c_frequency(&obj, (cap.minimum_frequency + cap.maximum_frequency) / 2);
    TEST_ASSERT(set_frequency >= cap.minimum_frequency && set_frequency <= cap.maximum_frequency);

    set_frequency = i2c_frequency(&obj, cap.maximum_frequency);
    TEST_ASSERT_EQUAL(cap.maximum_frequency, set_frequency);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, sda, scl, DEVICE_AS_SLAVE);
    i2c_get_capabilities(&cap);
    set_frequency = i2c_frequency(&obj, cap.minimum_frequency);

    TEST_ASSERT_EQUAL(cap.minimum_frequency, set_frequency);
    set_frequency = i2c_frequency(&obj, (cap.minimum_frequency + cap.maximum_frequency) / 2);

    TEST_ASSERT(set_frequency >= cap.minimum_frequency && set_frequency <= cap.maximum_frequency);
    set_frequency = i2c_frequency(&obj, cap.maximum_frequency);

    TEST_ASSERT_EQUAL(cap.maximum_frequency, set_frequency);
    i2c_free(&obj);
#endif
}

void fpga_i2c_test_blocking_transmission_timeout(PinName sda, PinName scl, uint32_t frequency)
{
    // Remap pins for test
    tester.pin_map_reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    int num_writes = 0;
    int num_reads = 0;
    int num_acks = 0;
    int num_nacks = 0;
    int num_starts = 0;
    int num_stops = 0;
    int num_dev_addr_matches = 0;
    int num_dev_addr_mismatches = 0;
    int ret;
    // byte_time = (1/frequency * 10 * 1000000)
    const uint32_t byte_time_us = (10 * 1000000) / frequency;
    const uint32_t byte_timeout_us = (3 * 10 * 1000000) / frequency;

    // Initialize Mbed I2C pins
    i2c_t i2c = {};
    i2c_init(&i2c, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&i2c, frequency);

    uint8_t data[10] = { 111, 112, 113, 114, 115, 116, 117, 118, 119, 120 };
    const uint32_t data_size = sizeof(data);

    ret = i2c_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, true);
    TEST_ASSERT_EQUAL(data_size, ret);
    num_starts += 1;
    num_stops += 1;
    num_writes += data_size;
    num_acks += 1; // address match
    num_acks += ret;
    num_dev_addr_matches += 1;

    i2c_timeout(&i2c, byte_time_us);
    ret = i2c_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, true);
    TEST_ASSERT_EQUAL(I2C_ERROR_TIMEOUT, ret);
    num_starts += 1;
    num_stops += 1;
    num_writes += data_size;
    num_acks += 1; // address match
    num_acks += data_size;
    num_dev_addr_matches += 1;

    i2c_timeout(&i2c, byte_timeout_us * (data_size + 1));
    ret = i2c_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, true);
    TEST_ASSERT_EQUAL(data_size, ret);
    num_starts += 1;
    num_stops += 1;
    num_writes += data_size;
    num_acks += 1; // address match
    num_acks += ret;
    num_dev_addr_matches += 1;

    ret = i2c_read(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(data_size, ret);
    num_starts += 1;
    num_stops += 1;
    num_reads += data_size;
    num_acks += 1; // address match
    num_acks += ret - 1;
    num_nacks += 1;
    num_dev_addr_matches += 1;

    i2c_timeout(&i2c, byte_time_us);
    ret = i2c_read(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(I2C_ERROR_TIMEOUT, ret);
    num_starts += 1;
    num_stops += 1;
    num_reads += data_size;
    num_acks += 1; // address match
    num_acks += data_size - 1;
    num_nacks += 1;
    num_dev_addr_matches += 1;

    i2c_timeout(&i2c, byte_timeout_us * (data_size + 1));
    ret = i2c_read(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(data_size, ret);
    num_starts += 1;
    num_stops += 1;
    num_reads += data_size;
    num_acks += 1; // address match
    num_acks += ret - 1;
    num_nacks += 1;
    num_dev_addr_matches += 1;

    TEST_ASSERT(tester.num_writes() <= num_writes);
    TEST_ASSERT(tester.num_reads() <= num_reads);
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT(tester.transfer_count() <= num_writes + num_reads + num_dev_addr_matches);
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT(tester.num_acks() <= num_acks);
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());

    i2c_free(&i2c);

    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
}

void fpga_i2c_test_transmission_no_slave(PinName sda, PinName scl, uint32_t frequency)
{
    // Remap pins for test
    tester.pin_map_reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    int num_writes = 0;
    int num_reads = 0;
    int num_acks = 0;
    int num_nacks = 0;
    int num_starts = 0;
    int num_stops = 0;
    int num_dev_addr_matches = 0;
    int num_dev_addr_mismatches = 0;
    int ret;

    // Initialize Mbed I2C pins
    i2c_t i2c = {};
    i2c_init(&i2c, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&i2c, frequency);

    uint8_t data[1] = { 123 };
    const uint32_t data_size = sizeof(data);

    ret = i2c_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS_MIN), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(I2C_ERROR_NO_SLAVE, ret);
    num_starts += 1;
    num_stops += 1;
    num_nacks += 1;
    num_dev_addr_mismatches += 1;

    ret = i2c_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(data_size, ret);
    num_starts += 1;
    num_stops += 1;
    num_writes += data_size;
    num_acks += 1; // address match
    num_acks += ret;
    num_dev_addr_matches += 1;

    ret = i2c_read(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS_MIN), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(I2C_ERROR_NO_SLAVE, ret);
    num_starts += 1;
    num_stops += 1;
    num_nacks += 1;
    num_dev_addr_mismatches += 1;

    ret = i2c_read(&i2c, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), data, data_size, STOP_EACH);
    TEST_ASSERT_EQUAL(data_size, ret);
    num_starts += 1;
    num_stops += 1;
    num_reads += data_size;
    num_acks += 1; // address match
    num_nacks += 1; // end of transfer
    num_dev_addr_matches += 1;

#if DEVICE_I2C_ASYNCH
    async_status transfer_status = {};
    i2c_transfer_async(&i2c, data, data_size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS_MIN), STOP_EACH, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(true, transfer_status.event.error);
    ret = transfer_status.event.error_status == 0 ? transfer_status.event.sent_bytes : transfer_status.event.error_status;
    TEST_ASSERT_EQUAL(I2C_ERROR_NO_SLAVE, ret);
    num_starts += 1;
    num_stops += 1;
    num_nacks += 1;
    num_dev_addr_mismatches += 1;

    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, data, data_size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), STOP_EACH, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);
    TEST_ASSERT_EQUAL(data_size, transfer_status.event.sent_bytes);
    num_starts += 1;
    num_stops += 1;
    num_writes += data_size;
    num_acks += 1; // address match
    num_acks += transfer_status.event.sent_bytes;
    num_dev_addr_matches += 1;

    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, NULL, 0, data, data_size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS_MIN), STOP_EACH, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(true, transfer_status.event.error);
    ret = transfer_status.event.error_status == 0 ? transfer_status.event.received_bytes : transfer_status.event.error_status;
    TEST_ASSERT_EQUAL(I2C_ERROR_NO_SLAVE, ret);
    num_starts += 1;
    num_stops += 1;
    num_nacks += 1;
    num_dev_addr_mismatches += 1;

    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, NULL, 0, data, data_size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), STOP_EACH, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);
    TEST_ASSERT_EQUAL(data_size, transfer_status.event.received_bytes);
    num_starts += 1;
    num_stops += 1;
    num_reads += data_size;
    num_acks += 1; // address match
    num_nacks += 1; // end of transfer
    num_acks += transfer_status.event.received_bytes - 1;
    num_dev_addr_matches += 1;
#endif //DEVICE_I2C_ASYNCH

    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_dev_addr_mismatches, tester.num_dev_addr_mismatches());
    TEST_ASSERT_EQUAL(num_writes + num_reads + num_dev_addr_matches + num_dev_addr_mismatches, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(0, tester.state_num());

    i2c_free(&i2c);

    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
}

void fpga_i2c_test_blocking_write_read(PinName sda, PinName scl, uint16_t addr, uint32_t frequency, TransferType ttype, uint32_t data_size, uint32_t transfer_count, bool stop_each)
{
    // Remap pins for test
    tester.pin_map_reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Reset tester stats and select SPI
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(addr));

    int num_writes = 0;
    int num_reads = 0;
    int num_acks = 0;
    int num_nacks = 0;
    int num_starts = 0;
    int num_stops = 0;
    uint32_t write_checksum = 0;
    uint32_t read_checksum = 0;
    int num_dev_addr_matches = 0;

    // Initialize Mbed I2C pins
    i2c_t i2c = {};
    i2c_init(&i2c, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&i2c, frequency);

    uint8_t *data_out = new uint8_t[data_size];
    uint8_t *data_in = new uint8_t[data_size];
    srand(ticker_read(get_us_ticker_data()));
    for (uint32_t i = 0; i < data_size; i++) {
        data_out[i] = (uint8_t)(rand() & 0xFF);
        data_in[i] = 0;
    }

    for (uint32_t i = 0; i < transfer_count; i++) {
        if (ttype == WRITE_READ || ttype == WRITE) {
            int _num_writes = i2c_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(addr), data_out, data_size, ttype == WRITE_READ ? STOP_AT_END : stop_each);
            TEST_ASSERT_EQUAL(data_size, _num_writes);
            num_starts += 1;
            num_dev_addr_matches += 1;
            num_acks += 1; // address match
            num_writes += _num_writes;
            num_acks += _num_writes;
            for (uint32_t i = 0; i < data_size; i++) {
                write_checksum += data_out[i];
            }
        }
        if (ttype == WRITE_READ || ttype == READ) {
            int _num_reads  = i2c_read(&i2c, MAKE_7BIT_READ_ADDRESS(addr), data_in, data_size, stop_each);
            TEST_ASSERT_EQUAL(data_size, _num_reads);
            num_starts += 1;
            num_dev_addr_matches += 1;
            num_acks += 1; // address match
            num_acks += data_size - 1;
            num_reads += _num_reads;
            num_nacks += 1; // end of transfer
            for (uint32_t i = 0; i < data_size; i++) {
                read_checksum += data_in[i];
            }
        }
        if (stop_each) {
            num_stops += 1;
        }
    }
    if (!stop_each) {
        num_stops += 1;
        i2c_stop(&i2c);
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    if (ttype == WRITE_READ || ttype == WRITE) {
        TEST_ASSERT_EQUAL(data_size * transfer_count, num_writes);
    }
    if (ttype == WRITE_READ || ttype == READ) {
        TEST_ASSERT_EQUAL(data_size * transfer_count, num_reads);
    }
    TEST_ASSERT_EQUAL(num_writes + num_reads + ((ttype == WRITE_READ) ? 2 : 1) * transfer_count, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(write_checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(read_checksum, tester.get_send_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());

    if (ttype == WRITE_READ || ttype == WRITE) {
        if (data_size > 3) {
            TEST_ASSERT_EQUAL(data_out[data_size - 4], tester.get_prev_to_slave_4());
        }
        if (data_size > 2) {
            TEST_ASSERT_EQUAL(data_out[data_size - 3], tester.get_prev_to_slave_3());
        }
        if (data_size > 1) {
            TEST_ASSERT_EQUAL(data_out[data_size - 2], tester.get_prev_to_slave_2());
        }
        TEST_ASSERT_EQUAL(data_out[data_size - 1], tester.get_prev_to_slave_1());
    }
    if (ttype == WRITE_READ || ttype == READ) {
        TEST_ASSERT_EQUAL(((data_size * transfer_count + 1) & 0xFF), tester.get_next_from_slave());
    }
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    i2c_free(&i2c);
    delete [] data_out;
    delete [] data_in;

    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
}

#if DEVICE_I2C_ASYNCH
void fpga_i2c_test_async_write_read(PinName sda, PinName scl, uint16_t addr, uint32_t frequency, TransferType ttype, uint32_t data_size, uint32_t transfer_count, bool stop_each)
{
    // Remap pins for test
    tester.pin_map_reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(addr));

    int num_writes = 0;
    int num_reads = 0;
    int num_acks = 0;
    int num_nacks = 0;
    int num_starts = 0;
    int num_stops = 0;
    uint32_t write_checksum = 0;
    uint32_t read_checksum = 0;
    int num_dev_addr_matches = 0;

    // Initialize mbed I2C pins
    i2c_t i2c = {};
    i2c_init(&i2c, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&i2c, frequency);

    uint8_t *data_out = new uint8_t[data_size];
    uint8_t *data_in = new uint8_t[data_size];
    srand(ticker_read(get_us_ticker_data()));
    for (uint32_t i = 0; i < data_size; i++) {
        data_out[i] = (uint8_t)(rand() & 0xFF);
        data_in[i] = 0;
    }

    for (uint32_t i = 0; i < transfer_count; i++) {
        async_status transfer_status = {};
        if (ttype == WRITE_READ) {
            i2c_transfer_async(&i2c, data_out, data_size, data_in, data_size, MAKE_7BIT_SLAVE_ADDRESS(addr), stop_each, async_callback, &transfer_status);
            while (!transfer_status.done);
            num_dev_addr_matches += 2;
            num_starts += 2;
            num_acks += 2; // address match
            num_acks += transfer_status.event.sent_bytes;
            num_acks += transfer_status.event.received_bytes - 1;
            num_nacks += 1; // end of transfer
        } else if (ttype == WRITE) {
            i2c_transfer_async(&i2c, data_out, data_size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(addr), stop_each, async_callback, &transfer_status);
            while (!transfer_status.done);
            num_dev_addr_matches += 1;
            num_starts += 1;
            num_acks += 1; // address match
            num_acks += transfer_status.event.sent_bytes;
        } else if (ttype == READ) {
            i2c_transfer_async(&i2c, NULL, 0, data_in, data_size, MAKE_7BIT_SLAVE_ADDRESS(addr), stop_each, async_callback, &transfer_status);
            while (!transfer_status.done);
            num_dev_addr_matches += 1;
            num_starts += 1;
            num_acks += 1; // address match
            num_acks += data_size - 1;
            num_nacks += 1; // end of transfer
        }
        TEST_ASSERT_EQUAL(false, transfer_status.event.error);
        if (ttype == WRITE_READ || ttype == WRITE) {
            TEST_ASSERT_EQUAL(data_size, transfer_status.event.sent_bytes);
            num_writes += transfer_status.event.sent_bytes;
        }
        if (ttype == WRITE_READ || ttype == READ) {
            TEST_ASSERT_EQUAL(data_size, transfer_status.event.received_bytes);
            num_reads += transfer_status.event.received_bytes;
        }

        if (stop_each) {
            num_stops += 1;
        }
        for (uint32_t i = 0; i < data_size; i++) {
            write_checksum += data_out[i];
            read_checksum += data_in[i];
        }
    }
    if (!stop_each) {
        num_stops += 1;
        i2c_stop(&i2c);
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    if (ttype == WRITE_READ || ttype == WRITE) {
        TEST_ASSERT_EQUAL(data_size * transfer_count, num_writes);
        TEST_ASSERT_EQUAL(write_checksum, tester.get_receive_checksum());
        TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    }
    if (ttype == WRITE_READ || ttype == READ) {
        TEST_ASSERT_EQUAL(data_size * transfer_count, num_reads);
        TEST_ASSERT_EQUAL(read_checksum, tester.get_send_checksum());
        TEST_ASSERT_EQUAL(num_reads, tester.num_reads());
    }

    TEST_ASSERT_EQUAL(num_writes + num_reads + ((ttype == WRITE_READ) ? 2 : 1) * transfer_count, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    if (ttype == WRITE_READ || ttype == WRITE) {
        if (data_size > 3) {
            TEST_ASSERT_EQUAL(data_out[data_size - 4], tester.get_prev_to_slave_4());
        }
        if (data_size > 2) {
            TEST_ASSERT_EQUAL(data_out[data_size - 3], tester.get_prev_to_slave_3());
        }
        if (data_size > 1) {
            TEST_ASSERT_EQUAL(data_out[data_size - 2], tester.get_prev_to_slave_2());
        }
        TEST_ASSERT_EQUAL(data_out[data_size - 1], tester.get_prev_to_slave_1());
    }
    if (ttype == WRITE_READ || ttype == READ) {
        TEST_ASSERT_EQUAL(((data_size * transfer_count + 1) & 0xFF), tester.get_next_from_slave());
    }

    i2c_free(&i2c);
    delete [] data_out;
    delete [] data_in;

    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
}

void fpga_i2c_test_async_abort(PinName sda, PinName scl, uint32_t frequency)
{
    // Remap pins for test
    tester.pin_map_reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    int num_writes = 0;
    int num_reads = 0;
    int num_acks = 0;
    int num_nacks = 0;
    int num_starts = 0;
    int num_stops = 0;
    uint32_t write_checksum = 0;
    uint32_t read_checksum = 0;
    int num_dev_addr_matches = 0;

    // Initialize mbed I2C pins
    i2c_t i2c = {};
    i2c_init(&i2c, sda, scl, DEVICE_AS_MASTER);
    i2c_frequency(&i2c, frequency);

    uint8_t data_out[100];
    uint8_t data_in[100];
    const uint32_t data_size = sizeof(data_out);
    srand(ticker_read(get_us_ticker_data()));
    for (uint32_t i = 0; i < data_size; i++) {
        data_out[i] = (uint8_t)(rand() & 0xFF);
        write_checksum += data_out[i];
        data_in[i] = 0;
    }

    // test async transfer
    async_status transfer_status = {};
    i2c_transfer_async(&i2c, data_out, data_size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);
    TEST_ASSERT_EQUAL(data_size, transfer_status.event.sent_bytes);

    num_dev_addr_matches += 1;
    num_starts += 1;
    num_stops += 1;
    num_acks += 1; // address match
    num_acks += data_size;
    num_writes += transfer_status.event.sent_bytes;
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(write_checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());

    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    // test async transfer abort
    num_dev_addr_matches = num_starts = num_stops = num_acks = num_nacks = num_writes = 0;
    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, data_out, data_size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, async_callback, &transfer_status);
    wait_us(100); // wait for transfer start
    i2c_abort_async(&i2c);
    i2c_stop(&i2c);
    wait(1);
    TEST_ASSERT_EQUAL(false, transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);

    num_dev_addr_matches += 1;
    num_starts += 1;
    num_stops += 1;
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());

    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    // test async transfer
    num_dev_addr_matches = num_starts = num_stops = num_acks = num_nacks = num_writes = 0;
    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, data_out, data_size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);
    TEST_ASSERT_EQUAL(data_size, transfer_status.event.sent_bytes);

    num_dev_addr_matches += 1;
    num_starts += 1;
    num_stops += 1;
    num_acks += 1; // address match
    num_acks += data_size;
    num_writes += transfer_status.event.sent_bytes;
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(write_checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());

    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    // test async receive
    num_dev_addr_matches = num_starts = num_stops = num_acks = num_nacks = num_reads = 0;
    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, NULL, 0, data_in, data_size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);
    TEST_ASSERT_EQUAL(data_size, transfer_status.event.received_bytes);

    num_dev_addr_matches += 1;
    num_starts += 1;
    num_stops += 1;
    num_acks += 1; // address match
    num_acks += data_size - 1;
    num_nacks += 1;
    num_reads += transfer_status.event.received_bytes;
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    read_checksum = 0;
    for (uint32_t i = 0; i < data_size; i++) {
        read_checksum += data_in[i];
    }
    TEST_ASSERT_EQUAL(read_checksum, tester.get_send_checksum());

    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    // test async receive abort
    num_dev_addr_matches = num_starts = num_stops = num_acks = num_nacks = num_reads = 0;
    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, NULL, 0, data_in, data_size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, async_callback, &transfer_status);
    wait_us(100); // wait for transfer start
    i2c_abort_async(&i2c);
    i2c_stop(&i2c);
    wait(1);
    TEST_ASSERT_EQUAL(false, transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);

    num_dev_addr_matches += 1;
    num_starts += 1;
    num_stops += 1;
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());

    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    // test async receive
    num_dev_addr_matches = num_starts = num_stops = num_acks = num_nacks = num_reads = 0;
    memset(&transfer_status, 0, sizeof(async_status));
    i2c_transfer_async(&i2c, NULL, 0, data_in, data_size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, async_callback, &transfer_status);
    while (!transfer_status.done);
    TEST_ASSERT_EQUAL(false, transfer_status.event.error);
    TEST_ASSERT_EQUAL(data_size, transfer_status.event.received_bytes);

    num_dev_addr_matches += 1;
    num_starts += 1;
    num_stops += 1;
    num_acks += 1; // address match
    num_acks += data_size - 1;
    num_nacks += 1;
    num_reads += transfer_status.event.received_bytes;
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());

    read_checksum = 0;
    for (uint32_t i = 0; i < data_size; i++) {
        read_checksum += data_in[i];
    }
    TEST_ASSERT_EQUAL(read_checksum, tester.get_send_checksum());

    i2c_free(&i2c);
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
}
#endif // DEVICE_I2C_ASYNCH


template<uint32_t frequency>
void test_i2c_blocking_transmission_timeout(PinName sda, PinName scl)
{
    fpga_i2c_test_blocking_transmission_timeout(sda, scl, frequency);
}

template<uint32_t frequency>
void test_i2c_transmission_no_slave(PinName sda, PinName scl)
{
    fpga_i2c_test_transmission_no_slave(sda, scl, frequency);
}

template<uint16_t addr, uint32_t frequency, uint32_t data_size, uint32_t transfer_count, bool stop_each>
void test_i2c_blocking_write(PinName sda, PinName scl)
{
    fpga_i2c_test_blocking_write_read(sda, scl, addr, frequency, WRITE, data_size, transfer_count, stop_each);
}

template<uint16_t addr, uint32_t frequency, uint32_t data_size, uint32_t transfer_count, bool stop_each>
void test_i2c_blocking_read(PinName sda, PinName scl)
{
    fpga_i2c_test_blocking_write_read(sda, scl, addr, frequency, READ, data_size, transfer_count, stop_each);
}

template<uint16_t addr, uint32_t frequency, uint32_t data_size, uint32_t transfer_count, bool stop_each>
void test_i2c_blocking_write_read(PinName sda, PinName scl)
{
    fpga_i2c_test_blocking_write_read(sda, scl, addr, frequency, WRITE_READ, data_size, transfer_count, stop_each);
}
#if DEVICE_I2C_ASYNCH
template<uint32_t frequency>
void test_i2c_async_abort(PinName sda, PinName scl)
{
    fpga_i2c_test_async_abort(sda, scl, frequency);
}
template<uint16_t addr, uint32_t frequency, uint32_t data_size, uint32_t transfer_count, bool stop_each>
void test_i2c_async_write(PinName sda, PinName scl)
{
    fpga_i2c_test_async_write_read(sda, scl, addr, frequency, WRITE, data_size, transfer_count, stop_each);
}

template<uint16_t addr, uint32_t frequency, uint32_t data_size, uint32_t transfer_count, bool stop_each>
void test_i2c_async_read(PinName sda, PinName scl)
{
    fpga_i2c_test_async_write_read(sda, scl, addr, frequency, READ, data_size, transfer_count, stop_each);
}

template<uint16_t addr, uint32_t frequency, uint32_t data_size, uint32_t transfer_count, bool stop_each>
void test_i2c_async_write_read(PinName sda, PinName scl)
{
    fpga_i2c_test_async_write_read(sda, scl, addr, frequency, WRITE_READ, data_size, transfer_count, stop_each);
}
#endif // DEVICE_I2C_ASYNCH


Case cases[] = {
    Case("i2c - test get_capabilities",                                  fpga_i2c_test_get_capabilities),
    Case("i2c - test init/free",                                         all_ports<I2CPort, DefaultFormFactor, fpga_i2c_test_init_free>),
    Case("i2c - test frequency setting",                                 all_ports<I2CPort, DefaultFormFactor, fpga_i2c_test_frequency>),

    Case("i2c - test async_abort",                                       all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_abort<FAST_MODE_FREQ_400kHz> >),
    Case("i2c - test transmission errors when no slave present",         all_peripherals<I2CPort, DefaultFormFactor, test_i2c_transmission_no_slave<FAST_MODE_FREQ_400kHz> >),
    Case("i2c - test blocking transmission timeout",                     all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_transmission_timeout<FAST_MODE_FREQ_400kHz> >),

    Case("i2c - test blocking write 1x100 400kHz stop each",             all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 1,   100, STOP_EACH> >),
    Case("i2c - test blocking write 1x100 400kHz stop at the end",       all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write<I2C_7BIT_ADDRESS_MAX, FAST_MODE_FREQ_400kHz, 1,   100, STOP_AT_END> >),
    Case("i2c - test blocking write 100x2 400kHz stop each",             all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write<I2C_7BIT_ADDRESS_MAX, FAST_MODE_FREQ_400kHz, 100, 5,   STOP_EACH> >),
    Case("i2c - test blocking write 600x1 400kHz stop at the end",       all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 600, 1,   STOP_AT_END> >),

    Case("i2c - test blocking read 1x100 400kHz stop each",              all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 1,   100, STOP_EACH> >),
    Case("i2c - test blocking read 1x100 400kHz stop at the end",        all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_read<I2C_7BIT_ADDRESS_MAX, FAST_MODE_FREQ_400kHz, 1,   100, STOP_AT_END> >),
    Case("i2c - test blocking read 100x2 400kHz stop each",              all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 100, 5,   STOP_EACH> >),
    Case("i2c - test blocking read 600x1 400kHz stop at the end",        all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 600, 1,   STOP_AT_END> >),

    Case("i2c - test blocking write/read 1x100 100kHz stop each",        all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write_read<I2C_7BIT_ADDRESS_MIN, STANDARD_MODE_FREQ_100kHz, 1,   100, STOP_EACH> >),
    Case("i2c - test blocking write/read 1x100 100kHz stop at the end",  all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write_read<I2C_7BIT_ADDRESS_MAX, STANDARD_MODE_FREQ_100kHz, 1,   100, STOP_AT_END> >),
    Case("i2c - test blocking write/read 100x2 100kHz stop each",        all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write_read<I2C_7BIT_ADDRESS_MIN, STANDARD_MODE_FREQ_100kHz, 100, 5,   STOP_EACH> >),
    Case("i2c - test blocking write/read 600x1 400kHz stop at the end",  all_peripherals<I2CPort, DefaultFormFactor, test_i2c_blocking_write_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 600, 1,   STOP_AT_END> >),
#if DEVICE_I2C_ASYNCH
    Case("i2c - test async write 1x100 400kHz stop each",                all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 1,   100, STOP_EACH> >),
    Case("i2c - test async write 1x100 400kHz stop at the end",          all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write<I2C_7BIT_ADDRESS_MAX, FAST_MODE_FREQ_400kHz, 1,   100, STOP_AT_END> >),
    Case("i2c - test async write 100x2 400kHz stop each",                all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 100, 5,   STOP_EACH> >),
    Case("i2c - test async write 600x1 400kHz stop at the end",          all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 600, 1,   STOP_AT_END> >),

    Case("i2c - test async read 1x100 400kHz stop each",                 all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 1,   100, STOP_EACH> >),
    Case("i2c - test async read 1x100 400kHz stop at the end",           all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_read<I2C_7BIT_ADDRESS_MAX, FAST_MODE_FREQ_400kHz, 1,   100, STOP_AT_END> >),
    Case("i2c - test async read 100x2 400kHz stop each",                 all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 100, 5,   STOP_EACH> >),
    Case("i2c - test async read 600x1 400kHz stop at the end",           all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 600, 1,   STOP_AT_END> >),

    Case("i2c - test async write/read 1x100 100kHz stop each",           all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write_read<I2C_7BIT_ADDRESS_MIN, STANDARD_MODE_FREQ_100kHz, 1,   100, STOP_EACH> >),
    Case("i2c - test async write/read 1x100 100kHz stop at the end",     all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write_read<I2C_7BIT_ADDRESS_MAX, STANDARD_MODE_FREQ_100kHz, 1,   100, STOP_AT_END> >),
    Case("i2c - test async write/read 100x2 100kHz stop each",           all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write_read<I2C_7BIT_ADDRESS_MIN, STANDARD_MODE_FREQ_100kHz, 100, 5,   STOP_EACH> >),
    Case("i2c - test async write/read 600x1 400kHz stop at the end",     all_peripherals<I2CPort, DefaultFormFactor, test_i2c_async_write_read<I2C_7BIT_ADDRESS_MIN, FAST_MODE_FREQ_400kHz, 600, 1,   STOP_AT_END> >)
#endif // DEVICE_I2C_ASYNCH
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
#endif /* !DEVICE_I2C */
