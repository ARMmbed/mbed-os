/*
 *  FIPS-197 compliant AES implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Portions Copyright (c) 2016 - 2017 Analog Devices, Inc.
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
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */   
 
#include "mbedtls/aes.h"

#if defined(MBEDTLS_AES_ALT)

#include "adi_crypto.h"
#include "mbed_assert.h" 


/* CRYPTO Device number */
#define CRYPTO_DEV_NUM               (0u)

/* Memory Required for crypto driver */
static uint32_t DeviceMemory[(ADI_CRYPTO_MEMORY_SIZE+3)/4];

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v; while( n-- ) *p++ = 0;
}


void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    ADI_CRYPTO_HANDLE       hDevice;  
    ADI_CRYPTO_TRANSACTION  *pBuffer = ctx->pBuffer;
    
#ifdef CRYPTO_ENABLE_CALLBACK    
    CALLBACK_PARAMETERS     *pCBParam = ctx->pCBParam;    
    void *      	    pfCryptoCallback = ctx->pfCryptoCallback;
#endif    
    
    MBED_ASSERT(ctx !=  NULL);
    
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
    memset( DeviceMemory, 0, sizeof( DeviceMemory ) );
    memset( pBuffer, 0, sizeof( ADI_CRYPTO_TRANSACTION ) );
    
#ifdef CRYPTO_ENABLE_CALLBACK
    memset( pCBParam, 0, sizeof(  CALLBACK_PARAMETERS ) ); 
#endif        
  
    /* Open the crypto device */
    adi_crypto_Open(CRYPTO_DEV_NUM, DeviceMemory, sizeof(DeviceMemory), &hDevice);
 
    ctx->hDevice    = hDevice;
    ctx->pBuffer    = pBuffer;
    
#ifdef CRYPTO_ENABLE_CALLBACK    
    ctx->pCBParam           = pCBParam;    
    
    /* Register Callback */
    adi_crypto_RegisterCallback (hDevice, (ADI_CALLBACK const)pfCryptoCallback, pCBParam);
#endif    

    return;    
}

/*
 * AES key schedule (encryption)
 */
#if defined(MBEDTLS_AES_SETKEY_ENC_ALT)
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits )
{	
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;

    pBuffer->pKey = (uint8_t *)key;
    pBuffer->eAesKeyLen = keybits;      

    return( 0 );
}
#endif /*  MBEDTLS_AES_SETKEY_ENC_ALT */

/*
 * AES-ECB block encryption
 */
#if defined(MBEDTLS_AES_ENCRYPT_ALT)
void mbedtls_aes_encrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{
    ADI_CRYPTO_HANDLE      hDevice = ctx->hDevice; 
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;
    
/* If (Callback mode enabled) */
#ifdef CRYPTO_ENABLE_CALLBACK    
    CALLBACK_PARAMETERS    *pCBParam = ctx->pCBParam;    
#else    
    static volatile ADI_CRYPTO_TRANSACTION *pGottenBuffer;
#endif /* CRYPTO_ENABLE_CALLBACK */            
    
    pBuffer->eCipherMode    = ADI_CRYPTO_MODE_ECB;
    pBuffer->eCodingMode    = ADI_CRYPTO_ENCODE;
    pBuffer->pInputData     = (uint32_t*)input;
    pBuffer->pOutputData    = (uint32_t*)output;
    
    /* Submit the buffer for encryption */
    adi_crypto_SubmitBuffer (hDevice, pBuffer);    

#ifdef CRYPTO_ENABLE_CALLBACK
    /* reset callback counter */
    pCBParam->numBuffersReturned = 0;
#endif

    /* Enable the device */
    adi_crypto_Enable (hDevice, true);

#ifdef CRYPTO_ENABLE_CALLBACK
    /* await any callback */
    while (pCBParam->numBuffersReturned == 0)
        ;
    MBED_ASSERT(pBuffer == pCBParam->pcbReturnedBuffer);      
#else
    /* Get ECB ComputedCipher */
    adi_crypto_GetBuffer (hDevice, (ADI_CRYPTO_TRANSACTION ** const)&pGottenBuffer);  
    MBED_ASSERT(pBuffer == pGottenBuffer);  
#endif

    /* Disable the device */
    adi_crypto_Enable (hDevice, false);
}
#endif /* !  */


void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;
        
    adi_crypto_Close (ctx->hDevice);
    mbedtls_zeroize( ctx, sizeof( mbedtls_aes_context ) );    
}

/*
 * AES key schedule (decryption)
 */
#if defined(MBEDTLS_AES_SETKEY_DEC_ALT)
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits )
{
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;

    pBuffer->pKey = (uint8_t *)key; 
    pBuffer->eAesKeyLen = keybits;           

    return( 0 );
} 
#endif  // defined(MBEDTLS_AES_SETKEY_DEC_ALT)     

/*
 * AES-ECB block decryption
 */
