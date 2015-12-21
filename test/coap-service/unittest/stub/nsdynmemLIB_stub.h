/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#ifndef __NSDYNMEMLIB_STUB_H__
#define __NSDYNMEMLIB_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    uint8_t returnCounter;
    void* expectedPointer;
} nsdynmemlib_stub_data_t;

extern nsdynmemlib_stub_data_t nsdynmemlib_stub;


void *ns_dyn_mem_alloc(int16_t alloc_size);
void *ns_dyn_mem_temporary_alloc(int16_t alloc_size);
void ns_dyn_mem_free(void *block);

#ifdef __cplusplus
}
#endif

#endif
