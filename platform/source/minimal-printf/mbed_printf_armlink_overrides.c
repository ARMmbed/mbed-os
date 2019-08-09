/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/**
 * Arm Compiler uses dedicated functions for each format specifier used by
 * [sn/v/vsn]printf. When minimal-printf overwrites [sn/v/vsn]printf the
 * linker is unable to remove unused functions related to printf.
 *
 * The following stubs replace the built-in functions and helps the linker
 * to resolve dependencies and correctly remove unused functions.
 */

#ifdef MBED_MINIMAL_PRINTF

#if defined(TOOLCHAIN_ARM)

#include "mbed_printf_implementation.h"

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * Arm Compiler uses __2[s/sn/vsn]printf internally.
 */
int $Sub$$__2printf(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int $Sub$$__2sprintf(char *buffer, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int $Sub$$__2snprintf(char *buffer, size_t length, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int $Sub$$__2vprintf(char *buffer, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, LONG_MAX, format, arguments, NULL);
    va_end(arguments);

    return result;
}

int $Sub$$__2vsnprintf(char *buffer, size_t length, const char *format, va_list arguments)
{
    return mbed_minimal_formatted_string(buffer, length, format, arguments, NULL);
}

/**
 * Replace the built-in functions which the linker is unable to prune with dummy stubs
 * that take up less space.
 */
int $Sub$$_printf_a(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_c(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_char(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_char_common(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_char_file_locked(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_charcount(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_cs_common(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_d(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_dec(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_e(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_f(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_fp_dec(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_fp_dec_real(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_fp_hex(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_fp_hex_real(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_fp_infnan(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_g(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_hex_int_ll_ptr(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_hex_ptr(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_i(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_int_common(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_int_dec(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_int_hex(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_int_oct(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_l(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_lc(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_lcs_common(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_ll(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_ll_hex(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_ll_oct(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_lld(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_lli(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_llo(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_llu(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_llx(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_longlong_dec(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_longlong_hex(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_longlong_oct(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_ls(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_n(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_o(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_oct_int_ll(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_p(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_pad(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_percent(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_percent_end(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_s(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_str(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_string(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_truncate(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_truncate_signed(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_truncate_unsigned(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_u(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_wchar(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_wctomb(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_wstring(const char *format, ...)
{
    return 0;
}

int $Sub$$_printf_x(const char *format, ...)
{
    return 0;
}

#endif

#endif // MBED_MINIMAL_PRINTF