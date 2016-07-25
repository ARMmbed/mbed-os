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
#include "mbed-client/m2mbase.h"
#include "mbed-client/m2mobservationhandler.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mtimer.h"
#include "include/m2mreporthandler.h"
#include "include/nsdllinker.h"
#include "mbed-trace/mbed_trace.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define TRACE_GROUP "mClt"

M2MBase& M2MBase::operator=(const M2MBase& other)
{
    if (this != &other) { // protect against invalid self-assignment
        _operation = other._operation;
        _mode = other._mode;
        _name = other._name;
        _resource_type = other._resource_type;
        _interface_description = other._interface_description;
        _coap_content_type = other._coap_content_type;
        _instance_id = other._instance_id;
        _observable = other._observable;
        _observation_number = other._observation_number;
        _observation_level = other._observation_level;
        _observation_handler = other._observation_handler;
        _register_uri = other._register_uri;
        _uri_path = other._uri_path;
        _max_age = other._max_age;
        _is_under_observation = other._is_under_observation;

        free(_token);
        _token = NULL;

        _token_length = other._token_length;
        if(other._token) {
            _token = alloc_string_copy(other._token, other._token_length);
        }

        delete _report_handler;
        _report_handler = NULL;

        if(other._report_handler) {
            _report_handler = new M2MReportHandler(*other._report_handler);
        }
    }
    return *this;
}

M2MBase::M2MBase(const M2MBase& other) :
    _report_handler(NULL),
    _observation_handler(other._observation_handler),
    _name(other._name),
    _resource_type(other._resource_type),
    _interface_description(other._interface_description),
    _uri_path(other._uri_path),
    _max_age(other._max_age),
    _instance_id(other._instance_id),
    _observation_number(other._observation_number),
    _token(NULL),
    _token_length(other._token_length),
    _coap_content_type(other._coap_content_type),
    _operation(other._operation),
    _mode(other._mode),
    _observation_level(other._observation_level),
    _observable(other._observable),
    _register_uri(other._register_uri),
    _is_under_observation(other._is_under_observation),
    _function_pointer(NULL)
{

    if(other._token) {
        _token = alloc_string_copy((uint8_t *)other._token, other._token_length);
    }

    if(other._report_handler) {
        _report_handler = new M2MReportHandler(*other._report_handler);
    }
}

M2MBase::M2MBase(const String & resource_name,
                 M2MBase::Mode mde)
: _report_handler(NULL),
  _observation_handler(NULL),
  _name(resource_name),
  _uri_path(""),
  _max_age(0),
  _instance_id(0),
  _observation_number(0),
  _token(NULL),
  _token_length(0),
  _coap_content_type(0),
  _operation(M2MBase::NOT_ALLOWED),
  _mode(mde),
  _observation_level(M2MBase::None),
  _observable(false),
  _register_uri(true),
  _is_under_observation(false),
  _function_pointer(NULL)
{
    if(is_integer(_name) && _name.size() <= MAX_ALLOWED_STRING_LENGTH) {
        _name_id = strtoul(_name.c_str(), NULL, 10);
        if(_name_id > 65535){
            _name_id = -1;
        }
    } else {
        _name_id = -1;
    }
}

M2MBase::~M2MBase()
{
    delete _report_handler;
    free(_token);
    delete _function_pointer;
}

void M2MBase::set_operation(M2MBase::Operation opr)
{
    // If the mode is Static, there is only GET_ALLOWED
   // supported.
    if(M2MBase::Static == _mode) {
        _operation = M2MBase::GET_ALLOWED;
    } else {
        _operation = opr;
    }
}

void M2MBase::set_interface_description(const String &desc)
{
    _interface_description = desc;
}

void M2MBase::set_resource_type(const String &res_type)
{
    _resource_type = res_type;
}

void M2MBase::set_coap_content_type(const uint8_t con_type)
{
    _coap_content_type = con_type;
}

void M2MBase::set_observable(bool observable)
{
    _observable = observable;
}

void M2MBase::add_observation_level(M2MBase::Observation obs_level)
{
    _observation_level = (M2MBase::Observation)(_observation_level | obs_level);
}

void M2MBase::remove_observation_level(M2MBase::Observation obs_level)
{
    _observation_level = (M2MBase::Observation)(_observation_level & ~obs_level);
}

void M2MBase::set_under_observation(bool observed,
                                    M2MObservationHandler *handler)
{

    tr_debug("M2MBase::set_under_observation - observed: %d", observed);
    tr_debug("M2MBase::set_under_observation - base_type: %d", _base_type);
    _is_under_observation = observed;
    _observation_handler = handler;
    if(handler) {
        if (_base_type != M2MBase::ResourceInstance) {
            if(!_report_handler){
                _report_handler = new M2MReportHandler(*this);
            }
            _report_handler->set_under_observation(observed);
        }
    } else {
        delete _report_handler;
        _report_handler = NULL;
    }
}

void M2MBase::set_observation_token(const uint8_t *token, const uint8_t length)
{
     free(_token);
     _token = NULL;
     _token_length = 0;

    if( token != NULL && length > 0 ) {
        _token = alloc_string_copy((uint8_t *)token, length);
        if(_token) {
            _token_length = length;
        }
    }
}

void M2MBase::set_instance_id(const uint16_t inst_id)
{
    _instance_id = inst_id;
}


void M2MBase::set_observation_number(const uint16_t /*observation_number*/)
{
}

void M2MBase::set_max_age(const uint32_t max_age)
{
    _max_age = max_age;
}

M2MBase::BaseType M2MBase::base_type() const
{
    return _base_type;
}

