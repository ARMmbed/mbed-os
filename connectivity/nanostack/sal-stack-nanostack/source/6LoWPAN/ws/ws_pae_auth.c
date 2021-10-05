/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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
#include <randLIB.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "ws_management_api.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "ns_address.h"
#include "Service_Libs/utils/ns_file.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_socket_if.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/key_sec_prot/key_sec_prot.h"
#include "Security/protocols/eap_tls_sec_prot/auth_eap_tls_sec_prot.h"
#include "Security/protocols/eap_tls_sec_prot/radius_eap_tls_sec_prot.h"
#include "Security/protocols/tls_sec_prot/tls_sec_prot.h"
#include "Security/protocols/fwh_sec_prot/auth_fwh_sec_prot.h"
#include "Security/protocols/gkh_sec_prot/auth_gkh_sec_prot.h"
#include "Security/protocols/radius_sec_prot/radius_client_sec_prot.h"
#include "Security/protocols/msg_sec_prot/msg_sec_prot.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "6LoWPAN/ws/ws_pae_auth.h"
#include "6LoWPAN/ws/ws_pae_lib.h"
#include "6LoWPAN/ws/ws_pae_time.h"
#include "6LoWPAN/ws/ws_pae_key_storage.h"

#ifdef HAVE_WS
#ifdef HAVE_PAE_AUTH

#define TRACE_GROUP "wspa"

#define PAE_TASKLET_INIT                       1
#define PAE_TASKLET_EVENT                      2
#define PAE_TASKLET_TIMER                      3

/* Wait for supplicant to indicate activity (e.g. to send a message) when
   authentication is ongoing */
#define WAIT_FOR_AUTHENTICATION_TICKS          2 * 60 * 10  // 2 minutes
// Wait after authentication has completed before supplicant entry goes inactive
#define WAIT_AFTER_AUTHENTICATION_TICKS        15 * 10      // 15 seconds
// Waiting for authentication supplicant list maximum size
#define WAITING_SUPPLICANT_LIST_MAX_SIZE       50

// Default for maximum number of supplicants
#define SUPPLICANT_MAX_NUMBER                  5000

/* Default for number of supplicants to purge per garbage collect call from
   nanostack monitor */
#define SUPPLICANT_NUMBER_TO_PURGE             5

// Short GTK lifetime value, for GTK install check
#define SHORT_GTK_LIFETIME                     10 * 3600  // 10 hours

// Frame counter exhaust check timer
#define FRAME_CNT_TIMER                        3600

#define SECONDS_IN_DAY                         (3600 * 24)
#define TIME_MINIMUM_DIFFERENCE                5
#define TIME_DIFFERENCE_THRESHOLD              3600

typedef struct {
    ns_list_link_t link;                                     /**< Link */
    uint16_t pan_id;                                         /**< PAN ID */
    char network_name[33];                                   /**< Network name */
    kmp_service_t *kmp_service;                              /**< KMP service */
    protocol_interface_info_entry_t *interface_ptr;          /**< Interface pointer */
    ws_pae_auth_gtk_hash_set *hash_set;                      /**< GTK hash set callback */
    ws_pae_auth_nw_key_insert *nw_key_insert;                /**< Key insert callback */
    ws_pae_auth_nw_keys_remove *nw_keys_remove;              /**< Network keys remove callback */
    ws_pae_auth_nw_key_index_set *nw_key_index_set;          /**< Key index set callback */
    ws_pae_auth_nw_info_updated *nw_info_updated;            /**< Security keys network info updated callback */
    ws_pae_auth_ip_addr_get *ip_addr_get;                    /**< IP address get callback */
    ws_pae_auth_congestion_get *congestion_get;              /**< Congestion get callback */
    ws_pae_auth_nw_frame_counter_read *nw_frame_cnt_read;    /**< Network frame counter read callback */
    supp_list_t active_supp_list;                            /**< List of active supplicants */
    supp_list_t waiting_supp_list;                           /**< List of waiting supplicants */
    shared_comp_list_t shared_comp_list;                     /**< Shared component list */
    arm_event_storage_t *timer;                              /**< Timer */
    sec_prot_gtk_keys_t *next_gtks;                          /**< Next GTKs */
    const sec_prot_certs_t *certs;                           /**< Certificates */
    sec_prot_keys_nw_info_t *sec_keys_nw_info;               /**< Security keys network information */
    sec_cfg_t *sec_cfg;                                      /**< Security configuration */
    frame_counters_t *frame_counters;                        /**< Frame counters */
    uint64_t prev_system_time;                               /**< Previous system time */
    uint64_t system_time_diff;                               /**< System time diffence */
    uint32_t prev_frame_cnt;                                 /**< Previous frame counter */
    uint16_t prev_frame_cnt_timer;                           /**< Previous frame counter timer */
    uint16_t supp_max_number;                                /**< Max number of stored supplicants */
    uint16_t waiting_supp_list_size;                         /**< Waiting supplicants list size */
    uint8_t relay_socked_msg_if_instance_id;                 /**< Relay socket message interface instance identifier */
    uint8_t radius_socked_msg_if_instance_id;                /**< Radius socket message interface instance identifier */
    bool timer_running : 1;                                  /**< Timer is running */
    bool gtk_new_inst_req_exp : 1;                           /**< GTK new install required timer expired */
    bool gtk_new_act_time_exp : 1;                           /**< GTK new activation time expired */
    bool prev_system_time_set : 1;                           /**< Previous system time set */
    bool prev_frame_cnt_set : 1;                             /**< Previous frame counter set */
} pae_auth_t;

static int8_t ws_pae_auth_network_keys_from_gtks_set(pae_auth_t *pae_auth, bool force_install);
static int8_t ws_pae_auth_active_gtk_set(pae_auth_t *pae_auth, uint8_t index);
static int8_t ws_pae_auth_network_key_index_set(pae_auth_t *pae_auth, uint8_t index);
static void ws_pae_auth_free(pae_auth_t *pae_auth);
static pae_auth_t *ws_pae_auth_get(protocol_interface_info_entry_t *interface_ptr);
static pae_auth_t *ws_pae_auth_by_kmp_service_get(kmp_service_t *service);
static int8_t ws_pae_auth_event_send(kmp_service_t *service, void *data);
static void ws_pae_auth_tasklet_handler(arm_event_s *event);
static uint32_t ws_pae_auth_lifetime_key_frame_cnt_check(pae_auth_t *pae_auth, uint8_t gtk_index, uint16_t seconds);
static uint32_t ws_pae_auth_lifetime_system_time_check(pae_auth_t *pae_auth, int8_t gtk_index, uint16_t seconds, uint32_t dec_extra_seconds);
static void ws_pae_auth_gtk_key_insert(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_new_gtk_activate(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_timer_if_start(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_auth_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_auth_timer_start(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_timer_stop(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_shared_comp_add(kmp_service_t *service, kmp_shared_comp_t *data);
static int8_t ws_pae_auth_shared_comp_remove(kmp_service_t *service, kmp_shared_comp_t *data);
static bool ws_pae_auth_timer_running(pae_auth_t *pae_auth);
static void ws_pae_auth_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr);
static void ws_pae_auth_kmp_service_ip_addr_get(kmp_service_t *service, kmp_api_t *kmp, uint8_t *address);
static kmp_api_t *ws_pae_auth_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type);
static bool ws_pae_auth_active_limit_reached(uint16_t active_supp, pae_auth_t *pae_auth);
static kmp_api_t *ws_pae_auth_kmp_incoming_ind(kmp_service_t *service, uint8_t msg_if_instance_id, kmp_type_e type, const kmp_addr_t *addr, const void *pdu, uint16_t size);
static void ws_pae_auth_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result);
static void ws_pae_auth_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr);
static bool ws_pae_auth_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys);
static bool ws_pae_auth_next_kmp_trigger(pae_auth_t *pae_auth, supp_entry_t *supp_entry);
static kmp_type_e ws_pae_auth_next_protocol_get(pae_auth_t *pae_auth, supp_entry_t *supp_entry);
static kmp_api_t *ws_pae_auth_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, uint8_t socked_msg_if_instance_id, supp_entry_t *supp_entry, sec_cfg_t *sec_cfg);
static void ws_pae_auth_kmp_api_finished(kmp_api_t *kmp);
static void ws_pae_auth_active_supp_deleted(void *pae_auth);
static void ws_pae_auth_waiting_supp_deleted(void *pae_auth);

