/*
 *  Hardware entropy collector for the K64F, using Freescale's RNGA
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
#include "cmsis.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "trng_api.h"

void trng_init(trng_t *obj)
{
    (void)obj;
    CLOCK_EnableClock(kCLOCK_Rnga0);
    CLOCK_DisableClock(kCLOCK_Rnga0);
    CLOCK_EnableClock(kCLOCK_Rnga0);
}

void trng_free(trng_t *obj)
{
    (void)obj;
    CLOCK_DisableClock(kCLOCK_Rnga0);
}

/*
 * Get one byte of entropy from the RNG, assuming it is up and running.
 * As recommended, get only one bit of each output.
 */
static void trng_get_byte(unsigned char *byte)
{
    *byte = 0;
    size_t bit;

    /* 34.5 Steps 3-4-5: poll SR and read from OR when ready */
    for( bit = 0; bit < 8; bit++ )
    {
        while((RNG->SR & RNG_SR_OREG_LVL_MASK) == 0 );
        *byte |= (RNG->OR & 1) << bit;
    }
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;
    size_t i;

    /* Set "Interrupt Mask", "High Assurance" and "Go",
     * unset "Clear interrupt" and "Sleep" */
    RNG->CR = RNG_CR_INTM_MASK | RNG_CR_HA_MASK | RNG_CR_GO_MASK;

    for (i = 0; i < length; i++) {
        trng_get_byte(output + i);
    }

    /* Just be extra sure that we didn't do it wrong */
    if ((RNG->SR & RNG_SR_SECV_MASK) != 0) {
        return -1;
    }

    *output_length = length;

    return 0;
}

#endif
