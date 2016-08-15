/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef COMMON_STUB_H
#define COMMON_STUB_H

#include <sys/types.h>
#include <netdb.h>
//#include "UDPSocket.h"
#include "sockets/UDPSocket.h"
#include "nsdlaccesshelper.h"
#include "ip6string.h"
#include "Ticker.h"

//some internal test related stuff
namespace common_stub
{
    extern socket_error_t error;
    extern socket_event_t* event;
    extern socket_addr* addr;
    extern bool visited;
    extern bool bool_value;
    extern void* void_value;
    extern int int_value;
    extern int int2_value;
    extern uint16_t uint_value;
    extern omalw_certificate_list_t *cert;
    extern addrinfo* addrinfo;
    extern sn_coap_hdr_s *coap_header;
    extern sn_nsdl_resource_info_s *resource;
    extern pthread_t thread;
    extern const char *char_value;
    void clear();
}

#endif // COMMON_STUB_H
