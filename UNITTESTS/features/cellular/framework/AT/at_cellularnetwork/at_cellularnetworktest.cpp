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
#include "AT_CellularDevice.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "ATHandler_stub.h"
#include "AT_CellularStack.h"
#include "myCellularDevice.h"
#include "CellularDevice_stub.h"
#include "AT_CellularDevice_stub.h"

using namespace mbed;
using namespace events;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularNetwork : public testing::Test {
protected:

    void SetUp()
    {
        ATHandler_stub::int_count = kRead_int_table_size;
        ATHandler_stub::read_string_index = kRead_string_table_size;
        ATHandler_stub::resp_stop_success_count = kResp_stop_count_default;
        ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
        ATHandler_stub::int_value = -1;
        AT_CellularDevice_stub::supported_bool = false;

        _dev = new myCellularDevice(&_fh);
    }

    void TearDown()
    {
        delete _dev;
        AT_CellularDevice_stub::supported_bool = false;
    }

    FileHandle_stub _fh;
    AT_CellularDevice *_dev;
};


class my_AT_CN : public AT_CellularNetwork {
public:
    my_AT_CN(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device) {}
    virtual ~my_AT_CN() {}
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology op_rat)
    {
        return NSAPI_ERROR_OK;
    }
};

static int network_cb_count;
static void network_cb(nsapi_event_t ev, intptr_t intptr)
{
    network_cb_count++;
}

TEST_F(TestAT_CellularNetwork, Create)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork *cn = new AT_CellularNetwork(at, *_dev);
    EXPECT_TRUE(cn != NULL);
    delete cn;
}

int expected_rat = 0;
int expected_status = 0;
int expected_cellid = 0;

void status_cb_urc(nsapi_event_t ev, intptr_t ptr)
{
    const cell_callback_data_t *data = (const cell_callback_data_t *)ptr;
    switch (ev) {
    case CellularRadioAccessTechnologyChanged:
        EXPECT_EQ(NSAPI_ERROR_OK, data->error);
        EXPECT_EQ(expected_rat, data->status_data);
        break;
    case CellularRegistrationStatusChanged:
        EXPECT_EQ(NSAPI_ERROR_OK, data->error);
        EXPECT_EQ(expected_status, data->status_data);
        break;
    case CellularCellIDChanged:
        EXPECT_EQ(NSAPI_ERROR_OK, data->error);
        EXPECT_EQ(expected_cellid, data->status_data);
        break;
    default:
        if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE) {
            EXPECT_EQ(NSAPI_STATUS_DISCONNECTED, (int)ptr);
        } else {
            FAIL();
        }
    }
}