#if defined(MBEDTLS_AES_DECRYPT_ALT)
void mbedtls_aes_decrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )                         
{
    ADI_CRYPTO_HANDLE      hDevice = ctx->hDevice; 
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;
    
/* If (Callback mode enabled) */    
#ifdef CRYPTO_ENABLE_CALLBACK     
    CALLBACK_PARAMETERS    *pCBParam = ctx->pCBParam;   
#else    
    static volatile ADI_CRYPTO_TRANSACTION *pGottenBuffer;
#endif /* CRYPTO_ENABLE_CALLBACK */   

    /* switch buffers for decode */
    pBuffer->eCipherMode    = ADI_CRYPTO_MODE_ECB;
    pBuffer->eCodingMode    = ADI_CRYPTO_DECODE;
    pBuffer->pInputData     = (uint32_t*)input;
    pBuffer->pOutputData    = (uint32_t*)output;    
    
    /* Submit the buffer for decryption */
    adi_crypto_SubmitBuffer (hDevice, pBuffer);
    
#ifdef CRYPTO_ENABLE_CALLBACK    
    /* reset callback counter */
    pCBParam->numBuffersReturned = 0;
#endif

    /* Enable the device */
    adi_crypto_Enable (hDevice, true);

#ifdef CRYPTO_ENABLE_CALLBACK
    /* await any callback */
    while (pCBParam->numBuffersReturned == 0)
        ;
    MBED_ASSERT(pBuffer == pCBParam->pcbReturnedBuffer);
#else    
    /* Get ECB plaintext */
    adi_crypto_GetBuffer (hDevice, (ADI_CRYPTO_TRANSACTION ** const)&pGottenBuffer);
    MBED_ASSERT(pBuffer == pGottenBuffer); 
#endif

    /* Disable the device */
    adi_crypto_Enable (hDevice, false);    

}
#endif  // defined(MBEDTLS_AES_DECRYPT_ALT)    


/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )                         
{
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
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output )
{  
    ADI_CRYPTO_HANDLE      hDevice  = ctx->hDevice; 
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;                                             
    
/* If (Callback mode enabled) */    
#ifdef CRYPTO_ENABLE_CALLBACK    
    CALLBACK_PARAMETERS    *pCBParam = ctx->pCBParam;
#else    
    static volatile ADI_CRYPTO_TRANSACTION *pGottenBuffer;
#endif /* CRYPTO_ENABLE_CALLBACK */                  
   
    if( mode == MBEDTLS_AES_ENCRYPT)
    {                                                                                                                                                           
        pBuffer->eCipherMode    = ADI_CRYPTO_MODE_CBC;                                                         
        pBuffer->pNonceIV       = (uint8_t *) iv;                                     
        pBuffer->eCodingMode    = ADI_CRYPTO_ENCODE;                                                                                                                        
        pBuffer->pInputData     = (uint32_t *)input;                                             
        pBuffer->numInputBytes  = length;                                                       
        pBuffer->pOutputData    = (uint32_t *)output;                               
        pBuffer->numOutputBytes = length;                                                    
                                                                                                             
        /* Submit the buffer for encryption */                                                               
        adi_crypto_SubmitBuffer (hDevice, pBuffer);                                                     
                                                                                                             
#ifdef CRYPTO_ENABLE_CALLBACK                                                                            
        /* reset callback counter */                                                                         
        pCBParam->numBuffersReturned = 0;                                                                              
#endif                                                                                                   
                                                                                                             
        /* Enable the device */                                                                              
        adi_crypto_Enable (hDevice, true);                                                                               
                                                                                                             
#ifdef CRYPTO_ENABLE_CALLBACK                                                                           
        /* await any callback */                                                                          
         while (pCBParam->numBuffersReturned == 0)                                                                   
             ;                                                                                             
        MBED_ASSERT(pBuffer == pCBParam->pcbReturnedBuffer);                                                      
#else           
        /* Get ECB ComputedCipher */                                                                         
        adi_crypto_GetBuffer (hDevice, (ADI_CRYPTO_TRANSACTION ** const)&pGottenBuffer);                                           
        MBED_ASSERT(pBuffer == pGottenBuffer);                                                                                                                                                                
#endif
                                                                                                   
        /* Disable the device */                                                                   
        adi_crypto_Enable (hDevice, false);                                                        
    }
    else    // MBEDTLS_AES_ENCRYPT
    {               
        /* switch buffers for decode */ 
        pBuffer->eCipherMode    = ADI_CRYPTO_MODE_CBC;                                                                              
        pBuffer->eCodingMode    = ADI_CRYPTO_DECODE;                                                               
        pBuffer->pInputData     = (uint32_t *)input;                                                
        pBuffer->pOutputData    = (uint32_t *)output;                                                 
                                                                                                                 
        /* Submit the buffer for decryption */                                                                   
        adi_crypto_SubmitBuffer (hDevice, pBuffer);                                                                    
                                                                                                                 
#ifdef CRYPTO_ENABLE_CALLBACK                                                                                
        /* reset callback counter */                                                                             
        pCBParam->numBuffersReturned = 0;                                                                              
#endif                                                                                                       
                                                                                                                 
        /* Enable the device */                                                                                  
        adi_crypto_Enable (hDevice, true);                                                                                 
                                                                                                                 
#ifdef CRYPTO_ENABLE_CALLBACK                                                                               
        /* await any callback */                                                                                 
        while (pCBParam->numBuffersReturned == 0)                                                                          
            ;                                                                                                    
        MBED_ASSERT(pBuffer == pCBParam->pcbReturnedBuffer);                                                             
#else
        /* Get ECB plaintext */                                                                                  
        adi_crypto_GetBuffer (hDevice, (ADI_CRYPTO_TRANSACTION ** const) &pGottenBuffer);                                           
        MBED_ASSERT(pBuffer == pGottenBuffer);                                                                        
#endif                                                                                                       
                                                                                                                 
        /* Disable the device */                                                                                 
        adi_crypto_Enable (hDevice, false);                                                                                                                                                                                                                  
    }                                                                                                            
    return( 0 );    
}

