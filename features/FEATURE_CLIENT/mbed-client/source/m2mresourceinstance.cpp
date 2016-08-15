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
#include <stdlib.h>
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mobservationhandler.h"
#include "mbed-client/m2mobjectinstance.h"
#include "include/m2mreporthandler.h"
#include "include/nsdllinker.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "mClt"

M2MResourceInstance& M2MResourceInstance::operator=(const M2MResourceInstance& other)
{
    if (this != &other) { // protect against invalid self-assignment

        free(_value);
        _value = NULL;

        _value_length = other._value_length;
        if(other._value) {
            _value = (uint8_t *)alloc_string_copy(other._value, other._value_length);
        }
    }
    return *this;
}

M2MResourceInstance::M2MResourceInstance(const M2MResourceInstance& other)
: M2MBase(other),
  _object_instance_callback(other._object_instance_callback),
  _execute_callback(NULL),
  _value(NULL),
  _value_length(0),
  _resource_callback(NULL),
  _object_name(other._object_name),
  _function_pointer(NULL),
  _object_instance_id(other._object_instance_id),
  _resource_type(M2MResourceInstance::STRING)
{
    this->operator=(other);
}

M2MResourceInstance::M2MResourceInstance(const String &res_name,
                                         const String &resource_type,
                                         M2MResourceInstance::ResourceType type,
                                         M2MObjectInstanceCallback &object_instance_callback,
                                         const uint16_t object_instance_id,
                                         const String &object_name)
: M2MBase(res_name,
          M2MBase::Dynamic),
 _object_instance_callback(object_instance_callback),
 _execute_callback(NULL),
 _value(NULL),
 _value_length(0),
 _resource_callback(NULL),
 _object_name(object_name),
 _function_pointer(NULL),
 _object_instance_id(object_instance_id),
 _resource_type(type)
{
    M2MBase::set_resource_type(resource_type);
    M2MBase::set_base_type(M2MBase::ResourceInstance);
}

M2MResourceInstance::M2MResourceInstance(const String &res_name,
                                         const String &resource_type,
                                         M2MResourceInstance::ResourceType type,
                                         const uint8_t *value,
                                         const uint8_t value_length,
                                         M2MObjectInstanceCallback &object_instance_callback,
                                         const uint16_t object_instance_id,
                                         const String &object_name)
: M2MBase(res_name,
          M2MBase::Static),
 _object_instance_callback(object_instance_callback),
 _execute_callback(NULL),
 _value(NULL),
 _value_length(0),
 _resource_callback(NULL),
 _object_name(object_name),
 _function_pointer(NULL),
 _object_instance_id(object_instance_id),
 _resource_type(type)
{
    M2MBase::set_resource_type(resource_type);
    M2MBase::set_base_type(M2MBase::Resource);
    if( value != NULL && value_length > 0 ) {
        _value = alloc_string_copy(value, value_length);
        if(_value) {
            _value_length = value_length;
        }
    }
}

M2MResourceInstance::~M2MResourceInstance()
{
    free(_value);
    delete _function_pointer;
}

M2MBase::BaseType M2MResourceInstance::base_type() const
{
    return M2MBase::base_type();
}

M2MResourceInstance::ResourceType M2MResourceInstance::resource_instance_type() const
{
    return _resource_type;
}

bool M2MResourceInstance::handle_observation_attribute(char *&query)
{
    tr_debug("M2MResourceInstance::handle_observation_attribute - is_under_observation(%d)", is_under_observation());
    bool success = false;
    M2MReportHandler *handler = M2MBase::report_handler();
    if (handler) {
        success = handler->parse_notification_attribute(query,
                M2MBase::base_type(), _resource_type);
        if(success) {
            if (is_under_observation()) {
                handler->set_under_observation(true);
            }
        } else {
            handler->set_default_values();
        }
    }
    return success;
}

void M2MResourceInstance::set_execute_function(execute_callback callback)
{
    _execute_callback = callback;
}

void M2MResourceInstance::set_execute_function(execute_callback_2 callback)
{
    delete _function_pointer;

    _function_pointer = new FP1<void, void*>(callback);
    set_execute_function(execute_callback(_function_pointer, &FP1<void, void*>::call));
}

void M2MResourceInstance::clear_value()
{
    tr_debug("M2MResourceInstance::clear_value");

     free(_value);
     _value = NULL;
     _value_length = 0;

    report();
}

bool M2MResourceInstance::set_value(int64_t value)
{
    bool success;
    // max len of "-9223372036854775808" plus zero termination
    char buffer[20+1];
    uint32_t size = m2m::itoa_c(value, buffer);

    success = set_value((const uint8_t*)buffer, size);

    return success;
}

