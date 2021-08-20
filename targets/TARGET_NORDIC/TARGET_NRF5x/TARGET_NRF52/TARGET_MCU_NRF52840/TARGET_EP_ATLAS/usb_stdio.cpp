/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
 * Copyright (c) 2021 Embedded Planet, Inc.
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

#ifndef MBED_CONF_EP_ATLAS_ENABLE_USB_STDIO_CONSOLE
#define MBED_CONF_EP_ATLAS_ENABLE_USB_STDIO_CONSOLE 0
#endif

#if MBED_CONF_EP_ATLAS_ENABLE_USB_STDIO_CONSOLE

#include "USBSerial.h"
#include "platform/mbed_retarget.h"

/* Retarget stdio to USBSerial */
mbed::FileHandle *mbed::mbed_target_override_console(int fd)
{
    static USBSerial usb_serial;
    return &usb_serial;
}

#endif
