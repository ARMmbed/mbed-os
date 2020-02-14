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
#include "features/storage/blockdevice/FlashSimBlockDevice.h"
#include "stubs/BlockDevice_mock.h"

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)
#define ERASE_VALUE (2)

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::DoAll;

class FlashSimBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock;
    FlashSimBlockDevice bd{&bd_mock, ERASE_VALUE};
    uint8_t *magic;
    uint8_t *magic2;
    uint8_t *erased_mem;
    uint8_t *buf;
    virtual void SetUp()
    {
        ON_CALL(bd_mock, size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_erase_size(_)).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_erase_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_program_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_read_size()).WillByDefault(Return(BLOCK_SIZE));

        EXPECT_CALL(bd_mock, init());
        ASSERT_EQ(bd.init(), 0);
        magic = new uint8_t[BLOCK_SIZE];
        magic2 = new uint8_t[BLOCK_SIZE];
        erased_mem = new uint8_t[BLOCK_SIZE];
        buf = new uint8_t[BLOCK_SIZE];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE; i++) {
            magic[i] = 0xaa + i;
            magic2[i] = 0xaa + i + 1;
            erased_mem[i] = ERASE_VALUE;
        }
    }

    virtual void TearDown()
    {
        EXPECT_CALL(bd_mock, deinit());
        ASSERT_EQ(bd.deinit(), 0);
        delete[] magic;
        delete[] magic2;
        delete[] erased_mem;
        delete[] buf;
    }
};

TEST_F(FlashSimBlockModuleTest, init)
{
    FlashSimBlockDevice b(&bd_mock, ERASE_VALUE);
    EXPECT_EQ(b.get_erase_size(), 0);
    EXPECT_EQ(b.get_erase_size(0), 0);
    EXPECT_EQ(b.get_read_size(), 0);
    EXPECT_EQ(b.get_program_size(), 0);
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(b.erase(0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.program(magic, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.read(buf, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.deinit(), BD_ERROR_OK);
    EXPECT_EQ(b.sync(), BD_ERROR_DEVICE_ERROR);

    EXPECT_CALL(bd_mock, init());
    EXPECT_EQ(b.init(), 0);

    EXPECT_CALL(bd_mock, get_type()).WillOnce(Return("mytype"));
    EXPECT_CALL(bd_mock, deinit());

    EXPECT_EQ(b.get_erase_size(), bd_mock.get_erase_size());
    EXPECT_EQ(b.get_erase_size(0), bd_mock.get_erase_size(0));
    EXPECT_EQ(b.get_erase_value(), ERASE_VALUE);
    EXPECT_EQ(b.get_program_size(), 512);
    EXPECT_EQ(b.get_read_size(), 512);
    EXPECT_EQ(b.size(), bd_mock.size());
    EXPECT_EQ(b.get_type(), "mytype");
    EXPECT_EQ(b.sync(), 0);
    EXPECT_EQ(b.deinit(), 0);

}

TEST_F(FlashSimBlockModuleTest, program_unaligned)
{
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
}

TEST_F(FlashSimBlockModuleTest, program_no_erase)
{
    // Data has not been erased, so this fails
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), mbed::BD_ERROR_NOT_ERASED);

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(erased_mem, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), 0);

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(magic, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    // No program() call should happen for this call.
    EXPECT_EQ(bd.program(magic2, 0, BLOCK_SIZE), mbed::BD_ERROR_NOT_ERASED);
}

TEST_F(FlashSimBlockModuleTest, reprogram)
{
    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(erased_mem, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), 0);

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(magic, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(2)
    .WillRepeatedly(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);

    // Programming the same data.
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(erased_mem, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), 0);
}

TEST_F(FlashSimBlockModuleTest, erase_invalid)
{
    // Unaligned erase should fail
    EXPECT_EQ(bd.erase(0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
}
