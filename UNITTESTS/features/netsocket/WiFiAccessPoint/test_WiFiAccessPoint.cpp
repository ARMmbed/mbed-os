/*
 * Copyright (c) 2018, Arm Limited and affiliates
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

#include "gtest/gtest.h"
#include "features/netsocket/WiFiAccessPoint.h"
#include "features/netsocket/NetworkInterface.h"
#include <cstring>

class TestWiFiAccessPoint : public testing::Test {
protected:
    WiFiAccessPoint *ap;

    virtual void SetUp()
    {
        ap = new WiFiAccessPoint;
    }

    virtual void TearDown()
    {
        delete ap;
    }
};

TEST_F(TestWiFiAccessPoint, constructor)
{
    EXPECT_TRUE(ap);
}

TEST_F(TestWiFiAccessPoint, getters_default)
{
    // ssid and bssid return pointers to uninitialized memory - impossible to set expectations.
    EXPECT_EQ(0, ap->get_channel());
    EXPECT_EQ(0, ap->get_rssi());
    EXPECT_EQ(NSAPI_SECURITY_NONE, ap->get_security());
}

TEST_F(TestWiFiAccessPoint, set_data)
{
    nsapi_wifi_ap_t testAp = {
        "mySSID67890123456789012345678901",
        {0xd8, 0xc7, 0xc8, 0xcc, 0x43, 0x24},
        NSAPI_SECURITY_WPA2,
        0x64,
        0x02,
    };
    WiFiAccessPoint *ap1 = new WiFiAccessPoint(testAp);
    EXPECT_EQ(std::string(testAp.ssid), std::string(ap1->get_ssid()));
    EXPECT_TRUE(0 == std::memcmp(testAp.bssid, ap1->get_bssid(), sizeof(testAp.bssid)));
    EXPECT_EQ(testAp.channel, ap1->get_channel());
    EXPECT_EQ(testAp.rssi, ap1->get_rssi());
    EXPECT_EQ(testAp.security, ap1->get_security());
    delete ap1;
}

