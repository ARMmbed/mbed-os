/*
 * Copyright (c) 2015, 2017, Arm Limited and affiliates.
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
#include "CppUTest/TestHarness.h"
#include "test_thread_nvm_store.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "ns_file_system.h"
#include "nsdynmemLIB_stub.h"

TEST_GROUP(thread_nvm_store)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};



TEST(thread_nvm_store, test_thread_nvm_store_init)
{
    nsdynmemlib_stub.returnCounter = 2;
    CHECK(test_thread_nvm_store_init());
}

TEST(thread_nvm_store, test_thread_nvm_store_init_failed)
{
    CHECK(test_thread_nvm_store_init_failed());
}

TEST(thread_nvm_store, test_thread_nvm_store_init_failed_read)
{
    nsdynmemlib_stub.returnCounter = 2;
    CHECK(test_thread_nvm_store_init_failed_read());
}

TEST(thread_nvm_store, test_thread_nvm_store_init_failed_write)
{
    CHECK(test_thread_nvm_store_init_failed_write());
}

TEST(thread_nvm_store, test_thread_nvm_store_seq_counter)
{
    nsdynmemlib_stub.returnCounter = 2;
    CHECK(test_thread_nvm_store_seq_counter());
}

TEST(thread_nvm_store, test_thread_nvm_store_mle_frame_counter)
{
    nsdynmemlib_stub.returnCounter = 2;
    CHECK(test_thread_nvm_store_mle_frame_counter());
    free(nsdynmemlib_stub.expectedPointer);
}

TEST(thread_nvm_store, test_thread_nvm_store_mac_frame_counter)
{
    nsdynmemlib_stub.returnCounter = 2;
    CHECK(test_thread_nvm_store_mac_frame_counter());
}






