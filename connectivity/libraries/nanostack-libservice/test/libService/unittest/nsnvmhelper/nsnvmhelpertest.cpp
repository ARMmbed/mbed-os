/*
 * Copyright (c) 2016, 2020, Pelion and affiliates.
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
#include "test_ns_nvm_helper.h"

TEST_GROUP(NS_NVM_HELPER)
{
    void setup() {

    }

    void teardown() {

    }
};

TEST(NS_NVM_HELPER, test_ns_nvm_helper_platform_error_in_write)
{
    CHECK(test_ns_nvm_helper_platform_error_in_write() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_platform_error)
{
    CHECK(test_ns_nvm_helper_platform_error() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_concurrent_requests)
{
    CHECK(test_ns_nvm_helper_concurrent_requests() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_read)
{
    CHECK(test_ns_nvm_helper_read() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_delete)
{
    CHECK(test_ns_nvm_helper_delete() == true);
}

/* Write must be the last test (executed first) as it initialized the platform NVM */
TEST(NS_NVM_HELPER, test_ns_nvm_helper_write)
{
    CHECK(test_ns_nvm_helper_write() == true);
}

