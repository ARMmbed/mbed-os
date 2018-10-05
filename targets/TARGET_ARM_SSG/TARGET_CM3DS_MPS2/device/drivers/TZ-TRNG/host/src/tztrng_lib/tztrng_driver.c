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

unsigned long gCcRegBase = 0;

static CCError_t RNG_PLAT_SetUserRngParameters(CCRndParams_t *pTrngParams)
{
    CCError_t  error = CC_OK;

    /* Set TRNG parameters */
    pTrngParams->SubSamplingRatio1 = CC_CONFIG_SAMPLE_CNT_ROSC_1;
    pTrngParams->SubSamplingRatio2 = CC_CONFIG_SAMPLE_CNT_ROSC_2;
    pTrngParams->SubSamplingRatio3 = CC_CONFIG_SAMPLE_CNT_ROSC_3;
    pTrngParams->SubSamplingRatio4 = CC_CONFIG_SAMPLE_CNT_ROSC_4;

    /* Allowed ROSCs lengths b'0-3. If bit value 1 - appropriate ROSC is allowed. */
    pTrngParams->RoscsAllowed = (((pTrngParams->SubSamplingRatio1 > 0) ? 0x1 : 0x0) |
            ((pTrngParams->SubSamplingRatio2 > 0) ? 0x2 : 0x0) |
            ((pTrngParams->SubSamplingRatio3 > 0) ? 0x4 : 0x0) |
            ((pTrngParams->SubSamplingRatio4 > 0) ? 0x8 : 0x0));
    pTrngParams->SubSamplingRatio = 0;

    if (0 == pTrngParams->RoscsAllowed) {
        TRNG_LOG_DEBUG("No ROSC allowed!!\n");
        return LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR;
    }

    return error;
}

uint32_t CC_TrngGetSource(unsigned long rngRegBase, uint8_t *outAddr, size_t *outLen, size_t reqBits)
{
    CCError_t Err = CC_OK;
    uint32_t rndWorkBuff[CC_RND_WORK_BUFFER_SIZE_WORDS];
    CCRndParams_t rndParams;
    CCRndState_t rndState = {0};
    uint32_t  *rndSrc_ptr;
    uint32_t  sourceOutSizeBytes = 0;
    uint32_t requireBytes = (reqBits % 8) ? (reqBits / 8 + 1) : (reqBits / 8);

    if (NULL == outAddr || NULL == outLen) {
        TRNG_LOG_DEBUG("NULL pointer!!\n");
        return LLF_RND_TRNG_ILLEGAL_PTR_ERROR;
    }

    /* zeroe the rnd buff  */
    tztrng_memset((uint8_t *)rndWorkBuff, 0, sizeof(rndWorkBuff));
    tztrng_memset((uint8_t *)&rndParams, 0, sizeof(CCRndParams_t));
    gCcRegBase = rngRegBase;

    if (gCcRegBase == 0) {
        TRNG_LOG_DEBUG("register base not initialized\n");
        return LLF_RND_TRNG_ILLEGAL_PTR_ERROR;
    }

    /* Get parameters and set them in the RND structures */
    Err = RNG_PLAT_SetUserRngParameters(&rndParams);
    if (Err != CC_OK)
        return Err;

    Err = LLF_RND_RunTrngStartupTest(&rndState, &rndParams, rndWorkBuff);
    if (Err != CC_OK)
        return Err;

    *outLen = requireBytes;
    while (requireBytes > 0) {
        /* Get TRNG Source */
        Err = LLF_RND_GetTrngSource(
                &rndState,
                &rndParams,
                0,
                &rndSrc_ptr,
                &sourceOutSizeBytes,
                rndWorkBuff);

        if (Err) {
            TRNG_LOG_DEBUG("LLF_RND_GetTrngSource failed, err[0x%X]\n", (unsigned int)Err);
            /* memset 0 to outAddr for security concern */
            tztrng_memset(outAddr, 0, *outLen);
            *outLen = 0;
            return Err;
        }

        /* try to reduce the memcpy */
        if (requireBytes < sourceOutSizeBytes) {
            tztrng_memcpy(outAddr, (uint8_t *)rndSrc_ptr, requireBytes);
            requireBytes = 0;
        } else {
            tztrng_memcpy(outAddr, (uint8_t *)rndSrc_ptr, sourceOutSizeBytes);
            requireBytes -= sourceOutSizeBytes;
        }

        outAddr += sourceOutSizeBytes;
    }

    /* Clear the rndWorkBuff to not leave entropy on the stack */
    tztrng_memset((uint8_t *)rndWorkBuff, 0, sizeof(rndWorkBuff));

    return Err;
}

