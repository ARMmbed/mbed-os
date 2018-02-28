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
#include "test_at_cellularbase.h"
#include "EventQueue.h"
#include "AT_CellularBase.h"
#include "ATHandler_stub.h"
#include "FileHandle_stub.h"
#include <string.h>

using namespace mbed;
using namespace events;

Test_AT_CellularBase::Test_AT_CellularBase()
{

}

Test_AT_CellularBase::~Test_AT_CellularBase()
{
}

void Test_AT_CellularBase::test_AT_CellularBase_get_at_handler()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 100, ",");
    AT_CellularBase at(ah);

    CHECK(&ah == &at.get_at_handler());
}

void Test_AT_CellularBase::test_AT_CellularBase_get_device_error()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularBase at(ah);

    ATHandler_stub::device_err_value.errCode = 8;

    CHECK_EQUAL(8, at.get_device_error().errCode);

    ATHandler_stub::device_err_value.errCode = 0;
}
