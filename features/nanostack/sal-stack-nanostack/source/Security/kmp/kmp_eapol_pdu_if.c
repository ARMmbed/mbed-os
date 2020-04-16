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
#include "6LoWPAN/ws/ws_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_eapol_pdu_if.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"

#ifdef HAVE_WS

#define TRACE_GROUP "kmep"

#define EAPOL_PDU_IF_HEADER_SIZE                 1

typedef struct {
    uint8_t kmp_id;                                   /**< Kmp id */
    uint8_t kmp_data;                                 /**< Kmp data e.g. eapol frame */
} eapol_kmp_pdu_t;

typedef struct {
    kmp_service_t *kmp_service;                       /**< KMP service */
    protocol_interface_info_entry_t *interface_ptr;   /**< Interface pointer */
    ns_list_link_t link;                              /**< Link */
} kmp_eapol_pdu_if_t;

static NS_LIST_DEFINE(kmp_eapol_pdu_if_list, kmp_eapol_pdu_if_t, link);

static int8_t kmp_eapol_pdu_if_send(kmp_service_t *service, kmp_type_e kmp_id, const kmp_addr_t *addr, void *pdu, uint16_t size, uint8_t tx_identifier);
static int8_t kmp_eapol_pdu_if_tx_status(protocol_interface_info_entry_t *interface_ptr, eapol_pdu_tx_status_e tx_status, uint8_t tx_identifier);

int8_t kmp_eapol_pdu_if_register(kmp_service_t *service, protocol_interface_info_entry_t *interface_ptr)
{
    if (!service || !interface_ptr) {
        return -1;
    }

    ns_list_foreach(kmp_eapol_pdu_if_t, entry, &kmp_eapol_pdu_if_list) {
        if (entry->kmp_service == service || entry->interface_ptr == interface_ptr) {
            return -1;
        }
    }

    kmp_eapol_pdu_if_t *eapol_pdu_if = ns_dyn_mem_alloc(sizeof(kmp_eapol_pdu_if_t));
    if (!eapol_pdu_if) {
        return -1;
    }

    eapol_pdu_if->kmp_service = service;
    eapol_pdu_if->interface_ptr = interface_ptr;

    if (kmp_service_msg_if_register(service, kmp_eapol_pdu_if_send, EAPOL_PDU_IF_HEADER_SIZE) < 0) {
        ns_dyn_mem_free(eapol_pdu_if);
        return -1;
    }

    ns_list_add_to_end(&kmp_eapol_pdu_if_list, eapol_pdu_if);

    return 0;
}

int8_t kmp_eapol_pdu_if_unregister(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    ns_list_foreach_safe(kmp_eapol_pdu_if_t, entry, &kmp_eapol_pdu_if_list) {
        if (entry->kmp_service == service) {
            ns_list_remove(&kmp_eapol_pdu_if_list, entry);
            ns_dyn_mem_free(entry);
            kmp_service_msg_if_register(service, NULL, 0);
        }
    }
    return 0;
}

static int8_t kmp_eapol_pdu_if_send(kmp_service_t *service, kmp_type_e kmp_id, const kmp_addr_t *addr, void *pdu, uint16_t size, uint8_t tx_identifier)
{
    if (!service || !addr || !pdu) {
        return -1;
    }

    protocol_interface_info_entry_t *interface_ptr = NULL;

    ns_list_foreach(kmp_eapol_pdu_if_t, entry, &kmp_eapol_pdu_if_list) {
        if (entry->kmp_service == service) {
            interface_ptr = entry->interface_ptr;
            break;
        }
    }

    if (!interface_ptr) {
        return -1;
    }

    const uint8_t *eui_64 = kmp_address_eui_64_get(addr);
    if (!eui_64) {
        return -1;
    }

    uint8_t *ptr = pdu;
    *ptr = kmp_id;

    int8_t ret = ws_eapol_pdu_send_to_mpx(interface_ptr, eui_64, pdu, size, pdu, kmp_eapol_pdu_if_tx_status, tx_identifier);

    return ret;
}

int8_t kmp_eapol_pdu_if_receive(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64, void *pdu, uint16_t size)
{
    kmp_service_t *service = NULL;

    ns_list_foreach(kmp_eapol_pdu_if_t, entry, &kmp_eapol_pdu_if_list) {
        if (entry->interface_ptr == interface_ptr) {
            service = entry->kmp_service;
            break;
        }
    }

    if (!service) {
        return -1;
    }

    kmp_addr_t addr;
    kmp_address_init(KMP_ADDR_EUI_64, &addr, eui_64);

    eapol_kmp_pdu_t *eapol_kmp_pdu = pdu;
    uint16_t data_pdu_size = size - sizeof(uint8_t);
    void *data_pdu = &eapol_kmp_pdu->kmp_data;

    kmp_type_e type = kmp_api_type_from_id_get(eapol_kmp_pdu->kmp_id);
    if (type == KMP_TYPE_NONE) {
        return -1;
    }

    int8_t ret = kmp_service_msg_if_receive(service, type, &addr, data_pdu, data_pdu_size);

    return ret;
}

static int8_t kmp_eapol_pdu_if_tx_status(protocol_interface_info_entry_t *interface_ptr, eapol_pdu_tx_status_e tx_status, uint8_t tx_identifier)
{
    kmp_service_t *service = NULL;

    ns_list_foreach(kmp_eapol_pdu_if_t, entry, &kmp_eapol_pdu_if_list) {
        if (entry->interface_ptr == interface_ptr) {
            service = entry->kmp_service;
            break;
        }
    }

    if (!service) {
        return -1;
    }

    kmp_tx_status_e kmp_tx_status;
    if (tx_status == EAPOL_PDU_TX_OK) {
        kmp_tx_status = KMP_TX_OK;
    } else if (tx_status == EAPOL_PDU_TX_ERR_TX_NO_ACK) {
        kmp_tx_status = KMP_TX_ERR_TX_NO_ACK;
    } else {
        kmp_tx_status = KMP_TX_ERR_UNSPEC;
    }

    int8_t ret = kmp_service_tx_status_indication(service, kmp_tx_status, tx_identifier);

    return ret;
}


#endif /* HAVE_WS */

