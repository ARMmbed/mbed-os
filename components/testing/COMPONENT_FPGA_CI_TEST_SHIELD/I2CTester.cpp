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

#include "I2CTester.h"
#include "fpga_config.h"

uint8_t I2CTester::num_starts()
{
    uint8_t num_starts = 0;
    read(TESTER_I2C_STARTS, &num_starts, sizeof(num_starts));
    return num_starts;
}

uint8_t I2CTester::num_stops()
{
    uint8_t num_stops = 0;
    read(TESTER_I2C_STOPS, &num_stops, sizeof(num_stops));
    return num_stops;
}

uint16_t I2CTester::num_acks()
{
    uint16_t num_acks = 0;
    read(TESTER_I2C_ACKS, (uint8_t *)&num_acks, sizeof(num_acks));
    return num_acks;
}

uint16_t I2CTester::num_nacks()
{
    uint16_t num_nacks = 0;
    read(TESTER_I2C_NACKS, (uint8_t *)&num_nacks, sizeof(num_nacks));
    return num_nacks;
}

uint16_t I2CTester::transfer_count()
{
    uint16_t transfers = 0;
    MBED_ASSERT(sizeof(transfers) == TESTER_I2C_TRANSFERS_SIZE);
    read(TESTER_I2C_TRANSFERS, (uint8_t *)&transfers, sizeof(transfers));
    return transfers;
}

uint32_t I2CTester::get_receive_checksum()
{
    uint32_t to_slave_checksum = 0;
    MBED_ASSERT(sizeof(to_slave_checksum) == TESTER_I2C_TO_SLAVE_CHECKSUM_SIZE);
    read(TESTER_I2C_TO_SLAVE_CHECKSUM, (uint8_t *)&to_slave_checksum, sizeof(to_slave_checksum));
    return to_slave_checksum;
}

uint32_t I2CTester::get_send_checksum()
{
    uint32_t from_slave_checksum = 0;
    MBED_ASSERT(sizeof(from_slave_checksum) == TESTER_I2C_FROM_SLAVE_CHECKSUM_SIZE);
    read(TESTER_I2C_FROM_SLAVE_CHECKSUM, (uint8_t *)&from_slave_checksum, sizeof(from_slave_checksum));
    return from_slave_checksum;
}

uint8_t I2CTester::state_num()
{
    uint8_t state_num = 0;
    read(TESTER_I2C_STATE_NUM, &state_num, sizeof(state_num));
    return state_num;
}

uint8_t I2CTester::num_dev_addr_matches()
{
    uint8_t num_dev_addr_matches = 0;
    read(TESTER_I2C_NUMBER_DEV_ADDR_MATCHES, &num_dev_addr_matches, sizeof(num_dev_addr_matches));
    return num_dev_addr_matches;
}

uint8_t I2CTester::num_dev_addr_mismatches()
{
    uint8_t num_dev_addr_mismatches = 0;
    read(TESTER_I2C_NUMBER_DEV_ADDR_MISMATCHES, &num_dev_addr_mismatches, sizeof(num_dev_addr_mismatches));
    return num_dev_addr_mismatches;
}

void I2CTester::set_device_address(uint16_t addr)
{
    uint16_t data = addr;
    write(TESTER_I2C_DEVICE_ADDRESS, (uint8_t *)&data, sizeof(data));
}

uint16_t I2CTester::get_device_address()
{
    uint16_t addr = 0;
    read(TESTER_I2C_DEVICE_ADDRESS, (uint8_t *)&addr, sizeof(addr));
    return addr;
}

void I2CTester::set_sda(uint8_t value)
{
    uint8_t val = value;
    write(TESTER_I2C_SET_SDA, &val, sizeof(val));
}

uint8_t I2CTester::get_prev_to_slave_4()
{
    uint8_t prev_to_slave_4 = 0;
    read(TESTER_I2C_PREV_TO_SLAVE_4, &prev_to_slave_4, sizeof(prev_to_slave_4));
    return prev_to_slave_4;
}
uint8_t I2CTester::get_prev_to_slave_3()
{
    uint8_t prev_to_slave_3 = 0;
    read(TESTER_I2C_PREV_TO_SLAVE_3, &prev_to_slave_3, sizeof(prev_to_slave_3));
    return prev_to_slave_3;
}
uint8_t I2CTester::get_prev_to_slave_2()
{
    uint8_t prev_to_slave_2 = 0;
    read(TESTER_I2C_PREV_TO_SLAVE_2, &prev_to_slave_2, sizeof(prev_to_slave_2));
    return prev_to_slave_2;
}
uint8_t I2CTester::get_prev_to_slave_1()
{
    uint8_t prev_to_slave_1 = 0;
    read(TESTER_I2C_PREV_TO_SLAVE_1, &prev_to_slave_1, sizeof(prev_to_slave_1));
    return prev_to_slave_1;
}
void I2CTester::set_next_from_slave(uint8_t value)
{
    uint8_t val = value;
    write(TESTER_I2C_NEXT_FROM_SLAVE, &val, sizeof(val));
}
uint8_t I2CTester::get_next_from_slave()
{
    uint8_t next_from_slave = 0;
    read(TESTER_I2C_NEXT_FROM_SLAVE, &next_from_slave, sizeof(next_from_slave));
    return next_from_slave;
}
uint16_t I2CTester::num_writes()
{
    uint16_t num_writes = 0;
    read(TESTER_I2C_NUM_WRITES, (uint8_t *)&num_writes, sizeof(num_writes));
    return num_writes;
}
uint16_t I2CTester::num_reads()
{
    uint16_t num_reads = 0;
    read(TESTER_I2C_NUM_READS, (uint8_t *)&num_reads, sizeof(num_reads));
    return num_reads;
}
