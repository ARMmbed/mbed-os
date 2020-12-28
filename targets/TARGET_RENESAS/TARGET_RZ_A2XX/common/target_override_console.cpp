/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 Renesas Electronics Corporation
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

#if defined(OVERRIDE_CONSOLE_USBSERIAL)

#include "USBSerial_FileHandle.h"
#include "platform/mbed_wait_api.h"

static mbed::USBSerial_FileHandle console(false);

mbed::FileHandle *mbed::mbed_target_override_console(int fd)
{
    console.connect();
    for (int i = 0; (i < 50) && (console.ready() == false); i++) {
        wait_ms(10);
    }
    return &console;
}

#endif
