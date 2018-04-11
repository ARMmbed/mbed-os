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

#if defined(MBED_CONF_RTOS_PRESENT) && !defined(NDEBUG)
#include <string.h>
#include "platform/logger_internal.h"
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"
#include "mbed_events.h"

using namespace mbed;

#if defined (MBED_ID_BASED_TRACING)
static LOG_DATA_TYPE_ log_id_buf_[LOG_SINGLE_STR_SIZE_];
static uint32_t log_count_ = 0;
#endif

// Globals related to ISR logging
static LOG_DATA_TYPE_ log_isr_str_[LOG_SINGLE_STR_SIZE_];
static EventQueue *log_isr_equeue_;

// Globals related to time printing
static const ticker_data_t *const log_ticker_ = get_us_ticker_data();
static bool time_enable_ = true;

// Globals related to external buffer capturing
static LOG_DATA_TYPE_ *extern_buf_ = NULL;
static uint32_t xbuf_count_ = 0;

static void log_update_buf(const char *data, uint32_t size)
{
    if ((NULL == extern_buf_) || (NULL == data) || (0 == size)) {
        return;
    }
    uint32_t count = 0;
    while(size--)
    {
        extern_buf_[xbuf_count_++] = data[count++];
        xbuf_count_ %= MBED_CONF_EXTERNAL_BUFFER_SIZE;
    }
}

#if defined (MBED_ID_BASED_TRACING)
// Note: ISR data is lossy, and with too many interrupts it might be
// over-written and last valid ISR data will be printed.
static void log_isr_id_data()
{
    mbed_log_helper_lock();
    uint32_t count = 0;
    while(log_count_--) {
        fprintf(stderr, "0x%x ", log_isr_str_[count++]);
    }
    mbed_log_helper_unlock();
}
#else
// Note: ISR data is lossy, and with too many interrupts it might be
// over-written and last valid ISR data will be printed.
static void log_isr_queue()
{
    mbed_log_helper_lock();
    fputs(log_isr_str_, stderr);
    mbed_log_helper_unlock();
}

static void log_str_isr_data(const char *format, va_list args)
{
    core_util_critical_section_enter();
    volatile uint64_t time = ticker_read_us(log_ticker_);
    uint32_t count = 0;
    if (time_enable_) {
        count = snprintf(log_isr_str_, LOG_SINGLE_STR_SIZE_, "[%-8lld]", time);
    }
    vsnprintf(log_isr_str_+count, (LOG_SINGLE_STR_SIZE_-count), format, args);
    if (NULL != extern_buf_) {
        log_update_buf(log_isr_str_, strlen(log_isr_str_));
    } else {
        log_isr_equeue_->call(log_isr_queue);
    }
    core_util_critical_section_exit();
}

static void log_str_usr_data(const char *format, va_list args)
{
    mbed_log_helper_lock();
    LOG_DATA_TYPE_ one_line[LOG_SINGLE_STR_SIZE_ << 1];
    uint32_t size = (LOG_SINGLE_STR_SIZE_ << 1);

    volatile uint64_t time = ticker_read_us(log_ticker_);
    uint32_t count = 0;
    if (time_enable_) {
        count = snprintf(one_line, size, "[%-8lld]", time);
    }
    vsnprintf(one_line+count, (size-count), format, args);

    if (NULL != extern_buf_) {
        log_update_buf(one_line, strlen(one_line));
    } else {
        fputs(one_line, stderr);
    }
    mbed_log_helper_unlock_all();
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

void mbed_logging_start(void)
{
    log_isr_equeue_ = mbed_highprio_event_queue();
    MBED_ASSERT(log_isr_equeue_ != NULL);
}

void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES
    volatile uint64_t time = ticker_read_us(log_ticker_);
#define ASSERT_BUF_LENGTH   10
    char data[ASSERT_BUF_LENGTH];
    memset((void *)data, 0, ASSERT_BUF_LENGTH);

    core_util_critical_section_enter();
    va_list args;
    va_start(args, format);
    if (time_enable_) {
        mbed_error_printf("[%-8lld]", time);
    }
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

void log_reset(void)
{
    core_util_critical_section_enter();
    time_enable_ = true;
    xbuf_count_ = 0;
    core_util_critical_section_exit();
}

void log_buffer_data(LOG_DATA_TYPE_ *str)
{
    extern_buf_ = str;
}

void log_disable_time_capture(void)
{
    core_util_critical_section_enter();
    time_enable_ = false;
    core_util_critical_section_exit();
}

void log_enable_time_capture(void)
{
    core_util_critical_section_enter();
    time_enable_ = true;
    core_util_critical_section_exit();
}

#if defined (MBED_ID_BASED_TRACING)
// uint32_t time | uint32 (ID) | uint32 args ... | uint32_t checksum | 0
void log_id_data(uint32_t argCount, ...)
{
    bool in_isr = false;
    LOG_DATA_TYPE_ *buf;
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        core_util_critical_section_enter();
        in_isr = true;
        buf = log_isr_str_;
    } else {
        mbed_log_helper_lock();
        core_util_critical_section_enter();
        buf = log_id_buf_;
        in_isr = false;
    }
    volatile uint64_t time = ticker_read_us(log_ticker_);
    LOG_DATA_TYPE_ checksum = 0;
    uint32_t count = 0;

    va_list args;
    va_start(args, argCount);
    if (time_enable_) {
        buf[count] = (LOG_DATA_TYPE_)(time/1000);
        checksum ^= buf[count];
        count++;
    }
    for (uint32_t i = 0; i < argCount; i++) {
        buf[count] = va_arg(args, uint32_t);
        checksum ^= buf[count];
        count++;
    }
    buf[count++] = checksum;
    va_end(args);

    if (NULL != extern_buf_) {
        buf[count++] = 0x0;
        log_update_buf(buf, count);
    } else {
        if (in_isr) {
            log_count = count;
            log_isr_equeue_->call(log_isr_id_data);
            core_util_critical_section_exit();
        } else {
            core_util_critical_section_exit();
            uint32_t i = 0;
            while(count--) {
                fprintf(stderr, "0x%x ", buf[i++]);
            }
            mbed_log_helper_unlock();
        }
    }
}

#else // String based implementation
void log_str_data(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_str_vdata(format, args);
    va_end(args);
}

void log_str_vdata(const char *format, va_list args)
{
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        log_str_isr_data(format, args);
    } else {
        log_str_usr_data(format, args);
    }
}
#endif

#ifdef __cplusplus
}
#endif

#endif
