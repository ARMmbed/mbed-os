/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "device.h"
#include "platform/source/mbed_crash_data_offsets.h"
#include "platform/mbed_atomic.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "platform/source/mbed_error_hist.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_stats.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "rtx_os.h"
#endif

#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#ifndef NDEBUG
#define ERROR_REPORT(ctx, error_msg, error_filename, error_line) print_error_report(ctx, error_msg, error_filename, error_line)
static void print_error_report(const mbed_error_ctx *ctx, const char *, const char *error_filename, int error_line);
#else
#define ERROR_REPORT(ctx, error_msg, error_filename, error_line) ((void) 0)
#endif

static bool error_in_progress;
static core_util_atomic_flag halt_in_progress = CORE_UTIL_ATOMIC_FLAG_INIT;
static int error_count = 0;
static mbed_error_ctx first_error_ctx = {0};

static mbed_error_ctx last_error_ctx = {0};
static mbed_error_hook_t error_hook = NULL;
static mbed_error_status_t handle_error(mbed_error_status_t error_status, unsigned int error_value, const char *filename, int line_number, void *caller);

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
//Global for populating the context in exception handler
static mbed_error_ctx *const report_error_ctx = (mbed_error_ctx *)(ERROR_CONTEXT_LOCATION);
static bool is_reboot_error_valid = false;

