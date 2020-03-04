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
#include "AT_CellularContext.h"
#include "events/EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularDevice.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "ATHandler_stub.h"
#include "AT_CellularStack.h"
#include "Semaphore_stub.h"
#include "CellularDevice_stub.h"
#include "AT_CellularDevice_stub.h"
#include "equeue_stub.h"
#include "CellularUtil_stub.h"
#include "PinNames.h"

using namespace mbed;
using namespace events;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularContext : public testing::Test {
protected:

    void SetUp()
    {
        ATHandler_stub::nsapi_error_value = 0;
        ATHandler_stub::nsapi_error_ok_counter = 0;
        ATHandler_stub::int_value = -1;
        ATHandler_stub::timeout = 0;
        ATHandler_stub::default_timeout = 0;
        ATHandler_stub::debug_on = 0;
        ATHandler_stub::ssize_value = 0;
        ATHandler_stub::read_string_value = NULL;
        ATHandler_stub::size_value = 0;
        ATHandler_stub::return_given_size = false;
        ATHandler_stub::bool_value = false;
        ATHandler_stub::uint8_value = 0;
        ATHandler_stub::fh_value = NULL;
        ATHandler_stub::call_immediately = false;
        ATHandler_stub::resp_info_true_counter = false;
        ATHandler_stub::info_elem_true_counter = false;
        ATHandler_stub::int_valid_count_table[kRead_int_table_size];
        ATHandler_stub::int_count = kRead_int_table_size;
        ATHandler_stub::read_string_index = kRead_string_table_size;
        ATHandler_stub::read_string_table[kRead_string_table_size];
        ATHandler_stub::resp_stop_success_count = kResp_stop_count_default;
        CellularDevice_stub::connect_counter = 2;

        CellularUtil_stub::char_table[0] = (char *)"\0";
        CellularUtil_stub::table_idx = 0;
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*
class my_stack : public AT_CellularStack {
public:
    my_stack(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularStack(atHandler, 1, IPV4_STACK, device) {}
    virtual int get_max_packet_size()
    {
        return 200;
    }
    virtual nsapi_error_t socket_close_impl(int sock_id)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_error_t create_socket_impl(CellularSocket *socket)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                     const void *data, nsapi_size_t size)
    {
        return 100;
    }
    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                       void *buffer, nsapi_size_t size)
    {
        return 100;
    }

    virtual const char *get_ip_address()
    {
        return "1.2.3.4";
    }
};

class my_AT_CTX : public AT_CellularContext {
public:
    my_AT_CTX(ATHandler &at, CellularDevice *device, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN) :
        AT_CellularContext(at, device, apn), _st(at, *get_device())
    {
        AT_CellularDevice_stub::supported_bool = false;
    }
    virtual ~my_AT_CTX() {}

    virtual NetworkStack *get_stack()
    {
        return &_st;
    }
    virtual uint32_t get_timeout_for_operation(ContextOperation op) const
    {
        return 10;
    }
    virtual void cellular_callback(nsapi_event_t ev, intptr_t ptr)
    {
        AT_CellularContext::cellular_callback(ev, ptr);
    }

    void activ_non_ip_context()
    {
        activate_non_ip_context();
    }

    void deactiv_non_ip_context()
    {
        deactivate_non_ip_context();
    }

    my_stack _st;
};

class my_AT_CTXIPV6 : public AT_CellularContext {
public:
    my_AT_CTXIPV6(ATHandler &at, CellularDevice *device, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN) :
        AT_CellularContext(at, device, apn), _st(at, *get_device()) {}
    virtual ~my_AT_CTXIPV6() {}
    virtual NetworkStack *get_stack()
    {
        return &_st;
    }
    virtual uint32_t get_timeout_for_operation(ContextOperation op) const
    {
        return 10;
    }
    my_stack _st;
};

class def_AT_CTX : public AT_CellularContext {
public:
    def_AT_CTX(ATHandler &at, CellularDevice *device, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN) :
        AT_CellularContext(at, device, apn) {}
    virtual ~def_AT_CTX() {}

    uint32_t do_op()
    {
        return AT_CellularContext::get_timeout_for_operation(mbed::CellularContext::ContextOperation(_op));
    }
    int _op;
};

class AT_CTX_cid: public AT_CellularContext {
public:
    AT_CTX_cid(ATHandler &at, CellularDevice *device, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN) :
        AT_CellularContext(at, device, apn) {}
    virtual ~AT_CTX_cid() {}

    void set_cid(int cid)
    {
        _cid = cid;
    }
};

static int network_cb_count;
static void network_cb(nsapi_event_t ev, intptr_t intptr)
{
    network_cb_count++;
}

TEST_F(TestAT_CellularContext, Create)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);

    AT_CellularContext *ctx = new AT_CellularContext(at, NULL);
    EXPECT_TRUE(ctx != NULL);
    delete ctx;

    ctx = new AT_CellularContext(at, &dev);
    EXPECT_TRUE(ctx != NULL);

    ctx->get_device();
    EXPECT_EQ(NSAPI_STATUS_DISCONNECTED, ctx->get_connection_status());
    delete ctx;
}

