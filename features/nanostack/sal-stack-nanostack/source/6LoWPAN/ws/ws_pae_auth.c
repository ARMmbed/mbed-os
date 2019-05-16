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
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "ns_address.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_socket_if.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/key_sec_prot/key_sec_prot.h"
#include "Security/protocols/eap_tls_sec_prot/auth_eap_tls_sec_prot.h"
#include "Security/protocols/tls_sec_prot/tls_sec_prot.h"
#include "Security/protocols/fwh_sec_prot/auth_fwh_sec_prot.h"
#include "Security/protocols/gkh_sec_prot/auth_gkh_sec_prot.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_pae_auth.h"
#include "6LoWPAN/ws/ws_pae_lib.h"

#ifdef HAVE_WS
#ifdef HAVE_PAE_AUTH

#define TRACE_GROUP "wspa"

#define PAE_TASKLET_INIT                       1
#define PAE_TASKLET_EVENT                      2
#define PAE_TASKLET_TIMER                      3

// Wait for for supplicant to indicate activity (e.g. to send a message)
#define WAIT_FOR_AUTHENTICATION_TICKS          5 * 60 * 10  // 5 minutes

typedef struct {
    ns_list_link_t link;                                     /**< Link */
    kmp_service_t *kmp_service;                              /**< KMP service */
    protocol_interface_info_entry_t *interface_ptr;          /**< Interface pointer */
    supp_list_t active_supp_list;                            /**< List of active supplicants */
    supp_list_t inactive_supp_list;                          /**< List of inactive supplicants */
    arm_event_storage_t *timer;                              /**< Timer */
    sec_prot_gtk_keys_t *gtks;                               /**< GTKs */
    const sec_prot_certs_t *certs;                           /**< Certificates */
    bool timer_running;                                      /**< Timer is running */
} pae_auth_t;

static void ws_pae_auth_free(pae_auth_t *pae_auth);
static pae_auth_t *ws_pae_auth_get(protocol_interface_info_entry_t *interface_ptr);
static pae_auth_t *ws_pae_auth_by_kmp_service_get(kmp_service_t *service);
static int8_t ws_pae_auth_event_send(kmp_service_t *service, void *data);
static void ws_pae_auth_tasklet_handler(arm_event_s *event);
static int8_t ws_pae_auth_timer_if_start(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_auth_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp);
static int8_t ws_pae_auth_timer_start(pae_auth_t *pae_auth);
static int8_t ws_pae_auth_timer_stop(pae_auth_t *pae_auth);
static bool ws_pae_auth_timer_running(pae_auth_t *pae_auth);
static void ws_pae_auth_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr);
static kmp_api_t *ws_pae_auth_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type);
static kmp_api_t *ws_pae_auth_kmp_incoming_ind(kmp_service_t *service, kmp_type_e type, const kmp_addr_t *addr);
static void ws_pae_auth_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result);
static void ws_pae_auth_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr);
static void ws_pae_auth_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys);
static kmp_api_t *ws_pae_auth_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, supp_entry_t *supp_entry);
static void ws_pae_auth_kmp_api_finished(kmp_api_t *kmp);

static int8_t tasklet_id = -1;
static NS_LIST_DEFINE(pae_auth_list, pae_auth_t, link);

