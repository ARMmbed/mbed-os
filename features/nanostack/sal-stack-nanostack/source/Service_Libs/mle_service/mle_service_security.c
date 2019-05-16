/*
 * Copyright (c) 2015-2019, Arm Limited and affiliates.
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
 * \file mle_service_security.c
 * \brief Add short description about this file!!!
 *
 */

#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "ccmLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/ns_address_internal.h"
#include "Core/include/ns_buffer.h"
#include "MLE/mle.h"
#include "mac_common_defines.h"
#include "Service_Libs/mle_service/mle_service_security.h"

#define TRACE_GROUP "mlSS"

typedef struct {
    mle_security_components_t security_params;
    int8_t interface_id;
    ns_list_link_t link;
} mle_service_security_isntance_list_t;

typedef NS_LIST_HEAD(mle_service_security_isntance_list_t, link) mle_service_security_instance_list_t;

static mle_service_security_instance_list_t srv_security_instance_list;


static mle_security_key_t *mle_service_security_key_entry_get(mle_security_components_t *sec_ptr, bool primary);

static mle_service_security_isntance_list_t *mle_service_security_instance_get(int8_t interface_id)
{
    ns_list_foreach(mle_service_security_isntance_list_t, cur_ptr, &srv_security_instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

mle_security_components_t *mle_service_security_params_get(int8_t interface_id)
{
    ns_list_foreach(mle_service_security_isntance_list_t, cur_ptr, &srv_security_instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            return &cur_ptr->security_params;
        }
    }
    return NULL;
}


int mle_service_security_instance_allocate(int8_t interface_id)
{
    mle_service_security_isntance_list_t *srv_ptr = mle_service_security_instance_get(interface_id);

    if (!srv_ptr) {
        srv_ptr = ns_dyn_mem_alloc(sizeof(mle_service_security_isntance_list_t));
        if (!srv_ptr) {
            return -1;
        }
        ns_list_add_to_start(&srv_security_instance_list, srv_ptr);
        srv_ptr->interface_id = interface_id;
        srv_ptr->security_params.key_req = NULL;
        srv_ptr->security_params.security_notify = NULL;
        srv_ptr->security_params.security_frame_counter = 0;
        srv_ptr->security_params.sec_level = 0;
    }

    return 0;
}

int mle_service_security_instance_delete(int8_t interface_id)
{
    ns_list_foreach(mle_service_security_isntance_list_t, cur_ptr, &srv_security_instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            ns_list_remove(&srv_security_instance_list, cur_ptr);
            ns_dyn_mem_free(cur_ptr);
            return 0;
        }
    }
    return -1;
}

mle_security_key_t *mle_service_security_key_get(mle_security_components_t *sec_ptr, uint8_t key_id)
{
    uint8_t i;
    mle_security_key_t *key;
    for (i = 0; i < MLE_MAX_KEY_TABLE_SIZE; i++) {
        key = &sec_ptr->mle_security_key_table[i];

        if (key->key_valid && key->key_id == key_id) {
            return key;
        }
    }
    return NULL;
}


static mle_security_key_t *mle_service_security_key_get_free_entry(mle_security_components_t *sec_ptr, bool primary)
{
    uint8_t i;
    mle_security_key_t *key;
    for (i = 0; i < MLE_MAX_KEY_TABLE_SIZE; i++) {
        key = &sec_ptr->mle_security_key_table[i];

        if (!key->key_valid && key->primary_key == primary) {
            return key;
        }
    }
    return NULL;
}

static mle_security_key_t *mle_service_security_key_entry_get(mle_security_components_t *sec_ptr, bool primary)
{
    uint8_t i;
    mle_security_key_t *key;
    for (i = 0; i < MLE_MAX_KEY_TABLE_SIZE; i++) {
        key = &sec_ptr->mle_security_key_table[i];

        if (key->key_valid && key->primary_key == primary) {
            return key;
        }
    }
    return NULL;
}

void mle_service_security_parameters_init(mle_security_components_t *sec_ptr)
{
    uint8_t i;
    for (i = 0; i < MLE_MAX_KEY_TABLE_SIZE; i++) {
        sec_ptr->mle_security_key_table[i].key_id = 0xff;
        if (i) {
            sec_ptr->mle_security_key_table[i].primary_key = false;
        } else {
            sec_ptr->mle_security_key_table[i].primary_key = true;
        }
        sec_ptr->mle_security_key_table[i].key_valid = false;
        sec_ptr->mle_security_key_table[i].pending_primary = false;
        memset(sec_ptr->mle_security_key_table[i].aes_key, 0, 16);
    }
}

uint32_t mle_service_security_get_framecounter(mle_security_components_t *sec_ptr, bool increment)
{
    if (increment) {
        return ++sec_ptr->security_frame_counter;
    }
    return sec_ptr->security_frame_counter;
}

uint8_t mle_service_security_get_default_key_id(mle_security_components_t *sec_ptr)
{
    mle_security_key_t *default_key = mle_service_security_key_entry_get(sec_ptr, true);
    if (default_key) {
        return default_key->key_id;
    }
    return 0;
}

