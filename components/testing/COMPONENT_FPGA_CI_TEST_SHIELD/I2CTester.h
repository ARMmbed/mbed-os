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

#ifndef I2C_TESTER_H
#define I2C_TESTER_H

#include "MbedTester.h"


class I2CTester: public MbedTester {
public:

    I2CTester(const PinList *form_factor, const PinList *exclude_pins)
        : MbedTester(form_factor, exclude_pins)
    {

    }

    /* **I2C peripheral functions** */

    /**
     * Get the number of start conditions since last I2C reset
     *
     * @return The number of start conditions
     */
    uint8_t num_starts();

    /**
     * Get the number of stop conditions since last I2C reset
     *
     * @return The number of stop conditions
     */
    uint8_t num_stops();

    /**
     * Get the number of ACKs since last I2C reset
     *
     * @return The number of ACKs
     */
    uint16_t num_acks();

    /**
     * Get the number of NACKs since last I2C reset
     *
     * @return The number of NACKs
     */
    uint16_t num_nacks();

    /**
     * Read the number of transfers which have occurred
     *
     * @return The number of I2C transfers that have completed since
     *         i2c was reset, including the device address byte.
     */
    uint16_t transfer_count();

    /**
     * Read a checksum of data send to the tester
     *
     * @return The sum of all bytes sent to the tester since reset
     */
    uint32_t get_receive_checksum();

    /**
     * Read a checksum the tester calculated for data it has already sent
     *
     * @return The sum of all bytes the tester has sent since reset
     */
    uint32_t get_send_checksum();

    /**
     * Get the I2C slave state number
     *
     * @return The state number
     */
    uint8_t state_num();

    /**
     * Get the number of times the device address has been sent correctly
     *
     * @return The number of times the device address has been sent correctly
     */
    uint8_t num_dev_addr_matches();

    /**
     * Get the number of times the device address has been sent incorrectly
     *
     * @return The number of times the device address has been sent incorrectly
     */
    uint8_t num_dev_addr_mismatches();

    /**
     * Set the I2C slave device address
     *
     * @param addr New address for slave device
     */
    void set_device_address(uint16_t addr);

    /**
     * Get the I2C slave device address
     *
     * @return The slave device address
     */
    uint16_t get_device_address();

    /**
     * Set SDA (test mode)
     *
     * @param value Test value for SDA
     */
    void set_sda(uint8_t value);

    /**
     * Get the value written to slave in the fourth to last transaction
     *
     * @return value written to slave in the fourth to last transaction
     */
    uint8_t get_prev_to_slave_4();

    /**
     * Get the value written to slave in the third to last transaction
     *
     * @return value written to slave in the third to last transaction
     */
    uint8_t get_prev_to_slave_3();

    /**
     * Get the value written to slave in the second to last transaction
     *
     * @return value written to slave in the second to last transaction
     */
    uint8_t get_prev_to_slave_2();

    /**
     * Get the value written to slave in the last transaction
     *
     * @return value written to slave in the last transaction
     */
    uint8_t get_prev_to_slave_1();

    /**
     * Set the value to be read from slave in next read transaction
     *
     * @param value Value to be read from slave in next read transaction
     */
    void set_next_from_slave(uint8_t value);

    /**
     * Get the value to be read from slave in next read transaction
     *
     * @return Value to be read from slave in next read transaction
     */
    uint8_t get_next_from_slave();

    /**
     * Read the number of writes which have occurred
     *
     * @return The number of I2C writes that have completed since
     *         i2c was reset, not including the device address byte
     */
    uint16_t num_writes();

    /**
     * Read the number of reads which have occurred
     *
     * @return The number of I2C reads that have completed since
     *         i2c was reset, not including the device address byte
     */
    uint16_t num_reads();

};

#endif
