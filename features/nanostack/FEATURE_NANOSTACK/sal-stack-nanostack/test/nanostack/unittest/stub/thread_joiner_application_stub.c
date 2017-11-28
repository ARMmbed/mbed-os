/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "thread_tmfcop_lib.h"
#include "shalib.h"
#include "thread_management_if.h"
#include "thread_config.h"
#include "thread_meshcop_lib.h"
#include "thread_commissioning_if.h"
#include "thread_joiner_application.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_JOINER_APPLICATION
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

const uint8_t mle_active_configuration_dataset_ignore_tlvs[] = {
        MESHCOP_TLV_ACTIVE_TIME_STAMP
};

const uint8_t mle_pending_configuration_dataset_ignore_tlvs[] = {
        MESHCOP_TLV_PENDING_TIMESTAMP
};

uint8_t mle_active_configuration_dataset_ignore_tlvs_size(void)
{
    return sizeof(mle_active_configuration_dataset_ignore_tlvs);
}

uint8_t mle_pending_configuration_dataset_ignore_tlvs_size(void)
{
    return sizeof(mle_pending_configuration_dataset_ignore_tlvs);
}

uint8_t active_configuration_dataset_tlvs_size(void)
{
    return sizeof(active_configuration_dataset_tlvs);
}

uint8_t pending_configuration_dataset_tlvs_size(void)
{
    return sizeof(pending_configuration_dataset_tlvs);
}

uint8_t mle_active_configuration_dataset_tlvs_size(void)
{
    return sizeof(mle_active_configuration_dataset_tlvs);
}

uint16_t thread_joiner_application_active_config_length(uint8_t interface_id, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignore_tlv_ptr, uint8_t ignore_tlv_len)
{
    return 0;
}
uint8_t *thread_joiner_application_active_config_write(uint8_t interface_id, uint8_t *ptr, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignore_tlv_ptr, uint8_t ignore_tlv_len)
{
    return NULL;
}
uint64_t thread_joiner_application_active_timestamp_get(int8_t interface_id)
{
    return 0;
}
int thread_joiner_application_domain_prefix_get(int8_t interface_id, uint8_t *options_ptr, uint8_t *domain_prefix_ptr, uint8_t *domain_prefix_len)
{
    return 0;
}
void thread_joiner_application_active_timestamp_set(int8_t interface_id, uint64_t timestamp)
{
    return;
}
uint8_t *thread_joiner_application_active_config_tlv_list_get(uint8_t interface_id, uint16_t *length)
{
    return NULL;
}

uint8_t mle_pending_configuration_dataset_tlvs_size(void)
{
    return sizeof(mle_pending_configuration_dataset_tlvs);
}
uint8_t entrust_dataset_tlvs_size(void)
{
    return sizeof(entrust_dataset_tlvs);
}

link_configuration_s *link_configuration_create(void)
{
    return NULL;
}

void link_configuration_delete(link_configuration_s *this)
{
}

void link_configuration_copy(link_configuration_s *this, link_configuration_s *configuration_ptr)
{
}

int thread_joiner_application_init(int8_t interface_id, device_configuration_s *device_configuration_ptr, struct link_configuration *static_configuration_ptr)
{
    return 0;
}

void thread_joiner_application_deinit(int8_t interface_id)
{
}

link_configuration_s *thread_joiner_application_get_config(int8_t interface_id)
{
    return NULL;
}

bool thread_joiner_application_nvm_operation_in_progress(int8_t interface_id)
{
    return false;
}

bool thread_joiner_application_nvm_link_configuration_load(int8_t interface_id)
{
    return false;
}

int joiner_application_security_done_cb(int8_t service_id, uint8_t IID[8], uint8_t keyblock[40])
{
    return 0;
}

int thread_joiner_application_pskd_commission_start(int8_t interface_id, uint8_t parent_address[16], uint16_t port, uint16_t panid, uint8_t xpanid[8], uint8_t channel, thread_joiner_application_commission_done_cb *done_cb)
{
    return 0;
}

int thread_joiner_application_pending_config_create(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len)
{
    return 0;
}
void thread_joiner_application_pending_config_delete(int8_t interface_id)
{
    return;
}

uint32_t thread_joiner_application_pending_timeout_get(int8_t interface_id)
{
    return 0;
}

bool thread_joiner_application_old_config_exists(int8_t interface_id)
{
    return false;
}
int thread_joiner_application_old_config_activate(int8_t interface_id)
{
    return 0;
}

int thread_joiner_application_pending_config_enable(int8_t interface_id, uint32_t timeout_in_ms)
{
    return 0;
}

