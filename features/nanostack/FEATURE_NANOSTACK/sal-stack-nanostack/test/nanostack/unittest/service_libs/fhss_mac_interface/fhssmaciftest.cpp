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
#include "test_fhss_mac_interface.h"


TEST_GROUP(fhss_mac_if)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(fhss_mac_if, test_fhss_is_broadcast_channel_cb)
{
    CHECK(test_fhss_is_broadcast_channel_cb());
}

TEST(fhss_mac_if, test_fhss_use_broadcast_queue_cb)
{
    CHECK(test_fhss_use_broadcast_queue_cb());
}

TEST(fhss_mac_if, test_fhss_tx_handle_cb)
{
    CHECK(test_fhss_tx_handle_cb());
}

TEST(fhss_mac_if, test_fhss_check_tx_conditions_cb)
{
    CHECK(test_fhss_check_tx_conditions_cb());
}

TEST(fhss_mac_if, test_fhss_receive_frame_cb)
{
    CHECK(test_fhss_receive_frame_cb());
}

TEST(fhss_mac_if, test_fhss_data_tx_done_cb)
{
    CHECK(test_fhss_data_tx_done_cb());
}

TEST(fhss_mac_if, test_fhss_data_tx_fail_cb)
{
    CHECK(test_fhss_data_tx_fail_cb());
}

TEST(fhss_mac_if, test_fhss_synch_state_set_cb)
{
    CHECK(test_fhss_synch_state_set_cb());
}

TEST(fhss_mac_if, test_fhss_read_timestamp_cb)
{
    CHECK(test_fhss_read_timestamp_cb());
}

TEST(fhss_mac_if, test_fhss_init_callbacks_cb)
{
    CHECK(test_fhss_init_callbacks_cb());
}

TEST(fhss_mac_if, test_fhss_get_retry_period_cb)
{
    CHECK(test_fhss_get_retry_period_cb());
}