TEST_F(TestAT_CellularContext, set_blocking)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_blocking(false));
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_blocking(true));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_blocking(false));
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_blocking(true));
}

TEST_F(TestAT_CellularContext, get_stack)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    NetworkStack *stack = ctx.get_stack();
    EXPECT_TRUE(stack == NULL);

    my_AT_CTX ctx1(at, NULL);
    stack = ctx1.get_stack();
    EXPECT_TRUE(stack != NULL);
}

TEST_F(TestAT_CellularContext, get_ip_address)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    SocketAddress addr;
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, ctx.get_ip_address(&addr));

    my_AT_CTX ctx1(at, NULL);
    EXPECT_EQ(NSAPI_ERROR_OK, ctx1.get_ip_address(&addr));
}

TEST_F(TestAT_CellularContext, get_interface_name)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CTX_cid ctx(at, &dev);

    char ifn[5];
    EXPECT_TRUE(NULL == ctx.get_interface_name(ifn));

    ctx.set_cid(1);
    EXPECT_STREQ("ce1", ctx.get_interface_name(ifn));
    EXPECT_STREQ("ce1", ifn);
}

TEST_F(TestAT_CellularContext, attach)
{
    EventQueue que;
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularContext ctx(at, &dev);
    ctx.attach(&network_cb);
}

TEST_F(TestAT_CellularContext, set_plmn)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    ctx.set_plmn(NULL);
    ctx.set_plmn("12345");
}

TEST_F(TestAT_CellularContext, set_sim_pin)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    ctx.set_sim_pin(NULL);
    ctx.set_sim_pin("12345");
}

TEST_F(TestAT_CellularContext, set_credentials)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);

    ctx.set_credentials(NULL);
    ctx.set_credentials("apn");
    ctx.set_credentials("apn", NULL);
    ctx.set_credentials("apn", NULL, NULL);
    ctx.set_credentials("apn", NULL, "passwd");
    ctx.set_credentials("apn", "user", NULL);
    ctx.set_credentials("apn", "user");
    ctx.set_credentials("apn", "user", "passwd");
}

TEST_F(TestAT_CellularContext, get_netmask_gateway)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    SocketAddress addr;
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == ctx.get_netmask(&addr));
    EXPECT_TRUE(addr.get_ip_address() == NULL);
    EXPECT_TRUE(NSAPI_ERROR_UNSUPPORTED == ctx.get_gateway(&addr));
    EXPECT_TRUE(addr.get_ip_address() == NULL);
}

