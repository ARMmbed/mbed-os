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
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/key_sec_prot/key_sec_prot.h"
#include "Security/protocols/eap_tls_sec_prot/supp_eap_tls_sec_prot.h"
#include "Security/protocols/tls_sec_prot/tls_sec_prot.h"
#include "Security/protocols/fwh_sec_prot/supp_fwh_sec_prot.h"
#include "Security/protocols/gkh_sec_prot/supp_gkh_sec_prot.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "6LoWPAN/ws/ws_pae_supp.h"
#include "6LoWPAN/ws/ws_pae_lib.h"
#include "6LoWPAN/ws/ws_pae_time.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"
#include "6LoWPAN/ws/ws_pae_nvm_data.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"
#include "Security/kmp/kmp_eapol_pdu_if.h"

#ifdef HAVE_WS
#ifdef HAVE_PAE_SUPP

#define TRACE_GROUP "wsps"

#define PAE_TASKLET_INIT                       1
#define PAE_TASKLET_EVENT                      2
#define PAE_TASKLET_TIMER                      3

// Wait for for authenticator to continue with authentication (e.g. after EAP-TLS to initiate 4WH)
#define WAIT_FOR_AUTHENTICATION_TICKS          30 * 10      // 30 seconds

// Wait for re-authentication after GTK update
#define WAIT_FOR_REAUTHENTICATION_TICKS        120 * 10     // 120 seconds

// Ticks added to wait for authenticator timer when authentication protocol is started (e.g. EAP-TLS)
#define START_AUTHENTICATION_TICKS             5 * 10       // 10 seconds

// How many times in maximum stored keys are used for authentication
#define STORED_KEYS_MAXIMUM_USE_COUNT          1

// Delay for sending the initial EAPOL-Key
#define INITIAL_KEY_TIMER_MIN                  1
#define INITIAL_KEY_TIMER_MAX                  3

typedef struct {
    ns_list_link_t link;                                   /**< Link */
    kmp_service_t *kmp_service;                            /**< KMP service */
    protocol_interface_info_entry_t *interface_ptr;        /**< Interface */
    ws_pae_supp_auth_completed *auth_completed;            /**< Authentication completed callback, continue bootstrap */
    ws_pae_supp_auth_next_target *auth_next_target;        /**< Authentication next target callback */
    ws_pae_supp_nw_key_insert *nw_key_insert;              /**< Key insert callback */
    ws_pae_supp_nw_key_index_set *nw_key_index_set;        /**< Key index set callback */
    ws_pae_supp_gtk_hash_ptr_get *gtk_hash_ptr_get;        /**< Get pointer to GTK hash storage callback */
    ws_pae_supp_nw_info_updated *nw_info_updated;          /**< Security keys network info updated callback */
    supp_entry_t entry;                                    /**< Supplicant data */
    kmp_addr_t target_addr;                                /**< EAPOL target (parent) address */
    uint16_t initial_key_timer;                            /**< Timer to trigger initial EAPOL-Key */
    trickle_t gtk_req_trickle_timer;                       /**< Trickle timer for re-sending initial EAPOL-key on GTK mismatch */
    trickle_params_t gtk_req_trickle_params;               /**< Trickle parameters for initial EAPOL-key on GTK mismatch */
    uint8_t new_br_eui_64[8];                              /**< Border router EUI-64 indicated by bootstrap after bootstrap start */
    uint8_t comp_br_eui_64[8];                             /**< Border router EUI-64 indicated by bootstrap after bootstrap completed */
    sec_prot_keys_nw_info_t *sec_keys_nw_info;             /**< Security keys network information */
    sec_cfg_t *sec_cfg;                                    /**< Security configuration */
    uint8_t nw_keys_used_cnt;                              /**< How many times bootstrap has been tried with current keys */
    uint16_t initial_key_retry_max_value;                  /**< Initial EAPOL-Key retry exponential backoff max value */
    uint16_t initial_key_retry_timer;                      /**< Initial EAPOL-Key retry exponential backoff timer */
    uint8_t initial_key_retry_cnt;                         /**< initial EAPOL-Key retry counter */
    bool gtk_update_trickle_running : 1;                   /**< GTK update Initial EAPOL-Key Trickle timer running */
    bool auth_requested : 1;                               /**< Authentication has been requested by the bootstrap */
    bool timer_running : 1;                                /**< Timer is running */
    bool new_br_eui_64_set : 1;                            /**< Border router address has been set after bootstrap start */
    bool new_br_eui_64_fresh : 1;                          /**< Border router address is fresh (set during this authentication attempt) */
    bool comp_br_eui_64_set : 1;                           /**< Border router address has been set after bootstrap completed */
    bool entry_address_active: 1;                          /**< EAPOL target address is set */
    bool tx_failure_on_initial_key: 1;                     /**< TX failure has happened on initial EAPOL-key sequence */
} pae_supp_t;

// How many times sending of initial EAPOL-key is initiated on key update
#define KEY_UPDATE_RETRY_COUNT             3
#define LIFETIME_MISMATCH_RETRY_COUNT      1   /* No retries */

// Timer on how long to wait response after last retry before failing authentication
#define LAST_INTERVAL_MIN_SECS             120  /* 2 minutes */
#define LAST_INTERVAL_MAX_SECS             240  /* 4 minutes */

