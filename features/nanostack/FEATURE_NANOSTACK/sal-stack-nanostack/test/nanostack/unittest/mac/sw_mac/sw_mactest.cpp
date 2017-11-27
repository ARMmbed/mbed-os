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
#include "test_sw_mac.h"
#include "mac_mcps_sap_stub.h"


TEST_GROUP(sw_mac)
{
    void setup()
    {
        mac_mcps_sap_stub.int8_value = 0;
    }

    void teardown()
    {
    }
};

TEST(sw_mac, test_ns_sw_mac_fhss_register)
{
    CHECK(test_ns_sw_mac_fhss_register());
}

TEST(sw_mac, test_ns_sw_mac_create)
{
    CHECK(test_ns_sw_mac_create());
}

TEST(sw_mac, test_get_sw_mac_api)
{
    CHECK(test_get_sw_mac_api());
}

TEST(sw_mac, test_get_sw_mac_ptr_by_mac_api)
{
    CHECK(test_get_sw_mac_ptr_by_mac_api());
}

TEST(sw_mac, test_get_sw_mac_ptr_by_fhss_api)
{
    CHECK(test_get_sw_mac_ptr_by_fhss_api());
}

TEST(sw_mac, test_get_sw_mac_ptr_by_timer)
{
    CHECK(test_get_sw_mac_ptr_by_timer());
}

TEST(sw_mac, test_get_sw_mac_ptr_by_driver_id)
{
    CHECK(test_get_sw_mac_ptr_by_driver_id());
}

TEST(sw_mac, test_initialize)
{
    CHECK(test_initialize());
}

TEST(sw_mac, test_mlme_req)
{
    CHECK(test_mlme_req());
}

TEST(sw_mac, test_mcps_data_req)
{
    CHECK(test_mcps_data_req());
}

TEST(sw_mac, test_mac64_get)
{
    CHECK(test_mac64_get());
}

TEST(sw_mac, test_mac64_set)
{
    CHECK(test_mac64_set());
}

TEST(sw_mac, test_sw_mac_net_phy_rx)
{
    CHECK(test_sw_mac_net_phy_rx());
}

TEST(sw_mac, test_sw_mac_net_phy_tx_done)
{
    CHECK(test_sw_mac_net_phy_tx_done());
}

TEST(sw_mac, test_sw_mac_storage_decription_sizes_get)
{
    CHECK(test_sw_mac_storage_decription_sizes_get());
}

TEST(sw_mac, test_ns_sw_mac_virtual_client_register)
{
    CHECK(test_ns_sw_mac_virtual_client_register());
}

TEST(sw_mac, test_ns_sw_mac_virtual_client_unregister)
{
    CHECK(test_ns_sw_mac_virtual_client_unregister());
}

