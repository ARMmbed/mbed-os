/*
 *  NIST SP800-38C compliant CCM implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (c) 2022, Nuvoton Technology Corporation
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
 * Definition of CCM:
 * http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf
 * RFC 3610 "Counter with CBC-MAC (CCM)"
 *
 * Related:
 * RFC 5116 "An Interface and Algorithms for Authenticated Encryption"
 */

#include "common.h"
#include "mbedtls/aes.h"

#if defined(MBEDTLS_CCM_C)

#include "mbedtls/ccm.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"
#include <string.h>

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

#if defined(MBEDTLS_CCM_ALT)

#include "M460.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"

#define CCM_VALIDATE_RET( cond ) \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_CCM_BAD_INPUT )
#define CCM_VALIDATE( cond ) \
    MBEDTLS_INTERNAL_VALIDATE( cond )

#define CCM_ENCRYPT 1
#define CCM_DECRYPT 0



#define CCM_MODE    (AES_MODE_CCM << CRPT_AES_CTL_OPMODE_Pos)
#define START       CRPT_AES_CTL_START_Msk
#define DMAEN       CRPT_AES_CTL_DMAEN_Msk
#define DMALAST     CRPT_AES_CTL_DMALAST_Msk
#define DMACC       CRPT_AES_CTL_DMACSCAD_Msk
#define START       CRPT_AES_CTL_START_Msk
#define FBIN        CRPT_AES_CTL_FBIN_Msk
#define FBOUT       CRPT_AES_CTL_FBOUT_Msk


#define Debug_CCM_Info(x) {} 
//#define Debug_CCM_Info(x) { printf x; }

