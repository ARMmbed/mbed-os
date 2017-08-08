/*
 *  cc_rand.h
 *
 *  Copyright (C) 2018, ARM Limited, All Rights Reserved
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
#ifndef __CC_RAND_H__
#define __CC_RAND_H__
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    int  (*f_rng)(void* ctx, unsigned char* output, size_t outSizeBytes);
    void* ctx;

}mbedtls_rand_func_container;


uint32_t mbedtls_to_cc_rand_func( void *mbedtls_rnd_ctx, uint16_t outSizeBytes, uint8_t *out_ptr )
{
    uint16_t i = 0;
    uint8_t temp = 0;
    mbedtls_rand_func_container* mbedtls_rand = (mbedtls_rand_func_container*)mbedtls_rnd_ctx;
    uint32_t ret = mbedtls_rand->f_rng( mbedtls_rand->ctx, out_ptr, outSizeBytes );
    if( ret != 0 )
        return ret;

    /*
     * CC requires the random data as LE, so reversing the data
     * (although this is random, but test vectors are in specific Endianess)
     */
    while ( i < ( outSizeBytes / 2 ) )
    {
        temp = out_ptr[outSizeBytes - 1 - i];
        out_ptr[outSizeBytes - 1 - i] = out_ptr[i];
        out_ptr[i] = temp;
        ++i;
    }
    /*
     * CC increases the random data by one, to put the vector in the proper range (1 to  n),
     * The RFC tests supply a data buffer within range, and in order to generate the proper ephemeral key,
     * need to decrease one from this data, before CC increases the data, so the output will be as expected
     */
    i = 0;
    while( out_ptr[i] == 0 )
    {
        ++i;
    }
    while( i > 0 )
    {
        --out_ptr[i];
        --i;
    }
    --out_ptr[0];
    return ret;
}


#endif/* __CC_RAND_H__ */
