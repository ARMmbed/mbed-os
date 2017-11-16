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
#ifndef _ICMPV6_H
#define _ICMPV6_H
#include "Common_Protocols/icmpv6_prefix.h"

#define ICMPV6_TYPE_ERROR_DESTINATION_UNREACH       1
#define ICMPV6_TYPE_ERROR_PACKET_TOO_BIG            2
#define ICMPV6_TYPE_ERROR_TIME_EXCEEDED             3
#define ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM         4

#define ICMPV6_TYPE_INFO_ECHO_REQUEST               128
#define ICMPV6_TYPE_INFO_ECHO_REPLY                 129
#define ICMPV6_TYPE_INFO_MCAST_LIST_QUERY           130
#define ICMPV6_TYPE_INFO_MCAST_LIST_REPORT          131
#define ICMPV6_TYPE_INFO_MCAST_LIST_DONE            132
#define ICMPV6_TYPE_INFO_RS                         133
#define ICMPV6_TYPE_INFO_RA                         134
#define ICMPV6_TYPE_INFO_NS                         135
#define ICMPV6_TYPE_INFO_NA                         136
#define ICMPV6_TYPE_INFO_REDIRECT                   137
#define ICMPV6_TYPE_INFO_MCAST_LIST_REPORT_V2       143
#define ICMPV6_TYPE_INFO_RPL_CONTROL                155
#define ICMPV6_TYPE_INFO_DAR                        157
#define ICMPV6_TYPE_INFO_DAC                        158
#define ICMPV6_TYPE_INFO_MPL_CONTROL                159

#define ICMPV6_CODE_DST_UNREACH_NO_ROUTE            0
#define ICMPV6_CODE_DST_UNREACH_ADM_PROHIB          1
#define ICMPV6_CODE_DST_UNREACH_BEYOND_SCOPE        2
#define ICMPV6_CODE_DST_UNREACH_ADDR_UNREACH        3
#define ICMPV6_CODE_DST_UNREACH_PORT_UNREACH        4
#define ICMPV6_CODE_DST_UNREACH_SRC_FAILED_POLICY   5
#define ICMPV6_CODE_DST_UNREACH_ROUTE_REJECTED      6
#define ICMPV6_CODE_DST_UNREACH_SRC_RTE_HDR_ERR     7

#define ICMPV6_CODE_TME_EXCD_HOP_LIM_EXCD           0
#define ICMPV6_CODE_TME_EXCD_FRG_REASS_TME_EXCD     1

#define ICMPV6_CODE_PARAM_PRB_HDR_ERR               0
#define ICMPV6_CODE_PARAM_PRB_UNREC_NEXT_HDR        1
#define ICMPV6_CODE_PARAM_PRB_UNREC_IPV6_OPT        2
#define ICMPV6_CODE_PARAM_PRB_FIRST_FRAG_IPV6_HDR   3

#define ICMPV6_CODE_RPL_DIS                         0x00
#define ICMPV6_CODE_RPL_DIO                         0x01
#define ICMPV6_CODE_RPL_DAO                         0x02
#define ICMPV6_CODE_RPL_DAO_ACK                     0x03
#define ICMPV6_CODE_RPL_P2P_DRO                     0x04
#define ICMPV6_CODE_RPL_P2P_DRO_ACK                 0x05
#define ICMPV6_CODE_RPL_SECURE_DIS                  0x80
#define ICMPV6_CODE_RPL_SECURE_DIO                  0x81
#define ICMPV6_CODE_RPL_SECURE_DAO                  0x82
#define ICMPV6_CODE_RPL_SECURE_DAO_ACK              0x83
#define ICMPV6_CODE_RPL_SECURE_P2P_DRO              0x84
#define ICMPV6_CODE_RPL_SECURE_P2P_DRO_ACK          0x85
#define ICMPV6_CODE_RPL_CC                          0x8A

/* Options in ICMPv6 Neighbor Discovery Protocol (RPL has totally different options...) */
#define ICMPV6_OPT_SRC_LL_ADDR                      1
#define ICMPV6_OPT_TGT_LL_ADDR                      2
#define ICMPV6_OPT_PREFIX_INFO                      3
#define ICMPV6_OPT_REDIRECTED_HDR                   4
#define ICMPV6_OPT_MTU                              5
#define ICMPV6_OPT_ROUTE_INFO                       24
#define ICMPV6_OPT_ADDR_REGISTRATION                33
#define ICMPV6_OPT_6LOWPAN_CONTEXT                  34
#define ICMPV6_OPT_AUTHORITATIVE_BORDER_RTR         35

/* Neighbour Advertisement flags */
#define NA_R    0x80
#define NA_S    0x40
#define NA_O    0x20

