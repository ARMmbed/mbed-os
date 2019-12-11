/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef WICED_NETWORK_H
#define WICED_NETWORK_H
#include "w_dtls_types.h"
#include "wiced_result.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

#define IP_HANDLE(interface)   (wiced_ip_handle[(interface) & 3])

/******************************************************
 *                    Constants
 ******************************************************/

#define WICED_MAXIMUM_NUMBER_OF_SOCKETS_WITH_CALLBACKS    (1)
#define WICED_MAXIMUM_NUMBER_OF_SERVER_SOCKETS            (1)

#define SIZE_OF_ARP_ENTRY           sizeof(1)

#define IP_STACK_SIZE               (2 * 1024)
#define ARP_CACHE_SIZE              (6 * SIZE_OF_ARP_ENTRY)
#define DHCP_STACK_SIZE             (1024)
#define WICED_PKT_SIZE              (1500)
#define WICED_ANY_PORT              (0)

/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum {
    WICED_SOCKET_ERROR
} wiced_socket_state_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef struct {
    uint8_t *payload;
    uint16_t len;
    uint16_t pktstart;
} wiced_packet_t;


//typedef NOOS_DUMMY wiced_tls_context_type_t;
//typedef NOOS_DUMMY wiced_tls_context_t;
//typedef NOOS_DUMMY wiced_tls_session_t;
//typedef NOOS_DUMMY wiced_tls_certificate_t;
//typedef NOOS_DUMMY  wiced_tls_endpoint_type_t;
typedef void *NOOS_TCP_SOCKET;

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct wiced_tcp_socket_struct wiced_tcp_socket_t;
typedef struct wiced_udp_socket_struct wiced_udp_socket_t;

typedef wiced_result_t (*wiced_tcp_socket_callback_t)(wiced_tcp_socket_t *socket, void *arg);
typedef wiced_result_t (*wiced_udp_socket_callback_t)(wiced_udp_socket_t *socket, void *arg);

struct wiced_udp_socket_struct {
    wiced_dtls_context_t       *dtls_context;
    struct {
        wiced_tcp_socket_callback_t disconnect;
        wiced_tcp_socket_callback_t receive;
        wiced_tcp_socket_callback_t connect;
    } callbacks;
    void                *callback_arg;
};

struct wiced_tcp_socket_struct {
    NOOS_TCP_SOCKET             socket;
    wiced_tls_context_t        *tls_context;
    wiced_bool_t                context_malloced;
    struct {
        wiced_tcp_socket_callback_t disconnect;
        wiced_tcp_socket_callback_t receive;
        wiced_tcp_socket_callback_t connect;
    } callbacks;
    void                *callback_arg;
};

typedef struct {
    wiced_tcp_socket_t  socket[WICED_MAXIMUM_NUMBER_OF_SERVER_SOCKETS];
    int                 interface;
    uint16_t            port;
    wiced_tls_identity_t *tls_identity;
} wiced_tcp_server_t;

/******************************************************
 *                 Global Variables
 ******************************************************/
typedef struct {
    int dummy;
} NOOS_IP;
typedef struct {
    int dummy;
} NOOS_PACKET_POOL;
/*
 * Note: These objects are for internal use only!
 */
extern NOOS_IP          wiced_ip_handle       [3];
extern NOOS_PACKET_POOL wiced_packet_pools    [2]; /* 0=TX, 1=RX */

/******************************************************
 *               Function Declarations
 ******************************************************/


#ifdef __cplusplus
} /*extern "C" */
#endif
#endif
