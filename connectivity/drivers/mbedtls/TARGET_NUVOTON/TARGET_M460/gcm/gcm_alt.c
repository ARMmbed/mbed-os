/*
 *  NIST SP800-38D compliant GCM implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2022, Nuvoton Technology Corp., All Rights Reserved
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
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

/*
 * http://csrc.nist.gov/publications/nistpubs/800-38D/SP-800-38D.pdf
 *
 * See also:
 * [MGV] http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-revised-spec.pdf
 *
 * We use the algorithm described as Shoup's method with 4-bit tables in
 * [MGV] 4.1, pp. 12-13, to enhance speed without using too much memory.
 */


#include "common.h"


#if defined(MBEDTLS_GCM_C)

#include "mbedtls/gcm.h"
#include "mbedtls/platform_util.h"

#include <string.h>

#if defined(MBEDTLS_AESNI_C)
#include "config.h"
#endif

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
#include "mbedtls/aes.h"
#include "mbedtls/platform.h"
#if !defined(MBEDTLS_PLATFORM_C)
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */


#if defined(MBEDTLS_GCM_ALT)

#include "M460.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"

/* Parameter validation macros */
#define GCM_VALIDATE_RET( cond ) \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_GCM_BAD_INPUT )
#define GCM_VALIDATE( cond ) \
    MBEDTLS_INTERNAL_VALIDATE( cond )


#define START       CRPT_AES_CTL_START_Msk
#define DMAEN       CRPT_AES_CTL_DMAEN_Msk
#define DMALAST     CRPT_AES_CTL_DMALAST_Msk
#define DMACC       CRPT_AES_CTL_DMACSCAD_Msk
#define START       CRPT_AES_CTL_START_Msk
#define FBIN        CRPT_AES_CTL_FBIN_Msk
#define FBOUT       CRPT_AES_CTL_FBOUT_Msk

#define GCM_MODE    (AES_MODE_GCM << CRPT_AES_CTL_OPMODE_Pos)
#define GHASH_MODE  (AES_MODE_GHASH << CRPT_AES_CTL_OPMODE_Pos)
#define CTR_MODE    (AES_MODE_CTR << CRPT_AES_CTL_OPMODE_Pos)

#define Debug_GCM_Info(x) {} 
//#define Debug_GCM_Info(x) { printf x; }

/*
 * Initialize a context
 */
void mbedtls_gcm_init( mbedtls_gcm_context *ctx )
{
    GCM_VALIDATE( ctx != NULL );
    memset( ctx, 0, sizeof( mbedtls_gcm_context ) );
        
    /* Init crypto module including Reset Crypto */
    crypto_init();
    
}

void mbedtls_gcm_free( mbedtls_gcm_context *ctx )
{
    if( ctx == NULL )
        return;
    
    /* Uninit crypto module */
    crypto_uninit();    
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_gcm_context ) );
}

static int32_t ToBigEndian(uint8_t *pbuf, uint32_t u32Size)
{
    uint32_t i;
    uint8_t u8Tmp;
    uint32_t u32Tmp;

    /* pbuf must be word alignment */
    if((uint32_t)pbuf & 0x3)
    {
        /* The buffer must be 32-bit alignment. */
        return -1;
    }

    while(u32Size >= 4)
    {
        u8Tmp = *pbuf;
        *(pbuf) = *(pbuf + 3);
        *(pbuf + 3) = u8Tmp;

        u8Tmp = *(pbuf + 1);
        *(pbuf + 1) = *(pbuf + 2);
        *(pbuf + 2) = u8Tmp;

        u32Size -= 4;
        pbuf += 4;
    }

    if(u32Size > 0)
    {
        u32Tmp = 0;
        for(i = 0; i < u32Size; i++)
        {
            u32Tmp |= *(pbuf + i) << (24 - i * 8);
        }

        *((uint32_t *)pbuf) = u32Tmp;
    }

    return 0;
}



