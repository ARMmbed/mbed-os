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
#include "thread_discovery.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_extension.h"
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

typedef struct {
    uint8_t type;
    uint8_t *data;
    uint16_t length;
}mescop_tlv_t;

#define TRACE_GROUP "tdis"

static int8_t thread_discover_tasklet_id = -1;
static bool thread_discover_timer_active = false;

#define discover_optional_start_pointer(x)  (&(x)->filter_tlv_data[0])

//Discovery request class
typedef struct {
    uint16_t active_timer;
    uint32_t channel_mask;
    uint16_t random_panid;
    uint8_t temporary_mac64[8];
    thread_discovery_ready_cb *response_cb;
    uint8_t active_channel;
    uint8_t channel_page;
    bool waiting_response:1;
    bool joiner_flag:1;
    bool native_commisioner_scan:1;
    uint8_t filter_tlv_length; //Optional Filter data length
    uint8_t filter_tlv_data[]; //Do not anything after this definition
} thread_discovery_request_info_t;


//Discovery request class
typedef struct {
    uint16_t active_timer;
    uint32_t channel_mask;
    uint8_t active_channel;
    uint16_t pan_id;
    uint64_t active_time_stamp;
    thread_announce_scan_ready_cb *response_cb;
    announce_discovery_response_t *network;
    bool waiting_response:1;
} thread_announce_request_info_t;


typedef struct {
    uint8_t extentedAddress[8];
    uint16_t panId;
    uint8_t timer;
    bool randomJitter;
    ns_list_link_t link;
} thread_discovery_response_msg_t;

typedef NS_LIST_HEAD(thread_discovery_response_msg_t, link) thread_discovery_response_msg_list;

