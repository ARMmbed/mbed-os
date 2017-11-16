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
#include "test_mac_data_poll.h"

TEST_GROUP(mac_data_poll)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_data_poll, test_mac_poll_timer_trig)
{
    CHECK(test_mac_poll_timer_trig());
}

TEST(mac_data_poll, test_mac_mlme_poll_confirm)
{
    CHECK(test_mac_mlme_poll_confirm());
}



TEST(mac_data_poll, test_mac_data_poll_disable)
{
    CHECK(test_mac_data_poll_disable());
}

TEST(mac_data_poll, test_mac_data_poll_init_protocol_poll)
{
    CHECK(test_mac_data_poll_init_protocol_poll());
}

TEST(mac_data_poll, test_mac_data_poll_host_poll_time_max)
{
    CHECK(test_mac_data_poll_host_poll_time_max());
}

TEST(mac_data_poll, test_mac_data_poll_host_timeout)
{
    CHECK(test_mac_data_poll_host_timeout());
}

TEST(mac_data_poll, test_mac_data_poll_protocol_poll_mode_decrement)
{
    CHECK(test_mac_data_poll_protocol_poll_mode_decrement());
}

TEST(mac_data_poll, test_mac_data_poll_protocol_poll_mode_disable)
{
    CHECK(test_mac_data_poll_protocol_poll_mode_disable());
}

TEST(mac_data_poll, test_mac_data_poll_enable_check)
{
    CHECK(test_mac_data_poll_enable_check());
}

TEST(mac_data_poll, test_mac_data_poll_host_mode_get)
{
    CHECK(test_mac_data_poll_host_mode_get());
}

TEST(mac_data_poll, test_mac_data_poll_host_mode_set)
{
    CHECK(test_mac_data_poll_host_mode_set());
}

TEST(mac_data_poll, test_mac_data_poll_init)
{
    CHECK(test_mac_data_poll_init());
}

TEST(mac_data_poll, test_mac_data_poll_get_max_sleep_period)
{
    CHECK(test_mac_data_poll_get_max_sleep_period());
}
