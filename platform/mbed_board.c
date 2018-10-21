/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <stdio.h>
#include "hal/gpio_api.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_retarget.h"
#include "platform/mbed_critical.h"

WEAK void mbed_die(void)
{
#if !defined (NRF51_H) && !defined(TARGET_EFM32)
    core_util_critical_section_enter();
#endif
    gpio_t led_err;
    gpio_init_out(&led_err, LED1);

    while (1) {
        for (int i = 0; i < 4; ++i) {
            gpio_write(&led_err, 1);
            wait_ms(150);
            gpio_write(&led_err, 0);
            wait_ms(150);
        }

        for (int i = 0; i < 4; ++i) {
            gpio_write(&led_err, 1);
            wait_ms(400);
            gpio_write(&led_err, 0);
            wait_ms(400);
        }
    }
}

void mbed_error_printf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    mbed_error_vprintf(format, arg);
    va_end(arg);
}

void mbed_error_vprintf(const char *format, va_list arg)
{
    core_util_critical_section_enter();
    char buffer[132];
    int size = vsnprintf(buffer, sizeof buffer, format, arg);
    if (size >= sizeof buffer) {
        /* Output was truncated - indicate by overwriting last 4 bytes of buffer
         * with ellipsis and newline.
         * (Note that although vsnprintf always leaves a NUL terminator, we
         * don't need a terminator and can use the entire buffer)
         */
        memcpy(&buffer[sizeof buffer - 4], "...\n", 4);
        size = sizeof buffer;
    }
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES || MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES
    char stdio_out_prev = '\0';
    for (int i = 0; i < size; i++) {
        if (buffer[i] == '\n' && stdio_out_prev != '\r') {
            const char cr = '\r';
            write(STDERR_FILENO, &cr, 1);
        }
        write(STDERR_FILENO, &buffer[i], 1);
        stdio_out_prev = buffer[i];
    }
#else
    write(STDERR_FILENO, buffer, size);
#endif
    core_util_critical_section_exit();
}

void mbed_error_vfprintf(const char *format, va_list arg)
{
    mbed_error_vprintf(format, arg);
}
