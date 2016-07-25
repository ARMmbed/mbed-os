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
#include <cstdio>
#include "mbed-client/m2mfirmware.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"

#define BUFFER_SIZE 21
#define TRACE_GROUP "mClt"

M2MFirmware* M2MFirmware::_instance = NULL;

M2MFirmware* M2MFirmware::get_instance()
{
    if(_instance == NULL) {
        _instance = new M2MFirmware();
    }
    return _instance;
}

void M2MFirmware::delete_instance()
{
    delete _instance;
    _instance = NULL;
}

M2MFirmware::M2MFirmware()
: M2MObject(M2M_FIRMWARE_ID)
{
    M2MBase::set_register_uri(false);
    M2MBase::set_operation(M2MBase::GET_PUT_ALLOWED);
    _firmware_instance = M2MObject::create_object_instance();
    if(_firmware_instance) {
        _firmware_instance->set_operation(M2MBase::GET_PUT_ALLOWED);
        create_mandatory_resources();
    }
}

M2MFirmware::~M2MFirmware()
{
}

void M2MFirmware::create_mandatory_resources()
{
    _firmware_instance->set_coap_content_type(COAP_CONTENT_OMA_TLV_TYPE);
    M2MResource* res = _firmware_instance->create_dynamic_resource(FIRMWARE_PACKAGE,
                                                                 OMA_RESOURCE_TYPE,
                                                                 M2MResourceInstance::OPAQUE,
                                                                 false);
    if(res) {
        res->set_operation(M2MBase::PUT_ALLOWED);
        res->set_register_uri(false);
    }

    res = _firmware_instance->create_dynamic_resource(FIRMWARE_PACKAGE_URI,
                                                    OMA_RESOURCE_TYPE,
                                                    M2MResourceInstance::STRING,
                                                    false);
    if(res) {
        res->set_operation(M2MBase::PUT_ALLOWED);
        res->set_register_uri(false);
    }

    res = _firmware_instance->create_dynamic_resource(FIRMWARE_UPDATE,
                                                    OMA_RESOURCE_TYPE,
                                                    M2MResourceInstance::OPAQUE,
                                                    false);
    if(res) {
        res->set_operation(M2MBase::NOT_ALLOWED);
        res->set_register_uri(false);
    }

    res = _firmware_instance->create_dynamic_resource(FIRMWARE_STATE,
                                                    OMA_RESOURCE_TYPE,
                                                    M2MResourceInstance::INTEGER,
                                                    true);
    set_zero_value(res);
    if(res) {        
        res->set_operation(M2MBase::GET_ALLOWED);
        res->set_register_uri(false);
    }
    res = _firmware_instance->create_dynamic_resource(FIRMWARE_UPDATE_RESULT,
                                                    OMA_RESOURCE_TYPE,
                                                    M2MResourceInstance::INTEGER,
                                                    true);
    set_zero_value(res);
    if(res) {
        res->set_operation(M2MBase::GET_ALLOWED);
        res->set_register_uri(false);
    }
}

M2MResource* M2MFirmware::create_resource(FirmwareResource resource, const String &value)
{
    M2MResource* res = NULL;
    const char* firmware_id_ptr = "";
    M2MBase::Operation operation = M2MBase::GET_ALLOWED;
    if(!is_resource_present(resource)) {
        switch(resource) {
            case PackageName:
                firmware_id_ptr = FIRMWARE_PACKAGE_NAME;
                break;
            case PackageVersion:
                firmware_id_ptr = FIRMWARE_PACKAGE_VERSION;
                break;
            default:
                break;
        }
    }
    String firmware_id(firmware_id_ptr);

    if(!firmware_id.empty() && value.size() < 256) {
        if(_firmware_instance) {
            res = _firmware_instance->create_dynamic_resource(firmware_id,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::STRING,
                                                            false);

            if(res) {
                res->set_register_uri(false);
                res->set_operation(operation);
                if(value.empty()) {
                    res->clear_value();
                } else {
                    res->set_value((const uint8_t*)value.c_str(),
                                   (uint32_t)value.length());
                }
            }
        }
    }
    return res;
}

M2MResource* M2MFirmware::create_resource(FirmwareResource resource, int64_t value)
{
    M2MResource* res = NULL;
    const char* firmware_id_ptr = "";
    M2MBase::Operation operation = M2MBase::GET_ALLOWED;
    if(!is_resource_present(resource)) {
        switch(resource) {
        case UpdateSupportedObjects:
            if(check_value_range(resource, value)) {
                firmware_id_ptr = FIRMWARE_UPDATE_SUPPORTED_OBJECTS;
                operation = M2MBase::GET_PUT_ALLOWED;
            }
            break;
        default:
            break;
        }
    }

    const String firmware_id(firmware_id_ptr);
    
    if(!firmware_id.empty()) {
        if(_firmware_instance) {
            res = _firmware_instance->create_dynamic_resource(firmware_id,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::INTEGER,
                                                            false);

            if(res) {
                res->set_register_uri(false);

                res->set_operation(operation);
                res->set_value(value);
            }
        }
    }
    return res;
}

bool M2MFirmware::set_resource_value(FirmwareResource resource,
                                   const String &value)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MFirmware::PackageUri == resource  ||
           M2MFirmware::PackageName == resource ||
           M2MFirmware::PackageVersion == resource) {
            if (value.size() < 256) {
                if(value.empty()) {
                    res->clear_value();
                    success = true;
                } else {
                    success = res->set_value((const uint8_t*)value.c_str(),(uint32_t)value.length());
                }
            }
        }
    }
    return success;
}

