/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#ifndef __COAP_MESSAGE_HANDLER_STUB_H__
#define __COAP_MESSAGE_HANDLER_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "coap_message_handler.h"

typedef struct {
    int8_t int8_value;
    int16_t int16_value;
    uint16_t uint16_value;
    coap_msg_handler_t *coap_ptr;
    coap_transaction_t *coap_tx_ptr;
    int16_t (*cb)(int8_t, sn_coap_hdr_s *, coap_transaction_t *);
} coap_message_handler_stub_def;

extern coap_message_handler_stub_def coap_message_handler_stub;

#ifdef __cplusplus
}
#endif

#endif // __COAP_MESSAGE_HANDLER_STUB_H__
