/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#ifndef MESH_H_
#define MESH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Thread network has routing cost limit 16. */
#ifndef MESH_DEFAULT_HOPS_LEFT
#define MESH_DEFAULT_HOPS_LEFT 16
#endif

typedef struct mesh_routing_route_response_s {
    uint8_t addr_len;
    uint8_t address[8];
    bool intercept;
} mesh_routing_route_response_t;

typedef int_fast8_t mesh_first_hop_fn_t(protocol_interface_info_entry_t *cur,
                                        uint_fast8_t dest_addr_len, const uint8_t dest_addr[],
                                        mesh_routing_route_response_t *resp);
typedef int_fast8_t mesh_route_fn_t(protocol_interface_info_entry_t *cur,
                                    uint_fast8_t last_hop_addr_len, const uint8_t last_hop_addr[],
                                    uint_fast8_t dest_addr_len, const uint8_t dest_addr[],
                                    mesh_routing_route_response_t *resp);
typedef int_fast8_t mesh_multicast_fn_t(protocol_interface_info_entry_t *cur, uint16_t addr, buffer_t *buf);
typedef bool mesh_header_needed_fn_t(const protocol_interface_info_entry_t *cur, const buffer_t *buf);
typedef bool mesh_forwardable_address_t(const protocol_interface_info_entry_t *cur, addrtype_t addr_type, const uint8_t *addr);
typedef bool mesh_address_map_fn_t(protocol_interface_info_entry_t *cur, addrtype_t *addr_type, uint8_t *addr);

typedef struct mesh_callbacks_s {
    mesh_first_hop_fn_t *first_hop;
    mesh_route_fn_t *route;
    mesh_multicast_fn_t *multicast;
    mesh_header_needed_fn_t *header_needed;
    mesh_forwardable_address_t *forwardable_address;
    mesh_address_map_fn_t *address_map;
} mesh_callbacks_t;

#ifdef HAVE_MESH
buffer_t *mesh_up(buffer_t *buf);
buffer_t *mesh_down(buffer_t *buf);

bool mesh_address_map(protocol_interface_info_entry_t *cur, addrtype_t *addr_type, uint8_t *addr);
bool mesh_forwardable_address(const protocol_interface_info_entry_t *cur, addrtype_t addr_type, const uint8_t *addr);
bool mesh_header_needed(const buffer_t *buf);
bool mesh_supports_multicast(void);
void mesh_all_addresses_unicast(bool flag);
bool mesh_short_address_is_multicast(uint16_t addr);
bool mesh_short_address_is_broadcast_or_multicast(uint16_t addr);
void mesh_rewrite_bc0_header(uint8_t *header);
uint_fast8_t mesh_header_size(const buffer_t *buf);
uint_fast8_t mesh_header_len_from_type_byte(uint8_t type);
uint_fast8_t mesh_header_len_from_buffer_type_byte(const buffer_t *buf);
#else
#define mesh_address_map(cur, addr_type, addr) true
#define mesh_forwardable_address(cur, addr_type, addr) false
#define mesh_header_needed(buf) false
#define mesh_rewrite_bc0_header(header) ((void) 0)
#define mesh_header_size(buf) false
#define mesh_header_len_from_type_byte(type) 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* MESH_H_ */
