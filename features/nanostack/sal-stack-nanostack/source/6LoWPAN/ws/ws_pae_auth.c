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
#include <randLIB.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "ns_address.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_socket_if.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/key_sec_prot/key_sec_prot.h"
#include "Security/protocols/eap_tls_sec_prot/auth_eap_tls_sec_prot.h"
#include "Security/protocols/tls_sec_prot/tls_sec_prot.h"
#include "Security/protocols/fwh_sec_prot/auth_fwh_sec_prot.h"
#include "Security/protocols/gkh_sec_prot/auth_gkh_sec_prot.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "6LoWPAN/ws/ws_pae_auth.h"
#include "6LoWPAN/ws/ws_pae_lib.h"

#ifdef HAVE_WS
#ifdef HAVE_PAE_AUTH

#define TRACE_GROUP "wspa"

#define PAE_TASKLET_INIT                       1
#define PAE_TASKLET_EVENT                      2
#define PAE_TASKLET_TIMER                      3

// Wait for for supplicant to indicate activity (e.g. to send a message)
#define WAIT_FOR_AUTHENTICATION_TICKS          5 * 60 * 10  // 5 minutes

// Maximum number of simultaneous EAP-TLS negotiations
#define MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS  3

/* If EAP-TLS is delayed due to simultaneous negotiations limit, defines how
   long to wait for previous negotiation to complete */
#define EAP_TLS_NEGOTIATION_TRIGGER_TIMEOUT    60 * 10 // 60 seconds

// Default for maximum number of supplicants
#define SUPPLICANT_MAX_NUMBER                  1000

/* Default for number of supplicants to purge per garbage collect call from
   nanostack monitor */
#define SUPPLICANT_NUMBER_TO_PURGE             5

typedef struct {
    ns_list_link_t link;                                     /**< Link */
    kmp_service_t *kmp_service;                              /**< KMP service */
    protocol_interface_info_entry_t *interface_ptr;          /**< Interface pointer */
    ws_pae_auth_gtk_hash_set *hash_set;                      /**< GTK hash set callback */
    ws_pae_auth_nw_key_insert *nw_key_insert;                /**< Key insert callback */
    ws_pae_auth_nw_key_index_set *nw_key_index_set;          /**< Key index set callback */
    supp_list_t active_supp_list;                            /**< List of active supplicants */
    supp_list_t inactive_supp_list;                          /**< List of inactive supplicants */
    arm_event_storage_t *timer;                              /**< Timer */
    sec_prot_gtk_keys_t *gtks;                               /**< GTKs */
    sec_prot_gtk_keys_t *next_gtks;                          /**< Next GTKs */
    const sec_prot_certs_t *certs;                           /**< Certificates */
    timer_settings_t *timer_settings;                        /**< Timer settings */
    uint16_t supp_max_number;                                /**< Max number of stored supplicants */
    uint16_t slow_timer_seconds;                             /**< Slow timer seconds */
    bool timer_running : 1;                                  /**< Timer is running */
    bool gtk_new_inst_req_exp : 1;                           /**< GTK new install required timer expired */
    bool gtk_new_act_time_exp: 1;                            /**< GTK new activation time expired */
} pae_auth_t;

static int8_t ws_pae_auth_network_keys_from_gtks_set(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_active_gtk_set(pae_auth_t *pae_auth, uint8_t index);
static int8_t ws_pae_auth_network_key_index_set(pae_auth_t *pae_auth, uint8_t index);
static void ws_pae_auth_free(pae_auth_t *pae_auth);
static pae_auth_t *ws_pae_auth_get(protocol_interface_info_entry_t *interface_ptr);
static pae_auth_t *ws_pae_auth_by_kmp_service_get(kmp_service_t *service);
static int8_t ws_pae_auth_event_send(kmp_service_t *service, void *data);
static void ws_pae_auth_tasklet_handler(arm_event_s *event);
static void ws_pae_auth_gtk_key_insert(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_new_gtk_activate(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_timer_if_start(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_auth_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_auth_timer_start(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_timer_stop(pae_auth_t *pae_auth);
static bool ws_pae_auth_timer_running(pae_auth_t *pae_auth);
static void ws_pae_auth_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr);
static kmp_api_t *ws_pae_auth_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type);
static kmp_api_t *ws_pae_auth_kmp_incoming_ind(kmp_service_t *service, kmp_type_e type, const kmp_addr_t *addr);
static void ws_pae_auth_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result);
static void ws_pae_auth_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr);
static void ws_pae_auth_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys);
static void ws_pae_auth_next_kmp_trigger(pae_auth_t *pae_auth, supp_entry_t *supp_entry);
static kmp_type_e ws_pae_auth_next_protocol_get(supp_entry_t *supp_entry);
static kmp_api_t *ws_pae_auth_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, supp_entry_t *supp_entry);
static void ws_pae_auth_kmp_api_finished(kmp_api_t *kmp);

