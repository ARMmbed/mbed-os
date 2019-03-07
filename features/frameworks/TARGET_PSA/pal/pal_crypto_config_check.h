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

/**
 * \file pal_crypto_config_check.h
 *
 * \brief Consistency checks for configuration options
 *
 */

#ifndef _PAL_CRYPTO_CONFIG_CHECK_H_
#define _PAL_CRYPTO_CONFIG_CHECK_H_

#if defined(ARCH_TEST_RSA_1024) && !defined(ARCH_TEST_RSA)
#error "ARCH_TEST_RSA_1024 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_RSA_2048) && !defined(ARCH_TEST_RSA)
#error "ARCH_TEST_RSA_2048 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_RSA_3072) && !defined(ARCH_TEST_RSA)
#error "ARCH_TEST_RSA_3072 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_ECC_CURVE_SECP192R1) && !defined(ARCH_TEST_ECC)
#error "ARCH_TEST_ECC_CURVE_SECP192R1 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_ECC_CURVE_SECP224R1) && !defined(ARCH_TEST_ECC)
#error "ARCH_TEST_ECC_CURVE_SECP224R1 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_ECC_CURVE_SECP256R1) && !defined(ARCH_TEST_ECC)
#error "ARCH_TEST_ECC_CURVE_SECP256R1 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_ECC_CURVE_SECP384R1) && !defined(ARCH_TEST_ECC)
#error "ARCH_TEST_ECC_CURVE_SECP384R1 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_AES_128) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_AES_128 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_AES_256) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_AES_256 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_AES_512) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_AES_512 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_DES_1KEY) && !defined(ARCH_TEST_DES)
#error "ARCH_TEST_DES_1KEY defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_DES_2KEY) && !defined(ARCH_TEST_DES)
#error "ARCH_TEST_DES_2KEY defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_DES_3KEY) && !defined(ARCH_TEST_DES)
#error "ARCH_TEST_DES_3KEY defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CIPER_MODE_CTR) && !defined(ARCH_TEST_CIPER)
#error "ARCH_TEST_CIPER_MODE_CTR defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CIPER_MODE_CFB) && !defined(ARCH_TEST_CIPER)
#error "ARCH_TEST_CIPER_MODE_CFB defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CIPER_MODE_CBC) && !defined(ARCH_TEST_CIPER)
#error "ARCH_TEST_CIPER_MODE_CBC defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CTR_AES) &&\
    (!defined(ARCH_TEST_CIPER) || !defined(ARCH_TEST_AES) || !defined(ARCH_TEST_CIPER_MODE_CTR))
#error "ARCH_TEST_CTR_AES defined, but not all prerequisites"
#endif

#if (defined(ARCH_TEST_CBC_AES)|| defined(ARCH_TEST_CBC_AES_NO_PADDING)) &&\
    (!defined(ARCH_TEST_CIPER) || !defined(ARCH_TEST_AES) || !defined(ARCH_TEST_CIPER_MODE_CBC))
#error "ARCH_TEST_CBC_AES defined, but not all prerequisites"
#endif

#if (defined(ARCH_TEST_CBC_NO_PADDING)) &&\
    (!defined(ARCH_TEST_CIPER) ||!defined(ARCH_TEST_CIPER_MODE_CBC))
#error "ARCH_TEST_CBC_NO_PADDING defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CFB_AES) &&\
    (!defined(ARCH_TEST_CIPER) || !defined(ARCH_TEST_AES) || !defined(ARCH_TEST_CIPER_MODE_CFB))
#error "ARCH_TEST_CFB_AES defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_RSA_PKCS1V15_SIGN) &&\
    (!defined(ARCH_TEST_RSA) || !defined(ARCH_TEST_PKCS1V15))
#error "ARCH_TEST_RSA_PKCS1V15_SIGN defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_RSA_PKCS1V15_SIGN_RAW) &&\
    (!defined(ARCH_TEST_RSA) || !defined(ARCH_TEST_PKCS1V15))
#error "ARCH_TEST_RSA_PKCS1V15_SIGN_RAW defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_RSA_PKCS1V15_CRYPT) &&\
    (!defined(ARCH_TEST_RSA) || !defined(ARCH_TEST_PKCS1V15))
#error "ARCH_TEST_RSA_PKCS1V15_CRYPT defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CBC_PKCS7) && !defined(ARCH_TEST_CIPER_MODE_CBC)
#error "ARCH_TEST_CBC_PKCS7 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_HMAC) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_HMAC defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_MD2) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_MD2 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_MD4) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_MD4 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_MD5) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_MD5 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_RIPEMD160) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_RIPEMD160 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA1) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA1 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA224) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA224 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA256) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA256 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA512) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA512 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA512_224) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA512_224 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA512_256) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA512_256 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA3_224) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA3_224 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA3_256) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA3_256 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA3_384) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA3_256 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_SHA3_512) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_SHA3_256 defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_HKDF) && !defined(ARCH_TEST_HASH)
#error "ARCH_TEST_HKDF defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CMAC) && !defined(ARCH_TEST_AES) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_CMAC defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_GMAC) && !defined(ARCH_TEST_AES) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_GMAC defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_HMAC) && !defined(ARCH_TEST_AES) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_HMAC defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_CCM) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_CCM defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_GCM) && !defined(ARCH_TEST_AES)
#error "ARCH_TEST_GCM defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_ECDH) && !defined(ARCH_TEST_ECC)
#error "ARCH_TEST_ECDH defined, but not all prerequisites"
#endif

#if defined(ARCH_TEST_ECDSA) && !defined(ARCH_TEST_ECC)
#error "ARCH_TEST_ECDSA defined, but not all prerequisites"
#endif

#endif /* _PAL_CRYPTO_CONFIG_CHECK_H_ */
