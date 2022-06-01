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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_RSA_C)

#include "mbedtls/rsa.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_RSA_ALT)

#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"
#include "nu_bitutil.h"
#include "nu_timer.h"
#include "crypto-misc.h"

#include "crypto_rsa_hw.h"

/* Enable RSA debug */
//#define NU_CRYPTO_RSA_ENABLE_DEBUG
//#define NU_CRYPTO_RSA_ENABLE_DEBUG_V

/* Notes for RSA H/W port
 *
 * 1. For non-CRT+SCAP (excluding CRT+SCAP) mode, DMA buffer for MADDR6 must be key length plus 128 bits.
 * 2. DMA buffer must be 4-word aligned, or RSA H/W will trap.
 * 3. CRT is not well-tested yet, esp with 4096 key bits.
 * 4. Following designer's resolution, disable SCAP.
 */

/* RSA context for DMA */
static struct {
    struct {
        uint32_t    mode;           // RSA_MODE_NORMAL/CRT/SCAP/CRT_SCAP
        uint32_t    keybits_code;   // RSA_KEY_SIZE_1024/2048/3072/4096
        void *      dmabuf;         // Pointer to active DMA buffer
        uint32_t    dmabuf_size;    // Active DMA buffer in bytes
    } config;

    union {
        RSA_BUF_NORMAL_T    norm;
        RSA_BUF_CRT_T       crt;
        RSA_BUF_SCAP_T      scap;
        RSA_BUF_CRT_SCAP_T  crt_scap;
    } dmabuf_u;

    struct {
        MBED_ALIGN(4) char  R[RSA_KBUF_HLEN];   // Output result
        MBED_ALIGN(4) char  M[RSA_KBUF_HLEN];   // Input message
        MBED_ALIGN(4) char  N[RSA_KBUF_HLEN];   // Modulus
        MBED_ALIGN(4) char  E[RSA_KBUF_HLEN];   // Exponent (public or private)
        MBED_ALIGN(4) char  P[RSA_KBUF_HLEN];   // First prime factor
        MBED_ALIGN(4) char  Q[RSA_KBUF_HLEN];   // Second prime factor
    } keyctx_hs;
} rsa_hw_ctx_inst;
                                     
int crypto_rsa_init(mbedtls_rsa_context *ctx)
{
    /* Acquire ownership of RSA H/W */
    crypto_rsa_acquire();

    /* Initialize crypto module */
    crypto_init();

    /* RSA H/W DMA buffer has the following requirements:
     * (1) Word-aligned buffer base address
     * (2) Word-aligned buffer size
     * (3) Located in 0x20000000-0x2FFFFFFF region */
    if ((! crypto_dma_buff_compat(&(rsa_hw_ctx_inst.dmabuf_u), sizeof(rsa_hw_ctx_inst.dmabuf_u), 4))) {
        error("Buffer for RSA H/W DMA requires to be word-aligned and located in 0x20000000-0x2FFFFFFF region.");
    }

    /* Release ownership of RSA accelerator */
    crypto_rsa_release();

    return 0;
}

void crypto_rsa_free(mbedtls_rsa_context *ctx)
{
    /* Acquire ownership of RSA H/W */
    crypto_rsa_acquire();

    /* Uninit crypto module */
    crypto_uninit();

    /* Release ownership of RSA accelerator */
    crypto_rsa_release();
}

int crypto_rsa_encrypt_norm_capable(const mbedtls_rsa_context *ctx)
{
    return crypto_rsa_crypt_capable(ctx, 
                                    false,  // Encrypt operation
                                    false,  // Normal algorithm
                                    false); // No blinding
}

int crypto_rsa_decrypt_norm_capable(const mbedtls_rsa_context *ctx, bool blinding)
{
    return crypto_rsa_crypt_capable(ctx, 
                                    true,   // Decrypt operation
                                    false,  // Normal algorithm
                                    blinding);
}

int crypto_rsa_decrypt_crt_capable(const mbedtls_rsa_context *ctx, bool blinding)
{
    return crypto_rsa_crypt_capable(ctx, 
                                    true,   // Decrypt operation
                                    true,   // CRT algorithm
                                    blinding);
}

