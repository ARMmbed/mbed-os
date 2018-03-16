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

#ifndef THREAD_DISCOVERY_H_
#define THREAD_DISCOVERY_H_

struct protocol_interface_info_entry;

typedef struct discovery_response_list {
    uint8_t reserved_opaque[21];
    uint8_t network_name[16];
    uint8_t steering_data[16];
    uint8_t extented_pan_id[8];
    uint8_t extented_mac[8];
    uint16_t pan_id;
    uint16_t joiner_port;
    uint16_t commissioner_port;
    uint8_t channel;
    int8_t dbm;
    unsigned version:4;
    unsigned steering_data_valid:5;

    ns_list_link_t link;
} discovery_response_list_t;


typedef struct {
    uint64_t active_timestamp;
    uint16_t pan_id;
    uint8_t  channel;
} announce_discovery_response_t;

typedef NS_LIST_HEAD(discovery_response_list_t, link) thread_nwk_discovery_response_list_t;

typedef struct {
    uint8_t *extented_pan_id;
    uint8_t *network_name;
    uint16_t pan_id;
    unsigned version:4;
} discover_server_info_t;

/**
 * Thread network discovery
 */
typedef struct {
    uint32_t channel_mask;
    bool joiner_flag:1;
    bool native_commisioner; //Set for native commisioner
    uint8_t filter_tlv_length; //Optional Filter data length
    uint8_t *filter_tlv_data; //Pointer for filter specific data
} thread_discover_reques_t;

/**
 * Thread network discovery
 */
typedef struct {
    uint64_t active_timestamp;
    uint32_t channel_mask;
    uint16_t pan_id;
} thread_announce_discover_reques_t;


/**
 *  Thread discovery ready callback
 *
 *
 *  \param cur_interface Interface pointer
 *  \param discover_response discovery response list (linked list)
 */
typedef void (thread_discovery_ready_cb)(struct protocol_interface_info_entry *cur_interface, thread_nwk_discovery_response_list_t *discover_response);

/**
 *  Thread Orphan nwk discovery ready callback
 *
 *
 *  \param cur_interface Interface pointer
 *  \param nwk discovery response list (linked list)
 */
typedef void (thread_announce_scan_ready_cb)(struct protocol_interface_info_entry *cur_interface, announce_discovery_response_t *discover_response);

/**
 * Init discovery service
 */
int thread_discovery_init(int8_t interface_id, struct protocol_interface_info_entry *cur_interface, uint8_t version, bool reedDevice);

/**
 * Reset discovery class state to idle
 */
int thread_discovery_reset(int8_t interface_id);

/**
 * Enable thread discovery request response support
 */
int thread_discovery_responser_enable(int8_t interface_id, bool enable_service);

/**
 * Start Thread network discovery
 */
int thread_discovery_network_scan(int8_t interface_id, thread_discover_reques_t *scan_request, thread_discovery_ready_cb *ready_cb);

/**
 * Start device orphan scan
 */
int thread_discovery_announce_network_scan(int8_t interface_id, thread_announce_discover_reques_t *scan_request, thread_announce_scan_ready_cb *ready_cb);

/**
 * Get primary network from discovered list
 */
discovery_response_list_t *thread_discovery_network_description_get(int8_t interface_id);

#if 0
/**
 * Free all allocated memory and free class
 * Not used API function, flagged away. This should be taken in use when refactoring ifdown - functionality.
 */
int thread_discovery_free(int8_t interface_id);
#endif
#endif /* THREAD_DISCOVERY_H_ */