static void ws_pae_supp_free(pae_supp_t *pae_supp);
static void ws_pae_supp_authenticate_response(pae_supp_t *pae_supp, auth_result_e result);
static int8_t ws_pae_supp_initial_key_send(pae_supp_t *pae_supp);
static void ws_pae_supp_nvm_update(pae_supp_t *pae_supp);
static int8_t ws_pae_supp_network_name_compare(char *name1, char *name2);
static int8_t ws_pae_supp_nw_keys_valid_check(pae_supp_t *pae_supp, uint16_t pan_id, char *dest_network_name);
static int8_t ws_pae_supp_nvm_keys_write(pae_supp_t *pae_supp);
static pae_supp_t *ws_pae_supp_get(protocol_interface_info_entry_t *interface_ptr);
static int8_t ws_pae_supp_event_send(kmp_service_t *service, void *data);
static void ws_pae_supp_tasklet_handler(arm_event_s *event);
static void ws_pae_supp_initial_key_update_trickle_timer_start(pae_supp_t *pae_supp, uint8_t timer_expirations);
static bool ws_pae_supp_authentication_ongoing(pae_supp_t *pae_supp);
static int8_t ws_pae_supp_timer_if_start(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_supp_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_supp_timer_start(pae_supp_t *pae_supp);
static int8_t ws_pae_supp_timer_stop(pae_supp_t *pae_supp);
static bool ws_pae_supp_timer_running(pae_supp_t *pae_supp);
static void ws_pae_supp_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr);
static kmp_api_t *ws_pae_supp_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type);
static kmp_api_t *ws_pae_supp_kmp_incoming_ind(kmp_service_t *service, uint8_t instance_id, kmp_type_e type, const kmp_addr_t *addr, const void *pdu, uint16_t size);
static kmp_api_t *ws_pae_supp_kmp_tx_status_ind(kmp_service_t *service, uint8_t instance_id);
static kmp_api_t *ws_pae_supp_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, pae_supp_t *pae_supp);
static int8_t ws_pae_supp_eapol_pdu_address_check(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);
static int8_t ws_pae_supp_parent_eui_64_get(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64);
static int8_t ws_pae_supp_gtk_hash_mismatch_check(pae_supp_t *pae_supp);

static void ws_pae_supp_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result);
static void ws_pae_supp_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr);
static bool ws_pae_supp_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys);
static void ws_pae_supp_kmp_api_finished(kmp_api_t *kmp);


static const eapol_pdu_recv_cb_data_t eapol_pdu_recv_cb_data = {
    .priority = EAPOL_PDU_RECV_HIGH_PRIORITY,
    .filter_requsted = false,
    .addr_check = ws_pae_supp_eapol_pdu_address_check,
    .receive = kmp_eapol_pdu_if_receive
};

static const char *KEYS_FILE = KEYS_FILE_NAME;

static int8_t tasklet_id = -1;
static NS_LIST_DEFINE(pae_supp_list, pae_supp_t, link);

static void ws_pae_supp_address_set(pae_supp_t *pae_supp, kmp_addr_t *address)
{
    if (address) {
        // Sets target address in use
        pae_supp->entry.addr = *address;
        pae_supp->entry_address_active = true;
    } else {
        pae_supp->entry_address_active = false;
    }
}

static bool ws_pae_supp_address_is_set(pae_supp_t *pae_supp)
{
    return pae_supp->entry_address_active;
}

int8_t ws_pae_supp_authenticate(protocol_interface_info_entry_t *interface_ptr, uint16_t dest_pan_id, uint8_t *dest_eui_64, char *dest_network_name)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    if (ws_pae_supp_nw_keys_valid_check(pae_supp, dest_pan_id, dest_network_name) >= 0) {
        pae_supp->auth_completed(interface_ptr, AUTH_RESULT_OK, NULL);
        return 0;
    }

    // Delete GTKs
    sec_prot_keys_gtks_init(pae_supp->sec_keys_nw_info->gtks);

    /* Network name or PAN ID has changed, delete key data associated with border router
       i.e PMK, PTK, EA-IE data (border router EUI-64) */
    if (ws_pae_supp_network_name_compare(pae_supp->sec_keys_nw_info->network_name, dest_network_name) != 0 ||
            (pae_supp->sec_keys_nw_info->key_pan_id != 0xFFFF && pae_supp->sec_keys_nw_info->key_pan_id != dest_pan_id)) {
        sec_prot_keys_pmk_delete(&pae_supp->entry.sec_keys);
        sec_prot_keys_ptk_delete(&pae_supp->entry.sec_keys);
        sec_prot_keys_ptk_eui_64_delete(&pae_supp->entry.sec_keys);
    }

    pae_supp->sec_keys_nw_info->key_pan_id = dest_pan_id;

    // Prepare to receive new border router address
    pae_supp->new_br_eui_64_fresh = false;
    pae_supp->comp_br_eui_64_set = false;

    // Stores target/parent address
    kmp_address_init(KMP_ADDR_EUI_64, &pae_supp->target_addr, dest_eui_64);
    // Sets target address in use
    ws_pae_supp_address_set(pae_supp, &pae_supp->target_addr);

    pae_supp->auth_requested = true;

    // Randomizes the sending of initial EAPOL-Key message
    pae_supp->initial_key_timer = randLIB_get_random_in_range(INITIAL_KEY_TIMER_MIN, INITIAL_KEY_TIMER_MAX);

    // Starts supplicant timer
    ws_pae_supp_timer_start(pae_supp);

    tr_debug("PAE active, timer %i", pae_supp->initial_key_timer);

    return 1;
}

int8_t ws_pae_supp_border_router_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    memcpy(pae_supp->new_br_eui_64, eui_64, 8);
    pae_supp->new_br_eui_64_set = true;
    pae_supp->new_br_eui_64_fresh = true;

    return 0;
}

int8_t ws_pae_supp_border_router_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    // Check if there is border router EUI-64 on used on 4WH PTK generation
    uint8_t *br_eui_64 = sec_prot_keys_ptk_eui_64_get(&pae_supp->entry.sec_keys);
    if (!br_eui_64) {
        // Check if there is border router EUI-64 indicated by the bootstrap when bootstrap completed
        if (!pae_supp->comp_br_eui_64_set) {
            return -1;
        }
        br_eui_64 = pae_supp->comp_br_eui_64;
    }

    memcpy(eui_64, br_eui_64, 8);

    return 0;
}

int8_t ws_pae_supp_nw_key_valid(protocol_interface_info_entry_t *interface_ptr, uint8_t *br_iid)
{
    (void) br_iid;

    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    tr_info("NW key valid indication");

    // Stored keys are valid
    pae_supp->nw_keys_used_cnt = 0;

    // Update NVM if data has been changed
    ws_pae_supp_nvm_update(pae_supp);

    return 0;
}

static int8_t ws_pae_supp_gtk_hash_mismatch_check(pae_supp_t *pae_supp)
{
    uint8_t *gtkhash = pae_supp->gtk_hash_ptr_get(pae_supp->interface_ptr);
    if (!gtkhash) {
        return -1;
    }

    // Check GTK hashes and initiate EAPOL procedure if mismatch is detected */
    gtk_mismatch_e mismatch = sec_prot_keys_gtks_hash_update(pae_supp->sec_keys_nw_info->gtks, gtkhash, false);
    if (mismatch != GTK_NO_MISMATCH) {
        return -1;
    }

    tr_info("GTKs match to GTK hash");
    return 0;
}

