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
#include "test_fhss_channel.h"


TEST_GROUP(fhss_channel)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(fhss_channel, test_fhss_change_to_rx_channel)
{
    CHECK(test_fhss_change_to_rx_channel());
}

TEST(fhss_channel, test_fhss_change_to_parent_channel)
{
    CHECK(test_fhss_change_to_parent_channel());
}

TEST(fhss_channel, test_fhss_change_to_tx_channel)
{
    CHECK(test_fhss_change_to_tx_channel());
}

TEST(fhss_channel, test_fhss_update_channel)
{
    CHECK(test_fhss_update_channel());
}

TEST(fhss_channel, test_fhss_is_current_channel_broadcast)
{
    CHECK(test_fhss_is_current_channel_broadcast());
}

TEST(fhss_channel, test_channels_used_equally)
{
    CHECK(test_channels_used_equally());
}

TEST(fhss_channel, test_channel_list_repeating_64_channels)
{
    CHECK(test_channel_list_repeating_64_channels());
}

TEST(fhss_channel, test_channel_list_repeating_50_channels)
{
    CHECK(test_channel_list_repeating_50_channels());
}

TEST(fhss_channel, test_channel_randomization)
{
    CHECK(test_channel_randomization());
}

TEST(fhss_channel, test_fhss_get_offset)
{
    CHECK(test_fhss_get_offset());
}
