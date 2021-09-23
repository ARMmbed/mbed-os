/*
 * Copyright (c) 2016-2020, Pelion and affiliates.
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
#include "ns_address.h"
#include "nsdynmemLIB.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6_constants.h"
#include "socket_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_socket_if.h"
#include "common_functions.h"

#ifdef HAVE_WS

#define TRACE_GROUP "kmsi"

#define SOCKET_IF_HEADER_SIZE        27
#define INSTANCE_SOCKETS_NUMBER      3

typedef struct {
    kmp_service_t *kmp_service;                       /**< KMP service */
    uint8_t instance_id;                              /**< Instance identifier */
    bool relay;                                       /**< Interface is relay interface */
    ns_address_t remote_addr;                         /**< Remote address */
    int8_t socket_id[INSTANCE_SOCKETS_NUMBER];        /**< Socket ID */
    unsigned socket_id_in_use : 4;                    /**< Socket ID is in use */
    ns_list_link_t link;                              /**< Link */
} kmp_socket_if_t;

static int8_t kmp_socket_if_send(kmp_service_t *service, uint8_t instance_id, kmp_type_e kmp_id, const kmp_addr_t *addr, void *pdu, uint16_t size, uint8_t tx_identifier, uint8_t connection_num, uint8_t flags);
static void kmp_socket_if_socket_cb(void *ptr);

static NS_LIST_DEFINE(kmp_socket_if_list, kmp_socket_if_t, link);
static uint8_t kmp_socket_if_instance_id = 1;

int8_t kmp_socket_if_register(kmp_service_t *service, uint8_t *instance_id, bool relay, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port)
{
    if (!service || !remote_addr) {
        return -1;
    }

    kmp_socket_if_t *socket_if = NULL;
    bool new_socket_if_allocated = false;

    ns_list_foreach(kmp_socket_if_t, entry, &kmp_socket_if_list) {
        if (entry->kmp_service == service && entry->instance_id == *instance_id) {
            socket_if = entry;
        }
    }

    if (!socket_if) {
        socket_if = ns_dyn_mem_alloc(sizeof(kmp_socket_if_t));
        if (!socket_if) {
            return -1;
        }
        memset(socket_if, 0, sizeof(kmp_socket_if_t));
        for (uint8_t socket_num = 0; socket_num < INSTANCE_SOCKETS_NUMBER; socket_num++) {
            socket_if->socket_id[socket_num] = -1;
        }
        socket_if->socket_id_in_use = 1;
        new_socket_if_allocated = true;
    }

    socket_if->kmp_service = service;

    if (*instance_id == 0) {
        socket_if->instance_id = kmp_socket_if_instance_id++;
        if (socket_if->instance_id == 0) {
            socket_if->instance_id = kmp_socket_if_instance_id++;
        }
        *instance_id = socket_if->instance_id;
    }

    socket_if->relay = relay;

    socket_if->remote_addr.type = ADDRESS_IPV6;

    bool address_changed = false;
    if (memcmp(&socket_if->remote_addr.address, remote_addr, 16) != 0 ||
            socket_if->remote_addr.identifier != remote_port) {
        address_changed = true;
    }
    memcpy(&socket_if->remote_addr.address, remote_addr, 16);
    socket_if->remote_addr.identifier = remote_port;

    for (uint8_t socket_num = 0; socket_num < INSTANCE_SOCKETS_NUMBER; socket_num++) {
        if (socket_if->socket_id_in_use & (1 << socket_num)) {

            if ((socket_if->socket_id[socket_num] < 1) || address_changed) {
                if (socket_if->socket_id[socket_num] >= 0) {
                    socket_close(socket_if->socket_id[socket_num]);
                }
                socket_if->socket_id[socket_num] = socket_open(IPV6_NH_UDP, local_port, &kmp_socket_if_socket_cb);
                if (socket_if->socket_id[socket_num] < 0) {
                    ns_dyn_mem_free(socket_if);
                    return -1;
                }
            }
        }
    }

    uint8_t header_size = 0;
    if (relay) {
        header_size = SOCKET_IF_HEADER_SIZE;
    }

    if (kmp_service_msg_if_register(service, *instance_id, kmp_socket_if_send, header_size, INSTANCE_SOCKETS_NUMBER) < 0) {
        for (uint8_t socket_num = 0; socket_num < INSTANCE_SOCKETS_NUMBER; socket_num++) {
            if (socket_if->socket_id[socket_num] >= 0) {
                socket_close(socket_if->socket_id[socket_num]);
            }
        }
        ns_dyn_mem_free(socket_if);
        return -1;
    }

    if (new_socket_if_allocated) {
        ns_list_add_to_end(&kmp_socket_if_list, socket_if);
    }

    return 0;
}

