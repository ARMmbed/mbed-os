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
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "6LoWPAN/ws/ws_pae_supp.h"
#include "6LoWPAN/ws/ws_pae_auth.h"
#include "mbedtls/sha256.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wspc"

typedef int8_t ws_pae_delete(protocol_interface_info_entry_t *interface_ptr);
typedef void ws_pae_timer(uint16_t ticks);
typedef int8_t ws_pae_br_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);
typedef int8_t ws_pae_br_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64);
typedef void ws_pae_gtks_updated(protocol_interface_info_entry_t *interface_ptr);
typedef int8_t ws_pae_gtk_hash_update(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash);
typedef int8_t ws_pae_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

typedef struct {
    uint8_t hash[8];                                                 /**< GTK hash for the key */
    bool installed : 1;                                              /**< Key has been installed on MAC */
    bool fresh : 1;                                                  /**< Key is fresh i.e. not used on sending */
} nw_key_t;

typedef struct {
    ns_list_link_t link;                                             /**< Link */
    uint8_t target_eui_64[8];                                        /**< EAPOL target */
    uint16_t target_pan_id;                                          /**< EAPOL target PAN ID */
    uint8_t br_eui_64[8];                                            /**< Border router EUI-64 */
    sec_prot_gtk_keys_t gtks;                                        /**< GTKs */
    sec_prot_gtk_keys_t next_gtks;                                   /**< Next GTKs */
    int8_t gtk_index;                                                /**< GTK index */
    uint8_t gtkhash[32];                                             /**< GTK hashes */
    sec_prot_certs_t certs;                                          /**< Certificates */
    nw_key_t nw_key[4];                                              /**< Currently active network keys (on MAC) */
    char *network_name;                                              /**< Network name for GAK generation */
    timer_settings_t timer_settings;                                 /**< Timer settings */
    protocol_interface_info_entry_t *interface_ptr;                  /**< List link entry */
    ws_pae_controller_auth_completed *auth_completed;                /**< Authentication completed callback, continue bootstrap */
    ws_pae_controller_nw_key_set *nw_key_set;                        /**< Key set callback */
    ws_pae_controller_nw_key_clear *nw_key_clear;                    /**< Key clear callback */
    ws_pae_controller_nw_send_key_index_set *nw_send_key_index_set;  /**< Send key index set callback */
    ws_pae_controller_nw_frame_counter_set *nw_frame_counter_set;    /**< Frame counter set callback */
    ws_pae_controller_pan_ver_increment *pan_ver_increment;          /**< PAN version increment callback */
    ws_pae_delete *pae_delete;                                       /**< PAE delete callback */
    ws_pae_timer *pae_fast_timer;                                    /**< PAE fast timer callback */
    ws_pae_timer *pae_slow_timer;                                    /**< PAE slow timer callback */
    ws_pae_br_addr_write *pae_br_addr_write;                         /**< PAE Border router EUI-64 write callback */
    ws_pae_br_addr_read *pae_br_addr_read;                           /**< PAE Border router EUI-64 read callback */
    ws_pae_gtks_updated *pae_gtks_updated;                           /**< PAE GTKs updated */
    ws_pae_gtk_hash_update *pae_gtk_hash_update;                     /**< PAE GTK HASH update */
    ws_pae_nw_key_index_update *pae_nw_key_index_update;             /**< PAE NW key index update */
    bool gtks_set : 1;                                               /**< GTKs are set */
    bool gtkhash_set : 1;                                            /**< GTK hashes are set */
    bool key_index_set : 1;                                          /**< NW key index is set */
} pae_controller_t;

static pae_controller_t *ws_pae_controller_get(protocol_interface_info_entry_t *interface_ptr);
static pae_controller_t *ws_pae_controller_get_or_create(int8_t interface_id);
static void ws_pae_controller_gtk_hash_set(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash);
static int8_t ws_pae_controller_nw_key_check_and_insert(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks);
static void ws_pae_controller_active_nw_key_clear(nw_key_t *nw_key);
static void ws_pae_controller_active_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t index);
static int8_t ws_pae_controller_gak_from_gtk(uint8_t *gak, uint8_t *gtk, char *network_name);
static void ws_pae_controller_nw_key_index_check_and_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index);
static void ws_pae_controller_data_init(pae_controller_t *controller);

static NS_LIST_DEFINE(pae_controller_list, pae_controller_t, link);

#if !defined(HAVE_PAE_SUPP) && !defined(HAVE_PAE_AUTH)

