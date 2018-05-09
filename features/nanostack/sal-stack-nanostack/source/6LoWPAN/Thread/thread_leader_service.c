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

#include <string.h>
#include "ns_types.h"
#include <nsdynmemLIB.h>
#include <ns_list.h>
#include "randLIB.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "thread_management_if.h"
#include "thread_border_router_api.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_commissioning_if.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "MPL/mpl.h"
#include "MLE/mle.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "coap_service_api.h"

#define TRACE_GROUP "tLdr"

#ifdef HAVE_THREAD_LEADER_SERVICE

/*
 * Leader instance data.
 */
typedef struct thread_leader_service {
    int8_t interface_id;
    int8_t coap_service_id;
    bool leader_enabled: 1;
    ns_list_link_t link;
} thread_leader_service_t;

/*
 * Helper structure for router_id handling
 */
typedef struct {
    uint8_t     routerID[8];
    uint32_t    validLifeTime;
    uint32_t    preferredLifeTime;
    uint16_t    shortAddress;
    uint8_t     dataSeq;
} thread_leader_service_router_id_resp_t;

static NS_LIST_DEFINE(leader_service_instance_list, thread_leader_service_t, link);

static void thread_leader_service_commissioner_timeout_cb(void* arg);

/* Private functions */
static thread_leader_service_t *thread_leader_service_find_by_interface(int8_t interface_id)
{
    thread_leader_service_t *this = NULL;
    ns_list_foreach(thread_leader_service_t, cur_br, &leader_service_instance_list) {
        if (cur_br->interface_id == interface_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static thread_leader_service_t *thread_leader_service_find_by_service(int8_t service_id)
{
    thread_leader_service_t *this = NULL;
    ns_list_foreach(thread_leader_service_t, cur_br, &leader_service_instance_list) {
        if (cur_br->coap_service_id == service_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static char* thread_commissioning_if_commissioner_id_get(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->registered_commissioner.commissioner_valid) {
        return NULL;
    }

    return cur->thread_info->registered_commissioner.commissioner_id_ptr;
}


static bool thread_leader_service_commissioner_address_changed(int8_t interface_id, uint8_t border_router_address[static 16])
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->registered_commissioner.commissioner_valid) {
        return false;
    }

    if(!memcmp(cur->thread_info->registered_commissioner.border_router_address, border_router_address, 16)) {
        // Border router address is same
        return false;
    }
    if( common_read_16_bit(&border_router_address[14]) >= 0xfc00 ) {
        // Address is not valid RLOC we dont change address to this.
        return false;
    }
    memcpy(cur->thread_info->registered_commissioner.border_router_address, border_router_address, 16);
    thread_leader_service_network_data_changed(cur,false,true);
    // This will trigger advertisement either from following commands or from leader tick
    return true;
}
static int thread_leader_service_steering_data_set(int8_t interface_id, uint8_t *buf_ptr, uint16_t length)
{
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;

    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->leader_private_data || !cur->thread_info->registered_commissioner.commissioner_valid) {
        return -1;
    }
    if (length > 16) {
        return -2;
    }

    tr_debug("Save new steering data %s", trace_array(buf_ptr, length));

    cur->thread_info->registered_commissioner.steering_data_len = 0;

    if (buf_ptr && length > 0) {
        memcpy(cur->thread_info->registered_commissioner.steering_data, buf_ptr, length);
        cur->thread_info->registered_commissioner.steering_data_len = length;
    }

    thread_leader_service_network_data_changed(cur, false, true);

    return 0;
}

static bool thread_leader_service_commissioner_unregister(int8_t interface_id, uint16_t session_id)
{
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;

    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if(!linkConfiguration)
        return false;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->leader_private_data)
        return false;

    if (cur->thread_info->registered_commissioner.session_id != session_id) {
        tr_debug("Commissioner session id not valid - unregister failed");
        return false;
    }
    // Removing commissioner we need to increase the session id
    cur->thread_info->registered_commissioner.session_id++;

    if(cur->thread_info->registered_commissioner.commissioner_id_ptr){
       ns_dyn_mem_free(cur->thread_info->registered_commissioner.commissioner_id_ptr);
       cur->thread_info->registered_commissioner.commissioner_id_ptr = NULL;
    }

    cur->thread_info->registered_commissioner.commissioner_valid = false;
    cur->thread_info->registered_commissioner.steering_data_len = 0;

    /* Send unregistration without delay */
    thread_leader_service_network_data_changed(cur, false, true);

    eventOS_timeout_cancel(cur->thread_info->registered_commissioner.commissioner_timeout);
    cur->thread_info->registered_commissioner.commissioner_timeout = NULL;
    cur->thread_info->registered_commissioner.commissioner_registration = THREAD_COMMISSIONER_NOT_REGISTERED;

    return true;
}

static bool thread_leader_service_commissioner_session_refresh(int8_t interface_id, uint16_t session_id)
{
    protocol_interface_info_entry_t *cur;

    // Check pointers
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->leader_private_data || !cur->thread_info->registered_commissioner.commissioner_valid) {
        return false;
    }

    if(cur->thread_info->registered_commissioner.commissioner_registration == THREAD_COMMISSIONER_NOT_REGISTERED) {
        tr_debug("Commissioner not registered - refresh failed");
        return false;
    }

    if (cur->thread_info->registered_commissioner.session_id != session_id) {
        tr_debug("Commissioner session id not valid - refresh failed");
        return false;
    }

    cur->thread_info->registered_commissioner.commissioner_registration = THREAD_COMMISSIONER_REGISTERED;

    eventOS_timeout_cancel(cur->thread_info->registered_commissioner.commissioner_timeout);
    cur->thread_info->registered_commissioner.commissioner_timeout = eventOS_timeout_ms(thread_leader_service_commissioner_timeout_cb, COMMISSIONER_OBSOLETE_TIMEOUT, cur->thread_info);

    return true;
}

static void thread_leader_service_commissioner_timeout_cb(void* arg)
{
    thread_info_t *thread_ptr = arg;

    thread_ptr->registered_commissioner.commissioner_timeout = NULL;

    if (!thread_ptr->registered_commissioner.commissioner_valid) {
        return;
    }

    if (!thread_ptr->leader_private_data) {
        return;
    }

    switch(thread_ptr->registered_commissioner.commissioner_registration)
    {
    case THREAD_COMMISSIONER_REGISTRATION_OBSOLETE:
        thread_ptr->registered_commissioner.commissioner_registration = THREAD_COMMISSIONER_NOT_REGISTERED;
        if(false == thread_leader_service_commissioner_unregister(thread_ptr->interface_id, thread_ptr->registered_commissioner.session_id)){
            tr_debug("Commissioner registration remove failed");
        }
        else{
            tr_debug("Commissioner registration removed");
        }
        break;
    case THREAD_COMMISSIONER_REGISTERED:
        tr_debug("Commissioner registration obsoleted");
        thread_ptr->registered_commissioner.commissioner_registration = THREAD_COMMISSIONER_REGISTRATION_OBSOLETE;
        thread_ptr->registered_commissioner.commissioner_timeout = eventOS_timeout_ms(thread_leader_service_commissioner_timeout_cb, COMMISSIONER_REMOVE_TIMEOUT, thread_ptr);
        break;
    case THREAD_COMMISSIONER_NOT_REGISTERED:
    default:
       break;
    }
}