int32_t ToBigEndian(uint8_t *pbuf, uint32_t u32Size)
{
    uint32_t i;
    uint8_t u8Tmp;
    uint32_t u32Tmp;

    /* pbuf must be word alignment */
    if((uint32_t)pbuf & 0x3)
    {
        printf("The buffer must be 32-bit alignment.");
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


/*
    CCM input format must be block alignment. The block size is 16 bytes.

    ----------------------------------------------------------------------
     Block B0
          Formatting of the Control Information and the Nonce
    ----------------------------------------------------------------------
    First block B_0:
    0        .. 0        flags
    1        .. iv_len   nonce (aka iv)
    iv_len+1 .. 15       length


    flags:
    With flags as (bits):
    7        0
    6        add present?
    5 .. 3   (t - 2) / 2
    2 .. 0   q - 1

*/

int32_t CCMPacker(const uint8_t *iv, uint32_t ivlen, const uint8_t *A, uint32_t alen, const uint8_t *P, uint32_t plen, uint8_t *pbuf, uint32_t *psize, uint32_t tlen, int32_t enc)
{
    uint32_t i, j;
    uint32_t alen_aligned, plen_aligned;
    uint32_t u32Offset = 0;
    uint8_t u8Tmp;
    uint32_t q = 0;


    /* Flags in B0
    *With flags as(bits) :
        7        0
        6        add present ?
        5 .. 3   (t - 2) / 2
        2 .. 0   q - 1, q = 15 - nlen
    */
    if( tlen > 0 )
    {
        q = 15 - ivlen;
        u8Tmp = (q - 1) | ((tlen - 2) / 2 << 3) | ((alen > 0) ? 0x40 : 0);
        pbuf[0] = u8Tmp;            // flags
        for(i = 0; i < ivlen; i++)  // N
            pbuf[i + 1] = iv[i];
        for(i = ivlen + 1, j = q - 1; i < 16; i++, j--)    // Q
        {
            if(j >= 4)
                pbuf[i] = 0;
            else
            {
                pbuf[i] = (plen >> j * 8) & 0xfful;
            }
        }

        u32Offset = 16;
    }
    /* Formatting addition data */
    /* alen. It is limited to be smaller than 2^16-2^8 */
    if(alen > 0)
    {
        pbuf[u32Offset] = (alen >> 8) & 0xfful;
        pbuf[u32Offset + 1] = alen & 0xfful;

        for(i = 0; i < alen; i++)
            pbuf[u32Offset + i + 2] = A[i];

        alen_aligned = ((alen + 2 + 15) / 16) * 16;
        for(i = u32Offset + 2 + alen; i < alen_aligned; i++)
        {
            pbuf[i] = (enc) ? 0:0xff; // padding zero or 0xff
        }

        u32Offset += alen_aligned;
    }

    /* Formatting payload */
    if( (plen > 0) && (P != 0x00))
    {
        plen_aligned = ((plen + 15) / 16) * 16;
        for(i = 0; i < plen; i++)
        {
            pbuf[u32Offset + i] = P[i];
        }
        for(; i < plen_aligned; i++)
        {
            pbuf[u32Offset + i] = 0; // padding zero
        }

        u32Offset += plen_aligned;
    }


    /* Formatting Ctr0 */
    if( ivlen > 0 )
    {
        pbuf[u32Offset] = q - 1; // Flags
        for(i = 0; i < ivlen; i++) // N
        {
            pbuf[u32Offset + 1 + i] = iv[i];
        }
        for(; i < 16; i++)
        {
            pbuf[u32Offset + 1 + i] = 0; // padding zero to block alignment
        }
    }
    *psize = u32Offset;

    return 0;
}

#ifdef NVT_CCM_DEBUG
static void DumpBuffHex(uint8_t *pucBuff, int nBytes)
{
    int32_t i32Idx, i, len;


    i32Idx = 0;
    while(nBytes > 0)
    {
        printf("0x%04X  ", i32Idx);

        len = (nBytes < 16) ? nBytes : 16;
        for(i = 0; i < len; i++)
            printf("%02x ", pucBuff[i32Idx + i]);
        for(; i < 16; i++)
        {
            printf("   ");
        }
        printf("  ");
        for(i = 0; i < len; i++)
        {
            if((pucBuff[i32Idx + i] >= 0x20) && (pucBuff[i32Idx + i] < 127))
                printf("%c", pucBuff[i32Idx + i]);
            else
                printf(".");
            nBytes--;
        }
        i32Idx += len;
        printf("\n");
    }
    printf("\n");
}

static void DumpBuffHex2(uint8_t *pucBuff, int nBytes)
{
    int32_t i32Idx, i;

    i32Idx = 0;
    while(nBytes > 0)
    {
        printf("0x%04X  ", i32Idx);
        for(i = 0; i < 16; i += 4)
            printf("%08x ", *((uint32_t *)&pucBuff[i32Idx + i]));
        i32Idx += 16;
        nBytes -= 16;
        printf("\n");
    }
    printf("\n");
}
#endif


static int32_t _CCM_OneShot(mbedtls_ccm_context *ctx, int32_t encDec, const uint8_t *iv, uint32_t ivlen, const uint8_t *A, uint32_t alen, const uint8_t *P, uint32_t plen, uint8_t *buf, uint8_t *tag, uint32_t tlen)
{
    uint32_t size, plen_aligned;
    const uint8_t* pIn;
    uint8_t* pOut;
    bool ret;


    Debug_CCM_Info(("## FUNC: %s, alen=%lu, plen=%lu, ivlen=%lu, tlen=%lu\n", __FUNCTION__, alen, plen, ivlen, tlen));    
#ifdef NVT_CCM_DEBUG
    printf("\n");

    printf("key (%d):\n", (4 + 2*ctx->keySize)*4 );
    DumpBuffHex(ctx->keys, (4 + 2*ctx->keySize)*4);

    printf("N (%d):\n", ivlen);
    DumpBuffHex(iv, ivlen);

    printf("A (%d):\n", alen);
    DumpBuffHex(A, alen);

    printf("P (%d):\n", plen);
    DumpBuffHex(P, plen);

    printf("T len = %d\n", tlen);
#endif    
    
    
    /* Prepare the blocked buffer for CCM */
    memset(ctx->ccm_buf, 0, MAX_CCM_BUF);
    CCMPacker(iv, ivlen, A, alen, P, plen, ctx->ccm_buf, &size, tlen, encDec);

    ToBigEndian(ctx->ccm_buf, size + 16);

    plen_aligned = (plen & 0xful) ? ((plen + 15) / 16) * 16 : plen;

#ifdef NVT_CCM_DEBUG
    printf("input blocks (%d):\n", size);
    DumpBuffHex2(ctx->ccm_buf, size);
    
    printf("input key (%d):\n", size);
    DumpBuffHex2(ctx->keys, (4 + 2*ctx->keySize)*4);   

    printf("input Ctr0:\n");
    DumpBuffHex2(&(ctx->ccm_buf[size]), 16);
#endif
    
    AES_Open(CRPT, 0, encDec, ctx->opMode, ctx->keySize, AES_OUT_SWAP);
    /* Set AES Key into H/W */
    AES_SetKey(CRPT, 0, ctx->keys, ctx->keySize);
    
    /* For ctr0 */
    AES_SetInitVect(CRPT, 0, (uint32_t *)&ctx->ccm_buf[size]);


    /* Set bytes count of A */
    CRPT->AES_GCM_ACNT[0] = size - plen_aligned;
    CRPT->AES_GCM_ACNT[1] = 0;
    CRPT->AES_GCM_PCNT[0] = plen;
    CRPT->AES_GCM_PCNT[1] = 0;
    
    pIn = ctx->ccm_buf;
    pOut = ctx->out_buf;
    AES_SetDMATransfer(CRPT, 0, (uint32_t)pIn, (uint32_t)pOut, size);

    crypto_aes_prestart();

    /* Start AES crypt */
    AES_Start(CRPT, 0, CRYPTO_DMA_ONE_SHOT);
        
    /* Waiting for AES calculation */
    ret = crypto_aes_wait();
    if( ret == false )
    {
        printf("###[WARN] AES CCM got NU_CRYPTO_DONE_ERR \n");
        return (-1);
    }   

    memcpy(buf, ctx->out_buf, plen);
    
    if(tlen > 16)
    {
        tlen = 16;
    }
    memcpy(tag, &ctx->out_buf[plen_aligned], tlen);

    Debug_CCM_Info(("OneShot output blocks (%lu)(%lu):\n", size, plen));
#ifdef NVT_CCM_DEBUG
    DumpBuffHex2(buf, plen);
    printf("OneShot output blocks Tag :\n");
    DumpBuffHex2(&ctx->out_buf[plen_aligned], 16);
#endif    
    return 0;
}

static int32_t AES_Run(mbedtls_ccm_context *ctx, uint32_t u32Option)
{

    bool ret;
        
    crypto_aes_prestart();
    CRPT->AES_CTL = u32Option | START;
    ret = crypto_aes_wait();
    if( ret == false )
    {
        printf("###[WARN] AES CCM got NU_CRYPTO_DONE_ERR \n");
    }
    
    return 0;
}

static int32_t _CCM_Cascade(mbedtls_ccm_context *ctx, int32_t encDec, const uint8_t *iv, uint32_t ivlen, const uint8_t *A, uint32_t alen, const uint8_t *P, uint32_t plen, uint8_t *buf, uint8_t *tag, uint32_t tlen)
{
    uint32_t size, plen_aligned;
    uint32_t u32OptBasic;
    int32_t plen_cur;
    int32_t len;
    const uint8_t *pin; 
    uint8_t *pout;

    Debug_CCM_Info(("## FUNC: %s, alen=%lu, plen=%lu, ivlen=%lu, tlen=%lu\n", __FUNCTION__, alen, plen, ivlen, tlen));    
    /* Prepare the blocked buffer for CCM */
    memset(ctx->ccm_buf, 0, MAX_CCM_BUF);
    CCMPacker(iv, ivlen, A, alen, 0, plen, ctx->ccm_buf, &size, tlen, encDec);

    ToBigEndian(ctx->ccm_buf, size + 16);

    plen_aligned = (plen & 0xful) ? ((plen + 15) / 16) * 16 : plen;

    
    AES_Open(CRPT, 0, encDec, ctx->opMode, ctx->keySize, AES_OUT_SWAP);
    /* Set AES Key into H/W */
    AES_SetKey(CRPT, 0, ctx->keys, ctx->keySize);

#ifdef NVT_CCM_DEBUG
    printf("input blocks (%d):\n", size);
    DumpBuffHex2(ctx->ccm_buf, size);    
    printf("input Ctr0:\n");
    DumpBuffHex2(&(ctx->ccm_buf[size]), 16);
#endif
    
    /* For ctr0 */
    AES_SetInitVect(CRPT, 0, (uint32_t *)&ctx->ccm_buf[size]);

    /* Set bytes count of A */
    CRPT->AES_GCM_ACNT[0] = size;
    CRPT->AES_GCM_ACNT[1] = 0;
    CRPT->AES_GCM_PCNT[0] = plen; //plen_aligned;  /* set plen_aligned will get wrong tag @plen=24 */
    CRPT->AES_GCM_PCNT[1] = 0;
    
    AES_SetDMATransfer(CRPT, 0, (uint32_t)ctx->ccm_buf, (uint32_t)ctx->out_buf, size);

    /* feedback buffer is necessary for cascade mode */
    CRPT->AES_FBADDR = (uint32_t)ctx->fb_buf;;

    u32OptBasic = CRPT->AES_CTL ;
    
    AES_Run(ctx, u32OptBasic | CCM_MODE | FBOUT | DMAEN);
      
    plen_cur = plen;
    pin = P;
    pout = buf;
    while(plen_cur)
    {
          len = plen_cur;
          if(len > CCM_PBLOCK_SIZE)
          {
              len = CCM_PBLOCK_SIZE;
          }
          plen_cur -= len;

          /* Prepare the blocked buffer for CCM */
          CCMPacker(0, 0, 0, 0, pin, len, ctx->ccm_buf, &size, 0, encDec);
          
          ToBigEndian(ctx->ccm_buf, size);

          AES_SetDMATransfer(CRPT, 0, (uint32_t)ctx->ccm_buf, (uint32_t)pout, size);

          /* Start AES Eecrypt */
          if(plen_cur)
          {
              /* casecade n */
              AES_Run(ctx, u32OptBasic | CCM_MODE | FBIN | FBOUT | DMAEN | DMACC);
          }
          else
          {
              /* last cascade */
              AES_SetDMATransfer(CRPT, 0, (uint32_t)ctx->ccm_buf, (uint32_t)ctx->out_buf, size);
              AES_Run(ctx, u32OptBasic | CCM_MODE | FBIN | FBOUT | DMAEN | DMACC | DMALAST);
              memcpy(pout, ctx->out_buf, len);
          }
          Debug_CCM_Info(("output blocks (%lu)(%lu):\n", size, len));
#ifdef NVT_CCM_DEBUG
          DumpBuffHex2(pout, len);
#endif
          pin += len;
          pout += size;
    } 
#ifdef NVT_CCM_DEBUG
    printf("output blocks Tag :\n");
    DumpBuffHex2(&ctx->out_buf[size], 16);
#endif
    if(tlen > 16)
    {
        tlen = 16;
    }
    memcpy(tag, &ctx->out_buf[size], tlen);

    return 0;
}


static int32_t _CCM(mbedtls_ccm_context *ctx, int32_t encDec, const uint8_t *iv, uint32_t ivlen, const uint8_t *A, uint32_t alen, const uint8_t *P, uint32_t plen, uint8_t *buf, uint8_t *tag, uint32_t tlen)
{
    uint32_t plen_aligned, alen_aligned;
    int32_t ret;

    Debug_CCM_Info(("## FUNC: %s, alen=%lu, plen=%lu, ivlen=%lu, tlen=%lu\n", __FUNCTION__, alen, plen, ivlen, tlen));
    
    if(ivlen > 16)
        return -1;

    /* Formatting addition data: 2 bytes to store A's length  */
    alen_aligned = ((alen + 2 + 15) / 16) * 16;
    plen_aligned = ((plen + 15) / 16) * 16;
    
    /* Length of B0's formatting nonce (aka iv): 16  */
    if( (16 + alen_aligned + plen_aligned ) < CCM_PBLOCK_SIZE  )
    {
        ret = _CCM_OneShot(ctx, encDec, iv, ivlen, A, alen, P, plen, buf, tag, tlen);
    } else {
        ret = _CCM_Cascade(ctx, encDec, iv, ivlen, A, alen, P, plen, buf, tag, tlen);        
    }
    
    return(ret);
}

/*
 * Initialize context
 */
void mbedtls_ccm_init( mbedtls_ccm_context *ctx )
{
    CCM_VALIDATE( ctx != NULL );
    memset( ctx, 0, sizeof( mbedtls_ccm_context ) );
    
    ctx->opMode = AES_MODE_CCM;
     /* Init crypto module */
    crypto_init();    
}

int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    int32_t klen;
    
    CCM_VALIDATE_RET( ctx != NULL );
    CCM_VALIDATE_RET( key != NULL );
    CCM_VALIDATE_RET( keybits == 128 || keybits == 192 || keybits == 256 );


    klen = keybits / 8;
    
    memcpy(ctx->keys, key, klen);    
    ToBigEndian((uint8_t *)ctx->keys, klen);

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

    return( 0 );
}

/*
 * Free context
 */
void mbedtls_ccm_free( mbedtls_ccm_context *ctx )
{
    if( ctx == NULL )
        return;

    /* Uninit crypto module */
    crypto_uninit();
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_ccm_context ) );
}


