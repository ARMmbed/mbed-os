/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include "nsconfig.h"
#ifdef HAVE_THREAD
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "thread_config.h"
#include "common_functions.h"
#include <string.h>
#include "ns_trace.h"
#include "ns_list.h"
#include "randLIB.h"
#include <nsdynmemLIB.h>
#include "6LoWPAN/Thread/thread_discovery.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_tmfcop_lib.h"
#include "thread_meshcop_lib.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"

typedef enum {
    THREAD_DISCOVER_INIT = 0,
    THREAD_DISCOVER_TIMER
} thread_discover_event_id_e;

#define TRACE_GROUP "tdis"

//Discovery request class
typedef struct {
    uint16_t active_timer;
    uint32_t channel_mask;
    uint16_t random_panid;
    uint8_t temporary_mac64[8];
    thread_nwk_discovery_response_list_t network_list;
    thread_discovery_ready_cb *response_cb;
    uint8_t active_channel;
    uint8_t channel_page;
    bool waiting_response:1;
    bool joiner_flag:1;
    bool native_commisioner_scan;
    uint8_t filter_tlv_length; //Optional Filter data length
    uint8_t filter_tlv_data[]; //Do not anything after this definition
} thread_discovery_request_info_t;

typedef struct {
    uint8_t extentedAddress[8];
    uint8_t timer;
    bool randomJitter;
    ns_list_link_t link;
} thread_discovery_response_msg_t;

typedef NS_LIST_HEAD(thread_discovery_response_msg_t, link) thread_discovery_response_msg_list;

typedef struct {
    struct protocol_interface_info_entry *interface;
    thread_discovery_request_info_t *discovery_request;
    thread_discovery_response_msg_list srv_respose_msg_list;
    thread_discovery_response_msg_list srv_respose_msg_buffers;
    thread_discovery_response_msg_t *msg_buffers;
    thread_nwk_discovery_response_list_t discovered_network;
    int8_t interface_id;
    uint8_t version;
    bool discovery_server_active;
    ns_list_link_t link;
} thread_discovery_class_t;

#define THREAD_DISCOVER_TIMER_PERIOD 50

int thread_discovery_init(int8_t interface_id, struct protocol_interface_info_entry *cur_interface, uint8_t version, bool reedDevice)
{
    return 0;
}

/**
 * Free all allocated memory and free class
 */
int thread_discovery_free(int8_t interface_id)
{
    return 0;
}

/**
 * Reset discovery class state to idle
 */
int thread_discovery_reset(int8_t interface_id)
{
    return 0;
}

/**
 * Enable thread discovery request response support
 */
int thread_discovery_responser_enable(int8_t interface_id, bool enable_service)
{
    return 0;
}

/**
 * Start Thread network discovery
 */
int thread_discovery_network_scan(int8_t interface_id, thread_discover_reques_t *scan_request, thread_discovery_ready_cb *ready_cb)
{
    return 0;
}

discovery_response_list_t *thread_discovery_network_description_get(int8_t interface_id)
{
    return NULL;
}

int thread_discovery_announce_network_scan(int8_t interface_id, thread_announce_discover_reques_t *scan_request, thread_announce_scan_ready_cb *ready_cb)
{
    return 0;
}

#endif
