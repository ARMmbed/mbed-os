/*
 *  Hardware entropy collector for the STM32 families
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

#if defined(DEVICE_TRNG)

#include <stdlib.h>
#include "cmsis.h"
#include "trng_api.h"

/** trng_get_byte
 *  @brief Get one byte of entropy from the RNG, assuming it is up and running.
 *  @param obj TRNG obj
 *  @param pointer to the hardware generated random byte.
 */
static void trng_get_byte(trng_t *obj, unsigned char *byte )
{
    *byte = (unsigned char)HAL_RNG_GetRandomNumber(&obj->handle);
}

void trng_init(trng_t *obj)
{
#if defined(TARGET_STM32L4)
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /*Select PLLQ output as RNG clock source */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
    PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
#endif

    /* RNG Peripheral clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();

    /* Initialize RNG instance */
    obj->handle.Instance = RNG;
    HAL_RNG_Init(&obj->handle);

    /* first random number generated after setting the RNGEN bit should not be used */
    HAL_RNG_GetRandomNumber(&obj->handle);

}

void trng_free(trng_t *obj)
{
    /*Disable the RNG peripheral */
    HAL_RNG_DeInit(&obj->handle);
    /* RNG Peripheral clock disable - assume we're the only users of RNG  */
    __HAL_RCC_RNG_CLK_DISABLE();
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int ret;

    /* Get Random byte */
    for( uint32_t i = 0; i < length; i++ ){
        trng_get_byte(obj, output + i );
    }

    *output_length = length;
    /* Just be extra sure that we didn't do it wrong */
    if( ( __HAL_RNG_GET_FLAG(&obj->handle, (RNG_FLAG_CECS | RNG_FLAG_SECS)) ) != 0 ) {
        ret = -1;
    } else {
        ret = 0;
    }

    return( ret );
}

#endif
