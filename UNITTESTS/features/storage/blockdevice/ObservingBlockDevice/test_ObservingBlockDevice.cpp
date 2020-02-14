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
#include "features/storage/blockdevice/ObservingBlockDevice.h"
#include "features/storage/blockdevice/ReadOnlyBlockDevice.h"
#include "stubs/BlockDevice_mock.h"

using ::testing::_;
using ::testing::Return;

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)

class ObservingBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock;
    ObservingBlockDevice bd{&bd_mock};
    uint8_t *magic;
    static int cnt;
    virtual void SetUp()
    {
        cnt = 0;
        ASSERT_EQ(bd.init(), 0);
        magic = new uint8_t[BLOCK_SIZE];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE; i++) {
            magic[i] = 0xaa + i;
        }
    }

    virtual void TearDown()
    {
        ASSERT_EQ(bd.deinit(), 0);
        delete[] magic;
    }

    static void bd_change(BlockDevice* bd) {
        cnt++;
    }
};

int ObservingBlockModuleTest::cnt = 0;

TEST_F(ObservingBlockModuleTest, init)
{
    EXPECT_EQ(bd.get_erase_size(), bd_mock.get_erase_size());
    EXPECT_EQ(bd.get_erase_size(0), bd_mock.get_erase_size(0));
    EXPECT_EQ(bd.get_erase_value(), bd_mock.get_erase_value());
    EXPECT_EQ(bd.get_program_size(), bd_mock.get_program_size());
    EXPECT_EQ(bd.get_read_size(), bd_mock.get_read_size());
    EXPECT_EQ(bd.size(), bd_mock.size());
    EXPECT_EQ(bd.get_type(), bd_mock.get_type());
}

TEST_F(ObservingBlockModuleTest, program_erase_cb)
{
    EXPECT_CALL(bd_mock, program(magic, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    bd.attach(this->bd_change);
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(cnt, 1);

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), 0);
    EXPECT_EQ(cnt, 2);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    uint8_t buf[BLOCK_SIZE];
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), 0);
    EXPECT_EQ(cnt, 2); // Counter stays the same
}
