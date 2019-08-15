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

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

/***************************/
/* MBED                    */
/***************************/
#if TARGET_LIKE_MBED

#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
static char mbed_stdio_out_prev = 0;
#endif


/***************************/
/* Linux                   */
/***************************/
#else
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
 * Prototypes
 */
static void mbed_minimal_formatted_string_signed(char *buffer, size_t length, int *result, MBED_SIGNED_STORAGE value, FILE *stream);
static void mbed_minimal_formatted_string_unsigned(char *buffer, size_t length, int *result, MBED_UNSIGNED_STORAGE value, FILE *stream);
static void mbed_minimal_formatted_string_hexadecimal(char *buffer, size_t length, int *result, MBED_UNSIGNED_STORAGE value, FILE *stream, bool upper);
static void mbed_minimal_formatted_string_void_pointer(char *buffer, size_t length, int *result, const void *value, FILE *stream);
static void mbed_minimal_formatted_string_character(char *buffer, size_t length, int *result, char character, FILE *stream);
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
        /* write data only if there's enough space */
        if ((size_t)*result < length) {
            if (buffer) {
                buffer[*result] = data;
            } else {
                fputc(data, stream);
            }
        }
        /* increment 'result' even if data was not written. This ensures that
           'mbed_minimal_formatted_string' returns the correct value. */
        *result += 1;
    }
}

/**
 * @brief      Print signed integer.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 */
static void mbed_minimal_formatted_string_signed(char *buffer, size_t length, int *result, MBED_SIGNED_STORAGE value, FILE *stream)
{
    MBED_UNSIGNED_STORAGE new_value = 0;

    /* if value is negative print sign and treat as positive number */
    if (value < 0) {
        /* write sign */
        mbed_minimal_putchar(buffer, length, result, '-', stream);

        /* get absolute value using two's complement */
        new_value = ~((MBED_UNSIGNED_STORAGE) value) + 1;
    } else {
        new_value = value;
    }

    /* use unsigned long int function */
    mbed_minimal_formatted_string_unsigned(buffer, length, result, new_value, stream);
}

/**
 * @brief      Print unsigned integer.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 */
static void mbed_minimal_formatted_string_unsigned(char *buffer, size_t length, int *result, MBED_UNSIGNED_STORAGE value, FILE *stream)
{
    /* treat 0 as a corner case */
    if (value == 0) {
        mbed_minimal_putchar(buffer, length, result, '0', stream);
    } else {
        /* allocate 3 digits per byte */
        char scratch[sizeof(MBED_UNSIGNED_STORAGE) * 3] = { 0 };

        size_t index = 0;

        /* write numbers in reverse order to scratch pad */
        for (; value > 0; index++) {
            /* use '0' as base and add digit */
            scratch[index] = '0' + (value % 10);

            /* shift value one decimal position */
            value = value / 10;
        }

        /* write scratch pad to buffer or output */
        for (; index > 0; index--) {
            mbed_minimal_putchar(buffer, length, result, scratch[index - 1], stream);
        }
    }
}

/**
 * @brief      Print hexadecimal.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 * @param      upper   Flag to print the hexadecimal in upper or lower case.
 */
