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
#include "test_mac_security_mib.h"

TEST_GROUP(mac_security_mib)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_security_mib, test_mac_sec_mib_init)
{
    CHECK(test_mac_sec_mib_init());
}

TEST(mac_security_mib, test_mac_sec_mib_deinit)
{
    CHECK(test_mac_sec_mib_deinit());
}

TEST(mac_security_mib, test_mac_sec_mib_device_description_set)
{
    CHECK(test_mac_sec_mib_device_description_set());
}

TEST(mac_security_mib, test_mac_sec_mib_key_description_set)
{
    CHECK(test_mac_sec_mib_key_description_set());
}

TEST(mac_security_mib, test_mac_sec_mib_device_description_get_attribute_index)
{
    CHECK(test_mac_sec_mib_device_description_get_attribute_index());
}

TEST(mac_security_mib, test_mac_sec_mib_device_description_get)
{
    CHECK(test_mac_sec_mib_device_description_get());
}

TEST(mac_security_mib, test_mac_mib_device_descption_attribute_get_by_descriptor)
{
    CHECK(test_mac_mib_device_descption_attribute_get_by_descriptor());
}

TEST(mac_security_mib, test_mac_sec_key_description_get)
{
    CHECK(test_mac_sec_key_description_get());
}

TEST(mac_security_mib, test_mac_sec_mib_key_device_description_list_update)
{
    CHECK(test_mac_sec_mib_key_device_description_list_update());
}

TEST(mac_security_mib, test_mac_sec_mib_key_device_description_discover_from_list)
{
    CHECK(test_mac_sec_mib_key_device_description_discover_from_list());
}

TEST(mac_security_mib, test_mac_sec_mib_device_description_blacklist)
{
    CHECK(test_mac_sec_mib_device_description_blacklist());
}

