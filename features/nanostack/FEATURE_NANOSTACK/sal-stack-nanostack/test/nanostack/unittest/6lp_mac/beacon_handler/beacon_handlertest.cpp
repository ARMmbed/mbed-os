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
#include "test_beacon_handler.h"

TEST_GROUP(beacon_handler)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(beacon_handler, test_beacon_received)
{
    CHECK(test_beacon_received());
}

TEST(beacon_handler, test_mac_beacon_link_beacon_compare_rx_callback_set)
{
    CHECK(test_mac_beacon_link_beacon_compare_rx_callback_set());
}

TEST(beacon_handler, test_beacon_join_priority_update)
{
    CHECK(test_beacon_join_priority_update());
}

TEST(beacon_handler, test_mac_beacon_link_beacon_join_priority_tx_callback_set)
{
    CHECK(test_mac_beacon_link_beacon_join_priority_tx_callback_set());
}


