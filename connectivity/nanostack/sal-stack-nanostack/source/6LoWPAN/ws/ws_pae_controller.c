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
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "ns_address.h"
#include "ws_management_api.h"
#include "ws_bbr_api.h"
#include "Service_Libs/utils/ns_file.h"
#include "Service_Libs/utils/ns_time.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "6LoWPAN/ws/ws_pae_supp.h"
#include "6LoWPAN/ws/ws_pae_auth.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"
#include "6LoWPAN/ws/ws_pae_nvm_data.h"
#include "6LoWPAN/ws/ws_pae_time.h"
#include "6LoWPAN/ws/ws_pae_key_storage.h"
#include "mbedtls/sha256.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wspc"

typedef int8_t ws_pae_delete(protocol_interface_info_entry_t *interface_ptr);
typedef void ws_pae_timer(uint16_t ticks);
typedef int8_t ws_pae_br_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);
typedef int8_t ws_pae_br_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64);
typedef void ws_pae_gtks_updated(protocol_interface_info_entry_t *interface_ptr);
typedef int8_t ws_pae_gtk_hash_update(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash, bool del_gtk_on_mismatch);
typedef int8_t ws_pae_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index);
typedef int8_t ws_pae_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name, bool updated);

typedef struct {
    uint8_t gtk[GTK_LEN];                                            /**< GTK key */
    bool set : 1;                                                    /**< Key has been set */
    bool installed : 1;                                              /**< Key has been installed on MAC */
} nw_key_t;

typedef struct {
    ns_list_link_t link;                                             /**< Link */
#ifdef HAVE_PAE_SUPP
    uint8_t target_eui_64[8];                                        /**< EAPOL target */
    uint16_t target_pan_id;                                          /**< EAPOL target PAN ID */
#endif
    uint8_t br_eui_64[8];                                            /**< Border router EUI-64 */
    sec_prot_gtk_keys_t gtks;                                        /**< GTKs */
#ifdef HAVE_PAE_AUTH
    sec_prot_gtk_keys_t next_gtks;                                   /**< Next GTKs */
#endif
    sec_prot_keys_nw_info_t sec_keys_nw_info;                        /**< Security keys network information */
    int8_t gtk_index;                                                /**< GTK index */
    uint8_t gtkhash[32];                                             /**< GTK hashes */
    sec_prot_certs_t certs;                                          /**< Certificates */
    nw_key_t nw_key[GTK_NUM];                                        /**< Currently active network keys (on MAC) */
    uint16_t frame_cnt_store_timer;                                  /**< Timer to check if storing of frame counter value is needed */
    uint32_t frame_cnt_store_force_timer;                            /**< Timer to force storing of frame counter, if no other updates */
    frame_counters_t frame_counters;                                 /**< Frame counters */
    sec_cfg_t sec_cfg;                                               /**< Security configuration (configuration set values) */
    uint32_t restart_cnt;                                            /**< Re-start counter */
    protocol_interface_info_entry_t *interface_ptr;                  /**< List link entry */
    ws_pae_controller_nw_key_set *nw_key_set;                        /**< Key set callback */
    ws_pae_controller_nw_key_clear *nw_key_clear;                    /**< Key clear callback */
    ws_pae_controller_nw_send_key_index_set *nw_send_key_index_set;  /**< Send key index set callback */
    ws_pae_controller_nw_frame_counter_set *nw_frame_counter_set;    /**< Frame counter set callback */
    ws_pae_controller_nw_frame_counter_read *nw_frame_counter_read;  /**< Frame counter read callback */
#ifdef HAVE_PAE_SUPP
    ws_pae_controller_auth_completed *auth_completed;                /**< Authentication completed callback, continue bootstrap */
    ws_pae_controller_auth_next_target *auth_next_target;            /**< Authentication next target callback */
#endif
#ifdef HAVE_PAE_AUTH
    ws_pae_controller_pan_ver_increment *pan_ver_increment;          /**< PAN version increment callback */
    ws_pae_controller_nw_info_updated *nw_info_updated;              /**< Network information updated callback */
    ws_pae_controller_congestion_get *congestion_get;                /**< Congestion get callback */
    ws_pae_controller_ip_addr_get *ip_addr_get;                      /**< IP address get callback */
#endif
    ws_pae_delete *pae_delete;                                       /**< PAE delete callback */
    ws_pae_timer *pae_fast_timer;                                    /**< PAE fast timer callback */
    ws_pae_timer *pae_slow_timer;                                    /**< PAE slow timer callback */
    ws_pae_br_addr_write *pae_br_addr_write;                         /**< PAE Border router EUI-64 write callback */
    ws_pae_br_addr_read *pae_br_addr_read;                           /**< PAE Border router EUI-64 read callback */
    ws_pae_gtks_updated *pae_gtks_updated;                           /**< PAE GTKs updated */
    ws_pae_gtk_hash_update *pae_gtk_hash_update;                     /**< PAE GTK HASH update */
    ws_pae_nw_key_index_update *pae_nw_key_index_update;             /**< PAE NW key index update */
    ws_pae_nw_info_set *pae_nw_info_set;                             /**< PAE security key network info set */
    uint8_t pae_nvm_buffer[PAE_NVM_DEFAULT_BUFFER_SIZE];             /**< Buffer for PAE NVM read and write operations */
    bool gtks_set : 1;                                               /**< GTKs are set */
    bool gtkhash_set : 1;                                            /**< GTK hashes are set */
    bool key_index_set : 1;                                          /**< NW key index is set */
    bool frame_counter_read : 1;                                     /**< Frame counters has been read */
    bool auth_started : 1;                                           /**< Authenticator has been started */
} pae_controller_t;

typedef struct {
    sec_radius_cfg_t *radius_cfg;                                    /**< Radius configuration settings */
    uint16_t node_limit;                                             /**< Max number of stored supplicants */
    bool node_limit_set : 1;                                         /**< Node limit set */
    bool ext_cert_valid_enabled : 1;                                 /**< Extended certificate validation enabled */
} pae_controller_config_t;

