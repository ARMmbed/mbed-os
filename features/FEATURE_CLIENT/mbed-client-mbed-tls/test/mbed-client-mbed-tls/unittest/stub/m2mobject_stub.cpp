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
#include "m2mobject_stub.h"

u_int16_t m2mobject_stub::int_value;
bool m2mobject_stub::bool_value;
M2MObjectInstance* m2mobject_stub::inst;
M2MObjectInstanceList m2mobject_stub::instance_list;
M2MBase::BaseType m2mobject_stub::base_type;
sn_coap_hdr_s *m2mobject_stub::header;

void m2mobject_stub::clear()
{
    int_value = 0;
    bool_value = false;
    m2mobject_stub::base_type = M2MBase::Object;
    instance_list.clear();
    header = NULL;
}

M2MObject::M2MObject(const String &object_name)
: M2MBase(object_name,M2MBase::Dynamic)
{
}

M2MObject::~M2MObject()
{
}

M2MObject& M2MObject::operator=(const M2MObject& )
{
    return *this;
}

M2MObject::M2MObject(const M2MObject& other)
: M2MBase(other)
{
    *this = other;
}

M2MObjectInstance* M2MObject::create_object_instance(uint16_t instance_id)
{
    return m2mobject_stub::inst;
}

bool M2MObject::remove_object_instance(uint16_t)
{
    return m2mobject_stub::bool_value;
}

M2MObjectInstance* M2MObject::object_instance(uint16_t) const
{
    return m2mobject_stub::inst;
}

const M2MObjectInstanceList& M2MObject::instances() const
{
    return m2mobject_stub::instance_list;
}

uint16_t M2MObject::instance_count() const
{
    return m2mobject_stub::int_value;
}

M2MBase::BaseType M2MObject::base_type() const
{
    return m2mobject_stub::base_type;
}

void M2MObject::add_observation_level(M2MBase::Observation)
{

}

void M2MObject::remove_observation_level(M2MBase::Observation)
{

}

sn_coap_hdr_s* M2MObject::handle_get_request(nsdl_s *,
                                  sn_coap_hdr_s *,
                                  M2MObservationHandler *)
{
    return m2mobject_stub::header;
}

sn_coap_hdr_s* M2MObject::handle_put_request(nsdl_s *,
                                  sn_coap_hdr_s *,
                                  M2MObservationHandler *,
                                  bool &execute)
{
    execute = m2mobject_stub::bool_value;
    return m2mobject_stub::header;
}

sn_coap_hdr_s* M2MObject::handle_post_request(nsdl_s *,
                                   sn_coap_hdr_s *,
                                   M2MObservationHandler *,
                                   bool &)
{
    return m2mobject_stub::header;
}

void M2MObject::notification_update(uint16_t obj_instance_id)
{
}
