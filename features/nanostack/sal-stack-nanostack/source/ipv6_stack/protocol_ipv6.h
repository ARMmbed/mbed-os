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

#ifndef PROTOCOL_IPV6_H_
#define PROTOCOL_IPV6_H_

#ifdef HAVE_ETHERNET

struct protocol_interface_info_entry;

/**
 * \brief Initialize Ethernet MAC and IPv6 base
 *
 * Call only once at bootstrap.
 *
 * \param mac_address pointer to 48-bit MAC address
 *
 * \return 0> Interface ID for IPv6
 * \return -1 Init fail
 *
 */
extern int8_t ipv6_MAC_init(int8_t ethernet_dev_driver_id);

void ipv6_interface_phy_sap_register(struct protocol_interface_info_entry *cur);

int8_t ipv6_interface_configure_ipv6_bootstrap_set(int8_t interface_id, net_ipv6_mode_e bootstrap_mode, const uint8_t *ipv6_prefix_pointer);

/**
 * \brief Ethernet IPv6 Interface Configure
 *
 * \param cur pointer to interface pointer.
 *
 */
extern int8_t ipv6_interface_up(struct protocol_interface_info_entry *cur);
/**
 * \brief Disable IPv6 protocol stack
 */
extern int8_t ipv6_interface_down(struct protocol_interface_info_entry *cur);

struct ipv6_neighbour;
struct ipv6_neighbour_cache;
struct buffer;
struct protocol_interface_info_entry;

void ipv6_core_timer_event_handle(struct protocol_interface_info_entry *cur, uint8_t event);
void ipv6_core_slow_timer_event_handle(struct protocol_interface_info_entry *cur);

int ipv6_prefix_register(uint8_t *prefix_64, uint32_t lifetime, uint32_t prefer_lifetime);
int ipv6_prefix_router_flag_activate(uint8_t *ipv6_address);

void ipv6_nd_ra_advert(struct protocol_interface_info_entry *cur, const uint8_t *dest);
void ipv6_interface_slaac_handler(struct protocol_interface_info_entry *cur, uint8_t *slaacPrefix, uint8_t prefixLen, uint32_t validLifeTime, uint32_t preferredLifeTime);
void ipv6_stack_route_advert_update(uint8_t *address, uint8_t prefixLength, uint8_t routePrefer);
void ipv6_stack_route_advert_remove(uint8_t *address, uint8_t prefixLength);
void ipv6_prefix_on_link_update(uint8_t *address);
void ipv6_prefix_on_link_remove(uint8_t *address);
int8_t ipv6_interface_accept_ra(int8_t interface_id, net_ipv6_accept_ra_e accept_ra);
#else
#define ipv6_interface_configure_ipv6_bootstrap_set(nwk_interface_id, bootstrap_mode, ipv6_prefix_pointer) -1
#define ipv6_core_slow_timer_event_handle(cur) ((void)0)
#define ipv6_core_timer_event_handle(cur, event) ((void)0)
#define ipv6_interface_slaac_handler(cur, slaacPrefix, prefixLen, validLifeTime, preferredLifeTime) ((void)0)
#define ipv6_nd_ra_advert(cur, dest) ((void)0)
#define ipv6_interface_sitelocal_clone(buf) ((void)0)
#define ipv6_stack_route_advert_remove(address, prefixLength) ((void)0)
#define ipv6_prefix_on_link_update(address) ((void)0)
#define ipv6_prefix_on_link_remove(address) ((void)0)
#define ipv6_stack_route_advert_update(address, prefixLength, routePrefer) ((void)0)
#define ipv6_interface_accept_ra(interface_id, accept_ra) -1
#endif

#endif /* PROTOCOL_IPV6_H_ */
