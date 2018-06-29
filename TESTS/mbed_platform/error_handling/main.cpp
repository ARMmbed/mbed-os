/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed.h"
#include <LittleFileSystem.h>
#include "HeapBlockDevice.h"

using utest::v1::Case;

/** Test error count and reset functionality
 */
void test_error_count_and_reset()
{
    int count = 7;
    
    //Log multiple errors and get the error count and make sure its 15
    for(int i=0; i<count; i++) {
        MBED_WARNING1(MBED_ERROR_OUT_OF_MEMORY, "Out of memory", i);
    }
    
    TEST_ASSERT_EQUAL_INT(count, mbed_get_error_count());
    
    //clear the errors and error count to 0
    mbed_clear_all_errors();
    
    //Now the error count should be 0
    TEST_ASSERT_EQUAL_INT(0, mbed_get_error_count());
    
}

/** Test error type encoding and test capturing of system, custom, posix errors
 *  and ensure the status/error code/type/error value is correct
 */
void test_error_capturing()
{
    uint32_t error_value = 0xAA11BB22;
    mbed_error_ctx error_ctx = {0};
    
    //first clear all errors and start afresh
    
    MBED_WARNING1(MBED_ERROR_OUT_OF_RESOURCES, "System type error", 0x1100 );
    mbed_error_status_t lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_TYPE_SYSTEM, MBED_GET_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_MODULE_UNKNOWN, MBED_GET_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_CODE_OUT_OF_RESOURCES, MBED_GET_ERROR_CODE(lastError));
    
    mbed_error_status_t error = MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_OUT_OF_RESOURCES);
    MBED_WARNING1(error, "Error Serial", 0xAA );
    lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    error = MBED_MAKE_CUSTOM_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNKNOWN);
    MBED_WARNING1(error, "Custom Error Unknown", 0x1234 );
    lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    MBED_WARNING1(MBED_ERROR_EPERM, "Posix Error Eperm", 0x1234 );
    lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_EPERM, lastError);
    
    error = MBED_MAKE_CUSTOM_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_CREATE_FAILED);
    MBED_WARNING1(error, "Custom Error Type", error_value);
    lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_TYPE_CUSTOM, MBED_GET_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_MODULE_PLATFORM, MBED_GET_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_CODE_CREATE_FAILED, MBED_GET_ERROR_CODE(lastError));
    mbed_error_status_t status = mbed_get_last_error_info( &error_ctx );
    TEST_ASSERT(status == MBED_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    
    error_value = 0xAABBCC;
    MBED_WARNING1(MBED_ERROR_EACCES, "Posix Error", error_value );
    lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_TYPE_POSIX, MBED_GET_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_MODULE_UNKNOWN, MBED_GET_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_CODE_EACCES, MBED_GET_ERROR_CODE(lastError));
    status = mbed_get_last_error_info( &error_ctx );
    TEST_ASSERT(status == MBED_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    
    error_value = 0;
    error = MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_UNKNOWN);
    MBED_WARNING1(error, "HAL Entity error", error_value );
    lastError = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_TYPE_SYSTEM, MBED_GET_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_MODULE_HAL, MBED_GET_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_CODE_UNKNOWN, MBED_GET_ERROR_CODE(lastError));
    status = mbed_get_last_error_info( &error_ctx );
    TEST_ASSERT(status == MBED_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    
    MBED_WARNING1(MBED_ERROR_MUTEX_LOCK_FAILED, "Mutex lock failed", 0x4455 );
    error = mbed_get_last_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_MUTEX_LOCK_FAILED, error);
    
    error = mbed_get_first_error();
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_OUT_OF_RESOURCES, error);
    
}

/** Test error context capture
 */
