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
#include "test_at_cellularinformation.h"
#include <string.h>
#include "ATHandler_stub.h"
#include "EventQueue.h"
#include "FileHandle_stub.h"
#include "ATHandler.h"
#include "AT_CellularInformation.h"
#include "AT_CellularBase.h"

using namespace mbed;
using namespace events;

Test_AT_CellularInformation::Test_AT_CellularInformation()
{

}

Test_AT_CellularInformation::~Test_AT_CellularInformation()
{
}

void Test_AT_CellularInformation::test_AT_CellularInformation_get_manufacturer()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah);

    ATHandler_stub::nsapi_error_value = 8;

    char buf[8];
    CHECK(8 == aci.get_manufacturer(buf, 8));
}

void Test_AT_CellularInformation::test_AT_CellularInformation_get_model()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah);

    ATHandler_stub::nsapi_error_value = 7;

    char buf[8];
    CHECK(7 == aci.get_model(buf, 8));
}

void Test_AT_CellularInformation::test_AT_CellularInformation_get_revision()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");

    //Used heap var here to visit heap constructor
    AT_CellularInformation *aci = new AT_CellularInformation(ah);

    ATHandler_stub::nsapi_error_value = 6;

    char buf[8];
    CHECK(6 == aci->get_revision(buf, 8));

    delete aci;
}

