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
#include "features/storage/kvstore/filesystemstore/FileSystemStore.h"
#include "features/storage/filesystem/littlefs/LittleFileSystem.h"
#include "mbed_error.h"
#include <stdlib.h>

#define HEAPBLOCK_SIZE (4096)

using namespace mbed;

class FileSystemStoreModuleTest : public testing::Test {
protected:
    HeapBlockDevice heap{HEAPBLOCK_SIZE};
    LittleFileSystem *fs;
    FileSystemStore *store;

    virtual void SetUp()
    {
        fs = new LittleFileSystem("kvstore", &heap);
        if(fs->mount(&heap) != MBED_SUCCESS) {
            EXPECT_EQ(fs->reformat(&heap), MBED_SUCCESS);
        }
        store = new FileSystemStore(fs);
        EXPECT_EQ(store->init(), MBED_SUCCESS);
    }

    virtual void TearDown()
    {
        EXPECT_EQ(store->deinit(), MBED_SUCCESS);
        delete store;
        EXPECT_EQ(fs->unmount(), MBED_SUCCESS);
        delete fs;
    }
};

TEST_F(FileSystemStoreModuleTest, init)
{
    EXPECT_EQ(store->deinit(), MBED_SUCCESS);
    EXPECT_EQ(store->init(), MBED_SUCCESS);
    EXPECT_EQ(store->init(), MBED_SUCCESS);
}

TEST_F(FileSystemStoreModuleTest, set_get)
{
    char buf[100];
    size_t size;
    EXPECT_EQ(store->set("key", "data", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(store->get("key", buf, 100, &size), MBED_SUCCESS);
    EXPECT_EQ(size, 5);
    EXPECT_STREQ("data", buf);
}

TEST_F(FileSystemStoreModuleTest, erased_set_get)
{
    EXPECT_EQ(store->deinit(), MBED_SUCCESS);
    EXPECT_EQ(heap.init(), MBED_SUCCESS);
    EXPECT_EQ(heap.erase(0, heap.size()), MBED_SUCCESS);
    EXPECT_EQ(heap.deinit(), MBED_SUCCESS);
    EXPECT_EQ(store->init(), MBED_SUCCESS);
    char buf[100];
    size_t size;
    EXPECT_EQ(store->set("key", "data", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(store->get("key", buf, 100, &size), MBED_SUCCESS);
    EXPECT_EQ(size, 5);
    EXPECT_STREQ("data", buf);
}

TEST_F(FileSystemStoreModuleTest, set_deinit_init_get)
{
    char buf[100];
    size_t size;
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(store->set("key", "data", 5, 0), MBED_SUCCESS);
        EXPECT_EQ(store->deinit(), MBED_SUCCESS);
        EXPECT_EQ(store->init(), MBED_SUCCESS);
        EXPECT_EQ(store->get("key", buf, 100, &size), MBED_SUCCESS);
        EXPECT_EQ(size, 5);
        EXPECT_STREQ("data", buf);
        EXPECT_EQ(store->remove("key"), MBED_SUCCESS);
    }
}

TEST_F(FileSystemStoreModuleTest, set_multiple_iterate)
{
    char buf[100];
    KVStore::iterator_t iterator;
    EXPECT_EQ(store->set("primary_key", "data", 5, 0), MBED_SUCCESS);
    EXPECT_EQ(store->set("primary_second_key", "value", 6, 0), MBED_SUCCESS);
    EXPECT_EQ(store->iterator_open(&iterator, "primary"), MBED_SUCCESS);
    EXPECT_EQ(store->iterator_next(iterator, buf, 100), MBED_SUCCESS);
    EXPECT_EQ(store->iterator_next(iterator, buf, 100), MBED_SUCCESS);
    EXPECT_EQ(store->iterator_next(iterator, buf, 100), MBED_ERROR_ITEM_NOT_FOUND);
    EXPECT_EQ(store->iterator_close(iterator), MBED_SUCCESS);
}