typedef struct {
    struct protocol_interface_info_entry *interface;
    thread_discovery_request_info_t *discovery_request;
    thread_announce_request_info_t *thread_announce_request;
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


static NS_LIST_DEFINE(thread_discovery_class_list, thread_discovery_class_t, link);

static bool thread_discovery_timer_update(void);
static void thread_discover_timer_trig(void);
static discovery_response_list_t *thread_discover_response_msg_allocate(void);
static discovery_response_list_t *thread_discover_response_msg_get_discover_from_list(thread_nwk_discovery_response_list_t *list, uint8_t channel, uint16_t panId);


static int stringlen(const char *s, int n)
{
    char *end = memchr(s, 0, n);
    return end?end-s:n;
}

static void thread_discover_timer_trig(void)
{
    if (thread_discover_timer_active || thread_discover_tasklet_id == -1) {

    }
    eventOS_event_timer_request(3, THREAD_DISCOVER_TIMER, thread_discover_tasklet_id, THREAD_DISCOVER_TIMER_PERIOD);
    thread_discover_timer_active = true;
}

static thread_discovery_request_info_t * thread_discovery_request_allocate(thread_discover_reques_t *scan_request, thread_discovery_ready_cb *response_cb)
{
    thread_discovery_request_info_t * discover_request = ns_dyn_mem_temporary_alloc(sizeof(thread_discovery_request_info_t) + scan_request->filter_tlv_length);
    if (discover_request) {
        discover_request->waiting_response = false;
        discover_request->active_timer = 0;
        discover_request->channel_mask = scan_request->channel_mask;
        discover_request->joiner_flag = scan_request->joiner_flag;
        discover_request->native_commisioner_scan = scan_request->native_commisioner;
        discover_request->filter_tlv_length = scan_request->filter_tlv_length;
        discover_request->random_panid = randLIB_get_random_in_range(1, 0xfffd); //Generate random pan-id
        randLIB_get_n_bytes_random(discover_request->temporary_mac64, 8); //Generate random temporary mac64

        discover_request->response_cb = response_cb;
        if (discover_request->filter_tlv_length) {
            memcpy(discover_optional_start_pointer(discover_request), scan_request->filter_tlv_data, discover_request->filter_tlv_length);
        }
    }
    return discover_request;
}


static thread_announce_request_info_t * thread_announce_discovery_request_allocate(thread_announce_discover_reques_t *scan_request, thread_announce_scan_ready_cb *response_cb)
{
    thread_announce_request_info_t * discover_request = ns_dyn_mem_temporary_alloc(sizeof(thread_announce_request_info_t));
    if (discover_request) {
        discover_request->waiting_response = false;
        discover_request->active_timer = 0;
        discover_request->channel_mask = scan_request->channel_mask;
        discover_request->pan_id = scan_request->pan_id;
        discover_request->active_time_stamp = scan_request->active_timestamp;
        discover_request->response_cb = response_cb;
        discover_request->network = NULL;
    }
    return discover_request;
}

static void thread_discovery_server_msg_clean(thread_discovery_class_t *class)
{
    class->discovery_server_active = false;
    ns_list_foreach_safe(thread_discovery_response_msg_t, cur, &class->srv_respose_msg_list) {
        ns_list_remove(&class->srv_respose_msg_list, cur);
        ns_list_add_to_start(&class->srv_respose_msg_buffers, cur);
    }
}

static bool thread_discovery_server_msg_buffer_allocate(thread_discovery_class_t *class)
{
    thread_discovery_response_msg_t *msg_buffer = ns_dyn_mem_alloc(sizeof(thread_discovery_class_t) * 4);
    if (!msg_buffer) {
        return false;
    }
    class->msg_buffers = msg_buffer;
    for (uint8_t i = 0; i< 4; i++) {
        ns_list_add_to_start(&class->srv_respose_msg_buffers, msg_buffer++);
    }
    return true;
}

static void thread_discovery_server_msg_buffer_free(thread_discovery_class_t *class)
{
    thread_discovery_server_msg_clean(class);
    ns_dyn_mem_free(class->msg_buffers);
    class->msg_buffers = NULL;
}

static bool thread_discovery_response_pending_at_list(thread_discovery_response_msg_list *list, uint8_t *extended_address)
{
    ns_list_foreach(thread_discovery_response_msg_t, cur, list) {
        if (memcmp(cur->extentedAddress, extended_address, 8) == 0) {
            return true;
        }
    }
    return false;
}

static thread_discovery_response_msg_t *thread_discovery_response_allocate(thread_discovery_response_msg_list *list)
{
    thread_discovery_response_msg_t *entry = ns_list_get_first(list);
    if (entry) {
        //Remove from the list
        ns_list_remove(list, entry);
        // -1 because timer accuracy is 50ms and message sending must happen before 300ms
        entry->timer = randLIB_get_random_in_range(1, THREAD_DISCOVERY_MAX_JITTER / THREAD_DISCOVER_TIMER_PERIOD - 1);
        entry->randomJitter = true;
    }

    return entry;
}

static void thread_discovery_response_trig(thread_discovery_class_t *class, uint8_t *ll64, uint16_t dstPanId)
{
    //Start DoS verify
    if (thread_discovery_response_pending_at_list(&class->srv_respose_msg_list, ll64 + 8)) {
        tr_debug("Too Agressive Discovery");
        return;
    }

    //Allocate new response
    thread_discovery_response_msg_t *entry = thread_discovery_response_allocate(&class->srv_respose_msg_buffers);
    if (!entry) {
        tr_debug("Too much discovery");
        return;
    }
    //end DoS verify
    /* Add response messaged trigger to list */
    memcpy(entry->extentedAddress, ll64 + 8, 8);
    entry->panId = dstPanId;
    //Add to list
    ns_list_add_to_end(&class->srv_respose_msg_list, entry);
    thread_discover_timer_trig();

}

static discovery_response_list_t *thread_discover_response_msg_get_discover_from_list(thread_nwk_discovery_response_list_t *list, uint8_t channel, uint16_t panId)
{
    //Get last
    discovery_response_list_t *entry = ns_list_get_last(list);
    while (entry) {
        if (entry->channel == channel && entry->pan_id == panId) {
            return entry;
        }
        entry = ns_list_get_previous(list, entry);

    }
    return NULL;
}

static discovery_response_list_t *thread_discover_response_msg_allocate(void)
{
    discovery_response_list_t *msg = ns_dyn_mem_temporary_alloc(sizeof(discovery_response_list_t));
    if (msg) {
        memset(msg, 0, sizeof(discovery_response_list_t));
    }
    return msg;
}

static void thread_discovery_request_free(thread_discovery_class_t *class)
{
    if (!class->discovery_request) {
        return;
    }

    ns_dyn_mem_free(class->discovery_request);
    class->discovery_request = NULL;

}

static void thread_announce_discovery_request_free(thread_discovery_class_t *class)
{
    if (!class->thread_announce_request) {
        return;
    }
    ns_dyn_mem_free(class->thread_announce_request->network);
    ns_dyn_mem_free(class->thread_announce_request);
    class->thread_announce_request = NULL;

}

static void thread_discovery_prepare(protocol_interface_info_entry_t *interface, thread_discovery_request_info_t *discovery)
{
    mac_helper_default_security_level_set(interface, SEC_ENC_MIC32);
    mac_helper_beacon_payload_reallocate(interface, 0); // No beacons for thread
    mac_data_poll_init(interface);
    mac_helper_mac16_address_set(interface, 0xffff);
    mac_helper_mac64_set(interface,discovery->temporary_mac64);
    thread_set_link_local_address(interface); // only to generate IID
    discovery->active_timer = 1;
    discovery->waiting_response = false;
    thread_discover_timer_trig();
}

static void thread_announce_discovery_prepare(protocol_interface_info_entry_t *interface, thread_announce_request_info_t *discovery)
{
    mac_helper_default_security_level_set(interface, SEC_ENC_MIC32);
    if (thread_info(interface)->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
        thread_end_device_mode_set(interface, false);
    }
    mac_data_poll_init(interface);
    mac_helper_mac16_address_set(interface, 0xffff);
    discovery->active_timer = 1;
    discovery->waiting_response = false;
    thread_discover_timer_trig();
}

static bool thread_discovery_proces_ready(thread_discovery_request_info_t *discovery) {

    //Get next free channel
    uint8_t i;
    uint32_t mask = 1;

    for (i=0; i<32; i++)
    {
        if (discovery->channel_mask & mask)
        {
            discovery->channel_mask &= ~mask;
            discovery->active_channel = i;
            discovery->channel_page = 0; //Support only chnnel page 0
            return false;
        }
        mask <<= 1;
    }
    return true;
}

static bool thread_announce_discovery_process_ready(thread_announce_request_info_t *discovery) {

    //Get next free channel
    uint8_t i;
    uint32_t mask = 0x80000000;

    for (i=0; i<32; i++)
    {
        if (discovery->channel_mask & mask)
        {
            discovery->channel_mask &= ~mask;
            discovery->active_channel = i;
            return false;
        }
        mask >>= 1;
    }
    return true;
}

static void thread_discovery_process_end(protocol_interface_info_entry_t *interface)
{
    mlme_reset_t reset;
    reset.SetDefaultPIB = true;
    interface->mac_api->mlme_req(interface->mac_api, MLME_RESET, &reset);
}

/**
 * Activate mac to selected channel and pan-id
 */
static void thread_discovery_link_activate(protocol_interface_info_entry_t *interface, uint16_t pan_id, uint8_t channel, uint8_t channel_page)
{
    mlme_start_t start_req;
    memset(&start_req, 0, sizeof(mlme_start_t));

    interface->mac_parameters->pan_id = pan_id;
    interface->mac_parameters->mac_channel = channel;

    start_req.PANId = pan_id;
    start_req.LogicalChannel = channel;
    start_req.ChannelPage = channel_page;
    start_req.BeaconOrder = 0x0f;
    start_req.SuperframeOrder = 0x0f;

    if( interface->mac_api ){
        interface->mac_api->mlme_req(interface->mac_api, MLME_START, (void*)&start_req);
    }
}

static uint16_t thread_discover_tlv_get(uint8_t version, bool dynamic_bit)
{
    uint16_t thread_discover_tlv = 0;

    thread_discover_tlv |= (uint16_t) (version << 12);

    if (dynamic_bit) {
        thread_discover_tlv |= (uint16_t) (1 << 11);
    }

    return thread_discover_tlv;
}


static int thread_discovery_request_send(thread_discovery_class_t *class, thread_discovery_request_info_t *discovery)
{
    protocol_interface_info_entry_t *cur = class->interface;

    uint16_t buf_id = mle_service_msg_allocate(cur->id, 4 + discovery->filter_tlv_length + 2, false, MLE_COMMAND_DISCOVERY_REQUEST);
    if (buf_id == 0) {
        return -1;
    }

    mle_service_set_msg_destination_address(buf_id, ADDR_LINK_LOCAL_ALL_NODES);
    mle_service_msg_update_security_params(buf_id, 0, 0, 0);
    //Enable link layer security
    mle_service_set_msg_link_layer_security_mode(buf_id, true);
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);
    uint16_t discover_request_tlv = thread_discover_tlv_get(class->version, discovery->joiner_flag);

    *ptr++ = MLE_TYPE_DISCOVERY;
    *ptr++ = 4 + discovery->filter_tlv_length;

    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_DISCOVERY_REQUEST, discover_request_tlv);
    if (discovery->filter_tlv_length) {
        memcpy(ptr, discover_optional_start_pointer(discovery), discovery->filter_tlv_length);
        ptr += discovery->filter_tlv_length;
    }

    if (mle_service_update_length_by_ptr(buf_id,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_message_timeout_params_t timeout;
    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_set_msg_panid(buf_id, 0xffff);
    mle_service_send_message(buf_id);
    discovery->waiting_response = true;
    return 0;
}

static int thread_discovery_announce_request_send(thread_discovery_class_t *class, thread_announce_request_info_t *discovery)
{
    protocol_interface_info_entry_t *cur = class->interface;


    uint16_t buf_id = mle_service_msg_allocate(cur->id, 19, false, MLE_COMMAND_DATASET_ANNOUNCE);
    if (buf_id == 0) {
        return -1;
    }

    mle_service_set_msg_destination_address(buf_id, ADDR_LINK_LOCAL_ALL_NODES);
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    mle_service_set_msg_link_layer_security_mode(buf_id, true);
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);

    uint8_t channel_tlv[3];
    ptr = thread_meshcop_tlv_data_write_uint64(ptr,MLE_TYPE_ACTIVE_TIMESTAMP, discovery->active_time_stamp);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr,MLE_TYPE_PANID, discovery->pan_id);
    channel_tlv[0] = 0;
    common_write_16_bit(discovery->active_channel, &channel_tlv[1]);
    ptr = thread_meshcop_tlv_data_write(ptr,MLE_TYPE_CHANNEL, 3, channel_tlv);



    if (mle_service_update_length_by_ptr(buf_id,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_message_timeout_params_t timeout;
    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_set_msg_panid(buf_id, 0xffff);
    mle_service_send_message(buf_id);
    discovery->waiting_response = true;
    return 0;
}




static int thread_discovery_response_send(thread_discovery_class_t *class, thread_discovery_response_msg_t *msg_buffers)
{
    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(class->interface_id);
    if (!linkConfiguration) {
        return -1;
    }
    thread_management_server_data_t server_data;
    if (thread_management_server_commisoner_data_get(class->interface_id, &server_data) != 0) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = class->interface;

    // Calculate length
    uint16_t message_length = 16; // Default data to response always
    message_length += stringlen((char*)&linkConfiguration->name, 16);

    // [Joiner UDP Port TLV]
    if (server_data.joiner_router_enabled && server_data.joiner_router_port) {
        message_length += 4;
        // [Steering Data TLV]
        if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len){
            message_length += cur->thread_info->registered_commissioner.steering_data_len + 2;
        }
    }

    // [Commissioner UDP Port TLV]
    if (linkConfiguration->securityPolicy & SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED) {
        message_length += 4;
    }

    message_length +=  thread_extension_discover_response_len(cur);

    uint16_t buf_id = mle_service_msg_allocate(class->interface_id, message_length + 2, false, MLE_COMMAND_DISCOVERY_RESPONSE);
    if (buf_id == 0) {
        return -1;
    }

    //SET ll64 from euid64
    uint8_t *ll64 = mle_service_get_msg_destination_address_pointer(buf_id);
    memcpy(ll64, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(ll64 + 8,msg_buffers->extentedAddress , 8);
    //No link layer security and no mle security.
    mle_service_msg_update_security_params(buf_id, 0, 0, 0);
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);

    *ptr++ = MLE_TYPE_DISCOVERY;
    *ptr++ = message_length;
    uint16_t discover_response_tlv = thread_discover_tlv_get(class->version, (linkConfiguration->securityPolicy & SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED));

    thread_extension_discover_response_tlv_write(&discover_response_tlv, class->version, linkConfiguration->securityPolicy);

    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_DISCOVERY_RESPONSE, discover_response_tlv);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_XPANID, 8, linkConfiguration->extented_pan_id);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_NETWORK_NAME, stringlen((char*)&linkConfiguration->name, 16), linkConfiguration->name);
    //Optional Part

    if (linkConfiguration->securityPolicy & SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED) {
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_UDP_PORT, server_data.commissioner_port);
    }

    if (server_data.joiner_router_enabled && server_data.joiner_router_port) {
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_UDP_PORT, server_data.joiner_router_port);
        if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len) {
            ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_STEERING_DATA, cur->thread_info->registered_commissioner.steering_data_len, cur->thread_info->registered_commissioner.steering_data);
        }
    }

    ptr =  thread_extension_discover_response_write(cur, ptr);

    if (mle_service_update_length_by_ptr(buf_id,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_message_timeout_params_t timeout;
    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_set_msg_panid(buf_id, msg_buffers->panId);
    mle_service_send_message(buf_id);
    return 0;
}


static bool thread_discovery_request_timer_update(thread_discovery_class_t *class)
{
    class->discovery_request->active_timer--;
    if (class->discovery_request->active_timer) {
        return true;
    }

    if (thread_discovery_proces_ready(class->discovery_request)) {
        thread_discovery_process_end(class->interface);
        class->discovery_request->response_cb(class->interface, &class->discovered_network);
        //Free Entry
        thread_discovery_request_free(class);
        return false;
    }

    //Switch channel and set pan-id
    thread_discovery_link_activate(class->interface, class->discovery_request->random_panid, class->discovery_request->active_channel, class->discovery_request->channel_page);
    tr_debug("Discover channel %u", class->discovery_request->active_channel);
    //Process packet
    thread_discovery_request_send(class, class->discovery_request);
    //Set timer
    class->discovery_request->active_timer = THREAD_DISCOVERY_TIMEOUT / THREAD_DISCOVER_TIMER_PERIOD;
    return true;

}

static bool thread_announce_discovery_request_timer_update(thread_discovery_class_t *class)
{
    class->thread_announce_request->active_timer--;
    if (class->thread_announce_request->active_timer) {
        return true;
    }

    if (thread_announce_discovery_process_ready(class->thread_announce_request) ) {
        thread_discovery_process_end(class->interface);
        announce_discovery_response_t *result = class->thread_announce_request->network;
        thread_announce_scan_ready_cb *response_cb = class->thread_announce_request->response_cb;
        class->thread_announce_request->network = NULL;
        thread_announce_discovery_request_free(class);
        response_cb(class->interface, result);
        return false;
    }

    //Switch channel and set pan-id
    thread_discovery_link_activate(class->interface, class->thread_announce_request->pan_id, class->thread_announce_request->active_channel, 0);
    tr_debug("Announce Discover channel %u", class->thread_announce_request->active_channel);
    //Process packet
    thread_discovery_announce_request_send(class, class->thread_announce_request);
    //Set timer
    class->thread_announce_request->active_timer = THREAD_DISCOVERY_TIMEOUT / THREAD_DISCOVER_TIMER_PERIOD;
    return true;

}

static bool thread_discovery_response_jitter_timer_update(thread_discovery_class_t *class)
{
    bool pending_list = false;
    ns_list_foreach_safe(thread_discovery_response_msg_t, cur, &class->srv_respose_msg_list) {
        cur->timer--;
        if (!cur->timer) {
            //Send a packet
            thread_discovery_response_send(class, cur);
            ns_list_remove(&class->srv_respose_msg_list, cur);
            ns_list_add_to_start(&class->srv_respose_msg_buffers, cur);
        } else {
            pending_list = true;
        }
    }

    return pending_list;

}

static thread_discovery_class_t *thread_discovery_class_get(int8_t interface_id)
{
    ns_list_foreach(thread_discovery_class_t, cur_class, &thread_discovery_class_list) {
        if (cur_class->interface_id == interface_id) {
            return cur_class;
        }
    }
    return NULL;
}

static void thread_discovery_free_discovered_networks(thread_nwk_discovery_response_list_t *list)
{
    ns_list_foreach_safe(discovery_response_list_t, cur, list) {
        ns_list_remove(list, cur);
        ns_dyn_mem_free(cur);
    }
}

static thread_discovery_class_t *thread_discovery_class_allocate(bool reedDevice)
{
    thread_discovery_class_t *class_ptr = ns_dyn_mem_alloc(sizeof(thread_discovery_class_t));

    if (class_ptr) {
        ns_list_init(&class_ptr->srv_respose_msg_list);
        ns_list_init(&class_ptr->srv_respose_msg_buffers);
        ns_list_init(&class_ptr->discovered_network);

        if (reedDevice) {
            if (!thread_discovery_server_msg_buffer_allocate(class_ptr)) {
                ns_dyn_mem_free(class_ptr);
                return NULL;
            }
        } else {
            class_ptr->msg_buffers = NULL;
        }
        class_ptr->discovery_request = NULL;
        class_ptr->thread_announce_request = NULL;
        ns_list_add_to_start(&thread_discovery_class_list, class_ptr);
    }
    return class_ptr;
}

static void thread_discovery_class_free(thread_discovery_class_t *class_ptr)
{
    if (!class_ptr) {
        return;
    }
    ns_list_remove(&thread_discovery_class_list, class_ptr);

    thread_discovery_server_msg_buffer_free(class_ptr);
    thread_discovery_free_discovered_networks(&class_ptr->discovered_network);
    thread_discovery_request_free(class_ptr);
    thread_announce_discovery_request_free(class_ptr);
    ns_dyn_mem_free(class_ptr);
}

bool thread_discovery_tlv_spesific_data_discover(const uint8_t *ptr, uint16_t length, const mescop_tlv_t *compare_tlv)
{
    const uint8_t *p;
    if (!ptr || length < 2 || !compare_tlv || !compare_tlv->data || !compare_tlv->length) {
        return false;
    }

    p = ptr;
    while (p != NULL) {
        const uint8_t *tlv_data_ptr;
        uint16_t tlv_data_length;
        //tr_info("tlv_find first check");
        // check if we have enough length for normal length tlv
        if (p + 2 > ptr + length) {
            break;    //must have at least type and short length
        }

        if (p[1] == 0xff) {
            // Long length format
            if (p + 4 > ptr + length) {
                break;    // check if enough length for long length
            }

            tlv_data_length = common_read_16_bit(&p[2]);
            tlv_data_ptr = p + 4;
        } else {
            tlv_data_length = p[1];
            tlv_data_ptr = p + 2;
        }
        //tr_info("tlv_find check: %d, type: %d", tlv_data_length, *p);

        // check if length of tlv is correct
        if (tlv_data_ptr + tlv_data_length > ptr + length) {
            break;    //length goes past the data block
        }

        if (*p == compare_tlv->type) {
            if (tlv_data_length == compare_tlv->length) {
                if (memcmp(tlv_data_ptr, compare_tlv->data, tlv_data_length) == 0) {
                    return true;
                }
            }
        }

        p = tlv_data_ptr + tlv_data_length;
    }
    return false;
}

static bool thread_discovery_request_filter_validate(link_configuration_s *linkConfiguration, uint8_t *data_ptr, uint16_t length)
{
    //Validate PAN-ID
    uint8_t pan_id[2];

    mescop_tlv_t compare_tlv;
    //Validate PAN-id, ExtentedPAN-id & Network name

    compare_tlv.data = pan_id;
    compare_tlv.length = 2;
    compare_tlv.type = MESHCOP_TLV_PANID;

    common_write_16_bit(linkConfiguration->panId, compare_tlv.data);

    if (thread_discovery_tlv_spesific_data_discover(data_ptr, length, &compare_tlv) ) {
        return false;
    }

    compare_tlv.data = linkConfiguration->extented_pan_id;
    compare_tlv.length = 8;
    compare_tlv.type = MESHCOP_TLV_XPANID;

    if (thread_discovery_tlv_spesific_data_discover(data_ptr, length, &compare_tlv) ) {
        return false;
    }

    return true;

}

static void thread_discovery_request_msg_handler(thread_discovery_class_t * discovery_class, mle_message_t *mle_msg)
{
    //Validate that server is enabled
    if (!discovery_class->discovery_server_active ) {
        return;
    }

    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(discovery_class->interface_id);
    if (!linkConfiguration) {
        return;
    }
    tr_debug("Thread discovery request message RX");

    // Check if we have room for new neighbor
    if (mle_class_free_entry_count_get(discovery_class->interface_id) < 1) {
        tr_debug("MLE table full, skip request");
        return;
    }

    //validate message
    mle_tlv_info_t discovery_tlv;
    //Parse Message
    uint16_t discover_req_tlv;

    //Discover MLE_TYPE_DISCOVERY
    if (mle_tlv_option_discover(mle_msg->data_ptr, mle_msg->data_length, MLE_TYPE_DISCOVERY, &discovery_tlv) < 4) {
        return;
    }

    if (thread_meshcop_tlv_data_get_uint16(discovery_tlv.dataPtr, discovery_tlv.tlvLen, MESHCOP_TLV_DISCOVERY_REQUEST, &discover_req_tlv) < 2) {
        tr_debug("discover response not include all mandatory TLV's");
        return;
    }
    //Validate Version
    uint8_t version = (uint8_t)(discover_req_tlv >> 12);
    if (discovery_class->version < version) {
        tr_debug("Dropped by version %u != %u", discovery_class->version, version);
        return;
    }

    bool joiner_flag = (discover_req_tlv >> 11) & 1;

    if (joiner_flag) {
        //Can we respond
        thread_management_server_data_t joiner_router_info;
        if (0 != thread_management_server_commisoner_data_get(discovery_class->interface_id , &joiner_router_info) ||
                 !joiner_router_info.joiner_router_enabled) {
            if (!thread_extension_joining_enabled(discovery_class->interface_id)) {
                tr_debug("Drop by Joining disabled");
                return;
            }
        }
    }

    //Validate possible blacklist
    if (!thread_discovery_request_filter_validate(linkConfiguration, discovery_tlv.dataPtr, discovery_tlv.tlvLen)) {
        tr_debug("Dropped by filter");
        return;
    }

    //Trig response
    thread_discovery_response_trig(discovery_class, mle_msg->packet_src_address, mle_msg->src_pan_id);
}

static bool thread_seering_data_accept_any(uint8_t length, uint8_t *data)
{
    if (length == 1 && *data == 0xff) {
        return true;
    }
    return false;
}

static void thread_discovery_nwk_push_to_list_by_lqi(thread_nwk_discovery_response_list_t *result_list, discovery_response_list_t *nwk_info) {
    if (ns_list_count(result_list)) {
        ns_list_foreach_safe(discovery_response_list_t, cur_entry, result_list) {
            if (nwk_info->dbm > cur_entry->dbm) {

                ns_list_add_before(result_list, cur_entry, nwk_info);
                return;
            }
        }
        ns_list_add_to_end(result_list, nwk_info);
    } else {
        ns_list_add_to_end(result_list, nwk_info);
    }
}

static void thread_discovery_joiner_set(thread_nwk_discovery_response_list_t *result_list, discovery_response_list_t *nwk_info, bool new_accept_any) {
    if (ns_list_count(result_list)) {

        bool cur_acept_any;
        ns_list_foreach_safe(discovery_response_list_t, cur_entry, result_list) {

            cur_acept_any = thread_seering_data_accept_any(cur_entry->steering_data_valid, cur_entry->steering_data);

            if (!cur_acept_any && !new_accept_any) {
                if (nwk_info->dbm > cur_entry->dbm) {
                    ns_list_add_before(result_list, cur_entry, nwk_info);
                    return;
                }
            } else {
                if (!new_accept_any) {
                    //add this before cur
                    ns_list_add_before(result_list, cur_entry, nwk_info);
                    return;
                } else if (nwk_info->dbm > cur_entry->dbm) {
                    ns_list_add_before(result_list, cur_entry, nwk_info);
                    return;
                }
            }
        }
        ns_list_add_to_end(result_list, nwk_info);
    } else {
        ns_list_add_to_end(result_list, nwk_info);
    }
}



static void thread_discovery_response_msg_handler(thread_discovery_class_t * discovery_class, mle_message_t *mle_msg)
{
    if (!discovery_class->discovery_request || !discovery_class->discovery_request->waiting_response) {
        return;
    }

    mle_tlv_info_t discovery_tlv;
    //Parse Message
    uint16_t discover_response_tlv;
    uint8_t *nwk_name, *extented_panid;
    uint8_t nwk_name_length;

    //Discover MLE_TYPE_DISCOVERY
    if (mle_tlv_option_discover(mle_msg->data_ptr, mle_msg->data_length, MLE_TYPE_DISCOVERY, &discovery_tlv) < 4) {
        return;
    }

    nwk_name_length = thread_meshcop_tlv_find(discovery_tlv.dataPtr, discovery_tlv.tlvLen, MESHCOP_TLV_NETWORK_NAME, &nwk_name);

    if (thread_meshcop_tlv_data_get_uint16(discovery_tlv.dataPtr, discovery_tlv.tlvLen, MESHCOP_TLV_DISCOVERY_RESPONSE, &discover_response_tlv) < 2
        || thread_meshcop_tlv_find(discovery_tlv.dataPtr, discovery_tlv.tlvLen, MESHCOP_TLV_XPANID, &extented_panid) < 8
        || nwk_name_length > 16) {

        tr_debug("discover response not include all mandatory TLV's");
        return;
    }

    tr_debug("Thread discovery response message RX");

    uint8_t version = (uint8_t)(discover_response_tlv >> 12);
    if (discovery_class->version > version) {
        tr_debug("Dropped by version %u != %u", discovery_class->version, version);
        return;
    }

    if (discovery_class->discovery_request->native_commisioner_scan) {
        bool native_commioner_bit = (discover_response_tlv >> 11) & 1;
        if (!native_commioner_bit) {
            tr_debug("Native commisioner not supported");
            return;
        }
    }
    uint16_t pan_id = mle_msg->src_pan_id;
    uint8_t *steering_data;
    uint16_t joiner_port;
    bool joiner_port_valid;

    uint8_t steerin_data_length = thread_meshcop_tlv_find(discovery_tlv.dataPtr,  discovery_tlv.tlvLen, MESHCOP_TLV_STEERING_DATA, &steering_data);
    if (steerin_data_length > 16) {
        steerin_data_length = 0;
    }

    if (thread_meshcop_tlv_data_get_uint16(discovery_tlv.dataPtr, discovery_tlv.tlvLen, MESHCOP_TLV_JOINER_UDP_PORT, &joiner_port) >= 2) {
        joiner_port_valid = true;
    } else {
        joiner_port_valid = false;
    }

    if (discovery_class->discovery_request->joiner_flag && (!joiner_port_valid || steerin_data_length == 0)) {
        if (thread_extension_version_check(discovery_class->interface->thread_info->version)) {
            if (!discovery_class->interface->thread_info->extension_credentials_ptr) {
                tr_debug("Dropped, no joiner info");
            }
        } else {
            tr_debug("Dropped by no valid joiner info %u %u",joiner_port_valid, steerin_data_length);
            return;
        }
    }

    discovery_response_list_t *nwk_info = thread_discover_response_msg_get_discover_from_list(
                    &discovery_class->discovered_network,
                    discovery_class->discovery_request->active_channel, pan_id);
    if (nwk_info) {
        if (nwk_info->dbm < mle_msg->dbm) {
            goto save_optional_data;
        }
        return;
    }

    nwk_info = thread_discover_response_msg_allocate();
    if (!nwk_info) {
        return;
    }
    //Set parameters
    nwk_info->version = (discover_response_tlv >> 12);
    nwk_info->dbm = mle_msg->dbm;
    nwk_info->channel = discovery_class->discovery_request->active_channel;

    nwk_info->pan_id = pan_id;
    memcpy(nwk_info->extented_pan_id, extented_panid, 8);

    memset(nwk_info->network_name, 0, 16);
    memcpy(nwk_info->network_name, nwk_name, nwk_name_length);

    thread_meshcop_tlv_data_get_uint16(discovery_tlv.dataPtr, discovery_tlv.tlvLen, MESHCOP_TLV_COMMISSIONER_UDP_PORT, &nwk_info->commissioner_port);

    thread_extension_discover_response_read(nwk_info, discover_response_tlv, discovery_tlv.dataPtr, discovery_tlv.tlvLen);

    //Add to last
    if (discovery_class->discovery_request->native_commisioner_scan) {
        thread_discovery_nwk_push_to_list_by_lqi(&discovery_class->discovered_network, nwk_info);
    } else {
        //Validate is steering data
        thread_discovery_joiner_set(&discovery_class->discovered_network, nwk_info, thread_seering_data_accept_any(nwk_info->steering_data_valid, nwk_info->steering_data));
    }


save_optional_data:
    memcpy(nwk_info->extented_mac, mle_msg->packet_src_address + 8, 8);
    nwk_info->extented_mac[0] ^= 2;
    if (steerin_data_length) {
        memcpy(nwk_info->steering_data,steering_data, steerin_data_length);
        nwk_info->steering_data_valid = steerin_data_length;
    }

    if (joiner_port_valid) {
        nwk_info->joiner_port = joiner_port;
    }

}

static void thread_announce_discovery_message_receiver_cb(int8_t interface_id, mle_message_t *mle_msg)
{
    if (mle_msg->message_type != MLE_COMMAND_DATASET_ANNOUNCE) {
        return;
    }
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class || !discovery_class->thread_announce_request) {
        return;
    }

    tr_debug("MLE ANNOUNCE RX");
    uint64_t timestamp;
    uint16_t panid;
    uint8_t *ptr;
    uint16_t channel;

    tr_debug("Host Recv Dataset Announce %s", trace_ipv6(mle_msg->packet_src_address));
    if (8 > thread_tmfcop_tlv_data_get_uint64(mle_msg->data_ptr, mle_msg->data_length,MLE_TYPE_ACTIVE_TIMESTAMP,&timestamp)) {
        tr_error("Missing timestamp TLV");
        return;
    }
    if (2 > thread_tmfcop_tlv_data_get_uint16(mle_msg->data_ptr, mle_msg->data_length,MLE_TYPE_PANID,&panid)) {
        tr_error("Missing Panid TLV");
        return;
    }
    if (3 > thread_tmfcop_tlv_find(mle_msg->data_ptr, mle_msg->data_length,MLE_TYPE_CHANNEL,&ptr)) {
        tr_error("Missing Channel TLV");
        return;
    }
    channel = common_read_16_bit(&ptr[1]);

    if (timestamp <= discovery_class->thread_announce_request->active_time_stamp) {
        tr_debug("Drop by timestamp");
        return;
    }

    //Validate
    announce_discovery_response_t *response = NULL;
    if (discovery_class->thread_announce_request->network) {
        response = discovery_class->thread_announce_request->network;
        if (timestamp <= discovery_class->thread_announce_request->network->active_timestamp ) {
            response = NULL;
        }
    } else {
        discovery_class->thread_announce_request->network = ns_dyn_mem_temporary_alloc(sizeof(announce_discovery_response_t));
        response = discovery_class->thread_announce_request->network;
    }

    if (response) {
        tr_debug("Save data");
        response->active_timestamp = timestamp;
        response->channel = channel;
        response->pan_id = panid;
    }
}


