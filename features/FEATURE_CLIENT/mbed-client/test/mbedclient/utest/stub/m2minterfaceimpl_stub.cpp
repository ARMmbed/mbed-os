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
#include "m2minterfaceimpl_stub.h"
#include "common_stub.h"

u_int8_t m2minterfaceimpl_stub::int_value;
String m2minterfaceimpl_stub::string_value;
bool m2minterfaceimpl_stub::bool_value;

void m2minterfaceimpl_stub::clear()
{
    int_value = 0;
    bool_value = false;
    string_value = "";
}

M2MInterfaceImpl::M2MInterfaceImpl(M2MInterfaceObserver& observer,
                                   const String &,
                                   const String &,
                                   const int32_t,
                                   const uint16_t,
                                   const String &,
                                   M2MInterface::BindingMode,
                                   M2MInterface::NetworkStack,
                                   const String &)
: _observer(observer),
  _current_state(0),
  _max_states( STATE_MAX_STATES ),
  _event_generated(false),
  _event_data(NULL)
{
}

M2MInterfaceImpl::~M2MInterfaceImpl()
{
}
void M2MInterfaceImpl::bootstrap(M2MSecurity *)
{
}

void M2MInterfaceImpl::cancel_bootstrap()
{
}

void M2MInterfaceImpl::register_object(M2MSecurity *, const M2MObjectList &)
{
}

void M2MInterfaceImpl::update_registration(M2MSecurity *, const uint32_t)
{
}

void M2MInterfaceImpl::unregister_object(M2MSecurity*)
{
}

void M2MInterfaceImpl::set_queue_sleep_handler(callback_handler)
{

}

void M2MInterfaceImpl::set_platform_network_handler(void *)
{

}

void M2MInterfaceImpl::set_random_number_callback(random_number_cb)
{

}

void M2MInterfaceImpl::set_entropy_callback(entropy_cb)
{

}

void M2MInterfaceImpl::coap_message_ready(uint8_t *,
                                uint16_t ,
                                sn_nsdl_addr_s *)
{

}

void M2MInterfaceImpl::client_registered(M2MServer*)
{

}

void M2MInterfaceImpl::registration_error(uint8_t, bool)
{

}

void M2MInterfaceImpl::client_unregistered()
{

}

void M2MInterfaceImpl::bootstrap_done(M2MSecurity *)
{

}

void M2MInterfaceImpl::bootstrap_error()
{

}

void M2MInterfaceImpl::coap_data_processed()
{

}

void M2MInterfaceImpl::data_available(uint8_t*,
                            uint16_t,
                            const M2MConnectionObserver::SocketAddress &)
{

}

void M2MInterfaceImpl::socket_error(uint8_t, bool)
{

}

void M2MInterfaceImpl::address_ready(const M2MConnectionObserver::SocketAddress &,
                           M2MConnectionObserver::ServerType,
                           const uint16_t)
{

}

void M2MInterfaceImpl::data_sent()
{
}

void M2MInterfaceImpl::timer_expired(M2MTimerObserver::Type)
{

}

void M2MInterfaceImpl::registration_updated(const M2MServer &)
{
}

void M2MInterfaceImpl::value_updated(M2MBase *)
{

}