static void ws_pae_controller_keys_nw_info_init(sec_prot_keys_nw_info_t *sec_keys_nw_info, sec_prot_gtk_keys_t *gtks);
static void ws_pae_controller_nw_info_updated_check(protocol_interface_info_entry_t *interface_ptr);
#ifdef HAVE_PAE_AUTH
static void ws_pae_controller_auth_ip_addr_get(protocol_interface_info_entry_t *interface_ptr, uint8_t *address);
static bool ws_pae_controller_auth_congestion_get(protocol_interface_info_entry_t *interface_ptr, uint16_t active_supp);
static int8_t  ws_pae_controller_auth_nw_frame_counter_read(protocol_interface_info_entry_t *interface_ptr, uint32_t *counter, uint8_t gtk_index);
#endif
static pae_controller_t *ws_pae_controller_get(protocol_interface_info_entry_t *interface_ptr);
static void ws_pae_controller_frame_counter_timer(uint16_t seconds, pae_controller_t *entry);
static void ws_pae_controller_frame_counter_store(pae_controller_t *entry, bool use_threshold);
static void ws_pae_controller_nvm_frame_counter_write(frame_cnt_nvm_tlv_t *tlv_entry);
static int8_t ws_pae_controller_nvm_frame_counter_read(uint32_t *restart_cnt, uint64_t *stored_time, uint16_t *pan_version, frame_counters_t *counters);
static pae_controller_t *ws_pae_controller_get_or_create(int8_t interface_id);
#ifdef HAVE_PAE_AUTH
static void ws_pae_controller_gtk_hash_set(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash);
#endif
static int8_t ws_pae_controller_nw_key_check_and_insert(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks, bool force_install);
static void ws_pae_controller_active_nw_key_clear(nw_key_t *nw_key);
#ifdef HAVE_PAE_SUPP
static void ws_pae_controller_active_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t index);
#endif
static int8_t ws_pae_controller_gak_from_gtk(uint8_t *gak, uint8_t *gtk, char *network_name);
static void ws_pae_controller_frame_counter_store_and_nw_keys_remove(protocol_interface_info_entry_t *interface_ptr, pae_controller_t *controller, bool use_threshold);
#ifdef HAVE_PAE_AUTH
static void ws_pae_controller_nw_key_index_check_and_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index);
#endif
static void ws_pae_controller_data_init(pae_controller_t *controller);
static int8_t ws_pae_controller_frame_counter_read(pae_controller_t *controller);
static void ws_pae_controller_frame_counter_reset(frame_counters_t *frame_counters);
static void ws_pae_controller_frame_counter_index_reset(frame_counters_t *frame_counters, uint8_t index);
static int8_t ws_pae_controller_nw_info_read(pae_controller_t *controller, sec_prot_gtk_keys_t *gtks);
static int8_t ws_pae_controller_nvm_nw_info_write(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks, uint64_t stored_time, uint8_t time_changed);
static int8_t ws_pae_controller_nvm_nw_info_read(protocol_interface_info_entry_t *interface_ptr, uint16_t *pan_id, char *network_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks, uint64_t current_time, uint8_t *time_changed);


static const char *FRAME_COUNTER_FILE = FRAME_COUNTER_FILE_NAME;
static const char *NW_INFO_FILE = NW_INFO_FILE_NAME;

static NS_LIST_DEFINE(pae_controller_list, pae_controller_t, link);

pae_controller_config_t pae_controller_config = {
    .radius_cfg = NULL,
    .node_limit = 0,
    .node_limit_set = false,
    .ext_cert_valid_enabled = false
};

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
            ws_pae_controller_nw_key_check_and_insert(controller->interface_ptr, &controller->gtks, false);
            sec_prot_keys_gtks_updated_reset(&controller->gtks);
            ws_pae_supp_gtks_set(controller->interface_ptr, &controller->gtks);
        }
        controller->auth_completed(interface_ptr, AUTH_RESULT_OK, NULL);
        return 0;
    }

    if (ws_pae_supp_authenticate(controller->interface_ptr, controller->target_pan_id, controller->target_eui_64, controller->sec_keys_nw_info.network_name) < 0) {
        controller->auth_completed(interface_ptr, AUTH_RESULT_ERR_UNSPEC, controller->target_eui_64);
    }
#endif

    return 0;
}

int8_t ws_pae_controller_bootstrap_done(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

#ifdef HAVE_PAE_SUPP
    // RPL parent is known, remove EAPOL target that what was set using the authenticate call */
    ws_pae_supp_eapol_target_remove(interface_ptr);

    /* Trigger GTK hash update to supplicant, so it can check whether keys have been updated
       during bootstrap. Does nothing if GTKs are up to date. */
    ws_pae_supp_gtk_hash_update(interface_ptr, controller->gtkhash, false);
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

    if (ws_pae_auth_addresses_set(interface_ptr, local_port, remote_addr, remote_port) < 0) {
        return -1;
    }

    // If either radius address or password is set, both must be set
    if (controller->sec_cfg.radius_cfg != NULL) {
        if (controller->sec_cfg.radius_cfg->radius_addr_set || controller->sec_cfg.radius_cfg->radius_shared_secret_len > 0) {
            if (!controller->sec_cfg.radius_cfg->radius_addr_set) {
                return -1;
            }
            if (controller->sec_cfg.radius_cfg->radius_shared_secret_len == 0) {
                return -1;
            }
            if (ws_pae_auth_radius_address_set(interface_ptr, controller->sec_cfg.radius_cfg->radius_addr) < 0) {
                return -1;
            }
        }
    }

    if (pae_controller_config.node_limit_set) {
        ws_pae_auth_node_limit_set(controller->interface_ptr, pae_controller_config.node_limit);
    }

    ws_pae_auth_cb_register(interface_ptr, ws_pae_controller_gtk_hash_set, ws_pae_controller_nw_key_check_and_insert, ws_pae_controller_nw_key_index_check_and_set, ws_pae_controller_nw_info_updated_check, ws_pae_controller_auth_ip_addr_get, ws_pae_controller_auth_congestion_get, ws_pae_controller_auth_nw_frame_counter_read);

    controller->auth_started = true;

    ws_pae_auth_start(interface_ptr);

    return 0;
}

int8_t ws_pae_controller_nw_key_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_nw_key_set *nw_key_set, ws_pae_controller_nw_key_clear *nw_key_clear, ws_pae_controller_nw_send_key_index_set *nw_send_key_index_set, ws_pae_controller_nw_frame_counter_set *nw_frame_counter_set, ws_pae_controller_nw_frame_counter_read *nw_frame_counter_read)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->nw_key_set = nw_key_set;
    controller->nw_key_clear = nw_key_clear;
    controller->nw_send_key_index_set = nw_send_key_index_set;
    controller->nw_frame_counter_set = nw_frame_counter_set;
    controller->nw_frame_counter_read = nw_frame_counter_read;
    return 0;
}

int8_t ws_pae_controller_authentication_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_auth_completed *completed, ws_pae_controller_auth_next_target *auth_next_target)
{
#ifdef HAVE_PAE_SUPP
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->auth_completed = completed;
    controller->auth_next_target = auth_next_target;
    return 0;
#else
    (void) interface_ptr;
    (void) completed;
    (void) auth_next_target;
    return 0;
#endif
}

int8_t ws_pae_controller_information_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_ip_addr_get *ip_addr_get, ws_pae_controller_nw_info_updated *nw_info_updated, ws_pae_controller_congestion_get *congestion_get)
{
#ifdef HAVE_PAE_AUTH
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->ip_addr_get = ip_addr_get;
    controller->nw_info_updated = nw_info_updated;
    controller->congestion_get = congestion_get;
    return 0;
#else
    (void) interface_ptr;
    (void) ip_addr_get;
    (void) nw_info_updated;
    (void) congestion_get;
    return 0;
#endif
}

int8_t ws_pae_controller_pan_version_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_pan_ver_increment *pan_ver_increment)
{
#ifdef HAVE_PAE_AUTH
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->pan_ver_increment = pan_ver_increment;
    return 0;
#else
    (void) interface_ptr;
    (void) pan_ver_increment;
    return 0;
#endif
}

int8_t ws_pae_controller_set_target(protocol_interface_info_entry_t *interface_ptr, uint16_t target_pan_id, uint8_t *target_eui_64)
{
#ifdef HAVE_PAE_SUPP
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
#else
    (void) interface_ptr;
    (void) target_pan_id;
    (void) target_eui_64;
    return -1;
#endif
}