static int8_t tasklet_id = -1;
static NS_LIST_DEFINE(pae_auth_list, pae_auth_t, link);

int8_t ws_pae_auth_init(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *next_gtks, const sec_prot_certs_t *certs, sec_cfg_t *sec_cfg, sec_prot_keys_nw_info_t *sec_keys_nw_info, frame_counters_t *frame_counters)
{
    if (!interface_ptr || !next_gtks || !certs || !sec_cfg || !sec_keys_nw_info || !frame_counters) {
        return -1;
    }

    if (ws_pae_auth_get(interface_ptr) != NULL) {
        return 0;
    }

    pae_auth_t *pae_auth = ns_dyn_mem_alloc(sizeof(pae_auth_t));
    if (!pae_auth) {
        return -1;
    }

    memset(&pae_auth->network_name, 0, 33);
    pae_auth->pan_id = 0xffff;
    pae_auth->interface_ptr = interface_ptr;
    ws_pae_lib_supp_list_init(&pae_auth->active_supp_list);
    ws_pae_lib_supp_list_init(&pae_auth->waiting_supp_list);
    ws_pae_lib_shared_comp_list_init(&pae_auth->shared_comp_list);
    pae_auth->timer = NULL;

    pae_auth->hash_set = NULL;
    pae_auth->nw_key_insert = NULL;
    pae_auth->nw_keys_remove = NULL;
    pae_auth->nw_key_index_set = NULL;
    pae_auth->nw_info_updated = NULL;
    pae_auth->ip_addr_get = NULL;
    pae_auth->congestion_get = NULL;
    pae_auth->nw_frame_cnt_read = NULL;

    pae_auth->next_gtks = next_gtks;
    pae_auth->certs = certs;
    pae_auth->sec_keys_nw_info = sec_keys_nw_info;
    pae_auth->sec_cfg = sec_cfg;
    pae_auth->frame_counters = frame_counters;
    pae_auth->prev_system_time = 0;
    pae_auth->system_time_diff = 0;
    pae_auth->prev_frame_cnt = 0;
    pae_auth->prev_frame_cnt_timer = FRAME_CNT_TIMER;
    pae_auth->supp_max_number = SUPPLICANT_MAX_NUMBER;
    pae_auth->waiting_supp_list_size = 0;

    pae_auth->gtk_new_inst_req_exp = false;
    pae_auth->gtk_new_act_time_exp = false;
    pae_auth->prev_frame_cnt_set = false;
    pae_auth->prev_system_time_set = false;

    pae_auth->relay_socked_msg_if_instance_id = 0;
    pae_auth->radius_socked_msg_if_instance_id = 0;

    pae_auth->kmp_service = kmp_service_create();
    if (!pae_auth->kmp_service) {
        goto error;
    }

    if (kmp_service_cb_register(pae_auth->kmp_service, ws_pae_auth_kmp_incoming_ind, NULL, ws_pae_auth_kmp_service_addr_get, ws_pae_auth_kmp_service_ip_addr_get, ws_pae_auth_kmp_service_api_get)) {
        goto error;
    }

    if (kmp_service_event_if_register(pae_auth->kmp_service, ws_pae_auth_event_send)) {
        goto error;
    }

    if (kmp_service_timer_if_register(pae_auth->kmp_service, ws_pae_auth_timer_if_start, ws_pae_auth_timer_if_stop)) {
        goto error;
    }

    if (kmp_service_shared_comp_if_register(pae_auth->kmp_service, ws_pae_auth_shared_comp_add, ws_pae_auth_shared_comp_remove)) {
        goto error;
    }

    if (auth_key_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    // Register radius EAP-TLS and radius client security protocols
    if (radius_eap_tls_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }
    if (radius_client_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    // Register EAP-TLS and TLS security protocols
    if (auth_eap_tls_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }
    if (server_tls_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (auth_fwh_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (auth_gkh_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (msg_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (tasklet_id < 0) {
        tasklet_id = eventOS_event_handler_create(ws_pae_auth_tasklet_handler, PAE_TASKLET_INIT);
        if (tasklet_id < 0) {
            goto error;
        }
    }

    if (ws_pae_auth_timer_stop(pae_auth) < 0) {
        goto error;
    }

    ns_list_add_to_end(&pae_auth_list, pae_auth);

    return 0;

error:
    ws_pae_auth_free(pae_auth);

    return -1;
}

int8_t ws_pae_auth_addresses_set(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port)
{
    if (!interface_ptr || !remote_addr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }
    if (!pae_auth->kmp_service) {
        return -1;
    }

    if (kmp_socket_if_register(pae_auth->kmp_service, &pae_auth->relay_socked_msg_if_instance_id, true, local_port, remote_addr, remote_port) < 0) {
        return -1;
    }

    return 0;
}

int8_t ws_pae_auth_radius_address_set(protocol_interface_info_entry_t *interface_ptr, const uint8_t *remote_addr)
{
    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }
    if (!pae_auth->kmp_service) {
        return -1;
    }

    if (kmp_socket_if_register(pae_auth->kmp_service, &pae_auth->radius_socked_msg_if_instance_id, false, 0, remote_addr, 1812) < 0) {
        return -1;
    }

    return 0;
}

int8_t ws_pae_auth_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    ws_pae_auth_free(pae_auth);
    return 0;
}

void ws_pae_auth_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_auth_gtk_hash_set *hash_set, ws_pae_auth_nw_key_insert *nw_key_insert, ws_pae_auth_nw_key_index_set *nw_key_index_set, ws_pae_auth_nw_info_updated *nw_info_updated, ws_pae_auth_ip_addr_get *ip_addr_get, ws_pae_auth_congestion_get *congestion_get, ws_pae_auth_nw_frame_counter_read *nw_frame_cnt_read)
{
    if (!interface_ptr) {
        return;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return;
    }

    pae_auth->hash_set = hash_set;
    pae_auth->nw_key_insert = nw_key_insert;
    pae_auth->nw_key_index_set = nw_key_index_set;
    pae_auth->nw_info_updated = nw_info_updated;
    pae_auth->ip_addr_get = ip_addr_get;
    pae_auth->congestion_get = congestion_get;
    pae_auth->nw_frame_cnt_read = nw_frame_cnt_read;
}

void ws_pae_auth_start(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return;
    }

    // Checks if there is predefined active key
    int8_t index = sec_prot_keys_gtk_status_active_get(pae_auth->sec_keys_nw_info->gtks);
    if (index < 0) {
        // If there is no key, inserts a new one
        ws_pae_auth_gtk_key_insert(pae_auth);
        index = sec_prot_keys_gtk_install_order_first_index_get(pae_auth->sec_keys_nw_info->gtks);
        ws_pae_auth_active_gtk_set(pae_auth, index);
    } else {
        ws_pae_auth_active_gtk_set(pae_auth, index);
    }
    // Update keys to NVM as needed
    pae_auth->nw_info_updated(pae_auth->interface_ptr);

    // Inserts keys and updates GTK hash on stack
    ws_pae_auth_network_keys_from_gtks_set(pae_auth, false);

    // Sets active key index
    ws_pae_auth_network_key_index_set(pae_auth, index);

    pae_auth->prev_system_time = ws_pae_current_time_get();
    pae_auth->prev_system_time_set = true;
}

void ws_pae_auth_gtks_updated(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return;
    }

    ws_pae_auth_network_keys_from_gtks_set(pae_auth, false);
}

int8_t ws_pae_auth_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    ws_pae_auth_active_gtk_set(pae_auth, index);
    ws_pae_auth_network_key_index_set(pae_auth, index);
    return 0;
}

