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

#include "cmsis.h"

// Bit-Band base addresses of the 10 GPIO ports
__IO uint8_t *gpio_bitband_base[10] = {
    &BITBAND_PERI(P1->IN, 0), &BITBAND_PERI(P2->IN, 0),
    &BITBAND_PERI(P3->IN, 0), &BITBAND_PERI(P4->IN, 0),
    &BITBAND_PERI(P5->IN, 0), &BITBAND_PERI(P6->IN, 0),
    &BITBAND_PERI(P7->IN, 0), &BITBAND_PERI(P8->IN, 0),
    &BITBAND_PERI(P9->IN, 0), &BITBAND_PERI(P10->IN, 0)
};

// Standard base addresses of the 10 ports
__I uint8_t *port_base[10] = {
    &P1->IN, &P2->IN, &P3->IN, &P4->IN, &P5->IN,
    &P6->IN, &P7->IN, &P8->IN, &P9->IN, &P10->IN
};
