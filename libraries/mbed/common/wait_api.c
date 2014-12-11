/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "wait_api.h"
#include "us_ticker_api.h"

#if defined(TARGET_RZ_A1H)
#define EXPIRE_US (128849020)
#else
#define EXPIRE_US (0)
#endif

void wait(float s) {
    wait_us(s * 1000000.0f);
}

void wait_ms(int ms) {
    wait_us(ms * 1000);
}

void wait_us(int us) {
    uint32_t start = us_ticker_read();
    uint32_t expire_time = 0;
    uint32_t now;
#if (EXPIRE_US > 0)
    uint32_t last  = start;
#endif

    do {
        now = us_ticker_read();
#if (EXPIRE_US > 0)
        if ( last > now ) expire_time += EXPIRE_US;
        last = now;
#endif
    } while ((now + expire_time - start) < (uint32_t)us);
}
