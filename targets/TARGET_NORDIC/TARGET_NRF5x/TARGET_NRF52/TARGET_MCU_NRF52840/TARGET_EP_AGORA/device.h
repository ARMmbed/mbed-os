/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#include "objects.h"

/**
 * This allows applications and external libraries to provide
 * structs typically included in objects.h
 *
 * Useful if a chip doesn't have a certain peripheral (eg: CAN) but
 * can be equipped with one by external hardware (eg: CAN via SPI). This allows
 * the standard APIs to be used without any extra hacks.
 */
#if MBED_CONF_TARGET_ENABLE_OBJECTS_EXTENSIONS
#include "objects_extensions.h"
#endif

#endif
