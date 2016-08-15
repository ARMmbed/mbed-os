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
#include "m2mserver_stub.h"

uint32_t m2mserver_stub::int_value;
bool m2mserver_stub::bool_value;
String m2mserver_stub::string_value;
M2MResource* m2mserver_stub::resource;

void m2mserver_stub::clear()
{
    int_value = 0;
    bool_value = false;
    string_value = "";
    resource = NULL;
}

M2MServer::M2MServer()
: M2MObject("2")
{
}

M2MServer::~M2MServer()
{
}

M2MResource* M2MServer::create_resource(ServerResource, uint32_t)
{
    return m2mserver_stub::resource;
}

M2MResource* M2MServer::create_resource(ServerResource)
{
    return m2mserver_stub::resource;
}

bool M2MServer::delete_resource(ServerResource)
{
    return m2mserver_stub::bool_value;
}

bool M2MServer::set_resource_value(ServerResource,
                                   const String &)
{
    return m2mserver_stub::bool_value;
}

bool M2MServer::set_resource_value(ServerResource,
                                   uint32_t)
{
    return m2mserver_stub::bool_value;
}

String M2MServer::resource_value_string(ServerResource) const
{
    return m2mserver_stub::string_value;
}


uint32_t M2MServer::resource_value_int(ServerResource) const
{
    return m2mserver_stub::int_value;
}

bool M2MServer::is_resource_present(ServerResource) const
{
    return m2mserver_stub::bool_value;
}

uint16_t M2MServer::total_resource_count() const
{
    return (uint16_t)m2mserver_stub::int_value;
}
