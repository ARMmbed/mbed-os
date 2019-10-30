/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "CellularList.h"

using namespace mbed;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class Testlist : public testing::Test {
protected:

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*

struct entry_t {
    int i;
    entry_t *next;
};

TEST_F(Testlist, test_list_int)
{
    CellularList<entry_t> list;
    EXPECT_TRUE(NULL == list.get_head());

    entry_t *first = list.add_new();
    first->i = 1;
    EXPECT_TRUE(NULL != first);

    entry_t *second = list.add_new();
    first->i = 2;
    EXPECT_TRUE(NULL != second);

    entry_t *third = list.add_new();
    first->i = 3;
    EXPECT_TRUE(NULL != third);

    EXPECT_EQ(3, list.get_head()->i);

    list.delete_last(); // Deletes first
    EXPECT_EQ(3, list.get_head()->i);

    list.delete_all();
    EXPECT_TRUE(NULL == list.get_head());
}

TEST_F(Testlist, delete_last_until_empty)
{
    CellularList<entry_t> list;
    list.add_new();
    list.add_new();
    list.delete_last();
    list.delete_last();
    EXPECT_TRUE(NULL == list.get_head());
}

TEST_F(Testlist, empty_list_delete_last)
{
    CellularList<entry_t> list;
    list.delete_last();
    EXPECT_TRUE(NULL == list.get_head());
}

TEST_F(Testlist, empty_list_delete_all)
{
    CellularList<entry_t> list;
    list.delete_all();
    EXPECT_TRUE(NULL == list.get_head());
}