//Helper function to calculate CRC
//NOTE: It would have been better to use MbedCRC implementation. But
//MbedCRC uses table based calculation and we dont want to keep that table memory
//used up for this purpose. Also we cannot force bitwise calculation in MbedCRC
//and it also requires a new wrapper to be called from C implementation. Since
//we dont have many uses cases to create a C wrapper for MbedCRC and the data
//we calculate CRC on in this context is very less we will use a local
//implementation here.
static unsigned int compute_crc32(const void *data, int datalen)
{
    const unsigned int polynomial = 0x04C11DB7; /* divisor is 32bit */
    unsigned int crc = 0; /* CRC value is 32bit */
    unsigned char *buf = (unsigned char *)data;//use a temp variable to make code readable and to avoid typecasting issues.

    for (; datalen > 0; datalen--) {
        unsigned char b = *buf++;
        crc ^= (unsigned int)(b << 24); /* move byte into upper 8bit */
        for (int i = 0; i < 8; i++) {
            /* is MSB 1 */
            if ((crc & 0x80000000) != 0) {
                crc = (unsigned int)((crc << 1) ^ polynomial);
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}
#endif

//Helper function to halt the system
static MBED_NORETURN void mbed_halt_system(void)
{
    // Prevent recursion if halt is called again during halt attempt - try
    // something simple instead.
    if (core_util_atomic_flag_test_and_set(&halt_in_progress)) {
        core_util_critical_section_enter();
        __DSB();
        for (;;) {
            __WFE(); // Not WFI, as don't want to wake for pending interrupts
        }
    }

    //If in ISR context, call mbed_die directly
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        mbed_die();
    }

    // In normal context, try orderly exit(1), which eventually calls mbed_die
    exit(1);
}

WEAK MBED_NORETURN void error(const char *format, ...)
{
    // Prevent recursion if error is called again during store+print attempt
    if (!core_util_atomic_exchange_bool(&error_in_progress, true)) {
        handle_error(MBED_ERROR_UNKNOWN, 0, NULL, 0, MBED_CALLER_ADDR());
        ERROR_REPORT(&last_error_ctx, "Fatal Run-time error", NULL, 0);

#ifndef NDEBUG
        va_list arg;
        va_start(arg, format);
        mbed_error_vprintf(format, arg);
        va_end(arg);
        // Add a newline to prevent any line buffering
        mbed_error_puts("\n");
#endif
    }

    mbed_halt_system();
}

//Set an error status with the error handling system
static mbed_error_status_t handle_error(mbed_error_status_t error_status, unsigned int error_value, const char *filename, int line_number, void *caller)
{
    mbed_error_ctx current_error_ctx;

    //Error status should always be < 0
    if (error_status >= 0) {
        //This is a weird situation, someone called mbed_error with an invalid error code.
        //We will still handle the situation but change the error code to ERROR_INVALID_ARGUMENT, at least the context will have info on who called it
        error_status = MBED_ERROR_INVALID_ARGUMENT;
    }

    //Clear the context capturing buffer
    memset(&current_error_ctx, 0, sizeof(mbed_error_ctx));
    //Capture error information
    current_error_ctx.error_status = error_status;
    current_error_ctx.error_address = (uint32_t)caller;
    current_error_ctx.error_value = error_value;
#ifdef MBED_CONF_RTOS_PRESENT
    //Capture thread info
    osRtxThread_t *current_thread = osRtxInfo.thread.run.curr;
    current_error_ctx.thread_id = (uint32_t)current_thread;
    current_error_ctx.thread_entry_address = (uint32_t)current_thread->thread_addr;
    current_error_ctx.thread_stack_size = current_thread->stack_size;
    current_error_ctx.thread_stack_mem = (uint32_t)current_thread->stack_mem;
    current_error_ctx.thread_current_sp = (uint32_t)&current_error_ctx; // Address local variable to get a stack pointer
#endif //MBED_CONF_RTOS_PRESENT

#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
    //Capture filename/linenumber if provided
    //Index for tracking error_filename
    strncpy(current_error_ctx.error_filename, filename, MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN);
    current_error_ctx.error_line_number = line_number;
#endif

    //Prevent corruption by holding out other callers
    core_util_critical_section_enter();

    //Increment error count
    error_count++;

    //Capture the first system error and store it
    if (error_count == 1) { //first error
        memcpy(&first_error_ctx, &current_error_ctx, sizeof(mbed_error_ctx));
    }

    //copy this error to last error
    memcpy(&last_error_ctx, &current_error_ctx, sizeof(mbed_error_ctx));

#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED
    //Log the error with error log
    mbed_error_hist_put(&current_error_ctx);
#endif

    //Call the error hook if available
    if (error_hook != NULL) {
        error_hook(&last_error_ctx);
    }

    core_util_critical_section_exit();

    return MBED_SUCCESS;
}

WEAK void mbed_error_reboot_callback(mbed_error_ctx *error_context)
{
    //Dont do anything here, let application override this if required.
}

//Initialize Error handling system and report any errors detected on rebooted
mbed_error_status_t mbed_error_initialize(void)
{
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    uint32_t crc_val = 0;

    //Just check if we have valid value for error_status, if error_status is positive(which is not valid), no need to check crc
    if (report_error_ctx->error_status < 0) {
        crc_val = compute_crc32(report_error_ctx, offsetof(mbed_error_ctx, crc_error_ctx));
        //Read report_error_ctx and check if CRC is correct, and with valid status code
        if ((report_error_ctx->crc_error_ctx == crc_val) && (report_error_ctx->is_error_processed == 0)) {
            is_reboot_error_valid = true;

            //Call the mbed_error_reboot_callback, this enables applications to do some handling before we do the handling
            mbed_error_reboot_callback(report_error_ctx);

            //We let the callback reset the error info, so check if its still valid and do the rest only if its still valid.
            if (report_error_ctx->error_reboot_count > 0) {

                report_error_ctx->is_error_processed = 1;//Set the flag that we already processed this error
                crc_val = compute_crc32(report_error_ctx, offsetof(mbed_error_ctx, crc_error_ctx));
                report_error_ctx->crc_error_ctx = crc_val;

                //Enforce max-reboot only if auto reboot is enabled
#if MBED_CONF_PLATFORM_FATAL_ERROR_AUTO_REBOOT_ENABLED
                if (report_error_ctx->error_reboot_count >= MBED_CONF_PLATFORM_ERROR_REBOOT_MAX) {
                    mbed_halt_system();
                }
#endif
            }
        }
    }
#endif
    return MBED_SUCCESS;
}

//Return the first error
mbed_error_status_t mbed_get_first_error(void)
{
    //return the first error recorded
    return first_error_ctx.error_status;
}

//Return the last error
mbed_error_status_t mbed_get_last_error(void)
{
    //return the last error recorded
    return last_error_ctx.error_status;
}

//Gets the current error count
int mbed_get_error_count(void)
{
    //return the current error count
    return error_count;
}

//Reads the fatal error occurred" flag
bool mbed_get_error_in_progress(void)
{
    return core_util_atomic_load_bool(&error_in_progress);
}

//Sets a non-fatal error
mbed_error_status_t mbed_warning(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number)
{
    return handle_error(error_status, error_value, filename, line_number, MBED_CALLER_ADDR());
}

//Sets a fatal error, this function is marked WEAK to be able to override this for some tests
WEAK MBED_NORETURN mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number)
{
    // Prevent recursion if error is called again during store+print attempt
    if (!core_util_atomic_exchange_bool(&error_in_progress, true)) {
        //set the error reported
        (void) handle_error(error_status, error_value, filename, line_number, MBED_CALLER_ADDR());

        //On fatal errors print the error context/report
        ERROR_REPORT(&last_error_ctx, error_msg, filename, line_number);
    }

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    uint32_t crc_val = 0;
    crc_val = compute_crc32(report_error_ctx, offsetof(mbed_error_ctx, crc_error_ctx));
    //Read report_error_ctx and check if CRC is correct for report_error_ctx
    if (report_error_ctx->crc_error_ctx == crc_val) {
        uint32_t current_reboot_count = report_error_ctx->error_reboot_count;
        last_error_ctx.error_reboot_count = current_reboot_count + 1;
    } else {
        last_error_ctx.error_reboot_count = 1;
    }
    last_error_ctx.is_error_processed = 0;//Set the flag that this is a new error
    //Update the struct with crc
    last_error_ctx.crc_error_ctx = compute_crc32(&last_error_ctx, offsetof(mbed_error_ctx, crc_error_ctx));
    //Protect report_error_ctx while we update it
    core_util_critical_section_enter();
    memcpy(report_error_ctx, &last_error_ctx, sizeof(mbed_error_ctx));
    core_util_critical_section_exit();
    //We need not call delete_mbed_crc(crc_obj) here as we are going to reset the system anyway, and calling delete while handling a fatal error may cause nested exception
#if MBED_CONF_PLATFORM_FATAL_ERROR_AUTO_REBOOT_ENABLED && (MBED_CONF_PLATFORM_ERROR_REBOOT_MAX > 0)
#ifndef NDEBUG
    mbed_error_printf("\n= System will be rebooted due to a fatal error =\n");
    if (report_error_ctx->error_reboot_count >= MBED_CONF_PLATFORM_ERROR_REBOOT_MAX) {
        //We have rebooted more than enough, hold the system here.
        mbed_error_printf("= Reboot count(=%" PRIi32") reached maximum, system will halt after rebooting =\n", report_error_ctx->error_reboot_count);
    }
#endif
    system_reset();//do a system reset to get the system rebooted
#endif
#endif
    mbed_halt_system();
}

//Register an application defined callback with error handling
mbed_error_status_t mbed_set_error_hook(mbed_error_hook_t error_hook_in)
{
    //register the new hook/callback
    if (error_hook_in != NULL) {
        error_hook = error_hook_in;
        return MBED_SUCCESS;
    }

    return MBED_ERROR_INVALID_ARGUMENT;
}

//Reset the reboot error context
mbed_error_status_t mbed_reset_reboot_error_info()
{
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    //Protect for thread safety
    core_util_critical_section_enter();
    memset(report_error_ctx, 0, sizeof(mbed_error_ctx));
    core_util_critical_section_exit();
#endif
    return MBED_SUCCESS;
}

//Reset the reboot error context
mbed_error_status_t mbed_reset_reboot_count()
{
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    if (is_reboot_error_valid) {
        uint32_t crc_val = 0;
        core_util_critical_section_enter();
        report_error_ctx->error_reboot_count = 0;//Set reboot count to 0
        //Update CRC
        crc_val = compute_crc32(report_error_ctx, offsetof(mbed_error_ctx, crc_error_ctx));
        report_error_ctx->crc_error_ctx = crc_val;
        core_util_critical_section_exit();
        return MBED_SUCCESS;
    }
#endif
    return MBED_ERROR_ITEM_NOT_FOUND;
}

//Retrieve the reboot error context
mbed_error_status_t mbed_get_reboot_error_info(mbed_error_ctx *error_info)
{
    mbed_error_status_t status = MBED_ERROR_ITEM_NOT_FOUND;
#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    if (is_reboot_error_valid) {
        if (error_info != NULL) {
            memcpy(error_info, report_error_ctx, sizeof(mbed_error_ctx));
            status = MBED_SUCCESS;
        } else {
            status = MBED_ERROR_INVALID_ARGUMENT;
        }
    }
#endif
    return status;
}

