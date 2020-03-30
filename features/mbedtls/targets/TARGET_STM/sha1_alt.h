/**
 * \file sha1.h
 *
 * \brief This file contains SHA-1 definitions and functions.
 *
 * The Secure Hash Algorithm 1 (SHA-1) cryptographic hash function is defined in
 * <em>FIPS 180-4: Secure Hash Standard (SHS)</em>.
 *
 * \warning   SHA-1 is considered a weak message digest and its use constitutes
 *            a security risk. We recommend considering stronger message
 *            digests instead.
 */
/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
 *  Copyright (C) 2019-2020, STMicroelectronics, All Rights Reserved
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
 *  This file implements STMicroelectronics SHA1 API with HW services based
 *  on mbed TLS API
 */

#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#if defined (MBEDTLS_SHA1_ALT)
/* Includes ------------------------------------------------------------------*/
#include "hash_stm32.h"

#define ST_SHA1_BLOCK_SIZE  ((size_t)  64)          /*!< HW handles 512 bits, ie 64 bytes */
#define ST_SHA1_NB_HASH_REG ((uint32_t)57)          /*!< Number of HASH HW context Registers:
                                                         CR + STR + IMR + CSR[54] */

/**
 * \brief          SHA-1 context structure
 */
typedef struct mbedtls_sha1_context {
    HASH_HandleTypeDef hhash;                       /*!< Handle of HASH HAL */
    uint8_t sbuf[ST_SHA1_BLOCK_SIZE];               /*!< Buffer to store input data until ST_SHA1_BLOCK_SIZE
                                                         is reached, or until last input data is reached */
    uint8_t sbuf_len;                               /*!< Number of bytes stored in sbuf */
    uint32_t ctx_save_regs[ST_SHA1_NB_HASH_REG];
}
mbedtls_sha1_context;


#endif /* MBEDTLS_SHA1_ALT */

#endif /* MBEDTLS_SHA1_ALT_H */