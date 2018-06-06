/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ns_nvm_helper.h"
#include "randLIB.h"
#include "common_functions.h"
#include "thread_tmfcop_lib.h"
#include "ns_sha256.h"

#include "NWK_INTERFACE/Include/protocol.h"
#include "Service_Libs/blacklist/blacklist.h"

#include "coap_service_api.h"

#include "eventOS_event_timer.h"
#include "thread_discovery.h"
#include "thread_beacon.h"
#include "thread_config.h"
#include "thread_meshcop_lib.h"
#include "thread_commissioning_if.h"
#include "thread_management_if.h"
#include "thread_common.h"
#include "thread_bootstrap.h"
#include "thread_network_synch.h"
#include "thread_network_data_lib.h"
#include "thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_extension_bootstrap.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "thread_nvm_store.h"
#include "thread_management_internal.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_JOINER_APPLICATION

/* List of active/pending configuration dataset TLVs*/
const uint8_t mle_pending_configuration_dataset_tlvs[] = {
        MESHCOP_TLV_ACTIVE_TIME_STAMP,
        MESHCOP_TLV_NETWORK_NAME,
        MESHCOP_TLV_PANID,
        MESHCOP_TLV_CHANNEL,
        MESHCOP_TLV_CHANNEL_MASK,
        MESHCOP_TLV_XPANID,
        MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA,
        MESHCOP_TLV_NETWORK_MASTER_KEY,
        MESHCOP_TLV_PSKC,
        MESHCOP_TLV_SECURITY_POLICY};

const uint8_t pending_configuration_dataset_tlvs[] = {
        MESHCOP_TLV_PENDING_TIMESTAMP,
        MESHCOP_TLV_ACTIVE_TIME_STAMP,
        MESHCOP_TLV_NETWORK_NAME,
        MESHCOP_TLV_PANID,
        MESHCOP_TLV_CHANNEL,
        MESHCOP_TLV_CHANNEL_MASK,
        MESHCOP_TLV_XPANID,
        MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA,
        MESHCOP_TLV_NETWORK_MASTER_KEY,
        MESHCOP_TLV_PSKC,
        MESHCOP_TLV_SECURITY_POLICY};

const uint8_t active_configuration_dataset_tlvs[] = {
        MESHCOP_TLV_ACTIVE_TIME_STAMP,
        MESHCOP_TLV_NETWORK_NAME,
        MESHCOP_TLV_PANID,
        MESHCOP_TLV_CHANNEL,
        MESHCOP_TLV_CHANNEL_MASK,
        MESHCOP_TLV_XPANID,
        MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA,
        MESHCOP_TLV_NETWORK_MASTER_KEY,
        MESHCOP_TLV_PSKC,
        MESHCOP_TLV_SECURITY_POLICY};

const uint8_t mle_active_configuration_dataset_tlvs[] = {
        MESHCOP_TLV_NETWORK_NAME,
        MESHCOP_TLV_PANID,
        MESHCOP_TLV_CHANNEL,
        MESHCOP_TLV_CHANNEL_MASK,
        MESHCOP_TLV_XPANID,
        MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA,
        MESHCOP_TLV_NETWORK_MASTER_KEY,
        MESHCOP_TLV_PSKC,
        MESHCOP_TLV_SECURITY_POLICY};

const uint8_t entrust_dataset_tlvs[] = {
        MESHCOP_TLV_ACTIVE_TIME_STAMP,
        MESHCOP_TLV_NETWORK_NAME,
        MESHCOP_TLV_CHANNEL_MASK,// Missing specification
        MESHCOP_TLV_XPANID,
        MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA,
        MESHCOP_TLV_NETWORK_MASTER_KEY,
        MESHCOP_TLV_PSKC,
        MESHCOP_TLV_SECURITY_POLICY,
        MESHCOP_TLV_NETWORK_KEY_SEQUENCE};

const uint8_t meshcop_pending_set_ignore[] = {
        MESHCOP_TLV_BORDER_ROUTER_LOCATOR,
        MESHCOP_TLV_COMMISSIONER_SESSION_ID,
        MESHCOP_TLV_PENDING_TIMESTAMP,
        MESHCOP_TLV_COMMISSIONER_ID,
        MESHCOP_TLV_STEERING_DATA,
        MESHCOP_TLV_DELAY_TIMER,
        MESHCOP_TLV_GET,
        MESHCOP_TLV_COMMISSIONER_UDP_PORT,
        MESHCOP_TLV_JOINER_UDP_PORT,
        MESHCOP_TLV_STATE,
        MESHCOP_TLV_NETWORK_KEY_SEQUENCE
};

const uint8_t meshcop_active_set_ignore[] = {
        MESHCOP_TLV_PENDING_TIMESTAMP,
        MESHCOP_TLV_COMMISSIONER_SESSION_ID,
        MESHCOP_TLV_COMMISSIONER_ID,
        MESHCOP_TLV_STEERING_DATA,
        MESHCOP_TLV_DELAY_TIMER,
        MESHCOP_TLV_NETWORK_KEY_SEQUENCE
        };

const uint8_t mle_active_configuration_dataset_ignore_tlvs[] = {
        MESHCOP_TLV_ACTIVE_TIME_STAMP
};

const uint8_t mle_pending_configuration_dataset_ignore_tlvs[] = {
        MESHCOP_TLV_PENDING_TIMESTAMP
};

uint8_t mle_active_configuration_dataset_tlvs_size(void)
{
    return sizeof(mle_active_configuration_dataset_tlvs);
}

uint8_t active_configuration_dataset_tlvs_size(void)
{
    return sizeof(active_configuration_dataset_tlvs);
}

uint8_t mle_active_configuration_dataset_ignore_tlvs_size(void)
{
    return sizeof(mle_active_configuration_dataset_ignore_tlvs);
}

uint8_t mle_pending_configuration_dataset_ignore_tlvs_size(void)
{
    return sizeof(mle_pending_configuration_dataset_ignore_tlvs);
}

uint8_t mle_pending_configuration_dataset_tlvs_size(void)
{
    return sizeof(mle_pending_configuration_dataset_tlvs);
}
uint8_t pending_configuration_dataset_tlvs_size(void)
{
    return sizeof(pending_configuration_dataset_tlvs);
}
uint8_t entrust_dataset_tlvs_size(void)
{
    return sizeof(entrust_dataset_tlvs);
}
#define MAX_OPERATIONAL_DATASET_SIZE 254

typedef struct {
	uint32_t timeout_in_ms;
	uint64_t timestamp;
	uint16_t length;
	uint8_t data[MAX_OPERATIONAL_DATASET_SIZE];
} configuration_set_t;

typedef struct {
    link_configuration_s *configuration_ptr;
    configuration_set_t *active_configuration_ptr;
    configuration_set_t *pending_configuration_ptr;
    configuration_set_t *old_active_configuration_ptr;
    configuration_set_t *next_active_configuration_ptr;
    configuration_set_t *next_pending_configuration_ptr;
    device_configuration_s *device_configuration_ptr;
    uint8_t parent_address[16];
    uint16_t parent_port;
    thread_joiner_application_commission_done_cb *done_cb;
    timeout_t *attach_timeout;
    timeout_t *entrust_timeout;
    uint32_t provisioning_timeout;
    thread_provisioning_status_e provisioning_done;
    bool configuration_valid: 1; //TODO this should???? be put in link configuration
    bool finalisation_done: 1;
    bool nvm_link_configuration_load: 1; // load link configuration settings from NVM in restart
    int8_t interface_id;
    int8_t coap_service_id;
    int8_t secure_coap_service_id;
    bool pending_set_in_sync:1;
    ns_list_link_t link;
} thread_joiner_t;


static NS_LIST_DEFINE(instance_list, thread_joiner_t, link);

static int thread_joiner_application_nvm_link_config_read(thread_joiner_t *this);
static bool thread_joiner_application_validate_settings(thread_joiner_t *this);
static uint8_t *thread_joiner_application_write_channel(uint8_t *ptr, uint16_t data);

static int stringlen(const char *s, int n)
{
    char *end = memchr(s, 0, n);
    return end?end-s:n;
}

static char *str_dub(char *str)
{
    char *result_ptr = NULL;
    if (str) {
        result_ptr = ns_dyn_mem_alloc(strlen(str) + 1);
        if(result_ptr) {
            strcpy(result_ptr,str);
        }
    }
    return result_ptr;
}