int mbedtls_gcm_setkey( mbedtls_gcm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{

    int32_t i, klen;
    uint32_t keySizeOpt;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( key != NULL );
    GCM_VALIDATE_RET( keybits == 128 || keybits == 192 || keybits == 256 );

    klen = keybits / 8;
    ctx->keySize = klen;

    memcpy(ctx->keys, key, klen);    
    ToBigEndian((uint8_t *)ctx->keys, klen);
    
    /* Prepare key size option */
    i = klen >> 3;
    keySizeOpt = (((i >> 2) << 1) | (i & 1)) << CRPT_AES_CTL_KEYSZ_Pos;
    
    /* Basic options for AES */
    ctx->basicOpt = CRPT_AES_CTL_INSWAP_Msk | 
                    CRPT_AES_CTL_OUTSWAP_Msk | 
                    CRPT_AES_CTL_DMAEN_Msk |
                    GCM_MODE |
                    keySizeOpt;

    return( 0 );
}


#define swap32(x) (((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | ((x) & 0xff0000) >> 8 | ((x) >> 24) & 0xff)
static void swap64(uint8_t *p)
{
    uint8_t tmp;
    int32_t i;

    for(i = 0; i < 4; i++)
    {
        tmp = p[i];
        p[i] = p[7 - i];
        p[7 - i] = tmp;
    }
}


/*
NOTE: pbuf must be word alignment

    GCM input format must be block alignment. The block size is 16 bytes.
    {IV}{IV nbits}{A}{P/C}


*/

int32_t AES_GCMPacker(const uint8_t *iv, uint32_t iv_len, const uint8_t *A, uint32_t A_len, const uint8_t *P, uint32_t P_len, uint8_t *pbuf, uint32_t *psize)
{
    uint32_t i;
    uint32_t iv_len_aligned, A_len_aligned, P_len_aligned;
    uint32_t u32Offset = 0;
    uint8_t *pu8;

    /* IV Section:

       if bitlen(IV) == 96
         IV section = IV || 31'bit 0 || 1

       if bitlen(IV) != 96
         IV section = 128'align(IV) || 64'bit 0 || 64'bitlen(IV)
    */
    if(iv_len > 0)
    {
        iv_len_aligned = iv_len;
        if(iv_len & 0xful)
            iv_len_aligned = ((iv_len + 16) >> 4) << 4;

        /* fill iv to output */
        for(i = 0; i < iv_len_aligned; i++)
        {
            if(i < iv_len)
                pbuf[i] = iv[i];
            else
                pbuf[i] = 0; // padding zero
        }

        /* fill iv len to putput */
        if(iv_len == 12)
        {
            pbuf[15] = 1;
            u32Offset += iv_len_aligned;
        }
        else
        {
            /* Padding zero. 64'bit 0 */
            memset(&pbuf[iv_len_aligned], 0, 8);

            /* 64'bitlen(IV) */
            pu8 = &pbuf[iv_len_aligned + 8];
            *((uint64_t *)pu8) = iv_len * 8;
            swap64(pu8);
            u32Offset += iv_len_aligned + 16;
        }
    }


    /* A Section = 128'align(A) */
    if(A_len > 0)
    {
        A_len_aligned = A_len;
        if(A_len & 0xful)
            A_len_aligned = ((A_len + 16) >> 4) << 4;

        for(i = 0; i < A_len_aligned; i++)
        {
            if(i < A_len)
                pbuf[u32Offset + i] = A[i];
            else
                pbuf[u32Offset + i] = 0; // padding zero
        }

        u32Offset += A_len_aligned;
    }

    /* P/C Section = 128'align(P/C) */
    if(P_len > 0)
    {
        P_len_aligned = P_len;
        if(P_len & 0xful)
            P_len_aligned = ((P_len + 16) >> 4) << 4;

        for(i = 0; i < P_len_aligned; i++)
        {
            if(i < P_len)
                pbuf[u32Offset + i] = P[i];
            else
                pbuf[u32Offset + i] = 0; // padding zero
        }
        u32Offset += P_len_aligned;
    }

    *psize = u32Offset;

    return 0;
}


static int32_t AES_Run(mbedtls_gcm_context *ctx, uint32_t u32Option)
{
    int32_t i, klen;
    bool ret;
    
    klen = ctx->keySize;
    
    /* Set AES Key into H/W */
    for( i = 0; i < klen / 4; i++)
    {
        CRPT->AES_KEY[i] = ctx->keys[i];
    }
    
    crypto_aes_prestart();
    CRPT->AES_CTL = u32Option | START;
    ret = crypto_aes_wait();
    if( ret == false )
    {
        printf("###[WARN] AES GCM got NU_CRYPTO_DONE_ERR \n");
    }
    
    return 0;
}


#if 0
static void dump(char* buf, int size, char* str)
{
    int i;
    printf("\r\n%s:", str);
    for(i = 0; i < size; i++)
    {
        if((i % 16) == 0)
            printf("\r\n");
        printf("%02x ", buf[i]);
    }
    printf("\r\n");

}
#endif



int mbedtls_gcm_starts( mbedtls_gcm_context *ctx,
                int mode,
                const unsigned char *iv,
                size_t iv_len,
                const unsigned char *add,
                size_t add_len )
{
    
    
    uint32_t size;
    size_t *pSz;
    int32_t ret;
    
    if( ctx-> pcntLen == 0 ) ctx-> pcntLen = -1;
    ctx->len= 0x00;
    
    Debug_GCM_Info(("## FUNC: %s, mode=%s, pcnt=%d, ctx->len=%d\n", __FUNCTION__, (mode) ? "Enc":"Dec", ctx-> pcntLen, ctx->len));
    /* Acquire ownership of AES H/W */
    crypto_aes_acquire();
    
#if 1
    /* Force AES free */
    CRPT->AES_CTL = CRPT_AES_CTL_STOP_Msk;
#endif
    
    AES_ENABLE_INT(CRPT);
    
    if(mode == MBEDTLS_GCM_ENCRYPT)
    {
        ctx->basicOpt |= CRPT_AES_CTL_ENCRPT_Msk;
    } 
    else 
    {
        ctx->basicOpt &= ~CRPT_AES_CTL_ENCRPT_Msk;
    }
    /* Set byte count of IV */
    pSz = (size_t *)CRPT->AES_GCM_IVCNT;
    *pSz = iv_len;
    /* Set bytes count of A */
    pSz = (size_t *)CRPT->AES_GCM_ACNT;
    *pSz = add_len;
    
    AES_GCMPacker(iv, iv_len, add, add_len, 0, 0, ctx->gcm_buf, &size);
    ctx->gcm_buf_bytes = size;

    /* Configure DMA */
    CRPT->AES_SADDR = (uint32_t)ctx->gcm_buf;
    CRPT->AES_DADDR = (uint32_t)ctx->out_buf;
    CRPT->AES_FBADDR = (uint32_t)ctx->fb_buf;
    CRPT->AES_CNT   = ctx->gcm_buf_bytes;
    
    /* Set a big number for unknown P length */
    CRPT->AES_GCM_PCNT[0] = ctx-> pcntLen; //(uint32_t)-1;
    CRPT->AES_GCM_PCNT[1] = 0;

    /* Start with cascade mode */
//    if((ret = AES_Run(ctx, ctx->basicOpt | FBOUT)))
    if((ret = AES_Run(ctx, ctx->basicOpt | GCM_MODE | FBOUT | DMAEN)))
    {
        return ret;
    }

    ctx->firstFlag = 1;
    ctx->endFlag = 0;
    
    return( 0 );
}



int mbedtls_gcm_update( mbedtls_gcm_context *ctx,
                size_t input_length,
                const unsigned char *input,
                unsigned char *output )
{
    

    int32_t ret;
    int32_t len, len_aligned;
    uint32_t u32Size;
    Debug_GCM_Info(("## FUNC: %s, input_length=%d\n", __FUNCTION__, input_length));    
    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( input_length == 0 || input != NULL );
    GCM_VALIDATE_RET( input_length == 0 || output != NULL );
    
    len = (int32_t)input_length;
    /* Error if length too large */
    if( (size_t)len != input_length)
        return( MBEDTLS_ERR_GCM_BAD_INPUT );
            
    
    len_aligned = (len & 0xf) ? (int32_t)(len & (~0xful))+16:len;

    if(len == 0)
    {
        CRPT->AES_GCM_PCNT[0] = ctx->len;
        CRPT->AES_CNT = ctx->gcm_buf_bytes;
        if(ctx->firstFlag == 1)
        {
            /* No any P/C data, just use one shot to redo */
            AES_Run(ctx, ctx->basicOpt);
            ctx->gcm_buf_bytes = 0;
            
            /* The tag should be in out_buf if P len is 0 */
            memcpy(ctx->tag, ctx->out_buf, 16);
            ctx->endFlag = 1;
            ctx->firstFlag = 0;
            
        }
        else
        {
            /* zero block, it should be end of gcm. Restore feedback buffer and do GCM again with last cascade */
            memcpy(ctx->fb_buf, ctx->fb_buf2, 72);
            CRPT->AES_GCM_PCNT[0] = ctx->len;
            if((ret = AES_Run(ctx, ctx->basicOpt | FBIN | FBOUT | DMACC | DMALAST)))
            {
                return ret;
            }
            ctx->gcm_buf_bytes = 0;
            ctx->endFlag = 1;
            
            /* Output p/c data */
            memcpy(output, ctx->out_buf, len);
            
            /* Output tag */
            memcpy(ctx->tag, ctx->out_buf+len_aligned, 16);
        }
    }
    else
    {
        if(len <= MAX_GCM_BUF)
        {
            AES_GCMPacker(0, 0, 0, 0, input, len, ctx->gcm_buf, &u32Size);
            ctx->len += len;
            ctx->gcm_buf_bytes = u32Size;
        }
        else
        {

            /* Over buffer size */
            return (MBEDTLS_ERR_GCM_BAD_INPUT);
        }
            
        /* Do GCM with cascade */
        if(len & 0xf)
        {
            /* No 16 bytes alignment, it should be last */
            
            CRPT->AES_GCM_PCNT[0] = ctx->len;
            CRPT->AES_GCM_PCNT[1] = 0;
            CRPT->AES_CNT = u32Size;

            if((ret = AES_Run(ctx, ctx->basicOpt | FBIN | FBOUT | DMACC | DMALAST)))
            {
                return ret;
            }
            ctx->endFlag = 1;
        }
        else
        {
            /* backup feedback buffer. If this is last block, we could back feedback buffer to do it again. */
            memcpy(ctx->fb_buf2, ctx->fb_buf, 72);
//            ctx->firstFlag = 0;
            CRPT->AES_CNT = u32Size;

            if((ret = AES_Run(ctx, ctx->basicOpt | FBIN | FBOUT | DMACC)))
            {
                return ret;
            }

        }

        /* Output p/c data */
        memcpy(output, ctx->out_buf, len);
        
        if(ctx->endFlag)
        {
            /* Output tag */
            memcpy(ctx->tag, ctx->out_buf+len_aligned, 16);
        }
    }

    return( 0 );
}


int mbedtls_gcm_finish( mbedtls_gcm_context *ctx,
                unsigned char *tag,
                size_t tag_len )
{

    int32_t ret = 0;
    Debug_GCM_Info(("## FUNC: %s, tag_len=%d, pcnt=%d\n", __FUNCTION__, tag_len, ctx->len));    
    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( tag != NULL );
    
    if(ctx->endFlag == 0)
    {
        /* end the gcm */
        CRPT->AES_GCM_PCNT[0] = ctx->len;
        memcpy(ctx->fb_buf, ctx->fb_buf2, 72);
        if((ret = AES_Run(ctx, ctx->basicOpt | FBIN | FBOUT | DMACC | DMALAST)))
        {
            goto gcm_exit;
        }
        ctx->endFlag = 1;
        
        /* The tag should be in out_buf if P len is 0 */
        
        memcpy(ctx->tag, ctx->out_buf+ctx->gcm_buf_bytes, 16);
    }
    
    
    if(tag_len > 16)
    {
        tag_len = 16;
    }
    memcpy(tag, ctx->tag, tag_len);

gcm_exit:
    /* Release ownership of AES H/W */
    crypto_aes_release();
    
    return( ret );
}


//#define NVT_GCM_USE_GHASH_CTR_MODE

#ifndef NVT_GCM_USE_GHASH_CTR_MODE
/* 
* Apply GCM mode only for Decrypt, Encrypt & Tag
*/
int mbedtls_gcm_crypt_and_tag( mbedtls_gcm_context *ctx,
                       int mode,
                       size_t length,
                       const unsigned char *iv,
                       size_t iv_len,
                       const unsigned char *add,
                       size_t add_len,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t tag_len,
                       unsigned char *tag )
{
    int32_t plen_cur;
    int32_t len, len_aligned;
    const uint8_t *pin;
    uint8_t *pout;

    int ret = MBEDTLS_ERR_GCM_AUTH_FAILED;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );
    GCM_VALIDATE_RET( tag != NULL );
    ctx-> pcntLen = length;
    if( ( ret = mbedtls_gcm_starts( ctx, mode, iv, iv_len, add, add_len ) ) != 0 )
        return( ret );

    if( length == 0 )  /* if P length > 0, mbedtls_gcm_update not need gcm_buf_bytes for AES_CNT */
    {
        ctx->gcm_buf_bytes = 16;
    }else{
        ctx->gcm_buf_bytes = 0;
    }
    plen_cur = length;
    pin = input;
    pout = output;
    do
    {
        len = plen_cur;
        if(len > GCM_PBLOCK_SIZE)
        {
            len = GCM_PBLOCK_SIZE;
        }
        plen_cur -= len;

        /* Prepare the blocked buffer for GCM */
        memcpy(ctx->gcm_buf, pin, len);

        if( ( ret = mbedtls_gcm_update( ctx, len, pin, pout ) ) != 0 )
            return( ret );
        pin += len;
        pout += len;
    }while(plen_cur);
    
    if( ( ret = mbedtls_gcm_finish( ctx, tag, tag_len ) ) != 0 )
        return( ret );

    return( 0 );
}

