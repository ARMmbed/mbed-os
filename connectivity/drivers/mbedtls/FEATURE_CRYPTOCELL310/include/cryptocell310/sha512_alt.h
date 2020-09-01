/*
 *  sha512_alt.h
 *
 *  Copyright (C) 2019, ARM Limited, All Rights Reserved
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

#ifndef __SHA512_ALT__
#define __SHA512_ALT__

#if defined(MBEDTLS_SHA512_ALT)

#include "crys_hash.h"

/**
 * \brief          SHA-512 context structure
 */
typedef struct
{
    CRYS_HASHUserContext_t crys_hash_ctx;
} mbedtls_sha512_context;

#endif // MBEDTLS_SHA256_ALT__
#endif //__SHA256_ALT__
