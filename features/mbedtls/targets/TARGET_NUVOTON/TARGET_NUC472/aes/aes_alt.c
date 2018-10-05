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

#include "NUC472_442.h"
#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = (unsigned char*)v;
    while( n-- ) *p++ = 0;
}

/* AES DMA compatible backup buffer if user buffer doesn't meet requirements
 * 
 * AES DMA buffer location requires to be:
 * (1) Word-aligned
 * (2) Located in 0x2xxxxxxx region. Check linker files to ensure global variables are placed in this region.
 *
 * AES DMA buffer size MAX_DMA_CHAIN_SIZE must be a multiple of 16-byte block size.
 * Its value is estimated to trade memory footprint off against performance.
 *
 */
#define MAX_DMA_CHAIN_SIZE (16*6)
MBED_ALIGN(4) static uint8_t au8OutputData[MAX_DMA_CHAIN_SIZE];
MBED_ALIGN(4) static uint8_t au8InputData[MAX_DMA_CHAIN_SIZE];

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
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

    /* Fetch key byte data in big-endian */
    for( i = 0; i < ( keybits >> 5 ); i++ ) {
        ctx->keys[i] = nu_get32_be(key + i * 4);
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

/* Do AES encrypt/decrypt with H/W accelerator
 *
 * NOTE: As input/output buffer doesn't follow constraint of DMA buffer, static allocated 
 *       DMA compatible buffer is used for DMA instead and this needs extra copy.
 *
 * NOTE: dataSize requires to be: 
 *       1) Multiple of block size 16 
 *       2) <= MAX_DMA_CHAIN_SIZE
 */
static void __nvt_aes_crypt( mbedtls_aes_context *ctx,
                             const unsigned char *input,
                             unsigned char *output, size_t dataSize)
{
    const unsigned char* pIn;
    unsigned char* pOut;

    MBED_ASSERT((dataSize % 16 == 0) && (dataSize <= MAX_DMA_CHAIN_SIZE));
    
    /* AES DMA buffer has the following requirements:
     * (1) Word-aligned buffer base address
     * (2) 16-byte aligned buffer size
     * (3) Located in 0x20000000-0x2FFFFFFF region
     */
    if ((! crypto_dma_buff_compat(au8OutputData, MAX_DMA_CHAIN_SIZE, 16)) ||
        (! crypto_dma_buff_compat(au8InputData, MAX_DMA_CHAIN_SIZE, 16))) {
        error("Buffer for AES alter. DMA requires to be word-aligned and located in 0x20000000-0x2FFFFFFF region.");
    }

    /* TODO: Change busy-wait to other means to release CPU */
    /* Acquire ownership of AES H/W */
    while (! crypto_aes_acquire());
    
    /* Init crypto module */
    crypto_init();
    /* Enable AES interrupt */
    AES_ENABLE_INT();
    
    /* We support multiple contexts with context save & restore and so needs just one 
     * H/W channel. Always use H/W channel #0. */

    /* AES_IN_OUT_SWAP: Let H/W know both input/output data are arranged in little-endian */
    AES_Open(0, ctx->encDec, ctx->opMode, ctx->keySize, AES_IN_OUT_SWAP);
    AES_SetInitVect(0, ctx->iv);
    AES_SetKey(0, ctx->keys, ctx->keySize);
    
    /* AES DMA buffer requirements same as above */
    if (! crypto_dma_buff_compat(input, dataSize, 16)) {
        memcpy(au8InputData, input, dataSize);
        pIn = au8InputData;
    } else {
        pIn = input;
    }
    /* AES DMA buffer requirements same as above */
    if (! crypto_dma_buff_compat(output, dataSize, 16)) {
        pOut = au8OutputData;
    } else {
        pOut = output;
    }

    /* Even though AES H/W has limited support for overlapped DMA input/output buffers,
     * we still seek to one backup buffer to make them non-overlapped for simplicity. */
    if (crypto_dma_buffs_overlap(pIn, dataSize, pOut, dataSize)) {
        memcpy(au8InputData, input, dataSize);
        pIn = au8InputData;
    }
    MBED_ASSERT(! crypto_dma_buffs_overlap(pIn, dataSize, pOut, dataSize));
    
    AES_SetDMATransfer(0, (uint32_t)pIn, (uint32_t)pOut, dataSize);

    crypto_aes_prestart();
    AES_Start(0, CRYPTO_DMA_ONE_SHOT);
    crypto_aes_wait();
    
    if( pOut != output ) {
        memcpy(output, au8OutputData, dataSize);
    }
    
    /* Save IV for next block */
    ctx->iv[0] = CRPT->AES_FDBCK0;
    ctx->iv[1] = CRPT->AES_FDBCK1;
    ctx->iv[2] = CRPT->AES_FDBCK2;
    ctx->iv[3] = CRPT->AES_FDBCK3;
    
    /* Disable AES interrupt */
    AES_DISABLE_INT();
    /* Uninit crypto module */
    crypto_uninit();
    
    /* Release ownership of AES H/W */
    crypto_aes_release();
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
    int length = len;
    int blockChainLen;

    if( length % 16 )
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );


    ctx->opMode = AES_MODE_CBC;
    /* Fetch IV byte data in big-endian */
    ctx->iv[0] = nu_get32_be(iv);
    ctx->iv[1] = nu_get32_be(iv + 4);
    ctx->iv[2] = nu_get32_be(iv + 8);
    ctx->iv[3] = nu_get32_be(iv + 12);
        
    if( mode == MBEDTLS_AES_ENCRYPT ) {
        ctx->encDec = 1;
    }
    else {
        ctx->encDec = 0;
    }
    
    while( length > 0 ) {
        blockChainLen = (length > MAX_DMA_CHAIN_SIZE) ? MAX_DMA_CHAIN_SIZE : length;

        __nvt_aes_crypt(ctx, input, output, blockChainLen);
        
        length -= blockChainLen;
        input  += blockChainLen;
        output += blockChainLen;
    }

    /* Save IV for next block cipher */
    nu_set32_be(iv, ctx->iv[0]);
    nu_set32_be(iv + 4, ctx->iv[1]);
    nu_set32_be(iv + 8, ctx->iv[2]);
    nu_set32_be(iv + 12, ctx->iv[3]);
    
    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                              int mode,
                              size_t length,
                              size_t *iv_off,
                              unsigned char iv[16],
                              const unsigned char *input,
                              unsigned char *output )
{
    int c;
    size_t n = *iv_off;

    /* First incomplete block */
    if (n % 16) {
        while (n && length) {
            if (mode == MBEDTLS_AES_DECRYPT) {
                c = *input++;
                *output++ = (unsigned char)( c ^ iv[n] );
                iv[n] = (unsigned char) c;
            }
            else {
                iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );
            }

            n = ( n + 1 ) & 0x0F;
            length --;
        }
    }

    /* Middle complete block(s) */
    size_t block_chain_len = length - (length % 16);
        
    if (block_chain_len) {
        ctx->opMode = AES_MODE_CFB;
        if (mode == MBEDTLS_AES_DECRYPT) {
            ctx->encDec = 0;
        }
        else {
            ctx->encDec = 1;
        }
                
        /* Fetch IV byte data in big-endian */
        ctx->iv[0] = nu_get32_be(iv);
        ctx->iv[1] = nu_get32_be(iv + 4);
        ctx->iv[2] = nu_get32_be(iv + 8);
        ctx->iv[3] = nu_get32_be(iv + 12);
            
        while (block_chain_len) {
            size_t block_chain_len2 = (block_chain_len > MAX_DMA_CHAIN_SIZE) ? MAX_DMA_CHAIN_SIZE : block_chain_len;

            __nvt_aes_crypt(ctx, input, output, block_chain_len2);
                    
            input += block_chain_len2;
            output += block_chain_len2;
            length -= block_chain_len2;

            block_chain_len -= block_chain_len2;
        }
        
        /* NOTE: Buffers input/output could overlap. See ctx->iv rather than input/output
         *       for iv of next block cipher. */
        /* Fetch IV byte data in big-endian */
        nu_set32_be(iv, ctx->iv[0]);
        nu_set32_be(iv + 4, ctx->iv[1]);
        nu_set32_be(iv + 8, ctx->iv[2]);
        nu_set32_be(iv + 12, ctx->iv[3]);
    }
        
    /* Last incomplete block */
    if (length) {
        mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
            
        while (length --) {
            if (mode == MBEDTLS_AES_DECRYPT) {
                c = *input++;
                *output++ = (unsigned char)( c ^ iv[n] );
                iv[n] = (unsigned char) c;
            }
            else {
                iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );
            }
            
            n = ( n + 1 ) & 0x0F;
        }
    }

    *iv_off = n;

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
