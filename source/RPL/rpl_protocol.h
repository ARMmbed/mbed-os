/*
 * Copyright (c) 2015, 2017, Arm Limited and affiliates.
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

#ifndef RPL_PROTOCOL_H_
#define RPL_PROTOCOL_H_

#include "net_rpl.h" // for RPL_INSTANCE_LOCAL

/* RPL messages are defined as ICMPv6 Codes in Common_Protocols/icmpv6.h */

/* Options in ICMPv6 RPL messages (RFC 6550) */
#define RPL_PAD1_OPTION         0
#define RPL_PADN_OPTION         1
#define RPL_DAG_METRIC_OPTION   2
#define RPL_ROUTE_INFO_OPTION   3
#define RPL_DODAG_CONF_OPTION   4
#define RPL_TARGET_OPTION       5
#define RPL_TRANSIT_OPTION      6
#define RPL_SOL_INFO_OPTION     7
#define RPL_PREFIX_INFO_OPTION  8
#define RPL_TARGET_DESC_OPTION  9
#define RPL_P2P_ROUTE_DESC_OPTION 0x0a

/* Flags in DAO_Transit Information Option */
#define TRANSIT_FLAG_EXTERNAL       0x80

/* Predicates in DIS Solicited Information Option */
#define RPL_SOLINFO_PRED_VERSION        0x80
#define RPL_SOLINFO_PRED_INSTANCEID     0x40
#define RPL_SOLINFO_PRED_DODAGID        0x20

/* Flags in DODAG Configuration */
#define RPL_CONF_FLAG_AUTH      0x08
#define RPL_CONF_PCS_MASK       0x07

/* Flags in DAO Base Object */
#define RPL_DAO_FLAG_ACK_REQ    0x80    /* 'K' */
#define RPL_DAO_FLAG_DODAGID    0x40    /* 'D' */

/* Flags in DAO-ACK Base Object */
#define RPL_DAO_ACK_FLAG_DODAGID 0x80   /* 'D' */

/* RPL Objective Code Points */
#define RPL_OCP_OF0             0       /* RFC 6552 */
#define RPL_OCP_MRHOF           1       /* RFC 6719 */

/* Routing Metric/Constraint Types (RFC 6551) */
#define RPL_MC_NODE_STATE       1
#define RPL_MC_NODE_ENERGY      2
#define RPL_MC_HOP_COUNT        3
#define RPL_MC_LINK_THROUGHPUT  4
#define RPL_MC_LINK_LATENCY     5
#define RPL_MC_LINK_QUALITY     6
#define RPL_MC_LINK_ETX         7
#define RPL_MC_LINK_COLOR       8

/* Flags in IPv6 RPL Option (RFC 6553) */
#define RPL_OPT_DOWN            0x80    /* 'O' */
#define RPL_OPT_RANK_ERROR      0x40    /* 'R' */
#define RPL_OPT_FWD_ERROR       0x20    /* 'F' */

/* Flags in RPL InstanceID */
// RPL_INSTANCE_LOCAL defined in net_rpl.h, as it's part of public API
#define RPL_INSTANCE_DEST       0x40    /* 'D' */

#define rpl_instance_id_is_local(id)  (((id) & RPL_INSTANCE_LOCAL) != 0)
#define rpl_instance_id_is_global(id) (((id) & RPL_INSTANCE_LOCAL) == 0)

#define RPL_RANK_INFINITE       0xffff

extern const uint8_t ADDR_LINK_LOCAL_ALL_RPL_NODES[16];

#endif /* RPL_PROTOCOL_H_ */
