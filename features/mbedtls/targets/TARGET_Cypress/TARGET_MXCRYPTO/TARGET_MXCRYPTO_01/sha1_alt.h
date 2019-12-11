/*
 *  mbed Microcontroller Library
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019 Cypress Semiconductor Corporation
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
 */

/**
 * \file     sha1_alt.h
 * \version  1.0
 *
 * \brief    header file - wrapper for mbedtls SHA1 HW acceleration
 *
 */

#if !defined(SHA1_ALT_H)
#define SHA1_ALT_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "config.h"
#endif

#include "crypto_common.h"

#if defined(MBEDTLS_SHA1_ALT)

typedef struct mbedtls_sha1_context {
    cy_hw_crypto_t obj;
    cy_stc_crypto_sha_state_t hashState;         /* Structure used by CY Crypto Driver   */
    cy_stc_crypto_v1_sha1_buffers_t shaBuffers;  /* Structure used by CY Crypto Driver   */
}
mbedtls_sha1_context;

#endif /* MBEDTLS_SHA1_ALT */

#endif /* (SHA1_ALT_H) */
