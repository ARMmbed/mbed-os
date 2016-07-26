/*
 *  Hardware entropy collector for the STM32F7 family
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
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


#include <stdlib.h>
#include "cmsis.h"

/* RNG handler declaration */
RNG_HandleTypeDef RngHandle;

/*
 * Get one byte of entropy from the RNG, assuming it is up and running.
 */
static void rng_get_byte( unsigned char *byte )
{
    *byte = (unsigned char)HAL_RNG_GetRandomNumber(&RngHandle);
}

/*
 * Get len bytes of entropy from the hardware RNG.
 */
int mbedtls_hardware_poll( void *data,
                    unsigned char *output, size_t len, size_t *olen )
{
    size_t i;
    int ret;
    ((void) data);

    /* RNG Peripheral clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();

    /* Initialize RNG instance */
    RngHandle.Instance = RNG;
    HAL_RNG_Init(&RngHandle);


    /* Get Random byte */
    for( i = 0; i < len; i++ )
        rng_get_byte( output + i );

    /* Just be extra sure that we didn't do it wrong */
    if( ( __HAL_RNG_GET_FLAG(&RngHandle, (RNG_FLAG_CECS|RNG_FLAG_SECS)) ) != 0 )
    {
        ret = -1;
        goto cleanup;
    }

    *olen = len;
    ret = 0;

cleanup:
    /*Disable the RNG peripheral */
    HAL_RNG_DeInit(&RngHandle);
    /* RNG Peripheral clock disable - assume we're the only users of RNG  */
    __HAL_RCC_RNG_CLK_DISABLE();


    return( ret );
}

