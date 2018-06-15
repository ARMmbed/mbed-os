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
#include "AT_CellularStack.h"

using namespace mbed;
using namespace events;

class my_stack :public AT_CellularStack {
public:
    my_stack(ATHandler &atHandler) : AT_CellularStack(atHandler, 1, IPV4_STACK) {}
    virtual int get_max_socket_count() { return 1;}
    virtual int get_max_packet_size() {return 200;}
    virtual bool is_protocol_supported(nsapi_protocol_t protocol) {return true;}
    virtual nsapi_error_t socket_close_impl(int sock_id) {return NSAPI_ERROR_OK;}
    virtual nsapi_error_t create_socket_impl(CellularSocket *socket) {return NSAPI_ERROR_OK;}
    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
            const void *data, nsapi_size_t size) {return 100;}
    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
            void *buffer, nsapi_size_t size) {return 100;}
};

class my_AT_CN : public AT_CellularNetwork {
public:
    my_AT_CN(ATHandler &atHandler) : AT_CellularNetwork(atHandler) {}
    virtual ~my_AT_CN() {}
    NetworkStack *get_stack() {
        if (!_stack) {
            return new my_stack(get_at_handler());
        } else {
            return _stack;
        }}
    virtual bool has_registration(RegistrationType reg_type) {
        if (reg_type == C_GREG) {
            return false;
        }
        return true;
    }
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology op_rat) { return NSAPI_ERROR_OK;}
    virtual bool get_modem_stack_type(nsapi_ip_stack_t requested_stack) {
        if (requested_stack == IPV4_STACK || requested_stack == DEFAULT_STACK) {
            return true;
        }
        return false;
    }
};

class my_AT_CNipv6 : public AT_CellularNetwork {
public:
    my_AT_CNipv6(ATHandler &atHandler) : AT_CellularNetwork(atHandler) {}
    virtual ~my_AT_CNipv6() {}
    NetworkStack *get_stack() {
        if (!_stack) {
            return new my_stack(get_at_handler());
        } else {
            return _stack;
        }}
    virtual bool has_registration(RegistrationType reg_type) {
        if (reg_type == C_GREG) {
            return false;
        }
        return true;
    }
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology op_rat) { return NSAPI_ERROR_OK;}
    virtual bool get_modem_stack_type(nsapi_ip_stack_t requested_stack) {
        if (requested_stack == IPV6_STACK || requested_stack == DEFAULT_STACK) {
            return true;
        }
        return false;
    }
};