int8_t ws_pae_auth_node_keys_remove(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64)
{
    int8_t ret_value = -1;

    if (!interface_ptr) {
        return ret_value;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return ret_value;
    }

    // Checks if supplicant is active or waiting
    supp_entry_t *supp = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->active_supp_list, eui_64);
    if (!supp) {
        supp = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->waiting_supp_list, eui_64);
    }

    if (supp) {
        // Deletes keys and marks as revoked
        sec_prot_keys_pmk_delete(&supp->sec_keys);
        sec_prot_keys_ptk_delete(&supp->sec_keys);
        supp->access_revoked = true;
        tr_info("Access revoked; keys removed, eui-64: %s", trace_array(eui_64, 8));
        ret_value = 0;
    }

    if (ws_pae_key_storage_supp_delete(pae_auth, eui_64)) {
        tr_info("Access revoked; key store deleted, eui-64: %s", trace_array(eui_64, 8));
        ret_value = 0;
    }

    return ret_value;
}

int8_t ws_pae_auth_node_access_revoke_start(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    // Gets active GTK
    int8_t active_index = sec_prot_keys_gtk_status_active_get(pae_auth->sec_keys_nw_info->gtks);

    if (active_index >= 0) {
        // As default removes other keys than active
        int8_t not_removed_index = active_index;

        uint32_t revocation_lifetime = ws_pae_timers_gtk_revocation_lifetime_get(pae_auth->sec_cfg);

        uint32_t active_lifetime = sec_prot_keys_gtk_lifetime_get(pae_auth->sec_keys_nw_info->gtks, active_index);

        uint64_t current_time = ws_pae_current_time_get();

        // If active GTK lifetime is larger than revocation lifetime decrements active GTK lifetime
        if (active_lifetime > revocation_lifetime) {
            sec_prot_keys_gtk_lifetime_decrement(pae_auth->sec_keys_nw_info->gtks, active_index, current_time, active_lifetime - revocation_lifetime, true);
            tr_info("Access revocation start, GTK active index: %i, revoked lifetime: %"PRIu32"", active_index, revocation_lifetime);
        } else {
            // Otherwise decrements lifetime of the GTK to be installed after the active one
            int8_t second_index = sec_prot_keys_gtk_install_order_second_index_get(pae_auth->sec_keys_nw_info->gtks);
            if (second_index >= 0) {
                // Second GTK revocation lifetime is the active GTK lifetime added with revocation time
                uint32_t second_revocation_lifetime = active_lifetime + revocation_lifetime;

                uint32_t second_lifetime = sec_prot_keys_gtk_lifetime_get(pae_auth->sec_keys_nw_info->gtks, second_index);
                if (second_lifetime > second_revocation_lifetime) {
                    sec_prot_keys_gtk_lifetime_decrement(pae_auth->sec_keys_nw_info->gtks, second_index, current_time, second_lifetime - second_revocation_lifetime, true);
                    tr_info("Access revocation start, GTK second active index: %i, revoked lifetime: %"PRIu32"", second_index, second_revocation_lifetime);
                }
                // Removes other keys than active and GTK to be installed next
                not_removed_index = second_index;
            }
        }

        // Deletes other GTKs
        int8_t last_index = sec_prot_keys_gtk_install_order_last_index_get(pae_auth->sec_keys_nw_info->gtks);
        while (last_index >= 0 && last_index != not_removed_index) {
            tr_info("Access revocation GTK clear index: %i", last_index);
            sec_prot_keys_gtk_clear(pae_auth->sec_keys_nw_info->gtks, last_index);
            last_index = sec_prot_keys_gtk_install_order_last_index_get(pae_auth->sec_keys_nw_info->gtks);
        }
    }

    // Adds new GTK
    ws_pae_auth_gtk_key_insert(pae_auth);
    ws_pae_auth_network_keys_from_gtks_set(pae_auth, false);

    // Update keys to NVM as needed
    pae_auth->nw_info_updated(pae_auth->interface_ptr);

    return 0;
}

int8_t ws_pae_auth_node_limit_set(protocol_interface_info_entry_t *interface_ptr, uint16_t limit)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    pae_auth->supp_max_number = limit;

    return 0;
}

void ws_pae_auth_forced_gc(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return;
    }

    /* Purge in maximum five entries from supplicant list (starting from oldest one)
       per call to the function (called by nanostack monitor) */
    ws_pae_lib_supp_list_purge(pae_auth, &pae_auth->active_supp_list, 0, SUPPLICANT_NUMBER_TO_PURGE, NULL);
    ws_pae_lib_supp_list_purge(pae_auth, &pae_auth->waiting_supp_list, 0, SUPPLICANT_NUMBER_TO_PURGE, ws_pae_auth_waiting_supp_deleted);
}

int8_t ws_pae_auth_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name, bool updated)
{
    (void) updated;

    if (!interface_ptr || !network_name) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    // On authenticator pan_id is always selected locally and is always valid for keys
    if (pae_auth->sec_keys_nw_info->key_pan_id != pan_id) {
        pae_auth->sec_keys_nw_info->key_pan_id = pan_id;
        pae_auth->sec_keys_nw_info->updated = true;
    }

    bool update_keys = false;
    if (pae_auth->pan_id != 0xffff && pae_auth->pan_id != pan_id) {
        update_keys = true;
    }
    pae_auth->pan_id = pan_id;

    bool force_install = false;
    if (strlen((char *) &pae_auth->network_name) > 0 && strcmp((char *) &pae_auth->network_name, network_name) != 0) {
        update_keys = true;
        // Force GTK install to update the new network name to GAK
        force_install = true;
    }
    strcpy((char *) &pae_auth->network_name, network_name);

    if (!update_keys) {
        return 0;
    }

    if (pae_auth->nw_keys_remove) {
        pae_auth->nw_keys_remove(pae_auth->interface_ptr);
    }

    ws_pae_auth_network_keys_from_gtks_set(pae_auth, force_install);

    int8_t index = sec_prot_keys_gtk_status_active_get(pae_auth->sec_keys_nw_info->gtks);
    if (index >= 0) {
        // Sets active key index
        ws_pae_auth_network_key_index_set(pae_auth, index);
    }

    return 0;
}

static int8_t ws_pae_auth_network_keys_from_gtks_set(pae_auth_t *pae_auth, bool force_install)
{
    // Authenticator keys are always fresh
    sec_prot_keys_gtk_status_all_fresh_set(pae_auth->sec_keys_nw_info->gtks);

    if (pae_auth->hash_set) {
        uint8_t gtk_hash[32];
        sec_prot_keys_gtks_hash_generate(pae_auth->sec_keys_nw_info->gtks, gtk_hash);
        pae_auth->hash_set(pae_auth->interface_ptr, gtk_hash);
    }

    if (pae_auth->nw_key_insert) {
        pae_auth->nw_key_insert(pae_auth->interface_ptr, pae_auth->sec_keys_nw_info->gtks, force_install);
    }

    return 0;
}

