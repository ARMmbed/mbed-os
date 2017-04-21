/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */
#ifndef __CONFIGURATION_STORE_STUB_H__
#define __CONFIGURATION_STORE_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    int32_t ret_val;
    int32_t delete_ret_val;
    int32_t read_ret_val;
    int32_t close_ret_val;
    int32_t write_ret_val;
    ARM_CFSTORE_OPCODE cmd_code;
    void *client_context;
    ARM_CFSTORE_HANDLE handle;
    ARM_CFSTORE_CALLBACK callback;
} configuration_store_stub_data_t;

extern configuration_store_stub_data_t cfstore_stub;

void test_cfstore_callback_trigger(void);

#ifdef __cplusplus
}
#endif

#endif
