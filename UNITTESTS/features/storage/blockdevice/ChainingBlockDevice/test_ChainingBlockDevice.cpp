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
#include "features/storage/blockdevice/ChainingBlockDevice.cpp"
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
#define SECTORS_NUM (8)
#define DEVICE_SIZE (BLOCK_SIZE * SECTORS_NUM)

class ChainingBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock1;
    BlockDeviceMock bd_mock2;

    BlockDevice *bds[2] = {&bd_mock1, &bd_mock2};
    ChainingBlockDevice bd{bds, 2};
    uint8_t *magic;
    uint8_t *buf;
    virtual void SetUp()
    {
        ON_CALL(bd_mock1, size()).WillByDefault(Return((SECTORS_NUM / 2)*BLOCK_SIZE));
        ON_CALL(bd_mock2, size()).WillByDefault(Return((SECTORS_NUM    )*BLOCK_SIZE));

        ON_CALL(bd_mock1, get_erase_size(_)).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock2, get_erase_size(_)).WillByDefault(Return(2*BLOCK_SIZE));

        ON_CALL(bd_mock1, get_erase_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock2, get_erase_size()).WillByDefault(Return(2*BLOCK_SIZE));

        EXPECT_CALL(bd_mock1, init());
        EXPECT_CALL(bd_mock2, init());
        // Init performs checks on the parameters of underlying BDs
        // the BDs used for this test have different parameters:
        // bd_mock1 has erase/program/read size of   BLOCK_SIZE and total (SECTORS_NUM / 2)*BLOCK_SIZE size
        // bd_mock1 has erase/program/read size of 2*BLOCK_SIZE and total (SECTORS_NUM    )*BLOCK_SIZE size
        EXPECT_CALL(bd_mock1, get_erase_value()).WillOnce(Return(0));
        EXPECT_CALL(bd_mock1, get_read_size()).WillOnce(Return(BLOCK_SIZE));
        EXPECT_CALL(bd_mock1, get_program_size()).WillOnce(Return(BLOCK_SIZE));

        EXPECT_CALL(bd_mock2, get_erase_value()).WillOnce(Return(0));
        EXPECT_CALL(bd_mock2, get_read_size()).WillOnce(Return(BLOCK_SIZE * 2));
        EXPECT_CALL(bd_mock2, get_program_size()).WillOnce(Return(BLOCK_SIZE * 2));

        ASSERT_EQ(bd.init(), 0);
        magic = new uint8_t[BLOCK_SIZE * 4];
        buf = new uint8_t[BLOCK_SIZE * 4];
        // Generate simple pattern to verify against
        for (int i = 0; i < BLOCK_SIZE * 4; i++) {
            magic[i] = 0xaa + i;
        }
    }

    virtual void TearDown()
    {
        EXPECT_CALL(bd_mock1, deinit());
        EXPECT_CALL(bd_mock2, deinit());
        ASSERT_EQ(bd.deinit(), 0);
        delete[] magic;
        delete[] buf;
    }
};

TEST_F(ChainingBlockModuleTest, init)
{
    ChainingBlockDevice b(this->bds, 2);
    EXPECT_EQ(b.get_erase_size(), 0);
    EXPECT_EQ(b.get_erase_size(0), 0);
    EXPECT_EQ(b.get_erase_value(), -1);
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(b.program(magic, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.read(buf, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
//    EXPECT_EQ(b.deinit(), BD_ERROR_OK);
    EXPECT_EQ(b.sync(), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.get_type(), "CHAINING");

    EXPECT_CALL(bd_mock1, init());
    EXPECT_CALL(bd_mock1, size()).WillOnce(Return((SECTORS_NUM / 2)*BLOCK_SIZE));
    EXPECT_CALL(bd_mock1, get_erase_size()).WillOnce(Return(BLOCK_SIZE));
    EXPECT_CALL(bd_mock1, get_read_size()).WillOnce(Return(BLOCK_SIZE));
    EXPECT_CALL(bd_mock1, get_program_size()).WillOnce(Return(BLOCK_SIZE));
    EXPECT_CALL(bd_mock1, get_erase_value()).WillOnce(Return(2));
    EXPECT_CALL(bd_mock2, init());
    EXPECT_CALL(bd_mock2, size()).WillOnce(Return((SECTORS_NUM    )*BLOCK_SIZE));
    EXPECT_CALL(bd_mock2, get_erase_size()).WillOnce(Return(BLOCK_SIZE * 2));
    EXPECT_CALL(bd_mock2, get_read_size()).WillOnce(Return(BLOCK_SIZE * 2));
    EXPECT_CALL(bd_mock2, get_program_size()).WillOnce(Return(BLOCK_SIZE * 2));
    EXPECT_CALL(bd_mock2, get_erase_value()).WillOnce(Return(3));

    EXPECT_EQ(b.init(), 0);

    EXPECT_EQ(b.get_program_size(), BLOCK_SIZE * 2);
    EXPECT_EQ(b.get_read_size(), BLOCK_SIZE * 2);
    EXPECT_EQ(b.get_erase_size(), BLOCK_SIZE * 2);
    EXPECT_EQ(b.get_erase_value(), -1);
}

TEST_F(ChainingBlockModuleTest, memory_overlap)
{
    // program
    // Half of the data should go into bd_mock1
    EXPECT_CALL(bd_mock1, program(ByteBufferMatcher(magic, BLOCK_SIZE*2), (SECTORS_NUM / 2 - 2) * BLOCK_SIZE, 2 * BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    // Another half should go to bd_mock2
    EXPECT_CALL(bd_mock2, program(ByteBufferMatcher(magic+BLOCK_SIZE*2, BLOCK_SIZE*2), 0, 2 * BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.program(magic, (SECTORS_NUM / 2 - 2) * BLOCK_SIZE, 4 * BLOCK_SIZE), BD_ERROR_OK);


    // read
    // On read half of the data will be read out from bd_mock1
    EXPECT_CALL(bd_mock1, read(_, (SECTORS_NUM / 2 - 2) * BLOCK_SIZE, 2 * BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, 2*BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock2, read(_, 0, 2 * BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic+2*BLOCK_SIZE, 2*BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_EQ(bd.read(buf, (SECTORS_NUM / 2 - 2) * BLOCK_SIZE, 4 * BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(memcmp(magic, buf, BLOCK_SIZE * 4), 0);


    // erase
    EXPECT_CALL(bd_mock1, erase((SECTORS_NUM / 2 - 2) * BLOCK_SIZE, 2 * BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock2, erase(0, 2 * BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.erase((SECTORS_NUM / 2 - 2) * BLOCK_SIZE, 4 * BLOCK_SIZE), BD_ERROR_OK);
}

