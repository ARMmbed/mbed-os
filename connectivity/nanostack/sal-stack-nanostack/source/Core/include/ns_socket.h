/*
 * Copyright (c) 2008-2017, 2019, Arm Limited and affiliates.
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
 *
 * \file socket.h
 * \brief Socket API definitions.
 *
 *  Socket API functions and relevant structures.
 *
 *
 */


#ifndef _NS_SOCKET_H
#define _NS_SOCKET_H

#include "Core/include/ns_error_types.h"
#include "Core/include/ns_buffer.h"
#include "Core/include/sockbuf.h"

#ifndef SOCKET_RX_LIMIT
#define SOCKET_RX_LIMIT 3
#endif

#ifndef SOCKETS_MAX
#define SOCKETS_MAX 16
#endif

#ifndef SOCKET_DEFAULT_STREAM_RCVBUF
#define SOCKET_DEFAULT_STREAM_RCVBUF 2048
#endif

#ifndef SOCKET_DEFAULT_STREAM_SNDBUF
#define SOCKET_DEFAULT_STREAM_SNDBUF 2048
#endif

#ifndef SOCKET_DEFAULT_STREAM_SNDLOWAT
#define SOCKET_DEFAULT_STREAM_SNDLOWAT 512
#endif

typedef enum socket_family_e {
    SOCKET_FAMILY_NONE,
    SOCKET_FAMILY_IPV6,
} socket_family_t;

// This is always true because we currently have only 1 type
// If we support more types in future it becomes "((socket)->family == SOCKET_FAMILY_IPV6)"
#define socket_is_ipv6(socket) true

typedef enum socket_type_e {
    SOCKET_TYPE_DGRAM,
    SOCKET_TYPE_STREAM,
    SOCKET_TYPE_RAW
} socket_type_t;

#define SOCKET_FLAG_CLOSED          128     /* Has been closed by application */
#define SOCKET_FLAG_PENDING         64      /* Is waiting for accept on a listening socket */
#define SOCKET_FLAG_CANT_RECV_MORE  32
#define SOCKET_FLAG_SHUT_WR         16
#define SOCKET_FLAG_CONNECTED       8       /* Connection completed (and possibly since broken) */
#define SOCKET_LISTEN_STATE         4
#define SOCKET_BUFFER_CB            2
#define SOCKET_FLAG_CONNECTING      1       /* Connection in progress */

#ifndef SOCKET_IPV6_TCLASS_DEFAULT
#define SOCKET_IPV6_TCLASS_DEFAULT  0   /** Default traffic class for a new socket */
#endif

#ifndef SOCKET_LISTEN_BACKLOG_MAX
#define SOCKET_LISTEN_BACKLOG_MAX 10
#endif

typedef enum {
    ARM_SOCKET_INIT = 0,
    ARM_SOCKET_EVENT_CB = 1,
    ARM_SOCKET_DATA_CB = 2,  // Original data event - one buffer stored in event
    ARM_SOCKET_DATA_QUEUED_CB = 3, // New data event - no buffer in event, buffer in queue
    ARM_SOCKET_TCP_TIMER_CB,
} arm_socket_event_id_e;

typedef struct socket_buffer_callback_t {
    uint8_t event_type;
    int8_t socket_id;
    int8_t interface_id;
    buffer_t *buf;
    void *session_ptr;
} socket_buffer_callback_t;

typedef enum {
    SOCKET_SRC_ADDRESS_PRIMARY = 0,
    SOCKET_SRC_ADDRESS_SECONDARY = 1,
} socket_src_address_type;

struct inet_pcb_s;
struct ns_msghdr;
struct socket;
typedef NS_LIST_HEAD_INCOMPLETE(struct socket) socket_queue_list_t;

struct socket_pending {
    ns_list_link_t link;        /*!< Link as member of connection queue */
    struct socket *listen_head; /*!< Pointer to listening socket we're queued against */
};

struct socket_live {
    socket_queue_list_t queue;  /*!< Incoming connection queue */
    void (*fptr)(void *);
};

/** Socket structure */
typedef struct socket {
    /* This listen queue must be first, due to need to use incomplete list head */
    union {
        struct socket_pending pending; /* !< Info if PENDING flag set (pending connection) */
        struct socket_live live;      /* !< Info if PENDING flag clear (normal socket) */
    } u;
    int8_t    id;                     /*!< socket id */
    uint8_t   flags;                  /*!< Socket option flags */
    int8_t    tasklet;                /*!< Receiver tasklet */
    uint8_t   refcount;
    socket_family_t family;
    socket_type_t type;
    int8_t    default_interface_id;
    int8_t    broadcast_pan;
    uint8_t   listen_backlog;       /*!< Limit if pending connection queue */
    struct inet_pcb_s *inet_pcb;    /*!< shortcut to Internet control block */
    sockbuf_t   rcvq;
    sockbuf_t   sndq;
    ns_list_link_t link;            /*!< link */
} socket_t;

NS_STATIC_ASSERT(offsetof(socket_t, u.pending.link) == 0, "Listen queue link must be first (NS_LIST_HEAD_INCOMPLETE)")

