/*
 *  Hardware entropy collector for the KW41Z, using Freescale's TRNG
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

/*
 * Reference: "MKW41Z/31Z/21Z Reference Manual", chapter 43
 */

#if defined(DEVICE_TRNG)

#include "fsl_trng.h"
#include "trng_api.h"

void trng_init(trng_t *obj)
{
    trng_config_t trngConfig;

    TRNG_GetDefaultConfig(&trngConfig);
    /* Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.*/
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
    /* Initialize TRNG */
    TRNG_Init(TRNG0, &trngConfig);
}

void trng_free(trng_t *obj)
{
    TRNG_Deinit(TRNG0);
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    status_t result;

    result = TRNG_GetRandomData(TRNG0, output, length);

    if (result == kStatus_Success) {
        *output_length = length;
        return 0;
    } else {
        return -1;
    }
}

#endif
