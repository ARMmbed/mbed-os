/* Copyright (c) 2019 ARM Limited
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
#include "features/storage/blockdevice/HeapBlockDevice.h"
#include "features/storage/blockdevice/SlicingBlockDevice.h"

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)

class VerifyBorders_HeapBlockDevice : public  mbed::HeapBlockDevice {
public:
    mutable bool borders_crossed;
    mutable bd_size_t lower_limit;
    mutable bd_size_t upper_limit;

    VerifyBorders_HeapBlockDevice(bd_size_t size)
        : HeapBlockDevice(size)
    {
        borders_crossed = false;
        lower_limit = 0;
        upper_limit = size;
    }

    virtual bool is_valid_read(bd_addr_t addr, bd_size_t size) const
    {
        borders_crossed |= addr < lower_limit;
        borders_crossed |= addr + size > upper_limit;
        return BlockDevice::is_valid_read(addr, size);
    }

    virtual bool is_valid_program(bd_addr_t addr, bd_size_t size) const
    {
        borders_crossed |= addr < lower_limit;
        borders_crossed |= addr + size > upper_limit;
        return BlockDevice::is_valid_program(addr, size);
    }

    virtual bool is_valid_erase(bd_addr_t addr, bd_size_t size) const
    {
        borders_crossed |= addr < lower_limit;
        borders_crossed |= addr + size > upper_limit;
        return BlockDevice::is_valid_erase(addr, size);
    }
};

class SlicingBlockModuleTest : public testing::Test {
protected:
    VerifyBorders_HeapBlockDevice bd{DEVICE_SIZE};
    uint8_t *magic;
    uint8_t *buf;
    virtual void SetUp()
    {
        bd.init();
        magic = new uint8_t[BLOCK_SIZE];
        buf = new uint8_t[BLOCK_SIZE];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE; i++) {
            magic[i] = 0xaa + i;
        }
    }

    virtual void TearDown()
    {
        bd.deinit();
        delete[] magic;
        delete[] buf;
    }
};

TEST_F(SlicingBlockModuleTest, constructor)
{
    mbed::SlicingBlockDevice slice(&bd, 0, bd.size());
    EXPECT_EQ(slice.init(), BD_ERROR_OK);
    EXPECT_EQ(slice.get_read_size(), bd.get_read_size());
    EXPECT_EQ(slice.get_program_size(), bd.get_read_size());
    EXPECT_EQ(slice.get_erase_size(), bd.get_read_size());
    EXPECT_EQ(slice.get_erase_size(0), bd.get_read_size());
    EXPECT_EQ(slice.deinit(), BD_ERROR_OK);
}

TEST_F(SlicingBlockModuleTest, slice_in_middle)
{
    uint8_t *program = new uint8_t[BLOCK_SIZE] {0xbb,0xbb,0xbb};

    //Write magic value to heap block before and after the space for slice
    bd.program(magic, 0, BLOCK_SIZE);
    bd.program(magic, BLOCK_SIZE * 3, BLOCK_SIZE);

    bd.upper_limit = BLOCK_SIZE * 3;
    bd.lower_limit = BLOCK_SIZE;
    bd.borders_crossed = false;

    //Skip first block, then create sclicing device, with size of 2 blocks
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    EXPECT_EQ(slice.init(), BD_ERROR_OK);
    EXPECT_EQ(BLOCK_SIZE * 2, slice.size());
    EXPECT_EQ(bd.borders_crossed, false);

    //Program a test value
    EXPECT_EQ(slice.program(program, 0, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(slice.program(program, BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(bd.borders_crossed, false);

    //Verify that blocks before and after the slicing blocks are not touched
    bd.read(buf, 0, BLOCK_SIZE);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));
    bd.read(buf, BLOCK_SIZE * 3, BLOCK_SIZE);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));

    delete[] program;
}

TEST_F(SlicingBlockModuleTest, slice_at_the_end)
{
    uint8_t *program = new uint8_t[BLOCK_SIZE] {0xbb,0xbb,0xbb};

    //Write magic value to heap block before the space for slice
    // our bd is 10*BLOCK_SIZE, so sector 7
    bd.program(magic, BLOCK_SIZE * 7, BLOCK_SIZE);

    //Screate sclicing device, with size of 2 blocks
    // Use negative index
    mbed::SlicingBlockDevice slice(&bd, -BLOCK_SIZE*2);
    EXPECT_EQ(slice.init(), BD_ERROR_OK);
    EXPECT_EQ(BLOCK_SIZE * 2, slice.size());

    //Program a test value
    EXPECT_EQ(slice.program(program, 0, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(slice.program(program, BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_OK);

    //Verify that blocks before and after the slicing blocks are not touched
    bd.read(buf, BLOCK_SIZE * 7, BLOCK_SIZE);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));

    delete[] program;
}

TEST_F(SlicingBlockModuleTest, over_write)
{
    uint8_t *program = new uint8_t[BLOCK_SIZE] {0xbb,0xbb,0xbb};
    uint8_t *buf = new uint8_t[BLOCK_SIZE];

    //Screate sclicing device, with size of 2 blocks
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    EXPECT_EQ(slice.init(), BD_ERROR_OK);

    EXPECT_EQ(slice.program(program, 0, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(slice.program(program, BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_OK);

    // Verify written value
    EXPECT_EQ(slice.read(buf, BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(0, memcmp(buf, program, BLOCK_SIZE));

    //Program a test value to address that is one pass the device size
    EXPECT_EQ(slice.program(program, 2 * BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);

    delete[] buf;
    delete[] program;
}

TEST_F(SlicingBlockModuleTest, over_read)
{
    uint8_t *buf = new uint8_t[BLOCK_SIZE];

    //Screate sclicing device, with size of 2 blocks
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    EXPECT_EQ(slice.init(), BD_ERROR_OK);

    //Try to read a block after the slice
    EXPECT_EQ(slice.read(buf, 2 * BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    delete[] buf;
}

TEST_F(SlicingBlockModuleTest, get_type)
{
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    EXPECT_EQ(bd.get_type(), slice.get_type());
}

TEST_F(SlicingBlockModuleTest, get_erase_value)
{
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    EXPECT_EQ(bd.get_erase_value(), slice.get_erase_value());
}

TEST_F(SlicingBlockModuleTest, erase)
{
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    EXPECT_EQ(slice.erase(0, BLOCK_SIZE), BD_ERROR_OK);
    // Erase one block after the slice
    EXPECT_EQ(slice.erase(2*BLOCK_SIZE, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
}

TEST_F(SlicingBlockModuleTest, sync)
{
    mbed::SlicingBlockDevice slice(&bd, BLOCK_SIZE, BLOCK_SIZE * 3);
    // Just a pass through
    EXPECT_EQ(slice.sync(), 0);
}

TEST_F(SlicingBlockModuleTest, too_big_to_init)
{
    mbed::SlicingBlockDevice slice(&bd, 0, DEVICE_SIZE + BLOCK_SIZE);
    // Just a pass through
    EXPECT_EQ(slice.init(), BD_ERROR_DEVICE_ERROR);
}
