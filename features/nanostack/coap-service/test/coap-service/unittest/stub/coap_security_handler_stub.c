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

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "coap_connection_handler.h"
#include "coap_security_handler_stub.h"

thread_sec_def coap_security_handler_stub;

struct coap_security_s {
    bool _is_started;
};

coap_security_t *coap_security_create(int8_t socket_id, int8_t timer_id, void *handle, SecureConnectionMode mode,
                                      int (*send_cb)(int8_t socket_id, void *handle, const void *, size_t),
                                      int (*receive_cb)(int8_t socket_id, unsigned char *, size_t),
                                      void (*start_timer_cb)(int8_t timer_id, uint32_t min, uint32_t fin),
                                      int (*timer_status_cb)(int8_t timer_id))
{
    coap_security_handler_stub.send_cb = send_cb;
    coap_security_handler_stub.receive_cb = receive_cb;
    coap_security_handler_stub.start_timer_cb = start_timer_cb;
    coap_security_handler_stub.timer_status_cb = timer_status_cb;
    return coap_security_handler_stub.sec_obj;
}

coap_security_t *coap_security_handler_stub_alloc(void)
{
    return calloc(1, sizeof(coap_security_t));
}


void coap_security_destroy(coap_security_t *sec)
{

}

int coap_security_handler_connect_non_blocking(coap_security_t *sec, bool is_server, SecureSocketMode sock_mode, coap_security_keys_t keys, uint32_t timeout_min, uint32_t timeout_max)
{
    sec->_is_started = true;
    if (coap_security_handler_stub.counter >= 0) {
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

int coap_security_handler_continue_connecting(coap_security_t *sec)
{
    if (coap_security_handler_stub.counter >= 0) {
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }

    return coap_security_handler_stub.int_value;
}


int coap_security_handler_send_message(coap_security_t *sec, unsigned char *message, size_t len)
{
    if (coap_security_handler_stub.counter >= 0) {
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

int coap_security_send_close_alert(coap_security_t *sec)
{
    if (coap_security_handler_stub.counter >= 0) {
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

int coap_security_handler_read(coap_security_t *sec, unsigned char *buffer, size_t len)
{
    if (coap_security_handler_stub.counter >= 0) {
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

bool coap_security_handler_is_started(const coap_security_t *sec)
{
    return sec->_is_started;
}

const void *coap_security_handler_keyblock(const coap_security_t *sec)
{
    return NULL;
}
