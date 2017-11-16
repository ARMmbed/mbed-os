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
#include "test_mac_mcps_sap.h"

TEST_GROUP(mac_mcps_sap)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_mcps_sap, test_mac_generic_event_trig)
{
    CHECK(test_mac_generic_event_trig());
}

TEST(mac_mcps_sap, test_mcps_sap_trig_tx)
{
    CHECK(test_mcps_sap_trig_tx());
}

TEST(mac_mcps_sap, test_mac_virtual_sap_data_cb)
{
    CHECK(test_mac_virtual_sap_data_cb());
}

TEST(mac_mcps_sap, test_mac_mcps_data_confirmation)
{
    CHECK(test_mac_mcps_data_confirmation());
}


TEST(mac_mcps_sap, test_mac_mcps_data_indication)
{
    CHECK(test_mac_mcps_data_indication());
}

TEST(mac_mcps_sap, test_mac_mcps_sap_tasklet_init)
{
    CHECK(test_mac_mcps_sap_tasklet_init());
}

TEST(mac_mcps_sap, test_mcps_sap_pre_parsed_frame_buffer_free)
{
    CHECK(test_mcps_sap_pre_parsed_frame_buffer_free());
}

TEST(mac_mcps_sap, test_mcps_sap_prebuild_frame_buffer_get)
{
    CHECK(test_mcps_sap_prebuild_frame_buffer_get());
}

TEST(mac_mcps_sap, test_mcps_sap_prebuild_frame_buffer_free)
{
    CHECK(test_mcps_sap_prebuild_frame_buffer_free());
}

TEST(mac_mcps_sap, test_mcps_sap_pd_req_queue_write)
{
    CHECK(test_mcps_sap_pd_req_queue_write());
}

//TEST(mac_mcps_sap, test_mcps_data_confirm_handle)
//{
//    CHECK(test_mcps_data_confirm_handle());
//}

TEST(mac_mcps_sap, test_mcps_sap_pre_parsed_frame_buffer_get)
{
    CHECK(test_mcps_sap_pre_parsed_frame_buffer_get());
}

TEST(mac_mcps_sap, test_mcps_sap_pd_ind)
{
    CHECK(test_mcps_sap_pd_ind());
}

TEST(mac_mcps_sap, test_mcps_sap_pd_confirm)
{
    CHECK(test_mcps_sap_pd_confirm());
}

TEST(mac_mcps_sap, test_mcps_sap_data_req_handler)
{
    CHECK(test_mcps_sap_data_req_handler());
}

TEST(mac_mcps_sap, test_mac_mcps_trig_buffer_from_queue)
{
    CHECK(test_mac_mcps_trig_buffer_from_queue());
}

TEST(mac_mcps_sap, test_mac_mcps_buffer_queue_free)
{
    CHECK(test_mac_mcps_buffer_queue_free());
}