static void ws_pae_controller_keys_nw_info_init(sec_prot_keys_nw_info_t *sec_keys_nw_info, sec_prot_gtk_keys_t *gtks)
{
    if (!sec_keys_nw_info) {
        return;
    }

    memset(sec_keys_nw_info, 0, sizeof(sec_prot_keys_nw_info_t));

    sec_keys_nw_info->gtks = gtks;
    sec_keys_nw_info->new_pan_id = 0xFFFF;
    sec_keys_nw_info->key_pan_id = 0xFFFF;
    sec_keys_nw_info->updated = false;
}

int8_t ws_pae_controller_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, uint16_t pan_version, char *network_name)
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

    bool updated = false;

    // Network name has been modified
    if (network_name && strcmp(controller->sec_keys_nw_info.network_name, network_name) != 0) {
        strncpy(controller->sec_keys_nw_info.network_name, network_name, 32);
        controller->sec_keys_nw_info.updated = true;
        updated = true;
    }

    // PAN ID has been modified
    if (pan_id != 0xffff && pan_id != controller->sec_keys_nw_info.new_pan_id) {
        controller->sec_keys_nw_info.new_pan_id = pan_id;
        controller->sec_keys_nw_info.updated = true;
        updated = true;
    }

    // Store pan version
    controller->sec_keys_nw_info.pan_version = pan_version;

    if (controller->pae_nw_info_set) {
        controller->pae_nw_info_set(interface_ptr, pan_id, network_name, updated);
    }

    return 0;
}

static void ws_pae_controller_nw_info_updated_check(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return;
    }

    if (controller->sec_keys_nw_info.updated || sec_prot_keys_gtks_are_updated(controller->sec_keys_nw_info.gtks)) {
        // Get own EUI-64
        uint8_t gtk_eui64[8] = {0};
        link_layer_address_s mac_params;
        if (arm_nwk_mac_address_read(interface_ptr->id, &mac_params) >= 0) {
            memcpy(gtk_eui64, mac_params.mac_long, 8);
        }
        uint64_t system_time = ws_pae_current_time_get();
        ws_pae_controller_nvm_nw_info_write(interface_ptr, controller->sec_keys_nw_info.key_pan_id, controller->sec_keys_nw_info.network_name, gtk_eui64, controller->sec_keys_nw_info.gtks, system_time, controller->sec_keys_nw_info.system_time_changed);
        controller->sec_keys_nw_info.updated = false;
        sec_prot_keys_gtks_updated_reset(controller->sec_keys_nw_info.gtks);
    }
}

#ifdef HAVE_PAE_AUTH
static void ws_pae_controller_auth_ip_addr_get(protocol_interface_info_entry_t *interface_ptr, uint8_t *address)
{
    if (!interface_ptr) {
        return;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return;
    }

    controller->ip_addr_get(interface_ptr, address);
}

static bool ws_pae_controller_auth_congestion_get(protocol_interface_info_entry_t *interface_ptr, uint16_t active_supp)
{
    if (!interface_ptr) {
        return 0;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return 0;
    }

    return controller->congestion_get(interface_ptr, active_supp);
}

static int8_t ws_pae_controller_auth_nw_frame_counter_read(protocol_interface_info_entry_t *interface_ptr, uint32_t *counter, uint8_t gtk_index)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    controller->nw_frame_counter_read(interface_ptr, counter, gtk_index);
    return 0;
}
#endif

int8_t ws_pae_controller_nw_key_valid(protocol_interface_info_entry_t *interface_ptr, uint8_t *br_iid)
{
    (void) br_iid;

    if (!interface_ptr) {
        return -1;
    }

    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    return ws_pae_supp_nw_key_valid(interface_ptr, br_iid);
}

static int8_t ws_pae_controller_nw_key_check_and_insert(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks, bool force_install)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    int8_t ret = -1;

    // Adds, removes and updates network keys to MAC based on new GTKs
    nw_key_t *nw_key = controller->nw_key;
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        // Gets GTK for the index (new, modified or none)
        uint8_t *gtk = sec_prot_keys_gtk_get(gtks, i);

        // If network key is set and GTK key is not set or not the same, removes network key
        if (nw_key[i].set && (!gtk || memcmp(nw_key[i].gtk, gtk, GTK_LEN) != 0)) {
            // Removes key from MAC if installed
            if (nw_key[i].installed) {
                controller->nw_key_clear(interface_ptr, i);
            }
            nw_key[i].installed = false;
            nw_key[i].set = false;
            tr_info("NW key remove: %i", i);
        }

        if (force_install) {
            // Install always
            nw_key[i].installed = false;
            // Frame counters are fresh
            ws_pae_controller_frame_counter_index_reset(&controller->frame_counters, i);
        }

        // If GTK key is not set, continues to next GTK
        if (!gtk) {
            continue;
        }

        // Network key is set and installed, all done
        if (nw_key[i].set && nw_key[i].installed) {
            continue;
        }

        // If network key is not set, stores the new GTK key to network key
        if (!nw_key[i].set) {
            nw_key[i].set = true;
            nw_key[i].installed = false;
            memcpy(nw_key[i].gtk, gtk, GTK_LEN);
        }

        // If network key has not been installed, installs it and updates frame counter as needed
        if (!nw_key[i].installed) {
            uint8_t gtkhash[GTK_HASH_LEN];
            sec_prot_keys_gtk_hash_generate(gtk, gtkhash);
            tr_info("NW key set: %i, hash: %s", i, trace_array(gtkhash, 8));
            uint8_t gak[GTK_LEN];
            if (ws_pae_controller_gak_from_gtk(gak, gtk, controller->sec_keys_nw_info.network_name) >= 0) {
                // Install the new network key derived from GTK and network name (GAK) to MAC
                controller->nw_key_set(interface_ptr, i, i, gak);
                nw_key[i].installed = true;
                ret = 0;
#ifdef EXTRA_DEBUG_INFO
                tr_info("NW name: %s", controller->sec_keys_nw_info.network_name);
                size_t nw_name_len = strlen(controller->sec_keys_nw_info.network_name);
                tr_info("NW name: %s", trace_array((uint8_t *)controller->sec_keys_nw_info.network_name, nw_name_len));
                tr_info("GTK: %s", trace_array(gtk, 16));
                tr_info("GAK: %s", trace_array(gak, 16));
#endif
            } else {
                tr_error("GAK generation failed network name: %s", controller->sec_keys_nw_info.network_name);
                continue;
            }

            // If frame counter value has been stored for the network key, updates the frame counter if needed
            if (controller->frame_counters.counter[i].set &&
                    memcmp(gtk, controller->frame_counters.counter[i].gtk, GTK_LEN) == 0) {
                // Read current counter from MAC
                uint32_t curr_frame_counter;
                controller->nw_frame_counter_read(controller->interface_ptr, &curr_frame_counter, i);

                // If stored frame counter is greater than MAC counter
                if (controller->frame_counters.counter[i].frame_counter > curr_frame_counter) {
                    tr_debug("Frame counter set: %i, stored %"PRIu32" current: %"PRIu32"", i,
                             controller->frame_counters.counter[i].frame_counter, curr_frame_counter);
                    curr_frame_counter = controller->frame_counters.counter[i].frame_counter;
                    // Updates MAC frame counter
                    controller->nw_frame_counter_set(controller->interface_ptr, curr_frame_counter, i);
                }
            }
        }
    }

    return ret;
}