static thread_joiner_t *thread_joiner_find(int8_t interface_id)
{
    thread_joiner_t *this = NULL;
    ns_list_foreach(thread_joiner_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static thread_joiner_t *thread_joiner_find_by_service(int8_t service_id)
{
    thread_joiner_t *this = NULL;
    ns_list_foreach(thread_joiner_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id || cur_ptr->secure_coap_service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static thread_joiner_t *thread_joiner_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        this = ns_dyn_mem_alloc(sizeof(thread_joiner_t));
        if(this){
            memset(this, 0, sizeof(thread_joiner_t));
            ns_list_add_to_start(&instance_list, this);
            this->interface_id = interface_id;
            this->provisioning_done = PROVISIONING_STATUS_DONE; // Default value for provisioning is done
        }
    }
    return this;
}

static void thread_joiner_application_commission_clean(thread_joiner_t *this)
{
    thread_commissioning_if_pairwise_key_delete_all(this->interface_id);
    memset(this->parent_address,0,16);
    this->parent_port = 0;
    coap_service_delete(this->secure_coap_service_id);
    coap_service_delete(this->coap_service_id);
    this->secure_coap_service_id = 0;
    this->coap_service_id = 0;
    eventOS_timeout_cancel(this->attach_timeout);
    eventOS_timeout_cancel(this->entrust_timeout);
    this->attach_timeout = NULL;
    this->entrust_timeout = NULL;
}

//@TODO wrong place move these
//@TODO Steering data is wrongly made it is not part of static configuration
//@TODO PSKc is static data and should always be present not dynamic.
static link_configuration_s *link_configuration_create(void)
{
    link_configuration_s *this = ns_dyn_mem_alloc(sizeof(link_configuration_s));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(link_configuration_s));
    this->securityPolicy = SECURITY_POLICY_ALL_SECURITY;        // Set all default values ('1') for security policy flags
    return this;
}

static void link_configuration_delete(link_configuration_s *this)
{
    if (!this) {
        return;
    }
    ns_dyn_mem_free(this);
}

static void link_configuration_copy(link_configuration_s *this, link_configuration_s *configuration_ptr)
{
    if (!this || !configuration_ptr) {
        return;
    }
    memcpy(this->name, configuration_ptr->name, 16);
    memcpy(this->PSKc, configuration_ptr->PSKc, 16);
    memcpy(this->master_key, configuration_ptr->master_key, 16);
    memcpy(this->mesh_local_ula_prefix, configuration_ptr->mesh_local_ula_prefix, 8);
    memcpy(this->extented_pan_id, configuration_ptr->extented_pan_id, 8);
    memcpy(this->channel_mask, configuration_ptr->channel_mask,5);
    this->key_rotation = configuration_ptr->key_rotation;
    this->key_sequence = configuration_ptr->key_sequence;
    this->panId = configuration_ptr->panId;
    this->rfChannel = configuration_ptr->rfChannel;
    this->securityPolicy = configuration_ptr->securityPolicy;
    this->timestamp = configuration_ptr->timestamp;
    return;
}
static int link_configuration_update(link_configuration_s *link_configuration, uint8_t *msg_ptr, uint16_t msg_len)
{
    uint8_t *ptr;
    uint16_t len;

    if( !msg_ptr || !link_configuration ){
        return -1;
    }
    len = thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_NETWORK_NAME, &ptr);
    if ( len > 0 ) {
        memset(link_configuration->name, 0, 16);
        memcpy(link_configuration->name, ptr, (len > 16) ? 16 : len);
    }

    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_CHANNEL, &ptr) >= 3) {
        //channel_page = channel_ptr[0];
        link_configuration->rfChannel = common_read_16_bit(&ptr[1]);
    }
    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_CHANNEL_MASK, &ptr) > 5) {
        //channel_page = channel_ptr[0];
        if(ptr[0] == 0 && ptr[1] == 4) {
            link_configuration->channel_page = ptr[0];
            memcpy(link_configuration->channel_mask,&ptr[2],4);
        } else {
            tr_warn("Invalid channel mask");
        }
    }
    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_XPANID, &ptr) >= 8) {
        memcpy(link_configuration->extented_pan_id, ptr, 8);
    }

    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA, &ptr) >= 8) {
        memcpy(link_configuration->mesh_local_ula_prefix, ptr, 8);
    }

    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_NETWORK_MASTER_KEY, &ptr) >= 16) {
        memcpy(link_configuration->master_key, ptr, 16);
    }

    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_ACTIVE_TIME_STAMP, &ptr) >= 8) {
        link_configuration->timestamp = common_read_64_bit(ptr);
    }
    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_PANID, &ptr) >= 2) {
        link_configuration->panId = common_read_16_bit(ptr);
    }
    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_PSKC, &ptr) >= 16) {
        memcpy(link_configuration->PSKc, ptr, 16);
    }

    if (thread_meshcop_tlv_find(msg_ptr, msg_len, MESHCOP_TLV_SECURITY_POLICY, &ptr) >= 3) {
        link_configuration->securityPolicy = ptr[2];
        link_configuration->key_rotation = common_read_16_bit(ptr);
    }

    return 0;
}

static void link_configuration_trace(link_configuration_s *this)
{
    if (!this) {
        return;
    }
    tr_debug("NwkName: %s", trace_array(this->name, 16));
    tr_debug("Mesh ula: %s", trace_array(this->mesh_local_ula_prefix, 8));
    tr_debug("Extendend PanId: %s", trace_array(this->extented_pan_id, 8));
    tr_debug("panid: %"PRIu16", Channel:%"PRIu16", keyRot:%"PRIu16", keySeq:%"PRIu32, this->panId, this->rfChannel, this->key_rotation, this->key_sequence);
    return;
}

static device_configuration_s *device_configuration_create(void)
{
    device_configuration_s *this = ns_dyn_mem_alloc(sizeof(device_configuration_s));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(device_configuration_s));
    return this;
}

static void device_configuration_delete(device_configuration_s *this)
{
    if (!this) {
        return;
    }
    ns_dyn_mem_free(this->PSKd_ptr);
    ns_dyn_mem_free(this->provisioning_uri_ptr);
    ns_dyn_mem_free(this->vendor_name_ptr);
    ns_dyn_mem_free(this->vendor_model_ptr);
    ns_dyn_mem_free(this->vendor_sw_version_ptr);
    ns_dyn_mem_free(this->vendor_data_ptr);
    ns_dyn_mem_free(this);
}
static void device_configuration_copy(device_configuration_s *this, device_configuration_s *configuration_ptr)
{
    if (!this || !configuration_ptr) {
        return;
    }
    memcpy(this->eui64, configuration_ptr->eui64,8);
    memcpy(this->extended_random_mac, configuration_ptr->extended_random_mac,8);
    memcpy(this->mesh_local_eid, configuration_ptr->mesh_local_eid,8);
    memcpy(this->vendor_stack_version, configuration_ptr->vendor_stack_version,6);
    //TODO: count PSKc instead and use that
    if( configuration_ptr->PSKd_ptr && configuration_ptr->PSKd_len > 0 ){
        this->PSKd_ptr = ns_dyn_mem_alloc(configuration_ptr->PSKd_len);
        if( this->PSKd_ptr ){
            memcpy(this->PSKd_ptr, configuration_ptr->PSKd_ptr, configuration_ptr->PSKd_len);
            this->PSKd_len = configuration_ptr->PSKd_len;
        }else{
            this->PSKd_len = 0;
        }
    }

    this->provisioning_uri_ptr = str_dub(configuration_ptr->provisioning_uri_ptr);
    this->vendor_name_ptr = str_dub(configuration_ptr->vendor_name_ptr);
    this->vendor_model_ptr = str_dub(configuration_ptr->vendor_model_ptr);
    this->vendor_sw_version_ptr = str_dub(configuration_ptr->vendor_sw_version_ptr);

    uint16_t vendor_data_len = configuration_ptr->vendor_data_len;

    if(vendor_data_len) {
        if (vendor_data_len > 64) {
            vendor_data_len = 64;
        }
        this->vendor_data_ptr = ns_dyn_mem_alloc(vendor_data_len);
        if(this->vendor_data_ptr) {
            memcpy(this->vendor_data_ptr, configuration_ptr->vendor_data_ptr, vendor_data_len);
            this->vendor_data_len = vendor_data_len;
        }
    }
    return;
}
static void device_configuration_trace(device_configuration_s *this)
{
    if (!this) {
        return;
    }
    tr_debug("Mesh local eid: %s", trace_array(this->mesh_local_eid, 8));
    tr_debug("extended random: %s", trace_array(this->extended_random_mac, 8));
    tr_debug("uri: %s", this->provisioning_uri_ptr ? this->provisioning_uri_ptr: "(none)");
    tr_debug("name: %s", this->vendor_name_ptr);
    tr_debug("mode: %s", this->vendor_model_ptr);
    tr_debug("version: %s", this->vendor_sw_version_ptr);
    tr_debug("vendor data: %s", trace_array(this->vendor_data_ptr, this->vendor_data_len));
    return;
}

static void device_configuration_validate(device_configuration_s *this)
{
    // Set defaults if not overridden by client
    if (!this->vendor_model_ptr) {
        this->vendor_model_ptr = str_dub(THREAD_VENDOR_MODEL);
    }
    if (!this->vendor_name_ptr) {
        this->vendor_name_ptr = str_dub(THREAD_VENDOR_NAME);
    }
    if (!this->vendor_sw_version_ptr) {
        this->vendor_sw_version_ptr = str_dub(THREAD_SW_VERSION);
    }
    if (memcmp(this->vendor_stack_version, "\0\0\0\0\0\0",6) == 0) {
        // Empty lets set defaults
        this->vendor_stack_version[0] = (uint8_t)(THREAD_ARM_OUI >> 16);
        this->vendor_stack_version[1] = (uint8_t)(THREAD_ARM_OUI >> 8);
        this->vendor_stack_version[2] = (uint8_t)(THREAD_ARM_OUI);
        this->vendor_stack_version[3] = (uint8_t)(THREAD_BUILD_NUMBER >> 4);
        this->vendor_stack_version[4] = (uint8_t)(((THREAD_BUILD_NUMBER & 0x0f) << 4) || THREAD_REVISION_NUMBER);
        this->vendor_stack_version[5] = (uint8_t)((THREAD_VERSION_MIN  << 4) || THREAD_VERSION_MAJ);
    }
}

static configuration_set_t *configuration_set_copy(configuration_set_t *source_ptr)
{
    configuration_set_t *result_ptr;
    if (!source_ptr) {
        return NULL;
    }
    result_ptr = ns_dyn_mem_alloc(sizeof(configuration_set_t));
    if (!result_ptr) {
        return NULL;
    }
    memcpy(result_ptr->data,source_ptr->data,source_ptr->length);
    result_ptr->length = source_ptr->length;
    result_ptr->timeout_in_ms = source_ptr->timeout_in_ms;
    result_ptr->timestamp = source_ptr->timestamp;
    return result_ptr;
}

