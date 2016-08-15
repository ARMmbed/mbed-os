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
#include <stdio.h>
#include "include/m2mtlvserializer.h"
#include "include/nsdllinker.h"
#include "mbed-client/m2mconstants.h"

#define TRACE_GROUP "mClt"

M2MTLVSerializer::M2MTLVSerializer()
{
}

M2MTLVSerializer::~M2MTLVSerializer()
{
}

uint8_t* M2MTLVSerializer::serialize(M2MObjectInstanceList object_instance_list, uint32_t &size)
{
    return serialize_object_instances(object_instance_list, size);
}

uint8_t* M2MTLVSerializer::serialize(M2MResourceList resource_list, uint32_t &size)
{
    bool valid = true;
    return serialize_resources(resource_list, size,valid);
}

uint8_t* M2MTLVSerializer::serialize(M2MResource *resource, uint32_t &size)
{
    uint8_t* data = NULL;
    serialize(resource, data, size);
    return data;
}

uint8_t* M2MTLVSerializer::serialize_object_instances(M2MObjectInstanceList object_instance_list, uint32_t &size)
{
    uint8_t *data = NULL;

    if(!object_instance_list.empty()) {
        M2MObjectInstanceList::const_iterator it;
        it = object_instance_list.begin();
        for (; it!=object_instance_list.end(); it++) {
            uint16_t id = (*it)->instance_id();
            serialize(id, *it, data, size);
        }
    }
    return data;
}

uint8_t* M2MTLVSerializer::serialize_resources(M2MResourceList resource_list, uint32_t &size, bool &valid)
{
    uint8_t *data = NULL;

    if(!resource_list.empty()) {
        M2MResourceList::const_iterator it;
        it = resource_list.begin();
        for (; it!=resource_list.end(); it++) {
            if((*it)->name_id() == -1) {
                valid = false;
                break;
            }
        }
        if(valid) {
            it = resource_list.begin();
            for (; it!=resource_list.end(); it++) {
                serialize(*it, data, size);
            }
        }
    }
    return data;
}

void M2MTLVSerializer::serialize(uint16_t id, M2MObjectInstance *object_instance, uint8_t *&data, uint32_t &size)
{
    uint8_t *resource_data = NULL;
    uint32_t resource_size = 0;

    bool valid = true;
    resource_data = serialize_resources(object_instance->resources(),resource_size,valid);
    if(valid) {
        serialize_TILV(TYPE_OBJECT_INSTANCE, id, resource_data, resource_size, data, size);
    }
    free(resource_data);
}

bool M2MTLVSerializer::serialize(M2MResource *resource, uint8_t *&data, uint32_t &size)
{
    bool success = false;
    if(resource->name_id() != -1) {
        success = resource->supports_multiple_instances() ?
                serialize_multiple_resource(resource, data, size) :
                serialize_resource(resource, data, size);
    }
    return success;
}

bool M2MTLVSerializer::serialize_resource(M2MResource *resource, uint8_t *&data, uint32_t &size)
{
    bool success = false;
    if(resource->name_id() != -1) {
        success = true;
        serialize_TILV(TYPE_RESOURCE, resource->name_id(), resource->value(), resource->value_length(), data, size);
    }
    return success;
}

bool M2MTLVSerializer::serialize_multiple_resource(M2MResource *resource, uint8_t *&data, uint32_t &size)
{
    bool success = false;
    uint8_t *nested_data = NULL;
    uint32_t nested_data_size = 0;

    M2MResourceInstanceList instance_list = resource->resource_instances();
    if(!instance_list.empty()) {
        M2MResourceInstanceList::const_iterator it;
        it = instance_list.begin();
        for (; it!=instance_list.end(); it++) {
            uint16_t id = (*it)->instance_id();
            serialize_resource_instance(id, (*it), nested_data, nested_data_size);            
        }
    }
    if(resource->name_id() != -1) {
        success = true;
        serialize_TILV(TYPE_MULTIPLE_RESOURCE, resource->name_id(), nested_data, nested_data_size, data, size);
    }

    free(nested_data);
    nested_data = NULL;
    return success;
}

void M2MTLVSerializer::serialize_resource_instance(uint16_t id, M2MResourceInstance *resource, uint8_t *&data, uint32_t &size)
{
    serialize_TILV(TYPE_RESOURCE_INSTANCE, id, resource->value(), resource->value_length(), data, size);
}

void M2MTLVSerializer::serialize_TILV(uint8_t type, uint16_t id, uint8_t *value, uint32_t value_length, uint8_t *&data, uint32_t &size)
{
    uint8_t *tlv = 0;
    uint32_t type_length = 1;
    type += id < 256 ? 0 : ID16;
    type += value_length < 8 ? value_length :
            value_length < 256 ? LENGTH8 :
            value_length < 65536 ? LENGTH16 : LENGTH24;
    uint8_t *tlv_type = (uint8_t*)malloc(type_length+1);
    memset(tlv_type,0,type_length+1);
    *tlv_type = type & 0xFF;

    uint32_t id_size = 0;
    uint8_t* id_ptr = serialize_id(id, id_size);

    uint32_t length_size = 0;
    uint8_t* length_ptr = serialize_length(value_length, length_size);

    tlv = (uint8_t*)malloc(size + type_length + id_size + length_size + value_length+1);
    memset(tlv,0,size + type_length + id_size + length_size + value_length+1);
    if(data) {
        memcpy(tlv, data, size);
    }
    memcpy(tlv+size, tlv_type, type_length);
    memcpy(tlv+size+type_length, id_ptr, id_size);
    memcpy(tlv+size+type_length+id_size, length_ptr, length_size);
    memcpy(tlv+size+type_length+id_size+length_size, value, value_length);

    free(tlv_type) ;
    free(length_ptr);
    free(id_ptr);
    free(data);

    data = tlv;
    size += type_length + id_size + length_size + value_length;
}

uint8_t* M2MTLVSerializer::serialize_id(uint16_t id, uint32_t &size)
{
    uint32_t id_size = id > 255 ? 2 : 1;
    uint8_t *id_ptr = (uint8_t*)malloc(id_size);
    memset(id_ptr, 0 , id_size);
    size += id_size;
    if(id > 255) {
        *id_ptr = (id & 0xFF00) >> 8;
        id_ptr++;
        *id_ptr = id & 0xFF;
        id_ptr--;
    } else {
        *id_ptr = id & 0xFF;
    }
    return id_ptr;
}

uint8_t* M2MTLVSerializer::serialize_length(uint32_t length, uint32_t &size)
{
    uint8_t *length_ptr = 0;
    uint32_t length_size = 0;
    if (length > 65535) {
        length_size = 3;
        length_ptr = (uint8_t*)malloc(length_size+1);
        memset(length_ptr,0,length_size+1);
        *length_ptr = (length & 0xFF0000) >> 16;
        length_ptr++;
        *length_ptr = (length & 0xFF00) >> 8;
        length_ptr++;
        *length_ptr = length & 0xFF;
        length_ptr--;
        length_ptr--;
    } else if (length > 255) {
        length_size = 2;
        length_ptr = (uint8_t*)malloc(length_size+1);
        memset(length_ptr,0,length_size+1);
        *length_ptr = (length & 0xFF00) >> 8;
        length_ptr++;
        *length_ptr = length & 0xFF;
        length_ptr--;
    } else if (length > 7) {
        length_size = 1;
        length_ptr = (uint8_t*)malloc(length_size+1);
        memset(length_ptr,0,length_size+1);
        *length_ptr = length & 0xFF;
    }
    size += length_size;
    return length_ptr;
}