static void ws_pae_controller_active_nw_key_clear(nw_key_t *nw_key)
{
    memset(nw_key, 0, sizeof(nw_key_t));
    nw_key->set = false;
    nw_key->installed = false;
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

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_sha256_starts(&ctx, 0) != 0) {
#else
    if (mbedtls_sha256_starts_ret(&ctx, 0) != 0) {
#endif
        ret_val = -1;
        goto error;
    }

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_sha256_update(&ctx, input, network_name_len + GTK_LEN) != 0) {
#else
    if (mbedtls_sha256_update_ret(&ctx, input, network_name_len + GTK_LEN) != 0) {
#endif
        ret_val = -1;
        goto error;
    }

    uint8_t output[32];

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_sha256_finish(&ctx, output) != 0) {
#else
    if (mbedtls_sha256_finish_ret(&ctx, output) != 0) {
#endif
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

    /* Stores frame counters if incremented by threshold and removes network keys from PAE
       controller and MAC */
    ws_pae_controller_frame_counter_store_and_nw_keys_remove(interface_ptr, controller, true);
}

static void ws_pae_controller_frame_counter_store_and_nw_keys_remove(protocol_interface_info_entry_t *interface_ptr, pae_controller_t *controller, bool use_threshold)
{
    /* Checks if frame counters needs to be stored when keys are removed */
    ws_pae_controller_frame_counter_store(controller, use_threshold);

    tr_info("NW keys remove");

    controller->gtk_index = -1;

    nw_key_t *nw_key = controller->nw_key;
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        // Deletes the key if it is set
        if (nw_key[i].set) {
            tr_info("NW key remove: %i", i);
            if (nw_key[i].installed) {
                controller->nw_key_clear(interface_ptr, i);
            }
            nw_key[i].set = false;
            nw_key[i].installed = false;
        }
    }
}

#ifdef HAVE_PAE_AUTH
static void ws_pae_controller_nw_key_index_check_and_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return;
    }

    if (controller->nw_send_key_index_set) {
        controller->gtk_index = index;
        /* Checks if frame counters needs to be stored for the new GTK that is taken into
           use; this is the last check that stored counters are in sync before activating key */
        ws_pae_controller_frame_counter_store(controller, true);
        tr_info("NW send key index set: %i", index + 1);
        controller->nw_send_key_index_set(interface_ptr, index);
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
#endif

#ifdef HAVE_PAE_SUPP
static void ws_pae_controller_active_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t index)
{
    pae_controller_t *controller = ws_pae_controller_get(cur);
    if (!controller) {
        return;
    }

    if (controller->nw_send_key_index_set) {
        controller->gtk_index = index;
        /* Checks if frame counters needs to be stored for the new GTK that is taken into
           use; this is the last check that stored counters are in sync before activating key */
        ws_pae_controller_frame_counter_store(controller, true);
        // Activates key on MAC
        controller->nw_send_key_index_set(controller->interface_ptr, index);
        tr_info("NW send key index set: %i", index + 1);

    }
}
#endif

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
        ns_dyn_mem_free(controller);
        return -1;
    }

    controller->interface_ptr = interface_ptr;
    controller->nw_key_set = NULL;
    controller->nw_key_clear = NULL;
    controller->nw_send_key_index_set = NULL;
    controller->nw_frame_counter_set = NULL;
    controller->nw_frame_counter_read = NULL;
#ifdef HAVE_PAE_SUPP
    controller->auth_completed = NULL;
    controller->auth_next_target = NULL;
#endif
#ifdef HAVE_PAE_AUTH
    controller->pan_ver_increment = NULL;
    controller->nw_info_updated = NULL;
    controller->congestion_get = NULL;
    controller->ip_addr_get = NULL;
#endif

    memset(&controller->sec_cfg, 0, sizeof(sec_cfg_t));

    ws_pae_controller_data_init(controller);

    ns_list_add_to_end(&pae_controller_list, controller);

    return 0;
}

int8_t ws_pae_controller_configure(protocol_interface_info_entry_t *interface_ptr, struct ws_sec_timer_cfg_s *sec_timer_cfg, struct ws_sec_prot_cfg_s *sec_prot_cfg, struct ws_timing_cfg_s *timing_cfg)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (controller == NULL) {
        return 0;
    }

    if (sec_prot_cfg) {
        controller->sec_cfg.prot_cfg.sec_prot_trickle_params.Imin = sec_prot_cfg->sec_prot_trickle_imin * 10;
        controller->sec_cfg.prot_cfg.sec_prot_trickle_params.Imax = sec_prot_cfg->sec_prot_trickle_imax * 10;
        controller->sec_cfg.prot_cfg.sec_prot_trickle_params.k = 0;
        controller->sec_cfg.prot_cfg.sec_prot_trickle_params.TimerExpirations = sec_prot_cfg->sec_prot_trickle_timer_exp;
        controller->sec_cfg.prot_cfg.sec_prot_retry_timeout = sec_prot_cfg->sec_prot_retry_timeout * 10;
        controller->sec_cfg.prot_cfg.initial_key_retry_min = sec_prot_cfg->initial_key_retry_min;
        controller->sec_cfg.prot_cfg.initial_key_retry_max = sec_prot_cfg->initial_key_retry_max;
        controller->sec_cfg.prot_cfg.initial_key_retry_max_limit = sec_prot_cfg->initial_key_retry_max_limit;
        controller->sec_cfg.prot_cfg.initial_key_retry_cnt = sec_prot_cfg->initial_key_retry_cnt;
    }

    if (sec_timer_cfg) {
        ws_pae_timers_settings_init(&controller->sec_cfg.timer_cfg, sec_timer_cfg);
    }

    controller->sec_cfg.radius_cfg = pae_controller_config.radius_cfg;

    if (timing_cfg) {
        controller->sec_cfg.timing_cfg.temp_eapol_min_timeout = timing_cfg->temp_eapol_min_timeout;
    }

    return 0;
}

