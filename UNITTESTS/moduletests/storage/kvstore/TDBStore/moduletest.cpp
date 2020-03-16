/* Copyright (c) 2020 ARM Limited
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

#define BLOCK_SIZE (512)
#define DEVICE_SIZE (BLOCK_SIZE*200)

using namespace mbed;

class TDBStoreModuleTest : public testing::Test {
protected:
    HeapBlockDevice heap{DEVICE_SIZE};
    FlashSimBlockDevice flash{&heap};
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

TEST_F(TDBStoreModuleTest, erased_set_get)
{
    EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
    EXPECT_EQ(flash.init(), MBED_SUCCESS);
    EXPECT_EQ(flash.erase(0, flash.size()), MBED_SUCCESS);
    EXPECT_EQ(flash.deinit(), MBED_SUCCESS);
    EXPECT_EQ(tdb.init(), MBED_SUCCESS);
    char buf[100];
    size_t size;
    EXPECT_EQ(tdb.set("key", "data", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(tdb.get("key", buf, 100, &size), MBED_SUCCESS);
    EXPECT_EQ(size, 5);
    EXPECT_STREQ("data", buf);
}

TEST_F(TDBStoreModuleTest, set_deinit_init_get)
{
    char buf[100];
    size_t size;
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(tdb.set("key", "data", 5, 0), MBED_SUCCESS);
        EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
        EXPECT_EQ(tdb.init(), MBED_SUCCESS);
        EXPECT_EQ(tdb.get("key", buf, 100, &size), MBED_SUCCESS);
        EXPECT_EQ(size, 5);
        EXPECT_STREQ("data", buf);
        EXPECT_EQ(tdb.remove("key"), MBED_SUCCESS);
    }
}

TEST_F(TDBStoreModuleTest, corrupted_set_deinit_init_get)
{
    char buf[100];
    char *block = new char[BLOCK_SIZE];
    size_t size;
    EXPECT_EQ(heap.init(), MBED_SUCCESS); // Extra init, so the heap will not be deinitialized

    srand(0); // Prefer to have always the same pattern

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
        // Corrupt the first part of the storage
        for (int j = 0; j < heap.size()/BLOCK_SIZE/2; j++) {
            for (int k = 0; k < BLOCK_SIZE; k++) {
                block[k] = rand();
            }
            EXPECT_EQ(heap.program(block, BLOCK_SIZE * j, BLOCK_SIZE), MBED_SUCCESS);
        }
        EXPECT_EQ(tdb.init(), MBED_SUCCESS);
        for (int j = 0; j < 100; ++j) {
            // Use random data, so the data has to be updated
            EXPECT_EQ(tdb.set("key", block+j, 50, 0), MBED_SUCCESS);
            EXPECT_EQ(tdb.deinit(), MBED_SUCCESS);
            EXPECT_EQ(tdb.init(), MBED_SUCCESS);
            EXPECT_EQ(tdb.get("key", buf, 100, &size), MBED_SUCCESS);
            EXPECT_EQ(size, 50);
            EXPECT_EQ(0, memcmp(buf, block+j, size));
        }
        EXPECT_EQ(tdb.remove("key"), MBED_SUCCESS);
    }

    EXPECT_EQ(heap.deinit(), MBED_SUCCESS);
    delete[] block;
}

TEST_F(TDBStoreModuleTest, set_multiple_iterate)
{
    char buf[100];
    KVStore::iterator_t iterator;
    EXPECT_EQ(tdb.set("primary_key", "data", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(tdb.set("primary_second_key", "value", 6, 0), MBED_SUCCESS);
    EXPECT_EQ(tdb.iterator_open(&iterator, "primary"), MBED_SUCCESS);
    EXPECT_EQ(tdb.iterator_next(iterator, buf, 100), MBED_SUCCESS);
    EXPECT_EQ(tdb.iterator_next(iterator, buf, 100), MBED_SUCCESS);
    EXPECT_EQ(tdb.iterator_next(iterator, buf, 100), MBED_ERROR_ITEM_NOT_FOUND);
    EXPECT_EQ(tdb.iterator_close(iterator), MBED_SUCCESS);
}

TEST_F(TDBStoreModuleTest, reserved_data_set_get)
{
    char reserved_key[] = "value";
    char buf[64];
    size_t size;
    EXPECT_EQ(tdb.reserved_data_set(reserved_key, 6), MBED_SUCCESS);
    EXPECT_EQ(tdb.reserved_data_get(buf, 64, &size), MBED_SUCCESS);
    EXPECT_STREQ("value", buf);
    EXPECT_EQ(size, 6);
    EXPECT_EQ(tdb.reserved_data_set(reserved_key, 6), MBED_ERROR_WRITE_FAILED);
}
