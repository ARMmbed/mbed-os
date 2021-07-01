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

#ifndef IPV6_FLOW_H_
#define IPV6_FLOW_H_

#include "ns_types.h"

extern bool ipv6_flow_auto_label;

uint_fast24_t ipv6_flow_5tuple(const uint8_t src_addr[static 16],
                               const uint8_t dst_addr[static 16],
                               uint8_t protocol,
                               uint16_t src_port,
                               uint16_t dst_port);

uint_fast24_t ipv6_flow_2tuple_flow(const uint8_t src_addr[static 16],
                                    const uint8_t dst_addr[static 16],
                                    uint_fast24_t flow);

uint_fast24_t ipv6_flow_random(void);

#endif /* IPV6_FLOW_H_ */
