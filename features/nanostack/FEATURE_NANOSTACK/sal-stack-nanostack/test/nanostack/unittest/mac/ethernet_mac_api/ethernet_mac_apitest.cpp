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
#include "test_ethernet_mac_api.h"

TEST_GROUP(ethernet_mac_api)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(ethernet_mac_api, test_ethernet_mac_create)
{
    CHECK(test_ethernet_mac_create());
}

TEST(ethernet_mac_api, test_ethernet_mac_destroy)
{
    CHECK(test_ethernet_mac_destroy());
}

TEST(ethernet_mac_api, test_data_request)
{
    CHECK(test_data_request());
}

TEST(ethernet_mac_api, test_data_confirm)
{
    CHECK(test_data_confirm());
}

TEST(ethernet_mac_api, test_data_indication)
{
    CHECK(test_data_indication());
}

TEST(ethernet_mac_api, test_eth_mac_mac48_address_set)
{
    CHECK(test_eth_mac_mac48_address_set());
}

TEST(ethernet_mac_api, test_eth_mac_mac48_address_get)
{
    CHECK(test_eth_mac_mac48_address_get());
}

TEST(ethernet_mac_api, test_eth_mac_api_initialize)
{
    CHECK(test_eth_mac_api_initialize());
}

TEST(ethernet_mac_api, test_rx_cb)
{
    CHECK(test_rx_cb());
}

TEST(ethernet_mac_api, test_tx_done_cb)
{
    CHECK(test_tx_done_cb());
}