int8_t ws_pae_supp_gtk_hash_update(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash, bool del_gtk_on_mismatch)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    // Check GTK hashes and initiate EAPOL procedure if mismatch is detected */
    gtk_mismatch_e mismatch = sec_prot_keys_gtks_hash_update(pae_supp->sec_keys_nw_info->gtks, gtkhash, del_gtk_on_mismatch);
    if (mismatch > GTK_NO_MISMATCH) {
        tr_info("GTK hash update %s %s %s %s",
                trace_array(&gtkhash[0], 8),
                trace_array(&gtkhash[8], 8),
                trace_array(&gtkhash[16], 8),
                trace_array(&gtkhash[24], 8));

        /* Mismatch, initiate EAPOL (if authentication not already ongoing or if not on
           wait time for the authenticator to answer) */
        if (!pae_supp->gtk_update_trickle_running || pae_supp->initial_key_retry_cnt == 0) {
            uint8_t timer_expirations = KEY_UPDATE_RETRY_COUNT;
            // For GTK lifetime mismatch send only once
            if (mismatch == GTK_LIFETIME_MISMATCH) {
                timer_expirations = LIFETIME_MISMATCH_RETRY_COUNT;
            }
            // Start trickle timer
            ws_pae_supp_initial_key_update_trickle_timer_start(pae_supp, timer_expirations);

            // Starts supplicant timer
            ws_pae_supp_timer_start(pae_supp);

            tr_info("GTK update start imin: %i, imax: %i, max mismatch: %i, tr time: %i", pae_supp->sec_cfg->timer_cfg.gtk_request_imin, pae_supp->sec_cfg->timer_cfg.gtk_request_imax, pae_supp->sec_cfg->timer_cfg.gtk_max_mismatch, pae_supp->gtk_req_trickle_timer.t);
        }
    }

    // Modify keys
    pae_supp->nw_key_insert(pae_supp->interface_ptr, pae_supp->sec_keys_nw_info->gtks, false);

    return 0;
}

int8_t ws_pae_supp_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    if (sec_prot_keys_gtk_status_active_set(pae_supp->sec_keys_nw_info->gtks, index) >= 0) {
        pae_supp->nw_key_index_set(interface_ptr, index);
    } else {
        tr_info("NW send key index: %i, no changes", index + 1);
    }

    return 0;
}

int8_t ws_pae_supp_gtks_set(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    *pae_supp->sec_keys_nw_info->gtks = *gtks;

    return 0;
}

int8_t ws_pae_supp_eapol_target_remove(protocol_interface_info_entry_t *interface_ptr)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    // Sets target/parent address to null
    ws_pae_supp_address_set(pae_supp, NULL);

    return 0;
}

static void ws_pae_supp_nvm_update(pae_supp_t *pae_supp)
{
    // Indicate to PAE controller that NW info or GTKs may have been changed
    pae_supp->nw_info_updated(pae_supp->interface_ptr);

    // Check if pairwise security keys have been changed
    if (sec_prot_keys_are_updated(&pae_supp->entry.sec_keys)) {
        ws_pae_supp_nvm_keys_write(pae_supp);
        sec_prot_keys_updated_reset(&pae_supp->entry.sec_keys);
    }
}

static int8_t ws_pae_supp_nvm_keys_write(pae_supp_t *pae_supp)
{
    keys_nvm_tlv_t *tlv = (keys_nvm_tlv_t *) ws_pae_controller_nvm_tlv_get(pae_supp->interface_ptr);
    if (!tlv) {
        return -1;
    }

    ws_pae_nvm_store_keys_tlv_create(tlv, &pae_supp->entry.sec_keys);
    ws_pae_nvm_store_tlv_file_write(KEYS_FILE, (nvm_tlv_t *) tlv);

    return 0;
}

static int8_t ws_pae_supp_nvm_keys_read(pae_supp_t *pae_supp)
{
    keys_nvm_tlv_t *tlv = (keys_nvm_tlv_t *) ws_pae_controller_nvm_tlv_get(pae_supp->interface_ptr);
    if (!tlv) {
        return -1;
    }
    ws_pae_nvm_store_generic_tlv_create((nvm_tlv_t *) tlv, PAE_NVM_KEYS_TAG, PAE_NVM_KEYS_LEN);
    if (ws_pae_nvm_store_tlv_file_read(KEYS_FILE_NAME, (nvm_tlv_t *) tlv) < 0) {
        return -1;
    }
    ws_pae_nvm_store_keys_tlv_read(tlv, &pae_supp->entry.sec_keys);
    return 0;
}

static void ws_pae_supp_authenticate_response(pae_supp_t *pae_supp, auth_result_e result)
{
    pae_supp->initial_key_retry_timer = 0;
    if (pae_supp->auth_requested && pae_supp->auth_completed) {
        pae_supp->auth_requested = false;
        pae_supp->auth_completed(pae_supp->interface_ptr, result, pae_supp->target_addr.eui_64);
    }
}

static int8_t ws_pae_supp_initial_key_send(pae_supp_t *pae_supp)
{
    if (!pae_supp->auth_requested) {
        // If not making initial authentication updates target (RPL parent) for each EAPOL-key message
        uint8_t parent_eui_64[8];
        if (ws_pae_supp_parent_eui_64_get(pae_supp->interface_ptr, parent_eui_64) >= 0) {
            // Stores target/parent address
            kmp_address_init(KMP_ADDR_EUI_64, &pae_supp->target_addr, parent_eui_64);
            // Sets parent address in use
            ws_pae_supp_address_set(pae_supp, &pae_supp->target_addr);
        } else if (ws_pae_supp_address_is_set(pae_supp)) {
            /* If there is no RPL parent but there is target address from initial authentication
               bootstrap, tries to use it. This can happen if BR updates keys after EAPOL authentication
               but before bootstrap is completed and RPL parent is known */
            tr_info("EAPOL initial auth target used");
        } else {
            // No target, failure
            return -1;
        }

        ws_pae_lib_supp_timer_ticks_set(&pae_supp->entry, WAIT_FOR_REAUTHENTICATION_TICKS);
        tr_info("PAE wait for auth seconds: %i", WAIT_FOR_REAUTHENTICATION_TICKS / 10);
    }

    kmp_api_t *kmp = ws_pae_supp_kmp_create_and_start(pae_supp->kmp_service, IEEE_802_1X_MKA_KEY, pae_supp);
    if (!kmp) {
        return -1;
    }

    tr_info("EAPOL target: %s", trace_array(kmp_address_eui_64_get(&pae_supp->entry.addr), 8));

    kmp_api_create_request(kmp, IEEE_802_1X_MKA_KEY, &pae_supp->entry.addr, &pae_supp->entry.sec_keys);

    return 0;
}

