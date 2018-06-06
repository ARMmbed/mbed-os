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
#include "CppUTest/TestHarness.h"
#include "test_at_cellularpower.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularPower.h"
#include "FileHandle_stub.h"
#include "ATHandler_stub.h"

using namespace mbed;
using namespace events;

Test_AT_CellularPower::Test_AT_CellularPower()
{
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
}

Test_AT_CellularPower::~Test_AT_CellularPower()
{
}

void Test_AT_CellularPower::test_AT_CellularPower_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower *pow = new AT_CellularPower(at);

    delete pow;
}

void Test_AT_CellularPower::test_AT_CellularPower_on()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    CHECK(NSAPI_ERROR_UNSUPPORTED == pow.on());
}

void Test_AT_CellularPower::test_AT_CellularPower_off()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    CHECK(NSAPI_ERROR_UNSUPPORTED == pow.off());
}

void Test_AT_CellularPower::test_AT_CellularPower_set_at_mode()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.set_at_mode());
}

void Test_AT_CellularPower::test_AT_CellularPower_set_power_level()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.set_power_level(6));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.set_power_level(1,1));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.set_power_level(1,0));
}

void Test_AT_CellularPower::test_AT_CellularPower_reset()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.reset());
}

void Test_AT_CellularPower::test_AT_CellularPower_opt_power_save_mode()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(0,0));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(10,0));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(912,0));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(1834,1834));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(18345,18345));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(101234,101234));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_power_save_mode(1012345,1012345));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == pow.opt_power_save_mode(39612345,39612345));
}

void Test_AT_CellularPower::test_AT_CellularPower_opt_receive_period()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.opt_receive_period(1, CellularPower::EDRXUTRAN_Iu_mode, 3));
}

void Test_AT_CellularPower::test_AT_CellularPower_is_device_ready()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == pow.is_device_ready());
}

static void device_ready_cb()
{
}

void Test_AT_CellularPower::test_AT_CellularPower_set_device_ready_urc_cb()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    CHECK(NSAPI_ERROR_UNSUPPORTED == pow.set_device_ready_urc_cb(&device_ready_cb));
}

void Test_AT_CellularPower::test_AT_CellularPower_remove_device_ready_urc_cb()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularPower pow(at);
    CHECK(NSAPI_ERROR_UNSUPPORTED == pow.set_device_ready_urc_cb(&device_ready_cb));

    pow.remove_device_ready_urc_cb(NULL);
    pow.remove_device_ready_urc_cb(&device_ready_cb);
}