bool M2MFirmware::set_resource_value(FirmwareResource resource,
                                       int64_t value)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MFirmware::State == resource          ||
           M2MFirmware::UpdateSupportedObjects == resource ||
           M2MFirmware::UpdateResult == resource) {
            // If it is any of the above resource
            // set the value of the resource.
            if (check_value_range(resource, value)) {
                
                success = res->set_value(value);
            }
        }
    }
    return success;
}

bool M2MFirmware::set_resource_value(FirmwareResource resource,
                                     const uint8_t *value,
                                     const uint32_t length)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MFirmware::Package == resource) {
            success = res->set_value(value,length);
        }
    }
    return success;
}

bool M2MFirmware::is_resource_present(FirmwareResource resource) const
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        success = true;
    }
    return success;
}

const String M2MFirmware::resource_name(FirmwareResource resource)
{
    const char* res_name = "";
    switch(resource) {
        case Package:
            res_name = FIRMWARE_PACKAGE;
            break;
        case PackageUri:
            res_name = FIRMWARE_PACKAGE_URI;
            break;
        case Update:
            res_name = FIRMWARE_UPDATE;
            break;
        case State:
            res_name = FIRMWARE_STATE;
            break;
        case UpdateSupportedObjects:
            res_name = FIRMWARE_UPDATE_SUPPORTED_OBJECTS;
            break;
        case UpdateResult:
            res_name = FIRMWARE_UPDATE_RESULT;
            break;
        case PackageName:
            res_name = FIRMWARE_PACKAGE_NAME;
            break;
        case PackageVersion:
            res_name = FIRMWARE_PACKAGE_VERSION;
            break;
    }
    return String(res_name);
}

uint16_t M2MFirmware::per_resource_count(FirmwareResource res) const
{
    uint16_t count = 0;
    if(_firmware_instance) {
        count = _firmware_instance->resource_count(resource_name(res));
    }
    return count;
}

uint16_t M2MFirmware::total_resource_count() const
{
    uint16_t count = 0;
    if(_firmware_instance) {
        count = _firmware_instance->resources().size();
    }
    return count;
}

uint32_t M2MFirmware::resource_value_buffer(FirmwareResource resource,
                               uint8_t *&data) const
{
    uint32_t size = 0;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MFirmware::Package == resource) {
            res->get_value(data,size);
        }
    }
    return size;
}

M2MResource* M2MFirmware::get_resource(FirmwareResource res) const
{
    M2MResource* res_object = NULL;
    if(_firmware_instance) {
        const char* res_name_ptr = "";
        switch(res) {
            case Package:
                res_name_ptr = FIRMWARE_PACKAGE;
                break;
            case PackageUri:
                res_name_ptr = FIRMWARE_PACKAGE_URI;
                break;
            case Update:
                res_name_ptr = FIRMWARE_UPDATE;
                break;
            case State:
                res_name_ptr = FIRMWARE_STATE;
                break;
            case UpdateSupportedObjects:
                res_name_ptr = FIRMWARE_UPDATE_SUPPORTED_OBJECTS;
                break;
            case UpdateResult:
                res_name_ptr = FIRMWARE_UPDATE_RESULT;
                break;
            case PackageName:
                res_name_ptr = FIRMWARE_PACKAGE_NAME;
                break;
            case PackageVersion:
                res_name_ptr = FIRMWARE_PACKAGE_VERSION;
                break;
        }
        const String res_name(res_name_ptr);
        
        res_object = _firmware_instance->resource(res_name);
    }
    return res_object;
}

bool M2MFirmware::delete_resource(FirmwareResource resource)
{
    bool success = false;
    if(M2MFirmware::UpdateSupportedObjects == resource ||
       M2MFirmware::PackageName == resource            ||
       M2MFirmware::PackageVersion == resource) {
        if(_firmware_instance) {
            success = _firmware_instance->remove_resource(resource_name(resource));
        }
    }
    return success;
}

int64_t M2MFirmware::resource_value_int(FirmwareResource resource) const
{
    int64_t value = -1;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MFirmware::State == resource          ||
           M2MFirmware::UpdateSupportedObjects == resource         ||
           M2MFirmware::UpdateResult == resource) {

            value = res->get_value_int();
        }
    }
    return value;
}

String M2MFirmware::resource_value_string(FirmwareResource resource) const
{
    String value = "";
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MFirmware::PackageUri == resource          ||
           M2MFirmware::PackageName == resource           ||
           M2MFirmware::PackageVersion == resource) {

            value = res->get_value_string();
        }
    }
    return value;
}

bool M2MFirmware::check_value_range(FirmwareResource resource, int64_t value) const
{
    bool success = false;
    switch (resource) {
        case UpdateSupportedObjects:
            if(value == 0 || value == 1) {
                success = true;
            }
            break;
        case State:
            if (value >= 0 && value <= 3) {
                success = true;
                M2MResource* updateRes = get_resource(M2MFirmware::Update);
                if (updateRes){
                    if (value == M2MFirmware::Downloaded) {
                        updateRes->set_operation(M2MBase::POST_ALLOWED);
                    }
                    else {
                        updateRes->set_operation(M2MBase::NOT_ALLOWED);
                    }
                }
            }
            break;
        case UpdateResult:
            if (value >= 0 && value <= 7) {
                success = true;
            }
            break;
    default:
        break;
    }
    return success;
}

void M2MFirmware::set_zero_value(M2MResource *resource)
{
    resource->set_value((const uint8_t*)"0", 1);
}
