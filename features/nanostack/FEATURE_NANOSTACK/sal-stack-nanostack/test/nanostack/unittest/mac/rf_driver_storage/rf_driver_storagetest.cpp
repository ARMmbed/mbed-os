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
#include "test_rf_driver_storage.h"

TEST_GROUP(rf_driver_storage)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(rf_driver_storage, test_arm_net_phy_register)
{
    CHECK(test_arm_net_phy_register());
}

TEST(rf_driver_storage, test_arm_net_phy_init)
{
    CHECK(test_arm_net_phy_init());
}

TEST(rf_driver_storage, test_arm_net_phy_driver_pointer)
{
    CHECK(test_arm_net_phy_driver_pointer());
}

//TEST(rf_driver_storage, test_dev_get_phy_datarate)
//{
//    CHECK(test_dev_get_phy_datarate());
//}

TEST(rf_driver_storage, test_arm_net_phy_mac64_set)
{
    CHECK(test_arm_net_phy_mac64_set());
}

TEST(rf_driver_storage, test_arm_net_phy_mac64_get)
{
    CHECK(test_arm_net_phy_mac64_get());
}

TEST(rf_driver_storage, test_arm_net_phy_rf_type)
{
    CHECK(test_arm_net_phy_rf_type());
}

TEST(rf_driver_storage, test_arm_net_phy_mtu_size)
{
    CHECK(test_arm_net_phy_mtu_size());
}

TEST(rf_driver_storage, test_arm_net_observer_cb_set)
{
    CHECK(test_arm_net_observer_cb_set());
}


