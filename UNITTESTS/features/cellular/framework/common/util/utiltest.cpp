/*
 * Copyright (c) 2018, Arm Limited and affiliates
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
#include "test_util.h"

class TestUtil : public testing::Test {
protected:
    Test_util *unit;

    virtual void SetUp()
    {
        unit = new Test_util();
    }

    virtual void TearDown()
    {
        delete unit;
    }
};

TEST_F(TestUtil, Create)
{
    EXPECT_TRUE(unit);
}

TEST_F(TestUtil, test_util_uint_to_binary_string)
{
    unit->test_util_uint_to_binary_string();
}

TEST_F(TestUtil, char_str_to_hex)
{
    unit->test_util_char_str_to_hex();
}

TEST_F(TestUtil, convert_ipv6)
{
    unit->test_util_convert_ipv6();
}

TEST_F(TestUtil, prefer_ipv6)
{
    unit->test_util_prefer_ipv6();
}

TEST_F(TestUtil, separate_ip_addresses)
{
    unit->test_util_separate_ip_addresses();
}