void thread_joiner_application_seconds_timer(int8_t interface_id, uint32_t seconds)
{

}
void thread_joiner_application_pending_config_add_missing_fields(uint8_t interface_id)
{
}

device_configuration_s *thread_joiner_application_get_device_config(int8_t interface_id)
{
    return NULL;
}
uint64_t thread_joiner_application_pending_config_timestamp_get(int8_t interface_id)
{
	return 0;
}
int thread_joiner_application_pending_config_timestamp_set(int8_t interface_id, uint64_t timestamp)
{
	return 0;
}
uint64_t thread_joiner_application_get_pending_timestamp(int8_t interface_id)
{
    return 0;
}
bool thread_joiner_application_pending_config_exists(int8_t interface_id)
{
	return true;
}
uint32_t thread_joiner_application_pending_config_timeout_get(int8_t interface_id)
{
	return 0;
}

uint16_t thread_joiner_application_pending_config_length(uint8_t interface_id, uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignore_tlv_ptr, uint8_t ignore_tlv_len)
{
	return 0;
}

uint8_t *thread_joiner_application_pending_config_build(uint8_t interface_id, uint8_t *ptr, uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignore_tlv_ptr, uint8_t ignore_tlv_len)
{
	return ptr;
}
uint8_t *thread_joiner_application_pending_config_tlv_list_get(uint8_t interface_id, uint16_t *length)
{
	return NULL;
}

int thread_joiner_application_set_pending_timestamp(int8_t interface_id, uint64_t timestamp)
{
    return 0;
}
bool thread_joiner_application_next_pending_config_exists(int8_t interface_id)
{
    return true;
}
void thread_joiner_application_next_pending_config_save(int8_t interface_id)
{
    return;
}
uint16_t thread_joiner_application_next_pending_config_length(int8_t interface_id)
{
	return 0;
}
uint8_t *thread_joiner_application_next_pending_config_build(int8_t interface_id, uint8_t *ptr)
{
	return ptr;
}
uint64_t thread_joiner_application_next_pending_config_timestamp_get(int8_t interface_id)
{
    return 0;
}
uint32_t thread_joiner_application_next_pending_config_timeout_get(int8_t interface_id)
{
    return 0;
}
void thread_joiner_application_next_pending_config_delete(int8_t interface_id)
{
    return;
}

bool thread_joiner_application_next_active_config_exists(int8_t interface_id)
{
    return false;
}
uint16_t thread_joiner_application_next_active_config_length(int8_t interface_id)
{
    return 0;
}

uint8_t *thread_joiner_application_next_active_config_write(int8_t interface_id, uint8_t *ptr)
{
    return NULL;
}


void thread_joiner_application_next_active_config_save(int8_t interface_id)
{
    return;
}

link_configuration_s *thread_joiner_application_next_active_config_get(int8_t interface_id)
{
    return NULL;
}

void thread_joiner_application_next_active_config_delete(int8_t interface_id)
{
    return;
}

int thread_joiner_application_configuration_nvm_save(int8_t interface_id)
{
    return 0;
}

int thread_joiner_application_update_configuration(uint8_t interface_id, uint8_t *msg_ptr, uint16_t msg_len, bool include_missing_tlvs)
{
    return 0;
}
int thread_joiner_application_configuration_length(link_configuration_s *link_configuration, const uint8_t *tlv_list, uint16_t list_len)
{
    return 0;
}

uint8_t *thread_joiner_application_configuration_build(link_configuration_s *link_configuration, uint8_t *response_ptr, const uint8_t *tlv_list, uint16_t list_len)
{
    return NULL;
}

link_configuration_s *thread_joiner_application_old_config_get(int8_t interface_id)
{
    return NULL;
}

void thread_joiner_application_old_config_delete(int8_t interface_id)
{
    return;
}

link_configuration_s *thread_joiner_application_old_config_create(int8_t interface_id)
{
    return NULL;
}
int thread_joiner_application_provisioning_set(int8_t interface_id, thread_provisioning_status_e status)
{
    return 0;
}

thread_provisioning_status_e thread_joiner_application_provisioning_get(int8_t interface_id)
{
    return PROVISIONING_STATUS_REJECTED;
}
uint8_t *thread_joiner_application_device_configuration_build(uint8_t *ptr, device_configuration_s *device_configuration)
{
    return NULL;
}
int thread_joiner_application_device_configuration_length(device_configuration_s *device_configuration)
{
    return 0;
}
void thread_joiner_pending_config_activate(int8_t interface_id)
{
    return;
}

int thread_joiner_application_link_configuration_store(int8_t interface_id, link_configuration_s *link_config)
{
    return 0;
}

int thread_joiner_application_link_configuration_delete(int8_t interface_id)
{
    return 0;
}