static int8_t ws_pae_auth_active_gtk_set(pae_auth_t *pae_auth, uint8_t index)
{
    return sec_prot_keys_gtk_status_active_set(pae_auth->sec_keys_nw_info->gtks, index);
}

static int8_t ws_pae_auth_gtk_clear(pae_auth_t *pae_auth, uint8_t index)
{
    return sec_prot_keys_gtk_clear(pae_auth->sec_keys_nw_info->gtks, index);
}

static int8_t ws_pae_auth_network_key_index_set(pae_auth_t *pae_auth, uint8_t index)
{
    if (pae_auth->nw_key_index_set) {
        pae_auth->nw_key_index_set(pae_auth->interface_ptr, index);
    }

    return 0;
}

static void ws_pae_auth_free(pae_auth_t *pae_auth)
{
    if (!pae_auth) {
        return;
    }

    ws_pae_lib_shared_comp_list_free(&pae_auth->shared_comp_list);

    ws_pae_lib_supp_list_delete(&pae_auth->active_supp_list);
    ws_pae_lib_supp_list_delete(&pae_auth->waiting_supp_list);

    kmp_socket_if_unregister(pae_auth->kmp_service);

    kmp_service_delete(pae_auth->kmp_service);

    ns_list_remove(&pae_auth_list, pae_auth);
    ns_dyn_mem_free(pae_auth);
}

static pae_auth_t *ws_pae_auth_get(protocol_interface_info_entry_t *interface_ptr)
{
    ns_list_foreach(pae_auth_t, entry, &pae_auth_list) {
        if (entry->interface_ptr == interface_ptr) {
            return entry;
        }
    }

    return NULL;
}

static pae_auth_t *ws_pae_auth_by_kmp_service_get(kmp_service_t *service)
{
    ns_list_foreach(pae_auth_t, entry, &pae_auth_list) {
        if (entry->kmp_service == service) {
            return entry;
        }
    }

    return NULL;
}

static int8_t ws_pae_auth_event_send(kmp_service_t *service, void *data)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return -1;
    }

    arm_event_s event = {
        .receiver = tasklet_id,
        .sender = 0,
        .event_id = pae_auth->interface_ptr->id,
        .data_ptr = data,
        .event_type = PAE_TASKLET_EVENT,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        return -1;
    }

    return 0;
}

static void ws_pae_auth_tasklet_handler(arm_event_s *event)
{
    if (event->event_type == PAE_TASKLET_INIT) {

    } else if (event->event_type == PAE_TASKLET_EVENT) {
        pae_auth_t *pae_auth = NULL;

        ns_list_foreach(pae_auth_t, entry, &pae_auth_list) {
            if (entry->interface_ptr->id == event->event_id) {
                pae_auth = entry;
                break;
            }
        }

        if (pae_auth) {
            kmp_service_event_if_event(pae_auth->kmp_service, event->data_ptr);
        }
    }
}

void ws_pae_auth_fast_timer(uint16_t ticks)
{
    ns_list_foreach(pae_auth_t, pae_auth, &pae_auth_list) {
        if (!ws_pae_auth_timer_running(pae_auth)) {
            continue;
        }

        // Updates KMP timers
        bool active_running = ws_pae_lib_supp_list_timer_update(pae_auth, &pae_auth->active_supp_list, ticks, kmp_service_timer_if_timeout, ws_pae_auth_active_supp_deleted);
        bool wait_running = ws_pae_lib_supp_list_timer_update(pae_auth, &pae_auth->waiting_supp_list, ticks, kmp_service_timer_if_timeout, ws_pae_auth_waiting_supp_deleted);
        if (!active_running && !wait_running) {
            ws_pae_auth_timer_stop(pae_auth);
        }
    }

    // Update key storage fast timer
    ws_pae_key_storage_fast_timer(ticks);
}

void ws_pae_auth_slow_timer(uint16_t seconds)
{
    ns_list_foreach(pae_auth_t, pae_auth, &pae_auth_list) {

        // Gets index of currently active GTK
        int8_t active_index = sec_prot_keys_gtk_status_active_get(pae_auth->sec_keys_nw_info->gtks);

        uint64_t current_time = ws_pae_current_time_get();

        for (uint8_t i = 0; i < GTK_NUM; i++) {
            if (!sec_prot_keys_gtk_is_set(pae_auth->sec_keys_nw_info->gtks, i)) {
                continue;
            }
            uint32_t gtk_lifetime_dec_extra_seconds = 0;
            if (active_index == i) {
                gtk_lifetime_dec_extra_seconds = ws_pae_auth_lifetime_key_frame_cnt_check(pae_auth, i, seconds);
                gtk_lifetime_dec_extra_seconds = ws_pae_auth_lifetime_system_time_check(pae_auth, i, seconds, gtk_lifetime_dec_extra_seconds);
            }
            uint32_t timer_seconds = sec_prot_keys_gtk_lifetime_decrement(pae_auth->sec_keys_nw_info->gtks, i, current_time, seconds + gtk_lifetime_dec_extra_seconds, true);
            if (active_index == i) {
                if (!pae_auth->gtk_new_inst_req_exp) {
                    pae_auth->gtk_new_inst_req_exp = ws_pae_timers_gtk_new_install_required(pae_auth->sec_cfg, timer_seconds);
                    if (pae_auth->gtk_new_inst_req_exp) {
                        int8_t second_index = sec_prot_keys_gtk_install_order_second_index_get(pae_auth->sec_keys_nw_info->gtks);
                        if (second_index < 0) {
                            tr_info("GTK new install required active index: %i, time: %"PRIu32", system time: %"PRIu32"", active_index, timer_seconds, protocol_core_monotonic_time / 10);
                            ws_pae_auth_gtk_key_insert(pae_auth);
                            ws_pae_auth_network_keys_from_gtks_set(pae_auth, false);
                            // Update keys to NVM as needed
                            pae_auth->nw_info_updated(pae_auth->interface_ptr);
                        } else {
                            tr_info("GTK new install already done; second index: %i, time: %"PRIu32", system time: %"PRIu32"", second_index, timer_seconds, protocol_core_monotonic_time / 10);
                        }
                    }
                }

                if (!pae_auth->gtk_new_act_time_exp) {
                    pae_auth->gtk_new_act_time_exp =  ws_pae_timers_gtk_new_activation_time(pae_auth->sec_cfg, timer_seconds);
                    if (pae_auth->gtk_new_act_time_exp) {
                        int8_t new_active_index = ws_pae_auth_new_gtk_activate(pae_auth);
                        tr_info("GTK new activation time active index: %i, time: %"PRIu32", new index: %i, system time: %"PRIu32"", active_index, timer_seconds, new_active_index, protocol_core_monotonic_time / 10);
                        if (new_active_index >= 0) {
                            ws_pae_auth_network_key_index_set(pae_auth, new_active_index);
                        }
                        pae_auth->gtk_new_inst_req_exp = false;
                        pae_auth->gtk_new_act_time_exp = false;
                        // Update keys to NVM as needed
                        pae_auth->nw_info_updated(pae_auth->interface_ptr);
                    }
                }
                if (gtk_lifetime_dec_extra_seconds != 0) {
                    // Update keys to NVM as needed
                    pae_auth->nw_info_updated(pae_auth->interface_ptr);
                }
            }

            if (timer_seconds == 0) {
                tr_info("GTK expired index: %i, system time: %"PRIu32"", i, protocol_core_monotonic_time / 10);
                ws_pae_auth_gtk_clear(pae_auth, i);
                ws_pae_auth_network_keys_from_gtks_set(pae_auth, false);
                // Update keys to NVM as needed
                pae_auth->nw_info_updated(pae_auth->interface_ptr);
            }
        }

        ws_pae_lib_supp_list_slow_timer_update(&pae_auth->active_supp_list, seconds);

        ws_pae_lib_shared_comp_list_timeout(&pae_auth->shared_comp_list, seconds);
    }

    // Update key storage timer
    ws_pae_key_storage_timer(seconds);
}