int mbedtls_gcm_auth_decrypt( mbedtls_gcm_context *ctx,
                      size_t length,
                      const unsigned char *iv,
                      size_t iv_len,
                      const unsigned char *add,
                      size_t add_len,
                      const unsigned char *tag,
                      size_t tag_len,
                      const unsigned char *input,
                      unsigned char *output )
{
    int ret = MBEDTLS_ERR_GCM_AUTH_FAILED;
    unsigned char check_tag[16];
    size_t i;
    int diff;

    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( tag != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );

    if( ( ret = mbedtls_gcm_crypt_and_tag( ctx, MBEDTLS_GCM_DECRYPT, length,
                                   iv, iv_len, add, add_len,
                                   input, output, tag_len, check_tag ) ) != 0 )
    {
        return( ret );
    }

    /* Check tag in "constant-time" */
    for( diff = 0, i = 0; i < tag_len; i++ )
        diff |= tag[i] ^ check_tag[i];

    if( diff != 0 )
    {
        mbedtls_platform_zeroize( output, length );
        return( MBEDTLS_ERR_GCM_AUTH_FAILED );
    }

    return( 0 );
}

#else /* NVT_GCM_USE_GHASH_CTR_MODE */

/* 
* Apply GHASH & CTR mode for Tag calculation @GCMEnc
*/
/*
    AES_GCMTag is only used by AES_GCMEnc to calculate tag.
*/
static int32_t _GCMTag(mbedtls_gcm_context *ctx, const uint8_t *iv, uint32_t ivlen, const uint8_t *A, uint32_t alen, const uint8_t *P, uint32_t plen, uint8_t *tagbuf)
{
    int32_t ret;
    int32_t i, len, plen_cur;
    const uint8_t *pin;
    uint8_t *pout;
    uint32_t inputblock[MAX_GCM_BUF * 2] = {0}; /* 2 block buffer, 1 for A, 1 for P */
    uint32_t ghashbuf[MAX_GCM_BUF + 16] = {0};
    uint8_t *pblock;
    uint32_t u32OptBasic;
    uint32_t u32OptKeySize;
    uint32_t tag[4];
    
    Debug_GCM_Info(("## FUNC: %s\n", __FUNCTION__));
    /* Prepare key size option */
    i = ctx->keySize >> 3;
    u32OptKeySize = (((i >> 2) << 1) | (i & 1)) << CRPT_AES_CTL_KEYSZ_Pos;

    /* Basic options for AES */
    u32OptBasic = CRPT_AES_CTL_ENCRPT_Msk | CRPT_AES_CTL_INSWAP_Msk | CRPT_AES_CTL_OUTSWAP_Msk | u32OptKeySize;

    /* Set byte count of IV */
    CRPT->AES_GCM_IVCNT[0] = ivlen;
    CRPT->AES_GCM_IVCNT[1] = 0;
    /* Set bytes count of A */
    CRPT->AES_GCM_ACNT[0] = alen;
    CRPT->AES_GCM_ACNT[1] = 0;
    /* Set bytes count of P */
    CRPT->AES_GCM_PCNT[0] = plen;
    CRPT->AES_GCM_PCNT[1] = 0;


    // GHASH(128'align(A) || 128'align(C) || 64'bitlen(A) || 64'bitlen(C))
    // GHASH Calculation
    if(plen <= GCM_PBLOCK_SIZE)
    {
        /* Just one shot if plen < maximum block size */

        pblock = (uint8_t *)&inputblock[0];
        AES_GCMPacker(0, 0, A, alen, P, plen, pblock, (uint32_t *)&len);

        /* append 64'bitlen(A) || 64'bitlen(C) */
        pblock += len;
        *((uint64_t *)pblock) = alen * 8;
        swap64(pblock);
        pblock += 8;

        *((uint64_t *)pblock) = plen * 8;
        swap64(pblock);
        pblock += 8;

        /* adding the length of 64'bitlen(A) and 64'bitlen(C) */
        len += 16;

        pblock = (uint8_t *)&inputblock[0];

        CRPT->AES_SADDR = (uint32_t)pblock;
        CRPT->AES_DADDR = (uint32_t)&ghashbuf[0];
        CRPT->AES_CNT = len;

        AES_Run(ctx, u32OptBasic | GHASH_MODE | DMAEN /*| DMALAST*/);
    }
    else
    {
        /* Calculate GHASH block by block, DMA casecade mode */

        /* feedback buffer is necessary for casecade mode */
        CRPT->AES_FBADDR = (uint32_t)ctx->fb_buf;
        memset(ctx->fb_buf, 0, sizeof(ctx->fb_buf));

        /* inital DMA for GHASH casecade */
        if(alen)
        {
            /* Prepare the blocked buffer for GCM */
            AES_GCMPacker(0, 0, A, alen, 0, 0, ctx->gcm_buf, (uint32_t *)&len);

            CRPT->AES_SADDR = (uint32_t)ctx->gcm_buf;
            CRPT->AES_DADDR = (uint32_t)&ghashbuf[0];
            CRPT->AES_CNT = len;

            AES_Run(ctx, u32OptBasic | GHASH_MODE | FBOUT | DMAEN);
        }

        /* Calculate GHASH block by block */
        pin = P;
        pout = (uint8_t *)&ghashbuf[0];
        plen_cur = plen;
        len = GCM_PBLOCK_SIZE;
        while(plen_cur)
        {

            len = plen_cur;
            if(len > GCM_PBLOCK_SIZE)
                len = GCM_PBLOCK_SIZE;
            plen_cur -= len;

            if(plen_cur)
            {
                /* Sill has data for next block, it means current block size is full size */

                /* len should be alway 16 bytes alignment in here */
                CRPT->AES_SADDR = (uint32_t)pin;
                CRPT->AES_DADDR = (uint32_t)pout;
                CRPT->AES_CNT = len;

                AES_Run(ctx, u32OptBasic | GHASH_MODE | FBIN | FBOUT | DMAEN | DMACC);
            }
            else
            {
                /* Next block data size is 0, it means current block size is not full size and this is last block */

                /* copy last C data to inputblock for zero padding */
                memcpy((uint8_t *)&inputblock[0], pin, len);
                pin = (uint8_t *)&inputblock[0];

                /* 16 bytes alignment check */
                if(len & 0xf)
                {
                    /* zero padding */
                    memset((void *)(pin + len), 0, 16 - (len & 0xf));

                    /* len must be 16 bytes alignment */
                    len = ((len + 16) >> 4) << 4;
                }

                /* append 64'bitlen(A) || 64'bitlen(C) */
                pblock = (uint8_t *)pin + len;
                *((uint64_t *)pblock) = alen * 8;
                swap64(pblock);
                pblock += 8;

                *((uint64_t *)pblock) = plen * 8;
                swap64(pblock);
                pblock += 8;

                /* adding the length of 64'bitlen(A) and 64'bitlen(C) */
                len += 16;

                CRPT->AES_SADDR = (uint32_t)pin;
                CRPT->AES_DADDR = (uint32_t)pout;
                CRPT->AES_CNT = len;

                AES_Run(ctx, u32OptBasic | GHASH_MODE | FBIN | FBOUT | DMAEN | DMACC | DMALAST);

            }

            pin += len;
        }
    }

    // CTR(IV, GHASH(128'align(A) || 128'align(C) || 64'bitlen(A) || 64'bitlen(C)))
    // CTR calculation

    /* Prepare IV */
    if(ivlen != 12)
    {
        uint32_t u32ivbuf[4] = {0};
        uint8_t *piv;

        // IV = GHASH(128'align(IV) || 64'bitlen(0) || 64'bitlen(IV))

        piv = (uint8_t *)&u32ivbuf[0];
        AES_GCMPacker(iv, ivlen, 0, 0, 0, 0, ctx->gcm_buf, (uint32_t *)&len);

        CRPT->AES_SADDR = (uint32_t)ctx->gcm_buf;
        CRPT->AES_DADDR = (uint32_t)piv;
        CRPT->AES_CNT = len;

        if((ret = AES_Run(ctx, u32OptBasic | GHASH_MODE | DMAEN/* | DMALAST*/)))
        {
            return ret;
        }

        /* SET CTR IV */
        for(i = 0; i < 4; i++)
        {
            CRPT->AES_IV[i] = (piv[i * 4 + 0] << 24) | (piv[i * 4 + 1] << 16) |
                              (piv[i * 4 + 2] << 8) | piv[i * 4 + 3];
        }
    }
    else
    {
        // IV = 128'align(IV) || 31'bitlen(0) || 1

        /* SET CTR IV */
        for(i = 0; i < 3; i++)
        {
            CRPT->AES_IV[i] = (iv[i * 4 + 0] << 24) | (iv[i * 4 + 1] << 16) |
                              (iv[i * 4 + 2] << 8) | iv[i * 4 + 3];
        }
        CRPT->AES_IV[3] = 0x00000001;
    }

    CRPT->AES_SADDR = (uint32_t)&ghashbuf[0];
    CRPT->AES_DADDR = (uint32_t)&tag[0];
    CRPT->AES_CNT = 16;

    ret = AES_Run(ctx, u32OptBasic | CTR_MODE | DMAEN /*| DMALAST*/);

    memcpy(tagbuf, tag, 16);
    Debug_GCM_Info(("## FUNC: %s finish tag 0x%x, 0x%x, 0x%x, 0x%x\n", __FUNCTION__, tag[0], tag[1], tag[2], tag[3]));
    
    return ret;
}


