/*
 * Copyright (c) 2008-2015, 2017-2019, Arm Limited and affiliates.
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
/**
 * \file socket.c
 * \brief Socket API
 *
 * The socket API functions
 */
#include "nsconfig.h"
#include "ns_trace.h"
#include <stdio.h>
#include "string.h"
#include "ns_types.h"
#include "randLIB.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "nsdynmemLIB.h"
#include "Core/include/ns_socket.h"
#include "socket_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/tcp.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "ip6string.h"
#include "common_functions.h"

#define TRACE_GROUP "sck"

#define RANDOM_PORT_NUMBER_START 49152
#define RANDOM_PORT_NUMBER_END 65535
#define RANDOM_PORT_NUMBER_COUNT (RANDOM_PORT_NUMBER_END - RANDOM_PORT_NUMBER_START + 1)
#define RANDOM_PORT_NUMBER_MAX_STEP 500

static uint16_t port_counter;

static socket_t *socket_instance[SOCKETS_MAX];

typedef struct socket_cb_data_t {
    int8_t tasklet;
    int8_t net_interface_id;
    socket_t *socket;
    buffer_t *buf;
} socket_cb_data_t;


typedef struct socket_cb_event_t {
    socket_t *socket;
    uint8_t socket_event;
    int8_t interface_id;
    uint16_t length;
    void *session_ptr;
} socket_cb_event_t;

socket_list_t socket_list = NS_LIST_INIT(socket_list);

static int8_t socket_event_handler = -1;
static uint8_t last_allocated_socket = SOCKETS_MAX - 1;

/**
 * Generate random port number between RANDOM_PORT_NUMBER_START and RANDOM_PORT_NUMBER_END.
 *
 * \return random port number
 */
uint16_t socket_generate_random_port(uint8_t protocol)
{
    uint16_t count = RANDOM_PORT_NUMBER_COUNT;

    do {
        port_counter += randLIB_get_random_in_range(1, RANDOM_PORT_NUMBER_MAX_STEP);
        while (port_counter >= RANDOM_PORT_NUMBER_COUNT) {
            port_counter -= RANDOM_PORT_NUMBER_COUNT;
        }
        uint16_t port = RANDOM_PORT_NUMBER_START + port_counter;
        if (socket_port_validate(port, protocol) == eOK) {
            return port;
        }
    } while (--count > 0);

    return 0;
}

int8_t socket_event_handler_id_get(void)
{
    return socket_event_handler;
}

socket_t *socket_pointer_get(int8_t socket)
{
    if (socket < 0 || socket >= SOCKETS_MAX) {
        return NULL;
    }
    if (!socket_instance[socket] || socket_instance[socket]->family == SOCKET_FAMILY_NONE) {
        return NULL;
    }

    return socket_instance[socket];

}

static void socket_data_event_push(buffer_t *buf)
{
    arm_event_s event = {
        .receiver = socket_event_handler,
        .sender = 0,
        .event_type = ARM_SOCKET_DATA_CB,
        .data_ptr = buf,
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        buffer_free(buf);
    }
}

bool socket_data_queued_event_push(socket_t *socket)
{
    arm_event_s event = {
        .receiver = socket_event_handler,
        .sender = 0,
        .event_type = ARM_SOCKET_DATA_QUEUED_CB,
        .data_ptr = socket_reference(socket),
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        return false;
    }
    return true;
}

static void socket_cb_event_run(const socket_cb_event_t *event)
{
    if (event->socket->id != -1) {
        eventOS_scheduler_set_active_tasklet(event->socket->tasklet);

        if (event->socket->flags & SOCKET_BUFFER_CB) {
            static socket_buffer_callback_t socket_cb_event_buffer;
            socket_cb_event_buffer.session_ptr = event->session_ptr;
            socket_cb_event_buffer.event_type = event->socket_event;
            socket_cb_event_buffer.socket_id = event->socket->id;
            socket_cb_event_buffer.buf = 0;
            socket_cb_event_buffer.interface_id = event->interface_id;
            event->socket->u.live.fptr(&socket_cb_event_buffer);
            socket_cb_event_buffer.session_ptr = 0;
        } else {
            static socket_callback_t socket_cb_event_stucture;
            socket_cb_event_stucture.event_type = event->socket_event;
            socket_cb_event_stucture.socket_id = event->socket->id;
            socket_cb_event_stucture.d_len = event->length;
            socket_cb_event_stucture.interface_id = event->interface_id;
            event->socket->u.live.fptr(&socket_cb_event_stucture);
        }
    }
}

void socket_buffer_cb_run(socket_t *socket, buffer_t *buffer)
{
    if (socket->id == -1 || !socket->u.live.fptr) {
        buffer_free(buffer);
        return;
    }

    eventOS_scheduler_set_active_tasklet(socket->tasklet);

    static socket_buffer_callback_t socket_cb_buffer;
    socket_cb_buffer.event_type = SOCKET_DATA;
    socket_cb_buffer.socket_id = socket->id;
    socket_cb_buffer.interface_id = buffer->interface->id;
    socket_cb_buffer.buf = buffer;
    socket_cb_buffer.session_ptr = NULL;

    socket->u.live.fptr(&socket_cb_buffer);
}

void socket_cb_run(socket_t *socket)
{
    if (socket->id == -1 || !socket->u.live.fptr) {
        return;
    }

    buffer_t *buf = ns_list_get_first(&socket->rcvq.bufs);

    eventOS_scheduler_set_active_tasklet(socket->tasklet);

    static socket_callback_t socket_cb_structure;

    socket_cb_structure.event_type = SOCKET_DATA;
    socket_cb_structure.socket_id = socket->id;
    socket_cb_structure.interface_id = buf ? buf->interface->id : 0;
    socket_cb_structure.LINK_LQI = buf ? buf->options.lqi : 0;
    socket_cb_structure.d_len = socket->rcvq.data_bytes;

    socket->u.live.fptr(&socket_cb_structure);
}

