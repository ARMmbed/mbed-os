/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
 * \file mac_pairwise_key.c
 *
 */
#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_common_defines.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"
#include "MLE/mle.h"
#include "NWK_INTERFACE/Include/protocol.h"

#define TRACE_GROUP "mPKe"

typedef struct mac_pairwise_key_entry {
    uint8_t             eui64[8]; /*!< eui64 which is key source */
    uint8_t             pairwiseKey[16]; /*!< Paiwise key */
    uint32_t            frameCounterRX;
    ns_list_link_t      link; /*!< List link entry */
} mac_pairwise_key_entry_t;

typedef struct mac_pairwise_key_info {
    uint8_t device_descriptor_attribute;
    uint8_t key_decriptor_attribute;
} mac_pairwise_key_info_t;

typedef NS_LIST_HEAD(mac_pairwise_key_entry_t, link) mac_pairwise_key_list_t;

typedef struct mac_pairwise_interface_entry {
    int8_t interfaceId;
    mac_pairwise_key_info_t *mac_pairwise_key_table;
    uint8_t key_table_size;
    uint8_t key_list_size;
    uint8_t network_key_offset;
    ns_list_link_t link; /*!< List link entry */
} mac_pairwise_interface_entry_t;

static NS_LIST_DEFINE(mac_pairwise_info, mac_pairwise_interface_entry_t, link);


static mac_pairwise_interface_entry_t *mac_pairwise_key_list_allocate(uint8_t list_size)
{
    mac_pairwise_interface_entry_t *newEntry = ns_dyn_mem_alloc(sizeof(mac_pairwise_interface_entry_t));

    mac_pairwise_key_info_t *key_table = ns_dyn_mem_alloc(sizeof(mac_pairwise_key_info_t) * list_size);
    if (!newEntry || !key_table) {
        ns_dyn_mem_free(newEntry);
        ns_dyn_mem_free(key_table);
        return NULL;
    }
    memset(newEntry, 0, sizeof(mac_pairwise_interface_entry_t));
    memset(key_table, 0, sizeof(mac_pairwise_key_info_t) * list_size);
    newEntry->mac_pairwise_key_table = key_table;
    newEntry->key_list_size = list_size;
    return newEntry;
}

static bool mac_pairwise_key_deacriptro_attribute_get(mac_pairwise_interface_entry_t *main_list, uint8_t key_attribute) {
    mac_pairwise_key_info_t *key_table = main_list->mac_pairwise_key_table;
    for (uint8_t i = 0; i<main_list->key_table_size; i++) {
        if (key_table->key_decriptor_attribute == key_attribute) {
            return false;
        }
        key_table++;
    }

    return true;
}

mac_pairwise_key_info_t *mac_pairwise_key_info_get(mac_pairwise_interface_entry_t *main_list, uint8_t device_id) {
    mac_pairwise_key_info_t *key_table = main_list->mac_pairwise_key_table;
    for (uint8_t i = 0; i<main_list->key_table_size; i++) {
        if (key_table->device_descriptor_attribute == device_id) {
            return key_table;
        }
        key_table++;
    }

    if (main_list->key_table_size == main_list->key_list_size ) {
        return NULL;
    }


    bool unique_id = false;
    mac_pairwise_key_info_t *new_entry = main_list->mac_pairwise_key_table + main_list->key_table_size;
    new_entry->device_descriptor_attribute = device_id;
    new_entry->key_decriptor_attribute = main_list->network_key_offset;
    //Allocate key id
    while(!unique_id) {
        unique_id = mac_pairwise_key_deacriptro_attribute_get(main_list, new_entry->key_decriptor_attribute);
        if (!unique_id) {
            new_entry->key_decriptor_attribute++;
        }
    }

    main_list->key_table_size++;
    return new_entry;
}

static bool mac_pairwise_key_info_delete(mac_pairwise_interface_entry_t *main_list, uint8_t device_id, uint8_t *key_attribute) {
    bool removed_entry = false;

    mac_pairwise_key_info_t *cur = main_list->mac_pairwise_key_table;
    mac_pairwise_key_info_t *prev = NULL;
    for (uint8_t i = 0; i<main_list->key_table_size; i++) {
        if (removed_entry) {
            *prev = *cur;
        } else {
            if (cur->device_descriptor_attribute == device_id) {
                removed_entry = true;
                *key_attribute = cur->key_decriptor_attribute;
            }
        }
        prev = cur;
        cur++;
    }
    if (removed_entry) {
        main_list->key_table_size--;
    }

    return removed_entry;
}


static mac_pairwise_interface_entry_t *mac_pairwise_key_main_class_discover(int8_t interfaceId)
{

    ns_list_foreach(mac_pairwise_interface_entry_t, cur, &mac_pairwise_info) {
        if (interfaceId == cur->interfaceId) {
            return cur;
        }
    }

    return NULL;
}


static mac_pairwise_interface_entry_t *mac_pairwise_key_main_class(uint8_t key_list_size)
{

    mac_pairwise_interface_entry_t *main_list = mac_pairwise_key_list_allocate(key_list_size);
    if (!main_list) {
        return NULL;
    }
    tr_debug("Set new Key main Class");
    ns_list_add_to_end(&mac_pairwise_info, main_list);

    return main_list;
}