static bool configuration_set_tlv_required(uint8_t tlv_id, const uint8_t *tlv_ptr, uint8_t tlv_len)
{
    while(tlv_len)
    {
        if(tlv_ptr[tlv_len-- -1] == tlv_id) {
            return true;
        }
    }
    return false;
}

static uint16_t configuration_set_length(configuration_set_t *config_ptr, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    const uint8_t *tlv_ptr;
    uint16_t tlv_len;
    int16_t result_len = 0;
    tlv_len = config_ptr->length;
    tlv_ptr = config_ptr->data;

    while (tlv_ptr && tlv_len) {
        if ((!req_tlv_ptr || req_tlv_len == 0 || configuration_set_tlv_required(*tlv_ptr, req_tlv_ptr, req_tlv_len)) &&
                !configuration_set_tlv_required(*tlv_ptr, ignored_tlv_ptr, ignored_tlv_len)){
            int16_t required_len = thread_meshcop_tlv_length_required(tlv_ptr, tlv_len);
            if (required_len < 0) {
                return 0;
            }
            result_len += required_len;
        }
        tlv_ptr = thread_meshcop_tlv_get_next(tlv_ptr, &tlv_len);
    }

    return result_len;
}

static uint8_t *configuration_set_write(configuration_set_t *config_ptr, uint8_t *ptr, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    const uint8_t *tlv_ptr;
    uint16_t tlv_len;
    tlv_len = config_ptr->length;
    tlv_ptr = config_ptr->data;
    while (tlv_ptr && tlv_len) {
        if ((!req_tlv_ptr || req_tlv_len == 0  || configuration_set_tlv_required(*tlv_ptr, req_tlv_ptr, req_tlv_len)) &&
                !configuration_set_tlv_required(*tlv_ptr, ignored_tlv_ptr,ignored_tlv_len)) {
            int16_t required_len = thread_meshcop_tlv_length_required(tlv_ptr,tlv_len);
            if (required_len < 0 || config_ptr->length + required_len > MAX_OPERATIONAL_DATASET_SIZE) {
                // Source configuration is corrupted or length exceeded
                return ptr;
            }
            memcpy(ptr,tlv_ptr,required_len);
            ptr += required_len;
        }
        tlv_ptr = thread_meshcop_tlv_get_next(tlv_ptr, &tlv_len);
    }
    return ptr;
}

static uint8_t *configuration_set_add_fields(configuration_set_t *destination_ptr, const uint8_t *source_ptr, uint16_t source_len, const uint8_t *required_tlv_ptr, uint8_t required_tlv_len)
{
    uint8_t *ptr;
    ptr = destination_ptr->data + destination_ptr->length;
    while (source_ptr && source_len) {
        if (configuration_set_tlv_required(*source_ptr, required_tlv_ptr, required_tlv_len)) {
            int16_t required_len = thread_meshcop_tlv_length_required(source_ptr,source_len);
            if (required_len < 0 || destination_ptr->length + required_len > MAX_OPERATIONAL_DATASET_SIZE) {
                // Source configuration is corrupted or length exceeded
                return ptr;
            }
            memcpy(ptr,source_ptr,required_len);
            destination_ptr->length += required_len;
            ptr += required_len;
        }
        source_ptr = thread_meshcop_tlv_get_next(source_ptr, &source_len);
    }
    return ptr;
}
static uint8_t *configuration_set_add_all_fields(configuration_set_t *destination_ptr, const uint8_t *source_ptr, uint16_t source_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    uint8_t *ptr;
    ptr = destination_ptr->data + destination_ptr->length;
    while (source_ptr && source_len) {
        if (!configuration_set_tlv_required(*source_ptr, ignored_tlv_ptr, ignored_tlv_len)) {
            int16_t required_len = thread_meshcop_tlv_length_required(source_ptr,source_len);
            if (required_len < 0 || destination_ptr->length + required_len > MAX_OPERATIONAL_DATASET_SIZE) {
                // Source configuration is corrupted or length exceeded
                return ptr;
            }
            memcpy(ptr,source_ptr,required_len);
            destination_ptr->length += required_len;
            ptr += required_len;
        }
        source_ptr = thread_meshcop_tlv_get_next(source_ptr, &source_len);
    }
    return ptr;
}
static void configuration_set_copy_missing(configuration_set_t *destination_ptr, configuration_set_t *source_ptr)
{
    uint8_t *tlv_list_ptr;
    uint16_t tlv_list_len = 0;
    if (!source_ptr || !destination_ptr) {
        return;
    }
    thread_meshcop_tlv_list_generate(source_ptr->data, source_ptr->length,NULL, &tlv_list_len);
    if (tlv_list_len == 0){
        return;
    }
    tlv_list_ptr = ns_dyn_mem_temporary_alloc(tlv_list_len);
    if (!tlv_list_ptr) {
        return;
    }
    thread_meshcop_tlv_list_generate(source_ptr->data, source_ptr->length,tlv_list_ptr, &tlv_list_len);
    tr_debug("list in source: %s",trace_array(tlv_list_ptr,tlv_list_len));

    for(uint8_t n = 0;n < tlv_list_len;n++) {
        if(thread_meshcop_tlv_exist(destination_ptr->data, destination_ptr->length, tlv_list_ptr[n])){
            // Depending specification 0 length TLVs from source should be also removed
            tlv_list_len = thread_meshcop_tlv_list_remove(tlv_list_ptr,tlv_list_len,tlv_list_ptr[n]);
            n--; // When we remove item from list we stay put
        }
    }
    if (tlv_list_len == 0) {
        // All mandatory TLVs present
        ns_dyn_mem_free(tlv_list_ptr);
        return;
    }
    tr_debug("mandatory TLVs needed: %s",trace_array(tlv_list_ptr,tlv_list_len));
    configuration_set_add_fields(destination_ptr, source_ptr->data, source_ptr->length, tlv_list_ptr, tlv_list_len);
    ns_dyn_mem_free(tlv_list_ptr);
}
static void configuration_set_remove_null_tlv(configuration_set_t *configuration_ptr)
{
    uint8_t *ptr;
    uint8_t *next_ptr;
    uint16_t length;
    if (!configuration_ptr) {
        return;
    }
    length = configuration_ptr->length;
    ptr = configuration_ptr->data;

    do {
        if(thread_meshcop_tlv_length(ptr,length) == 0){
            // remove this TLV
            configuration_ptr->length -= thread_meshcop_tlv_length_required(ptr,length);
            next_ptr = (uint8_t*)thread_meshcop_tlv_get_next(ptr,&length);
            if (next_ptr) {
                memmove(ptr,next_ptr,length);
            }
        } else {
            ptr = (uint8_t*)thread_meshcop_tlv_get_next(ptr,&length);
        }

    } while (ptr && length);
    return;
}

static void configuration_set_copy_mandatory(configuration_set_t *destination_ptr, configuration_set_t *source_ptr)
{
    uint8_t tlv_list[sizeof(active_configuration_dataset_tlvs)];
    uint16_t tlv_list_len = sizeof(active_configuration_dataset_tlvs);
    if (!source_ptr || !destination_ptr) {
        return;
    }
    memcpy(tlv_list,active_configuration_dataset_tlvs, sizeof(active_configuration_dataset_tlvs));

    for(uint8_t n = 0;n < sizeof(active_configuration_dataset_tlvs);n++) {
        if(0 < thread_meshcop_tlv_find(destination_ptr->data, destination_ptr->length, active_configuration_dataset_tlvs[n], NULL)){
            // Depending specification 0 length TLVs from source should be also removed
            tlv_list_len = thread_meshcop_tlv_list_remove(tlv_list,tlv_list_len,active_configuration_dataset_tlvs[n]);
        }
    }
    if (tlv_list_len == 0) {
        // All mandatory TLVs present
        return;
    }
    tr_debug("mandatory TLVs needed: %s",trace_array(tlv_list,tlv_list_len));
    configuration_set_add_fields(destination_ptr, source_ptr->data, source_ptr->length, tlv_list, tlv_list_len);
}
static void configuration_set_generate(configuration_set_t *destination_ptr, link_configuration_s *configuration_ptr)
{
    uint8_t *response_ptr;

    if (!destination_ptr || !configuration_ptr) {
        return;
    }
    response_ptr = destination_ptr->data;

    response_ptr = thread_joiner_application_write_channel(response_ptr, configuration_ptr->rfChannel);
    *response_ptr++ = MESHCOP_TLV_CHANNEL_MASK; // type
    *response_ptr++ = 6; // length
    *response_ptr++ = configuration_ptr->channel_page; // channel page
    *response_ptr++ = 4; // channel mask length
    memcpy(response_ptr,configuration_ptr->channel_mask,4);
    response_ptr += 4;
    response_ptr = thread_tmfcop_tlv_data_write(response_ptr, MESHCOP_TLV_XPANID, 8, configuration_ptr->extented_pan_id);
    response_ptr = thread_tmfcop_tlv_data_write(response_ptr, MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA, 8, configuration_ptr->mesh_local_ula_prefix);
    response_ptr = thread_tmfcop_tlv_data_write(response_ptr, MESHCOP_TLV_NETWORK_MASTER_KEY, 16, configuration_ptr->master_key);
    response_ptr = thread_tmfcop_tlv_data_write_uint16(response_ptr, MESHCOP_TLV_PANID, configuration_ptr->panId);
    response_ptr = thread_tmfcop_tlv_data_write(response_ptr, MESHCOP_TLV_PSKC, 16, configuration_ptr->PSKc);
    response_ptr = thread_tmfcop_tlv_data_write(response_ptr, MESHCOP_TLV_NETWORK_NAME, stringlen((char*)&configuration_ptr->name, 16), configuration_ptr->name);
    *response_ptr++ = MESHCOP_TLV_SECURITY_POLICY; // type
    *response_ptr++ = 3; // length
    response_ptr = common_write_16_bit(configuration_ptr->key_rotation,response_ptr);
    *response_ptr++ = configuration_ptr->securityPolicy;
    response_ptr = thread_tmfcop_tlv_data_write_uint64(response_ptr, MESHCOP_TLV_ACTIVE_TIME_STAMP, configuration_ptr->timestamp);

    destination_ptr->length = response_ptr - destination_ptr->data;
}

