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

#include "mbed.h"
#include "i2c_api.h"
#include "pinmap.h"
#include "test_utils.h"
#include "I2CTester.h"


using namespace utest::v1;

#define NACK    0
#define ACK     1
#define TIMEOUT 2

#define STANDARD_MODE_FREQ_100kHz   100000
#define FAST_MODE_FREQ_400kHz       400000

// 7bit addressing, allowed values from 0x8 to 0x77
#define I2C_7BIT_ADDRESS_MIN     (0x08)
#define I2C_7BIT_ADDRESS_MAX     (0x77)
#define I2C_7BIT_ADDRESS_DEFAULT (0x4C) // default i2c slave address on FPGA is 0x98 (0x4C << 1) until modified

#define I2C_ADDRESS_MASK_7BIT   (0x7F)
#define MAKE_7BIT_READ_ADDRESS(addr)    (((I2C_ADDRESS_MASK_7BIT & addr) << 1) | 1)
#define MAKE_7BIT_WRITE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)
#define MAKE_7BIT_SLAVE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)


I2CTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

void test_i2c_init_free(PinName sda, PinName scl, int frequency)
{
    i2c_t obj;
    memset(&obj, 0, sizeof(obj));
    i2c_init(&obj, sda, scl);
    i2c_frequency(&obj, frequency);

    /* Free up I2C pins
     *
     * The most suitable place to free up I2C pins is in i2c_free(). Due to
     * i2c_free() not available in I2C HAL, we free up I2C pins manually by
     * configuring them back to GPIO.
     *
     * Without free-up of I2C pins, SDA/SCL pins of the same I2C peripheral may
     * share by multiple ports due to 'all ports' tests here, and the following
     * I2C tests would be subject to interference by shared ports.
     */
    gpio_set(sda);
    gpio_set(scl);
}