static uint32_t ws_pae_auth_lifetime_key_frame_cnt_check(pae_auth_t *pae_auth, uint8_t gtk_index, uint16_t seconds)
{
    uint32_t decrement_seconds = 0;

    if (pae_auth->prev_frame_cnt_timer > seconds) {
        pae_auth->prev_frame_cnt_timer -= seconds;
        return 0;
    }
    pae_auth->prev_frame_cnt_timer = FRAME_CNT_TIMER;

    uint32_t frame_cnt = 0;
    if (pae_auth->nw_frame_cnt_read(pae_auth->interface_ptr, &frame_cnt, gtk_index) < 0) {
        return 0;
    }

    sec_timer_cfg_t *timer_cfg = &pae_auth->sec_cfg->timer_cfg;

    // For GTK lifetime and frame counter space calculate the percent that has been used
    uint32_t gtk_lifetime_left = sec_prot_keys_gtk_lifetime_get(pae_auth->sec_keys_nw_info->gtks, gtk_index);
    uint32_t gtk_lifetime = timer_cfg->gtk_expire_offset;
    uint32_t gtk_lifetime_left_percent = gtk_lifetime_left * 100 / gtk_lifetime;

    uint32_t frame_cnt_left_percent = ((uint64_t)((UINT32_MAX - frame_cnt))) * 100 / UINT32_MAX;

    tr_info("Active GTK lifetime %"PRIu32", frame counter %"PRIu32" percent, counter %"PRIu32, gtk_lifetime_left_percent, frame_cnt_left_percent, frame_cnt);

    /* If frame counter space has been exhausted faster than should be based on GTK lifetime
     * decrements GTK lifetime. Do not check until 20% of the frame counter space has been used
     * so that we have data from longer time period. As sanity check, validate that GTK lifetime
     * is not more than 105% of the GTK lifetime.
     */
    uint32_t gtk_new_install_req_seconds = timer_cfg->gtk_expire_offset - timer_cfg->gtk_new_install_req * timer_cfg->gtk_expire_offset / 100;
    if ((frame_cnt_left_percent < gtk_lifetime_left_percent && frame_cnt_left_percent < 80) ||
            gtk_lifetime_left_percent > 105) {
        // If not yet on GTK update period
        if (gtk_lifetime_left > (gtk_new_install_req_seconds + SECONDS_IN_DAY)) {
            uint32_t diff = gtk_lifetime_left_percent - frame_cnt_left_percent;
            decrement_seconds = gtk_lifetime * diff / 100 + SECONDS_IN_DAY;
            if (decrement_seconds > gtk_lifetime_left - gtk_new_install_req_seconds) {
                decrement_seconds = gtk_lifetime_left - gtk_new_install_req_seconds;
            }
            tr_info("Decrement GTK lifetime percent, seconds %"PRIu32, decrement_seconds);
        }
    }

    // Calculate how much frame counters have changed and store maximum if larger than previous maximum
    uint32_t frame_cnt_diff = 0;
    if (pae_auth->prev_frame_cnt_set && frame_cnt > pae_auth->prev_frame_cnt) {
        frame_cnt_diff = frame_cnt - pae_auth->prev_frame_cnt;
        if (frame_cnt_diff > pae_auth->frame_counters->counter[gtk_index].max_frame_counter_chg) {
            pae_auth->frame_counters->counter[gtk_index].max_frame_counter_chg = frame_cnt_diff;
        }
    }

    tr_info("Frame counter change %"PRIu32", max %"PRIu32, frame_cnt_diff, pae_auth->frame_counters->counter[gtk_index].max_frame_counter_chg);

    /* Calculates an estimate for how much free frame counter space is needed for the GTK update and
     * initiates it faster if needed (default length of GTK update is 6 days).
     */
    uint32_t max_needed_frame_counters =
        pae_auth->frame_counters->counter[gtk_index].max_frame_counter_chg * gtk_new_install_req_seconds / 3600;
    // Adds 20% to calculated value
    max_needed_frame_counters = max_needed_frame_counters * 120 / 100;
    // If estimated value is more than is left starts GTK update right away (if not already started)
    if (max_needed_frame_counters >= (UINT32_MAX - frame_cnt)) {
        if (gtk_lifetime_left > gtk_new_install_req_seconds) {
            decrement_seconds = gtk_lifetime_left - gtk_new_install_req_seconds;
            tr_info("Decrement GTK lifetime update, seconds %"PRIu32, decrement_seconds);
        }
    }

    /* Calculates an estimate for how much free frame counter space is needed for the GTK activation and
     * initiates it faster if needed (default length of GTK activation is 60 minutes).
     */
    uint32_t gtk_new_activation_time_seconds = timer_cfg->gtk_expire_offset / timer_cfg->gtk_new_act_time;
    // Calculates the estimated maximum value for frame counter during GTK update
    max_needed_frame_counters =
        pae_auth->frame_counters->counter[gtk_index].max_frame_counter_chg * gtk_new_activation_time_seconds / 3600;
    // Adds 200% to calculated value
    max_needed_frame_counters = max_needed_frame_counters * 300 / 100;
    // If estimated value is more than is left starts GTK update right away (if not already started)
    if (max_needed_frame_counters >= (UINT32_MAX - frame_cnt)) {
        if (gtk_lifetime_left > gtk_new_activation_time_seconds) {
            decrement_seconds = gtk_lifetime_left - gtk_new_activation_time_seconds;
            tr_info("Decrement GTK lifetime activation, seconds %"PRIu32, decrement_seconds);
        }
    }

    pae_auth->prev_frame_cnt = frame_cnt;
    pae_auth->prev_frame_cnt_set = true;

    return decrement_seconds;
}

static uint32_t ws_pae_auth_lifetime_system_time_check(pae_auth_t *pae_auth, int8_t gtk_index, uint16_t seconds, uint32_t dec_extra_seconds)
{
    // Read current system time and compare it to previous time
    uint64_t current_time = ws_pae_current_time_get();
    if (pae_auth->prev_system_time_set) {
        if (current_time > pae_auth->prev_system_time + TIME_MINIMUM_DIFFERENCE) {
            pae_auth->system_time_diff += current_time - pae_auth->prev_system_time;
        }
    }
    pae_auth->prev_system_time = current_time;
    pae_auth->prev_system_time_set = true;

    uint64_t time_diff = 0;
    // Update lifetimes only if time difference is more than hour
    if (pae_auth->system_time_diff > TIME_DIFFERENCE_THRESHOLD + seconds + dec_extra_seconds) {
        time_diff = pae_auth->system_time_diff - seconds - dec_extra_seconds;
    } else {
        return dec_extra_seconds;
    }
    pae_auth->system_time_diff = 0;

    uint32_t new_dec_extra_seconds = dec_extra_seconds;

    if (time_diff > 0) {
        /* If the system time has made a large jump then use the stored time to calculate the lifetime
           (this implies e.g. that new time has been received from NTP and old time was not valid) */
        if (!ws_pae_time_old_and_new_validate(current_time, current_time + time_diff)) {
            // Allow one jump without invalidating active GTK
            if (pae_auth->sec_keys_nw_info->system_time_changed == SYSTEM_TIME_NOT_CHANGED) {
                pae_auth->sec_keys_nw_info->system_time_changed = SYSTEM_TIME_CHANGED;
                tr_info("System time large change ignored; difference: %"PRIu64, time_diff);
                time_diff = 0;
            }
        }

        uint32_t gtk_lifetime_left = sec_prot_keys_gtk_lifetime_get(pae_auth->sec_keys_nw_info->gtks, gtk_index);
        sec_timer_cfg_t *timer_cfg = &pae_auth->sec_cfg->timer_cfg;
        uint32_t gtk_new_activation_time_seconds = timer_cfg->gtk_expire_offset / timer_cfg->gtk_new_act_time;

        // If there is GTK lifetime left
        if (gtk_lifetime_left > (seconds + dec_extra_seconds + time_diff)) {
            // If GTK lifetime would be less than new activation time sets decrements time to activation time
            if (gtk_lifetime_left - seconds - dec_extra_seconds - time_diff < gtk_new_activation_time_seconds) {
                new_dec_extra_seconds = gtk_lifetime_left - gtk_new_activation_time_seconds;
            } else {
                // Decrements GTK lifetime
                new_dec_extra_seconds = dec_extra_seconds + time_diff;
            }
        } else {
            // If there is no GTK lifetime left decrements time to activation time
            new_dec_extra_seconds = gtk_lifetime_left - gtk_new_activation_time_seconds;
        }

        tr_info("System change difference: %"PRIu64" decrement extra: %"PRIu32" (seconds: %"PRIu16" previous extra %"PRIu32")", time_diff, new_dec_extra_seconds, seconds, dec_extra_seconds);
    }

    return new_dec_extra_seconds;
}

