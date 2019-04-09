/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <stdio.h>
#include <string.h>
#include "hal/gpio_api.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_retarget.h"
#include "platform/mbed_critical.h"

WEAK MBED_NORETURN void mbed_die(void)
{
#if !defined (NRF51_H) && !defined(TARGET_EFM32)
    core_util_critical_section_enter();
#endif
    gpio_t led_err;
    gpio_init_out(&led_err, LED1);

    while (1) {
        for (int i = 0; i < 4; ++i) {
            gpio_write(&led_err, 1);
            wait_us(150000);
            gpio_write(&led_err, 0);
            wait_us(150000);
        }

        for (int i = 0; i < 4; ++i) {
            gpio_write(&led_err, 1);
            wait_us(400000);
            gpio_write(&led_err, 0);
            wait_us(400000);
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
    char buffer[132];
    int size = vsnprintf(buffer, sizeof buffer, format, arg);
    if ((unsigned int)size >= sizeof buffer) {
        /* Output was truncated - indicate by overwriting tail of buffer
         * with ellipsis, newline and null terminator.
         */
        static const char ellipsis[] = "...\n";
        memcpy(&buffer[sizeof buffer - sizeof ellipsis], ellipsis, sizeof ellipsis);
    }
    if (size > 0) {
        mbed_error_puts(buffer);
    }
}

void mbed_error_puts(const char *str)
{
    // Writing the string to the console in a critical section is
    // potentially beneficial - for example in UARTSerial it
    // forces the "unbuffered" mode that makes sure all characters
    // go out now. If we made the call not in a critical section,
    // it would go to the software buffer and we would be reliant
    // on platform.stdio-flush-at-exit forcing a fsync before
    // entering mbed_die().
    //
    // But this may be the very first write to the console, and hence
    // require it to be initialized - doing this in a critical
    // section could be problematic. So we prime it outside the
    // critical section with a zero-length write - this forces
    // the initialization.
    //
    // It's still possible that we were in a critical section
    // or interrupt on entry anyway (eg if this is an error coming
    // from inside RTX), so in other areas of the system we suppress
    // things like mutex creation asserts and RTX traps while
    // an error is in progress, so that console initialization
    // may work.
    write(STDERR_FILENO, str, 0);

    core_util_critical_section_enter();
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES || MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES
    char stdio_out_prev = '\0';
    for (; *str != '\0'; str++) {
        if (*str == '\n' && stdio_out_prev != '\r') {
            const char cr = '\r';
            write(STDERR_FILENO, &cr, 1);
        }
        write(STDERR_FILENO, str, 1);
        stdio_out_prev = *str;
    }
#else
    write(STDERR_FILENO, str, strlen(str));
#endif
    core_util_critical_section_exit();
}

void mbed_error_vfprintf(const char *format, va_list arg)
{
    mbed_error_vprintf(format, arg);
}