static void thread_announce_discover_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    //Verify security
    (void)security_headers;

    /* Check that message is from link-local scope */
    if(!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    thread_announce_discovery_message_receiver_cb(interface_id, mle_msg);
}

static void thread_discovery_message_receiver_cb(int8_t interface_id, mle_message_t *mle_msg)
{
    //Discovery interface get
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class) {
        return;
    }

    switch (mle_msg->message_type) {
        case MLE_COMMAND_DISCOVERY_REQUEST:
            //call message handler
            thread_discovery_request_msg_handler(discovery_class, mle_msg);
            break;

        case MLE_COMMAND_DISCOVERY_RESPONSE:
            //call message handler
            thread_discovery_response_msg_handler(discovery_class, mle_msg);
            break;

        default:

            break;
    }

}

static void thread_discover_event_handler(arm_event_s *event)
{
    switch (event->event_type) {
        case THREAD_DISCOVER_INIT:
            thread_discover_timer_trig();
            break;

        case THREAD_DISCOVER_TIMER:
            //Do list in future for each of mle user
            thread_discover_timer_active = false;
            if (thread_discovery_timer_update()) {
                //Request new timer
                thread_discover_timer_trig();
            }
            break;
    }
}

static int8_t thread_discover_class_event_handler_init(void)
{
    if (thread_discover_tasklet_id == -1) {
        //GENERATE TASKLET
        thread_discover_tasklet_id = eventOS_event_handler_create(&thread_discover_event_handler, THREAD_DISCOVER_INIT);
    }
    return thread_discover_tasklet_id;
}