static int8_t ws_pae_supp_network_name_compare(char *name1, char *name2)
{
    if (strlen(name1) == strlen(name2) && strcmp(name1, name2) == 0) {
        return 0;
    }
    return -1;
}

static int8_t ws_pae_supp_nw_keys_valid_check(pae_supp_t *pae_supp, uint16_t pan_id, char *dest_network_name)
{
    // Checks how many times authentication has been tried with current network keys
    if (pae_supp->nw_keys_used_cnt >= STORED_KEYS_MAXIMUM_USE_COUNT) {
        tr_debug("Keys not valid, delete GTKs");

        // Delete GTKs
        sec_prot_keys_gtks_init(pae_supp->sec_keys_nw_info->gtks);
        sec_prot_keys_gtks_updated_set(pae_supp->sec_keys_nw_info->gtks);
        ws_pae_supp_nvm_update(pae_supp);

        pae_supp->nw_keys_used_cnt = 0;
        return -1;
    }

    /* Checks if keys match to network name and PAN ID and that needed keys exists (PMK,
       PTK and a GTK), and calls inserts function that will update the network keys as
       needed */
    if ((ws_pae_supp_network_name_compare(dest_network_name, pae_supp->sec_keys_nw_info->network_name) == 0 &&
            pan_id == pae_supp->sec_keys_nw_info->key_pan_id) &&
            (sec_prot_keys_gtk_count(pae_supp->sec_keys_nw_info->gtks) > 0) &&
            (sec_prot_keys_pmk_get(&pae_supp->entry.sec_keys) != NULL) &&
            (sec_prot_keys_ptk_get(&pae_supp->entry.sec_keys) != NULL)) {
        tr_debug("Existing keys used, counter %i", pae_supp->nw_keys_used_cnt);
        if (pae_supp->nw_key_insert(pae_supp->interface_ptr, pae_supp->sec_keys_nw_info->gtks, false) >= 0) {
            tr_debug("Keys inserted");
        }
        pae_supp->nw_keys_used_cnt++;
        return 0;
    } else {
        pae_supp->nw_keys_used_cnt = 0;
        return -1;
    }
}

int8_t ws_pae_supp_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name, bool updated)
{
    (void) pan_id;
    (void) network_name;

    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    if (updated) {
        tr_info("Delete old keys, new PAN ID: %i network name: %s", pan_id, network_name);
        // Delete pair wise keys
        sec_prot_keys_pmk_delete(&pae_supp->entry.sec_keys);
        sec_prot_keys_ptk_delete(&pae_supp->entry.sec_keys);
        sec_prot_keys_ptk_eui_64_delete(&pae_supp->entry.sec_keys);
        // Delete GTKs
        sec_prot_keys_gtks_clear(pae_supp->sec_keys_nw_info->gtks);
        // If data is changed, store to NVM
        if (sec_prot_keys_are_updated(&pae_supp->entry.sec_keys) ||
                sec_prot_keys_gtks_are_updated(pae_supp->sec_keys_nw_info->gtks)) {
            ws_pae_supp_nvm_update(pae_supp);
        }
    }
    return 0;
}

void ws_pae_supp_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_supp_auth_completed *completed, ws_pae_supp_auth_next_target *auth_next_target, ws_pae_supp_nw_key_insert *nw_key_insert, ws_pae_supp_nw_key_index_set *nw_key_index_set, ws_pae_supp_gtk_hash_ptr_get *gtk_hash_ptr_get, ws_pae_supp_nw_info_updated *nw_info_updated)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return;
    }

    pae_supp->auth_completed = completed;
    pae_supp->auth_next_target = auth_next_target;
    pae_supp->nw_key_insert = nw_key_insert;
    pae_supp->nw_key_index_set = nw_key_index_set;
    pae_supp->gtk_hash_ptr_get = gtk_hash_ptr_get;
    pae_supp->nw_info_updated = nw_info_updated;
}

