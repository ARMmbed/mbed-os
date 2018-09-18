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


static void device_ready_cb()
{
}

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

TEST_F(TestAT_CellularPower, test_AT_CellularPower_opt_power_save_mode)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(0, 0));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(10, 0));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(912, 0));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(1834, 1834));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(18345, 18345));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(101234, 101234));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(1012345, 1012345));

    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_power_save_mode(39612345, 39612345));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == pow.opt_power_save_mode(0, 0));
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_opt_receive_period)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == pow.opt_receive_period(1, CellularPower::EDRXUTRAN_Iu_mode, 3));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == pow.opt_receive_period(1, CellularPower::EDRXUTRAN_Iu_mode, 3));
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_is_device_ready)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_TRUE(NSAPI_ERROR_AUTH_FAILURE == pow.is_device_ready());
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_set_device_ready_urc_cb)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == pow.set_device_ready_urc_cb(&device_ready_cb));
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == pow.set_device_ready_urc_cb(NULL));
}

TEST_F(TestAT_CellularPower, test_AT_CellularPower_remove_device_ready_urc_cb)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == pow.set_device_ready_urc_cb(&device_ready_cb));

    pow.remove_device_ready_urc_cb(NULL);
    pow.remove_device_ready_urc_cb(&device_ready_cb);
}
