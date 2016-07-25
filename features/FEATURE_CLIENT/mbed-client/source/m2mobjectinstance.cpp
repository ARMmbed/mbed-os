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
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mobservationhandler.h"
#include "mbed-client/m2mstring.h"
#include "include/m2mtlvserializer.h"
#include "include/m2mtlvdeserializer.h"
#include "include/nsdllinker.h"
#include "include/m2mreporthandler.h"
#include "mbed-trace/mbed_trace.h"

#define BUFFER_SIZE 10
#define TRACE_GROUP "mClt"

M2MObjectInstance& M2MObjectInstance::operator=(const M2MObjectInstance& other)
{
    if (this != &other) { // protect against invalid self-assignment
        if(!other._resource_list.empty()){
            M2MResource* ins = NULL;
            M2MResourceList::const_iterator it;
            it = other._resource_list.begin();
            for (; it!=other._resource_list.end(); it++ ) {
                ins = *it;
                _resource_list.push_back(new M2MResource(*ins));
            }
        }
    }
    return *this;
}

M2MObjectInstance::M2MObjectInstance(const M2MObjectInstance& other)
: M2MBase(other),
  _object_callback(other._object_callback)
{
    this->operator=(other);
}

M2MObjectInstance::M2MObjectInstance(const String &object_name,
                                     M2MObjectCallback &object_callback)
: M2MBase(object_name,M2MBase::Dynamic),
  _object_callback(object_callback)
{
    M2MBase::set_base_type(M2MBase::ObjectInstance);
    M2MBase::set_coap_content_type(COAP_CONTENT_OMA_TLV_TYPE);
}

M2MObjectInstance::~M2MObjectInstance()
{
    if(!_resource_list.empty()) {
        M2MResource* res = NULL;
        M2MResourceList::const_iterator it;
        it = _resource_list.begin();
        for (; it!=_resource_list.end(); it++ ) {
            //Free allocated memory for resources.
            res = *it;
            String obj_name = name();
            obj_name.push_back('/');
            obj_name.append_int(instance_id());
            obj_name.push_back('/');
            obj_name += (*it)->name();
            (*it)->remove_resource_from_coap(obj_name);
            delete res;
        }
        _resource_list.clear();
    }
}

M2MResource* M2MObjectInstance::create_static_resource(const String &resource_name,
                                                       const String &resource_type,
                                                       M2MResourceInstance::ResourceType type,
                                                       const uint8_t *value,
                                                       const uint8_t value_length,
                                                       bool multiple_instance)
{
    tr_debug("M2MObjectInstance::create_static_resource(resource_name %s)",resource_name.c_str());
    M2MResource *res = NULL;
    if( resource_name.empty() || resource_name.size() > MAX_ALLOWED_STRING_LENGTH){
        return res;
    }
    if(!resource(resource_name)) {
        res = new M2MResource(*this,resource_name, resource_type, type,
                              value, value_length, M2MBase::instance_id(),
                              M2MBase::name(), multiple_instance);
        if(res) {
            res->add_observation_level(observation_level());
            if (multiple_instance) {
                res->set_coap_content_type(COAP_CONTENT_OMA_TLV_TYPE);
            }
            _resource_list.push_back(res);
        }
    }
    return res;
}

M2MResource* M2MObjectInstance::create_dynamic_resource(const String &resource_name,
                                                const String &resource_type,
                                                M2MResourceInstance::ResourceType type,
                                                bool observable,
                                                bool multiple_instance)
{
    tr_debug("M2MObjectInstance::create_dynamic_resource(resource_name %s)",resource_name.c_str());
    M2MResource *res = NULL;
    if( resource_name.empty() || resource_name.size() > MAX_ALLOWED_STRING_LENGTH){
        return res;
    }
    if(!resource(resource_name)) {
        res = new M2MResource(*this,resource_name, resource_type, type,
                              observable, M2MBase::instance_id(),
                              M2MBase::name(), multiple_instance);
        if(res) {
            if (multiple_instance) {
                res->set_coap_content_type(COAP_CONTENT_OMA_TLV_TYPE);
            }
            res->add_observation_level(observation_level());
            _resource_list.push_back(res);
        }
    }
    return res;
}

