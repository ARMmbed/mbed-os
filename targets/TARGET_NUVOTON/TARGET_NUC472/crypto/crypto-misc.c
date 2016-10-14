/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
#include "mbed_assert.h"
#include "nu_modutil.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"

static int crypto_inited = 0;
static int crypto_sha_avail = 1;

void crypto_init(void)
{
    if (crypto_inited) {
        return;
    }
    crypto_inited = 1;
    
    CLK_EnableModuleClock(CRPT_MODULE);
}

/* Implementation that should never be optimized out by the compiler */
void crypto_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char*) v;
    while (n--) {
        *p++ = 0;
    }
}

int crypto_sha_acquire(void)
{
    if (crypto_sha_avail) {
        crypto_sha_avail = 0;
        return 1;
    }
    else {
        return 0;
    }
    
}

void crypto_sha_release(void)
{
    if (! crypto_sha_avail) {
        crypto_sha_avail = 1;
    }
}