static void mbed_minimal_formatted_string_hexadecimal(char *buffer, size_t length, int *result, MBED_UNSIGNED_STORAGE value, FILE *stream, bool upper)
{
    bool print_leading_zero = false;

    for (int index = 7; index >= 0; index--) {
        /* get most significant byte */
        uint8_t output = value >> (8 * index);

        /* only print leading zeros when set */
        if (print_leading_zero || (output != 0) || (index == 0)) {
            unsigned int nibble_one = (output >> 4);
            unsigned int nibble_two = (output & 0x0F);

            const char int2hex_lower[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                             '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
                                           };
            const char int2hex_upper[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                             '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
                                           };
            const char *int2hex = upper ? int2hex_upper : int2hex_lower;

            if (print_leading_zero || nibble_one != 0) {
                mbed_minimal_putchar(buffer, length, result, int2hex[nibble_one], stream);
            }
            mbed_minimal_putchar(buffer, length, result, int2hex[nibble_two], stream);

            /* print zeroes after the first non-zero byte */
            print_leading_zero = true;
        }
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
    mbed_minimal_formatted_string_hexadecimal(buffer, length, result, (ptrdiff_t) value, stream, true);
}

#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
/**
 * @brief      Write double.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 */
static void mbed_minimal_formatted_string_double(char *buffer, size_t length, int *result, double value, FILE *stream)
{
    /* get integer part */
    MBED_SIGNED_STORAGE integer = value;

    /* write integer part */
    mbed_minimal_formatted_string_signed(buffer, length, result, integer, stream);

    /* write decimal point */
    mbed_minimal_formatted_string_character(buffer, length, result, '.', stream);

    /* get decimal part */
    double precision = 1.0;

    for (size_t index = 0; index < MBED_CONF_PLATFORM_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS; index++) {
        precision *= 10;
    }

    value = (value - integer) * precision;

    /* convert to unsigned integer */
    MBED_UNSIGNED_STORAGE decimal = 0;

    if (value < 0) {
        MBED_SIGNED_STORAGE temp = value;
        decimal = ~((MBED_UNSIGNED_STORAGE) temp) + 1;
    } else {
        decimal = value;
    }

    /* round up or down */
    value -= decimal;

    if (!((value > -0.5) && (value < 0.5))) {
        decimal++;
    }

    /* convert precision to unsigned integer */
    MBED_UNSIGNED_STORAGE precision_in_uint = precision;
    precision_in_uint /= 10;

    /* ensure that leading zeros are printed if decimal equals 0  */
    MBED_UNSIGNED_STORAGE val = decimal ? decimal : decimal + 1;
    while (precision_in_uint > val) {
        /* print leading zeros */
        mbed_minimal_putchar(buffer, length, result, '0', stream);
        precision_in_uint /= 10;
    }

    /* write decimal part */
    mbed_minimal_formatted_string_unsigned(buffer, length, result, decimal, stream);
}
#endif

/**
 * @brief      Print character.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The character to be printed.
 */
static void mbed_minimal_formatted_string_character(char *buffer, size_t length, int *result, char character, FILE *stream)
{
    /* write character */
    if (buffer) {
        mbed_minimal_putchar(buffer, length, result, character, stream);
    } else {
        /* convert \n to \r\n if enabled in platform configuration */
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
        if (character == '\n' && mbed_stdio_out_prev != '\r') {
            mbed_minimal_putchar(buffer, length, result, '\r', stream);
        }

        /* cache character */
        mbed_stdio_out_prev = character;
#endif
        mbed_minimal_putchar(buffer, length, result, character, stream);
    }
}

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
                 * skip and ignore flags [-+(space)#0]
                 *************************************************************/
                if ((format[next_index] == '-') ||
                        (format[next_index] == '+') ||
                        (format[next_index] == ' ') ||
                        (format[next_index] == '#') ||
                        (format[next_index] == '0')) {
                    /* skip to next character */
                    next_index++;
                }

                /**************************************************************
                 * skip and ignore width [(number)*]
                 *************************************************************/
                if (format[next_index] == '*') {
                    /* skip to next character */
                    next_index++;

                    /* discard argument */
                    va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                } else {
                    while ((format[next_index] >= '0') &&
                            (format[next_index] <= '9')) {
                        /* skip to next character */
                        next_index++;
                    }
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
                    size_t inner_index = 0;

                    while ((format[next_index + inner_index] >= '0') &&
                            (format[next_index + inner_index] <= '9')) {
                        precision = precision * 10 + (format[next_index + inner_index] - '0');

                        inner_index++;
                    }

                    /* move index forward to point at next character */
                    next_index += inner_index;
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
#endif
                    {
                        /* use native storage type (which can be 32 or 64 bit) */
                        value = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                    }

                    /* constrict value based on lenght modifier */
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

                    mbed_minimal_formatted_string_signed(buffer, length, &result, value, stream);
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
#endif
                    {
                        /* use native storage type (which can be 32 or 64 bit) */
                        value = va_arg(arguments, MBED_UNSIGNED_NATIVE_TYPE);
                    }

                    /* constrict value based on lenght modifier */
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
                        mbed_minimal_formatted_string_unsigned(buffer, length, &result, value, stream);
                    } else {
                        mbed_minimal_formatted_string_hexadecimal(buffer, length, &result, value, stream, next == 'X');
                    }
                }
#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
                /* treat all floating points the same */
                else if ((next == 'f') || (next == 'F') || (next == 'g') || (next == 'G')) {
                    double value = va_arg(arguments, double);
                    index = next_index;

                    mbed_minimal_formatted_string_double(buffer, length, &result, value, stream);
                }
#endif
                /* character */
                else if (next == 'c') {
                    char value = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                    index = next_index;

                    mbed_minimal_formatted_string_character(buffer, length, &result, value, stream);
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
                    /* write all characters between format beginning and unrecognied modifier */
                    while (index < next_index) {
                        mbed_minimal_formatted_string_character(buffer, length, &result, format[index], stream);
                        index++;
                    }

                    /* if this is not the end of the string, write unrecognized modifier */
                    if (next != '\0') {
                        mbed_minimal_formatted_string_character(buffer, length, &result, format[index], stream);
                    } else {
                        /* break out of for loop */
                        break;
                    }
                }
            } else
                /* not a format specifier */
            {
                /* write normal character */
                mbed_minimal_formatted_string_character(buffer, length, &result, format[index], stream);
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

