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
#include "test_virtual_rf_client.h"

TEST_GROUP(virtual_rf_client)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};


TEST(virtual_rf_client, test_phy_rf_virtual_rx)
{
    CHECK(test_phy_rf_virtual_rx());
}

TEST(virtual_rf_client, test_phy_rf_tx_done)
{
    CHECK(test_phy_rf_tx_done());
}

TEST(virtual_rf_client, test_phy_rf_rx)
{
    CHECK(test_phy_rf_rx());
}

TEST(virtual_rf_client, test_virtual_rf_client_register)
{
    CHECK(test_virtual_rf_client_register());
}

