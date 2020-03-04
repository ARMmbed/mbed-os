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
#include "features/storage/blockdevice/MBRBlockDevice.h"
#include "stubs/BlockDevice_mock.h"

#define BLOCK_SIZE (512)
#define PART_DESC_SIZE (16)
#define PARTITIONS (4)
#define MBR_SIZE (PARTITIONS * PART_DESC_SIZE + 2)
#define SECTORS_NUM (20)
#define DEVICE_SIZE (BLOCK_SIZE * SECTORS_NUM)

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;

class MBRBlockModuleTest : public testing::Test {
protected:
    BlockDeviceMock bd_mock;
    MBRBlockDevice bd{&bd_mock, PARTITIONS};
    uint8_t *magic;
    uint8_t *magic2;
    uint8_t *buf;
    uint8_t MBR[MBR_SIZE] = {
            // mba_entry 0
            0x00,                   // status
            0x00, 0x02, 0x00,       // chs_start
            0x0C,                   // type (FAT32)
            0x00, 0x05, 0x00,       // chs_stop
            0x01, 0x00, 0x00, 0x00, // lba_offset
            SECTORS_NUM/4-1, 0x00, 0x00, 0x00, // lba_size
            // mba_entry 1
            0x00,                   // status
            0x00, 0x06, 0x00,       // chs_start
            0x0C,                   // type
            0x00, 0x0A, 0x00,       // chs_stop
            SECTORS_NUM/4, 0x00, 0x00, 0x00, // lba_offset
            SECTORS_NUM/4, 0x00, 0x00, 0x00, // lba_size
            // mba_entry 2
            0x00,                   // status
            0x00, 0x0B, 0x00,       // chs_start
            0x83,                   // type (Linux)
            0x00, 0x0F, 0x00,       // chs_stop
            2*SECTORS_NUM/4, 0x00, 0x00, 0x00, // lba_offset
            SECTORS_NUM/4, 0x00, 0x00, 0x00, // lba_size
            // mba_entry 3
            0x00,                   // status
            0x00, 0x10, 0x00,       // chs_start
            0x82,                   // type (Linux swap)
            0x00, 0x14, 0x00,       // chs_stop
            3*SECTORS_NUM/4, 0x00, 0x00, 0x00, // lba_offset
            SECTORS_NUM/4, 0x00, 0x00, 0x00, // lba_size (in sectors)
            // signature
            0x55,                   // signature 0
            0xaa                    // signature 1
    };
    uint8_t MBRbuf[BLOCK_SIZE];
    virtual void SetUp()
    {
        memset(MBRbuf, 0, BLOCK_SIZE);
        memcpy(MBRbuf + (BLOCK_SIZE - MBR_SIZE), MBR, MBR_SIZE);

        ON_CALL(bd_mock, size()).WillByDefault(Return(DEVICE_SIZE));
        ON_CALL(bd_mock, get_erase_size(_)).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_erase_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_program_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, get_read_size()).WillByDefault(Return(BLOCK_SIZE));
        ON_CALL(bd_mock, is_valid_erase(_, _)).WillByDefault(Return(true));
        ON_CALL(bd_mock, init()).WillByDefault(Return(BD_ERROR_OK));

        EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
        .Times(1)
        .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf, BLOCK_SIZE), Return(BD_ERROR_OK)));

        magic = new uint8_t[BLOCK_SIZE];
        magic2 = new uint8_t[BLOCK_SIZE];
        buf = new uint8_t[BLOCK_SIZE];

        ASSERT_EQ(bd.init(), BD_ERROR_OK);
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

TEST_F(MBRBlockModuleTest, init)
{
    MBRBlockDevice b(&bd_mock, PARTITIONS);
    EXPECT_EQ(b.get_erase_size(), 0);
    EXPECT_EQ(b.get_erase_size(0), 0);
    EXPECT_EQ(b.get_erase_value(), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.get_read_size(), 0);
    EXPECT_EQ(b.get_program_size(), 0);
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(b.erase(0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.program(magic, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.read(buf, 0, BLOCK_SIZE), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(b.deinit(), BD_ERROR_OK);
    EXPECT_EQ(b.sync(), BD_ERROR_DEVICE_ERROR);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, init());
    EXPECT_EQ(b.init(), 0);

    EXPECT_CALL(bd_mock, get_type()).WillOnce(Return("mytype"));
    EXPECT_CALL(bd_mock, deinit());

    EXPECT_EQ(b.get_erase_size(), bd_mock.get_erase_size());
    EXPECT_EQ(b.get_erase_size(0), bd_mock.get_erase_size(0));
    EXPECT_EQ(b.get_erase_value(), bd_mock.get_erase_value());
    EXPECT_EQ(b.get_program_size(), 512);
    EXPECT_EQ(b.get_read_size(), 512);
    EXPECT_EQ(b.get_partition_number(), PARTITIONS);
    EXPECT_EQ(b.get_partition_start(), 3*DEVICE_SIZE/4);
    EXPECT_EQ(b.get_partition_stop(), DEVICE_SIZE);
    EXPECT_EQ(b.get_partition_type(), 0x82);
    EXPECT_EQ(b.size(), DEVICE_SIZE/4);
    EXPECT_EQ(b.get_type(), "mytype");
    EXPECT_EQ(b.deinit(), 0);
}

TEST_F(MBRBlockModuleTest, init_incorrect_signature)
{
    MBRbuf[BLOCK_SIZE - MBR_SIZE + 64] = 0x00;

    EXPECT_CALL(bd_mock, init());
    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf, BLOCK_SIZE), Return(BD_ERROR_OK)));

    MBRBlockDevice b(&bd_mock, PARTITIONS);
    EXPECT_EQ(b.init(), mbed::BD_ERROR_INVALID_MBR);
    MBRbuf[BLOCK_SIZE - MBR_SIZE + 64] = 0xaa;
}

