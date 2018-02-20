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
#include "test_cellular_mux.h"

TEST_GROUP(cellular_mux)
{
    Test_cellular_mux* unit;

    void setup()
    {
        unit = new Test_cellular_mux();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(cellular_mux, Create)
{
    CHECK(unit != NULL);
}

TEST(cellular_mux, test_cellular_mux_module_init)
{
    unit->test_cellular_mux_module_init();
}

TEST(cellular_mux, test_cellular_mux_mux_start)
{
    unit->test_cellular_mux_mux_start();
}

TEST(cellular_mux, test_cellular_mux_dlci_establish)
{
    unit->test_cellular_mux_dlci_establish();
}

TEST(cellular_mux, test_cellular_mux_serial_attach)
{
    unit->test_cellular_mux_serial_attach();
}

TEST(cellular_mux, test_cellular_mux_eventqueue_attach)
{
    unit->test_cellular_mux_eventqueue_attach();
}