static int8_t tasklet_id = -1;
static NS_LIST_DEFINE(pae_auth_list, pae_auth_t, link);

int8_t ws_pae_auth_init(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks, sec_prot_gtk_keys_t *next_gtks, const sec_prot_certs_t *certs, timer_settings_t *timer_settings)
{
    if (!interface_ptr || !gtks || !certs) {
        return -1;
    }

    if (ws_pae_auth_get(interface_ptr) != NULL) {
        return 0;
    }

    pae_auth_t *pae_auth = ns_dyn_mem_alloc(sizeof(pae_auth_t));
    if (!pae_auth) {
        return -1;
    }

    pae_auth->interface_ptr = interface_ptr;
    ws_pae_lib_supp_list_init(&pae_auth->active_supp_list);
    ws_pae_lib_supp_list_init(&pae_auth->inactive_supp_list);
    pae_auth->timer = NULL;

    pae_auth->hash_set = NULL;
    pae_auth->nw_key_insert = NULL;
    pae_auth->nw_key_index_set = NULL;

    pae_auth->gtks = gtks;
    pae_auth->next_gtks = next_gtks;
    pae_auth->certs = certs;
    pae_auth->timer_settings = timer_settings;
    pae_auth->supp_max_number = SUPPLICANT_MAX_NUMBER;

    pae_auth->slow_timer_seconds = 0;
    pae_auth->gtk_new_inst_req_exp = false;
    pae_auth->gtk_new_act_time_exp = false;

    pae_auth->kmp_service = kmp_service_create();
    if (!pae_auth->kmp_service) {
        goto error;
    }

    if (kmp_service_cb_register(pae_auth->kmp_service, ws_pae_auth_kmp_incoming_ind, ws_pae_auth_kmp_service_addr_get, ws_pae_auth_kmp_service_api_get)) {
        goto error;
    }

    if (kmp_service_event_if_register(pae_auth->kmp_service, ws_pae_auth_event_send)) {
        goto error;
    }

    if (kmp_service_timer_if_register(pae_auth->kmp_service, ws_pae_auth_timer_if_start, ws_pae_auth_timer_if_stop)) {
        goto error;
    }

    if (key_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

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

int8_t ws_pae_auth_timing_adjust(uint8_t timing)
{
    auth_gkh_sec_prot_timing_adjust(timing);
    auth_fwh_sec_prot_timing_adjust(timing);
    auth_eap_tls_sec_prot_timing_adjust(timing);
    return 0;
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

    if (kmp_socket_if_register(pae_auth->kmp_service, local_port, remote_addr, remote_port) < 0) {
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

void ws_pae_auth_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_auth_gtk_hash_set *hash_set, ws_pae_auth_nw_key_insert *nw_key_insert, ws_pae_auth_nw_key_index_set *nw_key_index_set)
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
    int8_t index = sec_prot_keys_gtk_status_active_get(pae_auth->gtks);
    if (index < 0) {
        // If there is no key, inserts a new one
        ws_pae_auth_gtk_key_insert(pae_auth);
        index = sec_prot_keys_gtk_install_order_first_index_get(pae_auth->gtks);
        ws_pae_auth_active_gtk_set(pae_auth, index);
    } else {
        ws_pae_auth_active_gtk_set(pae_auth, index);
    }

    // Inserts keys and updates GTK hash on stack
    ws_pae_auth_network_keys_from_gtks_set(pae_auth);

    // Sets active key index
    ws_pae_auth_network_key_index_set(pae_auth, index);
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

    ws_pae_auth_network_keys_from_gtks_set(pae_auth);
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
    if (!interface_ptr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    // Checks if supplicant is active
    supp_entry_t *supp = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->active_supp_list, eui_64);

    if (supp) {
        // Deletes keys and marks as revoked
        sec_prot_keys_pmk_delete(&supp->sec_keys);
        sec_prot_keys_ptk_delete(&supp->sec_keys);
        supp->access_revoked = true;
        tr_info("Access revoked; keys removed, eui-64: %s", trace_array(supp->addr.eui_64, 8));
        return 0;
    }

    // Checks if supplicant is inactive
    supp = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->inactive_supp_list, eui_64);
    if (supp) {
        // Deletes supplicant
        tr_info("Access revoked; deleted, eui-64: %s", trace_array(supp->addr.eui_64, 8));
        ws_pae_lib_supp_list_remove(&pae_auth->inactive_supp_list, supp);
        return 0;
    }

    return -1;
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
    int8_t active_index = sec_prot_keys_gtk_status_active_get(pae_auth->gtks);

    if (active_index >= 0) {
        // As default removes other keys than active
        int8_t not_removed_index = active_index;

        uint32_t revocation_lifetime = ws_pae_timers_gtk_revocation_lifetime_get(pae_auth->timer_settings);

        uint32_t active_lifetime = sec_prot_keys_gtk_lifetime_get(pae_auth->gtks, active_index);

        // If active GTK lifetime is larger than revocation lifetime decrements active GTK lifetime
        if (active_lifetime > revocation_lifetime) {
            sec_prot_keys_gtk_lifetime_decrement(pae_auth->gtks, active_index, active_lifetime - revocation_lifetime);
            tr_info("Access revocation start, GTK active index: %i, revoked lifetime: %"PRIu32"", active_index, revocation_lifetime);
        } else {
            // Otherwise decrements lifetime of the GTK to be installed after the active one
            int8_t second_index = sec_prot_keys_gtk_install_order_second_index_get(pae_auth->gtks);
            if (second_index >= 0) {
                // Second GTK revocation lifetime is the active GTK lifetime added with revocation time
                uint32_t second_revocation_lifetime = active_lifetime + revocation_lifetime;

                uint32_t second_lifetime = sec_prot_keys_gtk_lifetime_get(pae_auth->gtks, second_index);
                if (second_lifetime > second_revocation_lifetime) {
                    sec_prot_keys_gtk_lifetime_decrement(pae_auth->gtks, second_index, second_lifetime - second_revocation_lifetime);
                    tr_info("Access revocation start, GTK second active index: %i, revoked lifetime: %"PRIu32"", second_index, second_revocation_lifetime);
                }
                // Removes other keys than active and GTK to be installed next
                not_removed_index = second_index;
            }
        }

        // Deletes other GTKs
        int8_t last_index = sec_prot_keys_gtk_install_order_last_index_get(pae_auth->gtks);
        while (last_index >= 0 && last_index != not_removed_index) {
            tr_info("Access revocation GTK clear index: %i", last_index);
            sec_prot_keys_gtk_clear(pae_auth->gtks, last_index);
            last_index = sec_prot_keys_gtk_install_order_last_index_get(pae_auth->gtks);
        }
    }

    // Adds new GTK
    ws_pae_auth_gtk_key_insert(pae_auth);
    ws_pae_auth_network_keys_from_gtks_set(pae_auth);

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
    ws_pae_lib_supp_list_purge(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, 0, SUPPLICANT_NUMBER_TO_PURGE);
}

static int8_t ws_pae_auth_network_keys_from_gtks_set(pae_auth_t *pae_auth)
{
    // Authenticator keys are always fresh
    sec_prot_keys_gtk_status_all_fresh_set(pae_auth->gtks);

    if (pae_auth->hash_set) {
        uint8_t gtk_hash[32];
        sec_prot_keys_gtks_hash_generate(pae_auth->gtks, gtk_hash);
        pae_auth->hash_set(pae_auth->interface_ptr, gtk_hash);
    }

    if (pae_auth->nw_key_insert) {
        pae_auth->nw_key_insert(pae_auth->interface_ptr, pae_auth->gtks);
    }

    return 0;
}

static int8_t ws_pae_auth_active_gtk_set(pae_auth_t *pae_auth, uint8_t index)
{
    return sec_prot_keys_gtk_status_active_set(pae_auth->gtks, index);
}

static int8_t ws_pae_auth_gtk_clear(pae_auth_t *pae_auth, uint8_t index)
{
    return sec_prot_keys_gtk_clear(pae_auth->gtks, index);
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

    ws_pae_lib_supp_list_delete(&pae_auth->active_supp_list);
    ws_pae_lib_supp_list_delete(&pae_auth->inactive_supp_list);

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
        bool running = ws_pae_lib_supp_list_timer_update(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, ticks, kmp_service_timer_if_timeout);
        if (!running) {
            ws_pae_auth_timer_stop(pae_auth);
        }
    }
}

