/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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

#ifndef _IPV6_H
#define _IPV6_H

#include "Core/include/ns_buffer.h"
#include "Common_Protocols/ipv6_constants.h"

extern uint16_t ipv6_header_size_required(buffer_t *buf);
extern uint16_t ipv6_max_unfragmented_payload(buffer_t *buf, uint16_t mtu_limit);
extern uint16_t ipv6_mtu(buffer_t *buf);

extern buffer_routing_info_t *ipv6_buffer_route_to(buffer_t *buf, const uint8_t *next_hop, struct protocol_interface_info_entry *next_if);
extern buffer_routing_info_t *ipv6_buffer_route(buffer_t *buf);

typedef enum ipv6_exthdr_stage {
    IPV6_EXTHDR_SIZE,
    IPV6_EXTHDR_INSERT,
    IPV6_EXTHDR_MODIFY
} ipv6_exthdr_stage_t;

#define IPV6_EXTHDR_MODIFY_TUNNEL 1

/* Hooks for routing code to insert/modify IPv6 extension headers.
 *
 * IPV6_EXTHDR_SIZE: buffer not yet fully formed, but we know the final
 * destination. Can perform routing decision. Call should set result to total size
 * of extension header that would be inserted by IPV6_EXTHDR_INSERT.
 *
 * IPV6_EXTHDR_INSERT: we are about to add IP header - provider can
 * prepend extension headers, and modify options.type to new outer NH.
 * (We do not support extension headers from more than one source per packet).
 * These headers can be placeholders, with filling deferred until
 * IPV6_EXTHDR_MODIFY. If a source routing header is inserted, buffer dst_sa
 * should be changed from final destination to next IP destination.
 * XXX no - won't work. Separate mechanism for varying IP dest?
 *
 * IPV6_EXTHDR_MODIFY: the buffer contains an existing complete IP packet,
 * possibly generated internally, and possibly received externally. Provider
 * can update fields in extension headers it knows.
 *
 * If it needs to insert headers that are not present, it can force insertion
 * into a tunnel, by:
 *    setting dst_sa and src_sa appropriate for the tunnel endpoints
 *      XXX does this work out okay with dest cache and PMTU?
 *    updating route info like next hop if necessary (probably not)
 *    returning IPV6_EXTHDR_MODIFY_TUNNEL
 * During new header formation for the tunnel, IPV6_EXTHDR_INSERT will be
 * called again, and obviously it must ensure headers are inserted so that
 * IPV6_EXTHDR_MODIFY doesn't request to tunnel a second time.
 *
 * In all cases, a negative value in result indicates error.
 */
typedef buffer_t *ipv6_exthdr_provider_fn_t(buffer_t *buf, ipv6_exthdr_stage_t stage, int16_t *result);
void ipv6_set_exthdr_provider(ipv6_route_src_t src, ipv6_exthdr_provider_fn_t *fn);

extern buffer_t *ipv6_down(buffer_t *buf);
extern buffer_t *ipv6_forwarding_down(buffer_t *buf);
extern buffer_t *ipv6_forwarding_up(buffer_t *buf);

void ipv6_transmit_multicast_on_interface(buffer_t *buf, struct protocol_interface_info_entry *cur);

#endif /* _IPV6_H */
