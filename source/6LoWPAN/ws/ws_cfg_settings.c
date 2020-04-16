/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#define CFG_FLAGS_DISABLE_VAL_SET             0x01
#define CFG_FLAGS_OVERRIDE_DISABLE_VAL_SET    0x02
#define CFG_FLAGS_FORCE_INTERNAL_CONFIG       0x04
#define CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE   0x08

#define TRICKLE_IMIN_60_SECS 60
#define TRICKLE_IMIN_30_SECS 30
#define TRICKLE_IMIN_15_SECS 15

typedef struct ws_cfg_nw_size_s {
    ws_timing_cfg_t timing;             /**< Timing configuration */
    ws_rpl_cfg_t rpl;                   /**< RPL configuration */
    ws_sec_prot_cfg_t sec_prot;         /**< Security protocols configuration */
} ws_cfg_nw_size_t;

typedef int8_t (*ws_cfg_default_set)(void *cfg);
typedef int8_t (*ws_cfg_validate)(void *cfg, void *new_cfg);
typedef int8_t (*ws_cfg_set)(protocol_interface_info_entry_t *cur, void *cfg, void *new_cfg, uint8_t *flags);

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
    ws_rpl_cfg_t rpl;
    ws_fhss_cfg_t fhss;
    ws_mpl_cfg_t mpl;
    ws_sec_timer_cfg_t sec_timer;
    ws_sec_prot_cfg_t sec_prot;
} ws_cfgs_t;

static int8_t ws_cfg_to_get(ws_cfgs_t **cfg, ws_cfgs_t *new_cfg, ws_cfg_validate valid_cb, ws_cfgs_t *external_cfg, uint8_t *cfg_flags, uint8_t *flags);