static void mac_pairwise_key_list_free(protocol_interface_info_entry_t *interface, mac_pairwise_interface_entry_t *main_list) {
    //Delete mle entries & Keys
    mle_neigh_table_entry_t *cur_entry;
    mac_pairwise_key_info_t *cur = main_list->mac_pairwise_key_table;
    for (uint8_t i = 0; i< main_list->key_table_size; i++) {
        cur_entry = mle_class_get_by_device_attribute_id(interface->id, cur->device_descriptor_attribute);
        if (cur_entry) {
            mle_class_remove_entry(interface->id, cur_entry);
        }
        mac_helper_security_pairwisekey_set(interface, NULL, NULL, cur->key_decriptor_attribute);
    }
    main_list->key_table_size = 0;
}

int mac_pairwise_key_interface_register(int8_t interface_id, uint8_t mac_supported_key_decription_size, uint8_t network_key_size)
{
    if (!mac_supported_key_decription_size || !network_key_size ) {
        return -2;
    }

    if (mac_supported_key_decription_size < network_key_size) {
        return -2;
    }

    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface) {
        return -1;
    }

    if (mac_pairwise_key_main_class_discover(interface_id)) {
        return -2;
    }

    uint8_t pairwise_key_list_size;
    uint8_t network_key_offset;

    if (mac_supported_key_decription_size == network_key_size) {
        pairwise_key_list_size = 1;
        network_key_offset = 0;
    } else {
        pairwise_key_list_size = (mac_supported_key_decription_size - network_key_size);
        network_key_offset = network_key_size;
    }

    mac_pairwise_interface_entry_t *main_list = mac_pairwise_key_main_class(pairwise_key_list_size);
    if (!main_list) {
        return -1;
    }

    main_list->interfaceId = interface_id;
    main_list->network_key_offset = network_key_offset;

    return 0;

}

int mac_pairwise_key_interface_unregister(int8_t interface_id)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface) {
        return -1;
    }

    mac_pairwise_interface_entry_t *main_list = mac_pairwise_key_main_class_discover(interface_id);
    if (!main_list) {
        return -1;
    }

    mac_pairwise_key_list_free(interface, main_list);

    //Can delete Main Class also
    ns_list_remove(&mac_pairwise_info, main_list);
    ns_dyn_mem_free(main_list->mac_pairwise_key_table);
    ns_dyn_mem_free(main_list);
    return 0;

}



int mac_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, const uint8_t eui64[static 8], const uint8_t key[static 16])
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    bool new_entry_created;

    if (!interface || !interface->mac_api) {
        return -1;
    }

    (void)valid_life_time;
    tr_debug("Pairwise_key addr %s, key %s", trace_array(eui64, 8), trace_array(key, 16));
    mac_pairwise_interface_entry_t *main_list = mac_pairwise_key_main_class_discover(interface_id);
    if (!main_list) {
        return -1;
    }

    //Allocate mle entry
    mle_neigh_table_entry_t *mle_entry = mle_class_get_entry_by_mac64(interface_id, 0, eui64, true, &new_entry_created);
    if (!mle_entry) {
        return -1;
    }
    mle_entry->thread_commission = true;
    mle_entry->short_adr = 0xffff;
    mle_entry->ttl = 20;

    //Allocate key description

    mac_pairwise_key_info_t *key_desc = mac_pairwise_key_info_get(main_list, mle_entry->attribute_index);

    if (!key_desc) {
        mle_class_remove_entry(interface_id, mle_entry);
        return -1;
    }

    //Set device descriptor
    mac_helper_devicetable_set(mle_entry, interface, 0, interface->mac_parameters->mac_default_key_index, new_entry_created);

    //set key descriptor
    if (mac_helper_security_pairwisekey_set(interface, key, eui64, key_desc->key_decriptor_attribute) != 0) {
        main_list->key_table_size--;
        mle_class_remove_entry(interface_id, mle_entry);
        return -1;
    }

    return 0;
}

int mac_pairwise_key_del(int8_t interface_id, const uint8_t eui64[static 8])
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface || !interface->mac_api) {
        return -1;
    }

    mac_pairwise_interface_entry_t *main_list;
    tr_debug("Pairwise_key delete %s", trace_array(eui64, 8));
    main_list = mac_pairwise_key_main_class_discover(interface_id);
    if (!main_list) {
        return -1;
    }
    //Get from mle
    mle_neigh_table_entry_t *mle_entry = mle_class_get_entry_by_mac64(interface_id, 0, eui64, true, NULL);
    if (!mle_entry) {
        return -1;
    }

    //discover by mle entry attribute
    uint8_t key_attribute;

    if (!mac_pairwise_key_info_delete(main_list, mle_entry->attribute_index, &key_attribute)) {
        return -1;
    }

    //kill Entry & overwrite key
    mac_helper_security_pairwisekey_set(interface, NULL, NULL, key_attribute);

    mle_class_remove_entry(interface_id, mle_entry);

    return 0;
}


int mac_pairwise_key_flush_list(int8_t interface_id)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface) {
        return -1;
    }

    tr_debug("Pairwise_key flush");
    mac_pairwise_interface_entry_t *main_list = mac_pairwise_key_main_class_discover(interface_id);
    if (!main_list) {
        return -1;
    }

    mac_pairwise_key_list_free(interface, main_list);

    return 0;
}


