/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mbedtls/aes.h"
#include "fsl_common.h"

/******************************************************************************/
/**************************** DCP *********************************************/
/******************************************************************************/
#if defined(FSL_FEATURE_SOC_DCP_COUNT) && (FSL_FEATURE_SOC_DCP_COUNT > 0)
static dcp_handle_t s_dcpHandle = {.channel = kDCP_Channel0, .keySlot = kDCP_KeySlot0, .swapConfig = kDCP_NoSwap};
#endif

/******************************************************************************/
/************************* Key slot management ********************************/
/******************************************************************************/
#if (defined(FSL_FEATURE_SOC_CAU3_COUNT) && (FSL_FEATURE_SOC_CAU3_COUNT > 0)) || (defined(MBEDTLS_FREESCALE_DCP_AES))
static const void *s_mbedtlsCtx[4] = {0};

static void crypto_attach_ctx_to_key_slot(const void *ctx, uint8_t keySlot)
{
    s_mbedtlsCtx[keySlot] = ctx;
}

static void crypto_detach_ctx_from_key_slot(const void *ctx)
{
    for (int i = 0; i < 4; i++)
    {
        if (ctx == s_mbedtlsCtx[i])
        {
            s_mbedtlsCtx[i] = NULL;
            break;
        }
    }
}

static bool crypto_key_is_loaded(const void *ctx)
{
    bool ret = false;
    for (int i = 0; i < 4; i++)
    {
        if (ctx == s_mbedtlsCtx[i])
        {
            ret = true;
            break;
        }
    }
    return ret;
}
#endif

#if defined(MBEDTLS_AES_ALT)

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v; while( n-- ) *p++ = 0;
}

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
#if defined(FSL_FEATURE_SOC_LTC_COUNT) && (FSL_FEATURE_SOC_LTC_COUNT > 0)
    LTC_Init(LTC_INSTANCE);
#endif
#if defined(FSL_FEATURE_SOC_DCP_COUNT) && (FSL_FEATURE_SOC_DCP_COUNT > 0)
    /* Initialize DCP */
    dcp_config_t dcpConfig;

    DCP_GetDefaultConfig(&dcpConfig);
    DCP_Init(DCP, &dcpConfig);
#endif
}

void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_aes_context ) );
#if defined(FSL_FEATURE_SOC_LTC_COUNT) && (FSL_FEATURE_SOC_LTC_COUNT > 0)
    LTC_Deinit(LTC_INSTANCE);
#endif
#if defined(FSL_FEATURE_SOC_DCP_COUNT) && (FSL_FEATURE_SOC_DCP_COUNT > 0)
    DCP_Deinit(DCP);
    crypto_detach_ctx_from_key_slot(ctx);
#endif
}

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    uint32_t *RK;

#ifdef MBEDTLS_AES_ALT_NO_192
    if (keybits == 192u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

#ifdef MBEDTLS_AES_ALT_NO_256
    if (keybits == 256u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

#if defined(MBEDTLS_FREESCALE_LTC_AES) || defined(MBEDTLS_FREESCALE_LPC_AES) || defined(MBEDTLS_FREESCALE_CAU3_AES) || \
    defined(MBEDTLS_FREESCALE_CAAM_AES) || defined(MBEDTLS_FREESCALE_DCP_AES)
    const unsigned char *key_tmp = key;
    ctx->rk = RK = ctx->buf;
    memcpy(RK, key_tmp, keybits / 8);

#if defined(MBEDTLS_FREESCALE_CAU3_AES) || defined(MBEDTLS_FREESCALE_DCP_AES)
    crypto_detach_ctx_from_key_slot(ctx);
#endif /* MBEDTLS_FREESCALE_CAU3_AES || MBEDTLS_FREESCALE_DCP_AES */

    switch (keybits)
    { /* Set keysize in bytes.*/
        case 128:
            ctx->nr = 16;
            break;
        case 192:
            ctx->nr = 24;
            break;
        case 256:
            ctx->nr = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }
#endif

    return (0);
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    uint32_t *RK;

#ifdef MBEDTLS_AES_ALT_NO_192
    if (keybits == 192u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

#ifdef MBEDTLS_AES_ALT_NO_256
    if (keybits == 256u)
    {
        return (MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE);
    }
#endif

    ctx->rk = RK = ctx->buf;

#if defined(MBEDTLS_FREESCALE_LTC_AES) || defined(MBEDTLS_FREESCALE_LPC_AES) || defined(MBEDTLS_FREESCALE_CAU3_AES) || \
    defined(MBEDTLS_FREESCALE_CAAM_AES) || defined(MBEDTLS_FREESCALE_DCP_AES)
    const unsigned char *key_tmp = key;
    memcpy(RK, key_tmp, keybits / 8);

#if defined(MBEDTLS_FREESCALE_CAU3_AES) || defined(MBEDTLS_FREESCALE_DCP_AES)
    crypto_detach_ctx_from_key_slot(ctx);
#endif /* MBEDTLS_FREESCALE_CAU3_AES || MBEDTLS_FREESCALE_DCP_AES */

    switch (keybits)
    {
        case 128:
            ctx->nr = 16;
            break;
        case 192:
            ctx->nr = 24;
            break;
        case 256:
            ctx->nr = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }
#endif

    return 0;
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] )
{
#if defined(MBEDTLS_AESNI_C) && defined(MBEDTLS_HAVE_X86_64)
    if (mbedtls_aesni_has_support(MBEDTLS_AESNI_AES))
        return (mbedtls_aesni_crypt_ecb (ctx, mode, input, output));
#endif

#if defined(MBEDTLS_PADLOCK_C) && defined(MBEDTLS_HAVE_X86)
    if (aes_padlock_ace) {
        if (mbedtls_padlock_xcryptecb (ctx, mode, input, output) == 0)
            return (0);

        // If padlock data misaligned, we just fall back to
        // unaccelerated mode
        //
    }
#endif

    if (mode == MBEDTLS_AES_ENCRYPT) {
        return (mbedtls_internal_aes_encrypt(ctx, input, output));
    } else {
        return (mbedtls_internal_aes_decrypt(ctx, input, output));
    }

    return( 0 );
}

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    uint8_t *key;

    key = (uint8_t *)ctx->rk;
#if defined(MBEDTLS_FREESCALE_LTC_AES)
    LTC_AES_EncryptEcb(LTC_INSTANCE, input, output, 16, key, ctx->nr);
#elif defined(MBEDTLS_FREESCALE_DCP_AES)
    if (!crypto_key_is_loaded(ctx))
    {
        DCP_AES_SetKey(DCP, &s_dcpHandle, key, ctx->nr);
        crypto_attach_ctx_to_key_slot(ctx, s_dcpHandle.keySlot);
    }
    DCP_AES_EncryptEcb(DCP, &s_dcpHandle, input, output, 16);
#endif

    return (0);
}

