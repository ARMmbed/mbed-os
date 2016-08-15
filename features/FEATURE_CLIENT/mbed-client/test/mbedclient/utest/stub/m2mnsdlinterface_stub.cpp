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
#include "m2mnsdlinterface_stub.h"


bool m2mnsdlinterface_stub::bool_value;
uint32_t m2mnsdlinterface_stub::int_value;
void * m2mnsdlinterface_stub::void_value;
String m2mnsdlinterface_stub::string_value;
void m2mnsdlinterface_stub::clear()
{
    bool_value = false;
    int_value = 0;
    string_value = "";
    void_value = NULL;
}

M2MNsdlInterface::M2MNsdlInterface(M2MNsdlObserver &observer)
: _observer(observer)
{
}

M2MNsdlInterface::~M2MNsdlInterface()
{
}

bool M2MNsdlInterface::initialize()
{
    return m2mnsdlinterface_stub::bool_value;
}

void M2MNsdlInterface::create_endpoint(const String &,
                                       const String &,
                                       const int32_t,
                                       const String &,
                                       const uint8_t,
                                       const String &)
{
}

void M2MNsdlInterface::delete_endpoint()
{
}

bool M2MNsdlInterface::create_nsdl_list_structure(const M2MObjectList &)
{
    return m2mnsdlinterface_stub::bool_value;
}

bool M2MNsdlInterface::delete_nsdl_resource(const String &)
{
    return m2mnsdlinterface_stub::bool_value;
}

bool M2MNsdlInterface::create_bootstrap_resource(sn_nsdl_addr_s *, const String& name)
{
    return m2mnsdlinterface_stub::bool_value;
}

bool M2MNsdlInterface::send_register_message(uint8_t*,
                                             const uint16_t,
                                             sn_nsdl_addr_type_e)
{
    return m2mnsdlinterface_stub::bool_value;
}

bool M2MNsdlInterface::send_update_registration(const uint32_t)
{
    return m2mnsdlinterface_stub::bool_value;
}

bool M2MNsdlInterface::send_unregister_message()
{
    return m2mnsdlinterface_stub::bool_value;
}

void *M2MNsdlInterface::memory_alloc(uint16_t size)
{
    return malloc(size);
}

void M2MNsdlInterface::memory_free(void *ptr)
{
    free(ptr);
    ptr = NULL;
}

uint8_t M2MNsdlInterface::send_to_server_callback(struct nsdl_s *,
                                                  sn_nsdl_capab_e,
                                                  uint8_t *,
                                                  uint16_t,
                                                  sn_nsdl_addr_s *)
{
    return (uint8_t)m2mnsdlinterface_stub::int_value;
}

uint8_t M2MNsdlInterface::received_from_server_callback(struct nsdl_s *,
                                                        sn_coap_hdr_s *,
                                                        sn_nsdl_addr_s *)
{
    return (uint8_t)m2mnsdlinterface_stub::int_value;
}

uint8_t M2MNsdlInterface::resource_callback(struct nsdl_s *,
                                            sn_coap_hdr_s *,
                                            sn_nsdl_addr_s *,
                                            sn_nsdl_capab_e)
{
    return (uint8_t)m2mnsdlinterface_stub::int_value;
}


bool M2MNsdlInterface::process_received_data(uint8_t *,
                                             uint16_t ,
                                             sn_nsdl_addr_s *)
{
    return m2mnsdlinterface_stub::bool_value;
}

void M2MNsdlInterface::stop_timers()
{

}

void M2MNsdlInterface::timer_expired(M2MTimerObserver::Type)
{
}

void M2MNsdlInterface::observation_to_be_sent(M2MBase *, uint16_t, Vector<uint16_t>, bool)
{
}

void M2MNsdlInterface::send_delayed_response(M2MBase *)
{
}

void M2MNsdlInterface::resource_to_be_deleted(const String &)
{
}

void M2MNsdlInterface::value_updated(M2MBase *, const String&)
{
}

void M2MNsdlInterface::remove_object(M2MBase *)
{
}

nsdl_s* M2MNsdlInterface::get_nsdl_handle()
{
    return (nsdl_s*) m2mnsdlinterface_stub::void_value;
}

void M2MNsdlInterface::handle_bootstrap_put_message(sn_coap_hdr_s *coap_header, sn_nsdl_addr_s *address)
{

}

void M2MNsdlInterface::handle_bootstrap_finished(sn_coap_hdr_s *coap_header,sn_nsdl_addr_s *address)
{

}

void M2MNsdlInterface::handle_bootstrap_delete(sn_coap_hdr_s *coap_header,sn_nsdl_addr_s *address)
{

}

bool M2MNsdlInterface::parse_bootstrap_message(sn_coap_hdr_s *coap_header, bool is_security_object)
{

}

bool M2MNsdlInterface::validate_security_object()
{

}

void M2MNsdlInterface::handle_bootstrap_error()
{

}

const String& M2MNsdlInterface::endpoint_name() const
{
    return m2mnsdlinterface_stub::string_value;
}