static void ws_pae_controller_test_keys_set(sec_prot_gtk_keys_t *gtks)
{
    uint8_t gtk[GTK_LEN];

    // Test data
    for (int i = 0; i < GTK_LEN; i++) {
        gtk[i] = 0xcf - i;
    }

    sec_prot_keys_gtk_set(gtks, 0, gtk, GTK_DEFAULT_LIFETIME);
}

#else

#define ws_pae_controller_test_keys_set(gtks);

#endif

int8_t ws_pae_controller_authenticate(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

#ifdef HAVE_PAE_SUPP
    // In case test keys are set uses those and does not initiate authentication
    if (controller->gtks_set) {
        if (sec_prot_keys_gtks_are_updated(&controller->gtks)) {
            ws_pae_controller_nw_key_check_and_insert(controller->interface_ptr, &controller->gtks);
            sec_prot_keys_gtks_updated_reset(&controller->gtks);
        }
        controller->auth_completed(interface_ptr, true);
        return 0;
    }

    if (ws_pae_supp_authenticate(controller->interface_ptr, controller->target_pan_id, controller->target_eui_64) < 0) {
        controller->auth_completed(interface_ptr, false);
    }

#else
    ws_pae_controller_test_keys_set(&controller->gtks);
    ws_pae_controller_nw_key_check_and_insert(interface_ptr, &controller->gtks);
    ws_pae_controller_nw_key_index_check_and_set(interface_ptr, 0);

    controller->auth_completed(interface_ptr, true);
#endif

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

#ifdef HAVE_PAE_AUTH
    if (sec_prot_keys_gtks_are_updated(&controller->gtks)) {
        ws_pae_auth_gtks_updated(interface_ptr);
        if (controller->gtk_index >= 0) {
            controller->pae_nw_key_index_update(interface_ptr, controller->gtk_index);
        }
        sec_prot_keys_gtks_updated_reset(&controller->gtks);
    }
#else
    ws_pae_controller_test_keys_set(&controller->gtks);
    ws_pae_controller_nw_key_check_and_insert(interface_ptr, &controller->gtks);
    ws_pae_controller_nw_key_index_check_and_set(interface_ptr, 0);
#endif

    if (ws_pae_auth_addresses_set(interface_ptr, local_port, remote_addr, remote_port) < 0) {
        return -1;
    }

    ws_pae_auth_cb_register(interface_ptr, ws_pae_controller_gtk_hash_set, ws_pae_controller_nw_key_check_and_insert, ws_pae_controller_nw_key_index_check_and_set);

    ws_pae_auth_start(interface_ptr);

    return 0;
}

int8_t ws_pae_controller_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_auth_completed *completed, ws_pae_controller_nw_key_set *nw_key_set, ws_pae_controller_nw_key_clear *nw_key_clear, ws_pae_controller_nw_send_key_index_set *nw_send_key_index_set, ws_pae_controller_nw_frame_counter_set *nw_frame_counter_set, ws_pae_controller_pan_ver_increment *pan_ver_increment)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->auth_completed = completed;
    controller->nw_key_set = nw_key_set;
    controller->nw_key_clear = nw_key_clear;
    controller->nw_send_key_index_set = nw_send_key_index_set;
    controller->nw_frame_counter_set = nw_frame_counter_set;
    controller->pan_ver_increment = pan_ver_increment;

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

    controller->network_name = network_name;

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

