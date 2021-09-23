/*
 * Copyright (c) 2015-2017, Pelion and affiliates.
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

#ifndef RPL_DATA_H_
#define RPL_DATA_H_

struct rpl_domain;
struct rpl_instance;
struct ipv6_route_info;
struct ns_sockaddr;

/* Internal API */
void rpl_data_sr_invalidate(void);

#ifdef HAVE_RPL
/* Public API */
void rpl_data_init(void);
#ifdef HAVE_RPL_ROOT
void rpl_data_init_root(void);
#endif
ipv6_route_predicate_fn_t *rpl_data_get_route_predicate(struct rpl_domain *domain, const buffer_t *buf);
bool rpl_data_process_hbh(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *opt, const struct ns_sockaddr *ll_src);
bool rpl_data_remember_outer(buffer_t *buf);
bool rpl_data_is_rpl_route(ipv6_route_src_t source);
bool rpl_data_is_rpl_parent_route(ipv6_route_src_t source);
bool rpl_data_forwarding_error(buffer_t *buf);
buffer_t *rpl_data_process_routing_header(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t *hdrlen_out, bool *forward_out);
buffer_t *rpl_data_source_route_error_handler(buffer_t *buf, protocol_interface_info_entry_t *cur);
bool rpl_data_get_srh_last_address(const uint8_t *srh, uint8_t *addr_out);
#else
#define rpl_data_remember_outer(buf) true
#endif

#endif /* RPL_DATA_H_ */
