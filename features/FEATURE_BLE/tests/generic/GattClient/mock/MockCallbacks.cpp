/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MockCallbacks.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor implementation of the mocked callbacks
//
// WARNING: Do not remove; it speedup compile time.

service_callback_t::service_callback_t() { }
service_callback_t::~service_callback_t() { }

characteristic_callback_t::characteristic_callback_t() { }
characteristic_callback_t::~characteristic_callback_t() { }

termination_callback_t::termination_callback_t() { }
termination_callback_t::~termination_callback_t() { }

read_callback_t::read_callback_t() { }
read_callback_t::~read_callback_t() { }

write_callback_t::write_callback_t() { }
write_callback_t::~write_callback_t() { }

server_event_callback_t::server_event_callback_t() { }
server_event_callback_t::~server_event_callback_t() { }

descriptor_callback_t::descriptor_callback_t() { }
descriptor_callback_t::~descriptor_callback_t() { }

descriptor_termination_callback_t::descriptor_termination_callback_t() { }
descriptor_termination_callback_t::~descriptor_termination_callback_t() { }

