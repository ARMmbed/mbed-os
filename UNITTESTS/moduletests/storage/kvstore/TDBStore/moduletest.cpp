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
#include "features/storage/blockdevice/FlashSimBlockDevice.h"
#include "features/storage/kvstore/tdbstore/TDBStore.h"
#include <stdlib.h>
#include "stubs/EmulatedSD.h"

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*20)

using namespace mbed;
// For testing purposes, block device should keep its data between test runs
// to ensure that we can cope with dirty-images
HeapBlockDevice sd{DEVICE_SIZE};

// For debugging purpose, we could use file to store the BlockDevice content
//EmulatedSD sd{"../UNITTESTS/moduletests/storage/kvstore/TDBStore/broken_image.img"};

class TDBStoreModuleTest : public testing::Test {
protected:
    FlashSimBlockDevice flash{&sd};
    TDBStore tdb{&flash};

    virtual void SetUp()
    {
        EXPECT_EQ(tdb.init(), MBED_SUCCESS);
        EXPECT_EQ(tdb.reset(), MBED_SUCCESS);
    }

    virtual void TearDown()
    {
        EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
    }
};

TEST_F(TDBStoreModuleTest, init)
{
    EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
    EXPECT_EQ(tdb.init(), MBED_SUCCESS);
    EXPECT_EQ(tdb.init(), MBED_SUCCESS);
}

TEST_F(TDBStoreModuleTest, set_get)
{
    char buf[100];
    size_t size;
    EXPECT_EQ(tdb.set("key", "data", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(tdb.get("key", buf, 100, &size), MBED_SUCCESS);
    EXPECT_EQ(size, 5);
    EXPECT_STREQ("data", buf);
}

TEST_F(TDBStoreModuleTest, reset)
{
    char buf[100];
    size_t size, read;
    // Write so much, that we are sure that garbage collection have kicked up
    for (int i = 0; i < 100; ++i) {
        size = sprintf(buf, "reset_%d", i);
        EXPECT_EQ(tdb.set("key", buf, size, 0), MBED_SUCCESS);
    }
    EXPECT_EQ(tdb.reset(), MBED_SUCCESS);
    EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
    EXPECT_EQ(tdb.init(), MBED_SUCCESS);
    // After reset->deinit()->init() there should be no data
    EXPECT_NE(tdb.get("key", buf, 100, &read), MBED_SUCCESS);
}

TEST_F(TDBStoreModuleTest, remove)
{
    char buf[100];
    size_t size;
    EXPECT_EQ(tdb.set("key", "data1", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(tdb.set("key", "data2", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(tdb.remove("key"), MBED_SUCCESS);
    // Previous key should not be found
    EXPECT_NE(tdb.get("key", buf, 100, &size), MBED_SUCCESS);
}

TEST_F(TDBStoreModuleTest, set_deinit_init_get)
{
    char buf[100];
    size_t size;
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(tdb.set("key", "data", 5, 0), MBED_SUCCESS);
        EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
        EXPECT_EQ(tdb.init(), MBED_SUCCESS);
        EXPECT_EQ(tdb.get("key", buf, 100, &size), MBED_SUCCESS);
        EXPECT_EQ(size, 5);
        EXPECT_STREQ("data", buf);
        EXPECT_EQ(tdb.remove("key"), MBED_SUCCESS);
    }
}

TEST_F(TDBStoreModuleTest, reserved_data)
{
    char data[20];
    char buf[20];
    for (int i = 0; i < sizeof(buf); ++i) {
        data[i] = rand();
    }
    EXPECT_EQ(tdb.reserved_data_set(data, sizeof(data)), MBED_SUCCESS);
    EXPECT_EQ(tdb.reserved_data_get(buf, sizeof(buf)), MBED_SUCCESS);
    EXPECT_EQ(0, memcmp(data, buf, sizeof(buf)));
    EXPECT_EQ(tdb.reserved_data_set(data, 1024), MBED_ERROR_INVALID_SIZE);
    EXPECT_EQ(tdb.reserved_data_set(data, sizeof(data)), MBED_ERROR_WRITE_FAILED);
    EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
    EXPECT_EQ(tdb.init(), MBED_SUCCESS);
    EXPECT_EQ(tdb.reserved_data_get(buf, sizeof(buf)), MBED_SUCCESS);
    EXPECT_EQ(0, memcmp(data, buf, sizeof(buf)));
}
