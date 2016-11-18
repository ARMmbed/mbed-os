/*
 *  Authenticated encryption algorithms implementation using CRYPTO hw module
 *
 *  Copyright (C) 2016 Silicon Labs, http://www.silabs.com
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
 */

#include "em_device.h"

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "ccm.h"
#include "cmac.h"
#include "aesdrv_internal.h"
#include "aesdrv_common_crypto.h"
#include "aesdrv_authencr.h"
#include "cryptodrv_internal.h"
#include "em_crypto.h"
#include "em_assert.h"
#include <string.h>

/*******************************************************************************
 ***************************   STATIC FUNCTIONS  *******************************
 ******************************************************************************/

/* CMAC specific functions. */
static inline Ecode_t aesdrv_CMAC_Finalize( AESDRV_Context_t* pAesdrvContext,
                                            bool              encrypt,
                                            uint8_t*          digest,
                                            uint16_t          digestLengthBits);

/* CCM specific functions. */
static        void aesdrv_CCM_HeaderProcess(AESDRV_Context_t* pAesdrvContext,
                                            uint8_t*          pData,
                                            uint32_t          la
                                            );
static inline void aesdrv_CCM_Prepare(      AESDRV_Context_t* pAesdrvContext,
                                            const uint8_t*    pKey
                                            );
static inline void aesdrv_CCM_NoncePrepare( AESDRV_Context_t* pAesdrvContext,
                                            uint8_t const*    pNonce,
                                            uint8_t           authTagLength,
                                            uint32_t          lm,
                                            uint16_t          la
                                            );
static inline void    aesdrv_CCM_SeqSet(    AESDRV_Context_t* pAesdrvContext,
                                            const uint8_t     authTagLength,
                                            const bool        encrypt
                                            );
static inline Ecode_t aesdrv_CCM_Execute(   AESDRV_Context_t* pAesdrvContext,
                                            const uint8_t*    pHdr,
                                            const uint8_t*    pDataInput,
                                            uint8_t*          pDataOutput,
                                            const uint8_t     authTagLength,
                                            uint32_t          la,
                                            uint32_t          lm
                                            );
/* GCM specific functions. */
static        void    aesdrv_GCM_Prepare(   AESDRV_Context_t* pAesdrvContext,
                                            const uint8_t*    pKey,
                                            uint8_t*          pInitialVector
                                            );
static        void    aesdrv_GCM_Finalize(  AESDRV_Context_t* pAesdrvContext,
                                            unsigned int      authDataLength,
                                            unsigned int      plaintextLength
                                            );
static inline void    aesdrv_GCM_SeqSet(    AESDRV_Context_t* pAesdrvContext,
                                            const bool        encrypt
                                            );
static        void    aesdrv_GCM_Execute(   AESDRV_Context_t* pAesdrvContext,
                                            const uint8_t*    pDataInput,
                                            uint8_t*          pDataOutput,
                                            uint16_t          dataLength,
                                            const uint8_t*    pHdr,
                                            uint32_t          hdrLength);

/* Functions used by CCM and GCM. */
static inline Ecode_t aesdrvAuthTagHandle(  AESDRV_Context_t* pAesdrvContext,
                                            uint8_t*          pAuthTag,
                                            uint8_t           authTagLength,
                                            bool              encrypt
                                            );
static        void    aesdrvAuthTagRead(    AESDRV_Context_t* pAesdrvContext,
                                            uint8_t*          pAuthTag,
                                            uint8_t           authTagLength
                                            );
static        Ecode_t aesdrvAuthTagCompare( AESDRV_Context_t* pAesdrvContext,
                                            uint8_t*          pAuthTag,
                                            uint8_t           authTagLength
                                            );
static        void    aesdrvDataLoad(       CRYPTO_TypeDef* crypto,
                                            uint8_t*        pData,
                                            uint16_t        length
                                            );
static        void    aesdrvDataLoadStore(  CRYPTO_TypeDef* crypto,
                                            const uint8_t*  pDataInput,
                                            uint8_t*        pDataOutput,
                                            uint16_t        length
                                            );
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
/* Asynch callbacks. */
static        void aesdrv_XCM_AsynchCallback( void*   asynchCallbackArgument );
static        void aesdrv_CMAC_AsynchCallback( void* asynchCallbackArgument );
static        void aesdrvDataLoadAsynch( CRYPTO_TypeDef* crypto,
                                         const uint8_t*  pData,
                                         uint16_t        length
                                         );
static        void aesdrvDataStoreAsynch( AESDRV_Context_t* pAesdrvContext );
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */


/*******************************************************************************
 ********************************   MACROS  ************************************
 ******************************************************************************/

#define GCM_BLOCKSIZE                      (16)

/** Patterns which keep repeating in sequencer codes for CCM and GCM are
 * extracted into small procedures in order to make code more readable.
 */

/** CBC tag calculcation procedure.
 *
 * Procedure is executed with following assumptions:
 * - data to be processed is in DATA0
 * - DATA2 contains current tag
 * - result is stored in DATA2
 */
#define AESDRV_CBC_PROC           \
    CRYPTO_CMD_INSTR_DATA2TODATA0XOR,\
    CRYPTO_CMD_INSTR_AESENC,         \
    CRYPTO_CMD_INSTR_DATA0TODATA2

/** Authentication tag encrypt (CCM) procedure.
 *
 * Procedure is executed with following assumptions:
 * - DATA1 contains counter
 * - DATA2 contains tag to be encrypted
 * - result is stored in DATA0
 */
#define AESDRV_CCMTAG_ENCRYPT_PROC \
    CRYPTO_CMD_INSTR_DATA1INCCLR,     \
    CRYPTO_CMD_INSTR_DATA1TODATA0,    \
    CRYPTO_CMD_INSTR_AESENC,          \
    CRYPTO_CMD_INSTR_DATA2TODATA0XOR

/** CTR block with preincrementation.
 *
 * Procedure is executed with following assumptions:
 * - DATA1 contains counter
 * - result is stored in DATA0
 */
#define AESDRV_CTR_PREPARE_PROC  \
    CRYPTO_CMD_INSTR_DATA1INC,      \
    CRYPTO_CMD_INSTR_DATA1TODATA0,  \
    CRYPTO_CMD_INSTR_AESENC

/** CTR encryption procedure.
 *
 * Procedure is executed with following assumptions:
 * - data to be processed is in DATA0
 * - DATA1 contains counter which is preincremented
 * - DATA3 is used as temporary register
 * - result is stored in DATA0
 */
#define AESDRV_CTR_PROC           \
    CRYPTO_CMD_INSTR_DATA0TODATA3,   \
    AESDRV_CTR_PREPARE_PROC,      \
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR

/** GHASH procedure.
 *
 * Procedure is executed with following assumptions:
 * - data to be processed is in DATA0 ( A[i] or C[i] )
 * - temporary GHASH is stored in DDATA0
 * - H is stored in DATA2 (part of DDATA3)
 * - DDATA1 is used as temporary register
 *
 */
#define AESDRV_GHASH_PROC         \
    CRYPTO_CMD_INSTR_SELDDATA0DDATA2,\
    CRYPTO_CMD_INSTR_XOR,            \
    CRYPTO_CMD_INSTR_BBSWAP128,      \
    CRYPTO_CMD_INSTR_DDATA0TODDATA1, \
    CRYPTO_CMD_INSTR_SELDDATA0DDATA3,\
    CRYPTO_CMD_INSTR_MMUL,           \
    CRYPTO_CMD_INSTR_BBSWAP128

/** Sequencer code for CCM authentication and encryption. There are 3 variants
 * of the code: MCU,BUFC and DMA.
 */
static const CRYPTO_InstructionSequence_TypeDef ccmAuthEncr[] = {
  {
    /* MCU */
    CRYPTO_CMD_INSTR_EXECIFA,

    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,
    CRYPTO_CMD_INSTR_DATA0TODATA3,

    AESDRV_CBC_PROC,

    AESDRV_CTR_PREPARE_PROC,

    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
  },
  {
    /* BUFC */
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA3,

    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,

    AESDRV_CTR_PREPARE_PROC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF,

    CRYPTO_CMD_INSTR_EXECIFLAST,
    AESDRV_CCMTAG_ENCRYPT_PROC
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA0TODATA3,

    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PREPARE_PROC,

    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0,

    CRYPTO_CMD_INSTR_EXECIFLAST,
    AESDRV_CCMTAG_ENCRYPT_PROC
  }
#endif
};

/** Sequencer code for CCM decryption and authentication validation. There are
 * 3 variants of the code: MCU,BUFC and DMA.
 */
static const CRYPTO_InstructionSequence_TypeDef ccmAuthDecr[] = {
  {
    /* MCU */
    CRYPTO_CMD_INSTR_EXECIFA,
    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,
    CRYPTO_CMD_INSTR_DDATA3TODDATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    AESDRV_CTR_PREPARE_PROC,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_DATA3TODATA0,
    CRYPTO_CMD_INSTR_DATA2TODATA0XORLEN,
    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_DATA3TODATA0
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PREPARE_PROC,
    
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA3TODATA0XORLEN,
    CRYPTO_CMD_INSTR_DATA0TOBUF,

    CRYPTO_CMD_INSTR_EXECIFA,
    CRYPTO_CMD_INSTR_BUFTODATA0,

    CRYPTO_CMD_INSTR_EXECALWAYS,
    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_EXECIFLAST,
    AESDRV_CCMTAG_ENCRYPT_PROC
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PREPARE_PROC,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DMA0TODATA, /* 5*/
    CRYPTO_CMD_INSTR_DATA3TODATA0XORLEN,
    CRYPTO_CMD_INSTR_DATATODMA0,

    CRYPTO_CMD_INSTR_EXECIFA,
    CRYPTO_CMD_INSTR_DMA0TODATA,

    CRYPTO_CMD_INSTR_EXECALWAYS, /* 10 */
    AESDRV_CBC_PROC,

    CRYPTO_CMD_INSTR_EXECIFLAST,
    AESDRV_CCMTAG_ENCRYPT_PROC
  }
#endif
};