M2MResourceInstance* M2MObjectInstance::create_static_resource_instance(const String &resource_name,
                                                                        const String &resource_type,
                                                                        M2MResourceInstance::ResourceType type,
                                                                        const uint8_t *value,
                                                                        const uint8_t value_length,
                                                                        uint16_t instance_id)
{
    tr_debug("M2MObjectInstance::create_static_resource_instance(resource_name %s)",resource_name.c_str());
    M2MResourceInstance *instance = NULL;
    if(resource_name.empty() || resource_name.size() > MAX_ALLOWED_STRING_LENGTH){
        return instance;
    }
    M2MResource *res = resource(resource_name);
    if(!res) {        
        res = new M2MResource(*this,resource_name, resource_type, type,
                              value, value_length, M2MBase::instance_id(),
                              M2MBase::name(), true);
        _resource_list.push_back(res);
        res->set_operation(M2MBase::GET_ALLOWED);
        res->set_observable(false);
        res->set_register_uri(false);
    }
    if(res->supports_multiple_instances()&& (res->resource_instance(instance_id) == NULL)) {
        instance = new M2MResourceInstance(resource_name, resource_type, type,
                                           value, value_length, *this,
                                           M2MBase::instance_id(), M2MBase::name());
        if(instance) {
            instance->set_operation(M2MBase::GET_ALLOWED);
            instance->set_instance_id(instance_id);
            res->add_resource_instance(instance);
        }
    }
    return instance;
}

M2MResourceInstance* M2MObjectInstance::create_dynamic_resource_instance(const String &resource_name,
                                                                         const String &resource_type,
                                                                         M2MResourceInstance::ResourceType type,
                                                                         bool observable,
                                                                         uint16_t instance_id)
{
    tr_debug("M2MObjectInstance::create_dynamic_resource_instance(resource_name %s)",resource_name.c_str());
    M2MResourceInstance *instance = NULL;
    if(resource_name.empty() || resource_name.size() > MAX_ALLOWED_STRING_LENGTH){
        return instance;
    }
    M2MResource *res = resource(resource_name);
    if(!res) {
        res = new M2MResource(*this,resource_name, resource_type, type,
                              false, M2MBase::instance_id(), M2MBase::name(), true);
        _resource_list.push_back(res);
        res->set_register_uri(false);
        res->set_operation(M2MBase::GET_ALLOWED);        
    }
    if(res->supports_multiple_instances() && (res->resource_instance(instance_id) == NULL)) {
        instance = new M2MResourceInstance(resource_name, resource_type, type, *this,
                                           M2MBase::instance_id(), M2MBase::name());
        if(instance) {
            instance->set_operation(M2MBase::GET_ALLOWED);
            instance->set_observable(observable);
            instance->set_instance_id(instance_id);
            res->add_resource_instance(instance);
        }
    }
    return instance;
}

bool M2MObjectInstance::remove_resource(const String &resource_name)
{
    tr_debug("M2MObjectInstance::remove_resource(resource_name %s)",
             resource_name.c_str());
    bool success = false;
    if(!_resource_list.empty()) {
         M2MResource* res = NULL;
         M2MResourceList::const_iterator it;
         it = _resource_list.begin();
         int pos = 0;
         for ( ; it != _resource_list.end(); it++, pos++ ) {
             if(((*it)->name() == resource_name)) {
                // Resource found and deleted.
                res = *it;

                String obj_name = name();
                obj_name.push_back('/');
                obj_name.append_int(instance_id());
                obj_name.push_back('/');
                obj_name += res->name();
                res->remove_resource_from_coap(obj_name);
                delete res;
                res = NULL;
                _resource_list.erase(pos);
                success = true;
                break;
             }
         }
     }
    return success;
}