//Retrieve the first error context from error log
mbed_error_status_t mbed_get_first_error_info(mbed_error_ctx *error_info)
{
    memcpy(error_info, &first_error_ctx, sizeof(first_error_ctx));
    return MBED_SUCCESS;
}

//Retrieve the last error context from error log
mbed_error_status_t mbed_get_last_error_info(mbed_error_ctx *error_info)
{
    memcpy(error_info, &last_error_ctx, sizeof(mbed_error_ctx));
    return MBED_SUCCESS;
}

//Makes an mbed_error_status_t value
mbed_error_status_t mbed_make_error(mbed_error_type_t error_type, mbed_module_type_t entity, mbed_error_code_t error_code)
{
    switch (error_type) {
        case MBED_ERROR_TYPE_POSIX:
            if (error_code >= MBED_POSIX_ERROR_BASE && error_code <= MBED_SYSTEM_ERROR_BASE) {
                return -error_code;
            }
            break;

        case MBED_ERROR_TYPE_SYSTEM:
            if (error_code >= MBED_SYSTEM_ERROR_BASE && error_code <= MBED_CUSTOM_ERROR_BASE) {
                return MAKE_MBED_ERROR(MBED_ERROR_TYPE_SYSTEM, entity, error_code);
            }
            break;

        case MBED_ERROR_TYPE_CUSTOM:
            if (error_code >= MBED_CUSTOM_ERROR_BASE) {
                return MAKE_MBED_ERROR(MBED_ERROR_TYPE_CUSTOM, entity, error_code);
            }
            break;

        default:
            break;
    }

    //If we are passed incorrect values return a generic system error
    return MAKE_MBED_ERROR(MBED_ERROR_TYPE_SYSTEM, MBED_MODULE_UNKNOWN, MBED_ERROR_CODE_UNKNOWN);
}

