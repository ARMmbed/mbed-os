/*
 * Copyright (c) 2015-2019, Pelion and affiliates.
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


#ifndef THREAD_CONFIG_H_
#define THREAD_CONFIG_H_

/**
 * Thread stack configuration values. All constants that are specified in Thread specification can be found from thread_constants.h
 */

#include "thread_constants.h"

/**
 * Threshold for when the mac frame counter is stored to nvm.
 */
#define MAC_FRAME_COUNTER_LIMIT 1000
/**
 * Threshold for when the mle frame counter is stored to nvm.
 */
#define MLE_FRAME_COUNTER_LIMIT 100

/**
 * Organisational Unique Identifier for ARM.
 */
#define THREAD_ARM_OUI 0x0002f7

/**
 * Build number value of 12 bit.
 */
#define THREAD_BUILD_NUMBER 100

/**
 * Stack revision number 4 bit.
 */
#define THREAD_REVISION_NUMBER 100

/**
 * Stack Major version number 4 bit.
 */
#define THREAD_VERSION_MAJ 1

/**
 * Stack Minor version number 4 bit.
 */
#define THREAD_VERSION_MIN 0

/**
 * Default vendor name.
 */
#define THREAD_VENDOR_NAME "ARM ltd"

/**
 * Default vendor model.
 */
#define THREAD_VENDOR_MODEL "MbedOS"

/**
 * Default SW Version.
 */
#define THREAD_SW_VERSION "1.1"

/**
 * Protocol ID in beacon for thread network discovery.
 */
#define THREAD_PROTOCOL_ID  3

/**
 * Protocol version present in version tlvs and in beacons
 */
#define THREAD_PROTOCOL_VERSION 2

/**
 * Default Maximum child count.
 */
#define THREAD_MAX_CHILD_COUNT 26

/**
 * Default Maximum child id count for router parent.
 */
#define THREAD_MAX_CHILD_ID_COUNT 511

/**
 *  Default Link lifetime for Router and Host.
 */
#define THREAD_DEFAULT_LINK_LIFETIME 100

/**
 * Default network instance weighting (average).
 */
#define THREAD_DEFAULT_WEIGHTING 64

/**
 * Context re use timer in 48 hours in seconds
 */
#define THREAD_CONTEXT_ID_REUSE_TIMEOUT (48 * 3600)

/**
 * Timeout for Leader to set commissioner session to obsolete.
 */
#define COMMISSIONER_OBSOLETE_TIMEOUT (THREAD_COMMISSIONER_KEEP_ALIVE_INTERVAL)

/**
 * Timeout for Leader to remove commissioner session.
 * This is a delay before commissioner is removed after Thread defined keep alive value.
 */
#define COMMISSIONER_REMOVE_TIMEOUT (1000)

/**
 * Router selection jitter
 * default 120
 */
#define THREAD_ROUTER_SELECTION_JITTER 120

/**
 * Maximum parallel Link requests
 * default 4
 */
#define THREAD_MAX_PARALLEL_MLE_LINK_REQUEST 4

/**
 * Number of free MLE entries for parent priority adjustment
 * default 4
 */
#define THREAD_FREE_MLE_ENTRY_THRESHOLD 4

/**
 * Maximum parallel Parent Requests
 * default 8
 */
#define THREAD_MAX_PARALLEL_MLE_PARENT_REQUEST 8

/**
 * Minimum pending delay for commissioner
 * default 30
 */
#define THREAD_DELAY_TIMER_MINIMAL_SECONDS (30)

/**
 * Minimum pending delay for end device
 * default 1800
 */

#define THREAD_DELAY_TIMER_DEFAULT_SECONDS (300)

/**
 * Minimum pending delay for end device
 * default 1800
 */
#define THREAD_MIN_DELAY_TIMER_THREAD_DEVICE_MASTER_KEY_CHANGE_SECONDS (1800)

/**
 * Maximum pending delay timer in seconds
 */
#define THREAD_MAX_DELAY_TIMER_SECONDS (72 * 3600)

/**
 * Maximum neighbor links for reeds and end devices
 */
#define THREAD_REED_AND_END_DEVICE_NEIGHBOR_LINKS 5

/**
 * Maximum data size of commissioner data set parameter - Border agent locator, Commissioner session id and Steering data TLVs are
 * minimum required TLVs at this point
 */
#define THREAD_MAX_COMMISSIONER_DATA_SIZE 50

/**
 * Trace group for THCI traces
 */
#define THCI_TRACE_GROUP "THCI"

#define THREAD_MAX_FRAME_RETRIES 3

/**
 * Thread discovery timeout for discovery response
 */

#define THREAD_DISCOVERY_TIMEOUT 300

#define THREAD_DISCOVERY_MAX_JITTER 250

/**
 *  Interval between increments of ID sequence number by the Leader.
 */
#define ID_SEQUENCE_PERIOD 10 /* 10 second */

/**
 *  Delay before new network data is being updated after update (seconds).
 */
#define THREAD_COMMISSION_DATA_UPDATE_DELAY 1

/**
 * SED configuration (used in Connectivity TLV).
 * -Buffer capacity in octets for all IPv6 datagrams destined to a given SED
 * -Queue capacity in IPv6 datagrams destined to a given SED
 */
#define THREAD_SED_BUFFER_SIZE     THREAD_SED_BUFFER_MIN_SIZE
#define THREAD_SED_DATAGRAM_COUNT  THREAD_SED_DATAGRAM_MIN_COUNT

