/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 ARM Limited
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

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#if !TARGET_LIKE_MBED
/* Linux implementation is for debug only */
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT 1
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS 6
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT 1
#endif

#ifndef MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT 0
#endif

#ifndef MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS 6
#endif

#ifndef MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT
#define MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT 1
#endif

/**
 * Check architecture and choose storage data type.
 * On 32 bit machines, the default storage type is 32 bit wide
 * unless 64 bit integers are enabled in the configuration.
 */
#if INTPTR_MAX == INT32_MAX
#define MBED_SIGNED_NATIVE_TYPE int32_t
#define MBED_UNSIGNED_NATIVE_TYPE uint32_t
#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT
#define MBED_SIGNED_STORAGE int64_t
#define MBED_UNSIGNED_STORAGE uint64_t
#else
#define MBED_SIGNED_STORAGE int32_t
#define MBED_UNSIGNED_STORAGE uint32_t
#endif

#elif INTPTR_MAX == INT64_MAX
#define MBED_SIGNED_NATIVE_TYPE int64_t
#define MBED_UNSIGNED_NATIVE_TYPE uint64_t
#define MBED_SIGNED_STORAGE int64_t
#define MBED_UNSIGNED_STORAGE uint64_t
#else
#error unsupported architecture
#endif

/**
 * Precision defines
 */
#define PRECISION_DEFAULT (INT_MAX)

/**
 * Enum for storing width modifier.
 */
typedef enum {
    LENGTH_NONE         = 0x00,
    LENGTH_H            = 0x11,
    LENGTH_L            = 0x21,
    LENGTH_J            = 0x31,
    LENGTH_Z            = 0x41,
    LENGTH_T            = 0x51,
    LENGTH_CAPITAL_L    = 0x61,
    LENGTH_HH           = 0x72,
    LENGTH_LL           = 0x82
} length_t;

/**
 * Enum for integer printing type
 */
typedef enum {
    INT_UNSIGNED,
    INT_SIGNED,
    HEX_LOWER,
    HEX_UPPER,
    ZERO_NEGATIVE /* special case when printing integer part of double values where it is 0 and the value is negative */
} integer_type_t;

/**
 * Prototypes
 */
static void mbed_minimal_formatted_string_integer(char *buffer, size_t length, int *result, MBED_UNSIGNED_STORAGE value, integer_type_t type, int width_size, bool prepend_zeros, FILE *stream);
static void mbed_minimal_formatted_string_void_pointer(char *buffer, size_t length, int *result, const void *value, FILE *stream);
static void mbed_minimal_formatted_string_string(char *buffer, size_t length, int *result, const char *string, size_t precision, FILE *stream);


/**
 * @brief      Print a single character, checking for buffer and size overflows.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  data    The char to be printed.
 */
static void mbed_minimal_putchar(char *buffer, size_t length, int *result, char data, FILE *stream)
{
    /* only continue if 'result' doesn't overflow */
    if ((*result >= 0) && (*result <= INT_MAX - 1)) {
        if (stream) {
            if (fputc(data, stream) == EOF) {
                *result = EOF;
            } else {
                *result += 1;
            }
        } else {
            if (buffer) {
                /* write data only if there's enough space */
                if ((size_t)*result < length) {
                    buffer[*result] = data;
                }
            }

            /* increment 'result' even if data was not written. This ensures that
               'mbed_minimal_formatted_string' returns the correct value. */
            *result += 1;
        }
    }
}

/**
 * @brief      Print integer in signed, unsigned or hexadecimal format.
 *
 * @param      buffer           The buffer to store output (NULL for stdout).
 * @param[in]  length           The length of the buffer.
 * @param      result           The current output location.
 * @param[in]  value            The value to be printed.
 * @param      type             The type of integer format that shall be printed (signed, unsigend or hexadecimal)
 * @param      width_size       The width modifier.
 * @param      prepend_zeros    Flag to prepends zeros when the width_size is greater than 0
 */
