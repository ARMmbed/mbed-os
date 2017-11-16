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
#include "test_serial_mac_api.h"

TEST_GROUP(serial_mac_api)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(serial_mac_api, test_serial_mac_create)
{
    CHECK(test_serial_mac_create());
}

TEST(serial_mac_api, test_data_request)
{
    CHECK(test_data_request());
}

TEST(serial_mac_api, test_data_indication)
{
    CHECK(test_data_indication());
}

TEST(serial_mac_api, test_serial_mac_api_initialize)
{
    CHECK(test_serial_mac_api_initialize());
}

TEST(serial_mac_api, test_serial_mac_virtual_initialize)
{
    CHECK(test_serial_mac_virtual_initialize());
}

TEST(serial_mac_api, test_serial_mac_net_phy_rx)
{
    CHECK(test_serial_mac_net_phy_rx());
}

TEST(serial_mac_api, test_serial_mac_net_phy_tx_done)
{
    CHECK(test_serial_mac_net_phy_tx_done());
}

TEST(serial_mac_api, test_serial_mac_virtual_data_request)
{
    CHECK(test_serial_mac_virtual_data_request());
}