bool M2MObjectInstance::remove_resource_instance(const String &resource_name,
                                                 uint16_t inst_id)
{
    tr_debug("M2MObjectInstance::remove_resource_instance(resource_name %s inst_id %d)",
             resource_name.c_str(), inst_id);
    bool success = false;
    M2MResource *res = resource(resource_name);
    if(res) {
        M2MResourceInstanceList list = res->resource_instances();
        M2MResourceInstanceList::const_iterator it;
        it = list.begin();
        for ( ; it != list.end(); it++) {
            if((*it)->instance_id() == inst_id) {
                String obj_name = name();
                obj_name.push_back('/');
                obj_name.append_int(instance_id());
                obj_name.push_back('/');
                obj_name += resource_name;
                obj_name.push_back('/');
                obj_name.append_int(inst_id);
                remove_resource_from_coap(obj_name);
                success = res->remove_resource_instance(inst_id);
                if(res->resource_instance_count() == 0) {
                    M2MResourceList::const_iterator itr;
                    itr = _resource_list.begin();
                    int pos = 0;
                    for ( ; itr != _resource_list.end(); itr++, pos++ ) {
                        if(((*itr)->name() == resource_name)) {
                            delete res;
                            res = NULL;
                            _resource_list.erase(pos);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    return success;
}

M2MResource* M2MObjectInstance::resource(const String &resource) const
{
    M2MResource *res = NULL;
    if(!_resource_list.empty()) {
        M2MResourceList::const_iterator it;
        it = _resource_list.begin();
        for (; it!=_resource_list.end(); it++ ) {
            if((*it)->name() == resource) {
                res = *it;
                break;
            }
        }
    }
    return res;
}

const M2MResourceList& M2MObjectInstance::resources() const
{
    return _resource_list;
}

uint16_t M2MObjectInstance::resource_count() const
{
    uint16_t count = 0;
    if(!_resource_list.empty()) {
        M2MResourceList::const_iterator it;
        it = _resource_list.begin();
        for ( ; it != _resource_list.end(); it++ ) {
            if((*it)->supports_multiple_instances()) {
                count += (*it)->resource_instance_count();
            } else {
                count++;
            }
        }
    }
    return count;
}

uint16_t M2MObjectInstance::resource_count(const String& resource) const
{
    uint16_t count = 0;
    if(!_resource_list.empty()) {
        M2MResourceList::const_iterator it;
        it = _resource_list.begin();
        for ( ; it != _resource_list.end(); it++ ) {
            if((*it)->name() == resource) {
                if((*it)->supports_multiple_instances()) {
                    count += (*it)->resource_instance_count();
                } else {
                    count++;
                }
            }
        }
    }
    return count;
}

M2MBase::BaseType M2MObjectInstance::base_type() const
{
    return M2MBase::base_type();
}

void M2MObjectInstance::add_observation_level(M2MBase::Observation observation_level)
{
    M2MBase::add_observation_level(observation_level);
    if(!_resource_list.empty()) {
        M2MResourceList::const_iterator it;
        it = _resource_list.begin();
        for ( ; it != _resource_list.end(); it++ ) {
            (*it)->add_observation_level(observation_level);
        }
    }
}

void M2MObjectInstance::remove_observation_level(M2MBase::Observation observation_level)
{
    M2MBase::remove_observation_level(observation_level);
    if(!_resource_list.empty()) {
        M2MResourceList::const_iterator it;
        it = _resource_list.begin();
        for ( ; it != _resource_list.end(); it++ ) {
           (*it)->remove_observation_level(observation_level);
        }
    }
}

sn_coap_hdr_s* M2MObjectInstance::handle_get_request(nsdl_s *nsdl,
                                                     sn_coap_hdr_s *received_coap_header,
                                                     M2MObservationHandler *observation_handler)
{
    tr_debug("M2MObjectInstance::handle_get_request()");
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;
    sn_coap_hdr_s * coap_response = sn_nsdl_build_response(nsdl,
                                                           received_coap_header,
                                                           msg_code);
    uint8_t * data = NULL;
    uint32_t  data_length = 0;    
    if(received_coap_header) {
        // process the GET if we have registered a callback for it
        if ((operation() & SN_GRS_GET_ALLOWED) != 0) {
            if(coap_response) {
                uint16_t coap_content_type = 0;
                bool content_type_present = false;
                if(received_coap_header->content_type_ptr) {
                    content_type_present = true;
                    coap_response->content_type_ptr = alloc_copy(received_coap_header->content_type_ptr,
                                                                  received_coap_header->content_type_len);
                    if(coap_response->content_type_ptr) {
                        coap_response->content_type_len = received_coap_header->content_type_len;
                        for(uint8_t i = 0; i < coap_response->content_type_len; i++) {
                            coap_content_type = (coap_content_type << 8) +
                                    (coap_response->content_type_ptr[i] & 0xFF);
                        }
                    }
                }
                if(!content_type_present &&
                   M2MBase::coap_content_type() == COAP_CONTENT_OMA_TLV_TYPE) {
                    coap_content_type = COAP_CONTENT_OMA_TLV_TYPE;
                }

                tr_debug("M2MObjectInstance::handle_get_request() - Request Content-Type %d", coap_content_type);
                if (!coap_response->content_type_ptr) {
                    coap_response->content_type_ptr =
                            m2m::String::convert_integer_to_array(coap_content_type,
                                                                  coap_response->content_type_len);
                    if (coap_response->content_type_ptr) {
                        set_coap_content_type(coap_content_type);
                    }
                }
                // fill in the CoAP response payload
                if(COAP_CONTENT_OMA_TLV_TYPE == coap_content_type) {
                    M2MTLVSerializer serializer;
                    data = serializer.serialize(_resource_list, data_length);
                } else {
                    msg_code = COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT; // Content format not supported
                }

                coap_response->payload_len = data_length;
                coap_response->payload_ptr = data;

                if(data) {
                    coap_response->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
                    memset(coap_response->options_list_ptr, 0, sizeof(sn_coap_options_list_s));

                    coap_response->options_list_ptr->max_age_ptr =
                            m2m::String::convert_integer_to_array(max_age(),
                                coap_response->options_list_ptr->max_age_len);

                    if(received_coap_header->options_list_ptr) {
                        if(received_coap_header->options_list_ptr->observe) {
                            if (is_observable()) {
                                uint32_t number = 0;
                                uint8_t observe_option = 0;
                                if(received_coap_header->options_list_ptr->observe_ptr) {
                                    observe_option = *received_coap_header->options_list_ptr->observe_ptr;
                                }
                                if(START_OBSERVATION == observe_option) {
                                    tr_debug("M2MObjectInstance::handle_get_request - Starts Observation");
                                    // If the observe length is 0 means register for observation.
                                    if(received_coap_header->options_list_ptr->observe_len != 0) {
                                        for(int i=0;i < received_coap_header->options_list_ptr->observe_len; i++) {
                                            number = (*(received_coap_header->options_list_ptr->observe_ptr + i) & 0xff) <<
                                                     8*(received_coap_header->options_list_ptr->observe_len- 1 - i);
                                        }
                                    }
                                    if(received_coap_header->token_ptr) {
                                        tr_debug("M2MObjectInstance::handle_get_request - Sets Observation Token to resource");
                                        set_observation_token(received_coap_header->token_ptr,
                                                              received_coap_header->token_len);
                                    }

                                    // If the observe value is 0 means register for observation.
                                    if(number == 0) {
                                        tr_debug("M2MObjectInstance::handle_get_request - Put Resource under Observation");
                                        set_under_observation(true,observation_handler);
                                        add_observation_level(M2MBase::OI_Attribute);
                                        coap_response->options_list_ptr->observe_ptr =
                                                m2m::String::convert_integer_to_array(observation_number(),
                                                      coap_response->options_list_ptr->observe_len);
                                    }
                                } else if (STOP_OBSERVATION == observe_option) {
                                    tr_debug("M2MObjectInstance::handle_get_request - Stops Observation");
                                    set_under_observation(false,NULL);
                                    remove_observation_level(M2MBase::OI_Attribute);

                                }
                                msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;
                            }
                            else {
                                msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                            }
                        }
                    }
                } else {
                    msg_code = COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT; // Content format not supported
                }
            }
        }else {
            tr_error("M2MObjectInstance::handle_get_request - Return COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED");
            // Operation is not allowed.
            msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
        }        
    } else {
        msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
    }
    if(coap_response) {
        coap_response->msg_code = msg_code;
    }
    return coap_response;
}

sn_coap_hdr_s* M2MObjectInstance::handle_put_request(nsdl_s *nsdl,
                                                     sn_coap_hdr_s *received_coap_header,
                                                     M2MObservationHandler *observation_handler,
                                                     bool &/*execute_value_updated*/)
{
    tr_debug("M2MObjectInstance::handle_put_request()");
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_RESPONSE_CHANGED; // 2.04
    sn_coap_hdr_s * coap_response = sn_nsdl_build_response(nsdl,
                                                           received_coap_header,
                                                           msg_code);;
    if(received_coap_header) {
        uint16_t coap_content_type = 0;
        bool content_type_present = false;
        if(received_coap_header->content_type_ptr) {
            content_type_present = true;
            if(coap_response) {
                coap_response->content_type_ptr = alloc_copy(received_coap_header->content_type_ptr,
                                                              received_coap_header->content_type_len);
                if(coap_response->content_type_ptr) {
                    coap_response->content_type_len = received_coap_header->content_type_len;
                    for(uint8_t i = 0; i < coap_response->content_type_len; i++) {
                        coap_content_type = (coap_content_type << 8) +
                                (coap_response->content_type_ptr[i] & 0xFF);
                    }
                }
            }
        }
        if(received_coap_header->options_list_ptr &&
           received_coap_header->options_list_ptr->uri_query_ptr) {
            char *query = (char*)alloc_string_copy(received_coap_header->options_list_ptr->uri_query_ptr,
                                                    received_coap_header->options_list_ptr->uri_query_len);
            if (query){
                tr_debug("M2MObjectInstance::handle_put_request() - Query %s", query);
                // if anything was updated, re-initialize the stored notification attributes
                if (!handle_observation_attribute(query)){
                    tr_debug("M2MObjectInstance::handle_put_request() - Invalid query");
                    msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST; // 4.00
                } else {
                    msg_code =COAP_MSG_CODE_RESPONSE_CHANGED;
                }
                free(query);
            }
        } else if ((operation() & SN_GRS_PUT_ALLOWED) != 0) {
            if(!content_type_present &&
               M2MBase::coap_content_type() == COAP_CONTENT_OMA_TLV_TYPE) {
                coap_content_type = COAP_CONTENT_OMA_TLV_TYPE;
            }

            tr_debug("M2MObjectInstance::handle_put_request() - Request Content-Type %d", coap_content_type);

            if(COAP_CONTENT_OMA_TLV_TYPE == coap_content_type) {
                M2MTLVDeserializer::Error error = M2MTLVDeserializer::None;
                M2MTLVDeserializer *deserializer = new M2MTLVDeserializer();
                if(deserializer && received_coap_header->payload_ptr) {
                    error = deserializer->deserialize_resources(received_coap_header->payload_ptr,
                                                                received_coap_header->payload_len,
                                                                *this,
                                                                M2MTLVDeserializer::Put);
                    switch(error) {
                        case M2MTLVDeserializer::None:
                            if(observation_handler) {
                                observation_handler->value_updated(this);
                            }
                            msg_code = COAP_MSG_CODE_RESPONSE_CHANGED;
                            break;
                        case M2MTLVDeserializer::NotFound:
                            msg_code = COAP_MSG_CODE_RESPONSE_NOT_FOUND;
                            break;
                        case M2MTLVDeserializer::NotAllowed:
                            msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                            break;
                        case M2MTLVDeserializer::NotValid:
                            msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
                            break;
                    }
                }
                delete deserializer;
            } else {
                msg_code =COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT;
            } // if(COAP_CONTENT_OMA_TLV_TYPE == coap_content_type)
        } else {
            // Operation is not allowed.
            tr_error("M2MObjectInstance::handle_put_request() - COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED");
            msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
        }
    } else {
       msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
    }
    if(coap_response) {
        coap_response->msg_code = msg_code;
    }
    return coap_response;
}

sn_coap_hdr_s* M2MObjectInstance::handle_post_request(nsdl_s *nsdl,
                                                      sn_coap_hdr_s *received_coap_header,
                                                      M2MObservationHandler *observation_handler,
                                                      bool &execute_value_updated)
{
    tr_debug("M2MObjectInstance::handle_post_request()");
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_RESPONSE_CHANGED; // 2.04
    sn_coap_hdr_s * coap_response = sn_nsdl_build_response(nsdl,
                                                           received_coap_header,
                                                           msg_code);
    if(received_coap_header) {
        if ((operation() & SN_GRS_POST_ALLOWED) != 0) {
            uint16_t coap_content_type = 0;
            bool content_type_present = false;
            if(received_coap_header->content_type_ptr) {
                content_type_present = true;
                if(coap_response) {
                    coap_response->content_type_ptr = alloc_copy(received_coap_header->content_type_ptr,
                                                                  received_coap_header->content_type_len);
                    if(coap_response->content_type_ptr) {
                        coap_response->content_type_len = received_coap_header->content_type_len;
                        for(uint8_t i = 0; i < coap_response->content_type_len; i++) {
                            coap_content_type = (coap_content_type << 8) +
                                    (coap_response->content_type_ptr[i] & 0xFF);
                        }
                    }
                }
            }
            if(!content_type_present &&
               M2MBase::coap_content_type() == COAP_CONTENT_OMA_TLV_TYPE) {
                coap_content_type = COAP_CONTENT_OMA_TLV_TYPE;
            }

            tr_debug("M2MObjectInstance::handle_post_request() - Request Content-Type %d", coap_content_type);

            if(COAP_CONTENT_OMA_TLV_TYPE == coap_content_type) {
                M2MTLVDeserializer *deserializer = new M2MTLVDeserializer();
                if(deserializer) {
                    String obj_name = "";
                    M2MTLVDeserializer::Error error = M2MTLVDeserializer::None;
                    error = deserializer->deserialize_resources(received_coap_header->payload_ptr,
                                                                received_coap_header->payload_len,
                                                                *this,
                                                                M2MTLVDeserializer::Post);

                    uint16_t instance_id = deserializer->instance_id(received_coap_header->payload_ptr);
                    switch(error) {
                        case M2MTLVDeserializer::None:
                            if(observation_handler) {
                                execute_value_updated = true;
                            }                            
                            coap_response->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
                            if (coap_response->options_list_ptr) {
                                memset(coap_response->options_list_ptr, 0, sizeof(sn_coap_options_list_s));

                                obj_name += M2MBase::name();
                                obj_name += "/";
                                obj_name.append_int(M2MBase::instance_id());
                                obj_name += "/";
                                obj_name.append_int(instance_id);

                                coap_response->options_list_ptr->location_path_len = obj_name.length();
                                if (coap_response->options_list_ptr->location_path_len != 0) {
                                    coap_response->options_list_ptr->location_path_ptr =
                                        alloc_string_copy((uint8_t*)obj_name.c_str(),
                                               coap_response->options_list_ptr->location_path_len);
                                }
                            }
                            msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
                            break;
                        case M2MTLVDeserializer::NotAllowed:
                            msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                            break;
                        case M2MTLVDeserializer::NotValid:
                            msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
                            break;
                        default:
                            break;
                    }
                    delete deserializer;
                }
            } else {
                msg_code =COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT;
            } // if(COAP_CONTENT_OMA_TLV_TYPE == coap_content_type)
        } else {
            // Operation is not allowed.
            tr_error("M2MObjectInstance::handle_post_request() - COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED");
            msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
        }
    } else {
        msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
    }
    if(coap_response) {
        coap_response->msg_code = msg_code;
    }
    return coap_response;
}

void M2MObjectInstance::notification_update(M2MBase::Observation observation_level)
{
    tr_debug("M2MObjectInstance::notification_update() - level(%d)", observation_level);
    if(M2MBase::O_Attribute == observation_level) {
        _object_callback.notification_update(instance_id());
    } else {
        M2MReportHandler *report_handler = M2MBase::report_handler();
        if(report_handler && is_under_observation()) {
            report_handler->set_notification_trigger();
        }
    }
}
