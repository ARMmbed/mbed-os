/* mbed Microcontroller Library
 * Copyright (c) 2019 STMicroelectronics
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

#ifndef MBED_FLASH_DATA_H
#define MBED_FLASH_DATA_H

#if DEVICE_FLASH

#ifdef FLASH_SIZE
#undef FLASH_SIZE
#endif

// Only the first the application side is accessible.
#define FLASH_SIZE    ((uint32_t)0xC0000) // 768 Kbytes

#endif
#endif