void socket_tasklet_event_handler(arm_event_s *event)
{
    switch (event->event_type) {
        case ARM_SOCKET_INIT:
            tr_debug("Socket Tasklet Generated");
            break;
        case ARM_SOCKET_EVENT_CB: {
            socket_cb_event_t *cb_event_ptr = event->data_ptr;
            socket_cb_event_run(cb_event_ptr);
            socket_dereference(cb_event_ptr->socket);
            ns_dyn_mem_free(cb_event_ptr);
            break;
        }
        case ARM_SOCKET_DATA_CB: {
            buffer_t *buf = event->data_ptr;
            /* Reference the socket here*/
            socket_t *socket = socket_reference(buf->socket);
            if (socket->flags & SOCKET_BUFFER_CB) {
                // They just take ownership of the buffer. No read calls.
                socket_buffer_cb_run(socket, buf);
            } else {
                // Emulate old one-callback-per-buffer behavior. Make sure
                // the recv queue is empty, then put this buffer on it, then
                // make the callback. Their read calls then read from the
                // recv queue. After the callback, flush the receive queue again.
                sockbuf_flush(&socket->rcvq);
                sockbuf_append(&socket->rcvq, buf);
                socket_cb_run(socket);
                sockbuf_flush(&socket->rcvq);
            }
            /* Dereference the socket here*/
            socket_dereference(socket);
            break;
        }
        case ARM_SOCKET_DATA_QUEUED_CB: {
            socket_t *socket = event->data_ptr;
            socket_cb_run(socket);
            socket_dereference(socket);
            break;
        }
        case ARM_SOCKET_TCP_TIMER_CB:
            tcp_handle_time_event((uint16_t)event->event_data);
            break;

        default:
            break;
    }
}
/**
 * Initialize API
 */
void socket_init(void)
{
    if (socket_event_handler == -1) {
        socket_event_handler = eventOS_event_handler_create(&socket_tasklet_event_handler, ARM_SOCKET_INIT);
    }

    port_counter = randLIB_get_random_in_range(0, RANDOM_PORT_NUMBER_COUNT - 1);
}

/**
 * Release a socket (detaching from application)
 *
 * We are either releasing directly from an application close (ID being freed,
 * reference being removed from ID table), or indirectly - a pending socket
 * attached to a listening socket being closed.
 *
 * \param socket socket pointer
 */
void socket_release(socket_t *socket)
{
    socket->flags |= SOCKET_FLAG_CLOSED | SOCKET_FLAG_SHUT_WR | SOCKET_FLAG_CANT_RECV_MORE;

    if (socket_is_ipv6(socket)) {
        // Do TCP cleanup first, while we have one reference count of our own,
        // so it can't make socket vanish.
        // Take care never to put tcp_info in local variable - these calls can delete it
        if (tcp_info(socket->inet_pcb)) {
            /* This may trigger a reset if pending data. Do it first so you
             * get just the reset, rather than a FIN. */
            tcp_error sock_status = tcp_session_shutdown_read(tcp_info(socket->inet_pcb));
            /* This can also cause TCP deletion */
            if (tcp_info(socket->inet_pcb)) {
                sock_status = tcp_session_close(tcp_info(socket->inet_pcb));
            }
            if (tcp_info(socket->inet_pcb)) {
                tcp_socket_released(tcp_info(socket->inet_pcb));
            }
            // prevent warning "statement with no effect" when TCP is disabled
            (void) sock_status;
        } else {
            /* Unbind the internet control block - ensures users are not prevented
             * from binding a new socket to the same port if the socket lingers
             * on due to pending events/buffers. (And also means the new socket
             * gets any such packets, with this getting none).
             */
            memcpy(socket->inet_pcb->local_address, ns_in6addr_any, 16);
            memcpy(socket->inet_pcb->remote_address, ns_in6addr_any, 16);
            socket->inet_pcb->local_port = 0;
            socket->inet_pcb->remote_port = 0;
            socket->inet_pcb->protocol = 0;
        }
        sockbuf_flush(&socket->rcvq);
    }

    if (!(socket->flags & SOCKET_FLAG_PENDING)) {
        // Release any pending sockets in a listening socket's queue
        ns_list_foreach_safe(socket_t, pending, &socket->u.live.queue) {
            // Release of a pending socket will make it non-pending and
            // hence take it off our queue (see immediately below) making
            // this loop work. (And maybe it will free it too).
            socket_release(pending);
        }
    } else {
        // This may make socket vanish, as leaving pending decrements count
        socket_leave_pending_state(socket, NULL);
    }
}

static void socket_free(socket_t *socket)
{
    if (socket->refcount != 0) {
        tr_err("free refed");
    }
    if (socket->flags & SOCKET_FLAG_PENDING) {
        tr_err("free pending");
    }
    ns_list_remove(&socket_list, socket);
    sockbuf_flush(&socket->rcvq);
    sockbuf_flush(&socket->sndq);

    socket_inet_pcb_free(socket->inet_pcb);
    ns_dyn_mem_free(socket);
}

socket_error_t socket_port_validate(uint16_t port, uint8_t protocol)
{
    ns_list_foreach(socket_t, socket, &socket_list) {
        if (!socket_is_ipv6(socket)) {
            continue;
        }
        if (socket->inet_pcb->local_port == port && socket->inet_pcb->protocol == protocol) {
            return eFALSE;
        }
    }
    return eOK;
}

int8_t socket_id_assign_and_attach(socket_t *socket)
{
    int pos = last_allocated_socket + 1;

    for (int i = 0; i < SOCKETS_MAX; i++, pos++) {
        if (pos >= SOCKETS_MAX) {
            pos = 0;
        }
        if (socket_instance[pos] == NULL) {
            socket->id = pos;
            socket_instance[pos] = socket_reference(socket);
            last_allocated_socket = pos;
            return pos;
        }
    }
    return -1;
}

void socket_id_detach(int8_t sid)
{
    socket_t *socket = socket_instance[sid];
    socket->id = -1;
    socket_release(socket);
    socket_instance[sid] = socket_dereference(socket);
}

inet_pcb_t *socket_inet_pcb_allocate(void)
{
    inet_pcb_t *inet_pcb = ns_dyn_mem_alloc(sizeof(inet_pcb_t));
    if (!inet_pcb) {
        return NULL;
    }

    memset(inet_pcb, 0, sizeof(inet_pcb_t));
#ifndef NO_TCP
    inet_pcb->session = NULL;
#endif
    inet_pcb->unicast_hop_limit = -1;
    inet_pcb->multicast_hop_limit = 1;
    inet_pcb->multicast_if = 0;
    inet_pcb->multicast_loop = true;
    inet_pcb->socket = NULL;
    inet_pcb->addr_preferences = 0;
    inet_pcb->recvhoplimit = false;
    inet_pcb->recvpktinfo = false;
    inet_pcb->recvtclass = false;
    inet_pcb->link_layer_security = -1;
#ifndef NO_IPV6_PMTUD
    inet_pcb->use_min_mtu = -1;
#endif
#ifndef NO_IP_FRAGMENT_TX
    inet_pcb->dontfrag = 0;
#endif
    inet_pcb->tclass = SOCKET_IPV6_TCLASS_DEFAULT;
    inet_pcb->flow_label = IPV6_FLOW_UNSPECIFIED;
    ns_list_init(&inet_pcb->mc_groups);

    return inet_pcb;
}

