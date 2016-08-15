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
#ifndef __MBED_NET_SOCKET_ABSTRACT_SOCKET_TYPES_H__
#define __MBED_NET_SOCKET_ABSTRACT_SOCKET_TYPES_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MBED_NET_LWIP
#define MBED_NET_LWIP 1
#endif

typedef enum {
    SOCKET_ERROR_NONE = 0,
    SOCKET_ERROR_UNKNOWN,
    SOCKET_ERROR_UNIMPLEMENTED,
    SOCKET_ERROR_BUSY,
    SOCKET_ERROR_NULL_PTR,
    SOCKET_ERROR_BAD_FAMILY,
    SOCKET_ERROR_TIMEOUT,
    SOCKET_ERROR_BAD_ALLOC,
    SOCKET_ERROR_NO_CONNECTION,
    SOCKET_ERROR_SIZE,
    SOCKET_ERROR_STACK_EXISTS,
    SOCKET_ERROR_STACKS,
    SOCKET_ERROR_BAD_STACK,
    SOCKET_ERROR_BAD_ADDRESS,
    SOCKET_ERROR_DNS_FAILED,
    SOCKET_ERROR_WOULD_BLOCK,
    SOCKET_ERROR_CLOSED,
    SOCKET_ERROR_VALUE,
    SOCKET_ERROR_ADDRESS_IN_USE,
    SOCKET_ERROR_ALREADY_CONNECTED,
    SOCKET_ERROR_ABORT,
    SOCKET_ERROR_RESET,
    SOCKET_ERROR_BAD_ARGUMENT,
    SOCKET_ERROR_INTERFACE_ERROR,
    SOCKET_ERROR_API_VERSION,
    SOCKET_ERROR_NOT_BOUND,

} socket_error_t;

typedef enum {
    SOCKET_ALLOC_HEAP = 0,
    SOCKET_ALLOC_POOL_BEST,
} socket_alloc_pool_t;

typedef enum {
    SOCKET_AF_UNINIT,
    SOCKET_AF_INET4,
    SOCKET_AF_INET6,
    SOCKET_AF_MAX,
} socket_address_family_t;

typedef enum {
    SOCKET_PROTO_UNINIT = 0,
    SOCKET_DGRAM,
    SOCKET_STREAM,
    SOCKET_PROTO_MAX,
} socket_proto_family_t;

typedef enum {
    SOCKET_EVENT_NONE = 0,
    SOCKET_EVENT_ERROR,
    SOCKET_EVENT_RX_DONE,
    SOCKET_EVENT_TX_DONE,
    SOCKET_EVENT_RX_ERROR,
    SOCKET_EVENT_TX_ERROR,
    SOCKET_EVENT_CONNECT,
    SOCKET_EVENT_DISCONNECT,
    SOCKET_EVENT_DNS,
    SOCKET_EVENT_ACCEPT,
} event_flag_t;

typedef enum {
    SOCKET_STATUS_IDLE      = 0,
    SOCKET_STATUS_RX_BUSY   = 1 << 0,
    SOCKET_STATUS_TX_BUSY   = 1 << 1,
    SOCKET_STATUS_CONNECTED = 1 << 2,
    SOCKET_STATUS_BOUND     = 1 << 3,
} socket_status_t;

typedef enum {
    SOCKET_STACK_UNINIT = 0,
    SOCKET_STACK_LWIP_IPV4,
    SOCKET_STACK_LWIP_IPV6,
    SOCKET_STACK_RESERVED,
    SOCKET_STACK_NANOSTACK_IPV6,
    SOCKET_STACK_PICOTCP,
    SOCKET_STACK_MAX,
} socket_stack_t;

typedef enum {
    SOCKET_BUFFER_UNINITIALISED = 0,
    SOCKET_BUFFER_RAW,
    SOCKET_BUFFER_LWIP_PBUF,
    SOCKET_BUFFER_NANOSTACK_PBUF,
} socket_buffer_type_t;

struct socket_addr {
    uint32_t ipv6be[4];
};

struct socket_buffer {
    socket_buffer_type_t type;
    const struct socket_buf_api *api;
    void *impl;
};

struct socket_rawbuf {
    void * buf;
    size_t size;
    size_t pos;
};

typedef void (*socket_api_handler_t)(void);
typedef struct {
    void * (*alloc)(void *,const size_t);
    void (*dealloc)(void *, void *);
    void *context;
} socket_allocator_t;

struct socket_tx_info {
    uint16_t sentbytes;
};

struct socket_dns_info {
  struct socket_addr addr; // A stack-specific socket address struct
  const char *domain;
};

struct socket_accept_info {
    void * newimpl;
    uint8_t reject;
};

struct socket_event {
    event_flag_t event;
    struct socket *sock;
    union {
        struct socket_tx_info t;
        socket_error_t e;
        struct socket_dns_info d;
        struct socket_accept_info a;
    } i;
};
typedef struct socket_event socket_event_t;

struct socket {
    socket_api_handler_t handler;
    socket_event_t *event; // TODO: (CThunk upgrade/Alpha3)
    const struct socket_api *api;
    void *impl;
    socket_status_t status;
    uint8_t family;
    socket_stack_t stack;
    void *rxBufChain; // TODO: Only required in some stacks, which do not support BSD-style buffering
};

#ifdef __cplusplus
}
#endif
#endif /* __MBED_NET_SOCKET_ABSTRACT_SOCKET_TYPES_H__ */