TEST_F(TestAT_CellularNetwork, test_urc_creg)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    cn.attach(status_cb_urc);

    EXPECT_STREQ("+CEREG:", ATHandler_stub::urc_handlers[0].urc);
    EXPECT_STREQ("+CREG:", ATHandler_stub::urc_handlers[1].urc);

    // Connected to home network
    expected_rat = CellularNetwork::RAT_NB1;
    expected_status = CellularNetwork::RegisteredHomeNetwork;
    expected_cellid = 305463233;

    ATHandler_stub::int_count = 4;
    ATHandler_stub::int_valid_count_table[3] = 1; // [1] STAT, Registered to home network
    ATHandler_stub::int_valid_count_table[2] = 9; // [4] ACT, NB-IoT
    ATHandler_stub::int_valid_count_table[1] = 1; // [5] cause_type, skipped
    ATHandler_stub::int_valid_count_table[0] = 1; // [6] reject_cause, skipped

    ATHandler_stub::read_string_index = 4;
    ATHandler_stub::read_string_table[3] = "00C3"; // [2] LAC, 195
    ATHandler_stub::read_string_table[2] = "1234FFC1"; // [3] ci, 305463233
    ATHandler_stub::read_string_table[1] = "00100100"; // [7] active time
    ATHandler_stub::read_string_table[0] = "01000111"; // [8] periodic-tau

    ATHandler_stub::urc_handlers[0].cb();

    // Disconnected
    expected_rat = CellularNetwork::RAT_NB1;
    expected_status = CellularNetwork::NotRegistered;
    expected_cellid = 0;

    ATHandler_stub::int_count = 4;
    ATHandler_stub::int_valid_count_table[3] = 0; // [1] STAT, Not reqistered
    ATHandler_stub::int_valid_count_table[2] = 9; // [4] ACT, NB-IoT
    ATHandler_stub::int_valid_count_table[1] = 1; // [5] cause_type, skipped
    ATHandler_stub::int_valid_count_table[0] = 1; // [6] reject_cause, skipped

    ATHandler_stub::read_string_index = 4;
    ATHandler_stub::read_string_table[3] = "0000"; // [2] LAC, 0000
    ATHandler_stub::read_string_table[2] = "00000000"; // [3] ci, 000000000
    ATHandler_stub::read_string_table[1] = "00100100"; // [7] active time
    ATHandler_stub::read_string_table[0] = "01000111"; // [8] periodic-tau

    ATHandler_stub::urc_handlers[0].cb();
    ATHandler_stub::read_string_index = kRead_string_table_size;
    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = 0;
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_registration)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_registration());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration("12345"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_registration("12345"));
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_registration_params)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = 3;
    CellularNetwork::registration_params_t reg_params;
    CellularNetwork::registration_params_t reg_params_check;

    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_registration_params(CellularNetwork::C_EREG, reg_params));
    EXPECT_TRUE(reg_params._status == CellularNetwork::RegistrationDenied);
    EXPECT_TRUE(reg_params._act == CellularNetwork::RAT_EGPRS);
    EXPECT_TRUE(reg_params._cell_id == -1);
    EXPECT_TRUE(reg_params._lac == -1);

    ATHandler_stub::read_string_index = 4;
    ATHandler_stub::read_string_table[3] = "00C3";
    ATHandler_stub::read_string_table[2] = "1234FFC1";//==  cellid and in dec: 305463233
    ATHandler_stub::read_string_table[1] = "00100100";
    ATHandler_stub::read_string_table[0] = "01000111";
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_registration_params(CellularNetwork::C_EREG, reg_params));
    EXPECT_TRUE(reg_params._lac == 0xc3);
    EXPECT_TRUE(reg_params._cell_id == 305463233);
    EXPECT_TRUE(reg_params._active_time == 240);
    EXPECT_TRUE(reg_params._periodic_tau == 70 * 60 * 60);
    ATHandler_stub::read_string_index = kRead_string_table_size;
    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = 0;
    // Check get_registration_params without specifying the registration type
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_registration_params(reg_params_check));
    ASSERT_EQ(reg_params_check._status, CellularNetwork::RegistrationDenied);
    ASSERT_EQ(reg_params_check._act, CellularNetwork::RAT_EGPRS);
    ASSERT_EQ(reg_params_check._cell_id, 305463233);
    ASSERT_EQ(reg_params_check._active_time, 240);
    ASSERT_EQ(reg_params_check._periodic_tau, 70 * 60 * 60);

    reg_params._status = CellularNetwork::NotRegistered;
    reg_params._act = CellularNetwork::RAT_GSM;
    reg_params._cell_id = 1;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_registration_params(CellularNetwork::C_REG, reg_params));
    ASSERT_EQ(reg_params._status, CellularNetwork::RegistrationDenied);
    ASSERT_EQ(reg_params._act, CellularNetwork::RAT_EGPRS);
    ASSERT_EQ(reg_params._cell_id, -1);

    my_AT_CN nw(at, *_dev);
    reg_params._status = CellularNetwork::NotRegistered;
    reg_params._act = CellularNetwork::RAT_GSM;

    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, nw.get_registration_params(CellularNetwork::C_GREG, reg_params));
    ASSERT_EQ(reg_params._status, CellularNetwork::NotRegistered);
    ASSERT_EQ(reg_params._act, CellularNetwork::RAT_GSM);

    ASSERT_EQ(NSAPI_ERROR_OK, nw.get_registration_params(CellularNetwork::C_EREG, reg_params));
    ASSERT_EQ(reg_params._status, CellularNetwork::RegistrationDenied);
    ASSERT_EQ(reg_params._act, CellularNetwork::RAT_EGPRS);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    reg_params._status = CellularNetwork::NotRegistered;
    reg_params._act = CellularNetwork::RAT_GSM;
    reg_params._cell_id = 1;
    reg_params._active_time = 2;
    reg_params._periodic_tau = 3;

    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.get_registration_params(CellularNetwork::C_EREG, reg_params));
    ASSERT_EQ(reg_params._status, CellularNetwork::NotRegistered);
    ASSERT_EQ(reg_params._act, CellularNetwork::RAT_UNKNOWN);
    EXPECT_TRUE(reg_params._cell_id == -1 && reg_params._active_time == -1 && reg_params._periodic_tau == -1);

    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.get_registration_params(CellularNetwork::C_REG, reg_params));
    ASSERT_EQ(reg_params._status, CellularNetwork::NotRegistered);
    ASSERT_EQ(reg_params._act, CellularNetwork::RAT_UNKNOWN);
    ASSERT_EQ(reg_params._cell_id, -1);

    reg_params._status = CellularNetwork::SearchingNetwork;
    reg_params._act = CellularNetwork::RAT_GSM;
    reg_params._cell_id = 1;
    reg_params._active_time = 2;
    reg_params._periodic_tau = 3;

    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == nw.get_registration_params(CellularNetwork::C_EREG, reg_params));
    EXPECT_TRUE(reg_params._status == CellularNetwork::NotRegistered);
    EXPECT_TRUE(reg_params._act == CellularNetwork::RAT_UNKNOWN);
    EXPECT_TRUE(reg_params._cell_id == -1 && reg_params._active_time == -1 && reg_params._periodic_tau == -1);
    // Check get_registration_params without specifying the registration type
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_registration_params(reg_params_check));
    EXPECT_TRUE(reg_params_check._status == CellularNetwork::NotRegistered);
    EXPECT_TRUE(reg_params_check._act == CellularNetwork::RAT_UNKNOWN);
    EXPECT_TRUE(reg_params_check._cell_id == -1 && reg_params_check._active_time == -1 && reg_params_check._periodic_tau == -1);
}

