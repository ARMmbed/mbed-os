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
#include "events/EventQueue.h"
#include "ATHandler.h"
#include "ATHandler_stub.h"
#include "AT_CellularSMS.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "myCellularDevice.h"

using namespace mbed;
using namespace events;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularSMS : public testing::Test {
protected:

    void SetUp()
    {
        ATHandler_stub::return_given_size = false;
        ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;

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

TEST_F(TestAT_CellularSMS, Create)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS *sms = new AT_CellularSMS(at, *_dev);

    EXPECT_TRUE(sms != NULL);
    delete sms;
}

void my_callback()
{

}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_initialize)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    ATHandler_stub::call_immediately = true;

    AT_CellularSMS sms(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_EQ(NSAPI_ERROR_AUTH_FAILURE, sms.initialize(CellularSMS::CellularSMSMmodeText));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_EQ(NSAPI_ERROR_OK, sms.initialize(CellularSMS::CellularSMSMmodeText));

    sms.set_sms_callback(&my_callback);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_EQ(NSAPI_ERROR_OK, sms.initialize(CellularSMS::CellularSMSMmodeText));

    ATHandler_stub::call_immediately = false;
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_send_sms)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    EXPECT_EQ(NSAPI_ERROR_PARAMETER, sms.send_sms(NULL, "2", 1));

    sms.initialize(CellularSMS::CellularSMSMmodeText);
    ATHandler_stub::size_value = 1;
    EXPECT_EQ(1, sms.send_sms("1", "22", 2));

    ATHandler_stub::size_value = 2;
    EXPECT_EQ(2, sms.send_sms("1", "22", 2));

    ATHandler_stub::return_given_size = true; // PDU mode write is much longer than than msg len
    sms.initialize(CellularSMS::CellularSMSMmodePDU);
    EXPECT_EQ(2, sms.send_sms("1", "23", 2));;

    ATHandler_stub::nsapi_error_ok_counter = 1;
    ATHandler_stub::size_value = 32;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_EQ(NSAPI_ERROR_AUTH_FAILURE, sms.send_sms("1", "23232323", 8));

    ATHandler_stub::nsapi_error_ok_counter = 2;
    ATHandler_stub::size_value = 32;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_EQ(NSAPI_ERROR_AUTH_FAILURE, sms.send_sms("1", "23232323", 8));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    char table[] = "232323232323232323232323232323232323232323232323232323\
                    232323232323232323232323232323232323232323232323232323\
                    232323232323232323232323232323232323232323232323232323\
                    23232323232323232323232323232323232323\0";

    EXPECT_EQ(strlen(table), sms.send_sms("1", table, strlen(table)));
    EXPECT_EQ(strlen(table), sms.send_sms("12", table, strlen(table)));
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_get_sms)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    char buf[16];
    char phone[21];
    char stamp[21];
    int size;
    EXPECT_TRUE(NSAPI_ERROR_PARAMETER == sms.get_sms(NULL, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::int_value = -1;
    EXPECT_TRUE(-1 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));

    //In below we are expecting the stub ATHandler info_resp() to respond
    //twice true, then once false, then again twice true so that the
    //below message contents would be read in AT_CellularSMS read_sms_from_index().
    //This is to avoid comparing empty date strings
    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::resp_info_false_counter = 1;
    ATHandler_stub::resp_info_true_counter2 = 2;
    ATHandler_stub::int_value = 11;
    ATHandler_stub::read_string_index = 4;
    ATHandler_stub::read_string_table[4] = "";
    ATHandler_stub::read_string_table[3] = "REC READ";
    ATHandler_stub::read_string_table[2] = "09/01/12,11:15:00+04";
    ATHandler_stub::read_string_table[1] = "REC READ";
    ATHandler_stub::read_string_table[0] = "24/12/12,11:15:00+04";

    EXPECT_TRUE(0 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));
    //TODO: Should make add_info to happen, before calling get_sms!
    ATHandler_stub::read_string_index = kRead_string_table_size;

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_value = 11;
    sms.initialize(CellularSMS::CellularSMSMmodePDU);
    EXPECT_TRUE(0 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_TRUE(NSAPI_ERROR_AUTH_FAILURE == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_set_sms_callback)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    sms.set_sms_callback(NULL);
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_set_cpms)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_TRUE(NSAPI_ERROR_AUTH_FAILURE == sms.set_cpms("2", "3", "4"));
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_set_csca)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_TRUE(NSAPI_ERROR_AUTH_FAILURE == sms.set_csca("2", 1));
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_set_cscs)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_TRUE(NSAPI_ERROR_AUTH_FAILURE == sms.set_cscs("2"));
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_delete_all_messages)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_TRUE(NSAPI_ERROR_AUTH_FAILURE == sms.delete_all_messages());
}

TEST_F(TestAT_CellularSMS, test_AT_CellularSMS_set_extra_sim_wait_time)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at, *_dev);
    sms.set_extra_sim_wait_time(56);
}
