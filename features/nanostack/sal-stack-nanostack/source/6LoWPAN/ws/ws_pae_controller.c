/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "ns_address.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_supp.h"
#include "6LoWPAN/ws/ws_pae_auth.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wspc"

typedef int8_t ws_pae_delete(protocol_interface_info_entry_t *interface_ptr);
typedef void ws_pae_timer(uint16_t ticks);
typedef int8_t ws_pae_br_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);
typedef int8_t ws_pae_br_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64);


typedef struct {
    ns_list_link_t link;                                 /**< Link */
    uint8_t target_eui_64[8];                            /**< EAPOL target */
    uint16_t target_pan_id;                              /**< EAPOL target PAN ID */
    uint8_t br_eui_64[8];                                /**< Border router EUI-64 */
    sec_prot_gtk_keys_t gtks;                            /**< GTKs */
    sec_prot_certs_t certs;                              /**< Certificates */
    protocol_interface_info_entry_t *interface_ptr;      /**< List link entry */
    ws_pae_controller_auth_completed *auth_completed;    /**< Authentication completed callback, continue bootstrap */
    ws_pae_controller_key_insert *key_insert;            /**< Key insert callback */
    ws_pae_delete *pae_delete;                           /**< PAE delete callback */
    ws_pae_timer *pae_timer;                             /**< PAE timer callback */
    ws_pae_br_addr_write *pae_br_addr_write;             /**< PAE Border router EUI-64 write callback */
    ws_pae_br_addr_read *pae_br_addr_read;               /**< PAE Border router EUI-64 read callback */
} pae_controller_t;

static void ws_pae_controller_test_keys_set(sec_prot_gtk_keys_t *gtks);
static pae_controller_t *ws_pae_controller_get(protocol_interface_info_entry_t *interface_ptr);

static NS_LIST_DEFINE(pae_controller_list, pae_controller_t, link);

static void ws_pae_controller_test_keys_set(sec_prot_gtk_keys_t *gtks)
{
    uint8_t gtk[2][GTK_LEN];

    // Test data
    for (int i = 0; i < GTK_LEN; i++) {
        gtk[0][i] = 0xcf - i;
        gtk[1][i] = 0xef - i;
    }

    sec_prot_keys_gtk_set(gtks, 0, gtk[0]);
    sec_prot_keys_gtk_set(gtks, 1, gtk[1]);

    sec_prot_keys_gtkl_set(gtks, 0xFF);

    sec_prot_keys_gtk_insert_index_set(gtks, 0);
}

int8_t ws_pae_controller_authenticate(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_supp_authenticate(controller->interface_ptr, controller->target_pan_id, controller->target_eui_64) == PAE_SUPP_NOT_ENABLED) {
        // Already authenticated
        ws_pae_controller_test_keys_set(&controller->gtks);

        uint8_t index;
        uint8_t *gtk = sec_prot_keys_get_gtk_to_insert(&controller->gtks, &index);

        controller->key_insert(controller->interface_ptr, index, gtk);
        controller->auth_completed(interface_ptr, true);
    }

    ///////////
    // For now fixed since not yet support for EA-IE
    const uint8_t addr[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    if (controller->pae_br_addr_write) {
        controller->pae_br_addr_write(interface_ptr, addr);
    }
    ////////////////

    return 0;
}

int8_t ws_pae_controller_authenticator_start(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port)
{
    (void) local_port;
    (void) remote_port;

    if (!interface_ptr || !remote_addr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    ws_pae_controller_test_keys_set(&controller->gtks);

    uint8_t index;
    uint8_t *gtk = sec_prot_keys_get_gtk_to_insert(&controller->gtks, &index);

    controller->key_insert(controller->interface_ptr, index, gtk);

    if (ws_pae_auth_addresses_set(interface_ptr, local_port, remote_addr, remote_port) < 0) {
        return -1;
    }

    return 0;
}

int8_t ws_pae_controller_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_auth_completed *completed, ws_pae_controller_key_insert *key_insert)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->auth_completed = completed;
    controller->key_insert = key_insert;

    return 0;
}

int8_t ws_pae_controller_set_target(protocol_interface_info_entry_t *interface_ptr, uint16_t target_pan_id, uint8_t *target_eui_64)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->target_pan_id = target_pan_id;
    memcpy(controller->target_eui_64, target_eui_64, 8);

    return 0;
}

int8_t ws_pae_controller_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name)
{
    (void) pan_id;
    (void) network_name;

    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    return ws_pae_supp_nw_info_set(interface_ptr, pan_id, network_name);
}

