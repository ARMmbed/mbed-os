/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019-2020, STMicroelectronics, All Rights Reserved
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
 *  This file implements ST shared HASH services based on API from mbed TLS
 */

/* Includes ------------------------------------------------------------------*/
#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_MD5_ALT)

#include "hash_stm32.h"

/* Variables -----------------------------------------------------------------*/
/* Mutex protection because of one Hash Hw instance is shared over several   */
/* algorithms (SHA-1, SHA-256, MD5 implementations may be enabled together)  */
#if defined(MBEDTLS_THREADING_C)
mbedtls_threading_mutex_t hash_mutex;
unsigned char hash_mutex_started = 0;
#endif /* MBEDTLS_THREADING_C */

unsigned int hash_context_count = 0;

/* Functions -----------------------------------------------------------------*/

/* Implementation that should never be optimized out by the compiler */
void hash_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char *)v;
    while (n--) {
        *p++ = 0;
    }
}

/* HAL function that should be implemented in the user file */
/**
* @brief HASH MSP Initialization
* This function configures the hardware resources used in this example
* @param hhash: HASH handle pointer
* @retval None
*/
void HAL_HASH_MspInit(HASH_HandleTypeDef *hhash)
{
    /* Peripheral clock enable */
    __HAL_RCC_HASH_CLK_ENABLE();
}

/**
* @brief HASH MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hhash: HASH handle pointer
* @retval None
*/
void HAL_HASH_MspDeInit(HASH_HandleTypeDef *hhash)
{
    /* Peripheral clock disable */
    __HAL_RCC_HASH_CLK_DISABLE();
}

#endif /* MBEDTLS_SHA1_ALT or MBEDTLS_SHA256_ALT or MBEDTLS_MD5_ALT */