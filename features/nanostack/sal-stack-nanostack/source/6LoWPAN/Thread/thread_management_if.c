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
#include <string.h>
#include "ns_types.h"
#include "mlme.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "thread_management_if.h"
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include <ns_list.h>
#include <net_thread_test.h>
#include <net_ipv6_api.h>
#include "ns_trace.h"
#include "Core/include/ns_buffer.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "thread_diagnostic.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "Service_Libs/mle_service/mle_service_security.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_constants.h"
#include "6LoWPAN/Thread/thread_extension_bootstrap.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "RPL/rpl_control.h" // insanity - bootstraps shouldn't be doing each others' clean-up
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_meshcop_lib.h"
#include "thread_commissioning_if.h"
#include "shalib.h"
#include "Common_Protocols/icmpv6.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "mac_common_defines.h"
#include "mlme.h"
#include "mac_api.h"

#ifdef HAVE_THREAD
#define TRACE_GROUP "thrm"

static const uint8_t thread_discovery_key[16] = {0x78, 0x58, 0x16, 0x86, 0xfd, 0xb4, 0x58,0x0f, 0xb0, 0x92, 0x54, 0x6a, 0xec, 0xbd, 0x15, 0x66};
static const uint8_t thread_discovery_extented_address[8] = {0x35,0x06, 0xfe, 0xb8, 0x23, 0xd4, 0x87, 0x12};

uint32_t thread_delay_timer_default = THREAD_DELAY_TIMER_DEFAULT_SECONDS;
uint32_t thread_router_selection_jitter = THREAD_ROUTER_SELECTION_JITTER;
uint16_t thread_joiner_port = THREAD_DEFAULT_JOINER_PORT;

/*
 * Prototypes
 */

static void thread_discover_key_descriptor_set(struct mac_api_s *api, const uint8_t *key, uint8_t id, uint32_t key32_bit_src, uint8_t attribute_index) {
    mlme_set_t set_req;
    mlme_key_id_lookup_descriptor_t lookup_description;
    mlme_key_descriptor_entry_t key_description;
    mlme_key_device_descriptor_t dev_descriptor;
    if (key) {
        memset(lookup_description.LookupData, 0, 9);
        common_write_32_bit(key32_bit_src, lookup_description.LookupData);
        lookup_description.LookupData[4] = id;
        lookup_description.LookupDataSize = 0;
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));
        memcpy(key_description.Key, key, 16);
        key_description.KeyIdLookupList = &lookup_description;
        key_description.KeyIdLookupListEntries = 1;
        dev_descriptor.Blacklisted = false;
        dev_descriptor.DeviceDescriptorHandle = attribute_index;
        dev_descriptor.UniqueDevice = true;
        key_description.KeyDeviceList = &dev_descriptor;
        key_description.KeyDeviceListEntries = 1;

    } else {
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));
    }
    set_req.attr = macKeyTable;
    set_req.attr_index = 3; //Allwayd firth key
    set_req.value_pointer = &key_description;
    set_req.value_size = sizeof(mlme_key_descriptor_entry_t);

    api->mlme_req(api, MLME_SET, &set_req);
}

static void thread_discover_device_descriptor_set(struct mac_api_s *api, const uint8_t *device_extended_address, uint8_t attribute_index)
{
    if (!api) {
        return;
    }

    mlme_device_descriptor_t device_desc;
    mlme_set_t set_req;
    device_desc.FrameCounter = 0;
    device_desc.Exempt = false;
    device_desc.ShortAddress = 0xffff;
    memcpy(device_desc.ExtAddress, device_extended_address, 8);
    device_desc.PANId = 0xffff;


    set_req.attr = macDeviceTable;
    set_req.attr_index = attribute_index;
    set_req.value_pointer = (void*)&device_desc;
    set_req.value_size = sizeof(mlme_device_descriptor_t);
    tr_debug("Register Discovery device descriptor");
    api->mlme_req(api,MLME_SET , &set_req);
}

static void thread_discover_security_material_update(protocol_interface_info_entry_t *cur, const mlme_security_t *security_params)
{
    if (!security_params || !cur) {
        return;
    }

    if (security_params->SecurityLevel != SEC_ENC_MIC32 || security_params->KeyIdMode != MAC_KEY_ID_MODE_SRC4_IDX || security_params->KeyIndex != THREAD_DISCOVERY_SECURITY_KEY_INDEX) {
        return;
    }
    mac_description_storage_size_t buffer;
    if (common_read_32_bit(security_params->Keysource) != THREAD_DISCOVERY_SECURITY_KEY_SOURCE) {
        return;
    }

    if (!cur->mac_api || !cur->mac_api->mac_storage_sizes_get || cur->mac_api->mac_storage_sizes_get(cur->mac_api, &buffer) != 0) {
        return;
    }
    thread_discover_device_descriptor_set(cur->mac_api, thread_discovery_extented_address, buffer.device_decription_table_size -1);
}

