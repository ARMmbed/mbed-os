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
#include <stdlib.h>
#include <stdarg.h>
#include "device.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_error.h"
#include "platform/mbed_interface.h"
#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif

static uint8_t error_in_progress = 0;

WEAK void error(const char* format, ...) {

    // Prevent recursion if error is called again
    if (error_in_progress) {
        return;
    }
    error_in_progress = 1;

#ifndef NDEBUG
    va_list arg;
    va_start(arg, format);
    mbed_error_vfprintf(format, arg);
    va_end(arg);
#endif
    exit(1);
}