int8_t ws_pae_auth_init(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs)
{
    if (!interface_ptr || !gtks || !certs) {
        return -1;
    }

    if (ws_pae_auth_get(interface_ptr) != NULL) {
        return 0;
    }

    pae_auth_t *pae_auth = ns_dyn_mem_alloc(sizeof(pae_auth_t));
    if (!pae_auth) {
        return -1;
    }

    pae_auth->interface_ptr = interface_ptr;
    ws_pae_lib_supp_list_init(&pae_auth->active_supp_list);
    ws_pae_lib_supp_list_init(&pae_auth->inactive_supp_list);
    pae_auth->timer = NULL;

    pae_auth->gtks = gtks;
    pae_auth->certs = certs;

    pae_auth->kmp_service = kmp_service_create();
    if (!pae_auth->kmp_service) {
        goto error;
    }

    if (kmp_service_cb_register(pae_auth->kmp_service, ws_pae_auth_kmp_incoming_ind, ws_pae_auth_kmp_service_addr_get, ws_pae_auth_kmp_service_api_get)) {
        goto error;
    }

    if (kmp_service_event_if_register(pae_auth->kmp_service, ws_pae_auth_event_send)) {
        goto error;
    }

    if (kmp_service_timer_if_register(pae_auth->kmp_service, ws_pae_auth_timer_if_start, ws_pae_auth_timer_if_stop)) {
        goto error;
    }

    if (key_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (auth_eap_tls_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (server_tls_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (auth_fwh_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (auth_gkh_sec_prot_register(pae_auth->kmp_service) < 0) {
        goto error;
    }

    if (tasklet_id < 0) {
        tasklet_id = eventOS_event_handler_create(ws_pae_auth_tasklet_handler, PAE_TASKLET_INIT);
        if (tasklet_id < 0) {
            goto error;
        }
    }

    if (ws_pae_auth_timer_stop(pae_auth) < 0) {
        goto error;
    }

    ns_list_add_to_end(&pae_auth_list, pae_auth);

    return 0;

error:
    ws_pae_auth_free(pae_auth);

    return -1;
}

int8_t ws_pae_auth_addresses_set(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port)
{
    if (!interface_ptr || !remote_addr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }
    if (!pae_auth->kmp_service) {
        return -1;
    }

    if (kmp_socket_if_register(pae_auth->kmp_service, local_port, remote_addr, remote_port) < 0) {
        return -1;
    }

    return 0;
}

int8_t ws_pae_auth_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    pae_auth_t *pae_auth = ws_pae_auth_get(interface_ptr);
    if (!pae_auth) {
        return -1;
    }

    ws_pae_auth_free(pae_auth);
    return 0;
}

static void ws_pae_auth_free(pae_auth_t *pae_auth)
{
    if (!pae_auth) {
        return;
    }

    ws_pae_lib_supp_list_delete(&pae_auth->active_supp_list);
    ws_pae_lib_supp_list_delete(&pae_auth->inactive_supp_list);

    kmp_socket_if_unregister(pae_auth->kmp_service);

    kmp_service_delete(pae_auth->kmp_service);

    ns_list_remove(&pae_auth_list, pae_auth);
    ns_dyn_mem_free(pae_auth);
}

static pae_auth_t *ws_pae_auth_get(protocol_interface_info_entry_t *interface_ptr)
{
    ns_list_foreach(pae_auth_t, entry, &pae_auth_list) {
        if (entry->interface_ptr == interface_ptr) {
            return entry;
        }
    }

    return NULL;
}

static pae_auth_t *ws_pae_auth_by_kmp_service_get(kmp_service_t *service)
{
    ns_list_foreach(pae_auth_t, entry, &pae_auth_list) {
        if (entry->kmp_service == service) {
            return entry;
        }
    }

    return NULL;
}

static int8_t ws_pae_auth_event_send(kmp_service_t *service, void *data)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return -1;
    }

    arm_event_s event = {
        .receiver = tasklet_id,
        .sender = 0,
        .event_id = pae_auth->interface_ptr->id,
        .data_ptr = data,
        .event_type = PAE_TASKLET_EVENT,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        return -1;
    }

    return 0;
}

static void ws_pae_auth_tasklet_handler(arm_event_s *event)
{
    if (event->event_type == PAE_TASKLET_INIT) {

    } else if (event->event_type == PAE_TASKLET_EVENT) {
        pae_auth_t *pae_auth = NULL;

        ns_list_foreach(pae_auth_t, entry, &pae_auth_list) {
            if (entry->interface_ptr->id == event->event_id) {
                pae_auth = entry;
                break;
            }
        }

        if (pae_auth) {
            kmp_service_event_if_event(pae_auth->kmp_service, event->data_ptr);
        }
    }
}

void ws_pae_auth_timer(uint16_t ticks)
{
    ns_list_foreach(pae_auth_t, pae_auth, &pae_auth_list) {
        if (!ws_pae_auth_timer_running(pae_auth)) {
            continue;
        }

        // Updates KMP timers
        bool running = ws_pae_lib_supp_list_timer_update(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, ticks, kmp_service_timer_if_timeout);
        if (!running) {
            ws_pae_auth_timer_stop(pae_auth);
        }
    }
}

static int8_t ws_pae_auth_timer_if_start(kmp_service_t *service, kmp_api_t *kmp)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return -1;
    }

    if (ws_pae_auth_timer_start(pae_auth) < 0) {
        return -1;
    }

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        return -1;
    }

    kmp_entry_t *entry = ws_pae_lib_kmp_list_entry_get(&supp_entry->kmp_list, kmp);
    if (!entry) {
        return -1;
    }

    ws_pae_lib_supp_list_to_active(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, supp_entry);

    ws_pae_lib_kmp_timer_start(&supp_entry->kmp_list, entry);
    return 0;
}