void ws_pae_auth_slow_timer(uint16_t seconds)
{
    ns_list_foreach(pae_auth_t, pae_auth, &pae_auth_list) {

        // Gets index of currently active GTK
        int8_t active_index = sec_prot_keys_gtk_status_active_get(pae_auth->gtks);

        for (uint8_t i = 0; i < GTK_NUM; i++) {
            if (!sec_prot_keys_gtk_is_set(pae_auth->gtks, i)) {
                continue;
            }
            uint32_t timer_seconds = sec_prot_keys_gtk_lifetime_decrement(pae_auth->gtks, i, seconds);
            if (active_index == i) {
                if (!pae_auth->gtk_new_inst_req_exp) {
                    pae_auth->gtk_new_inst_req_exp = ws_pae_timers_gtk_new_install_required(pae_auth->timer_settings, timer_seconds);
                    if (pae_auth->gtk_new_inst_req_exp) {
                        int8_t second_index = sec_prot_keys_gtk_install_order_second_index_get(pae_auth->gtks);
                        if (second_index < 0) {
                            tr_info("GTK new install required active index: %i, time: %"PRIu32", system time: %"PRIu32"", active_index, timer_seconds, protocol_core_monotonic_time / 10);
                            ws_pae_auth_gtk_key_insert(pae_auth);
                            ws_pae_auth_network_keys_from_gtks_set(pae_auth);
                        } else {
                            tr_info("GTK new install already done; second index: %i, time: %"PRIu32", system time: %"PRIu32"", second_index, timer_seconds, protocol_core_monotonic_time / 10);
                        }
                    }
                }

                if (!pae_auth->gtk_new_act_time_exp) {
                    pae_auth->gtk_new_act_time_exp =  ws_pae_timers_gtk_new_activation_time(pae_auth->timer_settings, timer_seconds);
                    if (pae_auth->gtk_new_act_time_exp) {
                        int8_t new_active_index = ws_pae_auth_new_gtk_activate(pae_auth);
                        tr_info("GTK new activation time active index: %i, time: %"PRIu32", new index: %i, system time: %"PRIu32"", active_index, timer_seconds, new_active_index, protocol_core_monotonic_time / 10);
                        if (new_active_index >= 0) {
                            ws_pae_auth_network_key_index_set(pae_auth, new_active_index);
                        }
                        pae_auth->gtk_new_inst_req_exp = false;
                        pae_auth->gtk_new_act_time_exp = false;
                    }
                }
            }

            if (timer_seconds == 0) {
                tr_info("GTK expired index: %i, system time: %"PRIu32"", i, protocol_core_monotonic_time / 10);
                ws_pae_auth_gtk_clear(pae_auth, i);
                ws_pae_auth_network_keys_from_gtks_set(pae_auth);
            }
        }

        pae_auth->slow_timer_seconds += seconds;
        if (pae_auth->slow_timer_seconds > 60) {
            ws_pae_lib_supp_list_slow_timer_update(&pae_auth->active_supp_list, pae_auth->timer_settings, pae_auth->slow_timer_seconds);
            ws_pae_lib_supp_list_slow_timer_update(&pae_auth->inactive_supp_list, pae_auth->timer_settings, pae_auth->slow_timer_seconds);
            pae_auth->slow_timer_seconds = 0;
        }
    }
}

