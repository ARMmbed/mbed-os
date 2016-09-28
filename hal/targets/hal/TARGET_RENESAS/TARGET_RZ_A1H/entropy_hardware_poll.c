/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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

#include <stdlib.h>
#include "us_ticker_api.h"

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    /* "Please update this function based on the security implementation on your platform" */

    uint32_t i;

    *olen = 0;

    if (len == 0) {
        return (0);
    }

    srand(us_ticker_read());

    for (i = 0; i < len; i++) {
        output[i] = rand() & 0xff;

        if ((i & 0x7) == 0x07) {
            srand(us_ticker_read());
        }
    }

    *olen = len;

    return (0);
}