/** Sequencer code for CCM encryption or decryption only (no authentication).
 * There are 3 variants of the code: MCU,BUFC and DMA.
 */
static const CRYPTO_InstructionSequence_TypeDef ccmEncrDecr[] =
{
  {
    /* MCU */
    CRYPTO_CMD_INSTR_EXECIFB,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    AESDRV_CTR_PREPARE_PROC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_EXECIFB,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    AESDRV_CTR_PREPARE_PROC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_EXECIFB,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    AESDRV_CTR_PREPARE_PROC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0
  }
#endif
};

/** Sequencer code for GCM encryption and authentication. There are
 * 3 variants of the code: MCU,BUFC and DMA.
 */
static const CRYPTO_InstructionSequence_TypeDef gcmAuthEncr[] =
{
  {
    /* MCU */
    CRYPTO_CMD_INSTR_EXECIFB,

    AESDRV_CTR_PROC,

    CRYPTO_CMD_INSTR_EXECALWAYS,

    AESDRV_GHASH_PROC

  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,

    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PROC,
    CRYPTO_CMD_INSTR_DATA0TOBUF,      /* Store Ciphertext */

    CRYPTO_CMD_INSTR_EXECALWAYS,
    AESDRV_GHASH_PROC
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,

    CRYPTO_CMD_INSTR_EXECIFB,

    AESDRV_CTR_PROC,
    CRYPTO_CMD_INSTR_DATATODMA0,      /* Store Ciphertext */

    CRYPTO_CMD_INSTR_EXECALWAYS,
    AESDRV_GHASH_PROC
  }
#endif
};
/** Sequencer code for GCM decryption and authentication validation. There are
 * 3 variants of the code: MCU,BUFC and DMA.
 */
static const CRYPTO_InstructionSequence_TypeDef gcmAuthDecr[] =
{
  {
    /* MCU */
    AESDRV_GHASH_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PROC
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    AESDRV_GHASH_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PROC,
    CRYPTO_CMD_INSTR_DATA0TOBUF      /* Store Ciphertext */
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    AESDRV_GHASH_PROC,

    CRYPTO_CMD_INSTR_EXECIFB,
    AESDRV_CTR_PROC,
    CRYPTO_CMD_INSTR_DATATODMA0      /* Store Ciphertext */
  }
#endif
};

/* const_rb should be 0x87, but implementation requires bit- and byteswapped
   constant */
static const uint32_t cmac_const_rb[4] = {0x00000000, 0x00000000,
                                          0x00000000, 0xe1000000};

/*******************************************************************************
 ***************************   GLOBAL FUNCTIONS  *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Generalized, internal CCM function supporting both CCM and CCM*.
 ******************************************************************************/
Ecode_t AESDRV_CCM_Generalized(AESDRV_Context_t* pAesdrvContext,
                               const uint8_t*    pDataInput,
                                     uint8_t*    pDataOutput,
                               const uint32_t    dataLength,
                               const uint8_t*    pHdr,
                               const uint32_t    hdrLength,
                               const uint8_t*    pKey,
                               const uint32_t    keyLength,
                               const uint8_t*    pNonce,
                               const uint32_t    nonceLength,
                                     uint8_t*    pAuthTag,
                               const uint8_t     authTagLength,
                               const bool        encrypt,
                               const bool        encryptedPayload)
{
  uint32_t lm;
  uint32_t la;
  Ecode_t status, retval;
  CRYPTODRV_Context_t* pCryptodrvContext =
    &pAesdrvContext->cryptodrvContext;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_CCM_AsynchContext_t* pAsynchContext =
    (AESDRV_CCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;
  AESDRV_AsynchCallback_t asynchCallback =
    pAsynchContext ? pAsynchContext->asynchCallback : 0;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
  if ( (keyLength != 128/8) ||
       (nonceLength != 13) ||
       (dataLength > CCM_MAX_DATA_LENGTH) )
  {
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  }
  
  if (encryptedPayload)
  {
    la=hdrLength;
    lm=dataLength;
  }
  else
  {
    la=hdrLength+dataLength;
    lm=0;
  }

  if ((0==authTagLength) && (0==lm))
  {
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    if (asynchCallback)
    {
      asynchCallback(ECODE_OK, pAsynchContext->asynchCallbackArgument);
    }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
    return ECODE_OK;
  }

  status = CRYPTODRV_Arbitrate(pCryptodrvContext);
  if (ECODE_OK != status)
    return status;

  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

  /* Enabling CRYPTO clock, initial CRYPTO configuration and key loading.*/
  aesdrv_CCM_Prepare(pAesdrvContext, pKey);

  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

  /* Set initial value for CBC (DATA2) and CTR (DATA1) */
  aesdrv_CCM_NoncePrepare(pAesdrvContext, pNonce, authTagLength, lm, la);

  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  
  /* Load appropriate instruction code to CRYPTO sequencer. */
  aesdrv_CCM_SeqSet(pAesdrvContext, authTagLength, encrypt);

  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  pAesdrvContext->cipherMode    = cipherModeCcm;
    
  if (pAsynchContext && (la || lm))
  {
    pAsynchContext->pAuthTag      = pAuthTag;
    pAsynchContext->encrypt       = encrypt;
    pAsynchContext->authTagLength = authTagLength;
    CRYPTODRV_SetAsynchCallback (pCryptodrvContext,
                                 aesdrv_XCM_AsynchCallback, pAesdrvContext);
    /* Process data. */
    retval = aesdrv_CCM_Execute(pAesdrvContext,
                                pHdr, pDataInput, pDataOutput,
                                authTagLength, la, lm);
    return retval;
  }
  else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
    /* Process data. */
    retval = aesdrv_CCM_Execute(pAesdrvContext,
                                pHdr, pDataInput, pDataOutput,
                                authTagLength, la, lm);

    if (ECODE_OK == retval)
    {
      /* read(encrypt==true) or validate tag. */
      retval = aesdrvAuthTagHandle(pAesdrvContext,
                                    pAuthTag, authTagLength, encrypt);
      if (MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == retval)
      {
        memset(pDataOutput, 0, dataLength);
      }
    }
    CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

    status = CRYPTODRV_Release(pCryptodrvContext);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    if (pAsynchContext)
    {
      if (asynchCallback)
      {
        if (MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == retval)
        {
          retval = (Ecode_t)MBEDTLS_ERR_CCM_AUTH_FAILED;
        }
        asynchCallback(retval, pAsynchContext->asynchCallbackArgument);
        /* In asynch mode return OK, since status is returned in callback. */
        retval = ECODE_OK;
      }
    }
#endif
    
    return retval==ECODE_OK? status : retval;
  }
}

