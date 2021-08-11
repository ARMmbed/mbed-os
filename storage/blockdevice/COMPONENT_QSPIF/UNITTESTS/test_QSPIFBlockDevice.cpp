/* Copyright (c) 2021 Arm Limited
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

#include <cassert>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "QSPIF/QSPIFBlockDevice.h"

// S25FS512S's CR3NV register needs a quirk
static const int S25FS512S_CR1NV = 0x2;
static const int S25FS512S_CR3NV = 0x4;

// JEDEC Manufacturer and Device ID, set by the test cases
static uint8_t const *device_id;

namespace mbed {

/**
 * Fake implementation of mbed::QSPI class for QSPIFBlockDevice unit tests.
 *
 * Note: Data output by this is either dummy or based on Cypress S25FS512S.
 * We can't use gMock for this because
 * - The entire mbed::QSPI is non-virtual. Mocks are derived classes, and
 * when a derived class instance is used as an instance of its base class,
 * non-virtual members of the base class get used.
 * - QSPIFBlockDevice's member _qspi (an instance of mbed::QSPI) is a value
 * instead of a reference, in order to support initializing from pins directly.
 * Also, mbed::QSPI is declared as NonCopyable whose copy-constructor is deleted,
 * so we can't copy a preexisting mbed::QSPI instance into QSPIFBlockDevice.
 * There's no way to dependency inject a mock instance.
 */
QSPI::QSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, int mode) : _qspi()
{
}

QSPI::~QSPI()
{
}

