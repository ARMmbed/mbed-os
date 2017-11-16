/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "test_fhss_beacon.h"


TEST_GROUP(fhss_beacon)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(fhss_beacon, test_fhss_beacon_update_payload)
{
    CHECK(test_fhss_beacon_update_payload());
}

TEST(fhss_beacon, test_fhss_beacon_encode_raw)
{
    CHECK(test_fhss_beacon_encode_raw());
}

TEST(fhss_beacon, test_fhss_calculate_uc_index)
{
    CHECK(test_fhss_calculate_uc_index());
}

TEST(fhss_beacon, test_fhss_get_time_to_next_channel_change)
{
    CHECK(test_fhss_get_time_to_next_channel_change());
}

TEST(fhss_beacon, test_fhss_beacon_decode)
{
    CHECK(test_fhss_beacon_decode());
}