int thread_discovery_init(int8_t interface_id, struct protocol_interface_info_entry *cur_interface, uint8_t version, bool reedDevice)
{
    if (!cur_interface) {
        return -2;
    }

    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (discovery_class) {
        //Verify reed boolean

        thread_discovery_request_free(discovery_class);
        thread_announce_discovery_request_free(discovery_class);
        thread_discovery_server_msg_buffer_free(discovery_class);
        if (reedDevice) {
            if (!thread_discovery_server_msg_buffer_allocate(discovery_class)) {
                thread_discovery_class_free(discovery_class);
                return -1;
            }
        }

        goto return_ok;
    }

    if (thread_discover_class_event_handler_init() < 0) {
        return -1;
    }

    //Allocate new entry
    discovery_class = thread_discovery_class_allocate(reedDevice);
    if (!discovery_class ) {
        mle_service_interface_receiver_bypass_handler_update(interface_id,  NULL);
        return -1;
    }
    discovery_class->interface_id = interface_id;

return_ok:
    discovery_class->discovery_server_active = false;
    discovery_class->interface = cur_interface;
    discovery_class->version = version;
    return 0;
}

/**
 * Reset discovery class state to idle
 */
int thread_discovery_reset(int8_t interface_id)
{
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class ) {
        return -1;
    }
    thread_discovery_request_free(discovery_class);
    thread_announce_discovery_request_free(discovery_class);
    thread_discovery_server_msg_clean(discovery_class);
    return 0;
}

