/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#ifndef __SOCKET_API_STUB_H__
#define __SOCKET_API_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    void (*recv_cb)(void *);
    int8_t int8_value;
    int counter;
    int values[10];
} socket_api_stub_data_t;

extern socket_api_stub_data_t socket_api_stub;

#ifdef __cplusplus
}
#endif

#endif //__SOCKET_API_STUB_H__





