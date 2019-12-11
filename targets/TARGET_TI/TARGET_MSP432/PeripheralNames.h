/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

/* mbed-os internally uses integer values
 * for peripheral names. Instead of defining
 * enums with new names, we simply reuse the
 * macros for the base addresses of the
 * peripheral units, which are defined in
 * msp.h and are self-explanatory (see
 * PeripheralPins.c).
 * So the name types are simply typedefs
 * to integers...
 */

#define DEVICE_SPI_COUNT 8

typedef uint32_t ADCName;
typedef uint32_t UARTName;
typedef uint32_t I2CName;
typedef uint32_t SPIName;
typedef uint32_t PWMName;

#endif
