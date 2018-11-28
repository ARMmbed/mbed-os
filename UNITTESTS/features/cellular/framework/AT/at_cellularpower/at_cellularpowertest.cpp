/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularPower.h"
#include "FileHandle_stub.h"
#include "ATHandler_stub.h"

using namespace mbed;
using namespace events;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularPower : public testing::Test {
protected:

    void SetUp() {
        ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    }

    void TearDown() {
    }
};
// *INDENT-ON*


TEST_F(TestAT_CellularPower, Create)
{

    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower *pow = new AT_CellularPower(at);

    EXPECT_TRUE(pow != NULL);

    delete pow;
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_on)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == pow.on());
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_off)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == pow.off());
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_set_at_mode)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.set_at_mode());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == pow.set_at_mode());
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_set_power_level)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.set_power_level(6));
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.set_power_level(1, 1));
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.set_power_level(1, 0));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == pow.set_power_level(6));
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_reset)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.reset());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == pow.reset());
}