static void ws_pae_controller_data_init(pae_controller_t *controller)
{
#ifdef HAVE_PAE_SUPP
    memset(controller->target_eui_64, 0, 8);
#endif
    memset(controller->br_eui_64, 0, 8);
    memset(controller->gtkhash, 0, 32);

    ws_pae_controller_active_nw_key_clear(&controller->nw_key[0]);
    ws_pae_controller_active_nw_key_clear(&controller->nw_key[1]);
    ws_pae_controller_active_nw_key_clear(&controller->nw_key[2]);
    ws_pae_controller_active_nw_key_clear(&controller->nw_key[3]);

#ifdef HAVE_PAE_SUPP
    controller->target_pan_id = 0xffff;
#endif
    controller->pae_delete = NULL;
    controller->pae_fast_timer = NULL;
    controller->pae_slow_timer = NULL;
    controller->pae_br_addr_write = NULL;
    controller->pae_br_addr_read = NULL;
    controller->pae_gtks_updated = NULL;
    controller->pae_gtk_hash_update = NULL;
    controller->pae_nw_key_index_update = NULL;
    controller->pae_nw_info_set = NULL;
    controller->gtks_set = false;
    controller->gtkhash_set = false;
    controller->key_index_set = false;
    controller->frame_counter_read = false;
    controller->gtk_index = -1;
    controller->frame_cnt_store_timer = FRAME_COUNTER_STORE_INTERVAL;
    controller->frame_cnt_store_force_timer = FRAME_COUNTER_STORE_FORCE_INTERVAL;
    controller->restart_cnt = 0;
    controller->auth_started = false;
    ws_pae_controller_frame_counter_reset(&controller->frame_counters);
    sec_prot_keys_gtks_init(&controller->gtks);
#ifdef HAVE_PAE_AUTH
    sec_prot_keys_gtks_init(&controller->next_gtks);
#endif
    sec_prot_certs_init(&controller->certs);
    sec_prot_certs_ext_certificate_validation_set(&controller->certs, pae_controller_config.ext_cert_valid_enabled);
    ws_pae_controller_keys_nw_info_init(&controller->sec_keys_nw_info, &controller->gtks);
}

static int8_t ws_pae_controller_frame_counter_read(pae_controller_t *controller)
{
    int8_t ret_value = 0;

    if (controller->frame_counter_read) {
        return ret_value;
    }
    controller->frame_counter_read = true;

    uint64_t stored_time = 0;

    // Read frame counters
    if (ws_pae_controller_nvm_frame_counter_read(&controller->restart_cnt, &stored_time, &controller->sec_keys_nw_info.pan_version, &controller->frame_counters) >= 0) {
        // Check if stored time is not valid
        if (ws_pae_stored_time_check_and_set(stored_time) < 0) {
            ret_value = -1;
        }
        // This is used to ensure that PMK replay counters are fresh after each re-start.
        controller->restart_cnt++;

        // Increments PAN version to ensure that it is fresh
        controller->sec_keys_nw_info.pan_version += PAN_VERSION_STORAGE_READ_INCREMENT;

        // Checks frame counters
        for (uint8_t index = 0; index < GTK_NUM; index++) {
            if (controller->frame_counters.counter[index].set) {
                // If there is room on frame counter space
                if (controller->frame_counters.counter[index].frame_counter < (UINT32_MAX - FRAME_COUNTER_INCREMENT * 2)) {
                    // Increments frame counters
                    controller->frame_counters.counter[index].frame_counter += FRAME_COUNTER_INCREMENT;
                } else {
                    tr_error("Frame counter space exhausted");
                    controller->frame_counters.counter[index].frame_counter = UINT32_MAX;
                }
                controller->frame_counters.counter[index].stored_frame_counter =
                    controller->frame_counters.counter[index].frame_counter;

                tr_info("Read frame counter: index %i value %"PRIu32"", index, controller->frame_counters.counter[index].frame_counter);
            }
        }
    }

    return ret_value;
}

static void ws_pae_controller_frame_counter_reset(frame_counters_t *frame_counters)
{
    for (uint8_t index = 0; index < GTK_NUM; index++) {
        ws_pae_controller_frame_counter_index_reset(frame_counters, index);
    }
    frame_counters->active_gtk_index = -1;
}

static void ws_pae_controller_frame_counter_index_reset(frame_counters_t *frame_counters, uint8_t index)
{
    memset(frame_counters->counter[index].gtk, 0, GTK_LEN);
    frame_counters->counter[index].frame_counter = 0;
    frame_counters->counter[index].stored_frame_counter = 0;
    frame_counters->counter[index].max_frame_counter_chg = 0;
    frame_counters->counter[index].set = false;
}

static int8_t ws_pae_controller_nw_info_read(pae_controller_t *controller, sec_prot_gtk_keys_t *gtks)
{
    uint8_t nvm_gtk_eui64[8];
    uint64_t system_time = ws_pae_current_time_get();

    uint8_t system_time_changed = controller->sec_keys_nw_info.system_time_changed;
    if (ws_pae_controller_nvm_nw_info_read(controller->interface_ptr, &controller->sec_keys_nw_info.key_pan_id, controller->sec_keys_nw_info.network_name, nvm_gtk_eui64, gtks, system_time, &controller->sec_keys_nw_info.system_time_changed) < 0) {
        // If no stored GTKs and network info (pan_id and network name) exits
        return -1;
    }
    if (system_time_changed != controller->sec_keys_nw_info.system_time_changed) {
        controller->sec_keys_nw_info.updated = true;
    }

    /* Get own EUI-64 and compare to the one read from the NVM. In case of mismatch delete GTKs and make
       full authentication to update keys with new EUI-64 and in case of authenticator to update new
       authenticator EUI-64 to the network. */
    uint8_t gtk_eui64[8] = {0};
    link_layer_address_s mac_params;
    if (arm_nwk_mac_address_read(controller->interface_ptr->id, &mac_params) >= 0) {
        memcpy(gtk_eui64, mac_params.mac_long, 8);
    }
    if (memcmp(nvm_gtk_eui64, gtk_eui64, 8) != 0) {
        tr_warn("NVM EUI-64 mismatch, current: %s stored: %s", tr_array(gtk_eui64, 8), tr_array(nvm_gtk_eui64, 8));
        sec_prot_keys_gtks_clear(gtks);
    }

    // Sets also new pan_id used for pan_id set by bootstrap
    controller->sec_keys_nw_info.new_pan_id = controller->sec_keys_nw_info.key_pan_id;

    return 0;
}

static int8_t ws_pae_controller_nvm_nw_info_write(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks, uint64_t stored_time, uint8_t time_changed)
{
    nw_info_nvm_tlv_t *tlv = (nw_info_nvm_tlv_t *) ws_pae_controller_nvm_tlv_get(interface_ptr);
    if (!tlv) {
        return -1;
    }

    ws_pae_nvm_store_nw_info_tlv_create(tlv, pan_id, network_name, gtk_eui64, gtks, stored_time, time_changed);

    ws_pae_nvm_store_tlv_file_write(NW_INFO_FILE, (nvm_tlv_t *) tlv);

    return 0;
}

static int8_t ws_pae_controller_nvm_nw_info_read(protocol_interface_info_entry_t *interface_ptr, uint16_t *pan_id, char *network_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks, uint64_t current_time, uint8_t *time_changed)
{
    nw_info_nvm_tlv_t *tlv_entry = (nw_info_nvm_tlv_t *) ws_pae_controller_nvm_tlv_get(interface_ptr);
    if (!tlv_entry) {
        return -1;
    }

    ws_pae_nvm_store_generic_tlv_create((nvm_tlv_t *) tlv_entry, PAE_NVM_NW_INFO_TAG, PAE_NVM_NW_INFO_LEN);

    if (ws_pae_nvm_store_tlv_file_read(NW_INFO_FILE, (nvm_tlv_t *) tlv_entry) < 0) {
        return -1;
    }

    if (ws_pae_nvm_store_nw_info_tlv_read(tlv_entry, pan_id, network_name, gtk_eui64, gtks, current_time, time_changed) < 0) {
        return -1;
    }

    return 0;
}

