/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6_constants.h"
#include "socket_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_socket_if.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "kmap"

struct kmp_api_s {
    void                         *app_data_ptr;           /**< Opaque pointer for application data */
    kmp_api_create_confirm       *create_conf;            /**< KMP-CREATE.confirm callback */
    kmp_api_create_indication    *create_ind;             /**< KMP-CREATE.indication callback */
    kmp_api_finished_indication  *finished_ind;           /**< KMP-FINISHED.indication callback */
    kmp_api_finished             *finished;               /**< Finished i.e. ready to be deleted callback */
    kmp_type_e                   type;                    /**< KMP type */
    kmp_addr_t                   *addr;                   /**< Supplicant EUI-64, Relay IP address, Relay port */
    kmp_service_t                *service;                /**< KMP service */
    bool                         timer_start_pending : 1; /**< Timer is pending to start */
    bool                         receive_disable : 1;     /**< Receiving disabled, do not route messages anymore */
    sec_prot_t                   sec_prot;                /**< Security protocol interface */
};

typedef struct {
    kmp_type_e                   type;                    /**< Security protocol type callback */
    kmp_sec_prot_size            *size;                   /**< Security protocol data size callback */
    kmp_sec_prot_init            *init;                   /**< Security protocol init */
    ns_list_link_t               link;                    /**< Link */
} kmp_sec_prot_entry_t;

typedef NS_LIST_HEAD(kmp_sec_prot_entry_t, link) kmp_sec_prot_list_t;

struct kmp_service_s {
    kmp_sec_prot_list_t              sec_prot_list;             /**< Security protocols list */
    kmp_service_incoming_ind         *incoming_ind;             /**< Callback to application to indicate incoming KMP frame */
    kmp_service_addr_get             *addr_get;                 /**< Callback to get addresses related to KMP */
    kmp_service_api_get              *api_get;                  /**< Callback to get KMP API from a service */
    kmp_service_msg_if_send          *send;                     /**< Callback to send KMP frames */
    kmp_service_timer_if_start       *timer_start;              /**< Callback to start timer */
    kmp_service_timer_if_stop        *timer_stop;               /**< Callback to stop timer */
    kmp_service_event_if_event_send  *event_send;               /**< Callback to send event */
    uint8_t                          header_size;               /**< Header size */
    ns_list_link_t                   link;                      /**< Link */
};

typedef struct {
    uint8_t kmp_id;               /**< Kmp id */
    uint8_t kmp_data;             /**< Kmp data e.g. eapol frame */
} kmp_pdu_t;

static NS_LIST_DEFINE(kmp_service_list, kmp_service_t, link);

static void kmp_api_sec_prot_create_confirm(sec_prot_t *prot, sec_prot_result_e result);
static void kmp_api_sec_prot_create_indication(sec_prot_t *prot);
static void kmp_api_sec_prot_finished_indication(sec_prot_t *prot, sec_prot_result_e result, sec_prot_keys_t *sec_keys);
static void kmp_api_sec_prot_finished(sec_prot_t *prot);
static int8_t kmp_sec_prot_send(sec_prot_t *prot, void *pdu, uint16_t size);
static void kmp_sec_prot_timer_start(sec_prot_t *prot);
static void kmp_sec_prot_timer_stop(sec_prot_t *prot);
static void kmp_sec_prot_state_machine_call(sec_prot_t *prot);
static void kmp_sec_prot_eui64_addr_get(sec_prot_t *prot, uint8_t *local_eui64,  uint8_t *remote_eui64);
static sec_prot_t *kmp_sec_prot_by_type_get(sec_prot_t *prot, uint8_t type);
static void kmp_sec_prot_receive_disable(sec_prot_t *prot);

#define kmp_api_get_from_prot(prot) (kmp_api_t *)(((uint8_t *)prot) - offsetof(kmp_api_t, sec_prot));