static int8_t ws_pae_controller_nw_key_check_and_insert(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks)
{
    int8_t ret = -1;

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return ret;
    }

    uint8_t gtkhash[GTK_ALL_HASHES_LEN];
    sec_prot_keys_gtks_hash_generate(gtks, gtkhash);

    nw_key_t *nw_key = controller->nw_key;

    // Delete old keys
    uint8_t *gtk_hash_ptr = gtkhash;
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        // If hash is not set for a key
        if (sec_prot_keys_gtk_hash_empty(gtk_hash_ptr)) {
            // Deletes the key if it is set
            if (!sec_prot_keys_gtk_hash_empty(nw_key[i].hash)) {
                tr_info("NW key remove: %i", i);
                controller->nw_key_clear(interface_ptr, i);
                nw_key[i].installed = false;
            }
        }
        gtk_hash_ptr += GTK_HASH_LEN;
    }

    // Insert new keys
    gtk_hash_ptr = gtkhash;
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        // If hash is set for a key
        if (!sec_prot_keys_gtk_hash_empty(gtk_hash_ptr)) {
            int hash_matches = memcmp(gtk_hash_ptr, nw_key[i].hash, GTK_HASH_LEN);
            // If the hash does not match (not set or modified) or not installed
            if (hash_matches != 0 || !nw_key[i].installed) {

                memcpy(nw_key[i].hash, gtk_hash_ptr, GTK_HASH_LEN);

                nw_key[i].installed = true;
                if (hash_matches != 0) {
                    nw_key[i].fresh = true;
                }

                uint8_t *gtk = sec_prot_keys_gtk_get(gtks, i);
                tr_info("NW key set: %i, hash: %s", i, trace_array(nw_key[i].hash, 8));
                uint8_t gak[GTK_LEN];
                if (ws_pae_controller_gak_from_gtk(gak, gtk, controller->network_name) >= 0) {
                    controller->nw_key_set(interface_ptr, i, i, gak);
                    ret = 0;
                } else {
                    tr_error("GAK generation failed network name: %s", controller->network_name);
                    ret = -1;
                }
            }
        }

        gtk_hash_ptr += GTK_HASH_LEN;
    }

    return ret;
}

static void ws_pae_controller_active_nw_key_clear(nw_key_t *nw_key)
{
    memset(nw_key, 0, sizeof(nw_key_t));
    nw_key->installed = false;
    nw_key->fresh = false;
}

static int8_t ws_pae_controller_gak_from_gtk(uint8_t *gak, uint8_t *gtk, char *network_name)
{
#if defined(HAVE_PAE_SUPP) || defined(HAVE_PAE_AUTH)
    uint8_t network_name_len = strlen(network_name);
    if (network_name_len == 0) {
        return -1;
    }

    uint8_t input[network_name_len + GTK_LEN];
    memcpy(input, network_name, network_name_len);
    memcpy(input + network_name_len, gtk, GTK_LEN);

    int8_t ret_val = 0;

    mbedtls_sha256_context ctx;

    mbedtls_sha256_init(&ctx);

    if (mbedtls_sha256_starts_ret(&ctx, 0) != 0) {
        ret_val = -1;
        goto error;
    }

    if (mbedtls_sha256_update_ret(&ctx, input, network_name_len + GTK_LEN) != 0) {
        ret_val = -1;
        goto error;
    }

    uint8_t output[32];

    if (mbedtls_sha256_finish_ret(&ctx, output) != 0) {
        ret_val = -1;
        goto error;
    }

    memcpy(gak, &output[0], 16);

error:
    mbedtls_sha256_free(&ctx);

    return ret_val;
#else
    (void) network_name;
    memcpy(gak, gtk, 16);
    return 0;
#endif
}

int8_t ws_pae_controller_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (controller->pae_nw_key_index_update) {
        controller->pae_nw_key_index_update(interface_ptr, index);
    }

    return 0;
}

void ws_pae_controller_nw_keys_remove(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return;
    }

    tr_info("NW keys remove");

    nw_key_t *nw_key = controller->nw_key;
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        // Deletes the key if it is set
        if (!sec_prot_keys_gtk_hash_empty(nw_key[i].hash)) {
            tr_info("NW key remove: %i", i);
            controller->nw_key_clear(interface_ptr, i);
            nw_key[i].installed = false;
        }
    }
}

static void ws_pae_controller_nw_key_index_check_and_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return;
    }

    if (controller->nw_send_key_index_set) {
        tr_info("NW send key index set: %i", index + 1);
        controller->nw_send_key_index_set(interface_ptr, index);
        controller->nw_frame_counter_set(interface_ptr, 0);
    }

    // Do not update PAN version for initial key index set
    if (controller->key_index_set) {
        if (controller->pan_ver_increment) {
            controller->pan_ver_increment(interface_ptr);
        }
    } else {
        controller->key_index_set = true;
    }
}

static void ws_pae_controller_active_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t index)
{
    pae_controller_t *controller = ws_pae_controller_get(cur);
    if (!controller) {
        return;
    }

    if (controller->nw_send_key_index_set) {
        controller->nw_send_key_index_set(controller->interface_ptr, index);

        // If index has changed and the key for the index is fresh reset frame counter
        if (controller->gtk_index != index && controller->nw_key[index].fresh) {
            controller->nw_frame_counter_set(cur, 0);
        }

        controller->gtk_index = index;
        controller->nw_key[index].fresh = false;
    }
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

    controller->interface_ptr = interface_ptr;
    controller->auth_completed = NULL;
    controller->nw_key_set = NULL;
    controller->nw_key_clear = NULL;
    controller->nw_send_key_index_set = NULL;
    controller->nw_frame_counter_set = NULL;
    controller->pan_ver_increment = NULL;

    ws_pae_controller_data_init(controller);

    ns_list_add_to_end(&pae_controller_list, controller);

    return 0;
}

