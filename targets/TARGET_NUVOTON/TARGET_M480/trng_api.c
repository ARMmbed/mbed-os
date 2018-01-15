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

#if DEVICE_TRNG

#include <stdlib.h>
#include <string.h>
#include "cmsis.h"
#include "us_ticker_api.h"
#include "trng_api.h"
#include "crypto-misc.h"

/*
 * Get Random number generator.
 */

#define PRNG_KEY_SIZE  (0x20UL)

static void trng_get(unsigned char *pConversionData)
{
    uint32_t *p32ConversionData;

    p32ConversionData = (uint32_t *)pConversionData;

    PRNG_Open(PRNG_KEY_SIZE_256, 1, us_ticker_read());
    crypto_prng_prestart();
    PRNG_Start();
    crypto_prng_wait();

    PRNG_Read(p32ConversionData);
}

void trng_init(trng_t *obj)
{
    (void)obj;
    
    /* Init crypto module */
    crypto_init();
    
    PRNG_ENABLE_INT();
}

void trng_free(trng_t *obj)
{
    (void)obj;
    
    PRNG_DISABLE_INT();
    
    /* Uninit crypto module */
    crypto_uninit();
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;
    unsigned char tmpBuff[PRNG_KEY_SIZE];
    size_t cur_length = 0;
    
    while (length >= sizeof(tmpBuff)) {
        trng_get(output);
        output += sizeof(tmpBuff);
        cur_length += sizeof(tmpBuff);
        length -= sizeof(tmpBuff);
    }
    if (length > 0) {
        trng_get(tmpBuff);
        memcpy(output, tmpBuff, length);
        cur_length += length;
        crypto_zeroize(tmpBuff, sizeof(tmpBuff));
    }
    *output_length = cur_length;
    return 0;
}

#endif