static void thread_security_trig_pending_key(protocol_interface_info_entry_t *cur)
{
    if (cur->mac_parameters->mac_next_key_index) {

        //Call MAC Key Update
        uint8_t key_id = cur->mac_parameters->mac_next_key_index;

        mac_helper_security_key_swap_next_to_default(cur);

        if (mle_service_security_key_trig(cur->id, key_id)) {
            thread_security_update_from_mac(cur);
        }
    }
}

static void thread_mac_security_key_update_cb(protocol_interface_info_entry_t *cur, const mlme_security_t *security_params)
{
    if (!cur->thread_info || !cur->mac_parameters) {
        return;
    }
    if (cur->mac_parameters->mac_next_key_index && (security_params->KeyIndex == cur->mac_parameters->mac_next_key_index)){
        if(cur->thread_info->masterSecretMaterial.keySwitchGuardTimer == 0) {
            tr_debug("Trig Next Key");
            thread_security_trig_pending_key(cur);
        }
    } else {
        thread_discover_security_material_update(cur, security_params);
    }
}

int8_t thread_node_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode)
{
    protocol_interface_info_entry_t *cur;
    uint8_t table_size = 4;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    //Read MAC device table sizes
    mac_description_storage_size_t buffer;
    if (!cur->mac_api || !cur->mac_api->mac_storage_sizes_get || cur->mac_api->mac_storage_sizes_get(cur->mac_api, &buffer) != 0) {
        return -1;
    }

    cur->configure_flags &= ~INTERFACE_BOOTSTRAP_DEFINED;
    mac_pairwise_key_interface_unregister(cur->id);

    if (buffer.key_description_table_size < 4) {
        tr_error("MAC key_description_table_size too short %d<4", buffer.key_description_table_size);
        return -1;
    }

    switch (bootstrap_mode) {
        case NET_6LOWPAN_HOST:
        case NET_6LOWPAN_SLEEPY_HOST:
            protocol_6lowpan_host_init(cur, bootstrap_mode == NET_6LOWPAN_SLEEPY_HOST);
            break;

        case NET_6LOWPAN_ROUTER:
            protocol_6lowpan_router_init(cur);
            table_size = buffer.key_description_table_size;
            break;

        default:
            return -3;
    }

    if (mac_pairwise_key_interface_register(cur->id, table_size, 4) < 0) {
        tr_error("MAC pairwise key in registration failed");
        return -1;
    }

    if (blacklist_init() != 0) {
        tr_debug("6LoWPAN MLE blacklist init failed.");
        return -1;
    }

    blacklist_params_set(
        THREAD_BLACKLIST_ENTRY_LIFETIME,
        THREAD_BLACKLIST_TIMER_MAX_TIMEOUT,
        THREAD_BLACKLIST_TIMER_TIMEOUT,
        THREAD_BLACKLIST_ENTRY_MAX_NBR,
        THREAD_BLACKLIST_PURGE_NBR,
        THREAD_BLACKLIST_PURGE_TIMER_TIMEOUT);

    if (thread_info_allocate_and_init(cur) < 0) {
        mac_pairwise_key_interface_unregister(cur->id);
        return -3;
    }

    if (thread_discovery_init(cur->id, cur, thread_info(cur)->version, bootstrap_mode == NET_6LOWPAN_ROUTER) != 0) {
        tr_error("Discovery init fail");
        mac_pairwise_key_interface_unregister(cur->id);
        return -3;
    }

    cur->configure_flags |= INTERFACE_BOOTSTRAP_DEFINED;
    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_MLE;
    rpl_control_remove_domain_from_interface(cur);
    //SET MAC key id mode 2 key and device
    thread_discover_key_descriptor_set(cur->mac_api, thread_discovery_key,THREAD_DISCOVERY_SECURITY_KEY_INDEX, THREAD_DISCOVERY_SECURITY_KEY_SOURCE,buffer.device_decription_table_size -1);

    thread_discover_device_descriptor_set(cur->mac_api, thread_discovery_extented_address, buffer.device_decription_table_size -1);

    cur->mac_security_key_usage_update_cb = thread_mac_security_key_update_cb;
    return 0;
}


void arm_thread_private_ula_prefix_set(protocol_interface_info_entry_t *cur, const uint8_t *ula_prefix)
{
    //Define Local Thread
    memcpy(cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, ula_prefix, 8);
    cur->thread_info->threadPrivatePrefixInfo.ulaValid = true;
}

/**
 * Calculate New Key Material by given security key material and sequency number
 *
 * \param key Thread Master key pointer
 * \param key_material_buf Buffer for calculated MAC & MLE key: first 128-bit for MLE and rest 128-bit for MAC
 * \param threadSequency Key material key index.
 *
 */
void thread_key_get(uint8_t *key, uint8_t *key_material_buf,  uint32_t key_sequence_counter)
{
    static uint8_t key_seed[6] = {'T', 'h', 'r', 'e', 'a', 'd'};
    static uint8_t temp32_buf[4];
    common_write_32_bit(key_sequence_counter, temp32_buf);
    SHALIB_init_HMAC(key, 16);
    SHALIB_push_data_HMAC(temp32_buf, 4);
    SHALIB_push_data_HMAC(key_seed, 6);
    SHALIB_finish_HMAC(key_material_buf, 8);
}