inet_pcb_t *socket_inet_pcb_clone(const inet_pcb_t *orig)
{
    inet_pcb_t *inet_pcb = ns_dyn_mem_alloc(sizeof(inet_pcb_t));
    if (!inet_pcb) {
        return NULL;
    }
    *inet_pcb = *orig;
    inet_pcb->socket = NULL;
#ifndef NO_TCP
    inet_pcb->session = NULL;
#endif
    return inet_pcb;
}


socket_t *socket_allocate(socket_type_t type)
{
    socket_t *socket = ns_dyn_mem_alloc(sizeof(socket_t));
    if (!socket) {
        return NULL;
    }
    memset(socket, 0, sizeof * socket);
    socket->id = -1;
    socket->type = type;
    socket->family = SOCKET_FAMILY_NONE;
    sockbuf_init(&socket->rcvq);
    sockbuf_init(&socket->sndq);
    if (type == SOCKET_TYPE_STREAM) {
        socket->sndq.low_water_mark = SOCKET_DEFAULT_STREAM_SNDLOWAT;
        sockbuf_reserve(&socket->rcvq, SOCKET_DEFAULT_STREAM_RCVBUF);
        sockbuf_reserve(&socket->sndq, SOCKET_DEFAULT_STREAM_SNDBUF);
    }

    ns_list_add_to_end(&socket_list, socket);
    return socket;
}

/* Increase reference counter on socket, returning now-owned pointer */
socket_t *socket_reference(socket_t *socket_ptr)
{
    if (!socket_ptr) {
        return NULL;
    }

    socket_ptr->refcount++;
    if (socket_ptr->refcount == 0) {
        socket_ptr->refcount--;
        tr_error("ref overflow");
    }

    return socket_ptr;
}

/* Decrease reference counter on socket, releasing if it hits zero */
/* Always returns NULL to indicate caller no longer owns it */
socket_t *socket_dereference(socket_t *socket_ptr)
{
    if (!socket_ptr) {
        return NULL;
    }

    if (socket_ptr->refcount == 0) {
        tr_error("ref underflow");
        return NULL;
    }
    if (--socket_ptr->refcount == 0) {
        socket_free(socket_ptr);
    }
    return NULL;
}


/**
 * Allocate a socket
 *
 * \param sid pointer to socket ID which will contain socket ID
 * \param port listen port for socket
 *
 * \return eOK socket opened
 * \return eFALSE no free sockets
 * \return eBUSY port reserved
 */
socket_error_t socket_create(socket_family_t family, socket_type_t type, uint8_t protocol, int8_t *sid, uint16_t port, void (*passed_fptr)(void *), bool buffer_type)
{
    if (sid) {
        *sid = -1;
    }

    if (passed_fptr == 0) {
        return eFALSE;
    }

    /* Note that IPv6 socket lookup implementation assumes we don't have raw
     * TCP or UDP sockets, thus we ensure protocol=UDP iff type=DGRAM,
     * and protocol=TCP iff type=STREAM.
     */
    if (family == SOCKET_FAMILY_IPV6) {
        switch (type) {
            case SOCKET_TYPE_DGRAM:
                protocol = IPV6_NH_UDP;
                break;
            case SOCKET_TYPE_STREAM:
                protocol = IPV6_NH_TCP;
                break;
            default:
                if (protocol == 0 || protocol == IPV6_NH_TCP || protocol == IPV6_NH_UDP) {
                    return eFALSE;
                }
                port = 0xFFFF;
                break;
        }
    } else {
        /* SOCKET_FAMILY_LOCAL still has inet_pcb - give it 0 protocol */
        protocol = 0;
        port = 0xFFFF;
    }

    if (port != 0 && socket_port_validate(port, protocol) == eFALSE) {
        return eBUSY;
    }

    socket_t *socket = socket_allocate(type);
    if (socket == NULL) {
        tr_error("Socket allocation fail");
        return eFALSE;
    }
    inet_pcb_t *inet_pcb = socket_inet_pcb_allocate();
    if (inet_pcb == NULL) {
        socket_free(socket); // don't leave half initialized sockets behind
        tr_error("inet_pcb allocation fail");
        return eFALSE;
    }
    socket->inet_pcb = inet_pcb;

    *sid = socket_id_assign_and_attach(socket);
    if (*sid == -1) {
        tr_error("Socket IDs exhausted");
        socket_free(socket);
        return eFALSE;
    }


    socket->flags = 0;

    tr_debug("Socket id %d allocated", socket->id);
    socket->tasklet = eventOS_scheduler_get_active_tasklet();
    socket->family = family;
    socket->u.live.fptr = passed_fptr;
    ns_list_init(&socket->u.live.queue);
    socket->default_interface_id = -1;
    socket->broadcast_pan = 0;
    if (buffer_type) {
        socket->flags |= SOCKET_BUFFER_CB;
    }

    // Fill Internet protocol control block
    inet_pcb->protocol = protocol;
    inet_pcb->local_port = port;
    inet_pcb->socket = socket;

    return eOK;
}

socket_t *socket_new_incoming_connection(socket_t *listen_socket)
{
    if (!socket_validate_listen_backlog(listen_socket)) {
        return NULL;
    }
    socket_t *new_socket = socket_allocate(listen_socket->type);
    if (!new_socket) {
        return NULL;
    }
    inet_pcb_t *inet_pcb = socket_inet_pcb_clone(listen_socket->inet_pcb);
    if (!inet_pcb) {
        socket_free(new_socket);
        return NULL;
    }
    inet_pcb->socket = new_socket;
    // They can fill this in on return
    new_socket->inet_pcb = inet_pcb;
    new_socket->flags = SOCKET_FLAG_PENDING | SOCKET_FLAG_CONNECTING;
    new_socket->family = listen_socket->family;
    new_socket->type = listen_socket->type;
    new_socket->default_interface_id = listen_socket->default_interface_id;
    new_socket->broadcast_pan = listen_socket->broadcast_pan;
    new_socket->tasklet = listen_socket->tasklet;
    new_socket->u.pending.listen_head = listen_socket;
    ns_list_add_to_end(&listen_socket->u.live.queue, socket_reference(new_socket));

    return new_socket;
}

