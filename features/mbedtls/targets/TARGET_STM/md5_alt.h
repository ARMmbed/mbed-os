/**
 * \file md5.h
 *
 * \brief MD5 message digest algorithm (hash function)
 *
 * \warning   MD5 is considered a weak message digest and its use constitutes a
 *            security risk. We recommend considering stronger message
 *            digests instead.
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
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
 *  This file implements STMicroelectronics MD5 API with HW services based
 *  on mbed TLS API
 */

#ifndef MBEDTLS_MD5_ALT_H
#define MBEDTLS_MD5_ALT_H

#if defined (MBEDTLS_MD5_ALT)
/* Includes ------------------------------------------------------------------*/
#include "hash_stm32.h"

#ifndef MBEDTLS_ERR_MD5_BAD_INPUT_DATA
#define MBEDTLS_ERR_MD5_BAD_INPUT_DATA                     -0x00AF
#endif

#define ST_MD5_BLOCK_SIZE  ((size_t)  64)           /*!< HW handles 512 bits, ie 64 bytes */
#define ST_MD5_NB_HASH_REG ((uint32_t)57)           /*!< Number of HASH HW context Registers:
                                                         CR + STR + IMR + CSR[54] */

/**
 * \brief          MD5 context structure
 *
 * STMicroelectronics edition
 */
typedef struct mbedtls_md5_context {
    HASH_HandleTypeDef hhash;                       /*!< Handle of HASH HAL */
    uint8_t sbuf[ST_MD5_BLOCK_SIZE];                /*!< Buffer to store input data until ST_MD5_BLOCK_SIZE
                                                         is reached, or until last input data is reached */
    uint8_t sbuf_len;                               /*!< Number of bytes stored in sbuf */
    uint32_t ctx_save_regs[ST_MD5_NB_HASH_REG];
}
mbedtls_md5_context;


#endif /* MBEDTLS_MD5_ALT */

#endif /* MBEDTLS_MD5_ALT_H */
