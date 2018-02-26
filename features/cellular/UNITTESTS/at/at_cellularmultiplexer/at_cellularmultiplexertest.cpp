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
#include "test_at_cellularmultiplexer.h"

TEST_GROUP(AT_CellularMultiplexer)
{
    Test_AT_CellularMultiplexer* unit;

    void setup()
    {
        unit = new Test_AT_CellularMultiplexer();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularMultiplexer, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularMultiplexer, test_AT_CellularMultiplexer_constructor)
{
    unit->test_AT_CellularMultiplexer_constructor();
}

TEST(AT_CellularMultiplexer, test_AT_CellularMultiplexer_multiplexer_mode_start)
{
    unit->test_AT_CellularMultiplexer_multiplexer_mode_start();
}

