/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"
#include "ble/GattClient.h"
#include "ble/SecurityManager.h"

class TestBLE : public testing::Test {
protected:
    void SetUp()
    {
        ble = &BLE::Instance();
        EXPECT_TRUE(ble);
    }

    void TearDown()
    {
    }

    BLE* ble;
};

TEST_F(TestBLE, reset)
{
    Gap &gap = ble->gap();
    gap.reset();

    GattClient &client = ble->gattClient();
    client.reset();

    GattServer &server = ble->gattServer();
    server.reset();

    SecurityManager &sm = ble->securityManager();
    sm.reset();
}
