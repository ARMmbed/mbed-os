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
#include "test_cellular_mux_data_service.h"

TEST_GROUP(cellular_mux_data_service)
{
    Test_cellular_mux_data_service* unit;

    void setup()
    {
        unit = new Test_cellular_mux_data_service();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(cellular_mux_data_service, Create)
{
    CHECK(unit != NULL);
}

TEST(cellular_mux_data_service, test_cellular_mux_write)
{
    unit->test_cellular_mux_write();
}

TEST(cellular_mux_data_service, test_cellular_mux_read)
{
    unit->test_cellular_mux_read();
}

TEST(cellular_mux_data_service, test_cellular_mux_seek)
{
    unit->test_cellular_mux_seek();
}

TEST(cellular_mux_data_service, test_cellular_mux_close)
{
    unit->test_cellular_mux_close();
}

TEST(cellular_mux_data_service, test_cellular_mux_sigio)
{
    unit->test_cellular_mux_sigio();
}
