/*
 * Copyright (c) 2020-2021, Pelion and affiliates.
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

#include <string.h>
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "net_interface.h"
#include "eventOS_event.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "ws_management_api.h"
#include "MPL/mpl.h"

#define TRACE_GROUP "cstr"

#ifdef HAVE_WS

#define CFG_SETTINGS_OK                       0
#define CFG_SETTINGS_CHANGED                  1

#define CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE   0x01
#define CFG_FLAGS_BOOTSTRAP_SET_VALUES        0x02

#define TRICKLE_IMIN_60_SECS 60
#define TRICKLE_IMIN_30_SECS 30
#define TRICKLE_IMIN_15_SECS 15

typedef struct ws_cfg_nw_size_s {
    ws_gen_cfg_t gen;                   /**< General configuration */
    ws_timing_cfg_t timing;             /**< Timing configuration */
    ws_bbr_cfg_t bbr;                   /**< RPL configuration */
    ws_sec_prot_cfg_t sec_prot;         /**< Security protocols configuration */
    ws_mpl_cfg_t mpl;                   /**< Multicast timing configuration*/
} ws_cfg_nw_size_t;

static uint32_t ws_test_temporary_entry_lifetime = 0;
typedef int8_t (*ws_cfg_default_set)(void *cfg);
typedef int8_t (*ws_cfg_validate)(void *new_cfg);
typedef int8_t (*ws_cfg_set)(protocol_interface_info_entry_t *cur, void *new_cfg, uint8_t flags);

typedef struct {
    ws_cfg_default_set default_set;
    ws_cfg_validate validate;
    ws_cfg_set set;
    uint16_t setting_offset;
} ws_cfg_cb_t;

typedef union {
    ws_gen_cfg_t gen;
    ws_phy_cfg_t phy;
    ws_timing_cfg_t timing;
    ws_bbr_cfg_t bbr;
    ws_fhss_cfg_t fhss;
    ws_mpl_cfg_t mpl;
    ws_sec_timer_cfg_t sec_timer;
    ws_sec_prot_cfg_t sec_prot;
} ws_cfgs_t;


typedef struct cfg_devices_in_config {
    uint8_t max_for_small;
    uint8_t max_for_medium;
    uint8_t max_for_large;
    uint8_t max_for_xlarge;
} cfg_devices_in_config_t;

/* Table for amount of devices that certain configuration should be used
 *
 * larger data rates allow more devices to be used with faster settings.
 *
 * For example with network the size of 2000 devices we use
 * Xlrage configuration with 50kbs data rate.
 * Large configuration with 300kbs data rate.
 * and with 600kbs data rate it is possible to use medium network settings.
 *
 */
const cfg_devices_in_config_t devices_by_datarate[] = {
    { 1,  5, 10,  25}, // Configuration for 50 -100kbs
    { 1, 10, 20,  40}, // Configuration for 150kbs - 200kbs
    { 1, 15, 30,  60}, // Configuration for 300kbs
    { 2, 20, 50, 100}, // Configuration for 600kbs - 2400kbs
};