/* Connection did not complete or was abandoned - notification from transport */
void socket_connection_abandoned(socket_t *socket, int8_t interface_id, uint8_t reason)
{
    if (!(socket->flags & (SOCKET_FLAG_CONNECTING | SOCKET_FLAG_CONNECTED))) {
        tr_err("abandoned: not connecting/connected");
        return;
    }
    socket->flags &= ~ SOCKET_FLAG_CONNECTING;
    // leaving CONNECTED flag set prevents weirdness like reconnecting. Good idea?
    socket->flags |= SOCKET_FLAG_SHUT_WR | SOCKET_FLAG_CANT_RECV_MORE | SOCKET_FLAG_CONNECTED;
    sockbuf_flush(&socket->sndq);
    if (socket->flags & SOCKET_FLAG_PENDING) {
        // By leaving pending state, without any remaining reference,
        // we just let the failed connection go away, without notifying user
        socket_leave_pending_state(socket, NULL);
        // Hard to say if this is right if it was fully connected, but seems the best
        // we can do for the minute - we can't queue up the reset/whatever event.
    } else if (reason != 0) {
        socket_event_push(reason, socket, interface_id, NULL, 0);
    }
}

/* Connection has completed - notification from transport */
void socket_connection_complete(socket_t *socket, int8_t interface_id)
{
    if (!(socket->flags & SOCKET_FLAG_CONNECTING)) {
        tr_err("complete: not connecting");
        return;
    }
    if (socket->flags & SOCKET_FLAG_CONNECTED) {
        tr_err("complete: already connected");
        return;
    }
    socket->flags &= ~ SOCKET_FLAG_CONNECTING;
    socket->flags |= SOCKET_FLAG_CONNECTED;
    if (socket->flags & SOCKET_FLAG_PENDING) {
        socket_event_push(SOCKET_INCOMING_CONNECTION, socket->u.pending.listen_head, interface_id, NULL, 0);
    } else {
        socket_event_push(SOCKET_CONNECT_DONE, socket, interface_id, NULL, 0);
    }
}

/* Socket is to leave pending state - either being accepted or released */
void socket_leave_pending_state(socket_t *socket, void (*fptr)(void *))
{
    if (!(socket->flags & SOCKET_FLAG_PENDING)) {
        tr_err("not pending");
        return;
    }
    ns_list_remove(&socket->u.pending.listen_head->u.live.queue, socket);
    socket->flags &= ~SOCKET_FLAG_PENDING;
    ns_list_init(&socket->u.live.queue);
    socket->u.live.fptr = fptr;
    socket_dereference(socket);
}

void socket_cant_recv_more(socket_t *socket, int8_t interface_id)
{
    socket->flags |= SOCKET_FLAG_CANT_RECV_MORE;
    socket_event_push(SOCKET_DATA, socket, interface_id, 0, 0);
}

socket_t *socket_lookup_ipv6(uint8_t protocol, const sockaddr_t *local_addr, const sockaddr_t *remote_addr, bool allow_wildcards)
{
    //tr_debug("socket_lookup() local=%s [%d] remote=%s [%d]", trace_ipv6(local_addr->address), local_addr->port, trace_ipv6(remote_addr->address), remote_addr->port);
    ns_list_foreach(socket_t, socket, &socket_list) {
        if (!socket_is_ipv6(cur)) {
            continue;
        }
        inet_pcb_t *cur = socket->inet_pcb;
        //tr_debug("cur local=%s [%d] remote=%s [%d]", trace_ipv6(cur->local_address), cur->local_port, trace_ipv6(cur->remote_address), cur->remote_port);
        /* Protocol must match */
        if (cur->protocol != protocol) {
            continue;
        }

        /* For TCP and UDP only, ports must match */
        if (protocol == IPV6_NH_UDP || protocol == IPV6_NH_TCP) {
            if (cur->local_port == 0 || cur->local_port != local_addr->port) {
                continue;
            }
            if (!(allow_wildcards && cur->remote_port == 0) && cur->remote_port != remote_addr->port) {
                continue;
            }
        }

        if (!(allow_wildcards && addr_ipv6_equal(cur->local_address, ns_in6addr_any))) {
            if (!addr_ipv6_equal(cur->local_address, local_addr->address)) {
                continue;
            }
        }

        if (!(allow_wildcards && addr_ipv6_equal(cur->remote_address, ns_in6addr_any))) {
            if (!addr_ipv6_equal(cur->remote_address, remote_addr->address)) {
                continue;
            }
        }

        return socket;
    }

    return NULL;
}

/* Write address + port neatly to out, returning characters written */
/* Maximum output length is 48, including the terminator, assuming ip6tos limited to 39 */
static int sprint_addr(char *out, const uint8_t addr[static 16], uint16_t port)
{
    char *init_out = out;
    if (addr_is_ipv6_unspecified(addr)) {
        *out++ = '*';
    } else {
        *out++ = '[';
        out += ip6tos(addr, out);
        *out++ = ']';
    }

    *out++ = ':';
    if (port == 0) {
        *out++ = '*';
    } else {
        out += sprintf(out, "%"PRIu16, port);
    }
    *out = '\0';
    return (int)(out - init_out);
}

static void socket_print(const socket_t *socket, int lwidth, int rwidth, route_print_fn_t *print_fn, char sep)
{
    if (!socket_is_ipv6(socket)) {
        return;
    }
    const inet_pcb_t *pcb = socket->inet_pcb;
    char remote_str[48];
    char local_str[48];

    char proto_buf[4];
    const char *proto_str;
    switch (pcb->protocol) {
        case IPV6_NH_TCP:
            proto_str = "TCP";
            break;
        case IPV6_NH_UDP:
            proto_str = "UDP";
            break;
        case IPV6_NH_ICMPV6:
            proto_str = "ICMP6";
            break;
        default:
            sprintf(proto_buf, "%"PRIu8, pcb->protocol);
            proto_str = proto_buf;
            break;
    }
    sprint_addr(local_str, pcb->local_address, pcb->local_port);
    sprint_addr(remote_str, pcb->remote_address, pcb->remote_port);

    const char *state = "";
    if (pcb->protocol == IPV6_NH_TCP) {
        if (tcp_info(pcb)) {
            state = tcp_state_name(tcp_info(pcb));
        } else if (socket->flags & SOCKET_LISTEN_STATE) {
            state = "LISTEN";
        } else {
            state = "CLOSED";
        }
    }
    print_fn("%3.*"PRId8"%c%3u%c%6"PRId32/*"%c%6"PRId32*/"%c%6"PRId32"%c%-5.5s%c%-*s%c%-*s%c%s",
             socket->id >= 0 ? 1 : 0, socket->id >= 0 ? socket->id : 0, sep,
             socket->refcount, sep,
             socket->rcvq.data_bytes, sep,
             /*socket->rcvq.buf_overhead_bytes, sep,*/
             socket->sndq.data_bytes, sep,
             proto_str, sep,
             lwidth, local_str, sep,
             rwidth, remote_str, sep,
             state);
}