static void ws_pae_auth_gtk_key_insert(pae_auth_t *pae_auth)
{
    // Gets index to install the key
    uint8_t install_index = sec_prot_keys_gtk_install_index_get(pae_auth->gtks);

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
        randLIB_get_n_bytes_random(gtk_value, GTK_LEN);
    }

    // Gets latest installed key lifetime and adds GTK expire offset to it
    uint32_t lifetime = pae_auth->timer_settings->gtk_expire_offset;
    int8_t last_index = sec_prot_keys_gtk_install_order_last_index_get(pae_auth->gtks);
    if (last_index >= 0) {
        lifetime += sec_prot_keys_gtk_lifetime_get(pae_auth->gtks, last_index);
    }

    // Installs the new key
    sec_prot_keys_gtk_clear(pae_auth->gtks, install_index);
    sec_prot_keys_gtk_set(pae_auth->gtks, install_index, gtk_value, lifetime);

    // Authenticator keys are always fresh
    sec_prot_keys_gtk_status_all_fresh_set(pae_auth->gtks);

    tr_info("GTK install new index: %i, lifetime: %"PRIu32" system time: %"PRIu32"", install_index, lifetime, protocol_core_monotonic_time / 10);
}

static int8_t ws_pae_auth_new_gtk_activate(pae_auth_t *pae_auth)
{
    int8_t new_active_index = sec_prot_keys_gtk_install_order_second_index_get(pae_auth->gtks);
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

    ws_pae_lib_supp_list_to_active(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, supp_entry);

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

static kmp_api_t *ws_pae_auth_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type)
{
    (void) service;

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        return NULL;
    }

    return ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, type);
}

