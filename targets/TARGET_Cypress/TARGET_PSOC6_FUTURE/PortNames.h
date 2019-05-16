/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef MBED_PORTNAMES_H
#define MBED_PORTNAMES_H

#ifdef __cplusplus
extern "C" {
#endif

// Port[15-0]
typedef enum {
    Port0  = 0x0,
    Port1  = 0x1,
    Port2  = 0x2,
    Port3  = 0x3,
    Port4  = 0x4,
    Port5  = 0x5,
    Port6  = 0x6,
    Port7  = 0x7,
    Port8  = 0x8,
    Port9  = 0x9,
    Port10 = 0xA,
    Port11 = 0xB,
    Port12 = 0xC,
    Port13 = 0xD,
    Port14 = 0xE
} PortName;

#ifdef __cplusplus
}
#endif
#endif