static int thread_leader_service_commissioner_register(int8_t interface_id, uint8_t border_router_address[static 16],
                                  uint8_t *commissioner_id_ptr, uint16_t commissioner_id_len,
                                  uint16_t *session_id)
{
    /* Save commissioner data to new commissioner structure and start advertise it
    Commissioner session id
    commissioner timestamp
    commissioner credentials -> static configuration
    security policy -> static configuration
    network name -> static configuration
    border router locator
    steering data
    */
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;

    tr_debug("Register interface %d commissioner: %s", interface_id, trace_ipv6(border_router_address));

    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || !cur->thread_info->leader_private_data) {
        return -1;
    }

    /* If commissioner already registered */
    if(cur->thread_info->registered_commissioner.commissioner_valid &&
            cur->thread_info->registered_commissioner.commissioner_registration == THREAD_COMMISSIONER_REGISTERED) {
        return -2;
    }

    ns_dyn_mem_free(cur->thread_info->registered_commissioner.commissioner_id_ptr);
    cur->thread_info->registered_commissioner.commissioner_id_ptr = ns_dyn_mem_alloc(commissioner_id_len + 1);
    if (!cur->thread_info->registered_commissioner.commissioner_id_ptr) {
        return -1;
    }
    memcpy(cur->thread_info->registered_commissioner.commissioner_id_ptr, commissioner_id_ptr, commissioner_id_len);
    cur->thread_info->registered_commissioner.commissioner_id_ptr[commissioner_id_len] = 0;

    // Initialise values
    cur->thread_info->registered_commissioner.session_id++;
    if (cur->thread_info->registered_commissioner.session_id == 0) {
        cur->thread_info->registered_commissioner.session_id++;
    }

    if (session_id) {
        *session_id = cur->thread_info->registered_commissioner.session_id;
    }

    //SET Border Router Locator
    memcpy(cur->thread_info->registered_commissioner.border_router_address, border_router_address, 16);
    cur->thread_info->registered_commissioner.commissioner_valid = true;

    cur->thread_info->registered_commissioner.commissioner_registration = THREAD_COMMISSIONER_REGISTERED;

    eventOS_timeout_cancel(cur->thread_info->registered_commissioner.commissioner_timeout);
    cur->thread_info->registered_commissioner.commissioner_timeout = eventOS_timeout_ms(thread_leader_service_commissioner_timeout_cb, COMMISSIONER_OBSOLETE_TIMEOUT, cur->thread_info);

    /* Delay network data propagation, perhaps we receive more data */
    thread_leader_service_network_data_changed(cur, false, true);

    return 0;
}



static int thread_leader_service_active_set_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    link_configuration_s *linkConfiguration;
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    uint16_t session_id;
    uint8_t payload[5]; // 4 + 1
    uint8_t *ptr;
    uint8_t *response_ptr;
    uint64_t timestamp;
    bool msg_from_commissioner = false;
    bool changing_connectivity = false;
    (void) source_address;
    (void) source_port;
    int ret = 0;

    if (!this) {
        return -1;
    }
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    linkConfiguration = thread_joiner_application_get_config(this->interface_id);
    if(!cur || !cur->thread_info || !linkConfiguration || !cur->thread_info->leader_private_data){
        return -1;
    }

    tr_info("thread management Active set");

    response_ptr = payload;

    if (!thread_management_server_source_address_check(this->interface_id, source_address)) {
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto send_error_response;
    }

    if (3 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_CHANNEL, &ptr) &&
         (linkConfiguration->rfChannel != common_read_16_bit(&ptr[1]) || linkConfiguration->channel_page != *ptr)){
        tr_debug("Channel changed");
        changing_connectivity = true;
        // validity check for channel page
        if (*ptr != 0 || common_read_16_bit(&ptr[1]) < 11 || common_read_16_bit(&ptr[1]) > 26) {
            tr_warn("invalid channel");
            ret = -1;
            goto send_response;
        }
    }
    if (2 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PANID, &ptr) &&
         linkConfiguration->panId != common_read_16_bit(ptr)){
        tr_debug("PANID changed");
        changing_connectivity = true;
        if (common_read_16_bit(ptr) > 0xfffe) {
            tr_warn("invalid panid");
            ret = -1;
            goto send_response;
        }
    }
    if (3 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_SECURITY_POLICY, &ptr)) {
        if (common_read_16_bit(ptr) < 3600) {
            tr_warn("invalid security policy");
            ret = -1;
            goto send_response;
        }
    }

    if ( 8 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA, &ptr) &&
         memcmp(linkConfiguration->mesh_local_ula_prefix,ptr,8) != 0) {
        tr_debug("ML prefix changed");
        changing_connectivity = true;
    }
    if ( 16 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_MASTER_KEY, &ptr) &&
         memcmp(linkConfiguration->master_key,ptr,16) != 0) {
        tr_debug("Master key changed");
        changing_connectivity = true;
    }

    if (2 <= thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        // Session id present must be valid
        tr_info("message from commissioner");
        msg_from_commissioner = true;
        if (cur->thread_info->registered_commissioner.session_id != session_id) {
            tr_warn("Invalid commissioner session id");
            ret = -1;
            goto send_response;
        }
        thread_leader_service_commissioner_address_changed(this->interface_id, source_address);
        // Check if address changed
        if (changing_connectivity == true) {
            tr_warn("Changes affecting connectivity rejected");
            ret = -1;
            goto send_response;
        }
    }

    if(8 > thread_meshcop_tlv_data_get_uint64(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_ACTIVE_TIME_STAMP, &timestamp)){
        tr_warn("no timestamp");
        ret = -1;
        goto send_response;
    }
    if (timestamp < linkConfiguration->timestamp){
        tr_warn("older timestamp");
        ret = -1;
        goto send_response;
    }
    if (timestamp == linkConfiguration->timestamp){
        tr_warn("same timestamp");
        ret = -1;
        goto send_response;
    }
    if (!msg_from_commissioner) {
        tr_debug("message from end device");
        if (cur->thread_info->registered_commissioner.commissioner_registration != THREAD_COMMISSIONER_NOT_REGISTERED) {
            // Node changed settings and Registered commissioner send changed_notify to commissioner
            tr_info("Notify commissioner that settings are changed");
            coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, cur->thread_info->registered_commissioner.border_router_address,
                         THREAD_MANAGEMENT_PORT, COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_DATASET_CHANGED,
                         COAP_CT_OCTET_STREAM, NULL, 0, NULL);
        }
    }
    if (changing_connectivity) {

        tr_info("creating pending configuration");

        if (thread_joiner_application_pending_config_exists(this->interface_id) &&
            thread_joiner_application_pending_config_timestamp_get(this->interface_id) >= timestamp ) {
            tr_info("Pending config is newer we reject");
            ret = -1;
            goto send_response;
        }
        if (thread_joiner_application_pending_config_create(this->interface_id, request_ptr->payload_ptr, request_ptr->payload_len) != 0) {
            tr_error("Could not create pending config");
            response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
            goto send_error_response;
        }

        // Start distributing pending configuration
        thread_joiner_application_pending_config_add_missing_fields(this->interface_id);
        thread_joiner_application_pending_config_timestamp_set(this->interface_id, timestamp);
        thread_joiner_application_pending_config_enable(this->interface_id, thread_delay_timer_default*1000);

    } else {
        tr_info("updating active configuration");
        ret = thread_joiner_application_update_configuration(cur->id, request_ptr->payload_ptr, request_ptr->payload_len, true);
        // link configuration is changed, read configuration again
        linkConfiguration = thread_joiner_application_get_config(cur->id);
        if (!linkConfiguration) {
            tr_error("Could not find current config");
            response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
            goto send_error_response;
        }
        // Activate new configuration.
        thread_configuration_thread_activate(cur, linkConfiguration);
    }
    thread_joiner_application_configuration_nvm_save(this->interface_id);

    // Start timer to propagate new data
    thread_leader_service_network_data_changed(cur, true, true);