void test_error_context_capture()
{
    uint32_t error_value = 0xABCD;
    mbed_error_ctx error_ctx = {0};
    
    MBED_WARNING1(MBED_ERROR_INVALID_ARGUMENT, "System type error", error_value );
    mbed_error_status_t status = mbed_get_last_error_info( &error_ctx );
    TEST_ASSERT(status == MBED_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    TEST_ASSERT_EQUAL_UINT((uint32_t)osThreadGetId(), error_ctx.thread_id);
    
    //Capture thread info and compare
    osRtxThread_t *current_thread = osRtxInfo.thread.run.curr;
    TEST_ASSERT_EQUAL_UINT((uint32_t)current_thread->thread_addr, error_ctx.thread_entry_address);
    TEST_ASSERT_EQUAL_UINT((uint32_t)current_thread->stack_size, error_ctx.thread_stack_size);
    TEST_ASSERT_EQUAL_UINT((uint32_t)current_thread->stack_mem, error_ctx.thread_stack_mem);
#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
    TEST_ASSERT_EQUAL_STRING(MBED_FILENAME, error_ctx.error_filename);
#endif    
}

#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED
/** Test error logging functionality
 */
void test_error_logging()
{
    mbed_error_ctx error_ctx = {0};
    
    //clear the current errors first
    mbed_clear_all_errors();
    
    //log 3 errors and retrieve them to ensure they are correct
    MBED_WARNING1(MBED_ERROR_INVALID_ARGUMENT, "Invalid argument", 1 );
    MBED_WARNING1(MBED_ERROR_INVALID_SIZE, "Invalid size", 2 );
    MBED_WARNING1(MBED_ERROR_INVALID_FORMAT, "Invalid format", 3 );
    
    mbed_error_status_t status = mbed_get_error_hist_info( 0, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_INVALID_ARGUMENT, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(1, error_ctx.error_value);
    
    status = mbed_get_error_hist_info( 1, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_INVALID_SIZE, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(2, error_ctx.error_value);
    
    status = mbed_get_error_hist_info( 2, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_INVALID_FORMAT, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(3, error_ctx.error_value);
    
    //Log a bunch of errors to overflow the error log and retrieve them
    MBED_WARNING1(MBED_ERROR_INVALID_ARGUMENT, "Invalid argument", 6 );
    MBED_WARNING1(MBED_ERROR_INVALID_SIZE, "Invalid size", 7 );
    MBED_WARNING1(MBED_ERROR_INVALID_FORMAT, "Invalid format", 8 );
    MBED_WARNING1(MBED_ERROR_NOT_READY, "Not ready error", 9 );
    
    //Last 4 entries
    MBED_WARNING1(MBED_ERROR_TIME_OUT, "Timeout error", 10 );
    MBED_WARNING1(MBED_ERROR_ALREADY_IN_USE, "Already in use error", 11 );
    MBED_WARNING1(MBED_ERROR_UNSUPPORTED, "Not supported", 12 );
    MBED_WARNING1(MBED_ERROR_ACCESS_DENIED, "Access denied", 13 );
    
    status = mbed_get_error_hist_info( 0, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_TIME_OUT, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(10, error_ctx.error_value);
    
    status = mbed_get_error_hist_info( 1, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_ALREADY_IN_USE, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(11, error_ctx.error_value);
    
    status = mbed_get_error_hist_info( 2, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_UNSUPPORTED, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(12, error_ctx.error_value);
    
    status = mbed_get_error_hist_info( 3, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_ACCESS_DENIED, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(13, error_ctx.error_value);
    
    //Try an index which is invalid, we should get ERROR_INVALID_ARGUMENT back
    status = mbed_get_error_hist_info( 99, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(MBED_ERROR_INVALID_ARGUMENT, status);
    
}

#define NUM_TEST_THREADS 5

//Error logger threads
void err_thread_func(mbed_error_status_t *error_status)
{
    MBED_WARNING1(*error_status, "Error from Multi-Threaded error logging test", *error_status );
}


/** Test error logging multithreaded
 */
void test_error_logging_multithread()
{
    mbed_error_ctx error_ctx = {0};
    int i=0;
    Thread *errThread[NUM_TEST_THREADS];
    mbed_error_status_t error_status[NUM_TEST_THREADS] = { 
                                        MBED_ERROR_INVALID_ARGUMENT, MBED_ERROR_INVALID_DATA_DETECTED, MBED_ERROR_INVALID_FORMAT, MBED_ERROR_INVALID_SIZE, MBED_ERROR_INVALID_OPERATION
    };
    
        
    for(; i<NUM_TEST_THREADS; i++) {
        errThread[i] = new Thread(osPriorityNormal1, 512, NULL, NULL);
        errThread[i]->start(callback(err_thread_func, &error_status[i]));
    }
    wait(2.0);
    for(i=0; i<NUM_TEST_THREADS; i++) {
        errThread[i]->join();
    }
    
    i = mbed_get_error_hist_count()-1;
    
    for(;i>=0;--i) {
        mbed_error_status_t status = mbed_get_error_hist_info( i, &error_ctx );
        if(status != MBED_SUCCESS) {
            TEST_FAIL();
        }
            
        TEST_ASSERT_EQUAL_UINT((unsigned int)error_ctx.error_value, (unsigned int)error_ctx.error_status);
    }
}
#endif

static Semaphore    callback_sem;
void MyErrorHook(const mbed_error_ctx *error_ctx)
{
    callback_sem.release();
}

/** Test error hook
 */
void test_error_hook()
{
    if( MBED_SUCCESS != mbed_set_error_hook(MyErrorHook)) {
        TEST_FAIL();
    }
    
    MBED_WARNING1(MBED_ERROR_INVALID_ARGUMENT, "Test for error hook", 1234);
    int32_t sem_status = callback_sem.wait(5000);
    
    TEST_ASSERT(sem_status > 0);
}

#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED && defined(MBED_TEST_SIM_BLOCKDEVICE)

// test configuration
#ifndef MBED_TEST_FILESYSTEM
#define MBED_TEST_FILESYSTEM LittleFileSystem
#endif

#ifndef MBED_TEST_FILESYSTEM_DECL
#define MBED_TEST_FILESYSTEM_DECL MBED_TEST_FILESYSTEM fs("fs")
#endif

#ifndef MBED_TEST_BLOCK_COUNT
#define MBED_TEST_BLOCK_COUNT 64
#endif

#ifndef MBED_TEST_SIM_BLOCKDEVICE_DECL
#define MBED_TEST_SIM_BLOCKDEVICE_DECL MBED_TEST_SIM_BLOCKDEVICE fd(MBED_TEST_BLOCK_COUNT*512, 1, 1, 512)
#endif

// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_FILESYSTEM)
#include INCLUDE(MBED_TEST_SIM_BLOCKDEVICE)

MBED_TEST_FILESYSTEM_DECL;
MBED_TEST_SIM_BLOCKDEVICE_DECL;

/** Test save error log
 */
void test_save_error_log()
{
    //Log some errors
    MBED_WARNING1(MBED_ERROR_TIME_OUT, "Timeout error", 1 );
    MBED_WARNING1(MBED_ERROR_ALREADY_IN_USE, "Already in use error", 2 );
    MBED_WARNING1(MBED_ERROR_UNSUPPORTED, "Not supported error", 3 );
    MBED_WARNING1(MBED_ERROR_ACCESS_DENIED, "Access denied error", 4 );
    MBED_WARNING1(MBED_ERROR_ITEM_NOT_FOUND, "Not found error", 5 );
    
    int error = 0;
    
    error = MBED_TEST_FILESYSTEM::format(&fd);
    if(error < 0) {
        TEST_FAIL_MESSAGE("Failed formatting");
    }
    
    error = fs.mount(&fd);
    if(error < 0) {
        TEST_FAIL_MESSAGE("Failed mounting fs");
    }
    
    if(MBED_SUCCESS != mbed_save_error_hist("/fs/errors.log")) {
        TEST_FAIL_MESSAGE("Failed saving error log");
    }
    
    FILE *error_file = fopen("/fs/errors.log", "r");
    if(error_file == NULL) {
        TEST_FAIL_MESSAGE("Unable to find error log in fs");
    }
    
    char buff[64] = {0};
    while (!feof(error_file)){
      int size = fread(&buff[0], 1, 15, error_file);
      fwrite(&buff[0], 1, size, stdout);
    }
    fclose(error_file);
    
    error = fs.unmount();
    if(error < 0) {
        TEST_FAIL_MESSAGE("Failed unmounting fs");
    }
}

#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test error counting and reset", test_error_count_and_reset),
    Case("Test error encoding, value capture, first and last errors", test_error_capturing),
#if MBED_CONF_RTOS_PRESENT
    Case("Test error context capture", test_error_context_capture),
#endif //MBED_CONF_RTOS_PRESENT
    Case("Test error hook", test_error_hook),
#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED    
    Case("Test error logging", test_error_logging),
#if MBED_CONF_RTOS_PRESENT
    Case("Test error handling multi-threaded", test_error_logging_multithread),
#endif //MBED_CONF_RTOS_PRESENT
#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED && defined(MBED_TEST_SIM_BLOCKDEVICE)
    Case("Test error save log", test_save_error_log),
#endif //MBED_TEST_SIM_BLOCKDEVICE
#endif //MBED_CONF_ERROR_HIST_DISABLED
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
