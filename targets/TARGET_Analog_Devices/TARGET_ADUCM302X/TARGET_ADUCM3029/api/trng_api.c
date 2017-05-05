/*
 *
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

#if defined(DEVICE_TRNG)

#include <stdlib.h>
#include <drivers/rng/adi_rng.h>
#include <services/pwr/adi_pwr.h>
#include "cmsis.h"
#include "trng_api.h"

// Sampling counter values
// Prescaler: 0 - 10
// LenReload: 0 - 4095
#define TRNG_CNT_VAL	4095
#define TRNG_PRESCALER 	2

/* RNG Device Handle */
static ADI_RNG_HANDLE     RNGhDevice;                     /* Memory to handle CRC Device */
static uint8_t            RngDevMem[ADI_RNG_MEMORY_SIZE]; /* Data buffers for Random numbers */

void trng_init(trng_t *obj)
{
    (void)obj;
    adi_rng_Open(0,RngDevMem,sizeof(RngDevMem),&RNGhDevice);

    // Set sample length for the H/W RN accumulator
    adi_rng_SetSampleLen(RNGhDevice, TRNG_PRESCALER, TRNG_CNT_VAL);

    // Enable the RNG
    adi_rng_Enable(RNGhDevice, true);
}

void trng_free(trng_t *obj)
{
    (void)obj;
    adi_rng_Enable(RNGhDevice, false);
    adi_rng_Close(RNGhDevice);
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;
    bool_t bRNGRdy;
    uint32_t nRandomNum, i;

    for (i = 0; i < length; ) {
        adi_rng_GetRdyStatus(RNGhDevice, &bRNGRdy);
        if (bRNGRdy) {
            adi_rng_GetRngData(RNGhDevice, &nRandomNum);
            output[i++] = (uint8_t) nRandomNum;
        }
    }
    *output_length = length;
    return 0;
}

#endif