static void mbed_minimal_formatted_string_integer(char *buffer, size_t length, int *result, MBED_UNSIGNED_STORAGE value, integer_type_t type, int width_size, bool prepend_zeros, FILE *stream)
{
    /* allocate 3 digits per byte */
    char scratch[sizeof(MBED_UNSIGNED_STORAGE) * 3] = { 0 };

    int index = 0;

    bool negative_value = false;

    const char filler = prepend_zeros ? '0' : ' ';

    if (type == INT_SIGNED) {
        if ((MBED_SIGNED_STORAGE) value < 0) {
            /* get absolute value using two's complement */
            value = ~value + 1;
            negative_value = true;
        }
    } else if (type == ZERO_NEGATIVE) {
        negative_value = true;
    }

    if (value == 0) {
        scratch[index] = '0';
        index++;
    } else {
        /* write numbers in reverse order to scratch pad */
        for (; value > 0; index++) {
            if (type == HEX_LOWER || type == HEX_UPPER) {
                /* get least significant byte */
                const uint8_t output = value & 0x0F;

                static const char int2hex_lower[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
                                                      };
                static const char int2hex_upper[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                                        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
                                                      };

                if (type == HEX_LOWER) {
                    scratch[index] = int2hex_lower[output];
                } else {
                    scratch[index] = int2hex_upper[output];
                }

                /* shift value one byte position */
                value = value >> 4;
            } else {
                /* use '0' as base and add digit */
                scratch[index] = '0' + (value % 10);

                /* shift value one decimal position */
                value = value / 10;
            }
        }
    }

    if (negative_value) {
        if (prepend_zeros) {
            mbed_minimal_putchar(buffer, length, result, '-', stream);
        }
        index++; // add one to index to count '-'
    }

    // print filler characters
    if (width_size > index) {
        for (int i = width_size; i > index; i--) {
            mbed_minimal_putchar(buffer, length, result, filler, stream);
        }
    }

    if (negative_value) {
        if (!prepend_zeros) {
            mbed_minimal_putchar(buffer, length, result, '-', stream);
        }
        index--; // Restore index to correct position
    }

    /* print absolute value of integer */
    for (; index > 0; index--) {
        mbed_minimal_putchar(buffer, length, result, scratch[index - 1], stream);
    }
}

/**
 * @brief      Print pointer.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The pointer to be printed.
 */
static void mbed_minimal_formatted_string_void_pointer(char *buffer, size_t length, int *result, const void *value, FILE *stream)
{
    /* write leading 0x */
    mbed_minimal_putchar(buffer, length, result, '0', stream);
    mbed_minimal_putchar(buffer, length, result, 'x', stream);

    /* write rest as a regular hexadecimal number */
    mbed_minimal_formatted_string_integer(buffer, length, result, (ptrdiff_t) value, HEX_UPPER, 0, false, stream);
}

#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
/**
 * @brief      Write double.
 *
 * @param      buffer          The buffer to store output (NULL for stdout).
 * @param[in]  length          The length of the buffer.
 * @param      result          The current output location.
 * @param[in]  value           The value to be printed.
 * @param[in]  dec_precision   The decimal precision. If PRECISION_DEFAULT MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS is used.
 * @param      width_size       The width modifier.
 * @param      prepend_zeros    Flag to prepends zeros when the width_size is greater than 0
 */
static void mbed_minimal_formatted_string_double(char *buffer, size_t length, int *result, double value, int dec_precision, int width_size, bool prepend_zeros, FILE *stream)
{
    /* get integer part */
    MBED_SIGNED_STORAGE integer = value;
    /* fractional part represented as the int that will be formatted after the dot, e.g. 95 for 1.95 */
    MBED_SIGNED_STORAGE decimal = 0;

    if (dec_precision == PRECISION_DEFAULT) {
        dec_precision = MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS;
    }

    if (dec_precision != 0) {
        /* get decimal part */
        MBED_SIGNED_STORAGE precision = 1;
        for (int index = 0; index < dec_precision; index++) {
            precision *= 10;
        }

        /* Multiply the frac part so we get an int value with the required accuracy.
           E.g. For 0.1234 and dec_precision=3 you'd get 123.4 */
        double decimal_double = (value - integer) * precision;
        if (value < 0) {
            /* The part after the dot does not have a sign, so negate the value before rounding */
            decimal = -decimal_double + 0.5;
            if (decimal >= precision) {
                /* Rounding carries over to value's integer part (e.g. -1.95 with dec_precision=1 -> -2.0) */
                integer--;
                decimal = 0;
            }
        } else {
            /* Round the value */
            decimal = decimal_double + 0.5;
            if (decimal >= precision) {
                /* Rounding carries over to value's integer part (e.g. 1.95 with dec_precision=1 -> 2.0) */
                integer++;
                decimal = 0;
            }
        }

        width_size -= dec_precision + 1; // decimal precision plus '.'
        if (width_size < 0) {
            width_size = 0;
        }
    } else {
        value = (value - integer) * 1.0;
        if (value > 0.5) {
            integer++;
        } else if (value < -0.5) {
            integer--;
        }
    }

    /* write integer part */
    if (integer == 0 && value < 0) {
        mbed_minimal_formatted_string_integer(buffer, length, result, integer, ZERO_NEGATIVE, width_size, prepend_zeros, stream);
    } else {
        mbed_minimal_formatted_string_integer(buffer, length, result, integer, INT_SIGNED, width_size, prepend_zeros, stream);
    }

    if (dec_precision != 0) {
        /* write decimal point */
        mbed_minimal_putchar(buffer, length, result, '.', stream);
        /* write decimal part */
        mbed_minimal_formatted_string_integer(buffer, length, result, decimal, INT_UNSIGNED, dec_precision, true, stream);
    }
}
#endif

