/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "AT_CellularStack.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace mbed;
using namespace mbed_cellular_util;

AT_CellularStack::AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type, AT_CellularDevice &device) :
    _socket(NULL), _cid(cid), _stack_type(stack_type), _at(atHandler), _device(device)
{
}

AT_CellularStack::~AT_CellularStack()
{
}

nsapi_error_t AT_CellularStack::get_ip_address(SocketAddress* address)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularStack::socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_close(nsapi_socket_t handle)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_bind(nsapi_socket_t handle, const SocketAddress &addr)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &addr)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularStack::socket_send(nsapi_socket_t handle, const void *data, unsigned size)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularStack::socket_sendto(nsapi_socket_t handle, const SocketAddress &addr, const void *data, unsigned size)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularStack::socket_recv(nsapi_socket_t handle, void *data, unsigned size)
{
    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t AT_CellularStack::socket_recvfrom(nsapi_socket_t handle, SocketAddress *addr, void *buffer, unsigned size)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularStack::socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data)
{
}

void AT_CellularStack::set_cid(int cid)
{
}