kmp_api_t *kmp_api_create(kmp_service_t *service, kmp_type_e type)
{
    if (!service) {
        return 0;
    }

    kmp_sec_prot_entry_t *sec_prot = 0;
    ns_list_foreach(kmp_sec_prot_entry_t, list_entry, &service->sec_prot_list) {
        if (list_entry->type == type) {
            sec_prot = list_entry;
            break;
        }
    }

    if (!sec_prot) {
        // Unknown security protocol
        return 0;
    }
    // Size for security protocol internal data
    uint16_t sec_size = sec_prot->size();

    kmp_api_t *kmp = ns_dyn_mem_temporary_alloc(sizeof(kmp_api_t) + sec_size);
    if (!kmp) {
        return 0;
    }

    kmp->type = type;
    kmp->app_data_ptr = 0;
    kmp->create_conf = 0;
    kmp->create_ind = 0;
    kmp->finished_ind = 0;
    kmp->finished = 0;
    kmp->addr = 0;
    kmp->service = service;
    kmp->timer_start_pending = false;
    kmp->receive_disable = false;

    memset(&kmp->sec_prot, 0, sec_size);

    kmp->sec_prot.header_size = service->header_size;
    kmp->sec_prot.create_conf = kmp_api_sec_prot_create_confirm;
    kmp->sec_prot.create_ind = kmp_api_sec_prot_create_indication;
    kmp->sec_prot.finished_ind = kmp_api_sec_prot_finished_indication;
    kmp->sec_prot.finished = kmp_api_sec_prot_finished;
    kmp->sec_prot.send = kmp_sec_prot_send;
    kmp->sec_prot.timer_start = kmp_sec_prot_timer_start;
    kmp->sec_prot.timer_stop = kmp_sec_prot_timer_stop;
    kmp->sec_prot.state_machine_call = kmp_sec_prot_state_machine_call;
    kmp->sec_prot.addr_get = kmp_sec_prot_eui64_addr_get;
    kmp->sec_prot.type_get = kmp_sec_prot_by_type_get;
    kmp->sec_prot.receive_disable = kmp_sec_prot_receive_disable;

    if (sec_prot->init(&kmp->sec_prot) < 0) {
        ns_dyn_mem_free(kmp);
        return 0;
    }

    return (kmp_api_t *) kmp;
}

int8_t kmp_api_start(kmp_api_t *kmp)
{
    if (kmp->timer_start_pending) {
        if (kmp->service->timer_start(kmp->service, kmp) < 0) {
            return -1;
        }
    }
    return 0;
}

void kmp_api_create_request(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr, kmp_sec_keys_t *sec_keys)
{
    kmp->type = type;
    kmp->addr = addr;
    kmp->sec_prot.create_req(&kmp->sec_prot, sec_keys);
}

void kmp_api_create_response(kmp_api_t *kmp, kmp_result_e result)
{
    kmp->sec_prot.create_resp(&kmp->sec_prot, (sec_prot_result_e) result);
}

static void kmp_api_sec_prot_create_confirm(sec_prot_t *prot, sec_prot_result_e result)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->create_conf((kmp_api_t *)kmp, (kmp_result_e) result);
}

static void kmp_api_sec_prot_create_indication(sec_prot_t *prot)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->create_ind((kmp_api_t *)kmp, kmp->type, kmp->addr);
}

static void kmp_api_sec_prot_finished_indication(sec_prot_t *prot, sec_prot_result_e result, sec_prot_keys_t *sec_keys)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->finished_ind((kmp_api_t *)kmp, (kmp_result_e) result, sec_keys);
}

static void kmp_api_sec_prot_finished(sec_prot_t *prot)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->finished((kmp_api_t *)kmp);
}

static int8_t kmp_sec_prot_send(sec_prot_t *prot, void *pdu, uint16_t size)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);

    // Convert from internal initial key type to real type if needed
    kmp_type_e kmp_id = kmp->type;
    if (kmp_id > IEEE_802_1X_INITIAL_KEY) {
        kmp_id -= IEEE_802_1X_INITIAL_KEY;
    }

    int8_t result = -1;

    if (kmp->service->send) {
        result = kmp->service->send(kmp->service, kmp_id, kmp->addr, pdu, size);
    }

    if (result < 0) {
        ns_dyn_mem_free(pdu);
    }

    return result;
}

