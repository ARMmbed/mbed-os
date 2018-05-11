/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THREAD_CONSTANTS_H_
#define THREAD_CONSTANTS_H_

/**
 * Constants defined in Thread specifications. All configured values are defined in thread_config.h
 */

/**
 * Default management port assigned by IANA.
 * nick:MM
 */
#define THREAD_MANAGEMENT_PORT 61631

/**
 * Default joiner port. Joiner must learn this from Discovery response during network discovery.
 * nick:MJ
 */
#define THREAD_DEFAULT_JOINER_PORT 49153

/**
 * Default Commissioning port. This can be changed if this is not available and advertised in network discovery.
 * nick: MC
 */
#define THREAD_COMMISSIONING_PORT 49191

/**
 *  End device default timeout.
 */
#define THREAD_END_DEVICE_DEFAULT_TIMEOUT 240

/**
 *  COAP Status TLV values (TLV TYPE 4)
 */
#define THREAD_COAP_STATUS_TLV_SUCCESS                  0
#define THREAD_COAP_STATUS_TLV_NO_ADDRESS_AVAILABLE     1
#define THREAD_COAP_STATUS_TLV_TOO_FEW_ROUTERS          2
#define THREAD_COAP_STATUS_TLV_HAVE_CHILD_ID_REQUEST    3
#define THREAD_COAP_STATUS_TLV_PARENT_PARTITION_CHANGE  4


/**
 * Constants for Connectivity TLV (TLV TYPE 15)
 */
#define THREAD_CONNECTIVITY_TLV_PARENT_PRIORITY_HIGH     1
#define THREAD_CONNECTIVITY_TLV_PARENT_PRIORITY_MEDIUM   0
#define THREAD_CONNECTIVITY_TLV_PARENT_PRIORITY_LOW      -1

/**
 * per-SED IPv6 buffer sizes and datagram count specified in the Thread Conformance specification
 */
#define THREAD_SED_BUFFER_MIN_SIZE          106
#define THREAD_SED_DATAGRAM_MIN_COUNT       1


/**
 * Thread TLV length
 */
#define THREAD_HAS_ROUTE_TLV_LENGTH         3
#define THREAD_BORDER_ROUTER_TLV_LENGTH     4
#define THREAD_6LOWPAN_ID_TLV_LENGTH        2
#define THREAD_IPV6_ADDRESS_TLV_LENGTH      16

/**
 * Uris defined by thread specifications
 */
#define THREAD_URI_NETWORK_DATA                 "a/sd"   //<* Leader
#define THREAD_URI_NEIGHBOR_DISCOVERY_DATA_REQ  "a/nd"   //<* 1.1 feature

#define THREAD_URI_ROUTER_ID_ASSIGNMENT         "a/as"   //<* Leader
#define THREAD_URI_ROUTER_ID_RELEASE            "a/ar"   //<* Leader

#define THREAD_URI_ADDRESS_QUERY_REQUEST        "a/aq"   //<* All Routers
#define THREAD_URI_ADDRESS_NOTIFICATION         "a/an"   //<* All Routers
#define THREAD_URI_ADDRESS_ERROR                "a/ae"   //<* All Nodes

#define THREAD_URI_DIAGNOSTIC_RESET             "d/dr"   //<* All Nodes
#define THREAD_URI_DIAGNOSTIC_REQUEST           "d/dg"   //<* All Nodes
#define THREAD_URI_DIAGNOSTIC_QUERY             "d/dq"   //<* All Routers
#define THREAD_URI_DIAGNOSTIC_ANSWER            "d/da"   //<* All Routers


#define THREAD_URI_MANAGEMENT_GET               "c/mg"  //<* Leader
#define THREAD_URI_MANAGEMENT_SET               "c/ms"  //<* Leader
#define THREAD_URI_LEADER_PETITION              "c/lp"  //<* Leader
#define THREAD_URI_LEADER_KEEP_ALIVE            "c/la"  //<* Leader

#define THREAD_URI_RELAY_TRANSMIT               "c/tx"  //<* Joiner router
#define THREAD_URI_RELAY_RECEIVE                "c/rx"  //<* Border router, native commissioner

#define THREAD_URI_JOINER_ENTRUST               "c/je"  //<* Joiner
#define THREAD_URI_JOINER_FINALIZATION          "c/jf"  //<* Commissioner, native commissioner
#define THREAD_URI_JOINER_APPLICATION_REQUEST   "c/ja"  //<* Any thread device

