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
#include <adi_rng.h>
#include <adi_pwr.h>
#include "cmsis.h"
#include "trng_api.h"

static ADI_RNG_HANDLE     RNGhDevice;                     /* Memory to handle CRC Device */
static uint32_t           RngDevMem[(ADI_RNG_MEMORY_SIZE + 3)/4]; /* Data buffers for Random numbers */

void trng_init(trng_t *obj)
{
    (void)obj;
	adi_rng_Open(0,RngDevMem,sizeof(RngDevMem),&RNGhDevice);
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
	bool bRNGRdy;
	uint32_t nRandomNum, i;
	adi_rng_SetSampleLen(RNGhDevice,1,(length + 1)/2);
	adi_rng_Enable(RNGhDevice, true);
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
