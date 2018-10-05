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
#include "tztrng_pal.h"

#define LLF_RND_TRNG90B_MAX_BYTES ( LLF_RND_HW_DMA_EHR_SAMPLES_NUM_ON_TRNG90B_MODE * LLF_RND_HW_TRNG_EHR_WIDTH_IN_BYTES)
#define CC_CONFIG_TRNG90B_ADAPTIVE_PROPORTION_WINDOW_SIZE      1024 /* binary noise source */


#define UINT8_SIZE_IN_BITS  8
#define UINT32_SIZE_IN_BITS (sizeof(uint32_t) * UINT8_SIZE_IN_BITS)
uint32_t getBitsFromUint32Array(uint32_t arrayBitsOffset, uint32_t numOfBits, uint32_t* arr)
{
    uint32_t res = 0;
    uint32_t byteOffset = arrayBitsOffset / UINT32_SIZE_IN_BITS;
    uint32_t bitOffset = arrayBitsOffset % UINT32_SIZE_IN_BITS;
    if (numOfBits > UINT32_SIZE_IN_BITS) {
        return 0;
    }
    res = arr[byteOffset] >> bitOffset;
    /* (UINT32_SIZE_IN_BITS - bitOffset) bits were taken from the first dword. */

    if (UINT32_SIZE_IN_BITS - bitOffset > numOfBits)
    /* we copied more bits than required. zero the extra bits. */
    {
        res &= (0xFFFFFFFF >> (UINT32_SIZE_IN_BITS - numOfBits));
    } else if (UINT32_SIZE_IN_BITS - bitOffset < numOfBits)
    /* we copied less bits than required. copy the next bits from the next dword. */
    {
        numOfBits -= UINT32_SIZE_IN_BITS - bitOffset;
        res |= (arr[byteOffset + 1] & (0xFFFFFFFF >> (UINT32_SIZE_IN_BITS - numOfBits))) << (UINT32_SIZE_IN_BITS - bitOffset);
    }

    return res;
}

/*
  implementation of Repetition Counter Test (NIST SP 800-90B (2nd Draft) 4.4.1)
  C = the cutoff value at which the Repetition Count Test fails
*/
CCError_t LLF_RND_RepetitionCounterTest(uint32_t* pData, uint32_t sizeInBytes, uint32_t C)
{
    uint32_t bitOffset=0;
    uint32_t newSample = 0;
    uint32_t A = 0;         /* the most recently seen sample value */
    uint32_t B = 0;         /* the number of consecutive times that the value A has been seen */
    uint32_t bitsPerSample = 1; /* always use single bit per sample for repetition counter test */


    if (pData == NULL || sizeInBytes == 0 || LLF_RND_TRNG90B_MAX_BYTES < sizeInBytes) {
        return LLF_RND_TRNG_REPETITION_COUNTER_ERROR;
    }

    /* the repetition count test is performed as follows: */
    for (bitOffset = 0; bitOffset <= (sizeInBytes * UINT8_SIZE_IN_BITS) - bitsPerSample; bitOffset += bitsPerSample) {
        newSample = getBitsFromUint32Array(bitOffset, bitsPerSample, (uint32_t*)pData);

        /* 1. Let A be the current sample value. */
        /* 2. Initialize the counter B to 1. */
        if (bitOffset == 0) {
            A = newSample;
            B = 1;
        }
        /* 3. If the next sample value is A, increment B by one. */
        else if (A == newSample) {
            ++B;
            /* If B is equal to C, return an error. */
            if (B == C) {
                return LLF_RND_TRNG_REPETITION_COUNTER_ERROR;
            }
        } else {
            /* Let A be the next sample value. */
            A = newSample;
            /* Initialize the counter B to 1. */
            B = 1;
            /* Repeat Step 3. */
        }
    }
    return CC_OK;
}

/*
  implementation of Adaptive Proportion Test (NIST SP 800-90B (2nd Draft) 4.4.2)
  N = the total number of samples that must be observed in one run of the test, also known as the "window size" of the test
  C = the cutoff value above which the test should fail
*/
static CCError_t LLF_RND_AdaptiveProportionTest(uint32_t* pData, uint32_t sizeInBytes, uint32_t C, uint32_t W)
{
    uint32_t bitOffset=0;
    uint32_t currentSample = 0;
    uint32_t A = 0;         /* the sample value currently being counted */
    uint32_t B = 0;         /* the current number of times that A has been seen in the S samples examined so far */
    uint32_t i = 0;         /* the counter for the number of samples examined in the current window */
    uint32_t bitsPerSample = 1; /* binary source */

    if (pData == NULL || sizeInBytes == 0 || LLF_RND_TRNG90B_MAX_BYTES < sizeInBytes || W == 0 || C == 0) {
        return LLF_RND_TRNG_ADAPTION_PROPORTION_ERROR;
    }

    /* The test is performed as follows: */
    for (bitOffset = 0; bitOffset <= (sizeInBytes * UINT8_SIZE_IN_BITS) - bitsPerSample; bitOffset += bitsPerSample) {
        currentSample = getBitsFromUint32Array(bitOffset, bitsPerSample, (uint32_t*)pData);

        /* 1. Let A be the current sample value. */
        /* 2. Initialize the counter B to 1 */
        if ((bitOffset == 0) || (i == W)) {
            A = currentSample;
            B = 1;
            i = 0;
        }
        /* 3. For i = 1 to W-1 */
        else {
            /* If the next sample is equal to A, increment B by 1. */
            if (A == currentSample) {
                ++B;
            }
        }
        /* 4. If B > C, return error. */
        if (i == W - 1) {
            if (B > C) {
                return LLF_RND_TRNG_ADAPTION_PROPORTION_ERROR;
            }
        }
        ++i;
        /* 5. Go to Step 1. */
    }
    return CC_OK;
}

/*
  implementation of Continuous Testing (NIST SP 800-90B 6.5.1.2)
*/
CCError_t runContinuousTesting(uint32_t* pData, uint32_t sizeInBytes)
{
    CCError_t error = CC_OK;
    uint32_t repC = CC_CONFIG_TRNG90B_REPETITION_COUNTER_CUTOFF;
    uint32_t adpW = CC_CONFIG_TRNG90B_ADAPTIVE_PROPORTION_WINDOW_SIZE;
    uint32_t adpC = CC_CONFIG_TRNG90B_ADAPTIVE_PROPORTION_CUTOFF;

    error = LLF_RND_RepetitionCounterTest(pData, sizeInBytes, repC);
    if (error != CC_OK) {
        return error;
    }
        error = LLF_RND_AdaptiveProportionTest(pData, sizeInBytes, adpC, adpW);
    if (error != CC_OK) {
        return error;
    }

    return CC_OK;
}