#endif /* MBEDTLS_CIPHER_MODE_CBC */    

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    ADI_CRYPTO_HANDLE      hDevice  = ctx->hDevice; 
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;                                             
    int32_t n;
    /* If (Callback mode enabled) */    
#ifdef CRYPTO_ENABLE_CALLBACK    
    CALLBACK_PARAMETERS    *pCBParam = ctx->pCBParam;    
#else    
    static volatile ADI_CRYPTO_TRANSACTION *pGottenBuffer;
#endif /* CRYPTO_ENABLE_CALLBACK */

    if(pBuffer->eCodingMode == ADI_CRYPTO_ENCODE)
    {    
    pBuffer->eCipherMode      = ADI_CRYPTO_MODE_CTR;
    pBuffer->pNonceIV         = (uint8_t*)&nonce_counter[0];
    
    pBuffer->pInputData     = (uint32_t*)input;
    pBuffer->numInputBytes  = length;
    pBuffer->pOutputData    = (uint32_t*)output;
    pBuffer->numOutputBytes = length;

    /* Submit the buffer for encryption */
    adi_crypto_SubmitBuffer (hDevice, pBuffer);

#ifdef CRYPTO_ENABLE_CALLBACK
    /* reset callback counter */
    pCBParam->numBuffersReturned = 0;
#endif

    /* Enable the device */
    adi_crypto_Enable (hDevice, true);

#ifdef CRYPTO_ENABLE_CALLBACK
    /* await any callback */
    while (pCBParam->numBuffersReturned == 0)
        ;
    MBED_ASSERT(pBuffer == pCBParam->pcbReturnedBuffer);
#else
     /* Get ECB ciphertext */
    adi_crypto_GetBuffer (hDevice, &pGottenBuffer);
    MBED_ASSERT(pBuffer != pGottenBuffer);
#endif    
            
    /* Disable the device */
    adi_crypto_Enable (hDevice, false);
    }
    else    // ADI_CRYPTO_ENCODE
    {
    /* switch buffers for decode */
    pBuffer->eCodingMode    = ADI_CRYPTO_DECODE;
    pBuffer->pInputData     = (uint32_t*)input;
    pBuffer->pOutputData    = (uint32_t*)output;

    /* Submit the buffer for decryption */
    adi_crypto_SubmitBuffer (hDevice, pBuffer);

#ifdef CRYPTO_ENABLE_CALLBACK
    /* reset callback counter */
    pCBParam->numBuffersReturned = 0;
#endif

    /* Enable the device */
    adi_crypto_Enable (hDevice, true);

#ifdef CRYPTO_ENABLE_CALLBACK
    /* await any callback */
    while (pCBParam->numBuffersReturned == 0)
        ;
    MBED_ASSERT(pBuffer == pCBParam->pcbReturnedBuffer);
#else
    /* Get CTR plaintext */
    adi_crypto_GetBuffer (hDevice, &pGottenBuffer);
    MBED_ASSERT(pBuffer == pGottenBuffer);
#endif

    /* Disable the device */
    adi_crypto_Enable (hDevice, false);
    
    for(n = 0; n < 16; n++) {
        stream_block[n] = input[n] ^ output[n];
    }
    }
    
    return(0);
}  
#endif // MBEDTLS_CIPHER_MODE_CTR  

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
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

    if( mode == MBEDTLS_AES_DECRYPT )
    {
        while( length-- )
        {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

            c = *input++;
            *output++ = (unsigned char)( c ^ iv[n] );
            iv[n] = (unsigned char) c;

            n = ( n + 1 ) & 0x0F;
        }
    }
    else
    {
        while( length-- )
        {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

            iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );

            n = ( n + 1 ) & 0x0F;
        }
    }

    *iv_off = n;
    
    //adi_crypto_Enable (ctx->hDevice, false);

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

    while( length-- )
    {
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

#endif /* !MBEDTLS_AES_ALT */
