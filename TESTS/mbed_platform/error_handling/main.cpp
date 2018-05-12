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

/** Test logging of system errors
 *  and ensure the status/erro code is correct
 */
void test_system_errors()
{
    MbedErrorStatus error = MAKE_ERROR(MODULE_APPLICATION, ERROR_CODE_UNKNOWN);
    SET_WARNING(error, "Error Unknown", 0xAABBCCDD );
    MbedErrorStatus lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    error = MAKE_ERROR(MODULE_PLATFORM, ERROR_CODE_CREATE_FAILED);
    SET_WARNING(error, "Error Platform", 0xABCDABCD );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    error = MAKE_ERROR(MODULE_DRIVER_SERIAL, ERROR_CODE_OUT_OF_RESOURCES);
    SET_WARNING(error, "Error Serial driver", 0xAA );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    error = MAKE_ERROR(MODULE_UNKNOWN, ERROR_CODE_OUT_OF_MEMORY);
    SET_WARNING(error, "Error Out of resources", 0x11223344 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
}

/** Test logging of custom errors
 *  and ensure the status/erro code is correct
 */
void test_custom_errors()
{
    MbedErrorStatus error = MAKE_CUSTOM_ERROR(MODULE_APPLICATION, ERROR_CODE_UNKNOWN);
    SET_WARNING(error, "Custom Error Unknown", 0x1234 );
    MbedErrorStatus lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    error = MAKE_CUSTOM_ERROR(MODULE_PLATFORM, ERROR_CODE_CREATE_FAILED);
    SET_WARNING(error, "Custom Error Platform", 0x5555 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
    
    error = MAKE_CUSTOM_ERROR(MODULE_HAL, ERROR_CODE_OUT_OF_MEMORY);
    SET_WARNING(error, "Custom Error Unknown", 0x33445566 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(error, lastError);
}

/** Test logging of posix errors
 *  and ensure the status/erro code is correct
 */
void test_posix_errors()
{
    SET_WARNING(ERROR_EPERM, "Posix Error Eperm", 0x1234 );
    MbedErrorStatus lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_EPERM, lastError);
    
    SET_WARNING(ERROR_EBADF, "Posix Error, bad file descriptor", 0x5555 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_EBADF, lastError);
    
    SET_WARNING(ERROR_ENOENT, "Posix error, no file or dir", 0x33445566 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_ENOENT, lastError);
}

/** Test first and last error capture
 */
void test_first_and_last_error_capture()
{
    //clear the errors and error count to 0
    clear_all_errors();
    
    SET_WARNING(ERROR_OUT_OF_RESOURCES, "System type error", 0x1100 );
    SET_WARNING(ERROR_OUT_OF_MEMORY, "Out of memory", 0x2233);
    SET_WARNING(ERROR_SEMAPHORE_LOCK_FAILED, "Sem lock failed", 0x3344 );
    SET_WARNING(ERROR_MUTEX_LOCK_FAILED, "Mutex lock failed", 0x4455 );
    SET_WARNING(ERROR_CREATE_FAILED, "Create failed", 0x5566 );
    SET_WARNING(ERROR_TIME_OUT, "Time out error", 0x7788 );
    SET_WARNING(ERROR_MUTEX_UNLOCK_FAILED, "Mutex unlock failed", 0x99AA );
    SET_WARNING(ERROR_SEMAPHORE_UNLOCK_FAILED, "Semaphore unlock failed", 0xBBCC );
    
    MbedErrorStatus error = get_last_error();
    printf("\nlastError = 0x%08X", error );
    TEST_ASSERT_EQUAL_UINT(ERROR_SEMAPHORE_UNLOCK_FAILED, error);
    
    error = get_first_error();
    printf("\nfirstError = 0x%08X", error );
    TEST_ASSERT_EQUAL_UINT(ERROR_OUT_OF_RESOURCES, error);
    
}

/** Test error count and reset functionality
 */
void test_error_count_and_reset()
{
    int count = 7;
    
    //Log multiple errors and get the error count and make sure its 15
    for(int i=0; i<count; i++) {
        SET_WARNING(ERROR_OUT_OF_MEMORY, "Out of memory", i);
    }
    
    TEST_ASSERT_EQUAL_INT(count, get_error_count());
    
    //clear the errors and error count to 0
    clear_all_errors();
    
    //Now the error count should be 0
    TEST_ASSERT_EQUAL_INT(0, get_error_count());
    
}

/** Test error type encoding
 */
void test_error_encoding()
{
    SET_WARNING(ERROR_OUT_OF_RESOURCES, "System type error", 0x1100 );
    MbedErrorStatus lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_TYPE_SYSTEM, GET_MBED_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MODULE_UNKNOWN, GET_MBED_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(ERROR_CODE_OUT_OF_RESOURCES, GET_MBED_ERROR_CODE(lastError));
    
    MbedErrorStatus error = MAKE_CUSTOM_ERROR(MODULE_PLATFORM, ERROR_CODE_CREATE_FAILED);
    SET_WARNING(error, "Custom Error Type", 0x2233);
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_TYPE_CUSTOM, GET_MBED_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MODULE_PLATFORM, GET_MBED_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(ERROR_CODE_CREATE_FAILED, GET_MBED_ERROR_CODE(lastError));
    
    SET_WARNING(ERROR_EACCES, "Posix Error 1", 0x3344 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_TYPE_POSIX, GET_MBED_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MODULE_UNKNOWN, GET_MBED_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(ERROR_CODE_EACCES, GET_MBED_ERROR_CODE(lastError));
    
    SET_WARNING(ERROR_ERANGE, "Posix Error 2", 0x3355 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_TYPE_POSIX, GET_MBED_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MODULE_UNKNOWN, GET_MBED_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(ERROR_CODE_ERANGE, GET_MBED_ERROR_CODE(lastError));
    
    error = MAKE_ERROR(MODULE_HAL, ERROR_CODE_UNKNOWN);
    SET_WARNING(error, "HAL Entity error", 0x5566 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_TYPE_SYSTEM, GET_MBED_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MODULE_HAL, GET_MBED_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(ERROR_CODE_UNKNOWN, GET_MBED_ERROR_CODE(lastError));
    
    SET_WARNING(ERROR_UNKNOWN, "Unknown Entity error", 7788 );
    lastError = get_last_error();
    printf("\nlastError = 0x%08X", lastError );
    TEST_ASSERT_EQUAL_UINT(ERROR_TYPE_SYSTEM, GET_MBED_ERROR_TYPE(lastError));
    TEST_ASSERT_EQUAL_UINT(MODULE_UNKNOWN, GET_MBED_ERROR_MODULE(lastError));
    TEST_ASSERT_EQUAL_UINT(ERROR_CODE_UNKNOWN, GET_MBED_ERROR_CODE(lastError));
    
}

/** Test error value
 */
void test_error_value()
{
    uint32_t error_value = 0xAA11BB22;
    mbed_error_ctx error_ctx = {0};
    
    SET_WARNING(ERROR_OUT_OF_RESOURCES, "System type error", error_value );
    MbedErrorStatus status = get_last_error_log_info( &error_ctx );
    TEST_ASSERT(status == ERROR_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    
    MbedErrorStatus error = MAKE_CUSTOM_ERROR(MODULE_PLATFORM, ERROR_CODE_CREATE_FAILED);
    error_value = 0xABCD;
    SET_WARNING(error, "Custom Error Type", error_value);
    status = get_last_error_log_info( &error_ctx );
    TEST_ASSERT(status == ERROR_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    
    error_value = 0x11223344;
    SET_WARNING(ERROR_EACCES, "Posix Error 1", error_value );
    status = get_last_error_log_info( &error_ctx );
    TEST_ASSERT(status == ERROR_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
}

/** Test error context capture
 */
void test_error_context_capture()
{
    uint32_t error_value = 0xABCD;
    mbed_error_ctx error_ctx = {0};
    
    SET_WARNING(ERROR_INVALID_ARGUMENT, "System type error", error_value );
    MbedErrorStatus status = get_last_error_log_info( &error_ctx );
    TEST_ASSERT(status == ERROR_SUCCESS);
    TEST_ASSERT_EQUAL_UINT(error_value, error_ctx.error_value);
    TEST_ASSERT_EQUAL_UINT(osThreadGetId(), error_ctx.thread_id);
    
    //Capture thread info and compare
    osRtxThread_t *current_thread = osRtxInfo.thread.run.curr;
    TEST_ASSERT_EQUAL_UINT((uint32_t)current_thread->thread_addr, error_ctx.thread_entry_address);
    TEST_ASSERT_EQUAL_UINT((uint32_t)current_thread->stack_size, error_ctx.thread_stack_size);
    TEST_ASSERT_EQUAL_UINT((uint32_t)current_thread->stack_mem, error_ctx.thread_stack_mem);
#ifdef MBED_CONF_ERROR_FILENAME_CAPTURE_ENABLED
    TEST_ASSERT_EQUAL_STRING(MBED_FILENAME, error_ctx.error_filename);
#endif    
}

#ifndef MBED_CONF_ERROR_LOG_DISABLED  
/** Test error logging functionality
 */
void test_error_logging()
{
    mbed_error_ctx error_ctx = {0};
    
    //clear the current errors first
    clear_all_errors();
    
    //log 3 errors and retrieve them to ensure they are correct
    SET_WARNING(ERROR_INVALID_ARGUMENT, "Invalid argument error", 1 );
    SET_WARNING(ERROR_INVALID_SIZE, "Invalid size error", 2 );
    SET_WARNING(ERROR_INVALID_FORMAT, "Invalid format error", 3 );
    
    MbedErrorStatus status = get_error_log_info( 0, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_INVALID_ARGUMENT, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(1, error_ctx.error_value);
    
    status = get_error_log_info( 1, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_INVALID_SIZE, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(2, error_ctx.error_value);
    
    status = get_error_log_info( 2, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_INVALID_FORMAT, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(3, error_ctx.error_value);
    
    //log 2 more errors to fill the log and then read them out to ensure that its correct
    SET_WARNING(ERROR_INVALID_DATA_DETECTED, "Invalid data", 4 );
    SET_WARNING(ERROR_INVALID_OPERATION, "Invalid operation", 5 );
    
    status = get_error_log_info( 3, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_INVALID_DATA_DETECTED, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(4, error_ctx.error_value);
    
    status = get_error_log_info( 4, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_INVALID_OPERATION, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(5, error_ctx.error_value);
    
    //Log a bunch of errors to overflow the error log and retrieve them
    SET_WARNING(ERROR_INVALID_ARGUMENT, "Invalid argument error", 6 );
    SET_WARNING(ERROR_INVALID_SIZE, "Invalid size error", 7 );
    SET_WARNING(ERROR_INVALID_FORMAT, "Invalid format error", 8 );
    SET_WARNING(ERROR_NOT_READY, "Not ready error", 9 );
    
    //Last 5 entries
    SET_WARNING(ERROR_TIME_OUT, "Timeout error", 10 );
    SET_WARNING(ERROR_ALREADY_IN_USE, "Already in use error", 11 );
    SET_WARNING(ERROR_UNSUPPORTED, "Not supported error", 12 );
    SET_WARNING(ERROR_ACCESS_DENIED, "Access denied error", 13 );
    SET_WARNING(ERROR_ITEM_NOT_FOUND, "Not found error", 14 );
    
    status = get_error_log_info( 0, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_TIME_OUT, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(10, error_ctx.error_value);
    
    status = get_error_log_info( 1, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_ALREADY_IN_USE, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(11, error_ctx.error_value);
    
    status = get_error_log_info( 2, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_UNSUPPORTED, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(12, error_ctx.error_value);
    
    status = get_error_log_info( 3, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_ACCESS_DENIED, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(13, error_ctx.error_value);
    
    status = get_error_log_info( 4, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_ITEM_NOT_FOUND, error_ctx.error_status);
    TEST_ASSERT_EQUAL_UINT(14, error_ctx.error_value);
    
    //Try an index which is invalid, we should get ERROR_INVALID_ARGUMENT back
    status = get_error_log_info( 99, &error_ctx );
    TEST_ASSERT_EQUAL_UINT(ERROR_INVALID_ARGUMENT, status);
    
}

#define NUM_TEST_THREADS 10

//Error logger threads
void err_thread_func(MbedErrorStatus *error_status)
{
    //printf("\nError Status = 0x%08X\n",*error_status);
    SET_WARNING(*error_status, "Error from Multi-Threaded error logging test", *error_status );
}


/** Test error logging multithreaded
 */
void test_error_logging_multithread()
{
    mbed_error_ctx error_ctx = {0};
    int i=0;
    Thread errThread[NUM_TEST_THREADS];
    MbedErrorStatus error_status[NUM_TEST_THREADS] = { 
                                        ERROR_INVALID_ARGUMENT, ERROR_INVALID_DATA_DETECTED, ERROR_INVALID_FORMAT, ERROR_INVALID_SIZE, ERROR_INVALID_OPERATION, 
                                        ERROR_ITEM_NOT_FOUND, ERROR_ACCESS_DENIED, ERROR_FAILED_OPERATION, ERROR_OPERATION_PROHIBITED, ERROR_OPERATION_ABORTED
    };
    
        
    for(; i<NUM_TEST_THREADS; i++) {
        errThread[i].start(callback(err_thread_func, &error_status[i]));
    }
    wait(2.0);
    for(i=0; i<NUM_TEST_THREADS; i++) {
        errThread[i].join();
    }
    
    i = get_error_log_count()-1;
    //printf("\nError log count = %d\n", i+1);
    for(;i>=0;--i) {
        MbedErrorStatus status = get_error_log_info( i, &error_ctx );
        if(status != ERROR_SUCCESS) {
            TEST_FAIL();
        }
            
        //printf("\nError Status[%d] = 0x%08X Value = 0x%08X\n", i, (unsigned int)error_ctx.error_status, (unsigned int)error_ctx.error_value);
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
    if( ERROR_SUCCESS != set_error_hook(MyErrorHook)) {
        TEST_FAIL();
    }
    
    SET_WARNING(ERROR_INVALID_ARGUMENT, "Test for error hook", 1234);
    int32_t sem_status = callback_sem.wait(5000);
    
    TEST_ASSERT(sem_status > 0);
}

#ifdef MBED_TEST_SIM_BLOCKDEVICE

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
    SET_WARNING(ERROR_TIME_OUT, "Timeout error", 1 );
    SET_WARNING(ERROR_ALREADY_IN_USE, "Already in use error", 2 );
    SET_WARNING(ERROR_UNSUPPORTED, "Not supported error", 3 );
    SET_WARNING(ERROR_ACCESS_DENIED, "Access denied error", 4 );
    SET_WARNING(ERROR_ITEM_NOT_FOUND, "Not found error", 5 );
    SET_WARNING(ERROR_INVALID_ARGUMENT, "Invalid argument error", 6 );
    SET_WARNING(ERROR_INVALID_SIZE, "Invalid size error", 7 );
    SET_WARNING(ERROR_INVALID_FORMAT, "Invalid format error", 8 );
    SET_WARNING(ERROR_INVALID_OPERATION, "Invalid operation", 9 );
    SET_WARNING(ERROR_NOT_READY, "Not ready error", 10 );
    
    int error = 0;
    
    error = MBED_TEST_FILESYSTEM::format(&fd);
    if(error < 0) {
        printf("Failed formatting");
        TEST_FAIL();
    }
    
    error = fs.mount(&fd);
    if(error < 0) {
        printf("Failed mounting fs");
        TEST_FAIL();
    }
    
    if(ERROR_SUCCESS != save_error_log("/fs/errors.log")) {
        printf("Failed saving error log");
        TEST_FAIL();
    }
    
    FILE *error_file = fopen("/fs/errors.log", "r");
    if(error_file == NULL) {
        printf("Unable to find error log in fs");
        TEST_FAIL();
    }
    
    char buff[64] = {0};
    while (!feof(error_file)){
      int size = fread(&buff[0], 1, 15, error_file);
      fwrite(&buff[0], 1, size, stdout);
    }
    printf("\r\n");
    fclose(error_file);
    
    error = fs.unmount();
    if(error < 0) {
        printf("Failed unmounting fs");
        TEST_FAIL();
    }
}

#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test error counting and reset", test_error_count_and_reset),
    Case("Test system errors", test_system_errors),
    Case("Test custom errors", test_custom_errors),
    Case("Test posix errors", test_posix_errors),
    Case("Test first and last error capture", test_first_and_last_error_capture),
    Case("Test error encoding", test_error_encoding),
    Case("Test error value", test_error_value),
    Case("Test error context capture", test_error_context_capture),
    Case("Test error hook", test_error_hook),
#ifndef MBED_CONF_ERROR_LOG_DISABLED    
    Case("Test error logging", test_error_logging),
    Case("Test error handling multi-threaded", test_error_logging_multithread),
#ifdef MBED_TEST_SIM_BLOCKDEVICE    
    Case("Test error save log", test_save_error_log),
#endif    
#endif    
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