static int32_t _GCM(mbedtls_gcm_context *ctx, const uint8_t *iv, uint32_t ivlen, const uint8_t *A, uint32_t alen, const uint8_t *P, uint32_t plen, uint8_t *buf, uint8_t *tag, uint32_t tag_len)
{
    int32_t ret;
    int32_t plen_cur;
    int32_t len, len_aligned;
    const uint8_t *pin;
    uint8_t *pout;
    uint32_t u32OptBasic;
    uint32_t plen_aligned;
    uint32_t size;

#if 0
    SYS->IPRST0 = SYS_IPRST0_CRPTRST_Msk;
    SYS->IPRST0 = 0;
#endif

    u32OptBasic = ctx->basicOpt;

    /* Set byte count of IV */
    CRPT->AES_GCM_IVCNT[0] = ivlen;
    CRPT->AES_GCM_IVCNT[1] = 0;

    /* Set bytes count of A */
    CRPT->AES_GCM_ACNT[0] = alen;
    CRPT->AES_GCM_ACNT[1] = 0;
    
    /* Set bytes count of P */
    CRPT->AES_GCM_PCNT[0] = plen;
    CRPT->AES_GCM_PCNT[1] = 0;

    plen_aligned = (plen & 0xful) ? ((plen + 16) / 16) * 16 : plen;
    
    if(plen == 0)   /* For AWS-IoT connection case, force go cascade instead of if(plen <= GCM_PBLOCK_SIZE) */
    {
        /* Just one shot */

        /* Prepare the blocked buffer for GCM */
        AES_GCMPacker(iv, ivlen, A, alen, P, plen, ctx->gcm_buf, &size);

        CRPT->AES_SADDR = (uint32_t)ctx->gcm_buf;
        CRPT->AES_DADDR = (uint32_t)ctx->out_buf;
        CRPT->AES_CNT = size;

        ret = AES_Run(ctx, u32OptBasic | GCM_MODE | DMAEN);

        memcpy(buf, ctx->out_buf, plen);
        memcpy(tag, ctx->out_buf + plen_aligned, tag_len);
      
    }
    else
    {

        /* Process P block by block, DMA casecade mode */

        /* inital DMA for AES-GCM casecade */

        /* Prepare the blocked buffer for GCM */
        AES_GCMPacker(iv, ivlen, A, alen, 0, 0, ctx->gcm_buf, &size);

        CRPT->AES_SADDR = (uint32_t)ctx->gcm_buf;
        CRPT->AES_DADDR = (uint32_t)ctx->out_buf;
        CRPT->AES_CNT = size;

        /* feedback buffer is necessary for casecade mode */
        CRPT->AES_FBADDR = (uint32_t)ctx->fb_buf;

        AES_Run(ctx, u32OptBasic | GCM_MODE | FBOUT | DMAEN);

        /* Start to encrypt P data */
        plen_cur = plen;
        pin = P;
        pout = buf;
        while(plen_cur)
        {
            len = plen_cur;
            if(len > GCM_PBLOCK_SIZE)
            {
                len = GCM_PBLOCK_SIZE;
            }
            plen_cur -= len;

            /* Prepare the blocked buffer for GCM */
            memcpy(ctx->gcm_buf, pin, len);
            /* padding 0 if necessary */
            if(len & 0xf)
            {
                memset(&ctx->gcm_buf[len], 0, 16 - (len & 0xf));
                len_aligned = ((len + 16) >> 4) << 4;
            }
            else
            {
                len_aligned = len;
            }

            CRPT->AES_SADDR = (uint32_t)ctx->gcm_buf;
            CRPT->AES_DADDR = (uint32_t)ctx->out_buf;
            CRPT->AES_CNT = len_aligned;


            if(plen_cur)
            {
                /* casecade n */
                ret = AES_Run(ctx, u32OptBasic | GCM_MODE | FBIN | FBOUT | DMAEN | DMACC);
            }
            else
            {
                /* last casecade */
                ret = AES_Run(ctx, u32OptBasic | GCM_MODE | FBIN | FBOUT | DMAEN | DMACC | DMALAST);
            }
            if(ret < 0)
            {
                return ret;
            }

            memcpy(pout, ctx->out_buf, len);
            
            pin += len;
            pout += len;
        }
        
        memcpy(tag, ctx->out_buf+len_aligned, tag_len);
        Debug_GCM_Info(("## Tag in FUNC: %s, plen=%d, tag=0x%x, 0x%x, 0x%x, 0x%x\n", __FUNCTION__, plen,  
                            *((uint32_t*)tag), *((uint32_t*)(tag+4)), *((uint32_t*)(tag+8)), *((uint32_t*)(tag+16)) )); 
    }
    
    if(ctx->mode)
    {
        /* H/W limitation under plen%16 as 1 or 15, need re-calculate tag by _GCMTag */
        /* Need to calculate Tag when plen % 16 == 1 or 15 */
        if(( (plen & 0xf) == 1) || ((plen & 0xf) == 15))
        {
            if((ret = _GCMTag(ctx, iv, ivlen, A, alen, ctx->out_buf, plen, tag)))
            {
                return ret;
            }
        }
    }

    return 0;
}

