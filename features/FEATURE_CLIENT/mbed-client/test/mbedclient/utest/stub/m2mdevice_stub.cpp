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
#include "m2mdevice_stub.h"

uint32_t m2mdevice_stub::int_value;
bool m2mdevice_stub::bool_value;
M2MResource* m2mdevice_stub::resource;
String m2mdevice_stub::string_value;

void m2mdevice_stub::clear()
{
    int_value = -1;
    bool_value = false;
    string_value = "";
    resource = NULL;
}

M2MDevice::M2MDevice()
: M2MObject("3")
{
}

M2MDevice::~M2MDevice()
{
}

M2MDevice* M2MDevice::get_instance()
{
    return new M2MDevice();
}

void M2MDevice::delete_instance()
{
}

M2MResource* M2MDevice::create_resource(DeviceResource, const String &)
{
    return m2mdevice_stub::resource;
}

M2MResource* M2MDevice::create_resource(DeviceResource)
{
    return m2mdevice_stub::resource;
}

bool M2MDevice::delete_resource(DeviceResource)
{
    return m2mdevice_stub::bool_value;
}

bool M2MDevice::delete_resource_instance(DeviceResource, uint16_t)
{
    return m2mdevice_stub::bool_value;
}

bool M2MDevice::set_resource_value(DeviceResource,
                                   const String &,
                                   uint16_t )
{
    return m2mdevice_stub::bool_value;
}

bool M2MDevice::set_resource_value(DeviceResource,
                                       int64_t,
                                       uint16_t)
{
    return m2mdevice_stub::bool_value;
}

String M2MDevice::resource_value_string(DeviceResource,
                                        uint16_t) const
{
    return m2mdevice_stub::string_value;
}

int64_t M2MDevice::resource_value_int(DeviceResource,
                                      uint16_t) const
{
    return m2mdevice_stub::int_value;
}

bool M2MDevice::is_resource_present(DeviceResource) const
{
    return m2mdevice_stub::bool_value;
}

uint16_t M2MDevice::per_resource_count(DeviceResource) const
{
    return (uint16_t)m2mdevice_stub::int_value;
}

uint16_t M2MDevice::total_resource_count() const
{
    return (uint16_t)m2mdevice_stub::int_value;
}
