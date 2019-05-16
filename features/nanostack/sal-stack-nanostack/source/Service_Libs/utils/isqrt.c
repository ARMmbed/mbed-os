/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "ns_types.h"
#include "isqrt.h"

/**
 * \brief Calculates integer square root
 *
 * Algorithm is from http://www.codecodex.com/wiki/Calculate_an_integer_square_root
 *
 * \param n number
 *
 * \return square root of the number (rounded down)
 */

uint32_t isqrt32(uint32_t n)
{
    uint32_t root, remainder, place;

    root = 0;
    remainder = n;
    place = 0x40000000;

    while (place > remainder) {
        place = place >> 2;
    }
    while (place)  {
        if (remainder >= root + place) {
            remainder = remainder - root - place;
            root = root + (place << 1);
        }
        root = root >> 1;
        place = place >> 2;
    }
    return root;
}