static int network_cb_count;
static void network_cb(nsapi_event_t ev, intptr_t intptr)
{
    network_cb_count++;
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

void Test_AT_CellularNetwork::test_AT_CellularNetwork_init()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularNetwork cn(at);

    CHECK(NSAPI_ERROR_OK == cn.init());
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_NO_MEMORY;
    CHECK(NSAPI_ERROR_NO_MEMORY == cn.init());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_credentials()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn"));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, NULL));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, NULL, NULL));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, NULL, "passwd"));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, "user", NULL));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, "user"));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, "user", "passwd"));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", NULL));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", NULL, NULL));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", NULL, "passwd"));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", "user", NULL));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", "user"));
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", "user", "passwd"));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_activate_context()
{

    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    my_AT_CN my_cn(at);
    my_AT_CNipv6 my_cnipv6(at);

    // get_context return true and new context created. But now stack and so error.
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.activate_context());

    // get_context return true and new context created, also do_user_authentication called with success.
    // But now stack and so error.
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, "user", "passwd"));
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.activate_context());

    // get_context return true and new context created, also do_user_authentication called with failure.
    ATHandler_stub::resp_stop_success_count = 2;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_OK == cn.set_credentials("apn", CellularNetwork::CHAP, "user", "passwd"));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == cn.activate_context());


    // get_context return true and new context created, also do_user_authentication called with success.
    // Now there is stack.
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::resp_stop_success_count = kResp_stop_count_default;
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials("apn", CellularNetwork::CHAP, "user", "passwd"));
    CHECK(NSAPI_ERROR_OK == my_cn.activate_context());

     // get_context return true and new context created, test delete context
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::resp_stop_success_count = kResp_stop_count_default;
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials("apn", CellularNetwork::CHAP, "user", "passwd"));
    CHECK(NSAPI_ERROR_OK == my_cn.activate_context());



    // get_context pdp type gives zero len, fails with no stack
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = "";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.activate_context());

    // get_context pdp type gives proper type, apn reading fails
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = "IPV6";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_NO_CONNECTION == cn.activate_context());

    // get_context pdp type gives proper type, apn does not match, now other contexts so new one created
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IP";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials("apn", CellularNetwork::CHAP, "user", "passwd"));
    CHECK(NSAPI_ERROR_OK == my_cn.activate_context());

    // get_context pdp type gives proper type, apn match
    ATHandler_stub::resp_info_true_counter = 2; // set to 2 so cgact will give that this context is active
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IPV4V6";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == my_cn.set_stack_type(IPV4_STACK));
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials("internet"));
    CHECK(NSAPI_ERROR_OK == my_cn.activate_context());

    // get_context pdp type gives proper type, apn match
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IPV6";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == my_cnipv6.set_stack_type(IPV6_STACK));
    CHECK(NSAPI_ERROR_OK == my_cnipv6.set_credentials("internet"));
    CHECK(NSAPI_ERROR_OK == my_cnipv6.activate_context());

    // get_context pdp type gives proper type, apn match
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IPV4V6";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK ==my_cn.set_stack_type(DEFAULT_STACK));
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials("internet"));
    CHECK(NSAPI_ERROR_OK == my_cn.activate_context());

    // get_context pdp type gives proper type, apn match
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IPV4V6";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK ==my_cnipv6.set_stack_type(DEFAULT_STACK));
    CHECK(NSAPI_ERROR_OK == my_cnipv6.set_credentials("internet"));
    CHECK(NSAPI_ERROR_OK == my_cnipv6.activate_context());

    // get_context pdp type gives proper type, apn match
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IPV6";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK ==my_cnipv6.set_stack_type(DEFAULT_STACK));
    CHECK(NSAPI_ERROR_OK == my_cnipv6.set_credentials("internet"));
    CHECK(NSAPI_ERROR_OK == my_cnipv6.activate_context());

    // get_context pdp type gives proper type, apn match
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[0] = "internet";
    ATHandler_stub::read_string_table[1] = "IP";
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK ==my_cn.set_stack_type(DEFAULT_STACK));
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials("internet"));
    CHECK(NSAPI_ERROR_OK == my_cn.activate_context());

    // get_context pdp type gives proper type, apn match. Test Delete the created context.
    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::int_value = 1;
    //ATHandler_stub::nsapi_error_ok_counter = 2;
    ATHandler_stub::resp_stop_success_count = 2;
    CHECK(NSAPI_ERROR_OK == my_cn.set_credentials(NULL, NULL, NULL));
    CHECK(NSAPI_ERROR_NO_CONNECTION == my_cn.activate_context());

}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_connect()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    // no stack so will fail
    cn.attach(&network_cb);
    network_cb_count = 0;

    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.connect("APN", "a", "b"));
    CHECK(NSAPI_STATUS_DISCONNECTED == cn.get_connection_status());
    CHECK(network_cb_count == 2);

    network_cb_count = 0;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_NO_CONNECTION == cn.connect("APN"));
    CHECK(network_cb_count == 2);
    CHECK(NSAPI_STATUS_DISCONNECTED == cn.get_connection_status());

    my_AT_CN my_cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    cn.set_stack_type(IPV4_STACK);
    CHECK(NSAPI_ERROR_OK == my_cn.connect());
    CHECK(network_cb_count == 2);
    CHECK(NSAPI_STATUS_GLOBAL_UP == my_cn.get_connection_status());

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

    my_AT_CN my_cn(at);
    my_stack* mystack = (my_stack*)my_cn.get_stack();
    CHECK(mystack);
    delete mystack;
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_registration()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.set_registration());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration("12345"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.set_registration("12345"));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_registration_status()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = 3;
    CellularNetwork::RegistrationStatus stat = CellularNetwork::NotRegistered;
    CHECK(NSAPI_ERROR_OK == cn.get_registration_status(CellularNetwork::C_EREG, stat));
    CHECK(stat == CellularNetwork::RegistrationDenied);
    stat = CellularNetwork::NotRegistered;
    CHECK(NSAPI_ERROR_OK == cn.get_registration_status(CellularNetwork::C_GREG, stat));
    CHECK(stat == CellularNetwork::RegistrationDenied);

    my_AT_CN nw(at);
    stat = CellularNetwork::NotRegistered;
    CHECK(NSAPI_ERROR_UNSUPPORTED == nw.get_registration_status(CellularNetwork::C_GREG, stat));
    CHECK(stat == CellularNetwork::NotRegistered);
    CHECK(NSAPI_ERROR_OK == nw.get_registration_status(CellularNetwork::C_EREG, stat));
    CHECK(stat == CellularNetwork::RegistrationDenied);


    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    stat = CellularNetwork::NotRegistered;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_registration_status(CellularNetwork::C_EREG, stat));
    CHECK(stat == -1);
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_registration_status(CellularNetwork::C_GREG, stat));
    CHECK(stat == -1);

    stat = CellularNetwork::NotRegistered;
    CHECK(NSAPI_ERROR_UNSUPPORTED == nw.get_registration_status(CellularNetwork::C_GREG, stat));
    CHECK(stat == CellularNetwork::NotRegistered);
    CHECK(NSAPI_ERROR_DEVICE_ERROR == nw.get_registration_status(CellularNetwork::C_EREG, stat));
    CHECK(stat == -1);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_network_registering_mode()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);

    ATHandler_stub::int_value = 0;
    CellularNetwork::NWRegisteringMode mode = CellularNetwork::NWModeManual;
    CHECK(NSAPI_ERROR_OK == cn.get_network_registering_mode(mode));
    CHECK(mode == CellularNetwork::NWModeAutomatic);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    mode = CellularNetwork::NWModeManual;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_network_registering_mode(mode));
    CHECK(mode == -1);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_registration_urc()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);

    CellularNetwork::RegistrationType type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_OK == cn.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_OK == cn.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_OK == cn.set_registration_urc(type, true));

    my_AT_CN nw(at);
    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_OK == nw.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_UNSUPPORTED == nw.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_OK == nw.set_registration_urc(type, true));

    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_OK == cn.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_OK == cn.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_OK == cn.set_registration_urc(type, false));

    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_OK == nw.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_UNSUPPORTED == nw.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_OK == nw.set_registration_urc(type, false));


    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration_urc(type, true));

    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == nw.set_registration_urc(type, true));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_UNSUPPORTED == nw.set_registration_urc(type, true));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == nw.set_registration_urc(type, true));

    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_registration_urc(type, false));

    type = CellularNetwork::C_EREG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == nw.set_registration_urc(type, false));
    type = CellularNetwork::C_GREG;
    CHECK(NSAPI_ERROR_UNSUPPORTED == nw.set_registration_urc(type, false));
    type = CellularNetwork::C_REG;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == nw.set_registration_urc(type, false));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_attach()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::int_value = 0;
    CHECK(NSAPI_ERROR_OK == cn.set_attach());
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == cn.set_attach());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_attach());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_attach()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::AttachStatus stat = CellularNetwork::Detached;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::bool_value = true;
    CHECK(NSAPI_ERROR_OK == cn.get_attach(stat));
    CHECK(stat == CellularNetwork::Attached);

    ATHandler_stub::int_value = 0;
    CHECK(NSAPI_ERROR_OK == cn.get_attach(stat));
    CHECK(stat == CellularNetwork::Detached);

    stat = CellularNetwork::Attached;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_attach(stat));
    CHECK(stat == CellularNetwork::Detached);


    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_OK == cn.get_attach(stat));
    CHECK(stat == CellularNetwork::Detached);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_attach(stat));
    CHECK(stat == CellularNetwork::Detached);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_detach()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    my_AT_CN cn(at);

    CHECK(NSAPI_ERROR_OK == cn.detach());

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.detach());

    // connect so we can test callback in detach
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    cn.set_stack_type(IPV4_STACK);
    CHECK(NSAPI_ERROR_OK == cn.connect());
    CHECK(NSAPI_STATUS_GLOBAL_UP == cn.get_connection_status());
    // attach callback
    cn.attach(&network_cb);
    network_cb_count = 0;
    CHECK(NSAPI_ERROR_OK == cn.detach());
    CHECK(network_cb_count == 1);
    CHECK(NSAPI_STATUS_DISCONNECTED == cn.get_connection_status());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_rate_control()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int ur = -1;
    CellularNetwork::RateControlExceptionReports reports = CellularNetwork::NotAllowedToBeSent;
    CellularNetwork::RateControlUplinkTimeUnit timeUnit = CellularNetwork::Unrestricted;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_rate_control(reports, timeUnit, ur));
    CHECK(reports == CellularNetwork::NotAllowedToBeSent);
    CHECK(timeUnit == CellularNetwork::Unrestricted);
    CHECK(ur == -1);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = 1;
    CHECK(NSAPI_ERROR_OK == cn.get_rate_control(reports, timeUnit, ur));
    CHECK(reports == CellularNetwork::AllowedToBeSent);
    CHECK(timeUnit == CellularNetwork::Minute);
    CHECK(ur == 1);

    // test second if in get_rate_control
    reports = CellularNetwork::NotAllowedToBeSent;
    timeUnit = CellularNetwork::Unrestricted;
    ur = -1;

    ATHandler_stub::int_count = 1;
    ATHandler_stub::int_valid_count_table[0] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_rate_control(reports, timeUnit, ur));
    CHECK(reports == CellularNetwork::NotAllowedToBeSent);
    CHECK(timeUnit == CellularNetwork::Unrestricted);
    CHECK(ur == -1);

    // test second if in get_rate_control
    ATHandler_stub::int_count = 2;
    ATHandler_stub::int_valid_count_table[0] = 1;
    ATHandler_stub::int_valid_count_table[1] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_rate_control(reports, timeUnit, ur));
    CHECK(reports == CellularNetwork::AllowedToBeSent);
    CHECK(timeUnit == CellularNetwork::Unrestricted);
    CHECK(ur == -1);

    // test third if in get_rate_control
    ATHandler_stub::int_count = 3;
    ATHandler_stub::int_valid_count_table[0] = 3;
    ATHandler_stub::int_valid_count_table[1] = 1;
    ATHandler_stub::int_valid_count_table[2] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_rate_control(reports, timeUnit, ur));
    CHECK(reports == CellularNetwork::AllowedToBeSent);
    CHECK(timeUnit == CellularNetwork::Day);
    CHECK(ur == -1);

    ATHandler_stub::int_count = 4;
    ATHandler_stub::int_valid_count_table[0] = 5;
    ATHandler_stub::int_valid_count_table[1] = 3;
    ATHandler_stub::int_valid_count_table[2] = 1;
    ATHandler_stub::int_valid_count_table[3] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.get_rate_control(reports, timeUnit, ur));
    CHECK(reports == CellularNetwork::AllowedToBeSent);
    CHECK(timeUnit == CellularNetwork::Day);
    CHECK(ur == 5);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_apn_backoff_timer()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int time = -1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_PARAMETER == cn.get_apn_backoff_timer(time));
    CHECK(time == -1);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_PARAMETER == cn.get_apn_backoff_timer(time));
    CHECK(time == -1);

    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    cn.set_credentials("internet", NULL, NULL);
    CHECK(NSAPI_ERROR_OK == cn.get_apn_backoff_timer(time));
    CHECK(time == -1);

    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::bool_value = true;
    ATHandler_stub::int_value = 55;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    cn.set_credentials("internet", NULL, NULL);
    CHECK(NSAPI_ERROR_OK == cn.get_apn_backoff_timer(time));
    CHECK(time == 55);
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
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::RAT_UNKNOWN));
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::RAT_GSM_COMPACT));

    my_AT_CN my_cn(at);
    CHECK(NSAPI_ERROR_OK == my_cn.set_access_technology(CellularNetwork::RAT_GSM_COMPACT));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_access_technology()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::RadioAccessTechnology rat;

    CHECK(NSAPI_ERROR_OK == cn.get_access_technology(rat));
    CHECK(CellularNetwork::RAT_UNKNOWN == rat);
}