static void ws_pae_controller_data_init(pae_controller_t *controller)
{
    memset(controller->target_eui_64, 0, 8);
    memset(controller->br_eui_64, 0, 8);
    memset(controller->gtkhash, 0, 32);

    ws_pae_controller_active_nw_key_clear(&controller->nw_key[0]);
    ws_pae_controller_active_nw_key_clear(&controller->nw_key[1]);
    ws_pae_controller_active_nw_key_clear(&controller->nw_key[2]);
    ws_pae_controller_active_nw_key_clear(&controller->nw_key[3]);

    controller->target_pan_id = 0xffff;
    controller->pae_delete = NULL;
    controller->pae_fast_timer = NULL;
    controller->pae_slow_timer = NULL;
    controller->pae_br_addr_write = NULL;
    controller->pae_br_addr_read = NULL;
    controller->pae_gtks_updated = NULL;
    controller->pae_gtk_hash_update = NULL;
    controller->pae_nw_key_index_update = NULL;
    controller->gtks_set = false;
    controller->gtkhash_set = false;
    controller->key_index_set = false;
    controller->gtk_index = -1;
    controller->network_name = NULL;
    sec_prot_keys_gtks_init(&controller->gtks);
    sec_prot_keys_gtks_init(&controller->next_gtks);
    sec_prot_certs_init(&controller->certs);
    ws_pae_timers_settings_init(&controller->timer_settings);
}

int8_t ws_pae_controller_supp_init(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_supp_init(controller->interface_ptr, &controller->certs, &controller->timer_settings) < 0) {
        return -1;
    }

    controller->pae_delete = ws_pae_supp_delete;
    controller->pae_fast_timer = ws_pae_supp_fast_timer;
    controller->pae_slow_timer = ws_pae_supp_slow_timer;
    controller->pae_br_addr_write = ws_pae_supp_border_router_addr_write;
    controller->pae_br_addr_read = ws_pae_supp_border_router_addr_read;
    controller->pae_gtk_hash_update = ws_pae_supp_gtk_hash_update;
    controller->pae_nw_key_index_update = ws_pae_supp_nw_key_index_update;

    ws_pae_supp_cb_register(controller->interface_ptr, controller->auth_completed, ws_pae_controller_nw_key_check_and_insert, ws_pae_controller_active_nw_key_set);

    return 0;
}

int8_t ws_pae_controller_auth_init(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_auth_init(controller->interface_ptr, &controller->gtks, &controller->next_gtks, &controller->certs, &controller->timer_settings) < 0) {
        return -1;
    }

    controller->pae_delete = ws_pae_auth_delete;
    controller->pae_fast_timer = ws_pae_auth_fast_timer;
    controller->pae_slow_timer = ws_pae_auth_slow_timer;
    controller->pae_gtks_updated = ws_pae_auth_gtks_updated;
    controller->pae_nw_key_index_update = ws_pae_auth_nw_key_index_update;

    return 0;
}
int8_t ws_pae_controller_stop(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    // If PAE has been initialized, deletes it
    if (controller->pae_delete) {
        controller->pae_delete(interface_ptr);
    }

    // Free data
    sec_prot_certs_delete(&controller->certs);

    // Init controller data
    ws_pae_controller_data_init(controller);

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
    ns_dyn_mem_free(controller);

    return 0;
}

int8_t ws_pae_controller_certificate_chain_set(const arm_certificate_chain_entry_s *new_chain)
{
    if (!new_chain) {
        return -1;
    }

    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        // Delete previous information
        sec_prot_certs_delete(&entry->certs);

        // Adds a trusted certificate from index 0
        if (new_chain->cert_chain[0]) {
            cert_chain_entry_t *root_ca_chain = sec_prot_certs_chain_entry_create();
            sec_prot_certs_cert_set(root_ca_chain, 0, (uint8_t *) new_chain->cert_chain[0], new_chain->cert_len[0]);
            sec_prot_certs_chain_list_add(&entry->certs.trusted_cert_chain_list, root_ca_chain);
        }

        // Adds own certificate chain from indexes 1 to 3
        for (uint8_t i = 1; i < SEC_PROT_CERT_CHAIN_DEPTH; i++) {
            if (new_chain->cert_chain[i]) {
                sec_prot_certs_cert_set(&entry->certs.own_cert_chain, i - 1, (uint8_t *) new_chain->cert_chain[i], new_chain->cert_len[i]);
                if (new_chain->key_chain[i]) {
                    // Will be the key from top certificate in chain after all certificates are added
                    uint8_t key_len = strlen((char *) new_chain->key_chain[i]) + 1;
                    sec_prot_certs_priv_key_set(&entry->certs.own_cert_chain, (uint8_t *) new_chain->key_chain[i], key_len);
                }
            }
        }
    }

    return 0;
}

