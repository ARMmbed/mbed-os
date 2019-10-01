/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/


#include "pal_attestation_intf.h"

/**
    @brief    - This API will call the requested attestation function
    @param    - type    : function code
                valist  : variable argument list
    @return   - error status
**/
int32_t pal_attestation_function(int type, va_list valist)
{
    uint8_t     *challenge, *token;
    uint32_t    challenge_size, *token_size, verify_token_size;

    switch (type)
    {
        case PAL_INITIAL_ATTEST_GET_TOKEN:
            challenge = va_arg(valist, uint8_t*);
            challenge_size = va_arg(valist, uint32_t);
            token = va_arg(valist, uint8_t*);
            token_size = va_arg(valist, uint32_t*);
            return psa_initial_attest_get_token(challenge, challenge_size, token, token_size);
        case PAL_INITIAL_ATTEST_GET_TOKEN_SIZE:
            challenge_size = va_arg(valist, uint32_t);
            token_size = va_arg(valist, uint32_t*);
            return psa_initial_attest_get_token_size(challenge_size, token_size);
        case PAL_INITIAL_ATTEST_VERIFY_TOKEN:
            challenge = va_arg(valist, uint8_t*);
            challenge_size = va_arg(valist, uint32_t);
            token = va_arg(valist, uint8_t*);
            verify_token_size = va_arg(valist, uint32_t);
            return pal_initial_attest_verify_token(challenge, challenge_size,
                                                   token, verify_token_size);
        default:
            return PAL_STATUS_UNSUPPORTED_FUNC;
    }
}
