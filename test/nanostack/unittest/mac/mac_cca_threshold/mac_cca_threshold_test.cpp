/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "test_mac_cca_threshold.h"

TEST_GROUP(mac_cca_threshold)
{
    void setup() {
    }

    void teardown() {
    }
};

TEST(mac_cca_threshold, test_mac_cca_thr_init)
{
    CHECK(test_mac_cca_thr_init());
}

TEST(mac_cca_threshold, test_mac_cca_thr_update_channel_threshold)
{
    CHECK(test_mac_cca_thr_update_channel_threshold());
}

TEST(mac_cca_threshold, test_mac_cca_thr_channel_failed)
{
    CHECK(test_mac_cca_thr_channel_failed());
}

TEST(mac_cca_threshold, test_mac_cca_threshold_update)
{
    CHECK(test_mac_cca_threshold_update());
}