int8_t ws_pae_supp_init(protocol_interface_info_entry_t *interface_ptr, const sec_prot_certs_t *certs, sec_cfg_t *sec_cfg, sec_prot_keys_nw_info_t *sec_keys_nw_info)
{
    if (!interface_ptr) {
        return -1;
    }

    if (ws_pae_supp_get(interface_ptr) != NULL) {
        return 0;
    }

    pae_supp_t *pae_supp = ns_dyn_mem_alloc(sizeof(pae_supp_t));
    if (!pae_supp) {
        return -1;
    }

    pae_supp->interface_ptr = interface_ptr;
    pae_supp->auth_completed = NULL;
    pae_supp->auth_next_target = NULL;
    pae_supp->nw_key_insert = NULL;
    pae_supp->nw_key_index_set = NULL;
    pae_supp->gtk_hash_ptr_get = NULL;
    pae_supp->initial_key_timer = 0;
    pae_supp->nw_keys_used_cnt = 0;
    pae_supp->initial_key_retry_cnt = MEDIUM_NW_INITIAL_KEY_RETRY_COUNT;
    pae_supp->sec_keys_nw_info = sec_keys_nw_info;
    pae_supp->sec_cfg = sec_cfg;
    pae_supp->gtk_update_trickle_running = false;
    pae_supp->auth_requested = false;
    pae_supp->timer_running = false;
    pae_supp->new_br_eui_64_set = false;
    pae_supp->new_br_eui_64_fresh = false;
    pae_supp->comp_br_eui_64_set = false;
    pae_supp->entry_address_active = false;
    pae_supp->initial_key_retry_max_value = 0;
    pae_supp->initial_key_retry_timer = 0;

    ws_pae_lib_supp_init(&pae_supp->entry);

    kmp_address_init(KMP_ADDR_EUI_64, &pae_supp->target_addr, 0);

    sec_prot_keys_gtks_init(pae_supp->sec_keys_nw_info->gtks);
    sec_prot_keys_init(&pae_supp->entry.sec_keys, pae_supp->sec_keys_nw_info->gtks, certs);
    memset(pae_supp->new_br_eui_64, 0, 8);

    pae_supp->kmp_service = kmp_service_create();
    if (!pae_supp->kmp_service) {
        goto error;
    }

    if (kmp_service_cb_register(pae_supp->kmp_service, ws_pae_supp_kmp_incoming_ind, ws_pae_supp_kmp_tx_status_ind, ws_pae_supp_kmp_service_addr_get, NULL, ws_pae_supp_kmp_service_api_get) < 0) {
        goto error;
    }

    if (kmp_service_event_if_register(pae_supp->kmp_service, ws_pae_supp_event_send)) {
        goto error;
    }

    if (kmp_service_timer_if_register(pae_supp->kmp_service, ws_pae_supp_timer_if_start, ws_pae_supp_timer_if_stop)) {
        goto error;
    }

    if (kmp_eapol_pdu_if_register(pae_supp->kmp_service, interface_ptr) < 0) {
        goto error;
    }

    if (ws_eapol_pdu_cb_register(interface_ptr, &eapol_pdu_recv_cb_data) < 0) {
        goto error;
    }

    if (supp_key_sec_prot_register(pae_supp->kmp_service) < 0) {
        goto error;
    }

    if (supp_eap_tls_sec_prot_register(pae_supp->kmp_service) < 0) {
        goto error;
    }

    if (client_tls_sec_prot_register(pae_supp->kmp_service) < 0) {
        goto error;
    }

    if (supp_fwh_sec_prot_register(pae_supp->kmp_service) < 0) {
        goto error;
    }

    if (supp_gkh_sec_prot_register(pae_supp->kmp_service) < 0) {
        goto error;
    }

    if (tasklet_id < 0) {
        tasklet_id = eventOS_event_handler_create(ws_pae_supp_tasklet_handler, PAE_TASKLET_INIT);
        if (tasklet_id < 0) {
            goto error;
        }
    }

    if (ws_pae_supp_timer_stop(pae_supp) < 0) {
        goto error;
    }

    ws_pae_supp_nvm_keys_read(pae_supp);

    ns_list_add_to_end(&pae_supp_list, pae_supp);

    return 0;

error:
    ws_pae_supp_free(pae_supp);

    return -1;
}

int8_t ws_pae_supp_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    ws_pae_supp_free(pae_supp);
    return 0;
}

static void ws_pae_supp_free(pae_supp_t *pae_supp)
{
    if (!pae_supp) {
        return;
    }

    ws_pae_lib_supp_delete(&pae_supp->entry);

    kmp_eapol_pdu_if_unregister(pae_supp->kmp_service);

    ws_eapol_pdu_cb_unregister(pae_supp->interface_ptr, &eapol_pdu_recv_cb_data);

    kmp_service_delete(pae_supp->kmp_service);

    ns_list_remove(&pae_supp_list, pae_supp);
    ns_dyn_mem_free(pae_supp);
}

static pae_supp_t *ws_pae_supp_get(protocol_interface_info_entry_t *interface_ptr)
{
    ns_list_foreach(pae_supp_t, entry, &pae_supp_list) {
        if (entry->interface_ptr == interface_ptr) {
            return entry;
        }
    }

    return NULL;
}

static pae_supp_t *ws_pae_supp_by_kmp_service_get(kmp_service_t *service)
{
    ns_list_foreach(pae_supp_t, entry, &pae_supp_list) {
        if (entry->kmp_service == service) {
            return entry;
        }
    }

    return NULL;
}

static int8_t ws_pae_supp_event_send(kmp_service_t *service, void *data)
{
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return -1;
    }

    arm_event_s event = {
        .receiver = tasklet_id,
        .sender = 0,
        .event_id = pae_supp->interface_ptr->id,
        .data_ptr = data,
        .event_type = PAE_TASKLET_EVENT,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        return -1;
    }

    return 0;
}

static void ws_pae_supp_tasklet_handler(arm_event_s *event)
{
    if (event->event_type == PAE_TASKLET_INIT) {

    } else if (event->event_type == PAE_TASKLET_EVENT) {
        pae_supp_t *pae_supp = NULL;

        ns_list_foreach(pae_supp_t, entry, &pae_supp_list) {
            if (entry->interface_ptr->id == event->event_id) {
                pae_supp = entry;
                break;
            }
        }

        if (pae_supp) {
            kmp_service_event_if_event(pae_supp->kmp_service, event->data_ptr);
        }
    }
}

void ws_pae_supp_fast_timer(uint16_t ticks)
{
    ns_list_foreach(pae_supp_t, pae_supp, &pae_supp_list) {
        if (!ws_pae_supp_timer_running(pae_supp)) {
            continue;
        }

        // Updates KMP timers and supplicant authentication ongoing timer
        bool running = ws_pae_lib_supp_timer_update(NULL, &pae_supp->entry, ticks, kmp_service_timer_if_timeout);

        // Checks whether timer needs to be active
        if (!ws_pae_supp_authentication_ongoing(pae_supp) && !running) {
            tr_debug("PAE idle");
            // If not already completed, restart bootstrap
            ws_pae_supp_authenticate_response(pae_supp, AUTH_RESULT_ERR_UNSPEC);

            ws_pae_supp_timer_stop(pae_supp);
        }
    }
}

static bool ws_pae_supp_authentication_ongoing(pae_supp_t *pae_supp)
{
    /* When either bootstrap initial authentication or re-authentication is ongoing */
    if (pae_supp->initial_key_timer || pae_supp->initial_key_retry_timer || pae_supp->gtk_update_trickle_running ||
            ws_pae_lib_supp_timer_is_running(&pae_supp->entry)) {
        return true;
    }

    return false;
}