void socket_list_print(route_print_fn_t *print_fn, char sep)
{
    int lwidth = 18, rwidth = 18;
    ns_list_foreach(const socket_t, socket, &socket_list) {
        if (!socket_is_ipv6(socket)) {
            continue;
        }
        const inet_pcb_t *pcb = socket->inet_pcb;
        char addr[48];
        int len;
        len = sprint_addr(addr, pcb->local_address, pcb->local_port);
        if (lwidth < len) {
            lwidth = len;
        }
        len = sprint_addr(addr, pcb->remote_address, pcb->remote_port);
        if (rwidth < len) {
            rwidth = len;
        }
    }
    print_fn("Sck%cRef%cRecv-Q%c"/*"Recv-B%c"*/"Send-Q%cProto%c%-*s%c%-*s%c(state)", sep, sep, sep, /*sep,*/ sep, sep, lwidth, "Local Address", sep, rwidth, "Remote Address", sep);
    ns_list_foreach(const socket_t, socket, &socket_list) {
        socket_print(socket, lwidth, rwidth, print_fn, sep);
    }

    /* Chuck in a consistency check */
    for (int i = 0; i < SOCKETS_MAX; i++) {
        if (socket_instance[i] && socket_instance[i]->id != i) {
            tr_err("ID %d points to %p with id %d\n", i, (void *)socket_instance[i], socket_instance[i]->id);
        }
    }
    ns_list_foreach(socket_t, socket, &socket_list) {
        if (socket->id != -1 && socket_pointer_get(socket->id) != socket) {
            tr_err("Socket %p has invalid ID %d\n", (void *)socket, socket->id);
        }
        sockbuf_check(&socket->rcvq);
        sockbuf_check(&socket->sndq);
    }
}

socket_t *socket_lookup(socket_family_t family, uint8_t protocol, const sockaddr_t *local_addr, const sockaddr_t *remote_addr)
{
    switch (family) {
        case SOCKET_FAMILY_IPV6:
            return socket_lookup_ipv6(protocol, local_addr, remote_addr, true);
        default:
            return NULL;
    }
}

/**
 * Push a buffer to a socket
 *
 * Used by the protocol core to push buffers to sockets.
 *
 * To determine the socket to send to, transport layer must ensure either:
 *   a) buf->socket_id already filled in, or
 *   b) buf->options.type is socket family (IPv6/Local)
 *      buf->options.code is protocol (IPv6=NH,Local=n/a)
 *
 * \param buf buffer to push
 *
 * \return eOK socket opened
 * \return eFALSE no socket found
 * \return eBUSY socket full
 */
socket_error_t socket_up(buffer_t *buf)
{
    socket_t *socket = buf->socket;

    if (!socket) {
        socket = socket_lookup((socket_family_t) buf->options.type, buf->options.code, &buf->dst_sa, &buf->src_sa);

        if (!socket) {
            //tr_debug("Socket:Drop");
            goto drop;
        }
        buffer_socket_set(buf, socket);
    }

    if ((socket->flags & (SOCKET_FLAG_PENDING | SOCKET_FLAG_CLOSED)) || socket->id == -1) {
        goto drop;
    }

    if (socket->rcvq.data_byte_limit == 0) {
        // Old-style one event per buffer
        socket_data_event_push(buf);
    } else {
        // Queuing enabled - new-style events
        if (sockbuf_space(&socket->rcvq) < buffer_data_length(buf)) {
            tr_debug("Socket %d Recv-Q full", socket->id);
            goto drop;
        }
        bool pushed = socket_data_queued_event_push(socket);
        // if this failed, it means total memory exhaustion - safest to drop the packet
        if (!pushed) {
            goto drop;
        }
        sockbuf_append(&socket->rcvq, buf);
    }
    return eOK;

drop:
    buffer_free(buf);
    return eFALSE;
}

void socket_event_push(uint8_t sock_event, socket_t *socket, int8_t interface_id, void *session_ptr, uint16_t length)
{
    if (socket->flags & (SOCKET_FLAG_PENDING | SOCKET_FLAG_CLOSED)) {
        return;
    }

    socket_cb_event_t *cb_event = ns_dyn_mem_temporary_alloc(sizeof(socket_cb_event_t));
    if (cb_event) {
        cb_event->socket = socket_reference(socket);
        cb_event->socket_event = sock_event;
        cb_event->session_ptr = session_ptr;
        cb_event->interface_id = interface_id;
        cb_event->length = length;
        arm_event_s event = {
            .receiver = socket_event_handler,
            .sender = 0,
            .data_ptr = cb_event,
            .event_type = ARM_SOCKET_EVENT_CB,
            .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
        };
        if (eventOS_event_send(&event) != 0) {
            socket_dereference(socket);
            ns_dyn_mem_free(cb_event);
        }
    }
}


/* Fill in a buffer's hop limit, based on the user's socket options */
void socket_inet_pcb_set_buffer_hop_limit(const inet_pcb_t *inet_pcb, buffer_t *buf, const int16_t *msg_hoplimit)
{
    /* If hop limit specified by ancillary data, obey that over socket options */
    if (msg_hoplimit) {
        if (*msg_hoplimit != -1) {
            /* Use the value they gave for this message */
            buf->options.hop_limit = *msg_hoplimit;
            return;
        }
        /* If -1, we will go with the default, ignoring socket options */
    } else if (addr_is_ipv6_multicast(buf->dst_sa.address)) {
        /* Multicasts have a fixed default - multicast_hop_limit will be user-specified, or that default */
        buf->options.hop_limit = inet_pcb->multicast_hop_limit;
        return;
    } else if (inet_pcb->unicast_hop_limit != -1) {
        /* If the user specified a unicast hop limit, use it */
        buf->options.hop_limit = inet_pcb->unicast_hop_limit;
        return;
    }

    /* User didn't specify a unicast hop limit, get the per-interface default */
    /* At the moment we can only rely on if_index in the down direction, not id */
    protocol_interface_info_entry_t *cur = buf->interface;
    buf->options.hop_limit = cur->cur_hop_limit;
}

/**
 * Calculate message payload length and validate message header iov vectors
 */
bool socket_message_validate_iov(const ns_msghdr_t *msg, uint16_t *length_out)
{
    if (msg->msg_iovlen != 0 && !msg->msg_iov) {
        return false;
    }

    uint16_t msg_length = 0;
    ns_iovec_t *msg_iov = msg->msg_iov;
    for (uint_fast16_t i = 0; i < msg->msg_iovlen; i++) {
        if (msg_iov->iov_len != 0 && !msg_iov->iov_base) {
            return false;
        }
        msg_length += msg_iov->iov_len;
        if (msg_length < msg_iov->iov_len) {
            return false;
        }
        msg_iov++;
    }

    if (length_out) {
        *length_out = msg_length;
    }

    return true;
}