int8_t kmp_socket_if_unregister(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    ns_list_foreach_safe(kmp_socket_if_t, entry, &kmp_socket_if_list) {
        if (entry->kmp_service == service) {
            ns_list_remove(&kmp_socket_if_list, entry);
            for (uint8_t socket_num = 0; socket_num < INSTANCE_SOCKETS_NUMBER; socket_num++) {
                if (entry->socket_id[socket_num] >= 0) {
                    socket_close(entry->socket_id[socket_num]);
                }
            }
            kmp_service_msg_if_register(service, entry->instance_id, NULL, 0, 0);
            ns_dyn_mem_free(entry);
        }
    }
    return 0;
}

static int8_t kmp_socket_if_send(kmp_service_t *service, uint8_t instance_id, kmp_type_e kmp_id, const kmp_addr_t *addr, void *pdu, uint16_t size, uint8_t tx_identifier, uint8_t connection_num, uint8_t flags)
{
    (void) tx_identifier;

    if (!service || !pdu || !addr) {
        return -1;
    }

    if (connection_num >= INSTANCE_SOCKETS_NUMBER) {
        return -1;
    }

    kmp_socket_if_t *socket_if = NULL;

    ns_list_foreach(kmp_socket_if_t, entry, &kmp_socket_if_list) {
        if (entry->kmp_service == service && entry->instance_id == instance_id) {
            socket_if = entry;
            break;
        }
    }

    if (!socket_if) {
        return -1;
    }

    if (socket_if->relay) {
        //Build UPD Relay
        uint8_t *ptr = pdu;
        memcpy(ptr, addr->relay_address, 16);
        ptr += 16;
        ptr = common_write_16_bit(addr->port, ptr);
        memcpy(ptr, kmp_address_eui_64_get(addr), 8);
        ptr += 8;
        *ptr = kmp_id;
    }

    int8_t socket_id = -1;
    if ((socket_if->socket_id_in_use & (1 << connection_num)) && socket_if->socket_id[connection_num] >= 0) {
        socket_id = socket_if->socket_id[connection_num];
    } else {
        if (socket_if->socket_id[connection_num] < 0) {
            socket_if->socket_id[connection_num] = socket_open(IPV6_NH_UDP, 0, &kmp_socket_if_socket_cb);
        }
        if (socket_if->socket_id[connection_num] < 0) {
            return -1;
        }
        socket_if->socket_id_in_use |= (1 << connection_num);
    }

    socket_sendto(socket_id, &socket_if->remote_addr, pdu, size);

    // Deallocate unless flags deny it
    if (flags & MSG_IF_SEND_FLAG_NO_DEALLOC) {
        return 0;
    }

    ns_dyn_mem_free(pdu);

    return 0;
}

static void kmp_socket_if_socket_cb(void *ptr)
{
    socket_callback_t *cb_data = ptr;

    if (cb_data->event_type != SOCKET_DATA) {
        return;
    }

    kmp_socket_if_t *socket_if = NULL;
    uint8_t connection_num = 0;

    ns_list_foreach(kmp_socket_if_t, entry, &kmp_socket_if_list) {
        for (uint8_t socket_num = 0; socket_num < INSTANCE_SOCKETS_NUMBER; socket_num++) {
            if (entry->socket_id[socket_num] == cb_data->socket_id) {
                socket_if = entry;
                connection_num = socket_num;
                break;
            }
        }
    }

    if (!socket_if) {
        return;
    }

    uint8_t *pdu = ns_dyn_mem_temporary_alloc(cb_data->d_len);

    if (socket_recvfrom(cb_data->socket_id, pdu, cb_data->d_len, 0, 0) != cb_data->d_len) {
        ns_dyn_mem_free(pdu);
        return;
    }

    kmp_addr_t addr;
    memset(&addr, 0, sizeof(kmp_addr_t));
    kmp_type_e type = KMP_TYPE_NONE;
    uint8_t *data_ptr = pdu;

    if (socket_if->relay) {
        addr.type = KMP_ADDR_EUI_64_AND_IP;
        memcpy(addr.relay_address, data_ptr, 16);
        data_ptr += 16;
        addr.port = common_read_16_bit(data_ptr);
        data_ptr += 2;
        memcpy(addr.eui_64, data_ptr, 8);
        data_ptr += 8;

        type = kmp_api_type_from_id_get(*data_ptr++);
        if (type == KMP_TYPE_NONE) {
            ns_dyn_mem_free(pdu);
            return;
        }
        cb_data->d_len -= SOCKET_IF_HEADER_SIZE;
    }

    kmp_service_msg_if_receive(socket_if->kmp_service, socket_if->instance_id, type, &addr, data_ptr, cb_data->d_len, connection_num);

    ns_dyn_mem_free(pdu);
}

#endif /* HAVE_WS */