int8_t ws_pae_controller_supp_init(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_supp_init(controller->interface_ptr, &controller->certs, &controller->sec_cfg, &controller->sec_keys_nw_info) < 0) {
        return -1;
    }

    controller->pae_delete = ws_pae_supp_delete;
    controller->pae_fast_timer = ws_pae_supp_fast_timer;
    controller->pae_slow_timer = ws_pae_supp_slow_timer;
    controller->pae_br_addr_write = ws_pae_supp_border_router_addr_write;
    controller->pae_br_addr_read = ws_pae_supp_border_router_addr_read;
    controller->pae_gtk_hash_update = ws_pae_supp_gtk_hash_update;
    controller->pae_nw_key_index_update = ws_pae_supp_nw_key_index_update;
    controller->pae_nw_info_set = ws_pae_supp_nw_info_set;

    ws_pae_supp_cb_register(controller->interface_ptr, controller->auth_completed, controller->auth_next_target, ws_pae_controller_nw_key_check_and_insert, ws_pae_controller_active_nw_key_set, ws_pae_controller_gtk_hash_ptr_get, ws_pae_controller_nw_info_updated_check);

    ws_pae_controller_frame_counter_read(controller);
    ws_pae_controller_nw_info_read(controller, controller->sec_keys_nw_info.gtks);
    // Set active key back to fresh so that it can be used again after re-start
    sec_prot_keys_gtk_status_active_to_fresh_set(&controller->gtks);
    sec_prot_keys_gtks_updated_reset(&controller->gtks);

    return 0;
}

int8_t ws_pae_controller_auth_init(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    if (ws_pae_auth_init(controller->interface_ptr, &controller->next_gtks, &controller->certs, &controller->sec_cfg, &controller->sec_keys_nw_info, &controller->frame_counters) < 0) {
        return -1;
    }

    // For Border Router enable the time acquired as default
    ns_time_system_time_acquired_set();

    controller->pae_delete = ws_pae_auth_delete;
    controller->pae_fast_timer = ws_pae_auth_fast_timer;
    controller->pae_slow_timer = ws_pae_auth_slow_timer;
    controller->pae_gtks_updated = ws_pae_auth_gtks_updated;
    controller->pae_nw_key_index_update = ws_pae_auth_nw_key_index_update;
    controller->pae_nw_info_set = ws_pae_auth_nw_info_set;

    sec_prot_gtk_keys_t *read_gtks_to = controller->sec_keys_nw_info.gtks;
    if (ws_pae_controller_frame_counter_read(controller) < 0) {
        tr_error("Stored key material invalid");
        // Key material invalid, do not read GTKs or any other security data
        read_gtks_to = NULL;
    }

#ifdef HAVE_PAE_AUTH
    if (sec_prot_keys_gtks_are_updated(&controller->gtks)) {
        // If application has set GTK keys prepare those for use
        ws_pae_auth_gtks_updated(interface_ptr);
        if (controller->gtk_index >= 0) {
            controller->pae_nw_key_index_update(interface_ptr, controller->gtk_index);
        }
        sec_prot_keys_gtks_updated_reset(&controller->gtks);
    }
    if (read_gtks_to && ws_pae_controller_nw_info_read(controller, read_gtks_to) >= 0) {
        /* If network information i.e pan_id and network name exists updates bootstrap with it,
           (in case already configured by application then no changes are made) */
        if (controller->nw_info_updated) {
            controller->nw_info_updated(interface_ptr, controller->sec_keys_nw_info.key_pan_id, controller->sec_keys_nw_info.pan_version, controller->sec_keys_nw_info.network_name);
        }
        if (sec_prot_keys_gtk_count(read_gtks_to) == 0) {
            // Key material invalid or GTKs are expired, delete GTKs from NVM
            uint8_t gtk_eui64[8] = {0}; // Set GTK EUI-64 to zero
            uint64_t system_time = ws_pae_current_time_get();
            ws_pae_controller_nvm_nw_info_write(controller->interface_ptr, controller->sec_keys_nw_info.key_pan_id, controller->sec_keys_nw_info.network_name, gtk_eui64, NULL, system_time, controller->sec_keys_nw_info.system_time_changed);
        }
    }
#endif
    ws_pae_key_storage_init();

    if (read_gtks_to) {
        ws_pae_key_storage_read(controller->restart_cnt);
    } else {
        // Key material invalid, delete key storage
        ws_pae_key_storage_remove();
    }

    return 0;
}

int8_t ws_pae_controller_stop(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return -1;
    }

    // Stores frame counters and removes network keys from PAE controller and MAC
    ws_pae_controller_frame_counter_store_and_nw_keys_remove(interface_ptr, controller, false);

    // Store security key network info if it has been modified
    ws_pae_controller_nw_info_updated_check(interface_ptr);

    // Store key storage if it has been modified
    ws_pae_key_storage_store();

    // Delete key storage
    ws_pae_key_storage_delete();

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

        // Updates the length of own certificates
        entry->certs.own_cert_chain_len = sec_prot_certs_cert_chain_entry_len_get(&entry->certs.own_cert_chain);
    }

    return 0;
}

int8_t ws_pae_controller_own_certificate_add(const arm_certificate_entry_s *cert)
{
    if (!cert) {
        return -1;
    }

    int8_t ret = -1;

    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        for (uint8_t i = 0; i < SEC_PROT_CERT_CHAIN_DEPTH; i++) {
            if (entry->certs.own_cert_chain.cert[i] == NULL) {
                sec_prot_certs_cert_set(&entry->certs.own_cert_chain, i, (uint8_t *) cert->cert, cert->cert_len);
                // Set private key if set for the certificate that is added
                if (cert->key && cert->key_len > 0) {
                    sec_prot_certs_priv_key_set(&entry->certs.own_cert_chain, (uint8_t *) cert->key, cert->key_len);
                }
                ret = 0;
                break;
            }
        }
        // Updates the length of own certificates
        entry->certs.own_cert_chain_len = sec_prot_certs_cert_chain_entry_len_get(&entry->certs.own_cert_chain);
    }

    return ret;
}

int8_t ws_pae_controller_own_certificates_remove(void)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        sec_prot_certs_chain_entry_init(&entry->certs.own_cert_chain);
        entry->certs.own_cert_chain_len = 0;
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

int8_t ws_pae_controller_trusted_certificates_remove(void)
{
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        sec_prot_certs_chain_list_delete(&entry->certs.trusted_cert_chain_list);
    }

    return 0;
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

sec_radius_cfg_t *ws_pae_controller_radius_config_get(void)
{
    if (pae_controller_config.radius_cfg != NULL) {
        return pae_controller_config.radius_cfg;
    }

    pae_controller_config.radius_cfg = ns_dyn_mem_alloc(sizeof(sec_radius_cfg_t));
    if (pae_controller_config.radius_cfg == NULL) {
        return NULL;
    }

    pae_controller_config.radius_cfg->radius_retry_trickle_params.Imin = RADIUS_CLIENT_RETRY_IMIN;
    pae_controller_config.radius_cfg->radius_retry_trickle_params.Imax = RADIUS_CLIENT_RETRY_IMAX;
    pae_controller_config.radius_cfg->radius_retry_trickle_params.k = 0;
    pae_controller_config.radius_cfg->radius_retry_trickle_params.TimerExpirations = RADIUS_CLIENT_TIMER_EXPIRATIONS;

    pae_controller_config.radius_cfg->radius_addr_set = false;
    pae_controller_config.radius_cfg->radius_shared_secret_len = 0;
    pae_controller_config.radius_cfg->radius_shared_secret = NULL;

    return pae_controller_config.radius_cfg;
}

