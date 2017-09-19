/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#include "mbedtls/aes.h"

#if defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_AES_ALT)

#include <string.h>
#include <stdbool.h>

#include "M480.h"
#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"



/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char*)v;
    while( n-- ) *p++ = 0;
}

extern volatile int  g_AES_done;

// Must be a multiple of 16 bytes block size
#define MAX_DMA_CHAIN_SIZE (16*6)

static void swapInitVector(unsigned char iv[16])
{
    unsigned int* piv;
    int i;
    // iv SWAP
    piv = (unsigned int*)iv;
    for( i=0; i< 4; i++) {
        *piv = (((*piv) & 0x000000FF) << 24) |
               (((*piv) & 0x0000FF00) << 8) |
               (((*piv) & 0x00FF0000) >> 8) |
               (((*piv) & 0xFF000000) >> 24);
        piv++;
    }
}

/* Check if buffer can be used for AES DMA. It requires to be:
 *   1) Word-aligned
 *   2) Located in 0x2xxxxxxx region
 */
static bool aes_dma_buff_compat(const void *buff)
{
    uint32_t buff_ = (uint32_t) buff;
    
    return (((buff_ & 0x03) == 0) && ((buff_ & 0x20000000) == 0x20000000));
}

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );

    ctx->swapType = AES_IN_OUT_SWAP;

    /* Unlock protected registers */
    SYS_UnlockReg();
    CLK_EnableModuleClock(CRPT_MODULE);
    /* Lock protected registers */
    SYS_LockReg();

    NVIC_EnableIRQ(CRPT_IRQn);
    AES_ENABLE_INT();
}

void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_aes_context ) );
}

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    unsigned int i;

    switch( keybits ) {
    case 128:
        ctx->keySize = AES_KEY_SIZE_128;
        break;
    case 192:
        ctx->keySize = AES_KEY_SIZE_192;
        break;
    case 256:
        ctx->keySize = AES_KEY_SIZE_256;
        break;
    default :
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    // key swap
    for( i = 0; i < ( keybits >> 5 ); i++ ) {
        ctx->buf[i] = (*(key+i*4) << 24) |
                      (*(key+1+i*4) << 16) |
                      (*(key+2+i*4) << 8) |
                      (*(key+3+i*4) );
    }

    return( 0 );
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    int ret;

    /* Also checks keybits */
    if( ( ret = mbedtls_aes_setkey_enc( ctx, key, keybits ) ) != 0 )
        goto exit;

exit:
    return( ret );
}


static void __nvt_aes_crypt( mbedtls_aes_context *ctx,
                             const unsigned char input[16],
                             unsigned char output[16], int dataSize)
{
    const unsigned char* pIn;
    unsigned char* pOut;

    /* AES DMA buffer requires to be:
     *   1) Word-aligned
     *   2) Located in 0x2xxxxxxx region
     */
    MBED_ALIGN(4) uint8_t au8OutputData[MAX_DMA_CHAIN_SIZE];
    MBED_ALIGN(4) uint8_t au8InputData[MAX_DMA_CHAIN_SIZE];
    if ((! aes_dma_buff_compat(au8OutputData)) || (! aes_dma_buff_compat(au8InputData))) {
        error("Buffer for AES alter. DMA requires to be word-aligned and located in 0x2xxxxxxx region.");
    }

    /* We support multiple contexts with context save & restore and so needs just one 
     * H/W channel. Always use H/W channel #0. */

    AES_Open(0, ctx->encDec, ctx->opMode, ctx->keySize, ctx->swapType);
    AES_SetInitVect(0, ctx->iv);
    AES_SetKey(0, ctx->buf, ctx->keySize);
    /* AES DMA buffer requirements same as above */
    if (! aes_dma_buff_compat(input)) {
        if (dataSize > MAX_DMA_CHAIN_SIZE) {
            error("Internal AES alter. error. DMA buffer is too small.");
        }
        memcpy(au8InputData, input, dataSize);
        pIn = au8InputData;
    } else {
        pIn = input;
    }
    /* AES DMA buffer requirements same as above */
    if (! aes_dma_buff_compat(output)) {
        pOut = au8OutputData;
    } else {
        pOut = output;
    }

    AES_SetDMATransfer(0, (uint32_t)pIn, (uint32_t)pOut, dataSize);

    g_AES_done = 0;
    AES_Start(0, CRYPTO_DMA_ONE_SHOT);
    while (!g_AES_done);

    if( pOut != output ) {
        if (dataSize > MAX_DMA_CHAIN_SIZE) {
            error("Internal AES alter. error. DMA buffer is too small.");
        }
        memcpy(output, au8OutputData, dataSize);
    }
    
    /* Save IV for next block */
    ctx->iv[0] = CRPT->AES_FDBCK[0];
    ctx->iv[1] = CRPT->AES_FDBCK[1];
    ctx->iv[2] = CRPT->AES_FDBCK[2];
    ctx->iv[3] = CRPT->AES_FDBCK[3];
}

/*
 * AES-ECB block encryption
 */
void mbedtls_aes_encrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{
    ctx->encDec = 1;
    __nvt_aes_crypt(ctx, input, output, 16);
}

/*
 * AES-ECB block decryption
 */
