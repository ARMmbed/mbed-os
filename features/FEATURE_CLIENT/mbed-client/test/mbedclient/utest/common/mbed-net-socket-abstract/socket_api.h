/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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
#ifndef __MBED_NET_SOCKET_ABSTRACT_SOCKET_API_H__
#define __MBED_NET_SOCKET_ABSTRACT_SOCKET_API_H__

#include <stdint.h>
#include "socket_types.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOCKET_MAX_STACKS
#define SOCKET_MAX_STACKS 2
#endif

typedef socket_error_t (*socket_init)();
typedef socket_error_t (*socket_create)(struct socket *socket,
        const socket_address_family_t af, const socket_proto_family_t pf,
        socket_api_handler_t const handler);
typedef socket_error_t (*socket_destroy)(struct socket *socket);
typedef socket_error_t (*socket_close)(struct socket *socket);
typedef socket_api_handler_t (*socket_periodic_task)(const struct socket * socket);
typedef uint32_t (*socket_periodic_interval)(const struct socket * socket);
typedef socket_error_t (*socket_resolve)(struct socket *socket, const char *address);
typedef socket_error_t (*socket_connect)(struct socket *sock, const struct socket_addr *address, const uint16_t port);
typedef socket_error_t (*socket_bind)(struct socket *socket, const struct socket_addr *address, const uint16_t port);
typedef socket_error_t (*socket_str2addr)(const struct socket *socket, struct socket_addr *addr, const char *address);
typedef socket_error_t (*socket_start_listen)(struct socket *socket, const uint32_t backlog);
typedef socket_error_t (*socket_stop_listen)(struct socket *socket);
typedef socket_error_t (*socket_accept)(struct socket *sock, socket_api_handler_t handler);
typedef socket_error_t (*socket_reject)(struct socket *sock);
typedef socket_error_t (*socket_send)(struct socket *socket, const void * buf, const size_t len);
typedef socket_error_t (*socket_send_to)(struct socket *socket, const void * buf, const size_t len, const struct socket_addr *addr, const uint16_t port);
typedef socket_error_t (*socket_recv)(struct socket *socket, void * buf, size_t *len);
typedef socket_error_t (*socket_recv_from)(struct socket *socket, void * buf, size_t *len, struct socket_addr *addr, uint16_t *port);

typedef socket_error_t (*socket_get_local_addr)(const struct socket *socket, struct socket_addr *addr);
typedef socket_error_t (*socket_get_remote_addr)(const struct socket *socket, struct socket_addr *addr);
typedef socket_error_t (*socket_get_local_port)(const struct socket *socket, uint16_t *port);
typedef socket_error_t (*socket_get_remote_port)(const struct socket *socket, uint16_t *port);

typedef uint8_t (*socket_is_connected)(const struct socket *socket);
typedef uint8_t (*socket_is_bound)(const struct socket *socket);

struct socket_api {
    socket_stack_t              stack;
    uint32_t                    version;
    socket_init                 init;
    socket_create               create;
    socket_destroy              destroy;
    socket_close                close;
    socket_periodic_task        periodic_task;
    socket_periodic_interval    periodic_interval;
    socket_resolve              resolve;
    socket_connect              connect;
    socket_str2addr             str2addr;
    socket_bind                 bind;
    socket_start_listen         start_listen;
    socket_stop_listen          stop_listen;
    socket_accept               accept;
    socket_reject               reject;
    socket_send                 send;
    socket_send_to              send_to;
    socket_recv                 recv;
    socket_recv_from            recv_from;
    socket_is_connected         is_connected;
    socket_is_bound             is_bound;
    socket_get_local_addr       get_local_addr;
    socket_get_remote_addr      get_remote_addr;
    socket_get_local_port       get_local_port;
    socket_get_remote_port      get_remote_port;
};
#define SOCKET_API_FIRST_PTR init
#define SOCKET_API_LAST_PTR get_remote_port

socket_error_t socket_register_stack(const struct socket_api * api);
const char * socket_strerror(const socket_error_t err);
const struct socket_api * socket_get_api(const socket_stack_t stack);

static inline int socket_addr_is_ipv4(const struct socket_addr *addr)
{
    if ((addr->ipv6be[0] == 0) &&
        (addr->ipv6be[1] == 0) &&
        (addr->ipv6be[2] == 0)) {
            return 1;
    } else {
        return 0;
    }
}
static inline int socket_addr_is_any(const struct socket_addr *addr)
{
    if ((addr->ipv6be[0] == 0) &&
        (addr->ipv6be[1] == 0) &&
        (addr->ipv6be[2] == 0) &&
        (addr->ipv6be[3] == 0)) {
            return 1;
    } else {
        return 0;
    }
}
static inline void socket_addr_set_ipv4_addr(struct socket_addr *addr, uint32_t ipv4addr)
{
    addr->ipv6be[0] = 0;
    addr->ipv6be[1] = 0;
    addr->ipv6be[2] = 0;
    addr->ipv6be[3] = ipv4addr;
}
static inline uint32_t socket_addr_get_ipv4_addr(const struct socket_addr *addr)
{
    if (!socket_addr_is_ipv4(addr)) {
        return 0;
    }
    return addr->ipv6be[3];
}
static inline const uint32_t * socket_addr_get_ipv4_addrp(const struct socket_addr *addr)
{
    if (!socket_addr_is_ipv4(addr)) {
        return NULL;
    }
    return &addr->ipv6be[3];
}
static inline void socket_addr_set_any(struct socket_addr *addr)
{
    addr->ipv6be[0] = 0;
    addr->ipv6be[1] = 0;
    addr->ipv6be[2] = 0;
    addr->ipv6be[3] = 0;
}

static inline void socket_addr_copy(struct socket_addr *dest, const struct socket_addr *src)
{
    (dest)->ipv6be[0] = (src)->ipv6be[0];
    (dest)->ipv6be[1] = (src)->ipv6be[1];
    (dest)->ipv6be[2] = (src)->ipv6be[2];
    (dest)->ipv6be[3] = (src)->ipv6be[3];
}
static inline int socket_addr_cmp(const struct socket_addr *a, const struct socket_addr *b)
{
    return
        a->ipv6be[0] != b->ipv6be[0] ||
        a->ipv6be[1] != b->ipv6be[1] ||
        a->ipv6be[2] != b->ipv6be[2] ||
        a->ipv6be[3] != b->ipv6be[3];
}

#ifdef __cplusplus
}
#endif

#endif /* __MBED_NET_SOCKET_ABSTRACT_SOCKET_API_H__ */