send_response:
    // build response
    response_ptr = thread_meshcop_tlv_data_write_uint8(response_ptr, MESHCOP_TLV_STATE, ret == 0 ? 1 : 0xff);

send_error_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, response_ptr - payload);
    return 0;
}

static int thread_leader_service_pending_set_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    link_configuration_s *linkConfiguration;
    uint8_t *ptr;
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    uint16_t session_id;
    uint8_t payload[5]; // 4 + 1
    uint8_t *secret_ptr;
    uint64_t active_timestamp;
    uint64_t pending_timestamp;
    uint32_t delay_timer;
    (void) source_address;
    (void) source_port;
    int ret = 0;
    bool msg_from_commissioner = false;
    bool master_key_changed = false;

    ptr = payload;
    if (!this) {
        return -1;
    }
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    linkConfiguration = thread_joiner_application_get_config(this->interface_id);
     if(!cur || !cur->thread_info || !linkConfiguration || !cur->thread_info->leader_private_data){
        return -1;
    }

    tr_info("thread management Pending set");

    if (!thread_management_server_source_address_check(this->interface_id, source_address)) {
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto send_error_response;
    }

    if (2 <= thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        // Session id present must be valid
        if (cur->thread_info->registered_commissioner.session_id != session_id) {
            tr_warn("Invalid commissioner session id");
            ret = -1;
            goto send_response;
        }
        thread_leader_service_commissioner_address_changed(this->interface_id, source_address);
        msg_from_commissioner = true;
    }
    if ( 16 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_MASTER_KEY, &secret_ptr) &&
         memcmp(linkConfiguration->master_key,secret_ptr,16) != 0) {
        tr_debug("Master key changed");
        master_key_changed = true;
    }

    if(8 > thread_meshcop_tlv_data_get_uint64(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PENDING_TIMESTAMP, &pending_timestamp)){
        tr_warn("no timestamp");
        ret = -1;
        goto send_response;
    }
    if(4 > thread_meshcop_tlv_data_get_uint32(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_DELAY_TIMER, &delay_timer)){
        tr_warn("no delay timer");
        ret = -1;
        goto send_response;
    }
    if(8 > thread_meshcop_tlv_data_get_uint64(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_ACTIVE_TIME_STAMP, &active_timestamp)){
        tr_warn("no active timestamp");
        ret = -1;
        goto send_response;
    }
    if (active_timestamp <= linkConfiguration->timestamp && !master_key_changed) {
        tr_warn("Timestamp is lower or same, without Master key change");
        ret = -1;
        goto send_response;
    }

    if (thread_joiner_application_pending_config_exists(this->interface_id) &&
        pending_timestamp <= thread_joiner_application_pending_config_timestamp_get(this->interface_id)){
        tr_warn("Older or same pending timestamp");
        ret = -1;
        goto send_response;
    }

    if (delay_timer > THREAD_MAX_DELAY_TIMER_SECONDS*1000) {
        delay_timer = THREAD_MAX_DELAY_TIMER_SECONDS*1000;
    }

    if (master_key_changed &&
            delay_timer < thread_delay_timer_default*1000) {
        //If message Changes the master key default value is used
        delay_timer = thread_delay_timer_default*1000;
    }
    if (delay_timer < THREAD_DELAY_TIMER_MINIMAL_SECONDS*1000) {
        // Absolute minimum value allowed
        delay_timer = THREAD_DELAY_TIMER_MINIMAL_SECONDS*1000;
    }

    if (thread_joiner_application_pending_config_create(this->interface_id, request_ptr->payload_ptr, request_ptr->payload_len) != 0){
        tr_error("Could not create pending config");
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_error_response;
    }

    if (!msg_from_commissioner) {
        tr_debug("pending set from end device");
        if (cur->thread_info->registered_commissioner.commissioner_registration != THREAD_COMMISSIONER_NOT_REGISTERED) {
            // Node changed settings and Registered commissioner send changed_notify to commissioner
            tr_info("Notify commissioner that settings are changed");
            coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, cur->thread_info->registered_commissioner.border_router_address,
                         THREAD_MANAGEMENT_PORT, COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_DATASET_CHANGED,
                         COAP_CT_OCTET_STREAM, NULL, 0, NULL);
        }
    }

    tr_info("creating new pending set delay: %"PRIu32, delay_timer);
    // Activate pending configuration
    thread_joiner_application_pending_config_add_missing_fields(this->interface_id);
    thread_joiner_application_pending_config_timestamp_set(this->interface_id, pending_timestamp);
    thread_joiner_application_pending_config_enable(this->interface_id, delay_timer);

    // Start timer to propagate new data
    thread_leader_service_network_data_changed(cur, true, true);

send_response:
    // build response
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, ret == 0 ? 1 : 0xff);

send_error_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

static int thread_leader_service_commissioner_set_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    uint16_t session_id;
    uint16_t br_locator;
    uint8_t payload[5]; // 4 + 1
    uint8_t *ptr;
    uint8_t *steering_data_ptr;
    uint16_t steering_data_len;
    (void) source_address;
    (void) source_port;
    int ret = -2;

    ptr = payload;
    if (!this) {
        return -1;
    }
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if(!cur || !cur->thread_info || !cur->thread_info->leader_private_data){
        return -1;
    }

    tr_info("thread management commissioner set");

    if (!thread_management_server_source_address_check(this->interface_id, source_address)) {
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto send_error_response;
    }
    //Check if the CoAp payload is greater than maximum commissioner data size and reject
    if (request_ptr->payload_len > THREAD_MAX_COMMISSIONER_DATA_SIZE){
        tr_error("Payload length greater than maximum commissioner data size");
        ret = -1;
        goto send_response;
    }
    //TODO must be checked against a list of all non permissible TLVs
    if (2 <= thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_BORDER_ROUTER_LOCATOR, &br_locator)){
        tr_error("BORDER ROUTER TLV should not be present");
        ret = -1;
        goto send_response;
    }

    // TODO must be checked against commissioner address but there is no way to get it.
    if (2 != thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        // Session id not present must be valid
        tr_warn("Commissioner session id not present");
        ret = -1;
        goto send_response;
    }
    if (cur->thread_info->registered_commissioner.session_id != session_id) {
        tr_warn("Invalid commissioner session id");
        ret = -1;
        goto send_response;
    }

    /* Set border router address */
    thread_leader_service_commissioner_address_changed(this->interface_id, source_address);

    /* Set steering data */
    if(thread_meshcop_tlv_exist(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_STEERING_DATA)){
        steering_data_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_STEERING_DATA, &steering_data_ptr);
        ret = thread_leader_service_steering_data_set(this->interface_id, steering_data_ptr, steering_data_len);
        tr_info("steering data set : %s, ret %d", trace_array(steering_data_ptr, steering_data_len), ret);
    }

send_response:
    // build response
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, ret == 0 ? 1 : 0xff);

send_error_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

static void thread_leader_allocate_router_id_by_allocated_id(thread_leader_info_t *info, uint8_t router_id, const uint8_t *eui64)
{
    memcpy(info->thread_router_id_list[router_id].eui64, eui64, 8);
    bit_set(info->master_router_id_mask, router_id);
    info->thread_router_id_list[router_id].validLifeTime = 0xffffffff;
    info->thread_router_id_list[router_id].reUsePossible = false;
}