void mbedtls_aes_decrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{
    ctx->encDec = 0;
    __nvt_aes_crypt(ctx, input, output, 16);
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    ctx->opMode = AES_MODE_ECB;
    if( mode == MBEDTLS_AES_ENCRYPT )
        mbedtls_aes_encrypt( ctx, input, output );
    else
        mbedtls_aes_decrypt( ctx, input, output );


    return( 0 );
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                           int mode,
                           size_t len,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    unsigned char temp[16];
    int length = len;
    int blockChainLen;

    if( length % 16 )
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );


    while( length > 0 ) {
        blockChainLen = (length > MAX_DMA_CHAIN_SIZE) ? MAX_DMA_CHAIN_SIZE : length;
        
        ctx->opMode = AES_MODE_CBC;
        swapInitVector(iv); // iv SWAP
        memcpy(ctx->iv, iv, 16);

        if( mode == MBEDTLS_AES_ENCRYPT ) {
            ctx->encDec = 1;
            __nvt_aes_crypt(ctx, input, output, blockChainLen);
            memcpy( iv, output+blockChainLen-16, 16 );
        } else {
            memcpy( temp, input+blockChainLen-16, 16 );
            ctx->encDec = 0;
            __nvt_aes_crypt(ctx, input, output, blockChainLen);
            memcpy( iv, temp, 16 );
        }
        length -= blockChainLen;
        input  += blockChainLen;
        output += blockChainLen;
    }

    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
/* Support partial block encryption/decryption */
static int __nvt_aes_crypt_partial_block_cfb128( mbedtls_aes_context *ctx,
        int mode,
        size_t length,
        size_t *iv_off,
        unsigned char iv[16],
        const unsigned char *input,
        unsigned char *output )
{
    int c;
    size_t n = *iv_off;
    unsigned char iv_tmp[16];

    if( mode == MBEDTLS_AES_DECRYPT ) {
        while( length-- ) {
            if( n == 0)
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
            else if( ctx->opMode == AES_MODE_CFB) { // For previous cryption is CFB mode
                memcpy(iv_tmp, iv, n);
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, ctx->prv_iv, iv );
                memcpy(iv, iv_tmp, n);
            }

            c = *input++;
            *output++ = (unsigned char)( c ^ iv[n] );
            iv[n] = (unsigned char) c;

            n = ( n + 1 ) & 0x0F;
        }
    } else {
        while( length-- ) {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
            else if( ctx->opMode == AES_MODE_CFB) { // For previous cryption is CFB mode
                memcpy(iv_tmp, iv, n);
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, ctx->prv_iv, iv );
                memcpy(iv, iv_tmp, n);
            }

            iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );

            n = ( n + 1 ) & 0x0F;
        }
    }

    *iv_off = n;

    return( 0 );
}

int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                              int mode,
                              size_t len,
                              size_t *iv_off,
                              unsigned char iv[16],
                              const unsigned char *input,
                              unsigned char *output )
{
    size_t n = *iv_off;
    unsigned char temp[16];
    int length=len;
    int blockChainLen;
    int remLen=0;
    int ivLen;


    // proceed: start with partial block by ECB mode first
    if( n !=0 ) {
        __nvt_aes_crypt_partial_block_cfb128(ctx, mode, 16 - n, iv_off, iv, input, output);
        input += (16 - n);
        output += (16 - n);
        length -= (16 - n);
    }

    // For address or byte count non-word alignment, go through reserved DMA buffer.
    if( (((uint32_t)input) & 0x03) || (((uint32_t)output) & 0x03) ) { // Must reserved DMA buffer for each block
        blockChainLen = (( length > MAX_DMA_CHAIN_SIZE ) ?  MAX_DMA_CHAIN_SIZE : length );
    } else if(length%4) {                                                                                       // Need reserved DMA buffer once for last chain
        blockChainLen = (( length > MAX_DMA_CHAIN_SIZE ) ?  (length - length%16) : length );
    } else {                                                                                                                // Not need reserved DMA buffer
        blockChainLen = length;
    }

    // proceed: start with block alignment
    while( length > 0 ) {

        ctx->opMode = AES_MODE_CFB;

        swapInitVector(iv); // iv SWAP

        memcpy(ctx->iv, iv, 16);
        remLen = blockChainLen%16;
        ivLen = (( remLen > 0) ? remLen: 16 );

        if( mode == MBEDTLS_AES_DECRYPT ) {
            memcpy(temp, input+blockChainLen - ivLen, ivLen);
            if(blockChainLen >= 16) memcpy(ctx->prv_iv, input+blockChainLen-remLen-16, 16);
            ctx->encDec = 0;
            __nvt_aes_crypt(ctx, input, output, blockChainLen);
            memcpy(iv,temp, ivLen);
        } else {
            ctx->encDec = 1;
            __nvt_aes_crypt(ctx, input, output, blockChainLen);
            if(blockChainLen >= 16) memcpy(ctx->prv_iv, output+blockChainLen-remLen-16, 16);
            memcpy(iv,output+blockChainLen-ivLen,ivLen);
        }
        length -= blockChainLen;
        input  += blockChainLen;
        output += blockChainLen;
        if(length < MAX_DMA_CHAIN_SIZE ) blockChainLen = length;        // For last remainder block chain
    }

    *iv_off = remLen;

    return( 0 );
}


/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
    unsigned char c;
    unsigned char ov[17];

    while( length-- ) {
        memcpy( ov, iv, 16 );
        mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

    return( 0 );
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *nc_off,
                           unsigned char nonce_counter[16],
                           unsigned char stream_block[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int c, i;
    size_t n = *nc_off;

    while( length-- ) {
        if( n == 0 ) {
            mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block );

            for( i = 16; i > 0; i-- )
                if( ++nonce_counter[i - 1] != 0 )
                    break;
        }
        c = *input++;
        *output++ = (unsigned char)( c ^ stream_block[n] );

        n = ( n + 1 ) & 0x0F;
    }

    *nc_off = n;

    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_AES_ALT */


#endif /* MBEDTLS_AES_C */
