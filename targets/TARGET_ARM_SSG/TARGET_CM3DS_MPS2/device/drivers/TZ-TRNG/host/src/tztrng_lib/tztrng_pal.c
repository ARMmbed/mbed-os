/******************************************************************************
* Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
* SPDX-License-Identifier: Apache-2.0                                         *
*                                                                             *
* Licensed under the Apache License, Version 2.0 (the "License");             *
* you may not use this file except in compliance with the License.            *
*                                                                             *
* You may obtain a copy of the License at                                     *
* http://www.apache.org/licenses/LICENSE-2.0                                  *
*                                                                             *
* Unless required by applicable law or agreed to in writing, software         *
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
*                                                                             *
* See the License for the specific language governing permissions and         *
* limitations under the License.                                              *
******************************************************************************/
#include "tztrng_defs.h"

uint32_t CC_HalWaitInterrupt(void)
{
    uint32_t isr = 0;
    uint32_t crngterr = 0;
    uint32_t abort = 0;

    #ifdef CHECK_VN_AC_ERR
    uint32_t vnerr = 0;
    #endif

    /* Polling ISR value */
    do {
        isr = CC_HAL_READ_REGISTER(DX_RNG_ISR_REG_OFFSET);
        if (isr & 0x1 << DX_RNG_ISR_EHR_VALID_BIT_SHIFT) {
            return 0;
        }

        if (isr & 0x1 << DX_RNG_ISR_CRNGT_ERR_BIT_SHIFT) {
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), 0x1 << DX_RNG_ISR_CRNGT_ERR_BIT_SHIFT);
            crngterr++;
            TRNG_LOG_DEBUG("CRNGT error detected[%d]\n", (int)crngterr);
            if (crngterr >= TRNG_MAX_CRNGT_ERRORS)
                abort = 1;
        }

        #ifdef CHECK_VN_AC_ERR
        if (isr & 0x1 << DX_RNG_ISR_VN_ERR_BIT_SHIFT) {
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), 0x1 << DX_RNG_ISR_VN_ERR_BIT_SHIFT);
            vnerr++;
            TRNG_LOG_DEBUG("VN error detected[%d]\n", vnerr);
            if (vnerr >= TRNG_MAX_VN_ERRORS)
                abort = 1;
        }

        if (isr & 0x1 << DX_RNG_ISR_AUTOCORR_ERR_BIT_SHIFT) {
            TRNG_LOG_DEBUG("AUTOCORR error detected\n");
            abort = 1;
        }
        #endif

    } while (!abort);

    TRNG_LOG_DEBUG("abort leave..\n");

    return LLF_RND_CRNGT_TEST_FAIL_ERROR;
}

/************************************************************************************/
/**
 * @brief The LLF_RND_TurnOffTrng stops the hardware random bits collection
 *        closes RND clocks and releases HW semaphore.
 *
 * @return CCError_t - On success CC_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
void LLF_RND_TurnOffTrng(void)
{
	/* disable the RND source  */
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG,RND_SOURCE_ENABLE), LLF_RND_HW_RND_SRC_DISABLE_VAL);

	/* clear RNG interrupts */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), 0xFFFFFFFF);

	return;
}

void tztrng_memcpy(uint8_t *dst, uint8_t *src, size_t size)
{
	size_t i;
	for(i = 0; i < size; i++)
		dst[i] = src[i];
}

void tztrng_memset(uint8_t *dst, uint8_t value, size_t size)
{
    size_t i;
    for(i = 0; i < size; i++)
        dst[i] = value;
}

