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
#include "test_at_cellularbase.h"
#include "AT_CellularBase.h"

class TestAT_CellularBase : public testing::Test {
protected:
    Test_AT_CellularBase *unit;

    virtual void SetUp()
    {
        unit = new Test_AT_CellularBase();
    }

    virtual void TearDown()
    {
        delete unit;
    }
};

TEST_F(TestAT_CellularBase, Create)
{
    EXPECT_TRUE(unit);
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_get_at_handler)
{
    unit->test_AT_CellularBase_get_at_handler();
}

TEST_F(TestAT_CellularBase, test_AT_CellularBase_get_device_error)
{
    unit->test_AT_CellularBase_get_device_error();
}