uint8_t *mle_service_security_get_default_key(mle_security_components_t *sec_ptr)
{
    mle_security_key_t *default_key = mle_service_security_key_entry_get(sec_ptr, true);
    if (default_key) {
        return default_key->aes_key;
    }
    return 0;
}

uint8_t mle_service_security_get_next_key_id(mle_security_components_t *sec_ptr)
{
    mle_security_key_t *default_key = mle_service_security_key_entry_get(sec_ptr, false);
    if (default_key) {
        return default_key->key_id;
    }
    return 0;
}

uint8_t *mle_service_security_get_next_key(mle_security_components_t *sec_ptr)
{
    mle_security_key_t *default_key = mle_service_security_key_entry_get(sec_ptr, false);
    if (default_key) {
        return default_key->aes_key;
    }
    return 0;
}

void mle_service_security_header_buf_init(mle_security_components_t *sec_ptr, mle_security_header_t *securityHeader, bool keySrcValid, uint32_t keySrc)
{
    securityHeader->securityLevel = sec_ptr->sec_level;
    if (securityHeader->securityLevel) {
        securityHeader->frameCounter = mle_service_security_get_framecounter(sec_ptr, true);
        securityHeader->KeyIndex = mle_service_security_get_default_key_id(sec_ptr);
        if (keySrcValid) {
            securityHeader->KeyIdMode = MAC_KEY_ID_MODE_SRC4_IDX;
            common_write_32_bit(keySrc, securityHeader->Keysource);
        } else {
            securityHeader->KeyIdMode = MAC_KEY_ID_MODE_IDX;
        }
    }
}

uint8_t *mle_service_security_get_key(mle_security_header_t *securityHeader, mle_security_components_t *sec_ptr, int8_t interfaceId)
{
    if (securityHeader->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
        if (sec_ptr->key_req) {
            uint32_t keySeq = common_read_32_bit(securityHeader->Keysource);
            return sec_ptr->key_req(interfaceId, securityHeader->KeyIndex, keySeq);
        }

    } else if (securityHeader->KeyIdMode == MAC_KEY_ID_MODE_IDX) {

        mle_security_key_t *key;

        key = mle_service_security_key_get(sec_ptr, securityHeader->KeyIndex);
        if (key) {

            if (key->pending_primary) {
                //Switch Key's
                mle_security_key_t *primary = mle_service_security_key_entry_get(sec_ptr, true);
                if (!primary) {
                    return NULL;
                }
                primary->primary_key = false;
                key->pending_primary = false;
                key->primary_key = true;
                sec_ptr->security_frame_counter = 0;
                //Inform Mac
                if (sec_ptr->security_notify) {
                    //Notify User
                    sec_ptr->security_notify(interfaceId, MLE_SEC_KEY_UPDATE_NOTIFY, securityHeader->KeyIndex);
                }

            }
            return key->aes_key;
        } else {
            if (sec_ptr->security_notify) {
                //Notify User
                return sec_ptr->security_notify(interfaceId, MLE_SEC_UNKNOWN_KEY, securityHeader->KeyIndex);
            }
        }
    }
    return NULL;
}

bool mle_service_security_key_update_trig(uint8_t interface_id, mle_security_components_t *sec_ptr, uint8_t keyId)
{
    mle_security_key_t *key = mle_service_security_key_get(sec_ptr, keyId);
    if (!key) {
        return false;
    }

    if (!key->pending_primary) {
        return false;
    }

    //Switch Key's
    mle_security_key_t *primary = mle_service_security_key_entry_get(sec_ptr, true);
    if (!primary) {
        return false;
    }

    mle_service_reset_frame_counters(interface_id);
    primary->primary_key = false;
    key->pending_primary = false;
    key->primary_key = true;

    return true;
}

bool mle_service_security_key_set(mle_security_components_t *sec_ptr, const uint8_t *key, uint8_t keyId, bool set_primary)
{
    bool key_changed = false;
    if (!sec_ptr) {
        return false;
    }

    mle_security_key_t *key_entry = mle_service_security_key_entry_get(sec_ptr, set_primary);
    if (!key_entry) {
        //Define first key
        key_entry = mle_service_security_key_get_free_entry(sec_ptr, set_primary);
        if (!key_entry) {
            return false;
        }
        key_entry->key_valid = true;
    }

    if (memcmp(key_entry->aes_key, key, 16) != 0) {
        key_changed = true;
    }

    key_entry->key_id = keyId;
    memcpy(key_entry->aes_key, key, 16);
    key_entry->primary_key = set_primary;
    key_entry->pending_primary = !set_primary;

    if (set_primary) {
        //Clean Old Pending if Primary is configured
        key_entry = mle_service_security_key_entry_get(sec_ptr, !set_primary);
        if (key_entry) {
            key_entry->key_valid = false;
        }
    }

    return key_changed;
}