/**
 * Increment Thread key sequence counter
 *
 * \param interface_id Network Interface
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
uint8_t *thread_management_key_request(int8_t interface_id, uint8_t keyId)
{
    protocol_interface_info_entry_t *cur;
    uint8_t *keyPtr = NULL;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return NULL;
    }
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->thread_info) {
        if (cur->thread_info->masterSecretMaterial.valid_Info) {

            if (cur->thread_info->masterSecretMaterial.historyKeyValid && (cur->thread_info->masterSecretMaterial.historyKeyId == keyId)) {

                keyPtr = cur->thread_info->masterSecretMaterial.historyKey;

            } else {
                uint32_t thrSequenceCounter;
                uint8_t compId = 0xff, keyIdDiff;
                thrSequenceCounter = linkConfiguration->key_sequence + 1;
                //Calculate Current Next key ID
                compId = THREAD_KEY_INDEX(thrSequenceCounter);

                if (keyId > compId) {
                    keyIdDiff = (keyId - compId);
                } else {
                    keyIdDiff = (128 - (compId - keyId));
                }

                if (keyIdDiff > 64) {
                    //Calc Temp Key
                    thrSequenceCounter -= (128 - keyIdDiff);
                    tr_debug("Gen temporary key id %"PRIu8" seq %"PRIu32, keyId, thrSequenceCounter);
                    thread_key_get(linkConfiguration->master_key, cur->thread_info->masterSecretMaterial.historyKey, thrSequenceCounter);
                    cur->thread_info->masterSecretMaterial.historyKeyId = keyId;
                    cur->thread_info->masterSecretMaterial.historyKeyValid = true;
                    keyPtr = cur->thread_info->masterSecretMaterial.historyKey;
                } else {
                    tr_debug("Gen new key id %"PRIu8" %"PRIu8" diff", keyId, keyIdDiff);
                    thrSequenceCounter += keyIdDiff;
                    //Generate
                    tr_debug("Missed n key Update...generated missed key %"PRIu8" update by seq %"PRIu32, keyIdDiff, thrSequenceCounter);
                    thread_management_key_sets_calc(cur, linkConfiguration, thrSequenceCounter);
                    //Get Default key
                    keyPtr =  mle_service_security_default_key_get(interface_id);
                }
                thread_nvm_store_seq_counter_write(linkConfiguration->key_sequence);
            }
        }
    }
    return keyPtr;
}

/**
 * Thread key sequence & key synch
 *
 */
void thread_management_key_synch_req(int8_t interface_id, uint32_t keySequnce)
{
    protocol_interface_info_entry_t *cur;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->thread_info) {
        if (cur->thread_info->masterSecretMaterial.valid_Info) {
            if (keySequnce != linkConfiguration->key_sequence) {
                if ((cur->thread_info->masterSecretMaterial.keySwitchGuardTimer == 0 && keySequnce > linkConfiguration->key_sequence)) {
                    tr_debug("Sync key material by %"PRIu32, keySequnce);
                    thread_management_key_sets_calc(cur, linkConfiguration, keySequnce);
                    thread_calculate_key_guard_timer(cur, linkConfiguration, false);
                }
            }
        }
    }
}

void thread_history_key_material_push(thread_info_t *thread_info, uint8_t *mleKeyPtr, uint8_t keyId)
{
    thread_info->masterSecretMaterial.historyKeyValid = true;
    thread_info->masterSecretMaterial.historyKeyId = keyId;
    memcpy(thread_info->masterSecretMaterial.historyKey, mleKeyPtr, 16);
}

void thread_security_update_from_mac(protocol_interface_info_entry_t *cur)
{
    uint8_t *mleKey;
    uint8_t historyKeyId;
    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return;
    }

    mleKey = mle_service_security_next_key_get(cur->id);
    if (!mleKey) {
        return;
    }
    historyKeyId = mle_service_security_next_key_id_get(cur->id);

    /* Set old secondary to history */
    thread_history_key_material_push(cur->thread_info, mleKey, historyKeyId);
    linkConfiguration->key_sequence++;
    mac_helper_link_frame_counter_set(cur->id, 0);
    thread_security_next_key_generate(cur, linkConfiguration->master_key, linkConfiguration->key_sequence);
    thread_calculate_key_guard_timer(cur, linkConfiguration, false);
}

void thread_security_key_generate(protocol_interface_info_entry_t *cur, uint8_t *masterKey, uint32_t keySequence)
{
    uint8_t key_material[32];
    uint8_t key_index;
    //primary key is generated from the current value of sequence counter
    uint32_t thrKeySequenceCounter = keySequence;
    /* Produced keys from Thread security material: MAC key | MLE key */
    thread_key_get(masterKey, key_material, thrKeySequenceCounter);
    /* Update keys as primary keys */
    key_index = THREAD_KEY_INDEX(thrKeySequenceCounter);
    tr_debug("Set key Id: %u", key_index);
    mac_helper_security_default_key_set(cur, &key_material[16], key_index, MAC_KEY_ID_MODE_IDX);
    mle_service_security_set_security_key(cur->id, key_material, key_index, true);
}

