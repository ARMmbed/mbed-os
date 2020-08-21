/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "mbed_printf_implementation.h"

#include <limits.h>


int mbed_printf(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stdout);
    va_end(arguments);

    return result;
}

int mbed_snprintf(char *buffer, size_t length, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int mbed_vprintf(const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stdout);
}

int mbed_vsnprintf(char *buffer, size_t length, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
}

int mbed_fprintf(FILE *stream, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stream);
    va_end(arguments);

    return result;
}

int mbed_vfprintf(FILE *stream, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stream);
}