/**
 * Clears all the last error, error count and all entries in the error log.
 * @return                      0 or MBED_SUCCESS on success.
 *
 */
mbed_error_status_t mbed_clear_all_errors(void)
{
    mbed_error_status_t status = MBED_SUCCESS;

    //Make sure we dont multiple clients resetting
    core_util_critical_section_enter();
    //Clear the error and context capturing buffer
    memset(&last_error_ctx, 0, sizeof(mbed_error_ctx));
    //reset error count to 0
    error_count = 0;
#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED
    status = mbed_error_hist_reset();
#endif
    core_util_critical_section_exit();

    return status;
}

static inline const char *name_or_unnamed(const char *name)
{
    return name ? name : "<unnamed>";
}

#if MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO && defined(MBED_CONF_RTOS_PRESENT)
/* Prints info of a thread(using osRtxThread_t struct)*/
static void print_thread(const osRtxThread_t *thread)
{
    mbed_error_printf("\n%s  State: 0x%" PRIX8 " Entry: 0x%08" PRIX32 " Stack Size: 0x%08" PRIX32 " Mem: 0x%08" PRIX32 " SP: 0x%08" PRIX32, name_or_unnamed(thread->name), thread->state, thread->thread_addr, thread->stack_size, (uint32_t)thread->stack_mem, thread->sp);
}

/* Prints thread info from a list */
static void print_threads_info(const osRtxThread_t *threads)
{
    while (threads != NULL) {
        print_thread(threads);
        threads = threads->thread_next;
    }
}
#endif