void ws_pae_supp_slow_timer(uint16_t seconds)
{
    ns_list_foreach(pae_supp_t, pae_supp, &pae_supp_list) {

        bool key_expired = false;

        if (pae_supp->initial_key_retry_timer > seconds) {
            pae_supp->initial_key_retry_timer -= seconds;
        } else if (pae_supp->initial_key_retry_timer != 0) {
            pae_supp->initial_key_retry_timer = 0;
            key_expired = true;
        }

        if (pae_supp->gtk_update_trickle_running) {
            if (trickle_timer(&pae_supp->gtk_req_trickle_timer, &pae_supp->gtk_req_trickle_params, seconds)) {
                key_expired = true;
            }
        }

        if (key_expired) {
            if (pae_supp->initial_key_retry_cnt > 0) {
                // On initial EAPOL-key TX failure, check for other parents
                if (pae_supp->auth_requested && pae_supp->tx_failure_on_initial_key) {
                    // Returns same target if no other valid targets found
                    const uint8_t *next_target = pae_supp->auth_next_target(pae_supp->interface_ptr, kmp_address_eui_64_get(&pae_supp->target_addr), &pae_supp->sec_keys_nw_info->key_pan_id);
                    kmp_address_eui_64_set(&pae_supp->target_addr, next_target);
                    ws_pae_supp_address_set(pae_supp, &pae_supp->target_addr);
                }
                pae_supp->tx_failure_on_initial_key = false;
                // Sends initial EAPOL-key
                if (ws_pae_supp_initial_key_send(pae_supp) < 0) {
                    tr_info("EAPOL-Key send failed");
                }
            }

            /* Wait time for the authenticator to answer the last re-transmit expires;
               fails authentication */
            if (pae_supp->initial_key_retry_cnt == 0) {
                bool retry = false;
                // If making key update and GTKs do not match to GTK hash
                if (!pae_supp->auth_requested && ws_pae_supp_gtk_hash_mismatch_check(pae_supp) < 0) {
                    tr_info("GTKs do not match to GTK hash");
                    retry = true;
                }
                auth_result_e result = AUTH_RESULT_ERR_UNSPEC;
                if (pae_supp->tx_failure_on_initial_key) {
                    result = AUTH_RESULT_ERR_TX_ERR;
                    pae_supp->tx_failure_on_initial_key = false;
                }
                ws_pae_supp_authenticate_response(pae_supp, result);
                if (retry) {
                    // Start trickle timer to try re-authentication
                    ws_pae_supp_initial_key_update_trickle_timer_start(pae_supp, KEY_UPDATE_RETRY_COUNT);
                }
            } else {
                if (pae_supp->initial_key_retry_cnt > 1) {
                    pae_supp->initial_key_retry_cnt--;

                    pae_supp->initial_key_retry_max_value *= 2;
                    if (pae_supp->initial_key_retry_max_value > pae_supp->sec_cfg->prot_cfg.initial_key_retry_max_limit) {
                        pae_supp->initial_key_retry_max_value = pae_supp->sec_cfg->prot_cfg.initial_key_retry_max_limit;
                    }
                    pae_supp->initial_key_retry_timer = randLIB_get_random_in_range(
                                                            pae_supp->sec_cfg->prot_cfg.initial_key_retry_min,
                                                            pae_supp->initial_key_retry_max_value);

                    tr_info("Initial EAPOL-Key retry timer %i seconds (range [%i,%i] seconds)",
                            pae_supp->initial_key_retry_timer,
                            pae_supp->sec_cfg->prot_cfg.initial_key_retry_min,
                            pae_supp->initial_key_retry_max_value);
                } else if (pae_supp->initial_key_retry_cnt > 0) {
                    pae_supp->initial_key_retry_cnt = 0;
                    // Starts wait time for the authenticator to answer
                    pae_supp->initial_key_retry_timer = randLIB_get_random_in_range(
                                                            LAST_INTERVAL_MIN_SECS, LAST_INTERVAL_MAX_SECS);
                    tr_info("Initial EAPOL-Key wait for last re-transmit answer %i seconds (range [120,240] seconds)",
                            pae_supp->initial_key_retry_timer);
                }
            }
        }

        // Decrements GTK lifetimes
        for (uint8_t i = 0; i < GTK_NUM; i++) {
            if (!sec_prot_keys_gtk_is_set(pae_supp->sec_keys_nw_info->gtks, i)) {
                continue;
            }
            uint64_t current_time = ws_pae_current_time_get();
            sec_prot_keys_gtk_lifetime_decrement(pae_supp->sec_keys_nw_info->gtks, i, current_time, seconds, false);
        }

        if (pae_supp->initial_key_timer > 0) {
            if (pae_supp->initial_key_timer > seconds) {
                pae_supp->initial_key_timer -= seconds;
            } else {
                pae_supp->initial_key_timer = 0;
                pae_supp->tx_failure_on_initial_key = false;
                pae_supp->initial_key_retry_cnt = pae_supp->sec_cfg->prot_cfg.initial_key_retry_cnt;
                // Sends initial EAPOL-Key message
                if (ws_pae_supp_initial_key_send(pae_supp) < 0) {
                    tr_info("EAPOL-Key send failed");
                }
                // Starts initial EAPOL-key retry exponential backoff timer
                pae_supp->initial_key_retry_max_value = pae_supp->sec_cfg->prot_cfg.initial_key_retry_max;
                pae_supp->initial_key_retry_timer = randLIB_get_random_in_range(
                                                        pae_supp->sec_cfg->prot_cfg.initial_key_retry_min,
                                                        pae_supp->initial_key_retry_max_value);

                tr_info("Initial EAPOL-Key retry timer %i seconds (range [%i,%i] seconds)",
                        pae_supp->initial_key_retry_timer,
                        pae_supp->sec_cfg->prot_cfg.initial_key_retry_min,
                        pae_supp->initial_key_retry_max_value);
            }
        }
    }
}

static void ws_pae_supp_initial_key_update_trickle_timer_start(pae_supp_t *pae_supp, uint8_t timer_expirations)
{
    // Starts trickle for the key update
    pae_supp->gtk_req_trickle_params.Imin = pae_supp->sec_cfg->timer_cfg.gtk_request_imin;
    pae_supp->gtk_req_trickle_params.Imax = pae_supp->sec_cfg->timer_cfg.gtk_request_imax;
    pae_supp->gtk_req_trickle_params.k = 0;
    pae_supp->gtk_req_trickle_params.TimerExpirations = timer_expirations;

    trickle_start(&pae_supp->gtk_req_trickle_timer, &pae_supp->gtk_req_trickle_params);
    tr_info("Initial EAPOL-Key trickle I: [%i,%i] %i, t: %i", pae_supp->gtk_req_trickle_params.Imin, pae_supp->gtk_req_trickle_params.Imax, pae_supp->gtk_req_trickle_timer.I, pae_supp->gtk_req_trickle_timer.t);
    pae_supp->gtk_update_trickle_running = true;
    pae_supp->initial_key_retry_cnt = timer_expirations;
}