/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    uint8_t *key;

    key = (uint8_t *)ctx->rk;
#if defined(MBEDTLS_FREESCALE_LTC_AES)
    LTC_AES_DecryptEcb(LTC_INSTANCE, input, output, 16, key, ctx->nr, kLTC_EncryptKey);
#elif defined(MBEDTLS_FREESCALE_DCP_AES)
    if (!crypto_key_is_loaded(ctx))
    {
        DCP_AES_SetKey(DCP, &s_dcpHandle, key, ctx->nr);
        crypto_attach_ctx_to_key_slot(ctx, s_dcpHandle.keySlot);
    }
    DCP_AES_DecryptEcb(DCP, &s_dcpHandle, input, output, 16);
#endif

    return (0);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
#if defined(MBEDTLS_FREESCALE_LTC_AES)
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    uint8_t *key = (uint8_t *)ctx->rk;
    uint32_t keySize = ctx->nr;

    if (length % 16)
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);

    if (mode == MBEDTLS_AES_DECRYPT)
    {
        uint8_t tmp[16];
        memcpy(tmp, input + length - 16, 16);
        LTC_AES_DecryptCbc(LTC_INSTANCE, input, output, length, iv, key, keySize, kLTC_EncryptKey);
        memcpy(iv, tmp, 16);
    }
    else
    {
        LTC_AES_EncryptCbc(LTC_INSTANCE, input, output, length, iv, key, keySize);
        memcpy(iv, output + length - 16, 16);
    }

    return (0);
}
#elif defined(MBEDTLS_FREESCALE_DCP_AES)
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    uint8_t *key;

    if (length % 16)
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);

    key = (uint8_t *)ctx->rk;
    if (!crypto_key_is_loaded(ctx))
    {
        DCP_AES_SetKey(DCP, &s_dcpHandle, key, ctx->nr);
        crypto_attach_ctx_to_key_slot(ctx, s_dcpHandle.keySlot);
    }

    if (mode == MBEDTLS_AES_DECRYPT)
    {
        uint8_t tmp[16];
        memcpy(tmp, input + length - 16, 16);
        DCP_AES_DecryptCbc(DCP, &s_dcpHandle, input, output, length, iv);
        memcpy(iv, tmp, 16);
    }
    else
    {
        DCP_AES_EncryptCbc(DCP, &s_dcpHandle, input, output, length, iv);
        memcpy(iv, output + length - 16, 16);
    }

    return (0);
}
#endif
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
#if defined(MBEDTLS_FREESCALE_LTC_AES)
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    uint8_t *key;
    uint32_t keySize;

    key = (uint8_t *)ctx->rk;
    keySize = ctx->nr;
    LTC_AES_CryptCtr(LTC_INSTANCE, input, output, length, nonce_counter, key, keySize, stream_block,
                     (uint32_t *)nc_off);

    return (0);
}
#endif
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /*MBEDTLS_AES_ALT*/