int8_t ws_pae_controller_trusted_certificate_add(const arm_certificate_entry_s *cert)
{
    if (!cert) {
        return -1;
    }

    int8_t ret = -1;

    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        cert_chain_entry_t *trusted_cert = sec_prot_certs_chain_entry_create();
        sec_prot_certs_cert_set(trusted_cert, 0, (uint8_t *) cert->cert, cert->cert_len);

        if (sec_prot_certs_chain_list_entry_find(&entry->certs.trusted_cert_chain_list, trusted_cert)) {
            sec_prot_certs_chain_entry_delete(trusted_cert);
            continue;
        }
        sec_prot_certs_chain_list_add(&entry->certs.trusted_cert_chain_list, trusted_cert);
        ret = 0;
    }

    return ret;
}

int8_t ws_pae_controller_trusted_certificate_remove(const arm_certificate_entry_s *cert)
{
    if (!cert) {
        return -1;
    }

    int8_t ret = -1;

    cert_chain_entry_t *trusted_cert = sec_prot_certs_chain_entry_create();
    sec_prot_certs_cert_set(trusted_cert, 0, (uint8_t *) cert->cert, cert->cert_len);

    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        cert_chain_entry_t *removed_cert = sec_prot_certs_chain_list_entry_find(&entry->certs.trusted_cert_chain_list, trusted_cert);
        if (removed_cert) {
            sec_prot_certs_chain_list_entry_delete(&entry->certs.trusted_cert_chain_list, removed_cert);
            ret = 0;
        }
    }

    sec_prot_certs_chain_entry_delete(trusted_cert);

    return ret;
}

int8_t ws_pae_controller_certificate_revocation_list_add(const arm_cert_revocation_list_entry_s *crl)
{
    if (!crl) {
        return -1;
    }

    int8_t ret = -1;

    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        cert_revocat_list_entry_t *cert_revoc_list = sec_prot_certs_revocat_list_entry_create();
        sec_prot_certs_revocat_list_set(cert_revoc_list, crl->crl, crl->crl_len);

        if (sec_prot_certs_revocat_lists_entry_find(&entry->certs.cert_revocat_lists, cert_revoc_list)) {
            sec_prot_certs_revocat_list_entry_delete(cert_revoc_list);
            continue;
        }

        sec_prot_certs_revocat_lists_add(&entry->certs.cert_revocat_lists, cert_revoc_list);
        ret = 0;
    }

    return ret;
}

int8_t ws_pae_controller_certificate_revocation_list_remove(const arm_cert_revocation_list_entry_s *crl)
{
    if (!crl) {
        return -1;
    }

    int8_t ret = -1;

    cert_revocat_list_entry_t *cert_revoc_list = sec_prot_certs_revocat_list_entry_create();
    sec_prot_certs_revocat_list_set(cert_revoc_list, crl->crl, crl->crl_len);

    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        cert_revocat_list_entry_t *removed_cert_revoc_list = sec_prot_certs_revocat_lists_entry_find(&entry->certs.cert_revocat_lists, cert_revoc_list);
        if (removed_cert_revoc_list) {
            sec_prot_certs_revocat_lists_entry_delete(&entry->certs.cert_revocat_lists, removed_cert_revoc_list);
            ret = 0;
        }
    }

    sec_prot_certs_revocat_list_entry_delete(cert_revoc_list);

    return ret;
}

