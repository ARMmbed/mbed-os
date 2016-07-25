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
#include "m2msecurity_stub.h"

bool m2msecurity_stub::has_value;
uint32_t m2msecurity_stub::int_value;
uint32_t m2msecurity_stub::sec_mode;
bool m2msecurity_stub::bool_value;
String *m2msecurity_stub::string_value;
M2MResource* m2msecurity_stub::resource;

void m2msecurity_stub::clear()
{
    has_value = false;
    int_value = -1;
    bool_value = false;
    string_value = NULL;
    resource = NULL;
}

M2MSecurity::M2MSecurity(ServerType ser_type)
: M2MObject("0"),
 _server_type(ser_type),
 _server_instance(NULL)
{
}

M2MSecurity::~M2MSecurity()
{
}

M2MResource* M2MSecurity::create_resource(SecurityResource, uint32_t)
{
    return m2msecurity_stub::resource;
}

bool M2MSecurity::delete_resource(SecurityResource)
{
    return m2msecurity_stub::bool_value;
}

bool M2MSecurity::set_resource_value(SecurityResource,
                                     const String &)
{
    return m2msecurity_stub::bool_value;
}

bool M2MSecurity::set_resource_value(SecurityResource,
                                     uint32_t)
{
    return m2msecurity_stub::bool_value;
}

bool M2MSecurity::set_resource_value(SecurityResource,
                                     const uint8_t *,
                                     const uint16_t)
{
    return m2msecurity_stub::bool_value;
}

String M2MSecurity::resource_value_string(SecurityResource) const
{
    return *m2msecurity_stub::string_value;
}

uint32_t M2MSecurity::resource_value_buffer(SecurityResource,
                               uint8_t *&value) const
{
    if( m2msecurity_stub::has_value ){
        value = (uint8_t *)malloc(5);
        return 5;
    }
    return m2msecurity_stub::int_value;
}

uint32_t M2MSecurity::resource_value_buffer(SecurityResource,
                               const uint8_t *&value) const
{
    if( m2msecurity_stub::has_value ){
        value = NULL;
        return 5;
    }
    return m2msecurity_stub::int_value;
}

uint32_t M2MSecurity::resource_value_int(SecurityResource res) const
{
    if (res == M2MSecurity::SecurityMode) {
        return m2msecurity_stub::sec_mode;
    }
    else if(res == M2MSecurity::BootstrapServer) {
        return m2msecurity_stub::bool_value;
    }
    else {
        return m2msecurity_stub::int_value;
    }
}

bool M2MSecurity::is_resource_present(SecurityResource) const
{
    return m2msecurity_stub::bool_value;
}

uint16_t M2MSecurity::total_resource_count() const
{
    return (uint16_t)m2msecurity_stub::int_value;
}

M2MSecurity::ServerType M2MSecurity::server_type() const
{
    return _server_type;
}

void M2MSecurity::clear_resources()
{

}

M2MResource* M2MSecurity::get_resource(SecurityResource res) const
{
    return m2msecurity_stub::resource;
}
