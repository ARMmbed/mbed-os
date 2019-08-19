/*
 *  Hardware entropy collector for the CC3200
 *
 *  Copyright (C) 2018-2019, ARM Limited, All Rights Reserved
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

/*
 * Reference: section 15.1.1.6 True Random Number in swru455e
 */

#if defined(DEVICE_TRNG)

#include "stdio.h"
#include "trng_api.h"
#include "ti/drivers/net/wifi/netutil.h"
#include "CC3220SF_Init.h"

void trng_init(trng_t *obj)
{
    static bool trng_initialized = false;

    (void)obj;
    if (!trng_initialized) {
        int ret = CC3220SF_initSimplelink();
        if (ret == 0) {
            trng_initialized = true;
        } else {
            printf("trng_init failed with %d\n", ret);
        }
    }
}

void trng_free(trng_t *obj)
{
    (void)obj;
}


int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    _u16 bytes_count = length;
    _i16   status;

    (void)obj;
    /* Retrieve a buffer of true random numbers from the networking subsystem.
    Maximum buffer length is 172 bytes for each retrieval. if the requested length exceeds 172 bytes, it is trimmed to 172 bytes.*/
    if (length > 172) {
        bytes_count = 172;
    }
    if (output) {
        status = sl_NetUtilGet(SL_NETUTIL_TRUE_RANDOM, 0, output, &bytes_count);
        if (output_length) {
            if (status == 0) {
                *output_length = bytes_count;
                return 0;
            } else {
                printf("sl_NetUtilGet failed with %d\n", status);
                *output_length = 0;
            }
        }
    }
    return -1;
}

#endif