static bool configuration_set_validate(uint8_t *configuration_set, uint16_t configuration_set_len, bool make_full_validation)
{
    if (thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_NETWORK_MASTER_KEY, NULL) < 16 ||
        thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA, NULL) < 8 ||
        thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_XPANID, NULL) < 8 ||
        thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_NETWORK_NAME, NULL) == 0 ||
        thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_PSKC, NULL) < 16 ) {
            // Absolutely minimum we must have is master secret to attach.
            // If commissioner wants to be connected we must have PSKc,Name,Xpanid
            // If there is some fields missing we could attach, but timestamp must be set to 0 which will then cause synchronization
            tr_debug("Not all TLv's included");
            return false;
    }

    if (make_full_validation) {
        if ((thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_CHANNEL, NULL) == 0) ||
            (thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_PANID, NULL) == 0) ||
            (thread_meshcop_tlv_find(configuration_set, configuration_set_len, MESHCOP_TLV_XPANID, NULL) == 0)) {
                tr_debug("Not all TLv's included");
                return false;
        }
    }
    return true;
}

static bool thread_joiner_application_validate_settings(thread_joiner_t *this)
{
    bool new_value_generated=0;
    if (memcmp(this->device_configuration_ptr->extended_random_mac,ADDR_UNSPECIFIED, 8) == 0) {
        randLIB_get_n_bytes_random(this->device_configuration_ptr->extended_random_mac, 8);
        this->device_configuration_ptr->extended_random_mac[0] |= 2; //Set Local Bit
        this->device_configuration_ptr->extended_random_mac[0] &= ~1; //Clear multicast bit
        new_value_generated = 1;
        tr_info("Generating Random MAC");
    }
    while (addr_iid_reserved(this->device_configuration_ptr->mesh_local_eid) ||
        memcmp(this->device_configuration_ptr->mesh_local_eid, ADDR_SHORT_ADR_SUFFIC,6) == 0 ) {
        // addr_iid_reserved checks the all zeroes case.
        randLIB_get_n_bytes_random(this->device_configuration_ptr->mesh_local_eid, 8);
        new_value_generated = 1;
        tr_info("Generating Random ML-EID");
    }
    if (this->configuration_ptr->key_rotation < 3600) {
        this->configuration_ptr->key_rotation = 3600;
    }
    return new_value_generated;
}

int thread_joiner_application_init(int8_t interface_id, device_configuration_s *device_configuration_ptr, link_configuration_s *default_configuration_ptr)
{
    thread_joiner_t *this;
    uint8_t pskd_len;

    if ( !device_configuration_ptr ) {
        return -1;
    }
    pskd_len = device_configuration_ptr->PSKd_len;

    if (pskd_len > 32 || pskd_len < 6 ) {
        return -2;
    }

    this = thread_joiner_get(interface_id);
    if (!this) {
        return -3;
    }

    if ( !this->device_configuration_ptr ) {
        this->device_configuration_ptr = device_configuration_create();
    }
    if ( !this->device_configuration_ptr ) {
        tr_error("Joiner Device creation failed");
        return -4;
    }
    device_configuration_copy(this->device_configuration_ptr, device_configuration_ptr);
    device_configuration_validate(this->device_configuration_ptr); // Set defaults if missing
    device_configuration_trace(this->device_configuration_ptr);
    if (memcmp(this->device_configuration_ptr->eui64, "\0\0\0\0\0\0\0\0",8) == 0) {
        //EUI64 is not set we then read that from Radio
        protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

        if (cur && cur->mac_api) {
            cur->mac_api->mac64_get(cur->mac_api,MAC_EXTENDED_READ_ONLY, this->device_configuration_ptr->eui64);
        }
    }
    // Adding entropy for our random seed
    randLIB_add_seed(common_read_64_bit(this->device_configuration_ptr->eui64));

    // create link configuration, could be replaced by configuration settings read from NVM
    if (!this->configuration_ptr) {
        this->configuration_ptr = link_configuration_create();
    }
    this->configuration_valid = false;

    if (!this->configuration_ptr) {
        tr_error("Joiner creation failed");
        device_configuration_delete(this->device_configuration_ptr);
        return -4;
    }
    if(!this->active_configuration_ptr){
        this->active_configuration_ptr = ns_dyn_mem_alloc(sizeof(configuration_set_t));
    }
    if (!this->active_configuration_ptr) {
        tr_error("Active configuration creation failed");
        device_configuration_delete(this->device_configuration_ptr);
        link_configuration_delete(this->configuration_ptr);
        return -5;
    }
    memset(this->active_configuration_ptr, 0, sizeof(configuration_set_t));

    if (default_configuration_ptr) {
        link_configuration_copy(this->configuration_ptr, default_configuration_ptr);
        thread_joiner_application_validate_settings(this);// Generate all random information
        if( memcmp(this->configuration_ptr->master_key,ADDR_UNSPECIFIED ,16) != 0 ||
            memcmp(this->configuration_ptr->PSKc,ADDR_UNSPECIFIED ,16) != 0     ) {
            //If no master key or PSKc set we assume not valid configuration for thread network others may be possible to be 0
            //This allows some configurations to be set statically for testing purposes
            this->configuration_valid = true;
            configuration_set_generate(this->active_configuration_ptr, this->configuration_ptr);
        }
    }
    // Always load link configuration from bootstrap state machine. NVM overrides Static configuration
    this->nvm_link_configuration_load = true;

    link_configuration_trace(this->configuration_ptr);

    return 0;
}

int thread_joiner_application_form_network(int8_t interface_id, uint8_t *commissioning_credentials_ptr, char *name_ptr)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    (void)commissioning_credentials_ptr;
    (void)name_ptr;
    tr_debug("Joiner form random network");
    if (!this) {
        return -1;
    }
    /*
     * TODO generate random network parameter and enable commissioner to join the network
     */

    return 0;
}

void thread_joiner_application_deinit(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (this) {
        ns_list_remove(&instance_list, this);
        link_configuration_delete(this->configuration_ptr);
        device_configuration_delete(this->device_configuration_ptr);
        if (this->pending_configuration_ptr) {
            ns_dyn_mem_free(this->pending_configuration_ptr);
        }
        if (this->next_pending_configuration_ptr) {
            ns_dyn_mem_free(this->next_pending_configuration_ptr);
        }
        if (this->active_configuration_ptr) {
            ns_dyn_mem_free(this->active_configuration_ptr);
        }
        if (this->old_active_configuration_ptr) {
            ns_dyn_mem_free(this->old_active_configuration_ptr);
        }
        ns_dyn_mem_free(this);
    }
}

link_configuration_s *thread_joiner_application_get_config(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        return NULL;
    }

    if (this->configuration_valid == false) {
        return NULL;
    }
    return this->configuration_ptr;
}

uint8_t *thread_joiner_application_network_name_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        return NULL;
    }
    return this->configuration_ptr->name;
}

static int thread_joiner_application_nvm_link_config_read(thread_joiner_t *this) {

    // read config from NVM, in case of failure current settings are unchanged.
    int nvm_read_status = thread_nvm_store_active_configuration_read(this->active_configuration_ptr, sizeof(configuration_set_t));
    tr_debug("active conf read %d", nvm_read_status);
    // validate that active configuration settings are valid, even if we couldn't read from nvm.
    if(!configuration_set_validate(this->active_configuration_ptr->data, this->active_configuration_ptr->length, true)) {
        tr_debug("No active configuration avail");
        return -1;
    }
    link_configuration_update(this->configuration_ptr, this->active_configuration_ptr->data, this->active_configuration_ptr->length);

    thread_nvm_fast_data_t fast_data;
    memset(&fast_data,0,sizeof(thread_nvm_fast_data_t));
    int fast_data_read_ret = thread_nvm_store_fast_data_read(&fast_data);
    tr_info("From NVM %d", fast_data_read_ret);
    tr_info("mac-counter %"PRIu32,fast_data.mac_frame_counter);
    tr_info("mle-counter %"PRIu32,fast_data.mle_frame_counter);
    tr_info("seq-counter %"PRIu32,fast_data.seq_counter);

    if (THREAD_NVM_FILE_SUCCESS == fast_data_read_ret) {
        if (this->configuration_ptr->key_sequence < fast_data.seq_counter) {
            this->configuration_ptr->key_sequence = fast_data.seq_counter;
        }
    }

    fast_data.mac_frame_counter += MAC_FRAME_COUNTER_LIMIT;
    fast_data.mle_frame_counter += MLE_FRAME_COUNTER_LIMIT;
    thread_nvm_store_fast_data_write(&fast_data);

    thread_nvm_store_device_configuration_read(this->device_configuration_ptr->extended_random_mac,this->device_configuration_ptr->mesh_local_eid);
    thread_nvm_store_link_info_read();
    // Generate all random information, if device configuration read failed then the random mac and eid are created.
    bool new_value_generated = thread_joiner_application_validate_settings(this);
    if (new_value_generated) {
        thread_nvm_store_device_configuration_write(this->device_configuration_ptr->extended_random_mac,this->device_configuration_ptr->mesh_local_eid);
    }

    this->configuration_valid = true;
    link_configuration_trace(this->configuration_ptr);

    //Add Security to MLE service
    uint8_t key_material[32];
    uint8_t key_index;
    //Define KEY's

    thread_key_get(this->configuration_ptr->master_key, key_material, this->configuration_ptr->key_sequence);
    key_index = THREAD_KEY_INDEX(this->configuration_ptr->key_sequence);
    //Set Keys
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    mac_helper_security_default_key_set(cur, &key_material[16], key_index, MAC_KEY_ID_MODE_IDX);
    mle_service_security_set_security_key(this->interface_id, key_material, key_index, true);
    // update counters
    mle_service_security_set_frame_counter(this->interface_id, fast_data.mle_frame_counter);
    mac_helper_link_frame_counter_set(this->interface_id, fast_data.mac_frame_counter);

    // this saves all configurations
    if (THREAD_NVM_FILE_SUCCESS!=nvm_read_status) {
        thread_joiner_application_configuration_nvm_save(this->interface_id);
    }
    else {
        tr_info("Reading pending set");
        configuration_set_t *pend_conf_ptr = ns_dyn_mem_alloc(sizeof(configuration_set_t));
        if (pend_conf_ptr) {
            memset(pend_conf_ptr, 0, sizeof(configuration_set_t));
            int pending_ret = thread_nvm_store_pending_configuration_read(pend_conf_ptr, sizeof(configuration_set_t));
            if(THREAD_NVM_FILE_SUCCESS==pending_ret) {
                if (this->pending_configuration_ptr) {
                    ns_dyn_mem_free(this->pending_configuration_ptr);
                }
                this->pending_configuration_ptr = pend_conf_ptr;
                this->pending_configuration_ptr->timeout_in_ms = 0;
                this->pending_set_in_sync = false;
            }
            else {
                tr_info("Reading pending from NVM error:%d", pending_ret);
                ns_dyn_mem_free(pend_conf_ptr);
            }
        }
    }
    this->nvm_link_configuration_load = false;
    return 0;
}

