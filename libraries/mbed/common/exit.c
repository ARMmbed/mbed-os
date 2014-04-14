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
#include "semihost_api.h"
#include "mbed_interface.h"
#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif

#ifdef TOOLCHAIN_GCC_CW
// TODO: Ideally, we would like to define directly "_ExitProcess"
void mbed_exit(int return_code) {
#else
void exit(int return_code) {
#endif

#if DEVICE_STDIO_MESSAGES
    fflush(stdout);
    fflush(stderr);
#endif

#if DEVICE_SEMIHOST
    if (mbed_interface_connected()) {
        semihost_exit();
    }
#endif
    if (return_code) {
        mbed_die();
    }

    while (1);
}
