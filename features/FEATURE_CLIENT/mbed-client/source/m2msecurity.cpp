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
#include "mbed-client/m2msecurity.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mstring.h"
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "mClt"

#define BUFFER_SIZE 21

M2MSecurity::M2MSecurity(ServerType ser_type)
: M2MObject(M2M_SECURITY_ID),
 _server_instance(NULL),
 _server_type(ser_type)
{
     _server_instance  = M2MObject::create_object_instance();

    if(_server_instance) {
        M2MResource* res = _server_instance->create_dynamic_resource(SECURITY_M2M_SERVER_URI,
                                                                     OMA_RESOURCE_TYPE,
                                                                     M2MResourceInstance::STRING,
                                                                     false);
        if(res) {
            res->set_operation(M2MBase::NOT_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SECURITY_BOOTSTRAP_SERVER,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::BOOLEAN,
                                                        false);
        if(res) {
            res->set_operation(M2MBase::NOT_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SECURITY_SECURITY_MODE,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::INTEGER,
                                                        false);
        if(res) {
            res->set_operation(M2MBase::NOT_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SECURITY_PUBLIC_KEY,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::OPAQUE,
                                                        false);
        if(res) {
            res->set_operation(M2MBase::NOT_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SECURITY_SERVER_PUBLIC_KEY,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::OPAQUE,
                                                        false);
        if(res) {
            res->set_operation(M2MBase::NOT_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SECURITY_SECRET_KEY,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::OPAQUE,
                                                        false);
        if(res) {
            res->set_operation(M2MBase::NOT_ALLOWED);
        }
        if(M2MSecurity::M2MServer == ser_type) {
            res = _server_instance->create_dynamic_resource(SECURITY_SHORT_SERVER_ID,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::INTEGER,
                                                            false);
            if(res) {
                res->set_operation(M2MBase::NOT_ALLOWED);
            }
        }
    }
}

M2MSecurity::~M2MSecurity()
{
}

M2MResource* M2MSecurity::create_resource(SecurityResource resource, uint32_t value)
{
    M2MResource* res = NULL;
    const char* security_id_ptr = "";
    if(!is_resource_present(resource)) {
        switch(resource) {
            case SMSSecurityMode:
               security_id_ptr = SECURITY_SMS_SECURITY_MODE;
               break;
            case M2MServerSMSNumber:
                security_id_ptr = SECURITY_M2M_SERVER_SMS_NUMBER;
                break;
            case ShortServerID:
                security_id_ptr = SECURITY_SHORT_SERVER_ID;
                break;
            case ClientHoldOffTime:
                security_id_ptr = SECURITY_CLIENT_HOLD_OFF_TIME;
                break;
            default:
                break;
        }
    }

    const String security_id(security_id_ptr);

    if(!security_id.empty()) {
        if(_server_instance) {
            res = _server_instance->create_dynamic_resource(security_id,OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::INTEGER,
                                                            false);

            if(res) {
                res->set_operation(M2MBase::NOT_ALLOWED);
                res->set_value(value);
            }
        }
    }
    return res;
}

bool M2MSecurity::delete_resource(SecurityResource resource)
{
    bool success = false;
    const char* security_id_ptr = "";
    switch(resource) {
        case SMSSecurityMode:
           security_id_ptr = SECURITY_SMS_SECURITY_MODE;
           break;
        case M2MServerSMSNumber:
            security_id_ptr = SECURITY_M2M_SERVER_SMS_NUMBER;
            break;
        case ShortServerID:
            if(M2MSecurity::Bootstrap == _server_type) {
                security_id_ptr = SECURITY_SHORT_SERVER_ID;
            }
            break;
        case ClientHoldOffTime:
            security_id_ptr = SECURITY_CLIENT_HOLD_OFF_TIME;
            break;
        default:
            // Others are mandatory resources hence cannot be deleted.
            break;
    }

    const String security_id(security_id_ptr);

    if(!security_id.empty()) {
        if(_server_instance) {
            success = _server_instance->remove_resource(security_id);
        }
    }
    return success;
}

bool M2MSecurity::set_resource_value(SecurityResource resource,
                                     const String &value)
{
    bool success = false;
    if(M2MSecurity::M2MServerUri == resource) {
        M2MResource* res = get_resource(resource);
        if(res) {
            success = res->set_value((const uint8_t*)value.c_str(),(uint32_t)value.length());
        }
    }
    return success;
}

bool M2MSecurity::set_resource_value(SecurityResource resource,
                                     uint32_t value)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MSecurity::SecurityMode == resource        ||
           M2MSecurity::SMSSecurityMode == resource     ||
           M2MSecurity::M2MServerSMSNumber == resource  ||
           M2MSecurity::ShortServerID == resource       ||
           M2MSecurity::ClientHoldOffTime == resource) {
            // If it is any of the above resource
            // set the value of the resource.
            uint8_t size = 0;
            uint8_t *buffer = String::convert_integer_to_array(value, size);
            success = res->set_value(buffer,size);
            free(buffer);
        }
    }
    return success;
}

bool M2MSecurity::set_resource_value(SecurityResource resource,
                                     const uint8_t *value,
                                     const uint16_t length)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MSecurity::PublicKey == resource           ||
           M2MSecurity::ServerPublicKey == resource     ||
           M2MSecurity::Secretkey == resource) {
            success = res->set_value(value,length);
        }
    }
    return success;
}