/**
 * @brief      Print string with precision.
 *
 * @param      buffer     The buffer to store output (NULL for stdout).
 * @param[in]  length     The length of the buffer.
 * @param      result     The current output location.
 * @param[in]  value      The string to be printed.
 * @param[in]  precision  The maximum number of characters to be printed.
 */
static void mbed_minimal_formatted_string_string(char *buffer, size_t length, int *result, const char *string, size_t precision, FILE *stream)
{
    while ((*string != '\0') && (precision)) {
        mbed_minimal_putchar(buffer, length, result, *string, stream);
        string++;
        precision--;
    }
}

/**
 * @brief Parse a string to an integer value as long as there are numerical characters in the string
 *
 * @param[in]   string      The input string. Has to begin with a numerical character to parse
 * @param[out]  value       The output value.
 * @return      size_t      The number of numerical characters parsed
 */
static size_t parse_string_to_integer(const char *string, int *value)
{
    size_t inner_index = 0;

    while ((string[inner_index] >= '0') && (string[inner_index] <= '9')) {
        *value = *value * 10 + (string[inner_index] - '0');

        inner_index++;
    }

    return inner_index;
}

/**
 * @brief      Parse formatted string and invoke write handlers based on type.
 *
 * @param      buffer     The buffer to write to, write to stdout if NULL.
 * @param[in]  length     The length of the buffer.
 * @param[in]  format     The formatted string.
 * @param[in]  arguments  The va_list arguments.
 *
 * @return     Number of characters written.
 */
