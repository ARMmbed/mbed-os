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

#ifdef MBED_MINIMAL_PRINTF

#include "mbed_printf_implementation.h"

#include <limits.h>


#if defined(__ARMCC_VERSION) || defined(__ICCARM__)
#    define PREFIX(x)    $Sub$$##x
#elif defined(__GNUC__)
#    define PREFIX(x)    __wrap_##x
#else
#warning "This compiler is not yet supported."
#endif

int PREFIX(printf)(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stdout);
    va_end(arguments);

    return result;
}

int PREFIX(sprintf)(char *buffer, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int PREFIX(snprintf)(char *buffer, size_t length, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int PREFIX(vprintf)(const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stdout);
}

int PREFIX(vsprintf)(char *buffer, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments, NULL);
}

int PREFIX(vsnprintf)(char *buffer, size_t length, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
}

int PREFIX(fprintf)(FILE *stream, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stream);
    va_end(arguments);

    return result;
}

int PREFIX(vfprintf)(FILE *stream, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stream);
}

#endif // MBED_MINIMAL_PRINTF