static kmp_api_t *ws_pae_auth_kmp_incoming_ind(kmp_service_t *service, kmp_type_e type, const kmp_addr_t *addr)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return NULL;
    }

    // Find supplicant from list of active supplicants
    supp_entry_t *supp_entry = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->active_supp_list, kmp_address_eui_64_get(addr));

    if (!supp_entry) {
        // Find supplicant from list of inactive supplicants
        supp_entry = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->inactive_supp_list, kmp_address_eui_64_get(addr));
        if (supp_entry) {
            // Move supplicant to active list
            ws_pae_lib_supp_list_to_active(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, supp_entry);
        }
    }

    // If does not exists add it to list
    if (!supp_entry) {
        // Checks if maximum number of supplicants is reached and purge supplicant list (starting from oldest one)
        ws_pae_lib_supp_list_purge(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, pae_auth->supp_max_number, 0);

        supp_entry = ws_pae_lib_supp_list_add(&pae_auth->active_supp_list, addr);
        if (!supp_entry) {
            return 0;
        }
        sec_prot_keys_init(&supp_entry->sec_keys, pae_auth->gtks, pae_auth->certs);
        // Fixes the address of the supplicant to keys
        sec_prot_keys_ptk_eui_64_write(&supp_entry->sec_keys, kmp_address_eui_64_get(addr));
    } else {
        // Updates relay address
        kmp_address_copy(&supp_entry->addr, addr);
    }

    // Increases waiting time for supplicant authentication
    ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_FOR_AUTHENTICATION_TICKS);

    // Get KMP for supplicant
    kmp_api_t *kmp = ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, type);
    if (kmp) {
        return kmp;
    }

    // Create a new KMP for initial eapol-key
    kmp = kmp_api_create(service, type + IEEE_802_1X_INITIAL_KEY);

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

static void ws_pae_auth_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys)
{
    (void) sec_keys;

    // For now, just ignore if not ok
    if (result != KMP_RESULT_OK) {
        return;
    }

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        // Should not be possible
        return;
    }
    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        // Should not be possible
        return;
    }

    ws_pae_auth_next_kmp_trigger(pae_auth, supp_entry);
}

