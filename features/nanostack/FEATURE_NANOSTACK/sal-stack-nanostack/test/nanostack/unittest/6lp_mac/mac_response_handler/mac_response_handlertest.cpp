/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_mac_response_handler.h"

TEST_GROUP(mac_response_handler)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_response_handler, test_mcps_data_confirm_handler)
{
    CHECK(test_mcps_data_confirm_handler());
}

TEST(mac_response_handler, test_mcps_data_indication_handler)
{
    CHECK(test_mcps_data_indication_handler());
}

TEST(mac_response_handler, test_mlme_confirm_handler)
{
    CHECK(test_mlme_confirm_handler());
}

TEST(mac_response_handler, test_mlme_indication_handler)
{
    CHECK(test_mlme_indication_handler());
}
