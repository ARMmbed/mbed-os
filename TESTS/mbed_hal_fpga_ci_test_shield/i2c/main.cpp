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
#include "hal/static_pinmap.h"
#include "test_utils.h"
#include "I2CTester.h"
#include "i2c_fpga_test.h"

using namespace utest::v1;

#define NACK    0
#define ACK     1
#define TIMEOUT 2
#define I2C_DEV_ADDR 0x98//default i2c slave address on FPGA is 0x98 until modified
const int TRANSFER_COUNT = 300;

I2CTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

void fpga_test_i2c_init_free(PinName sda, PinName scl)
{
    i2c_t obj = {};
    memset(&obj, 0, sizeof(obj));
    i2c_init(&obj, sda, scl);
    i2c_frequency(&obj, 100000);

    i2c_free(&obj);
}

template<bool init_direct>
void fpga_i2c_test_write(PinName sda, PinName scl)
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
    if (init_direct) {
#if STATIC_PINMAP_READY
        const i2c_pinmap_t pinmap = get_i2c_pinmap(sda, scl);
        i2c_init_direct(&i2c, &pinmap);
#else
        //skip this test case if static pinmap is not supported
        return;
#endif
    } else {
        i2c_init(&i2c, sda, scl);
    }
    i2c_frequency(&i2c, 100000);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);

    // Data out and in buffers and initialization
    uint8_t data_out[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_out[i] = i & 0xFF;
    }
    uint8_t data_in[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_in[i] = 0;
    }

    int num_writes;
    int num_reads;
    int num_acks;
    int num_nacks;
    int num_starts;
    int num_stops;
    uint32_t checksum;
    int num_dev_addr_matches;
    int ack_nack;//0 if NACK was received, 1 if ACK was received, 2 for timeout

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);

    // Write data for I2C complete transaction
    // Will write 0-(TRANSFER_COUNT-1) to FPGA, checksum must match checksum calculated in parallel on FPGA
    num_dev_addr_matches = 0;
    num_writes = 0;
    num_reads = 0;
    num_starts = 0;
    num_stops = 0;
    num_acks = 0;
    num_nacks = 0;
    checksum = 0;

    num_writes = i2c_write(&i2c, I2C_DEV_ADDR, (char *)data_out, TRANSFER_COUNT, true); //transaction ends with a stop condition
    num_acks = num_writes + 1;
    num_starts += 1;
    num_stops += 1;
    num_dev_addr_matches += 1;

    for (int i = 0; i < TRANSFER_COUNT; i++) {
        checksum += data_out[i];
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(TRANSFER_COUNT, num_writes);
    TEST_ASSERT_EQUAL(num_writes + 1, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 4], tester.get_prev_to_slave_4());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 3], tester.get_prev_to_slave_3());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 2], tester.get_prev_to_slave_2());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 1], tester.get_prev_to_slave_1());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
    i2c_free(&i2c);
}

template<bool init_direct>
void fpga_i2c_test_read(PinName sda, PinName scl)
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
    if (init_direct) {
        const i2c_pinmap_t pinmap = get_i2c_pinmap(sda, scl);
        i2c_init_direct(&i2c, &pinmap);
    } else {
        i2c_init(&i2c, sda, scl);
    }
    i2c_frequency(&i2c, 100000);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);

    // Data out and in buffers and initialization
    uint8_t data_out[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_out[i] = i & 0xFF;
    }
    uint8_t data_in[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_in[i] = 0;
    }

    int num_writes;
    int num_reads;
    int num_acks;
    int num_nacks;
    int num_starts;
    int num_stops;
    uint32_t checksum;
    int num_dev_addr_matches;
    int ack_nack;//0 if NACK was received, 1 if ACK was received, 2 for timeout

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);

    // Read data for I2C complete transaction
    // Will read <TRANSFER_COUNT> bytes, checksum must match checksum calculated in parallel on FPGA
    num_dev_addr_matches = 0;
    num_writes = 0;
    num_reads = 0;
    num_starts = 0;
    num_stops = 0;
    num_acks = 0;
    num_nacks = 0;
    checksum = 0;

    num_reads = i2c_read(&i2c, (I2C_DEV_ADDR | 1), (char *)data_in, TRANSFER_COUNT, true); //transaction ends with a stop condition
    num_starts += 1;
    num_stops += 1;
    num_acks += 1;
    num_acks += TRANSFER_COUNT - 1;
    num_nacks += 1;
    num_dev_addr_matches += 1;

    for (int i = 0; i < TRANSFER_COUNT; i++) {
        checksum += data_in[i];
    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(TRANSFER_COUNT, num_reads);
    TEST_ASSERT_EQUAL(num_reads + 1, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_send_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL(((TRANSFER_COUNT + 1) & 0xFF), tester.get_next_from_slave());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
    i2c_free(&i2c);
}

void fpga_i2c_test_byte_write(PinName sda, PinName scl)
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
    i2c_frequency(&i2c, 100000);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);

    // Data out and in buffers and initialization
    uint8_t data_out[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_out[i] = i & 0xFF;
    }
    uint8_t data_in[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_in[i] = 0;
    }

    int num_writes;
    int num_reads;
    int num_acks;
    int num_nacks;
    int num_starts;
    int num_stops;
    uint32_t checksum;
    int num_dev_addr_matches;
    int ack_nack;//0 if NACK was received, 1 if ACK was received, 2 for timeout

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);

    // Write data for I2C single byte transfers
    // Will write 0-(TRANSFER_COUNT-1) to FPGA, checksum must match checksum calculated in parallel on FPGA
    num_dev_addr_matches = 0;
    num_writes = 0;
    num_reads = 0;
    num_starts = 0;
    num_stops = 0;
    num_acks = 0;
    num_nacks = 0;
    checksum = 0;

    i2c_start(&i2c);//start condition
    num_starts += 1;
    i2c_byte_write(&i2c, I2C_DEV_ADDR);//send device address
    num_dev_addr_matches += 1;
    num_acks += 1;
    for (int i = 0; i < TRANSFER_COUNT; i++) {
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

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(TRANSFER_COUNT, num_writes);
    TEST_ASSERT_EQUAL(num_writes + 1, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 4], tester.get_prev_to_slave_4());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 3], tester.get_prev_to_slave_3());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 2], tester.get_prev_to_slave_2());
    TEST_ASSERT_EQUAL(data_out[TRANSFER_COUNT - 1], tester.get_prev_to_slave_1());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
    i2c_free(&i2c);
}

