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
#ifndef _TZTRNG_H_
#define _TZTRNG_H_

#include <stdint.h>

/*******************************************************************************/
/**
 * @brief The CC_TrngGetSource reads random source of needed size from TRNG.
 *
 * @param[in] rngRegBase - TRNG base address, given by the system.
 * @param[out] *outAddr - The pointer to the buffer address for TRNG data output,
 *                        prepared by the caller.
 * @param[out] *outLen - The pointer to size of TRNG returned data.
 * @param[in] reqBits - The request size of entropy in bits.
 *
 * @return uint32_t - On success 0 is returned, on failure, check CC_Error_t codes
 *
 */
uint32_t CC_TrngGetSource(unsigned long rngRegBase, /* in */
                        uint8_t *outAddr,           /* out */
                        size_t *outLen,             /* out */
                        size_t reqBits);            /* in */

#endif
