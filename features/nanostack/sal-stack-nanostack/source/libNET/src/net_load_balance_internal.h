/*
 * Copyright (c) 2016-2017, Pelion and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef NET_LOAD_BALANCE_INTERNAL_H_
#define NET_LOAD_BALANCE_INTERNAL_H_
#include "ns_types.h"
struct protocol_interface_info_entry;
void net_load_balance_internal_state_activate(struct protocol_interface_info_entry *interface_ptr, bool state);

#endif /* NET_LOAD_BALANCE_INTERNAL_H_ */
