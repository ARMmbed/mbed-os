/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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
#include "randLIB.h"
#include "coap_security_handler_stub.h"

thread_sec_def coap_security_handler_stub;

thread_security_t *thread_security_create(int8_t socket_id, int8_t timer_id, uint8_t *address_ptr, uint16_t port,
                                          int (*send_cb)(int8_t socket_id, uint8_t *address_ptr, uint16_t port, const unsigned char *, size_t),
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

void thread_security_destroy(thread_security_t *sec)
{

}

int coap_security_handler_connect(thread_security_t *sec, bool is_server, const unsigned char *pw, uint8_t len)
{
    sec->_is_started = true;
    if( coap_security_handler_stub.counter >= 0){
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

int coap_security_handler_continue_connecting(thread_security_t *sec)
{
    if( coap_security_handler_stub.counter >= 0){
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }

    return coap_security_handler_stub.int_value;
}


int coap_security_handler_send_message(thread_security_t *sec, unsigned char *message, size_t len)
{
    if( coap_security_handler_stub.counter >= 0){
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

int thread_security_send_close_alert(thread_security_t *sec)
{
    if( coap_security_handler_stub.counter >= 0){
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}

int coap_security_handler_read(thread_security_t *sec, unsigned char* buffer, size_t len)
{
    if( coap_security_handler_stub.counter >= 0){
        return coap_security_handler_stub.values[coap_security_handler_stub.counter--];
    }
    return coap_security_handler_stub.int_value;
}
