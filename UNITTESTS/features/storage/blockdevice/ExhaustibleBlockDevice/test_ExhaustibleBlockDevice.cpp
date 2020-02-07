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
#include "features/storage/blockdevice/ExhaustibleBlockDevice.h"
#include "stubs/BlockDevice_mock.h"

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*10)
#define ERASE_CYCLES (2)

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::DoAll;

class ExhaustibleBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock;
    ExhaustibleBlockDevice bd{&bd_mock, ERASE_CYCLES};
    uint8_t *magic;
    uint8_t *magic2;
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
        buf = new uint8_t[BLOCK_SIZE];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE; i++) {
            magic[i] = 0xaa + i;
            magic2[i] = 0xaa + i + 1;
        }
    }

    virtual void TearDown()
    {
        EXPECT_CALL(bd_mock, deinit());
        ASSERT_EQ(bd.deinit(), 0);
        delete[] magic;
        delete[] magic2;
        delete[] buf;
    }
};

TEST_F(ExhaustibleBlockModuleTest, init)
{
    ExhaustibleBlockDevice b(&bd_mock, ERASE_CYCLES);
    EXPECT_EQ(b.get_erase_size(), 0);
    EXPECT_EQ(b.get_erase_size(0), 0);
    EXPECT_EQ(b.get_erase_value(), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.get_read_size(), 0);
    EXPECT_EQ(b.get_program_size(), 0);
    EXPECT_EQ(b.size(), 0);
    b.set_erase_cycles(0, 100); // This should not take effect.
    EXPECT_EQ(b.get_erase_cycles(0), 0);
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
    EXPECT_EQ(b.get_erase_value(), bd_mock.get_erase_value());
    EXPECT_NE(b.get_erase_cycles(0), 100);
    EXPECT_EQ(b.get_program_size(), 512);
    EXPECT_EQ(b.get_read_size(), 512);
    EXPECT_EQ(b.size(), bd_mock.size());
    EXPECT_EQ(b.get_type(), "mytype");
    EXPECT_EQ(b.deinit(), 0);
}

TEST_F(ExhaustibleBlockModuleTest, program_unaligned)
{
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
}

TEST_F(ExhaustibleBlockModuleTest, erase_cycle_limit_reached)
{
    EXPECT_NE(bd.get_erase_cycles(0), 0);

    EXPECT_CALL(bd_mock, program(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillRepeatedly(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), 0);

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(ERASE_CYCLES)
    .WillRepeatedly(Return(BD_ERROR_OK));

    for (int i = 0; i < ERASE_CYCLES; i++) {
        EXPECT_EQ(bd.erase(0, BLOCK_SIZE), 0);
    }
    EXPECT_EQ(bd.get_erase_cycles(0), 0);

    // This calls are expect not to happen.
    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(bd.program(magic2, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
}

TEST_F(ExhaustibleBlockModuleTest, erase_invalid)
{
    ASSERT_GT(ERASE_CYCLES, 1);

    EXPECT_EQ(bd.get_erase_cycles(0), ERASE_CYCLES);

    // Unaligned erase should fail
    EXPECT_EQ(bd.erase(0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
    // Number of erase cycles should not change
    EXPECT_EQ(bd.get_erase_cycles(0), ERASE_CYCLES);
}

TEST_F(ExhaustibleBlockModuleTest, set_erase_cycles)
{
    EXPECT_EQ(bd.get_erase_cycles(0), ERASE_CYCLES);
    bd.set_erase_cycles(0, ERASE_CYCLES+1);
    EXPECT_EQ(bd.get_erase_cycles(0), ERASE_CYCLES+1);
}