static int8_t ws_pae_supp_timer_if_start(kmp_service_t *service, kmp_api_t *kmp)
{
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return -1;
    }

    if (ws_pae_supp_timer_start(pae_supp) < 0) {
        return -1;
    }

    kmp_entry_t *entry = kmp_api_data_get(kmp);
    if (!entry) {
        return -1;
    }
    ws_pae_lib_kmp_timer_start(&pae_supp->entry.kmp_list, entry);
    return 0;
}

static int8_t ws_pae_supp_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp)
{
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return -1;
    }

    kmp_entry_t *entry = kmp_api_data_get(kmp);
    if (!entry) {
        return -1;
    }
    ws_pae_lib_kmp_timer_stop(&pae_supp->entry.kmp_list, entry);
    return 0;
}

static int8_t ws_pae_supp_timer_start(pae_supp_t *pae_supp)
{
    pae_supp->timer_running = true;
    return 0;
}

static int8_t ws_pae_supp_timer_stop(pae_supp_t *pae_supp)
{
    pae_supp->timer_running = false;
    return 0;
}

static bool ws_pae_supp_timer_running(pae_supp_t *pae_supp)
{
    return pae_supp->timer_running;
}

static int8_t ws_pae_supp_eapol_pdu_address_check(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64)
{
    pae_supp_t *pae_supp = ws_pae_supp_get(interface_ptr);
    if (!pae_supp) {
        return -1;
    }

    // Message from EAPOL target node, route to self
    if (pae_supp->entry_address_active) {
        if (memcmp(eui_64, pae_supp->entry.addr.eui_64, 8) == 0) {
            return 0;
        }
    }

    // Get parent
    uint8_t parent_eui_64[8];
    if (ws_pae_supp_parent_eui_64_get(interface_ptr, parent_eui_64) < 0) {
        return -1;
    }

    // Message from RPL parent, route to self
    if (memcmp(parent_eui_64, eui_64, 8) == 0) {
        return 0;
    }

    return -1;
}

static int8_t ws_pae_supp_parent_eui_64_get(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64)
{
    rpl_dodag_info_t dodag_info;
    (void) dodag_info; // avoid unused warning in ws_host mode
    if (!interface_ptr->rpl_domain) {
        return -1;
    }
    struct rpl_instance *instance = rpl_control_enumerate_instances(interface_ptr->rpl_domain, NULL);
    if (instance && rpl_control_read_dodag_info(instance, &dodag_info)) {
        // Get parent
        const uint8_t *parent_ll_addr = rpl_control_preferred_parent_addr(instance, false);
        if (parent_ll_addr) {
            memcpy(eui_64, &parent_ll_addr[8], 8);
            eui_64[0] ^= 0x02;
            return 0;
        }
    }

    return -1;
}

static void ws_pae_supp_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr)
{
    (void) kmp;

    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return;
    }

    // Get own EUI-64
    link_layer_address_s mac_params;
    if (arm_nwk_mac_address_read(pae_supp->interface_ptr->id, &mac_params) >= 0) {
        kmp_address_eui_64_set(local_addr, mac_params.mac_long);
    }

    // BR address has been received during authentication attempt
    if (pae_supp->new_br_eui_64_fresh) {
        kmp_address_eui_64_set(remote_addr, pae_supp->new_br_eui_64);
    } else {
        uint8_t *eui_64 = sec_prot_keys_ptk_eui_64_get(&pae_supp->entry.sec_keys);
        // BR address is set on security keys (confirmed using 4WH)
        if (eui_64) {
            kmp_address_eui_64_set(remote_addr, eui_64);
        } else {
            // For initial EAPOL key, if BR address has been received during previous attempt, generate PMKID using it
            if (pae_supp->new_br_eui_64_set && kmp_api_type_get(kmp) >= IEEE_802_1X_INITIAL_KEY) {
                kmp_address_eui_64_set(remote_addr, pae_supp->new_br_eui_64);
            } else {
                memset(remote_addr, 0, sizeof(kmp_addr_t));
                tr_error("No border router EUI-64");
            }
        }
    }
}

static kmp_api_t *ws_pae_supp_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type)
{
    (void) kmp;

    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return NULL;
    }

    return ws_pae_lib_kmp_list_type_get(&pae_supp->entry.kmp_list, type);
}

static kmp_api_t *ws_pae_supp_kmp_incoming_ind(kmp_service_t *service, uint8_t instance_id, kmp_type_e type, const kmp_addr_t *addr, const void *pdu, uint16_t size)
{
    (void) instance_id;
    (void) pdu;
    (void) size;

    // Should be MKA, 4WH or GKH and never initial EAPOL-key for supplicant
    if (type > IEEE_802_1X_INITIAL_KEY) {
        return NULL;
    }

    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return NULL;
    }

    // If target address is not set or authentication is not ongoing
    if (!pae_supp->entry_address_active || !ws_pae_supp_authentication_ongoing(pae_supp)) {
        tr_info("Incoming KMP rejected, auth not ongoing, type: %i ", type);
        // Does no longer wait for authentication, ignores message
        return NULL;
    }

    // Updates parent address
    kmp_address_copy(&pae_supp->entry.addr, addr);

    // Check if ongoing
    kmp_api_t *kmp = ws_pae_lib_kmp_list_type_get(&pae_supp->entry.kmp_list, type);
    /* If kmp receiving is enabled or it is not GKH, routes message to existing KMP.

       For GKH creates an instance to handle message. If message is not valid (e.g. repeated
       message counter), GKH ignores message and waits for timeout. All further messages
       are routed to that instance. If valid message arrives, GKH instance handles the
       message, replies to authenticator and terminates. */
    if (kmp && (!kmp_api_receive_disable(kmp) || type != IEEE_802_11_GKH)) {
        return kmp;
    }

    // Create new instance
    kmp = ws_pae_supp_kmp_create_and_start(service, type, pae_supp);

    // Adds ticks to wait for authenticator to continue timer
    ws_pae_lib_supp_timer_ticks_add(&pae_supp->entry, START_AUTHENTICATION_TICKS);

    // For EAP-TLS create also TLS in addition to EAP-TLS
    if (type == IEEE_802_1X_MKA) {
        if (ws_pae_lib_kmp_list_type_get(&pae_supp->entry.kmp_list, TLS_PROT) != NULL) {
            // TLS already exists, wait for it to be deleted
            ws_pae_lib_kmp_list_delete(&pae_supp->entry.kmp_list, kmp);
            return NULL;
        }
        // Create TLS instance */
        if (ws_pae_supp_kmp_create_and_start(service, TLS_PROT, pae_supp) == NULL) {
            ws_pae_lib_kmp_list_delete(&pae_supp->entry.kmp_list, kmp);
            return NULL;
        }
    }

    return kmp;
}

