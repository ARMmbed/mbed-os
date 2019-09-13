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
#include "AT_CellularStack.h"
#include "AT_CellularNetwork.h"
#include "Semaphore_stub.h"
#include "equeue_stub.h"
#include "PinNames.h"
#include "Thread_stub.h"
#include "mbed_poll_stub.h"

using namespace mbed;
using namespace events;

static const intptr_t test_prop[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeEnable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularContext : public testing::Test {
protected:

    void SetUp()
    {
        mbed_poll_stub::revents_value = 0;
        mbed_poll_stub::int_value = 0;
        filehandle_stub_use_read_array = false;
        filehandle_stub_read_array_pos = 0;
        memset(filehandle_stub_read_array, '\0', sizeof(filehandle_stub_read_array));
        equeue_stub.void_ptr = NULL;
        equeue_stub.call_cb_immediately = false;
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*
class my_stack : public AT_CellularStack {
public:
    my_stack(ATHandler &atHandler) : AT_CellularStack(atHandler, 1, IPV4_STACK) {}
    virtual int get_max_socket_count()
    {
        return 1;
    }
    virtual int get_max_packet_size()
    {
        return 200;
    }
    virtual bool is_protocol_supported(nsapi_protocol_t protocol)
    {
        return true;
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
        AT_CellularContext(at, device, apn), _st(at)
    {
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
        AT_CellularContext(at, device, apn), _st(at) {}
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

    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_blocking(false));
    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_blocking(true));

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
    const char *ip = ctx.get_ip_address();
    EXPECT_TRUE(ip == NULL);

    my_AT_CTX ctx1(at, NULL);
    ip = ctx1.get_ip_address();
    EXPECT_TRUE(ip != NULL);
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
    const char *gg = ctx.get_netmask();
    EXPECT_TRUE(gg == NULL);
    gg = ctx.get_gateway();
    EXPECT_TRUE(gg == NULL);
}

TEST_F(TestAT_CellularContext, get_pdpcontext_params)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext cn(at, &dev);
    CellularContext::pdpContextList_t list;

    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == cn.get_pdpcontext_params(list));

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 13;
    char row1[] = "+CGCONTRDP:1,22,";
    char row2[] = "\"AP\"\r\n";
    char row3[] = "OK\r\n";
    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[2], row3, strlen(row3));

    ASSERT_EQ(cn.get_pdpcontext_params(list), NSAPI_ERROR_OK);
    CellularContext::pdpcontext_params_t *params = list.get_head();
    EXPECT_TRUE(params != NULL);
    EXPECT_TRUE(params->next == NULL);
    ASSERT_EQ(params->cid,  1);
    ASSERT_EQ(params->bearer_id, 22);

    ASSERT_STREQ(params->apn, "AP");
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

    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports, CellularContext::NotAllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Unrestricted);
    ASSERT_EQ(ur, -1);

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 13;
    char row1[] = "+CGAPNRC:1\r\n";
    char row2[] = "OK\r\n";
    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row2, strlen(row2));

    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_rate_control(reports, timeUnit, ur));
    ASSERT_EQ(reports,  CellularContext::NotAllowedToBeSent);
    ASSERT_EQ(timeUnit, CellularContext::Unrestricted);
    ASSERT_EQ(ur, -1);
}

TEST_F(TestAT_CellularContext, get_apn_backoff_timer)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext cn(at, &dev);
    int time = -1;
    ASSERT_EQ(NSAPI_ERROR_PARAMETER, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, -1);

    ASSERT_EQ(NSAPI_ERROR_PARAMETER, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, -1);

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 13;
    char row1[] = "+CABTRDP:1,2\r\n";
    char row2[] = "OK\r\n";
    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row2, strlen(row2));

    cn.set_credentials("internet", NULL, NULL);
    ASSERT_EQ(NSAPI_ERROR_OK, cn.get_apn_backoff_timer(time));
    ASSERT_EQ(time, 2);
}

TEST_F(TestAT_CellularContext, set_file_handle)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);
    ctx.set_file_handle(&fh1);

    UARTSerial ss(NC, NC);

    ctx.set_file_handle(&ss, PTC0, true);
    ctx.enable_hup(true);
}