int crypto_rsa_crypt_capable(const mbedtls_rsa_context *ctx,
                             bool decrypt,
                             bool crt,
                             bool blinding)
{
    /* CRT is applicable only for decrypt operation */
    if (!decrypt && crt) {
        return 0;
    }

    /* NOTE: Check above RSA H/W comment for disabling crt */
    if (crt) {
        return 0;
    }

    /* Blinding (SCAP) is applicable only for decrypt operation */
    if (!decrypt && blinding) {
        return 0;
    }

    /* Support key bits
     *
     * Normal algorithm:    1024/2048/3072/4096
     * CRT algorithm:       2048/3072/4096
     */
    uint32_t keybits = ctx->len * 8;
    if (keybits == 1024) {
        return crt ? 0 : 1;
    } else if (keybits == 2048 || keybits == 3072 || keybits == 4096) {
        return 1;
    } else {
        return 0;
    }
}

int crypto_rsa_encrypt_norm(mbedtls_rsa_context *ctx,
                            const unsigned char *input,
                            unsigned char *output)
{
    return crypto_rsa_crypt(ctx,
                            false,      // Encrypt operation
                            false,      // Normal algorithm
                            false,
                            input,
                            output);
}

int crypto_rsa_decrypt_norm(mbedtls_rsa_context *ctx,
                            bool blinding,
                            const unsigned char *input,
                            unsigned char *output)
{
    return crypto_rsa_crypt(ctx,
                            true,       // Decrypt operation
                            false,      // Normal algorithm
                            blinding,
                            input,
                            output);
}
                            
int crypto_rsa_decrypt_crt(mbedtls_rsa_context *ctx,
                           bool blinding,
                           const unsigned char *input,
                           unsigned char *output)
{
    return crypto_rsa_crypt(ctx,
                            true,       // Decrypt operation
                            true,       // CRT algorithm
                            blinding,
                            input,
                            output);
}

int crypto_rsa_crypt(mbedtls_rsa_context *ctx,
                     bool decrypt,
                     bool crt,
                     bool blinding,
                     const unsigned char *input,
                     unsigned char *output)
{
    /* Acquire ownership of RSA H/W */
    crypto_rsa_acquire();

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    bool rsa_done;
    size_t olen;
    uint32_t keybits;

    /* NOTE: Check above RSA H/W comment for disabling blinding */
    blinding = false;

#if defined(NU_CRYPTO_RSA_ENABLE_DEBUG)
    mbedtls_printf("[CRPT][RSA] decrypt=%d, crt=%d, blinding=%d\n", decrypt, crt, blinding);
#endif

    mbedtls_mpi M, R;

    mbedtls_mpi_init(&M);
    mbedtls_mpi_init(&R);

    /* Read input message */
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&M, input, ctx->len));
    
    /* Driver key context requests hex string form. */

    /* Populate M (input) into driver key context */
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_string(&M, 16, rsa_hw_ctx_inst.keyctx_hs.M, RSA_KBUF_HLEN, &olen));

    /* Populate N (modulus) into driver key context */
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_string(&ctx->N, 16, rsa_hw_ctx_inst.keyctx_hs.N, RSA_KBUF_HLEN, &olen));

    if (decrypt) {
        /* Populate D (private key) into driver key context (exponent) */
        MBEDTLS_MPI_CHK(mbedtls_mpi_write_string(&ctx->D, 16, rsa_hw_ctx_inst.keyctx_hs.E, RSA_KBUF_HLEN, &olen));

        /* Populate P/Q (first/second prime factor) into driver key context */
        MBEDTLS_MPI_CHK(mbedtls_mpi_write_string(&ctx->P, 16, rsa_hw_ctx_inst.keyctx_hs.P, RSA_KBUF_HLEN, &olen));
        MBEDTLS_MPI_CHK(mbedtls_mpi_write_string(&ctx->Q, 16, rsa_hw_ctx_inst.keyctx_hs.Q, RSA_KBUF_HLEN, &olen));
    } else {
        /* Populate E (public key) into driver key context (exponent) */
        MBEDTLS_MPI_CHK(mbedtls_mpi_write_string(&ctx->E, 16, rsa_hw_ctx_inst.keyctx_hs.E, RSA_KBUF_HLEN, &olen));
    }

