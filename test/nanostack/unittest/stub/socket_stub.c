/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include <stdint.h>
#include <inttypes.h>
#include "socket.h"
#include "ns_error_types.h"
#include "sec_lib_definitions.h"
#include "socket_api.h"
#include "tcp.h"
#include "socket_stub.h"
#include "ip6string.h"
#include "common_functions.h"

#define STUB_BUFFER_SIZE 1024

socket_stub_def socket_stub;

inet_pcb_t inet_pcb[SOCKETS_MAX];
socket_t socket_instance[SOCKETS_MAX];

socket_stub_data_t socket_stub_data = {
    0,  // socket_id
    eOK // err code
};

const uint8_t ns_in6addr_any[16];
socket_list_t socket_list = NS_LIST_INIT(socket_list);

void socket_stub_setup(void)
{
    socket_stub_data.socket_read_buffer = malloc(sizeof (buffer_t) + STUB_BUFFER_SIZE);
    // Cleared by socket_stub_clear_sockets later
}

void socket_stub_teardown(void)
{
    free(socket_stub_data.socket_read_buffer);
}

socket_stub_data_t *socket_stub_get_test_data(void)
{
    socket_stub_clear_sockets();
    socket_stub_data.socket_id = 0;
    socket_stub_data.err = eOK;
    socket_stub_data.socket_ptr = &socket_instance[0];
    socket_stub_data.socket_ptr->inet_pcb = &inet_pcb[0];
    return &socket_stub_data;
}

void socket_connection_abandoned(socket_t *socket, int8_t interface_id, uint8_t reason)
{

}

socket_t *socket_reference(socket_t *socket_ptr)
{
    return socket_ptr;
}

socket_t *socket_dereference(socket_t *socket_ptr)
{
    return NULL;
}

void socket_inet_pcb_set_buffer_hop_limit(const inet_pcb_t *inet_pcb, buffer_t *buf, const int16_t *msg_hoplimit)
{
}

void socket_inet_pcb_list_print(route_print_fn_t *print_fn, char sep)
{
}

void socket_stub_clear_sockets()
{
    int i;
    for (i = 0; i < SOCKETS_MAX; i++) {
        memset(&socket_instance[i], 0, sizeof(socket_t));
    }
    memset(socket_stub_data.socket_read_buffer, 0, sizeof (buffer_t));
    socket_stub_data.socket_read_buffer->size = 1024;
}

int8_t socket_event_handler_id_get(void)
{
    return 0;
}

void socket_init(void)
{
}

void socket_release(socket_t *socket)
{

}

error_t socket_port_validate(uint16_t port, uint8_t protocol)
{
    return socket_stub_data.err;
}

error_t socket_create(socket_family_t family, socket_type_t type, uint8_t protocol, int8_t *sid, uint16_t port, void (*passed_fptr)(void *), bool buffer_type)
{
    if (eOK == socket_stub_data.err) {
        *sid = socket_stub_data.socket_id;
        socket_instance[socket_stub_data.socket_id].type = type;
        socket_instance[socket_stub_data.socket_id].family = family;
        socket_instance[socket_stub_data.socket_id].inet_pcb = &inet_pcb[socket_stub_data.socket_id];
        socket_stub_data.socket_ptr = &socket_instance[socket_stub_data.socket_id];
    }
    return socket_stub_data.err;
}

socket_t *socket_lookup_ipv6(uint8_t protocol, const sockaddr_t *local_addr, const sockaddr_t *remote_addr, bool allow_wildcards)
{
    return 0;
}

socket_t *socket_lookup(socket_family_t family, uint8_t protocol, const sockaddr_t *local_addr, const sockaddr_t *remote_addr)
{
    return 0;
}

error_t socket_up(buffer_t *buf)
{
    return eOK;
}

void socket_event_push(uint8_t sock_event, socket_t *socket, int8_t interface_id, void *session_ptr, uint16_t length)
{

}

int16_t socket_buffer_sendmsg(int8_t sid, buffer_t *buf, const struct ns_msghdr *msg, int flags)
{
    return 0;
}

inet_pcb_t *socket_inet_pcb_free(inet_pcb_t *inet_pcb)
{
    return socket_stub.inet_pcb;
}


inet_pcb_t *socket_inet_pcb_allocate(void)
{
    return socket_stub.inet_pcb;
}

bool socket_validate_listen_backlog(const socket_t *socket_ptr) {
    return true;
}

int8_t socket_inet_pcb_join_group(inet_pcb_t *inet_pcb, int8_t interface_id, const uint8_t group[static 16])
{
    return 0;
}

int8_t socket_inet_pcb_leave_group(inet_pcb_t *inet_pcb, int8_t interface_id, const uint8_t group[static 16])
{
    return 0;
}

struct protocol_interface_info_entry *socket_interface_determine(const socket_t *socket, buffer_t *buf) {
    return socket_stub.entry_ptr;
}

socket_t *socket_pointer_get(int8_t socket)
{
    return socket_stub.socket_structure;
}

void socket_tx_buffer_event_and_free(buffer_t *buf, uint8_t status)
{

}

buffer_t *socket_tx_buffer_event(buffer_t *buf, uint8_t status)
{
    return socket_stub.buffer_ptr;
}

uint16_t socket_generate_random_port(uint8_t protocol)
{
    return 99;
}

void socket_connection_complete(socket_t *socket, int8_t interface_id)
{

}

socket_t *socket_new_incoming_connection(socket_t *listen_socket)
{
    return socket_stub.socket_structure;
}

bool socket_data_queued_event_push(socket_t *socket)
{
    return true;
}

void socket_cant_recv_more(socket_t *socket, int8_t interface_id)
{

}

void socket_list_print(route_print_fn_t *print_fn, char sep)
{

}

void socket_id_detach(int8_t sid)
{

}

void socket_leave_pending_state(socket_t *socket, void (*fptr)(void *))
{

}

int8_t socket_id_assign_and_attach(socket_t *socket)
{
    return 0;
}

bool socket_message_validate_iov(const ns_msghdr_t *msg, uint16_t *length_out)
{
    return true;
}