static int thread_leader_service_router_id_allocate(const uint8_t *eui64, protocol_interface_info_entry_t *cur, thread_leader_service_router_id_resp_t *reponse)
{
    int ret_val = -1;
    uint8_t allocated_id = 64;
    uint8_t i;
    uint8_t compareId;
    thread_leader_info_t *leader_private_ptr = 0;
    thread_info_t *thread_info = cur->thread_info;
    uint8_t activeRouterCount;

    if (!thread_info || !thread_info->leader_private_data) {
        return -1;
    }

    leader_private_ptr = thread_info->leader_private_data;
    //Check if we have already allocated router address for this MAC address
    for (i = 0; i < 64; i++) {
        if (bit_test(leader_private_ptr->master_router_id_mask, i)) {
            //Active ID
            if (memcmp(eui64, leader_private_ptr->thread_router_id_list[i].eui64, 8) == 0) {
                allocated_id = i;
                break;
            }
        }
    }

    if (allocated_id  < 64) {
        goto alloc_ready;
    }
    activeRouterCount = thread_routing_count_active_routers_from_mask(leader_private_ptr->master_router_id_mask);
    if (activeRouterCount >= cur->thread_info->testMaxActiveRouterIdLimit) {
        // Only 32 router id allowed for one Thread partition
        goto alloc_ready;
    }


    //Try to allocate the same router ID that was previously used
    if (thread_is_router_addr(reponse->shortAddress)) {
        compareId = thread_router_id_from_addr(reponse->shortAddress);
        tr_debug("HInt: %02x", compareId);
        if (!bit_test(leader_private_ptr->master_router_id_mask, compareId)) {
            //Take Free entry By Hint if valid lifetime is zero
            if (leader_private_ptr->thread_router_id_list[compareId].reUsePossible) {
                allocated_id = compareId;
            }
        }
    }

    if (allocated_id  < 64) {
        goto alloc_new;
    }

    //We need to allocate new Router ID
    uint16_t random_base = randLIB_get_random_in_range(0,64);
    // New addresses are assigned randomly
    for (i = 0; i < 64; i++) {
        uint16_t id = (random_base + i) % (MAX_ROUTER_ID + 1);
        if (!bit_test(leader_private_ptr->master_router_id_mask, id)) {
            if (leader_private_ptr->thread_router_id_list[id].reUsePossible) {
                allocated_id = id;
                break;
            }
        }
    }

alloc_new:
    if (allocated_id < 64) {
        //Update Route MASK and push NEW Route Info
        thread_leader_allocate_router_id_by_allocated_id(leader_private_ptr, allocated_id, eui64);

        //Increment Route TLV data sequency
        thread_leader_service_update_id_set(cur);
    }

alloc_ready:

    if (allocated_id < 64) {
        thread_leader_allocate_router_id_by_allocated_id(leader_private_ptr, allocated_id, eui64);
        reponse->shortAddress = (allocated_id << 10);
        memcpy(reponse->routerID, leader_private_ptr->master_router_id_mask, 8);
        reponse->dataSeq = leader_private_ptr->maskSeq;
        reponse->preferredLifeTime = 0xffffffff;
        reponse->validLifeTime = 0xffffffff;
        ret_val = 0;
    }
    return ret_val;
}

static int thread_leader_service_leader_data_allocate(protocol_interface_info_entry_t *cur)
{
    thread_leader_info_t *leader_info = 0;
    leader_info = thread_allocate_and_init_leader_private_data();
    if (leader_info == NULL) {
        return -1;
    }
    cur->thread_info->leader_private_data = leader_info;
    cur->thread_info->thread_device_mode = THREAD_DEVICE_MODE_ROUTER;
    cur->thread_info->thread_attached_state = THREAD_STATE_CONNECTED_ROUTER;
    return 0;
}

static int thread_leader_service_router_id_deallocate(const uint8_t *eui64, protocol_interface_info_entry_t *cur)
{
    uint8_t i;
    thread_leader_info_t *leader_private_ptr = 0;
    thread_info_t *thread_info = cur->thread_info;
    if (thread_info && thread_info->leader_private_data) {
        leader_private_ptr = thread_info->leader_private_data;
        //Check by MAC address
        for (i = 0; i < 64; i++) {
            if (bit_test(leader_private_ptr->master_router_id_mask, i)) {
                //Active ID
                if (memcmp(eui64, leader_private_ptr->thread_router_id_list[i].eui64, 8) == 0) {
                    tr_debug("Release Router Id %d", i);
                    thread_leader_service_route_mask_bit_clear(leader_private_ptr, i);
                    leader_private_ptr->thread_router_id_list[i].reUsePossible = true;
                    leader_private_ptr->thread_router_id_list[i].validLifeTime = 0;
                    //Increment Route TLV data sequency
                    thread_leader_service_update_id_set(cur);
                    thread_routing_activate(&cur->thread_info->routing);
                    return 0;
                }
            }
        }
    }

    return -1;
}

static int thread_leader_service_routerid_assign(protocol_interface_info_entry_t *cur, const uint8_t mac[8], uint8_t router_id_mask_out[9], uint16_t *router_id)
{
    thread_leader_service_router_id_resp_t router_id_response;
    tr_debug("RId Server assign");
    router_id_response.shortAddress = *router_id; // Set Hint

    if (thread_leader_service_router_id_allocate(mac, cur, &router_id_response) != 0) {
        tr_debug("Assign Fail");
        return -1;
    }
    *router_id = router_id_response.shortAddress;
    router_id_mask_out[0] = router_id_response.dataSeq;
    memcpy(&router_id_mask_out[1], router_id_response.routerID , 8);
    return 0;
}

static int thread_leader_service_routerid_release(int8_t interface_id, uint8_t mac[8], uint16_t router_id)
{
    (void) router_id;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_error("No Interface");
        return -1;
    }

    if (!cur->thread_info) {
        tr_error("Not Thread Interface");
        return -1;
    }

    if (!cur->thread_info->leader_private_data) {
        tr_error("Not Leader");
        return -1;
    }

    return thread_leader_service_router_id_deallocate(mac, cur);
}

/**
 * Router id assignment callback for leader
 * uri = tn/d/as
 */
static int thread_leader_service_assign_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    uint8_t payload[24]; //!< response payload 4 + 9, 4 + 2, 4 + 1
    uint8_t *mac_ptr = NULL;
    uint8_t router_id_mask[9] = {0};
    uint16_t router_id = 0xfffe;
    uint8_t *ptr;
    uint8_t *status;
    (void) source_address;
    (void) source_port;

    if (!this) {
        return -1;
    }

    tr_debug("Thread router id assign request");
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!cur || !cur->thread_info) {
        return -1;
    }

    // LOCATOR is optional others are mandatory
    (void)thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_RLOC16, &router_id);

    if (8 > thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_MAC_ADDRESS, &mac_ptr) ) {
        // Failure in message
        return -1;
    }

        // Failure in message
    if (1 > thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_STATUS, &status) ) {
        return -1;
    }

    int ret = -1;
    if (*status == THREAD_COAP_STATUS_TLV_TOO_FEW_ROUTERS) {
        uint8_t activeRouterCount = 0;
        if (cur->thread_info->leader_private_data) {
            activeRouterCount = thread_routing_count_active_routers_from_mask(cur->thread_info->leader_private_data->master_router_id_mask);
        }

        if (activeRouterCount < cur->thread_info->routerSelectParameters.routerUpgradeThresHold) {
            ret = thread_leader_service_routerid_assign(cur, mac_ptr, router_id_mask, &router_id);
        } else {
            tr_info("Router ID not assigned, routers %d >= %d(upgrade threshold)", activeRouterCount, cur->thread_info->routerSelectParameters.routerUpgradeThresHold);
        }
    } else {
        ret = thread_leader_service_routerid_assign(cur, mac_ptr, router_id_mask, &router_id);
    }

    ptr = payload;
    if (ret == 0) {
        tr_debug("Thread router id mac %s, mask %s, locator %x", trace_array(mac_ptr, 8), trace_array(router_id_mask, 9), router_id);
        ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, THREAD_COAP_STATUS_TLV_SUCCESS);
        ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, router_id);
        ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_ROUTER_MASK, 9, router_id_mask);
    } else {
        ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, THREAD_COAP_STATUS_TLV_NO_ADDRESS_AVAILABLE);
    }

    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

