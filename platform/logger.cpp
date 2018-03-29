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

#if defined(MBED_CONF_RTOS_PRESENT) && !defined(MBED_CONF_ZERO_BUFFER_LOGGING) && !defined(NDEBUG)
#include <string.h>
#include "platform/logger.h"
#include "platform/CircularBuffer.h"
#include "platform/mbed_wait_api.h"
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"
#include "rtos/EventFlags.h"

#define LOG_MAX_BUFFER_SIZE_            (MBED_CONF_LOG_MAX_BUFFER_SIZE/sizeof(LOG_DATA_TYPE_))
// Circular buffer should always have space for helper functions
// in same line
#define LOG_FREE_BUF_FLAG_              0x1u

using namespace rtos;
using namespace mbed;

static CircularBuffer <LOG_DATA_TYPE_, LOG_MAX_BUFFER_SIZE_> log_buffer;
static const ticker_data_t *const log_ticker = get_us_ticker_data();
static uint32_t log_bytes_lost = 0;
static EventFlags log_event_flag("logging");
static LOG_DATA_TYPE_ *extern_buf = NULL;
static uint32_t xbuf_count = 0;
static bool time_enable = true;
#ifdef __cplusplus
extern "C" {
#endif

void log_print_data(void)
{
    LOG_DATA_TYPE_ data = 0;
    uint32_t count = 0;
    while (1) {
        while (!log_buffer.empty()) {
            log_buffer.pop(data);

            if (NULL != extern_buf) {
                extern_buf[xbuf_count++] = data;
                xbuf_count %= MBED_CONF_LOG_MAX_BUFFER_SIZE;
            } else {
#if DEVICE_STDIO_MESSAGES
#if defined (MBED_ID_BASED_TRACING)
                fprintf(stderr, "0x%x ", data);
#else
                fputc(data, stderr);
#endif
#endif
            }
            if(++count > (LOG_SINGLE_STR_SIZE_ << 1)) {
                break;
            }
        }
        log_event_flag.set(LOG_FREE_BUF_FLAG_);
    }
}

void log_reset(void)
{
    log_buffer.reset();
    time_enable = true;
    xbuf_count = 0;
}

uint32_t log_get_bytes_lost(void)
{
    return log_bytes_lost;
}

void log_buffer_data(LOG_DATA_TYPE_ *str) {
    extern_buf = str;
}

void log_disable_time_capture(void) {
    core_util_critical_section_enter();
    time_enable = false;
    core_util_critical_section_exit();
}

void log_enable_time_capture(void) {
    core_util_critical_section_enter();
    time_enable = true;
    core_util_critical_section_exit();
}

#if defined (MBED_ID_BASED_TRACING)
static inline void log_push_data(const LOG_DATA_TYPE_ data)
{
    if (log_buffer.full()) {
        log_bytes_lost++;
    }
    log_buffer.push(data);
}

// uint32_t time | uint32 (ID) | uint32 args ... | uint32_t checksum | 0
void log_buffer_id_data(uint32_t argCount, ...)
{
    volatile uint64_t time = ticker_read_us(log_ticker);
    data = (LOG_DATA_TYPE_)(time/1000);
    LOG_DATA_TYPE_ data = 0;
    LOG_DATA_TYPE_ checksum = 0;

    va_list args;
    va_start(args, argCount);
    core_util_critical_section_enter();
    if (time_enable) {
        log_push_data(data);
        checksum ^= data;
    }
    for (uint32_t i = 0; i < argCount; i++) {
        data = va_arg(args, uint32_t);
        log_push_data(data);
        checksum ^= data;
    }
    log_push_data(checksum);
    log_push_data(0x0);
    core_util_critical_section_exit();
    va_end(args);
}

extern void log_terminate_thread(void);
void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES
    volatile uint64_t time = ticker_read_us(log_ticker);
    uint32_t data;

    log_terminate_thread();

    // Empty the thread buffer first
    while (!log_buffer.empty()) {
        log_buffer.pop(data);
        mbed_error_printf("0x%x ", data);
    }
    core_util_critical_section_enter();
    va_list args;
    va_start(args, format);
    if (time_enable) {
        mbed_error_printf("\n[%-8lld]", time);
    }
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

#else // String based implementation
// Push data in critical section, this is for ISR mode and could be lossy
static void log_push_data_lossy(const LOG_DATA_TYPE_ *data, uint32_t count)
{
    MBED_ASSERT(true == core_util_in_critical_section());
    uint32_t bytes_written = 0;
    while (count--) {
        if (log_buffer.full()) {
            log_buffer.push('*');
            log_buffer.push('\n');
            log_bytes_lost += count;
            break;
        } else {
            log_buffer.push(data[bytes_written++]);
        }
    }
}

// Push data in thread mode
#define LOG_RETRY_COUNT     3
static void log_push_data_blocking(const LOG_DATA_TYPE_ *data, uint32_t count, uint32_t buf_limit)
{
    MBED_ASSERT(true == core_util_in_critical_section());
    uint32_t bytes_written = 0;
    uint32_t retry = LOG_RETRY_COUNT;

    // Get the buffer length and when size more then that is available push all
    // data in one go
    while (retry--) {
        if (buf_limit > (LOG_MAX_BUFFER_SIZE_ - log_buffer.size())) {
            core_util_critical_section_exit();
            log_event_flag.clear(LOG_FREE_BUF_FLAG_);
            uint32_t flag = log_event_flag.wait_any(LOG_FREE_BUF_FLAG_);
            MBED_ASSERT(true != (flag & osFlagsError));
            core_util_critical_section_enter();
        }
        else {
            break;
        }
    }
    while (count--) {
        if (log_buffer.full()) {
            log_buffer.push('*');
            log_buffer.push('\n');
            log_bytes_lost += count;
            break;
        } else {
            log_buffer.push(data[bytes_written++]);
        }
    }
}

void log_buffer_string_data(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_buffer_string_vdata(format, args);
    va_end(args);
}

void log_buffer_string_vdata(const char *format, va_list args)
{
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        log_buffer_string_vdata_critical(format, args, true);
    } else {
        mbed_log_helper_lock();
        log_buffer_string_vdata_critical(format, args, false);
        mbed_log_helper_unlock_all();
    }
}

