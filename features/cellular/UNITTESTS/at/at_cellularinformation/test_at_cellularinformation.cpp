/*
 * Copyright (c) 2018 ARM. All rights reserved.
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
    ATHandler ah(&fh, eq, 0);
    AT_CellularInformation aci(ah);

    ATHandler_stub::nsapi_error_value = 8;

    char buf[8];
    CHECK(8 == aci.get_manufacturer(buf, 8));
}

void Test_AT_CellularInformation::test_AT_CellularInformation_get_model()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0);
    AT_CellularInformation aci(ah);

    ATHandler_stub::nsapi_error_value = 7;

    char buf[8];
    CHECK(7 == aci.get_model(buf, 8));
}

void Test_AT_CellularInformation::test_AT_CellularInformation_get_revision()
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0);

    //Used heap var here to visit heap constructor
    AT_CellularInformation *aci = new AT_CellularInformation(ah);

    ATHandler_stub::nsapi_error_value = 6;

    char buf[8];
    CHECK(6 == aci->get_revision(buf, 8));

    delete aci;
}

