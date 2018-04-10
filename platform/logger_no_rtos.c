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

// In case of no rtos, prints from ISR mode will be discarded
#if !defined(MBED_CONF_RTOS_PRESENT) || defined(NDEBUG)

#include "platform/mbed_logger.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_critical.h"

#ifdef __cplusplus
extern "C" {
#endif

void mbed_logging_start(void)
{
}

void log_reset(void)
{
}

#if defined (MBED_ID_BASED_TRACING)
void log_buffer_id_data(uint32_t argCount, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        return;
    }
    fprintf(stderr, "%d ", argCount);
    va_list args;
    va_start(args, argCount);
    for (uint32_t i = 0; i < argCount; i++) {
        fprintf(stderr, "%d ", va_arg(args, int));
    }
    fputc('\n', stderr);
    va_end(args);
#endif
}

#else
void log_buffer_string_data(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    va_list args;
    va_start(args, format);
    log_buffer_string_vdata(format, args);
    va_end(args);
#endif
}

void log_buffer_string_vdata(const char *format, va_list args)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        return;
    }
    vfprintf(stderr, format, args);
    fputc('\n', stderr);
    if (mbed_log_valid_helper_data()) {
        mbed_log_helper_unlock();
    }
#endif
}
#endif

void log_assert(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    core_util_critical_section_enter();
    va_list args;
    va_start(args, format);
    mbed_error_vfprintf(format, args);
    va_end(args);
    mbed_die();
#endif
}

#ifdef __cplusplus
}
#endif

#endif
