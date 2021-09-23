/*
 * Copyright (c) 2019-2020, Pelion and affiliates.
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

#ifndef WS_PAE_TIMERS_H_
#define WS_PAE_TIMERS_H_

/**
 * ws_pae_timers_settings_init initializes timer settings structure
 *
 * \param timer_settings timer settings
 * \param new_timer_settings new timer settings
 *
 */
void ws_pae_timers_settings_init(sec_timer_cfg_t *timer_settings, ws_sec_timer_cfg_t *new_timer_settings);

/**
 *  ws_pae_timers_lifetime_set sets GTK, PTK and PTK lifetimes
 *
 * \param timer_settings timer settings
 * \param gtk_lifetime GTK lifetime
 * \param pmk_lifetime PMK lifetime
 * \param ptk_lifetime PTK lifetime
 *
 */
void ws_pae_timers_lifetime_set(sec_timer_cfg_t *timer_settings, uint32_t gtk_lifetime, uint32_t pmk_lifetime, uint32_t ptk_lifetime);

/**
 *  ws_pae_timers_gtk_time_settings_set sets GTK, PTK and PTK lifetimes
 *
 * \param timer_settings timer settings
 * \param revocat_lifetime_reduct revocation lifetime reduction
 * \param new_activation_time new activation time
 * \param new_install_req new install required
 * \param max_mismatch max mismatch
 *
 */
void ws_pae_timers_gtk_time_settings_set(sec_timer_cfg_t *timer_settings, uint8_t revocat_lifetime_reduct, uint8_t new_activation_time, uint8_t new_install_req, uint32_t max_mismatch);

/**
 *  ws_pae_timers_gtk_new_install_required GTK new install required check
 *
 * \param sec_cfg security configuration
 * \param seconds elapsed seconds
 *
 * \return true new GTK install required expired
 * \return false GTK install not required
 *
 */
bool ws_pae_timers_gtk_new_install_required(sec_cfg_t *sec_cfg, uint32_t seconds);

/**
 *  ws_pae_timers_gtk_new_activation_time GTK new activation time
 *
 * \param sec_cfg security configuration
 * \param seconds elapsed seconds
 *
 * \return true GTK new activation time expired
 * \return false GTK new activation time not expired
 *
 */
bool ws_pae_timers_gtk_new_activation_time(sec_cfg_t *sec_cfg, uint32_t seconds);

/**
 *  ws_pae_timers_gtk_revocation_lifetime_get GTK revocation lifetime get
 *
 * \param sec_cfg security configuration
 *
 * \return GTK revocation lifetime
 *
 */
uint32_t ws_pae_timers_gtk_revocation_lifetime_get(sec_cfg_t *sec_cfg);

#endif /* WS_PAE_TIMERS_H_ */
