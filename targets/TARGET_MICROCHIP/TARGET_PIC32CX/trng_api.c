/***************************************************************************//**
* @file trng_api.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
*******************************************************************************
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/

#include "device.h"
#include "clocking.h"

#if DEVICE_TRNG

#include <string.h>
#include "trng_api.h"
#include "trng.h"

/** Initialize the TRNG peripheral
*
* @param obj The TRNG object
*/
void trng_init(trng_t *obj)
{
    (void)obj;

    /* Enable TRNG clock domain */
    pmc_enable_periph_clk(ID_TRNG);

    /* Get peripheral clock frequency */
    uint32_t ul_cpuhz = sysclk_get_peripheral_hz();

    /* Enable TRNG */
    trng_enable(TRNG, ul_cpuhz);
}


/** Deinitialize the TRNG peripheral
*
* @param obj The TRNG object
*/
void trng_free(trng_t *obj)
{
    (void)obj;

    /* Disable TRNG */
    trng_disable(TRNG);
}

/** Get random data from TRNG peripheral
*
* @param obj The TRNG object
* @param output The pointer to an output array
* @param length The size of output data, to avoid buffer overwrite
* @param output_length The length of generated data
* @return 0 success, -1 fail
*/
int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;
	uint32_t skip;
    uint32_t data;
    size_t idx = 0;
    int i;

    /* Get random data */
    while (idx < length) {

		/* Read out TRNG*/
        data = trng_read_output_data(TRNG);

        for (i = 0; ((i < 4) && (idx < length)); i++) {
            output[idx++] = (data >> (i * 8)) & 0xFF;
        }

        /* Skip next 32 random numbers for better entropy */
        for (skip = 0; skip < 32; skip++) {
            trng_read_output_data(TRNG);
        }
    }

    *output_length = idx;

    /* Zeroize to avoid leakage of entropy on the stack. Also ensure this is not removed by compiler optimizations */
    *((volatile uint32_t*) &data) = 0;

    return (idx == length ? 0 : -1);
}

#endif
