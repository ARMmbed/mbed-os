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
#include "CppUTest/TestHarness.h"
#include "test_at_cellulardevice.h"

TEST_GROUP(AT_CellularDevice)
{
    Test_AT_CellularDevice* unit;

    void setup()
    {
        unit = new Test_AT_CellularDevice();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularDevice, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularDevice, test_AT_CellularDevice_constructor)
{
    unit->test_AT_CellularDevice_constructor();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_get_at_handler)
{
    unit->test_AT_CellularDevice_get_at_handler();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_open_network)
{
    unit->test_AT_CellularDevice_open_network();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_open_sms)
{
    unit->test_AT_CellularDevice_open_sms();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_open_power)
{
    unit->test_AT_CellularDevice_open_power();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_open_sim)
{
    unit->test_AT_CellularDevice_open_sim();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_open_information)
{
    unit->test_AT_CellularDevice_open_information();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_close_network)
{
    unit->test_AT_CellularDevice_close_network();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_close_sms)
{
    unit->test_AT_CellularDevice_close_sms();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_close_power)
{
    unit->test_AT_CellularDevice_close_power();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_close_sim)
{
    unit->test_AT_CellularDevice_close_sim();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_close_information)
{
    unit->test_AT_CellularDevice_close_information();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_set_timeout)
{
    unit->test_AT_CellularDevice_set_timeout();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_modem_debug_on)
{
    unit->test_AT_CellularDevice_modem_debug_on();
}

TEST(AT_CellularDevice, test_AT_CellularDevice_get_stack)
{
    unit->test_AT_CellularDevice_get_stack();
}