M2MBase::Operation M2MBase::operation() const
{
    return _operation;
}

const String& M2MBase::name() const
{
    return _name;
}

int32_t M2MBase::name_id() const
{
    return _name_id;
}

uint16_t M2MBase::instance_id() const
{
    return _instance_id;
}

const String& M2MBase::interface_description() const
{
    return _interface_description;
}

const String& M2MBase::resource_type() const
{
    return _resource_type;
}

uint8_t M2MBase::coap_content_type() const
{
    return _coap_content_type;
}

bool M2MBase::is_observable() const
{
    return _observable;
}

M2MBase::Observation M2MBase::observation_level() const
{
    return _observation_level;
}

void M2MBase::get_observation_token(uint8_t *&token, uint32_t &token_length)
{
    token_length = 0;
    free(token);

    token = alloc_string_copy((uint8_t *)_token, _token_length);
    if(token) {
        token_length = _token_length;
    }
}

M2MBase::Mode M2MBase::mode() const
{
    return _mode;
}

uint16_t M2MBase::observation_number() const
{
    return _observation_number;
}

uint32_t M2MBase::max_age() const
{
    return _max_age;
}

bool M2MBase::handle_observation_attribute(char *&query)
{
    tr_debug("M2MBase::handle_observation_attribute - under observation(%d)", is_under_observation());
    bool success = false;
    if(_report_handler) {
        success = _report_handler->parse_notification_attribute(query,_base_type);
        if (success) {
            if (is_under_observation()) {
                _report_handler->set_under_observation(true);
            }
         } else {
            _report_handler->set_default_values();
        }
    }
    return success;
}

void M2MBase::observation_to_be_sent(m2m::Vector<uint16_t> changed_instance_ids, bool send_object)
{
    //TODO: Move this to M2MResourceInstance
    if(_observation_handler) {
       _observation_number++;
       _observation_handler->observation_to_be_sent(this,
                                                    _observation_number,
                                                    changed_instance_ids,
                                                    send_object);
    }
}

void M2MBase::set_base_type(M2MBase::BaseType type)
{
    _base_type = type;
}

void M2MBase::remove_resource_from_coap(const String &resource_name)
{
    if(_observation_handler) {
        _observation_handler->resource_to_be_deleted(resource_name);
    }
}

void M2MBase::remove_object_from_coap()
{
    if(_observation_handler) {
        _observation_handler->remove_object(this);
    }
}

sn_coap_hdr_s* M2MBase::handle_get_request(nsdl_s */*nsdl*/,
                                           sn_coap_hdr_s */*received_coap_header*/,
                                           M2MObservationHandler */*observation_handler*/)
{
    //Handled in M2MResource, M2MObjectInstance and M2MObject classes
    return NULL;
}

sn_coap_hdr_s* M2MBase::handle_put_request(nsdl_s */*nsdl*/,
                                           sn_coap_hdr_s */*received_coap_header*/,
                                           M2MObservationHandler */*observation_handler*/,
                                           bool &)
{
    //Handled in M2MResource, M2MObjectInstance and M2MObject classes
    return NULL;
}

sn_coap_hdr_s* M2MBase::handle_post_request(nsdl_s */*nsdl*/,
                                            sn_coap_hdr_s */*received_coap_header*/,
                                            M2MObservationHandler */*observation_handler*/,
                                            bool &)
{
    //Handled in M2MResource, M2MObjectInstance and M2MObject classes
    return NULL;
}

void *M2MBase::memory_alloc(uint32_t size)
{
    if(size)
        return malloc(size);
    else
        return 0;
}

void M2MBase::memory_free(void *ptr)
{
    free(ptr);
}

uint8_t* M2MBase::alloc_string_copy(const uint8_t* source, uint32_t size)
{
    assert(source != NULL);

    uint8_t* result = (uint8_t*)memory_alloc(size + 1);
    if (result) {
        memcpy(result, source, size);
        result[size] = '\0';
    }
    return result;
}

uint8_t* M2MBase::alloc_copy(const uint8_t* source, uint32_t size)
{
    assert(source != NULL);

    uint8_t* result = (uint8_t*)memory_alloc(size);
    if (result) {
        memcpy(result, source, size);
    }
    return result;
}

M2MReportHandler* M2MBase::report_handler()
{
    return _report_handler;
}

M2MObservationHandler* M2MBase::observation_handler()
{
    return _observation_handler;
}

void M2MBase::set_register_uri( bool register_uri)
{
    _register_uri = register_uri;
}

bool M2MBase::register_uri()
{
    return _register_uri;
}

bool M2MBase::is_integer(const String &value)
{
    const char *s = value.c_str();
    if(value.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
        return false;
    }
    char * p ;
    strtol(value.c_str(), &p, 10);
    return (*p == 0);
}

void M2MBase::set_uri_path(const String &path)
{
    _uri_path = path;
}

const String& M2MBase::uri_path() const
{
    return _uri_path;
}

bool M2MBase::is_under_observation() const
{
    return _is_under_observation;
}

void M2MBase::set_value_updated_function(value_updated_callback callback)
{
    _value_updated_callback = callback;
}

void M2MBase::set_value_updated_function(value_updated_callback2 callback)
{
    delete _function_pointer;
    _function_pointer = new FP1<void, const char*>(callback);
    set_value_updated_function(value_updated_callback(_function_pointer,
                                                      &FP1<void, const char*>::call));
}
bool M2MBase::is_value_updated_function_set()
{
    return (_value_updated_callback) ? true : false;
}

void M2MBase::execute_value_updated(const String& name)
{
    if(_value_updated_callback) {
        _value_updated_callback(name.c_str());
    }
}
