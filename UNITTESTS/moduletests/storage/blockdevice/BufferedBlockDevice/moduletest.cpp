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
#include "features/storage/blockdevice/BufferedBlockDevice.h"

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)

class BufferedBlockModuleTest : public testing::Test {
protected:
    HeapBlockDevice bd_heap{DEVICE_SIZE};
    BufferedBlockDevice bd{&bd_heap};
    uint8_t *magic;
    uint8_t *buf;
    virtual void SetUp()
    {
        ASSERT_EQ(bd.init(), 0);
        magic = new uint8_t[BLOCK_SIZE];
        buf = new uint8_t[BLOCK_SIZE];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE; i++) {
            magic[i] = 0xaa + i;
        }
    }

    virtual void TearDown()
    {
        ASSERT_EQ(bd.deinit(), 0);
        delete[] magic;
        delete[] buf;
    }
};

TEST_F(BufferedBlockModuleTest, init)
{
    BufferedBlockDevice b(&bd_heap);
    EXPECT_EQ(b.get_erase_size(), 0);
    EXPECT_EQ(b.get_erase_size(0), 0);
    EXPECT_EQ(b.get_erase_value(), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.size(), 0);

    EXPECT_EQ(b.init(), 0);

    EXPECT_EQ(b.get_erase_size(), bd_heap.get_erase_size());
    EXPECT_EQ(b.get_erase_size(0), bd_heap.get_erase_size(0));
    EXPECT_EQ(b.get_erase_value(), bd_heap.get_erase_value());
    EXPECT_EQ(b.get_program_size(), 1);
    EXPECT_EQ(b.get_read_size(), 1);
    EXPECT_EQ(b.size(), bd_heap.size());
    EXPECT_EQ(b.get_type(), bd_heap.get_type());
}

TEST_F(BufferedBlockModuleTest, read_full_block)
{
    bd_heap.program(magic, 0, BLOCK_SIZE);
    EXPECT_EQ(0, bd.read(buf, 0, BLOCK_SIZE));
    EXPECT_EQ(0, memcmp(magic, buf, BLOCK_SIZE));
}

TEST_F(BufferedBlockModuleTest, over_read)
{
    bd_heap.program(magic, DEVICE_SIZE - BLOCK_SIZE, BLOCK_SIZE);
    EXPECT_EQ(bd.read(buf, DEVICE_SIZE - BLOCK_SIZE, BLOCK_SIZE), 0);
    EXPECT_EQ(bd.read(buf, DEVICE_SIZE, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
}

TEST_F(BufferedBlockModuleTest, unalign_read)
{
    bd_heap.program(magic, 0, BLOCK_SIZE);
    bd_heap.program(magic, BLOCK_SIZE, BLOCK_SIZE);
    EXPECT_EQ(bd.read(buf, BLOCK_SIZE/2, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic+(BLOCK_SIZE/2), BLOCK_SIZE/2));
    EXPECT_EQ(0, memcmp(magic, buf+(BLOCK_SIZE/2), BLOCK_SIZE/2));
}

TEST_F(BufferedBlockModuleTest, align_big_read)
{
    uint8_t *buffer = new uint8_t[BLOCK_SIZE*2];
    bd_heap.program(magic, 0, BLOCK_SIZE);
    bd_heap.program(magic, BLOCK_SIZE, BLOCK_SIZE);
    // Should cause 1 full block to be read unaligned from the device
    // second block would require buffering, because it is not a full (-1)
    EXPECT_EQ(bd.read(buffer, 0, (BLOCK_SIZE*2)-1), 0);
    EXPECT_EQ(0, memcmp(magic, buffer, BLOCK_SIZE));
    EXPECT_EQ(0, memcmp(magic, buffer+BLOCK_SIZE, BLOCK_SIZE-1));
    delete[] buffer;
}

TEST_F(BufferedBlockModuleTest, program_small_chunks)
{
    for (int i=0; i < BLOCK_SIZE - 1; ++i) {
        EXPECT_EQ(bd.program(magic+i, i, 1), 0);
    }
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE - 1));
}

TEST_F(BufferedBlockModuleTest, program_and_read_from_cache)
{
    bd_heap.program(magic, 0, BLOCK_SIZE);
    EXPECT_EQ(bd.program("a", 0, 1), 0);
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ('a', buf[0]);
    EXPECT_EQ(0, memcmp(buf+1, magic+1, BLOCK_SIZE-1));
}

TEST_F(BufferedBlockModuleTest, program_and_verify_from_storage)
{
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE/2), 0);
    EXPECT_EQ(bd.program(magic+BLOCK_SIZE/2, BLOCK_SIZE/2, BLOCK_SIZE/2), 0); // This should actually write to device
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));
    // Verify that data actually is in the underlying block device
    bd_heap.read(buf, 0, BLOCK_SIZE);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));
}

TEST_F(BufferedBlockModuleTest, flush_automatically)
{
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE/2), 0); // Don't write full block
    EXPECT_EQ(bd.program(magic, BLOCK_SIZE, BLOCK_SIZE/2), 0); // This should cause flush() for previous data in cache
    // Verify that data actually is in the underlying block device
    bd_heap.read(buf, 0, BLOCK_SIZE);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE/2));
}

TEST_F(BufferedBlockModuleTest, flush_at_exit)
{
    bd_heap.init(); // Extra init, to prevent deinit() when BufferedBlockDevice de-inits.
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE/2), 0); // Don't write full block
    EXPECT_EQ(bd.deinit(), 0);
    // Verify that data actually is in the underlying block device
    EXPECT_EQ(bd_heap.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE/2));
}
