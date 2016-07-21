/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved.
 */

#include "ns_types.h"
#include "platform/arm_hal_nvm.h"

typedef struct {
    platform_nvm_status status;
    nvm_callback *saved_callback;
    void *saved_context;
} test_platform_nvm_api_data;

test_platform_nvm_api_data test_data = {0, NULL, NULL};


void test_platform_nvm_api_set_retval(platform_nvm_status return_value)
{
    test_data.status = return_value;
}

void test_platform_nvm_api_callback()
{
    test_data.saved_callback(test_data.status, test_data.saved_context);
}

platform_nvm_status platform_nvm_init(nvm_callback *callback, void *context)
{
    test_data.saved_callback = callback;
    test_data.saved_context = context;
    return test_data.status;
}

platform_nvm_status platform_nvm_finalize(nvm_callback *callback, void *context)
{
    (void) callback;
    (void) context;
    return test_data.status;
}

platform_nvm_status platform_nvm_key_create(nvm_callback *callback, const char *key_name, uint16_t value_len, uint32_t flags, void *context)
{
    (void) key_name;
    (void) value_len;
    (void) flags;
    test_data.saved_callback = callback;
    test_data.saved_context = context;
    return test_data.status;
}

platform_nvm_status platform_nvm_key_delete(nvm_callback *callback, const char *key_name, void *context)
{
    (void) callback;
    (void) key_name;
    (void) context;
    test_data.saved_callback = callback;
    test_data.saved_context = context;
    return test_data.status;
}

platform_nvm_status platform_nvm_write(nvm_callback *callback, const char *key_name, const void *data, uint16_t *data_len, void *context)
{
    (void) key_name;
    (void) data;
    (void) data_len;
    test_data.saved_callback = callback;
    test_data.saved_context = context;
    return test_data.status;
}

platform_nvm_status platform_nvm_read(nvm_callback *callback, const char *key_name, void *buf, uint16_t *buf_len, void *context)
{
    (void) key_name;
    (void) buf;
    (void) buf_len;
    test_data.saved_callback = callback;
    test_data.saved_context = context;
    return test_data.status;
}

platform_nvm_status platform_nvm_flush(nvm_callback *callback, void *context)
{
    (void) callback;
    (void) context;
    test_data.saved_callback = callback;
    test_data.saved_context = context;
    return test_data.status;
}

