/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if defined(TOOLCHAIN_GCC)
#define SUB_PRINTF       __wrap_printf
#define SUB_SNPRINTF     __wrap_snprintf
#define SUB_SPRINTF      __wrap_sprintf
#define SUB_VSNPRINTF    __wrap_vsnprintf
#elif defined(TOOLCHAIN_ARM)
#define SUPER_PRINTF     $Super$$printf
#define SUB_PRINTF       $Sub$$printf
#define SUPER_SNPRINTF   $Super$$snprintf
#define SUB_SNPRINTF     $Sub$$snprintf
#define SUPER_SPRINTF    $Super$$sprintf
#define SUB_SPRINTF      $Sub$$sprintf
#define SUPER_VSNPRINTF  $Super$$vsnprintf
#define SUB_VSNPRINTF    $Sub$$vsnprintf
#elif defined(__ICCARM__)
#define SUPER_PRINTF     $Super$$__iar_printf
#define SUB_PRINTF       $Sub$$__iar_printf
#define SUPER_SNPRINTF   $Super$$__iar_snprintf
#define SUB_SNPRINTF     $Sub$$__iar_snprintf
#define SUPER_SPRINTF    $Super$$__iar_sprintf
#define SUB_SPRINTF      $Sub$$__iar_sprintf
#define SUPER_VSNPRINTF  $Super$$__iar_vsnprintf
#define SUB_VSNPRINTF    $Sub$$__iar_vsnprintf
#endif

int SUB_PRINTF(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments);
    va_end(arguments);

    return result;
}

int SUB_SNPRINTF(char* buffer, size_t length, const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, length, format, arguments);
    va_end(arguments);

    return result;
}

int SUB_SPRINTF(char* buffer, const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments);
    va_end(arguments);

    return result;
}

int SUB_VSNPRINTF(char* buffer, size_t length, const char* format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, length, format, arguments);
}
