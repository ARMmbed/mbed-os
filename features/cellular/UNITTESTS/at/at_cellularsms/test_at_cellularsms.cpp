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
#include "test_at_cellularsms.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "ATHandler_stub.h"
#include "AT_CellularSMS.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

Test_AT_CellularSMS::Test_AT_CellularSMS()
{
    ATHandler_stub::return_given_size = false;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
}

Test_AT_CellularSMS::~Test_AT_CellularSMS()
{
}

void Test_AT_CellularSMS::test_AT_CellularSMS_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS *sms = new AT_CellularSMS(at);

    delete sms;
}

void Test_AT_CellularSMS::test_AT_CellularSMS_initialize()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_NO_MEMORY == sms.initialize(CellularSMS::CellularSMSMmodeText));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_send_sms()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    LONGS_EQUAL(NSAPI_ERROR_PARAMETER, sms.send_sms(NULL, "2", 1));

    sms.initialize(CellularSMS::CellularSMSMmodeText);
    ATHandler_stub::size_value = 1;
    LONGS_EQUAL(1, sms.send_sms("1", "22", 2));

    ATHandler_stub::size_value = 2;
    LONGS_EQUAL(2, sms.send_sms("1", "22", 2));

    ATHandler_stub::return_given_size = true; // PDU mode write is much longer than than msg len
    sms.initialize(CellularSMS::CellularSMSMmodePDU);
    LONGS_EQUAL(2, sms.send_sms("1", "23", 2));;

    ATHandler_stub::nsapi_error_ok_counter = 1;
    ATHandler_stub::size_value = 32;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    LONGS_EQUAL(NSAPI_ERROR_AUTH_FAILURE, sms.send_sms("1", "23232323", 8));

    ATHandler_stub::nsapi_error_ok_counter = 2;
    ATHandler_stub::size_value = 32;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    LONGS_EQUAL(NSAPI_ERROR_AUTH_FAILURE, sms.send_sms("1", "23232323", 8));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    char table[] = "232323232323232323232323232323232323232323232323232323\
                    232323232323232323232323232323232323232323232323232323\
                    232323232323232323232323232323232323232323232323232323\
                    23232323232323232323232323232323232323\0";

    LONGS_EQUAL(strlen(table), sms.send_sms("1", table, strlen(table)));
    LONGS_EQUAL(strlen(table), sms.send_sms("12", table, strlen(table)));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_get_sms()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    char buf[16];
    char phone[21];
    char stamp[21];
    int size;
    CHECK(NSAPI_ERROR_PARAMETER == sms.get_sms(NULL, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::int_value = 0;
    CHECK(-1 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_value = 11;
    CHECK(0 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));
    //TODO: Should make add_info to happen, before calling get_sms!

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_value = 11;
    sms.initialize(CellularSMS::CellularSMSMmodePDU);
    CHECK(0 == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.get_sms(buf, 16, phone, 21, stamp, 21, &size));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_sms_callback()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    sms.set_sms_callback(NULL);
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_cpms()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.set_cpms("2", "3", "4"));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_csca()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.set_csca("2", 1));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_cscs()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.set_cscs("2"));
}


void Test_AT_CellularSMS::test_AT_CellularSMS_delete_all_messages()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sms.delete_all_messages());
}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_extra_sim_wait_time()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSMS sms(at);
    sms.set_extra_sim_wait_time(56);

}