#define THREAD_DISCOVERY_SECURITY_KEY_INDEX 0xff
#define THREAD_DISCOVERY_SECURITY_KEY_SOURCE 0xffffffff

/**
 * The threshold value that must be exceed by a packet to be
 * considered big. Thread conformance document specifies the
 * threshold as 106.
 */
#define THREAD_INDIRECT_BIG_PACKET_THRESHOLD 106

/**
 * Thread conformance document says: "A Thread Router MUST be able
 * to buffer at least one (1) 1280-octet IPv6 datagram destined for
 * an attached SED" and "A Thread Router MUST be able to buffer at
 * least one (1) 106-octet IPv6 datagram per attached SED".
 *
 * The defines below tell how many small (i.e. up to the big packet
 * threshold) packets per sleepy child and big (i.e. over the big
 * packet threshold) packets total we buffer in the indirect TX
 * queue. The minimum values are 1 for both, but here we use larger
 * value for better performance.
 */
#define THREAD_INDIRECT_BIG_PACKETS_TOTAL 10
#define THREAD_INDIRECT_SMALL_PACKETS_PER_CHILD 3

/**
 * Maximum number of MTD children, default 16
 * Thread conformance minimum is 10
 */
#define THREAD_MAX_MTD_CHILDREN 16

/**
 * Maximum number of sleepy children, default 10
 * Thread conformance minimum is 6
 */
#define THREAD_MAX_SED_CHILDREN 10

/**
 * Base value for blacklist interval value
 */
#define THREAD_BLACKLIST_TIMER_TIMEOUT 4

/**
 * Maximum value for blacklist interval value
 */
#define THREAD_BLACKLIST_TIMER_MAX_TIMEOUT 500

/**
 * Base value for blacklist entry lifetime
 */
#define THREAD_BLACKLIST_ENTRY_LIFETIME 8

/**
 * Maximum number of entries in the blacklist table
 */
#define THREAD_BLACKLIST_ENTRY_MAX_NBR 15

/**
 * Number of extra entries to purge from the blacklist table
 */
#define THREAD_BLACKLIST_PURGE_NBR 3

/**
 * Timeout value for purging extra blacklist entries
 */
#define THREAD_BLACKLIST_PURGE_TIMER_TIMEOUT 6

/**
 * Base value for commission blacklist interval value
 */
#define THREAD_COMM_BLACKLIST_TIMER_TIMEOUT 60

/**
 * Maximum value for commission blacklist interval value
 */
#define THREAD_COMM_BLACKLIST_TIMER_MAX_TIMEOUT 600

/**
 * Base value for commission blacklist entry lifetime
 */
#define THREAD_COMM_BLACKLIST_ENTRY_LIFETIME 600

/**
 * Delay value for sending proactive address notification after router upgrade
 */
#define THREAD_PROACTIVE_AN_SEND_DELAY 2

/*
 * Parent response wait time (in 100ms) when "R" bit is set in scan mask TLV (rounded up from  0.75 seconds)
 */
#define THREAD_PARENT_REQ_SCANMASK_R_TIMEOUT 9

/*
 * Parent response wait time (in 100ms) when both "R" and "E" bit is set in scan mask TLV (rounded up from 1.25 seconds)
 */
#define THREAD_PARENT_REQ_SCANMASK_RE_TIMEOUT 15

/*
 * When BBR is started, router address is requested from leader with following status
 */
#define THREAD_BBR_ROUTER_ID_REQUEST_STATUS THREAD_COAP_STATUS_TLV_HAVE_CHILD_ID_REQUEST

/* Border Router IPv6 neighbour and destination cache configuration
 * Number of neighbor cache entries assuming 250 thread devices (worst case) connecting to cloud service.
 * Six entries reserved for backbone devices.
 */
#define THREAD_BBR_IPV6_NEIGHBOUR_CACHE_SIZE                256
#define THREAD_BBR_IPV6_NEIGHBOUR_CACHE_SHORT_TERM          128
#define THREAD_BBR_IPV6_NEIGHBOUR_CACHE_LONG_TERM           32
#define THREAD_BBR_IPV6_NEIGHBOUR_CACHE_LIFETIME            600

/* Router IPv6 neighbour and destination cache configuration */
#define THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_SIZE             128
#define THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_SHORT_TERM       64
#define THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_LONG_TERM        8
#define THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_LIFETIME         600
#define THREAD_ROUTER_IPV6_DESTINATION_CACHE_SIZE           32
#define THREAD_ROUTER_IPV6_DESTINATION_CACHE_SHORT_TERM     16
#define THREAD_ROUTER_IPV6_DESTINATION_CACHE_LONG_TERM      4
#define THREAD_ROUTER_IPV6_DESTINATION_CACHE_LIFETIME       600

/* End device IPv6 neighbour and destination cache configuration */
#define THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_SIZE         32
#define THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_SHORT_TERM   16
#define THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_LONG_TERM    4
#define THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_LIFETIME     600
#define THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_SIZE       16
#define THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_SHORT_TERM 8
#define THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_LONG_TERM  4
#define THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_LIFETIME   600

/*
 * Timeout to solicit address from DHCP if previous request fails.
 */
#define THREAD_MAINTENANCE_TIMER_INTERVAL 300

/**
 * Build time flag to enable THCI special traces for test harness purposes
 */
#ifdef THREAD_THCI_SUPPORT
#define thci_trace(...) mbed_tracef(TRACE_LEVEL_INFO, THCI_TRACE_GROUP, __VA_ARGS__ );
#else
#define thci_trace(...)
#endif

#endif /* THREAD_CONFIG_H_ */