static kmp_api_t *ws_pae_supp_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, pae_supp_t *pae_supp)
{
    // Create new instance
    kmp_api_t *kmp = kmp_api_create(service, type, 0, pae_supp->sec_cfg);
    if (!kmp) {
        return NULL;
    }

    // Updates parent address
    kmp_api_addr_set(kmp, &pae_supp->entry.addr);

    // Sets security keys to KMP
    kmp_api_sec_keys_set(kmp, &pae_supp->entry.sec_keys);

    kmp_api_cb_register(
        kmp, ws_pae_supp_kmp_api_create_confirm,
        ws_pae_supp_kmp_api_create_indication,
        ws_pae_supp_kmp_api_finished_indication,
        ws_pae_supp_kmp_api_finished);

    kmp_entry_t *kmp_entry = ws_pae_lib_kmp_list_add(&pae_supp->entry.kmp_list, kmp);
    if (!kmp_entry) {
        kmp_api_delete(kmp);
        return NULL;
    }

    kmp_api_data_set(kmp, kmp_entry);

    if (kmp_api_start(kmp) < 0) {
        ws_pae_lib_kmp_list_delete(&pae_supp->entry.kmp_list, kmp);
        return NULL;
    }

    return kmp;
}

static void ws_pae_supp_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result)
{
    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return;
    }

    // KMP-CREATE.request has failed, authentication error
    if (result != KMP_RESULT_OK) {
        ws_pae_supp_authenticate_response(pae_supp, AUTH_RESULT_ERR_UNSPEC);
    }
}

static void ws_pae_supp_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr)
{
    (void) addr;
    (void) type;

    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return;
    }

    // Incoming KMP protocol has started, no longer runs trickle timer for re-sending EAPOL-key message
    pae_supp->gtk_update_trickle_running = false;
    pae_supp->initial_key_retry_timer = 0;

    // For now, accept every KMP-CREATE.indication
    kmp_api_create_response(kmp, KMP_RESULT_OK);
}

static bool ws_pae_supp_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys)
{
    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return false;
    }

    kmp_type_e type = kmp_api_type_get(kmp);

    // Whenever EAP-TLS, 4WH or GKH completes increase timer for wait for authentication
    if (type < IEEE_802_1X_INITIAL_KEY && result == KMP_RESULT_OK) {
        ws_pae_lib_supp_timer_ticks_set(&pae_supp->entry, WAIT_FOR_AUTHENTICATION_TICKS);
    }

    /* When 4WH or GKH completes inserts keys and indicates authentication completed
       (if not alredy indicated) */
    if ((type == IEEE_802_11_4WH || type == IEEE_802_11_GKH) && result == KMP_RESULT_OK) {
        if (sec_keys) {
            sec_prot_keys_t *keys = sec_keys;
            pae_supp->nw_key_insert(pae_supp->interface_ptr, keys->gtks, false);
        }

        ws_pae_supp_authenticate_response(pae_supp, AUTH_RESULT_OK);
    }

    /* If initial EAPOL-key message sending fails to tx no acknowledge, indicates failure so
       that bootstrap can decide if EAPOL target should be changed */
    else if (type > IEEE_802_1X_INITIAL_KEY && (result == KMP_RESULT_ERR_TX_NO_ACK || result == KMP_RESULT_ERR_TX_UNSPEC)) {
        tr_info("Initial EAPOL-Key TX failure, target: %s", trace_array(kmp_address_eui_64_get(&pae_supp->entry.addr), 8));
        /* Fails authentication only if other authentication protocols are not yet
           started by authenticator */
        if (ws_pae_lib_kmp_list_count(&pae_supp->entry.kmp_list) <= 1) {
            // Continues with trickle but selects different parent
            pae_supp->tx_failure_on_initial_key = true;
        }

    } else if ((type == IEEE_802_1X_MKA || type == IEEE_802_11_4WH || type == IEEE_802_11_GKH) && result != KMP_RESULT_OK) {

        if (!pae_supp->auth_requested && ws_pae_supp_gtk_hash_mismatch_check(pae_supp) < 0) {
            // Start trickle timer
            ws_pae_supp_initial_key_update_trickle_timer_start(pae_supp, KEY_UPDATE_RETRY_COUNT);

            // Starts supplicant timer
            ws_pae_supp_timer_start(pae_supp);

            tr_info("GTK update re-start imin: %i, imax: %i, max mismatch: %i, tr time: %i", pae_supp->sec_cfg->timer_cfg.gtk_request_imin, pae_supp->sec_cfg->timer_cfg.gtk_request_imax, pae_supp->sec_cfg->timer_cfg.gtk_max_mismatch, pae_supp->gtk_req_trickle_timer.t);
        }

    }

    return false;
}

static void ws_pae_supp_kmp_api_finished(kmp_api_t *kmp)
{
    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return;
    }

    // Delete KMP
    ws_pae_lib_kmp_list_delete(&pae_supp->entry.kmp_list, kmp);
}

static kmp_api_t *ws_pae_supp_kmp_tx_status_ind(kmp_service_t *service, uint8_t instance_id)
{
    pae_supp_t *pae_supp = ws_pae_supp_by_kmp_service_get(service);
    if (!pae_supp) {
        return NULL;
    }

    kmp_api_t *kmp = ws_pae_lib_kmp_list_instance_id_get(&pae_supp->entry.kmp_list, instance_id);
    if (!kmp) {
        return NULL;
    }

    return kmp;
}

#endif /* HAVE_PAE_SUPP */
#endif /* HAVE_WS */

