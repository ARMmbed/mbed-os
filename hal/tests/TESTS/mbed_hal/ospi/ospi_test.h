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

/** \addtogroup hal_ospi
 *  @{
 *  \defgroup hal_ospi_tests Tests
 *  OSPI tests of the HAL.
 *  @{
 */
#ifndef MBED_OSPI_TEST_H
#define MBED_OSPI_TEST_H

#include "ospi_test_utils.h"
#include "ospi_api.h"


#if DEVICE_OSPI

/** Test that ospi_init/ospi_free can be called multiple times.
 *
 *  Given board provides OSPI.
 *  When ospi_init/ospi_free is called multiple times.
 *  Then ospi_init/ospi_free are successfully performed (no exception is generated).
 *
 */
void ospi_init_free_test(void);

/** Test ospi frequency setting.
 *
 *  Given board provides OSPI, with OSPI already initialized.
 *  When set OSPI frequency.
 *  Then freguency setting is successfully performed (no exception is generated).
 *
 */
void ospi_frequency_test(void);

/** Template for write/read tests
 *
 *  Test single write/read operation of a block of data to/from the specific memory address
 *  Given board provides OSPI, with OSPI already initialized.
 *  When perform write and then read operations.
 *  Then data is successfully written and then read (no exception is generated) and the read data is valid.
 *
 *  Test multiple write/read operation of a block of data to/from the same specific memory address
 *  Given board provides OSPI, with OSPI already initialized.
 *  When perform write and then read operations.
 *  Then data is successfully written and then read (no exception is generated) and the read data is valid.
 *
 *  Test multiple adjacent write and single read operation of a block of data to/from the specific memory address
 *  Given board provides OSPI, with OSPI already initialized.
 *  When perform write and then read operations.
 *  Then data is successfully written and then read (no exception is generated) and the read data is valid.
 *
 *  Test single write and multiple adjacent read operation of a block of data to/from the specific memory address
 *  Given board provides OSPI, with OSPI already initialized.
 *  When perform write and then read operations.
 *  Then data is successfully written and then read (no exception is generated) and the read data is valid.
 *
 */
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
void ospi_write_read_test(void);

#endif

#endif

/** @}*/
/** @}*/
