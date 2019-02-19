/*
 * Copyright (c) 2016, 2018, Arm Limited and affiliates.
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
#include "test_cs_nvm.h"

TEST_GROUP(CS_NVM)
{
    void setup() {

    }

    void teardown() {

    }
};

TEST(CS_NVM, test_cs_nvm_operation_in_progress)
{
    CHECK(test_cs_nvm_operation_in_progress() == true);
}

TEST(CS_NVM, test_cs_nvm_flush)
{
    CHECK(test_cs_nvm_flush() == true);
}

TEST(CS_NVM, test_cs_nvm_write_failure_in_close)
{
    CHECK(test_cs_nvm_write_failure_in_close() == true);
}

TEST(CS_NVM, test_cs_nvm_write_failure)
{
    CHECK(test_cs_nvm_write_failure() == true);
}

TEST(CS_NVM, test_cs_nvm_write)
{
    CHECK(test_cs_nvm_write() == true);
}

TEST(CS_NVM, test_cs_nvm_read_failure_in_close)
{
    CHECK(test_cs_nvm_read_failure_in_close() == true);
}

TEST(CS_NVM, test_cs_nvm_read_failure)
{
    CHECK(test_cs_nvm_read_failure() == true);
}

TEST(CS_NVM, test_cs_nvm_read)
{
    CHECK(test_cs_nvm_read() == true);
}

TEST(CS_NVM, test_cs_nvm_key_delete_failure_in_close)
{
    CHECK(test_cs_nvm_key_delete_failure_in_close() == true);
}

TEST(CS_NVM, test_cs_nvm_key_delete)
{
    CHECK(test_cs_nvm_key_delete() == true);
}

TEST(CS_NVM, test_cs_nvm_key_create)
{
    CHECK(test_cs_nvm_key_create() == true);
}

TEST(CS_NVM, test_cs_nvm_init_finalize)
{
    CHECK(test_cs_nvm_init_finalize() == true);
}