void thread_security_next_key_generate(protocol_interface_info_entry_t *cur, uint8_t *masterKey, uint32_t keySequence)
{
    uint8_t key_material[32];
    uint8_t key_index;
    uint32_t thrKeySequenceCounter;
    /* Will wrap modulo 32 bits */
    thrKeySequenceCounter = keySequence + 1;
    /* Produced keys from Thread security material: MAC key | MLE key */
    thread_key_get(masterKey, key_material, thrKeySequenceCounter);
    /* Update keys as non-primary keys */
    key_index = THREAD_KEY_INDEX(thrKeySequenceCounter);
    tr_debug("Set next key Id: %u", key_index);
    mac_helper_security_next_key_set(cur, &key_material[16], key_index, MAC_KEY_ID_MODE_IDX); /* Auth counter not relevant for Thread */
    mle_service_security_set_security_key(cur->id, key_material, key_index, false);
}

int thread_management_key_sets_calc(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration, uint32_t thrKeySequenceCounter)
{
    int ret_val = -1;

    if (thrKeySequenceCounter == linkConfiguration->key_sequence) {
        /* Same key - do not change */
        ret_val = 0;
    } else if (thrKeySequenceCounter == (linkConfiguration->key_sequence + 1)) {
        /* Next key - trig pending key */
        thread_security_trig_pending_key(cur);
        ret_val = 0;
    } else {
        /* Generate new key set */
        uint8_t key_material[32];
        uint8_t key_index;
        thread_nvm_fast_data_t fast_data;
        //SET History key, Current Key & Next Key
        //Generate History Key
        //Clean All Keys
        mac_helper_security_key_clean(cur);
        thrKeySequenceCounter--;
        /* Update keys as non-primary keys */
        thread_key_get(linkConfiguration->master_key, key_material, thrKeySequenceCounter);
        key_index = THREAD_KEY_INDEX(thrKeySequenceCounter);
        //copy master secret material to history
        memcpy(cur->thread_info->masterSecretMaterial.historyKey, key_material, 32);
        cur->thread_info->masterSecretMaterial.historyKeyId = key_index;
        cur->thread_info->masterSecretMaterial.historyKeyValid = true;

        mac_helper_security_prev_key_set(cur, &key_material[16], key_index, MAC_KEY_ID_MODE_IDX);
        //Set New Active Key
        thrKeySequenceCounter++;
        linkConfiguration->key_sequence = thrKeySequenceCounter;
        fast_data.seq_counter = thrKeySequenceCounter;
        fast_data.mac_frame_counter = 0;
        fast_data.mle_frame_counter = mle_service_security_get_frame_counter(cur->interface_mode);
        thread_nvm_store_fast_data_write(&fast_data);
        mac_helper_link_frame_counter_set(cur->id, 0);
        thread_security_key_generate(cur,linkConfiguration->master_key,linkConfiguration->key_sequence);
        thread_security_next_key_generate(cur,linkConfiguration->master_key,linkConfiguration->key_sequence);
        ret_val = 0;
    }
    return ret_val;
}

int thread_management_get_my_iid16(int8_t interface_id, uint8_t *iidPtr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!cur->thread_info->threadPrivatePrefixInfo.ulaValid) {
        return -1;
    }

    memcpy(iidPtr, ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(mac_helper_mac16_address_get(cur), (iidPtr + 6));

    return 0;
}

int thread_management_get_current_keysequence(int8_t interface_id, uint32_t *sequencePtr)
{

    link_configuration_s *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        return -1;
    }
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (!cur->thread_info->masterSecretMaterial.valid_Info) {
        return -1;
    }

    *sequencePtr = linkConfiguration->key_sequence;

    return 0;
}

int thread_management_increment_key_sequence_counter(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur && cur->thread_info) {
        /* Not sure if this check is needed - not sure exactly what the flag means! */
        if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
            return -1;
        }

        if ((cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED) == 0) {
            return -1;
        }

        //Trig Pending Key
        thread_security_trig_pending_key(cur);
        return 0;
    }
    return -1;
}

int thread_management_get_ml_prefix(int8_t interface_id, uint8_t *prefix_ptr)
{// TODO get from static configuration
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !prefix_ptr) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!cur->thread_info->threadPrivatePrefixInfo.ulaValid) {
        return -1;
    }

    memcpy(prefix_ptr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);

    return 0;
}

/* Get my mesh local prefix /112 */
int thread_management_get_ml_prefix_112(int8_t interface_id, uint8_t *prefix_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !prefix_ptr) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!cur->thread_info->threadPrivatePrefixInfo.ulaValid) {
        return -1;
    }

    memcpy(prefix_ptr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(prefix_ptr + 8, ADDR_SHORT_ADR_SUFFIC, 6);

    return 0;
}

#endif

/**
 *  Public interface functions.
 */

/**
 * Set DHCPV6 server for Thread GP data purpose
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix
 *
 * return 0, Set OK
 * return <0 Set Not OK
 */
