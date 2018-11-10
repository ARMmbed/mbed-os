/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include <inttypes.h>
#include "test_ns_nvm_helper.h"
#include "ns_nvm_helper.h"
#include "nsdynmemLIB_stub.h"
#include "platform/arm_hal_nvm.h"

#define TEST_NS_NVM_HELPER_CONTEXT1 0x01
#define TEST_NS_NVM_HELPER_CONTEXT2 0x02
#define TEST_NS_NVM_HELPER_CONTEXT3 0x03

const char *key1 = "ns_nvm_test_key1";
#define BUF_LEN 1000
static uint8_t buf[BUF_LEN];
uint16_t buf_len = BUF_LEN;

static int read_callback_status = 0;
static void *read_callback_context = NULL;
static int write_callback_status = 0;
static void *write_callback_context = NULL;
static int delete_callback_status = 0;
static void *delete_callback_context = NULL;

extern void test_platform_nvm_api_set_retval(platform_nvm_status return_value);

void test_ns_nvm_helper_write_callback(int status, void *context)
{
    write_callback_status = status;
    write_callback_context = context;
}

void test_ns_nvm_helper_read_callback(int status, void *context)
{
    read_callback_status = status;
    read_callback_context = context;
}

void test_ns_nvm_helper_delete_callback(int status, void *context)
{
    delete_callback_status = status;
    delete_callback_context = context;
}

bool test_ns_nvm_helper_write()
{
    int ret_val;

    write_callback_status = -1;
    write_callback_context = 0;

    // test with invalid parameters - callback NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_write(NULL, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - key NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, NULL, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - buf NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, NULL, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - buf_len NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, NULL, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with valid parameters - memory allocation fails 1
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 0;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_MEMORY) {
        return false;
    }

    // test with valid parameters - memory allocation fails 2
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_MEMORY) {
        return false;
    }

    // test with valid parameters - platform_init fails
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    nsdynmemlib_stub.returnCounter = 2;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_ERROR) {
        return false;
    }

    // test with valid parameters - OK
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 2;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // make initialize callback
    test_platform_nvm_api_callback();

    // make create callback
    test_platform_nvm_api_callback();

    // make write callback
    test_platform_nvm_api_callback();

    // make flush callback
    test_platform_nvm_api_callback();

    if (write_callback_status != NS_NVM_OK || write_callback_context != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    return true;
}

bool test_ns_nvm_helper_read()
{
    int ret_val;

    read_callback_status = -1;
    read_callback_context = NULL;

    // test with invalid parameters - callback NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_read(NULL, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - key NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, NULL, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - buf NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, NULL, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - buf_len NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, &buf, NULL, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with valid parameters - memory allocation fails 1
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 0;
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val != NS_NVM_MEMORY) {
        return false;
    }

    // test with valid parameters - read ok
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // make read callback
    test_platform_nvm_api_callback();

    if (read_callback_status != NS_NVM_OK || read_callback_context != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    return true;
}