static void kmp_sec_prot_timer_start(sec_prot_t *prot)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->timer_start_pending = false;
    if (kmp->service->timer_start(kmp->service, kmp) < 0) {
        kmp->timer_start_pending = true;
    }
}

static void kmp_sec_prot_timer_stop(sec_prot_t *prot)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->service->timer_stop(kmp->service, kmp);
    kmp->timer_start_pending = false;
}

static void kmp_sec_prot_eui64_addr_get(sec_prot_t *prot, uint8_t *local_eui64, uint8_t *remote_eui64)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);

    kmp_addr_t local_addr;
    kmp_address_init(KMP_ADDR_EUI_64, &local_addr, NULL);

    kmp_addr_t remote_addr;
    kmp_address_init(KMP_ADDR_EUI_64, &remote_addr, NULL);

    kmp->service->addr_get(kmp->service, kmp, &local_addr, &remote_addr);

    if (local_eui64) {
        memcpy(local_eui64, local_addr.eui_64, 8);
    }
    if (remote_eui64) {
        memcpy(remote_eui64, remote_addr.eui_64, 8);
    }
}

static sec_prot_t *kmp_sec_prot_by_type_get(sec_prot_t *prot, uint8_t type)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);

    kmp_type_e kmp_type;

    switch (type) {
        case SEC_PROT_TYPE_EAP_TLS:
            kmp_type = IEEE_802_1X_MKA;
            break;
        case SEC_PROT_TYPE_TLS:
            kmp_type = TLS_PROT;
            break;
        default:
            return NULL;
    }

    kmp_api_t *kmp_by_type = kmp->service->api_get(kmp->service, kmp, kmp_type);
    if (!kmp_by_type) {
        return NULL;
    }

    return &kmp_by_type->sec_prot;
}

static void kmp_sec_prot_receive_disable(sec_prot_t *prot)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->receive_disable = true;
}

void kmp_api_delete(kmp_api_t *kmp)
{
    if (kmp->sec_prot.delete) {
        kmp->sec_prot.delete(&kmp->sec_prot);
    }
    ns_dyn_mem_free(kmp);
}

void kmp_api_cb_register(kmp_api_t *kmp, kmp_api_create_confirm *create_conf, kmp_api_create_indication *create_ind, kmp_api_finished_indication *finished_ind, kmp_api_finished *finished)
{
    if (!kmp) {
        return;
    }

    kmp->create_conf = create_conf;
    kmp->create_ind = create_ind;
    kmp->finished_ind = finished_ind;
    kmp->finished = finished;
}

kmp_type_e kmp_api_type_get(kmp_api_t *kmp)
{
    return kmp->type;
}

kmp_type_e kmp_api_type_from_id_get(uint8_t kmp_id)
{
    switch (kmp_id) {
        case IEEE_802_1X_MKA:
            return IEEE_802_1X_MKA;
        case IEEE_802_11_4WH:
            return IEEE_802_11_4WH;
        case IEEE_802_11_GKH:
            return IEEE_802_11_GKH;
        default:
            return KMP_TYPE_NONE;
    }
}

kmp_service_t *kmp_api_service_get(kmp_api_t *kmp)
{
    return kmp->service;
}

void kmp_api_data_set(kmp_api_t *kmp, void *data)
{
    kmp->app_data_ptr = data;
}

void *kmp_api_data_get(kmp_api_t *kmp)
{
    return kmp->app_data_ptr;
}

void kmp_api_addr_set(kmp_api_t *kmp, kmp_addr_t *addr)
{
    kmp->addr = addr;
}

void kmp_api_sec_keys_set(kmp_api_t *kmp, kmp_sec_keys_t *sec_keys)
{
    kmp->sec_prot.sec_keys = sec_keys;
}

kmp_service_t *kmp_service_create(void)
{
    kmp_service_t *service = ns_dyn_mem_alloc(sizeof(kmp_service_t));
    if (!service) {
        return NULL;
    }

    ns_list_init(&service->sec_prot_list);
    service->incoming_ind = 0;
    service->addr_get = 0;
    service->api_get = 0;
    service->send = 0;
    service->header_size = 0;

    ns_list_add_to_start(&kmp_service_list, service);

    return service;
}