static int8_t ws_pae_auth_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp)
{
    (void) service;

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);

    kmp_entry_t *entry = ws_pae_lib_kmp_list_entry_get(&supp_entry->kmp_list, kmp);
    if (!entry) {
        return -1;
    }

    ws_pae_lib_kmp_timer_stop(&supp_entry->kmp_list, entry);
    return 0;
}

static int8_t ws_pae_auth_timer_start(pae_auth_t *pae_auth)
{
    pae_auth->timer_running = true;
    return 0;
}

static int8_t ws_pae_auth_timer_stop(pae_auth_t *pae_auth)
{
    pae_auth->timer_running = false;
    return 0;
}

static bool ws_pae_auth_timer_running(pae_auth_t *pae_auth)
{
    return pae_auth->timer_running;
}

static void ws_pae_auth_kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr)
{
    (void) service;

#if 0
    // Get own EUI-64
    link_layer_address_s mac_params;
    if (arm_nwk_mac_address_read(pae_auth->interface_ptr->id, &mac_params) >= 0) {
        kmp_address_eui_64_set(local_addr, mac_params.mac_long);
    }
#endif

    // For now fixed since not yet support for EA-IE in supplicants
    uint8_t addr[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    kmp_address_eui_64_set(local_addr, addr);

    // Get supplicant address
    supp_entry_t *entry = kmp_api_data_get(kmp);
    if (entry && entry->addr) {
        kmp_address_copy(remote_addr, entry->addr);
    }
}

static kmp_api_t *ws_pae_auth_kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type)
{
    (void) service;

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        return NULL;
    }

    return ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, type);
}

static kmp_api_t *ws_pae_auth_kmp_incoming_ind(kmp_service_t *service, kmp_type_e type, const kmp_addr_t *addr)
{
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return NULL;
    }

    // Find supplicant from list of active supplicants
    supp_entry_t *supp_entry = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->active_supp_list, kmp_address_eui_64_get(addr));

    if (!supp_entry) {
        // Find supplicant from list of inactive supplicants
        supp_entry = ws_pae_lib_supp_list_entry_eui_64_get(&pae_auth->inactive_supp_list, kmp_address_eui_64_get(addr));
        if (supp_entry) {
            // Move supplicant to active list
            ws_pae_lib_supp_list_to_active(&pae_auth->active_supp_list, &pae_auth->inactive_supp_list, supp_entry);
        }
    }

    // If does not exists add it to list
    if (!supp_entry) {
        supp_entry = ws_pae_lib_supp_list_add(&pae_auth->active_supp_list, addr);
        if (!supp_entry) {
            return 0;
        }
        sec_prot_keys_init(&supp_entry->sec_keys, pae_auth->gtks, pae_auth->certs);
    } else {
        // Updates relay address
        kmp_address_copy(supp_entry->addr, addr);
    }

    // Increases waiting time for supplicant authentication
    ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_FOR_AUTHENTICATION_TICKS);

    // Get KMP for supplicant
    kmp_api_t *kmp = ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, type);
    if (kmp) {
        return kmp;
    }

    // Create a new KMP for initial eapol-key
    kmp = kmp_api_create(service, type + IEEE_802_1X_INITIAL_KEY);

    if (!kmp) {
        return 0;
    }

    kmp_api_data_set(kmp, supp_entry);
    // Sets address to KMP
    kmp_api_addr_set(kmp, supp_entry->addr);

    // Sets security keys to KMP
    kmp_api_sec_keys_set(kmp, &supp_entry->sec_keys);

    if (ws_pae_lib_kmp_list_add(&supp_entry->kmp_list, kmp) == NULL) {
        kmp_api_delete(kmp);
        return 0;
    }

    kmp_api_cb_register(kmp,
                        ws_pae_auth_kmp_api_create_confirm,
                        ws_pae_auth_kmp_api_create_indication,
                        ws_pae_auth_kmp_api_finished_indication,
                        ws_pae_auth_kmp_api_finished);

    if (kmp_api_start(kmp) < 0) {
        ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);
        return 0;
    }

    return kmp;
}

static void ws_pae_auth_kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result)
{
    (void) kmp;
    (void) result;
    // If KMP-CREATE.request has failed, authentication error, just stop for now
}