void fpga_i2c_test_byte_read(PinName sda, PinName scl)
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
    i2c_frequency(&i2c, 100000);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(I2CTester::PeripheralI2C);

    // Data out and in buffers and initialization
    uint8_t data_out[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_out[i] = i & 0xFF;
    }
    uint8_t data_in[TRANSFER_COUNT];
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_in[i] = 0;
    }

    int num_writes;
    int num_reads;
    int num_acks;
    int num_nacks;
    int num_starts;
    int num_stops;
    uint32_t checksum;
    int num_dev_addr_matches;
    int ack_nack;//0 if NACK was received, 1 if ACK was received, 2 for timeout

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);
    tester.set_next_from_slave(0);
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        data_in[i] = 0;
    }

    // Read data for I2C single byte transfers
    // Will read <TRANSFER_COUNT> bytes, checksum must match checksum calculated in parallel on FPGA
    num_dev_addr_matches = 0;
    num_writes = 0;
    num_reads = 0;
    num_starts = 0;
    num_stops = 0;
    num_acks = 0;
    num_nacks = 0;
    checksum = 0;

    i2c_start(&i2c);//start condition
    num_starts += 1;
    i2c_byte_write(&i2c, (I2C_DEV_ADDR | 1));//send device address for reading
    num_dev_addr_matches += 1;
    num_acks += 1;
    for (int i = 0; i < TRANSFER_COUNT; i++) {
        if (num_reads == (TRANSFER_COUNT - 1)) {
            data_in[i] = i2c_byte_read(&i2c, 1);//send NACK
            checksum += data_in[i];
            num_reads += 1;
            num_nacks += 1;
        } else {
            data_in[i] = i2c_byte_read(&i2c, 0);//send ACK
            checksum += data_in[i];
            num_reads += 1;
            num_acks += 1;
        }
    }

    i2c_stop(&i2c);
    num_stops += 1;

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(TRANSFER_COUNT, num_reads);
    TEST_ASSERT_EQUAL(num_reads + 1, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_send_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL(((TRANSFER_COUNT) & 0xFF), tester.get_next_from_slave());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    tester.reset();
    tester.pin_set_pull(sda, MbedTester::PullNone);
    tester.pin_set_pull(scl, MbedTester::PullNone);
    i2c_free(&i2c);
}

Case cases[] = {
    Case("i2c - init/free test all pins", all_ports<I2CPort, DefaultFormFactor, fpga_test_i2c_init_free>),
    Case("i2c - test write i2c API", all_peripherals<I2CPort, DefaultFormFactor, fpga_i2c_test_write<false>>),
    Case("i2c (direct init) - test write i2c API", all_peripherals<I2CPort, DefaultFormFactor, fpga_i2c_test_write<true>>),
    Case("i2c - test read i2c API", all_peripherals<I2CPort, DefaultFormFactor, fpga_i2c_test_read<false>>),
    Case("i2c (direct init) - test read i2c API", all_peripherals<I2CPort, DefaultFormFactor, fpga_i2c_test_read<true>>),
    Case("i2c - test single byte write i2c API", all_peripherals<I2CPort, DefaultFormFactor, fpga_i2c_test_byte_write>),
    Case("i2c - test single byte read i2c API", all_peripherals<I2CPort, DefaultFormFactor, fpga_i2c_test_byte_read>)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !DEVICE_I2C */
