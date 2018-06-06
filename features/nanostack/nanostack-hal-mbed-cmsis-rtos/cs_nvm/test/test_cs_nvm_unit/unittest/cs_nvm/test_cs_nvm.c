/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include <inttypes.h>
#include <configuration-store/configuration_store.h>
#include "platform/arm_hal_nvm.h"

#include "test_cs_nvm.h"
#include "nsdynmemLIB_stub.h"
#include "configuration_store_stub.h"

#define TEST_NS_NVM_HELPER_CONTEXT_CLEAR 0xffff
#define TEST_PLATFORM_NVM_STATUS_CLEAR 0xffff

#define TEST_NS_NVM_HELPER_CONTEXT1 0x01
#define TEST_NS_NVM_HELPER_CONTEXT2 0x02
#define TEST_NS_NVM_HELPER_CONTEXT3 0x03

const char *key_name = "testkey";
uint8_t buf[100];
uint16_t buf_len;
uint16_t data_len;

typedef struct 
{
    platform_nvm_status status;
    void *ctx;
} test_platform_nvm_api_callback_t;

static test_platform_nvm_api_callback_t *active_cb_data_ptr;
static test_platform_nvm_api_callback_t cb_data = {TEST_PLATFORM_NVM_STATUS_CLEAR, TEST_NS_NVM_HELPER_CONTEXT_CLEAR};

void test_nvm_callback(platform_nvm_status status, void *context)
{
    active_cb_data_ptr->status = status;
    active_cb_data_ptr->ctx = context;
}

static void test_callback_data_clear(test_platform_nvm_api_callback_t *callback_data_ptr)
{
    callback_data_ptr->status = TEST_PLATFORM_NVM_STATUS_CLEAR;
    callback_data_ptr->ctx = TEST_NS_NVM_HELPER_CONTEXT_CLEAR;
}

static bool test_callback_executed(test_platform_nvm_api_callback_t *callback_data_ptr)
{
    if (callback_data_ptr->status != TEST_PLATFORM_NVM_STATUS_CLEAR) {
        return true;
    }
    if (callback_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT_CLEAR) {
        return true;
    }
    return false;
}

static bool test_nvm_initialize()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);

    // OK initialization
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.cmd_code = CFSTORE_OPCODE_INITIALIZE;
    nsdynmemlib_stub.returnCounter = 1;
    ret = platform_nvm_init(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_POWER_CONTROL;
    test_cfstore_callback_trigger();

    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_INITIALIZE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    return true;
}

static bool test_nvm_finalize()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);

    // finalize NVM - OK
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    ret = platform_nvm_finalize(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_POWER_CONTROL;
    test_cfstore_callback_trigger();

    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_UNINITIALIZE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }
    return true;
}