static void ws_pae_auth_gtk_key_insert(pae_auth_t *pae_auth)
{
    // Gets index to install the key
    uint8_t install_index = sec_prot_keys_gtk_install_index_get(pae_auth->sec_keys_nw_info->gtks);

    // Key to install
    uint8_t gtk_value[GTK_LEN];

    // Checks if next GTK values are set and gets first GTK to install
    int8_t next_gtk_index = sec_prot_keys_gtk_install_order_first_index_get(pae_auth->next_gtks);
    if (next_gtk_index >= 0) {
        // Gets GTK value
        uint8_t *gtk = sec_prot_keys_gtk_get(pae_auth->next_gtks, next_gtk_index);
        memcpy(gtk_value, gtk, GTK_LEN);
        // Sets same key back to next GTKs but as the last key to be installed
        sec_prot_keys_gtk_clear(pae_auth->next_gtks, next_gtk_index);
        sec_prot_keys_gtk_set(pae_auth->next_gtks, next_gtk_index, gtk_value, 0);
    } else {
        do {
            randLIB_get_n_bytes_random(gtk_value, GTK_LEN);
        } while (sec_prot_keys_gtk_valid_check(gtk_value) < 0);
    }

    // Gets latest installed key lifetime and adds GTK expire offset to it
    uint32_t lifetime = pae_auth->sec_cfg->timer_cfg.gtk_expire_offset;
    int8_t last_index = sec_prot_keys_gtk_install_order_last_index_get(pae_auth->sec_keys_nw_info->gtks);
    if (last_index >= 0) {
        lifetime += sec_prot_keys_gtk_lifetime_get(pae_auth->sec_keys_nw_info->gtks, last_index);
    }

    // Installs the new key
    sec_prot_keys_gtk_clear(pae_auth->sec_keys_nw_info->gtks, install_index);
    sec_prot_keys_gtk_set(pae_auth->sec_keys_nw_info->gtks, install_index, gtk_value, lifetime);

    // Authenticator keys are always fresh
    sec_prot_keys_gtk_status_all_fresh_set(pae_auth->sec_keys_nw_info->gtks);

    tr_info("GTK install new index: %i, lifetime: %"PRIu32" system time: %"PRIu32"", install_index, lifetime, protocol_core_monotonic_time / 10);
}

static int8_t ws_pae_auth_new_gtk_activate(pae_auth_t *pae_auth)
{
    int8_t new_active_index = sec_prot_keys_gtk_install_order_second_index_get(pae_auth->sec_keys_nw_info->gtks);
    if (new_active_index >= 0) {
        ws_pae_auth_active_gtk_set(pae_auth, new_active_index);
    }

    return new_active_index;
}

static int8_t ws_pae_auth_timer_if_start(kmp_service_t *service, kmp_api_t *kmp)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return -1;
    }

    if (ws_pae_auth_timer_start(pae_auth) < 0) {
        return -1;
    }

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        return -1;
    }

    kmp_entry_t *entry = ws_pae_lib_kmp_list_entry_get(&supp_entry->kmp_list, kmp);
    if (!entry) {
        return -1;
    }

    ws_pae_lib_kmp_timer_start(&supp_entry->kmp_list, entry);
    return 0;
}

static int8_t ws_pae_auth_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp)
{
    (void) service;

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);

    kmp_entry_t *entry = ws_pae_lib_kmp_list_entry_get(&supp_entry->kmp_list, kmp);
    if (!entry) {
        return -1;
    }

    ws_pae_lib_kmp_timer_stop(&supp_entry->kmp_list, entry);
    return 0;
}

static int8_t ws_pae_auth_shared_comp_add(kmp_service_t *service, kmp_shared_comp_t *data)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return -1;
    }

    return ws_pae_lib_shared_comp_list_add(&pae_auth->shared_comp_list, data);
}

static int8_t ws_pae_auth_shared_comp_remove(kmp_service_t *service, kmp_shared_comp_t *data)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return -1;
    }

    return ws_pae_lib_shared_comp_list_remove(&pae_auth->shared_comp_list, data);
}

static int8_t ws_pae_auth_timer_start(pae_auth_t *pae_auth)
{
    pae_auth->timer_running = true;
    return 0;
}

static int8_t ws_pae_auth_timer_stop(pae_auth_t *pae_auth)
{
    pae_auth->timer_running = false;
    return 0;
}

static bool ws_pae_auth_timer_running(pae_auth_t *pae_auth)
{
    return pae_auth->timer_running;
}

static void ws_pae_auth_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return;
    }

    // Get own EUI-64
    link_layer_address_s mac_params;
    if (arm_nwk_mac_address_read(pae_auth->interface_ptr->id, &mac_params) >= 0) {
        kmp_address_eui_64_set(local_addr, mac_params.mac_long);
    }

    // Get supplicant address
    supp_entry_t *entry = kmp_api_data_get(kmp);
    if (entry) {
        kmp_address_copy(remote_addr, &entry->addr);
    }
}

static void ws_pae_auth_kmp_service_ip_addr_get(kmp_service_t *service, kmp_api_t *kmp, uint8_t *address)
{
    (void) kmp;

    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return;
    }

    pae_auth->ip_addr_get(pae_auth->interface_ptr, address);
}

static kmp_api_t *ws_pae_auth_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type)
{
    (void) service;

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        return NULL;
    }

    return ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, type);
}

static bool ws_pae_auth_active_limit_reached(uint16_t active_supp, pae_auth_t *pae_auth)
{
    return pae_auth->congestion_get(pae_auth->interface_ptr, active_supp);
}

static void ws_pae_auth_waiting_supp_remove_oldest(pae_auth_t *pae_auth, const kmp_addr_t *addr)
{
    supp_entry_t *delete_supp = ns_list_get_last(&pae_auth->waiting_supp_list);
    if (!delete_supp) {
        return;
    }
    tr_info("PAE: waiting list full, eui-64: %s, deleted eui-64: %s", trace_array(addr->eui_64, 8), trace_array(delete_supp->addr.eui_64, 8));
    // Create new instance
    kmp_api_t *new_kmp = ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, MSG_PROT, pae_auth->relay_socked_msg_if_instance_id, delete_supp, pae_auth->sec_cfg);
    if (!new_kmp) {
        return;
    }
    kmp_api_create_request(new_kmp, MSG_PROT, &delete_supp->addr, &delete_supp->sec_keys);
    (void) ws_pae_lib_supp_list_remove(pae_auth, &pae_auth->waiting_supp_list, delete_supp, ws_pae_auth_waiting_supp_deleted);
}