static void ws_pae_auth_next_kmp_trigger(pae_auth_t *pae_auth, supp_entry_t *supp_entry)
{
    // Disables KMP retry timer
    supp_entry->retry_ticks = 0;

    // Get next protocol based on what keys supplicant has
    kmp_type_e next_type = ws_pae_auth_next_protocol_get(supp_entry);

    if (next_type == KMP_TYPE_NONE) {
        // All done
        return;
    } else {

        kmp_api_t *api = ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, next_type);
        if (api != NULL) {
            if (next_type != IEEE_802_11_GKH) {
                tr_info("KMP already ongoing; ignored, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
                return;
            }
            // Delete KMP
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, api);
        }
    }

    // Increases waiting time for supplicant authentication
    ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_FOR_AUTHENTICATION_TICKS);

    if (next_type == IEEE_802_1X_MKA) {
        /* For EAP-TLS, limits the number of ongoing negotiations. If limit
           is reached, authenticator does not initiate EAP-TLS right away.
           If previous EAP-TLS negotiation completes before negotiation
           trigger timeout, authenticator initiates EAP-TLS towards
           supplicant. Otherwise supplicant must re-send initial EAPOL-Key
           to try again using its trickle schedule */
        uint16_t ongoing_eap_tls_cnt = ws_pae_lib_supp_list_kmp_count(&pae_auth->active_supp_list, IEEE_802_1X_MKA);
        if (ongoing_eap_tls_cnt >= MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS) {
            supp_entry->retry_ticks = EAP_TLS_NEGOTIATION_TRIGGER_TIMEOUT;
            tr_info("EAP-TLS max ongoing reached, count %i, delayed: eui-64: %s", ongoing_eap_tls_cnt, trace_array(supp_entry->addr.eui_64, 8));
            return;
        }
    }

    // Create new instance
    kmp_api_t *new_kmp = ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, next_type, supp_entry);
    if (!new_kmp) {
        return;
    }

    // For EAP-TLS create also TLS in addition to EAP-TLS
    if (next_type == IEEE_802_1X_MKA) {
        if (ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, TLS_PROT) != NULL) {
            // TLS already exists, wait for it to be deleted
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return;
        }
        // Create TLS instance */
        if (ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, TLS_PROT, supp_entry) == NULL) {
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return;
        }
    }

    kmp_api_create_request(new_kmp, next_type, &supp_entry->addr, &supp_entry->sec_keys);
}

static kmp_type_e ws_pae_auth_next_protocol_get(supp_entry_t *supp_entry)
{
    kmp_type_e next_type = KMP_TYPE_NONE;
    sec_prot_keys_t *sec_keys = &supp_entry->sec_keys;

    // Supplicant has indicated that PMK is not valid
    if (sec_keys->pmk_mismatch) {
        sec_keys->ptk_mismatch = true;
        // start EAP-TLS towards supplicant
        next_type = IEEE_802_1X_MKA;
        tr_info("PAE start EAP-TLS, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
    } else if (sec_keys->ptk_mismatch) {
        // start 4WH towards supplicant
        next_type = IEEE_802_11_4WH;
        tr_info("PAE start 4WH, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
    }

    int8_t gtk_index = -1;
    if (next_type != IEEE_802_1X_MKA) {
        // Checks if GTK needs to be inserted
        gtk_index = sec_prot_keys_gtk_insert_index_from_gtkl_get(sec_keys);

        // For 4WH insert always a key, in case no other then active
        if (next_type == IEEE_802_11_4WH && gtk_index < 0) {
            gtk_index = sec_prot_keys_gtk_status_active_get(sec_keys->gtks);
        }
    }

    if (gtk_index >= 0) {
        if (next_type == KMP_TYPE_NONE && gtk_index >= 0) {
            // Update just GTK
            next_type = IEEE_802_11_GKH;
            tr_info("PAE start GKH, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
        }

        tr_info("PAE update GTK index: %i, eui-64: %s", gtk_index, trace_array(supp_entry->addr.eui_64, 8));
    }

    if (next_type == KMP_TYPE_NONE) {
        tr_info("PAE authenticated, eui-64: %s", trace_array(supp_entry->addr.eui_64, 8));
    }

    return next_type;
}


static kmp_api_t *ws_pae_auth_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, supp_entry_t *supp_entry)
{
    // Create KMP instance for new authentication
    kmp_api_t *kmp = kmp_api_create(service, type);

    if (!kmp) {
        return NULL;
    }

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

    pae_auth_t *pae_auth = NULL;
    supp_entry_t *retry_supp = NULL;
    // When EAP-TLS completes check if there are other supplicants that have requested it lately
    if (kmp_api_type_get(kmp) == IEEE_802_1X_MKA) {
        kmp_service_t *service = kmp_api_service_get(kmp);
        pae_auth = ws_pae_auth_by_kmp_service_get(service);
        if (pae_auth) {
            retry_supp = ws_pae_lib_supp_list_entry_retry_timer_get(&pae_auth->active_supp_list);
        }
    }

    // Delete KMP
    ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);

    if (retry_supp) {
        tr_info("PAE next KMP trigger, eui-64: %s", trace_array(retry_supp->addr.eui_64, 8));
        ws_pae_auth_next_kmp_trigger(pae_auth, retry_supp);
    }

}

#endif /* HAVE_PAE_AUTH */
#endif /* HAVE_WS */

