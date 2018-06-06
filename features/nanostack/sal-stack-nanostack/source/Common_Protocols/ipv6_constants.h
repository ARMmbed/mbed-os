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

#ifndef IPV6_CONSTANTS_H_
#define IPV6_CONSTANTS_H_

#define IPV6_MIN_LINK_MTU   1280    /* All links can transfer 1280-octet IP packets */
#define IPV6_MIN_FRAG_MRU   1500    /* All hosts can receive 1500-octet fragmented datagrams */

/*
 * IPv6 header offsets
 */
#define IPV6_HDROFF_FLOW_LABEL      1
#define IPV6_HDROFF_PAYLOAD_LENGTH  4
#define IPV6_HDROFF_NH              6
#define IPV6_HDROFF_HOP_LIMIT       7
#define IPV6_HDROFF_SRC_ADDR        8
#define IPV6_HDROFF_DST_ADDR        24
#define IPV6_HDRLEN                 40

/*
 * Standard IPv6 Next Header numbers
 */
#define IPV6_NH_HOP_BY_HOP  0
#define IPV6_NH_TCP         6
#define IPV6_NH_UDP         17
#define IPV6_NH_IPV6        41
#define IPV6_NH_ROUTING     43
#define IPV6_NH_FRAGMENT    44
#define IPV6_NH_AUTH        51
#define IPV6_NH_MOBILITY    55
#define IPV6_NH_ICMPV6      58
#define IPV6_NH_NONE        59
#define IPV6_NH_DEST_OPT    60
#define IPV6_NH_HIP         139
#define IPV6_NH_SHIM6       140

/* Options in IPv6 Destination/Hop-by-Hop Options Headers */
#define IPV6_OPTION_PAD1            0x00 /* (skip, does not change en-route) */
#define IPV6_OPTION_PADN            0x01 /* (skip, does not change en-route) */
#define IPV6_OPTION_ROUTER_ALERT    0x05 /* (skip, does not change en-route) */
#define IPV6_OPTION_RPL             0x63 /* (discard, may change en-route) */
#define IPV6_OPTION_MPL             0x6D /* (discard, may change en-route) */
#define IPV6_OPTION_MPL_EXP         0x7E /* (discard, may change en-route) */

/* Encoded bits in IPv6 Option numbers; action if unrecognised, and mutability */
#define IPV6_OPTION_ACTION_MASK             0xC0
#define IPV6_OPTION_ACTION_SKIP             0x00    /* ignore unrecognised */
#define IPV6_OPTION_ACTION_DISCARD          0x40    /* discard packet */
#define IPV6_OPTION_ACTION_ERROR            0x80    /* discard, and send ICMP Parameter Problem */
#define IPV6_OPTION_ACTION_ERROR_UNICAST    0xC0    /* discard, and send ICMP Parameter Problem if unicast dst */

#define IPV6_OPTION_CHANGE_MASK             0x20
#define IPV6_OPTION_CHANGE_DOES_NOT         0x00    /* does not change en-route */
#define IPV6_OPTION_CHANGE_MAY              0x20    /* may change en-route */

/* Routing Types */
#define IPV6_ROUTING_TYPE_RPL               3

/* Scope values (RFC 7346) */
#define IPV6_SCOPE_INTERFACE_LOCAL          0x1
#define IPV6_SCOPE_LINK_LOCAL               0x2
#define IPV6_SCOPE_REALM_LOCAL              0x3
#define IPV6_SCOPE_ADMIN_LOCAL              0x4
#define IPV6_SCOPE_SITE_LOCAL               0x5
#define IPV6_SCOPE_ORGANIZATION_LOCAL       0x8
#define IPV6_SCOPE_GLOBAL                   0xE

/* Router Alert values (RFC 2711) */
#define IPV6_ROUTER_ALERT_MLD               0



#endif /* IPV6_CONSTANTS_H_ */