void i2c_test_write(PinName sda, PinName scl, uint32_t frequency, uint16_t address, uint32_t size, uint32_t count)
{
    // Remap pins for test
    tester.reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Initialize mbed I2C pins
    i2c_t i2c;
    memset(&i2c, 0, sizeof(i2c));
    i2c_init(&i2c, sda, scl);
    i2c_frequency(&i2c, frequency);

    // Data out and in buffers and initialization
    uint8_t *data_out = new uint8_t[size];
    for (int i = 0; i < size; i++) {
        data_out[i] = i & 0xFF;
    }

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(address));

    // Write data for I2C complete transaction
    // Will write <size * count> bytes to FPGA, checksum must match checksum calculated in parallel on FPGA
    int num_dev_addr_matches = 0;
    int num_writes_total = 0;
    int num_reads_total = 0;
    int num_starts = 0;
    int num_stops = 0;
    int num_acks = 0;
    int num_nacks = 0;
    uint32_t checksum = 0;

    for (int i = 0; i < count; i++) {
        int num_writes = i2c_write(&i2c, (int)MAKE_7BIT_WRITE_ADDRESS(address), (char *)data_out, size, (i == (count - 1)) ? true : false);
        num_writes_total += num_writes;
        num_acks += num_writes;
        if (i == 0) {
            num_acks += 1;
            num_dev_addr_matches += 1;
            num_starts += 1;
        }
        if (i == count - 1) {
            num_stops += 1;
        }

        for (int j = 0; j < size; j++) {
            checksum += data_out[j];
        }
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(size * count, num_writes_total);
    TEST_ASSERT_EQUAL(num_writes_total + num_dev_addr_matches, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    if (size > 3) {
        TEST_ASSERT_EQUAL(data_out[size - 4], tester.get_prev_to_slave_4());
    }
    if (size > 2) {
        TEST_ASSERT_EQUAL(data_out[size - 3], tester.get_prev_to_slave_3());
    }
    if (size > 1) {
        TEST_ASSERT_EQUAL(data_out[size - 2], tester.get_prev_to_slave_2());
    }
    TEST_ASSERT_EQUAL(data_out[size - 1], tester.get_prev_to_slave_1());
    TEST_ASSERT_EQUAL(num_writes_total, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads_total, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);

    delete [] data_out;
}

void i2c_test_read(PinName sda, PinName scl, uint32_t frequency, uint16_t address, uint32_t size, uint32_t count)
{
    // Remap pins for test
    tester.reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Initialize mbed I2C pins
    i2c_t i2c;
    memset(&i2c, 0, sizeof(i2c));
    i2c_init(&i2c, sda, scl);
    i2c_frequency(&i2c, frequency);

    // Data out and in buffers and initialization
    uint8_t *data_in = new uint8_t[size];

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(address));
    tester.set_next_from_slave(0);

    // Read data for I2C complete transaction
    // Will read <size * count> bytes, checksum must match checksum calculated in parallel on FPGA
    int num_dev_addr_matches = 0;
    int num_writes_total = 0;
    int num_reads_total = 0;
    int num_starts = 0;
    int num_stops = 0;
    int num_acks = 0;
    int num_nacks = 0;
    uint32_t checksum = 0;

    for (uint32_t i = 0; i < count; i++) {
        int num_reads = i2c_read(&i2c, (int)MAKE_7BIT_READ_ADDRESS(address), (char *)data_in, size, (i == (count - 1)) ? true : false); //transaction ends with a stop condition
        num_dev_addr_matches += 1;
        num_starts += 1;
        num_reads_total += num_reads;
        num_acks += num_reads;
        num_nacks += 1;
        if (i == count - 1) {
            num_stops += 1;
        }
        for (uint32_t j = 0; j < size; j++) {
            checksum += data_in[j];
        }
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(size * count, num_reads_total);
    TEST_ASSERT_EQUAL(num_reads_total + num_dev_addr_matches, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_send_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL(((size * count) & 0xFF), tester.get_next_from_slave());
    TEST_ASSERT_EQUAL(num_writes_total, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads_total, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);

    delete [] data_in;
}

void i2c_test_byte_write(PinName sda, PinName scl, uint32_t frequency, uint16_t address, uint32_t size, uint32_t count)
{
    // Remap pins for test
    tester.reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Initialize mbed I2C pins
    i2c_t i2c;
    memset(&i2c, 0, sizeof(i2c));
    i2c_init(&i2c, sda, scl);
    i2c_frequency(&i2c, frequency);

    // Data out and in buffers and initialization
    uint8_t *data_out = new uint8_t[size];
    for (int i = 0; i < size; i++) {
        data_out[i] = i & 0xFF;
    }

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(address));

    // Write data for I2C single byte transfers
    // Will write <size * count> bytes to FPGA, checksum must match checksum calculated in parallel on FPGA
    int num_dev_addr_matches = 0;
    int num_writes = 0;
    int num_reads = 0;
    int num_starts = 0;
    int num_stops = 0;
    int num_acks = 0;
    int num_nacks = 0;
    uint32_t checksum = 0;
    int ack_nack;//0 if NACK was received, 1 if ACK was received, 2 for timeout

    for (uint32_t j = 0; j < count; j++) {
        i2c_start(&i2c);//start condition
        num_starts += 1;
        i2c_byte_write(&i2c, MAKE_7BIT_WRITE_ADDRESS(address));//send device address
        num_dev_addr_matches += 1;
        num_acks += 1;
        for (uint32_t i = 0; i < size; i++) {
            ack_nack = i2c_byte_write(&i2c, data_out[i]);//send data
            if (ack_nack == ACK) {
                num_acks += 1;
            } else if (ack_nack == NACK) {
                num_nacks += 1;
            } else {
                printf("Timeout error\n\r");
            }
            checksum += data_out[i];
            num_writes += 1;
        }
        i2c_stop(&i2c);
        num_stops += 1;
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(size * count, num_writes);
    TEST_ASSERT_EQUAL(num_writes + num_dev_addr_matches, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    if (size > 3) {
        TEST_ASSERT_EQUAL(data_out[size - 4], tester.get_prev_to_slave_4());
    }
    if (size > 2) {
        TEST_ASSERT_EQUAL(data_out[size - 3], tester.get_prev_to_slave_3());
    }
    if (size > 1) {
        TEST_ASSERT_EQUAL(data_out[size - 2], tester.get_prev_to_slave_2());
    }
    TEST_ASSERT_EQUAL(data_out[size - 1], tester.get_prev_to_slave_1());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);

    delete [] data_out;
}

void i2c_test_byte_read(PinName sda, PinName scl, uint32_t frequency, uint16_t address, uint32_t size, uint32_t count)
{
    // Remap pins for test
    tester.reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

    tester.pin_set_pull(sda, MbedTester::PullUp);
    tester.pin_set_pull(scl, MbedTester::PullUp);

    // Data out and in buffers and initialization
    uint8_t *data_in = new uint8_t[size];

    int num_writes = 0;
    int num_reads = 0;
    int num_acks = 0;
    int num_nacks = 0;
    int num_starts = 0;
    int num_stops = 0;
    uint32_t checksum = 0;
    int num_dev_addr_matches = 0;

    // Initialize mbed I2C pins
    i2c_t i2c;
    memset(&i2c, 0, sizeof(i2c));
    i2c_init(&i2c, sda, scl);
    i2c_frequency(&i2c, frequency);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);
    tester.set_device_address(MAKE_7BIT_SLAVE_ADDRESS(address));
    tester.set_next_from_slave(0);

    for (uint32_t i = 0; i < size; i++) {
        data_in[i] = 0;
    }

    // Read data for I2C single byte transfers
    // Will read <count * size> bytes, checksum must match checksum calculated in parallel on FPGA
    for (uint32_t j = 0; j < count; j++) {
        i2c_start(&i2c);//start condition
        num_starts += 1;
        i2c_byte_write(&i2c, (int)MAKE_7BIT_READ_ADDRESS(address));//send device address for reading
        num_dev_addr_matches += 1;
        num_acks += 1;
        for (uint32_t i = 0; i < size - 1; i++) {
            data_in[i] = i2c_byte_read(&i2c, 0);//send ACK
            checksum += data_in[i];
            num_reads += 1;
            num_acks += 1;
        }
        data_in[size - 1] = i2c_byte_read(&i2c, 1);//send NACK
        checksum += data_in[size - 1];
        num_reads += 1;
        num_nacks += 1;

        i2c_stop(&i2c);
        num_stops += 1;
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(count * size, num_reads);
    TEST_ASSERT_EQUAL(num_reads + count, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_send_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL(((size * count) & 0xFF), tester.get_next_from_slave());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);

    delete [] data_in;
}

template <int frequency>
void test_i2c_init_free(PinName sda, PinName scl)
{
    test_i2c_init_free(sda, scl, frequency);
}

template <uint32_t size, uint32_t count, uint32_t frequency, uint16_t address>
void i2c_test_write(PinName sda, PinName scl)
{
    i2c_test_write(sda, scl, frequency, address, size, count);
}

template <uint32_t size, uint32_t count, uint32_t frequency, uint16_t address>
void i2c_test_read(PinName sda, PinName scl)
{
    i2c_test_read(sda, scl, frequency, address, size, count);
}

template <uint32_t size, uint32_t count, uint32_t frequency, uint16_t address>
void i2c_test_byte_write(PinName sda, PinName scl)
{
    i2c_test_byte_write(sda, scl, frequency, address, size, count);
}

template <uint32_t size, uint32_t count, uint32_t frequency, uint16_t address>
void i2c_test_byte_read(PinName sda, PinName scl)
{
    i2c_test_byte_read(sda, scl, frequency, address, size, count);
}

Case cases[] = {
    Case("i2c - test init/free on all pins at a frequency of 100kHz", all_ports<I2CPort, DefaultFormFactor, test_i2c_init_free<STANDARD_MODE_FREQ_100kHz> >),
    Case("i2c - test init/free on all pins at a frequency of 400kHz", all_ports<I2CPort, DefaultFormFactor, test_i2c_init_free<FAST_MODE_FREQ_400kHz> >),
    Case("i2c - test write i2c API at a frequency of 100kHz", all_peripherals<I2CPort, DefaultFormFactor, i2c_test_write<300, 10, STANDARD_MODE_FREQ_100kHz, I2C_7BIT_ADDRESS_DEFAULT> >),
    Case("i2c - test read i2c API at a frequency of 400kHz", all_peripherals<I2CPort, DefaultFormFactor, i2c_test_read<300, 10, FAST_MODE_FREQ_400kHz, I2C_7BIT_ADDRESS_DEFAULT> >),
    Case("i2c - test single byte write i2c API at a frequency of 400kHz", all_peripherals<I2CPort, DefaultFormFactor, i2c_test_byte_write<300, 10, FAST_MODE_FREQ_400kHz, I2C_7BIT_ADDRESS_MIN> >),
    Case("i2c - test single byte read i2c API at a frequency of 100kHz", all_peripherals<I2CPort, DefaultFormFactor, i2c_test_byte_read<300, 10, STANDARD_MODE_FREQ_100kHz, I2C_7BIT_ADDRESS_MAX> >)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !DEVICE_I2C */
