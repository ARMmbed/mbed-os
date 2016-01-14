/*
 * Copyright (c) 2015-2016 ARM Limited. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __COAP_SECURITY_HANDLER_H__
#define __COAP_SECURITY_HANDLER_H__

#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>
#include "mbedtls/ssl.h"
#include "mbedtls/sha256.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#define COOKIE_SIMPLE_LEN 8
typedef struct simple_cookie {
    unsigned char value[COOKIE_SIMPLE_LEN];
    size_t        len;
} simple_cookie_t;

#define KEY_BLOCK_LEN 40
typedef struct key_block {
    unsigned char value[KEY_BLOCK_LEN];
} key_block_t;

typedef int send_cb(int8_t socket_id, uint8_t *address_ptr, uint16_t port, const unsigned char *, size_t);
typedef int receive_cb(int8_t socket_id, unsigned char *, size_t);
typedef void start_timer_cb(int8_t timer_id, uint32_t min, uint32_t fin);
typedef int timer_status_cb(int8_t timer_id);

typedef struct thread_security_s {
    mbedtls_ssl_config          _conf;
    mbedtls_ssl_context         _ssl;

    mbedtls_ctr_drbg_context    _ctr_drbg;
    mbedtls_entropy_context     _entropy;
    bool                        _is_started;
    simple_cookie_t             _cookie;
    key_block_t                 _keyblk;

    uint8_t                     _remote_address[16];
    uint16_t                    _remote_port;

    uint8_t                     _pw[64];
    uint8_t                     _pw_len;

    int8_t                      _socket_id;
    int8_t                      _timer_id;
    send_cb                     *_send_cb;
    receive_cb                  *_receive_cb;
    start_timer_cb              *_start_timer_cb;
    timer_status_cb             *_timer_status_cb;

} coap_security_t;

coap_security_t *thread_security_create(int8_t socket_id, int8_t timer_id, uint8_t *address_ptr, uint16_t port,
                                          send_cb *send_cb,
                                          receive_cb *receive_cb,
                                          start_timer_cb *start_timer_cb,
                                          timer_status_cb *timer_status_cb);

void thread_security_destroy(coap_security_t *sec);

int coap_security_handler_connect(coap_security_t *sec, bool is_server, const unsigned char *pw, uint8_t len);

int coap_security_handler_continue_connecting(coap_security_t *sec);

int coap_security_handler_send_message(coap_security_t *sec, unsigned char *message, size_t len);

int thread_security_send_close_alert(coap_security_t *sec);

int coap_security_handler_read(coap_security_t *sec, unsigned char* buffer, size_t len);

#endif