int8_t ws_pae_controller_nw_key_valid(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    return ws_pae_supp_nw_key_valid(interface_ptr);
}

int8_t ws_pae_controller_init(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    if (ws_pae_controller_get(interface_ptr) != NULL) {
        return 0;
    }

    pae_controller_t *controller = ns_dyn_mem_alloc(sizeof(pae_controller_t));
    if (!controller) {
        return -1;
    }

    memset(controller->target_eui_64, 0, 8);
    memset(controller->br_eui_64, 0, 8);
    controller->interface_ptr = interface_ptr;
    controller->auth_completed = NULL;
    controller->key_insert = NULL;
    controller->pae_delete = NULL;
    controller->pae_timer = NULL;
    controller->pae_br_addr_write = NULL;
    controller->pae_br_addr_read = NULL;

    sec_prot_keys_gtks_init(&controller->gtks);
    sec_prot_certs_init(&controller->certs);

    ns_list_add_to_end(&pae_controller_list, controller);

    return 0;
}

int8_t ws_pae_controller_supp_init(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_supp_init(controller->interface_ptr, &controller->certs) < 0) {
        return -1;
    }

    controller->pae_delete = ws_pae_supp_delete;
    controller->pae_timer = ws_pae_supp_timer;
    controller->pae_br_addr_write = ws_pae_supp_border_router_addr_write;
    controller->pae_br_addr_read = ws_pae_supp_border_router_addr_read;

    ws_pae_supp_cb_register(controller->interface_ptr, controller->auth_completed, controller->key_insert);

    return 0;
}

int8_t ws_pae_controller_auth_init(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_auth_init(controller->interface_ptr, &controller->gtks, &controller->certs) < 0) {
        return -1;
    }

    controller->pae_delete = ws_pae_auth_delete;
    controller->pae_timer = ws_pae_auth_timer;

    return 0;
}
int8_t ws_pae_controller_stop(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    // If PAE has been initialized, deletes it
    if (controller->pae_delete) {
        controller->pae_delete(interface_ptr);
    }

    return 0;
}

int8_t ws_pae_controller_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    ws_pae_controller_stop(interface_ptr);

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    ns_list_remove(&pae_controller_list, controller);

    sec_prot_certs_delete(&controller->certs);

    ns_dyn_mem_free(controller);

    return 0;
}

int8_t ws_pae_controller_certificate_chain_set(const arm_certificate_chain_entry_s *new_chain)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        // Delete previous information
        sec_prot_certs_delete(&entry->certs);

        if (new_chain->cert_chain[0]) {
            cert_chain_entry_t *root_ca_chain = sec_prot_certs_chain_entry_create();
            sec_prot_certs_cert_set(root_ca_chain, 0, (uint8_t *) new_chain->cert_chain[0], new_chain->cert_len[0]);
            sec_prot_certs_chain_list_add(&entry->certs.trusted_cert_chain_list, root_ca_chain);
        }

        if (new_chain->cert_chain[1] && new_chain->key_chain[1]) {
            sec_prot_certs_cert_set(&entry->certs.own_cert_chain, 0, (uint8_t *) new_chain->cert_chain[1], new_chain->cert_len[1]);
            uint8_t key_len = strlen((char *) new_chain->key_chain[1]) + 1;
            sec_prot_certs_priv_key_set(&entry->certs.own_cert_chain, (uint8_t *) new_chain->key_chain[1], key_len);
        }
    }

    return 0;
}

int8_t ws_pae_controller_border_router_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64)
{
    if (!interface_ptr || !eui_64) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (controller->pae_br_addr_write) {
        return controller->pae_br_addr_write(interface_ptr, eui_64);
    } else {
        memcpy(controller->br_eui_64, eui_64, 8);
    }

    return 0;

}

int8_t ws_pae_controller_border_router_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64)
{
    if (!interface_ptr || !eui_64) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (controller->pae_br_addr_read) {
        return controller->pae_br_addr_read(interface_ptr, eui_64);
    } else {
        memcpy(eui_64, controller->br_eui_64, 8);
    }

    return 0;
}

void ws_pae_controller_timer(uint16_t ticks)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        if (entry->pae_timer) {
            entry->pae_timer(ticks);
        }
    }
}

static pae_controller_t *ws_pae_controller_get(protocol_interface_info_entry_t *interface_ptr)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        if (entry->interface_ptr == interface_ptr) {
            return entry;
        }
    }

    return NULL;
}

#endif /* HAVE_WS */

