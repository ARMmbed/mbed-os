/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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
#include "test_util.h"

TEST_GROUP(util)
{
    Test_util* unit;

    void setup()
    {
        unit = new Test_util();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(util, Create)
{
    CHECK(unit != NULL);
}

TEST(util, test_util_uint_to_binary_string)
{
    unit->test_util_uint_to_binary_string();
}

TEST(util, char_str_to_hex)
{
    unit->test_util_char_str_to_hex();
}

TEST(util, convert_ipv6)
{
    unit->test_util_convert_ipv6();
}

TEST(util, prefer_ipv6)
{
    unit->test_util_prefer_ipv6();
}

TEST(util, separate_ip_addresses)
{
    unit->test_util_separate_ip_addresses();
}
