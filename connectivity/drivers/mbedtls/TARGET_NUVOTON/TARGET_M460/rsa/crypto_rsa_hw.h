/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CRYPTO_RSA_HW_H
#define CRYPTO_RSA_HW_H

#include "mbedtls/rsa.h"

#if defined(MBEDTLS_RSA_ALT)

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Initialize/Free Crypto RSA H/W
 *
 * \return          \c 0 on success.
 * \return          A non-zero error code on failure.
 *
 * \note            crypto_rsa_init()/crypto_rsa_free() are like pre-op/post-op calls
 *                  and they guarantee:
 *
 *                  1. Paired
 *                  2. No overlapping
 *                  3. Upper public function cannot return when RSA alter. is still activated.
 */
int crypto_rsa_init(mbedtls_rsa_context *ctx);
void crypto_rsa_free(mbedtls_rsa_context *ctx);

/**
 * \brief           Query whether or not RSA H/W supports encrypt/decrypt operation in this context
 *                  in normal/CRT algorithm
 *
 * \param ctx       The initialized RSA context to use.
 * \param decrypt   true for decrypt, or encrypt.
 * \param crt       true for CRT algorithm, or normal.
 * \param blinding  Blinding (SCAP) or not.
 *
 * \return          \c 1 on capable, or 0 on incapable.
 *
 * \note            Blinding is applicable only for decrypt operation.
 * \note            CRT is applicable only for decrypt operation.
 */
int crypto_rsa_encrypt_norm_capable(const mbedtls_rsa_context *ctx);
int crypto_rsa_decrypt_norm_capable(const mbedtls_rsa_context *ctx, bool blinding);
int crypto_rsa_decrypt_crt_capable(const mbedtls_rsa_context *ctx, bool blinding);
int crypto_rsa_crypt_capable(const mbedtls_rsa_context *ctx,
                             bool decrypt,
                             bool crt,
                             bool blinding);
/**
 * \brief           Run RSA encrypt/decrypt operation in normal/CRT algorithm
 *
 * \param ctx       The initialized RSA context to use.
 * \param decrypt   true for decrypt, or encrypt.
 * \param crt       true for CRT algorithm, or normal.
 * \param blinding  Blinding (SCAP) or not.
 * \param input     The input buffer. This must be a readable buffer
 *                  of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                  for an 2048-bit RSA modulus.
 * \param output    The output buffer. This must be a writable buffer
 *                  of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                  for an 2048-bit RSA modulus.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int crypto_rsa_encrypt_norm(mbedtls_rsa_context *ctx,
                            const unsigned char *input,
                            unsigned char *output);
int crypto_rsa_decrypt_norm(mbedtls_rsa_context *ctx,
                            bool blinding,
                            const unsigned char *input,
                            unsigned char *output);
int crypto_rsa_decrypt_crt(mbedtls_rsa_context *ctx,
                           bool blinding,
                           const unsigned char *input,
                           unsigned char *output);
int crypto_rsa_crypt(mbedtls_rsa_context *ctx,
                     bool decrypt,
                     bool crt,
                     bool blinding,
                     const unsigned char *input,
                     unsigned char *output);

/**
 * \brief           Abort Crypto RSA H/W
 *
 * \param ctx       The initialized RSA context to use.
 * \param timeout_us    Timeout in microseconds.
 *
 * \return          \c 0 on success.
 * \return          A non-zero error code on failure.
 */
int crypto_rsa_abort(mbedtls_rsa_context *ctx,
                     uint32_t timeout_us);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_RSA_ALT */

#endif /* CRYPTO_RSA_HW_H */