int thread_dhcpv6_server_add(int8_t interface_id, uint8_t *prefix_ptr, uint32_t max_client_cnt, bool stableData)
{
#ifdef HAVE_DHCPV6_SERVER
    protocol_interface_info_entry_t *cur;
    thread_prefix_tlv_t prefixTlv;
    thread_border_router_tlv_entry_t service;
    uint8_t temp[16];
    uint8_t *ptr = temp;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    if (DHCPv6_server_service_init(interface_id, prefix_ptr, cur->mac, DHCPV6_DUID_HARDWARE_EUI64_TYPE) != 0) {
        tr_warn("SerVER alloc fail");
        return -1;
    }

    prefixTlv.domainId = 0;
    prefixTlv.Prefix = prefix_ptr;
    prefixTlv.PrefixLen = 64;

    memset(&service, 0, sizeof(thread_border_router_tlv_entry_t));
    service.Prf = 1;
    service.P_dhcp = true;
    service.P_on_mesh = true;
    service.stableData = stableData;

    //SET Timeout
    DHCPv6_server_service_set_address_validlifetime(interface_id, prefix_ptr, THREAD_MIN_PREFIX_LIFETIME);

    // SET maximum number of accepted clients
    DHCPv6_server_service_set_max_clients_accepts_count(interface_id, prefix_ptr, max_client_cnt);

    //Enable Mapping
    //DHCPv6_server_service_set_gua_address_mapping(interface_id,prefix_ptr, true, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix);
    tr_debug("GUA server Generate OK");
    memcpy(ptr, prefix_ptr, 8);
    memset(ptr + 8, 0, 8);
    //Network Data
    if (thread_local_server_list_add_on_mesh_server(&cur->thread_info->localServerDataBase, &prefixTlv, &service) != 0) {
        return -1;
    }

    return 0;
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) max_client_cnt;
    (void) stableData;
    return -1;
#endif
}

int thread_dhcpv6_server_set_lifetime(int8_t interface_id, uint8_t *prefix_ptr, uint32_t valid_lifetime)
{
#ifdef HAVE_DHCPV6_SERVER
    if (!prefix_ptr) {
        return -1;
    }

    return DHCPv6_server_service_set_address_validlifetime(interface_id, prefix_ptr, valid_lifetime);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) valid_lifetime;
    return -1;
#endif
}

int thread_dhcpv6_server_set_max_client(int8_t interface_id, uint8_t *prefix_ptr, uint32_t max_client_count)
{
#ifdef HAVE_DHCPV6_SERVER
    if (!prefix_ptr) {
        return -1;
    }

    return DHCPv6_server_service_set_max_clients_accepts_count(interface_id, prefix_ptr, max_client_count);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) max_client_count;
    return -1;
#endif
}

int thread_dhcpv6_server_set_anonymous_addressing(int8_t interface_id, uint8_t *prefix_ptr, bool anonymous)
{
#ifdef HAVE_DHCPV6_SERVER
    if (!prefix_ptr) {
        return -1;
    }

    return DHCPv6_server_service_set_address_autonous_flag(interface_id, prefix_ptr, anonymous);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) anonymous;
    return -1;
#endif

}


int thread_dhcpv6_server_delete(int8_t interface_id, uint8_t *prefix_ptr)
{
#ifdef HAVE_DHCPV6_SERVER
    uint8_t temp[16];
    protocol_interface_info_entry_t *cur;
    thread_prefix_tlv_t prefixTlv;
    tr_debug("GUA server Delete");
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info) {
        return -1;
    }

    memcpy(temp, prefix_ptr, 8);
    memset(temp + 8, 0, 8);

    prefixTlv.domainId = 0;
    prefixTlv.Prefix = prefix_ptr;
    prefixTlv.PrefixLen = 64;
    //Delete dhcp service
    DHCPv6_server_service_delete(interface_id, prefix_ptr, false);
    ipv6_route_delete(temp, 64, cur->id, NULL, ROUTE_THREAD);
    thread_local_server_list_del_on_mesh_server(&cur->thread_info->localServerDataBase, &prefixTlv);

    return 0;
#else
    (void) interface_id;
    (void) prefix_ptr;
    return -1;
#endif
}

#ifdef HAVE_THREAD
void thread_comm_status_indication_cb(int8_t if_id, const mlme_comm_status_t* status)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(if_id);
    if (!cur) {
        return;
    }

    switch (status->status) {

        case MLME_SECURITY_FAIL:

            break;

        case MLME_COUNTER_ERROR:

            break;

        case MLME_DATA_POLL_NOTIFICATION:
            mle_refresh_entry_timeout(if_id, status->SrcAddr, (addrtype_t)status->SrcAddrMode, false);
            break;
        default:
            break;
    }
}
#endif

