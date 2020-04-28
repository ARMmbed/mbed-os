/*
 *  Hardware entropy collector for the K66F, using Freescale's RNGA
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
 * Reference: "K66 Sub-Family Reference Manual, Rev. 2", chapter 38
 */

#include <stdlib.h>
#include "cmsis.h"
#include "fsl_trng.h"
#include "trng_api.h"

void trng_init(trng_t *obj)
{
    (void)obj;
    trng_config_t trngConfig;
    TRNG_GetDefaultConfig(&trngConfig);
    TRNG_Init(TRNG0, &trngConfig);
}

void trng_free(trng_t *obj)
{
    (void)obj;
    TRNG_Deinit(TRNG0);
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;

    if(TRNG_GetRandomData(TRNG0, output, length) != kStatus_Success) return -1;
    *output_length = length;

    return 0;
}
