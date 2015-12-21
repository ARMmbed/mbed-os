/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#ifndef __COAP_SECURITY_HANDLER_STUB_H__
#define __COAP_SECURITY_HANDLER_STUB_H__

#include <inttypes.h>
#include "coap_security_handler.h"

typedef struct tsh{
    thread_security_t *sec_obj;
    int int_value;
    int counter;
    int values[10];

    int (*send_cb)(int8_t socket_id, uint8_t *address_ptr, uint16_t port, const unsigned char *, size_t);
    int (*receive_cb)(int8_t socket_id, unsigned char *, size_t);
    void (*start_timer_cb)(int8_t timer_id, uint32_t min, uint32_t fin);
    int (*timer_status_cb)(int8_t timer_id);
} thread_sec_def;

extern thread_sec_def coap_security_handler_stub;

#endif //__COAP_SECURITY_HANDLER_STUB_H__
