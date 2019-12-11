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
#include "NWK_INTERFACE/Include/protocol.h"
#include "mac_api.h"
#include "mac_mcps.h"
#include "Common_Protocols/ipv6_constants.h"
#include "socket_api.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"
#include "6LoWPAN/ws/ws_eapol_relay_lib.h"
#include "6LoWPAN/ws/ws_eapol_relay.h"

#ifdef HAVE_WS
#ifdef HAVE_EAPOL_RELAY

#define TRACE_GROUP "wser"

typedef struct {
    protocol_interface_info_entry_t *interface_ptr;         /**< Interface pointer */
    ns_address_t remote_addr;                               /**< Remote address (border router address) */
    int8_t socket_id;                                       /**< Socket ID for relay */
    ns_list_link_t link;                                    /**< Link */
} eapol_relay_t;

static eapol_relay_t *ws_eapol_relay_get(protocol_interface_info_entry_t *interface_ptr);
static int8_t ws_eapol_relay_eapol_pdu_address_check(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);
static int8_t ws_eapol_relay_eapol_pdu_receive(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64, void *pdu, uint16_t size);
static void ws_eapol_relay_socket_cb(void *cb);

static const eapol_pdu_recv_cb_data_t eapol_pdu_recv_cb_data = {
    .priority = EAPOL_PDU_RECV_LOW_PRIORITY,
    .addr_check = ws_eapol_relay_eapol_pdu_address_check,
    .receive = ws_eapol_relay_eapol_pdu_receive
};

static NS_LIST_DEFINE(eapol_relay_list, eapol_relay_t, link);

int8_t ws_eapol_relay_start(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port)
{
    if (!interface_ptr || !remote_addr) {
        return -1;
    }

    if (ws_eapol_relay_get(interface_ptr)) {
        return 0;
    }

    eapol_relay_t *eapol_relay = ns_dyn_mem_alloc(sizeof(eapol_relay_t));
    if (!eapol_relay) {
        return -1;
    }

    eapol_relay->interface_ptr = interface_ptr;

    eapol_relay->remote_addr.type = ADDRESS_IPV6;
    memcpy(&eapol_relay->remote_addr.address, remote_addr, 16);
    eapol_relay->remote_addr.identifier = remote_port;

    eapol_relay->socket_id = socket_open(IPV6_NH_UDP, local_port, &ws_eapol_relay_socket_cb);
    if (eapol_relay->socket_id < 0) {
        ns_dyn_mem_free(eapol_relay);
        return -1;
    }

    if (ws_eapol_pdu_cb_register(interface_ptr, &eapol_pdu_recv_cb_data) < 0) {
        ns_dyn_mem_free(eapol_relay);
        return -1;
    }

    ns_list_add_to_end(&eapol_relay_list, eapol_relay);

    return 0;
}

int8_t ws_eapol_relay_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    eapol_relay_t *eapol_relay = ws_eapol_relay_get(interface_ptr);
    if (!eapol_relay) {
        return -1;
    }

    socket_close(eapol_relay->socket_id);

    ws_eapol_pdu_cb_unregister(interface_ptr, &eapol_pdu_recv_cb_data);

    ns_list_remove(&eapol_relay_list, eapol_relay);
    ns_dyn_mem_free(eapol_relay);

    return 0;
}

static eapol_relay_t *ws_eapol_relay_get(protocol_interface_info_entry_t *interface_ptr)
{
    ns_list_foreach(eapol_relay_t, entry, &eapol_relay_list) {
        if (entry->interface_ptr == interface_ptr) {
            return entry;
        }
    }

    return NULL;
}

static int8_t ws_eapol_relay_eapol_pdu_address_check(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64)
{
    (void) eui_64;
    (void) interface_ptr;

    // Low priority, always route all here if asked
    return 0;
}

static int8_t ws_eapol_relay_eapol_pdu_receive(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64, void *pdu, uint16_t size)
{
    eapol_relay_t *eapol_relay = ws_eapol_relay_get(interface_ptr);
    if (!eapol_relay) {
        return -1;
    }

    ws_eapol_relay_lib_send_to_relay(eapol_relay->socket_id, eui_64, &eapol_relay->remote_addr, pdu, size);

    return 0;
}

static void ws_eapol_relay_socket_cb(void *cb)
{
    socket_callback_t *cb_data = cb;

    if (cb_data->event_type != SOCKET_DATA) {
        return;
    }

    eapol_relay_t *eapol_relay = NULL;

    ns_list_foreach(eapol_relay_t, entry, &eapol_relay_list) {
        if (entry->socket_id == cb_data->socket_id) {
            eapol_relay = entry;
            break;
        }
    }

    if (!eapol_relay) {
        return;
    }

    uint8_t *socket_pdu = ns_dyn_mem_temporary_alloc(cb_data->d_len);
    if (!socket_pdu) {
        return;
    }

    ns_address_t src_addr;

    if (socket_recvfrom(cb_data->socket_id, socket_pdu, cb_data->d_len, 0, &src_addr) != cb_data->d_len) {
        ns_dyn_mem_free(socket_pdu);
        return;
    }

    //First 8 byte is EUID64 and rsr payload
    if (ws_eapol_pdu_send_to_mpx(eapol_relay->interface_ptr, socket_pdu, socket_pdu + 8, cb_data->d_len - 8, socket_pdu, NULL, 0) < 0) {
        ns_dyn_mem_free(socket_pdu);
    }
}

#endif /* HAVE_EAPOL_RELAY */
#endif /* HAVE_WS */

