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
#include "features/netsocket/NetworkInterface.h"

class stubNetworkInterface : public NetworkInterface {
    virtual nsapi_error_t connect()
    {
        return NSAPI_ERROR_OK;
    };
    virtual nsapi_error_t disconnect()
    {
        return NSAPI_ERROR_OK;
    };
    virtual NetworkStack *get_stack()
    {
        return NULL;
    };
};

class TestNetworkInterface : public testing::Test {
protected:
    NetworkInterface *iface;

    virtual void SetUp()
    {
        iface = new stubNetworkInterface();
    }

    virtual void TearDown()
    {
        delete iface;
    }
};

TEST_F(TestNetworkInterface, constructor)
{
    EXPECT_TRUE(iface);
}

TEST_F(TestNetworkInterface, get_mac_address)
{
    char *n = 0;
    EXPECT_EQ(iface->get_mac_address(), n);
}
