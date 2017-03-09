/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <configuration-store/configuration_store.h>
#include "configuration_store_stub.h"

configuration_store_stub_data_t cfstore_stub = {ARM_DRIVER_OK, ARM_DRIVER_OK, ARM_DRIVER_OK, ARM_DRIVER_OK, ARM_DRIVER_OK, 0, NULL, NULL, NULL};

static int32_t test_cfstore_close(ARM_CFSTORE_HANDLE hkey)
{
    return cfstore_stub.close_ret_val;
}

static int32_t test_cfstore_create(const char* key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    return cfstore_stub.ret_val;
}

static int32_t test_cfstore_delete(ARM_CFSTORE_HANDLE hkey)
{
    return cfstore_stub.delete_ret_val;
}

static int32_t test_cfstore_find(const char* key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    return ARM_DRIVER_OK;
}

static int32_t test_cfstore_flush(void)
{
    return cfstore_stub.ret_val;
}

ARM_CFSTORE_CAPABILITIES test_cfstore_get_capabilities(void)
{
    static ARM_CFSTORE_CAPABILITIES cfstore_caps_g = { .asynchronous_ops = 1, .uvisor_support_enabled = 0 };
    return cfstore_caps_g;
}

static int32_t test_cfstore_get_key_name(ARM_CFSTORE_HANDLE hkey, char* key_name, uint8_t *key_name_len)
{
    return ARM_DRIVER_OK;
}

static ARM_CFSTORE_STATUS test_cfstore_get_status(void)
{
    ARM_CFSTORE_STATUS status;
    return status;
}

static int32_t test_cfstore_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    return ARM_DRIVER_OK;
}

ARM_DRIVER_VERSION test_cfstore_get_version(void)
{
    static const ARM_DRIVER_VERSION cfstore_driver_version_g = { .api = ARM_CFSTORE_API_VERSION, .drv = ARM_CFSTORE_DRV_VERSION };
    return cfstore_driver_version_g;
}

static int32_t test_cfstore_initialise(ARM_CFSTORE_CALLBACK callback, void* client_context)
{
    cfstore_stub.callback = callback;
    cfstore_stub.client_context = client_context;
    return cfstore_stub.ret_val;
}

static int32_t test_cfstore_open(const char* key_name, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey)
{
    return cfstore_stub.ret_val;
}

static int32_t test_cfstore_power_control(ARM_POWER_STATE state)
{
    return ARM_DRIVER_OK;
}

static int32_t test_cfstore_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    return cfstore_stub.read_ret_val;
}

static int32_t test_cfstore_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    return ARM_DRIVER_OK;
}

static int32_t test_cfstore_uninitialise(void)
{
    return cfstore_stub.ret_val;
}

static int32_t test_cfstore_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    return cfstore_stub.write_ret_val;
}

ARM_CFSTORE_DRIVER cfstore_driver =
{
        .Close = test_cfstore_close,
        .Create = test_cfstore_create,
        .Delete= test_cfstore_delete,
        .Find = test_cfstore_find,
        .Flush = test_cfstore_flush,
        .GetCapabilities = test_cfstore_get_capabilities,
        .GetKeyName = test_cfstore_get_key_name,
        .GetStatus = test_cfstore_get_status,
        .GetValueLen = test_cfstore_get_value_len,
        .GetVersion = test_cfstore_get_version,
        .Initialize = test_cfstore_initialise,
        .Open = test_cfstore_open,
        .PowerControl = test_cfstore_power_control,
        .Read = test_cfstore_read,
        .Rseek = test_cfstore_rseek,
        .Uninitialize = test_cfstore_uninitialise,
        .Write = test_cfstore_write,
};


void test_cfstore_callback_trigger()
{
    cfstore_stub.callback(cfstore_stub.ret_val, cfstore_stub.cmd_code, cfstore_stub.client_context, cfstore_stub.handle);
}

