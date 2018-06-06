/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "socket_api.h"
#include "socket_api_stub.h"

socket_api_stub_data_t socket_api_stub;
const uint8_t ns_in6addr_any[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int8_t socket_raw_open(void (*passed_fptr)(void *))
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *))
{
    socket_api_stub.recv_cb = passed_fptr;
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}

int8_t socket_close(int8_t socket)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_listen(int8_t socket, uint8_t backlog)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_bind(int8_t socket, const ns_address_t *address)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}

int16_t socket_read(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length)
{
    if( address ){
        memset(&address->address, 0, 16);
        address->identifier = 0;
    }
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_getpeername(int8_t socket, ns_address_t *address)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int16_t socket_sendmsg(int8_t socket, const ns_msghdr_t *msg, int flags)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}

int16_t socket_recvmsg(int8_t socket, ns_msghdr_t *msg, int flags)
{
    return -1;
}

ns_cmsghdr_t *NS_CMSG_NXTHDR(const ns_msghdr_t *msgh, const ns_cmsghdr_t *cmsg)
{
    return NULL;
}