bool test_ns_nvm_helper_delete()
{
    int ret_val;

    delete_callback_status = -1;
    delete_callback_context = NULL;

    // test with invalid parameters - callback NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_key_delete(NULL, key1, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with invalid parameters - key NULL
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    ret_val = ns_nvm_key_delete(test_ns_nvm_helper_delete_callback, NULL, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret_val == NS_NVM_OK) {
        return false;
    }

    // test with valid parameters - memory allocation fails
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    nsdynmemlib_stub.returnCounter = 0;
    ret_val = ns_nvm_key_delete(test_ns_nvm_helper_delete_callback, key1, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret_val != NS_NVM_MEMORY) {
        return false;
    }

    // test with valid parameters - OK
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_key_delete(test_ns_nvm_helper_delete_callback, key1, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // make delete callback
    test_platform_nvm_api_callback();

    // make flush callback
    test_platform_nvm_api_callback();

    if (delete_callback_status != NS_NVM_OK || delete_callback_context != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    return true;
}

bool test_ns_nvm_helper_concurrent_requests()
{
    int ret_val;

    write_callback_status = -1;
    write_callback_context = NULL;
    read_callback_status = -1;
    read_callback_context = NULL;
    delete_callback_status = -1;
    delete_callback_context = NULL;

    // read ok
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // write ok
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 2;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT2);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // delete ok
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_key_delete(test_ns_nvm_helper_delete_callback, key1, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // Read - should complete first, make callback
    test_platform_nvm_api_callback();

    if (read_callback_status != NS_NVM_OK || read_callback_context != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    // Write - should complete second, make create callback
    test_platform_nvm_api_callback();

    // make write callback
    test_platform_nvm_api_callback();

    // make write-flush callback with error status, note also delete will be given same error as pending request is triggered
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    test_platform_nvm_api_callback();

    if (write_callback_status != NS_NVM_ERROR || write_callback_context != TEST_NS_NVM_HELPER_CONTEXT2) {
        return false;
    }

    // delete callback is called after write because write was failing
    if (delete_callback_status != NS_NVM_ERROR || delete_callback_context != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    return true;
}

bool test_ns_nvm_helper_platform_error()
{
    int ret_val;

    write_callback_status = -1;
    write_callback_context = NULL;
    read_callback_status = -1;
    read_callback_context = NULL;
    delete_callback_status = -1;
    delete_callback_context = NULL;

    // read request fails directly in platform_nvm_api
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_ERROR) {
        return false;
    }

    // read request fails in platform_nvm_api callback
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_data_read(test_ns_nvm_helper_read_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_OK) {
        return false;
    }
    // make read callback
    test_platform_nvm_api_set_retval(PLATFORM_NVM_KEY_NOT_FOUND);
    test_platform_nvm_api_callback();
    if (read_callback_status != NS_NVM_DATA_NOT_FOUND || read_callback_context != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    // delete fails in platform api callback
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 1;
    ret_val = ns_nvm_key_delete(test_ns_nvm_helper_delete_callback, key1, TEST_NS_NVM_HELPER_CONTEXT3);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    // make delete callback
    test_platform_nvm_api_callback();
    if (delete_callback_status != NS_NVM_ERROR || delete_callback_context != TEST_NS_NVM_HELPER_CONTEXT3) {
        return false;
    }

    return true;
}


bool test_ns_nvm_helper_platform_error_in_write()
{
    int ret_val;

    write_callback_status = -1;
    write_callback_context = NULL;

    // create callback fails
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 2;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    test_platform_nvm_api_callback();
    if (write_callback_status != NS_NVM_ERROR || write_callback_context != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    write_callback_status = -1;
    write_callback_context = NULL;

    // write calback fails
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 2;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // make create callback
    test_platform_nvm_api_callback();

    // make write callback with error
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    test_platform_nvm_api_callback();
    if (write_callback_status != NS_NVM_ERROR || write_callback_context != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    write_callback_status = -1;
    write_callback_context = NULL;

    // flush calback fails
    test_platform_nvm_api_set_retval(PLATFORM_NVM_OK);
    nsdynmemlib_stub.returnCounter = 2;
    ret_val = ns_nvm_data_write(test_ns_nvm_helper_write_callback, key1, &buf, &buf_len, TEST_NS_NVM_HELPER_CONTEXT1);
    if (ret_val != NS_NVM_OK) {
        return false;
    }

    // make create callback
    test_platform_nvm_api_callback();

    // make write callback
    test_platform_nvm_api_callback();

    // make flush callback with error
    test_platform_nvm_api_set_retval(PLATFORM_NVM_ERROR);
    test_platform_nvm_api_callback();
    if (write_callback_status != NS_NVM_ERROR || write_callback_context != TEST_NS_NVM_HELPER_CONTEXT1) {
        return false;
    }

    return true;
}
