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
#include "test_at_cellularsim.h"

TEST_GROUP(AT_CellularSIM)
{
    Test_AT_CellularSIM* unit;

    void setup()
    {
        unit = new Test_AT_CellularSIM();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularSIM, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularSIM, test_AT_CellularSIM_constructor)
{
    unit->test_AT_CellularSIM_constructor();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_set_pin)
{
    unit->test_AT_CellularSIM_set_pin();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_change_pin)
{
    unit->test_AT_CellularSIM_change_pin();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_set_pin_query)
{
    unit->test_AT_CellularSIM_set_pin_query();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_get_sim_state)
{
    unit->test_AT_CellularSIM_get_sim_state();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_get_imsi)
{
    unit->test_AT_CellularSIM_get_imsi();
}