/**
 * Router id assignment callback for leader
 * uri = tn/d/ar
 */
static int thread_leader_service_release_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    uint8_t payload[5]; //!< response payload 4 + 1
    uint8_t *mac_ptr;
    uint16_t router_id;
    uint8_t *ptr;
    (void) source_address;
    (void) source_port;

    if (!this) {
        return -1;
    }

    tr_debug("Thread router id release request");
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_RLOC16, &router_id)) {
        return -1;
    }

    if (8 > thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_MAC_ADDRESS, &mac_ptr)) {
        // Failure in message
        return -1;
    }
    tr_debug("Thread router release id mac %s, locator %x", trace_array(mac_ptr, 8), router_id);

    thread_leader_service_routerid_release(this->interface_id, mac_ptr, router_id);

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, 0);
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload, ptr - payload);

    return 0;
}

/**
 * Thread commissioning petition handler
 * uri = tn/mc/lp
 */
static int thread_leader_service_petition_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    uint8_t payload[79];// max length for commissioner id is 64 + 4 byte header + 4 + 1 + 4 + 2
    uint8_t *ptr;
    uint16_t session_id = 0;
    uint8_t *tlv_data_ptr = NULL;
    uint16_t tlv_length;
    char *commissioner_id_ptr = NULL;

    (void) source_port;

    ptr = payload;
    if (!this) {
        return -1;
    }

    tr_debug("Thread management commissioner petition");

    if (!thread_management_server_source_address_check(this->interface_id, source_address)) {
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto send_error_response;
    }

    // save values from message
    tlv_length = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_ID, &tlv_data_ptr);

    // register to thread interface
    int ret = thread_leader_service_commissioner_register(this->interface_id, source_address, tlv_data_ptr, tlv_length, &session_id);

    // Build response
    if (ret == 0) {
        // Register OK
        ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 1);
        ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_COMMISSIONER_ID, tlv_length, tlv_data_ptr);
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, session_id);
    } else if (ret == -2) {
        // Commissioner already registered - reject and send commissioner ID
        ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);
        commissioner_id_ptr = thread_commissioning_if_commissioner_id_get(this->interface_id);
        if(commissioner_id_ptr)
            ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_COMMISSIONER_ID, strlen(commissioner_id_ptr), (uint8_t *)commissioner_id_ptr);
    }
    else{
        // Reject, anyhow, no commissioner registered - no commissioner ID
        ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);
    }

    tr_debug("Petition req recv id %s, RESP session id: %d ret %d", commissioner_id_ptr, session_id, ret);

send_error_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

/**
 * Thread commissioning keep alive handler
 * uri = tn/mc/la
 */
static int thread_leader_service_petition_ka_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    uint8_t payload[5]; //status 4 + 1
    uint8_t *ptr;
    uint16_t session_id = 0;
    bool state = false;

    (void) source_address;
    (void) source_port;

    if (!this) {
        return -1;
    }

    tr_debug("Thread management commissioner keep alive");

    if (!thread_management_server_source_address_check(this->interface_id, source_address)) {
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        ptr = payload;
        goto send_error_response;
    }

    if (2 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &ptr)) {
        session_id = common_read_16_bit(ptr);
    }
    if (1 <= thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_STATE, &ptr))
    {
        if (*ptr == 0x01) {
            state = thread_leader_service_commissioner_session_refresh(this->interface_id, session_id);
        } else if(*ptr == 0xff) {
            thread_leader_service_commissioner_unregister(this->interface_id, session_id);
            state = false;
        }
    }
    if (state == true) {
        thread_leader_service_commissioner_address_changed(this->interface_id, source_address);
    }
    tr_info("Commissioner keep alive req recv, interface: %d session id %d state %d", this->interface_id, session_id, state);
    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, state == true ? 1 : 0xff);

send_error_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

static int thread_leader_service_network_data_register(protocol_interface_info_entry_t *cur, uint8_t *network_data_ptr, uint16_t network_data_length, uint16_t routerId)
{
    tr_debug("Register network data for %x", routerId);
    // Mark all data to be cleared
    thread_network_data_router_id_mark_delete(&cur->thread_info->networkDataStorage, routerId, true);

    // update data from this device
    int retVal = thread_bootstrap_network_data_process(cur, network_data_ptr, network_data_length);

    if (retVal < 0) {
        tr_warn("network data update failed: %s", trace_array(network_data_ptr, network_data_length));
        return retVal;
    }
    thread_network_data_router_id_free(&cur->thread_info->networkDataStorage, true, cur);

    thread_leader_service_network_data_changed(cur, false, false);
    return 0;
}

/**
 * Thread network data registration handler
 * uri = tn/sd
 */
static int thread_leader_service_register_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_leader_service_t *this = thread_leader_service_find_by_service(service_id);
    uint16_t payload_len;
    uint8_t *payload_ptr;
    uint16_t network_data_len;
    uint8_t *network_data_ptr;
    protocol_interface_info_entry_t *cur;
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    int8_t ret = -3;
    uint16_t routerId;
    uint16_t old_rloc;
    (void) source_port;
    tr_debug("register network data");

    if (!this) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }

    //send data to leader api.
    routerId = common_read_16_bit(&source_address[14]);
    payload_len = request_ptr->payload_len;
    payload_ptr = request_ptr->payload_ptr;
    request_ptr->payload_ptr = NULL;


    if (2 <= thread_tmfcop_tlv_data_get_uint16(payload_ptr, payload_len, TMFCOP_TLV_RLOC16, &old_rloc)) {
        // This will delete everything from old rloc
        tr_warn("Remove network data from: %x", old_rloc);
        ret = thread_leader_service_network_data_register(cur, NULL, 0, old_rloc);
    }

    if (!thread_tmfcop_tlv_exist(payload_ptr, payload_len, TMFCOP_TLV_NETWORK_DATA)) {
        goto send_response;
    }
    network_data_len = thread_tmfcop_tlv_find(payload_ptr, payload_len, TMFCOP_TLV_NETWORK_DATA, &network_data_ptr);

    /* Check that network data does NOT produce Network Data TLV bigger than MAX_NETWORK_DATA_SIZE */
    int32_t len = thread_network_data_resulting_tlv_size(&cur->thread_info->networkDataStorage, network_data_ptr, network_data_len, routerId);

    if (len <= 0 || len > THREAD_MAX_NETWORK_DATA_SIZE) {
        tr_warn("Network data would produce Network Data TLV size %"PRId32, len);
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto send_response;
    }

    if (network_data_len < 256) {
        ret = thread_leader_service_network_data_register(cur, network_data_ptr, network_data_len, routerId);
    }
    if (ret != 0) {
        /* error happened*/
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST; // corruption is the default or not enough room
        if (ret == -2) {
            response_code = COAP_MSG_CODE_RESPONSE_FORBIDDEN;    // Not leader
        }
    }

send_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, NULL, 0);
    return 0;
}

