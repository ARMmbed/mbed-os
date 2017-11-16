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
#include "test_virtual_rf_driver.h"

TEST_GROUP(virtual_rf_driver)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(virtual_rf_driver, phy_rf_mlme_orserver_tx)
{
    CHECK(phy_rf_mlme_orserver_tx());
}

TEST(virtual_rf_driver, test_phy_rf_extension)
{
    CHECK(test_phy_rf_extension());
}

TEST(virtual_rf_driver, test_phy_rf_address_write)
{
    CHECK(test_phy_rf_address_write());
}

TEST(virtual_rf_driver, test_phy_rf_tx)
{
    CHECK(test_phy_rf_tx());
}

TEST(virtual_rf_driver, test_phy_rf_state_control)
{
    CHECK(test_phy_rf_state_control());
}

TEST(virtual_rf_driver, test_phy_rf_virtual_rx)
{
    CHECK(test_phy_rf_virtual_rx());
}

//Must be last test
TEST(virtual_rf_driver, test_virtual_rf_device_register)
{
    CHECK(test_virtual_rf_device_register());
}