int mbed_minimal_formatted_string(char *buffer, size_t length, const char *format, va_list arguments, FILE *stream)
{
    int result = 0;
    bool empty_buffer = false;

    /* ensure that function wasn't called with an empty buffer, or with or with
       a buffer size that is larger than the maximum 'int' value, or with
       a NULL format specifier */
    if (format && length <= INT_MAX) {
        /* Make sure that there's always space for the NULL terminator */
        if (length > 0) {
            length --;
        } else {
            /* the buffer is empty, there's no place to write the terminator */
            empty_buffer = true;
        }
        /* parse string */
        for (size_t index = 0; format[index] != '\0'; index++) {
            /* format specifier begin */
            if (format[index] == '%') {
                size_t next_index = index + 1;

                /**************************************************************
                 * skip and ignore flags [-+(space)#]
                 *************************************************************/
                if ((format[next_index] == '-') ||
                        (format[next_index] == '+') ||
                        (format[next_index] == ' ') ||
                        (format[next_index] == '#')) {
                    /* skip to next character */
                    next_index++;
                }

                /**************************************************************
                 * look for width and prepending zeros [(number)], skip [*]
                 *************************************************************/
                bool prepend_zeros = false;
                int width_size = 0;

                if (format[next_index] == '*') {
                    /* skip to next character */
                    next_index++;

                    /* discard argument */
                    va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                } else {
                    if (format[next_index] == '0') {
                        prepend_zeros = true;
                        do {
                            next_index++;
                        } while (format[next_index] == '0');
                    }

                    /* parse width modifier until not a decimal */
                    next_index += parse_string_to_integer(&format[next_index], &width_size);
                }

                /**************************************************************
                 * look for precision modifier
                 *************************************************************/
                int precision = PRECISION_DEFAULT;

                if ((format[next_index] == '.') &&
                        (format[next_index + 1] == '*')) {
                    next_index += 2;

                    /* read precision from argument list */
                    precision = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                } else if (format[next_index] == '.') {
                    /* precision modifier found, reset default to 0 and increment index */
                    next_index++;
                    precision = 0;

                    /* parse precision until not a decimal */
                    next_index += parse_string_to_integer(&format[next_index], &precision);
                }

                /**************************************************************
                 * look for length modifier, default to NONE
                 *************************************************************/
                length_t length_modifier = LENGTH_NONE;

                /* look for two character length modifier */
                if ((format[next_index] == 'h') && (format[next_index + 1] == 'h')) {
                    length_modifier = LENGTH_HH;
                } else if ((format[next_index] == 'l') && (format[next_index + 1] == 'l')) {
                    length_modifier = LENGTH_LL;
                }
                /* look for one character length modifier if two character search failed */
                else if (format[next_index] == 'h') {
                    length_modifier = LENGTH_H;
                } else if (format[next_index] == 'l') {
                    length_modifier = LENGTH_L;
                } else if (format[next_index] == 'j') {
                    length_modifier = LENGTH_J;
                } else if (format[next_index] == 'z') {
                    length_modifier = LENGTH_Z;
                } else if (format[next_index] == 't') {
                    length_modifier = LENGTH_T;
                } else if (format[next_index] == 'L') {
                    length_modifier = LENGTH_CAPITAL_L;
                }

                /* increment index, length is encoded in modifier enum */
                next_index += (length_modifier & 0x0F);

                /**************************************************************
                 * read out character - this is a supported format character,
                 * '\0', or a not suported character
                 *************************************************************/
                char next = format[next_index];

                /* signed integer */
                if ((next == 'd') || (next == 'i')) {
                    MBED_SIGNED_STORAGE value = 0;

#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT
                    /* if 64 bit is enabled and the integer types are larger than the native type */
                    if (((length_modifier == LENGTH_LL)   && (sizeof(long long int) > sizeof(MBED_SIGNED_NATIVE_TYPE))) ||
                            ((length_modifier == LENGTH_L)    && (sizeof(long int)      > sizeof(MBED_SIGNED_NATIVE_TYPE))) ||
                            ((length_modifier == LENGTH_NONE) && (sizeof(int)           > sizeof(MBED_SIGNED_NATIVE_TYPE)))) {
                        /* use 64 bit storage type for readout */
                        value = va_arg(arguments, MBED_SIGNED_STORAGE);
                    } else
#else
                    /* If 64 bit is not enabled, print %ll[di] rather than truncated value */
                    if (length_modifier == LENGTH_LL) {
                        mbed_minimal_putchar(buffer, length, &result, '%', stream);
                        if (next == '%') {
                            // Continue printing loop after `%`
                            index = next_index;
                        }
                        continue;
                    }
#endif
                    {
                        /* use native storage type (which can be 32 or 64 bit) */
                        value = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                    }

                    /* constrict value based on length modifier */
                    switch (length_modifier) {
                        case LENGTH_NONE:
                            value = (int) value;
                            break;
                        case LENGTH_HH:
                            value = (signed char) value;
                            break;
                        case LENGTH_H:
                            value = (short int) value;
                            break;
                        case LENGTH_L:
                            value = (long int) value;
                            break;
                        case LENGTH_LL:
                            value = (long long int) value;
                            break;
                        case LENGTH_J:
                            value = (intmax_t) value;
                            break;
                        case LENGTH_T:
                            value = (ptrdiff_t) value;
                            break;
                        default:
                            break;
                    }

                    index = next_index;

                    mbed_minimal_formatted_string_integer(buffer, length, &result, value, INT_SIGNED, width_size, prepend_zeros, stream);
                }
                /* unsigned integer */
                else if ((next == 'u') || (next == 'x') || (next == 'X')) {
                    MBED_UNSIGNED_STORAGE value = 0;

#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_64_BIT
                    /* if 64 bit is enabled and the integer types are larger than the native type */
                    if (((length_modifier == LENGTH_LL)   && (sizeof(unsigned long long int) > sizeof(MBED_UNSIGNED_NATIVE_TYPE))) ||
                            ((length_modifier == LENGTH_L)    && (sizeof(unsigned long int)      > sizeof(MBED_UNSIGNED_NATIVE_TYPE))) ||
                            ((length_modifier == LENGTH_NONE) && (sizeof(unsigned int)           > sizeof(MBED_UNSIGNED_NATIVE_TYPE)))) {
                        /* use 64 bit storage type for readout */
                        value = va_arg(arguments, MBED_UNSIGNED_STORAGE);
                    } else
#else
                    /* If 64 bit is not enabled, print %ll[uxX] rather than truncated value */
                    if (length_modifier == LENGTH_LL) {
                        mbed_minimal_putchar(buffer, length, &result, '%', stream);
                        if (next == '%') {
                            // Continue printing loop after `%`
                            index = next_index;
                        }
                        continue;
                    }
#endif
                    {
                        /* use native storage type (which can be 32 or 64 bit) */
                        value = va_arg(arguments, MBED_UNSIGNED_NATIVE_TYPE);
                    }

                    /* constrict value based on length modifier */
                    switch (length_modifier) {
                        case LENGTH_NONE:
                            value = (unsigned int) value;
                            break;
                        case LENGTH_HH:
                            value = (unsigned char) value;
                            break;
                        case LENGTH_H:
                            value = (unsigned short int) value;
                            break;
                        case LENGTH_L:
                            value = (unsigned long int) value;
                            break;
                        case LENGTH_LL:
                            value = (unsigned long long int) value;
                            break;
                        case LENGTH_J:
                            value = (uintmax_t) value;
                            break;
                        case LENGTH_Z:
                            value = (size_t) value;
                            break;
                        case LENGTH_T:
                            value = (ptrdiff_t) value;
                            break;
                        default:
                            break;
                    }

                    index = next_index;

                    /* write unsigned or hexadecimal */
                    if (next == 'u') {
                        mbed_minimal_formatted_string_integer(buffer, length, &result, value, INT_UNSIGNED, width_size, prepend_zeros, stream);
                    } else if (next == 'X') {
                        mbed_minimal_formatted_string_integer(buffer, length, &result, value, HEX_UPPER, width_size, prepend_zeros, stream);
                    } else {
                        mbed_minimal_formatted_string_integer(buffer, length, &result, value, HEX_LOWER, width_size, prepend_zeros, stream);
                    }
                }
#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
                /* treat all floating points the same */
                else if ((next == 'f') || (next == 'F') || (next == 'g') || (next == 'G')) {
                    double value = va_arg(arguments, double);
                    index = next_index;

                    mbed_minimal_formatted_string_double(buffer, length, &result, value, precision, width_size, prepend_zeros, stream);
                }
#endif
                /* character */
                else if (next == 'c') {
                    char value = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                    index = next_index;

                    mbed_minimal_putchar(buffer, length, &result, value, stream);
                }
                /* string */
                else if (next == 's') {
                    char *value = va_arg(arguments, char *);
                    index = next_index;

                    mbed_minimal_formatted_string_string(buffer, length, &result, value, precision, stream);
                }
                /* pointer */
                else if (next == 'p') {
                    void *value = va_arg(arguments, void *);
                    index = next_index;

                    mbed_minimal_formatted_string_void_pointer(buffer, length, &result, value, stream);
                } else {
                    // Unrecognised, or `%%`. Print the `%` that led us in.
                    mbed_minimal_putchar(buffer, length, &result, '%', stream);
                    if (next == '%') {
                        // Continue printing loop after `%%`
                        index = next_index;
                    }
                    // Otherwise we continue the printing loop after the leading `%`, so an
                    // unrecognised thing like "Blah = %a" will just come out as "Blah = %a"
                }
            } else
                /* not a format specifier */
            {
                /* write normal character */
                mbed_minimal_putchar(buffer, length, &result, format[index], stream);
            }
        }

        if (buffer && !empty_buffer) {
            /* NULL-terminate the buffer no matter what. We use '<=' to compare instead of '<'
               because we know that we initially reserved space for '\0' by decrementing length */
            if ((size_t)result <= length) {
                buffer[result] = '\0';
            } else {
                buffer[length] = '\0';
            }
        }
    }

    return result;
}
