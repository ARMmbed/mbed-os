 /*
  *  trng.c
  *
  *  Copyright (C) 2017, Arm Limited, All Rights Reserved
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

#include <string.h>
#include "trng_api.h"

extern CRYS_RND_State_t     rndState;
extern CRYS_RND_WorkBuff_t  rndWorkBuff;

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v;
    while( n-- ) *p++ = 0;
}

CRYSError_t RNG_PLAT_SetUserRngParameters(
                         CRYS_RND_State_t  *pRndState,
                         CRYS_RND_Params_t *pTrngParams);

CRYSError_t LLF_RND_GetTrngSource(
                         CRYS_RND_State_t  *rndState_ptr,
                         CRYS_RND_Params_t *trngParams_ptr,
                         SaSiBool_t        isContinued,
                         uint32_t          *entropySize_ptr,
                         uint32_t          **sourceOut_ptr_ptr,
                         uint32_t          *sourceOutSize_ptr,
                         uint32_t          *rndWorkBuff_ptr);


void trng_init(trng_t *obj)
{
    RNG_PLAT_SetUserRngParameters(&rndState, obj);
}

void trng_free(trng_t *obj)
{
    (void)obj;
}


int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *outputLength)
{
    (void)obj;
    int ret;
    uint32_t entropySizeBits;
    uint32_t *entrSource_ptr;
    uint32_t actualLength;

    ret = LLF_RND_GetTrngSource(
                &rndState ,    /*in/out*/
                obj,       /*in/out*/
                0,       /*in*/
                &entropySizeBits,  /*in/out*/
                &entrSource_ptr,   /*out*/
                &actualLength,  /*out*/
                (uint32_t*)&rndWorkBuff.crysRndWorkBuff /*in*/);
    if ( ret != 0 )
        return -1;

    if ( length < actualLength )
        actualLength = length;

    *outputLength = actualLength;

    memcpy( output, entrSource_ptr + CRYS_RND_TRNG_SRC_INNER_OFFSET_WORDS, *outputLength );
    mbedtls_zeroize( entrSource_ptr + CRYS_RND_TRNG_SRC_INNER_OFFSET_WORDS, *outputLength );
    return 0;
}

#endif //DEVICE_TRNG
