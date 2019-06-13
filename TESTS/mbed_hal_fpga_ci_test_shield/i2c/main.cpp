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
#elif !defined(FULL_TEST_SHIELD)
#error [NOT_SUPPORTED] The I2C test does not run on prototype hardware
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
#define I2C_DEV_ADDR 0x98//default i2c slave address on FPGA is 0x98 until modified
const int TRANSFER_COUNT = 300;

I2CTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

void test_i2c_init_free(PinName sda, PinName scl)
{
    i2c_t obj = {};
    memset(&obj, 0, sizeof(obj));
    i2c_init(&obj, sda, scl);
    i2c_frequency(&obj, 100000);
}

void i2c_test_all(PinName sda, PinName scl)
{
    // Remap pins for test
    tester.reset();
    tester.pin_map_set(sda, MbedTester::LogicalPinI2CSda);
    tester.pin_map_set(scl, MbedTester::LogicalPinI2CScl);

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

    printf("I2C complete write transaction test on sda=%s (%i), scl=%s (%i)\r\n",
           pinmap_ff_default_pin_to_string(sda), sda,
           pinmap_ff_default_pin_to_string(scl), scl);

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
    tester.io_metrics_start();
    num_writes = i2c_write(&i2c, I2C_DEV_ADDR, (char *)data_out, TRANSFER_COUNT, true); //transaction ends with a stop condition
    num_acks = num_writes + 1;
    num_starts += 1;
    num_stops += 1;
    num_dev_addr_matches += 1;
    tester.io_metrics_stop();
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

    printf("  Pin combination works\r\n");

    printf("I2C complete read transaction test on sda=%s (%i), scl=%s (%i)\r\n",
           pinmap_ff_default_pin_to_string(sda), sda,
           pinmap_ff_default_pin_to_string(scl), scl);

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
    tester.io_metrics_start();
    num_reads = i2c_read(&i2c, (I2C_DEV_ADDR | 1), (char *)data_in, TRANSFER_COUNT, true); //transaction ends with a stop condition
    num_starts += 1;
    num_stops += 1;
    num_acks += 1;
    num_dev_addr_matches += 1;
    tester.io_metrics_stop();
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
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL((TRANSFER_COUNT & 0xFF), tester.get_next_from_slave());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    printf("  Pin combination works\r\n");

    printf("I2C write single bytes test on sda=%s (%i), scl=%s (%i)\r\n",
           pinmap_ff_default_pin_to_string(sda), sda,
           pinmap_ff_default_pin_to_string(scl), scl);

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
    tester.io_metrics_start();
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
    tester.io_metrics_stop();

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

    printf("  Pin combination works\r\n");

    printf("I2C read single bytes test on sda=%s (%i), scl=%s (%i)\r\n",
           pinmap_ff_default_pin_to_string(sda), sda,
           pinmap_ff_default_pin_to_string(scl), scl);

    // Reset tester stats and select I2C
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralI2C);
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
    tester.io_metrics_start();
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
        } else {
            data_in[i] = i2c_byte_read(&i2c, 0);//send ACK
            checksum += data_in[i];
            num_reads += 1;
        }
    }

    i2c_stop(&i2c);
    num_stops += 1;
    tester.io_metrics_stop();

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(num_dev_addr_matches, tester.num_dev_addr_matches());
    TEST_ASSERT_EQUAL(TRANSFER_COUNT, num_reads);
    TEST_ASSERT_EQUAL(num_reads + 1, tester.transfer_count());
    TEST_ASSERT_EQUAL(num_starts, tester.num_starts());
    TEST_ASSERT_EQUAL(num_stops, tester.num_stops());
    TEST_ASSERT_EQUAL(num_acks, tester.num_acks());
    TEST_ASSERT_EQUAL(num_nacks, tester.num_nacks());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());
    TEST_ASSERT_EQUAL(0, tester.state_num());
    TEST_ASSERT_EQUAL((TRANSFER_COUNT & 0xFF), tester.get_next_from_slave());
    TEST_ASSERT_EQUAL(num_writes, tester.num_writes());
    TEST_ASSERT_EQUAL(num_reads, tester.num_reads());

    printf("  Pin combination works\r\n");

    tester.reset();
}

Case cases[] = {
    Case("i2c - init/free test all pins", all_ports<I2CPort, DefaultFormFactor, test_i2c_init_free>),
    Case("i2c - test all i2c APIs", all_peripherals<I2CPort, DefaultFormFactor, i2c_test_all>)
    // Case("i2c - simple write test all peripherals", all_peripherals<I2CPort, DefaultFormFactor, test_i2c_simple_write>),
    // Case("i2c - simple read test all peripherals",  all_peripherals<I2CPort, DefaultFormFactor, test_i2c_simple_read>),
    // Case("i2c - XXXXXXX test single pin",           one_peripheral<I2CPort, DefaultFormFactor, test_i2c_XXXXXXX>)
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
