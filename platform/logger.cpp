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
#include "platform/mbed_logger.h"
#include "platform/CircularBuffer.h"
#include "platform/mbed_wait_api.h"
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"

using namespace mbed;

#if defined (MBED_ID_BASED_TRACING)
#define LOG_DATA_TYPE   uint32_t
CircularBuffer <LOG_DATA_TYPE, (MBED_CONF_LOG_MAX_BUFFER_SIZE/4)> log_buffer;
#else
#define LOG_DATA_TYPE   char
CircularBuffer <LOG_DATA_TYPE, MBED_CONF_LOG_MAX_BUFFER_SIZE> log_buffer;
#endif
static const ticker_data_t *const log_ticker = get_us_ticker_data();
static uint32_t logger_bytes_lost = 0;

#ifdef __cplusplus
extern "C" {
#endif

void log_reset(void)
{
    core_util_critical_section_enter();
    log_buffer.reset();
    logger_bytes_lost = 0;
    core_util_critical_section_exit();
}

uint32_t log_bytes_lost(void)
{
    return logger_bytes_lost;
}

// Note: this call should be inside the critical section
static inline void log_push_data(LOG_DATA_TYPE data)
{
    if (log_buffer.full()) {
        logger_bytes_lost++;
    }
    log_buffer.push(data);
}

void log_thread(void)
{
    LOG_DATA_TYPE data = 0;
    while (1) {
        while (!log_buffer.empty()) {
            log_buffer.pop(data);
#if DEVICE_STDIO_MESSAGES
#if defined (MBED_ID_BASED_TRACING)
            fprintf(stderr, "0x%x ", data);
#else
            fputc(data, stderr);
#endif
#endif
        }
        wait_ms(1);
    }
}

#if defined (MBED_ID_BASED_TRACING)
// uint32_t time | uint32 (ID) | uint32 args ... | uint32_t checksum | 0
void log_buffer_id_data(uint8_t argCount, ...)
{
    volatile uint64_t time = ticker_read_us(log_ticker);
    LOG_DATA_TYPE data = 0;
    LOG_DATA_TYPE checksum = 0;

    data = (LOG_DATA_TYPE)(time/1000);

    va_list args;
    va_start(args, argCount);
    core_util_critical_section_enter();
    log_push_data(data);
    checksum ^= data;
    for (uint8_t i = 0; i < argCount; i++) {
        data = va_arg(args, uint32_t);
        log_push_data(data);
        checksum ^= data;
    }
    log_push_data(checksum);
    log_push_data(0x0);
    core_util_critical_section_exit();
    va_end(args);
}

void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES
    volatile uint64_t time = ticker_read_us(log_ticker);
    uint32_t data;

    // Empty the thread buffer first
    while (!log_buffer.empty()) {
        log_buffer.pop(data);
        mbed_error_printf("0x%x ", data);
    }
    core_util_critical_section_enter();
    va_list args;
    va_start(args, format);
    mbed_error_printf("\n[%-8lld]", time);
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

#else // String based implementation
void log_buffer_string_data(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_buffer_string_vdata(format, args);
    va_end(args);
}

void log_buffer_string_vdata(const char *format, va_list args)
{
    volatile uint64_t time = ticker_read_us(log_ticker);
    char one_line[MBED_CONF_MAX_LOG_STR_SIZE];
    uint8_t count = snprintf(one_line, MBED_CONF_MAX_LOG_STR_SIZE, "[%-8lld]", time);
    uint8_t bytes_written = 0;

    vsnprintf(one_line+count, (MBED_CONF_MAX_LOG_STR_SIZE-count), format, args);
    count = strlen(one_line);

    core_util_critical_section_enter();
    while (count--) {
        log_push_data(one_line[bytes_written++]);
    }
    if (mbed_log_valid_helper_data()) {
        char *str = mbed_log_get_helper_data();
        bytes_written = 0;
        count = strlen(str);
        while (count--) {
            log_push_data(str[bytes_written++]);
        }
        mbed_log_helper_unlock();
    }
    log_push_data('\n');
    core_util_critical_section_exit();
}

void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES
    volatile uint64_t time = ticker_read_us(log_ticker);
#define ASSERT_BUF_LENGTH   10
    char data[ASSERT_BUF_LENGTH];
    memset((void *)data, 0, ASSERT_BUF_LENGTH);
    int count = 0;

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
    mbed_error_printf("\n[%-8lld]", time);
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
