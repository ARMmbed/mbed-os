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
#include "m2mtlvserializer_stub.h"

uint8_t* m2mtlvserializer_stub::uint8_value;

void m2mtlvserializer_stub::clear()
{
    uint8_value = NULL;
}

M2MTLVSerializer::M2MTLVSerializer()
{
}

M2MTLVSerializer::~M2MTLVSerializer()
{
}

uint8_t* M2MTLVSerializer::serialize(M2MObjectInstanceList object_instance_list, uint32_t &)
{
    return m2mtlvserializer_stub::uint8_value;
}

uint8_t* M2MTLVSerializer::serialize(M2MResourceList resource_list, uint32_t &size)
{
    return m2mtlvserializer_stub::uint8_value;
}

uint8_t* M2MTLVSerializer::serialize(M2MResource *, uint32_t &)
{
    return m2mtlvserializer_stub::uint8_value;
}