/**
 *   Function is an implementation of CCM optimized for BLE.
 *   Please refer to @ref aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CCMBLE(AESDRV_Context_t*   pAesdrvContext,
                      uint8_t*            pData,
                      const uint32_t      dataLength,
                      uint8_t             hdr,
                      const uint8_t*      pKey,
                      const uint8_t*      pNonce,
                            uint8_t*      pAuthTag,
                      const bool          encrypt)
{
  /* Local variables used to optimize load/store sequences from memory to
     crypto. We want to load all 4 32bit data words to local register
     variables in the first sequence, then store them all in the second
     sequence.*/
  register uint32_t    iv0;
  register uint32_t    iv1;
  register uint32_t    iv2;
  register uint32_t    iv3;
  Ecode_t              status, retval=ECODE_OK;
  CRYPTODRV_Context_t* pCryptodrvContext =
    &pAesdrvContext->cryptodrvContext;
  CRYPTO_TypeDef*      crypto = pCryptodrvContext->device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_CCM_AsynchContext_t* pAsynchContext =
    (AESDRV_CCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  if (pAesdrvContext->ioMode != aesdrvIoModeBufc)
  {
    return AESDRV_CCM_Generalized(pAesdrvContext,
                                  pData, pData, dataLength,
                                  &hdr, 1,
                                  pKey, 128/8, /* keyLength */
                                  pNonce, 13,
                                  (uint8_t*)pAuthTag, 4,
                                  encrypt, true);
  }
  else
  {
    status = CRYPTODRV_Arbitrate(pCryptodrvContext);
    if (ECODE_OK != status)
      return status;

    CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

    /* Setup CRYPTO for AES-128 mode (256 not supported) */
    crypto->CTRL      = CRYPTO_CTRL_AES_AES128;
    
    AESDRV_HwIoSetup(pAesdrvContext, pData, 0, 0);
    
    if (pKey)
    {
      CRYPTO_KeyBuf128Write(crypto, (uint32_t *)pKey);
    }
    /* Calculate Counter IV for encryption. */
    iv0 = 0x01 | (*(uint32_t *)(&pNonce[0]) << 8);
    iv1 = *(uint32_t *)(&pNonce[3]);
    iv2 = *(uint32_t *)(&pNonce[7]);
    iv3 = *(uint16_t *)(&pNonce[11]);
    /* Store Counter IV in crypto->DATA1 */
    crypto->DATA1 = iv0;
    crypto->DATA1 = iv1;
    crypto->DATA1 = iv2;
    crypto->DATA1 = iv3;
    
    /* Calculate CBC IV for authentication. */
    iv0 |= 0x49;
    iv3 |= __REV(dataLength);
    /* Store CBC IV in crypto->DATA0 */
    crypto->DATA0 = iv0;
    crypto->DATA0 = iv1;
    crypto->DATA0 = iv2;
    crypto->DATA0 = iv3;
    
    /* Store header in crypto->DATA3 */
    crypto->DATA3 = 0x0100 | (hdr << 16);
    crypto->DATA3 = 0;
    crypto->DATA3 = 0;
    crypto->DATA3 = 0;

    crypto->SEQCTRL  = dataLength;
    crypto->SEQCTRLB = 0;

    /* The following code is tested to run faster than using instruction
       sequences. */
    crypto->CMD = CRYPTO_CMD_INSTR_AESENC;
    crypto->CMD = CRYPTO_CMD_INSTR_DATA3TODATA0XOR;
    crypto->CMD = CRYPTO_CMD_INSTR_AESENC;
    crypto->CMD = CRYPTO_CMD_INSTR_DATA0TODATA3;

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    pAesdrvContext->cipherMode = cipherModeCcmBle;
      
    if (pAsynchContext)
    {
      pAsynchContext->pAuthTag = (uint8_t*)pAuthTag;
      pAsynchContext->encrypt  = encrypt;
      pAsynchContext->authTagLength = 4;
      pAsynchContext->encryptingHeader = false;
      pAsynchContext->la    = 0;
      pAsynchContext->lm    = 0;
      CRYPTODRV_SetAsynchCallback (pCryptodrvContext,
                                   aesdrv_XCM_AsynchCallback, pAesdrvContext);
    }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
    if (encrypt)
    {
      CRYPTO_EXECUTE_14(crypto,
                        CRYPTO_CMD_INSTR_EXECIFA,
                        CRYPTO_CMD_INSTR_BUFTODATA0,
                        CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
                        CRYPTO_CMD_INSTR_AESENC,
                        CRYPTO_CMD_INSTR_DATA0TODATA3,
                        CRYPTO_CMD_INSTR_DATA1INC,
                        CRYPTO_CMD_INSTR_DATA1TODATA0,
                        CRYPTO_CMD_INSTR_AESENC,
                        CRYPTO_CMD_INSTR_DATA0TOBUFXOR,
                        CRYPTO_CMD_INSTR_EXECIFLAST,
                        CRYPTO_CMD_INSTR_DATA1INCCLR,
                        CRYPTO_CMD_INSTR_DATA1TODATA0,
                        CRYPTO_CMD_INSTR_AESENC,
                        CRYPTO_CMD_INSTR_DATA3TODATA0XOR
                        );
      
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
      if (0==pAsynchContext)
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
      {
        uint32_t * _pAuthTag = (uint32_t *)pAuthTag;
        *_pAuthTag = crypto->DATA0;
        
        CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

        status = CRYPTODRV_Release(pCryptodrvContext);
      }
    }
    else
    {
      CRYPTO_EXECUTE_16(crypto,
                        CRYPTO_CMD_INSTR_EXECIFA,
                        /* AESDRV_CTR_PREPARE_PROC */
                        CRYPTO_CMD_INSTR_DATA1INC,
                        CRYPTO_CMD_INSTR_DATA1TODATA0,
                        CRYPTO_CMD_INSTR_AESENC,

                        CRYPTO_CMD_INSTR_BUFTODATA0XOR,
                        CRYPTO_CMD_INSTR_DATA0TOBUF,
                        
                        CRYPTO_CMD_INSTR_DATA0TODATA2,
                        CRYPTO_CMD_INSTR_DATA3TODATA0,
                        CRYPTO_CMD_INSTR_DATA2TODATA0XORLEN,
                        
                        CRYPTO_CMD_INSTR_AESENC,
                        CRYPTO_CMD_INSTR_DATA0TODATA3,
                        
                        CRYPTO_CMD_INSTR_EXECIFLAST,
                        CRYPTO_CMD_INSTR_DATA1INCCLR,
                        CRYPTO_CMD_INSTR_DATA1TODATA0,
                        CRYPTO_CMD_INSTR_AESENC,
                        CRYPTO_CMD_INSTR_DATA3TODATA0XOR
                        );

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
      if (0==pAsynchContext)
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
      {
        uint32_t * _pAuthTag = (uint32_t *)pAuthTag;
#if (CRYPTO_COUNT > 1)
        CRYPTO_InstructionSequenceWait(crypto);
#endif
        if (crypto->DATA0 != *_pAuthTag)
        {
          retval = MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED;
        }
        
        CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

        status = CRYPTODRV_Release(pCryptodrvContext);
      }
    }
  
    return retval==ECODE_OK? status : retval;
  }
}

/*
 *   GCM (Galois Counter Mode) block cipher mode encryption/decryption based
 *   on 128 bit AES.
 *   Please refer to @ref aesdrv.h for detailed description.
 */
Ecode_t AESDRV_GCM(AESDRV_Context_t* pAesdrvContext,
                   const uint8_t*    pDataInput,
                         uint8_t*    pDataOutput,
                   const uint32_t    dataLength,
                   const uint8_t*    pHdr,
                   const uint32_t    hdrLength,
                   const uint8_t*    pKey,
                   const uint32_t    keyLength,
                   const uint8_t*    pInitialVector,
                   const uint32_t    initialVectorLength,
                   uint8_t*          pAuthTag,
                   const uint8_t     authTagLength,
                   const bool        encrypt)
{
  Ecode_t             status, retval = ECODE_OK;
  CRYPTODRV_Context_t* pCryptodrvContext =
    &pAesdrvContext->cryptodrvContext;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_GCM_AsynchContext_t* pAsynchContext =
    (AESDRV_GCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;
  AESDRV_AsynchCallback_t asynchCallback =
    pAsynchContext ? pAsynchContext->asynchCallback : 0;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  if ( (keyLength != 128/8) || (initialVectorLength != 12) )
  {
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  }

  status = CRYPTODRV_Arbitrate(pCryptodrvContext);
  if (ECODE_OK != status)
    return status;

  /* Prepare for GCM loop: set registers to inital values */
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  aesdrv_GCM_Prepare(pAesdrvContext, pKey, (uint8_t*)pInitialVector);
  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

  /* Load appropriate sequencer code to CRYPTO sequencer. */
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  aesdrv_GCM_SeqSet(pAesdrvContext, encrypt);
  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  pAesdrvContext->cipherMode           = cipherModeGcm;
  
  if (pAsynchContext && (dataLength || hdrLength))
  {
    pAsynchContext->pAuthTag             = pAuthTag;
    pAsynchContext->encrypt              = encrypt;
    pAsynchContext->authTagLength        = authTagLength;
    CRYPTODRV_SetAsynchCallback (pCryptodrvContext,
                                 aesdrv_XCM_AsynchCallback, pAesdrvContext);

    /* Process data */
    aesdrv_GCM_Execute(pAesdrvContext,
                       pDataInput, pDataOutput, dataLength,
                       pHdr, hdrLength);
                                
  }
  else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
    /* Process data */
    aesdrv_GCM_Execute(pAesdrvContext,
                       pDataInput, pDataOutput, dataLength,
                       pHdr, hdrLength);

    /*
    ** Compute last part of the GHASH and authentication tag:
    **
    ** Xm+n+1 = (Xm+n XOR (len(A)|len(C))) * H
    **
    ** Compute the final authentication tag now.
    ** T = MSBt (GHASH(H, A, C) XOR E(K, Y0))
    ** where GHASH(H, A, C) is Xm+n+1 which is stored in DATA0 (LSWord of DDATA2)
    ** and
    ** E(K, Y0) is stored in DATA3.
    */
    aesdrv_GCM_Finalize(pAesdrvContext, hdrLength, dataLength);

    /* read(encrypt==true) or validate tag. */
    retval = aesdrvAuthTagHandle(pAesdrvContext,
                                  pAuthTag, authTagLength, encrypt);
    if (MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == retval)
    {
      memset(pDataOutput, 0, dataLength);
    }
    CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

    status = CRYPTODRV_Release(pCryptodrvContext);
    
    if (ECODE_OK == retval)
      retval = status;

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    if (asynchCallback)
    {
      asynchCallback(retval, pAsynchContext->asynchCallbackArgument);
    }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  }
  return retval;
}

/**
 * @brief
 *  Function is an implementation of CMAC-AES128
 * @details
 *  Function assumes fixed key length of 128bit, digest of max 128bit.
 *
 * @param[in] pAesdrvContext
 *  Pointer to CMAC context structure.
 *
 * @param[in] pData
 *  Pointer to data (message) Be careful: this memory should be allocated on
 *  block-size (128-bit) boundaries!
 *
 * @param[in] dataLengthBits
 *  length of actual data in bits
 *
 * @param[in] key
 *  Pointer to key buffer for the AES algorithm.
 *  Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] keyLength
 *   The length in bytes, of the @p pKey, i.e. the 'K' parameter in CCM.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in/out] digest
 *  128-bit (maximum) digest. If encrypting, the digest will be stored there.
 *  If verifying, the calculated digest will be compared to the one stored in
 *  this place.
 *  Warning: regardless of digestLengthBits, 128 bits will get written here.
 *
 * @param[in] digestLengthBits
 *  Requested length of the message digest in bits. LSB's will be zeroed out.
 *
 * @param[in] encrypt
 *  true - Generate hash
 *  false - Verify hash
 *
 * @return
 *   ECODE_OK if success. Error code if failure.
 *   Encryption will always succeed.
 *   Decryption may fail if the authentication fails.
 */