static int thread_leader_service_leader_start(protocol_interface_info_entry_t *cur)
{
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return -1;
    }

    //Allocate Setup
    if (thread_leader_service_leader_data_allocate(cur) != 0) {
        return -1;
    }

    if (thread_leader_service_start(cur->id)) {
        thread_nd_service_delete(cur->id);
        return -1;
    }
    if (thread_beacon_create_payload(cur) != 0) {
        thread_nd_service_delete(cur->id);
        return -1;
    }
    cur->thread_info->rfc6775 = false;
    cur->thread_info->leader_private_data->leader_id_seq_timer = ID_SEQUENCE_PERIOD;

    cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
    cur->lowpan_info |= INTERFACE_NWK_ROUTER_DEVICE;
    cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    mac_helper_default_security_level_set(cur, SEC_ENC_MIC32);
    mac_helper_default_security_key_id_mode_set(cur,MAC_KEY_ID_MODE_IDX);
    thread_discovery_responser_enable(cur->id, true);
    if (cur->mac_api) {
        mlme_start_t start_req;
        memset(&start_req, 0, sizeof(mlme_start_t));
        start_req.BeaconOrder = 15;
        start_req.SuperframeOrder = 15;
        start_req.PANCoordinator = true;
        start_req.LogicalChannel = cur->mac_parameters->mac_channel;
        start_req.PANId = cur->mac_parameters->pan_id;
        cur->mac_api->mlme_req(cur->mac_api, MLME_START , &start_req);
    }
    return 0;
}

static int thread_leader_service_leader_init(protocol_interface_info_entry_t *cur)
{
    //Clean All allocated stuff's
    thread_info_t *thread_info = cur->thread_info;
    // mark and delete previous leader network data information
    thread_network_data_router_id_mark_delete(&thread_info->networkDataStorage,thread_router_addr_from_id(cur->thread_info->thread_leader_data->leaderRouterId), true);
    thread_network_data_router_id_free(&thread_info->networkDataStorage, false, cur);
    thread_leader_service_leader_data_free(thread_info);

    thread_info->rfc6775 = false;
    thread_routing_free(&thread_info->routing);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD, NULL);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_BORDER_ROUTER, NULL);
    thread_dhcp_client_delete(cur->id);
    thread_nd_service_delete(cur->id);
    mpl_clear_realm_scope_seeds(cur);
    ipv6_neighbour_cache_flush(&cur->ipv6_neighbour_cache);
    thread_clean_all_routers_from_neighbor_list(cur->id);
    cur->mesh_callbacks = NULL;
    cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;

    return thread_leader_service_leader_start(cur);
}

static void thread_leader_service_leader_data_initialize(protocol_interface_info_entry_t *cur, uint8_t routerId)
{
    thread_leader_data_t *leader_data = cur->thread_info->thread_leader_data;

    if (cur->thread_info->thread_leader_data) {
        leader_data->partitionId = randLIB_get_32bit(); //Generate Random Instance
        leader_data->leaderRouterId = routerId; //Set leader data to zero by Default
        leader_data->dataVersion = randLIB_get_8bit();
        leader_data->stableDataVersion = randLIB_get_8bit();
        leader_data->weighting = cur->thread_info->partition_weighting;
    }
}

static void thread_leader_service_private_routemask_init(thread_leader_info_t *leader_info)
{
    memset(leader_info, 0, sizeof(thread_leader_info_t));
    leader_info->maskSeq = randLIB_get_8bit();
    uint8_t i;
    for (i=0; i<64; i++) {
        leader_info->thread_router_id_list[i].reUsePossible = true;
        leader_info->thread_router_id_list[i].validLifeTime = 0xffffffff;
    }
}

static void thread_leader_service_interface_setup_activate(protocol_interface_info_entry_t *cur)
{
    uint8_t routerId;
    thread_leader_info_t *private = cur->thread_info->leader_private_data;

    routerId = thread_router_id_from_addr(cur->thread_info->routerShortAddress);

    thread_leader_service_leader_data_initialize(cur, routerId);
    // Test code
    if(cur->thread_info->testRandomPartitionId != 0){
        cur->thread_info->thread_leader_data->partitionId = cur->thread_info->testRandomPartitionId;
        cur->thread_info->testRandomPartitionId = 0;
    }
    thread_leader_service_private_routemask_init(private);
    //SET Router ID
    thread_leader_allocate_router_id_by_allocated_id(private, routerId, cur->mac);
    thread_old_partition_data_purge(cur);
    cur->lowpan_address_mode = NET_6LOWPAN_GP16_ADDRESS;
    thread_bootstrap_update_ml16_address(cur, cur->thread_info->routerShortAddress);
    thread_generate_ml64_address(cur);
    thread_extension_address_generate(cur);
    thread_bootstrap_routing_activate(cur);
    thread_routing_update_id_set(cur, private->maskSeq, private->master_router_id_mask);
    thread_routing_activate(&cur->thread_info->routing);
    cur->thread_info->leader_private_data->leader_id_seq_timer = ID_SEQUENCE_PERIOD;
}

/*
 * Function to clear network data from router children.
 * When router is removed, network data from router child must be removed as well.
 */
static void thread_leader_router_child_network_data_clear(protocol_interface_info_entry_t *cur, uint16_t router_short_addr)
{
    uint16_t child_short_addr;

    child_short_addr = thread_leader_service_child_id_from_networkdata_get(cur->thread_info, router_short_addr);
    while (child_short_addr != 0) {
        tr_debug("Removing Network data from child %04x", child_short_addr);
        thread_leader_service_network_data_register(cur, NULL, 0, child_short_addr);
        child_short_addr = thread_leader_service_child_id_from_networkdata_get(cur->thread_info, router_short_addr);
    }
}


/* Public functions */

int thread_leader_service_init(int8_t interface_id, uint8_t coap_service_id)
{
    thread_leader_service_t *this = thread_leader_service_find_by_interface(interface_id);

    if (this) {
        return 0;
    }

    this = ns_dyn_mem_alloc(sizeof(thread_leader_service_t));
    if (!this) {
        return -2;
    }

    this->interface_id = interface_id;
    this->coap_service_id = coap_service_id;
    this->leader_enabled = false;

    ns_list_add_to_start(&leader_service_instance_list, this);

    return 0;
}

void thread_leader_service_delete(int8_t interface_id)
{
    thread_leader_service_t *this = thread_leader_service_find_by_interface(interface_id);
    if (!this) {
        return;
    }

    if (this->leader_enabled) {
        thread_leader_service_stop(interface_id);
    }

    ns_list_remove(&leader_service_instance_list, this);
    ns_dyn_mem_free(this);
}

int thread_leader_service_start(int8_t interface_id)
{
    thread_leader_service_t *this = thread_leader_service_find_by_interface(interface_id);

    if (!this) {
        return -1;
    }

    this->leader_enabled = true;
    /**
     * Router id uri register
     */
    coap_service_register_uri(this->coap_service_id, THREAD_URI_ROUTER_ID_ASSIGNMENT, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_assign_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_ROUTER_ID_RELEASE, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_release_cb);
    /**
     * Thread commission registration uri handling
     */
    coap_service_register_uri(this->coap_service_id, THREAD_URI_LEADER_PETITION, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_petition_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_LEADER_KEEP_ALIVE, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_petition_ka_cb);
    /**
     * Thread network data management handler
     */
    coap_service_register_uri(this->coap_service_id, THREAD_URI_NETWORK_DATA, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_register_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_ACTIVE_SET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_active_set_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_PENDING_SET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_pending_set_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_COMMISSIONER_SET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_leader_service_commissioner_set_cb);

    return 0;
}