/**
 * Update discovery timer state's
 */
static bool thread_discovery_timer_update(void)
{
    bool keep_timer_active = false;
    ns_list_foreach(thread_discovery_class_t, cur_class, &thread_discovery_class_list) {
        if (cur_class->discovery_server_active) {
            if (thread_discovery_response_jitter_timer_update(cur_class)) {
                keep_timer_active = true;
            }
        } else if (cur_class->discovery_request) {
            if (thread_discovery_request_timer_update(cur_class) ) {
                keep_timer_active = true;
            }
        } else if(cur_class->thread_announce_request) {
            if ( thread_announce_discovery_request_timer_update(cur_class) ) {
                keep_timer_active = true;
            }
        }
    }
    return keep_timer_active;
}

/**
 * Enable thread discovery request response support
 */
int thread_discovery_responser_enable(int8_t interface_id, bool enable_service)
{
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class || !discovery_class->msg_buffers) {
        return -1;
    }


    //Clean server message list always
    thread_discovery_server_msg_clean(discovery_class);
    if (mle_service_interface_receiver_bypass_handler_update(interface_id,  thread_discovery_message_receiver_cb) != 0) {
        return -1;
    }

    discovery_class->discovery_server_active = enable_service;

    return 0;
}

static void thread_discovery_normal_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    if (security_headers->securityLevel == 0) {
        thread_discovery_message_receiver_cb(interface_id, mle_msg);
    }
}

