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
#include "test_mac_mlme.h"

TEST_GROUP(mac_mlme)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_mlme, test_mac_mlme_link_beacon_join_priority_tx_callback_set)
{
    CHECK(test_mac_mlme_link_beacon_join_priority_tx_callback_set());
}

TEST(mac_mlme, test_mac_mlme_link_beacon_compare_rx_callback_set)
{
    CHECK(test_mac_mlme_link_beacon_compare_rx_callback_set());
}

TEST(mac_mlme, test_mac_mlme_scan_confirmation_handle)
{
    CHECK(test_mac_mlme_scan_confirmation_handle());
}

TEST(mac_mlme, test_mac_mlme_scan_request)
{
    CHECK(test_mac_mlme_scan_request());
}

TEST(mac_mlme, test_mac_mlme_energy_scan_request)
{
    CHECK(test_mac_mlme_energy_scan_request());
}

TEST(mac_mlme, test_mac_mlme_start_req)
{
    CHECK(test_mac_mlme_start_req());
}

TEST(mac_mlme, test_mac_mlme_reset)
{
    CHECK(test_mac_mlme_reset());
}

TEST(mac_mlme, test_mac_mlme_set_req)
{
    CHECK(test_mac_mlme_set_req());
}

TEST(mac_mlme, test_mac_mlme_get_req)
{
    CHECK(test_mac_mlme_get_req());
}

TEST(mac_mlme, test_mac_extended_mac_set)
{
    CHECK(test_mac_extended_mac_set());
}

TEST(mac_mlme, test_mac_mlme_poll_req)
{
    CHECK(test_mac_mlme_poll_req());
}

TEST(mac_mlme, test_mac_mlme_poll_process_confirm)
{
    CHECK(test_mac_mlme_poll_process_confirm());
}

TEST(mac_mlme, test_mac_mlme_mac_radio_disabled)
{
    CHECK(test_mac_mlme_mac_radio_disabled());
}

TEST(mac_mlme, test_mac_mlme_mac_radio_enable)
{
    CHECK(test_mac_mlme_mac_radio_enable());
}

TEST(mac_mlme, test_mac_mlme_rf_channel_change)
{
    CHECK(test_mac_mlme_rf_channel_change());
}

TEST(mac_mlme, test_mac_mlme_active_scan_response_timer_start)
{
    CHECK(test_mac_mlme_active_scan_response_timer_start());
}

TEST(mac_mlme, test_mac_mlme_beacon_notify)
{
    CHECK(test_mac_mlme_beacon_notify());
}

TEST(mac_mlme, test_mac_mlme_event_cb)
{
    CHECK(test_mac_mlme_event_cb());
}

TEST(mac_mlme, test_mac_mlme_timer_cb)
{
    CHECK(test_mac_mlme_timer_cb());
}

TEST(mac_mlme, test_mac_mlme_set_active_state)
{
    CHECK(test_mac_mlme_set_active_state());
}

TEST(mac_mlme, test_mac_mlme_data_base_allocate)
{
    CHECK(test_mac_mlme_data_base_allocate());
}

TEST(mac_mlme, test_mac_mlme_data_base_deallocate)
{
    CHECK(test_mac_mlme_data_base_deallocate());
}

TEST(mac_mlme, test_mac_mlme_set_new_sqn)
{
    CHECK(test_mac_mlme_set_new_sqn());
}

TEST(mac_mlme, test_mac_mlme_set_new_beacon_sqn)
{
    CHECK(test_mac_mlme_set_new_beacon_sqn());
}

TEST(mac_mlme, test_mac_mlme_get_panid)
{
    CHECK(test_mac_mlme_get_panid());
}

TEST(mac_mlme, test_mac_frame_src_address_set_from_interface)
{
    CHECK(test_mac_frame_src_address_set_from_interface());
}

TEST(mac_mlme, test_mac_mlme_beacon_tx)
{
    CHECK(test_mac_mlme_beacon_tx());
}

TEST(mac_mlme, test_mac_mlme_beacon_req_tx)
{
    CHECK(test_mac_mlme_beacon_req_tx());
}

TEST(mac_mlme, test_mac_mlme_virtual_confirmation_handle)
{
    CHECK(test_mac_mlme_virtual_confirmation_handle());
}
