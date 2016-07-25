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
#include "m2mobjectinstance_stub.h"

u_int8_t m2mobjectinstance_stub::int_value;
bool m2mobjectinstance_stub::bool_value;
M2MResource* m2mobjectinstance_stub::resource;
M2MResource* m2mobjectinstance_stub::create_resource;

// The statically initialized list must be bigh enough to cater 
// for all the tests, or the utest framework will complain for memory leak.
M2MResourceList m2mobjectinstance_stub::resource_list(12);

M2MResourceInstance* m2mobjectinstance_stub::create_resource_instance;
sn_coap_hdr_s* m2mobjectinstance_stub::header;
M2MBase::BaseType m2mobjectinstance_stub::base_type;


void m2mobjectinstance_stub::clear()
{
    int_value = 0;
    bool_value = false;
    resource = NULL;
    create_resource = NULL;
    create_resource_instance = NULL;
    header = NULL;
    base_type = M2MBase::ObjectInstance;
    resource_list.clear();
}

M2MObjectInstance& M2MObjectInstance::operator=(const M2MObjectInstance& )
{
    return *this;
}

M2MObjectInstance::M2MObjectInstance(const M2MObjectInstance& other)
: M2MBase(other),
  _object_callback(other._object_callback)
{
    *this = other;
}

M2MObjectInstance::M2MObjectInstance(const String &object_name, M2MObjectCallback &object_callback)
: M2MBase(object_name,M2MBase::Dynamic),
  _object_callback(object_callback)
{
}

M2MObjectInstance::~M2MObjectInstance()
{
}

M2MResource* M2MObjectInstance::create_static_resource(const String &,
                                               const String &,
                                               M2MResourceInstance::ResourceType,
                                               const uint8_t *,
                                               const uint8_t,
                                               bool)
{
    return m2mobjectinstance_stub::create_resource;
}

M2MResource* M2MObjectInstance::create_dynamic_resource(const String &,
                                                const String &,
                                                M2MResourceInstance::ResourceType,
                                                bool,
                                                bool)
{
    return m2mobjectinstance_stub::create_resource;
}

M2MResourceInstance* M2MObjectInstance::create_static_resource_instance(const String &,
                                               const String &,
                                               M2MResourceInstance::ResourceType,
                                               const uint8_t *,
                                               const uint8_t,
                                               uint16_t)
{
    return m2mobjectinstance_stub::create_resource_instance;
}

M2MResourceInstance* M2MObjectInstance::create_dynamic_resource_instance(const String &,
                                                const String &,
                                                M2MResourceInstance::ResourceType,
                                                bool,
                                                uint16_t)
{
    return m2mobjectinstance_stub::create_resource_instance;
}

bool M2MObjectInstance::remove_resource(const String &)
{
    return m2mobjectinstance_stub::bool_value;
}

bool M2MObjectInstance::remove_resource_instance(const String &, uint16_t)
{
    return m2mobjectinstance_stub::bool_value;
}

M2MResource* M2MObjectInstance::resource(const String &) const
{
    return m2mobjectinstance_stub::resource;
}

const M2MResourceList& M2MObjectInstance::resources() const
{
    return m2mobjectinstance_stub::resource_list;
}

uint16_t M2MObjectInstance::resource_count() const
{
    return m2mobjectinstance_stub::int_value;
}

uint16_t M2MObjectInstance::resource_count(const String& ) const
{
    return m2mobjectinstance_stub::int_value;
}

M2MBase::BaseType M2MObjectInstance::base_type() const
{
    return m2mobjectinstance_stub::base_type;
}

void M2MObjectInstance::add_observation_level(M2MBase::Observation)
{
}

void M2MObjectInstance::remove_observation_level(M2MBase::Observation)
{
}

sn_coap_hdr_s* M2MObjectInstance::handle_get_request(nsdl_s *,
                                  sn_coap_hdr_s *,
                                  M2MObservationHandler *)
{
    return m2mobjectinstance_stub::header;
}

sn_coap_hdr_s* M2MObjectInstance::handle_put_request(nsdl_s *,
                                  sn_coap_hdr_s *,
                                  M2MObservationHandler *,
                                  bool &)
{
    return m2mobjectinstance_stub::header;
}

sn_coap_hdr_s* M2MObjectInstance::handle_post_request(nsdl_s *,
                                   sn_coap_hdr_s *,
                                   M2MObservationHandler *,
                                   bool &execute)
{
    execute = m2mobjectinstance_stub::bool_value;
    return m2mobjectinstance_stub::header;
}

void M2MObjectInstance::notification_update(M2MBase::Observation)
{
}