/**
 * Start Thread network discovery
 */
int thread_discovery_network_scan(int8_t interface_id, thread_discover_reques_t *scan_request, thread_discovery_ready_cb *ready_cb)
{

    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class || !ready_cb || !scan_request) {
        return -1;
    }

    //Check Is discovery started already
    if (discovery_class->discovery_request) {
        return -2;
    }

    if (!scan_request->channel_mask || (scan_request->filter_tlv_length > 0 && !scan_request->filter_tlv_data)) {
        return -1;
    }

    discovery_class->discovery_request = thread_discovery_request_allocate(scan_request, ready_cb);

    if (!discovery_class->discovery_request) {
        return -3;
    }

    if (mle_service_interface_register(interface_id, thread_discovery_normal_receive_cb, discovery_class->discovery_request->temporary_mac64,8) != 0) {
        thread_discovery_request_free(discovery_class);
        return -1;
    }

    if (mle_service_interface_receiver_bypass_handler_update(interface_id, thread_discovery_message_receiver_cb) != 0) {
        thread_discovery_request_free(discovery_class);
        return -1;
    }

    //Free old networks
    thread_discovery_free_discovered_networks(&discovery_class->discovered_network);
    //Set temporary mac and generate ll64
    thread_discovery_prepare(discovery_class->interface, discovery_class->discovery_request);
    return 0;
}