static int thread_joiner_application_nvm_link_config_delete(thread_joiner_t *this)
{
    if (!this) {
        return -1;
    }
    int ret = thread_nvm_store_pending_configuration_remove();

    if (ret!=THREAD_NVM_FILE_SUCCESS) {
        tr_error("Pending configuration delete error: %d", ret);
    }

    ret = thread_nvm_store_active_configuration_remove();

    if (ret!=THREAD_NVM_FILE_SUCCESS) {
        tr_error("Active configuration delete error: %d", ret);
    }

    // delete link configuration from cache, device will be restarted after settings are deleted
    this->configuration_valid = false;
    return 0;
}

uint64_t thread_joiner_application_active_timestamp_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        return 0;
    }

    return this->configuration_ptr->timestamp;
}

uint8_t thread_joiner_application_security_policy_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        return 0;
    }

    return this->configuration_ptr->securityPolicy;
}

uint8_t *thread_joiner_application_random_mac_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->device_configuration_ptr) {
        tr_error("thread_joiner_application_random_mac_get NULL parameter ");
        return (uint8_t*)ADDR_UNSPECIFIED;
    }
    return this->device_configuration_ptr->extended_random_mac;
}
uint8_t *thread_joiner_application_ml_eid_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->device_configuration_ptr) {
        tr_error("thread_joiner_application_ml_eid_get parameter NULL");
        return (uint8_t*)ADDR_UNSPECIFIED;
    }
    return this->device_configuration_ptr->mesh_local_eid;
}
void thread_joiner_application_active_timestamp_set(int8_t interface_id, uint64_t timestamp)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    uint8_t *timestamp_ptr = NULL;
    if (!this) {
        return;
    }

    thread_meshcop_tlv_find(this->active_configuration_ptr->data, this->active_configuration_ptr->length, MESHCOP_TLV_ACTIVE_TIME_STAMP, &timestamp_ptr);
    if (timestamp_ptr) {
        common_write_64_bit(timestamp,timestamp_ptr);
    }

    //update timestamps in active configuration and link configuration
    this->active_configuration_ptr->timestamp = timestamp;
    this->configuration_ptr->timestamp = timestamp;
    return;
}

void thread_joiner_pending_config_activate(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);

    if (!this || !this->pending_configuration_ptr) {
        tr_error("No pending configuration or joiner!");
        return;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!cur) {
        tr_error("No interface information!");
        return;
    }

    /*
     * If pending configuration active timestamp is lower than current active timestamp allow pending configuration activation only
     * if master key is changed by the pending set
     */
    link_configuration_s *link_configuration = thread_joiner_application_get_config(interface_id);

    if (!link_configuration) {
        return;
    }

    // Validate new link configuration
	uint8_t *master_key_ptr = NULL;
    uint64_t pending_active_timestamp = 0;
    thread_meshcop_tlv_data_get_uint64(this->pending_configuration_ptr->data,this->pending_configuration_ptr->length,MESHCOP_TLV_ACTIVE_TIME_STAMP, &pending_active_timestamp);
	thread_meshcop_tlv_find(this->pending_configuration_ptr->data,this->pending_configuration_ptr->length,MESHCOP_TLV_NETWORK_MASTER_KEY,&master_key_ptr);

    if ((pending_active_timestamp < thread_joiner_application_active_timestamp_get(interface_id))) {
    	// If new active timestamp is older than current master key must be changed.
        if (!master_key_ptr ||
            memcmp(master_key_ptr,link_configuration->master_key,16) == 0) {
            tr_info("*** Pending set activation aborted - device has newer active timestamp");
            thread_joiner_application_pending_config_delete(interface_id);
            return;
        }
    }

    if (master_key_ptr && memcmp(master_key_ptr,link_configuration->master_key,16) != 0) {
        this->configuration_ptr->key_sequence = 0;
        // if write fails, keep going...
        (void)thread_nvm_store_seq_counter_write(this->configuration_ptr->key_sequence);
    }

    tr_info("*** Activating pending configuration.");

    ns_dyn_mem_free(this->old_active_configuration_ptr);
    this->old_active_configuration_ptr = this->active_configuration_ptr;
    this->active_configuration_ptr = this->pending_configuration_ptr;
    this->active_configuration_ptr->timestamp = pending_active_timestamp;
    // All information is copied from old configuration so if configuration is corrupt we dont change anything.
    this->pending_configuration_ptr = NULL;
    (void)thread_nvm_store_pending_configuration_remove();
    configuration_set_copy_mandatory(this->active_configuration_ptr, this->old_active_configuration_ptr);
    link_configuration_update(this->configuration_ptr,this->active_configuration_ptr->data, this->active_configuration_ptr->length);
    link_configuration_trace(this->configuration_ptr);

    thread_joiner_application_configuration_nvm_save(this->interface_id);


    if (thread_link_configuration_activate(cur, this->configuration_ptr) != 0) {
        tr_error("Activating pending configuration failed.");
    }

    tr_info("*** Activating pending configuration done.");
}


void thread_joiner_application_seconds_timer(int8_t interface_id, uint32_t seconds)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    //tr_debug("seconds tick for joiner pending timeout in %d",this->pending_timeout_in_ms);
    if (!this) {
        return;
    }
    if (this->provisioning_timeout) {
        if (this->provisioning_timeout < seconds) {
            this->provisioning_done = PROVISIONING_STATUS_NOT_DONE;
        }
        this->provisioning_timeout -= seconds;
    }
    if (!this->pending_configuration_ptr || this->pending_configuration_ptr->timeout_in_ms == 0) {
        return;
    }
    if (this->pending_configuration_ptr->timeout_in_ms > seconds*1000) {
    	this->pending_configuration_ptr->timeout_in_ms -= seconds*1000;
        return;
    }
    // Activate the pending config
    thread_joiner_pending_config_activate(interface_id);
}

int thread_joiner_application_pending_config_create(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || data_len > MAX_OPERATIONAL_DATASET_SIZE) {
        return -1;
    }
    if(!this->pending_configuration_ptr){
        this->pending_configuration_ptr = ns_dyn_mem_alloc(sizeof(configuration_set_t));
    }
    if (!this->pending_configuration_ptr) {
        tr_error("pending configuration creation failed");
        return -2;
    }
    memset(this->pending_configuration_ptr, 0, sizeof(configuration_set_t));
    configuration_set_add_all_fields(this->pending_configuration_ptr,data_ptr,data_len, meshcop_pending_set_ignore, sizeof(meshcop_pending_set_ignore));
    return 0;
}

bool thread_joiner_application_pending_config_exists(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return false;
    }
    return true;
}

bool thread_joiner_application_pending_delay_timer_in_sync(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return false;
    }
    return this->pending_set_in_sync;
}

uint64_t thread_joiner_application_pending_config_timestamp_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr || !this->pending_set_in_sync) {
        return 0;
    }
    return this->pending_configuration_ptr->timestamp;
}

int thread_joiner_application_pending_config_timestamp_set(int8_t interface_id, uint64_t timestamp)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return -1;
    }
    this->pending_configuration_ptr->timestamp = timestamp;
    return 0;
}


int thread_joiner_application_pending_config_enable(int8_t interface_id, uint32_t timeout_in_ms)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return -1;
    }
    this->pending_configuration_ptr->timeout_in_ms = timeout_in_ms;
    this->pending_set_in_sync = true;

    if(this->pending_configuration_ptr->timeout_in_ms > THREAD_MAX_DELAY_TIMER_SECONDS*1000) {
        this->pending_configuration_ptr->timeout_in_ms = THREAD_MAX_DELAY_TIMER_SECONDS*1000;
    }
    thread_joiner_application_configuration_nvm_save(this->interface_id);

    return 0;
}

uint32_t thread_joiner_application_pending_config_timeout_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return 0;
    }
    return this->pending_configuration_ptr->timeout_in_ms;
}