#if defined(NU_CRYPTO_RSA_ENABLE_DEBUG_V)
    mbedtls_printf("[CRPT][RSA] M:\n%s\n", rsa_hw_ctx_inst.keyctx_hs.M);
    mbedtls_printf("[CRPT][RSA] N:\n%s\n", rsa_hw_ctx_inst.keyctx_hs.N);
    mbedtls_printf("[CRPT][RSA] E:\n%s\n", rsa_hw_ctx_inst.keyctx_hs.E);
    if (decrypt) {
        mbedtls_printf("[CRPT][RSA] P:\n%s\n", rsa_hw_ctx_inst.keyctx_hs.P);
        mbedtls_printf("[CRPT][RSA] Q:\n%s\n", rsa_hw_ctx_inst.keyctx_hs.Q);
    }
#endif

    /* Resolve CRT/SCAP mode and corresponding DMA buffer */
    if (crt) {
        if (blinding) {
            rsa_hw_ctx_inst.config.mode = RSA_MODE_CRT_SCAP;
            rsa_hw_ctx_inst.config.dmabuf = &rsa_hw_ctx_inst.dmabuf_u.crt_scap;
            rsa_hw_ctx_inst.config.dmabuf_size = sizeof(rsa_hw_ctx_inst.dmabuf_u.crt_scap);
        } else {
            rsa_hw_ctx_inst.config.mode = RSA_MODE_CRT;
            rsa_hw_ctx_inst.config.dmabuf = &rsa_hw_ctx_inst.dmabuf_u.crt;
            rsa_hw_ctx_inst.config.dmabuf_size = sizeof(rsa_hw_ctx_inst.dmabuf_u.crt);
        }
    } else {
        if (blinding) {
            rsa_hw_ctx_inst.config.mode = RSA_MODE_SCAP;
            rsa_hw_ctx_inst.config.dmabuf = &rsa_hw_ctx_inst.dmabuf_u.scap;
            rsa_hw_ctx_inst.config.dmabuf_size = sizeof(rsa_hw_ctx_inst.dmabuf_u.scap);
        } else {
            rsa_hw_ctx_inst.config.mode = RSA_MODE_NORMAL;
            rsa_hw_ctx_inst.config.dmabuf = &rsa_hw_ctx_inst.dmabuf_u.norm;
            rsa_hw_ctx_inst.config.dmabuf_size = sizeof(rsa_hw_ctx_inst.dmabuf_u.norm);
        }
    }

    /* Resolve key bits code */
    keybits = ctx->len * 8;
    switch (keybits) {
    case 1024:
        rsa_hw_ctx_inst.config.keybits_code = RSA_KEY_SIZE_1024;
        break;

    case 2048:
        rsa_hw_ctx_inst.config.keybits_code = RSA_KEY_SIZE_2048;
        break;

    case 3072:
        rsa_hw_ctx_inst.config.keybits_code = RSA_KEY_SIZE_3072;
        break;

    case 4096:
        rsa_hw_ctx_inst.config.keybits_code = RSA_KEY_SIZE_4096;
        break;

    default:
        ret = MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
        goto cleanup;
    }

    /* Enable RSA interrupt */
    RSA_ENABLE_INT(CRPT);

    /* For safe, recover from previous failure if ever */
    MBEDTLS_MPI_CHK(crypto_rsa_abort(ctx, 5*1000*1000));

    /* NOTE: Driver (RSA_SetKey()/RSA_SetDMATransfer()) requests zero-padded
     *       hex string to configure DMA buffer (via Hex2Reg), but
     *       mbedtls_mpi_write_string() doesn't support this. Pre-clean
     *       DMA buffer as workaround. */
    mbedtls_platform_zeroize(rsa_hw_ctx_inst.config.dmabuf, rsa_hw_ctx_inst.config.dmabuf_size);

    MBED_ASSERT(rsa_hw_ctx_inst.config.dmabuf != NULL);
    MBED_ASSERT(rsa_hw_ctx_inst.config.dmabuf_size != 0);
    MBEDTLS_MPI_CHK((RSA_Open(CRPT, rsa_hw_ctx_inst.config.mode,
                              rsa_hw_ctx_inst.config.keybits_code,
                              rsa_hw_ctx_inst.config.dmabuf,
                              rsa_hw_ctx_inst.config.dmabuf_size,
                              0) == 0) ? 0 : MBEDTLS_ERR_RSA_BAD_INPUT_DATA);

    /* Set exponent (public/private key) into DMA buffer */
    RSA_SetKey(CRPT, rsa_hw_ctx_inst.keyctx_hs.E);

    /* Set most other parameters into DMA buffer */
    RSA_SetDMATransfer(CRPT,
                       rsa_hw_ctx_inst.keyctx_hs.M,
                       rsa_hw_ctx_inst.keyctx_hs.N,
                       rsa_hw_ctx_inst.keyctx_hs.P,
                       rsa_hw_ctx_inst.keyctx_hs.Q);

    /* Trigger and wait */
    crypto_rsa_prestart();
