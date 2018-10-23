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
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularDevice.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "ATHandler_stub.h"
#include "AT_CellularStack.h"

using namespace mbed;
using namespace events;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularContext : public testing::Test {
protected:

    void SetUp()
    {
        ATHandler_stub::int_count = kRead_int_table_size;
        ATHandler_stub::read_string_index = kRead_string_table_size;
        ATHandler_stub::resp_stop_success_count = kResp_stop_count_default;
        ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
        ATHandler_stub::int_value = -1;
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
};

class my_AT_CTX : public AT_CellularContext {
public:
    my_AT_CTX(ATHandler &at, CellularDevice *device, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN,
            nsapi_ip_stack_t stack = DEFAULT_STACK) : AT_CellularContext(at, device, apn, stack) {}
    virtual ~my_AT_CTX() {}
};

class my_AT_CTXIPV6 : public AT_CellularContext {
public:
    my_AT_CTXIPV6(ATHandler &at, CellularDevice *device, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN,
            nsapi_ip_stack_t stack = DEFAULT_STACK) : AT_CellularContext(at, device, apn, stack) {}
    virtual ~my_AT_CTXIPV6() {}
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

    AT_CellularContext *ctx = new AT_CellularContext(at, NULL);
    EXPECT_TRUE(ctx != NULL);
    delete ctx;
}