uint16_t thread_joiner_application_pending_config_length(uint8_t interface_id, uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    uint16_t result_len;
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return 0;
    }

    result_len = configuration_set_length(this->pending_configuration_ptr,req_tlv_ptr,req_tlv_len, ignored_tlv_ptr, ignored_tlv_len);

    /* Add pending timestamp if required and not ignored*/
    if (thread_meshcop_tlv_list_type_available(req_tlv_ptr, req_tlv_len, MESHCOP_TLV_PENDING_TIMESTAMP) &&
            !thread_meshcop_tlv_list_type_available(ignored_tlv_ptr, ignored_tlv_len, MESHCOP_TLV_PENDING_TIMESTAMP)) {
        result_len += 10;
    }

    /* Always add delay timer */
    result_len += 6;

	return result_len;
}

uint8_t *thread_joiner_application_pending_config_build(uint8_t interface_id, uint8_t *ptr, uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return ptr;
    }

    /* Build pending config data */
    ptr = configuration_set_write(this->pending_configuration_ptr, ptr, req_tlv_ptr, req_tlv_len, ignored_tlv_ptr, ignored_tlv_len);

    /* Add pending timestamp (len = 10) if required and not ignored */
    if (thread_meshcop_tlv_list_type_available(req_tlv_ptr, req_tlv_len, MESHCOP_TLV_PENDING_TIMESTAMP) &&
            !thread_meshcop_tlv_list_type_available(ignored_tlv_ptr, ignored_tlv_len, MESHCOP_TLV_PENDING_TIMESTAMP)) {
        ptr = thread_meshcop_tlv_data_write_uint64(ptr,MESHCOP_TLV_PENDING_TIMESTAMP, this->pending_configuration_ptr->timestamp);
    }

    /* Always add delay timer (len = 6) */
    ptr = thread_meshcop_tlv_data_write_uint32(ptr, MESHCOP_TLV_DELAY_TIMER, this->pending_configuration_ptr->timeout_in_ms);

	return ptr;
}

uint8_t *thread_joiner_application_pending_config_tlv_list_get(uint8_t interface_id, uint16_t *length)
{
    uint16_t data_list_len = 0;
    uint8_t *result_ptr;
    uint8_t *return_ptr;

    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return NULL;
    }
    *length = 0;
    thread_meshcop_tlv_list_generate(this->pending_configuration_ptr->data, this->pending_configuration_ptr->length, NULL, &data_list_len);
    if (data_list_len == 0){
        return NULL;
    }

    /* Add also room for pending timestamp */
    *length = data_list_len + 1;
    result_ptr = return_ptr = ns_dyn_mem_alloc(*length);

    if (!result_ptr) {
	    return NULL;
    }

	*result_ptr++ = MESHCOP_TLV_PENDING_TIMESTAMP;

	thread_meshcop_tlv_list_generate(this->pending_configuration_ptr->data, this->pending_configuration_ptr->length, result_ptr, &data_list_len);

    return return_ptr;
}

void thread_joiner_application_pending_config_add_missing_fields(uint8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return;
    }
    //TODO Specification open here
    // a) copy only mandatory fields
    // b) copy everything but remove zero length tlvs
    //configuration_set_copy_mandatory(this->pending_configuration_ptr, this->active_configuration_ptr);
    configuration_set_copy_missing(this->pending_configuration_ptr, this->active_configuration_ptr);
    configuration_set_remove_null_tlv(this->pending_configuration_ptr);
}


bool thread_joiner_application_old_config_exists(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->old_active_configuration_ptr) {
        return false;
    }
    return true;
}
void thread_joiner_application_old_config_delete(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->old_active_configuration_ptr) {
        return;
    }
    ns_dyn_mem_free(this->old_active_configuration_ptr);
    this->old_active_configuration_ptr = NULL;

    return;
}
int thread_joiner_application_old_config_activate(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->old_active_configuration_ptr) {
        return -1;
    }
    this->active_configuration_ptr = this->old_active_configuration_ptr;
    this->old_active_configuration_ptr = NULL;
    link_configuration_update(this->configuration_ptr, this->active_configuration_ptr->data, this->active_configuration_ptr->length);
    link_configuration_trace(this->configuration_ptr);

    return 0;
}

void thread_joiner_application_pending_config_delete(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return;
    }
    ns_dyn_mem_free(this->pending_configuration_ptr);
    this->pending_configuration_ptr = NULL;
    thread_joiner_application_configuration_nvm_save(interface_id);
    return;
}

void thread_joiner_application_next_pending_config_save(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->pending_configuration_ptr) {
        return;
    }
    this->next_pending_configuration_ptr = configuration_set_copy(this->pending_configuration_ptr);
    if (!this->next_pending_configuration_ptr) {
        tr_error("next pending configuration creation failed");
    }
    return;
}
bool thread_joiner_application_next_pending_config_exists(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_pending_configuration_ptr) {
        return false;
    }
    return true;
}

uint16_t thread_joiner_application_next_pending_config_length(int8_t interface_id)
{
    uint16_t response_len = 0;
	thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_pending_configuration_ptr) {
        return 0;
    }
    response_len += 2 + 8; // Pending timestamp
    response_len += 2 + 4;  // delay timer
    response_len += this->next_pending_configuration_ptr->length;
    return response_len;
}

uint8_t *thread_joiner_application_next_pending_config_build(int8_t interface_id, uint8_t *ptr)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_pending_configuration_ptr) {
        return ptr;
    }
    memcpy(ptr, this->next_pending_configuration_ptr->data, this->next_pending_configuration_ptr->length );
    ptr +=this->next_pending_configuration_ptr->length;
    ptr = thread_meshcop_tlv_data_write_uint64(ptr,MESHCOP_TLV_PENDING_TIMESTAMP, this->next_pending_configuration_ptr->timestamp);
    ptr = thread_meshcop_tlv_data_write_uint32(ptr,MESHCOP_TLV_DELAY_TIMER, this->next_pending_configuration_ptr->timeout_in_ms);
    return ptr;
}
void thread_joiner_application_next_pending_config_delete(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_pending_configuration_ptr) {
        return;
    }
    ns_dyn_mem_free(this->next_pending_configuration_ptr);
    this->next_pending_configuration_ptr = NULL;

    return;
}
void thread_joiner_application_next_active_config_save(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->configuration_ptr) {
        return;
    }
    this->next_active_configuration_ptr = configuration_set_copy(this->active_configuration_ptr);
    if (!this->next_active_configuration_ptr) {
        tr_error("next pending configuration creation failed");
        return;
    }
    return;
}
bool thread_joiner_application_next_active_config_exists(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_active_configuration_ptr) {
        return false;
    }
    return true;
}
uint16_t thread_joiner_application_next_active_config_length(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_active_configuration_ptr) {
        return 0;
    }
    return this->next_active_configuration_ptr->length;
}

uint8_t *thread_joiner_application_next_active_config_write(int8_t interface_id, uint8_t *ptr)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_active_configuration_ptr) {
        return ptr;
    }
    memcpy(ptr, this->next_active_configuration_ptr->data, this->next_active_configuration_ptr->length );
    ptr +=this->next_active_configuration_ptr->length;
    return ptr;
}

void thread_joiner_application_next_active_config_delete(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->next_active_configuration_ptr) {
        return;
    }
    ns_dyn_mem_free(this->next_active_configuration_ptr);
    this->next_active_configuration_ptr = NULL;

    return;
}

device_configuration_s *thread_joiner_application_get_device_config(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        return NULL;
    }
    return this->device_configuration_ptr;
}

int thread_joiner_application_link_configuration_store(int8_t interface_id, link_configuration_s *link_config)
{
    thread_joiner_t *this = thread_joiner_get(interface_id);

    if (!link_config || !this) {
        return -1;
    }

    if (!this->configuration_ptr) {
            this->configuration_ptr = link_configuration_create();
        }

    thread_joiner_application_validate_settings(this);// Generate all random information
    configuration_set_generate(this->active_configuration_ptr, link_config);
    link_configuration_update(this->configuration_ptr, this->active_configuration_ptr->data, this->active_configuration_ptr->length);
    this->configuration_ptr->key_sequence = link_config->key_sequence;
    this->configuration_valid = true;
    link_configuration_trace(this->configuration_ptr);

    // store link configuration to NVM. This also stores pending configuration and some other parameters!
    return thread_joiner_application_configuration_nvm_save(this->interface_id);
}

int thread_joiner_application_link_configuration_delete(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_get(interface_id);

    // delete link configuration from NVM
    return thread_joiner_application_nvm_link_config_delete(this);
}

bool thread_joiner_application_nvm_link_configuration_load(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (this && this->nvm_link_configuration_load == true) {
        this->nvm_link_configuration_load = false;
        if (thread_joiner_application_nvm_link_config_read(this) == 0) {
            return true;
        } else {
            tr_error("Failed to read link configuration settings from NVM");
        }
    }
    return false;
}

int thread_joiner_application_provisioning_set(int8_t interface_id, thread_provisioning_status_e status)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (this) {
        this->provisioning_done = status;
        if (status == PROVISIONING_STATUS_REJECTED) {
            // If commissioner rejects us retry done after some time -> state is changed back to not done
            this->provisioning_timeout = randLIB_get_random_in_range(20,120);
        } else {
            this->provisioning_timeout = 0;
        }
    }
    return 0;
}

thread_provisioning_status_e thread_joiner_application_provisioning_get(int8_t interface_id)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (this) {
        return this->provisioning_done;
    }
    return PROVISIONING_STATUS_NOT_DONE;
}

static int joiner_application_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t *pw, uint8_t *pw_len)
{
    int ret = -1;
    tr_debug("Thread joiner security started");
    (void)address;
    (void)port;

    thread_joiner_t *this = thread_joiner_find_by_service(service_id);

    if( this ){
        memcpy(pw, this->device_configuration_ptr->PSKd_ptr, this->device_configuration_ptr->PSKd_len );
        *pw_len = this->device_configuration_ptr->PSKd_len;
        ret = 0;
//        ret = coap_service_security_key_set( service_id, address, port,
//                                             this->device_configuration_ptr->PSKd_ptr,
//                                             this->device_configuration_ptr->PSKd_len );
    }

    return ret;
}

