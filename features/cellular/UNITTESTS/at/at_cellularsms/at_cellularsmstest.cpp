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
#include "test_at_cellularsms.h"

TEST_GROUP(AT_CellularSMS)
{
    Test_AT_CellularSMS* unit;

    void setup()
    {
        unit = new Test_AT_CellularSMS();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularSMS, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularSMS, test_AT_CellularSMS_constructor)
{
    unit->test_AT_CellularSMS_constructor();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_initialize)
{
    unit->test_AT_CellularSMS_initialize();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_send_sms)
{
    unit->test_AT_CellularSMS_send_sms();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_get_sms)
{
    unit->test_AT_CellularSMS_get_sms();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_set_sms_callback)
{
    unit->test_AT_CellularSMS_set_sms_callback();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_set_cpms)
{
    unit->test_AT_CellularSMS_set_cpms();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_set_csca)
{
    unit->test_AT_CellularSMS_set_csca();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_set_cscs)
{
    unit->test_AT_CellularSMS_set_cscs();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_delete_all_messages)
{
    unit->test_AT_CellularSMS_delete_all_messages();
}

TEST(AT_CellularSMS, test_AT_CellularSMS_set_extra_sim_wait_time)
{
    unit->test_AT_CellularSMS_set_extra_sim_wait_time();
}