int thread_management_node_init(
    int8_t interface_id,
    channel_list_s *channel_list,
    device_configuration_s *device_configuration,
    link_configuration_s *static_configuration)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    nwk_scan_params_t *scan_params;

    if (!device_configuration) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    if (!cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -1;
    }

    if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) !=  0) {
        return -1;
    }

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER || cur->border_router_setup) {
        return -1;
    }

    if (thread_init(cur) < 0) {
        thread_nd_service_delete(cur->id);
        tr_warn("Thread Boostarp Init Fail");
        return -1;
    }
    if (thread_management_server_init(cur->id) != 0) {
        tr_warn("Thread management server init fail");
        return -1;
    }

    if (thread_diagnostic_init(cur->id) != 0) {
        tr_warn("Thread diagnostic init fail");
        return -1;
    }
    //Store setup to application Joiner Application
    if (thread_joiner_application_init(cur->id, device_configuration, static_configuration) != 0) {
        tr_error("mandatory device configuration missing");
        return -2;
    }

    // Joiner application copies and massages the device configuration - switch to massaged version
    device_configuration = thread_joiner_application_get_device_config(cur->id);
    if (!device_configuration) {
        return -1;
    }
    cur->if_lowpan_security_params->nwk_security_mode = NET_SEC_MODE_NO_LINK_SECURITY;
    cur->mac_parameters->mac_configured_sec_level = 0;

    // If no-one has already set a secret key, use the EUI-64 - Thread
    // wants RFC 7217 IIDs. Also ensure they're on for the Thread interface.
    // (Note that this will likely enable opaque IIDs on Ethernet too).
    if (!addr_opaque_iid_key_is_set()) {
        // Could we include some private key here?
        arm_nwk_ipv6_opaque_iid_key(device_configuration->eui64, 8);
        arm_nwk_ipv6_opaque_iid_enable(cur->id, true);
    }
    // Copy the channel list
    memset(&cur->mac_parameters->mac_channel_list,0,sizeof(channel_list_s));
    if (channel_list) {
        // Application has given limited set of channels
        cur->mac_parameters->mac_channel_list = *channel_list;
    } else {
        cur->mac_parameters->mac_channel_list.channel_page = CHANNEL_PAGE_0;
        cur->mac_parameters->mac_channel_list.channel_mask[0] = 0x07FFF800;
    }

    scan_params = &cur->mac_parameters->nwk_scan_params;
    memset(&scan_params->stack_chan_list,0,sizeof(channel_list_s));
    if (channel_list) {
        // Application has given limited set of channels
        scan_params->stack_chan_list = *channel_list;
    } else {
        scan_params->stack_chan_list.channel_page = CHANNEL_PAGE_0;
        scan_params->stack_chan_list.channel_mask[0] = 0x07FFF800;
    }
    scan_params->scan_duration = 5;

    cur->thread_info->masterSecretMaterial.valid_Info = false;
    thread_calculate_key_guard_timer(cur, static_configuration, true);

    cur->thread_info->maxChildCount = THREAD_MAX_CHILD_COUNT;
    cur->thread_info->rfc6775 = false;
    cur->thread_info->host_link_timeout = THREAD_END_DEVICE_DEFAULT_TIMEOUT;
    /* Thread will manage the address query timing, and report negatives. Set this high so as not to interfere. */
    cur->ipv6_neighbour_cache.retrans_timer = 10000;

    // Set default partition weighting
    cur->thread_info->partition_weighting = THREAD_DEFAULT_WEIGHTING;

    /* IP forwarding is off by default */
    cur->ip_forwarding = false;

    lowpan_adaptation_indirect_queue_params_set(cur,
        THREAD_INDIRECT_BIG_PACKET_THRESHOLD,
        THREAD_INDIRECT_BIG_PACKETS_TOTAL,
        THREAD_INDIRECT_SMALL_PACKETS_PER_CHILD);

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST) {
        cur->thread_info->requestFullNetworkData = false;
    } else {
        cur->thread_info->requestFullNetworkData = true;
    }

    cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
    cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
    cur->comm_status_ind_cb = thread_comm_status_indication_cb;
    return 0;
#else
    (void) interface_id;
    (void) channel_list;
    (void) device_configuration;
    (void) static_configuration;
    return -1;
#endif
}

int thread_management_device_type_set(int8_t interface_id, thread_device_type_e device_type)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -1;
    }
    if (device_type == THREAD_DEVICE_REED) {
        // Change mode to router
        cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER;
    }
    else if (device_type == THREAD_DEVICE_FED) {
        //FED devices makes links and makes address resolutions
        cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST;
        cur->thread_info->end_device_link_synch = true;
    }
    else if (device_type == THREAD_DEVICE_MED) {
        cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST;
        cur->thread_info->end_device_link_synch = false;
    }
    else if (device_type == THREAD_DEVICE_SED) {
        cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST;
        cur->thread_info->end_device_link_synch = false;
    }
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active need to restart
        thread_bootstrap_reset_restart(interface_id);
    }

    return 0;
#else
    (void) interface_id;
    (void) device_type;
    return -1;
#endif
}

int thread_management_max_child_count(
    int8_t interface_id,
    uint8_t maxChildCount)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }

    if (!cur->thread_info) {
        tr_warn("Not Thread specific interface");
        return -1;
    }

    mac_description_storage_size_t buffer;
    if (!cur->mac_api || !cur->mac_api->mac_storage_sizes_get || cur->mac_api->mac_storage_sizes_get(cur->mac_api, &buffer) != 0) {
        return -1;
    }

    if (maxChildCount > buffer.device_decription_table_size) {
        tr_error("Accept values are between 0-%d for max Child count", buffer.device_decription_table_size);
        return -1;
    }
    cur->thread_info->maxChildCount = maxChildCount;
    return 0;