int8_t kmp_service_delete(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    ns_list_foreach_safe(kmp_service_t, list_entry, &kmp_service_list) {
        if (list_entry == service) {
            ns_list_foreach_safe(kmp_sec_prot_entry_t, sec_list_entry, &list_entry->sec_prot_list) {
                ns_list_remove(&list_entry->sec_prot_list, sec_list_entry);
                ns_dyn_mem_free(sec_list_entry);
            }

            ns_list_remove(&kmp_service_list, list_entry);
            ns_dyn_mem_free(list_entry);
            return 0;
        }
    }

    return -1;
}

static void kmp_sec_prot_state_machine_call(sec_prot_t *prot)
{
    kmp_api_t *kmp = kmp_api_get_from_prot(prot);
    kmp->service->event_send(kmp->service, prot);
}

int8_t kmp_service_cb_register(kmp_service_t *service, kmp_service_incoming_ind *incoming_ind, kmp_service_addr_get *addr_get, kmp_service_api_get *api_get)
{
    if (!service) {
        return -1;
    }

    service->incoming_ind = incoming_ind;
    service->addr_get = addr_get;
    service->api_get = api_get;

    return 0;
}

int8_t kmp_service_msg_if_register(kmp_service_t *service, kmp_service_msg_if_send *send, uint8_t header_size)
{
    if (!service) {
        return -1;
    }

    service->send = send;
    service->header_size = header_size;

    return 0;
}

int8_t kmp_service_msg_if_receive(kmp_service_t *service, kmp_type_e type, const kmp_addr_t *addr, void *pdu, uint16_t size)
{
    if (!service) {
        return -1;
    }

    kmp_api_t *kmp = (kmp_api_t *) service->incoming_ind(service, type, addr);
    if (!kmp) {
        return -1;
    }

    // Security protocol has disables message receiving
    if (kmp->receive_disable) {
        return -1;
    }

    int8_t ret = kmp->sec_prot.receive(&kmp->sec_prot, pdu, size);
    return ret;
}

int8_t kmp_service_sec_protocol_register(kmp_service_t *service, kmp_type_e type, kmp_sec_prot_size *size, kmp_sec_prot_init *init)
{
    if (!service) {
        return -1;
    }

    ns_list_foreach(kmp_sec_prot_entry_t, list_entry, &service->sec_prot_list) {
        // Already registered
        if (list_entry->type == type) {
            return -1;
        }
    }

    kmp_sec_prot_entry_t *sec_prot = ns_dyn_mem_temporary_alloc(sizeof(kmp_sec_prot_entry_t));
    if (!sec_prot) {
        return -1;
    }

    sec_prot->type = type;
    sec_prot->size = size;
    sec_prot->init = init;

    ns_list_add_to_start(&service->sec_prot_list, sec_prot);

    return 0;
}

int8_t kmp_service_sec_protocol_unregister(kmp_service_t *service, kmp_type_e type)
{
    ns_list_foreach(kmp_sec_prot_entry_t, list_entry, &service->sec_prot_list) {
        if (list_entry->type == type) {
            ns_list_remove(&service->sec_prot_list, list_entry);
            ns_dyn_mem_free(list_entry);
            return 0;
        }
    }
    return -1;
}

void kmp_service_timer_if_timeout(kmp_api_t *kmp, uint16_t ticks)
{
    kmp->sec_prot.timer_timeout(&kmp->sec_prot, ticks);
}

int8_t kmp_service_timer_if_register(kmp_service_t *service, kmp_service_timer_if_start start, kmp_service_timer_if_stop stop)
{
    if (!service) {
        return -1;
    }

    service->timer_start = start;
    service->timer_stop = stop;
    return 0;
}

void kmp_service_event_if_event(kmp_service_t *service, void *data)
{
    (void) service;

    // For now, only state machine events
    sec_prot_t *prot = data;
    prot->state_machine(prot);
}

int8_t kmp_service_event_if_register(kmp_service_t *service, kmp_service_event_if_event_send send)
{
    if (!service) {
        return -1;
    }

    service->event_send = send;
    return 0;
}

#endif /* HAVE_WS */

