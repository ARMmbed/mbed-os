/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "test_ns_mdns_api.h"

TEST_GROUP(MDNS)
{
    void setup() {
    }

    void teardown() {
    }
};

TEST(MDNS, test_ns_mdns_api_start_1)
{
    CHECK(test_ns_mdns_api_start_1());
}

TEST(MDNS, test_ns_mdns_api_service_1)
{
    CHECK(test_ns_mdns_api_service_1());
}

TEST(MDNS, test_ns_mdns_api_service_2)
{
    CHECK(test_ns_mdns_api_service_2());
}

TEST(MDNS, test_ns_mdns_api_announce_1)
{
    CHECK(test_ns_mdns_api_announce_1());
}