void Test_AT_CellularNetwork::test_AT_CellularNetwork_scan_plmn()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int c = -1;
    CellularNetwork::operList_t ops;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.scan_plmn(ops, c));
    CHECK(c == 0);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.scan_plmn(ops, c));
    CHECK(c == 0);


    ATHandler_stub::read_string_index = 3;
    ATHandler_stub::read_string_table[0] = "44444";
    ATHandler_stub::read_string_table[1] = "33333";
    ATHandler_stub::read_string_table[2] = "12345";
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::info_elem_true_counter = 1;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_OK == cn.scan_plmn(ops, c));
    CHECK(c == 1);
    CHECK(ops.get_head() != NULL);
    CellularNetwork::operator_t* op = ops.get_head();
    CHECK(op->op_status == CellularNetwork::operator_t::Available);
    CHECK(strcmp(op->op_long, "12345") == 0);
    CHECK(strcmp(op->op_short, "33333") == 0);
    CHECK(strcmp(op->op_num, "44444") == 0);
    ops.delete_all();

    ATHandler_stub::read_string_index = 3;
    ATHandler_stub::int_value = 6; // RAT_HSDPA_HSUPA
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::info_elem_true_counter = 1;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.set_access_technology(CellularNetwork::RAT_UTRAN));
    CHECK(NSAPI_ERROR_OK == cn.scan_plmn(ops, c));
    CHECK(c == 0);
    CHECK(ops.get_head() == NULL);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_set_ciot_optimization_config()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.set_ciot_optimization_config(CellularNetwork::SUPPORTED_UE_OPT_NO_SUPPORT, CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.set_ciot_optimization_config(CellularNetwork::SUPPORTED_UE_OPT_NO_SUPPORT, CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE));
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_ciot_optimization_config()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::Supported_UE_Opt sup = CellularNetwork::SUPPORTED_UE_OPT_NO_SUPPORT;
    CellularNetwork::Preferred_UE_Opt pref = CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.get_ciot_optimization_config(sup, pref));
    CHECK(sup == CellularNetwork::SUPPORTED_UE_OPT_CONTROL_PLANE);
    CHECK(pref == CellularNetwork::PREFERRED_UE_OPT_CONTROL_PLANE);

    sup = CellularNetwork::SUPPORTED_UE_OPT_NO_SUPPORT;
    pref = CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_ciot_optimization_config(sup, pref));
    CHECK(sup == CellularNetwork::SUPPORTED_UE_OPT_NO_SUPPORT);
    CHECK(pref == CellularNetwork::PREFERRED_UE_OPT_NO_PREFERENCE);
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

    my_AT_CN my_cn(at);
    CHECK(DEFAULT_STACK == my_cn.get_stack_type());
    CHECK(NSAPI_ERROR_OK == my_cn.set_stack_type(IPV4_STACK));
    CHECK(DEFAULT_STACK == my_cn.get_stack_type());
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_pdpcontext_params()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::pdpContextList_t list;

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_pdpcontext_params(list));

    // don't got to while loop
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::resp_info_true_counter = 0;
    CHECK(NSAPI_ERROR_OK == cn.get_pdpcontext_params(list));
    CHECK(NULL == list.get_head());

    // go to while loop and check values
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::int_count = 9;
    ATHandler_stub::int_valid_count_table[8] = 1;
    ATHandler_stub::int_valid_count_table[7] = 2;
    ATHandler_stub::int_valid_count_table[6] = 3;
    ATHandler_stub::int_valid_count_table[5] = 4;
    ATHandler_stub::int_valid_count_table[4] = 5;
    ATHandler_stub::int_valid_count_table[3] = 6;
    ATHandler_stub::int_valid_count_table[2] = 7;
    ATHandler_stub::int_valid_count_table[1] = 8;
    ATHandler_stub::int_valid_count_table[0] = 9;

    ATHandler_stub::read_string_index = 7;
    ATHandler_stub::read_string_table[6] = "internet";
    ATHandler_stub::read_string_table[5] = "1.2.3.4.5.6.7.8.9.10.11.112.13.14.15.16.1.2.3.44.55.6.7.8.9.10.11.12.13.14.15.16";
    ATHandler_stub::read_string_table[4] = "23.33.44.1.2.3.55.123.225.34.11.1.0.0.123.234";
    ATHandler_stub::read_string_table[3] = "1.2.3.4";
    ATHandler_stub::read_string_table[2] = "0.255.0.255";
    ATHandler_stub::read_string_table[1] = "25.66.77.88";
    ATHandler_stub::read_string_table[0] = "004.003.002.001";

    CHECK(NSAPI_ERROR_OK == cn.get_pdpcontext_params(list));
    CellularNetwork::pdpcontext_params_t *params = list.get_head();
    CHECK(params != NULL);
    CHECK(params->next == NULL);
    CHECK(params->cid == 1);
    CHECK(params->bearer_id == 2);
    CHECK(params->im_signalling_flag == 3);
    CHECK(params->lipa_indication == 4);
    CHECK(params->ipv4_mtu == 5);
    CHECK(params->wlan_offload == 6);
    CHECK(params->local_addr_ind == 7);
    CHECK(params->non_ip_mtu == 8);
    CHECK(params->serving_plmn_rate_control_value == 9);
    CHECK(strcmp(params->apn, "internet") == 0);
    CHECK(strcmp(params->local_addr, "102:304:506:708:90A:B70:D0E:F10") == 0);
    CHECK(strcmp(params->local_subnet_mask, "102:32C:3706:708:90A:B0C:D0E:F10") == 0);
    CHECK(strcmp(params->gateway_addr, "1721:2C01:203:377B:E122:B01:000:7BEA") == 0);
    CHECK(strcmp(params->dns_primary_addr, "1.2.3.4") == 0);
    CHECK(strcmp(params->dns_secondary_addr, "0.255.0.255") == 0);
    CHECK(strcmp(params->p_cscf_prim_addr, "25.66.77.88") == 0);
    CHECK(strcmp(params->p_cscf_sec_addr, "004.003.002.001") == 0);

}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_extended_signal_quality()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    int rx = -1, be = -1, rs = -1, ec = -1, rsrq = -1, rsrp = -1;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_extended_signal_quality(rx, be,rs,ec,rsrq, rsrp));
    CHECK(rx == -1 && be == -1 && rs == -1 && ec == -1 && rsrq == -1 && rsrp == -1);

    ATHandler_stub::int_value = 5;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.get_extended_signal_quality(rx, be,rs,ec,rsrq, rsrp));
    CHECK(rx == 5 && be == 5 && rs == 5 && ec == 5 && rsrq == 5 && rsrp == 5);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_signal_quality()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int rs = -1, ber = -1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_signal_quality(rs,ber));
    CHECK(rs == -1 && ber == -1);

    ATHandler_stub::int_value = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.get_signal_quality(rs,ber));
    CHECK(rs == 1 && ber == 1);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_cell_id()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    int id = 0;
    CHECK(NSAPI_ERROR_OK == cn.get_cell_id(id));
    CHECK(id == -1);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_OK == cn.get_cell_id(id));
    CHECK(id == -1);

    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[1] = "00C3";
    ATHandler_stub::read_string_table[0] = "1234FFC1"; //==  cellid and in dec: 305463233
    ATHandler_stub::int_value = 1;
    // Get registration status to modify cell_id
    CellularNetwork::RegistrationType type;
    CellularNetwork::RegistrationStatus status;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.get_registration_status(CellularNetwork::C_EREG, status));
    CHECK(NSAPI_ERROR_OK == cn.get_cell_id(id));
    CHECK(id == 305463233);
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
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_operator_params(format, ops));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = 0;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = "12345";
    CHECK(NSAPI_ERROR_OK == cn.get_operator_params(format, ops));
    CHECK(format == 0);
    CHECK(strcmp(ops.op_long, "12345") == 0);
    CHECK(strlen(ops.op_short) == 0);
    CHECK(strlen(ops.op_num) == 0);
    CHECK(ops.op_rat == CellularNetwork::RAT_GSM);

    ops.op_long[0] = 0;
    ATHandler_stub::int_value = 1;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = "12345";
    CHECK(NSAPI_ERROR_OK == cn.get_operator_params(format, ops));
    CHECK(format == 1);
    CHECK(strlen(ops.op_long) == 0);
    CHECK(strcmp(ops.op_short, "12345") == 0);
    CHECK(strlen(ops.op_num) == 0);
    CHECK(ops.op_rat == CellularNetwork::RAT_GSM_COMPACT);

    ops.op_short[0] = 0;
    ATHandler_stub::int_value = 2;
    ATHandler_stub::read_string_index = 1;
    ATHandler_stub::read_string_table[0] = "12345";
    CHECK(NSAPI_ERROR_OK == cn.get_operator_params(format, ops));
    CHECK(format == 2);
    CHECK(strlen(ops.op_long) == 0);
    CHECK(strlen(ops.op_short) == 0);
    CHECK(strcmp(ops.op_num, "12345") == 0);
    CHECK(ops.op_rat == CellularNetwork::RAT_UTRAN);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_get_operator_names()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    CellularNetwork::operator_names_list name_list;

    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::bool_value = false;
    CHECK(NSAPI_ERROR_OK == cn.get_operator_names(name_list));
    CHECK(name_list.get_head() == NULL);

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_DEVICE_ERROR == cn.get_operator_names(name_list));
    CHECK(name_list.get_head() != NULL);
    CHECK(name_list.get_head()->next == NULL);

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::read_string_table[1] = "12345";
    ATHandler_stub::read_string_table[0] = "56789";
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    name_list.delete_all();
    CHECK(NSAPI_ERROR_OK == cn.get_operator_names(name_list));
    CellularNetwork::operator_names_t *op_names = name_list.get_head();
    CHECK(op_names != NULL);
    CHECK(op_names->next == NULL);
    CHECK(strcmp(op_names->numeric, "12345") == 0);
    CHECK(strcmp(op_names->alpha, "56789") == 0);
}