#define THREAD_URI_COMMISSIONER_PETITION        "c/cp"  //<* Border router
#define THREAD_URI_COMMISSIONER_KEEP_ALIVE      "c/ca"  //<* Border router

#define THREAD_URI_UDP_RECVEIVE_NOTIFICATION    "c/ur"  //<* Commissioner
#define THREAD_URI_UDP_TRANSMIT_NOTIFICATION    "c/ut"  //<* Border router

#define THREAD_URI_PANID_QUERY                  "c/pq"  //<* 1.1 feature
#define THREAD_URI_PANID_CONFLICT               "c/pc"  //<* 1.1 feature
#define THREAD_URI_ED_SCAN                      "c/es"  //<* 1.1 feature
#define THREAD_URI_ED_REPORT                    "c/er"  //<* 1.1 feature
#define THREAD_URI_MANAGEMENT_ANNOUNCE_NTF      "c/an"  //<* 1.1 feature
#define THREAD_URI_MANAGEMENT_ANNOUNCE_BEGIN    "c/ab"  //<* 1.1 feature

#define THREAD_URI_ACTIVE_GET                   "c/ag"  //<* 1.1 feature
#define THREAD_URI_ACTIVE_SET                   "c/as"  //<* 1.1 feature
#define THREAD_URI_PENDING_GET                  "c/pg"  //<* 1.1 feature
#define THREAD_URI_PENDING_SET                  "c/ps"  //<* 1.1 feature
#define THREAD_URI_COMMISSIONER_GET             "c/cg"  //<* 1.1 feature
#define THREAD_URI_COMMISSIONER_SET             "c/cs"  //<* 1.1 feature
#define THREAD_URI_DATASET_CHANGED              "c/dc"  //<* 1.1 feature

#define THREAD_DEFAULT_COAP_RESOLUTION 1
#define THREAD_DEFAULT_KEY_SWITCH_GUARD_TIME 624 // Hours
#define THREAD_DEFAULT_KEY_ROTATION 672 // Hours
#define THREAD_COMMISSIONER_KEEP_ALIVE_INTERVAL 50000    // Default thread commissioner keep-alive message interval (milliseconds)

#define THREAD_FAILED_CHILD_TRANSMISSIONS 4
#define THREAD_FAILED_ROUTER_TRANSMISSIONS 4
#define THREAD_MAC_TRANSMISSIONS 4

#define THREAD_MAX_NETWORK_DATA_SIZE 254

// How many seconds a server MUST wait after a data update has been rejected by the Leader before resending the update
#define THREAD_DATA_RESUBMIT_DELAY  300

// The maximum length of Commissioning Data TLV (including type & length)
//   - Commissioner Session ID, Border Agent Locator TLV, Steering Data TLV
#define THREAD_MAX_COMM_DATA_TLV_LEN 28

// Channel page 0 super frame duration is used to convert times from ms time to exponent value in MAC
#define CHANNEL_PAGE_0_SUPERFRAME_DURATION  15.36

#define THREAD_ENTERPRISE_NUMBER 44970

#define THREAD_ADDR_REG_RETRY_INTERVAL 300
#define THREAD_PROACTIVE_AN_INTERVAL 3600

// Router defines
#define ROUTER_SELECTION_JITTER 120
#define REED_ADVERTISEMENT_INTERVAL 570
#define REED_ADVERTISEMENT_MAX_JITTER 60
#define MIN_DOWNGRADE_NEIGHBORS 7
#define THREAD_REED_ADVERTISEMENT_DELAY 5000

// Interval after which REED can send an advertisement to help others merge to higher partition.
// This is not related to default REED advertisements.
#define THREAD_REED_MERGE_ADVERTISEMENT_INTERVAL 120

/** Default Threshold for router Selection */
#define ROUTER_DOWNGRADE_THRESHOLD 23 // Define downGrade Threshold when active router is higher than this
#define ROUTER_UPGRADE_THRESHOLD 16 // Define upgrade Threshold fort REED when Active Router Count is smaller than this upgrade is possible

/** Thread prefix minimum lifetime in seconds */
#define THREAD_MIN_PREFIX_LIFETIME  3600

#endif /* THREAD_CONSTANTS_H_ */
