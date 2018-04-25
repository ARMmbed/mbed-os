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
/*
 Serial initialization and new line replacement is a direct copy from mbed_retarget.cpp
 If the static modifier were to be removed, this part of the code would not be necessary.
*/
#include "hal/serial_api.h"

#if DEVICE_SERIAL
static serial_t stdio_uart = { 0 };
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
static char mbed_stdio_out_prev = 0;
#endif
#endif

/* module variable for keeping track of initialization */
static bool not_initialized = true;

static void init_serial()
{
    if (not_initialized)
    {
        not_initialized = false;

#if DEVICE_SERIAL
        serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE
        serial_baud(&stdio_uart, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#endif
#endif
    }
}

#define MBED_INITIALIZE_PRINT(x) { init_serial(); }
#define MBED_PRINT_CHARACTER(x) { serial_putc(&stdio_uart, x); }

/***************************/
/* Linux                   */
/***************************/
#else
/* Linux implementation is for debug only */
#define MBED_CONF_MINIMAL_PRINTF_ENABLE_FLOATING_POINT 1
#define MBED_CONF_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS 6
#define MBED_CONF_MINIMAL_PRINTF_ENABLE_64_BIT 1
#define MBED_INITIALIZE_PRINT(x) { ; }
#define MBED_PRINT_CHARACTER(x) { printf("%c", x); }
#endif

#ifndef MBED_CONF_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
#define MBED_CONF_MINIMAL_PRINTF_ENABLE_FLOATING_POINT 0
#endif

#ifndef MBED_CONF_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS
#define MBED_CONF_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS 6
#endif

#ifndef MBED_CONF_MINIMAL_PRINTF_ENABLE_64_BIT
#define MBED_CONF_MINIMAL_PRINTF_ENABLE_64_BIT 1
#endif

/**
 * Check architecture and choose storage data type.
 * On 32 bit machines, the default storage type is 32 bit wide
 * unless 64 bit integers are enabled in the configuration.
 */
#if INTPTR_MAX == INT32_MAX
#define MBED_SIGNED_NATIVE_TYPE int32_t
#define MBED_UNSIGNED_NATIVE_TYPE uint32_t
#if MBED_CONF_MINIMAL_PRINTF_ENABLE_64_BIT
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
 * Enum for storing width modifier.
 */
typedef enum {
    LENGTH_NONE,
    LENGTH_HH,
    LENGTH_H,
    LENGTH_L,
    LENGTH_LL,
    LENGTH_J,
    LENGTH_Z,
    LENGTH_T,
    LENGTH_CAPITAL_L
} length_t;

/**
 * Prototypes
 */
static void mbed_minimal_formatted_string_signed(char* buffer, size_t length, int* result, MBED_SIGNED_STORAGE value);
static void mbed_minimal_formatted_string_unsigned(char* buffer, size_t length, int* result, MBED_UNSIGNED_STORAGE value);
static void mbed_minimal_formatted_string_hexadecimal(char* buffer, size_t length, int* result, MBED_UNSIGNED_STORAGE value);
static void mbed_minimal_formatted_string_void_pointer(char* buffer, size_t length, int* result, const void* value);
static void mbed_minimal_formatted_string_character(char* buffer, size_t length, int* result, char character);
static void mbed_minimal_formatted_string_string(char* buffer, size_t length, int* result, const char* string);

/**
 * @brief      Print signed integer.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 */
static void mbed_minimal_formatted_string_signed(char* buffer, size_t length, int* result, MBED_SIGNED_STORAGE value)
{
    /* only continue if buffer can fit at least 1 character and if
       'result' doesn't overflow */
    if ((*result >= 0) && (*result <= INT_MAX - 1) && ((size_t)*result + 1 <= length))
    {
        MBED_UNSIGNED_STORAGE new_value = 0;

        /* if value is negative print sign and treat as positive number */
        if (value < 0)
        {
            /* write sign */
            if (buffer)
            {
                buffer[*result] = '-';
            }
            else
            {
                MBED_PRINT_CHARACTER('-');
            }

            *result += 1;

            /* get absolute value using two's complement */
            new_value = ~((MBED_UNSIGNED_STORAGE) value) + 1;
        }
        else
        {
            new_value = value;
        }

        /* use unsigned long int function */
        mbed_minimal_formatted_string_unsigned(buffer, length, result, new_value);
    }
}

/**
 * @brief      Print unsigned integer.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 */
static void mbed_minimal_formatted_string_unsigned(char* buffer, size_t length, int* result, MBED_UNSIGNED_STORAGE value)
{
    /* only continue if buffer can fit at least 1 character and if
       'result' doesn't overflow */
    if ((*result >= 0) && (*result <= INT_MAX - 1) && ((size_t)*result + 1 <= length))
    {
        /* treat 0 as a corner case */
        if (value == 0)
        {
            if (buffer)
            {
                buffer[*result] = '0';
            }
            else
            {
                MBED_PRINT_CHARACTER('0');
            }

            *result += 1;
        }
        else
        {
            /* allocate 3 digits per byte */
            char scratch[sizeof(MBED_UNSIGNED_STORAGE) * 3] = { 0 };

            size_t index = 0;

            /* write numbers in reverse order to scratch pad */
            for ( ; value > 0; index++)
            {
                /* use '0' as base and add digit */
                scratch[index] = '0' + (value % 10);

                /* shift value one decimal position */
                value = value / 10;
            }

            /* write scratch pad to buffer or output */
            for ( ; (*result <= INT_MAX- 1) && ((size_t)*result < length) && (index > 0); index--)
            {
                if (buffer)
                {
                    buffer[*result] = scratch[index - 1];
                }
                else
                {
                    MBED_PRINT_CHARACTER(scratch[index - 1]);
                }

                *result += 1;
            }
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
 */
static void mbed_minimal_formatted_string_hexadecimal(char* buffer, size_t length, int* result, MBED_UNSIGNED_STORAGE value)
{
    bool print_leading_zero = false;

    /* only continue each loop if buffer can fit at least 2 characters
       and if 'result' doesn't overflow */
    for (int index = 7; (*result >= 0) && (*result <= INT_MAX - 2) && ((size_t)*result + 2 <= length) && (index >= 0); index--)
    {
        /* get most significant byte */
        uint8_t output = value >> (8 * index);

        /* only print leading zeros when set */
        if (print_leading_zero || (output != 0) || (index == 0))
        {
            /* print zeroes after the first non-zero byte */
            print_leading_zero = true;

            unsigned int nibble_one = (output >> 4);
            unsigned int nibble_two = (output & 0x0F);

            const char int2hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                       '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

            /* write to buffer or stdout */
            if (buffer)
            {
                if (nibble_one != 0) {
                    buffer[*result] = int2hex[nibble_one];
                    *result += 1;
                }
                buffer[*result] = int2hex[nibble_two];
                *result += 1;
            }
            else
            {
                if (nibble_one != 0) {
                    MBED_PRINT_CHARACTER(int2hex[nibble_one]);
                    *result += 1;
                }
                MBED_PRINT_CHARACTER(int2hex[nibble_two]);
                *result += 1;
            }

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
static void mbed_minimal_formatted_string_void_pointer(char* buffer, size_t length, int* result, const void* value)
{
    /* only continue if buffer can fit '0x' and twice the size of a void*
       and if 'result' doesn't overflow */
    size_t needed = 2 + 2 * sizeof(void*);
    if ((*result >= 0) && ((size_t)*result <= INT_MAX - needed) && ((size_t)*result + needed <= length))
    {
        /* write leading 0x */
        if (buffer)
        {
            buffer[*result] = '0';
            buffer[*result + 1] = 'x';
        }
        else
        {
            MBED_PRINT_CHARACTER('0');
            MBED_PRINT_CHARACTER('x');
        }

        *result += 2;

        /* write rest as a regular hexadecimal number */
        mbed_minimal_formatted_string_hexadecimal(buffer, length, result, (ptrdiff_t) value);
    }
}

#if MBED_CONF_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
/**
 * @brief      Write double.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The value to be printed.
 */
static void mbed_minimal_formatted_string_double(char* buffer, size_t length, int* result, double value)
{
    /* only continue if buffer can fit at least 1 character and if
       'result' doesn't overflow */
    if ((*result >= 0) && (*result <= INT_MAX - 1) && ((size_t)*result + 1 <= length))
    {
        /* get integer part */
        MBED_SIGNED_STORAGE integer = value;

        /* write integer part */
        mbed_minimal_formatted_string_signed(buffer, length, result, integer);

        /* write decimal point */
        mbed_minimal_formatted_string_character(buffer, length, result, '.');

        /* get decimal part */
        double precision = 1.0;

        for (size_t index = 0; index < MBED_CONF_MINIMAL_PRINTF_SET_FLOATING_POINT_MAX_DECIMALS; index++)
        {
            precision *= 10;
        }

        value = (value - integer) * precision;

        /* convert to unsigned integer */
        MBED_UNSIGNED_STORAGE decimal = 0;

        if (value < 0)
        {
            MBED_SIGNED_STORAGE temp = value;
            decimal = ~((MBED_UNSIGNED_STORAGE) temp) + 1;
        }
        else
        {
            decimal = value;
        }

        /* round up or down */
        value -= decimal;

        if (!((value > -0.5) && (value < 0.5)))
        {
            decimal++;
        }

        /* write decimal part */
        mbed_minimal_formatted_string_unsigned(buffer, length, result, decimal);
    }
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
static void mbed_minimal_formatted_string_character(char* buffer, size_t length, int* result, char character)
{
    /* only continue if the buffer can fit 1 character and if
       'result' doesn't overflow */
    if ((*result >= 0) && (*result <= INT_MAX - 1) && ((size_t)*result + 1 <= length))
    {
        /* write character */
        if (buffer)
        {
            buffer[*result] = character;
        }
        else
        {
            /* convert \n to \r\n if enabled in platform configuration */
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
            if (character == '\n' && mbed_stdio_out_prev != '\r')
            {
                MBED_PRINT_CHARACTER('\r');
                *result += 1;
            }

            /* cache character */
            mbed_stdio_out_prev = character;
#endif

            /* write character to stdout */
            MBED_PRINT_CHARACTER(character);
        }

        *result += 1;
    }
}

/**
 * @brief      Print string.
 *
 * @param      buffer  The buffer to store output (NULL for stdout).
 * @param[in]  length  The length of the buffer.
 * @param      result  The current output location.
 * @param[in]  value   The string to be printed.
 */
static void mbed_minimal_formatted_string_string(char* buffer, size_t length, int* result, const char* string)
{
    /* only continue if the buffer can fit at least 1 character and if
       'result' doesn't overflow */
    if ((*result >= 0) && (*result <= INT_MAX - 1) && ((size_t)*result + 1 <= length))
    {
        /* count characters in string */
        size_t remaining = length - *result;
        size_t string_length = 0;

        /* only count characters that will fit into buffer */
        while ((string[string_length] != '\0') && (string_length < remaining))
        {
            string_length++;
        }

        /* copy string to buffer */
        if (buffer)
        {
            /* ensure that the value of "result" doesn't overflow */
            if (string_length + *result > INT_MAX)
            {
                string_length = (size_t)INT_MAX - *result;
            }
            for (size_t index = 0; index < string_length; index++)
            {
                buffer[*result + index] = string[index];
            }
        }
        /* print string */
        else
        {
            for (size_t index = 0; index < string_length; index++)
            {
                MBED_PRINT_CHARACTER(string[index]);
            }
        }

        /* add length to counter */
        *result += string_length;
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
int mbed_minimal_formatted_string(char* buffer, size_t length, const char* format, va_list arguments)
{
    /* initialize output if needed */
    MBED_INITIALIZE_PRINT();

    int result = 0;

    /* ensure that function wasn't called with an empty buffer, or with or with
       a buffer size that is larger than the maximum 'int' value, or with
       a NULL format specifier */
    if (format && length > 0 && length <= INT_MAX)
    {
        /* parse string */
        for (size_t index = 0; format[index] != '\0'; index++)
        {
            /* format specifier begin */
            if (format[index] == '%')
            {
                size_t next_index = index + 1;

                /* while format string is not empty */
                while ((format[next_index] != '\0') &&
                        /* skip all flags and precision modifiers */
                        (((format[next_index] >= '0') && (format[next_index] <= '9')) ||
                         (format[next_index] == '-') ||
                         (format[next_index] == '+') ||
                         (format[next_index] == '#') ||
                         (format[next_index] == '.')))
                {
                    /* skip to next character */
                    next_index++;
                }

                /* look for length modifier, default to NONE */
                length_t length_modifier = LENGTH_NONE;

                /* look for two character length modifier */
                if (format[next_index + 1] != '\0')
                {
                    if ((format[next_index] == 'h') && (format[next_index + 1] == 'h'))
                    {
                        length_modifier = LENGTH_HH;
                    }
                    else if ((format[next_index] == 'l') && (format[next_index + 1] == 'l'))
                    {
                        length_modifier = LENGTH_LL;
                    }

                    /* increment next_index if length modifier was found */
                    if (length_modifier != LENGTH_NONE)
                    {
                        next_index += 2;
                    }
                }

                /* look for one character length modifier if two character search failed */
                if ((length_modifier == LENGTH_NONE) && (format[next_index] != '\0'))
                {
                    if (format[next_index] == 'h')
                    {
                        length_modifier = LENGTH_H;
                    }
                    else if (format[next_index] == 'l')
                    {
                        length_modifier = LENGTH_L;
                    }
                    else if (format[next_index] == 'j')
                    {
                        length_modifier = LENGTH_J;
                    }
                    else if (format[next_index] == 'z')
                    {
                        length_modifier = LENGTH_Z;
                    }
                    else if (format[next_index] == 't')
                    {
                        length_modifier = LENGTH_T;
                    }
                    else if (format[next_index] == 'L')
                    {
                        length_modifier = LENGTH_CAPITAL_L;
                    }

                    /* increment next_index if length modifier was found */
                    if (length_modifier != LENGTH_NONE)
                    {
                        next_index++;
                    }
                }

                /* read out character - this is a supported format character, '\0', or a not suported character */
                char next = format[next_index];

                /* signed integer */
                if ((next == 'd') || (next == 'i'))
                {
                    MBED_SIGNED_STORAGE value = 0;

    #if MBED_CONF_MINIMAL_PRINTF_ENABLE_64_BIT
                    /* if 64 bit is enabled and the integer types are larger than the native type */
                    if (((length_modifier == LENGTH_LL)   && (sizeof(long long int) > sizeof(MBED_SIGNED_NATIVE_TYPE))) ||
                        ((length_modifier == LENGTH_L)    && (sizeof(long int)      > sizeof(MBED_SIGNED_NATIVE_TYPE))) ||
                        ((length_modifier == LENGTH_NONE) && (sizeof(int)           > sizeof(MBED_SIGNED_NATIVE_TYPE))))
                    {
                        /* use 64 bit storage type for readout */
                        value = va_arg(arguments, MBED_SIGNED_STORAGE);
                    }
                    else
    #endif
                    {
                        /* use native storage type (which can be 32 or 64 bit) */
                        value = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                    }

                    /* constrict value based on lenght modifier */
                    switch (length_modifier)
                    {
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

                    mbed_minimal_formatted_string_signed(buffer, length, &result, value);
                }
                /* unsigned integer */
                else if ((next == 'u') || (next == 'x') || (next == 'X'))
                {
                    MBED_UNSIGNED_STORAGE value = 0;

    #if MBED_CONF_MINIMAL_PRINTF_ENABLE_64_BIT
                    /* if 64 bit is enabled and the integer types are larger than the native type */
                    if (((length_modifier == LENGTH_LL)   && (sizeof(unsigned long long int) > sizeof(MBED_UNSIGNED_NATIVE_TYPE))) ||
                        ((length_modifier == LENGTH_L)    && (sizeof(unsigned long int)      > sizeof(MBED_UNSIGNED_NATIVE_TYPE))) ||
                        ((length_modifier == LENGTH_NONE) && (sizeof(unsigned int)           > sizeof(MBED_UNSIGNED_NATIVE_TYPE))))
                    {
                        /* use 64 bit storage type for readout */
                        value = va_arg(arguments, MBED_UNSIGNED_STORAGE);
                    }
                    else
    #endif
                    {
                        /* use native storage type (which can be 32 or 64 bit) */
                        value = va_arg(arguments, MBED_UNSIGNED_NATIVE_TYPE);
                    }

                    /* constrict value based on lenght modifier */
                    switch (length_modifier)
                    {
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
                    if (next == 'u')
                    {
                        mbed_minimal_formatted_string_unsigned(buffer, length, &result, value);
                    }
                    else
                    {
                        mbed_minimal_formatted_string_hexadecimal(buffer, length, &result, value);
                    }
                }
    #if MBED_CONF_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
                /* treat all floating points the same */
                else if ((next == 'f') || (next == 'F') || (next == 'g') || (next == 'G'))
                {
                    double value = va_arg(arguments, double);
                    index = next_index;

                    mbed_minimal_formatted_string_double(buffer, length, &result, value);
                }
    #endif
                /* character */
                else if (next == 'c')
                {
                    char value = va_arg(arguments, MBED_SIGNED_NATIVE_TYPE);
                    index = next_index;

                    mbed_minimal_formatted_string_character(buffer, length, &result, value);
                }
                /* string */
                else if (next == 's')
                {
                    char* value = va_arg(arguments, char*);
                    index = next_index;

                    mbed_minimal_formatted_string_string(buffer, length, &result, value);
                }
                /* pointer */
                else if (next == 'p')
                {
                    void* value = va_arg(arguments, void*);
                    index = next_index;

                    mbed_minimal_formatted_string_void_pointer(buffer, length, &result, value);
                }
                else
                {
                    /* write all characters between format beginning and unrecognied modifier */
                    while (index < next_index)
                    {
                        mbed_minimal_formatted_string_character(buffer, length, &result, format[index]);
                        index++;
                    }

                    /* if this is not the end of the string, write unrecognized modifier */
                    if (next != '\0')
                    {
                        mbed_minimal_formatted_string_character(buffer, length, &result, format[index]);
                    }
                    else
                    {
                        /* break out of for loop */
                        break;
                    }
                }
            }
            else
            /* not a format specifier */
            {
                /* write normal character */
                mbed_minimal_formatted_string_character(buffer, length, &result, format[index]);
            }
        }

        /* if writing to buffer, NULL terminate string in reserved space*/
        if (buffer && ((size_t)result < length))
        {
            buffer[result] = '\0';
        }
    }

    return result;
}