static void ws_pae_auth_kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr)
{
    (void) type;
    (void) addr;
    // For now, accept every KMP-CREATE.indication
    kmp_api_create_response(kmp, KMP_RESULT_OK);
}

static void ws_pae_auth_kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys)
{
    (void) sec_keys;

    // For now, just ignore if not ok
    if (result != KMP_RESULT_OK) {
        return;
    }

    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        // Should not be possible
        return;
    }

    // Gets type
    kmp_type_e type = kmp_api_type_get(kmp);

    if (type > IEEE_802_1X_INITIAL_KEY) {
        // For EAPOL-key, start EAP-TLS towards supplicant
        type = IEEE_802_1X_MKA;
        tr_debug("PAE start EAP-TLS, eui-64: %s", trace_array(kmp_address_eui_64_get(supp_entry->addr), 8));
    } else if (type == IEEE_802_1X_MKA) {
        // After EAP-TLS start 4WH towards supplicant
        type = IEEE_802_11_4WH;
        // Insert GTK0
        sec_prot_keys_gtk_insert_index_set(supp_entry->sec_keys.gtks, 0);
        tr_debug("PAE start 4WH, eui-64: %s", trace_array(kmp_address_eui_64_get(supp_entry->addr), 8));
    } else if (type == IEEE_802_11_4WH) {
        // After 4WH start GKH towards supplicant
        type = IEEE_802_11_GKH;
        // Insert GTK1
        sec_prot_keys_gtk_insert_index_set(supp_entry->sec_keys.gtks, 1);
        tr_debug("PAE start GKH, eui-64: %s", trace_array(kmp_address_eui_64_get(supp_entry->addr), 8));
    } else if (type == IEEE_802_11_GKH) {
        tr_debug("PAE authenticated, eui-64: %s", trace_array(kmp_address_eui_64_get(supp_entry->addr), 8));
        // After GKH end
        return;
    }

    // Increases waiting time for supplicant authentication
    ws_pae_lib_supp_timer_ticks_set(supp_entry, WAIT_FOR_AUTHENTICATION_TICKS);

    kmp_service_t *service = kmp_api_service_get(kmp);
    pae_auth_t *pae_auth = ws_pae_auth_by_kmp_service_get(service);
    if (!pae_auth) {
        return;
    }

    // Create new instance
    kmp_api_t *new_kmp = ws_pae_auth_kmp_create_and_start(pae_auth->kmp_service, type, supp_entry);
    if (!new_kmp) {
        return;
    }

    // For EAP-TLS create also TLS in addition to EAP-TLS
    if (type == IEEE_802_1X_MKA) {
        if (ws_pae_lib_kmp_list_type_get(&supp_entry->kmp_list, TLS_PROT) != NULL) {
            // TLS already exists, wait for it to be deleted
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return;
        }
        // Create TLS instance */
        if (ws_pae_auth_kmp_create_and_start(service, TLS_PROT, supp_entry) == NULL) {
            ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, new_kmp);
            return;
        }
    }

    kmp_api_create_request(new_kmp, type, supp_entry->addr, &supp_entry->sec_keys);
}

static kmp_api_t *ws_pae_auth_kmp_create_and_start(kmp_service_t *service, kmp_type_e type, supp_entry_t *supp_entry)
{
    // Create KMP instance for new authentication
    kmp_api_t *kmp = kmp_api_create(service, type);

    if (!kmp) {
        return NULL;
    }

    if (ws_pae_lib_kmp_list_add(&supp_entry->kmp_list, kmp) == NULL) {
        kmp_api_delete(kmp);
        return NULL;
    }

    kmp_api_cb_register(kmp,
                        ws_pae_auth_kmp_api_create_confirm,
                        ws_pae_auth_kmp_api_create_indication,
                        ws_pae_auth_kmp_api_finished_indication,
                        ws_pae_auth_kmp_api_finished);

    kmp_api_data_set(kmp, supp_entry);

    if (kmp_api_start(kmp) < 0) {
        ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);
        return NULL;
    }

    return kmp;
}

static void ws_pae_auth_kmp_api_finished(kmp_api_t *kmp)
{
    supp_entry_t *supp_entry = kmp_api_data_get(kmp);
    if (!supp_entry) {
        // Should not be possible
        return;
    }

    // Delete KMP
    ws_pae_lib_kmp_list_delete(&supp_entry->kmp_list, kmp);
}

#endif /* HAVE_PAE_AUTH */
#endif /* HAVE_WS */

