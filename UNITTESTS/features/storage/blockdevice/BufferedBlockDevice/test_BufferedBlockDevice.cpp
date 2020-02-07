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
#include "features/storage/blockdevice/BufferedBlockDevice.h"
#include "stubs/BlockDevice_mock.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::DoAll;

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)

class BufferedBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock;
    BufferedBlockDevice bd{&bd_mock};
    uint8_t *magic;
    uint8_t *buf;
    virtual void SetUp()
    {
        EXPECT_CALL(bd_mock, init());
        EXPECT_CALL(bd_mock, get_read_size()).WillOnce(Return(BLOCK_SIZE));
        EXPECT_CALL(bd_mock, get_program_size()).WillOnce(Return(BLOCK_SIZE));
        EXPECT_CALL(bd_mock, size()).WillOnce(Return(DEVICE_SIZE));
        ASSERT_EQ(bd.init(), 0);
        magic = new uint8_t[2*BLOCK_SIZE];
        buf = new uint8_t[2*BLOCK_SIZE];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE*2; i++) {
            magic[i] = 0xaa + i;
            buf[i] = 0;
        }
    }

    virtual void TearDown()
    {
        EXPECT_CALL(bd_mock, deinit());
        EXPECT_CALL(bd_mock, sync()); // Called on deinit
        ASSERT_EQ(bd.deinit(), 0);
        delete[] magic;
        delete[] buf;
    }
};

