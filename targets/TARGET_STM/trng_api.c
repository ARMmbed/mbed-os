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
#include "mbed_error.h"
#include "mbed_critical.h"

static uint8_t users = 0;

void trng_init(trng_t *obj)
{
    uint32_t dummy;

    /*  We're only supporting a single user of RNG */
    if (core_util_atomic_incr_u8(&users, 1) > 1) {
        error("Only 1 RNG instance supported\r\n");
    }

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
    obj->handle.State = HAL_RNG_STATE_RESET;
    obj->handle.Lock = HAL_UNLOCKED;

    HAL_RNG_Init(&obj->handle);

    /* first random number generated after setting the RNGEN bit should not be used */
    HAL_RNG_GenerateRandomNumber(&obj->handle, &dummy);
}

void trng_free(trng_t *obj)
{
    /*Disable the RNG peripheral */
    HAL_RNG_DeInit(&obj->handle);
    /* RNG Peripheral clock disable - assume we're the only users of RNG  */
    __HAL_RCC_RNG_CLK_DISABLE();

    users = 0;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int ret = 0;
    volatile uint8_t random[4];
    *output_length = 0;

    /* Get Random byte */
    while ((*output_length < length) && (ret == 0)) {
        if (HAL_RNG_GenerateRandomNumber(&obj->handle, (uint32_t *)random) != HAL_OK) {
            ret = -1;
        } else {
            for (uint8_t i = 0; (i < 4) && (*output_length < length) ; i++) {
                *output++ = random[i];
                *output_length += 1;
                random[i] = 0;
            }
        }
    }

    /* Just be extra sure that we didn't do it wrong */
    if ((__HAL_RNG_GET_FLAG(&obj->handle, (RNG_FLAG_CECS | RNG_FLAG_SECS))) != 0) {
        ret = -1;
    }

    return (ret);
}

#endif
