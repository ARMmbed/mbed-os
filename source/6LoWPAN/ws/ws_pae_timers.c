/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "ws_management_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_pae_timers.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wspt"

#define SECONDS_IN_MINUTE                       60

#define DEFAULT_GTK_REQUEST_IMIN                4                       // 4 minutes
#define DEFAULT_GTK_REQUEST_IMAX                64                      // 64 minutes

static void ws_pae_timers_calculate(sec_timer_cfg_t *timer_settings);

void ws_pae_timers_settings_init(sec_timer_cfg_t *timer_settings, ws_sec_timer_cfg_t *new_timer_settings)
{
    if (timer_settings == NULL || new_timer_settings == NULL) {
        return;
    }

    timer_settings->gtk_expire_offset = new_timer_settings->gtk_expire_offset * SECONDS_IN_MINUTE;
    timer_settings->pmk_lifetime = new_timer_settings->pmk_lifetime * SECONDS_IN_MINUTE;
    timer_settings->ptk_lifetime = new_timer_settings->ptk_lifetime * SECONDS_IN_MINUTE;
    timer_settings->gtk_new_act_time = new_timer_settings->gtk_new_act_time;
    timer_settings->revocat_lifetime_reduct = new_timer_settings->revocat_lifetime_reduct;
    timer_settings->gtk_request_imin = new_timer_settings->gtk_request_imin * SECONDS_IN_MINUTE;
    timer_settings->gtk_request_imax = new_timer_settings->gtk_request_imax * SECONDS_IN_MINUTE;
    timer_settings->gtk_max_mismatch = new_timer_settings->gtk_max_mismatch * SECONDS_IN_MINUTE;
    timer_settings->gtk_new_install_req = new_timer_settings->gtk_new_install_req;

    ws_pae_timers_calculate(timer_settings);
}

void ws_pae_timers_lifetime_set(sec_timer_cfg_t *timer_settings, uint32_t gtk_lifetime, uint32_t pmk_lifetime, uint32_t ptk_lifetime)
{
    if (gtk_lifetime) {
        timer_settings->gtk_expire_offset = gtk_lifetime * 60;
    }
    if (pmk_lifetime) {
        timer_settings->pmk_lifetime = pmk_lifetime * 60;
    }
    if (ptk_lifetime) {
        timer_settings->ptk_lifetime = ptk_lifetime * 60;
    }
    ws_pae_timers_calculate(timer_settings);
}

void ws_pae_timers_gtk_time_settings_set(sec_timer_cfg_t *timer_settings, uint8_t revocat_lifetime_reduct, uint8_t new_activation_time, uint8_t new_install_req, uint32_t max_mismatch)
{
    if (revocat_lifetime_reduct) {
        timer_settings->revocat_lifetime_reduct = revocat_lifetime_reduct;
    }
    if (new_activation_time) {
        timer_settings->gtk_new_act_time = new_activation_time;
    }
    if (new_install_req) {
        timer_settings->gtk_new_install_req = new_install_req;
    }
    if (max_mismatch) {
        timer_settings->gtk_max_mismatch = max_mismatch * 60;
    }
    ws_pae_timers_calculate(timer_settings);
}

static void ws_pae_timers_calculate(sec_timer_cfg_t *timer_settings)
{
    // Calculate GTK_NEW_INSTALL_REQUIRED < 100 * (1 - 1 / REVOCATION_LIFETIME_REDUCTION)
    uint8_t calc_gtk_new_install_req = 100 - (100 / timer_settings->revocat_lifetime_reduct);

    if (timer_settings->gtk_expire_offset < 3600) {
        // For very short GTKs give some more time to distribute the new GTK key to network, tune this if needed
        calc_gtk_new_install_req = calc_gtk_new_install_req * 60 / 100;
    }

    if (timer_settings->gtk_new_install_req > calc_gtk_new_install_req) {
        tr_info("GTK new install required adjusted %i", calc_gtk_new_install_req);
        timer_settings->gtk_new_install_req = calc_gtk_new_install_req;
    }

    // Verify that GTK request Imin and Imax are sensible when compared to revocation lifetime
    uint32_t gtk_revocation_lifetime = timer_settings->gtk_expire_offset / timer_settings->revocat_lifetime_reduct;
    uint32_t new_activation_time = timer_settings->gtk_expire_offset / timer_settings->gtk_new_act_time;

    uint32_t time_to_update = gtk_revocation_lifetime;
    if (gtk_revocation_lifetime > new_activation_time) {
        time_to_update = gtk_revocation_lifetime - new_activation_time;
    }

    tr_info("Key timers revocation lifetime: %"PRIu32", new activation time: %"PRIu32", max mismatch %i, time to update: %"PRIu32"", gtk_revocation_lifetime, new_activation_time, timer_settings->gtk_max_mismatch, time_to_update);

    // If time to update results smaller GTK request Imax use it for calculation otherwise use GTK max mismatch
    if (time_to_update < timer_settings->gtk_max_mismatch) {
        // If time to update is smaller than GTK request Imax update GTK request values
        if (timer_settings->gtk_request_imax > time_to_update) {
            timer_settings->gtk_request_imin = time_to_update / 4;
            timer_settings->gtk_request_imax = time_to_update / 2;
            tr_info("GTK request timers adjusted Imin: %i, Imax: %i", timer_settings->gtk_request_imin, timer_settings->gtk_request_imax);
        }
    } else if (timer_settings->gtk_request_imax > timer_settings->gtk_max_mismatch) {
        // If GTK request Imax is larger than GTK max mismatch update GTK request values

        // For small GTK max mismatch times, scale the Imin to be larger than default  4 / 64;
        uint16_t scaler;
        if (timer_settings->gtk_max_mismatch < 50) {
            scaler = 10;
        } else if (timer_settings->gtk_max_mismatch > 600) {
            scaler = 1;
        } else {
            // About 1 minute mismatch, results 37 seconds Imin and 60 seconds Imax
            scaler = (600 - timer_settings->gtk_max_mismatch) / 54;
        }

        timer_settings->gtk_request_imin = timer_settings->gtk_max_mismatch * scaler * DEFAULT_GTK_REQUEST_IMIN / DEFAULT_GTK_REQUEST_IMAX;
        timer_settings->gtk_request_imax = timer_settings->gtk_max_mismatch;
        tr_info("GTK request timers adjusted Imin: %i, Imax: %i", timer_settings->gtk_request_imin, timer_settings->gtk_request_imax);
    }
}

bool ws_pae_timers_gtk_new_install_required(sec_timer_cfg_t *timer_settings, uint32_t seconds)
{
    uint32_t gtk_new_install_req_seconds = timer_settings->gtk_expire_offset - timer_settings->gtk_new_install_req * timer_settings->gtk_expire_offset / 100;

    if (seconds < gtk_new_install_req_seconds) {
        return true;
    } else {
        return false;
    }
}

bool ws_pae_timers_gtk_new_activation_time(sec_timer_cfg_t *timer_settings, uint32_t seconds)
{
    uint32_t gtk_gtk_new_activation_time_seconds = timer_settings->gtk_expire_offset / timer_settings->gtk_new_act_time;

    if (seconds < gtk_gtk_new_activation_time_seconds) {
        return true;
    } else {
        return false;
    }
}

uint32_t ws_pae_timers_gtk_revocation_lifetime_get(sec_timer_cfg_t *timer_settings)
{
    return timer_settings->gtk_expire_offset / timer_settings->revocat_lifetime_reduct;
}

#endif /* HAVE_WS */