TEST_F(TestAT_CellularContext, get_pdpcontext_params)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext cn(at, &dev);
    CellularContext::pdpContextList_t list;

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_pdpcontext_params(list));

    // don't got to while loop
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::resp_info_true_counter = 0;
    ASSERT_EQ(cn.get_pdpcontext_params(list), NSAPI_ERROR_OK);
    EXPECT_TRUE(NULL == list.get_head());

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
    ATHandler_stub::read_string_table[6] = (char *)"internet";
    ATHandler_stub::read_string_table[5] = (char *)"1.2.3.4.5.6.7.8.9.10.11.112.13.14.15.16.1.2.3.44.55.6.7.8.9.10.11.12.13.14.15.16";
    ATHandler_stub::read_string_table[4] = (char *)"23.33.44.1.2.3.55.123.225.34.11.1.0.0.123.234";
    ATHandler_stub::read_string_table[3] = (char *)"1.2.3.4";
    ATHandler_stub::read_string_table[2] = (char *)"0.255.0.255";
    ATHandler_stub::read_string_table[1] = (char *)"25.66.77.88";
    ATHandler_stub::read_string_table[0] = (char *)"004.003.002.001";

    CellularUtil_stub::table_idx = 12;
    CellularUtil_stub::char_table[11] = (char *)"102:304:506:708:90A:B70:D0E:F10\0";
    CellularUtil_stub::char_table[10] = (char *)"102:32C:3706:708:90A:B0C:D0E:F10\0";
    CellularUtil_stub::char_table[9] = (char *)"1721:2C01:203:377B:E122:B01:000:7BEA\0";
    CellularUtil_stub::char_table[8] = (char *)"1.2.3.4\0";
    CellularUtil_stub::char_table[7] = (char *)"1.2.3.4\0";
    CellularUtil_stub::char_table[6] = (char *)"1.2.3.4\0";
    CellularUtil_stub::char_table[5] = (char *)"0.255.0.255\0";
    CellularUtil_stub::char_table[4] = (char *)"1.2.3.4\0";
    CellularUtil_stub::char_table[3] = (char *)"25.66.77.88\0";
    CellularUtil_stub::char_table[2] = (char *)"1.2.3.4\0";
    CellularUtil_stub::char_table[1] = (char *)"004.003.002.001\0";
    CellularUtil_stub::char_table[0] = (char *)"1.2.3.4\0";

    EXPECT_TRUE(NSAPI_ERROR_OK == cn.get_pdpcontext_params(list));
    CellularContext::pdpcontext_params_t *params = list.get_head();
    EXPECT_TRUE(params != NULL);
    EXPECT_TRUE(params->next == NULL);
    ASSERT_EQ(params->cid,  1);
    ASSERT_EQ(params->bearer_id, 2);
    ASSERT_EQ(params->im_signalling_flag, 3);
    ASSERT_EQ(params->lipa_indication,  4);
    ASSERT_EQ(params->ipv4_mtu, 5);
    ASSERT_EQ(params->wlan_offload, 6);
    ASSERT_EQ(params->local_addr_ind, 7);
    ASSERT_EQ(params->non_ip_mtu, 8);
    ASSERT_EQ(params->serving_plmn_rate_control_value,  9);
    ASSERT_STREQ(params->apn, "internet");
    ASSERT_STREQ(params->local_addr, "102:304:506:708:90A:B70:D0E:F10");
    ASSERT_STREQ(params->local_subnet_mask, "102:32C:3706:708:90A:B0C:D0E:F10");
    ASSERT_STREQ(params->gateway_addr, "1721:2C01:203:377B:E122:B01:000:7BEA");
    ASSERT_STREQ(params->dns_primary_addr, "1.2.3.4");
    ASSERT_STREQ(params->dns_secondary_addr, "0.255.0.255");
    ASSERT_STREQ(params->p_cscf_prim_addr, "25.66.77.88");
    ASSERT_STREQ(params->p_cscf_sec_addr, "004.003.002.001");
}