int8_t ws_pae_controller_border_router_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64)
{
    if (!eui_64) {
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
    if (!eui_64) {
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

int8_t ws_pae_controller_gtk_update(int8_t interface_id, uint8_t *gtk[4])
{
    if (!gtk) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    // Removes keys set as not used
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (!gtk[i]) {
            sec_prot_keys_gtk_clear(&controller->gtks, i);
        }
    }

    // Inserts new keys
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (gtk[i]) {
            uint32_t lifetime = sec_prot_keys_gtk_install_order_last_lifetime_get(&controller->gtks);
            lifetime += controller->timer_settings.gtk_expire_offset;
            if (sec_prot_keys_gtk_set(&controller->gtks, i, gtk[i], lifetime) >= 0) {
                tr_info("GTK set index: %i, lifetime %"PRIu32", system time: %"PRIu32"", i, lifetime, protocol_core_monotonic_time / 10);
            }
        }
    }

    // Notifies PAE authenticator that GTKs have been updated */
    if (controller->pae_gtks_updated) {
        controller->pae_gtks_updated(controller->interface_ptr);
    }

    return 0;
}

int8_t ws_pae_controller_next_gtk_update(int8_t interface_id, uint8_t *gtk[4])
{
    if (!gtk) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    // Inserts new keys and removed keys set as not used
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (gtk[i]) {
            sec_prot_keys_gtk_set(&controller->next_gtks, i, gtk[i], 0);
        } else {
            sec_prot_keys_gtk_clear(&controller->next_gtks, i);
        }
    }

    return 0;
}

int8_t ws_pae_controller_active_key_update(int8_t interface_id, uint8_t index)
{
    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    controller->gtk_index = index;

    if (controller->pae_nw_key_index_update) {
        controller->pae_nw_key_index_update(controller->interface_ptr, index);
    }

    return 0;
}

int8_t ws_pae_controller_key_lifetime_update(int8_t interface_id, uint32_t gtk_lifetime, uint32_t pmk_lifetime, uint32_t ptk_lifetime)
{
    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    ws_pae_timers_lifetime_set(&controller->timer_settings, gtk_lifetime, pmk_lifetime, ptk_lifetime);

    return 0;
}

int8_t ws_pae_controller_gtk_time_settings_update(int8_t interface_id, uint8_t revocat_lifetime_reduct, uint8_t new_activation_time, uint8_t new_install_req, uint32_t max_mismatch)
{
    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    ws_pae_timers_gtk_time_settings_set(&controller->timer_settings, revocat_lifetime_reduct, new_activation_time, new_install_req, max_mismatch);

    return 0;
}

int8_t ws_pae_controller_node_keys_remove(int8_t interface_id, uint8_t *eui_64)
{
#ifndef HAVE_PAE_AUTH
    (void) eui_64;
#endif

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    return ws_pae_auth_node_keys_remove(controller->interface_ptr, eui_64);
}

int8_t ws_pae_controller_node_access_revoke_start(int8_t interface_id)
{
    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    ws_pae_auth_node_access_revoke_start(controller->interface_ptr);

    return -1;
}

static void ws_pae_controller_gtk_hash_set(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return;
    }

    memcpy(controller->gtkhash, gtkhash, 32);

    tr_info("GTK hash set %s %s %s %s",
            trace_array(&gtkhash[0], 8),
            trace_array(&gtkhash[8], 8),
            trace_array(&gtkhash[16], 8),
            trace_array(&gtkhash[24], 8));

    // Do not update PAN version for initial hash set
    if (controller->gtkhash_set) {
        if (controller->pan_ver_increment) {
            controller->pan_ver_increment(interface_ptr);
        }
    } else {
        controller->gtkhash_set = true;
    }
}

uint8_t *ws_pae_controller_gtk_hash_ptr_get(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return NULL;
    }

    return controller->gtkhash;
}

int8_t ws_pae_controller_gtk_hash_update(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    memcpy(controller->gtkhash, gtkhash, 32);

    if (controller->pae_gtk_hash_update) {
        return controller->pae_gtk_hash_update(interface_ptr, gtkhash);
    }

    return 0;
}

void ws_pae_controller_fast_timer(uint16_t ticks)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        if (entry->pae_fast_timer) {
            entry->pae_fast_timer(ticks);
        }
    }
}

void ws_pae_controller_slow_timer(uint16_t seconds)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        if (entry->pae_slow_timer) {
            entry->pae_slow_timer(seconds);
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

static pae_controller_t *ws_pae_controller_get_or_create(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return NULL;
    }

    pae_controller_t *controller = ws_pae_controller_get(cur);

    if (!controller) {
        if (ws_pae_controller_init(cur) < 0) {
            return NULL;
        }
        controller = ws_pae_controller_get(cur);
    }

    return controller;
}

#endif /* HAVE_WS */

