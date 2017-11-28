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
#include "test_channel_list.h"


TEST_GROUP(channel_list)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(channel_list, test_channel_get)
{
    CHECK(test_channel_get());
}

TEST(channel_list, test_gap_in_channel_list)
{
    CHECK(test_gap_in_channel_list());
}

TEST(channel_list, test_channel_list_count_channels_reverse)
{
    CHECK(test_channel_list_count_channels_reverse());
}

TEST(channel_list, test_empty_channel_list)
{
    CHECK(test_empty_channel_list());
}

TEST(channel_list, test_channel_list_count_channels)
{
    CHECK(test_channel_list_count_channels());
}
