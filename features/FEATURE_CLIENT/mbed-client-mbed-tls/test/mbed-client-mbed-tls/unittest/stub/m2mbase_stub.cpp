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
#include "m2mbase_stub.h"

#include <assert.h>

uint8_t m2mbase_stub::uint8_value;
uint16_t m2mbase_stub::uint16_value;
uint32_t m2mbase_stub::uint32_value;
uint16_t m2mbase_stub::int_value;
int32_t m2mbase_stub::name_id_value;

bool m2mbase_stub::bool_value;
String *m2mbase_stub::string_value;

M2MBase::BaseType m2mbase_stub::base_type;
M2MBase::Operation m2mbase_stub::operation;
M2MBase::Mode m2mbase_stub::mode_value;
M2MBase::Observation m2mbase_stub::observation_level_value;

void *m2mbase_stub::void_value;
M2MObservationHandler *m2mbase_stub::observe;
M2MReportHandler *m2mbase_stub::report;
bool m2mbase_stub::is_value_updated_function_set;


void m2mbase_stub::clear()
{
    int_value = 0;
    uint8_value = 0;
    uint16_value = 0;
    uint32_value = 0;
    string_value = NULL;
    name_id_value = -1;
    mode_value = M2MBase::Static;
    base_type = M2MBase::Object;
    observation_level_value = M2MBase::None;
    bool_value = false;
    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;
    void_value = NULL;
    observe = NULL;
    report = NULL;
    is_value_updated_function_set = false;
}

M2MBase::M2MBase(const String &/*resource_name*/,
                 M2MBase::Mode /*mode*/)
{
}

M2MBase& M2MBase::operator=(const M2MBase& other)
{
    if (this != &other) { // protect against invalid self-assignment
    }
    return *this;
}

M2MBase::M2MBase(const M2MBase& other)
{
    this->operator=(other);
}

M2MBase::~M2MBase()
{
}

void M2MBase::set_operation(M2MBase::Operation opr)
{
    m2mbase_stub::operation = opr;
}

void M2MBase::set_interface_description(const String &/*desc*/)
{
}

void M2MBase::set_resource_type(const String &/*res_type*/)
{
}

void M2MBase::set_coap_content_type(const uint8_t /*con_type*/)
{
}

void M2MBase::set_instance_id(const uint16_t /*inst_id*/)
{
}

void M2MBase::set_observation_number(const uint16_t /*observation_number*/)
{
}

void M2MBase::set_max_age(const uint32_t /*max_age*/)
{
}

M2MBase::Operation M2MBase::operation() const
{
    return m2mbase_stub::operation;
}

const String& M2MBase::name() const
{
    return *m2mbase_stub::string_value;
}

int32_t M2MBase::name_id() const
{
    return m2mbase_stub::name_id_value;
}

uint16_t M2MBase::instance_id() const
{
    return m2mbase_stub::int_value;
}

const String& M2MBase::interface_description() const
{
    return *m2mbase_stub::string_value;
}

const String& M2MBase::resource_type() const
{
    return *m2mbase_stub::string_value;
}

uint8_t M2MBase::coap_content_type() const
{
    return m2mbase_stub::uint8_value;
}

uint32_t M2MBase::max_age() const
{
    return m2mbase_stub::uint32_value;
}

void M2MBase::set_observable(bool /*observable*/)
{
}

void M2MBase::add_observation_level(M2MBase::Observation)
{
}

void M2MBase::remove_observation_level(M2MBase::Observation)
{
}

void M2MBase::set_under_observation(bool /*observed*/,
                                   M2MObservationHandler */*handler*/)
{
}

void M2MBase::set_observation_token(const uint8_t */*token*/,
                                    const uint8_t /*length*/)
{
}

bool M2MBase::is_observable() const
{
    return m2mbase_stub::bool_value;
}

M2MBase::Observation M2MBase::observation_level() const
{
    return m2mbase_stub::observation_level_value;
}

void M2MBase::get_observation_token(uint8_t *&/*token*/,
                                    uint32_t &/*length*/)
{
}

void M2MBase::set_base_type(M2MBase::BaseType /*type*/)
{
}

M2MBase::BaseType M2MBase::base_type() const
{
    return m2mbase_stub::base_type;
}

M2MBase::Mode M2MBase::mode() const
{
    return m2mbase_stub::mode_value;
}

uint16_t M2MBase::observation_number() const
{
    return m2mbase_stub::uint16_value;
}

void M2MBase::remove_resource_from_coap(const String &)
{
}

void M2MBase::remove_object_from_coap()
{
}

bool M2MBase::handle_observation_attribute(char *&query)
{
    return m2mbase_stub::bool_value;
}

void M2MBase::observation_to_be_sent(m2m::Vector<uint16_t>, bool)
{
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
    if(ptr)
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
    return m2mbase_stub::report;
}

M2MObservationHandler* M2MBase::observation_handler()
{
    return m2mbase_stub::observe;
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
                                           bool &execute_value_updated)
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

void M2MBase::set_register_uri( bool register_uri)
{
}

bool M2MBase::register_uri()
{
    return m2mbase_stub::bool_value;
}

void M2MBase::set_uri_path(const String &uri_path)
{
}

const String& M2MBase::uri_path() const
{
    return *m2mbase_stub::string_value;
}

bool M2MBase::is_under_observation() const
{
    return m2mbase_stub::bool_value;
}

void M2MBase::set_value_updated_function(value_updated_callback callback)
{

}

void M2MBase::set_value_updated_function(value_updated_callback2 callback)
{

}

bool M2MBase::is_value_updated_function_set()
{
    return m2mbase_stub::is_value_updated_function_set;
}

void M2MBase::execute_value_updated(const String& name)
{

}