static void ws_cfg_network_size_config_set_small(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_medium(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_large(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_xlarge(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_certificate(ws_cfg_nw_size_t *cfg);
static int8_t ws_cfg_network_size_default_set(ws_gen_cfg_t *cfg);
static int8_t ws_cfg_gen_default_set(ws_gen_cfg_t *cfg);
static int8_t ws_cfg_bbr_default_set(ws_bbr_cfg_t *cfg);
static int8_t ws_cfg_mpl_default_set(ws_mpl_cfg_t *cfg);
static int8_t ws_cfg_sec_timer_default_set(ws_sec_timer_cfg_t *cfg);
static int8_t ws_cfg_sec_prot_default_set(ws_sec_prot_cfg_t *cfg);

#define CFG_CB(default_cb, validate_cb, set_cb, offset) \
    {                                                   \
    .default_set = (ws_cfg_default_set) default_cb,     \
    .validate = (ws_cfg_validate) validate_cb,          \
    .set = (ws_cfg_set) set_cb,                         \
    .setting_offset = offset,                           \
    }

// Create validate and set callback table
static const ws_cfg_cb_t cfg_cb[] = {
    // Network size configuration must be done first
    CFG_CB(ws_cfg_network_size_default_set, ws_cfg_network_size_validate, ws_cfg_network_size_set, offsetof(ws_cfg_t, gen)),
    CFG_CB(ws_cfg_gen_default_set, ws_cfg_gen_validate, ws_cfg_gen_set, offsetof(ws_cfg_t, gen)),
    CFG_CB(ws_cfg_phy_default_set, ws_cfg_phy_validate, ws_cfg_phy_set, offsetof(ws_cfg_t, phy)),
    CFG_CB(ws_cfg_timing_default_set, ws_cfg_timing_validate, ws_cfg_timing_set, offsetof(ws_cfg_t, timing)),
    CFG_CB(ws_cfg_bbr_default_set, ws_cfg_bbr_validate, ws_cfg_bbr_set, offsetof(ws_cfg_t, bbr)),
    CFG_CB(ws_cfg_mpl_default_set, ws_cfg_mpl_validate, ws_cfg_mpl_set, offsetof(ws_cfg_t, mpl)),
    CFG_CB(ws_cfg_fhss_default_set, ws_cfg_fhss_validate, ws_cfg_fhss_set, offsetof(ws_cfg_t, fhss)),
    CFG_CB(ws_cfg_sec_timer_default_set, ws_cfg_sec_timer_validate, ws_cfg_sec_timer_set, offsetof(ws_cfg_t, sec_timer)),
    CFG_CB(ws_cfg_sec_prot_default_set, ws_cfg_sec_prot_validate, ws_cfg_sec_prot_set, offsetof(ws_cfg_t, sec_prot)),
};

#define CFG_CB_NUM (sizeof(cfg_cb) / sizeof(ws_cfg_cb_t))

// Wisun configuration storage
ws_cfg_t ws_cfg;

#ifdef FEA_TRACE_SUPPORT
static void ws_cfg_trace(ws_cfgs_t *cfg, ws_cfgs_t *new_cfg, uint8_t size, char *name)
{
    uint8_t *start = 0;
    uint8_t *end = 0;

    tr_debug("config set: %s, changed fields:", name);

    bool print_index = true;
    for (uint8_t i = 0; i < size; i++) {
        if (((uint8_t *) cfg)[i] != ((uint8_t *) new_cfg)[i]) {
            if (print_index) {
                start = &((uint8_t *) new_cfg)[i];
                print_index = false;
            }
            end = &((uint8_t *) new_cfg)[i];
        } else {
            if (start && end) {
                tr_debug("i: %p v: %s ", (void *)(start - ((uint8_t *) new_cfg)), trace_array(start, end - start + 1));
            }
            start = NULL;
            end = NULL;
            print_index = true;
        }
    }

    if (start && end) {
        tr_debug("i: %p v: %s ", (void *)(start - ((uint8_t *) new_cfg)), trace_array(start, end - start + 1));
    }
}
#else
#define ws_cfg_trace(cfg, new_cfg, size, name)
#endif

static int8_t ws_cfg_network_size_default_set(ws_gen_cfg_t *cfg)
{
    cfg->network_size = NETWORK_SIZE_MEDIUM;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_network_size_get(ws_gen_cfg_t *cfg)
{
    *cfg = ws_cfg.gen;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_network_size_validate(ws_gen_cfg_t *new_cfg)
{
    ws_gen_cfg_t *cfg = &ws_cfg.gen;
    if (cfg->network_size != new_cfg->network_size) {
        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

typedef void (*ws_cfg_network_size_config_set_size)(ws_cfg_nw_size_t *cfg);

int8_t ws_cfg_network_size_set(protocol_interface_info_entry_t *cur, ws_gen_cfg_t *new_cfg, uint8_t flags)
{
    (void) flags;

    if (ws_cfg_network_size_validate(new_cfg) != CFG_SETTINGS_CHANGED) {
        return CFG_SETTINGS_OK;
    }

    ws_gen_cfg_t *cfg = &ws_cfg.gen;

    cfg->network_size = new_cfg->network_size;

    ws_cfg_nw_size_t nw_size_cfg;
    ws_cfg_gen_get(&nw_size_cfg.gen);
    ws_cfg_timing_get(&nw_size_cfg.timing);
    ws_cfg_bbr_get(&nw_size_cfg.bbr);
    ws_cfg_sec_prot_get(&nw_size_cfg.sec_prot);
    ws_cfg_mpl_get(&nw_size_cfg.mpl);

    ws_cfg_network_size_config_set_size set_function = NULL;

    if (ws_cfg_network_config_get(cur) == CONFIG_CERTIFICATE) {
        set_function = ws_cfg_network_size_config_set_certificate;
    } else if (ws_cfg_network_config_get(cur) == CONFIG_SMALL || cfg->network_size == NETWORK_SIZE_AUTOMATIC) {
        set_function = ws_cfg_network_size_config_set_small;
    } else if (ws_cfg_network_config_get(cur) == CONFIG_MEDIUM) {
        set_function = ws_cfg_network_size_config_set_medium;
    } else if (ws_cfg_network_config_get(cur) == CONFIG_LARGE) {
        set_function = ws_cfg_network_size_config_set_large;
    } else {
        set_function = ws_cfg_network_size_config_set_xlarge;
    }

    // Overrides the values on the new configuration
    if (set_function != NULL) {
        set_function(&nw_size_cfg);
    }

    /* Sets values if changed */
    ws_cfg_gen_set(cur, &nw_size_cfg.gen, 0x00);
    ws_cfg_timing_set(cur, &nw_size_cfg.timing, 0x00);
    ws_cfg_bbr_set(cur, &nw_size_cfg.bbr, 0x00);
    ws_cfg_sec_prot_set(cur, &nw_size_cfg.sec_prot, 0x00);
    ws_cfg_mpl_set(cur, &nw_size_cfg.mpl, 0x00);

    return CFG_SETTINGS_OK;
}

static uint8_t ws_cfg_config_get_by_size(protocol_interface_info_entry_t *cur, uint8_t network_size)
{
    (void)cur;

    ws_phy_cfg_t phy_cfg;
    if (ws_cfg_phy_get(&phy_cfg) < 0) {
        return CONFIG_SMALL;
    }
    uint32_t data_rate = ws_common_datarate_get_from_phy_mode(phy_cfg.phy_mode_id, phy_cfg.operating_mode);

    uint8_t index;
    if (data_rate < 150000) {
        index = 0;
    } else if (data_rate < 300000) {
        index = 1;
    } else if (data_rate < 600000) {
        index = 2;
    } else {
        index = 3;
    }

    if (network_size == NETWORK_SIZE_CERTIFICATE) {
        return CONFIG_CERTIFICATE;
    } else if (network_size <= devices_by_datarate[index].max_for_small) {
        return CONFIG_SMALL;
    } else if (network_size <= devices_by_datarate[index].max_for_medium) {
        return CONFIG_MEDIUM;
    } else if (network_size <= devices_by_datarate[index].max_for_large) {
        return CONFIG_LARGE;
    }
    return CONFIG_XLARGE;
}

cfg_network_size_type_e ws_cfg_network_config_get(protocol_interface_info_entry_t *cur)
{
    // Get size of the network Amount of devices in the network
    // Get the data rate of the network
    // Adjust the configuration type based on the network size and data rate

    (void)cur;

    ws_gen_cfg_t cfg;
    if (ws_cfg_gen_get(&cfg) < 0) {
        return CONFIG_SMALL;
    }

    return ws_cfg_config_get_by_size(cur, cfg.network_size);
}


static void ws_cfg_network_size_config_set_small(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN parent configuration
    cfg->gen.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->gen.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // Configure the Wi-SUN timing trickle parameter
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_15_SECS;       // 15 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_15_SECS << 2;  // 60 seconds
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_SMALL_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;
    cfg->timing.temp_eapol_min_timeout = WS_EAPOL_TEMPORARY_ENTRY_SMALL_TIMEOUT;

    // RPL configuration
    cfg->bbr.dio_interval_min = WS_RPL_DIO_IMIN_SMALL;               // 15; 32s seconds
    cfg->bbr.dio_interval_doublings = WS_RPL_DIO_DOUBLING_SMALL;     // 2; 128
    cfg->bbr.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_SMALL;  // Disabled
    cfg->bbr.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->bbr.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->bbr.rpl_default_lifetime = WS_RPL_DEFAULT_LIFETIME;
    cfg->bbr.dhcp_address_lifetime = WS_DHCP_ADDRESS_LIFETIME_SMALL;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;

    cfg->sec_prot.initial_key_retry_min = SMALL_NW_INITIAL_KEY_RETRY_MIN_SECS;
    cfg->sec_prot.initial_key_retry_max = SMALL_NW_INITIAL_KEY_RETRY_MAX_SECS;
    cfg->sec_prot.initial_key_retry_max_limit = SMALL_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS;
    cfg->sec_prot.initial_key_retry_cnt = SMALL_NW_INITIAL_KEY_RETRY_COUNT;

    // Multicast timing configuration
    cfg->mpl.mpl_trickle_imin = MPL_SMALL_IMIN;
    cfg->mpl.mpl_trickle_imax = MPL_SMALL_IMAX;
    cfg->mpl.mpl_trickle_k = MPL_SMALL_K;
    cfg->mpl.mpl_trickle_timer_exp = MPL_SMALL_EXPIRATIONS;
    cfg->mpl.seed_set_entry_lifetime = MPL_SMALL_SEED_LIFETIME;

}

static void ws_cfg_network_size_config_set_medium(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN parent configuration
    cfg->gen.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->gen.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_60_SECS;       // 60 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_60_SECS << 4;      // 960 seconds; 16 minutes
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_MEDIUM_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;
    cfg->timing.temp_eapol_min_timeout = WS_EAPOL_TEMPORARY_ENTRY_MEDIUM_TIMEOUT;

    // RPL configuration
    cfg->bbr.dio_interval_min = WS_RPL_DIO_IMIN_MEDIUM;              // 17; 128s
    cfg->bbr.dio_interval_doublings = WS_RPL_DIO_DOUBLING_MEDIUM;    // 3; 1024s
    cfg->bbr.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_MEDIUM; // 10
    cfg->bbr.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->bbr.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->bbr.rpl_default_lifetime = WS_RPL_DEFAULT_LIFETIME_MEDIUM;
    cfg->bbr.dhcp_address_lifetime = WS_DHCP_ADDRESS_LIFETIME_MEDIUM;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;

    cfg->sec_prot.initial_key_retry_min = MEDIUM_NW_INITIAL_KEY_RETRY_MIN_SECS;
    cfg->sec_prot.initial_key_retry_max = MEDIUM_NW_INITIAL_KEY_RETRY_MAX_SECS;
    cfg->sec_prot.initial_key_retry_max_limit = MEDIUM_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS;
    cfg->sec_prot.initial_key_retry_cnt = MEDIUM_NW_INITIAL_KEY_RETRY_COUNT;

    // Multicast timing configuration
    cfg->mpl.mpl_trickle_imin = MPL_MEDIUM_IMIN;
    cfg->mpl.mpl_trickle_imax = MPL_MEDIUM_IMAX;
    cfg->mpl.mpl_trickle_k = MPL_MEDIUM_K;
    cfg->mpl.mpl_trickle_timer_exp = MPL_MEDIUM_EXPIRATIONS;
    cfg->mpl.seed_set_entry_lifetime = MPL_MEDIUM_SEED_LIFETIME;
}

static void ws_cfg_network_size_config_set_large(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN parent configuration
    cfg->gen.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->gen.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_60_SECS << 1;       // 120 seconds
    cfg->timing.disc_trickle_imax = 1536;      // 1536 seconds; 25 minutes
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_LARGE_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_LARGE;
    cfg->timing.temp_eapol_min_timeout = WS_EAPOL_TEMPORARY_ENTRY_LARGE_TIMEOUT;

    // RPL configuration
    cfg->bbr.dio_interval_min = WS_RPL_DIO_IMIN_LARGE;               // 18; 256s, 4.5min
    cfg->bbr.dio_interval_doublings = WS_RPL_DIO_DOUBLING_LARGE;     // 3; 2048s, 34min
    cfg->bbr.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_LARGE;  // 10
    cfg->bbr.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->bbr.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->bbr.rpl_default_lifetime = WS_RPL_DEFAULT_LIFETIME_LARGE;
    cfg->bbr.dhcp_address_lifetime = WS_DHCP_ADDRESS_LIFETIME_LARGE;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_LARGE_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_LARGE_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_LARGE;

    cfg->sec_prot.initial_key_retry_min = LARGE_NW_INITIAL_KEY_RETRY_MIN_SECS;
    cfg->sec_prot.initial_key_retry_max = LARGE_NW_INITIAL_KEY_RETRY_MAX_SECS;
    cfg->sec_prot.initial_key_retry_max_limit = LARGE_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS;
    cfg->sec_prot.initial_key_retry_cnt = LARGE_NW_INITIAL_KEY_RETRY_COUNT;

    // Multicast timing configuration
    cfg->mpl.mpl_trickle_imin = MPL_LARGE_IMIN;
    cfg->mpl.mpl_trickle_imax = MPL_LARGE_IMAX;
    cfg->mpl.mpl_trickle_k = MPL_LARGE_K;
    cfg->mpl.mpl_trickle_timer_exp = MPL_LARGE_EXPIRATIONS;
    cfg->mpl.seed_set_entry_lifetime = MPL_LARGE_SEED_LIFETIME;
}

static void ws_cfg_network_size_config_set_xlarge(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN parent configuration
    cfg->gen.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->gen.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_60_SECS << 2;       // 240 seconds
    cfg->timing.disc_trickle_imax = 1920;      // 1920 seconds; 32 minutes
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_XLARGE_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_LARGE;
    cfg->timing.temp_eapol_min_timeout = WS_EAPOL_TEMPORARY_ENTRY_LARGE_TIMEOUT;

    // RPL configuration
    cfg->bbr.dio_interval_min = WS_RPL_DIO_IMIN_XLARGE;               // 18; 262s, 4.5min
    cfg->bbr.dio_interval_doublings = WS_RPL_DIO_DOUBLING_XLARGE;     // 4; 2048s, 34min
    cfg->bbr.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_XLARGE;  // 10
    cfg->bbr.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->bbr.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->bbr.rpl_default_lifetime = WS_RPL_DEFAULT_LIFETIME_XLARGE;
    cfg->bbr.dhcp_address_lifetime = WS_DHCP_ADDRESS_LIFETIME_LARGE;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_LARGE_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_LARGE_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_LARGE;

    cfg->sec_prot.initial_key_retry_min = EXTRA_LARGE_NW_INITIAL_KEY_RETRY_MIN_SECS;
    cfg->sec_prot.initial_key_retry_max = EXTRA_LARGE_NW_INITIAL_KEY_RETRY_MAX_SECS;
    cfg->sec_prot.initial_key_retry_max_limit = EXTRA_LARGE_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS;
    cfg->sec_prot.initial_key_retry_cnt = EXTRA_LARGE_NW_INITIAL_KEY_RETRY_COUNT;

    // Multicast timing configuration
    cfg->mpl.mpl_trickle_imin = MPL_XLARGE_IMIN;
    cfg->mpl.mpl_trickle_imax = MPL_XLARGE_IMAX;
    cfg->mpl.mpl_trickle_k = MPL_XLARGE_K;
    cfg->mpl.mpl_trickle_timer_exp = MPL_XLARGE_EXPIRATIONS;
    cfg->mpl.seed_set_entry_lifetime = MPL_XLARGE_SEED_LIFETIME;
}

static void ws_cfg_network_size_config_set_certificate(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN parent configuration
    cfg->gen.rpl_parent_candidate_max = WS_CERTIFICATE_RPL_PARENT_CANDIDATE_MAX;
    cfg->gen.rpl_selected_parent_max = WS_CERTIFICATE_RPL_SELECTED_PARENT_MAX;

    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_15_SECS;       // 15 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_15_SECS << 2;  // 60 seconds
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_SMALL_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;
    cfg->timing.temp_eapol_min_timeout = WS_EAPOL_TEMPORARY_ENTRY_SMALL_TIMEOUT;

    // RPL configuration (small)
    cfg->bbr.dio_interval_min = WS_RPL_DIO_IMIN_SMALL;               // 15; 32s seconds
    cfg->bbr.dio_interval_doublings = WS_RPL_DIO_DOUBLING_SMALL;     // 2; 128
    cfg->bbr.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_SMALL;  // Disabled
    cfg->bbr.dag_max_rank_increase = WS_CERTIFICATE_RPL_MAX_HOP_RANK_INCREASE;
    cfg->bbr.min_hop_rank_increase = WS_CERTIFICATE_RPL_MIN_HOP_RANK_INCREASE;
    cfg->bbr.rpl_default_lifetime = WS_RPL_DEFAULT_LIFETIME;
    cfg->bbr.dhcp_address_lifetime = WS_DHCP_ADDRESS_LIFETIME_SMALL;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;

    cfg->sec_prot.initial_key_retry_min = SMALL_NW_INITIAL_KEY_RETRY_MIN_SECS;
    cfg->sec_prot.initial_key_retry_max = SMALL_NW_INITIAL_KEY_RETRY_MAX_SECS;
    cfg->sec_prot.initial_key_retry_max_limit = SMALL_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS;
    cfg->sec_prot.initial_key_retry_cnt = SMALL_NW_INITIAL_KEY_RETRY_COUNT;

    // Multicast timing configuration for certification uses the LARGE values as it is the one mentioned ins specification
    cfg->mpl.mpl_trickle_imin = MPL_XLARGE_IMIN;
    cfg->mpl.mpl_trickle_imax = MPL_XLARGE_IMAX;
    cfg->mpl.mpl_trickle_k = MPL_XLARGE_K;
    cfg->mpl.mpl_trickle_timer_exp = MPL_XLARGE_EXPIRATIONS;
    cfg->mpl.seed_set_entry_lifetime = MPL_XLARGE_SEED_LIFETIME;
}

static int8_t ws_cfg_gen_default_set(ws_gen_cfg_t *cfg)
{
    memset(cfg->network_name, 0, sizeof(cfg->network_name));
    cfg->rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_gen_get(ws_gen_cfg_t *cfg)
{
    *cfg = ws_cfg.gen;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_gen_validate(ws_gen_cfg_t *new_cfg)
{
    ws_gen_cfg_t *cfg = &ws_cfg.gen;

    if (strlen(new_cfg->network_name) > 32) {
        return CFG_SETTINGS_ERROR_GEN_CONF;
    }

    // Regulator domain, operating mode or class has changed
    if (strcmp(cfg->network_name, new_cfg->network_name) != 0 ||
            cfg->rpl_parent_candidate_max != new_cfg->rpl_parent_candidate_max ||
            cfg->rpl_selected_parent_max != new_cfg->rpl_selected_parent_max) {
        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_gen_set(protocol_interface_info_entry_t *cur, ws_gen_cfg_t *new_cfg, uint8_t flags)
{
    int8_t ret = ws_cfg_gen_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_gen_cfg_t *cfg = &ws_cfg.gen;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_gen_cfg_t), "gen");

    cfg->network_size = new_cfg->network_size;
    if (&cfg->network_name != &new_cfg->network_name) {
        strncpy(cfg->network_name, new_cfg->network_name, 32);
    }
    cfg->rpl_parent_candidate_max = new_cfg->rpl_parent_candidate_max;
    cfg->rpl_selected_parent_max = new_cfg->rpl_selected_parent_max;

    if (cur && !(flags & CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE)) {
        ws_bootstrap_restart_delayed(cur->id);
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_phy_default_set(ws_phy_cfg_t *cfg)
{
    // FHSS configuration
    cfg->regulatory_domain = REG_DOMAIN_EU;
    cfg->operating_mode = OPERATING_MODE_3;
    cfg->operating_class = 2;
    cfg->phy_mode_id = 255;
    cfg->channel_plan_id = 255;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_phy_get(ws_phy_cfg_t *cfg)
{
    *cfg = ws_cfg.phy;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_phy_validate(ws_phy_cfg_t *new_cfg)
{
    ws_phy_cfg_t *cfg = &ws_cfg.phy;

    // Regulator domain, operating mode or class has changed
    if (cfg->regulatory_domain != new_cfg->regulatory_domain ||
            cfg->operating_mode != new_cfg->operating_mode ||
            cfg->operating_class != new_cfg->operating_class ||
            cfg->phy_mode_id != new_cfg->phy_mode_id ||
            cfg->channel_plan_id != new_cfg->channel_plan_id) {

        ws_hopping_schedule_t hopping_schdule = {
            .regulatory_domain = new_cfg->regulatory_domain,
            .operating_mode = new_cfg->operating_mode,
            .operating_class = new_cfg->operating_class,
            .phy_mode_id = new_cfg->phy_mode_id,
            .channel_plan_id = new_cfg->channel_plan_id
        };

        // Check that new settings are valid
        if (ws_common_regulatory_domain_config(NULL, &hopping_schdule) < 0) {
            // Invalid regulatory domain set
            return CFG_SETTINGS_ERROR_PHY_CONF;
        }

        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_phy_set(protocol_interface_info_entry_t *cur, ws_phy_cfg_t *new_cfg, uint8_t flags)
{
    int8_t ret = ws_cfg_phy_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }
    // Check settings and configure interface
    if (cur) {
        // Set operating mode for FSK if given with PHY mode ID
        if ((new_cfg->phy_mode_id == 1) || (new_cfg->phy_mode_id == 17)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_1a;
        } else if ((new_cfg->phy_mode_id == 2) || (new_cfg->phy_mode_id == 18)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_1b;
        } else if ((new_cfg->phy_mode_id == 3) || (new_cfg->phy_mode_id == 19)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_2a;
        } else if ((new_cfg->phy_mode_id == 4) || (new_cfg->phy_mode_id == 20)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_2b;
        } else if ((new_cfg->phy_mode_id == 5) || (new_cfg->phy_mode_id == 21)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_3;
        } else if ((new_cfg->phy_mode_id == 6) || (new_cfg->phy_mode_id == 22)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_4a;
        } else if ((new_cfg->phy_mode_id == 7) || (new_cfg->phy_mode_id == 23)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_4b;
        } else if ((new_cfg->phy_mode_id == 8) || (new_cfg->phy_mode_id == 24)) {
            cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_5;
        } else {
            cur->ws_info->hopping_schdule.operating_mode = new_cfg->operating_mode;
        }
        cur->ws_info->hopping_schdule.phy_mode_id = new_cfg->phy_mode_id;
        cur->ws_info->hopping_schdule.channel_plan_id = new_cfg->channel_plan_id;
        cur->ws_info->hopping_schdule.regulatory_domain = new_cfg->regulatory_domain;
        cur->ws_info->hopping_schdule.operating_class = new_cfg->operating_class;

        if (ws_common_regulatory_domain_config(cur, &cur->ws_info->hopping_schdule) < 0) {
            return CFG_SETTINGS_ERROR_PHY_CONF;
        }
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_phy_cfg_t *cfg = &ws_cfg.phy;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_phy_cfg_t), "phy");

    *cfg = *new_cfg;

    if (cur && !(flags & CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE)) {
        ws_bootstrap_restart_delayed(cur->id);
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_timing_default_set(ws_timing_cfg_t *cfg)
{
    // Configure the Wi-SUN timing trickle parameters
    cfg->disc_trickle_imin = TRICKLE_IMIN_60_SECS;       // 60 seconds
    cfg->disc_trickle_imax = TRICKLE_IMIN_60_SECS << 4;  // 960 seconds; 16 minutes
    cfg->disc_trickle_k = 1;
    cfg->pan_timeout = PAN_VERSION_MEDIUM_NETWORK_TIMEOUT;
    cfg->temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;
    cfg->temp_eapol_min_timeout = WS_EAPOL_TEMPORARY_ENTRY_MEDIUM_TIMEOUT;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_timing_get(ws_timing_cfg_t *cfg)
{
    *cfg = ws_cfg.timing;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_timing_validate(ws_timing_cfg_t *new_cfg)
{
    ws_timing_cfg_t *cfg = &ws_cfg.timing;

    if (cfg->disc_trickle_imin != new_cfg->disc_trickle_imin ||
            cfg->disc_trickle_imax != new_cfg->disc_trickle_imax ||
            cfg->disc_trickle_k != new_cfg->disc_trickle_k ||
            cfg->pan_timeout != new_cfg->pan_timeout ||
            cfg->temp_link_min_timeout != new_cfg->temp_link_min_timeout) {

        // Discovery Imin 1 to 255
        if (new_cfg->disc_trickle_imin < 1 || new_cfg->disc_trickle_imin > 255) {
            return CFG_SETTINGS_ERROR_TIMING_CONF;
        }
        // Discovery Imax, 1 to 8 doublings of imin
        if (new_cfg->disc_trickle_imax < new_cfg->disc_trickle_imin * 2 ||
                new_cfg->disc_trickle_imax > new_cfg->disc_trickle_imin * 256) {
            return CFG_SETTINGS_ERROR_TIMING_CONF;
        }
        // Discovery k parameter defined to be 1
        if (cfg->disc_trickle_k != 1) {
            return CFG_SETTINGS_ERROR_TIMING_CONF;
        }

        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_timing_set(protocol_interface_info_entry_t *cur, ws_timing_cfg_t *new_cfg, uint8_t flags)
{
    (void) flags;

    int8_t ret = ws_cfg_timing_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur) {
        cur->ws_info->trickle_params_pan_discovery.Imin = new_cfg->disc_trickle_imin * 10;
        cur->ws_info->trickle_params_pan_discovery.Imax = new_cfg->disc_trickle_imax * 10;
        cur->ws_info->trickle_params_pan_discovery.k = new_cfg->disc_trickle_k;
        cur->ws_info->trickle_params_pan_discovery.TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE;
        ws_pae_controller_configure(cur, NULL, NULL, new_cfg);
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_timing_cfg_t *cfg = &ws_cfg.timing;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_timing_cfg_t), "timing");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_bbr_default_set(ws_bbr_cfg_t *cfg)
{
    // Something in between
    // imin: 17 (128s)
    // doublings:3 (1024s)
    // redundancy; 10
    //ws_bbr_rpl_config(cur, 17, 3, 10, WS_RPL_MAX_HOP_RANK_INCREASE, WS_RPL_MIN_HOP_RANK_INCREASE);

    cfg->dio_interval_min = WS_RPL_DIO_IMIN_MEDIUM;           // 128s
    cfg->dio_interval_doublings = WS_RPL_DIO_DOUBLING_MEDIUM; // 1024s
    cfg->dio_redundancy_constant = 10;
    cfg->dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->rpl_default_lifetime = WS_RPL_DEFAULT_LIFETIME_MEDIUM;
    cfg->dhcp_address_lifetime = WS_DHCP_ADDRESS_LIFETIME_MEDIUM;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_bbr_get(ws_bbr_cfg_t *cfg)
{
    *cfg = ws_cfg.bbr;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_bbr_validate(ws_bbr_cfg_t *new_cfg)
{
    ws_bbr_cfg_t *cfg = &ws_cfg.bbr;
    if (cfg->dio_interval_min != new_cfg->dio_interval_min ||
            cfg->dio_interval_doublings != new_cfg->dio_interval_doublings ||
            cfg->dio_redundancy_constant != new_cfg->dio_redundancy_constant ||
            cfg->dag_max_rank_increase != new_cfg->dag_max_rank_increase ||
            cfg->min_hop_rank_increase != new_cfg->min_hop_rank_increase ||
            cfg->dhcp_address_lifetime != new_cfg->dhcp_address_lifetime ||
            cfg->rpl_default_lifetime != new_cfg->rpl_default_lifetime) {
        return CFG_SETTINGS_CHANGED;
    }

    if (cfg->dio_interval_min == 0 ||
            cfg->min_hop_rank_increase < 32 ||
            cfg->dhcp_address_lifetime < 60 ||
            cfg->rpl_default_lifetime < 60) {
        return CFG_SETTINGS_ERROR_GEN_CONF;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_bbr_set(protocol_interface_info_entry_t *cur, ws_bbr_cfg_t *new_cfg, uint8_t flags)
{
    (void) flags;

    int8_t ret = ws_cfg_bbr_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur) {
        // cur is optional, default values are for Wi-SUN small network parameters,
        ws_bbr_rpl_config(cur, new_cfg->dio_interval_min, new_cfg->dio_interval_doublings,
                          new_cfg->dio_redundancy_constant, new_cfg->dag_max_rank_increase,
                          new_cfg->min_hop_rank_increase, new_cfg->rpl_default_lifetime);
        ws_bbr_dhcp_address_lifetime_set(cur, new_cfg->dhcp_address_lifetime);
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_bbr_cfg_t *cfg = &ws_cfg.bbr;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_bbr_cfg_t), "rpl");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_mpl_default_set(ws_mpl_cfg_t *cfg)
{
    // MPL configuration
    cfg->mpl_trickle_imin = MPL_MEDIUM_IMIN;
    cfg->mpl_trickle_imax = MPL_MEDIUM_IMAX;
    cfg->mpl_trickle_k = MPL_MEDIUM_K;
    cfg->mpl_trickle_timer_exp = MPL_MEDIUM_EXPIRATIONS;
    cfg->seed_set_entry_lifetime = MPL_MEDIUM_SEED_LIFETIME;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_mpl_get(ws_mpl_cfg_t *cfg)
{
    *cfg = ws_cfg.mpl;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_mpl_validate(ws_mpl_cfg_t *new_cfg)
{
    ws_mpl_cfg_t *cfg = &ws_cfg.mpl;

    // MPL configuration has changed
    if (cfg->mpl_trickle_imin != new_cfg->mpl_trickle_imin ||
            cfg->mpl_trickle_imax != new_cfg->mpl_trickle_imax ||
            cfg->mpl_trickle_k != new_cfg->mpl_trickle_k ||
            cfg->mpl_trickle_timer_exp != new_cfg->mpl_trickle_timer_exp ||
            cfg->seed_set_entry_lifetime != new_cfg->seed_set_entry_lifetime) {
        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_mpl_set(protocol_interface_info_entry_t *cur, ws_mpl_cfg_t *new_cfg, uint8_t flags)
{
    (void) flags;

    // In Wi-SUN Border router will have modified settings to improve reliability
    if (cur && cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        // Border router sends multiple packets to ensure start of sequence
        if (new_cfg->mpl_trickle_timer_exp < MPL_BORDER_ROUTER_MIN_EXPIRATIONS) {
            new_cfg->mpl_trickle_timer_exp = MPL_BORDER_ROUTER_MIN_EXPIRATIONS;
            // Lifetime is calculated using the original IMAX
            new_cfg->seed_set_entry_lifetime = new_cfg->mpl_trickle_imax * new_cfg->mpl_trickle_timer_exp * MPL_SAFE_HOP_COUNT;
        }
        // Border router should have shorter IMAX to speed startup
        if (new_cfg->mpl_trickle_imax > MPL_BORDER_ROUTER_MAXIMUM_IMAX) {
            new_cfg->mpl_trickle_imax = MPL_BORDER_ROUTER_MAXIMUM_IMAX;
        }
    }

    int8_t ret = ws_cfg_mpl_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur) {
        cur->mpl_data_trickle_params.Imin = MPL_MS_TO_TICKS(new_cfg->mpl_trickle_imin * 1000);
        cur->mpl_data_trickle_params.Imax = MPL_MS_TO_TICKS(new_cfg->mpl_trickle_imax * 1000);
        cur->mpl_data_trickle_params.k = new_cfg->mpl_trickle_k;
        cur->mpl_data_trickle_params.TimerExpirations = new_cfg->mpl_trickle_timer_exp;
        cur->mpl_seed_set_entry_lifetime = new_cfg->seed_set_entry_lifetime;

        if (cur->mpl_domain) {
            // Update MPL settings
            mpl_domain_change_timing(cur->mpl_domain, &cur->mpl_data_trickle_params, cur->mpl_seed_set_entry_lifetime);
        }
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_mpl_cfg_t *cfg = &ws_cfg.mpl;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_mpl_cfg_t), "mpl");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_fhss_default_set(ws_fhss_cfg_t *cfg)
{
    // Set defaults for the device. user can modify these.
    cfg->fhss_uc_fixed_channel = 0xffff;
    cfg->fhss_bc_fixed_channel = 0xffff;
    cfg->fhss_uc_dwell_interval = WS_FHSS_UC_DWELL_INTERVAL;
    cfg->fhss_bc_interval = WS_FHSS_BC_INTERVAL;
    cfg->fhss_bc_dwell_interval = WS_FHSS_BC_DWELL_INTERVAL;
    cfg->fhss_uc_channel_function = WS_DH1CF;
    cfg->fhss_bc_channel_function = WS_DH1CF;

    for (uint8_t n = 0; n < 8; n++) {
        cfg->fhss_channel_mask[n] = 0xffffffff;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_fhss_get(ws_fhss_cfg_t *cfg)
{
    *cfg = ws_cfg.fhss;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_fhss_validate(ws_fhss_cfg_t *new_cfg)
{
    ws_fhss_cfg_t *cfg = &ws_cfg.fhss;

    if (memcmp(cfg->fhss_channel_mask, new_cfg->fhss_channel_mask, sizeof(uint32_t) * 8) != 0 ||
            cfg->fhss_uc_dwell_interval != new_cfg->fhss_uc_dwell_interval ||
            cfg->fhss_bc_dwell_interval != new_cfg->fhss_bc_dwell_interval ||
            cfg->fhss_bc_interval != new_cfg->fhss_bc_interval ||
            cfg->fhss_uc_channel_function != new_cfg->fhss_uc_channel_function ||
            cfg->fhss_bc_channel_function != new_cfg->fhss_bc_channel_function ||
            cfg->fhss_uc_fixed_channel != new_cfg->fhss_uc_fixed_channel ||
            cfg->fhss_bc_fixed_channel != new_cfg->fhss_bc_fixed_channel) {

        if (new_cfg->fhss_uc_dwell_interval < 15) {
            return CFG_SETTINGS_ERROR_FHSS_CONF;
        }

        if (new_cfg->fhss_bc_dwell_interval < 100) {
            return CFG_SETTINGS_ERROR_FHSS_CONF;
        }

        if (cfg->fhss_uc_channel_function != WS_FIXED_CHANNEL &&
                cfg->fhss_uc_channel_function != WS_VENDOR_DEF_CF &&
                cfg->fhss_uc_channel_function != WS_DH1CF &&
                cfg->fhss_uc_channel_function != WS_TR51CF) {
            return CFG_SETTINGS_ERROR_FHSS_CONF;
        }

        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_fhss_set(protocol_interface_info_entry_t *cur, ws_fhss_cfg_t *new_cfg, uint8_t flags)
{
    (void) cur;

    int8_t ret = ws_cfg_fhss_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_fhss_cfg_t *cfg = &ws_cfg.fhss;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_fhss_cfg_t), "fhss");

    *cfg = *new_cfg;

    if (cfg->fhss_uc_channel_function == WS_FIXED_CHANNEL && cfg->fhss_uc_fixed_channel == 0xffff) {
        cfg->fhss_uc_fixed_channel = 0;
        tr_warn("UC fixed channel not configured. Set to 0");
    }

    if (cfg->fhss_uc_channel_function != WS_FIXED_CHANNEL) {
        cfg->fhss_uc_fixed_channel = 0xffff;
    }

    if (cfg->fhss_bc_channel_function == WS_FIXED_CHANNEL && cfg->fhss_bc_fixed_channel == 0xffff) {
        cfg->fhss_bc_fixed_channel = 0;
        tr_warn("BC fixed channel not configured. Set to 0");
    }

    if (cfg->fhss_bc_channel_function != WS_FIXED_CHANNEL) {
        cfg->fhss_bc_fixed_channel = 0xffff;
    }

    if (cur && !(flags & CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE)) {
        ws_bootstrap_restart_delayed(cur->id);
    }

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_sec_timer_default_set(ws_sec_timer_cfg_t *cfg)
{
    cfg->gtk_expire_offset = DEFAULT_GTK_EXPIRE_OFFSET;
    cfg->pmk_lifetime = DEFAULT_PMK_LIFETIME;
    cfg->ptk_lifetime = DEFAULT_PTK_LIFETIME;
    cfg->gtk_new_act_time = DEFAULT_GTK_NEW_ACTIVATION_TIME;
    cfg->revocat_lifetime_reduct = DEFAULT_REVOCATION_LIFETIME_REDUCTION;
    cfg->gtk_request_imin = DEFAULT_GTK_REQUEST_IMIN;
    cfg->gtk_request_imax = DEFAULT_GTK_REQUEST_IMAX;
    cfg->gtk_max_mismatch = DEFAULT_GTK_MAX_MISMATCH;
    cfg->gtk_new_install_req = DEFAULT_GTK_NEW_INSTALL_REQUIRED;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_timer_get(ws_sec_timer_cfg_t *cfg)
{
    *cfg = ws_cfg.sec_timer;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_timer_validate(ws_sec_timer_cfg_t *new_cfg)
{
    ws_sec_timer_cfg_t *cfg = &ws_cfg.sec_timer;

    if (cfg->gtk_expire_offset != new_cfg->gtk_expire_offset ||
            cfg->pmk_lifetime != new_cfg->pmk_lifetime ||
            cfg->ptk_lifetime != new_cfg->ptk_lifetime ||
            cfg->gtk_new_act_time != new_cfg->gtk_new_act_time ||
            cfg->revocat_lifetime_reduct != new_cfg->revocat_lifetime_reduct ||
            cfg->gtk_request_imin != new_cfg->gtk_request_imin ||
            cfg->gtk_request_imax != new_cfg->gtk_request_imax ||
            cfg->gtk_max_mismatch != new_cfg->gtk_max_mismatch ||
            cfg->gtk_new_install_req != new_cfg->gtk_new_install_req) {

        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_timer_set(protocol_interface_info_entry_t *cur, ws_sec_timer_cfg_t *new_cfg, uint8_t flags)
{
    (void) flags;

    int8_t ret = ws_cfg_sec_timer_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur) {
        ws_pae_controller_configure(cur, new_cfg, NULL, NULL);
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_sec_timer_cfg_t *cfg = &ws_cfg.sec_timer;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_sec_timer_cfg_t), "sec_timer");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_sec_prot_default_set(ws_sec_prot_cfg_t *cfg)
{
    cfg->sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;
    cfg->max_simult_sec_neg_tx_queue_min = MAX_SIMULTANEOUS_SECURITY_NEGOTIATIONS_TX_QUEUE_MIN;
    cfg->max_simult_sec_neg_tx_queue_max = MAX_SIMULTANEOUS_SECURITY_NEGOTIATIONS_TX_QUEUE_MAX;
    cfg->initial_key_retry_min = MEDIUM_NW_INITIAL_KEY_RETRY_MIN_SECS;
    cfg->initial_key_retry_max = MEDIUM_NW_INITIAL_KEY_RETRY_MAX_SECS;
    cfg->initial_key_retry_max_limit = MEDIUM_NW_INITIAL_KEY_RETRY_MAX_LIMIT_SECS;
    cfg->initial_key_retry_cnt = MEDIUM_NW_INITIAL_KEY_RETRY_COUNT;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_prot_get(ws_sec_prot_cfg_t *cfg)
{
    *cfg = ws_cfg.sec_prot;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_prot_validate(ws_sec_prot_cfg_t *new_cfg)
{
    ws_sec_prot_cfg_t *cfg = &ws_cfg.sec_prot;

    if (cfg->sec_prot_trickle_imin != new_cfg->sec_prot_trickle_imin ||
            cfg->sec_prot_trickle_imax != new_cfg->sec_prot_trickle_imax ||
            cfg->sec_prot_trickle_timer_exp != new_cfg->sec_prot_trickle_timer_exp ||
            cfg->sec_prot_retry_timeout != new_cfg->sec_prot_retry_timeout ||
            cfg->max_simult_sec_neg_tx_queue_min != new_cfg->max_simult_sec_neg_tx_queue_min ||
            cfg->max_simult_sec_neg_tx_queue_max != new_cfg->max_simult_sec_neg_tx_queue_max ||
            cfg->initial_key_retry_min != new_cfg->initial_key_retry_min ||
            cfg->initial_key_retry_max != new_cfg->initial_key_retry_max ||
            cfg->initial_key_retry_max_limit != new_cfg->initial_key_retry_max_limit ||
            cfg->initial_key_retry_cnt != new_cfg->initial_key_retry_cnt) {

        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_prot_set(protocol_interface_info_entry_t *cur, ws_sec_prot_cfg_t *new_cfg, uint8_t flags)
{
    (void) flags;

    int8_t ret = ws_cfg_sec_prot_validate(new_cfg);
    if (!(flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) && ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur) {
        ws_pae_controller_configure(cur, NULL, new_cfg, NULL);
    }

    if (flags & CFG_FLAGS_BOOTSTRAP_SET_VALUES) {
        return CFG_SETTINGS_OK;
    }

    ws_sec_prot_cfg_t *cfg = &ws_cfg.sec_prot;

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_sec_prot_cfg_t), "sec_prot");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_settings_init(void)
{
    ws_cfg_settings_default_set();
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_settings_default_set(void)
{
    int8_t ret_value = 0;

    // Set default configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        if (cfg_cb[index].default_set) {
            if (cfg_cb[index].default_set(
                        ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset) < 0) {
                ret_value = CFG_SETTINGS_OTHER_ERROR;
            }
        }
    }

    // Set new configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        if (cfg_cb[index].set(NULL,
                              ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                              0x00) < 0) {
            tr_info("FATAL CONFIG FAILURE");
            ret_value = CFG_SETTINGS_OTHER_ERROR;
        }
    }

    return ret_value;
}

int8_t ws_cfg_settings_interface_set(protocol_interface_info_entry_t *cur)
{
    int8_t ret_value = 0;

    cur->ws_info->cfg = &ws_cfg;

    // Set new configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        // Validation
        if (cfg_cb[index].set) {
            if (cfg_cb[index].set(cur,
                                  ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                                  CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE | CFG_FLAGS_BOOTSTRAP_SET_VALUES) < 0) {
                tr_info("FATAL CONFIG FAILURE");
                ret_value = CFG_SETTINGS_OTHER_ERROR;
            }
        }
    }

    return ret_value;
}

int8_t ws_cfg_settings_get(ws_cfg_t *cfg)
{
    *cfg = ws_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_settings_validate(struct ws_cfg_s *new_cfg)
{
    // Validate new configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        if (cfg_cb[index].validate) {
            int8_t ret = cfg_cb[index].validate(
                             ((uint8_t *)new_cfg) + cfg_cb[index].setting_offset);
            if (ret < 0) {
                // Validation failed
                return ret;
            }
        }
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_settings_set(protocol_interface_info_entry_t *cur, ws_cfg_t *new_cfg)
{
    int8_t ret_value = CFG_SETTINGS_OK;

    bool call_cfg_set[CFG_CB_NUM];

    // Validate new configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        if (cfg_cb[index].validate) {
            int8_t ret = cfg_cb[index].validate(
                             ((uint8_t *)new_cfg) + cfg_cb[index].setting_offset);

            if (ret < 0) {
                // Validation failed
                return ret;
            } else if (ret == CFG_SETTINGS_CHANGED) {
                call_cfg_set[index] = true;
            } else {
                call_cfg_set[index] = false;
            }
        } else {
            // If validation not needed, set right away
            call_cfg_set[index] = true;
        }
    }

    // Set new configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        // Validation
        if (call_cfg_set[index]) {
            if (cfg_cb[index].set(cur,
                                  ((uint8_t *)new_cfg) + cfg_cb[index].setting_offset, 0x00) < 0) {
                tr_info("FATAL CONFIG FAILURE");
                ret_value = CFG_SETTINGS_OTHER_ERROR;
            }
        }
    }

    return ret_value;
}

uint32_t ws_cfg_neighbour_temporary_lifetime_get(void)
{
    if (ws_test_temporary_entry_lifetime) {
        return ws_test_temporary_entry_lifetime;
    }
    return WS_NEIGHBOUR_TEMPORARY_ENTRY_LIFETIME;
}
void ws_cfg_neighbour_temporary_lifetime_set(uint32_t lifetime)
{
    if (lifetime > WS_NEIGHBOUR_TEMPORARY_NEIGH_MAX_LIFETIME || lifetime == 0) {
        if (lifetime > WS_NEIGHBOR_LINK_TIMEOUT) {
            lifetime = WS_NEIGHBOR_LINK_TIMEOUT;
        }
        ws_test_temporary_entry_lifetime = lifetime;
    }
}

#endif //HAVE_WS