typedef struct inet_group {
    uint8_t group_addr[16];
    int8_t interface_id;
    ns_list_link_t link;
} inet_group_t;

/** Internet protocol control block */
typedef struct inet_pcb_s {
    socket_t    *socket;
#ifndef NO_TCP
    void        *session;           /*!< Session for transport protocol - only used by TCP at present */
#endif
    uint8_t     local_address[16];  /*!< Local address */
    uint8_t     remote_address[16];    /*!< Destination address */
    uint16_t    local_port;         /*!< Local port */
    uint16_t    remote_port;           /*!< Destination port */
    int16_t     unicast_hop_limit;  /*!< May be -1, which will give per-interface default */
    uint8_t     multicast_hop_limit;
    int8_t      multicast_if;
    uint32_t    addr_preferences;
    uint8_t     protocol;           /*!< IP type IPV6_NH_TCP, IPV6_NH_UDP, IPV6_NH_ICMPV6... */
    int8_t      link_layer_security;
#ifndef NO_IPV6_PMTUD
    int8_t      use_min_mtu;        /*!< RFC 3542 socket option */
#endif
#ifndef NO_IP_FRAGMENT_TX
    int8_t      dontfrag;
#endif
    uint8_t     tclass;
    bool        multicast_loop: 1;
    bool        recvpktinfo: 1;
    bool        recvhoplimit: 1;
    bool        recvtclass: 1;
    int_least24_t flow_label;
    NS_LIST_HEAD(inet_group_t, link) mc_groups;
} inet_pcb_t;

typedef NS_LIST_HEAD(socket_t, link) socket_list_t;
extern socket_list_t socket_list;

extern const uint8_t ns_in6addr_any[16];

extern void socket_init(void);
extern int8_t socket_event_handler_id_get(void);
extern bool socket_data_queued_event_push(socket_t *socket);
extern void socket_event_push(uint8_t sock_event, socket_t *socket, int8_t interface_id, void *session_ptr, uint16_t length);
extern socket_error_t socket_create(socket_family_t family, socket_type_t type, uint8_t protocol, int8_t *sid, uint16_t port, void (*passed_fptr)(void *), bool buffer_type);
extern socket_t *socket_new_incoming_connection(socket_t *listen_socket);
void socket_connection_abandoned(socket_t *socket, int8_t interface_id, uint8_t reason);
void socket_connection_complete(socket_t *socket, int8_t interface_id);
extern void socket_leave_pending_state(socket_t *socket, void (*fptr)(void *));
void socket_cant_recv_more(socket_t *socket, int8_t interface_id);
extern int8_t socket_id_assign_and_attach(socket_t *socket);
extern void socket_id_detach(int8_t sid);
extern buffer_t *socket_buffer_read(socket_t *socket);
extern socket_t *socket_lookup(socket_family_t family, uint8_t protocol, const sockaddr_t *local_addr, const sockaddr_t *remote_addr);
extern socket_t *socket_lookup_ipv6(uint8_t protocol, const sockaddr_t *local_addr, const sockaddr_t *remote_addr, bool allow_wildcards);
extern socket_error_t socket_port_validate(uint16_t port, uint8_t protocol);
extern socket_error_t socket_up(buffer_t *buf);
extern bool socket_message_validate_iov(const struct ns_msghdr *msg, uint16_t *length_out);
extern int16_t socket_buffer_sendmsg(int8_t sid, buffer_t *buf, const struct ns_msghdr *msg, int flags);
extern socket_t *socket_pointer_get(int8_t socket);
extern void socket_inet_pcb_set_buffer_hop_limit(const inet_pcb_t *socket, buffer_t *buf, const int16_t *msg_hoplimit);
extern bool socket_validate_listen_backlog(const socket_t *socket_ptr);
extern void socket_list_print(route_print_fn_t *print_fn, char sep);
extern socket_t *socket_reference(socket_t *);
extern socket_t *socket_dereference(socket_t *);

extern void socket_release(socket_t *socket);

extern void socket_tx_buffer_event_and_free(buffer_t *buf, uint8_t status);
extern buffer_t *socket_tx_buffer_event(buffer_t *buf, uint8_t status);

extern inet_pcb_t *socket_inet_pcb_allocate(void);
extern inet_pcb_t *socket_inet_pcb_clone(const inet_pcb_t *orig);

/**
 * Free reserved inet_pcb
 */
extern inet_pcb_t *socket_inet_pcb_free(inet_pcb_t *inet_pcb);

extern int8_t socket_inet_pcb_join_group(inet_pcb_t *inet_pcb, int8_t interface_id, const uint8_t group[__static 16]);
extern int8_t socket_inet_pcb_leave_group(inet_pcb_t *inet_pcb, int8_t interface_id, const uint8_t group[__static 16]);

/**
 * Determine interface based on socket_id and address
 */
extern struct protocol_interface_info_entry *socket_interface_determine(const socket_t *socket, buffer_t *buf);

extern uint16_t socket_generate_random_port(uint8_t protocol);

#endif /*_NS_SOCKET_H*/
