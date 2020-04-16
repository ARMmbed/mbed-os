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

#ifndef WS_CFG_STORAGE_H_
#define WS_CFG_STORAGE_H_

/**
 * \brief Struct ws_gen_cfg_t General configuration
 */
typedef struct ws_gen_cfg_s {
    /* Changing the network size resets the configuration settings depending on it to
       default values */
    uint8_t network_size;               /**< Network size selection; default medium (= 8) */
    char network_name[33];              /**< Network name; max 32 octets + terminating 0 */
    uint16_t network_pan_id;            /**< PAN identifier; PAN_ID; default 0xffff */
} ws_gen_cfg_t;

/**
 * \brief Struct ws_phy_cfg_t Physical layer configuration
 */
typedef struct ws_phy_cfg_s {
    uint8_t regulatory_domain;          /**< PHY regulatory domain; default "KR" 0x09 */
    uint8_t operating_class;            /**< PHY operating class; default 1 */
    uint8_t operating_mode;             /**< PHY operating mode; default "1b" symbol rate 50, modulation index 1 */
} ws_phy_cfg_t;

/**
 * \brief Struct ws_timing_cfg_t Timing configuration
 */
typedef struct ws_timing_cfg_s {
    uint16_t disc_trickle_imin;         /**< Discovery trickle Imin; DISC_IMIN; seconds; range 1-255; default 30 */
    uint16_t disc_trickle_imax;         /**< Discovery trickle Imax; DISC_IMAX; seconds; range (2-2^8)*Imin‬; default 960 */
    uint8_t disc_trickle_k;             /**< Discovery trickle k; DISC_K; default 1 */
    uint16_t pan_timeout;               /**< PAN timeout; PAN_TIMEOUT; seconds; range 60-15300; default 3840 */
    uint16_t temp_link_min_timeout;     /**< Temporary neighbor link minimum timeout; seconds; default 260 */
} ws_timing_cfg_t;

/**
 * \brief Struct ws_rpl_cfg_t RPL configuration
 */
typedef struct ws_rpl_cfg_s {
    uint8_t dio_interval_min;           /**> DIO interval min; DEFAULT_DIO_INTERVAL_MIN; 2^value in milliseconds; range 1-255; default */
    uint8_t dio_interval_doublings;     /**> DIO interval doublings; DEFAULT_DIO_INTERVAL_DOUBLINGS; range 1-8; default */
    uint8_t dio_redundancy_constant;    /**> DIO redundancy constant; DEFAULT_DIO_REDUNDANCY_CONSTANT; range 0-10; default */
    uint16_t dag_max_rank_increase;
    uint16_t min_hop_rank_increase;
    uint16_t rpl_parent_candidate_max;  /**< RPL parent candidate maximum value; default 5 */
    uint16_t rpl_selected_parent_max;   /**< RPL selected parent maximum value; default 2 */
} ws_rpl_cfg_t;

/**
 * \brief Struct ws_fhss_cfg_t Frequency hopping configuration
 */
typedef struct ws_fhss_cfg_s {
    uint8_t fhss_uc_dwell_interval;     /**< FHSS unicast dwell interval; range 15-250 milliseconds; default 255 */
    uint8_t fhss_bc_dwell_interval;     /**< FHSS broadcast dwell interval; range 15-250 milliseconds; default 255 */
    uint32_t fhss_bc_interval;          /**< FHSS broadcast interval; duration between broadcast dwell intervals. range: 0-16777216 milliseconds; default 1020 */
    uint8_t fhss_uc_channel_function;   /**< FHSS WS unicast channel function; default 2 direct hash channel function */
    uint8_t fhss_bc_channel_function;   /**< FHSS WS broadcast channel function; default 2 direct hash channel function */
    uint16_t fhss_uc_fixed_channel;     /**< FHSS unicast fixed channel; default 0xffff */
    uint16_t fhss_bc_fixed_channel;     /**< FHSS broadcast fixed channel; default 0xffff */
    uint32_t fhss_channel_mask[8];      /**< FHSS channel mask; default; 0xffffffff * 8 */
} ws_fhss_cfg_t;

