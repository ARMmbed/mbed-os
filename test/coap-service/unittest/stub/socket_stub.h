/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#ifndef __SOCKET_STUB_H__
#define __SOCKET_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    uint8_t socket_id;  // socket id returned when socket created
    error_t err;        // socket error returned by the socket call
} socket_stub_data_t;

socket_stub_data_t *socket_stub_get_test_data(void);

#ifdef __cplusplus
}
#endif

#endif //__SOCKET_STUB_H__