bool M2MResourceInstance::set_value(const uint8_t *value,
                                    const uint32_t value_length)
{
    tr_debug("M2MResourceInstance::set_value()");
    bool success = false;
    bool value_changed = false;
    if(is_value_changed(value,value_length)) {
        value_changed = true;
    }
    if( value != NULL && value_length > 0 ) {
        success = true;

        free(_value);
        _value_length = 0;

        _value = alloc_string_copy(value, value_length);
        if(_value) {
            _value_length = value_length;
            if( value_changed ) { //
                if (_resource_type == M2MResourceInstance::STRING) {
                    M2MReportHandler *report_handler = M2MBase::report_handler();
                    if(report_handler && is_under_observation()) {
                        report_handler->set_notification_trigger();
                    }
                }
                else {
                    report();
                }
            }
        }
    }
    return success;
}

void M2MResourceInstance::report()
{
    tr_debug("M2MResourceInstance::report()");
    M2MBase::Observation  observation_level = M2MBase::observation_level();
    if(M2MBase::R_Attribute != observation_level) {
        tr_debug("M2MResourceInstance::report() -- object level");
        _object_instance_callback.notification_update(observation_level);
    }
    if(M2MBase::Dynamic == mode() && M2MBase::R_Attribute == observation_level) {
        tr_debug("M2MResourceInstance::report() - resource level");
        if(!_resource_callback && _resource_type != M2MResourceInstance::STRING) {
            M2MReportHandler *report_handler = M2MBase::report_handler();
            if (report_handler && is_observable()) {
                if(_value) {
                    report_handler->set_value(atof((const char*)_value));
                } else {
                    report_handler->set_value(0);
                }
            }
        }
        else {
            if (_resource_callback && base_type() == M2MBase::ResourceInstance) {
                _resource_callback->notification_update();
            }
        }
    } else if(M2MBase::Static == mode()) {
        M2MObservationHandler *observation_handler = M2MBase::observation_handler();
        if(observation_handler) {
            observation_handler->value_updated(this);
        }
    } else {
        tr_debug("M2MResourceInstance::report() - mode = %d, is_observable = %d", mode(), is_observable());
    }
}

bool M2MResourceInstance::is_value_changed(const uint8_t* value, const uint32_t value_len)
{
    bool changed = false;
    if(value_len != _value_length) {
        changed = true;
    } else if(value && !_value) {
        changed = true;
    } else if(_value && !value) {
        changed = true;
    } else {        
        if (_value) {
            if (strcmp((char*)value, (char*)_value) != 0) {
                changed = true;
            }
        }
    }
    tr_debug("M2MResourceInstance::is_value_changed() -- %s", changed ? "true" : "false");
    return changed;
}

void M2MResourceInstance::execute(void *arguments)
{
    tr_debug("M2MResourceInstance::execute");
    if(_execute_callback) {
        _execute_callback(arguments);
    }
}

void M2MResourceInstance::get_value(uint8_t *&value, uint32_t &value_length)
{
    value_length = 0;
    if(value) {
        free(value);
        value = NULL;
    }
    if(_value && _value_length > 0) {
        value = alloc_string_copy(_value, _value_length);
        if(value) {
            value_length = _value_length;
        }
    }
}

int M2MResourceInstance::get_value_int()
{
    int value_int = 0;
    // Get the value and convert it into integer. This is not the most
    // efficient way, as it takes pointless heap copy to get the zero termination.
    uint8_t* buffer = NULL;
    uint32_t length;
    get_value(buffer,length);
    if(buffer) {
        value_int = atoi((const char*)buffer);
        free(buffer);
    }
    return value_int;
}

String M2MResourceInstance::get_value_string() const
{
    // XXX: do a better constructor to avoid pointless malloc
    String value;
    if (_value) {
        value.append_raw((char*)_value, _value_length);
    }
    
    return value;
}

uint8_t* M2MResourceInstance::value() const
{
    return _value;
}

uint32_t M2MResourceInstance::value_length() const
{
    return _value_length;
}