TEST_F(MBRBlockModuleTest, init_invalid_type)
{
    MBRbuf[BLOCK_SIZE - MBR_SIZE + 4] = 0x05; // 0x05 is extended partition - not supported

    EXPECT_CALL(bd_mock, init());
    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf, BLOCK_SIZE), Return(BD_ERROR_OK)));

    MBRBlockDevice b(&bd_mock, 1);
    EXPECT_EQ(b.init(), mbed::BD_ERROR_INVALID_PARTITION);
    MBRbuf[BLOCK_SIZE - MBR_SIZE + 4] = 0x0C;
}

TEST_F(MBRBlockModuleTest, init_invalid_status)
{
    MBRbuf[BLOCK_SIZE - MBR_SIZE] = 0x01;

    EXPECT_CALL(bd_mock, init());
    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf, BLOCK_SIZE), Return(BD_ERROR_OK)));

    MBRBlockDevice b(&bd_mock, 1);
    EXPECT_EQ(b.init(), mbed::BD_ERROR_INVALID_PARTITION);
    MBRbuf[BLOCK_SIZE - MBR_SIZE] = 0x00;
}

TEST_F(MBRBlockModuleTest, program_unaligned)
{
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
}

TEST_F(MBRBlockModuleTest, program_read_erase)
{
    constexpr uint32_t partition_offset = BLOCK_SIZE * 3 * SECTORS_NUM / PARTITIONS;
    //Unalinged addressing is not allowed
    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(bd.erase(0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);
    EXPECT_EQ(bd.read(magic, 0, BLOCK_SIZE-1), BD_ERROR_DEVICE_ERROR);

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(magic, BLOCK_SIZE), partition_offset, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, read(_, partition_offset, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(magic, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, erase(partition_offset, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(bd.program(magic, 0, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(bd.read(buf, 0, BLOCK_SIZE), BD_ERROR_OK);
    EXPECT_EQ(0, memcmp(buf, magic, BLOCK_SIZE));
    EXPECT_EQ(bd.erase(0, BLOCK_SIZE), BD_ERROR_OK);
}

TEST_F(MBRBlockModuleTest, partitioning)
{
    uint8_t MBRbuf2[BLOCK_SIZE];
    memset(MBRbuf2, 0xFF, BLOCK_SIZE);
    memset(MBRbuf2 + BLOCK_SIZE - MBR_SIZE, 0x0, MBR_SIZE);


    // Partition 1
    memcpy(MBRbuf2 + BLOCK_SIZE - MBR_SIZE, MBRbuf + BLOCK_SIZE - MBR_SIZE, PART_DESC_SIZE);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf2, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    MBRbuf2[BLOCK_SIZE - 2] = MBRbuf[BLOCK_SIZE - 2]; //0x55
    MBRbuf2[BLOCK_SIZE - 1] = MBRbuf[BLOCK_SIZE - 1]; //0xaa

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(MBRbuf2, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(MBRBlockDevice::partition(&bd_mock, 1, 0x0C, 0,
            (SECTORS_NUM / 4) * BLOCK_SIZE), 0);


    // Partition 2
    memcpy(MBRbuf2 + BLOCK_SIZE - MBR_SIZE + PART_DESC_SIZE,
           MBRbuf + BLOCK_SIZE - MBR_SIZE + PART_DESC_SIZE, PART_DESC_SIZE);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf2, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(MBRbuf2, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(MBRBlockDevice::partition(&bd_mock, 2, 0x0C, (SECTORS_NUM / 4) * BLOCK_SIZE,
            (SECTORS_NUM / 4) * BLOCK_SIZE * 2), 0);


    // Partition 3
    memcpy(MBRbuf2 + BLOCK_SIZE - MBR_SIZE + 2 * PART_DESC_SIZE,
           MBRbuf + BLOCK_SIZE - MBR_SIZE + 2 * PART_DESC_SIZE, PART_DESC_SIZE);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf2, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(MBRbuf2, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(MBRBlockDevice::partition(&bd_mock, 3, 0x83, (SECTORS_NUM / 4) * BLOCK_SIZE * 2,
            (SECTORS_NUM / 4) * BLOCK_SIZE * 3), 0);

    // Partition 4
    memcpy(MBRbuf2 + BLOCK_SIZE - MBR_SIZE + 3 * PART_DESC_SIZE,
           MBRbuf + BLOCK_SIZE - MBR_SIZE + 3 * PART_DESC_SIZE, PART_DESC_SIZE);

    EXPECT_CALL(bd_mock, read(_, 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(DoAll(SetArg0ToCharPtr(MBRbuf2, BLOCK_SIZE), Return(BD_ERROR_OK)));

    EXPECT_CALL(bd_mock, erase(0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_CALL(bd_mock, program(ByteBufferMatcher(MBRbuf2, BLOCK_SIZE), 0, BLOCK_SIZE))
    .Times(1)
    .WillOnce(Return(BD_ERROR_OK));

    EXPECT_EQ(MBRBlockDevice::partition(&bd_mock, 4, 0x82, -(SECTORS_NUM / 4) * BLOCK_SIZE), 0);
}
