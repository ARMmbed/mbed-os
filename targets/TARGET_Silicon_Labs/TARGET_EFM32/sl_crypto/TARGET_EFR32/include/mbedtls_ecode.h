/**
 * \file mbedtls_ecode.h
 *
 * \brief Silicon Labs specific mbedtls error code definitions.
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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

#ifndef MBEDTLS_ECODE_H
#define MBEDTLS_ECODE_H

/**
 * This file defines the Silicon Labs specific mbedtls error code layout
 * and values.
 *
 * Error code layout.
 *
 * The standard mbedtls error code layout use the negative space of 16 bits
 * signed integers in order to support all architectures. However the standard
 * mbedtls error code layout is fixed by ARM and does not define a sub-space
 * for additional vendor specific error codes. Additionally the 16 bit
 * requirement is not relevant for the Silicon Labs 32bit MCUs. Therefore
 * Silicon Labs defines a 32 bit signed integer error code layout which does
 * not use the values of the standard mbedtls error code layout given that the
 * 16 most significant bits of standard mbedtls error codes are all ones (which
 * should be the case when running on 32-bit architectures and the application
 * does not strip off the 16 most significant bits):
 * 
 *   bit 31:      Sign bit. 1 to signify that the value is part of the error
 *                code space. 0 for non error codes, in which case all 32 bits
 *                are 0 which means OK (i.e. operation succeded).
 *   bits 24-30:  sub-system id, for example mbedtls, emdrv, etc
 *   bits 16-23:  module id, for example UART, LCD,...
 *   bits 0-15:   error code, specific error code
 *
 * The sub-system id and module id are not allowed to be all ones.
 *
 * Below is the list of module ids for the mbedtls sub-system.
 * Module specific error codes are defined in the header file of
 * each respective module.
 */

#define MBEDTLS_ECODE_BASE  ( 0xF1000000 )  /*!< Base value for all error codes. */

#define MBEDTLS_ECODE_AESDRV_BASE    ( MBEDTLS_ECODE_BASE | 0x00005000 ) /*!< Base value for AESDRV error codes. */
#define MBEDTLS_ECODE_ECC_BASE       ( MBEDTLS_ECODE_BASE | 0x00006000 ) /*!< Base value for ECC error codes. */
#define MBEDTLS_ECODE_SHA_BASE       ( MBEDTLS_ECODE_BASE | 0x0000A000 ) /*!< Base value for SHA error codes. */
#define MBEDTLS_ECODE_CRYPTODRV_BASE ( MBEDTLS_ECODE_BASE | 0x0000C000 ) /*!< Base value for CRYPTO error codes. */
#define MBEDTLS_ERR_CMAC_BASE        ( MBEDTLS_ECODE_AESDRV_BASE | 0xf00) /**< Base value of error codes for CMAC submodule (of AESDRV module). */

#endif /* #ifndef MBEDTLS_ECODE_H */