static int disconnect_cb_count = 0;
static bool disconnect_cb_param_check = false;
static void disconnect_cb(nsapi_event_t ev, intptr_t intptr)
{
    disconnect_cb_count++;

    if (disconnect_cb_count == 3 && disconnect_cb_param_check) {
        EXPECT_TRUE(ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE);
        EXPECT_TRUE(intptr == NSAPI_STATUS_DISCONNECTED);
    }
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_registration_status_change)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);


    cn.attach(&disconnect_cb);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::process_oob_urc = true;

    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[2] = "+CREG:";
    ATHandler_stub::read_string_table[1] = "00C3";
    ATHandler_stub::read_string_table[0] = "A13F";
    ATHandler_stub::int_count = 2;
    //ATHandler_stub::int_valid_count_table[2] = 1; //URC status is skipped
    ATHandler_stub::int_valid_count_table[1] = 0; //not registered
    ATHandler_stub::int_valid_count_table[0] = 1;
    at.process_oob();

    disconnect_cb_count = 0;
    disconnect_cb_param_check = true;
    ATHandler_stub::read_string_index = 4;
    ATHandler_stub::read_string_table[4] = "+CREG:";
    ATHandler_stub::read_string_table[3] = "00C3";
    ATHandler_stub::read_string_table[2] = "A13F";
    ATHandler_stub::read_string_table[1] = "FF";
    ATHandler_stub::read_string_table[0] = "FF";
    ATHandler_stub::int_count = 2;
    //ATHandler_stub::int_valid_count_table[2] = 1; //URC status is skipped
    ATHandler_stub::int_valid_count_table[1] = 1; //registered, home network
    ATHandler_stub::int_valid_count_table[0] = 1;
    at.process_oob();

    ATHandler_stub::read_string_index = 5;
    ATHandler_stub::int_count = 3;
    ATHandler_stub::read_string_index = 4;
    ATHandler_stub::read_string_table[4] = "+CREG:";
    ATHandler_stub::read_string_table[3] = "00C3";
    ATHandler_stub::read_string_table[2] = "A13F";
    ATHandler_stub::read_string_table[1] = "FF";
    ATHandler_stub::read_string_table[0] = "FF";
    ATHandler_stub::int_count = 2;
    //ATHandler_stub::int_valid_count_table[2] = 1; //URC status is skipped
    ATHandler_stub::int_valid_count_table[1] = 4; //unknown registration status
    ATHandler_stub::int_valid_count_table[0] = 1;
    at.process_oob();
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_network_registering_mode)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);

    ATHandler_stub::int_value = 0;
    CellularNetwork::NWRegisteringMode mode = CellularNetwork::NWModeManual;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_network_registering_mode(mode));
    EXPECT_TRUE(mode == CellularNetwork::NWModeAutomatic);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    mode = CellularNetwork::NWModeManual;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_network_registering_mode(mode));
    EXPECT_TRUE(mode == -1);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_registration_urc)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);

    CellularNetwork::RegistrationType type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, cn.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.set_registration_urc(type, true));

    my_AT_CN nw(at, *_dev);
    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_OK, nw.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, nw.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_OK, nw.set_registration_urc(type, true));

    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, cn.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.set_registration_urc(type, false));

    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_OK, nw.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, nw.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_OK, nw.set_registration_urc(type, false));


    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, cn.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.set_registration_urc(type, true));

    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, nw.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, nw.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, nw.set_registration_urc(type, true));

    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, cn.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.set_registration_urc(type, false));

    type = CellularNetwork::C_EREG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, nw.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    ASSERT_EQ(NSAPI_ERROR_UNSUPPORTED, nw.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, nw.set_registration_urc(type, false));
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_attach)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::int_value = 0;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_attach());
    ATHandler_stub::int_value = 1;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_attach());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_attach());
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_attach)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    CellularNetwork::AttachStatus stat = CellularNetwork::Detached;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::bool_value = true;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_attach(stat));
    EXPECT_TRUE(stat == CellularNetwork::Attached);

    ATHandler_stub::int_value = 0;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_attach(stat));
    EXPECT_TRUE(stat == CellularNetwork::Detached);

    stat = CellularNetwork::Attached;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_attach(stat));
    EXPECT_TRUE(stat == CellularNetwork::Detached);


    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_attach(stat));
    EXPECT_TRUE(stat == CellularNetwork::Detached);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::bool_value = false;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_attach(stat));
    EXPECT_TRUE(stat == CellularNetwork::Detached);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_detach)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    my_AT_CN cn(at, *_dev);

    EXPECT_TRUE(NSAPI_ERROR_OK == cn.detach());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.detach());
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_access_technology)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::RAT_UNKNOWN));
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::RAT_GSM_COMPACT));

    my_AT_CN my_cn(at, *_dev);
    EXPECT_TRUE(NSAPI_ERROR_OK == my_cn.set_access_technology(CellularNetwork::RAT_GSM_COMPACT));
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_scan_plmn)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    int c = -1;
    CellularNetwork::operList_t ops;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.scan_plmn(ops, c));
    EXPECT_TRUE(c == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.scan_plmn(ops, c));
    EXPECT_TRUE(c == 0);


    ATHandler_stub::read_string_index = 3;
    ATHandler_stub::read_string_table[0] = (char *)"44444";
    ATHandler_stub::read_string_table[1] = (char *)"33333";
    ATHandler_stub::read_string_table[2] = (char *)"12345";
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::info_elem_true_counter = 1;
    ATHandler_stub::bool_value = false;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.scan_plmn(ops, c));
    EXPECT_TRUE(c == 1);
    EXPECT_TRUE(ops.get_head() != NULL);
    CellularNetwork::operator_t *op = ops.get_head();
    EXPECT_TRUE(op->op_status == CellularNetwork::operator_t::Available);
    EXPECT_TRUE(strcmp(op->op_long, "12345") == 0);
    EXPECT_TRUE(strcmp(op->op_short, "33333") == 0);
    EXPECT_TRUE(strcmp(op->op_num, "44444") == 0);
    ops.delete_all();

    ATHandler_stub::read_string_index = 3;
    ATHandler_stub::int_value = 6; // RAT_HSDPA_HSUPA
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::info_elem_true_counter = 1;
    ATHandler_stub::bool_value = false;
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::RAT_UTRAN));
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.scan_plmn(ops, c));
    EXPECT_TRUE(c == 1);
    EXPECT_TRUE(ops.get_head() != NULL);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_ciot_optimization_config)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_ciot_optimization_config(CellularNetwork::CIOT_OPT_NO_SUPPORT, CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE, NULL));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_ciot_optimization_config(CellularNetwork::CIOT_OPT_NO_SUPPORT, CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE, NULL));
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_ciot_ue_optimization_config)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    CellularNetwork::CIoT_Supported_Opt sup = CellularNetwork::CIOT_OPT_NO_SUPPORT;
    CellularNetwork::CIoT_Preferred_UE_Opt pref = CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_ciot_ue_optimization_config(sup, pref));
    EXPECT_TRUE(sup == CellularNetwork::CIOT_OPT_CONTROL_PLANE);
    EXPECT_TRUE(pref == CellularNetwork::PREFERRED_UE_OPT_CONTROL_PLANE);

    sup = CellularNetwork::CIOT_OPT_NO_SUPPORT;
    pref = CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::nsapi_error_ok_counter = 0;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_ciot_ue_optimization_config(sup, pref));
    EXPECT_TRUE(sup == CellularNetwork::CIOT_OPT_NO_SUPPORT);
    EXPECT_TRUE(pref == CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_ciot_network_optimization_config)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    CellularNetwork::CIoT_Supported_Opt sup = CellularNetwork::CIOT_OPT_NO_SUPPORT;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_ciot_network_optimization_config(sup));
    EXPECT_TRUE(sup == CellularNetwork::CIOT_OPT_MAX);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_signal_quality)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    int rs = -1, ber = -1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_signal_quality(rs, &ber));
    EXPECT_TRUE(rs == -1 && ber == -1);

    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_signal_quality(rs, &ber));
    EXPECT_EQ(rs, -111);
    EXPECT_EQ(ber, 1);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_3gpp_error)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::int_value = 8;
    EXPECT_TRUE(8 == cn.get_3gpp_error());
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_operator_params)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    int format;
    CellularNetwork::operator_t ops;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_operator_params(format, ops));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = 0;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = (char *)"12345";
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_operator_params(format, ops));
    EXPECT_TRUE(format == 0);
    EXPECT_TRUE(strcmp(ops.op_long, "12345") == 0);
    EXPECT_TRUE(strlen(ops.op_short) == 0);
    EXPECT_TRUE(strlen(ops.op_num) == 0);
    EXPECT_TRUE(ops.op_rat == CellularNetwork::RAT_GSM);

    ops.op_long[0] = 0;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = (char *)"12345";
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_operator_params(format, ops));
    EXPECT_TRUE(format == 1);
    EXPECT_TRUE(strlen(ops.op_long) == 0);
    EXPECT_TRUE(strcmp(ops.op_short, "12345") == 0);
    EXPECT_TRUE(strlen(ops.op_num) == 0);
    EXPECT_TRUE(ops.op_rat == CellularNetwork::RAT_GSM_COMPACT);

    ops.op_short[0] = 0;
    ATHandler_stub::int_value = 2;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = (char *)"12345";
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_operator_params(format, ops));
    EXPECT_TRUE(format == 2);
    EXPECT_TRUE(strlen(ops.op_long) == 0);
    EXPECT_TRUE(strlen(ops.op_short) == 0);
    EXPECT_TRUE(strcmp(ops.op_num, "12345") == 0);
    EXPECT_TRUE(ops.op_rat == CellularNetwork::RAT_UTRAN);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_get_operator_names)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    CellularNetwork::operator_names_list name_list;

    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::bool_value = false;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_operator_names(name_list));
    EXPECT_TRUE(name_list.get_head() == NULL);

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_operator_names(name_list));
    EXPECT_TRUE(name_list.get_head() != NULL);
    EXPECT_TRUE(name_list.get_head()->next == NULL);

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[1] = (char *)"12345";
    ATHandler_stub::read_string_table[0] = (char *)"56789";
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    name_list.delete_all();
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_operator_names(name_list));
    CellularNetwork::operator_names_t *op_names = name_list.get_head();
    EXPECT_TRUE(op_names != NULL);
    EXPECT_TRUE(op_names->next == NULL);
    EXPECT_TRUE(strcmp(op_names->numeric, "12345") == 0);
    EXPECT_TRUE(strcmp(op_names->alpha, "56789") == 0);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_attach)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    network_cb_count = 0;
    cn.attach(&network_cb);
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_receive_period)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_receive_period(1, CellularNetwork::EDRXUTRAN_Iu_mode, 3));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_receive_period(1, CellularNetwork::EDRXUTRAN_Iu_mode, 3));
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_set_packet_domain_event_reporting)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == cn.set_packet_domain_event_reporting(true));
    AT_CellularDevice_stub::supported_bool = true;

    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_packet_domain_event_reporting(true));
    EXPECT_TRUE(NSAPI_ERROR_OK == cn.set_packet_domain_event_reporting(false));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_packet_domain_event_reporting(true));
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.set_packet_domain_event_reporting(false));
}