#else
    (void) interface_id;
    (void) maxChildCount;
    return -1;
#endif
}

link_configuration_s *thread_management_configuration_get(int8_t interface_id)
{
#ifdef HAVE_THREAD
    return thread_joiner_application_get_config(interface_id);
#else
    (void) interface_id;
    return NULL;
#endif
}

device_configuration_s *thread_management_device_configuration_get(int8_t interface_id)
{
#ifdef HAVE_THREAD
    return thread_joiner_application_get_device_config(interface_id);
#else
    (void) interface_id;
    return NULL;
#endif
}

int thread_management_link_configuration_store(int8_t interface_id, link_configuration_s *link_config)
{
#ifdef HAVE_THREAD
    int ret = thread_joiner_application_link_configuration_store(interface_id, link_config);

    if (interface_id == -1) {
        return ret;
    }

    if (ret >= 0) {
        protocol_interface_info_entry_t *cur =  protocol_stack_interface_info_get_by_id(interface_id);
        if (!link_config || !cur  || !cur->thread_info) {
            return -2;
        }
        if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) !=  0) {
            // take new settings into use after restart
            thread_bootstrap_reset_restart(interface_id);
        }
    }

    return ret;
#else
    (void) interface_id;
    (void) link_config;
    return -1;
#endif
}

int thread_management_link_configuration_add(int8_t interface_id, uint8_t *additional_ptr, uint8_t additional_len)
{
#ifdef HAVE_THREAD
    if (interface_id < 0) {
        return -1;
    }

    int ret = thread_joiner_application_update_configuration(interface_id, additional_ptr, additional_len, true);
    if (ret != 0) {
        return ret;
    }

    protocol_interface_info_entry_t *cur =  protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur  || !cur->thread_info) {
        return -2;
    }
    if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) !=  0) {
        // take new settings into use after restart
        thread_bootstrap_reset_restart(interface_id);
    }

    return ret;
#else
    (void) interface_id;
    (void) additional_ptr;
    (void) additional_len;
    return -1;
#endif
}

int thread_management_link_configuration_delete(int8_t interface_id)
{
#ifdef HAVE_THREAD
    int ret = thread_joiner_application_link_configuration_delete(interface_id);

    if (interface_id == -1) {
        return ret;
    }

    if (ret >= 0) {
        protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
        if (!cur  || !cur->thread_info) {
            return -2;
        }
        if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) !=  0) {
            thread_bootstrap_reset_restart(interface_id);
        }
    }
    return ret;
#else
    (void) interface_id;
    return -1;
#endif
}

int thread_management_get_leader_address(int8_t interface_id, uint8_t *address_buffer)
{
#ifdef HAVE_THREAD

    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if ((cur->thread_info) && (thread_attach_ready(cur) == 0)  &&
            (cur->thread_info->threadPrivatePrefixInfo.ulaValid) ) {
                thread_addr_write_mesh_local_16(address_buffer, thread_router_addr_from_id(cur->thread_info->thread_leader_data->leaderRouterId), cur->thread_info);
                return 0;
        }
    }
    return -1;
#else
    (void) interface_id;
    (void) address_buffer;
    return -1;
#endif
}

int thread_management_get_leader_aloc(int8_t interface_id, uint8_t *address_buffer)
{
#ifdef HAVE_THREAD

    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        if ((cur->thread_info) && (thread_attach_ready(cur) == 0)  &&
            (cur->thread_info->threadPrivatePrefixInfo.ulaValid)) {
                thread_addr_write_mesh_local_16(address_buffer, 0xfc00, cur->thread_info);
                return 0;
        }
    }
    return -1;
#else
    (void) interface_id;
    (void) address_buffer;
    return -1;
#endif
}
int thread_management_get_ml64_address(int8_t interface_id, uint8_t *address_ptr)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!address_ptr) {
        return -1;
    }

    if (0 != thread_management_get_ml_prefix(interface_id, address_ptr)) {
        return -2;
    }

    memcpy(&address_ptr[8], cur->iid_slaac, 8);
    return 0;
#else
    (void) interface_id;
    (void) address_ptr;
    return -1;
#endif
}

int thread_management_get_ml16_address(int8_t interface_id, uint8_t *address_ptr)
{
#ifdef HAVE_THREAD
    if (!address_ptr) {
        return -1;
    }

    if (0 != thread_management_get_ml_prefix(interface_id, address_ptr)) {
        return -2;
    }

    if (0 != thread_management_get_my_iid16(interface_id, address_ptr + 8)) {
        return -2;
    }

    return 0;
#else
    (void) interface_id;
    (void) address_ptr;
    return -1;
#endif
}

int thread_management_get_parent_address(int8_t interface_id, uint8_t *address_ptr)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info || !address_ptr) {
        return -1;
    }
    memset(address_ptr,0,16);
    if (cur->thread_info->thread_endnode_parent) {
        memcpy(address_ptr, ADDR_LINK_LOCAL_PREFIX, 8);
        address_ptr += 8;
        memcpy(address_ptr, cur->thread_info->thread_endnode_parent->mac64, 8);
        *address_ptr ^= 2;
    }

    return 0;
