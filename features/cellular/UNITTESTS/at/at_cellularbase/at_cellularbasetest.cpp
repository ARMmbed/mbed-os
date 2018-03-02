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
#include "test_at_cellularbase.h"
#include "AT_CellularBase.h"

TEST_GROUP(AT_CellularBase)
{
    Test_AT_CellularBase* unit;

    void setup()
    {
        unit = new Test_AT_CellularBase();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularBase, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularBase, test_AT_CellularBase_get_at_handler)
{
    unit->test_AT_CellularBase_get_at_handler();
}

TEST(AT_CellularBase, test_AT_CellularBase_get_device_error)
{
    unit->test_AT_CellularBase_get_device_error();
}

