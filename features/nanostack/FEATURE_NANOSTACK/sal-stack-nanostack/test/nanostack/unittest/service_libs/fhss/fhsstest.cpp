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
#include "test_fhss.h"


TEST_GROUP(fhss)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(fhss, test_fhss_timeout_stop)
{
    CHECK(test_fhss_timeout_stop());
}

TEST(fhss, test_fhss_timeout_start)
{
    CHECK(test_fhss_timeout_start());
}

TEST(fhss, test_fhss_down)
{
    CHECK(test_fhss_down());
}

TEST(fhss, test_fhss_event_timer_cb)
{
    CHECK(test_fhss_event_timer_cb());
}

TEST(fhss, test_fhss_superframe_handler)
{
    CHECK(test_fhss_superframe_handler());
}

TEST(fhss, test_fhss_update_synch_parent_address)
{
    CHECK(test_fhss_update_synch_parent_address());
}

TEST(fhss, test_fhss_compare_with_synch_parent_address)
{
    CHECK(test_fhss_compare_with_synch_parent_address());
}

TEST(fhss, test_fhss_get_parent_address)
{
    CHECK(test_fhss_get_parent_address());
}

TEST(fhss, test_fhss_sync_with_beacon)
{
    CHECK(test_fhss_sync_with_beacon());
}

TEST(fhss, test_fhss_set_datarate)
{
    CHECK(test_fhss_set_datarate());
}

TEST(fhss, test_fhss_add_beacon_info)
{
    CHECK(test_fhss_add_beacon_info());
}

TEST(fhss, test_fhss_set_synch_configuration)
{
    CHECK(test_fhss_set_synch_configuration());
}

TEST(fhss, test_fhss_check_bad_channel)
{
    CHECK(test_fhss_check_bad_channel());
}

TEST(fhss, test_fhss_check_tx_allowed)
{
    CHECK(test_fhss_check_tx_allowed());
}

TEST(fhss, test_fhss_check_channel_type)
{
    CHECK(test_fhss_check_channel_type());
}

TEST(fhss, test_fhss_disable)
{
    CHECK(test_fhss_disable());
}

TEST(fhss, test_fhss_enable)
{
    CHECK(test_fhss_enable());
}

TEST(fhss, test_fhss_clear_active_event)
{
    CHECK(test_fhss_clear_active_event());
}

TEST(fhss, test_fhss_update_beacon_info_lifetimes)
{
    CHECK(test_fhss_update_beacon_info_lifetimes());
}

TEST(fhss, test_fhss_failed_handle)
{
    CHECK(test_fhss_failed_handle());
}

TEST(fhss, test_fhss_is_synch_root)
{
    CHECK(test_fhss_is_synch_root());
}