qspi_status_t QSPI::configure_format(qspi_bus_width_t inst_width, qspi_bus_width_t address_width, qspi_address_size_t address_size, qspi_bus_width_t alt_width, qspi_alt_size_t alt_size, qspi_bus_width_t data_width, int dummy_cycles)
{
    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::set_frequency(int hz)
{
    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::read(int address, char *rx_buffer, size_t *rx_length)
{
    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::write(int address, const char *tx_buffer, size_t *tx_length)
{
    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::read(qspi_inst_t instruction, int alt, int address, char *rx_buffer, size_t *rx_length)
{
    if (!rx_buffer) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    switch (address) {
        // CR1NV and CR3NV registers' factory default values are both 0x0
        case S25FS512S_CR1NV:
        case S25FS512S_CR3NV:
            if (!rx_length || *rx_length < 1) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            rx_buffer[0] = 0x00;
            break;
    }

    switch (instruction) {
        case mbed::SFDP_READ_CMD_INST: // read SFDP table
            if (!rx_length || *rx_length < SFDP_BASIC_PARAMS_TBL_SIZE) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            // set dummy data (zeros), to avoid warning of uninitialized
            // data from Valgrind and inconsistent test behavior.
            memset(rx_buffer, 0x00, *rx_length);
            // soft reset needs one instruction to enable reset,
            // another instruction to perform reset
            rx_buffer[61] = 0x30;
            break;
    }

    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::write(qspi_inst_t instruction, int alt, int address, const char *tx_buffer, size_t *tx_length)
{
    return QSPI_STATUS_OK;
}

qspi_status_t QSPI::command_transfer(qspi_inst_t instruction, int address, const char *tx_buffer, size_t tx_length, const char *rx_buffer, size_t rx_length)
{
    static char status_registers[2];
    switch (instruction) {
        case 0x01: // write status registers
            if (!tx_buffer || tx_length < 2) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            memcpy(status_registers, tx_buffer, tx_length);
            break;
        case 0x05: // read status register 1
            if (!rx_buffer || rx_length < 1) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            const_cast<char *>(rx_buffer)[0] = status_registers[0];
            break;
        case 0x35: // read status register 2
            if (!rx_buffer || rx_length < 1) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            const_cast<char *>(rx_buffer)[0] = status_registers[1];
            break;
        case 0x06: // set write enabled bit
            status_registers[0] |= 0x2;
            break;
        case 0x9F: // read Manufacturer and JDEC Device ID
            assert(nullptr != device_id); // Test cases should set device_id
            if (!rx_buffer || rx_length < 3) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            memcpy(const_cast<char *>(rx_buffer), device_id, 3);
            break;
    }
    return QSPI_STATUS_OK;
}

void QSPI::lock()
{
}

void QSPI::unlock()
{
}

bool QSPI::_initialize()
{
    return true;
}

bool QSPI::_initialize_direct()
{
    return true;
}

void QSPI::_build_qspi_command(qspi_inst_t instruction, int address, int alt)
{
}

/**
 * Fake implementation of SFDP functions.
 * They can't be mocked with gMock which supports only class member functions,
 * not free/global functions.
 */
static Callback<int(bd_addr_t, sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, bd_size_t)> test_sfdp_reader;

int sfdp_parse_headers(Callback<int(bd_addr_t, sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &sfdp_info)
{
    // The SFDP Basic Parameters Table size is used as a QSPI buffer
    // size, so it must be set.
    sfdp_info.bptbl.size = SFDP_BASIC_PARAMS_TBL_SIZE;
    return 0;
}

int sfdp_parse_sector_map_table(Callback<int(bd_addr_t, sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &sfdp_info)
{
    // The real implementation of this function queries
    // the CR3NV register on S25FS512S.
    test_sfdp_reader = sfdp_reader;
    return 0;
}

size_t sfdp_detect_page_size(uint8_t *bptbl_ptr, size_t bptbl_size)
{
    return 0;
}

int sfdp_detect_erase_types_inst_and_size(uint8_t *bptbl_ptr, sfdp_hdr_info &sfdp_info)
{
    return 0;
}

int sfdp_find_addr_region(bd_addr_t offset, const sfdp_hdr_info &sfdp_info)
{
    return 0;
}

int sfdp_iterate_next_largest_erase_type(uint8_t bitfield,
                                         bd_size_t size,
                                         bd_addr_t offset,
                                         int region,
                                         const sfdp_smptbl_info &smptbl)
{
    return 0;
}

int sfdp_detect_device_density(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info)
{
    return 0;
}

int sfdp_detect_addressability(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info)
{
    return 0;
}

} // namespace mbed


class TestQSPIFBlockDevice : public testing::Test {
protected:
    TestQSPIFBlockDevice()
        : bd(PinName(0), PinName(1), PinName(2), PinName(3), PinName(4), PinName(5)) // fake pins
    {
    }

    virtual void TearDown()
    {
        device_id = nullptr;
    }

    QSPIFBlockDevice bd;
};

/**
 * Test that the quirk on the reported values of the CR1NV and CR3NV
 * registers is applied when the flash chip is S25FS512S.
 */
TEST_F(TestQSPIFBlockDevice, TestS25FS512SQuirkApplied)
{
    // Use flash chip S25FS512S
    const uint8_t id_S25FS512S[] { 0x01, 0x02, 0x20 };
    device_id = id_S25FS512S;
    EXPECT_EQ(QSPIF_BD_ERROR_OK, bd.init());

    const uint8_t id_N25Q128A[] { 0x20, 0xBB, 0x18 };

    // Read the CR1NV register
    uint8_t CR1NV;
    EXPECT_EQ(QSPIF_BD_ERROR_OK, mbed::test_sfdp_reader(
                  S25FS512S_CR1NV,
                  mbed::SFDP_CMD_ADDR_SIZE_VARIABLE,
                  0x65,
                  mbed::SFDP_CMD_DUMMY_CYCLES_VARIABLE,
                  &CR1NV,
                  sizeof(CR1NV)));

    // Read the CR3NV register
    uint8_t CR3NV;
    EXPECT_EQ(QSPIF_BD_ERROR_OK, mbed::test_sfdp_reader(
                  S25FS512S_CR3NV,
                  mbed::SFDP_CMD_ADDR_SIZE_VARIABLE,
                  0x65,
                  mbed::SFDP_CMD_DUMMY_CYCLES_VARIABLE,
                  &CR3NV,
                  sizeof(CR3NV)));

    // The hardware value of CR3NV[1] is 0 but S25FS512S's SFDP table
    // expects it to be 1.
    EXPECT_EQ(0x2, CR3NV & 0x02);

    // The factory default configuration is CR1NV[2] == 0 and CR3NV[3] == 0
    // (eight 4KB sectors overlaying the start of the flash) but we treat
    // the flash chip as if CR1NV[2] == 0 and CR3NV[3] == 1 (no overlaying 4KB
    // sectors), as Mbed OS does not support this type of flash layout.
    EXPECT_EQ(0x0, CR1NV & 0x4);
    EXPECT_EQ(0x8, CR3NV & 0x8);

    // Deinitialization
    EXPECT_EQ(QSPIF_BD_ERROR_OK, bd.deinit());
}

/**
 * Test that the quirk on the reported values of the CR1NV and CR3NV
 * registers is not applied when the flash chip is not S25FS512S.
 * Note: Other flash chips most likely do not have CR1NV or CR3NV, but
 * they might have something else readable at the same addresses.
 */
TEST_F(TestQSPIFBlockDevice, TestS25FS512SQuirkNotApplied)
{
    // Use flash chip N25Q128A
    const uint8_t id_N25Q128A[] { 0x20, 0xBB, 0x18 };
    device_id = id_N25Q128A;
    EXPECT_EQ(QSPIF_BD_ERROR_OK, bd.init());

    // Read the value at the address of S25FS512S's CR1NV register,
    // assuming this address is readable.
    uint8_t CR1NV;
    EXPECT_EQ(QSPIF_BD_ERROR_OK, mbed::test_sfdp_reader(
                  S25FS512S_CR1NV,
                  mbed::SFDP_CMD_ADDR_SIZE_VARIABLE,
                  0x65,
                  mbed::SFDP_CMD_DUMMY_CYCLES_VARIABLE,
                  &CR1NV,
                  sizeof(CR1NV)));

    // Read the value at the address of S25FS512S's CR3NV register,
    // assuming this address is readable.
    uint8_t CR3NV;
    EXPECT_EQ(QSPIF_BD_ERROR_OK, mbed::test_sfdp_reader(
                  S25FS512S_CR3NV,
                  mbed::SFDP_CMD_ADDR_SIZE_VARIABLE,
                  0x65,
                  mbed::SFDP_CMD_DUMMY_CYCLES_VARIABLE,
                  &CR3NV,
                  sizeof(CR3NV)));

    // Values (0) reported by the fake QSPI::read() should be unmodifed
    EXPECT_EQ(0, CR1NV);
    EXPECT_EQ(0, CR3NV);

    // Deinitialization
    EXPECT_EQ(QSPIF_BD_ERROR_OK, bd.deinit());
}
