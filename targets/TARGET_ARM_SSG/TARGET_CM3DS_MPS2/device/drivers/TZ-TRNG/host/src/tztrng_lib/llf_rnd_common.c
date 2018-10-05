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

CCError_t LLF_RND_GetRoscSampleCnt(uint32_t rosc, CCRndParams_t *pTrngParams)
{
    switch (rosc) {
    case 0x1:
        pTrngParams->SubSamplingRatio = pTrngParams->SubSamplingRatio1;
        break;
    case 0x2:
        pTrngParams->SubSamplingRatio = pTrngParams->SubSamplingRatio2;
        break;
    case 0x4:
        pTrngParams->SubSamplingRatio = pTrngParams->SubSamplingRatio3;
        break;
    case 0x8:
        pTrngParams->SubSamplingRatio = pTrngParams->SubSamplingRatio4;
        break;
    default:
        return LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR;
    }

    return CC_OK;
}

uint32_t LLF_RND_TRNG_RoscMaskToNum(uint32_t mask)
{
    return (mask == LLF_RND_HW_TRNG_ROSC3_BIT) ? LLF_RND_HW_TRNG_ROSC3_NUM :
            (mask == LLF_RND_HW_TRNG_ROSC2_BIT) ? LLF_RND_HW_TRNG_ROSC2_NUM :
            (mask == LLF_RND_HW_TRNG_ROSC1_BIT) ? LLF_RND_HW_TRNG_ROSC1_NUM :
            LLF_RND_HW_TRNG_ROSC0_NUM;
}

/**
 * The function gets next allowed rosc
 *
 * @author reuvenl (9/12/2012)
 *
 * @param trngParams_ptr - a pointer to params structure.
 * @param rosc_ptr - a pointer to previous rosc /in/, and
 * 			to next rosc /out/.
 * @param isNext - defines is increment of rosc ID needed or not.
 *      	   if isNext = TRUE - the function shifts rosc by one bit; Then
 *      	   the function checks is this rosc allowed, if yes - updates
 *      	   the rosc, else repeats previous steps. If no roscs allowed -
 *      	   returns an error.
 *
 * @return CCError_t
 */
CCError_t LLF_RND_GetFastestRosc(
				  CCRndParams_t *trngParams_ptr,
				  uint32_t *rosc_ptr	 /*in/out*/)
{
	/* setting rosc */
	do {
		if (*rosc_ptr & trngParams_ptr->RoscsAllowed) {
			return CC_OK;
		} else {
			*rosc_ptr <<= 1;
		}

	} while (*rosc_ptr <= 0x08);

	return LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR;
}

