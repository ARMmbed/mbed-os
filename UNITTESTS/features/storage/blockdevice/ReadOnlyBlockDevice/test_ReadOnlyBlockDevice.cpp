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
#include "stubs/BlockDevice_mock.h"
#include "features/storage/blockdevice/ReadOnlyBlockDevice.h"
#include "platform/mbed_error.h"

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;

class ReadOnlyBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock;
    ReadOnlyBlockDevice bd{&bd_mock};
    uint8_t *magic;
    uint8_t *buf;
    virtual void SetUp()
    {
        ON_CALL(bd_mock, size()).WillByDefault(Return(DEVICE_SIZE));
        ON_CALL(bd_mock, get_erase_size(_)).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_erase_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_program_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_read_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, is_valid_erase(_, _)).WillByDefault(Return(true));
        ON_CALL(bd_mock, init()).WillByDefault(Return(BD_ERROR_OK));

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

TEST_F(ReadOnlyBlockModuleTest, init)
{
    EXPECT_EQ(bd.get_erase_size(), bd_mock.get_erase_size());
    EXPECT_EQ(bd.get_erase_size(0), bd_mock.get_erase_size(0));
    EXPECT_EQ(bd.get_erase_value(), bd_mock.get_erase_value());
    EXPECT_EQ(bd.get_program_size(), bd_mock.get_program_size());
    EXPECT_EQ(bd.get_read_size(), bd_mock.get_read_size());
    EXPECT_EQ(bd.size(), bd_mock.size());
    EXPECT_EQ(bd.get_type(), bd_mock.get_type());
}

TEST_F(ReadOnlyBlockModuleTest, write_protection) {
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), MBED_ERROR_WRITE_PROTECTED);
    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), MBED_ERROR_WRITE_PROTECTED);
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(bd.sync(), 0); // Should not have any influence
}