TEST_F(TestAT_CellularContext, get_rate_control)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext cn(at, &dev);
    int ur = -1;
    CellularContext::RateControlExceptionReports reports = CellularContext::NotAllowedToBeSent;
    CellularContext::RateControlUplinkTimeUnit timeUnit = CellularContext::Unrestricted;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports, CellularContext::NotAllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Unrestricted);
    ASSERT_EQ(ur, -1);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::int_value = 1;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports,  CellularContext::AllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Minute);
    ASSERT_EQ(ur, 1);

    // test second if in get_rate_control
    reports = CellularContext::NotAllowedToBeSent;
    timeUnit = CellularContext::Unrestricted;
    ur = -1;

    ATHandler_stub::int_count = 1;
    ATHandler_stub::int_valid_count_table[0] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports, CellularContext::NotAllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Unrestricted);
    ASSERT_EQ(ur, -1);

    // test second if in get_rate_control
    ATHandler_stub::int_count = 2;
    ATHandler_stub::int_valid_count_table[0] = 1;
    ATHandler_stub::int_valid_count_table[1] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports, CellularContext::AllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Unrestricted);
    ASSERT_EQ(ur, -1);

    // test third if in get_rate_control
    ATHandler_stub::int_count = 3;
    ATHandler_stub::int_valid_count_table[0] = 3;
    ATHandler_stub::int_valid_count_table[1] = 1;
    ATHandler_stub::int_valid_count_table[2] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_rate_control(reports, timeUnit, ur));
    EXPECT_TRUE(reports == CellularContext::AllowedToBeSent);
    EXPECT_TRUE(timeUnit == CellularContext::Day);
    EXPECT_TRUE(ur == -1);

    ATHandler_stub::int_count = 4;
    ATHandler_stub::int_valid_count_table[0] = 5;
    ATHandler_stub::int_valid_count_table[1] = 3;
    ATHandler_stub::int_valid_count_table[2] = 1;
    ATHandler_stub::int_valid_count_table[3] = 1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports, CellularContext::AllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Day);
    ASSERT_EQ(ur, 5);
}

TEST_F(TestAT_CellularContext, get_apn_backoff_timer)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext cn(at, &dev);
    int time = -1;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ASSERT_EQ(NSAPI_ERROR_PARAMETER, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, -1);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(NSAPI_ERROR_PARAMETER, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, -1);

    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::bool_value = false;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    cn.set_credentials("internet", NULL, NULL);
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, -1);

    ATHandler_stub::resp_info_true_counter = 0;
    ATHandler_stub::bool_value = true;
    ATHandler_stub::int_value = 55;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    cn.set_credentials("internet", NULL, NULL);
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, 55);
}

TEST_F(TestAT_CellularContext, connect_disconnect_sync)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    ctx.attach(&network_cb);
    network_cb_count = 0;
    // connect in sync mode, semaphore will return 0 so timeout is returned
    ASSERT_EQ(ctx.connect(), NSAPI_ERROR_TIMEOUT);
    ASSERT_EQ(network_cb_count, 1);

    my_AT_CTX ctx1(at, &dev);
    ctx1.attach(&network_cb);
    Semaphore_stub::acquire_return_value = true;

    // call callback so that network is opened which is needed in disconnect
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx1.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);

    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::read_string_table[0] = (char *)"APN";
    ATHandler_stub::read_string_table[1] = (char *)"IP";
    ATHandler_stub::read_string_index = 2;
    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);

    ASSERT_EQ(network_cb_count, 4);

    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::read_string_table[1] = (char *)"Non-IP";
    ATHandler_stub::read_string_index = 2;
    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);

    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::read_string_table[1] = (char *)"IPV6";
    ATHandler_stub::read_string_index = 2;
    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);

    AT_CellularDevice_stub::supported_bool = true;
    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::read_string_table[1] = (char *)"IPV4V6";
    ATHandler_stub::read_string_index = 2;
    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);
    AT_CellularDevice_stub::supported_bool = false;

    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    ATHandler_stub::resp_info_true_counter = 1;
    ATHandler_stub::read_string_table[0] = (char *)"APN2";
    ATHandler_stub::read_string_table[1] = (char *)"IPV4V6";
    ATHandler_stub::read_string_index = 2;
    ATHandler_stub::int_value = 10;
    ctx1.set_credentials("APN");
    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);
    ATHandler_stub::int_value = -1;

    ASSERT_EQ(ctx1.connect(), NSAPI_ERROR_IS_CONNECTED);

    EXPECT_TRUE(ctx1.is_connected() == true);

    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    EXPECT_TRUE(ctx1.is_connected() == false);

    ATHandler_stub::get_debug_clear();
    EXPECT_FALSE(ATHandler_stub::is_get_debug_run());
    ATHandler_stub::debug_call_count_clear();
    at.set_debug(true);
    ASSERT_EQ(ctx1.connect("1234", "internet", "usern", "pwd"), NSAPI_ERROR_OK);
    EXPECT_TRUE(ATHandler_stub::is_get_debug_run());
    EXPECT_TRUE(ATHandler_stub::set_debug_call_count_get() == 3);
    EXPECT_TRUE(at.get_debug());
    EXPECT_TRUE(ctx1.is_connected() == true);
    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    EXPECT_TRUE(ctx1.is_connected() == false);

    ATHandler_stub::get_debug_clear();
    EXPECT_FALSE(ATHandler_stub::is_get_debug_run());
    ATHandler_stub::debug_call_count_clear();
    at.set_debug(false);
    ASSERT_EQ(ctx1.connect("1234", "internet", "usern", "pwd"), NSAPI_ERROR_OK);
    EXPECT_TRUE(ATHandler_stub::is_get_debug_run());
    EXPECT_TRUE(ATHandler_stub::set_debug_call_count_get() == 3);
    EXPECT_FALSE(at.get_debug());
    EXPECT_TRUE(ctx1.is_connected() == true);
    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    EXPECT_TRUE(ctx1.is_connected() == false);

    // More connect test after we are re-writted getting of PDP context...
}

