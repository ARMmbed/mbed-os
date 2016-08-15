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
#include "mbed-client/m2mserver.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mstring.h"

#define TRACE_GROUP "mClt"

#define BUFFER_SIZE 21

M2MServer::M2MServer()
: M2MObject(M2M_SERVER_ID)
{
    M2MObject::create_object_instance();

    _server_instance = object_instance();

    if(_server_instance) {

        M2MResource* res = _server_instance->create_dynamic_resource(SERVER_SHORT_SERVER_ID,
                                                                     OMA_RESOURCE_TYPE,
                                                                     M2MResourceInstance::INTEGER,
                                                                     true);
        if(res) {
            res->set_operation(M2MBase::GET_PUT_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SERVER_LIFETIME,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::INTEGER,
                                                        true);
        if(res) {
            res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SERVER_NOTIFICATION_STORAGE,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::BOOLEAN,
                                                        true);
        if(res) {
            res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SERVER_BINDING,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::STRING,
                                                        true);
        if(res) {
            res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
        }
        res = _server_instance->create_dynamic_resource(SERVER_REGISTRATION_UPDATE,
                                                        OMA_RESOURCE_TYPE,
                                                        M2MResourceInstance::OPAQUE,
                                                        false);
        if(res) {
          res->set_operation(M2MBase::POST_ALLOWED);
        }
    }
}

M2MServer::~M2MServer()
{

}

M2MResource* M2MServer::create_resource(ServerResource resource, uint32_t value)
{
    M2MResource* res = NULL;
    const char* server_id_ptr = "";
    if(!is_resource_present(resource)) {
        switch(resource) {
        case DefaultMinPeriod:
            server_id_ptr = SERVER_DEFAULT_MIN_PERIOD;
            break;
        case DefaultMaxPeriod:
            server_id_ptr = SERVER_DEFAULT_MAX_PERIOD;
            break;
        case DisableTimeout:
            server_id_ptr = SERVER_DISABLE_TIMEOUT;
            break;
        default:
            break;
        }
    }
    String server_id(server_id_ptr);
    
    if(!server_id.empty()) {
        if(_server_instance) {
            res = _server_instance->create_dynamic_resource(server_id,
                                                            OMA_RESOURCE_TYPE,
                                                            M2MResourceInstance::INTEGER,
                                                            true);
            if(res) {
                res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
                
                res->set_value(value);
            }
        }
    }
    return res;
}

M2MResource* M2MServer::create_resource(ServerResource resource)
{
    M2MResource* res = NULL;
    if(!is_resource_present(resource)) {
        if(M2MServer::Disable == resource) {
                if(_server_instance) {
                    res = _server_instance->create_dynamic_resource(SERVER_DISABLE,
                                                                    OMA_RESOURCE_TYPE,
                                                                    M2MResourceInstance::OPAQUE,
                                                                    false);
                if(res) {
                    res->set_operation(M2MBase::POST_ALLOWED);
                }
            }
        }
    }
    return res;
}

bool M2MServer::delete_resource(ServerResource resource)
{
    bool success = false;
    const char* server_id_ptr;
    switch(resource) {
        case DefaultMinPeriod:
           server_id_ptr = SERVER_DEFAULT_MIN_PERIOD;
           break;
        case DefaultMaxPeriod:
            server_id_ptr = SERVER_DEFAULT_MAX_PERIOD;
            break;
        case Disable:
            server_id_ptr = SERVER_DISABLE;
            break;
        case DisableTimeout:
            server_id_ptr = SERVER_DISABLE_TIMEOUT;
            break;
        default:
            server_id_ptr = "";
            break;
    }
    String server_id(server_id_ptr);
    
    if(!server_id.empty()) {
        if(_server_instance) {
            success = _server_instance->remove_resource(server_id);
        }
    }
    return success;
}

bool M2MServer::set_resource_value(ServerResource resource,
                                   const String &value)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res && (M2MServer::Binding == resource)) {
        success = res->set_value((const uint8_t*)value.c_str(),(uint32_t)value.length());
    }
    return success;
}

bool M2MServer::set_resource_value(ServerResource resource,
                                   uint32_t value)
{
    bool success = false;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MServer::ShortServerID == resource     ||
           M2MServer::Lifetime == resource          ||
           M2MServer::DefaultMinPeriod == resource  ||
           M2MServer::DefaultMaxPeriod == resource  ||
           M2MServer::DisableTimeout == resource    ||
           M2MServer::NotificationStorage == resource) {
            // If it is any of the above resource
            // set the value of the resource.

            success = res->set_value(value);
        }
    }
    return success;
}

String M2MServer::resource_value_string(ServerResource resource) const
{
    String value = "";
    M2MResource* res = get_resource(resource);
    if(res && (M2MServer::Binding == resource)) {

        value = res->get_value_string();
    }
    return value;
}


uint32_t M2MServer::resource_value_int(ServerResource resource) const
{
    uint32_t value = 0;
    M2MResource* res = get_resource(resource);
    if(res) {
        if(M2MServer::ShortServerID == resource     ||
           M2MServer::Lifetime == resource          ||
           M2MServer::DefaultMinPeriod == resource  ||
           M2MServer::DefaultMaxPeriod == resource  ||
           M2MServer::DisableTimeout == resource    ||
           M2MServer::NotificationStorage == resource) {

            value = res->get_value_int();
        }
    }
    return value;
}

bool M2MServer::is_resource_present(ServerResource resource) const
{
    bool success = false;
    M2MResource *res = get_resource(resource);
    if(res) {
        success = true;
    }
    return success;
}

uint16_t M2MServer::total_resource_count() const
{
    uint16_t total_count = 0;
    if(_server_instance) {
    total_count = _server_instance->resources().size();
    }
    return total_count;
}

M2MResource* M2MServer::get_resource(ServerResource res) const
{
    M2MResource* res_object = NULL;
    const char* res_name_ptr = "";
    switch(res) {
    case ShortServerID:
        res_name_ptr = SERVER_SHORT_SERVER_ID;
        break;
    case Lifetime:
        res_name_ptr = SERVER_LIFETIME;
        break;
    case DefaultMinPeriod:
        res_name_ptr = SERVER_DEFAULT_MIN_PERIOD;
        break;
    case DefaultMaxPeriod:
        res_name_ptr = SERVER_DEFAULT_MAX_PERIOD;
        break;
    case Disable:
        res_name_ptr = SERVER_DISABLE;
        break;
    case DisableTimeout:
        res_name_ptr = SERVER_DISABLE_TIMEOUT;
        break;
    case NotificationStorage:
        res_name_ptr = SERVER_NOTIFICATION_STORAGE;
        break;
    case Binding:
        res_name_ptr = SERVER_BINDING;
        break;
    case RegistrationUpdate:
        res_name_ptr = SERVER_REGISTRATION_UPDATE;
        break;
    }

    const String res_name(res_name_ptr);

    if(!res_name.empty()) {
        if(_server_instance) {
        res_object = _server_instance->resource(res_name);
        }
    }
    return res_object;
}
