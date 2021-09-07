/* Copyright (c) 2020-2021 ARM Limited
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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "blockdevice/internal/SFDP.h"

using ::testing::_;
using ::testing::MockFunction;
using ::testing::Return;

/**
 * The Sector Map Parameter Table of Cypress S25FS512S:
 * https://www.cypress.com/file/216376/download Table 71.
 */
static const mbed::bd_addr_t sector_map_start_addr = 0xD81000;

/**
 * Based on Cypress S25FS512S, modified to have one descriptor,
 * three regions for test purpose.
 */
static const uint8_t sector_map_single_descriptor[] {
    0xFF, 0x01, 0x02, 0xFF,     // header, highest region = 0x02
    0xF1, 0x7F, 0x00, 0x00,     // region 0
    0xF4, 0x7F, 0x03, 0x00,     // region 1
    0xF4, 0xFF, 0xFB, 0x03      // region 2
};

/**
 * Based on Cypress S25FS512S, modified to have one descriptor,
 * twelve regions for test purpose.
 */
static const uint8_t sector_map_single_descriptor_twelve_regions[] {
    0xFF, 0x01, 0x0B, 0xFF,     // header, highest region = 0x0B
    0xF1, 0x7F, 0x00, 0x00,     // region 0
    0xF4, 0x7F, 0x03, 0x00,     // region 1
    0xF4, 0xFF, 0xFB, 0x03,     // region 2
    0xF1, 0x7F, 0x00, 0x00,     // region 3
    0xF4, 0x7F, 0x03, 0x00,     // region 4
    0xF4, 0xFF, 0xFB, 0x03,     // region 5
    0xF1, 0x7F, 0x00, 0x00,     // region 6
    0xF4, 0x7F, 0x03, 0x00,     // region 7
    0xF4, 0xFF, 0xFB, 0x03,     // region 8
    0xF1, 0x7F, 0x00, 0x00,     // region 9
    0xF4, 0x7F, 0x03, 0x00,     // region 10
    0xF4, 0xFF, 0xFB, 0x03,     // region 11
};

class TestSFDP : public testing::Test {

public:
    mbed::Callback<int(mbed::bd_addr_t, void*, bd_size_t)> sfdp_reader_callback;

protected:
    TestSFDP() : sfdp_reader_callback(this, &TestSFDP::sfdp_reader) {};

    int sfdp_reader(mbed::bd_addr_t addr, void *buff, bd_size_t buff_size)
    {
        int mock_return = sfdp_reader_mock.Call(addr, buff, buff_size);
        if (mock_return != 0) {
            return mock_return;
        }

        memcpy(buff, sector_descriptors, sector_descriptors_size);
        return 0;
    };

    void set_sector_map_param_table(mbed::sfdp_smptbl_info &smptbl, const uint8_t *table, const size_t table_size)
    {
        smptbl.size = table_size;
        smptbl.addr = sector_map_start_addr;

        sector_descriptors = table;
        sector_descriptors_size = table_size;
    }

    MockFunction<int(mbed::bd_addr_t, void*, bd_size_t)> sfdp_reader_mock;
    const uint8_t *sector_descriptors;
    bd_size_t sector_descriptors_size;
};

/**
 * Utilities for conversions to bytes.
 */
namespace{
    auto operator "" _B(unsigned long long int size) {
        return size;
    }

    auto operator "" _KB(unsigned long long int size) {
        return size * 1024;
    }

    auto operator "" _MB(unsigned long long int size) {
        return size * 1024 * 1024;
    }
}

/**
 * Test if sfdp_iterate_next_largest_erase_type() returns the most
 * optimal erase type, whose erase size is as large as possible
 * while being compatible with both alignment and size requirements.
 */
TEST_F(TestSFDP, TestEraseTypeAlgorithm)
{
    // Erase 104KB starting at 92KB
    int address = 92 * 1024;
    int size = 104 * 1024;
    int region = 1;
    int type;

    // The mock flash supports 4KB, 32KB and 64KB erase types
    struct mbed::sfdp_smptbl_info smptbl;
    smptbl.erase_type_size_arr[0] = 4 * 1024;
    smptbl.erase_type_size_arr[1] = 32 * 1024;
    smptbl.erase_type_size_arr[2] = 64 * 1024;

    // The mock flash has three regions, with address ranges:
    // * 0 to 64KB - 1B
    // * 64KB to 256KB - 1B
    // * 256KB to 1024KB - 1B
    smptbl.region_high_boundary[0] = 64 * 1024 - 1;
    smptbl.region_high_boundary[1] = 256 * 1024 - 1;
    smptbl.region_high_boundary[2] = 1024 * 1024 - 1;

    // Bitfields indicating which regions support which erase types
    smptbl.region_erase_types_bitfld[0] = 0b0001;   // 4KB only
    smptbl.region_erase_types_bitfld[1] = 0b0111;   // 64KB, 32KB, 4KB
    smptbl.region_erase_types_bitfld[2] = 0b0110;   // 64KB, 32KB

    // Expected outcome:
    // * The starting position 92KB is 4KB-aligned
    // * The next position 96KB (92KB + 4KB) is 32KB-aligned
    // * The next position 128KB (96KB + 32KB) is 64KB-aligned
    // * At the final position 192KB (128KB + 64KB), we only
    //   have 4KB (104KB - 4KB - 32KB - 64KB) remaining to erase
    int expected_erase_KBs[] = {4, 32, 64, 4};

    for (int i = 0; i < sizeof(expected_erase_KBs) / sizeof(int); i++) {
        type = sfdp_iterate_next_largest_erase_type(
                    smptbl.region_erase_types_bitfld[region],
                    size,
                    address,
                    region,
                    smptbl);
        int erase_size = smptbl.erase_type_size_arr[type];
        EXPECT_EQ(erase_size, expected_erase_KBs[i] * 1024);
        address += erase_size;
        size -= erase_size;
    }

    EXPECT_EQ(size, 0); // All erased

    // Test unaligned erase
    // Region 2 only allows at least 32KB-aligned erases
    address = (512 + 16) * 1024;
    size = 64 * 1024;
    region = 2;
    type = sfdp_iterate_next_largest_erase_type(
                smptbl.region_erase_types_bitfld[region],
                size,
                address,
                region,
                smptbl);
    EXPECT_EQ(type, -1); // Invalid erase
}