void log_buffer_string_vdata_critical(const char *format, va_list args, uint8_t lossy)
{
    volatile uint64_t time = ticker_read_us(log_ticker);
    LOG_DATA_TYPE_ one_line[LOG_SINGLE_STR_SIZE_+1];
    uint32_t count = 0;
    if (time_enable) {
        count = snprintf(one_line, LOG_SINGLE_STR_SIZE_, "[%-8lld]", time);
    }
    vsnprintf(one_line+count, (LOG_SINGLE_STR_SIZE_-count), format, args);
    count = strlen(one_line);

    core_util_critical_section_enter();
    if (true == lossy) {
        one_line[count++] = '\n';
        one_line[count] = '\0';
        log_push_data_lossy(one_line, count);
    } else {
        if (mbed_log_valid_helper_data()) {
            log_push_data_blocking(one_line, count, (LOG_SINGLE_STR_SIZE_ << 1));
            char *str = mbed_log_get_helper_data();
            count = strlen(str);
            str[count++] = '\n';
            str[count] = '\0';
            log_push_data_blocking(str, count, 0);
        } else {
            one_line[count++] = '\n';
            one_line[count] = '\0';
            log_push_data_blocking(one_line, count, count);
        }
    }
    core_util_critical_section_exit();
}

extern void log_terminate_thread(void);
void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES
    volatile uint64_t time = ticker_read_us(log_ticker);
#define ASSERT_BUF_LENGTH   10
    char data[ASSERT_BUF_LENGTH];
    memset((void *)data, 0, ASSERT_BUF_LENGTH);
    int count = 0;

    log_terminate_thread();

    // Empty the thread buffer first
    while (!log_buffer.empty()) {
        log_buffer.pop(data[count++]);
        if (count == ASSERT_BUF_LENGTH) {
            mbed_error_printf("%s", data);
            memset((void *)data, 0, ASSERT_BUF_LENGTH);
            count = 0;
        }
    }
    if (count) {
        mbed_error_printf("%s", data);
        count = 0;
    }

    core_util_critical_section_enter();
    va_list args;
    va_start(args, format);
    if (time_enable) {
        mbed_error_printf("\n[%-8lld]", time);
    }
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

#endif

#ifdef __cplusplus
}
#endif

#endif
