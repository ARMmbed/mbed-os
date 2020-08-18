/*
 * Copyright (c) 2014-2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <string.h>
#include "common_functions.h"
#include "ip4string.h"

static void ipv4_itoa(char *string, uint8_t byte);

/**
 * Print binary IPv4 address to a string.
 * String must contain enough room for full address, 16 bytes exact.
 * \param addr IPv4 address.
 * \p buffer to write string to.
 */
uint_fast8_t ip4tos(const void *ip4addr, char *p)
{
    uint_fast8_t outputPos = 0;
    const uint8_t *byteArray = ip4addr;

    for (uint_fast8_t component = 0; component < 4; ++component) {
        //Convert the byte to string
        ipv4_itoa(&p[outputPos], byteArray[component]);

        //Move outputPos to the end of the string
        while (p[outputPos] != '\0') {
            outputPos += 1;
        }

        //Append a dot if this is not the last digit
        if (component < 3) {
            p[outputPos++] = '.';
        }
    }

    // Return length of generated string, excluding the terminating null character
    return outputPos;
}

static void ipv4_itoa(char *string, uint8_t byte)
{
    char *baseString = string;

    //Write the digits to the buffer from the least significant to the most
    //  This is the incorrect order but we will swap later
    do {
        *string++ = '0' + byte % 10;
        byte /= 10;
    } while (byte);

    //We put the final \0, then go back one step on the last digit for the swap
    *string-- = '\0';

    //We now swap the digits
    while (baseString < string) {
        uint8_t tmp = *string;
        *string-- = *baseString;
        *baseString++ = tmp;
    }
}
