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

#if DEVICE_TRNG

#include <stdlib.h>
#include "cmsis.h"
#include "trng_api.h"
#include "mbed_error.h"
#include "mbed_atomic.h"
#if defined (TARGET_STM32WB)
/*  Family specific include for WB with HW semaphores */
#include "hw.h"
#include "hw_conf.h"
#endif

static uint8_t users = 0;

void trng_init(trng_t *obj)
{
    uint32_t dummy;

    /*  We're only supporting a single user of RNG */
    if (core_util_atomic_incr_u8(&users, 1) > 1) {
        error("Only 1 RNG instance supported\r\n");
    }

#if !defined(TARGET_STM32WB)
    /*  Because M0 core of WB also needs RG RNG is already clocked by default */
#if defined(RCC_PERIPHCLK_RNG)
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /*Select PLLQ output as RNG clock source */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
#if ((CLOCK_SOURCE) & USE_PLL_MSI)
    PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_MSI;
#else
    PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
#endif
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error("RNG clock configuration error\n");
    }
#endif
#endif //!defined(TARGET_STM32WB)

    /* RNG Peripheral clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();

    /* Initialize RNG instance */
    obj->handle.Instance = RNG;
    obj->handle.State = HAL_RNG_STATE_RESET;
    obj->handle.Lock = HAL_UNLOCKED;

#if defined(CFG_HW_RNG_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID));
#endif
    HAL_RNG_Init(&obj->handle);

    /* first random number generated after setting the RNGEN bit should not be used */
    HAL_RNG_GenerateRandomNumber(&obj->handle, &dummy);

#if defined(CFG_HW_RNG_SEMID)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
#endif
}

void trng_free(trng_t *obj)
{
#if defined(CFG_HW_RNG_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID));
#endif
    /*Disable the RNG peripheral */
    HAL_RNG_DeInit(&obj->handle);

#if defined(CFG_HW_RNG_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
#else
    /* RNG Peripheral clock disable - assume we're the only users of RNG  */
    __HAL_RCC_RNG_CLK_DISABLE();
#endif

    users = 0;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int ret = 0;
    volatile uint8_t random[4];
    *output_length = 0;

#if defined(CFG_HW_RNG_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID));
#endif

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

#if defined(CFG_HW_RNG_SEMID)
    /*  In case RNG is a shared ressource, get the HW semaphore first */
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
#endif

    return (ret);
}

#endif