void thread_leader_service_stop(int8_t interface_id)
{
    thread_leader_service_t *this = thread_leader_service_find_by_interface(interface_id);

    if (!this) {
        return;
    }

    this->leader_enabled = false;
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_ROUTER_ID_ASSIGNMENT);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_ROUTER_ID_RELEASE);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_LEADER_PETITION);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_LEADER_KEEP_ALIVE);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_NETWORK_DATA);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_ACTIVE_SET);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_PENDING_SET);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_COMMISSIONER_SET);
    return;
}
void thread_leader_service_network_data_changed(protocol_interface_info_entry_t *cur, bool force_stable_update, bool force_unstable_update)
{
    if (!cur->thread_info) {
        return;
    }

    if (force_stable_update) {
        cur->thread_info->networkDataStorage.stableUpdatePushed = true;
    }
    if (force_unstable_update) {
        cur->thread_info->networkDataStorage.temporaryUpdatePushed = true;
    }
    // Generate network data from network data structures
    if (cur->thread_info->networkDataStorage.stableUpdatePushed) {
        tr_debug("Temporary Network Data Update");
        cur->thread_info->thread_leader_data->stableDataVersion++;
        cur->thread_info->thread_leader_data->dataVersion++;
    } else if (cur->thread_info->networkDataStorage.temporaryUpdatePushed) {
        tr_debug("Stable Network Data Update");
        cur->thread_info->thread_leader_data->dataVersion++;
    } else {
        // No changes in network data detected
        return;
    }
    tr_debug("network data changed stable:%d, unstable:%d", cur->thread_info->networkDataStorage.stableUpdatePushed, cur->thread_info->networkDataStorage.temporaryUpdatePushed);
    thread_leader_service_generate_network_data(cur);

    /* set timer to update new network data */
    if (0 == thread_info(cur)->networkDataStorage.network_data_update_delay) {
        thread_info(cur)->networkDataStorage.network_data_update_delay = THREAD_COMMISSION_DATA_UPDATE_DELAY;
    }
}

void thread_leader_service_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
    if (!cur->thread_info->leader_private_data) {
        // I am not a leader
        return;
    }

    // check for network data update
    if (thread_info(cur)->networkDataStorage.network_data_update_delay) {
        if (thread_info(cur)->networkDataStorage.network_data_update_delay > ticks) {
            thread_info(cur)->networkDataStorage.network_data_update_delay -= ticks;
        } else {
            // Send New network data.
            thread_info(cur)->networkDataStorage.network_data_update_delay = 0;
            thread_bootstrap_network_data_update(cur);
        }
    }
    if (cur->thread_info->leader_private_data->leader_id_seq_timer) {
        if (cur->thread_info->leader_private_data->leader_id_seq_timer > ticks) {
            cur->thread_info->leader_private_data->leader_id_seq_timer -= ticks;
        } else {
            cur->thread_info->leader_private_data->leader_id_seq_timer = 0;
        }
    } else  {
        // Leader id sequence number updated
        cur->thread_info->leader_private_data->leader_id_seq_timer = ID_SEQUENCE_PERIOD;

        thread_leader_service_update_id_set(cur);
        thread_network_data_context_re_use_timer_update(&cur->thread_info->networkDataStorage, 10, &cur->lowpan_contexts);
        // Send update to network data if needed
        thread_leader_service_network_data_changed(cur, false, false);
    }

    thread_leader_service_router_id_valid_lifetime_update(cur, ticks);

}

void thread_leader_service_leader_data_free(thread_info_t *thread_info)
{
    if (thread_info && thread_info->thread_leader_data) {
        ns_dyn_mem_free(thread_info->leader_private_data);
        thread_info->leader_private_data = NULL;
    }
}

void thread_leader_service_thread_partitition_generate(int8_t interface_id, bool newPartition)
{
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(interface_id);
    uint8_t *parent_mac_addr = NULL;
    uint16_t leaderRloc;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return;
    }

    if (!linkConfiguration) {
        goto failure_exit;
    }

    mle_service_interface_tx_queue_clean(interface_id);

    if (!newPartition) {
        if (cur->nwk_bootstrap_state == ER_ACTIVE_SCAN && cur->nwk_nd_re_scan_count < THREAD_NETWORK_ACTIVE_SCAN_COUNTER) {
            tr_debug("RE trig Active Scan");
            cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(1, 6);
            return;
        }
    }

    if (cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_ROUTER) {
        tr_debug("End device did not find networks");
        goto failure_exit;
    }
    leaderRloc = cur->thread_info->routerShortAddress;

    //Set For Destination cache clean purpose
    cur->mac_parameters->pan_id = linkConfiguration->panId;

    if (thread_leader_service_leader_init(cur) != 0) {
        tr_debug("Leader allocate Fail");
        goto failure_exit;
    }

    /* Delete old parent info */
    if (cur->thread_info->thread_endnode_parent) {
        ns_dyn_mem_free(cur->thread_info->thread_endnode_parent);
        cur->thread_info->thread_endnode_parent = NULL;
    }

    tr_debug("Leader Link generated & Started");

    //Run Nomal Init
    thread_interface_init(cur);

    // Init clears the address
    cur->thread_info->routerShortAddress = leaderRloc;

    if (cur->thread_info->routerShortAddress == 0xfffe ||
        !thread_is_router_addr(cur->thread_info->routerShortAddress)) {
        //If we were REED do not use reed address
        cur->thread_info->routerShortAddress = thread_router_addr_from_id(randLIB_get_random_in_range(0,MAX_ROUTER_ID));
    }

    // clean network data
    thread_network_data_free_and_clean(&cur->thread_info->networkDataStorage);
    thread_network_data_base_init(&cur->thread_info->networkDataStorage);
    thread_leader_service_interface_setup_activate(cur);

    //memset(cur->thread_info->lastValidRouteMask, 0, (N_THREAD_ROUTERS+7)/8);
    //SET SHort Address & PAN-ID
    cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    mle_class_mode_set(cur->id, MLE_CLASS_ROUTER);
    mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
    cur->interface_mode = INTERFACE_UP;
    cur->nwk_mode = ARM_NWK_GP_IP_MODE;
    thread_bootstrap_ready(cur);
    thread_configuration_mle_activate(cur);
    if (cur->thread_info->thread_endnode_parent) {
        parent_mac_addr = cur->thread_info->thread_endnode_parent->mac64;
    }
    thread_nvm_store_link_info_write(parent_mac_addr, mac_helper_mac16_address_get(cur));

    if (thread_nd_own_service_list_data_size(&cur->thread_info->localServerDataBase)) {
        // We publish our services if we have some BUG leader cannot remove old ones
        thread_border_router_publish(cur->id);
    }
    return;

failure_exit:
    nwk_bootsrap_state_update(ARM_NWK_NWK_SCAN_FAIL, cur);
}

int thread_leader_service_thread_partitition_restart(int8_t interface_id, mle_tlv_info_t *routing)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    if (!thread_info(cur)->leader_private_data) {
        thread_info(cur)->leader_private_data = thread_allocate_and_init_leader_private_data();
    }
    if (!thread_info(cur)->leader_private_data) {
        return -1;
    }
    if (thread_leader_service_start(cur->id)) {
        return -1;
    }
    //Learn network data, we remove own data from here it should be re given by application
    //thread_management_network_data_register(cur->id, networkData.dataPtr, networkData.tlvLen, address16 );

    // initialize private data
    thread_info(cur)->leader_private_data->maskSeq = *routing->dataPtr;
    memcpy(thread_info(cur)->leader_private_data->master_router_id_mask,routing->dataPtr + 1,8);
    for (int i = 0; i < 64; i++) {
        memset(thread_info(cur)->leader_private_data->thread_router_id_list[i].eui64,0,8);
        if (bit_test(thread_info(cur)->leader_private_data->master_router_id_mask, i)) {
            //Active ID
            thread_info(cur)->leader_private_data->thread_router_id_list[i].reUsePossible = false;

        } else {
            // Free id
            thread_info(cur)->leader_private_data->thread_router_id_list[i].reUsePossible = true;
        }
        thread_info(cur)->leader_private_data->thread_router_id_list[i].validLifeTime = 0xffffffff;
    }
    // Clear network data (if exists) and propagate new empty network data
    thread_network_data_free_and_clean(&cur->thread_info->networkDataStorage);
    thread_network_data_base_init(&cur->thread_info->networkDataStorage);
    return 0;
}

