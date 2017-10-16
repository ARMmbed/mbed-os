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

#include "mbed_printf.h"

#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <stdbool.h>

/* module variable for keeping track of initialization */
static bool not_initialized = true;

/***************************/
/* MBED                    */
/***************************/
#if TARGET_LIKE_MBED
#include "hal/serial_api.h"

#if DEVICE_SERIAL
static serial_t stdio_uart = { 0 };
//#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
//static char stdio_in_prev = 0;
//static char stdio_out_prev = 0;
//#endif
#endif

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
#define MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT 1
#define MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT_MAX_DECIMALS 10
#define MBED_INITIALIZE_PRINT(x) { ; }
#define MBED_PRINT_CHARACTER(x) { printf("%c", x); }
#endif

static void mbed_minimal_formatted_string_long_int(char* buffer, size_t length, int* result, long int value);
static void mbed_minimal_formatted_string_unsigned_long_int(char* buffer, size_t length, int* result, unsigned long int value);
static void mbed_minimal_formatted_string_unsigned_char(char* buffer, size_t length, int* result, int value);
static void mbed_minimal_formatted_string_void_pointer(char* buffer, size_t length, int* result, void* value);
static void mbed_minimal_formatted_string_character(char* buffer, size_t length, int* result, int character);
static void mbed_minimal_formatted_string_string(char* buffer, size_t length, int* result, char* string);

#ifndef MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT
#define MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT 0
#endif

#ifndef MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT_MAX_DECIMALS
#define MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT_MAX_DECIMALS  0
#endif

static void mbed_minimal_formatted_string_long_int(char* buffer, size_t length, int* result, long int value)
{
    /* only continue if buffer can fit at least 1 characters */
    if ((*result + 1) <= (long int) length)
    {
        unsigned long int new_value = 0;

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

            /* get absolute value */
            new_value = (unsigned long int) (-1 * value);
        }
        else
        {
            new_value = (unsigned long int) value;
        }

        /* use unsigned long int function */
        mbed_minimal_formatted_string_unsigned_long_int(buffer, length, result, new_value);
    }
}