int joiner_application_security_done_cb(int8_t service_id, uint8_t address[16], uint8_t keyblock[40])
{
    thread_joiner_t *this = thread_joiner_find_by_service(service_id);
    uint8_t kek[32];
    int ret;

    tr_debug("Thread joiner security done service: %d, address: %s", service_id, trace_ipv6(address));
    if (!this) {
        return -1;
    }

    ns_sha256(keyblock, 40, kek);

    ret = thread_commissioning_if_pairwise_key_add(this->interface_id, 10000, &address[8], kek);
    if (ret) {
        tr_debug("pairwise key set failed %d", ret);
        //TODO error?
    }
    thread_commissioning_if_enable_security(this->interface_id);
    return 0;
}

static void thread_joiner_attach_cb(void* arg)
{
    thread_joiner_t *this = arg;
    tr_debug("Attach to new network");
    if (!this){
        return;
    }
    // Cleaning up the joining information
    thread_joiner_application_commission_clean(this);
    if (this->done_cb) {// This is successfull
        this->done_cb(this->interface_id);
    }
}
static void thread_joiner_entrust_timeout_cb(void* arg)
{
    thread_joiner_t *this = arg;
    tr_debug("No valid configuration received in time");
    if (!this){
        return;
    }
    // Cleaning up the joining information
    thread_joiner_application_commission_clean(this);
    if (this->done_cb) {// Failed
        this->done_cb(this->interface_id);
    }
}

void thread_joiner_attach_if_needed(thread_joiner_t *this)
{
    tr_debug("attach check");
    if (this->finalisation_done && this->configuration_valid) {
        tr_debug("Start attach timer");// one second timer gives time for joiner router to get entrust response
        this->attach_timeout = eventOS_timeout_ms(thread_joiner_attach_cb, 1000, this);
    } else {
        tr_debug("Start entrust timeout");// 30 timer to get credentials
        this->entrust_timeout = eventOS_timeout_ms(thread_joiner_entrust_timeout_cb, 30000, this);
    }
}


int thread_joiner_application_finalisation_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_joiner_t *this = thread_joiner_find_by_service(service_id);
    uint8_t result;
    (void)source_address;
    (void)source_port;

    tr_debug("finalisation done");

    if (!this) {
        return -1;
    }

    // CoAP message failed - try to reattach
    if(!response_ptr || !response_ptr->payload_ptr){
        tr_debug("finalisation failed - no response");
        goto error;
    }

    thci_trace("Device - Joiner|Direction - recv|EUI - %s|Type - JOIN_FIN.resp|Length - %d|Payload - %s",trace_array(this->device_configuration_ptr->eui64,8), response_ptr->payload_len, trace_array(response_ptr->payload_ptr, response_ptr->payload_len));
    coap_service_close_secure_connection(service_id, this->parent_address, this->parent_port);
    // todo: free certificates
    if (1 <= thread_tmfcop_tlv_data_get_uint8(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &result)) {
        //Todo make attach
        this->finalisation_done = true;
        this->provisioning_done = PROVISIONING_STATUS_NOT_DONE;
        if (result == 1) {
            // If accepted no additional actions needed
            tr_debug("Provisioning done");
            this->provisioning_done = PROVISIONING_STATUS_DONE;
        }
        thread_joiner_attach_if_needed(this);
        return 0;
    }
    thci_trace("joinerError");
    tr_debug("finalisation rejected");

error:
    //TODO cant delete as old configuration pointer is in use I quess
//    link_configuration_delete(this->configuration_ptr);
    thread_joiner_application_commission_clean(this);
    this->configuration_valid = false;
    this->done_cb(this->interface_id);
    blacklist_update(source_address, false);
    //This trigger re scan and we try to connect again
    //TODO this needs status code to allow blacklisting of thread network
    return 0;
}
static int thread_joiner_application_send_finalisation(thread_joiner_t *this, uint8_t parent_address[16], uint16_t port)
{
    uint8_t *ptr, *data_ptr;
    uint16_t length;

    tr_debug("Thread joiner finalisation send");
    length = thread_joiner_application_device_configuration_length(this->device_configuration_ptr);
    length += 3;// State

    data_ptr = ptr = ns_dyn_mem_alloc(length);
    if (!ptr) {
        tr_error("Failed to start Commissioning");
        return -1;
    }
    ptr = thread_joiner_application_device_configuration_build(ptr, this->device_configuration_ptr);
    ptr = thread_tmfcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);

    thci_trace("Device - Joiner|Direction - sent|EUI - %s|Type - JOIN_FIN.req|Length - %d|Payload - %s",trace_array(this->device_configuration_ptr->eui64,8),(int)( ptr - data_ptr), trace_array(data_ptr, ptr - data_ptr));
    coap_service_request_send(this->secure_coap_service_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, parent_address, port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_JOINER_FINALIZATION, COAP_CT_OCTET_STREAM, data_ptr, ptr - data_ptr, thread_joiner_application_finalisation_cb);

    ns_dyn_mem_free(data_ptr);

    return 0;
}

static uint8_t *thread_joiner_application_write_channel(uint8_t *ptr, uint16_t data)
{
    *ptr++ = MESHCOP_TLV_CHANNEL; // type
    *ptr++ = 3; // length
    *ptr++ = 0; // channel page
    return common_write_16_bit(data, ptr);
}

static int thread_joiner_application_entrust_recv_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_joiner_t *this = thread_joiner_find_by_service(service_id);
    (void)source_address;
    (void)source_port;

    if (!this) {
        return -1;
    }

    if (thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_MASTER_KEY, NULL) < 16 ||
        thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA, NULL) < 8 ||
        thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_XPANID, NULL) < 8 ||
        thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_NAME, NULL) == 0 ||
        thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PSKC, NULL) < 16 ) {
        // Absolutely minimum we must have is master secret to attach.
        // If commissioner wants to be connected we must have PSKc,Name,Xpanid
        // If there is some fields missing we could attach, but timestamp must be set to 0 which will then cause synchronization
        tr_debug("Not include all TLv's");
        thci_trace("joinerError");
        return -1;
    }
    uint8_t *result_ptr;
    memcpy(this->active_configuration_ptr->data,request_ptr->payload_ptr, request_ptr->payload_len);
    this->active_configuration_ptr->length = request_ptr->payload_len;

    result_ptr = this->active_configuration_ptr->data + this->active_configuration_ptr->length;

    if (thread_meshcop_tlv_find(this->active_configuration_ptr->data, this->active_configuration_ptr->length, MESHCOP_TLV_ACTIVE_TIME_STAMP, NULL) == 0) {
        result_ptr = thread_meshcop_tlv_data_write_uint64(result_ptr,MESHCOP_TLV_ACTIVE_TIME_STAMP, 0);
    }
    if (thread_meshcop_tlv_find(this->active_configuration_ptr->data, this->active_configuration_ptr->length, MESHCOP_TLV_XPANID, NULL) == 0) {
        result_ptr = thread_meshcop_tlv_data_write(result_ptr,MESHCOP_TLV_XPANID, 8, this->configuration_ptr->extented_pan_id );
    }
    if (thread_meshcop_tlv_find(this->active_configuration_ptr->data, this->active_configuration_ptr->length, MESHCOP_TLV_CHANNEL, NULL) == 0) {
        result_ptr = thread_joiner_application_write_channel(result_ptr, this->configuration_ptr->rfChannel);
    }
    if (thread_meshcop_tlv_find(this->active_configuration_ptr->data, this->active_configuration_ptr->length, MESHCOP_TLV_PANID, NULL) == 0) {
        result_ptr = thread_meshcop_tlv_data_write_uint16(result_ptr,MESHCOP_TLV_PANID, this->configuration_ptr->panId );
    }

    this->active_configuration_ptr->length = result_ptr - this->active_configuration_ptr->data;

    // check if everything is present
    if (!thread_meshcop_tlv_list_present(this->active_configuration_ptr->data, this->active_configuration_ptr->length, mle_active_configuration_dataset_tlvs, sizeof(mle_active_configuration_dataset_tlvs))) {
        // Some fields are missing we need to sync settings during attach
        uint8_t *timestamp_ptr = NULL;
        thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_ACTIVE_TIME_STAMP, &timestamp_ptr);
        if (timestamp_ptr) {
            common_write_64_bit(0,timestamp_ptr);
        }
    }

    thread_meshcop_tlv_data_get_uint64(this->active_configuration_ptr->data, this->active_configuration_ptr->length, MESHCOP_TLV_ACTIVE_TIME_STAMP, &this->active_configuration_ptr->timestamp);

    uint32_t network_key_seq = 0;
    if (thread_meshcop_tlv_data_get_uint32(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_KEY_SEQUENCE, &network_key_seq)) {
        this->configuration_ptr->key_sequence = network_key_seq;
    }

    /*We must null out the master secret*/
#ifdef THREAD_THCI_SUPPORT
    uint8_t *master_secret_ptr;
    if (thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_NETWORK_MASTER_KEY, &master_secret_ptr) >= 16) {
        memset(master_secret_ptr,0,16);
    }
    thci_trace("Device - Joiner|Direction - recv|EUI - %s|Type - JOIN_ent.req|Length - %d|Payload - %s",trace_array(this->device_configuration_ptr->eui64,8), request_ptr->payload_len, trace_array(request_ptr->payload_ptr, request_ptr->payload_len));
