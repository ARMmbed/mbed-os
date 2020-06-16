/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019-2020 STMicroelectronics, All Rights Reserved
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
 *  This file implements ST shared CRYP services based on API from mbed TLS
 */

/* Includes ------------------------------------------------------------------*/
#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if !(TARGET_STM32L4)
#if defined(MBEDTLS_AES_ALT) || defined(MBEDTLS_CCM_ALT) || defined(MBEDTLS_GCM_ALT)

#include "cryp_stm32.h"


/* Variables -----------------------------------------------------------------*/
/* Mutex protection because of one Crypt Hw instance is shared over several   */
/* mode of operations (AES, GCM, CCM implementations may be enabled together) */
#if defined(MBEDTLS_THREADING_C)
mbedtls_threading_mutex_t cryp_mutex;
unsigned char cryp_mutex_started = 0;
#endif /* MBEDTLS_THREADING_C */

unsigned int cryp_context_count = 0;

/* Functions -----------------------------------------------------------------*/

/* Implementation that should never be optimized out by the compiler */
void cryp_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char *)v;
    while (n--) {
        *p++ = 0;
    }
}


/* HAL function that should be implemented in the user file */
/**
  * @brief CRYP MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripherals clock enable
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
#if defined (AES)
    /* Enable CRYP clock */
    __HAL_RCC_AES_CLK_ENABLE();

    /* Force the CRYP Peripheral Clock Reset */
    __HAL_RCC_AES_FORCE_RESET();

    /* Release the CRYP Peripheral Clock Reset */
    __HAL_RCC_AES_RELEASE_RESET();
#elif defined (AES1)
    /* Enable CRYP clock */
    __HAL_RCC_AES1_CLK_ENABLE();

    /* Force the CRYP Peripheral Clock Reset */
    __HAL_RCC_AES1_FORCE_RESET();

    /* Release the CRYP Peripheral Clock Reset */
    __HAL_RCC_AES1_RELEASE_RESET();
#else /* CRYP */
    /* Enable CRYP clock */
    __HAL_RCC_CRYP_CLK_ENABLE();

    /* Force the CRYP Peripheral Clock Reset */
    __HAL_RCC_CRYP_FORCE_RESET();

    /* Release the CRYP Peripheral Clock Reset */
    __HAL_RCC_CRYP_RELEASE_RESET();
#endif /* AES */
}

/**
  * @brief CRYP MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripherals clock
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
#if defined (AES)
    __HAL_RCC_AES_CLK_DISABLE();
#elif defined (AES1)
    __HAL_RCC_AES1_CLK_DISABLE();
#else /* CRYP */
    /* Disable CRYP clock */
    __HAL_RCC_CRYP_CLK_DISABLE();
#endif /* AES */
}
#endif /* MBEDTLS_AES_ALT or MBEDTLS_CCM_ALT or MBEDTLS_GCM_ALT */
#endif /* ! TARGET_STM32L4 */
