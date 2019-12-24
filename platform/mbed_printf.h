/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBED_PRINTF_H
#define MBED_PRINTF_H

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_printf Mbed printf family functions
 *
 * Library supports both printf and snprintf in 1252 bytes of flash.
 *
 * Prints directly to stdio/UART without using malloc. All flags and precision modifiers are ignored.
 * There is no error handling if a writing error occurs.
 *
 * Supports:
 * * %d: signed integer [h, hh, (none), l, ll, z, j, t].
 * * %i: signed integer [h, hh, (none), l, ll, z, j, t].
 * * %u: unsigned integer [h, hh, (none), l, ll, z, j, t].
 * * %x: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., ff).
 * * %X: unsigned integer [h, hh, (none), l, ll, z, j, t], printed as hexadecimal number (e.g., FF).
 * * %f: floating point (enabled by default).
 * * %F: floating point (enabled by default, treated as %f).
 * * %g: floating point (enabled by default, treated as %f).
 * * %G: floating point (enabled by default, treated as %f).
 * * %c: character.
 * * %s: string.
 * * %p: pointer (e.g. 0x00123456).
 *
 * Unrecognized format specifiers are treated as ordinary characters.
 *
 * Floating point limitations:
 * * All floating points are treated as %f.
 * * No support for inf, infinity or nan
 *
 * For more information have a look at mbed-os/platform/source/minimal-printf/README.md
 *
 * @{
 */

/**
 * Minimal printf
 *
 * Prints directly to stdio/UART without using malloc.
 */
int mbed_printf(const char *format, ...);

/**
 * Minimal snprintf
 *
 * Prints directly to buffer without using malloc.
 */
int mbed_snprintf(char *buffer, size_t length, const char *format, ...);

/**
 * Minimal snprintf
 *
 * Prints directly to buffer without using malloc.
 */
int mbed_sprintf(char *buffer, const char *format, ...);

/**
 * Minimal printf
 *
 * Prints directly to stdio/UART without using malloc.
 */
int mbed_vprintf(const char *format, va_list arguments);

/**
 * Minimal snprintf
 *
 * Prints directly to buffer without using malloc.
 */
int mbed_vsnprintf(char *buffer, size_t length, const char *format, va_list arguments);

/**
 * Minimal sprintf
 *
 * Prints directly to buffer without using malloc.
 */
int mbed_vsprintf(char *buffer, const char *format, va_list arguments);

/**
 * Minimal fprintf
 *
 * Prints directly to file stream without using malloc.
 */
int mbed_fprintf(FILE *stream, const char *format, ...);

/**
 * Minimal vfprintf
 *
 * Prints directly to file stream without using malloc.
 */
int mbed_vfprintf(FILE *stream, const char *format, va_list arguments);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // MBED_PRINTF_H