String M2MSecurity::resource_value_string(SecurityResource resource) const
{
    String value = "";
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MSecurity::M2MServerUri == resource) {
            value = res->get_value_string();
        }
    }
    return value;
}

uint32_t M2MSecurity::resource_value_buffer(SecurityResource resource,
                               uint8_t *&data) const
{
    uint32_t size = 0;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MSecurity::PublicKey == resource        ||
           M2MSecurity::ServerPublicKey == resource  ||
           M2MSecurity::Secretkey == resource) {
            res->get_value(data,size);
        }
    }
    return size;
}

uint32_t M2MSecurity::resource_value_buffer(SecurityResource resource,
                               const uint8_t *&data) const
{
    uint32_t size = 0;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MSecurity::PublicKey == resource        ||
           M2MSecurity::ServerPublicKey == resource  ||
           M2MSecurity::Secretkey == resource) {
            data = res->value();
            size = res->value_length();
        }
    }
    return size;
}


uint32_t M2MSecurity::resource_value_int(SecurityResource resource) const
{
    uint32_t value = 0;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MSecurity::SecurityMode == resource        ||
           M2MSecurity::SMSSecurityMode == resource     ||
           M2MSecurity::M2MServerSMSNumber == resource  ||
           M2MSecurity::ShortServerID == resource       ||
           M2MSecurity::ClientHoldOffTime == resource) {
            // Get the value and convert it into integer. This is not the most
            // efficient way, as it takes pointless heap copy to get the zero termination.
            uint8_t* buffer = NULL;
            uint32_t length = 0;
            res->get_value(buffer,length);
            if(buffer) {
                value = String::convert_array_to_integer(buffer,length);
                free(buffer);
            }
        }
    }
    return value;
}

bool M2MSecurity::is_resource_present(SecurityResource resource) const
{
    bool success = false;
    M2MResource *res = get_resource(resource);
    if(res) {
        success = true;
    }
    return success;
}

uint16_t M2MSecurity::total_resource_count() const
{
    uint16_t count = 0;
    if(_server_instance) {
        count = _server_instance->resources().size();
    }
    return count;
}

M2MSecurity::ServerType M2MSecurity::server_type() const
{
    return _server_type;
}

M2MResource* M2MSecurity::get_resource(SecurityResource res) const
{
    M2MResource* res_object = NULL;
    if(_server_instance) {
        const char* res_name_ptr = "";
        switch(res) {
            case M2MServerUri:
                res_name_ptr = SECURITY_M2M_SERVER_URI;
                break;
            case BootstrapServer:
                res_name_ptr = SECURITY_BOOTSTRAP_SERVER;
                break;
            case SecurityMode:
                res_name_ptr = SECURITY_SECURITY_MODE;
                break;
            case PublicKey:
                res_name_ptr = SECURITY_PUBLIC_KEY;
                break;
            case ServerPublicKey:
                res_name_ptr = SECURITY_SERVER_PUBLIC_KEY;
                break;
            case Secretkey:
                res_name_ptr = SECURITY_SECRET_KEY;
                break;
            case SMSSecurityMode:
                res_name_ptr = SECURITY_SMS_SECURITY_MODE;
                break;
            case SMSBindingKey:
                res_name_ptr = SECURITY_SMS_BINDING_KEY;
                break;
            case SMSBindingSecretKey:
                res_name_ptr = SECURITY_SMS_BINDING_SECRET_KEY;
                break;
            case M2MServerSMSNumber:
                res_name_ptr = SECURITY_M2M_SERVER_SMS_NUMBER;
                break;
            case ShortServerID:
                res_name_ptr = SECURITY_SHORT_SERVER_ID;
                break;
            case ClientHoldOffTime:
                res_name_ptr = SECURITY_CLIENT_HOLD_OFF_TIME;
                break;
        }
        const String res_name(res_name_ptr);

        res_object = _server_instance->resource(res_name);
    }
    return res_object;
}

void M2MSecurity::clear_resources()
{
    for(int i = 0; i <= M2MSecurity::ClientHoldOffTime; i++) {
        M2MResource *res = get_resource((SecurityResource) i);
        if (res) {
            res->clear_value();
        }
    }
}