void Test_AT_CellularNetwork::test_AT_CellularNetwork_attach()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    network_cb_count = 0;
    cn.attach(&network_cb);
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.connect());
    CHECK(network_cb_count == 2); // check that attached callback was called twice
}

void Test_AT_CellularNetwork::test_get_connection_status()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    network_cb_count = 0;
    cn.attach(&network_cb);
    CHECK(NSAPI_ERROR_UNSUPPORTED == cn.connect());
    CHECK(network_cb_count == 2); // check that attached callback was called twice
    CHECK(NSAPI_STATUS_DISCONNECTED == cn.get_connection_status());

    my_AT_CN my_cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    cn.set_stack_type(IPV4_STACK);
    CHECK(NSAPI_ERROR_OK == my_cn.connect());
    CHECK(network_cb_count == 2);
    CHECK(NSAPI_STATUS_GLOBAL_UP == my_cn.get_connection_status());
}

void Test_AT_CellularNetwork::test_set_blocking()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularNetwork cn(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    CHECK(NSAPI_ERROR_OK == cn.set_blocking(false));
    CHECK(NSAPI_ERROR_OK == cn.set_blocking(true));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    CHECK(NSAPI_ERROR_OK == cn.set_blocking(false));
    CHECK(NSAPI_ERROR_OK == cn.set_blocking(true));
}

