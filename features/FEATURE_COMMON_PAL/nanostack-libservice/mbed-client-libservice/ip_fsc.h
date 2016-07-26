/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
#ifndef _NS_FSC_H
#define _NS_FSC_H

#include "ns_types.h"

#define NEXT_HEADER_TCP     0x06
#define NEXT_HEADER_UDP     0x11
#define NEXT_HEADER_ICMP6   0x3A

extern uint16_t ip_fcf_v(uint_fast8_t count, const ns_iovec_t vec[static count]);
extern uint16_t ipv6_fcf(const uint8_t src_address[static 16], const uint8_t dest_address[static 16],
                         uint16_t data_length, const uint8_t data_ptr[static data_length],  uint8_t next_protocol);

#endif
