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
#include "m2mresource_stub.h"

uint32_t m2mresource_stub::int_value;
uint8_t* m2mresource_stub::delayed_token;
uint8_t m2mresource_stub::delayed_token_len;
bool m2mresource_stub::bool_value;

// The statically initialized list must be bigh enough to cater 
// for all the tests, or the utest framework will complain for memory leak.
M2MResourceInstanceList m2mresource_stub::list(12);

M2MResourceInstance *m2mresource_stub::instance;
sn_coap_hdr_s *m2mresource_stub::header;
void m2mresource_stub::clear()
{
    int_value = 0;
    delayed_token = NULL;
    delayed_token_len = 0;
    bool_value = false;
    list.clear();
    instance = NULL;
}

M2MResource& M2MResource::operator=(const M2MResource& other)
{
    return *this;
}

M2MResource::M2MResource(const M2MResource& other)
: M2MResourceInstance(other)
{
    *this = other;
}

M2MResource::M2MResource(M2MObjectInstanceCallback &object_instance_callback,
                         const String &resource_name,
                         const String &resource_type,
                         M2MResourceInstance::ResourceType type,
                         const uint8_t *value,
                         const uint8_t value_length,
                         const uint16_t object_instance_id,
                         const String &object_name,
                         bool)
: M2MResourceInstance(resource_name, resource_type, type, value, value_length,
                      object_instance_callback, object_instance_id, object_name)
{
}

M2MResource::M2MResource(M2MObjectInstanceCallback &object_instance_callback,
                         const String &resource_name,
                         const String &resource_type,
                         M2MResourceInstance::ResourceType type,
                         bool,
                         const uint16_t object_instance_id,
                         const String &object_name,
                         bool)
: M2MResourceInstance(resource_name, resource_type, type,
                      object_instance_callback, object_instance_id, object_name)
{
}

M2MResource::~M2MResource()
{
}

bool M2MResource::supports_multiple_instances() const
{
    return m2mresource_stub::bool_value;
}

void M2MResource::get_delayed_token(unsigned char *&token, unsigned char &token_len)
{
    token_len = 0;
    if(token) {
        free(token);
        token = NULL;
    }
    token = (uint8_t *)malloc(m2mresource_stub::delayed_token_len);
    if(token) {
        token_len = m2mresource_stub::delayed_token_len;
        memcpy((uint8_t *)token, (uint8_t *)m2mresource_stub::delayed_token, token_len);
    }
}

bool M2MResource::remove_resource_instance(uint16_t inst_id)
{
    return m2mresource_stub::bool_value;
}

M2MResourceInstance* M2MResource::resource_instance(uint16_t inst_id) const
{
    return m2mresource_stub::instance;
}

const M2MResourceInstanceList& M2MResource::resource_instances() const
{
    return m2mresource_stub::list;
}

uint16_t M2MResource::resource_instance_count() const
{
    return m2mresource_stub::int_value;
}

bool M2MResource::handle_observation_attribute(char *&query)
{
    return m2mresource_stub::bool_value;
}

void M2MResource::add_resource_instance(M2MResourceInstance *)
{
}

void M2MResource::add_observation_level(M2MBase::Observation)
{
}

void M2MResource::remove_observation_level(M2MBase::Observation)
{
}

void M2MResource::notification_update()
{
}

sn_coap_hdr_s* M2MResource::handle_get_request(nsdl_s *,
                                               sn_coap_hdr_s *,
                                               M2MObservationHandler *)
{
    return m2mresource_stub::header;
}

sn_coap_hdr_s* M2MResource::handle_put_request(nsdl_s *,
                                               sn_coap_hdr_s *,
                                               M2MObservationHandler *,
                                               bool &)
{
    return m2mresource_stub::header;
}

sn_coap_hdr_s* M2MResource::handle_post_request(nsdl_s *,
                                               sn_coap_hdr_s *,
                                               M2MObservationHandler *,
                                               bool &)
{
    return m2mresource_stub::header;
}