bool test_cs_nvm_init_finalize()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);

    // No callback - fail
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.cmd_code = CFSTORE_OPCODE_INITIALIZE;
    ret = platform_nvm_init(NULL, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // memory allocation fails
    nsdynmemlib_stub.returnCounter = 0;
    ret = platform_nvm_init(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // configuration-store returns failure
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    nsdynmemlib_stub.returnCounter = 1;
    ret = platform_nvm_init(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // OK initialization
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    nsdynmemlib_stub.returnCounter = 1;
    ret = platform_nvm_init(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_POWER_CONTROL;
    test_cfstore_callback_trigger();

    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_INITIALIZE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    // Already initialized
    ret = platform_nvm_init(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // finalize NVM - no callback
    ret = platform_nvm_finalize(NULL, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // finalize NVM - configuration-store error
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_finalize(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    cfstore_stub.cmd_code = CFSTORE_OPCODE_UNINITIALIZE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    // finalize NVM - OK
    if (!test_nvm_finalize()){
        return false;
    }

    // finalize - already finalized
    ret = platform_nvm_finalize(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    return true;
}

bool test_cs_nvm_key_create()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);

    // OK initialization
    if (!test_nvm_initialize()) {
        return false;
    }

    // Test - no callback
    ret = platform_nvm_key_create(NULL, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // No key_name
    ret = platform_nvm_key_create(test_nvm_callback, NULL, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // configuration-store returns error
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    // simulate configuration_store callback and cs_nvm timer callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CREATE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    //OK key creation
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // create callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CREATE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();

    // close callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    //OK key creation - close failing
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // create callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CREATE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();

    // close callback
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    // OK key re-creation failure
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY;
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    cfstore_stub.cmd_code = CFSTORE_OPCODE_CREATE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    // finalize NVM - OK
    if (!test_nvm_finalize()){
        return false;
    }

    // Create - not initialized
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    return true;
}

bool test_cs_nvm_key_delete()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);

    // NVM not initialized
    ret = platform_nvm_key_delete(test_nvm_callback, key_name, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    if (!test_nvm_initialize()) {
        return false;
    }

    test_callback_data_clear(active_cb_data_ptr);

    // No callback
    ret = platform_nvm_key_delete(NULL, key_name, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // No key_name
    ret = platform_nvm_key_delete(test_nvm_callback, NULL, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }   

    // Configuration-store returns error in OPEN directly
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_key_delete(test_nvm_callback, key_name, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    // simulate failing configuration store
    test_callback_data_clear(active_cb_data_ptr);
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    // Configuration-store returns error in DELETE
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.delete_ret_val = ARM_DRIVER_ERROR;
    test_callback_data_clear(active_cb_data_ptr);
    ret = platform_nvm_key_delete(test_nvm_callback, key_name, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    // OK delete
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.delete_ret_val = ARM_DRIVER_OK;
    ret = platform_nvm_key_delete(test_nvm_callback, key_name, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }      

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate delete
    cfstore_stub.cmd_code = CFSTORE_OPCODE_DELETE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect since state is deleting
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_key_delete_failure_in_close()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // Delete is OK but closing fails
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.read_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_ERROR;
    test_callback_data_clear(active_cb_data_ptr);
    ret = platform_nvm_key_delete(test_nvm_callback, key_name, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate delete
    cfstore_stub.cmd_code = CFSTORE_OPCODE_DELETE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_read()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.read_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;

    // read uninitialized
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // no callback
    ret = platform_nvm_read(NULL, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // no key_name
    ret = platform_nvm_read(test_nvm_callback, NULL, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // no buf
    ret = platform_nvm_read(test_nvm_callback, key_name, NULL, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // no buf_len
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, NULL, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // OK
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate read
    cfstore_stub.cmd_code = CFSTORE_OPCODE_READ;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect since state is deleting
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    test_callback_data_clear(active_cb_data_ptr);
    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_read_failure()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // Open fails
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    // Read fails
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.read_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;
    test_callback_data_clear(active_cb_data_ptr);
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    cfstore_stub.read_ret_val = ARM_DRIVER_ERROR;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate read
    cfstore_stub.cmd_code = CFSTORE_OPCODE_READ;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect since state is deleting
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_read_failure_in_close()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // Open fails
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    // Read is OK but closing fails
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.read_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_ERROR;
    test_callback_data_clear(active_cb_data_ptr);
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate read
    cfstore_stub.cmd_code = CFSTORE_OPCODE_READ;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_write()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.read_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;

    // write uninitialized
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // no callback
    ret = platform_nvm_write(NULL, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // no key_name
    ret = platform_nvm_write(test_nvm_callback, NULL, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // no buf
    ret = platform_nvm_write(test_nvm_callback, key_name, NULL, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // no buf_len
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, NULL, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // OK
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate write
    cfstore_stub.cmd_code = CFSTORE_OPCODE_WRITE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect since state is deleting
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    test_callback_data_clear(active_cb_data_ptr);
    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_write_failure()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // write fails on open
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    cfstore_stub.write_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    // write fails on write and also close fails
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.write_ret_val = ARM_DRIVER_ERROR;
    cfstore_stub.close_ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    // simulate open, write - fails and close fails
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    test_callback_data_clear(active_cb_data_ptr);
    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_write_failure_in_close()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // write is OK but closing fails
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.write_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_ERROR;
    test_callback_data_clear(active_cb_data_ptr);
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // simulate open
    cfstore_stub.cmd_code = CFSTORE_OPCODE_OPEN;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    // simulate write
    cfstore_stub.cmd_code = CFSTORE_OPCODE_WRITE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();

    // simulate close
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CLOSE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();  //trigger callback

    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_flush()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.write_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;

    // flush with NVM uninitialized
    ret = platform_nvm_flush(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    if (!test_nvm_initialize()) {
        return false;
    }
    test_callback_data_clear(active_cb_data_ptr);

    // no callback
    ret = platform_nvm_flush(NULL, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // flush failure
    cfstore_stub.ret_val = ARM_DRIVER_ERROR;
    ret = platform_nvm_flush(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    // simulate flush
    cfstore_stub.cmd_code = CFSTORE_OPCODE_FLUSH;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (active_cb_data_ptr->status != PLATFORM_NVM_ERROR || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    // OK flush
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    ret = platform_nvm_flush(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }
    // simulate flush
    cfstore_stub.cmd_code = CFSTORE_OPCODE_FLUSH;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger(); // should not have any effect as state is opening
    if (active_cb_data_ptr->status != PLATFORM_NVM_OK || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    test_callback_data_clear(active_cb_data_ptr);
    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

bool test_cs_nvm_operation_in_progress()
{
    platform_nvm_status ret;
    active_cb_data_ptr = &cb_data;
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_DRIVER_OK;
    cfstore_stub.write_ret_val = ARM_DRIVER_OK;
    cfstore_stub.close_ret_val = ARM_DRIVER_OK;


    if (!test_nvm_initialize()) {
        return false;
    }

    // Make key
    test_callback_data_clear(active_cb_data_ptr);
    cfstore_stub.ret_val = ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_OK) {
        return false;
    }

    // make new requests while key creation is in progress
    ret = platform_nvm_init(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }
    ret = platform_nvm_key_create(test_nvm_callback, key_name, data_len, 0, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }
    ret = platform_nvm_key_delete(test_nvm_callback, key_name, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }
    ret = platform_nvm_read(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }
    ret = platform_nvm_write(test_nvm_callback, key_name, buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }
    ret = platform_nvm_flush(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }
    ret = platform_nvm_finalize(test_nvm_callback, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret != PLATFORM_NVM_ERROR) {
        return false;
    }

    // continue with key creation
    cfstore_stub.cmd_code = CFSTORE_OPCODE_CREATE;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (active_cb_data_ptr->status != PLATFORM_NVM_KEY_NOT_FOUND || active_cb_data_ptr->ctx != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    // test2, check that other cmd_codes are not handled/affecting
    cfstore_stub.cmd_code = CFSTORE_OPCODE_MAX;
    test_callback_data_clear(active_cb_data_ptr);
    test_cfstore_callback_trigger();
    test_eventOS_timeout_trigger();
    if (test_callback_executed(active_cb_data_ptr)) {
        return false;
    }

    if (!test_nvm_finalize()) {
        return false;
    }

    return true;
}