/**
 * Write a data buffer to a socket
 *
 * Used by the application to send data with meta data from msg
 *
 * Data will be taken either from buf if non-NULL else from msg->msg_iov.
 *
 * Using buf is not permitted for stream sockets.
 *
 * \param sid socket id
 * \param buf pointer to buffer is plain buffer with payload only - all metadata ignored
 * \param msg pointer to message-specific data message name define destination address and control message define ancillary data
 *
 * \return 0 Ok done
 * \return -1 Unsupported socket or message parameter
 * \return -5 Socket not properly connected
 * \return -6 Packet too short
 */
int16_t socket_buffer_sendmsg(int8_t sid, buffer_t *buf, const struct ns_msghdr *msg, int flags)
{
    int8_t ret_val;
    int8_t interface_id = -1;

    const int16_t *msg_hoplimit = NULL;
    uint16_t payload_length;

    //Validate socket id
    socket_t *socket_ptr = socket_pointer_get(sid);

    if (!socket_ptr || !socket_ptr->inet_pcb) {
        tr_warn("Socket Id %i", sid);
        ret_val = -1;
        goto fail;
    }
    inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;

    if (buf) {
        payload_length = buffer_data_length(buf);
    } else {
        if (!socket_message_validate_iov(msg, &payload_length)) {
            return -1;
        }
    }

#ifndef NO_TCP
    /* Special handling for stream - basically no advanced features of sendmsg operate.
     * Can't set address, can't set ancilliary data.
     * Don't want to fill in other sticky options into buffer - TCP must handle this itself,
     * as it must do it for all packets, not just those coming from sendmsg.
     */
    if (socket_ptr->type == SOCKET_TYPE_STREAM) {
        /* Stream sockets must be connected and not shutdown for write */
        if ((socket_ptr->flags & (SOCKET_FLAG_CONNECTED | SOCKET_FLAG_SHUT_WR)) != SOCKET_FLAG_CONNECTED) {
            ret_val = -5;
            goto fail;
        }

        /* Stream sockets cannot have address specified (POSIX "EISCONN") */
        if (msg && msg->msg_name && msg->msg_namelen) {
            ret_val = -5;
            goto fail;
        }

        if (payload_length == 0) {
            goto success; // Sending nothing is a no-op
        }

        // Figure out how much we can actually take, and reduce
        // payload_length if necessary (but if fed a buffer, we always just
        // accept the whole thing).
        if (!buf) {
            int32_t space = sockbuf_space(&socket_ptr->sndq);
            if (space < payload_length) {
                if (space < (int32_t) socket_ptr->sndq.low_water_mark) {
                    ret_val = NS_EWOULDBLOCK;
                    goto fail;
                }
                if (flags & NS_MSG_LEGACY0) {
                    // Can't do a partial write, as we can't actually
                    // indicate that we've done so. For compatibility,
                    // accept a write of any size if there is some buffer
                    // space available. This could make us massively go
                    // over the buffer size limit.
                } else {
                    payload_length = space;
                }
            }
        }
    }
#endif // NO_TCP

    // Now copy (some of) the data into a buffer
    if (!buf) {
        buf = buffer_get(payload_length);
        if (!buf) {
            ret_val = -2;
            goto fail;
        }

        const ns_iovec_t *msg_iov = msg->msg_iov;
        uint16_t to_copy = payload_length;
        for (uint_fast16_t i = 0; i < msg->msg_iovlen; i++, msg_iov++) {
            uint16_t len = msg_iov->iov_len < to_copy ? msg_iov->iov_len : to_copy;
            buffer_data_add(buf, msg_iov->iov_base, len);
            to_copy -= len;
            if (to_copy == 0) {
                break;
            }
        }
    }

    buf->payload_length = payload_length;

#ifndef NO_TCP
    if (socket_ptr->type == SOCKET_TYPE_STREAM) {
        tcp_session_t *tcp_info = tcp_info(inet_pcb);
        if (!tcp_info) {
            tr_warn("No TCP session for cur Socket");
            ret_val = -3;
            goto fail;
        }

        switch (tcp_session_send(tcp_info, buf)) {
            case TCP_ERROR_NO_ERROR:
                break;
            case TCP_ERROR_WRONG_STATE:
            default:
                ret_val = -3;
                goto fail;
        }
        goto success;
    }
    // TCP system has taken ownership of buffer, or we've failed
    // Everything below this point is non-TCP
#endif //NO_TCP

    /**
     * Mark Socket id to buffer meta data
     */
    buffer_socket_set(buf, socket_ptr);

    /**
     * Allocate random port if port is not defined.
     */
    if (inet_pcb->local_port == 0) {
        inet_pcb->local_port = socket_generate_random_port(inet_pcb->protocol);
        if (inet_pcb->local_port == 0) {
            ret_val = -2;
            goto fail;
        }
    }

    /**
     * Set socket configured parameters
     */
    buf->options.traffic_class = inet_pcb->tclass;
    buf->options.ipv6_use_min_mtu = inet_pcb->use_min_mtu;
    buf->options.ipv6_dontfrag = inet_pcb->dontfrag;
    buf->options.multicast_loop = inet_pcb->multicast_loop;

    /* Set default remote address from PCB */
    if (inet_pcb->remote_port != 0 && !addr_ipv6_equal(inet_pcb->remote_address, ns_in6addr_any)) {
        memcpy(buf->dst_sa.address, inet_pcb->remote_address, 16);
        buf->dst_sa.port = inet_pcb->remote_port;
        buf->dst_sa.addr_type = ADDR_IPV6;
    } else {
        buf->dst_sa.addr_type = ADDR_NONE;
    }

    /**
     * validate Message name and Parse Ancillary Data when Message header is shared.
     */
    if (msg) {
        /**
         * Validate message name (destination) and set address to buffer
         */
        if (msg->msg_namelen) {
            if (!msg->msg_name || msg->msg_namelen != sizeof(ns_address_t)) {
                ret_val = -1;
                goto fail;
            }

            ns_address_t *address = msg->msg_name;
            buf->dst_sa.port = address->identifier;
            buf->dst_sa.addr_type = ADDR_IPV6;
            memcpy(buf->dst_sa.address, address->address, 16);
        }

        ns_cmsghdr_t *cmsg = NS_CMSG_FIRSTHDR(msg);
        //Stay at while when full ancillary data is analyzed
        while (cmsg) {
            if (cmsg->cmsg_level == SOCKET_IPPROTO_IPV6) {
                const void *data_ptr = NS_CMSG_DATA(cmsg);
                switch (cmsg->cmsg_type) {
                    case SOCKET_IPV6_PKTINFO: {
                        if (cmsg->cmsg_len != NS_CMSG_LEN(sizeof(ns_in6_pktinfo_t))) {
                            ret_val = -1;
                            goto fail;
                        }
                        const ns_in6_pktinfo_t *packetinfo = data_ptr;
                        memcpy(buf->src_sa.address, packetinfo->ipi6_addr, 16);
                        buf->src_sa.addr_type = addr_ipv6_equal(packetinfo->ipi6_addr, ns_in6addr_any) ? ADDR_NONE : ADDR_IPV6;
                        /**
                         * Set interface id when it is configured >0
                         */
                        if (packetinfo->ipi6_ifindex > 0) {
                            interface_id = packetinfo->ipi6_ifindex;
                        }
                        break;
                    }

                    case SOCKET_IPV6_HOPLIMIT:
                        if (cmsg->cmsg_len != NS_CMSG_LEN(sizeof(int16_t))) {
                            ret_val = -1;
                            goto fail;
                        }
                        msg_hoplimit = data_ptr;
                        if (*msg_hoplimit < -1 || *msg_hoplimit > 255) {
                            ret_val = -1;
                            goto fail;
                        }
                        break;

                    case SOCKET_IPV6_TCLASS: {
                        if (cmsg->cmsg_len != NS_CMSG_LEN(sizeof(int16_t))) {
                            ret_val = -1;
                            goto fail;
                        }
                        int16_t tclass = *(const int16_t *)data_ptr;

                        if (tclass == -1) {
                            buf->options.traffic_class = SOCKET_IPV6_TCLASS_DEFAULT;
                        } else if (tclass >= 0 && tclass <= 255) {
                            buf->options.traffic_class = tclass;
                        } else {
                            ret_val = -1;
                            goto fail;
                        }
                        break;
                    }
#ifndef NO_IPV6_PMTUD
                    case SOCKET_IPV6_USE_MIN_MTU: {
                        if (cmsg->cmsg_len != NS_CMSG_LEN(sizeof(int8_t))) {
                            ret_val = -1;
                            goto fail;
                        }
                        int8_t use_min_mtu = *(const int8_t *) data_ptr;
                        if (use_min_mtu < -1 || use_min_mtu > 1) {
                            ret_val = -1;
                            goto fail;
                        }
                        buf->options.ipv6_use_min_mtu = use_min_mtu;
                        break;
                    }
#endif
#ifndef NO_IP_FRAGMENT_TX
                    case SOCKET_IPV6_DONTFRAG: {
                        if (cmsg->cmsg_len != NS_CMSG_LEN(sizeof(int8_t))) {
                            ret_val = -1;
                            goto fail;
                        }
                        int8_t dont_frag = *(const int8_t *) data_ptr;
                        if (dont_frag < 0 || dont_frag > 1) {
                            ret_val = -1;
                            goto fail;
                        }
                        buf->options.ipv6_dontfrag = dont_frag;
                        break;
                    }
#endif
                    case SOCKET_IPV6_MULTICAST_LOOP: {
                        if (cmsg->cmsg_len != NS_CMSG_LEN(sizeof(bool))) {
                            ret_val = -1;
                            goto fail;
                        }
                        buf->options.multicast_loop = *(const bool *) data_ptr;
                        break;
                    }
                    default:
                        break;
                }
            }
            cmsg = NS_CMSG_NXTHDR(msg, cmsg);
        }
    }

    /* Should have destination address by now */
    if (buf->dst_sa.addr_type == ADDR_NONE) {
        return -5;
    }

    switch (socket_ptr->type) {
        case SOCKET_TYPE_DGRAM:
            buf->info = (buffer_info_t)(B_DIR_DOWN + B_FROM_APP + B_TO_UDP);
            break;

        case SOCKET_TYPE_RAW:
            if (inet_pcb->protocol == IPV6_NH_ICMPV6) {
                if (buffer_data_length(buf) < 4) {
                    ret_val = -6;
                    goto fail;
                }
                uint8_t *ptr = buffer_data_pointer(buf);
                buf->options.type = ptr[0];
                buf->options.code = ptr[1];
                buffer_data_strip_header(buf, 4);
                buf->info = (buffer_info_t)(B_DIR_DOWN + B_FROM_APP + B_TO_ICMP);
            } else {
                buf->options.type = inet_pcb->protocol;
                buf->info = (buffer_info_t)(B_DIR_DOWN + B_FROM_APP + B_TO_IPV6);
            }
            break;
        default:
            ret_val = -1;
            goto fail;
    }

    /* If no address in packet info, use bound address, if any. Set before interface selection -
     * socket_interface_determine could otherwise auto-select source as a side-effect. */
    if (buf->src_sa.addr_type == ADDR_NONE && !addr_ipv6_equal(inet_pcb->local_address, ns_in6addr_any)) {
        memcpy(buf->src_sa.address, inet_pcb->local_address, 16);
        buf->src_sa.addr_type = ADDR_IPV6;
    }

    /* If address is specified manually we must validate that the
     * address is valid in one of the available interfaces
     * */
    if (buf->src_sa.addr_type == ADDR_IPV6 &&
            protocol_interface_address_compare(buf->src_sa.address) != 0) {
        tr_warn("Specified source address %s is not valid", trace_ipv6(buf->src_sa.address));
        ret_val = -3;
        goto fail;
    }

    /**
     * Select outgoing interface for this message
     */
    protocol_interface_info_entry_t *cur_interface;
    if (interface_id > 0) {
        cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    } else {
        cur_interface = socket_interface_determine(socket_ptr, buf);
    }

    if (!cur_interface) {
        tr_warn("sendto:No interface");
        ret_val = -4;
        goto fail;
    }

    buf->interface = cur_interface;

    /**
     * Link-specific configuration
     */
    buf->options.ll_security_bypass_tx = (inet_pcb->link_layer_security == 0);
    if (socket_ptr->broadcast_pan > 0) {
        buf->link_specific.ieee802_15_4.useDefaultPanId = false;
        buf->link_specific.ieee802_15_4.dstPanId = 0xffff;
    }

    /**
     * Set Hop Limit
     */
    socket_inet_pcb_set_buffer_hop_limit(inet_pcb, buf, msg_hoplimit);
    /**
     * Flow label set
     */
    buf->options.flow_label = inet_pcb->flow_label;

    /**
     * Select source address for this message
     */
    if (buf->src_sa.addr_type == ADDR_NONE) {
        const uint8_t *ipv6_ptr = addr_select_source(cur_interface, buf->dst_sa.address, inet_pcb->addr_preferences);
        if (!ipv6_ptr) {
            tr_warn("No address");
            ret_val = -3;
            goto fail;
        }

        memcpy(buf->src_sa.address, ipv6_ptr, 16);
        buf->src_sa.addr_type = ADDR_IPV6;
    }

    buf->src_sa.port = inet_pcb->local_port;

    protocol_push(buf);

#ifndef NO_TCP
    /* TCP jumps back to here */
success:
#endif
    if (flags & NS_MSG_LEGACY0) {
        return 0;
    } else {
        return payload_length;
    }

fail:
    tr_warn("fail: socket_buffer_sendmsg");
    if (buf) {
        buffer_free(buf);
    }

    return ret_val;
}