TEST_F(TestAT_CellularContext, connect_disconnect_sync)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    AT_CellularContext ctx(at, &dev);

    AT_CellularBase::set_cellular_properties(test_prop);
    ctx.attach(&network_cb);
    network_cb_count = 0;

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 130;
    char row1[] = "OK\r\n";
    char row2[] = "+CPIN:READY\r\n";
    char row3[] = "+COPS:1\r\n";

    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row1, strlen(row1));
    memset(filehandle_stub_read_array[2], '\0', 32);
    memcpy(filehandle_stub_read_array[3], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[4], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[5], row1, strlen(row1));
    memset(filehandle_stub_read_array[6], '\0', 32);
    memcpy(filehandle_stub_read_array[7], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[8], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[9], row3, strlen(row3)); //mode
    memcpy(filehandle_stub_read_array[10], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[11], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[12], row1, strlen(row1)); //C_EREG
    memcpy(filehandle_stub_read_array[13], row1, strlen(row1)); //C_GREG
    memcpy(filehandle_stub_read_array[14], row1, strlen(row1)); //C_REG
    char row4[] = "+CGACT:1,1\r\n";
    memcpy(filehandle_stub_read_array[15], row4, strlen(row4));
    memcpy(filehandle_stub_read_array[16], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[17], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[18], row1, strlen(row1));

    char row5[] = "+CSQ:1,1\r\n";
    memcpy(filehandle_stub_read_array[19], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[20], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[21], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[22], row1, strlen(row1));

    char row6[] = "+CEREG:1,1,,,0,1,1,,\r\n";
    memcpy(filehandle_stub_read_array[23], row6, strlen(row6)); //+CEREG:
    memcpy(filehandle_stub_read_array[24], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[25], row1, strlen(row1));
    char row7[] = "+CGATT:1\r\n";
    memcpy(filehandle_stub_read_array[26], row7, strlen(row7));
    memcpy(filehandle_stub_read_array[27], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[28], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[29], row1, strlen(row1));

    Semaphore_stub::acquire_return_value = 0;
    // connect in sync mode, semaphore will return 0 so timeout is returned
    ASSERT_EQ(ctx.connect(), NSAPI_ERROR_TIMEOUT);
    ASSERT_EQ(network_cb_count, 1);
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

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 130;
    char row1[] = "OK\r\n";
    char row2[] = "+CPIN:READY\r\n";
    char row3[] = "+COPS:1\r\n";

    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row1, strlen(row1));
    memset(filehandle_stub_read_array[2], '\0', 32);
    memcpy(filehandle_stub_read_array[3], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[4], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[5], row1, strlen(row1));
    memset(filehandle_stub_read_array[6], '\0', 32);
    memcpy(filehandle_stub_read_array[7], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[8], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[9], row3, strlen(row3)); //mode
    memcpy(filehandle_stub_read_array[10], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[11], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[12], row1, strlen(row1)); //C_EREG
    memcpy(filehandle_stub_read_array[13], row1, strlen(row1)); //C_GREG
    memcpy(filehandle_stub_read_array[14], row1, strlen(row1)); //C_REG
    char row4[] = "+CGACT:1,1\r\n";
    memcpy(filehandle_stub_read_array[15], row4, strlen(row4));
    memcpy(filehandle_stub_read_array[16], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[17], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[18], row1, strlen(row1));

    char row5[] = "+CSQ:1,1\r\n";
    memcpy(filehandle_stub_read_array[19], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[20], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[21], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[22], row1, strlen(row1));

    char row6[] = "+CEREG:1,1,,,0,1,1,,\r\n";
    memcpy(filehandle_stub_read_array[23], row6, strlen(row6)); //+CEREG:
    memcpy(filehandle_stub_read_array[24], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[25], row1, strlen(row1));
    char row7[] = "+CGATT:1\r\n";
    memcpy(filehandle_stub_read_array[26], row7, strlen(row7));
    memcpy(filehandle_stub_read_array[27], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[28], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[29], row1, strlen(row1));

    Semaphore_stub::acquire_return_value = 1;

    ASSERT_EQ(NSAPI_ERROR_OK, ctx.set_device_ready());
}

