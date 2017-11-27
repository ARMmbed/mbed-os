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
#include "test_mac_helper.h"

TEST_GROUP(mac_helper)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_helper, test_mac_create_scan_request)
{
    CHECK(test_mac_create_scan_request());
}

TEST(mac_helper, test_mac_helper_select_best_lqi)
{
    CHECK(test_mac_helper_select_best_lqi());
}

TEST(mac_helper, test_mac_helper_drop_selected_from_the_scanresult)
{
    CHECK(test_mac_helper_drop_selected_from_the_scanresult());
}

TEST(mac_helper, test_mac_helper_free_scan_confirm)
{
    CHECK(test_mac_helper_free_scan_confirm());
}

TEST(mac_helper, test_mac_helper_free_pan_descriptions)
{
    CHECK(test_mac_helper_free_pan_descriptions());
}

TEST(mac_helper, test_mac_helper_nwk_id_filter_set)
{
    CHECK(test_mac_helper_nwk_id_filter_set());
}

TEST(mac_helper, test_mac_helper_panid_set)
{
    CHECK(test_mac_helper_panid_set());
}

TEST(mac_helper, test_mac_helper_mac16_address_set)
{
    CHECK(test_mac_helper_mac16_address_set());
}

TEST(mac_helper, test_mac_helper_mac16_address_get)
{
    CHECK(test_mac_helper_mac16_address_get());
}


TEST(mac_helper, test_mac_helper_panid_get)
{
    CHECK(test_mac_helper_panid_get());
}

TEST(mac_helper, test_mac_helper_set_default_key_source)
{
    CHECK(test_mac_helper_set_default_key_source());
}

TEST(mac_helper, test_mac_helper_default_security_level_set)
{
    CHECK(test_mac_helper_default_security_level_set());
}

TEST(mac_helper, test_mac_helper_default_security_level_get)
{
    CHECK(test_mac_helper_default_security_level_get());
}

TEST(mac_helper, test_mac_helper_default_security_key_id_mode_set)
{
    CHECK(test_mac_helper_default_security_key_id_mode_set());
}


TEST(mac_helper, test_mac_helper_default_security_key_id_mode_get)
{
    CHECK(test_mac_helper_default_security_key_id_mode_get());
}

TEST(mac_helper, test_mac_helper_default_key_index_set)
{
    CHECK(test_mac_helper_default_key_index_set());
}

TEST(mac_helper, test_mac_helper_default_key_index_get)
{
    CHECK(test_mac_helper_default_key_index_get());
}

TEST(mac_helper, test_mac_helper_security_default_key_set)
{
    CHECK(test_mac_helper_security_default_key_set());
}

TEST(mac_helper, test_mac_helper_security_pairwisekey_set)
{
    CHECK(test_mac_helper_security_pairwisekey_set());
}

TEST(mac_helper, test_mac_helper_security_next_key_set)
{
    CHECK(test_mac_helper_security_next_key_set());
}

TEST(mac_helper, test_mac_helper_security_prev_key_set)
{
    CHECK(test_mac_helper_security_prev_key_set());
}

TEST(mac_helper, test_mac_helper_security_key_swap_next_to_default)
{
    CHECK(test_mac_helper_security_key_swap_next_to_default());
}

TEST(mac_helper, test_mac_helper_security_key_clean)
{
    CHECK(test_mac_helper_security_key_clean());
}

TEST(mac_helper, test_mac_helper_coordinator_address_set)
{
    CHECK(test_mac_helper_coordinator_address_set());
}

TEST(mac_helper, test_mac_helper_coordinator_address_get)
{
    CHECK(test_mac_helper_coordinator_address_get());
}

TEST(mac_helper, test_mac_helper_beacon_payload_reallocate)
{
    CHECK(test_mac_helper_beacon_payload_reallocate());
}

TEST(mac_helper, test_mac_helper_beacon_payload_register)
{
    CHECK(test_mac_helper_beacon_payload_register());
}

TEST(mac_helper, test_mac_helper_beacon_payload_pointer_get)
{
    CHECK(test_mac_helper_beacon_payload_pointer_get());
}

TEST(mac_helper, test_mac_helper_beacon_payload_length_get)
{
    CHECK(test_mac_helper_beacon_payload_length_get());
}

TEST(mac_helper, test_mac_helper_pib_boolean_set)
{
    CHECK(test_mac_helper_pib_boolean_set());
}

TEST(mac_helper, test_mac_helper_mac_channel_set)
{
    CHECK(test_mac_helper_mac_channel_set());
}

TEST(mac_helper, test_mac_helper_write_our_addr)
{
    CHECK(test_mac_helper_write_our_addr());
}

TEST(mac_helper, test_mac_helper_mac64_set)
{
    CHECK(test_mac_helper_mac64_set());
}


TEST(mac_helper, test_mac_helper_max_payload_size)
{
    CHECK(test_mac_helper_max_payload_size());
}

TEST(mac_helper, test_mac_helper_frame_overhead)
{
    CHECK(test_mac_helper_frame_overhead());
}

TEST(mac_helper, test_mac_helper_link_frame_counter_read)
{
    CHECK(test_mac_helper_link_frame_counter_read());
}

TEST(mac_helper, test_mac_helper_link_frame_counter_set)
{
    CHECK(test_mac_helper_link_frame_counter_set());
}

TEST(mac_helper, test_mac_helper_mac_mlme_max_retry_set)
{
    CHECK(test_mac_helper_mac_mlme_max_retry_set());
}



TEST(mac_helper, test_mac_helper_devicetable_remove)
{
    CHECK(test_mac_helper_devicetable_remove());
}

TEST(mac_helper, test_mac_helper_devicetable_set)
{
    CHECK(test_mac_helper_devicetable_set());
}