void socket_tx_buffer_event_and_free(buffer_t *buf, uint8_t status)
{
    buf = socket_tx_buffer_event(buf, status);
    if (buf) {
        buffer_free(buf);
    }
}

buffer_t *socket_tx_buffer_event(buffer_t *buf, uint8_t status)
{
    /* TODO here - if not a locally generated packet, and it's a TX_FAIL
     * (when we finally implement it), should probably generate ICMP
     * address unreachable. (Kind of needed if address resolution was skipped
     * and we mapped straight to MAC address).
     */

    if (buf->ack_receive_cb) {
        buf->ack_receive_cb(buf, status);
    }

    /* Suppress events once socket orphaned */
    if (!buf->socket || (buf->socket->flags & (SOCKET_FLAG_PENDING | SOCKET_FLAG_CLOSED))) {
        return buf;
    }

    protocol_interface_info_entry_t *cur_interface = buf->interface;

    socket_event_push(status, buf->socket, cur_interface ? cur_interface->id : -1, buf->session_ptr, buf->payload_length);

    return buf;
}

static void mc_group_free(inet_pcb_t *inet_pcb, inet_group_t *mc)
{
    protocol_interface_info_entry_t *cur_interface = protocol_stack_interface_info_get_by_id(mc->interface_id);
    if (cur_interface) {
        addr_delete_group(cur_interface, mc->group_addr);
    }

    ns_list_remove(&inet_pcb->mc_groups, mc);
    ns_dyn_mem_free(mc);
}

