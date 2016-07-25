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
#include "m2mfirmware_stub.h"
bool m2mfirmware_stub::has_value;
uint32_t m2mfirmware_stub::int_value;
bool m2mfirmware_stub::bool_value;
M2MResource* m2mfirmware_stub::resource;
String m2mfirmware_stub::string_value;

void m2mfirmware_stub::clear()
{
    int_value = -1;
    bool_value = false;
    string_value = "";
    resource = NULL;
    has_value = false;
}

M2MFirmware::M2MFirmware()
: M2MObject("5")
{
}

M2MFirmware::~M2MFirmware()
{
}

M2MFirmware* M2MFirmware::get_instance()
{
    return new M2MFirmware();
}

void M2MFirmware::delete_instance()
{
}

M2MResource* M2MFirmware::create_resource(FirmwareResource, const String &)
{
    return m2mfirmware_stub::resource;
}

bool M2MFirmware::set_resource_value(FirmwareResource,
                                     const uint8_t *,
                                     const uint32_t)
{
    return m2mfirmware_stub::bool_value;
}


uint32_t M2MFirmware::resource_value_buffer(FirmwareResource,
                               uint8_t *&value) const
{
    if( m2mfirmware_stub::has_value ){
        value = (uint8_t *)malloc(5);
        return 5;
    }
    return m2mfirmware_stub::int_value;
}

bool M2MFirmware::delete_resource(FirmwareResource)
{
    return m2mfirmware_stub::bool_value;
}

bool M2MFirmware::set_resource_value(FirmwareResource,
                                   const String &)
{
    return m2mfirmware_stub::bool_value;
}

bool M2MFirmware::set_resource_value(FirmwareResource,
                                       int64_t)
{
    return m2mfirmware_stub::bool_value;
}

String M2MFirmware::resource_value_string(FirmwareResource) const
{
    return m2mfirmware_stub::string_value;
}

int64_t M2MFirmware::resource_value_int(FirmwareResource) const
{
    return m2mfirmware_stub::int_value;
}

bool M2MFirmware::is_resource_present(FirmwareResource) const
{
    return m2mfirmware_stub::bool_value;
}

uint16_t M2MFirmware::per_resource_count(FirmwareResource) const
{
    return (uint16_t)m2mfirmware_stub::int_value;
}

uint16_t M2MFirmware::total_resource_count() const
{
    return (uint16_t)m2mfirmware_stub::int_value;
}
