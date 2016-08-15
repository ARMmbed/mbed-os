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
#include "m2mtlvdeserializer_stub.h"

bool m2mtlvdeserializer_stub::bool_value;
bool m2mtlvdeserializer_stub::is_object_bool_value;
M2MTLVDeserializer::Error m2mtlvdeserializer_stub::error;
uint16_t m2mtlvdeserializer_stub::int_value;

void m2mtlvdeserializer_stub::clear()
{
    bool_value = false;
    is_object_bool_value = false;
    error = M2MTLVDeserializer::None;
    int_value = 0;
}

M2MTLVDeserializer::M2MTLVDeserializer()
{
}

M2MTLVDeserializer::~M2MTLVDeserializer()
{
}

bool M2MTLVDeserializer::is_object_instance(uint8_t *)
{
    return m2mtlvdeserializer_stub::is_object_bool_value;
}

bool M2MTLVDeserializer::is_resource(uint8_t *)
{
    return m2mtlvdeserializer_stub::bool_value;
}

bool M2MTLVDeserializer::is_multiple_resource(uint8_t *)
{
    return m2mtlvdeserializer_stub::bool_value;
}

bool M2MTLVDeserializer::is_resource_instance(uint8_t *)
{
    return m2mtlvdeserializer_stub::bool_value;
}

M2MTLVDeserializer::Error M2MTLVDeserializer::deserialize_resources(uint8_t*,
                                                                    uint32_t,
                                                                    M2MObjectInstance &,
                                                                    M2MTLVDeserializer::Operation )
{
    return m2mtlvdeserializer_stub::error;
}

M2MTLVDeserializer::Error M2MTLVDeserializer::deserialize_resource_instances(uint8_t *,
                                                                             uint32_t,
                                                                             M2MResource &,
                                                                             M2MTLVDeserializer::Operation )
{
    return m2mtlvdeserializer_stub::error;
}

M2MTLVDeserializer::Error M2MTLVDeserializer::deserialise_object_instances(uint8_t*,
                                                                           uint32_t ,
                                                                           M2MObject &,
                                                                           M2MTLVDeserializer::Operation)
{
    return m2mtlvdeserializer_stub::error;
}

TypeIdLength* TypeIdLength::createTypeIdLength(uint8_t *, uint32_t)
{
    TypeIdLength *til = new TypeIdLength();
    return til;
}

TypeIdLength* TypeIdLength::deserialize()
{
    return this;
}

void TypeIdLength::deserialiseID(uint32_t idLength)
{
}

void TypeIdLength::deserialiseLength(uint32_t lengthType)
{
}

uint16_t M2MTLVDeserializer::instance_id(uint8_t *tlv)
{
    return m2mtlvdeserializer_stub::int_value;
}