int thread_discovery_announce_network_scan(int8_t interface_id, thread_announce_discover_reques_t *scan_request, thread_announce_scan_ready_cb *ready_cb)
{
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class || !ready_cb || !scan_request) {
        return -1;
    }

        //Check Is discovery started already
        if (discovery_class->discovery_request || discovery_class->thread_announce_request) {
            return -2;
        }

        if (!scan_request->channel_mask) {
            return -1;
        }

        discovery_class->thread_announce_request = thread_announce_discovery_request_allocate(scan_request, ready_cb);

        if (!discovery_class->thread_announce_request) {
            return -3;
        }

        //Update receiver callback
        if (mle_service_interface_receiver_handler_update(interface_id, thread_announce_discover_receive_cb) != 0) {
            thread_announce_discovery_request_free(discovery_class);
            return -1;
        }

        if (mle_service_interface_receiver_bypass_handler_update(interface_id,  thread_announce_discovery_message_receiver_cb) != 0) {
            thread_discovery_request_free(discovery_class);
            return -1;
        }

        //Set temporary mac and generate ll64
        thread_announce_discovery_prepare(discovery_class->interface, discovery_class->thread_announce_request);
        return 0;
}

discovery_response_list_t *thread_discovery_network_description_get(int8_t interface_id)
{
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class ) {
        return NULL;
    }

    discovery_response_list_t *entry = ns_list_get_first(&discovery_class->discovered_network);
    if (entry) {
        ns_list_remove(&discovery_class->discovered_network, entry);
    }

    return entry;

}

#if 0
/**
 * Free all allocated memory and free class
 * Not used API function, flagged away. This should be taken in use when refactoring ifdown - functionality.
 */
int thread_discovery_free(int8_t interface_id)
{
    thread_discovery_class_t * discovery_class = thread_discovery_class_get(interface_id);
    if (!discovery_class ) {
        return -1;
    }

    thread_discovery_class_free(discovery_class);


    return 0;
}
#endif
#endif
