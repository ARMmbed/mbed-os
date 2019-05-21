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
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
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
    (void) interface_id;
    (void) gtk_lifetime;
    (void) pmk_lifetime;
    (void) ptk_lifetime;

    return ws_pae_controller_key_lifetime_update(interface_id, gtk_lifetime, pmk_lifetime, ptk_lifetime);
}

int ws_test_gtk_time_settings_set(int8_t interface_id, uint8_t revocat_lifetime_reduct, uint8_t new_activation_time, uint8_t new_install_req, uint32_t max_mismatch)
{
    (void) interface_id;
    (void) revocat_lifetime_reduct;
    (void) new_activation_time;
    (void) new_install_req;
    (void) max_mismatch;


    return ws_pae_controller_gtk_time_settings_update(interface_id, revocat_lifetime_reduct, new_activation_time, new_install_req, max_mismatch);
}

int ws_test_next_gtk_set(int8_t interface_id, uint8_t *gtk[4])
{
    (void) interface_id;
    (void) gtk;

    return ws_pae_controller_next_gtk_update(interface_id, gtk);
}

#endif // HAVE_WS