int8_t ws_pae_controller_radius_address_set(int8_t interface_id, const uint8_t *address)
{
    sec_radius_cfg_t *radius_cfg = ws_pae_controller_radius_config_get();
    if (radius_cfg == NULL) {
        return -1;
    }

    if (address != NULL) {
        memcpy(radius_cfg->radius_addr, address, 16);
        radius_cfg->radius_addr_set = true;
    } else {
        radius_cfg->radius_addr_set = false;
    }

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return 0;
    }

    if (ws_pae_controller_configure(controller->interface_ptr, NULL, NULL, NULL) < 0) {
        return -1;
    }

    if (!radius_cfg->radius_addr_set) {
        return 0;
    }

    if (ws_pae_auth_radius_address_set(controller->interface_ptr, radius_cfg->radius_addr) < 0) {
        // If not set here since authenticator not created, then set on authenticator initialization
        return 0;
    }

    return 0;
}

int8_t ws_pae_controller_radius_address_get(int8_t interface_id, uint8_t *address)
{
    (void) interface_id;

    if (address == NULL) {
        return -1;
    }

    sec_radius_cfg_t *radius_cfg = ws_pae_controller_radius_config_get();
    if (radius_cfg == NULL) {
        return -1;
    }

    if (!radius_cfg->radius_addr_set) {
        return -1;
    }

    memcpy(address, radius_cfg->radius_addr, 16);

    return 0;
}

int8_t ws_pae_controller_radius_shared_secret_set(int8_t interface_id, const uint16_t shared_secret_len, const uint8_t *shared_secret)
{
    sec_radius_cfg_t *radius_cfg = ws_pae_controller_radius_config_get();
    if (radius_cfg == NULL) {
        return -1;
    }

    radius_cfg->radius_shared_secret = shared_secret;
    radius_cfg->radius_shared_secret_len = shared_secret_len;

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (controller) {
        ws_pae_controller_configure(controller->interface_ptr, NULL, NULL, NULL);
    }

    return 0;
}

int8_t ws_pae_controller_radius_shared_secret_get(int8_t interface_id, uint16_t *shared_secret_len, uint8_t *shared_secret)
{
    (void) interface_id;

    if (shared_secret_len == NULL) {
        return -1;
    }

    sec_radius_cfg_t *radius_cfg = ws_pae_controller_radius_config_get();
    if (radius_cfg == NULL) {
        return -1;
    }

    uint16_t length = radius_cfg->radius_shared_secret_len;
    if (shared_secret != NULL) {
        if (length > *shared_secret_len) {
            length = *shared_secret_len;
        }
        if (length > 0 && radius_cfg->radius_shared_secret != NULL) {
            memcpy(shared_secret, radius_cfg->radius_shared_secret, length);
        }
    }

    *shared_secret_len = length;

    return 0;
}

int8_t ws_pae_controller_radius_timing_set(int8_t interface_id, bbr_radius_timing_t *timing)
{
    (void) interface_id;

    if (timing == NULL) {
        return -1;
    }

    sec_radius_cfg_t *radius_cfg = ws_pae_controller_radius_config_get();
    if (radius_cfg == NULL) {
        return -1;
    }

    radius_cfg->radius_retry_trickle_params.Imin = timing->radius_retry_imin;
    radius_cfg->radius_retry_trickle_params.Imax = timing->radius_retry_imax;
    radius_cfg->radius_retry_trickle_params.TimerExpirations = timing->radius_retry_count;

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (controller) {
        ws_pae_controller_configure(controller->interface_ptr, NULL, NULL, NULL);
    }

    return 0;
}

int8_t ws_pae_controller_radius_timing_get(int8_t interface_id, bbr_radius_timing_t *timing)
{
    (void) interface_id;

    if (timing == NULL) {
        return -1;
    }

    if (pae_controller_config.radius_cfg == NULL) {
        timing->radius_retry_imin = RADIUS_CLIENT_RETRY_IMIN;
        timing->radius_retry_imax = RADIUS_CLIENT_RETRY_IMAX;
        timing->radius_retry_count = RADIUS_CLIENT_TIMER_EXPIRATIONS;
        return 0;
    }

    sec_radius_cfg_t *radius_cfg = ws_pae_controller_radius_config_get();
    if (radius_cfg == NULL) {
        return -1;
    }

    timing->radius_retry_imin = radius_cfg->radius_retry_trickle_params.Imin;
    timing->radius_retry_imax = radius_cfg->radius_retry_trickle_params.Imax;
    timing->radius_retry_count = radius_cfg->radius_retry_trickle_params.TimerExpirations;

    return 0;
}

int8_t ws_pae_controller_radius_timing_validate(int8_t interface_id, bbr_radius_timing_t *timing)
{
    (void) interface_id;

    if (timing == NULL) {
        return -1;
    }

    if (timing->radius_retry_imin == 0 || timing->radius_retry_imax == 0 ||
            timing->radius_retry_imin > timing->radius_retry_imax) {
        return -1;
    }

    return 0;
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

int8_t ws_pae_controller_gtk_update(int8_t interface_id, uint8_t *gtk[GTK_NUM])
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
            lifetime += controller->sec_cfg.timer_cfg.gtk_expire_offset;
            if (sec_prot_keys_gtk_set(&controller->gtks, i, gtk[i], lifetime) >= 0) {
                controller->gtks_set = true;
                tr_info("GTK set index: %i, lifetime %"PRIu32", system time: %"PRIu32"", i, lifetime, protocol_core_monotonic_time / 10);
            }
        }
    }

    // Sets active key
    int8_t index = sec_prot_keys_gtk_install_order_first_index_get(&controller->gtks);
    sec_prot_keys_gtk_status_active_set(&controller->gtks, index);

    // Notifies PAE authenticator that GTKs have been updated */
    if (controller->pae_gtks_updated) {
        controller->pae_gtks_updated(controller->interface_ptr);
    }

    return 0;
}

int8_t ws_pae_controller_next_gtk_update(int8_t interface_id, uint8_t *gtk[GTK_NUM])
{
#ifdef HAVE_PAE_AUTH
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
#else
    (void) interface_id;
    (void) gtk;
    return -1;
#endif
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

int8_t ws_pae_controller_node_limit_set(int8_t interface_id, uint16_t limit)
{
#ifdef HAVE_PAE_AUTH
    pae_controller_config.node_limit = limit;
    pae_controller_config.node_limit_set = true;

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    ws_pae_auth_node_limit_set(controller->interface_ptr, limit);

    return 0;
#else
    (void) interface_id;
    (void) limit;
    return -1;
#endif
}

int8_t ws_pae_controller_ext_certificate_validation_set(int8_t interface_id, bool enabled)
{
#ifdef HAVE_PAE_AUTH
    pae_controller_config.ext_cert_valid_enabled = enabled;

    pae_controller_t *controller = ws_pae_controller_get_or_create(interface_id);
    if (!controller) {
        return -1;
    }

    sec_prot_certs_ext_certificate_validation_set(&controller->certs, enabled);

    return 0;
#else
    (void) interface_id;
    (void) enabled;
    return -1;
#endif
}

void ws_pae_controller_forced_gc(bool full_gc)
{
    /* Purge only when on critical limit since node limit should handle limiting
       of entries in normal case */
    if (!full_gc) {
        return;
    }

    // Purge authenticators for each interface
    ns_list_foreach(pae_controller_t, entry, &pae_controller_list) {
        ws_pae_auth_forced_gc(entry->interface_ptr);
    }
}

#ifdef HAVE_PAE_AUTH
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
#endif

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
        return controller->pae_gtk_hash_update(interface_ptr, controller->gtkhash, true);
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
        ws_pae_controller_frame_counter_timer(seconds, entry);
    }

    ws_pae_current_time_update(seconds);
}

