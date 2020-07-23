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
#include "ATHandler_stub.h"
#include "events/EventQueue.h"
#include "FileHandle_stub.h"
#include "CellularDevice_stub.h"
#include "AT_CellularDevice_stub.h"
#include "ATHandler.h"
#include "myCellularDevice.h"
#include "AT_CellularInformation.h"

using namespace mbed;
using namespace events;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularInformation : public testing::Test {
protected:

    void SetUp()
    {
        ATHandler_stub::read_string_index = kRead_string_table_size;
        ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
        ATHandler_stub::read_string_value = NULL;
        ATHandler_stub::ssize_value = 0;

        _dev = new myCellularDevice(&_fh);
    }

    void TearDown()
    {
        delete _dev;
    }

    FileHandle_stub _fh;
    AT_CellularDevice *_dev;
};
// *INDENT-ON*

TEST_F(TestAT_CellularInformation, Create)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation *unit = new AT_CellularInformation(ah, *_dev);
    EXPECT_TRUE(unit != NULL);
    delete unit;
}

TEST_F(TestAT_CellularInformation, test_AT_CellularInformation_get_manufacturer)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah, *_dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"some";
    ATHandler_stub::ssize_value = 4;

    char buf[8];
    EXPECT_TRUE(NSAPI_ERROR_OK == aci.get_manufacturer(buf, 8));
    EXPECT_TRUE(strcmp("some", buf) == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::ssize_value = -1;
    buf[0] = 0;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == aci.get_manufacturer(buf, 8));
    EXPECT_TRUE(strlen(buf) == 0);
}

TEST_F(TestAT_CellularInformation, test_AT_CellularInformation_get_model)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah, *_dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"model";
    ATHandler_stub::ssize_value = 5;
    char buf[8];
    EXPECT_TRUE(NSAPI_ERROR_OK == aci.get_model(buf, 8));
    EXPECT_TRUE(strcmp("model", buf) == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::ssize_value = -1;
    buf[0] = 0;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == aci.get_model(buf, 8));
    EXPECT_TRUE(strlen(buf) == 0);
}

TEST_F(TestAT_CellularInformation, test_AT_CellularInformation_get_revision)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");

    //Used heap var here to visit heap constructor
    AT_CellularInformation *aci = new AT_CellularInformation(ah, *_dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"revision";
    ATHandler_stub::ssize_value = 8;

    char buf[9];
    EXPECT_TRUE(NSAPI_ERROR_OK == aci->get_revision(buf, 9));
    EXPECT_TRUE(strcmp("revision", buf) == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::ssize_value = -1;
    buf[0] = 0;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == aci->get_revision(buf, 8));
    EXPECT_TRUE(strlen(buf) == 0);

    EXPECT_TRUE(NSAPI_ERROR_PARAMETER == aci->get_revision(NULL, 8));
    EXPECT_TRUE(NSAPI_ERROR_PARAMETER == aci->get_revision(buf, 0));
    delete aci;
}

TEST_F(TestAT_CellularInformation, test_AT_CellularInformation_get_serial_number)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah, *_dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"1234567";
    ATHandler_stub::ssize_value = 7;
    char buf[8];

    EXPECT_TRUE(NSAPI_ERROR_OK == aci.get_serial_number(buf, 8, CellularInformation::SN));
    EXPECT_TRUE(strcmp("1234567", buf) == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::ssize_value = -1;
    buf[0] = 0;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == aci.get_serial_number(buf, 8, CellularInformation::SN));
    EXPECT_TRUE(strlen(buf) == 0);

    AT_CellularDevice_stub::supported_bool = false;
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == aci.get_serial_number(buf, 8, CellularInformation::IMEI));
    EXPECT_TRUE(strlen(buf) == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"1234567";
    ATHandler_stub::ssize_value = 7;
    AT_CellularDevice_stub::supported_bool = true;
    EXPECT_TRUE(NSAPI_ERROR_OK == aci.get_serial_number(buf, 8, CellularInformation::IMEI));
    EXPECT_TRUE(strcmp("1234567", buf) == 0);

    EXPECT_TRUE(NSAPI_ERROR_PARAMETER == aci.get_serial_number(NULL, 8, CellularInformation::IMEI));
    EXPECT_TRUE(NSAPI_ERROR_PARAMETER == aci.get_serial_number(buf, 0, CellularInformation::IMEI));
}

TEST_F(TestAT_CellularInformation, TestAT_CellularInformation_get_imsi)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah, *_dev);

    char imsi[16];
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"123456789012345";
    ATHandler_stub::ssize_value = 15;
    ASSERT_EQ(NSAPI_ERROR_OK, aci.get_imsi(imsi, sizeof(imsi)));
    ASSERT_STREQ(ATHandler_stub::read_string_value, imsi);

    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = -1;
    ATHandler_stub::read_string_index = -1;
    imsi[0] = 0;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, aci.get_imsi(imsi, sizeof(imsi)));
    ASSERT_EQ(strlen(imsi), 0);

    ASSERT_EQ(NSAPI_ERROR_PARAMETER, aci.get_imsi(NULL, sizeof(imsi)));

    char imsi2[5];
    ASSERT_EQ(NSAPI_ERROR_PARAMETER, aci.get_imsi(imsi2, sizeof(imsi2)));
}

TEST_F(TestAT_CellularInformation, TestAT_CellularInformation_get_iccid)
{
    EventQueue eq;
    FileHandle_stub fh;
    ATHandler ah(&fh, eq, 0, ",");
    AT_CellularInformation aci(ah, *_dev);

    char buf[16];
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"123456789012345";
    ATHandler_stub::ssize_value = 15;
    ASSERT_EQ(NSAPI_ERROR_OK,  aci.get_iccid(buf, 16));
    ASSERT_STREQ(ATHandler_stub::read_string_value, buf);

    buf[0] = 0;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = -1;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, aci.get_iccid(buf, 16));
    ASSERT_EQ(strlen(buf), 0);

    ASSERT_EQ(NSAPI_ERROR_PARAMETER, aci.get_iccid(buf, 0));
    ASSERT_EQ(NSAPI_ERROR_PARAMETER, aci.get_iccid(NULL, 16));
}