int mbedtls_gcm_crypt_and_tag( mbedtls_gcm_context *ctx,
                               int mode,
                               size_t length,
                               const unsigned char *iv,
                               size_t iv_len,
                               const unsigned char *add,
                               size_t add_len,
                               const unsigned char *input,
                               unsigned char *output,
                               size_t tag_len,
                               unsigned char *tag )
{
    int ret;
    Debug_GCM_Info(("## FUNC: %s, mode=%s, length=%d, tag_len=%d, in/out=0x%x/0x%x\n", __FUNCTION__, (mode) ? "Enc":"Dec", length, tag_len, input, output));
    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );
    GCM_VALIDATE_RET( tag != NULL );

    ctx->mode = mode;

    if(mode)
    {
        ctx->basicOpt |= CRPT_AES_CTL_ENCRPT_Msk;
    }
    else
    {
        ctx->basicOpt &= ~CRPT_AES_CTL_ENCRPT_Msk;
    }

    /* Acquire ownership of AES H/W */
    crypto_aes_acquire();   
#if 1
    /* Force AES free */
    CRPT->AES_CTL = CRPT_AES_CTL_STOP_Msk;
#endif    
    AES_ENABLE_INT(CRPT);
    
    ret = _GCM(ctx, iv, iv_len, add, add_len, input, length, output, tag, tag_len);

    /* Release ownership of AES H/W */
    crypto_aes_release();

    return (ret);
}

