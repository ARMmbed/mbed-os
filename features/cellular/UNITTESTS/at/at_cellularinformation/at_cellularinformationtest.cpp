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
#include "test_at_cellularinformation.h"

TEST_GROUP(AT_CellularInformation)
{
    Test_AT_CellularInformation* unit;

    void setup()
    {
        unit = new Test_AT_CellularInformation();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularInformation, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_manufacturer)
{
    unit->test_AT_CellularInformation_get_manufacturer();
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_model)
{
    unit->test_AT_CellularInformation_get_model();
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_revision)
{
    unit->test_AT_CellularInformation_get_revision();
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_serial_number)
{
    unit->test_AT_CellularInformation_get_serial_number();
}
