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

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    ADI_CRYPTO_HANDLE       hDevice;  
    ADI_CRYPTO_TRANSACTION  *pBuffer = ctx->pBuffer;
    
#ifdef CRYPTO_ENABLE_CALLBACK    
    CALLBACK_PARAMETERS     *pCBParam = ctx->pCBParam;    
    void                    *pfCryptoCallback = ctx->pfCryptoCallback;
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
    ctx->pCBParam   = pCBParam;    
    ctx->pfCryptoCallback = pfCryptoCallback;
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

    pBuffer->pKey = key;      

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
    
#ifdef CRYPTO_ENABLE_CALLBACK    
    CALLBACK_PARAMETERS    *pCBParam = ctx->pCBParam;    
    ADI_CALLBACK           cbf =  ctx->pfCryptoCallback;
#endif    
    
/* IF (Callback mode enabled) */
#ifdef CRYPTO_ENABLE_CALLBACK
    /* Register Callback */
    adi_crypto_RegisterCallback (hDevice, cbf, pCBParam);
#else    
    static volatile ADI_CRYPTO_TRANSACTION *pGottenBuffer;
#endif /* CRYPTO_ENABLE_CALLBACK */

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
    adi_crypto_GetBuffer (hDevice, &pGottenBuffer);  
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

    mbedtls_zeroize( ctx, sizeof( mbedtls_aes_context ) );
    adi_crypto_Close (ctx->hDevice);
}

/*
 * AES key schedule (decryption)
 */
#if defined(MBEDTLS_AES_SETKEY_DEC_ALT)
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits )
{
    ADI_CRYPTO_TRANSACTION *pBuffer = ctx->pBuffer;

    pBuffer->pKey = key;      

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
#ifdef CRYPTO_ENABLE_CALLBACK     
    CALLBACK_PARAMETERS    *pCBParam = ctx->pCBParam;   
    ADI_CALLBACK           cbf =  ctx->pfCryptoCallback;
#endif 

/* IF (Callback mode enabled) */
#ifdef CRYPTO_ENABLE_CALLBACK
    /* Register Callback */
    adi_crypto_RegisterCallback (hDevice, cbf, pCBParam);
#else    
    static volatile ADI_CRYPTO_TRANSACTION *pGottenBuffer;
#endif /* CRYPTO_ENABLE_CALLBACK */   
    
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
    /* Get ECB plaintext */
    adi_crypto_GetBuffer (hDevice, &pGottenBuffer);
    MBED_ASSERT(pBuffer == pGottenBuffer); 
#endif

    /* Disable the device */
    adi_crypto_Enable (hDevice, false);    

    /* Close the crypto device */
    adi_crypto_Close(hDevice);
    
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

#endif /* !MBEDTLS_AES_ALT */
