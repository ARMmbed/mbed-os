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
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include <net_ws_test.h>
#include "fhss_config.h"
#include "ws_management_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "randLIB.h"

#include "ns_trace.h"
#include "common_functions.h"

#define TRACE_GROUP "wste"

#ifdef HAVE_WS

int ws_test_pan_size_set(int8_t interface_id, uint16_t pan_size)
{

    (void) interface_id;
#ifdef HAVE_WS_BORDER_ROUTER
    test_pan_size_override = pan_size;
    return 0;
#else
    (void) pan_size;
    return -1;
#endif
}

int ws_test_max_child_count_set(int8_t interface_id, uint16_t child_count)
{

    (void) interface_id;
    test_max_child_count_override = child_count;
    return 0;
}

int ws_test_gtk_set(int8_t interface_id, uint8_t *gtk[4])
{
    (void) interface_id;
    (void) gtk;

    return ws_pae_controller_gtk_update(interface_id, gtk);
}

int ws_test_active_key_set(int8_t interface_id, uint8_t index)
{
    (void) interface_id;
    (void) index;

    return ws_pae_controller_active_key_update(interface_id, index);
}

int ws_test_key_lifetime_set(int8_t interface_id, uint32_t gtk_lifetime, uint32_t pmk_lifetime, uint32_t ptk_lifetime)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }

    ws_sec_timer_cfg_t cfg;
    if (ws_cfg_sec_timer_get(&cfg, NULL) < 0) {
        return -2;
    }

    if (gtk_lifetime > 0) {
        cfg.gtk_expire_offset = gtk_lifetime;
    }
    if (pmk_lifetime > 0) {
        cfg.pmk_lifetime = pmk_lifetime;
    }
    if (ptk_lifetime > 0) {
        cfg.ptk_lifetime = ptk_lifetime;
    }

    if (ws_cfg_sec_timer_set(cur, NULL, &cfg, NULL) < 0) {
        return -3;
    }

    return 0;
}

int ws_test_gtk_time_settings_set(int8_t interface_id, uint8_t revocat_lifetime_reduct, uint8_t new_activation_time, uint8_t new_install_req, uint32_t max_mismatch)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }

    ws_sec_timer_cfg_t cfg;
    if (ws_cfg_sec_timer_get(&cfg, NULL) < 0) {
        return -2;
    }

    if (revocat_lifetime_reduct > 0) {
        cfg.revocat_lifetime_reduct = revocat_lifetime_reduct;
    }
    if (new_activation_time > 0) {
        cfg.gtk_new_act_time = new_activation_time;
    }
    if (new_install_req > 0) {
        cfg.gtk_new_install_req = new_install_req;
    }
    if (max_mismatch > 0) {
        cfg.gtk_max_mismatch = max_mismatch;
    }

    if (ws_cfg_sec_timer_set(cur, NULL, &cfg, NULL) < 0) {
        return -3;
    }

    return 0;
}

int ws_test_next_gtk_set(int8_t interface_id, uint8_t *gtk[4])
{
    (void) interface_id;
    (void) gtk;

    return ws_pae_controller_next_gtk_update(interface_id, gtk);
}

#endif // HAVE_WS
