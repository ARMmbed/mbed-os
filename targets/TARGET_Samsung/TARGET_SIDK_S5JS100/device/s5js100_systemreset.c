/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/* @file  : s5js100_systemreset.c
 * @brief : board reset source file
 * @date  : June 2019
 *
 * @note  : Add chip dependent feature and more detail error status
 *
 */

/****************************************************************************
  Included Files
 ****************************************************************************/
#include "cmsis.h"
#include "mbed_crash_data_offsets.h"
#include "mbed_retarget.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include "mbed_error_hist.h"
#include "mbed_interface.h"
#include "mbed_power_mgmt.h"
#include "mbed_stats.h"
#include "mbed_atomic.h"
#include "mbed_fault_handler.h"

#include "rtx_os.h"
#include <inttypes.h>
#include <string.h>

#ifndef NDEBUG
#define ERROR_REPORT(ctx, error_msg, error_filename, error_line) print_error_report(ctx, error_msg, error_filename, error_line)
static void print_error_report(const mbed_error_ctx *ctx, const char *, const char *error_filename, int error_line);
#else
#define ERROR_REPORT(ctx, error_msg, error_filename, error_line) ((void) 0)
#endif

extern mbed_fault_context_t *mbed_fault_context;
static int error_count = 0;
static mbed_error_ctx first_error_ctx = {0};
static mbed_error_ctx last_error_ctx = {0};
static mbed_error_hook_t error_hook = NULL;
static mbed_error_status_t handle_error(mbed_error_status_t error_status, unsigned int error_value, const char *filename, int line_number, void *caller);

static inline const char *name_or_unnamed(const char *name)
{
    return name ? name : "<unnamed>";
}


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




/****************************************************************************
 * Name: up_systemreset
 *
 * Description:
 *   Internal, reset logic.
 *
 ****************************************************************************/
static void up_systemreset(void)
{
    putreg32(0x1, 0x8301100C);
    putreg32(0x1 << 1, 0x82020018);

    putreg32(0x4, 0x83011000); // enable watchdog
    putreg32(0x1, 0x83011010);
    putreg32(0x1, 0x83011020);
    putreg32(327, 0x83011004);  //set 10ms to be reset , 1 sec=32768
    putreg32(0xFF, 0x83011008); // force to load value to be reset

    /* Wait for the reset */
    for (; ;) {
    }
}

/****************************************************************************
 * Name: board_reset
 *
 * Description:
 *   Reset board.  This function may or may not be supported by a
 *   particular board architecture.
 *
 * Input Parameters:
 *   status - Status information provided with the reset event.  This
 *     meaning of this status information is board-specific.  If not used by
 *     a board, the value zero may be provided in calls to board_reset.
 *
 * Returned Value:
 *   If this function returns, then it was not possible to power-off the
 *   board due to some constraints.  The return value int this case is a
 *   board-specific reason for the failure to shutdown.
 *
 ****************************************************************************/
int board_reset(void)
{
    up_systemreset();
    return 0;
}
//Set an error status with the error handling system
static mbed_error_status_t handle_error(mbed_error_status_t error_status, unsigned int error_value, const char *filename, int line_number, void *caller)
{
    mbed_error_ctx current_error_ctx;

    //Error status should always be < 0
    if (error_status >= 0) {
        //This is a weird situation, someone called mbed_error with invalid error code.
        //We will still handle the situation but change the error code to ERROR_INVALID_ARGUMENT, atleast the context will have info on who called it
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

#define CONTAINER_OF(ptr, type, field) \
    ((type *)(((char *)(ptr)) - offsetof(type, field)))

mbed_error_status_t mbed_error(int error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number)
{
    int i;
    uint32_t current_psp;
    //set the error reported
    (void) handle_error(error_status, error_value, filename, line_number, MBED_CALLER_ADDR());

    //On fatal errors print the error context/report
    ERROR_REPORT(&last_error_ctx, error_msg, filename, line_number);
    //mbed_error_printf("\nPSP 0x%x\n", mbed_fault_context->PSP);
    current_psp = mbed_fault_context->PSP;
    mbed_error_printf("dump PSP[0x%x] \n", (unsigned int)current_psp);
    mbed_error_printf("======================     CUT     ========================= \n");

    if (current_psp != 0x0) {
        //dump stack contents
        for (i = 0; getreg32(current_psp) != 0xDEADBEEF; i++) {
            if (i % 0x8 == 0) {
                mbed_error_printf("\n");
                mbed_error_printf("0x%.8x:", (unsigned int)current_psp);
            }
            mbed_error_printf("0x%.8x ", getreg32(current_psp));
            current_psp += 4;
        }
        // last STACK Pointer
        mbed_error_printf("0x%.8x ", getreg32(current_psp));
    }
    mbed_error_printf("\n======================FOR ERROR DUMP ========================= \n");
    while (1);
}
