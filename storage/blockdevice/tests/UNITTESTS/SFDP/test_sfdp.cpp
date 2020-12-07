/* Copyright (c) 2020 ARM Limited
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

class TestSFDP : public testing::Test {
protected:
    struct mbed::sfdp_smptbl_info smptbl;

    /**
     * Construct Mbed OS SFDP info.
     * Normally this is parsed from the flash-chips's
     * raw SFDP table bytes, but for unit test we construct
     * SFDP info manually
     */
    virtual void SetUp()
    {
        // The mock flash supports 4KB, 32KB and 64KB erase types
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
    }
};

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