#endif

    // save link configuration to NVM

    thci_trace("joinerAccepted");
    link_configuration_update(this->configuration_ptr,this->active_configuration_ptr->data,this->active_configuration_ptr->length);
    this->configuration_valid = true;
    thread_joiner_application_configuration_nvm_save(this->interface_id);

    link_configuration_trace(this->configuration_ptr);

#ifdef THREAD_THCI_SUPPORT
    thci_trace("Device - Joiner|Direction - sent|EUI - %s|Type - JOIN_ent.resp|Length - 0|Payload - ", trace_array(this->device_configuration_ptr->eui64,8));
#endif
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);

    //TODO Add delay timer of 3 seconds and then do attach
    thread_joiner_attach_if_needed(this);

    return 0;
}

int thread_joiner_application_pskd_commission_start(int8_t interface_id, uint8_t parent_address[16], uint16_t joiner_port, uint16_t panid, uint8_t xpanid[8], uint8_t channel, thread_joiner_application_commission_done_cb *done_cb)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !done_cb || !cur) {
        return -1;
    }
    if (!this->configuration_ptr) {
        this->configuration_ptr = link_configuration_create();
    }

    if (!this->configuration_ptr) {
        tr_error("Commission start failed");
        return -2;
    }
    this->configuration_valid = false;


    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_JOINER_ENTRUST, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_joiner_application_entrust_recv_cb);

    this->secure_coap_service_id = coap_service_initialize(this->interface_id, THREAD_DEFAULT_JOINER_PORT, COAP_SERVICE_OPTIONS_EPHEMERAL_PORT | COAP_SERVICE_OPTIONS_SECURE | 0x80, joiner_application_security_start_cb, joiner_application_security_done_cb);

    tr_debug("start pskd commissioning: interface %d, parent: %s, port %d", interface_id, trace_ipv6(parent_address), joiner_port);
    // save initial information
    this->configuration_ptr->panId = panid;
    this->configuration_ptr->rfChannel = channel;
    if (xpanid) {
        memcpy(this->configuration_ptr->extented_pan_id, xpanid, 8);
    }

    link_configuration_trace(this->configuration_ptr);
    //TODO this callback needs status for blacklist
    this->done_cb = done_cb;
    memcpy(this->parent_address,parent_address,16);
    this->parent_port = joiner_port;

    thci_trace("joinerDtlsSessionStarted");
    thread_joiner_application_send_finalisation(this, parent_address, joiner_port);
    return 0;
}

int thread_joiner_application_configuration_nvm_save(int8_t interface_id)
{
    tr_info("thread_joiner_application_configuration_nvm_save");

    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this) {
        return -1;
    }

    thread_nvm_store_device_configuration_write(this->device_configuration_ptr->extended_random_mac,this->device_configuration_ptr->mesh_local_eid);
    thread_nvm_store_pending_configuration_write(this->pending_configuration_ptr, sizeof(configuration_set_t));
    thread_nvm_store_seq_counter_write(this->configuration_ptr->key_sequence);
    thread_nvm_store_active_configuration_write(this->active_configuration_ptr, sizeof(configuration_set_t));

    /* allow configuration to be read in bootstrap */
    this->nvm_link_configuration_load = true;
    return 0;
}

int thread_joiner_application_update_configuration(uint8_t interface_id, uint8_t *msg_ptr, uint16_t msg_len, bool include_missing_tlvs)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    configuration_set_t *configuration_ptr;
    if (!this) {
        return -1;
    }
    configuration_ptr = ns_dyn_mem_alloc(sizeof(configuration_set_t));
    if (!configuration_ptr) {
        return -2;
    }
    memcpy(configuration_ptr,this->active_configuration_ptr,sizeof(configuration_set_t));
    this->active_configuration_ptr->length = 0;
    configuration_set_add_all_fields(this->active_configuration_ptr,msg_ptr,msg_len, meshcop_active_set_ignore, sizeof(meshcop_active_set_ignore));
    if (include_missing_tlvs) {
        configuration_set_copy_missing(this->active_configuration_ptr, configuration_ptr);
        configuration_set_remove_null_tlv(this->active_configuration_ptr);
    } else {
        configuration_set_copy_mandatory(this->active_configuration_ptr, configuration_ptr);
    }
    thread_meshcop_tlv_data_get_uint64(msg_ptr, msg_len, MESHCOP_TLV_ACTIVE_TIME_STAMP, &this->active_configuration_ptr->timestamp);
    link_configuration_update(this->configuration_ptr,msg_ptr,msg_len);
    ns_dyn_mem_free(configuration_ptr);
    thread_joiner_application_configuration_nvm_save(interface_id);

    return 0;
}
uint8_t *thread_joiner_application_active_config_tlv_list_get(uint8_t interface_id, uint16_t *length)
{
    uint8_t *result_ptr;
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->active_configuration_ptr) {
        return NULL;
    }
    *length = 0;
    thread_meshcop_tlv_list_generate(this->active_configuration_ptr->data, this->active_configuration_ptr->length,NULL, length);
    if (*length == 0){
        return NULL;
    }
    result_ptr = ns_dyn_mem_alloc(*length);
    if (!result_ptr) {
    }
    thread_meshcop_tlv_list_generate(this->active_configuration_ptr->data, this->active_configuration_ptr->length,result_ptr, length);

    return result_ptr;
}
uint8_t *thread_joiner_application_active_config_params_get(uint8_t interface_id, uint16_t *length)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->active_configuration_ptr || !length) {
        return NULL;
    }
    *length = this->active_configuration_ptr->length;
    return this->active_configuration_ptr->data;
}

uint16_t thread_joiner_application_active_config_length(uint8_t interface_id, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->active_configuration_ptr) {
        return 0;
    }
    return configuration_set_length(this->active_configuration_ptr, req_tlv_ptr, req_tlv_len, ignored_tlv_ptr, ignored_tlv_len);
}

uint8_t *thread_joiner_application_active_config_write(uint8_t interface_id, uint8_t *ptr, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    if (!this || !this->active_configuration_ptr) {
        return ptr;
    }
    return configuration_set_write(this->active_configuration_ptr, ptr,req_tlv_ptr,req_tlv_len, ignored_tlv_ptr,ignored_tlv_len);
}

int thread_joiner_application_device_configuration_length(device_configuration_s *device_configuration)
{
    int length;

    if (!device_configuration) {
        return 0;
    }
    length = 2 + 1 + 2 + 6 + 2 + device_configuration->vendor_data_len;// Status + Stack version + vendor data
    if (device_configuration->provisioning_uri_ptr) {
        length += 2 + strlen(device_configuration->provisioning_uri_ptr);
    }
    if (device_configuration->vendor_name_ptr) {
        length += 2 + strlen(device_configuration->vendor_name_ptr);
    }
    if (device_configuration->vendor_model_ptr) {
        length += 2 + strlen(device_configuration->vendor_model_ptr);
    }
    if (device_configuration->vendor_sw_version_ptr) {
        length += 2 + strlen(device_configuration->vendor_sw_version_ptr);
    }
    return length;
}

uint8_t *thread_joiner_application_device_configuration_build(uint8_t *ptr, device_configuration_s *device_configuration)
{
    if (!ptr || !device_configuration) {
        return ptr;
    }
    ptr = thread_tmfcop_tlv_data_write(ptr, MESHCOP_TLV_VENDOR_STACK_VERSION, 6, device_configuration->vendor_stack_version);
    ptr = thread_tmfcop_tlv_data_write(ptr, MESHCOP_TLV_VENDOR_DATA,
            device_configuration->vendor_data_len,
            device_configuration->vendor_data_ptr);

    if (device_configuration->provisioning_uri_ptr) {
        ptr = thread_tmfcop_tlv_data_write(ptr, MESHCOP_TLV_PROVISIONING_URL,
                                           strlen(device_configuration->provisioning_uri_ptr),
                                           (uint8_t *)device_configuration->provisioning_uri_ptr);
    }
    if (device_configuration->vendor_name_ptr) {
        ptr = thread_tmfcop_tlv_data_write(ptr, MESHCOP_TLV_VENDOR_NAME,
                                           strlen(device_configuration->vendor_name_ptr),
                                           (uint8_t *)device_configuration->vendor_name_ptr);
    }
    if (device_configuration->vendor_model_ptr) {
        ptr = thread_tmfcop_tlv_data_write(ptr, MESHCOP_TLV_VENDOR_MODEL,
                                           strlen(device_configuration->vendor_model_ptr),
                                           (uint8_t *)device_configuration->vendor_model_ptr);
    }
    if (device_configuration->vendor_sw_version_ptr) {
        ptr = thread_tmfcop_tlv_data_write(ptr, MESHCOP_TLV_VENDOR_SW_VERSION,
                                           strlen(device_configuration->vendor_sw_version_ptr),
                                           (uint8_t *)device_configuration->vendor_sw_version_ptr);
    }
    return ptr;
}

int8_t thread_joiner_application_active_configuration_update(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len, const uint8_t *ignore_ptr, uint8_t ingore_len)
{
    thread_joiner_t *this = thread_joiner_find(interface_id);
    uint64_t timestamp = 0;

    if (!this) {
        return -1;
    }
    configuration_set_add_all_fields(this->active_configuration_ptr, data_ptr, data_len, ignore_ptr, ingore_len);
    thread_meshcop_tlv_data_get_uint64(data_ptr, data_len, MESHCOP_TLV_ACTIVE_TIME_STAMP, &timestamp);
    thread_joiner_application_active_timestamp_set(this->interface_id, timestamp);
    link_configuration_update(this->configuration_ptr,this->active_configuration_ptr->data,this->active_configuration_ptr->length);
    this->configuration_valid = true;
    thread_joiner_application_configuration_nvm_save(this->interface_id);

    link_configuration_trace(this->configuration_ptr);

    return 0;
}
#endif

