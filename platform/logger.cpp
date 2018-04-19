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
#include <stdlib.h>
#include "platform/logger_internal.h"
#include "hal/us_ticker_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_assert.h"
#include "platform/CircularBuffer.h"
#include "rtos/Thread.h"

using namespace rtos;
using namespace mbed;

// Globals related to ISR logging
static CircularBuffer <LOG_DATA_TYPE_, LOG_BUF_SIZE_> log_buffer_;

// Globals related to time printing
static const ticker_data_t *const log_ticker_ = get_us_ticker_data();
static bool time_enable_ = true;

// Globals related to external buffer capturing
static LOG_DATA_TYPE_ *extern_buf_ = NULL;
static uint32_t xbuf_count_ = 0;

static void log_update_buf(const char *data, int32_t size)
{
    if ((NULL == extern_buf_) || (NULL == data) || (size <= 0)) {
        return;
    }
    uint32_t count = 0;
    while(size--)
    {
        extern_buf_[xbuf_count_++] = data[count++];
        xbuf_count_ %= LOG_BUF_SIZE_;
    }
}

extern "C" void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES
    core_util_critical_section_enter();
    volatile uint64_t time = ticker_read_us(log_ticker_);
    va_list args;
    va_start(args, format);
    if (time_enable_) {
        mbed_error_printf("\n[%-8lld]", time);
    }
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

extern "C" void log_reset(void)
{
    core_util_critical_section_enter();
    time_enable_ = true;
    xbuf_count_ = 0;
    core_util_critical_section_exit();
}

extern "C" void log_buffer_data(LOG_DATA_TYPE_ *str)
{
    extern_buf_ = str;
}

extern "C" void log_disable_time_capture(void)
{
    core_util_critical_section_enter();
    time_enable_ = false;
    core_util_critical_section_exit();
}

extern "C" void log_enable_time_capture(void)
{
    core_util_critical_section_enter();
    time_enable_ = true;
    core_util_critical_section_exit();
}

#if defined (MBED_ID_BASED_TRACING)

// Note: ISR data is lossy, and with too many interrupts it might be
// over-written and last valid ISR data will be printed.
extern "C" void print_buf_data(void)
{
    LOG_DATA_TYPE_ data;
    while (1) {
        while (!log_buffer_.empty()) {
            log_buffer_.pop(data);
            fprintf(stderr, "0x%x ", data);
        }
        Thread::wait(1);
    }
}

// uint32_t time | uint32 (ID) | uint32 args ... | uint32_t checksum | 0
extern "C" void log_id_data(uint32_t argCount, ...)
{
    bool in_isr = false;
    LOG_DATA_TYPE_ buf[24];
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        core_util_critical_section_enter();
        in_isr = true;
    } else {
        mbed_log_lock();
        core_util_critical_section_enter();
        in_isr = false;
    }
    volatile uint64_t time = ticker_read_us(log_ticker_);
    LOG_DATA_TYPE_ checksum = 0;
    uint32_t count = 0;

    va_list args;
    va_start(args, argCount);
    MBED_ASSERT(argCount <= 20);
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
    buf[count++] = 0x0;
    va_end(args);

    if (NULL != extern_buf_) {
        log_update_buf(buf, count);
    } else {
        // Check buf size, if we have space for full line then only push
        if ( (LOG_BUF_SIZE_ - log_buffer_.size()) < count) {
            core_util_critical_section_exit();
            if (false == in_isr) {
                mbed_log_unlock();
            }
            return;
        }
        int32_t bytes_written = 0;
        while (count--) {
            log_buffer_.push(buf[bytes_written++]);
        }
        core_util_critical_section_exit();
        if (false == in_isr) {
            mbed_log_unlock();
        }
    }
}

#else // String based implementation

static void log_format_str_data(const char *format, va_list args, bool in_isr)
{
    LOG_DATA_TYPE_ one_line[LOG_SINGLE_STR_SIZE_ + LOG_SINGLE_HELPER_STR_SIZE_];
    int32_t size = LOG_SINGLE_STR_SIZE_ + LOG_SINGLE_HELPER_STR_SIZE_;

    volatile uint64_t time = ticker_read_us(log_ticker_);
    int32_t count = 0;
    if (time_enable_) {
        count = snprintf(one_line, size, "[%-8lld]", time);
        if (count < 0 || count > size) {
            return;
        }
    }
    count += vsnprintf(one_line + count, (size - count), format, args);
    if (count > size) {
        one_line[size-3] = '*';
        one_line[size-2] = '\n';
        one_line[size-1] = '\0';
        count = size;
    }
    if (NULL != extern_buf_) {
        log_update_buf(one_line, count);
    } else {
        if (true == in_isr) {
            count += 1;
            // Check buf size, if we have space for full line then only push
            if ( (LOG_BUF_SIZE_ - log_buffer_.size()) < count) {
                return;
            }
            int32_t bytes_written = 0;
            while (count--) {
                log_buffer_.push(one_line[bytes_written++]);
            }
        } else {
            fputs(one_line, stderr);
        }
    }
}

// Note: ISR data is lossy, and with too many interrupts it might be
// over-written and last valid ISR data will be printed.
extern "C" void print_buf_data(void)
{
    int32_t count = 0;
    LOG_DATA_TYPE_ one_line[LOG_SINGLE_STR_SIZE_];
    while (1) {
        while (!log_buffer_.empty()) {
            mbed_log_lock();
            log_buffer_.pop(one_line[count]);
            if ('\0' == one_line[count]) {
                fputs(one_line, stderr);
                count = 0;
            } else {
                count++;
                if (count >= LOG_SINGLE_STR_SIZE_) {
                    count = 0;
                }
            }
            mbed_log_unlock();
        }
        Thread::wait(1);
    }
}

extern "C" void log_str_data(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_str_vdata(format, args);
    va_end(args);
}

extern "C" void log_str_vdata(const char *format, va_list args)
{
    MBED_STATIC_ASSERT(LOG_SINGLE_STR_SIZE_ >= 32, "String size too small, min 32 bytes");

    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        core_util_critical_section_enter();
        log_format_str_data(format, args, true);
        core_util_critical_section_exit();
    } else {
        mbed_log_lock();
        log_format_str_data(format, args, false);
        mbed_log_unlock_all();
    }
}
#endif

#endif