Ecode_t AESDRV_CMAC(AESDRV_Context_t* pAesdrvContext,
                    const uint8_t*    pData,
                    uint32_t          dataLengthBits,
                    const uint8_t*    key,
                    const uint32_t    keyLength,
                    uint8_t*          digest,
                    uint16_t          digestLengthBits,
                    const bool        encrypt
                    )
{
  uint32_t             i;
  uint32_t             subKey[4];
  uint32_t             lastBlock[4];
  uint8_t*             lastBlockBytePtr = (uint8_t *)lastBlock;
  uint32_t*            dataPointer = (uint32_t *)pData;
  uint8_t              bitsToPad;
  Ecode_t              status, retval = ECODE_OK;
  CRYPTODRV_Context_t* pCryptodrvContext =
    &pAesdrvContext->cryptodrvContext;
  CRYPTO_TypeDef*      crypto = pCryptodrvContext->device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_CMAC_AsynchContext_t* pAsynchContext =
    (AESDRV_CMAC_AsynchContext_t*) pAesdrvContext->pAsynchContext;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  /* Check input arguments */
  if (digestLengthBits > 128)
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  if (digestLengthBits == 0)
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  if (keyLength != 128/8)
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;

  status = CRYPTODRV_Arbitrate(pCryptodrvContext);
  if (ECODE_OK != status)
    return status;

  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  
  /* Setup CRYPTO for AES-128 mode (256 not supported) */
  crypto->CTRL     = CRYPTO_CTRL_AES_AES128;
  crypto->SEQCTRL  = 0;
  crypto->SEQCTRLB = 0;
  
  /* Calculate subkeys */
  /* magic value in DATA1 */
  CRYPTO_DataWrite(&crypto->DATA1, cmac_const_rb);
  if (key)
  {
    /* Key in KeyBuf */
    CRYPTO_KeyBufWrite(crypto, (uint32_t*)key, cryptoKey128Bits);
  }
  /* Zero out DATA0 */
  for(i = 0; i < 4; i++) crypto->DATA0 = 0x00;
  
  CRYPTO_EXECUTE_17(crypto,
                    CRYPTO_CMD_INSTR_SELDATA0DATA1,
                    CRYPTO_CMD_INSTR_AESENC,
                    CRYPTO_CMD_INSTR_BBSWAP128,
                    CRYPTO_CMD_INSTR_SELDDATA0DATA1,
                    CRYPTO_CMD_INSTR_SHR,
                    CRYPTO_CMD_INSTR_EXECIFCARRY,
                    CRYPTO_CMD_INSTR_XOR,
                    CRYPTO_CMD_INSTR_EXECALWAYS,
                    CRYPTO_CMD_INSTR_BBSWAP128,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3, // subkey1 in DATA2 
                    CRYPTO_CMD_INSTR_BBSWAP128,
                    CRYPTO_CMD_INSTR_SHR,
                    CRYPTO_CMD_INSTR_EXECIFCARRY,
                    CRYPTO_CMD_INSTR_XOR,
                    CRYPTO_CMD_INSTR_EXECALWAYS,
                    CRYPTO_CMD_INSTR_BBSWAP128,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2 // subkey2 in DATA0
                    );
  CRYPTO_InstructionSequenceWait(crypto);
  
  /* Prepare input message for algorithm */
  bitsToPad = 128 - (dataLengthBits % 128);
  
  /* Determine which subKey we're going to use */
  if(bitsToPad != 128 || dataLengthBits == 0)
  {
    /* Input is treated as last block being incomplete */
    /* So store SubKey 2 */
    CRYPTO_DataRead(&crypto->DATA0, subKey);
  }
  else
  {
    /* Input is treated as block aligned, so store SubKey 1 */
    CRYPTO_DataRead(&crypto->DATA2, subKey);
  }

  /* We can take a break from using CRYPTO here */
  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  
  /* Copy the last block of data into our local copy because we need
     to change it */
  if(dataLengthBits < 128)
  {
    for(i = 0; i < 4; i++)
    {
      lastBlock[i] = dataPointer[i];
    }
  }
  else
  {
    for(i = 0; i < 4; i++)
    {
      lastBlock[i] = dataPointer[((dataLengthBits - 1) / 128)*4 + i];
    }
  }
  
  if(bitsToPad != 128)
  {
    /* Input message needs to be padded */
    
    /* Apply first one bit */
    if((bitsToPad % 8) == 0)
    {
      lastBlockBytePtr[16-(bitsToPad/8)] = 0x80;
      dataLengthBits += 8;
      bitsToPad -= 8;
    }
    else
    {
      lastBlockBytePtr[16-(bitsToPad/8)] |= (1 << ((bitsToPad - 1) % 8));
      dataLengthBits += bitsToPad % 8;
      bitsToPad -= bitsToPad % 8;
    }
    
    /* Apply zero-padding until block boundary */
    while(bitsToPad > 0)
    {
      lastBlockBytePtr[16-(bitsToPad/8)] = 0x00;
      dataLengthBits += 8;
      bitsToPad -= 8;
    }
  }
  else
  {
    if (dataLengthBits == 0)
    {
      /* Clear out the data */
      for(i = 0; i < 4; i++)
      {
        lastBlock[i] = 0x00000000;
      }
      /* Pad */
      lastBlockBytePtr[0] = 0x80;
      dataLengthBits = 128;
    }
    else
    {
      /* Input message was block-aligned, so no padding required */
    }
  }
  /* Store the XOR-ed version of the last block separate from the message */
  /* to avoid contamination of the input data */
  for(i = 0; i < 4; i++)
  {
    lastBlock[i] ^= subKey[i];
  }
  
  /* Calculate hash */
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  if (pAsynchContext)
  {
    crypto->SEQCTRL |= 16;
  }
  else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    crypto->SEQCTRL |= dataLengthBits / 8;
#else
    crypto->SEQCTRL |= 16;
#endif
  }
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  crypto->CTRL |= CRYPTO_CTRL_DMA0RSEL_DATA0;
#endif
  if (key)
  {
    CRYPTO_KeyBufWrite(crypto, (uint32_t*)key, cryptoKey128Bits);
  }
  for(i = 0; i < 4; i++) crypto->DATA0 = 0x00;

#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  CRYPTO_SEQ_LOAD_2(crypto,
                    CRYPTO_CMD_INSTR_DMA0TODATAXOR,
                    CRYPTO_CMD_INSTR_AESENC);
#else
  CRYPTO_SEQ_LOAD_1(crypto,
                    CRYPTO_CMD_INSTR_AESENC);
#endif

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  if (pAsynchContext)
  {
    uint32_t dataBlocks                        = dataLengthBits/128;

    pAsynchContext->digest            = digest;
    pAsynchContext->digestLengthBits  = digestLengthBits;
    pAsynchContext->encrypt           = encrypt;
    memcpy (pAsynchContext->lastBlock, lastBlock, sizeof(lastBlock));
    
    CRYPTODRV_SetAsynchCallback (pCryptodrvContext,
                                 aesdrv_CMAC_AsynchCallback, pAesdrvContext);
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    CRYPTO_InstructionSequenceExecute(crypto);
#endif
    if (dataBlocks > 1)
    {
      pAsynchContext->dataBlocks      = dataBlocks - 1;
      pAsynchContext->dataPointer     = dataPointer + 4;
      CRYPTODRV_DataWriteUnaligned(&crypto->DATA0XOR, (uint8_t*)dataPointer);
    }
    else
    {
      pAsynchContext->dataBlocks      = 0;
      CRYPTO_DataWrite(&crypto->DATA0XOR, lastBlock);
    }
#if !defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    CRYPTO_InstructionSequenceExecute(crypto);
#endif
  }
  else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    CRYPTO_InstructionSequenceExecute(crypto);
#endif
    /* Push all blocks except the last one */
    for(i = 0; i < (dataLengthBits/128) - 1; i++)
    {
      CRYPTODRV_DataWriteUnaligned(&crypto->DATA0XOR, (uint8_t*)&(dataPointer[i*4]));
#if !defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
      CRYPTO_InstructionSequenceExecute(crypto);
#endif
    }
    /* Don't forget to push the last block as well! */
    CRYPTO_DataWrite(&crypto->DATA0XOR, lastBlock);
#if !defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    CRYPTO_InstructionSequenceExecute(crypto);
#endif  
    CRYPTO_InstructionSequenceWait(crypto);

    retval = aesdrv_CMAC_Finalize(pAesdrvContext,
                                  encrypt, digest, digestLengthBits);
    
    CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

    status = CRYPTODRV_Release(pCryptodrvContext);
  }
  
  return retval == ECODE_OK ? status : retval;
}

/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)

/***************************************************************************//**
 * @brief
 *   Callback function for asynchronous CCM and GCM operations.
 *
 * @param[in] asynchCallbackArgument
 *   Callback argument.
 ******************************************************************************/