static supp_entry_t *ws_pae_auth_waiting_supp_list_add(pae_auth_t *pae_auth, supp_entry_t *supp_entry, const kmp_addr_t *addr)
{
    // Entry is already allocated
    if (supp_entry) {
        // If the waiting list if full removes the oldest entry from the list
        if (pae_auth->waiting_supp_list_size >= WAITING_SUPPLICANT_LIST_MAX_SIZE) {
            ws_pae_auth_waiting_supp_remove_oldest(pae_auth, addr);
        }
        ns_list_add_to_start(&pae_auth->waiting_supp_list, supp_entry);
        pae_auth->waiting_supp_list_size++;
    } else {
        // If the waiting list if full removes the oldest entry from the list
        if (pae_auth->waiting_supp_list_size >= WAITING_SUPPLICANT_LIST_MAX_SIZE) {
            ws_pae_auth_waiting_supp_remove_oldest(pae_auth, addr);
        }
        supp_entry = ws_pae_lib_supp_list_add(&pae_auth->waiting_supp_list, addr);
        if (!supp_entry) {
            tr_info("PAE: waiting list no memory, eui-64: %s", trace_array(addr->eui_64, 8));
            return NULL;
        }
        pae_auth->waiting_supp_list_size++;
        sec_prot_keys_init(&supp_entry->sec_keys, pae_auth->sec_keys_nw_info->gtks, pae_auth->certs);
    }

    // 90 percent of the EAPOL temporary entry lifetime (10 ticks per second)
    supp_entry->waiting_ticks = pae_auth->sec_cfg->timing_cfg.temp_eapol_min_timeout * 900 / 100;

    tr_info("PAE: to waiting, list size %i, retry %i, eui-64: %s", pae_auth->waiting_supp_list_size, supp_entry->waiting_ticks, trace_array(supp_entry->addr.eui_64, 8));

    return supp_entry;
}

static kmp_api_t *ws_pae_auth_kmp_incoming_ind(kmp_service_t *service, uint8_t msg_if_instance_id, kmp_type_e type, const kmp_addr_t *addr, const void *pdu, uint16_t size)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return NULL;
    }

    // For radius messages
    if (msg_if_instance_id == pae_auth->radius_socked_msg_if_instance_id) {
        // Find KMP from list of active supplicants based on radius message
        kmp_api_t *kmp_api = ws_pae_lib_supp_list_kmp_receive_check(&pae_auth->active_supp_list, pdu, size);
        return kmp_api;
    }

    // For relay messages find supplicant from list of active supplicants based on EUI-64
    supp_entry_t *supp_entry = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->active_supp_list, kmp_address_eui_64_get(addr));

    if (!supp_entry) {
        uint16_t active_supp = ns_list_count(&pae_auth->active_supp_list);

        // Check if supplicant is already on the the waiting supplicant list
        supp_entry = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->waiting_supp_list, kmp_address_eui_64_get(addr));
        if (supp_entry) {
            /* Remove from waiting list (supplicant is later added to active list, or if no room back to the start of the
             * waiting list with updated timer)
             */
            ns_list_remove(&pae_auth->waiting_supp_list, supp_entry);
            pae_auth->waiting_supp_list_size--;
            supp_entry->waiting_ticks = 0;
        } else {
            // Find supplicant from key storage
            supp_entry = ws_pae_key_storage_supp_read(pae_auth, kmp_address_eui_64_get(addr), pae_auth->sec_keys_nw_info->gtks, pae_auth->certs);
        }

        // Checks if active supplicant list has space for new supplicants
        if (ws_pae_auth_active_limit_reached(active_supp, pae_auth)) {
            tr_debug("PAE: active limit reached, eui-64: %s", trace_array(kmp_address_eui_64_get(addr), 8));
            // If there is no space, add supplicant entry to the start of the waiting supplicant list
            supp_entry = ws_pae_auth_waiting_supp_list_add(pae_auth, supp_entry, addr);
            if (!supp_entry) {
                return 0;
            }
        } else {
            if (supp_entry) {
                /*
                 * If there is space and there is already an allocated supplicant, add it to active list and
                 * start/continue authentication
                 */
                tr_debug("PAE: to active, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
                ns_list_add_to_start(&pae_auth->active_supp_list, supp_entry);
            }
        }
    }

    // If supplicant does not exists create a new supplicant entry to the active list
    if (!supp_entry) {
        supp_entry = ws_pae_lib_supp_list_add(&pae_auth->active_supp_list, addr);
        if (!supp_entry) {
            return 0;
        }
        sec_prot_keys_init(&supp_entry->sec_keys, pae_auth->sec_keys_nw_info->gtks, pae_auth->certs);
    } else {
        // Updates relay address
        kmp_address_copy(&supp_entry->addr, addr);
    }

    // Increases waiting time for supplicant authentication
    ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_FOR_AUTHENTICATION_TICKS);

    kmp_type_e kmp_type_to_search = type;

    // If radius is enabled, route EAP-TLS to radius EAP-TLS
    if (pae_auth->sec_cfg->radius_cfg != NULL && pae_auth->sec_cfg->radius_cfg->radius_addr_set && type == IEEE_802_1X_MKA) {
        kmp_type_to_search = RADIUS_IEEE_802_1X_MKA;
    }

    // Search for existing KMP for supplicant
    kmp_api_t *kmp = ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, kmp_type_to_search);
    if (kmp) {
        return kmp;
    }

    // Create a new KMP for initial eapol-key
    kmp = kmp_api_create(service, type + IEEE_802_1X_INITIAL_KEY, pae_auth->relay_socked_msg_if_instance_id, pae_auth->sec_cfg);

    if (!kmp) {
        return 0;
    }

    kmp_api_data_set(kmp, supp_entry);
    // Sets address to KMP
    kmp_api_addr_set(kmp, &supp_entry->addr);

    // Sets security keys to KMP
    kmp_api_sec_keys_set(kmp, &supp_entry->sec_keys);

    if (ws_pae_lib_kmp_list_add(&supp_entry->kmp_list, kmp) == NULL) {
        kmp_api_delete(kmp);
        return 0;
    }

    kmp_api_cb_register(kmp,
                        ws_pae_auth_kmp_api_create_confirm,
                        ws_pae_auth_kmp_api_create_indication,
                        ws_pae_auth_kmp_api_finished_indication,
                        ws_pae_auth_kmp_api_finished);

    if (kmp_api_start(kmp) < 0) {
        ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);
        return 0;
    }

    return kmp;
}

static void ws_pae_auth_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result)
{
    (void) kmp;
    (void) result;
    // If KMP-CREATE.request has failed, authentication error, just stop for now
}

static void ws_pae_auth_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr)
{
    (void) type;
    (void) addr;
    // For now, accept every KMP-CREATE.indication
    kmp_api_create_response(kmp, KMP_RESULT_OK);
}

static bool ws_pae_auth_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys)
{
    (void) sec_keys;

    // For now, just ignore if not ok
    if (result != KMP_RESULT_OK) {
        return false;
    }

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        // Should not be possible
        return false;
    }
    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        // Should not be possible
        return false;
    }

    // Ensures that supplicant is in active supplicant list before initiating next KMP
    if (!ws_pae_lib_supp_list_entry_is_in_list(&pae_auth->active_supp_list, supp_entry)) {
        return false;
    }

    return ws_pae_auth_next_kmp_trigger(pae_auth, supp_entry);
}