TEST_F(TestAT_CellularContext, de_and_activate_non_ip_context)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx(at, &dev);
    ctx.attach(&network_cb);
    Semaphore_stub::acquire_return_value = true;

    // call callback so that network is opened which is needed in disconnect
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);

    ctx.activ_non_ip_context();

    ctx.deactiv_non_ip_context();
}

TEST_F(TestAT_CellularContext, set_device_ready_sync)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx(at, &dev);
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_device_ready());
}

TEST_F(TestAT_CellularContext, set_sim_ready)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx(at, &dev);
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_sim_ready());
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);
    data.status_data = CellularDevice::SimStateReady;
    ctx.cellular_callback((nsapi_event_t)CellularSIMStatusChanged, (intptr_t)&data);
}

TEST_F(TestAT_CellularContext, register_to_network)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx(at, &dev);
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.register_to_network());
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);
    data.status_data = CellularNetwork::RegisteredHomeNetwork;
    ctx.cellular_callback((nsapi_event_t)CellularRegistrationStatusChanged, (intptr_t)&data);
}

TEST_F(TestAT_CellularContext, attach_to_network)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx(at, &dev);
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.attach_to_network());
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);
    data.status_data = CellularNetwork::Attached;
    ctx.cellular_callback((nsapi_event_t)CellularAttachNetwork, (intptr_t)&data);
}

TEST_F(TestAT_CellularContext, connect_disconnect_async)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    network_cb_count = 0;

    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx1(at, &dev);
    ctx1.attach(&network_cb);
    ASSERT_EQ(ctx1.set_blocking(false), NSAPI_ERROR_OK);

    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);

    // call callback so that network is opened which is needed in disconnect
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx1.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);

    // attach ready, ctx will continue in callback
    data.status_data = CellularNetwork::Attached;
    ctx1.cellular_callback((nsapi_event_t)CellularAttachNetwork, (intptr_t)&data);

    ASSERT_EQ(network_cb_count, 4);
    ASSERT_EQ(ctx1.connect(), NSAPI_ERROR_IS_CONNECTED);
    EXPECT_TRUE(ctx1.is_connected() == true);
    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_NO_MEMORY);
    EXPECT_TRUE(ctx1.is_connected() == true);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    ASSERT_EQ(ctx1.disconnect(), NSAPI_ERROR_OK);
    EXPECT_TRUE(ctx1.is_connected() == false);

    // sdet CellularDevice_stub::connect_counter = 0 so device is already attached and will return NSAPI_ERROR_ALREADY to context when calling connect
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = false;
    CellularDevice_stub::connect_counter = 0;
    // queue can't allocate so return NSAPI_ERROR_NO_MEMORY
    ASSERT_EQ(ctx1.connect(), NSAPI_ERROR_NO_MEMORY);

    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    ASSERT_EQ(ctx1.connect(), NSAPI_ERROR_OK);

    // More connect test after we are re-writted getting of PDP context...
}

TEST_F(TestAT_CellularContext, get_timeout_for_operation)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularDevice dev(&fh1);
    def_AT_CTX ctx1(at, &dev);
    ctx1._op = 1;
    EXPECT_EQ(300 * 1000, ctx1.do_op());

    ctx1._op = 0;
    EXPECT_EQ(300 * 1000, ctx1.do_op());

    ctx1._op = -1;
    EXPECT_EQ(1800 * 1000, ctx1.do_op());
}