static void aesdrv_XCM_AsynchCallback (void* asynchCallbackArgument)
{
  AESDRV_Context_t* pAesdrvContext = 
    (AESDRV_Context_t*) asynchCallbackArgument;

  if (pAesdrvContext)
  {
    CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;
    CRYPTO_TypeDef*      crypto            = pCryptodrvContext->device->crypto;
    AESDRV_CCM_AsynchContext_t* pAsynchContext =
      (AESDRV_CCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;

    int la = pAsynchContext->la;
    if (la && pAsynchContext->authTagLength)
    {
      aesdrvDataLoadAsynch(crypto, pAsynchContext->pHdr, la);
      if (la > 16)
      {
        pAsynchContext->la   -= 16;
        pAsynchContext->pHdr += 16;
      }
      else
      {
        pAsynchContext->la    = 0;
      }
      return;
    }
    else
    {
      int lm = pAsynchContext->lm;
      if (lm)
      {
        if (pAsynchContext->encryptingHeader)
        {
          /* We have just finished auth data handling mode, so should
             kick off the plaintext handling, and return. */
          pAsynchContext->encryptingHeader = false;
          crypto->SEQCTRL = 0;
          crypto->SEQCTRLB = lm > 16 ? 16 : lm;
          aesdrvDataLoadAsynch(crypto, pAsynchContext->pDataInput, lm);
          return;
        }

        aesdrvDataStoreAsynch(pAesdrvContext);

        if (lm > 16)
        {
          lm = pAsynchContext->lm;
          crypto->SEQCTRLB = lm > 16 ? 16 : lm;
          aesdrvDataLoadAsynch(crypto, pAsynchContext->pDataInput, lm);
          return;
        }
        else
        {
          lm = 0;
        }
      }

      if (lm == 0)
      {
        Ecode_t   status, retval;

        /* Turn off interrupts. */
        CRYPTODRV_SetAsynchCallback(pCryptodrvContext, 0, 0);
        
        /* Encrypt authentication tag. */
        switch (pAesdrvContext->cipherMode)
        {
          case cipherModeCcm:
            if (pAsynchContext->authTagLength)
            {
              crypto->SEQCTRL = 16;
              crypto->SEQCTRLB = 0;
              CRYPTO_EXECUTE_4(crypto,
                               CRYPTO_CMD_INSTR_DATA1INCCLR,
                               CRYPTO_CMD_INSTR_DATA1TODATA0,
                               CRYPTO_CMD_INSTR_AESENC,
                               CRYPTO_CMD_INSTR_DATA2TODATA0XOR);
              crypto->IFC = 3;
            }
            break;
          case cipherModeGcm:
            aesdrv_GCM_Finalize(pAesdrvContext,
                                pAsynchContext->hdrLength,
                                pAsynchContext->dataLength);
            break;
          case cipherModeCcmBle:
            break;
          case cipherModeCmac:
          case cipherModeBlockCipher:
          case cipherModeNone:
            /* Cipher mode should not be CMAC, block cipher or none.  */
            EFM_ASSERT (false);
            break;
        }

        /* read(encrypt==true) or validate tag. */
        status = aesdrvAuthTagHandle(pAesdrvContext,
                                      pAsynchContext->pAuthTag,
                                      pAsynchContext->authTagLength,
                                      pAsynchContext->encrypt);
        if (MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == status)
        {
          uint8_t *pDataStart = pAsynchContext->pDataOutput -
            (pAsynchContext->dataLength - (pAsynchContext->dataLength&0xf));
          memset(pDataStart, 0, pAsynchContext->dataLength);
          status = (Ecode_t)MBEDTLS_ERR_CCM_AUTH_FAILED;
        }
        
        CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

        retval = CRYPTODRV_Release(pCryptodrvContext);
        
        /* Finally call the user callback */
        if (pAsynchContext->asynchCallback)
        {
          pAsynchContext->asynchCallback(status==ECODE_OK? (int)retval : (int)status,
                                        pAsynchContext->asynchCallbackArgument);
        }
      }
    }
  }
}

/***************************************************************************//**
 * @brief
 *   Callback function for asynchronous CMAC operation.
 *
 * @param[in] asynchCallbackArgument
 *   Callback argument.
 ******************************************************************************/
static void aesdrv_CMAC_AsynchCallback (void* asynchCallbackArgument)
{
  AESDRV_Context_t* pAesdrvContext = 
    (AESDRV_Context_t*) asynchCallbackArgument;

  if (pAesdrvContext)
  {
    AESDRV_CMAC_AsynchContext_t* pAsynchContext =
      (AESDRV_CMAC_AsynchContext_t*) pAesdrvContext->pAsynchContext;
    uint32_t dataBlocks                    = pAsynchContext->dataBlocks;
    CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;
    CRYPTO_TypeDef* crypto                 = pCryptodrvContext->device->crypto;
  
    /* Push next block */
    if (dataBlocks)
    {
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
      CRYPTO_InstructionSequenceExecute(crypto);
#endif
      if (dataBlocks > 1)
      {
        uint32_t* dataPointer = pAsynchContext->dataPointer;
        pAsynchContext->dataBlocks  = dataBlocks - 1;
        pAsynchContext->dataPointer = dataPointer + 4;
        CRYPTODRV_DataWriteUnaligned(&crypto->DATA0XOR, (uint8_t*)dataPointer);
      }
      else
      {
        pAsynchContext->dataBlocks = 0;
        CRYPTO_DataWrite(&crypto->DATA0XOR, pAsynchContext->lastBlock);
      }
#if !defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
      CRYPTO_InstructionSequenceExecute(crypto);
#endif
    }
    else
    {
      Ecode_t   status;
      Ecode_t   retval;
      
      /* Turn off interrupts. */
      CRYPTODRV_SetAsynchCallback(pCryptodrvContext, 0, 0);
      
      status = aesdrv_CMAC_Finalize(pAesdrvContext,
                                    pAsynchContext->encrypt,
                                    pAsynchContext->digest,
                                    pAsynchContext->digestLengthBits);

      CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

      retval = CRYPTODRV_Release(pCryptodrvContext);
    
      /* Finally call the user callback */
      if (pAsynchContext->asynchCallback)
      {
        if (MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == status)
        {
          status = (Ecode_t)MBEDTLS_ERR_CMAC_AUTH_FAILED;
        }
        pAsynchContext->asynchCallback(status==ECODE_OK? (int)retval : (int)status,
                                      pAsynchContext->asynchCallbackArgument);
      }
    }
  }
  
  return;
}

#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

/***************************************************************************//**
 * @brief
 *   Perform final CMAC processing.
 *
 * @param[in] encrypt
 *   True if encryption was requested. False if decryption was requested.
 *
 * @param[in] digest
 *   Pointer to location where digest should be stored.
 *
 * @param[in] digestLengthBits
 *   Length of digest in bits.
 ******************************************************************************/
static inline Ecode_t aesdrv_CMAC_Finalize(AESDRV_Context_t* pAesdrvContext,
                                           bool              encrypt,
                                           uint8_t*          digest,
                                           uint16_t          digestLengthBits)
{
  int i;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  
  /* If needed, verify */
  if(encrypt)
  {
    /* Read final hash/digest from CRYPTO. */
    CRYPTODRV_DataReadUnaligned(&crypto->DATA0, digest);
    
    /* mask away unneeded bits */
    i = digestLengthBits;
    while (i < 128) {
      if (i % 8 == 0) {
        /* mask away a full byte */
        digest[i/8] = 0;
        i += 8;
      } else {
        /* mask away partial byte */
        digest[i/8] &= ~(1 << (7-(i%8)));
        i++;
      }
    }    
  }
  else
  {
    /* Get hash from CRYPTO unintrusively */
    uint32_t hash128[4];
    uint8_t *hash = (uint8_t*) hash128;

    /* Read final hash/digest and compare it with expected. */
    CRYPTO_DataRead(&crypto->DATA0, hash128);
    
    /* Compare the full length of the digest */
    i = 0;
    while( i < digestLengthBits )
    {
      if(digestLengthBits - i >= 8)
      {
        /* If at least a full byte to go, use byte comparison */
        if(hash[i/8] != digest[i/8])
        {
          return MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED;
        }
        i += 8;
      }
      else
      {
        /* If less then a byte to go, use bit comparison */
        if((hash[i/8] & (1 << (7-(i%8)))) != (digest[i/8] & (1 << (7-(i%8)))))
        {
          return MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED;
        }
        i += 1;
      }
    }
  }
  return ECODE_OK;
}

/***************************************************************************//**
 * @brief
 *   Function is doing initial CRYPTO setup and loads the key to KEYBUF
 *
 * @param[in] pKey
 *   128 bit key
 *
 ******************************************************************************/
static inline void aesdrv_CCM_Prepare(AESDRV_Context_t* pAesdrvContext,
                                      const uint8_t*        pKey)
{
  const uint32_t * const _pKey = (const uint32_t *)pKey;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  /* Setup CRYPTO registers for CCM operation:
     - AES-128 mode (256 not supported)
     - width of counter in CTR cipher mode to 2 bytes.
     - enable DMA unaligned access and set
  */
  crypto->CTRL = CRYPTO_CTRL_AES_AES128 |
                 CRYPTO_CTRL_INCWIDTH_INCWIDTH2 |
                 CRYPTO_CTRL_DMA0MODE_LENLIMIT;

  if (pKey)
  {
    /* Load key into high key for key buffer usage */
    CRYPTO_KeyBuf128Write(crypto, (uint32_t *)_pKey);
  }
}

/***************************************************************************//**
 * Function converts input 13 byte long nonce to 16 byte IV values used for
 * authentication and encryption in CCM. After function completes both IV values
 * are in CRYPTO registers. Counter IV in DATA1 and CBC IV in DATA2.
 *
 * @param pNonce
 *  13byte long nonce.
 *
 * @param authTagLength
 *  Size of MIC.
 *
 * @param lm
 *  Message length - payload to be encrypted.
 *
 *  @param la
 *  Authentication only length
 *
 ******************************************************************************/
static inline void aesdrv_CCM_NoncePrepare(AESDRV_Context_t* pAesdrvContext,
                                           uint8_t const * pNonce,
                                           uint8_t authTagLength,
                                           uint32_t lm,
                                           uint16_t la)
{
  uint8_t flags;
  /* Local variables used to optimize load/store sequences from memory to
     crypto. We want to load all 4 32bit data words to local register
     variables in the first sequence, then store them all in the second
     sequence.*/
  uint32_t volatile* regPtr;
  register uint32_t v0;
  register uint32_t v1;
  register uint32_t v2;
  register uint32_t v3;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  crypto->SEQCTRL  = 0;
  crypto->SEQCTRLB = 0;
  
  /* Flags for B0 are prepared */
  /* Set AData */
  flags = la ? 0x41 : 1;

  /* Set authentication field */
  flags |= ((authTagLength-2)/2)<<3;

  /* Partial authentication tag is kept in DATA2.
   *
   * DATA2 is loaded with B0 block and AES encrypted.
   */
  if (authTagLength)
  {
    v0 = (flags) | (*(uint32_t *)(&pNonce[0]) << 8);
    v1 = *(uint32_t *)(&pNonce[3]);
    v2 = *(uint32_t *)(&pNonce[7]);
    v3 = *(uint16_t *)(&pNonce[11]) | __REV(lm);
    regPtr = (uint32_t volatile*) &crypto->DATA2;
    /* Store data to CRYPTO */
    *regPtr = v0;
    *regPtr = v1;
    *regPtr = v2;
    *regPtr = v3;

    CRYPTO_EXECUTE_3(crypto,
                     CRYPTO_CMD_INSTR_DATA2TODATA0,
                     CRYPTO_CMD_INSTR_AESENC,
                     CRYPTO_CMD_INSTR_DATA0TODATA2);
  }

  /* Counter for CTR encryption is kept in DATA1.
   * DATA1 is loaded with initial CTR value.
   * flags in Counter value are constant because only L=2 is supported.
   */
  v0 = 1 | (*(uint32_t *)(&pNonce[0]) << 8);
  v1 = *(uint32_t *)(&pNonce[3]);
  v2 = *(uint32_t *)(&pNonce[7]);
  v3 = *(uint16_t *)(&pNonce[11]);
  regPtr = (uint32_t volatile*) &crypto->DATA1;
  /* Store data to CRYPTO */
  *regPtr = v0;
  *regPtr = v1;
  *regPtr = v2;
  *regPtr = v3;
}

/***************************************************************************//**
 * @brief
 *  Function loads CCM instruction sequence to CRYPTO based on input parameters
 *  and I/O mode.
 *
 * @param authTagLength
 *  length of authenticationTag
 *
 * @param encrypt
 *  true - encryption
 *  false - decryption
 *
 ******************************************************************************/
static inline void aesdrv_CCM_SeqSet(AESDRV_Context_t* pAesdrvContext,
                                     const uint8_t     authTagLength,
                                     const bool        encrypt)
{
  const uint8_t * instrSeq;
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  if (authTagLength)
  {
    if (encrypt)
    {
      instrSeq = ccmAuthEncr[ioMode];
    }
    else
    {
      instrSeq = ccmAuthDecr[ioMode];
    }
  }
  else
  {
    instrSeq = ccmEncrDecr[ioMode];
  }
  CRYPTO_InstructionSequenceLoad(crypto, instrSeq);
}

/***************************************************************************//**
 * @brief
 *  Function performs CCM algorithm. In case of HW I/O mode it sets
 *  sequences lengths, setups hw I/O mode and triggers CRYPTO sequencer.
 *  When HW I/O mode is not used function performs CCM using MCU core access
 *  to CRYPTO registers.
 *
 * @param pHdr
 *  pointer do header
 *
 * @param pData
 *  pointer to data
 *
 * @param authTagLength
 *  length of authentication tag.
 *
 * @param la
 *  length of data to be authenticated only.
 *
 * @param lm
 *  length of data to be encrypted (and possibly authenticated).
 *
 ******************************************************************************/
static inline Ecode_t aesdrv_CCM_Execute
(
 AESDRV_Context_t*           pAesdrvContext,
 const uint8_t*              pHdr,
 const uint8_t*              pDataInput,
 uint8_t*                    pDataOutput,
 const uint8_t               authTagLength,
 uint32_t                    la,
 uint32_t                    lm
 )
{
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_CCM_AsynchContext_t* pAsynchContext =
    (AESDRV_CCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
  if (pAesdrvContext->ioMode == aesdrvIoModeCore)
  {
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    if (pAsynchContext && (la || lm))
    {
      pAsynchContext->hdrLength     = la;
      pAsynchContext->dataLength    = lm;
      pAsynchContext->pHdr          = (uint8_t*) pHdr;
      pAsynchContext->la            = la;
      pAsynchContext->pDataInput    = (uint8_t*) pDataInput;
      pAsynchContext->pDataOutput   = pDataOutput;
      pAsynchContext->lm            = lm;
      pAsynchContext->authTagLength = authTagLength;

      /* Kick off interrupt driven CCM operation by processing header
         (additional data) if included. If not, kick off with data encryption */
      if (la)
      {
        aesdrv_CCM_HeaderProcess(pAesdrvContext, (uint8_t*)pHdr, la);
      }
      else
      {
        pAsynchContext->encryptingHeader = false;

        /* Check if payload to be encrypted. */
        if (lm)
        {
          crypto->SEQCTRL = 0;
          crypto->SEQCTRLB = 16;
          aesdrvDataLoadAsynch(crypto, pDataInput, lm);
        }
      }
    }
    else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
    {
      /* Process header (additional data) if included.*/
      if (la)
      {
        aesdrv_CCM_HeaderProcess(pAesdrvContext, (uint8_t*)pHdr, la);
      }

      /* If there is a payload to be encrypted to the encryption. */
      if (lm)
      {
        crypto->SEQCTRL = 0;
        crypto->SEQCTRLB = 16;
        aesdrvDataLoadStore(crypto, pDataInput, pDataOutput, lm);
      }

      /* Finally encrypt authentication tag. */
      if (authTagLength)
      {
        crypto->SEQCTRL = 16;
        crypto->SEQCTRLB = 0;
        CRYPTO_EXECUTE_4(crypto,
                         CRYPTO_CMD_INSTR_DATA1INCCLR,
                         CRYPTO_CMD_INSTR_DATA1TODATA0,
                         CRYPTO_CMD_INSTR_AESENC,
                         CRYPTO_CMD_INSTR_DATA2TODATA0XOR);
        /* Wait for completion */
        CRYPTO_InstructionSequenceWait(crypto);
      }
    }
  }
  else
  {
    crypto->SEQCTRLB = lm;
    crypto->SEQCTRL = la+2;

    /* Configure BUFC or DMA */
    AESDRV_HwIoSetup(pAesdrvContext, (uint8_t*)pHdr, la, lm);

    /* Load 16bit authentication length to CRYPTO. */
    crypto->DATA0BYTE = la >> 8;
    crypto->DATA0BYTE = la;
    
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    if (pAsynchContext)
    {
      pAsynchContext->la                   = 0;
      pAsynchContext->lm                   = 0;
    }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

    /* Trigger CRYPTO sequence. */
    CRYPTO_InstructionSequenceExecute(crypto);
    /* Wait for completion */
    CRYPTO_InstructionSequenceWait(crypto);
  }
  return ECODE_OK;
}

/***************************************************************************//**
 * @brief
 *   Function is handling authentication only part of CCM. It assumes that
 *   CRYPTO is preloaded with proper instruction sequence.
 *
 * @param[in] pAesdrvontext
 *   AESDRV context
 *
 * @param[in] pHdr
 *   Input data
 *
 * @param[in] la
 *   length of data to be authenticated only.
 ******************************************************************************/
static void aesdrv_CCM_HeaderProcess(AESDRV_Context_t* pAesdrvContext,
                                     uint8_t *         pHdr,
                                     uint32_t          la)
                                   
{
  uint32_t tempBuf32[4];
  uint8_t* tempBuf = (uint8_t*)tempBuf32;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_CCM_AsynchContext_t* pAsynchContext =
    (AESDRV_CCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  /* Prepare first block for CBC in CCM which consists of 2 byte length field
   * and auth data which must be zero padded if there is less data than 14 bytes.
   */
  memset(tempBuf, 0, 16);

  tempBuf[0] = (uint8_t)(la>>8);
  tempBuf[1] = (uint8_t)la;

  memcpy(&tempBuf[2], pHdr, la<14 ? la : 14);

  crypto->SEQCTRL = 16;
  crypto->SEQCTRLB = 0;

  CRYPTO_DataWrite(&crypto->DATA0, tempBuf32);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  /* In the asynch mode, set the encryptingHeader flag in the context data in
     order for the asynch callback to know that we are encrypting the header
     part. */
  if (pAsynchContext)
  {
    pAsynchContext->encryptingHeader = true;
    if (la > 14)
    {
      pAsynchContext->la   -= 14;
      pAsynchContext->pHdr += 14;
    }
    else
    {
      pAsynchContext->la    = 0;
    }
  }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
  CRYPTO_InstructionSequenceExecute(crypto);

  /* Handle remaining header data. */
  if (
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
      (0==pAsynchContext) &&
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
      (la>14))
  {
    la   -= 14;
    pHdr += 14;
    aesdrvDataLoad(crypto, pHdr, la);
  }
}

/**
 * Function setup crypto for GCM, loads Key to crypto and calculates initial
 * values.
 *
 * @details
 *  Function is a common implementation for GCM with MCU,DMA and BUFC.
 *
 * @param pKey
 *  Key (128bit)
 * @param pInitialVector
 *  Initial Vector (96bits)
 */
static void aesdrv_GCM_Prepare(AESDRV_Context_t* pAesdrvContext,
                               const uint8_t*    pKey,
                               uint8_t*          pInitialVector)
{
  uint32_t        j;
  uint32_t*       _ctr = (uint32_t *) pInitialVector;
  const uint32_t* _key = (const uint32_t *) pKey;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  /* Setup CRYPTO for GCM operation:
     - AES-128 mode (256 not supported)
     - width of counter in CTR cipher mode to 4 bytes.
     - enable DMA unaligned access and set
  */
  crypto->CTRL = CRYPTO_CTRL_AES_AES128 |
                 CRYPTO_CTRL_INCWIDTH_INCWIDTH4 |
                 CRYPTO_CTRL_DMA0MODE_LENLIMIT;
  crypto->WAC      = 0;
  crypto->SEQCTRL  = 16;
  crypto->SEQCTRLB = 0;
  
  /* Set modulus to GCM. */
  CRYPTO_ModulusSet(crypto, cryptoModulusGcmBin128);

  /* Need to set result width? */
  CRYPTO_ResultWidthSet(crypto, cryptoResult128Bits);

  /*
  ** CRYPTO register usage:
  ** DATA0 - temporary result and plaintext
  ** DATA2 - Hash key
  ** DATA3 - E(K, Y0) which is used finally to XOR GHASH to produce the final tag
  **
  ** DDATA0 - temporary results in the GHASH function (Xi-1)
  ** DDATA1 - A and C parameters of the GHASH
  ** DDATA2 - overlaps DATA0 and DATA1
  ** DDATA3 - overlaps DATA2 and DATA3
  ** DDATA4 - KEYBUF
  */

  /* Load initial values to the CRYPTO module. */
  for (j = 0; j < 4; j++)
  {
    if (pKey)
    {
      /* Load key into KEYBUFFER for key buffer usage. */
      crypto->KEYBUF = _key[j];
    }
    /* Load the initial counter value Y0 into DATA1.
       Y0 = IV | 1
       That is, the 12 byte Initial Vector concatenated with the initial
       counter value 1 in the least significant 32bit word.
    */
    if (j==3)
    {
      crypto->DATA1 = 0x01000000;
    }
    else
    {
      crypto->DATA1 = _ctr[j];
    }
  }

  /* Organize and Compute the initial values. */
  CRYPTO_EXECUTE_6(crypto,
                   /* Compute hash key H=E(K,0) */
                   CRYPTO_CMD_INSTR_DATA0TODATA0XOR,/* DATA0 = 0 */
                   CRYPTO_CMD_INSTR_AESENC,         /* DATA0 = AESENC(DATA0) */
                   CRYPTO_CMD_INSTR_SELDDATA2DDATA2,/* Select DATA0 for BBSWAP instruction */
                   CRYPTO_CMD_INSTR_BBSWAP128,      /* Swap bit in H, needed for GMAC */
                   CRYPTO_CMD_INSTR_DDATA0TODDATA3, /* Move resulting hash key
                                                       (in LSWord of DDATA0) to
                                                       DATA2 (LSWord ofDDATA3)*/
                   /* Prepare GHASH(H,A) */
                   CRYPTO_CMD_INSTR_CLR  /* DDATA0 = 0 */
                   );
}

/***************************************************************************//**
 * @brief
 *  Function loads GCM instruction sequence to CRYPTO based on input parameters
 *  and hw I/O mode.
 *
 * @param encrypt
 *  true - encryption
 *  false - decryption
 *
 ******************************************************************************/
static inline void aesdrv_GCM_SeqSet(AESDRV_Context_t* pAesdrvContext,
                                     const bool        encrypt)
{
  const uint8_t * instrSeq;
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  if (encrypt)
  {
    instrSeq = gcmAuthEncr[ioMode];
  }
  else
  {
    instrSeq = gcmAuthDecr[ioMode];
  }

  CRYPTO_InstructionSequenceLoad(crypto, instrSeq);
}

/***************************************************************************//**
 * @brief
 *  Function performs GCM algorithm. In case of HW I/O mode it sets
 *  sequences lengths, setups hw I/O mode and triggers CRYPTO sequencer.
 *  When HW I/O mode is not used function performs GCM using MCU access
 *  to CRYPTO registers.
 *
 * @param[in] pDataInput
 *   If @p encrypt is true, pDataInput is the plaintext.
 *   I.e. the payload data to encrypt. 
 *   If @p encrypt is false, pDataInput is the ciphertext.
 *   I.e. the ciphertext data to decrypt. 
 *
 * @param[out] pDataOutput
 *   If @p encrypt is true, pDataOutput is the ciphertext.
 *   I.e. the Ciphertext data as a result of encrypting the payload data.
 *   If @p encrypt is false, pDataOutput is the plaintext.
 *   I.e. the Payload data as a result of decrypting the ciphertext.
 *
 * @param dataLength
 *  data length
 *
 * @param pHdr
 *  pointer do header
 *
 * @param hdrLength
 *  header length.
 *
 ******************************************************************************/
static void aesdrv_GCM_Execute(AESDRV_Context_t* pAesdrvContext,
                               const uint8_t*    pDataInput,
                               uint8_t*          pDataOutput,
                               uint16_t          dataLength,
                               const uint8_t*    pHdr,
                               uint32_t          hdrLength)
{
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_GCM_AsynchContext_t* pAsynchContext =
    (AESDRV_GCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  if (pAesdrvContext->ioMode == aesdrvIoModeCore)
  {
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
    if (pAsynchContext)
    {
      pAsynchContext->hdrLength            = hdrLength;
      pAsynchContext->dataLength           = dataLength;
      pAsynchContext->lm                   = dataLength;
      pAsynchContext->pDataInput           = pDataInput;
      pAsynchContext->pDataOutput          = pDataOutput;

      /* Kick off interrupt driven GCM operation by processing header
         (if present) or data encryption. */
      if (hdrLength)
      {
        if (hdrLength > 16)
        {
          pAsynchContext->la               = hdrLength - 16;
          pAsynchContext->pHdr             = pHdr + 16;
        }
        else
        {
          pAsynchContext->la               = 0;
        }          
        pAsynchContext->encryptingHeader   = true;
        crypto->SEQCTRL = 16;
        crypto->SEQCTRLB = 0;
        aesdrvDataLoadAsynch(crypto, pHdr, hdrLength);
      }
      else
      {
        pAsynchContext->la                 = 0;
        /* Check if payload to be encrypted. */
        if (dataLength)
        {
          pAsynchContext->encryptingHeader = false;
          crypto->SEQCTRL = 0;
          crypto->SEQCTRLB = 16;
          aesdrvDataLoadAsynch(crypto, pDataInput, dataLength);
        }
      }
    }
    else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
    {
      /* Process authentication part only.*/
      if (hdrLength)
      {
        crypto->SEQCTRL = 16;
        crypto->SEQCTRLB = 0;
        aesdrvDataLoad(crypto, (uint8_t*)pHdr, hdrLength);
      }

      /* If there is a payload to be encrypted to the encryption. */
      if (dataLength)
      {
        crypto->SEQCTRL = 0;
        crypto->SEQCTRLB = 16;
        aesdrvDataLoadStore(crypto, pDataInput, pDataOutput, dataLength);
      }
      /* Wait for completion */
      while(!CRYPTO_InstructionSequenceDone(crypto));
    }
  }
  else
  {
    if (dataLength+hdrLength)
    {
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
      if (pAsynchContext)
      {
        pAsynchContext->hdrLength            = hdrLength;
        pAsynchContext->dataLength           = dataLength;
        pAsynchContext->la                   = 0;
        pAsynchContext->lm                   = 0;
      }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
      
      crypto->SEQCTRL = hdrLength;
      crypto->SEQCTRLB = dataLength;

      AESDRV_HwIoSetup(pAesdrvContext,
                       (uint8_t*)pHdr, hdrLength, dataLength);

      /* Trigger CRYPTO sequence. */
      CRYPTO_InstructionSequenceExecute(crypto);

      /* Wait for completion */
      while(!CRYPTO_InstructionSequenceDone(crypto));
    }
  }
}

/**
 * @brief
 *  Function calculates last part of GHASH (length fields) and
 *  Authentication Tag.
 *
 * @details
 *  Function is common for all three modes (MCU,BUFC,DMA).
 *
 * @param authDataLength
 *  Number of bytes in authentication part.
 *
 * @param plaintextLength
 *  Number of bytes in payload.
 *
 * @param pAuthTag
 *  pointer to memory where AuthTag is written.
 *
 * @param authTagLength
 *  length of authentication tag (in bytes). Up to 16 bytes.
 */
static void aesdrv_GCM_Finalize(AESDRV_Context_t* pAesdrvContext,
                                unsigned int      authDataLength,
                                unsigned int      plaintextLength)
{
  uint32_t ddata[8];
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  CRYPTO_DDataRead(&crypto->DDATA0, ddata);
  
  crypto->SEQCTRL = 16;
  crypto->SEQCTRLB = 0;

  crypto->KEY = 0;
  crypto->KEY = __REV((authDataLength * 8));
  crypto->KEY = 0;
  crypto->KEY = __REV((plaintextLength * 8));
  
  CRYPTO_EXECUTE_15(crypto,
                    /* Calculate last part of GHASH (length fields) */
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA1, /* A[i] and Xi-1 */
                    CRYPTO_CMD_INSTR_XOR,
                    CRYPTO_CMD_INSTR_BBSWAP128,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA3, /* temp result and H */
                    CRYPTO_CMD_INSTR_MMUL, /* Xi is stored in DDATA0 */
                    CRYPTO_CMD_INSTR_BBSWAP128,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                    CRYPTO_CMD_INSTR_DATA2TODATA0,
                    /* Calculate AuthTag */
                    CRYPTO_CMD_INSTR_DATA1INCCLR,
                    CRYPTO_CMD_INSTR_DATA0TODATA3,
                    CRYPTO_CMD_INSTR_DATA1INC,
                    CRYPTO_CMD_INSTR_DATA1TODATA0,
                    CRYPTO_CMD_INSTR_AESENC,
                    CRYPTO_CMD_INSTR_DATA3TODATA0XOR
                    ); /* DATA0 = DATA0 ^ DATA3*/
}

/***************************************************************************//**
 * @brief
 *  Function is called once Authentication Tag is already calculated (but not
 *  encrypted yet). Function is encrypting the tag and write back the tag (in
 *  case of encryption) or comparing received tag with calculcated tag (in case
 *  of decryption).
 *
 * @param[in,out] pAuthTag
 *  location of pAuthTag. In case of encryption tag will be stored there. In
 *  case of decryption location will be used for comparison with the tag
 *  calculated in CRYPTO.
 *
 * @param[in] encrypt
 *  true - encryption, false - decryption
 *
 * @param[in] authTagLength
 *  Length of authentication tag.
 *
 * @return
 *  true - in case of encryption always true, in case of decryption if
 *  authentication tag is validated with success.
 *  false - in case of decryption if authentication tag is invalid.
 *
 ******************************************************************************/
static inline Ecode_t aesdrvAuthTagHandle(AESDRV_Context_t* pAesdrvContext,
                                           uint8_t*          pAuthTag,
                                           uint8_t           authTagLength,
                                           bool              encrypt)
{
  Ecode_t status = ECODE_OK;

  if (authTagLength)
  {
    if (encrypt)
    {
      aesdrvAuthTagRead(pAesdrvContext, pAuthTag, authTagLength);
    }
    else
    {
      status = aesdrvAuthTagCompare(pAesdrvContext, pAuthTag, authTagLength);
    }
  }
  return status;
}

/***************************************************************************//**
 * @brief
 *  Function is reading authentication tag from CRYPTO.
 *  Function assumes that tag is in DATA0.
 *
 * @param[out] pAuthTag
 *  location of pAuthTag.
 *
 * @param[in] authTagLength
 *  Length of authentication tag.
 *
 ******************************************************************************/
static void aesdrvAuthTagRead(AESDRV_Context_t* pAesdrvContext,
                               uint8_t*          pAuthTag,
                               uint8_t           authTagLength)
{
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  
  if (pAesdrvContext->authTagOptimize)
  {
    uint32_t * _pAuthTag = (uint32_t *)pAuthTag;
    /* Local variables used to optimize load/store sequences from crypto to
       memory. We want to load all 4 32bit data words to local register
       variables in the first sequence, then store them all in the second
       sequence.*/
    if (authTagLength<=4)
    {
      _pAuthTag[0] = crypto->DATA0;
    }
    else
    {
      uint32_t volatile* regPtr=&crypto->DATA0;
      if (authTagLength > 12)
      {
        register uint32_t v0 = *regPtr;
        register uint32_t v1 = *regPtr;
        register uint32_t v2 = *regPtr;
        register uint32_t v3 = *regPtr;
        _pAuthTag[0] = v0;
        _pAuthTag[1] = v1;
        _pAuthTag[2] = v2;
        _pAuthTag[3] = v3;
      }
      else
      {
        if (authTagLength > 8)
        {
          register uint32_t v0 = *regPtr;
          register uint32_t v1 = *regPtr;
          register uint32_t v2 = *regPtr;
          _pAuthTag[0] = v0;
          _pAuthTag[1] = v1;
          _pAuthTag[2] = v2;
        }
        else
        {
          register uint32_t v0 = *regPtr;
          register uint32_t v1 = *regPtr;
          _pAuthTag[0] = v0;
          _pAuthTag[1] = v1;
        }
      }
    }
  }
  else
  {
    uint32_t tempBuf[4];
    CRYPTO_DataRead(&crypto->DATA0,tempBuf);
    memcpy(pAuthTag,tempBuf,authTagLength);
  }
}

/***************************************************************************//**
 * @brief
 *  Function is comparing authentication tag from CRYPTO with the received one.
 *  Function assumes that tag is in DATA0.
 *
 * @param[out] pAuthTag
 *  location of pAuthTag.
 *
 * @param[in] authTagLength
 *  Length of authentication tag.
 *
 * @return
 *  ECODE_OK - authentication tag is valid
 *  MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED - authentication tag is invalid.
 *
 ******************************************************************************/
static Ecode_t aesdrvAuthTagCompare(AESDRV_Context_t* pAesdrvContext,
                                    uint8_t*          pAuthTag,
                                    uint8_t           authTagLength)
{
  Ecode_t status = ECODE_OK;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  if (pAesdrvContext->authTagOptimize)
  {
    uint32_t mask;
    uint32_t zeroData;
    CRYPTODRV_DataWriteUnaligned(&crypto->DATA3, pAuthTag);

    crypto->CMD = CRYPTO_CMD_INSTR_DATA3TODATA0XOR;
    mask = (1 << (authTagLength/4))-1;
    zeroData = (crypto->DSTATUS & _CRYPTO_DSTATUS_DATA0ZERO_MASK) 
      >> _CRYPTO_DSTATUS_DATA0ZERO_SHIFT;

    if ( (mask & zeroData) != mask)
    {
      status = MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED;
    }
  }
  else
  {
    uint32_t tempBuf[4];
    CRYPTO_DataRead(&crypto->DATA0,tempBuf);
    if (memcmp(pAuthTag,tempBuf,authTagLength))
    {
      status = MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED;
    }
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *   Function is loading DATA0 register with provided data and triggers CRYPTO
 *   sequence. Additionally, function handles zeropadding for last block.
 *   Function doesn't read back the data from CRYPTO it is supposed to be used
 *   for MIC only calculation part of CCM (header).
 *
 * @param[in] pData
 *   Input data
 *
 * @param[in] length
 *   length of data in bytes.
 *
 ******************************************************************************/
static void aesdrvDataLoad(CRYPTO_TypeDef* crypto,
                            uint8_t * pData,
                            uint16_t length)
{
  uint32_t tempBuf[4];
  while(length)
  {
    /* Check if zero padding is needed. */
    if (length < 16)
    {
      /* Use temporary buffer for zero padding */
      memset(tempBuf,0,16);
      memcpy(tempBuf,pData,length);
      CRYPTO_DataWrite(&crypto->DATA0, tempBuf);
      length = 0;
    }
    else
    {
      CRYPTODRV_DataWriteUnaligned(&crypto->DATA0, pData);
      length -= 16;
      pData  += 16;
    }
    CRYPTO_InstructionSequenceExecute(crypto);
  }
}

/***************************************************************************//**
 * @brief
 *   Function is loading DATA0 register with provided data and triggers CRYPTO
 *   sequence. Once sequence is completed that is read back from DATA0 and
 *   input data is overwritten. Additionally, function handles zeropadding
 *   for last block. Function is supposed to be used for payload part of CCM
 *   where data is encrypted.
 *
 * @param[in] pData
 *   Input data
 *
 * @param[in] length
 *   length of data in bytes.
 *
 ******************************************************************************/
static void aesdrvDataLoadStore(CRYPTO_TypeDef* crypto,
                                 const uint8_t* pDataInput,
                                 uint8_t*       pDataOutput,
                                 uint16_t       length)
{
  uint32_t tempBuf[4];

  while (length)
  {
    if (length < 16)
    {
      /* Use temporary buffer for zero padding */
      crypto->SEQCTRLB = length;
      memset(tempBuf,0,16);
      memcpy(tempBuf,pDataInput,length);

      CRYPTO_DataWrite(&crypto->DATA0, tempBuf);
      CRYPTO_InstructionSequenceExecute(crypto);

      CRYPTO_DataRead(&crypto->DATA0, tempBuf);
      memcpy(pDataOutput,tempBuf,length);
      return;
    }
    else
    {
      CRYPTODRV_DataWriteUnaligned(&crypto->DATA0, pDataInput);
      CRYPTO_InstructionSequenceExecute(crypto);
      CRYPTODRV_DataReadUnaligned(&crypto->DATA0, pDataOutput);
      length       -= 16;
      pDataInput   += 16;
      pDataOutput  += 16;
    }
  }
}

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)

/***************************************************************************//**
 * @brief
 *   Load data to CRYPTO in asynch mode.
 *
 * @details
 *   Function is loading DATA0 register with provided data, triggers CRYPTO
 *   sequencer and returns. The completion is handled by asynch callback.
 *
 * @param[in] pAsynchContext
 *   Context data for asynch mode.
 *
 ******************************************************************************/
static void aesdrvDataLoadAsynch(CRYPTO_TypeDef* crypto,
                                 const uint8_t*  pData,
                                 uint16_t        length)
{
  if (length < 16)
  {
    /* Use temporary buffer for zero padding */
    uint32_t tempBuf[4];
    memset(tempBuf, 0, 16);
    memcpy(tempBuf, pData, length);
    CRYPTO_DataWrite(&crypto->DATA0, tempBuf);
  }
  else
  {
    CRYPTODRV_DataWriteUnaligned(&crypto->DATA0, pData);
  }
  CRYPTO_InstructionSequenceExecute(crypto);
}

/***************************************************************************//**
 * @brief
 *   Store output data from CRYPTO to memory in asynch mode.
 *
 * @details
 *   Function stores the output data located in the CRYPTO DATA0 register to
 *   the user output buffer in memory, and updates the asynch context data.
 *
 * @param[in] pAesdrvContext
 *   Context data aesdrv.
 *
 ******************************************************************************/
static void aesdrvDataStoreAsynch(AESDRV_Context_t* pAesdrvContext)
{
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  AESDRV_CCM_AsynchContext_t* pAsynchContext =
    (AESDRV_CCM_AsynchContext_t*) pAesdrvContext->pAsynchContext;

  int lm = pAsynchContext->lm;
  if (lm < 16)
  {
    /* Use temporary buffer to store 128 bits (16 bytes) from
       DATA0 register, and copy only lm bytes to user buffer. */
    uint32_t tempBuf[4];

    CRYPTO_DataRead(&crypto->DATA0, tempBuf);
    memcpy(pAsynchContext->pDataOutput, tempBuf, lm);

    pAsynchContext->lm = 0;
  }
  else
  {
    CRYPTODRV_DataReadUnaligned(&crypto->DATA0, pAsynchContext->pDataOutput);
    pAsynchContext->lm          -= 16;
    pAsynchContext->pDataInput  += 16;
    pAsynchContext->pDataOutput += 16;
  }
  return;
}

#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

/** @endcond */

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
