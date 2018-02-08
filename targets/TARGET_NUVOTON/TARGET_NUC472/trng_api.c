/*
 *  Hardware entropy collector for NUC472's RNGA
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#if DEVICE_TRNG

#include <stdlib.h>
#include <string.h>
#include "cmsis.h"
#include "NUC472_442.h"
#include "us_ticker_api.h"
#include "trng_api.h"

/*
 * Get Random number generator.
 */

#define PRNG_KEY_SIZE  (0x20UL)

static volatile int  g_PRNG_done;
volatile int  g_AES_done;

/* Implementation that should never be optimized out by the compiler */
static void trng_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v; while( n-- ) *p++ = 0;
}

void CRYPTO_IRQHandler()
{
    if (PRNG_GET_INT_FLAG()) {
        g_PRNG_done = 1;
        PRNG_CLR_INT_FLAG();
    }  else if (AES_GET_INT_FLAG()) {
        g_AES_done = 1;
        AES_CLR_INT_FLAG();
    }
} 

static void trng_get(unsigned char *pConversionData)
{
    uint32_t *p32ConversionData;

    p32ConversionData = (uint32_t *)pConversionData;

    PRNG_Open(PRNG_KEY_SIZE_256, 1, us_ticker_read());
    PRNG_Start();
    while (!g_PRNG_done);

    PRNG_Read(p32ConversionData);
}

void trng_init(trng_t *obj)
{
    (void)obj;
    /* Unlock protected registers */
    SYS_UnlockReg();    
    /* Enable IP clock */
    CLK_EnableModuleClock(CRPT_MODULE);
    
    /* Lock protected registers */
    SYS_LockReg();
    
    NVIC_EnableIRQ(CRPT_IRQn);
    PRNG_ENABLE_INT();
}

void trng_free(trng_t *obj)
{
    (void)obj;
    PRNG_DISABLE_INT();
    NVIC_DisableIRQ(CRPT_IRQn);
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
        trng_zeroize(tmpBuff, sizeof(tmpBuff));
    }
    *output_length = cur_length;
    return 0;
}
 
#endif