TEST_F(BufferedBlockModuleTest, init)
{
    BufferedBlockDevice b(&bd_mock);
    EXPECT_EQ(b.get_erase_size(), 0);
    EXPECT_EQ(b.get_erase_size(0), 0);
    EXPECT_EQ(b.get_erase_value(), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(b.program(magic, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.read(buf, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.trim(0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.deinit(), BD_ERROR_OK);
    EXPECT_EQ(b.sync(), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.erase(0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);

    EXPECT_CALL(bd_mock, init());
    EXPECT_CALL(bd_mock, size()).WillOnce(Return(DEVICE_SIZE));
    EXPECT_CALL(bd_mock, get_read_size()).WillOnce(Return(DEVICE_SIZE));
    EXPECT_CALL(bd_mock, get_program_size()).WillOnce(Return(DEVICE_SIZE));

    EXPECT_EQ(b.init(), 0);

    EXPECT_CALL(bd_mock, get_erase_size()).WillOnce(Return(17));
    EXPECT_CALL(bd_mock, get_erase_size(0)).WillOnce(Return(18));
    EXPECT_CALL(bd_mock, get_erase_value()).WillOnce(Return(19));
    EXPECT_CALL(bd_mock, get_type()).WillOnce(Return("mytype"));

    EXPECT_EQ(b.get_erase_size(), 17);
    EXPECT_EQ(b.get_erase_size(0), 18);
    EXPECT_EQ(b.get_erase_value(), 19);
    EXPECT_EQ(b.get_program_size(), 1);
    EXPECT_EQ(b.get_read_size(), 1);
    EXPECT_EQ(b.size(), DEVICE_SIZE);
    EXPECT_EQ(b.get_type(), "mytype");

    EXPECT_CALL(bd_mock, deinit()); // Called in b's destructor
    EXPECT_CALL(bd_mock, sync()); // Called on b's deinit
}

TEST_F(BufferedBlockModuleTest, read_full_block)
{
    EXPECT_CALL(bd_mock, is_valid_read(0, BLOCK_SIZE))
    .WillRepeatedly(Return(true));

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_EQ(0, bd.read(buf, 0, BLOCK_SIZE));
    EXPECT_EQ(0, memcmp(magic, buf, BLOCK_SIZE));
}

TEST_F(BufferedBlockModuleTest, over_read)
{
    EXPECT_CALL(bd_mock, is_valid_read(DEVICE_SIZE - BLOCK_SIZE, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(true));
    EXPECT_CALL(bd_mock, read(_, DEVICE_SIZE - BLOCK_SIZE, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_EQ(bd.read(buf, DEVICE_SIZE - BLOCK_SIZE, BLOCK_SIZE), 0);

    // This will return before mock's is_valid_read()
    EXPECT_EQ(bd.read(buf, DEVICE_SIZE, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
}

TEST_F(BufferedBlockModuleTest, unalign_read)
{
    EXPECT_CALL(bd_mock, is_valid_read(BLOCK_SIZE/2, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(bd_mock, read(_, BLOCK_SIZE/2, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE/2), Return(BD_ERROR_OK)));

    EXPECT_EQ(bd.read(buf, BLOCK_SIZE/2, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic+(BLOCK_SIZE/2), BLOCK_SIZE/2));
}

TEST_F(BufferedBlockModuleTest, unalign_erase)
{
    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_EQ(bd.program("a", BLOCK_SIZE/2, 1), 0);

    ON_CALL(bd_mock, get_erase_size(_)).WillByDefault(Return(BLOCK_SIZE));

    // Partial erase is not supported
    EXPECT_EQ(bd.erase(BLOCK_SIZE/2, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), BD_ERROR_OK);
}

TEST_F(BufferedBlockModuleTest, align_big_read)
{
    uint8_t *buffer = new uint8_t[BLOCK_SIZE*2];

    EXPECT_CALL(bd_mock, is_valid_read(0, (BLOCK_SIZE*2)-1))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE*2-1))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE*2-1), Return(BD_ERROR_OK)));

    // Should cause 1 full block to be read unaligned from the device
    // second block would require buffering, because it is not a full (-1)
    EXPECT_EQ(bd.read(buffer, 0, (BLOCK_SIZE*2)-1), 0);
    EXPECT_EQ(0, memcmp(magic, buffer, BLOCK_SIZE));
    EXPECT_EQ(0, memcmp(magic+BLOCK_SIZE, buffer+BLOCK_SIZE, BLOCK_SIZE-1));
    delete[] buffer;
}

TEST_F(BufferedBlockModuleTest, program_small_chunks)
{
    EXPECT_CALL(bd_mock, is_valid_read(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    for (int i=0; i < BLOCK_SIZE - 1; ++i) {
        EXPECT_EQ(bd.program(magic+i, i, 1), 0);
    }
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE - 1));

    // write cache will be flushed on deinit.
    EXPECT_CALL(bd_mock, program(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));
}

TEST_F(BufferedBlockModuleTest, program_and_read_from_cache)
{
    EXPECT_CALL(bd_mock, program(_, 0, BLOCK_SIZE))
    .Times(2) // Once on 1st program and once on deinit
    .WillRepeatedly(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, is_valid_read(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, sync()); // Triggered from program()

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(bd.program("a", 0, 1), 0);
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ('a', buf[0]);
    EXPECT_EQ(0, memcmp(buf+1, magic+1, BLOCK_SIZE-1));
}

TEST_F(BufferedBlockModuleTest, program_and_read_from_device)
{
    EXPECT_CALL(bd_mock, program(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillRepeatedly(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, program(_, BLOCK_SIZE, BLOCK_SIZE))
    .Times(1)
    .WillRepeatedly(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, read(_, BLOCK_SIZE, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, is_valid_read(BLOCK_SIZE*3, BLOCK_SIZE-1)).WillOnce(Return(false));

    EXPECT_CALL(bd_mock, read(_, BLOCK_SIZE*3, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE-1), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, sync()).Times(1); // Triggered from program()s

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(bd.program(magic, BLOCK_SIZE, BLOCK_SIZE-1), 0);
    EXPECT_EQ(bd.read(buf, BLOCK_SIZE*3, BLOCK_SIZE-1), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE-1));

    EXPECT_CALL(bd_mock, is_valid_read(BLOCK_SIZE*3, BLOCK_SIZE+1)).WillOnce(Return(false));

    EXPECT_CALL(bd_mock, read(_, BLOCK_SIZE*3, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)))
    .RetiresOnSaturation();

    EXPECT_CALL(bd_mock, read(_, BLOCK_SIZE*4, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic+BLOCK_SIZE, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_EQ(bd.read(buf, BLOCK_SIZE*3, BLOCK_SIZE+1), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE+1));
}

TEST_F(BufferedBlockModuleTest, program_and_verify_from_storage)
{
    EXPECT_CALL(bd_mock, program(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, is_valid_read(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(2)
    .WillRepeatedly(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, sync()); // Triggered from program()

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE/2), 0);
    EXPECT_EQ(bd.program(magic+BLOCK_SIZE/2, BLOCK_SIZE/2, BLOCK_SIZE/2), 0); // This should actually write to device
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));
}

TEST_F(BufferedBlockModuleTest, flush_automatically)
{
    // Validate cache
    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillRepeatedly(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE/2), 0); // Don't write full block

    // Validate cache for the second program
    EXPECT_CALL(bd_mock, read(_, BLOCK_SIZE, BLOCK_SIZE))
    .Times(1)
    .WillRepeatedly(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, program(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.program(magic, BLOCK_SIZE, BLOCK_SIZE/2), 0); // This should cause flush() for previous data in cache

    EXPECT_CALL(bd_mock, program(_, BLOCK_SIZE, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));
}