/**
 * \brief Struct ws_mpl_cfg_t Multicast configuration
 */
typedef struct ws_mpl_cfg_s {
    uint16_t mpl_trickle_imin;          /**< MPL trickle parameters Imin; DATA_MESSAGE_IMIN; seconds; range 1-255; default 10 */
    uint16_t mpl_trickle_imax;          /**< MPL trickle parameters Imax; DATA_MESSAGE_IMAX; seconds; range (2-2^8)*Imin‬;  default 10 */
    uint8_t mpl_trickle_k;              /**< MPL trickle parameters k; default 8 */
    uint8_t mpl_trickle_timer_exp;      /**< MPL trickle parameters timer expirations; default 3 */
    uint16_t seed_set_entry_lifetime;   /**< MPL minimum seed set lifetime; seconds; default 960 */
} ws_mpl_cfg_t;

/**
 * \brief Struct ws_sec_timer_cfg_t Security timers configuration
 */
typedef struct ws_sec_timer_cfg_s {
    uint32_t gtk_expire_offset;         /**< GTK lifetime; GTK_EXPIRE_OFFSET; minutes; default 43200 */
    uint32_t pmk_lifetime;              /**< PMK lifetime; minutes; default 172800 */
    uint32_t ptk_lifetime;              /**< PTK lifetime; minutes; default 86400 */
    uint16_t gtk_new_act_time;          /**< GTK_NEW_ACTIVATION_TIME (1/X of expire offset); default 720 */
    uint16_t revocat_lifetime_reduct;   /**< REVOCATION_LIFETIME_REDUCTION (reduction of lifetime); default 30 */
    uint16_t gtk_request_imin;          /**< GTK_REQUEST_IMIN; minutes; range 1-255; default 4 */
    uint16_t gtk_request_imax;          /**< GTK_REQUEST_IMAX; minutes; range (2-2^8)*Imin; default 64 */
    uint16_t gtk_max_mismatch;          /**< GTK_MAX_MISMATCH; minutes; default 64 */
    uint8_t gtk_new_install_req;        /**< GTK_NEW_INSTALL_REQUIRED; percent of GTK lifetime; range 1-100; default 80 */
} ws_sec_timer_cfg_t;

/**
 * \brief Struct ws_sec_prot_cfg_t Security protocols configuration
 */
typedef struct ws_sec_prot_cfg_s {
    uint16_t sec_prot_retry_timeout;    /**< Security protocol retry timeout; seconds; default 330 */
    uint16_t sec_prot_trickle_imin;     /**< Security protocol trickle parameters Imin; seconds; default 30 */
    uint16_t sec_prot_trickle_imax;     /**< Security protocol trickle parameters Imax; seconds; default 90 */
    uint8_t sec_prot_trickle_timer_exp; /**< Security protocol trickle timer expirations; default 2 */
    uint16_t sec_max_ongoing_authentication; /**< Pae authenticator max Accept ongoing authentication count */
} ws_sec_prot_cfg_t;

/**
 * \brief Struct ws_nw_size_cfg_t Network size configuration
 */
typedef struct ws_cfg_s {
    ws_gen_cfg_t gen;                   /**< General configuration */
    ws_phy_cfg_t phy;                   /**< Physical layer configuration */
    ws_timing_cfg_t timing;             /**< Timing configuration */
    ws_rpl_cfg_t rpl;                   /**< RPL configuration */
    ws_fhss_cfg_t fhss;                 /**< Frequency hopping configuration */
    ws_mpl_cfg_t mpl;                   /**< Multicast configuration */
    ws_sec_timer_cfg_t sec_timer;       /**< Security timers configuration */
    ws_sec_prot_cfg_t sec_prot;         /**< Security protocols configuration */
} ws_cfg_t;