int mbedtls_gcm_auth_decrypt( mbedtls_gcm_context *ctx,
                              size_t length,
                              const unsigned char *iv,
                              size_t iv_len,
                              const unsigned char *add,
                              size_t add_len,
                              const unsigned char *tag,
                              size_t tag_len,
                              const unsigned char *input,
                              unsigned char *output )
{
    int ret;
    unsigned char check_tag[16];
    size_t i;
    int diff;
    Debug_GCM_Info(("## FUNC: %s\n", __FUNCTION__));
    GCM_VALIDATE_RET( ctx != NULL );
    GCM_VALIDATE_RET( iv != NULL );
    GCM_VALIDATE_RET( add_len == 0 || add != NULL );
    GCM_VALIDATE_RET( tag != NULL );
    GCM_VALIDATE_RET( length == 0 || input != NULL );
    GCM_VALIDATE_RET( length == 0 || output != NULL );

    if( ( ret = mbedtls_gcm_crypt_and_tag( ctx, MBEDTLS_GCM_DECRYPT, length,
                                           iv, iv_len, add, add_len,
                                           input, output, tag_len, check_tag ) ) != 0 )
    {
        return( ret );
    }

    /* Check tag in "constant-time" */
    for( diff = 0, i = 0; i < tag_len; i++ )
        diff |= tag[i] ^ check_tag[i];

    if( diff != 0 )
    {
        mbedtls_platform_zeroize( output, length );
        return( MBEDTLS_ERR_GCM_AUTH_FAILED );
    }

    return( 0 );
}


#endif /* NVT_GCM_USE_GHASH_CTR_MODE */
#endif /* MBEDTLS_GCM_ALT */
#endif /* MBEDTLS_GCM_C */