static bool ws_pae_auth_next_kmp_trigger(pae_auth_t *pae_auth, supp_entry_t *supp_entry)
{
    // Get next protocol based on what keys supplicant has
    kmp_type_e next_type = ws_pae_auth_next_protocol_get(pae_auth, supp_entry);

    if (next_type == KMP_TYPE_NONE) {
        // Supplicant goes inactive after 15 seconds
        ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_AFTER_AUTHENTICATION_TICKS);
        // All done
        return true;
    } else {
        kmp_api_t *api = ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, next_type);
        if (api != NULL) {
            /* For other types than GTK, only one ongoing negotiation at the same time,
               for GTK there can be previous terminating and the new one for next key index */
            if (next_type != IEEE_802_11_GKH) {
                tr_info("KMP already ongoing; ignored, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
                return false;
            }
        }
    }

    // Increases waiting time for supplicant authentication
    ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_FOR_AUTHENTICATION_TICKS);

    // Create new instance
    kmp_api_t *new_kmp = ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, next_type, pae_auth->relay_socked_msg_if_instance_id, supp_entry, pae_auth->sec_cfg);
    if (!new_kmp) {
        return false;
    }

    // For radius EAP-TLS create also radius client in addition to EAP-TLS
    if (next_type == RADIUS_IEEE_802_1X_MKA) {
        if (ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, RADIUS_CLIENT_PROT) != NULL) {
            // Radius client already exists, wait for it to be deleted
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return false;
        }
        // Create radius client instance */
        if (ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, RADIUS_CLIENT_PROT, pae_auth->radius_socked_msg_if_instance_id, supp_entry, pae_auth->sec_cfg) == NULL) {
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return false;
        }
    }
    // For EAP-TLS create also TLS client in addition to EAP-TLS
    if (next_type == IEEE_802_1X_MKA) {
        if (ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, TLS_PROT) != NULL) {
            // TLS already exists, wait for it to be deleted
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return false;
        }
        // Create TLS instance */
        if (ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, TLS_PROT, pae_auth->relay_socked_msg_if_instance_id, supp_entry, pae_auth->sec_cfg) == NULL) {
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return false;
        }
    }

    kmp_api_create_request(new_kmp, next_type, &supp_entry->addr, &supp_entry->sec_keys);
    return false;
}

static kmp_type_e ws_pae_auth_next_protocol_get(pae_auth_t *pae_auth, supp_entry_t *supp_entry)
{
    kmp_type_e next_type = KMP_TYPE_NONE;
    sec_prot_keys_t *sec_keys = &supp_entry->sec_keys;

    // Supplicant has indicated that PMK is not valid
    if (sec_keys->pmk_mismatch) {
        sec_keys->ptk_mismatch = true;
        // start EAP-TLS towards supplicant
        if (pae_auth->sec_cfg->radius_cfg != NULL && pae_auth->sec_cfg->radius_cfg->radius_addr_set) {
            next_type = RADIUS_IEEE_802_1X_MKA;
        } else {
            next_type = IEEE_802_1X_MKA;
        }
        tr_info("PAE: start EAP-TLS, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
    } else if (sec_keys->ptk_mismatch) {
        // start 4WH towards supplicant
        next_type = IEEE_802_11_4WH;
        tr_info("PAE: start 4WH, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
    }

    int8_t gtk_index = -1;
    if (next_type != IEEE_802_1X_MKA && next_type != RADIUS_IEEE_802_1X_MKA) {
        // Checks if GTK needs to be inserted
        gtk_index = sec_prot_keys_gtk_insert_index_from_gtkl_get(sec_keys);

        // For 4WH insert always a key, in case no other then active
        if (next_type == IEEE_802_11_4WH && gtk_index < 0) {
            gtk_index = sec_prot_keys_gtk_status_active_get(sec_keys->gtks);
        }
    }

    if (gtk_index >= 0) {
        if (next_type == KMP_TYPE_NONE && gtk_index >= 0) {

            /* Check if the PTK has been already used to install GTK to specific index and if it
             * has been, trigger 4WH to update also the PTK. This prevents writing multiple
             * GTK keys to same index using same PTK.
             */
            if (pae_auth->sec_cfg->timer_cfg.gtk_expire_offset > SHORT_GTK_LIFETIME &&
                    sec_prot_keys_ptk_installed_gtk_hash_mismatch_check(sec_keys, gtk_index)) {
                // start 4WH towards supplicant
                next_type = IEEE_802_11_4WH;
                sec_keys->ptk_mismatch = true;
                tr_info("PAE: start 4WH due to GTK index re-use, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
            } else {
                // Update just GTK
                next_type = IEEE_802_11_GKH;
                tr_info("PAE: start GKH, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
            }
        }

        tr_info("PAE: update GTK index: %i, eui-64: %s", gtk_index, trace_array(supp_entry->addr.eui_64, 8));
    }

    if (next_type == KMP_TYPE_NONE) {
        tr_info("PAE: authenticated, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
    }

    return next_type;
}

static kmp_api_t *ws_pae_auth_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, uint8_t socked_msg_if_instance_id, supp_entry_t *supp_entry, sec_cfg_t *sec_cfg)
{
    // Create KMP instance for new authentication
    kmp_api_t *kmp = kmp_api_create(service, type, socked_msg_if_instance_id, sec_cfg);

    if (!kmp) {
        return NULL;
    }

    kmp_api_data_set(kmp, supp_entry);
    // Sets address to KMP
    kmp_api_addr_set(kmp, &supp_entry->addr);

    // Sets security keys to KMP
    kmp_api_sec_keys_set(kmp, &supp_entry->sec_keys);

    if (ws_pae_lib_kmp_list_add(&supp_entry->kmp_list, kmp) == NULL) {
        kmp_api_delete(kmp);
        return NULL;
    }

    kmp_api_cb_register(kmp,
                        ws_pae_auth_kmp_api_create_confirm,
                        ws_pae_auth_kmp_api_create_indication,
                        ws_pae_auth_kmp_api_finished_indication,
                        ws_pae_auth_kmp_api_finished);

    kmp_api_data_set(kmp, supp_entry);

    if (kmp_api_start(kmp) < 0) {
        ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);
        return NULL;
    }

    return kmp;
}

static void ws_pae_auth_kmp_api_finished(kmp_api_t *kmp)
{
    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        // Should not be possible
        return;
    }

    // Delete KMP
    ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);
}

static void ws_pae_auth_active_supp_deleted(void *pae_auth_ptr)
{
    pae_auth_t *pae_auth = pae_auth_ptr;

    tr_info("Supplicant deleted");

    uint16_t active_supp = ns_list_count(&pae_auth->active_supp_list);
    if (ws_pae_auth_active_limit_reached(active_supp, pae_auth)) {
        return;
    }

    supp_entry_t *retry_supp = ns_list_get_first(&pae_auth->waiting_supp_list);
    if (retry_supp != NULL) {
        ns_list_remove(&pae_auth->waiting_supp_list, retry_supp);
        pae_auth->waiting_supp_list_size--;
        ns_list_add_to_start(&pae_auth->active_supp_list, retry_supp);
        tr_info("PAE: waiting supplicant to active, eui-64: %s", trace_array(retry_supp->addr.eui_64, 8));
        retry_supp->waiting_ticks = 0;
        ws_pae_auth_next_kmp_trigger(pae_auth, retry_supp);
    }
}

static void ws_pae_auth_waiting_supp_deleted(void *pae_auth_ptr)
{
    pae_auth_t *pae_auth = pae_auth_ptr;
    pae_auth->waiting_supp_list_size--;
}

#endif /* HAVE_PAE_AUTH */
#endif /* HAVE_WS */

