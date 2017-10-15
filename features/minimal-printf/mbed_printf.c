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
#define MBED_INITIALIZE_PRINT(x) { ; }
#define MBED_PRINT_CHARACTER(x) { printf("%c", x); }
#endif

static void mbed_minimal_formatted_string_long_int(char* buffer, size_t length, int* result, long int value);
static void mbed_minimal_formatted_string_unsigned_long_int(char* buffer, size_t length, int* result, unsigned long int value);
static void mbed_minimal_formatted_string_unsigned_char(char* buffer, size_t length, int* result, int value);
static void mbed_minimal_formatted_string_void_pointer(char* buffer, size_t length, int* result, void* value);
static void mbed_minimal_formatted_string_character(char* buffer, size_t length, int* result, int character);
static void mbed_minimal_formatted_string_string(char* buffer, size_t length, int* result, char* string);

static void mbed_minimal_formatted_string_long_int(char* buffer, size_t length, int* result, long int value)
{
    /* only continue if buffer can fit at least 1 characters */
    if ((*result + 1) <= (int) length)
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
    if ((*result + 1) <= (int) length)
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
            for ( ; (*result < (int) length) && (index > 0); index--)
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
    if ((*result + 2) <= (int) length)
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

static void mbed_minimal_formatted_string_character(char* buffer, size_t length, int* result, int character)
{
    /* only continue if the buffer can fit 1 character */
    if ((*result + 1) <= (int) length)
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
    if ((*result + 1) <= (int) length)
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
            char next = format[index + 1];

            if (next == 'd')
            {
                long int value = va_arg(arguments, long int);
                index++;

                mbed_minimal_formatted_string_long_int(buffer, length, &result, value);
            }
            else if (next == 'u')
            {
                unsigned long int value = va_arg(arguments, unsigned long int);
                index++;

                mbed_minimal_formatted_string_unsigned_long_int(buffer, length, &result, value);
            }
            else if (next == 'X')
            {
                int value = va_arg(arguments, int);
                index++;

                mbed_minimal_formatted_string_unsigned_char(buffer, length, &result, value);
            }
            else if (next == 'p')
            {
                void* value = va_arg(arguments, void*);
                index++;

                mbed_minimal_formatted_string_void_pointer(buffer, length, &result, value);
            }
            else if (next == 'c')
            {
                int value = va_arg(arguments, int);
                index++;

                mbed_minimal_formatted_string_character(buffer, length, &result, value);
            }
            else if (next == 's')
            {
                char* value = va_arg(arguments, char*);
                index++;

                mbed_minimal_formatted_string_string(buffer, length, &result, value);
            }
            else if (next == '\0')
            {
                mbed_minimal_formatted_string_character(buffer, length, &result, '%');
            }
            else 
            {
                mbed_minimal_formatted_string_character(buffer, length, &result, '%');                
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
    int result = mbed_minimal_formatted_string(NULL, ULONG_MAX, format, arguments);
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
