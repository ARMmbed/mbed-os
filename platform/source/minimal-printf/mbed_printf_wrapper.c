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

#ifdef MBED_MINIMAL_PRINTF

#include "mbed_printf_implementation.h"

#include <limits.h>


#if defined(__GNUC__)   /* GCC        */
#define SUPER_PRINTF     __real_printf
#define SUB_PRINTF       __wrap_printf
#define SUPER_SPRINTF    __real_sprintf
#define SUB_SPRINTF      __wrap_sprintf
#define SUPER_SNPRINTF   __real_snprintf
#define SUB_SNPRINTF     __wrap_snprintf
#define SUPER_VPRINTF    __real_vprintf
#define SUB_VPRINTF      __wrap_vprintf
#define SUPER_VSPRINTF   __real_vsprintf
#define SUB_VSPRINTF     __wrap_vsprintf
#define SUPER_VSNPRINTF  __real_vsnprintf
#define SUB_VSNPRINTF    __wrap_vsnprintf
#define SUPER_FPRINTF  __real_fprintf
#define SUB_FPRINTF    __wrap_fprintf
#define SUPER_VFPRINTF __real_vfprintf
#define SUB_VFPRINTF   __wrap_vfprintf
#elif defined(TOOLCHAIN_ARM) /* ARMC5/ARMC6 */\
 || defined(__ICCARM__)      /* IAR        */
#define SUPER_PRINTF     $Super$$printf
#define SUB_PRINTF       $Sub$$printf
#define SUPER_SPRINTF    $Super$$sprintf
#define SUB_SPRINTF      $Sub$$sprintf
#define SUPER_SNPRINTF   $Super$$snprintf
#define SUB_SNPRINTF     $Sub$$snprintf
#define SUPER_VPRINTF    $Super$$vprintf
#define SUB_VPRINTF      $Sub$$vprintf
#define SUPER_VSPRINTF   $Super$$vsprintf
#define SUB_VSPRINTF     $Sub$$vsprintf
#define SUPER_VSNPRINTF  $Super$$vsnprintf
#define SUB_VSNPRINTF    $Sub$$vsnprintf
#define SUPER_FPRINTF    $Super$$fprintf
#define SUB_FPRINTF      $Sub$$fprintf
#define SUPER_VFPRINTF   $Super$$vfprintf
#define SUB_VFPRINTF     $Sub$$vfprintf
#else
#warning "This compiler is not yet supported."
#endif

int SUB_PRINTF(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stdout);
    va_end(arguments);

    return result;
}

int SUB_SPRINTF(char *buffer, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int SUB_SNPRINTF(char *buffer, size_t length, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int SUB_VPRINTF(const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stdout);
}

int SUB_VSPRINTF(char *buffer, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments, NULL);
}

int SUB_VSNPRINTF(char *buffer, size_t length, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
}

int SUB_FPRINTF(FILE *stream, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stream);
    va_end(arguments);

    return result;
}

int SUB_VFPRINTF(FILE *stream, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, stream);
}

#endif // MBED_MINIMAL_PRINTF