TEST_F(TestAT_CellularNetwork, test_AT_CellularNetwork_is_active_context)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at, *_dev);

    // No contexts
    int active_contexts = -1;
    EXPECT_FALSE(cn.is_active_context(&active_contexts));
    EXPECT_EQ(0, active_contexts);

    // Active contexts
    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_count = 4;
    ATHandler_stub::int_valid_count_table[3] = 0; // ctx 0
    ATHandler_stub::int_valid_count_table[2] = 0; // ctx 0 inactive
    ATHandler_stub::int_valid_count_table[1] = 1; // ctx 1
    ATHandler_stub::int_valid_count_table[0] = 1; // ctx 1 active

    EXPECT_TRUE(cn.is_active_context(&active_contexts));
    EXPECT_EQ(1, active_contexts);

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_count = 4;
    EXPECT_FALSE(cn.is_active_context(&active_contexts, 0));
    EXPECT_EQ(1, active_contexts);

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_count = 4;
    EXPECT_TRUE(cn.is_active_context(&active_contexts, 1));
    EXPECT_EQ(1, active_contexts);

    ATHandler_stub::resp_info_true_counter = 2;
    ATHandler_stub::int_count = 4;
    EXPECT_TRUE(cn.is_active_context(NULL, 1));
    EXPECT_EQ(1, active_contexts);
}