/**
 * Test that sfdp_parse_sector_map_table() treats a whole flash
 * as one region if no sector map is available.
 */
TEST_F(TestSFDP, TestNoSectorMap)
{
    const bd_size_t device_size = 512_KB;

    mbed::sfdp_hdr_info header_info;
    header_info.smptbl.size = 0; // No Sector Map Table
    header_info.bptbl.device_size_bytes = device_size;

    // No need to read anything
    EXPECT_CALL(sfdp_reader_mock, Call(_, _, _)).Times(0);

    EXPECT_EQ(0, sfdp_parse_sector_map_table(sfdp_reader_callback, header_info));

    EXPECT_EQ(1, header_info.smptbl.region_cnt);
    EXPECT_EQ(device_size, header_info.smptbl.region_size[0]);
    EXPECT_EQ(device_size - 1, header_info.smptbl.region_high_boundary[0]);
}

/**
 * When a Sector Map Parameter Table has a single descriptor (i.e. non-configurable flash layout).
 */
TEST_F(TestSFDP, TestSingleSectorConfig)
{
    mbed::sfdp_hdr_info header_info;
    set_sector_map_param_table(header_info.smptbl, sector_map_single_descriptor, sizeof(sector_map_single_descriptor));

    EXPECT_CALL(sfdp_reader_mock, Call(sector_map_start_addr, _, sizeof(sector_map_single_descriptor)))
    .Times(1)
    .WillOnce(Return(0));

    EXPECT_EQ(0, sfdp_parse_sector_map_table(sfdp_reader_callback, header_info));

    // Three regions
    EXPECT_EQ(3, header_info.smptbl.region_cnt);

    // Region 0: erase type 1 (32KB erase)
    EXPECT_EQ(32_KB, header_info.smptbl.region_size[0]);
    EXPECT_EQ(32_KB - 1_B, header_info.smptbl.region_high_boundary[0]);
    EXPECT_EQ(1 << (1 - 1), header_info.smptbl.region_erase_types_bitfld[0]);

    // Region 1: erase type 3 (256KB erase which includes 32KB from Region 0)
    EXPECT_EQ(224_KB, header_info.smptbl.region_size[1]);
    EXPECT_EQ(256_KB - 1_B, header_info.smptbl.region_high_boundary[1]);
    EXPECT_EQ(1 << (3 - 1), header_info.smptbl.region_erase_types_bitfld[1]);

    // Region 2: erase type 3 (256KB erase)
    EXPECT_EQ(64_MB - 32_KB - 224_KB, header_info.smptbl.region_size[2]);
    EXPECT_EQ(64_MB - 1_B, header_info.smptbl.region_high_boundary[2]);
    EXPECT_EQ(1 << (3 - 1), header_info.smptbl.region_erase_types_bitfld[2]);
}

/**
 * When an SFDP reader fails to read data requested by sfdp_parse_sector_map_table().
 */
TEST_F(TestSFDP, TestSFDPReadFailure)
{
    mbed::sfdp_hdr_info header_info;
    set_sector_map_param_table(header_info.smptbl, sector_map_single_descriptor, sizeof(sector_map_single_descriptor));

    EXPECT_CALL(sfdp_reader_mock, Call(sector_map_start_addr, _, sizeof(sector_map_single_descriptor)))
    .Times(1)
    .WillOnce(Return(-1)); // Emulate read failure

    EXPECT_EQ(-1, sfdp_parse_sector_map_table(sfdp_reader_callback, header_info));
}

/**
 * When a flash layout has more regions than Mbed OS supports (10).
 * Note: This is unlikely to happens in practice.
 */
TEST_F(TestSFDP, TestMoreRegionsThanSupported)
{
    mbed::sfdp_hdr_info header_info;
    set_sector_map_param_table(
        header_info.smptbl,
        sector_map_single_descriptor_twelve_regions,
        sizeof(sector_map_single_descriptor_twelve_regions)
    );

    EXPECT_CALL(sfdp_reader_mock, Call(sector_map_start_addr, _, sizeof(sector_map_single_descriptor_twelve_regions)))
    .Times(1)
    .WillOnce(Return(0));

    EXPECT_EQ(-1, sfdp_parse_sector_map_table(sfdp_reader_callback, header_info));
}