TEST_F(TestAT_CellularContext, set_sim_ready)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice dev(&fh1);
    my_AT_CTX ctx(at, &dev);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 130;
    char row1[] = "OK\r\n";
    char row2[] = "+CPIN:READY\r\n";
    char row3[] = "+COPS:1\r\n";

    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row1, strlen(row1));
    memset(filehandle_stub_read_array[2], '\0', 32);
    memcpy(filehandle_stub_read_array[3], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[4], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[5], row1, strlen(row1));
    memset(filehandle_stub_read_array[6], '\0', 32);
    memcpy(filehandle_stub_read_array[7], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[8], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[9], row3, strlen(row3)); //mode
    memcpy(filehandle_stub_read_array[10], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[11], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[12], row1, strlen(row1)); //C_EREG
    memcpy(filehandle_stub_read_array[13], row1, strlen(row1)); //C_GREG
    memcpy(filehandle_stub_read_array[14], row1, strlen(row1)); //C_REG
    char row4[] = "+CGACT:1,1\r\n";
    memcpy(filehandle_stub_read_array[15], row4, strlen(row4));
    memcpy(filehandle_stub_read_array[16], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[17], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[18], row1, strlen(row1));

    char row5[] = "+CSQ:1,1\r\n";
    memcpy(filehandle_stub_read_array[19], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[20], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[21], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[22], row1, strlen(row1));

    char row6[] = "+CEREG:1,1,,,0,1,1,,\r\n";
    memcpy(filehandle_stub_read_array[23], row6, strlen(row6)); //+CEREG:
    memcpy(filehandle_stub_read_array[24], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[25], row1, strlen(row1));
    char row7[] = "+CGATT:1\r\n";
    memcpy(filehandle_stub_read_array[26], row7, strlen(row7));
    memcpy(filehandle_stub_read_array[27], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[28], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[29], row1, strlen(row1));

    Semaphore_stub::acquire_return_value = 1;

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

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 130;
    char row1[] = "OK\r\n";
    char row2[] = "+CPIN:READY\r\n";
    char row3[] = "+COPS:1\r\n";

    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row1, strlen(row1));
    memset(filehandle_stub_read_array[2], '\0', 32);
    memcpy(filehandle_stub_read_array[3], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[4], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[5], row1, strlen(row1));
    memset(filehandle_stub_read_array[6], '\0', 32);
    memcpy(filehandle_stub_read_array[7], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[8], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[9], row3, strlen(row3)); //mode
    memcpy(filehandle_stub_read_array[10], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[11], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[12], row1, strlen(row1)); //C_EREG
    memcpy(filehandle_stub_read_array[13], row1, strlen(row1)); //C_GREG
    memcpy(filehandle_stub_read_array[14], row1, strlen(row1)); //C_REG
    char row4[] = "+CGACT:1,1\r\n";
    memcpy(filehandle_stub_read_array[15], row4, strlen(row4));
    memcpy(filehandle_stub_read_array[16], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[17], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[18], row1, strlen(row1));

    char row5[] = "+CSQ:1,1\r\n";
    memcpy(filehandle_stub_read_array[19], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[20], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[21], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[22], row1, strlen(row1));

    char row6[] = "+CEREG:1,1,,,0,1,1,,\r\n";
    memcpy(filehandle_stub_read_array[23], row6, strlen(row6)); //+CEREG:
    memcpy(filehandle_stub_read_array[24], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[25], row1, strlen(row1));
    char row7[] = "+CGATT:1\r\n";
    memcpy(filehandle_stub_read_array[26], row7, strlen(row7));
    memcpy(filehandle_stub_read_array[27], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[28], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[29], row1, strlen(row1));

    Semaphore_stub::acquire_return_value = 1;

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

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 130;
    char row1[] = "OK\r\n";
    char row2[] = "+CPIN:READY\r\n";
    char row3[] = "+COPS:1\r\n";

    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row1, strlen(row1));
    memset(filehandle_stub_read_array[2], '\0', 32);
    memcpy(filehandle_stub_read_array[3], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[4], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[5], row1, strlen(row1));
    memset(filehandle_stub_read_array[6], '\0', 32);
    memcpy(filehandle_stub_read_array[7], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[8], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[9], row3, strlen(row3)); //mode
    memcpy(filehandle_stub_read_array[10], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[11], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[12], row1, strlen(row1)); //C_EREG
    memcpy(filehandle_stub_read_array[13], row1, strlen(row1)); //C_GREG
    memcpy(filehandle_stub_read_array[14], row1, strlen(row1)); //C_REG
    char row4[] = "+CGACT:1,1\r\n";
    memcpy(filehandle_stub_read_array[15], row4, strlen(row4));
    memcpy(filehandle_stub_read_array[16], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[17], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[18], row1, strlen(row1));

    char row5[] = "+CSQ:1,1\r\n";
    memcpy(filehandle_stub_read_array[19], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[20], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[21], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[22], row1, strlen(row1));

    char row6[] = "+CEREG:1,1,,,0,1,1,,\r\n";
    memcpy(filehandle_stub_read_array[23], row6, strlen(row6)); //+CEREG:
    memcpy(filehandle_stub_read_array[24], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[25], row1, strlen(row1));
    char row7[] = "+CGATT:1\r\n";
    memcpy(filehandle_stub_read_array[26], row7, strlen(row7));
    memcpy(filehandle_stub_read_array[27], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[28], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[29], row1, strlen(row1));

    Semaphore_stub::acquire_return_value = 1;

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

    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;

    Thread_stub::osStatus_value = osErrorNoMemory;
    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_NO_MEMORY);

    // call callback so that network is opened which is needed in disconnect
    cell_callback_data_t data;
    data.error = NSAPI_ERROR_OK;
    ctx1.cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);

    Thread_stub::osStatus_value = osOK;
    network_cb_count = 0;

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    mbed_poll_stub::revents_value = POLLOUT | POLLIN;
    mbed_poll_stub::int_value = 13;
    fh1.size_value = 130;
    char row1[] = "OK\r\n";
    char row2[] = "+CPIN:READY\r\n";
    char row3[] = "+COPS:1\r\n";

    filehandle_stub_use_read_array = true;
    filehandle_stub_read_array_pos = 0;
    memcpy(filehandle_stub_read_array[0], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[1], row1, strlen(row1));
    memset(filehandle_stub_read_array[2], '\0', 32);
    memcpy(filehandle_stub_read_array[3], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[4], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[5], row1, strlen(row1));
    memset(filehandle_stub_read_array[6], '\0', 32);
    memcpy(filehandle_stub_read_array[7], row2, strlen(row2));
    memcpy(filehandle_stub_read_array[8], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[9], row3, strlen(row3)); //mode
    memcpy(filehandle_stub_read_array[10], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[11], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[12], row1, strlen(row1)); //C_EREG
    memcpy(filehandle_stub_read_array[13], row1, strlen(row1)); //C_GREG
    memcpy(filehandle_stub_read_array[14], row1, strlen(row1)); //C_REG
    char row4[] = "+CGACT:1,1\r\n";
    memcpy(filehandle_stub_read_array[15], row4, strlen(row4));
    memcpy(filehandle_stub_read_array[16], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[17], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[18], row1, strlen(row1));

    char row5[] = "+CSQ:1,1\r\n";
    memcpy(filehandle_stub_read_array[19], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[20], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[21], row5, strlen(row5)); //Signal strength
    memcpy(filehandle_stub_read_array[22], row1, strlen(row1));

    char row6[] = "+CEREG:1,1,,,0,1,1,,\r\n";
    memcpy(filehandle_stub_read_array[23], row6, strlen(row6)); //+CEREG:
    memcpy(filehandle_stub_read_array[24], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[25], row1, strlen(row1));
    char row7[] = "+CGATT:1\r\n";
    memcpy(filehandle_stub_read_array[26], row7, strlen(row7));
    memcpy(filehandle_stub_read_array[27], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[28], row1, strlen(row1));
    memcpy(filehandle_stub_read_array[29], row1, strlen(row1));

    Semaphore_stub::acquire_return_value = 0;

    ASSERT_EQ(ctx1.connect(),  NSAPI_ERROR_OK);
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