#else
    (void) interface_id;
    (void) address_ptr;
    return -1;
#endif
}

int thread_management_get_commissioner_address(int8_t interface_id, uint8_t *address_ptr, uint16_t *port_ptr)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info|| !address_ptr ) {
        return -1;
    }

    if (!cur->thread_info->registered_commissioner.commissioner_valid) {
        return -2;
    }
    memcpy(address_ptr,cur->thread_info->threadPrivatePrefixInfo.ulaPrefix,8);
    memcpy(address_ptr + 8, ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(0xfc30 + (cur->thread_info->registered_commissioner.session_id % 8), address_ptr + 14);

    if (port_ptr) {
        *port_ptr = THREAD_MANAGEMENT_PORT;// Default commissioner port
    }

    return 0;
#else
    (void) interface_id;
    (void) address_ptr;
    (void) port_ptr;
    return -1;
#endif
}
int8_t thread_management_set_link_timeout(int8_t interface_id, uint32_t link_timeout)
{
#ifdef HAVE_THREAD
   protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    thread_info_t *thread = cur->thread_info;
    if (!thread) {
        tr_warn("Thread not active");
        return -2;
    }
    tr_info("set new link timeout %"PRIu32" , old value %"PRIu32"", link_timeout, thread->host_link_timeout);
    thread->host_link_timeout = link_timeout;
    thread_bootstrap_child_update_trig(cur);
    return 0;
#else
    (void) interface_id;
    (void) link_timeout;
    return -1;
#endif
}

int8_t thread_management_get_link_timeout(int8_t interface_id, uint32_t *link_timeout)
{
#ifdef HAVE_THREAD
    const protocol_interface_info_entry_t *cur;

    if(!link_timeout) {
        tr_warn("Invalid input ptr");
        return -3;
    }
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    const thread_info_t *thread = cur->thread_info;
    if (!thread) {
        tr_warn("Thread not active");
        return -2;
    }
    *link_timeout = thread->host_link_timeout;
    return 0;
#else
    (void) interface_id;
    (void) link_timeout;
    return -1;
#endif
}

int8_t thread_management_set_request_full_nwk_data(int8_t interface_id, bool full_nwk_data)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    if (!cur->thread_info) {
        tr_warn("Thread not active");
        return -2;
    }
    if (cur->thread_info->requestFullNetworkData != full_nwk_data) {
        cur->thread_info->requestFullNetworkData = full_nwk_data;
        thread_bootstrap_child_update_trig(cur);
    }
    return 0;
#else
    (void) interface_id;
    (void) full_nwk_data;
    return -1;
#endif
}

int8_t thread_management_get_request_full_nwk_data(int8_t interface_id, bool *full_nwk_data)
{
#ifdef HAVE_THREAD
    const protocol_interface_info_entry_t *cur;

    if(!full_nwk_data) {
        tr_warn("Invalid input ptr");
        return -3;
    }
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("Invalid interface id");
        return -1;
    }
    if (!cur->thread_info) {
        tr_warn("Thread not active");
        return -2;
    }
    *full_nwk_data = cur->thread_info->requestFullNetworkData;
    return 0;
#else
    (void) interface_id;
    (void) full_nwk_data;
    return -1;
#endif
}

int thread_management_device_certificate_set(int8_t interface_id, const unsigned char *device_certificate_ptr, uint16_t device_certificate_len, const unsigned char *priv_key_ptr, uint16_t priv_key_len)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_warn("invalid interface id");
        return -1;
    }

    return thread_extension_bootstrap_device_certificate_set(cur, device_certificate_ptr, device_certificate_len, priv_key_ptr, priv_key_len);

#else
    (void) interface_id;
    (void) device_certificate_ptr;
    (void) device_certificate_len;
    (void) priv_key_ptr;
    (void) priv_key_len;
    return -1;
#endif
}
int thread_management_network_certificate_set(int8_t interface_id, const unsigned char *network_certificate_ptr, uint16_t network_certificate_len, const unsigned char *priv_key_ptr, uint16_t priv_key_len)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        tr_debug("invalid interface id");
        return -1;
    }

    if (0 > thread_extension_bootstrap_network_certificate_set(cur, network_certificate_ptr, network_certificate_len)) {
        return -1;
    }

    return thread_extension_bootstrap_network_private_key_set(cur, priv_key_ptr, priv_key_len);
#else
    (void) interface_id;
    (void) network_certificate_ptr;
    (void) network_certificate_len;
    (void) priv_key_ptr;
    (void) priv_key_len;
    return -1;
#endif
}

int thread_management_partition_weighting_set(int8_t interface_id, uint8_t partition_weighting)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        tr_debug("Invalid interface id");
        return -1;
    }

    if (cur->thread_info->partition_weighting == partition_weighting) {
        return 0;
    }

    cur->thread_info->partition_weighting = partition_weighting;
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active and weighting has changed
        thread_bootstrap_reset_restart(interface_id);
    }

    return 0;
#else
    (void) interface_id;
    (void) partition_weighting;
    return -1;
#endif
}