#if defined(NU_CRYPTO_RSA_ENABLE_DEBUG)
    mbedtls_printf("[CRPT][RSA] Crypto RSA ...\n");
#endif
    RSA_Start(CRPT);
    rsa_done = crypto_rsa_wait2(1000*1000); // 1s timeout
#if defined(NU_CRYPTO_RSA_ENABLE_DEBUG)
    mbedtls_printf("[CRPT][RSA] Crypto RSA ... %s\n", rsa_done ? "Done" : "Error");
#endif

    /* For safe, recover from current failure */
    if (!rsa_done) {
        crypto_rsa_abort(ctx, 5*1000*1000);
    }

    /* Disable RSA interrupt */
    RSA_DISABLE_INT(CRPT);

    MBEDTLS_MPI_CHK(rsa_done ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);

    /* Write output result */
    RSA_Read(CRPT, rsa_hw_ctx_inst.keyctx_hs.R);
    MBEDTLS_MPI_CHK(mbedtls_mpi_read_string(&R, 16, rsa_hw_ctx_inst.keyctx_hs.R));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&R, output, ctx->len));

#if defined(NU_CRYPTO_RSA_ENABLE_DEBUG_V)
    mbedtls_printf("[CRPT][RSA] R:\n%s\n", rsa_hw_ctx_inst.keyctx_hs.R);
#endif

cleanup:

    mbedtls_mpi_free(&M);
    mbedtls_mpi_free(&R);

    /* Release ownership of RSA accelerator */
    crypto_rsa_release();

    return ret;
}

int crypto_rsa_abort(MBED_UNUSED mbedtls_rsa_context *ctx,
                     uint32_t timeout_us)
{
    /* Acquire ownership of RSA H/W */
    crypto_rsa_acquire();

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    CRPT->RSA_CTL = CRPT_RSA_CTL_STOP_Msk;
    struct nu_countdown_ctx_s cd_ctx;
    nu_countdown_init(&cd_ctx, timeout_us);
    while (CRPT->RSA_STS & CRPT_RSA_STS_BUSY_Msk) {
        if (nu_countdown_expired(&cd_ctx)) {
            break;
        }
    }
    nu_countdown_free(&cd_ctx);
    if (CRPT->RSA_STS & CRPT_RSA_STS_BUSY_Msk) {
#if defined(NU_CRYPTO_RSA_ENABLE_DEBUG)
        mbedtls_printf("[CRPT][RSA] Crypto RSA ... Busy\n");
#endif
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }

    ret = 0;

cleanup:

    /* Release ownership of RSA accelerator */
    crypto_rsa_release();

    return ret;
}

#endif /* MBEDTLS_RSA_ALT */
#endif /* MBEDTLS_RSA_C */
