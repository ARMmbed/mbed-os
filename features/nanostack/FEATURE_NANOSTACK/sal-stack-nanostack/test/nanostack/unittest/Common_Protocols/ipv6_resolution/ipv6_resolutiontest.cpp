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
#include "test_ipv6_resolution.h"

TEST_GROUP(ipv6_resolution)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(ipv6_resolution, test_ipv6_map_ip_to_ll_and_call_ll_addr_handler)
{
    CHECK(test_ipv6_map_ip_to_ll_and_call_ll_addr_handler());
}

