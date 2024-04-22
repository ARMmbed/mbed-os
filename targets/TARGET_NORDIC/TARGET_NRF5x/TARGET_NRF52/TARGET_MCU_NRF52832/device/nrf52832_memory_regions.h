/*
 * Copyright (c) 2024 ARM Limited
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

#ifndef NRF52832_MEMORY_REGIONS_H
#define NRF52832_MEMORY_REGIONS_H

/* Default to no softdevice */
#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x0
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 0x80000
#endif

#if !defined(MBED_RAM_START)
  #define MBED_RAM_START 0x20000000
  #define MBED_RAM_SIZE 0x10000
#endif

#define MBED_RAM0_START MBED_RAM_START
#define MBED_RAM0_SIZE  0xE0
#define MBED_RAM1_START (MBED_RAM_START + MBED_RAM0_SIZE)
#define MBED_RAM1_SIZE  (MBED_RAM_SIZE - MBED_RAM0_SIZE)

#endif //NRF52832_MEMORY_REGIONS_H
