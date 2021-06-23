/*
 * Copyright (c) 2013-2017, Pelion and affiliates.
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
#ifndef CIPV6_FRAGMENTER_H
#define CIPV6_FRAGMENTER_H

struct buffer;
int8_t reassembly_interface_reset(int8_t interface_id);
int8_t reassembly_interface_init(int8_t interface_id, uint8_t reassembly_session_limit, uint16_t reassembly_timeout);
int8_t reassembly_interface_free(int8_t interface_id);

void cipv6_frag_timer(uint16_t seconds);
struct buffer *cipv6_frag_reassembly(int8_t interface_id, struct buffer *buf);



#endif
