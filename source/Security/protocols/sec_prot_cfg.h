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

#ifndef SEC_PROT_CFG_H_
#define SEC_PROT_CFG_H_

/* Security protocol configuration settings */

typedef struct sec_prot_cfg_s {
    trickle_params_t sec_prot_trickle_params;
    uint16_t sec_prot_retry_timeout;
    uint16_t initial_key_retry_min;
    uint16_t initial_key_retry_max;
    uint16_t initial_key_retry_max_limit;
    uint8_t initial_key_retry_cnt;
    uint8_t max_ongoing_auth_constant;
    uint16_t max_ongoing_auth_size_scaler;
} sec_prot_cfg_t;

/* Security timer configuration settings */

typedef struct sec_timer_cfg_s {
    uint32_t gtk_expire_offset;                      /* GTK lifetime; GTK_EXPIRE_OFFSET (seconds) */
    uint32_t pmk_lifetime;                           /* PMK lifetime (seconds) */
    uint32_t ptk_lifetime;                           /* PTK lifetime (seconds) */
    uint16_t gtk_new_act_time;                       /* GTK_NEW_ACTIVATION_TIME (1/X of expire offset) */
    uint16_t revocat_lifetime_reduct;                /* REVOCATION_LIFETIME_REDUCTION (reduction of lifetime) */
    uint16_t gtk_request_imin;                       /* GTK_REQUEST_IMIN (seconds) */
    uint16_t gtk_request_imax;                       /* GTK_REQUEST_IMAX (seconds) */
    uint16_t gtk_max_mismatch;                       /* GTK_MAX_MISMATCH (seconds) */
    uint8_t gtk_new_install_req;                     /* GTK_NEW_INSTALL_REQUIRED (percent of GTK lifetime) */
} sec_timer_cfg_t;

/* Security radius configuration settings */

typedef struct sec_radius_cfg_s {
    uint8_t radius_addr[16];                         /**< Radius server IPv6 address */
    const uint8_t *radius_shared_secret;             /**< Radius shared secret */
    uint16_t radius_shared_secret_len;               /**< Radius shared secret length */
    trickle_params_t radius_retry_trickle_params;    /**< Radius retry trickle params */
    bool radius_addr_set : 1;                        /**< Radius server address is set */
} sec_radius_cfg_t;

typedef struct sec_timing_cfg_s {
    uint16_t temp_eapol_min_timeout;                 /**< Temporary neighbor link minimum timeout; seconds; default 330 */
} sec_timing_cfg_t;

typedef struct sec_cfg_s {
    sec_prot_cfg_t prot_cfg;
    sec_timer_cfg_t timer_cfg;
    sec_timing_cfg_t timing_cfg;
    sec_radius_cfg_t *radius_cfg;
} sec_cfg_t;

#endif /* SEC_PROT_CONF_H_ */