sn_coap_hdr_s* M2MResourceInstance::handle_get_request(nsdl_s *nsdl,
                                               sn_coap_hdr_s *received_coap_header,
                                               M2MObservationHandler *observation_handler)
{
    tr_debug("M2MResourceInstance::handle_get_request()");
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;
    sn_coap_hdr_s *coap_response = sn_nsdl_build_response(nsdl,
                                                          received_coap_header,
                                                          msg_code);
    if(received_coap_header) {
        // process the GET if we have registered a callback for it
        if ((operation() & SN_GRS_GET_ALLOWED) != 0) {
            if(coap_response) {
                if(_resource_type == M2MResourceInstance::OPAQUE) {
                    coap_response->content_type_ptr =
                            m2m::String::convert_integer_to_array(COAP_CONTENT_OMA_OPAQUE_TYPE,
                                coap_response->content_type_len);
                } else {
                    coap_response->content_type_ptr =
                            m2m::String::convert_integer_to_array(0,
                                coap_response->content_type_len);
                }

                // fill in the CoAP response payload
                coap_response->payload_ptr = NULL;
                uint32_t payload_len = 0;
                get_value(coap_response->payload_ptr,payload_len);
                coap_response->payload_len = payload_len;

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
                                tr_debug("M2MResourceInstance::handle_get_request - Starts Observation");
                                // If the observe length is 0 means register for observation.
                                if(received_coap_header->options_list_ptr->observe_len != 0) {
                                    for(int i=0;i < received_coap_header->options_list_ptr->observe_len; i++) {
                                        number = (*(received_coap_header->options_list_ptr->observe_ptr + i) & 0xff) <<
                                                 8*(received_coap_header->options_list_ptr->observe_len- 1 - i);
                                        }
                                }
                                if(received_coap_header->token_ptr) {
                                    tr_debug("M2MResourceInstance::handle_get_request - Sets Observation Token to resource");
                                    set_observation_token(received_coap_header->token_ptr,
                                                          received_coap_header->token_len);
                                }
                                // If the observe value is 0 means register for observation.
                                if(number == 0) {
                                    tr_debug("M2MResourceInstance::handle_get_request - Put Resource under Observation");
                                    set_under_observation(true,observation_handler);
                                    M2MBase::add_observation_level(M2MBase::R_Attribute);
                                    coap_response->options_list_ptr->observe_ptr =
                                            m2m::String::convert_integer_to_array(observation_number(),
                                                  coap_response->options_list_ptr->observe_len);
                                }
                            } else if (STOP_OBSERVATION == observe_option) {
                                tr_debug("M2MResourceInstance::handle_get_request - Stops Observation");
                                set_under_observation(false,NULL);
                                M2MBase::remove_observation_level(M2MBase::R_Attribute);
                            }
                        } else {
                            msg_code = COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED;
                        }
                    }
                }
            }
        }else {
            tr_error("M2MResourceInstance::handle_get_request - Return COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED");
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

sn_coap_hdr_s* M2MResourceInstance::handle_put_request(nsdl_s *nsdl,
                                               sn_coap_hdr_s *received_coap_header,
                                               M2MObservationHandler *observation_handler,
                                               bool &execute_value_updated)
{
    tr_debug("M2MResourceInstance::handle_put_request()");
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_RESPONSE_CHANGED; // 2.04
    sn_coap_hdr_s * coap_response = sn_nsdl_build_response(nsdl,
                                                           received_coap_header,
                                                           msg_code);
    // process the PUT if we have registered a callback for it
    if(received_coap_header) {
        uint16_t coap_content_type = 0;
        if(received_coap_header->content_type_ptr) {
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
                tr_debug("M2MResourceInstance::handle_put_request() - Query %s", query);

                // if anything was updated, re-initialize the stored notification attributes
                if (!handle_observation_attribute(query)){
                    tr_debug("M2MResourceInstance::handle_put_request() - Invalid query");
                    msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST; // 4.00
                }
                free(query);
            }
        } else if ((operation() & SN_GRS_PUT_ALLOWED) != 0) {
            tr_debug("M2MResourceInstance::handle_put_request() - Request Content-Type %d", coap_content_type);

            if(COAP_CONTENT_OMA_TLV_TYPE == coap_content_type) {
                msg_code = COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT;
            } else {
                set_value(received_coap_header->payload_ptr, received_coap_header->payload_len);
                if(received_coap_header->payload_ptr) {
                   tr_debug("M2MResourceInstance::handle_put_request() - Update Resource with new values");
                    if(observation_handler) {
                        String value = "";
                        if (received_coap_header->uri_path_ptr != NULL &&
                            received_coap_header->uri_path_len > 0) {

                            value.append_raw((char*)received_coap_header->uri_path_ptr, received_coap_header->uri_path_len);
                        }
                        execute_value_updated = true;
                    }
                }
            }
        } else {
            // Operation is not allowed.
            tr_error("M2MResourceInstance::handle_put_request() - COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED");
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

void M2MResourceInstance::set_resource_observer(M2MResourceCallback *resource)
{
    _resource_callback = resource;
}

const String& M2MResourceInstance::object_name() const
{
    return _object_name;
}

uint16_t M2MResourceInstance::object_instance_id() const
{
    return _object_instance_id;
}
