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
#include "6LoWPAN/ws/ws_eapol_auth_relay.h"
#include "common_functions.h"

#ifdef HAVE_WS
#ifdef HAVE_PAE_AUTH

#define TRACE_GROUP "wsar"

typedef struct {
    protocol_interface_info_entry_t *interface_ptr;         /**< Interface pointer */
    ns_address_t remote_addr;                               /**< Remote address and port */
    ns_address_t relay_addr;                                /**< Relay address */
    int8_t socket_id;                                       /**< Socket ID for relay */
    ns_list_link_t link;                                    /**< Link */
} eapol_auth_relay_t;

static eapol_auth_relay_t *ws_eapol_auth_relay_get(protocol_interface_info_entry_t *interface_ptr);
static void ws_eapol_auth_relay_socket_cb(void *cb);
static int8_t ws_eapol_auth_relay_send_to_kmp(eapol_auth_relay_t *eapol_auth_relay, const uint8_t *eui_64, const uint8_t *ip_addr, uint16_t port, const void *data, uint16_t data_len);

static NS_LIST_DEFINE(eapol_auth_relay_list, eapol_auth_relay_t, link);

int8_t ws_eapol_auth_relay_start(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port)
{
    if (!interface_ptr || !remote_addr) {
        return -1;
    }

    if (ws_eapol_auth_relay_get(interface_ptr)) {
        return 0;
    }

    eapol_auth_relay_t *eapol_auth_relay = ns_dyn_mem_alloc(sizeof(eapol_auth_relay_t));
    if (!eapol_auth_relay) {
        return -1;
    }

    eapol_auth_relay->interface_ptr = interface_ptr;

    eapol_auth_relay->remote_addr.type = ADDRESS_IPV6;
    memcpy(&eapol_auth_relay->relay_addr.address, remote_addr, 16);
    eapol_auth_relay->relay_addr.identifier = remote_port;

    eapol_auth_relay->socket_id = socket_open(IPV6_NH_UDP, local_port, &ws_eapol_auth_relay_socket_cb);
    if (eapol_auth_relay->socket_id < 0) {
        ns_dyn_mem_free(eapol_auth_relay);
        return -1;
    }

    ns_list_add_to_end(&eapol_auth_relay_list, eapol_auth_relay);

    return 0;
}

int8_t ws_eapol_auth_relay_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    eapol_auth_relay_t *eapol_auth_relay = ws_eapol_auth_relay_get(interface_ptr);
    if (!eapol_auth_relay) {
        return -1;
    }

    socket_close(eapol_auth_relay->socket_id);

    ns_list_remove(&eapol_auth_relay_list, eapol_auth_relay);
    ns_dyn_mem_free(eapol_auth_relay);

    return 0;
}

static eapol_auth_relay_t *ws_eapol_auth_relay_get(protocol_interface_info_entry_t *interface_ptr)
{
    ns_list_foreach(eapol_auth_relay_t, entry, &eapol_auth_relay_list) {
        if (entry->interface_ptr == interface_ptr) {
            return entry;
        }
    }

    return NULL;
}

static void ws_eapol_auth_relay_socket_cb(void *cb)
{
    socket_callback_t *cb_data = cb;

    if (cb_data->event_type != SOCKET_DATA) {
        return;
    }

    eapol_auth_relay_t *eapol_auth_relay = NULL;

    ns_list_foreach(eapol_auth_relay_t, entry, &eapol_auth_relay_list) {
        if (entry->socket_id == cb_data->socket_id) {
            eapol_auth_relay = entry;
            break;
        }
    }

    if (!eapol_auth_relay) {
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

    // Message from source port 10254 (KMP service) -> to IP relay on node or on authenticator
    if (src_addr.identifier == eapol_auth_relay->relay_addr.identifier) {
        uint8_t *ptr = socket_pdu;
        uint8_t *eui_64;
        ns_address_t relay_ip_addr;
        relay_ip_addr.type = ADDRESS_IPV6;
        memcpy(relay_ip_addr.address, ptr, 16);
        ptr += 16;
        relay_ip_addr.identifier = common_read_16_bit(ptr);
        ptr += 2;
        eui_64 = ptr;
        ptr += 8;
        uint16_t data_len = cb_data->d_len - 26;
        ws_eapol_relay_lib_send_to_relay(eapol_auth_relay->socket_id, eui_64, &relay_ip_addr,
                                         ptr, data_len);
        ns_dyn_mem_free(socket_pdu);
        // Other source port (either 10253 or node relay source port) -> to KMP service
    } else {
        uint8_t *ptr = socket_pdu;
        ws_eapol_auth_relay_send_to_kmp(eapol_auth_relay, ptr, src_addr.address, src_addr.identifier,
                                        ptr + 8, cb_data->d_len - 8);
        ns_dyn_mem_free(socket_pdu);
    }
}

static int8_t ws_eapol_auth_relay_send_to_kmp(eapol_auth_relay_t *eapol_auth_relay, const uint8_t *eui_64, const uint8_t *ip_addr, uint16_t port, const void *data, uint16_t data_len)
{
    ns_address_t dest_addr = eapol_auth_relay->relay_addr;

    uint8_t temp_array[26];
    ns_iovec_t msg_iov[2];
    ns_msghdr_t msghdr;
    //Set messages name buffer
    msghdr.msg_name = &dest_addr;
    msghdr.msg_namelen = sizeof(dest_addr);
    msghdr.msg_iov = &msg_iov[0];
    msghdr.msg_iovlen = 2;
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;
    uint8_t *ptr = temp_array;
    memcpy(ptr, ip_addr, 16);
    ptr += 16;
    ptr = common_write_16_bit(port, ptr);
    memcpy(ptr, eui_64, 8);
    msg_iov[0].iov_base = temp_array;
    msg_iov[0].iov_len = 26;
    msg_iov[1].iov_base = (void *)data;
    msg_iov[1].iov_len = data_len;
    socket_sendmsg(eapol_auth_relay->socket_id, &msghdr, NS_MSG_LEGACY0);
    return 0;
}

#endif /* HAVE_PAE_AUTH */
#endif /* HAVE_WS */
