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
#include "test_at_cellularpower.h"

TEST_GROUP(AT_CellularPower)
{
    Test_AT_CellularPower* unit;

    void setup()
    {
        unit = new Test_AT_CellularPower();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularPower, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularPower, test_AT_CellularPower_constructor)
{
    unit->test_AT_CellularPower_constructor();
}

TEST(AT_CellularPower, test_AT_CellularPower_on)
{
    unit->test_AT_CellularPower_on();
}

TEST(AT_CellularPower, test_AT_CellularPower_off)
{
    unit->test_AT_CellularPower_off();
}

TEST(AT_CellularPower, test_AT_CellularPower_set_at_mode)
{
    unit->test_AT_CellularPower_set_at_mode();
}

TEST(AT_CellularPower, test_AT_CellularPower_set_power_level)
{
    unit->test_AT_CellularPower_set_power_level();
}

TEST(AT_CellularPower, test_AT_CellularPower_reset)
{
    unit->test_AT_CellularPower_reset();
}

TEST(AT_CellularPower, test_AT_CellularPower_opt_power_save_mode)
{
    unit->test_AT_CellularPower_opt_power_save_mode();
}

TEST(AT_CellularPower, test_AT_CellularPower_opt_receive_period)
{
    unit->test_AT_CellularPower_opt_receive_period();
}

TEST(AT_CellularPower, test_AT_CellularPower_is_device_ready)
{
    unit->test_AT_CellularPower_is_device_ready();
}

TEST(AT_CellularPower, test_AT_CellularPower_set_device_ready_urc_cb)
{
    unit->test_AT_CellularPower_set_device_ready_urc_cb();
}

TEST(AT_CellularPower, test_AT_CellularPower_remove_device_ready_urc_cb)
{
    unit->test_AT_CellularPower_remove_device_ready_urc_cb();
}