static void mbed_minimal_formatted_string_unsigned_long_int(char* buffer, size_t length, int* result, unsigned long int value)
{
    /* only continue if buffer can fit at least 1 characters */
    if ((*result + 1) <= (long int) length)
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
            char scratch[sizeof(unsigned long int) * 3] = { 0 };

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
            for ( ; (*result < (long int) length) && (index > 0); index--)
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

static void mbed_minimal_formatted_string_unsigned_char(char* buffer, size_t length, int* result, int value)
{
    /* only continue if buffer can fit at least 2 characters */
    if ((*result + 2) <= (long int) length)
    {
        unsigned int nibble_one = (value & 0xFF) >> 4;
        unsigned int nibble_two = (value & 0x0F);

        const char int2hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        if (buffer)
        {
            buffer[*result] = int2hex[nibble_one];
            buffer[*result + 1] = int2hex[nibble_two];
        }
        else
        {
            MBED_PRINT_CHARACTER(int2hex[nibble_one]);
            MBED_PRINT_CHARACTER(int2hex[nibble_two]);
        }

        *result += 2;
    }
}

static void mbed_minimal_formatted_string_void_pointer(char* buffer, size_t length, int* result, void* value)
{
    /* only continue if buffer can fit '0x' and twice the size of a void* */
    if ((*result + 2 + 2 * sizeof(void*)) <= length)
    {
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

        for (size_t index = 0; index < sizeof(void*); index++)
        {
            unsigned char byte = (unsigned char) (((unsigned long int) value) >> (8 * (sizeof(void*) - index - 1)));

            mbed_minimal_formatted_string_unsigned_char(buffer, length, result, byte);
        }
    }
}

#if MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT
static void mbed_minimal_formatted_string_double(char* buffer, size_t length, int* result, double value)
{
    /* only continue if buffer can fit at least 1 characters */
    if ((*result + 1) <= (long int) length)
    {
        /* get integer part */
        long int integer = value;

        /* write integer part */
        mbed_minimal_formatted_string_long_int(buffer, length, result, integer);

        /* write decimal point */
        mbed_minimal_formatted_string_character(buffer, length, result, '.');

        /* get decimal part */
        double precision = 1.0;

        for (size_t index = 0; index < MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT_MAX_DECIMALS; index++)
        {
            precision *= 10;
        }

        long int decimal = (value - integer) * precision;

        /* take absolute value */
        if (decimal < 0)
        {
            decimal = -1 * decimal;
        }

        /* write decimal part */
        mbed_minimal_formatted_string_long_int(buffer, length, result, decimal);
    }
}
#endif

static void mbed_minimal_formatted_string_character(char* buffer, size_t length, int* result, int character)
{
    /* only continue if the buffer can fit 1 character */
    if ((*result + 1) <= (long int) length)
    {
        if (buffer)
        {
            buffer[*result] = (char) character;
        }
        else
        {
            MBED_PRINT_CHARACTER((char) character);
        }

        *result += 1;
    }
}

static void mbed_minimal_formatted_string_string(char* buffer, size_t length, int* result, char* string)
{
    /* only continue if the buffer can fit at least 1 character */
    if ((*result + 1) <= (long int) length)
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

static int mbed_minimal_formatted_string(char* buffer, size_t length, const char* format, va_list arguments)
{
    int result = 0;

    /* if writing to buffer, reserve space for NULL termination */
    if (buffer)
    {
        length--;
    }

    /* parse string */
    for (size_t index = 0; format[index] != '\0'; index++)
    {
        /* format specifier begin */
        if (format[index] == '%')
        {
            size_t next_index = index + 1;

            /* while there is room in buffer and format string is not empty */
            while ((next_index < length) && (format[next_index] != '\0') &&
                    /* skip all size modifiers */
                    (((format[next_index] >= '0') && (format[next_index] <= '9')) ||
                     (format[next_index] == 'h') ||
                     (format[next_index] == 'l') ||
                     (format[next_index] == 'j') ||
                     (format[next_index] == 'z') ||
                     (format[next_index] == 't') ||
                     (format[next_index] == 'L') ||
                     (format[next_index] == '.')))
            {
                /* skip to next character */
                next_index++;
            }

            /* read out character - this a supported format character, '\0', or a not suported character */
            char next = format[next_index];

            /* signed integer */
            if ((next == 'd') || (next == 'i'))
            {
                long int value = va_arg(arguments, long int);
                index = next_index;

                mbed_minimal_formatted_string_long_int(buffer, length, &result, value);
            }
            /* unsigned integer */
            else if (next == 'u')
            {
                unsigned long int value = va_arg(arguments, unsigned long int);
                index = next_index;

                mbed_minimal_formatted_string_unsigned_long_int(buffer, length, &result, value);
            }
            /* treat both hexadecimeal modifiers the same */
            else if ((next == 'x') || (next == 'X'))
            {
                int value = va_arg(arguments, int);
                index = next_index;

                mbed_minimal_formatted_string_unsigned_char(buffer, length, &result, value);
            }
#if MBED_CONF_MINIMAL_PRINTF_FLOATING_POINT
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
                int value = va_arg(arguments, int);
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
        {
            mbed_minimal_formatted_string_character(buffer, length, &result, format[index]);
        }
    }

    /* if writing to buffer, NULL terminate string in reserved space*/
    if (buffer)
    {
        buffer[result] = '\0';
    }

    return result;
}

int mbed_printf(const char *format, ...)
{
    MBED_INITIALIZE_PRINT();

    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(NULL, LONG_MAX, format, arguments);
    va_end(arguments);

    return result;
}

int mbed_snprintf(char* buffer, size_t length, const char* format, ...)
{
    MBED_INITIALIZE_PRINT();

    va_list arguments;
    va_start(arguments, format);
    int result = mbed_minimal_formatted_string(buffer, length, format, arguments);
    va_end(arguments);

    return result;
}