static void ws_cfg_network_size_config_set_small(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_medium(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_large(ws_cfg_nw_size_t *cfg);
static void ws_cfg_network_size_config_set_certificate(ws_cfg_nw_size_t *cfg);
static int8_t ws_cfg_network_size_default_set(ws_gen_cfg_t *cfg);
static int8_t ws_cfg_gen_default_set(ws_gen_cfg_t *cfg);
static int8_t ws_cfg_phy_default_set(ws_phy_cfg_t *cfg);
static int8_t ws_cfg_timing_default_set(ws_timing_cfg_t *cfg);
static int8_t ws_cfg_rpl_default_set(ws_rpl_cfg_t *cfg);
static int8_t ws_cfg_mpl_default_set(ws_mpl_cfg_t *cfg);
static int8_t ws_cfg_fhss_default_set(ws_fhss_cfg_t *cfg);
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
    CFG_CB(ws_cfg_rpl_default_set, ws_cfg_rpl_validate, ws_cfg_rpl_set, offsetof(ws_cfg_t, rpl)),
    CFG_CB(ws_cfg_mpl_default_set, ws_cfg_mpl_validate, ws_cfg_mpl_set, offsetof(ws_cfg_t, mpl)),
    CFG_CB(ws_cfg_fhss_default_set, ws_cfg_fhss_validate, ws_cfg_fhss_set, offsetof(ws_cfg_t, fhss)),
    CFG_CB(ws_cfg_sec_timer_default_set, ws_cfg_sec_timer_validate, ws_cfg_sec_timer_set, offsetof(ws_cfg_t, sec_timer)),
    CFG_CB(ws_cfg_sec_prot_default_set, ws_cfg_sec_prot_validate, ws_cfg_sec_prot_set, offsetof(ws_cfg_t, sec_prot)),
};

#define CFG_CB_NUM (sizeof(cfg_cb) / sizeof(ws_cfg_cb_t))

// Wisun configuration storage
ws_cfg_t ws_cfg;

// If automatic network size mode; external configuration shown to towards users of external APIs
ws_cfg_nw_size_t *nw_size_external_cfg = NULL;

static int8_t ws_cfg_to_get(ws_cfgs_t **cfg, ws_cfgs_t *new_cfg, ws_cfg_validate valid_cb, ws_cfgs_t *ws_cfg_ptr, uint8_t *cfg_flags, uint8_t *flags)
{
    // In case target configuration is not set, uses ws_cfg storage
    if (*cfg == NULL) {
        // In case external configuration is not same as internal
        if (nw_size_external_cfg && (!flags || !(*flags & CFG_FLAGS_FORCE_INTERNAL_CONFIG))) {
            if (ws_cfg_ptr == (ws_cfgs_t *) &ws_cfg.timing) {
                *cfg = (ws_cfgs_t *) &nw_size_external_cfg->timing;
            } else if (ws_cfg_ptr == (ws_cfgs_t *) &ws_cfg.rpl) {
                *cfg = (ws_cfgs_t *) &nw_size_external_cfg->rpl;
            } else if (ws_cfg_ptr == (ws_cfgs_t *) &ws_cfg.sec_prot) {
                *cfg = (ws_cfgs_t *) &nw_size_external_cfg->sec_prot;
            } else {
                *cfg = ws_cfg_ptr;
            }
        } else {
            *cfg = ws_cfg_ptr;
        }

        if (valid_cb) {
            int8_t ret = valid_cb(*cfg, new_cfg);
            // On failure and if nothing is changed, returns
            if (ret != CFG_SETTINGS_CHANGED) {
                return ret;
            }
        }
    }

    if (!cfg_flags) {
        return CFG_SETTINGS_CHANGED;
    }
    *cfg_flags = 0;
    if (flags) {
        *cfg_flags |= *flags;
    }
    if (nw_size_external_cfg && !(*cfg_flags & CFG_FLAGS_OVERRIDE_DISABLE_VAL_SET)) {
        *cfg_flags |= CFG_FLAGS_DISABLE_VAL_SET;
    }

    return CFG_SETTINGS_CHANGED;
}

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

int8_t ws_cfg_network_size_get(ws_gen_cfg_t *cfg, uint8_t *flags)
{
    ws_gen_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.gen, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_network_size_validate(ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.gen, 0, 0);

    if (cfg->network_size != new_cfg->network_size) {
        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

typedef void (*ws_cfg_network_size_config_set_size)(ws_cfg_nw_size_t *cfg);

int8_t ws_cfg_network_size_set(protocol_interface_info_entry_t *cur, ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg, uint8_t *flags)
{
    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_network_size_validate, (ws_cfgs_t *) &ws_cfg.gen, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    uint8_t old_network_size = cfg->network_size;

    // If network size configuration has not changed, returns
    if (cfg->network_size == new_cfg->network_size) {
        return CFG_SETTINGS_OK;
    }

    cfg->network_size = new_cfg->network_size;

    ws_cfg_nw_size_t nw_size_cfg;
    ws_cfg_timing_get(&nw_size_cfg.timing, NULL);
    ws_cfg_rpl_get(&nw_size_cfg.rpl, NULL);
    ws_cfg_sec_prot_get(&nw_size_cfg.sec_prot, NULL);

    ws_cfg_network_size_config_set_size set_function = NULL;

    if (cfg->network_size == NETWORK_SIZE_CERTIFICATE) {
        set_function = ws_cfg_network_size_config_set_certificate;
    } else if (cfg->network_size <= NETWORK_SIZE_SMALL || cfg->network_size == NETWORK_SIZE_AUTOMATIC) {
        set_function = ws_cfg_network_size_config_set_small;
    } else if (cfg->network_size <=  NETWORK_SIZE_MEDIUM) {
        set_function = ws_cfg_network_size_config_set_medium;
    } else {
        set_function = ws_cfg_network_size_config_set_large;
    }

    // Overrides the values on the new configuration
    if (set_function != NULL) {
        set_function(&nw_size_cfg);
    }

    /* If no longer in an automatic network size mode, frees automatic configuration,
       so that new configuration is set */
    if (nw_size_external_cfg && old_network_size == NETWORK_SIZE_AUTOMATIC) {
        ns_dyn_mem_free(nw_size_external_cfg);
        nw_size_external_cfg = NULL;
    }

    uint8_t set_flags = 0;
    if (cfg->network_size == NETWORK_SIZE_AUTOMATIC) {
        set_flags = CFG_FLAGS_DISABLE_VAL_SET;
    }
    /* Sets values if changed or network size has been previously automatic (to make sure
       the settings are in sync */
    if (ws_cfg_timing_validate(&ws_cfg.timing, &nw_size_cfg.timing) == CFG_SETTINGS_CHANGED ||
            old_network_size == NETWORK_SIZE_AUTOMATIC) {
        ws_cfg_timing_set(cur, &ws_cfg.timing, &nw_size_cfg.timing, &set_flags);
    }
    if (ws_cfg_rpl_validate(&ws_cfg.rpl, &nw_size_cfg.rpl) == CFG_SETTINGS_CHANGED ||
            old_network_size == NETWORK_SIZE_AUTOMATIC) {
        ws_cfg_rpl_set(cur, &ws_cfg.rpl, &nw_size_cfg.rpl, &set_flags);
    }
    if (ws_cfg_sec_prot_validate(&ws_cfg.sec_prot, &nw_size_cfg.sec_prot) == CFG_SETTINGS_CHANGED ||
            old_network_size == NETWORK_SIZE_AUTOMATIC) {
        ws_cfg_sec_prot_set(cur, &ws_cfg.sec_prot, &nw_size_cfg.sec_prot, &set_flags);
    }

    // If is in an automatic network size mode, updates automatic configuration
    if (cfg->network_size == NETWORK_SIZE_AUTOMATIC && cur) {
        ws_cfg_network_size_configure(cur, cur->ws_info->pan_information.pan_size);
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_network_size_configure(protocol_interface_info_entry_t *cur, uint16_t network_size)
{
    // Read settings that are affected by network size
    ws_cfg_nw_size_t new_nw_size_cfg;
    uint8_t flags = CFG_FLAGS_OVERRIDE_DISABLE_VAL_SET | CFG_FLAGS_FORCE_INTERNAL_CONFIG;
    ws_cfg_timing_get(&new_nw_size_cfg.timing, &flags);
    ws_cfg_rpl_get(&new_nw_size_cfg.rpl, &flags);
    ws_cfg_sec_prot_get(&new_nw_size_cfg.sec_prot, &flags);

    if (!nw_size_external_cfg) {
        nw_size_external_cfg = ns_dyn_mem_alloc(sizeof(ws_cfg_nw_size_t));
        if (!nw_size_external_cfg) {
            return -1;
        }
        memcpy(nw_size_external_cfg, &new_nw_size_cfg, sizeof(ws_cfg_nw_size_t));
    }

    // Small
    if (network_size < 100) {
        // Automatic
        ws_cfg_network_size_config_set_small(&new_nw_size_cfg);
    } else if (network_size < 300) {
        // Medium
        ws_cfg_network_size_config_set_medium(&new_nw_size_cfg);
    } else {
        // Large
        ws_cfg_network_size_config_set_large(&new_nw_size_cfg);
    }

    ws_cfg_timing_set(cur, NULL, &new_nw_size_cfg.timing, &flags);
    ws_cfg_rpl_set(cur, NULL, &new_nw_size_cfg.rpl, &flags);
    ws_cfg_sec_prot_set(cur, NULL, &new_nw_size_cfg.sec_prot, &flags);

    return CFG_SETTINGS_OK;
}

static void ws_cfg_network_size_config_set_small(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN timing trickle parameter
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_15_SECS;       // 15 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_15_SECS << 2;  // 60 seconds
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_SMALL_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;

    // RPL configuration
    cfg->rpl.dio_interval_min = WS_RPL_DIO_IMIN_SMALL;               // 15; 32s seconds
    cfg->rpl.dio_interval_doublings = WS_RPL_DIO_DOUBLING_SMALL;     // 2; 128
    cfg->rpl.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_SMALL;  // Disabled
    cfg->rpl.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->rpl.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->rpl.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->rpl.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;
    cfg->sec_prot.sec_max_ongoing_authentication = MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_SMALL;
}

static void ws_cfg_network_size_config_set_medium(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_30_SECS;       // 30 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_30_SECS << 5;  // 960 seconds; 16 minutes
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_MEDIUM_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;

    // RPL configuration
    cfg->rpl.dio_interval_min = WS_RPL_DIO_IMIN_MEDIUM;              // 15; 32s
    cfg->rpl.dio_interval_doublings = WS_RPL_DIO_DOUBLING_MEDIUM;    // 2; 1024s
    cfg->rpl.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_MEDIUM; // 10
    cfg->rpl.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->rpl.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->rpl.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->rpl.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;
    cfg->sec_prot.sec_max_ongoing_authentication = MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_MEDIUM;
}

static void ws_cfg_network_size_config_set_large(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_60_SECS;       // 60 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_60_SECS << 4;  // 960 seconds; 16 minutes
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_LARGE_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_LARGE;

    // RPL configuration
    cfg->rpl.dio_interval_min = WS_RPL_DIO_IMIN_LARGE;               // 19; 524s, 9min
    cfg->rpl.dio_interval_doublings = WS_RPL_DIO_DOUBLING_LARGE;     // 1; 1024s, 17min
    cfg->rpl.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_LARGE;  // 10
    cfg->rpl.dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->rpl.min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->rpl.rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->rpl.rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_LARGE_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_LARGE_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_LARGE;
    cfg->sec_prot.sec_max_ongoing_authentication = MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_LARGE;
}

static void ws_cfg_network_size_config_set_certificate(ws_cfg_nw_size_t *cfg)
{
    // Configure the Wi-SUN timing trickle parameters
    cfg->timing.disc_trickle_imin = TRICKLE_IMIN_15_SECS;       // 15 seconds
    cfg->timing.disc_trickle_imax = TRICKLE_IMIN_15_SECS << 2;  // 60 seconds
    cfg->timing.disc_trickle_k = 1;
    cfg->timing.pan_timeout = PAN_VERSION_SMALL_NETWORK_TIMEOUT;
    cfg->timing.temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;

    // RPL configuration (small)
    cfg->rpl.dio_interval_min = WS_RPL_DIO_IMIN_SMALL;               // 15; 32s seconds
    cfg->rpl.dio_interval_doublings = WS_RPL_DIO_DOUBLING_SMALL;     // 2; 128
    cfg->rpl.dio_redundancy_constant = WS_RPL_DIO_REDUNDANCY_SMALL;  // Disabled
    cfg->rpl.dag_max_rank_increase = WS_CERTIFICATE_RPL_MAX_HOP_RANK_INCREASE;
    cfg->rpl.min_hop_rank_increase = WS_CERTIFICATE_RPL_MIN_HOP_RANK_INCREASE;
    cfg->rpl.rpl_parent_candidate_max = WS_CERTIFICATE_RPL_PARENT_CANDIDATE_MAX;
    cfg->rpl.rpl_selected_parent_max = WS_CERTIFICATE_RPL_SELECTED_PARENT_MAX;

    // EAPOL configuration
    cfg->sec_prot.sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot.sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot.sec_prot_trickle_timer_exp = SEC_PROT_TIMER_EXPIRATIONS;
    cfg->sec_prot.sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;
    cfg->sec_prot.sec_max_ongoing_authentication = MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_SMALL;
}

static int8_t ws_cfg_gen_default_set(ws_gen_cfg_t *cfg)
{
    memset(cfg->network_name, 0, sizeof(cfg->network_name));
    cfg->network_pan_id = 0xffff;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_gen_get(ws_gen_cfg_t *cfg, uint8_t *flags)
{
    ws_gen_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.gen, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_gen_validate(ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.gen, 0, 0);

    if (strlen(new_cfg->network_name) > 32) {
        return CFG_SETTINGS_ERROR_GEN_CONF;
    }

    // Regulator domain, operating mode or class has changed
    if (strcmp(cfg->network_name, new_cfg->network_name) != 0 ||
            cfg->network_pan_id != new_cfg->network_pan_id) {
        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_gen_set(protocol_interface_info_entry_t *cur, ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg, uint8_t *flags)
{
    (void) cur;
    (void) flags;

    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_gen_validate, (ws_cfgs_t *) &ws_cfg.gen, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }
    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }
    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_gen_cfg_t), "gen");

    if (&cfg->network_name != &new_cfg->network_name) {
        strncpy(cfg->network_name, new_cfg->network_name, 32);
    }
    cfg->network_pan_id = new_cfg->network_pan_id;

    if (cur && !(cfg_flags & CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE)) {
        ws_bootstrap_restart_delayed(cur->id);
    }

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_phy_default_set(ws_phy_cfg_t *cfg)
{
    // FHSS configuration
    cfg->regulatory_domain = REG_DOMAIN_EU;
    cfg->operating_mode = OPERATING_MODE_3;
    cfg->operating_class = 2;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_phy_get(ws_phy_cfg_t *cfg, uint8_t *flags)
{
    ws_phy_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.phy, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_phy_validate(ws_phy_cfg_t *cfg, ws_phy_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.phy, 0, 0);

    // Regulator domain, operating mode or class has changed
    if (cfg->regulatory_domain != new_cfg->regulatory_domain ||
            cfg->operating_mode != new_cfg->operating_mode ||
            cfg->operating_class != new_cfg->operating_class) {

        ws_hopping_schedule_t hopping_schdule = {
            .regulatory_domain = new_cfg->regulatory_domain,
            .operating_mode = new_cfg->operating_mode,
            .operating_class = new_cfg->operating_class
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

int8_t ws_cfg_phy_set(protocol_interface_info_entry_t *cur, ws_phy_cfg_t *cfg, ws_phy_cfg_t *new_cfg, uint8_t *flags)
{
    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_phy_validate, (ws_cfgs_t *) &ws_cfg.phy, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    // Check settings and configure interface
    if (cur && !(cfg_flags & CFG_FLAGS_DISABLE_VAL_SET)) {
        cur->ws_info->hopping_schdule.regulatory_domain = new_cfg->regulatory_domain;
        cur->ws_info->hopping_schdule.operating_mode = new_cfg->operating_mode;
        cur->ws_info->hopping_schdule.operating_class = new_cfg->operating_class;

        if (ws_common_regulatory_domain_config(cur, &cur->ws_info->hopping_schdule) < 0) {
            return CFG_SETTINGS_ERROR_PHY_CONF;
        }
    }

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_phy_cfg_t), "phy");

    *cfg = *new_cfg;

    if (cur && !(cfg_flags & CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE)) {
        ws_bootstrap_restart_delayed(cur->id);
    }

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_timing_default_set(ws_timing_cfg_t *cfg)
{
    // Configure the Wi-SUN timing trickle parameters
    cfg->disc_trickle_imin = TRICKLE_IMIN_30_SECS;       // 30 seconds
    cfg->disc_trickle_imax = TRICKLE_IMIN_30_SECS << 5;  // 960 seconds; 16 minutes
    cfg->disc_trickle_k = 1;
    cfg->pan_timeout = PAN_VERSION_MEDIUM_NETWORK_TIMEOUT;
    cfg->temp_link_min_timeout = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT_SMALL;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_timing_get(ws_timing_cfg_t *cfg, uint8_t *flags)
{
    ws_timing_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.timing, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_timing_validate(ws_timing_cfg_t *cfg, ws_timing_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.timing, 0, 0);

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

int8_t ws_cfg_timing_set(protocol_interface_info_entry_t *cur, ws_timing_cfg_t *cfg, ws_timing_cfg_t *new_cfg, uint8_t *flags)
{
    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_timing_validate, (ws_cfgs_t *) &ws_cfg.timing, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur && !(cfg_flags & CFG_FLAGS_DISABLE_VAL_SET)) {
        cur->ws_info->trickle_params_pan_discovery.Imin = new_cfg->disc_trickle_imin * 10;
        cur->ws_info->trickle_params_pan_discovery.Imax = new_cfg->disc_trickle_imax * 10;
        cur->ws_info->trickle_params_pan_discovery.k = new_cfg->disc_trickle_k;
        cur->ws_info->trickle_params_pan_discovery.TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE;
    }

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_timing_cfg_t), "timing");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_rpl_default_set(ws_rpl_cfg_t *cfg)
{
    // Something in between
    // imin: 15 (32s)
    // doublings:5 (960s)
    // redundancy; 10
    //ws_bbr_rpl_config(cur, 15, 5, 10, WS_RPL_MAX_HOP_RANK_INCREASE, WS_RPL_MIN_HOP_RANK_INCREASE);

    cfg->dio_interval_min = 15;        // 32s
    cfg->dio_interval_doublings = 5;   // 1024s
    cfg->dio_redundancy_constant = 10;
    cfg->dag_max_rank_increase = WS_RPL_MAX_HOP_RANK_INCREASE;
    cfg->min_hop_rank_increase = WS_RPL_MIN_HOP_RANK_INCREASE;
    cfg->rpl_parent_candidate_max = WS_RPL_PARENT_CANDIDATE_MAX;
    cfg->rpl_selected_parent_max = WS_RPL_SELECTED_PARENT_MAX;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_rpl_get(ws_rpl_cfg_t *cfg, uint8_t *flags)
{
    ws_rpl_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.rpl, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_rpl_validate(ws_rpl_cfg_t *cfg, ws_rpl_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.rpl, 0, 0);

    if (cfg->dio_interval_min != new_cfg->dio_interval_min ||
            cfg->dio_interval_doublings != new_cfg->dio_interval_doublings ||
            cfg->dio_redundancy_constant != new_cfg->dio_redundancy_constant ||
            cfg->dag_max_rank_increase != new_cfg->dag_max_rank_increase ||
            cfg->min_hop_rank_increase != new_cfg->min_hop_rank_increase ||
            cfg->rpl_parent_candidate_max != new_cfg->rpl_parent_candidate_max ||
            cfg->rpl_selected_parent_max != new_cfg->rpl_selected_parent_max) {
        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_rpl_set(protocol_interface_info_entry_t *cur, ws_rpl_cfg_t *cfg, ws_rpl_cfg_t *new_cfg, uint8_t *flags)
{
    (void) cur;
    (void) flags;

    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_rpl_validate, (ws_cfgs_t *) &ws_cfg.rpl, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (!(cfg_flags & CFG_FLAGS_DISABLE_VAL_SET)) {
        // cur is optional, default values are for Wi-SUN small network parameters,
        ws_bbr_rpl_config(cur, new_cfg->dio_interval_min, new_cfg->dio_interval_doublings,
                          new_cfg->dio_redundancy_constant, new_cfg->dag_max_rank_increase,
                          new_cfg->min_hop_rank_increase);
    }

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_rpl_cfg_t), "rpl");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_mpl_default_set(ws_mpl_cfg_t *cfg)
{
    // MPL configuration
    cfg->mpl_trickle_imin = DATA_MESSAGE_IMIN;
    cfg->mpl_trickle_imax = DATA_MESSAGE_IMAX;
    cfg->mpl_trickle_k = DATA_MESSAGE_K;
    cfg->mpl_trickle_timer_exp = DATA_MESSAGE_TIMER_EXPIRATIONS;
    cfg->seed_set_entry_lifetime = MPL_SEED_SET_ENTRY_TIMEOUT;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_mpl_get(ws_mpl_cfg_t *cfg, uint8_t *flags)
{
    ws_mpl_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.mpl, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_mpl_validate(ws_mpl_cfg_t *cfg, ws_mpl_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.mpl, 0, 0);

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

int8_t ws_cfg_mpl_set(protocol_interface_info_entry_t *cur, ws_mpl_cfg_t *cfg, ws_mpl_cfg_t *new_cfg, uint8_t *flags)
{
    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_mpl_validate, (ws_cfgs_t *) &ws_cfg.mpl, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur && !(cfg_flags & CFG_FLAGS_DISABLE_VAL_SET)) {
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

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_mpl_cfg_t), "mpl");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_fhss_default_set(ws_fhss_cfg_t *cfg)
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

int8_t ws_cfg_fhss_get(ws_fhss_cfg_t *cfg, uint8_t *flags)
{
    ws_fhss_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.fhss, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_fhss_validate(ws_fhss_cfg_t *cfg, ws_fhss_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.fhss, 0, 0);

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

        if (new_cfg->fhss_bc_dwell_interval < 15) {
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

int8_t ws_cfg_fhss_set(protocol_interface_info_entry_t *cur, ws_fhss_cfg_t *cfg, ws_fhss_cfg_t *new_cfg, uint8_t *flags)
{
    (void) cur;

    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_fhss_validate, (ws_cfgs_t *) &ws_cfg.fhss, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

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

    if (cur && !(cfg_flags & CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE)) {
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

int8_t ws_cfg_sec_timer_get(ws_sec_timer_cfg_t *cfg, uint8_t *flags)
{
    ws_sec_timer_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.sec_timer, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_timer_validate(ws_sec_timer_cfg_t *cfg, ws_sec_timer_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.sec_timer, 0, 0);

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

int8_t ws_cfg_sec_timer_set(protocol_interface_info_entry_t *cur, ws_sec_timer_cfg_t *cfg, ws_sec_timer_cfg_t *new_cfg, uint8_t *flags)
{
    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_sec_timer_validate, (ws_cfgs_t *) &ws_cfg.sec_timer, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur && !(cfg_flags & CFG_FLAGS_DISABLE_VAL_SET)) {
        ws_pae_controller_configure(cur, new_cfg, NULL);
    }

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

    ws_cfg_trace((ws_cfgs_t *) cfg, (ws_cfgs_t *) new_cfg, sizeof(ws_sec_timer_cfg_t), "sec_timer");

    *cfg = *new_cfg;

    return CFG_SETTINGS_OK;
}

static int8_t ws_cfg_sec_prot_default_set(ws_sec_prot_cfg_t *cfg)
{
    cfg->sec_prot_trickle_imin = SEC_PROT_SMALL_IMIN;
    cfg->sec_prot_trickle_imax = SEC_PROT_SMALL_IMAX;
    cfg->sec_prot_trickle_timer_exp = 2;
    cfg->sec_prot_retry_timeout = SEC_PROT_RETRY_TIMEOUT_SMALL;
    cfg->sec_max_ongoing_authentication = MAX_SIMULTANEOUS_EAP_TLS_NEGOTIATIONS_MEDIUM;
    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_prot_get(ws_sec_prot_cfg_t *cfg, uint8_t *flags)
{
    ws_sec_prot_cfg_t *get_cfg = NULL;
    ws_cfg_to_get((ws_cfgs_t **) &get_cfg, NULL, NULL, (ws_cfgs_t *) &ws_cfg.sec_prot, 0, flags);
    *cfg = *get_cfg;

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_prot_validate(ws_sec_prot_cfg_t *cfg, ws_sec_prot_cfg_t *new_cfg)
{
    ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, NULL, (ws_cfgs_t *) &ws_cfg.sec_prot, 0, 0);

    if (cfg->sec_prot_trickle_imin != new_cfg->sec_prot_trickle_imin ||
            cfg->sec_prot_trickle_imax != new_cfg->sec_prot_trickle_imax ||
            cfg->sec_prot_trickle_timer_exp != new_cfg->sec_prot_trickle_timer_exp ||
            cfg->sec_prot_retry_timeout != new_cfg->sec_prot_retry_timeout ||
            cfg->sec_max_ongoing_authentication != new_cfg->sec_max_ongoing_authentication) {

        return CFG_SETTINGS_CHANGED;
    }

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_sec_prot_set(protocol_interface_info_entry_t *cur, ws_sec_prot_cfg_t *cfg, ws_sec_prot_cfg_t *new_cfg, uint8_t *flags)
{
    uint8_t cfg_flags;
    int8_t ret = ws_cfg_to_get((ws_cfgs_t **) &cfg, (ws_cfgs_t *) new_cfg, (ws_cfg_validate) ws_cfg_sec_prot_validate, (ws_cfgs_t *) &ws_cfg.sec_prot, &cfg_flags, flags);
    if (ret != CFG_SETTINGS_CHANGED) {
        return ret;
    }

    if (cur && !(cfg_flags & CFG_FLAGS_DISABLE_VAL_SET)) {
        ws_pae_controller_configure(cur, NULL, new_cfg);
    }

    if (cfg == new_cfg) {
        return CFG_SETTINGS_OK;
    }

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
        uint8_t flags = 0;
        if (cfg_cb[index].set(NULL,
                              ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                              ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                              &flags) < 0) {
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
        uint8_t flags = CFG_FLAGS_BOOTSTRAP_RESTART_DISABLE;
        // Validation
        if (cfg_cb[index].set) {
            if (cfg_cb[index].set(cur,
                                  ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                                  ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                                  &flags) < 0) {
                tr_info("FATAL CONFIG FAILURE");
                ret_value = CFG_SETTINGS_OTHER_ERROR;
            }
        }
    }

    return ret_value;
}

int8_t ws_cfg_settings_get(protocol_interface_info_entry_t *cur, ws_cfg_t *cfg)
{
    (void) cur;

    *cfg = ws_cfg;

    ws_cfg_timing_get(&cfg->timing, NULL);
    ws_cfg_rpl_get(&cfg->rpl, NULL);
    ws_cfg_sec_prot_get(&cfg->sec_prot, NULL);

    return CFG_SETTINGS_OK;
}

int8_t ws_cfg_settings_validate(protocol_interface_info_entry_t *cur, struct ws_cfg_s *new_cfg)
{
    (void) cur;

    // Validate new configuration values
    for (uint8_t index = 0; index < CFG_CB_NUM; index++) {
        if (cfg_cb[index].validate) {
            int8_t ret = cfg_cb[index].validate(
                             ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
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
                             ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
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
        uint8_t flags = 0;
        // Validation
        if (call_cfg_set[index]) {
            if (cfg_cb[index].set(cur,
                                  ((uint8_t *)&ws_cfg) + cfg_cb[index].setting_offset,
                                  ((uint8_t *)new_cfg) + cfg_cb[index].setting_offset, &flags) < 0) {
                tr_info("FATAL CONFIG FAILURE");
                ret_value = CFG_SETTINGS_OTHER_ERROR;
            }
        }
    }

    return ret_value;
}

#endif //HAVE_WS