/*
 * Authenticated encryption or decryption
 */
static int ccm_auth_crypt( mbedtls_ccm_context *ctx, int mode, size_t length,
                           const unsigned char *iv, size_t iv_len,
                           const unsigned char *add, size_t add_len,
                           const unsigned char *input, unsigned char *output,
                           unsigned char *tag, size_t tag_len )
{
    int ret;

    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     *
     * Also, loosen the requirements to enable support for CCM* (IEEE 802.15.4).
     */

    if( tag_len == 2 || tag_len > 16 || tag_len % 2 != 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( add_len >= 0xFF00 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    

    ctx->opMode = AES_MODE_CCM;
    ctx->encDec = mode;


    /* Acquire ownership of AES H/W */
    crypto_aes_acquire();   
#if 1
    /* Force AES free */
    CRPT->AES_CTL = CRPT_AES_CTL_STOP_Msk;
#endif    
    AES_ENABLE_INT(CRPT);
    
    ret = _CCM(ctx, mode, iv, iv_len, add, add_len, input, length, output, tag, tag_len);
    
    /* Release ownership of AES H/W */
    crypto_aes_release();
    
    return( ret );
}

/*
 * Authenticated encryption
 */
int mbedtls_ccm_star_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                                      const unsigned char *iv, size_t iv_len,
                                      const unsigned char *add, size_t add_len,
                                      const unsigned char *input, unsigned char *output,
                                      unsigned char *tag, size_t tag_len )
{
    Debug_CCM_Info(("## FUNC: %s, alen=%u, plen=%u, ivlen=%u, tlen=%u\n", __FUNCTION__, add_len, length, iv_len, tag_len));
    CCM_VALIDATE_RET( ctx != NULL );
    CCM_VALIDATE_RET( iv != NULL );
    CCM_VALIDATE_RET( add_len == 0 || add != NULL );
    CCM_VALIDATE_RET( length == 0 || input != NULL );
    CCM_VALIDATE_RET( length == 0 || output != NULL );
    CCM_VALIDATE_RET( tag_len == 0 || tag != NULL );
    return( ccm_auth_crypt( ctx, CCM_ENCRYPT, length, iv, iv_len,
                            add, add_len, input, output, tag, tag_len ) );
}

int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                                 const unsigned char *iv, size_t iv_len,
                                 const unsigned char *add, size_t add_len,
                                 const unsigned char *input, unsigned char *output,
                                 unsigned char *tag, size_t tag_len )
{
    Debug_CCM_Info(("## FUNC: %s, alen=%u, plen=%u, ivlen=%u, tlen=%u\n", __FUNCTION__, add_len, length, iv_len, tag_len));
    CCM_VALIDATE_RET( ctx != NULL );
    CCM_VALIDATE_RET( iv != NULL );
    CCM_VALIDATE_RET( add_len == 0 || add != NULL );
    CCM_VALIDATE_RET( length == 0 || input != NULL );
    CCM_VALIDATE_RET( length == 0 || output != NULL );
    CCM_VALIDATE_RET( tag_len == 0 || tag != NULL );
    if( tag_len == 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    return( mbedtls_ccm_star_encrypt_and_tag( ctx, length, iv, iv_len, add,
            add_len, input, output, tag, tag_len ) );
}

/*
 * Authenticated decryption
 */
int mbedtls_ccm_star_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                                   const unsigned char *iv, size_t iv_len,
                                   const unsigned char *add, size_t add_len,
                                   const unsigned char *input, unsigned char *output,
                                   const unsigned char *tag, size_t tag_len )
{
    int ret;
    unsigned char check_tag[16];
    unsigned char i;
    int diff;

    Debug_CCM_Info(("## FUNC: %s, alen=%u, plen=%u, ivlen=%u, tlen=%u\n", __FUNCTION__, add_len, length, iv_len, tag_len));
    CCM_VALIDATE_RET( ctx != NULL );
    CCM_VALIDATE_RET( iv != NULL );
    CCM_VALIDATE_RET( add_len == 0 || add != NULL );
    CCM_VALIDATE_RET( length == 0 || input != NULL );
    CCM_VALIDATE_RET( length == 0 || output != NULL );
    CCM_VALIDATE_RET( tag_len == 0 || tag != NULL );

    if( ( ret = ccm_auth_crypt( ctx, CCM_DECRYPT, length,
                                iv, iv_len, add, add_len,
                                input, output, check_tag, tag_len ) ) != 0 )
    {
        return( ret );
    }

    /* Check tag in "constant-time" */
    for( diff = 0, i = 0; i < tag_len; i++ )
        diff |= tag[i] ^ check_tag[i];

    if( diff != 0 )
    {
        mbedtls_platform_zeroize( output, length );
        return( MBEDTLS_ERR_CCM_AUTH_FAILED );
    }

    return( 0 );
}

int mbedtls_ccm_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                              const unsigned char *iv, size_t iv_len,
                              const unsigned char *add, size_t add_len,
                              const unsigned char *input, unsigned char *output,
                              const unsigned char *tag, size_t tag_len )
{
    Debug_CCM_Info(("## FUNC: %s, alen=%u, plen=%u, ivlen=%u, tlen=%u\n", __FUNCTION__, add_len, length, iv_len, tag_len));
    CCM_VALIDATE_RET( ctx != NULL );
    CCM_VALIDATE_RET( iv != NULL );
    CCM_VALIDATE_RET( add_len == 0 || add != NULL );
    CCM_VALIDATE_RET( length == 0 || input != NULL );
    CCM_VALIDATE_RET( length == 0 || output != NULL );
    CCM_VALIDATE_RET( tag_len == 0 || tag != NULL );

    if( tag_len == 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    return( mbedtls_ccm_star_auth_decrypt( ctx, length, iv, iv_len, add,
                                           add_len, input, output, tag, tag_len ) );
}
#endif /* MBEDTLS_CCM_ALT */
#endif /* MBEDTLS_CCM_C */