/* Router Advertisement flags */
#define RA_M            0x80    // Managed
#define RA_O            0x40    // Other Configuration
#define RA_H            0x20    // Home Agent (RFC 6275)
#define RA_PRF_MASK     0x18    // Router Preference (RFC 4191)
#define RA_PRF_LOW      0x18    // (RA_PRF_xxx also occurs in Route Info Options)
#define RA_PRF_MEDIUM   0x00
#define RA_PRF_HIGH     0x08
#define RA_PRF_INVALID  0x10

struct buffer;
struct protocol_interface_info_entry;

typedef struct aro {
    uint16_t lifetime;
    uint8_t status;
    uint8_t eui64[8];
    bool present;
} aro_t;

typedef enum slaac_src {
    SLAAC_IID_DEFAULT,      // OPAQUE if key available and enabled on interface, else FIXED
    SLAAC_IID_EUI64,        // use fixed IID based on EUI-64/MAC-64 (iid_eui64)
    SLAAC_IID_FIXED,        // use fixed IID (iid_slaac)
    SLAAC_IID_OPAQUE,       // RFC 7217 opaque
    SLAAC_IID_6LOWPAN_SHORT // use IID based on 6LoWPAN short address
} slaac_src_e;

#define ARO_SUCCESS     0
#define ARO_DUPLICATE   1
#define ARO_FULL        2

extern void icmpv6_init(void);
extern struct buffer *icmpv6_down(struct buffer *buf);
extern struct buffer *icmpv6_up(struct buffer *buf);
extern struct buffer *icmpv6_error(struct buffer *buf, struct protocol_interface_info_entry *cur, uint8_t type, uint8_t code, uint32_t aux);

extern bool icmpv6_options_well_formed(const uint8_t *dptr, uint_fast16_t dlen);
extern bool icmpv6_options_well_formed_in_buffer(const struct buffer *buf, uint16_t offset);
extern const uint8_t *icmpv6_find_option(const uint8_t *dptr, uint_fast16_t dlen, uint8_t option, uint8_t optlen);
extern const uint8_t *icmpv6_find_option_in_buffer(const struct buffer *buf, uint_fast16_t offset, uint8_t option, uint8_t optlen);

struct protocol_interface_info_entry;

extern struct buffer *icmpv6_build_rs(struct protocol_interface_info_entry *cur, const uint8_t *dest_addr);
extern struct buffer *icmpv6_build_ns(struct protocol_interface_info_entry *cur, const uint8_t target_addr[static 16], const uint8_t *prompting_src_addr, bool unicast, bool unspecified_source, const struct aro *aro);
extern struct buffer *icmpv6_build_na(struct protocol_interface_info_entry *cur, bool solicited, bool override, bool tllao_required, const uint8_t target[static 16], const aro_t *aro, const uint8_t src_addr[static 16]);
extern struct buffer *icmpv6_build_dad(struct protocol_interface_info_entry *cur, struct buffer *buf, uint8_t type, const uint8_t dest_addr[16], const uint8_t eui64[8], const uint8_t reg_addr[16], uint8_t status, uint16_t lifetime);
extern void icmpv6_build_echo_req(struct protocol_interface_info_entry *cur, const uint8_t target_addr[16]);

extern void icmpv6_recv_ra_routes(struct protocol_interface_info_entry *cur, bool enable);
extern void icmpv6_recv_ra_prefixes(struct protocol_interface_info_entry *cur, bool enable);

extern void icmpv6_slaac_prefix_register_trig(struct protocol_interface_info_entry *cur, uint8_t *prefix_ptr, uint8_t prefix_len);
extern int icmpv6_slaac_prefix_update(struct protocol_interface_info_entry *cur, uint8_t *prefix_ptr, uint8_t prefix_len, uint32_t valid_lifetime, uint32_t preferred_lifetime);
extern struct if_address_entry *icmpv6_slaac_address_add(struct protocol_interface_info_entry *cur, uint8_t *prefix_ptr, uint8_t prefix_len, uint32_t valid_lifetime, uint32_t preferred_lifetime, bool skip_dad, slaac_src_e slaac_src);


/*
 * Write either an ICMPv6 Prefix Information Option for a Router Advertisement
 * (RFC4861+6275), or an RPL Prefix Information Option (RFC6550).
 * Same payload, different type/len.
 */
uint8_t *icmpv6_write_prefix_option(const prefix_list_t *prefixes,  uint8_t *dptr, uint8_t rpl_prefix, struct protocol_interface_info_entry *cur);
uint8_t *icmpv6_write_mtu_option(uint32_t mtu, uint8_t *dptr);
uint8_t *icmpv6_write_icmp_lla(struct protocol_interface_info_entry *cur, uint8_t *dptr, uint8_t icmp_opt, bool must, const uint8_t *ip_addr);

#endif /* _ICMPV6_H */