#ifndef NDEBUG
#define GET_TARGET_NAME_STR(tgt_name)   #tgt_name
#define GET_TARGET_NAME(tgt_name)       GET_TARGET_NAME_STR(tgt_name)
static void print_error_report(const mbed_error_ctx *ctx, const char *error_msg, const char *error_filename, int error_line)
{
    int error_code = MBED_GET_ERROR_CODE(ctx->error_status);
    int error_module = MBED_GET_ERROR_MODULE(ctx->error_status);

    mbed_error_printf("\n\n++ MbedOS Error Info ++\nError Status: 0x%X Code: %d Module: %d\nError Message: ", ctx->error_status, error_code, error_module);

    switch (error_code) {
        //These are errors reported by kernel handled from mbed_rtx_handlers
        case MBED_ERROR_CODE_RTOS_EVENT:
            mbed_error_printf("Kernel Error: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_THREAD_EVENT:
            mbed_error_printf("Thread: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_MUTEX_EVENT:
            mbed_error_printf("Mutex: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_SEMAPHORE_EVENT:
            mbed_error_printf("Semaphore: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_MEMORY_POOL_EVENT:
            mbed_error_printf("MemoryPool: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_EVENT_FLAGS_EVENT:
            mbed_error_printf("EventFlags: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_TIMER_EVENT:
            mbed_error_printf("Timer: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_RTOS_MESSAGE_QUEUE_EVENT:
            mbed_error_printf("MessageQueue: 0x%" PRIX32 ", ", ctx->error_value);
            break;

        case MBED_ERROR_CODE_ASSERTION_FAILED:
            mbed_error_printf("Assertion failed: ");
            break;

        default:
            //Nothing to do here, just print the error info down
            break;
    }
    mbed_error_puts(error_msg);
    mbed_error_printf("\nLocation: 0x%" PRIX32, ctx->error_address);

    /* We print the filename passed in, not any filename in the context. This
     * avoids the console print for mbed_error being limited to the presence
     * and length of the filename storage. Note that although the MBED_ERROR
     * macro compiles out filenames unless platform.error-filename-capture-enabled
     * is turned on, MBED_ASSERT always passes filenames, and other direct
     * users of mbed_error() may also choose to.
     */
    if (error_filename) {
        mbed_error_puts("\nFile: ");
        mbed_error_puts(error_filename);
        mbed_error_printf("+%d", error_line);
    }

    mbed_error_printf("\nError Value: 0x%" PRIX32, ctx->error_value);
#ifdef MBED_CONF_RTOS_PRESENT
    mbed_error_printf("\nCurrent Thread: %s  Id: 0x%" PRIX32 " Entry: 0x%" PRIX32 " StackSize: 0x%" PRIX32 " StackMem: 0x%" PRIX32 " SP: 0x%" PRIX32 " ",
                      name_or_unnamed(((osRtxThread_t *)ctx->thread_id)->name),
                      ctx->thread_id, ctx->thread_entry_address, ctx->thread_stack_size, ctx->thread_stack_mem, ctx->thread_current_sp);
#endif

#if MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO && defined(MBED_CONF_RTOS_PRESENT)
    mbed_error_printf("\nNext:");
    print_thread(osRtxInfo.thread.run.next);

    mbed_error_printf("\nReady:");
    print_threads_info(osRtxInfo.thread.ready.thread_list);

    mbed_error_printf("\nWait:");
    print_threads_info(osRtxInfo.thread.wait_list);

    mbed_error_printf("\nDelay:");
    print_threads_info(osRtxInfo.thread.delay_list);
#endif
#if !defined(MBED_SYS_STATS_ENABLED)
    mbed_error_printf("\nFor more info, visit: https://mbed.com/s/error?error=0x%08X&tgt=" GET_TARGET_NAME(TARGET_NAME), ctx->error_status);
#else
    mbed_stats_sys_t sys_stats;
    mbed_stats_sys_get(&sys_stats);
    mbed_error_printf("\nFor more info, visit: https://mbed.com/s/error?error=0x%08X&osver=%" PRId32 "&core=0x%08" PRIX32 "&comp=%d&ver=%" PRIu32 "&tgt=" GET_TARGET_NAME(TARGET_NAME), ctx->error_status, sys_stats.os_version, sys_stats.cpu_id, sys_stats.compiler_id, sys_stats.compiler_version);
#endif
    mbed_error_printf("\n-- MbedOS Error Info --\n");
}
#endif //ifndef NDEBUG


#if MBED_CONF_PLATFORM_ERROR_HIST_ENABLED
//Retrieve the error context from error log at the specified index
mbed_error_status_t mbed_get_error_hist_info(int index, mbed_error_ctx *error_info)
{
    return mbed_error_hist_get(index, error_info);
}

//Retrieve the error log count
int mbed_get_error_hist_count(void)
{
    return mbed_error_hist_get_count();
}

mbed_error_status_t mbed_save_error_hist(const char *path)
{
    mbed_error_status_t ret = MBED_SUCCESS;
    mbed_error_ctx ctx = {0};
    int log_count = mbed_error_hist_get_count();
    FILE *error_log_file = NULL;

    //Ensure path is valid
    if (path == NULL) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_ARGUMENT);
        goto exit;
    }

    //Open the file for saving the error log info
    if ((error_log_file = fopen(path, "w")) == NULL) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OPEN_FAILED);
        goto exit;
    }

    //First store the first and last errors
    if (fprintf(error_log_file, "\nFirst Error: Status:0x%x ThreadId:0x%x Address:0x%x Value:0x%x\n",
                (unsigned int)first_error_ctx.error_status,
                (unsigned int)first_error_ctx.thread_id,
                (unsigned int)first_error_ctx.error_address,
                (unsigned int)first_error_ctx.error_value) <= 0) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED);
        goto exit;
    }

    if (fprintf(error_log_file, "\nLast Error: Status:0x%x ThreadId:0x%x Address:0x%x Value:0x%x\n",
                (unsigned int)last_error_ctx.error_status,
                (unsigned int)last_error_ctx.thread_id,
                (unsigned int)last_error_ctx.error_address,
                (unsigned int)last_error_ctx.error_value) <= 0) {
        ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED);
        goto exit;
    }

    //Update with error log info
    while (--log_count >= 0) {
        mbed_error_hist_get(log_count, &ctx);
        //first line of file will be error log count
        if (fprintf(error_log_file, "\n%d: Status:0x%x ThreadId:0x%x Address:0x%x Value:0x%x\n",
                    log_count,
                    (unsigned int)ctx.error_status,
                    (unsigned int)ctx.thread_id,
                    (unsigned int)ctx.error_address,
                    (unsigned int)ctx.error_value) <= 0) {
            ret = MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_WRITE_FAILED);
            goto exit;
        }
    }

exit:
    fclose(error_log_file);

    return ret;
}
#endif
