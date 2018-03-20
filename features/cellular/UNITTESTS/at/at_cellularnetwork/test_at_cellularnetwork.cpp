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
#include "test_at_cellularnetwork.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularDevice.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "ATHandler_stub.h"

using namespace mbed;
using namespace events;

class my_AT_CN : public AT_CellularNetwork {
public:
    my_AT_CN(ATHandler &atHandler) : AT_CellularNetwork(atHandler) {}
    virtual ~my_AT_CN() {}
    NetworkStack *get_stack() {return AT_CellularNetwork::get_stack();}
};

void conn_stat_cb(nsapi_error_t error)
{

}

Test_AT_CellularNetwork::Test_AT_CellularNetwork()
{
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = -1;
}

Test_AT_CellularNetwork::~Test_AT_CellularNetwork()
{
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork *cn = new AT_CellularNetwork(at);

    delete cn;
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_credentials()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP));

    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn"));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_connect()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    cn.set_stack_type(IPV4V6_STACK);
    CHECK(NSAPI_ERROR_NO_CONNECTION == cn.connect("APN", "a", "b"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_NO_CONNECTION == cn.connect("APN"));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_disconnect()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(NSAPI_ERROR_OK == cn.disconnect());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_stack()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    my_AT_CN cn(at);
    CHECK(!cn.get_stack());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_registration()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.set_registration());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_registration_status()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::RegistrationStatus stat;
    CHECK(NSAPI_ERROR_OK == cn.get_registration_status(CellularNetwork::C_EREG, stat));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_attach()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.set_attach());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_attach()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::AttachStatus stat;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.get_attach(stat));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_rate_control()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int ur;
    CellularNetwork::RateControlExceptionReports reports;
    CellularNetwork::RateControlUplinkTimeUnit timeUnit;
    CHECK(NSAPI_ERROR_OK == cn.get_rate_control(reports, timeUnit, ur));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_apn_backoff_timer()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int time;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.get_apn_backoff_timer(time));
}


void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_ip_address()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(!cn.get_ip_address());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_access_technology()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::operator_t::RAT_UNKNOWN));
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::operator_t::RAT_GSM_COMPACT));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_scan_plmn()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int c;
    CellularNetwork::operList_t ops;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.scan_plmn(ops, c));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_ciot_optimization_config()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.set_ciot_optimization_config(CellularNetwork::SUPPORTED_UE_OPT_NO_SUPPORT, CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_ciot_optimization_config()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::Supported_UE_Opt sup;
    CellularNetwork::Preferred_UE_Opt pref;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.get_ciot_optimization_config(sup, pref));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_stack_type()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(NSAPI_ERROR_PARAMETER == cn.set_stack_type(IPV4_STACK));

    CHECK(NSAPI_ERROR_OK == cn.set_stack_type(DEFAULT_STACK));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_stack_type()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(DEFAULT_STACK == cn.get_stack_type());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_pdpcontext_params()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::pdpContextList_t list;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.get_pdpcontext_params(list));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_extended_signal_quality()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int rx,be,rs,ec,rsrq,rsrp;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_extended_signal_quality(rx, be,rs,ec,rsrq, rsrp));

    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == cn.get_extended_signal_quality(rx, be,rs,ec,rsrq, rsrp));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_signal_quality()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int rs,ber;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_signal_quality(rs,ber));

    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == cn.get_signal_quality(rs,ber));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_cell_id()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int id;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.get_cell_id(id));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_3gpp_error()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::int_value = 8;
    CHECK(8 == cn.get_3gpp_error());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_operator_params()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int format;
    CellularNetwork::operator_t ops;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_CONNECTION_LOST;
    CHECK(NSAPI_ERROR_CONNECTION_LOST == cn.get_operator_params(format, ops));
}