void thread_leader_service_update_id_set(protocol_interface_info_entry_t *cur)
{
    thread_leader_info_t *leader_private_ptr = cur->thread_info->leader_private_data;

    leader_private_ptr->maskSeq++;
    thread_routing_update_id_set(cur, leader_private_ptr->maskSeq, leader_private_ptr->master_router_id_mask);
}

void thread_leader_service_router_id_valid_lifetime_update(protocol_interface_info_entry_t *cur, uint32_t tickUpdate)
{
    uint8_t i;
    bool update_routeTable = false;
    thread_info_t *thread_info = cur->thread_info;
    thread_leader_info_t *leader_private_ptr = 0;
    if (thread_info && thread_info->leader_private_data) {
        leader_private_ptr = thread_info->leader_private_data;

        for (i = 0; i < 64; i++) {
            if (bit_test(leader_private_ptr->master_router_id_mask, i)) {
                if (i != thread_info->thread_leader_data->leaderRouterId) {
                    //Active ID
                    if (leader_private_ptr->thread_router_id_list[i].validLifeTime != 0xffffffff) {
                        if (leader_private_ptr->thread_router_id_list[i].validLifeTime > tickUpdate) {
                            //Increment Time
                            leader_private_ptr->thread_router_id_list[i].validLifeTime -= tickUpdate;
                        } else {
                            //RElease
                            tr_debug("Router ID bit Release: %x start re assign delay", i);
                            leader_private_ptr->thread_router_id_list[i].validLifeTime = ROUTER_ID_REUSE_DELAY;
                            leader_private_ptr->thread_router_id_list[i].reUsePossible = false;
                            thread_leader_service_route_mask_bit_clear(leader_private_ptr, i);
                            thread_leader_service_network_data_register(cur, NULL, 0, thread_router_addr_from_id(i));
                            thread_leader_router_child_network_data_clear(cur, thread_router_addr_from_id(i));
                            thread_routing_activate(&thread_info->routing);
                            update_routeTable = true;
                        }
                    }
                }
            } else {
                if (leader_private_ptr->thread_router_id_list[i].validLifeTime != 0xffffffff) {
                    if (leader_private_ptr->thread_router_id_list[i].validLifeTime > tickUpdate) {
                        //Increment Time
                        leader_private_ptr->thread_router_id_list[i].validLifeTime -= tickUpdate;
                    } else {
                        //RElease
                        tr_debug("Router ID available again: %x", i);
                        leader_private_ptr->thread_router_id_list[i].validLifeTime = 0xffffffff;
                        leader_private_ptr->thread_router_id_list[i].reUsePossible = true;
                    }
                }

            }
        }
        if (update_routeTable) {
            thread_leader_service_update_id_set(cur);
        }
    }
}

bool thread_leader_service_route_mask_bit_clear(thread_leader_info_t *info, uint8_t router_id)
{
    if (bit_test(info->master_router_id_mask, router_id)) {
        bit_clear(info->master_router_id_mask, router_id);
        return true;
    }
    return false;
}

static uint8_t thread_unstable_commission_data_length(protocol_interface_info_entry_t *cur)
{
    uint8_t localDataLength = 0;

    localDataLength += 4; // Commissioner session ID

    if (cur->thread_info->registered_commissioner.commissioner_valid) {
        localDataLength += 4; // Border router address
        if (cur->thread_info->registered_commissioner.steering_data_len) {
            localDataLength += cur->thread_info->registered_commissioner.steering_data_len;
            localDataLength += 2;
        }
    }
    return localDataLength;
}

static uint8_t *thread_unstable_commission_data_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{

    uint8_t length = thread_unstable_commission_data_length(cur);
    if( !length) {
        // No unstable TLV in thread 1.0 or if TLV is empty we skip it
        return ptr;
    }
    *ptr++ = THREAD_NWK_DATA_TYPE_COMMISSION_DATA ;
    *ptr++ = length;
    ptr = thread_meshcop_tlv_data_write_uint16(ptr,MESHCOP_TLV_COMMISSIONER_SESSION_ID, cur->thread_info->registered_commissioner.session_id);
    if (cur->thread_info->registered_commissioner.commissioner_valid) {
        //SET Commision TLV
        ptr = thread_meshcop_tlv_data_write(ptr,MESHCOP_TLV_BORDER_ROUTER_LOCATOR, 2, &cur->thread_info->registered_commissioner.border_router_address[14]);
        if (cur->thread_info->registered_commissioner.steering_data_len) {
            ptr = thread_meshcop_tlv_data_write(ptr,MESHCOP_TLV_STEERING_DATA, cur->thread_info->registered_commissioner.steering_data_len, cur->thread_info->registered_commissioner.steering_data);
        }
    }
    return ptr;
}

static uint16_t thread_leader_service_network_data_length(protocol_interface_info_entry_t *cur, bool fullist)
{
    uint16_t networkDataLength = 0;
    uint16_t prefixDataLength = 0;
    uint16_t commissionDataLength = 0;

    prefixDataLength = thread_network_data_prefix_set_size(&cur->thread_info->networkDataStorage, fullist);
    if (prefixDataLength) {
        networkDataLength += prefixDataLength;
    }
    prefixDataLength = thread_network_data_service_set_size(&cur->thread_info->networkDataStorage, fullist);
    if (prefixDataLength) {
        networkDataLength += prefixDataLength;
    }
    commissionDataLength = thread_unstable_commission_data_length(cur);
    if (fullist && commissionDataLength) {
        networkDataLength += 2 + commissionDataLength;
    }

    return networkDataLength;
}

static uint8_t *thread_leader_service_network_data_tlv_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    ptr = thread_network_data_prefix_set_write(&cur->thread_info->networkDataStorage, ptr);
    ptr = thread_network_data_service_set_write(&cur->thread_info->networkDataStorage, ptr);
    ptr = thread_unstable_commission_data_write(cur, ptr);

    return ptr;
}

void thread_leader_service_generate_network_data(protocol_interface_info_entry_t *cur)
{
    int16_t size = thread_leader_service_network_data_length(cur, true);

    if (size > 0) {
        tr_debug("Leader network data size %d", size);
        thread_leader_service_network_data_tlv_write(cur, cur->thread_info->networkDataStorage.network_data);
        cur->thread_info->networkDataStorage.network_data_len = size;
    }
}

uint16_t thread_leader_service_child_id_from_networkdata_get(thread_info_t *thread_info, uint16_t router_short_addr)
{
    return thread_network_data_service_child_id_from_networkdata_get(&thread_info->networkDataStorage, router_short_addr);
}

void thread_leader_service_router_state_changed(thread_info_t *thread_info, uint8_t router_id, bool available, int8_t interface_id)
{
    thread_leader_info_t *info = thread_info->leader_private_data;
    (void)interface_id;

    if (!info) {
        return;
    }
    if (available) {
        /* Stop the timer */
        info->thread_router_id_list[router_id].validLifeTime = 0xffffffff;
    } else {
        /* Reset the timer to ROUTER_ID_REUSE_DELAY, if it's not already running. */
        if (info->thread_router_id_list[router_id].validLifeTime == 0xffffffff) {
            tr_debug("Router ID: %d cost infinite", router_id);

            info->thread_router_id_list[router_id].validLifeTime = ROUTER_ID_INFINITY_DELAY;
            info->thread_router_id_list[router_id].reUsePossible = false;
        }
    }
}

#endif // HAVE_THREAD_LEADER_SERVICE */