/*
 * validate that socket backlog allows new connection.
 * Incoming socket needs to be listening socket.
 *
 * \return true if incoming connection can proceed
 * */
bool socket_validate_listen_backlog(const socket_t *socket_ptr)
{
    if (!(socket_ptr->flags & SOCKET_LISTEN_STATE)) {
        return false;
    }

    return ns_list_count(&socket_ptr->u.live.queue) < socket_ptr->listen_backlog;
}

inet_pcb_t *socket_inet_pcb_free(inet_pcb_t *inet_pcb)
{
    if (inet_pcb) {
        ns_list_foreach_safe(inet_group_t, mc, &inet_pcb->mc_groups) {
            mc_group_free(inet_pcb, mc);
        }
        ns_dyn_mem_free(inet_pcb);
    }
    return NULL;
}

int8_t socket_inet_pcb_join_group(inet_pcb_t *inet_pcb, int8_t interface_id, const uint8_t group[static 16])
{
    ns_list_foreach(inet_group_t, mc, &inet_pcb->mc_groups) {
        if ((interface_id == 0 || mc->interface_id == interface_id) && addr_ipv6_equal(mc->group_addr, group)) {
            return -3;
        }
    }
    if (interface_id == 0) {
        ipv6_route_t *route = ipv6_route_choose_next_hop(group, -1, NULL);
        if (!route) {
            return -3;
        }
        interface_id = route->info.interface_id;
    }
    protocol_interface_info_entry_t *cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur_interface) {
        return -3;
    }
    inet_group_t *mc = ns_dyn_mem_alloc(sizeof * mc);
    if (!mc) {
        return -3;
    }
    memcpy(mc->group_addr, group, 16);
    mc->interface_id = interface_id;
    if (!addr_add_group(cur_interface, group)) {
        /* This could also happen due to being an implicit group member, eg ff02::1 */
        /* In that case, doesn't seem unreasonable to return the same error as "already a member on this socket" */
        ns_dyn_mem_free(mc);
        return -3;
    }

    ns_list_add_to_end(&inet_pcb->mc_groups, mc);
    return 0;
}

int8_t socket_inet_pcb_leave_group(inet_pcb_t *inet_pcb, int8_t interface_id, const uint8_t group[static 16])
{
    inet_group_t *mc = NULL;
    ns_list_foreach(inet_group_t, cur, &inet_pcb->mc_groups) {
        if ((interface_id == 0 || cur->interface_id == interface_id) && addr_ipv6_equal(cur->group_addr, group)) {
            mc = cur;
            break;
        }
    }
    if (!mc) {
        return -3;
    }

    mc_group_free(inet_pcb, mc);

    return 0;
}

struct protocol_interface_info_entry *socket_interface_determine(const socket_t *socket_ptr, buffer_t *buf)
{
    protocol_interface_info_entry_t *cur_interface;

    /* Link or realm-local scope uses default interface id if set (as if dest scope id), else multicast if, else choose 6LoWPAN, else IPv6(Ethernet) */
    /* Also for packets addressed to ourself (not needed any more - have loopback routes? */
    if (addr_ipv6_scope(buf->dst_sa.address, NULL) <= IPV6_SCOPE_REALM_LOCAL) {
        if (socket_ptr->default_interface_id != -1) {
            cur_interface = protocol_stack_interface_info_get_by_id(socket_ptr->default_interface_id);
            if (cur_interface) {
                return cur_interface;
            }
        }
    }

    /* Sockets can specify an interface for multicast packets */
    if (addr_is_ipv6_multicast(buf->dst_sa.address) && socket_ptr->inet_pcb->multicast_if > 0) {
        cur_interface = protocol_stack_interface_info_get_by_id(socket_ptr->inet_pcb->multicast_if);
        if (cur_interface && ipv6_buffer_route_to(buf, buf->dst_sa.address, cur_interface)) {
            return cur_interface;
        } else {
            return NULL;
        }
    }

    /* Try a routing table entry for greater-than-realm scope */
    if (addr_ipv6_scope(buf->dst_sa.address, NULL) > IPV6_SCOPE_REALM_LOCAL) {
        if (ipv6_buffer_route(buf)) {
            return buf->interface;
        }
    }

    /* Realm-local scope or lower now chooses an interface - 6LoWPAN default */
    if (addr_ipv6_scope(buf->dst_sa.address, NULL) <= IPV6_SCOPE_REALM_LOCAL) {
        buf->interface = protocol_stack_interface_info_get(IF_6LoWPAN);
        if (buf->interface) {
            return buf->interface;
        }
        buf->interface = protocol_stack_interface_info_get(IF_IPV6);
        if (buf->interface) {
            return buf->interface;
        }
        return NULL;
    }

    return NULL;
}
