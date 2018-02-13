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

#if defined(MBED_CONF_ZERO_BUFFER_LOGGING) || !defined(MBED_CONF_RTOS_PRESENT)

#include "platform/mbed_logger.h"

#ifdef __cplusplus
extern "C" {
#endif

void log_buffer_string_data(const char *format, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
#endif
}

void log_buffer_id_data(uint8_t argCount, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    printf("%d ", argCount);
    va_list args;
    va_start(args, argCount);
    for (uint8_t i = 0; i < argCount; i++) {
        printf("%d ", va_arg(args, int));
    }
    printf("\n");
    va_end(args);
#endif
}

#ifdef __cplusplus
}
#endif

#endif