/** Configuration setting errors. */
#define CFG_SETTINGS_PARAMETER_ERROR         -1    /**< Function parameter error */
#define CFG_SETTINGS_OTHER_ERROR             -2    /**< Other error */
#define CFG_SETTINGS_ERROR_NW_SIZE_CONF      -10   /**< Network size configuration error */
#define CFG_SETTINGS_ERROR_GEN_CONF          -11   /**< General configuration error */
#define CFG_SETTINGS_ERROR_PHY_CONF          -12   /**< Physical layer configuration error */
#define CFG_SETTINGS_ERROR_TIMING_CONF       -13   /**< Timing configuration error */
#define CFG_SETTINGS_ERROR_RPL_CONF          -14   /**< RPL configuration error */
#define CFG_SETTINGS_ERROR_FHSS_CONF         -15   /**< Frequency hopping configuration error */
#define CFG_SETTINGS_ERROR_MPL_CONF          -16   /**< Multicast configuration error */
#define CFG_SETTINGS_ERROR_SEC_TIMER_CONF    -17   /**< Security timers configuration error */
#define CFG_SETTINGS_ERROR_SEC_PROT_CONF     -18   /**< Security protocols configuration error */

int8_t ws_cfg_settings_init(void);
int8_t ws_cfg_settings_default_set(void);
int8_t ws_cfg_settings_interface_set(protocol_interface_info_entry_t *cur);
int8_t ws_cfg_network_size_configure(protocol_interface_info_entry_t *cur, uint16_t network_size);

int8_t ws_cfg_network_size_get(ws_gen_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_network_size_validate(ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg);
int8_t ws_cfg_network_size_set(protocol_interface_info_entry_t *cur, ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_gen_get(ws_gen_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_gen_validate(ws_gen_cfg_t  *cfg, ws_gen_cfg_t  *new_cfg);
int8_t ws_cfg_gen_set(protocol_interface_info_entry_t *cur, ws_gen_cfg_t *cfg, ws_gen_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_phy_get(ws_phy_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_phy_validate(ws_phy_cfg_t *cfg, ws_phy_cfg_t *new_cfg);
int8_t ws_cfg_phy_set(protocol_interface_info_entry_t *cur, ws_phy_cfg_t *cfg, ws_phy_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_timing_get(ws_timing_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_timing_validate(ws_timing_cfg_t *cfg, ws_timing_cfg_t *new_cfg);
int8_t ws_cfg_timing_set(protocol_interface_info_entry_t *cur, ws_timing_cfg_t *cfg, ws_timing_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_rpl_get(ws_rpl_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_rpl_validate(ws_rpl_cfg_t *cfg, ws_rpl_cfg_t *new_cfg);
int8_t ws_cfg_rpl_set(protocol_interface_info_entry_t *cur, ws_rpl_cfg_t *cfg, ws_rpl_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_mpl_get(ws_mpl_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_mpl_validate(ws_mpl_cfg_t *cfg, ws_mpl_cfg_t *new_cfg);
int8_t ws_cfg_mpl_set(protocol_interface_info_entry_t *cur, ws_mpl_cfg_t *cfg, ws_mpl_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_fhss_get(ws_fhss_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_fhss_validate(ws_fhss_cfg_t *cfg, ws_fhss_cfg_t *new_cfg);
int8_t ws_cfg_fhss_set(protocol_interface_info_entry_t *cur, ws_fhss_cfg_t *cfg, ws_fhss_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_sec_timer_get(ws_sec_timer_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_sec_timer_validate(ws_sec_timer_cfg_t *cfg, ws_sec_timer_cfg_t *new_cfg);
int8_t ws_cfg_sec_timer_set(protocol_interface_info_entry_t *cur, ws_sec_timer_cfg_t *cfg, ws_sec_timer_cfg_t *new_cfg, uint8_t *flags);

int8_t ws_cfg_sec_prot_get(ws_sec_prot_cfg_t *cfg, uint8_t *flags);
int8_t ws_cfg_sec_prot_validate(ws_sec_prot_cfg_t *cfg, ws_sec_prot_cfg_t *new_cfg);
int8_t ws_cfg_sec_prot_set(protocol_interface_info_entry_t *cur, ws_sec_prot_cfg_t *cfg, ws_sec_prot_cfg_t *new_cfg, uint8_t *flags);

#endif // WS_CFG_STORAGE_H_
