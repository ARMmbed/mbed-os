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
#include "m2mconnectionhandler_stub.h"
#include "m2mconnectionsecurity.h"
#include "common_stub.h"

int m2mconnectionhandler_stub::int_value;
uint16_t m2mconnectionhandler_stub::uint_value;
bool m2mconnectionhandler_stub::bool_value;

void m2mconnectionhandler_stub::clear()
{
    int_value = -1;
    uint_value = 0;
    bool_value = false;
}

M2MConnectionHandler::M2MConnectionHandler(M2MConnectionObserver &observer,
                                           M2MConnectionSecurity* sec,
                                           M2MInterface::BindingMode,
                                           M2MInterface::NetworkStack)
:_observer(observer)
{
    if( sec ){
        delete sec;
        sec = NULL;
    }
}

M2MConnectionHandler::~M2MConnectionHandler()
{
}

bool M2MConnectionHandler::bind_connection(const uint16_t)
{
    return m2mconnectionhandler_stub::bool_value;
}

bool M2MConnectionHandler::resolve_server_address(const String&,
                                                  const uint16_t,
                                                  M2MConnectionObserver::ServerType,
                                                  const M2MSecurity* )
{
    return m2mconnectionhandler_stub::bool_value;
}

bool M2MConnectionHandler::start_listening_for_data()
{
    return m2mconnectionhandler_stub::bool_value;
}

bool M2MConnectionHandler::send_data(uint8_t *,
                                     uint16_t ,
                                     sn_nsdl_addr_s *)
{
    return m2mconnectionhandler_stub::bool_value;
}

void M2MConnectionHandler::stop_listening()
{
}

int M2MConnectionHandler::send_to_socket(const unsigned char *, size_t )
{
    return m2mconnectionhandler_stub::int_value;
}

int M2MConnectionHandler::receive_from_socket(unsigned char *buf, size_t len)
{
    return m2mconnectionhandler_stub::int_value;
}

void M2MConnectionHandler::handle_connection_error(int)
{
}

void M2MConnectionHandler::set_platform_network_handler(void *)
{
}

void M2MConnectionHandler::claim_mutex()
{
}

void M2MConnectionHandler::release_mutex()
{
}
