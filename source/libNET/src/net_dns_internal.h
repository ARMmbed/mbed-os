/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef NET_DNS_INTERNAL_H_
#define NET_DNS_INTERNAL_H_

/**
 * These functions are used to store DNS server related information
 * IF DNS information is learned from IPv6 Router advertisements
 * or from the DHCPv6 elements
 *
 * Address is the link local address of the default router or DHCPv6 server address
 */

int8_t net_dns_server_address_set(int8_t interface_id, const uint8_t address[16], const uint8_t dns_server_address[16], uint32_t lifetime);
int8_t net_dns_server_search_list_set(int8_t interface_id, const uint8_t address[16], uint8_t *dns_search_list_ptr, uint8_t dns_search_list_len, uint32_t lifetime);

int8_t net_dns_server_get(int8_t interface_id, uint8_t dns_server_address[16], uint8_t **dns_search_list_ptr, uint8_t *dns_search_list_len, uint8_t index);

/**
 * Storage for DNS query results
 */

int8_t net_dns_query_result_set(int8_t interface_id, const uint8_t address[16], const char *domain_name_ptr, uint32_t lifetime);

int8_t net_dns_query_result_get(int8_t interface_id, uint8_t address[16], const char *domain_name_ptr);

/**
 * Generic timeout handler for all interfaces and entries.
 */
void net_dns_timer_seconds(uint32_t seconds);

#endif /* NET_DNS_INTERNAL_H_ */