static void ws_pae_controller_frame_counter_timer(uint16_t seconds, pae_controller_t *entry)
{
    if (entry->frame_cnt_store_timer > seconds) {
        entry->frame_cnt_store_timer -= seconds;
    } else {
        entry->frame_cnt_store_timer = FRAME_COUNTER_STORE_INTERVAL;
        ws_pae_controller_frame_counter_store(entry, true);
    }

    if (entry->frame_cnt_store_force_timer > seconds) {
        entry->frame_cnt_store_force_timer -= seconds;
    } else {
        entry->frame_cnt_store_force_timer = 0;
        ws_pae_controller_frame_counter_store(entry, true);
    }
}

static void ws_pae_controller_frame_counter_store(pae_controller_t *entry, bool use_threshold)
{
    bool update_needed = false;

    for (int i = 0; i < GTK_NUM; i++) {
        /* If network key is set, checks if frame counter needs to be updated to NVM
         * Note! The frame counters for non-installed keys (previous frame counters) are not changed.
         *       This is because GTKs are removed e.g. if PAN configuration is not heard/cannot be
         *       de-crypted during a bootstrap. If BR later installs previous keys using 4WH/GKH, the
         *       frame counters will be still valid.
         */
        if (entry->nw_key[i].installed) {
            // Reads MAC frame counter for the key
            uint32_t curr_frame_counter;
            entry->nw_frame_counter_read(entry->interface_ptr, &curr_frame_counter, i);

            // If frame counter for the network key has already been stored
            if (entry->frame_counters.counter[i].set &&
                    memcmp(entry->nw_key[i].gtk, entry->frame_counters.counter[i].gtk, GTK_LEN) == 0) {

                if (curr_frame_counter > entry->frame_counters.counter[i].frame_counter) {
                    entry->frame_counters.counter[i].frame_counter = curr_frame_counter;
                }
                uint32_t frame_counter = entry->frame_counters.counter[i].frame_counter;

                /* If threshold check is disabled or frame counter has advanced for the threshold value, stores the new value.
                   If frame counter is at maximum at storage, do not initiate storing */
                if (!use_threshold || (
                            (frame_counter > entry->frame_counters.counter[i].stored_frame_counter + FRAME_COUNTER_STORE_THRESHOLD) &&
                            !(entry->frame_counters.counter[i].stored_frame_counter == UINT32_MAX &&
                              frame_counter >= UINT32_MAX - FRAME_COUNTER_STORE_THRESHOLD))) {
                    entry->frame_counters.counter[i].stored_frame_counter = frame_counter;
                    update_needed = true;
                    tr_debug("Stored updated frame counter: index %i value %"PRIu32"", i, frame_counter);
                }
            } else {
                // New or modified network key
                entry->frame_counters.counter[i].set = true;
                memcpy(entry->frame_counters.counter[i].gtk, entry->nw_key[i].gtk, GTK_LEN);
                entry->frame_counters.counter[i].frame_counter = curr_frame_counter;
                entry->frame_counters.counter[i].stored_frame_counter = curr_frame_counter;
                tr_debug("Pending to store new frame counter: index %i value %"PRIu32"", i, curr_frame_counter);
            }

            /* If currently active key is changed or active key is set for the first time,
               stores the frame counter value */
            if (entry->gtk_index == i && entry->frame_counters.active_gtk_index != i) {
                entry->frame_counters.active_gtk_index = entry->gtk_index;
                update_needed = true;
                // Updates MAC frame counter for the key
                entry->nw_frame_counter_set(entry->interface_ptr, entry->frame_counters.counter[i].frame_counter, i);
                tr_debug("Stored frame counters, active key set: index %i value %"PRIu32"", i, entry->frame_counters.counter[i].frame_counter);
            }
        }
    }

    if (update_needed || entry->frame_cnt_store_force_timer == 0) {
        tr_debug("Write frame counters: system time %"PRIu32"", protocol_core_monotonic_time / 10);
        uint64_t system_time = ws_pae_current_time_get();
        // Writes modified frame counters
        ws_pae_nvm_store_frame_counter_tlv_create((frame_cnt_nvm_tlv_t *) &entry->pae_nvm_buffer, entry->restart_cnt, entry->sec_keys_nw_info.pan_version, &entry->frame_counters, system_time);
        ws_pae_controller_nvm_frame_counter_write((frame_cnt_nvm_tlv_t *) &entry->pae_nvm_buffer);

        // Reset force interval when ever values are stored
        entry->frame_cnt_store_force_timer = FRAME_COUNTER_STORE_FORCE_INTERVAL;
    }
}

static int8_t ws_pae_controller_nvm_frame_counter_read(uint32_t *restart_cnt, uint64_t *stored_time, uint16_t *pan_version, frame_counters_t *counters)
{
    frame_cnt_nvm_tlv_t *tlv = (frame_cnt_nvm_tlv_t *) ws_pae_nvm_store_generic_tlv_allocate_and_create(
                                   PAE_NVM_FRAME_COUNTER_TAG, PAE_NVM_FRAME_COUNTER_LEN);
    if (!tlv) {
        return -1;
    }

    if (ws_pae_nvm_store_tlv_file_read(FRAME_COUNTER_FILE, (nvm_tlv_t *) tlv) < 0) {
        ws_pae_nvm_store_generic_tlv_free((nvm_tlv_t *) tlv);
        return -1;
    }

    if (ws_pae_nvm_store_frame_counter_tlv_read(tlv, restart_cnt, stored_time, pan_version, counters) < 0) {
        ws_pae_nvm_store_generic_tlv_free((nvm_tlv_t *) tlv);
        return -1;
    }

    ws_pae_nvm_store_generic_tlv_free((nvm_tlv_t *) tlv);

    return 0;
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

nvm_tlv_t *ws_pae_controller_nvm_tlv_get(protocol_interface_info_entry_t *interface_ptr)
{
    pae_controller_t *controller = ws_pae_controller_get(interface_ptr);
    if (!controller) {
        return NULL;
    }

    return (nvm_tlv_t *) &controller->pae_nvm_buffer;
}

static void ws_pae_controller_nvm_frame_counter_write(frame_cnt_nvm_tlv_t *tlv_entry)
{
    ws_pae_nvm_store_tlv_file_write(FRAME_COUNTER_FILE, (nvm_tlv_t *) tlv_entry);

}

#endif /* HAVE_WS */

