/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_mac_filter.h"

TEST_GROUP(mac_filter)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_filter, test_mac_filter_modify_link_quality)
{
    CHECK(test_mac_filter_modify_link_quality());
}

TEST(mac_filter, test_mac_filter_start)
{
    CHECK(test_mac_filter_start());
}

TEST(mac_filter, test_mac_filter_stop)
{
    CHECK(test_mac_filter_stop());
}

TEST(mac_filter, test_mac_filter_clear)
{
    CHECK(test_mac_filter_clear());
}

TEST(mac_filter, test_mac_filter_set_address_mapping)
{
    CHECK(test_mac_filter_set_address_mapping());
}

TEST(mac_filter, test_mac_filter_delete_short)
{
    CHECK(test_mac_filter_delete_short());
}

TEST(mac_filter, test_mac_filter_delete_long)
{
    CHECK(test_mac_filter_delete_long());
}

TEST(mac_filter, test_mac_filter_add_short)
{
    CHECK(test_mac_filter_add_short());
}

TEST(mac_filter, test_mac_filter_add_long)
{
    CHECK(test_mac_filter_add_long());
}

