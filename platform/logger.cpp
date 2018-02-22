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
CircularBuffer <uint32_t, (MBED_CONF_LOG_MAX_BUFFER_SIZE/4)> log_buffer;
#else
CircularBuffer <char, MBED_CONF_LOG_MAX_BUFFER_SIZE> log_buffer;
#endif
const ticker_data_t *const log_ticker = get_us_ticker_data();

#ifdef __cplusplus
extern "C" {
#endif

#if defined (MBED_ID_BASED_TRACING)
void log_thread(void)
{
    uint32_t data = 0;
    while (1) {
        while (!log_buffer.empty()) {
            log_buffer.pop(data);
#if DEVICE_STDIO_MESSAGES
            fprintf(stderr, "0x%x ", data);
#endif
        }
        wait_ms(1);
    }
}

// uint32_t time | uint32 (ID) | uint32 args ... | uint32_t checksum | 0
void log_buffer_id_data(uint8_t argCount, ...)
{
    volatile uint64_t time = ticker_read_us(log_ticker);
    uint32_t data = 0;
    uint32_t checksum = 0;

    data = (uint32_t)(time/1000);

    va_list args;
    va_start(args, argCount);
    core_util_critical_section_enter();
    log_buffer.push(data);
    checksum ^= data;
    for (uint8_t i = 0; i < argCount; i++) {
        data = va_arg(args, uint32_t);
        log_buffer.push(data);
        checksum ^= data;
    }
    log_buffer.push(checksum);
    log_buffer.push(0x0);
    core_util_critical_section_exit();
    va_end(args);
}

void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
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
    mbed_error_printf("\n %-8lld ", time);
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

#else
void log_thread(void)
{
    char data;
    while (1) {
        while (!log_buffer.empty()) {
            log_buffer.pop(data);
#if DEVICE_STDIO_MESSAGES
            fputc(data, stderr);
#endif
        }
        wait_ms(1);
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
    volatile uint64_t time = ticker_read_us(log_ticker);
    char one_line[MBED_CONF_MAX_LOG_STR_SIZE];
    uint8_t count = snprintf(one_line, MBED_CONF_MAX_LOG_STR_SIZE, "%-8lld ", time);
    uint8_t bytes_written = 0;

    vsnprintf(one_line+count, (MBED_CONF_MAX_LOG_STR_SIZE-count), format, args);
    count = strlen(one_line);
    snprintf(one_line+count, (MBED_CONF_MAX_LOG_STR_SIZE-count), "\n");
    count = strlen(one_line);

    core_util_critical_section_enter();
    while (count--) {
        log_buffer.push(one_line[bytes_written++]);
    }
    core_util_critical_section_exit();
}

void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
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
    mbed_error_printf("\n%-8lld ", time);
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
