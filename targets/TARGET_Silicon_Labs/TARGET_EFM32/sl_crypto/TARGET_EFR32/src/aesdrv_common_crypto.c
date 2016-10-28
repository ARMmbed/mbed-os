/*
 *  Common functions for AES based algorithms for the CRYPTO hw module.
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

#include "aesdrv_common_crypto.h"
#include "cryptodrv_internal.h"
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
#include "dmadrv.h"
#endif
#include "em_crypto.h"
#if defined(BUFC_PRESENT)
#include "em_bufc.h"
#endif
#include "em_assert.h"
#include <string.h>

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
#define AESDRV_UTILS_BUFC_BUFFER_NOT_SET 0xFF
#define AESDRV_UTILS_DMA_CHANNEL_NOT_SET 0xFFFF

/*******************************************************************************
 **************************   STATIC FUNCTIONS   *******************************
 ******************************************************************************/
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
static void    aesdrvDmaReset     (AESDRV_Context_t* pAesdrvContext);
static Ecode_t aesdrvDmaInit      (AESDRV_Context_t* pAesdrvContext);
static void    aesdrvDmaSetup     (AESDRV_Context_t* pAesdrvContext,
                                   uint8_t const*    pData,
                                   uint32_t          authDataLength,
                                   uint32_t          textLength
                                   );
static void    aesdrvDmaAddrLenGet(uint8_t**         pBufIn,
                                   uint8_t**         pBufOut,
                                   uint16_t          lengthA,
                                   uint16_t          lengthB,
                                   uint16_t*         dmaLengthIn,
                                   uint16_t*         dmaLengthOut);
#endif /* #if defined( MBEDTLS_INCLUDE_IO_MODE_DMA ) */

#if defined(BUFC_PRESENT)
static void    aesdrvBufcInit     (AESDRV_Context_t* pAesdrvContext,
                                   uint8_t           bufId
                                   );
static void    aesdrvBufcSetup    (AESDRV_Context_t* pAesdrvContext,
                                   uint8_t const*    pData,
                                   uint32_t          authDataLength);
#endif

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*
 *   Initializes an AESDRV context structure.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_Init(AESDRV_Context_t* pAesdrvContext)
{
  /* Start by clearing the device context. */
  memset(pAesdrvContext, 0, sizeof(AESDRV_Context_t));

  /* Set I/O mode to mcu core. */
  pAesdrvContext->ioMode = aesdrvIoModeCore;
  
  /* Clear the DMA channel config */
  pAesdrvContext->ioModeSpecific.dmaConfig.dmaChIn =
    AESDRV_UTILS_DMA_CHANNEL_NOT_SET;
  pAesdrvContext->ioModeSpecific.dmaConfig.dmaChOut =
    AESDRV_UTILS_DMA_CHANNEL_NOT_SET;

#if defined(BUFC_PRESENT)
  /* Clear the bufc buffer id */
  pAesdrvContext->ioModeSpecific.bufcConfig.bufId =
    AESDRV_UTILS_BUFC_BUFFER_NOT_SET;
#endif

  /* Disable authentication tag optimization */
  pAesdrvContext->authTagOptimize         = false;

  /* Set default CRYPTO device instance to use. */
  cryptodrvSetDeviceInstance(&pAesdrvContext->cryptodrvContext, 0);

  /* Set I/O mode to mcu core. */
  return AESDRV_SetIoMode(pAesdrvContext, aesdrvIoModeCore, 0);
}

/*
 *   DeInitializes AESDRV context.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_DeInit(AESDRV_Context_t* pAesdrvContext)
{
  switch( pAesdrvContext->ioMode )
  {
  case aesdrvIoModeCore:
    /* Do nothing. */
    break;
  case aesdrvIoModeDma:
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    aesdrvDmaReset(pAesdrvContext);
#endif
    break;
  case aesdrvIoModeBufc:
#if defined(BUFC_PRESENT)
    /* Clear the bufc buffer id */
    aesdrvBufcInit(pAesdrvContext, AESDRV_UTILS_BUFC_BUFFER_NOT_SET);
#endif
    break;
  }

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  /* If set, clear the asynch context. The size to clear depends on the
     cipher mode. */
  if (pAesdrvContext->pAsynchContext)
  {
    switch (pAesdrvContext->cipherMode)
    {
    case cipherModeBlockCipher:
      memset(pAesdrvContext->pAsynchContext, 0,
             sizeof(AESDRV_BlockCipherAsynchContext_t));
    break;
    case cipherModeCcm:
    case cipherModeCcmBle:
      memset(pAesdrvContext->pAsynchContext, 0,
             sizeof(AESDRV_CCM_AsynchContext_t));
    break;
    case cipherModeCmac:
      memset(pAesdrvContext->pAsynchContext, 0,
             sizeof(AESDRV_CMAC_AsynchContext_t));
    break;
    case cipherModeGcm:
      memset(pAesdrvContext->pAsynchContext, 0,
             sizeof(AESDRV_GCM_AsynchContext_t));
    break;
    case cipherModeNone:
    default:
      break;
    }
  }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
  /* Clear the device context. */
  memset(pAesdrvContext, 0, sizeof(AESDRV_Context_t));
  
  return ECODE_OK;
}

/*
 *   Set the AES/CRYPTO device instance.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_SetDeviceInstance(AESDRV_Context_t*  pAesdrvContext,
                                 unsigned int       devno)
{
  /* Set default CRYPTO device instance to use. */
  return cryptodrvSetDeviceInstance(&pAesdrvContext->cryptodrvContext,
                                    devno);
}

/*
 *   Set the AES encryption key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_SetKey(AESDRV_Context_t* pAesdrvContext,
                      const uint8_t*    pKey,
                      uint32_t          keyLength)
{
  Ecode_t retval = ECODE_OK;
  CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;

  EFM_ASSERT(pKey);
  
  retval = CRYPTODRV_Arbitrate(pCryptodrvContext);
  if (ECODE_OK != retval)
    return retval;

  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

  if (32==keyLength)
  {
    CRYPTO_KeyBufWrite(pCryptodrvContext->device->crypto,
                       (uint32_t*)pKey, cryptoKey256Bits);
  }
  else
  {
    if (16==keyLength)
    {
      CRYPTO_KeyBufWrite(pCryptodrvContext->device->crypto,
                         (uint32_t*)pKey, cryptoKey128Bits);
    }
    else
    {
      retval = MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
    }
  }

  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  retval = CRYPTODRV_Release(pCryptodrvContext);
  return retval;
}

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
/*
 *   Setup the asynchronous mode of an AESDRV context.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_SetAsynchMode
(
 AESDRV_Context_t*       pAesdrvContext,
 AESDRV_CipherMode_t     cipherMode,
 void*                   pAsynchContext,
 AESDRV_AsynchCallback_t asynchCallback,
 void*                   asynchCallbackArgument
 )
{
  pAesdrvContext->pAsynchContext         = pAsynchContext;
  pAesdrvContext->cipherMode             = cipherMode;

  switch (cipherMode)
  {
  default:
    pAesdrvContext->pAsynchContext       = 0;
    break;
  case cipherModeBlockCipher:
    {
      AESDRV_BlockCipherAsynchContext_t* pBlockCipherAsynchContext  =
        (AESDRV_BlockCipherAsynchContext_t*) pAsynchContext;
      pBlockCipherAsynchContext->asynchCallback              = asynchCallback;
      pBlockCipherAsynchContext->asynchCallbackArgument      = asynchCallbackArgument;
    }
    break;
  case cipherModeCcm:
  case cipherModeCcmBle:
    {
      AESDRV_CCM_AsynchContext_t* pCcmAsynchContext  =
        (AESDRV_CCM_AsynchContext_t*) pAsynchContext;
      pCcmAsynchContext->asynchCallback              = asynchCallback;
      pCcmAsynchContext->asynchCallbackArgument      = asynchCallbackArgument;
    }
    break;
  case cipherModeCmac:
    {
      AESDRV_CMAC_AsynchContext_t* pCmacAsynchContext  =
        (AESDRV_CMAC_AsynchContext_t*) pAsynchContext;
      pCmacAsynchContext->asynchCallback              = asynchCallback;
      pCmacAsynchContext->asynchCallbackArgument      = asynchCallbackArgument;
    }
    break;
  case cipherModeGcm:
    {
      AESDRV_GCM_AsynchContext_t* pGcmAsynchContext  =
        (AESDRV_GCM_AsynchContext_t*) pAsynchContext;
      pGcmAsynchContext->asynchCallback              = asynchCallback;
      pGcmAsynchContext->asynchCallbackArgument      = asynchCallbackArgument;
    }
    break;
  }

  return ECODE_OK;
}
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

/*
 *   Check if ioMode is valid for crypto device of context.
 */
bool aesdrvIoModeValid
(
 AESDRV_Context_t*        pAesdrvContext,
 AESDRV_IoMode_t          ioMode
 )
{
  if (ioMode != aesdrvIoModeBufc)
  {
    return true;
  }
  else
  {
#if (CRYPTO_COUNT == 1)
    (void) pAesdrvContext;
    return true;
#elif (CRYPTO_COUNT == 2)
    if ( pAesdrvContext->cryptodrvContext.device->crypto == CRYPTO1 )
    {
      return true;
    }
    else
    {
      return false;
    }
#else
#error Unsupported CRYPTO_COUNT.
#endif
  }
}
 
/*
 *   Setup CRYPTO I/O mode.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_SetIoMode
(
 AESDRV_Context_t*        pAesdrvContext,
 AESDRV_IoMode_t          ioMode,
 AESDRV_IoModeSpecific_t* ioModeSpecific
 )
{
  Ecode_t retval = ECODE_OK;
#if !defined(BUFC_PRESENT)
  (void) ioModeSpecific;
#endif

  if (aesdrvIoModeValid(pAesdrvContext, ioMode) == false)
  {
    return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
  }
  
  /* Start by reseting any previous settings, if applicable. */
  if ( ioMode != pAesdrvContext->ioMode )
  {
    switch(pAesdrvContext->ioMode )
    {
      case aesdrvIoModeCore:
        break;
      case aesdrvIoModeDma:
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
        aesdrvDmaReset(pAesdrvContext);
#endif
        break;
      case aesdrvIoModeBufc:
#if defined(BUFC_PRESENT)
        /* Clear the bufc buffer id */
        aesdrvBufcInit(pAesdrvContext, AESDRV_UTILS_BUFC_BUFFER_NOT_SET);
#endif
        break;
    }

    /* Set requested I/O mode now. */
    switch( ioMode )
    {
    case aesdrvIoModeCore:
      /* Do nothing. Just continue without hitting default label which signals
         error.*/
      break;
    
    case aesdrvIoModeBufc:
#if defined(BUFC_PRESENT)
    {
      BUFC_Init_TypeDef      bufcInit = BUFC_INIT_DEFAULT;
      
      /* Make sure BUFC clock is running. */
#if defined (CMU_HFRADIOCLKEN0_BUFC)
      CMU->HFRADIOCLKEN0 |= CMU_HFRADIOCLKEN0_BUFC;
#endif
#if defined (CMU_HFRADIOALTCLKEN0_BUFC)
      CMU->HFRADIOALTCLKEN0 |= CMU_HFRADIOALTCLKEN0_BUFC;
#endif
      
      BUFC_Init(&bufcInit);
      
      aesdrvBufcInit(pAesdrvContext, ioModeSpecific->bufcConfig.bufId);
    }
#else
    retval = MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
    break;
    case aesdrvIoModeDma:
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
      /* Start by reseting previous settings.*/
      aesdrvDmaReset(pAesdrvContext);
      retval = aesdrvDmaInit(pAesdrvContext);
#else
      retval = MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
      break;
      
    default:
      retval = MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
    }

    if (ECODE_OK == retval)
    {
      /* If success we store the I/O mode for later references. */
      pAesdrvContext->ioMode = ioMode;
    }
  }
  
  return retval;
}

/*
 *   Prepare CRYPTO I/O mode to transfer data
 */
void AESDRV_HwIoSetup(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          pData,
                      uint32_t          authDataLength,
                      uint32_t          textLength)
{
#if !defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  (void)pAesdrvContext; (void)pData; (void)authDataLength; (void)textLength;
#endif
  switch (pAesdrvContext->ioMode)
  {
  case aesdrvIoModeBufc:
#if defined(BUFC_PRESENT)
    aesdrvBufcSetup( pAesdrvContext, pData, authDataLength );
#endif
    break;
  case aesdrvIoModeDma:
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
    aesdrvDmaSetup( pAesdrvContext, pData, authDataLength, textLength );
#endif
    break;
  default:
    break;
  }
}

/*******************************************************************************
 ***********************   LOCAL STATIC FUNCTIONS   ****************************
 ******************************************************************************/

#if defined(BUFC_PRESENT)
/**
 * Function initializes BUFC for CRYPTO
 *
 * @details
 *  Function only needs to store buffer id to be used.
 *
 * @param bufId
 *  BUFC Buffer Id to be used later on by CRYPTO.
 */
static void aesdrvBufcInit(AESDRV_Context_t* pAesdrvContext,
                           uint8_t           bufId)
{
  pAesdrvContext->ioModeSpecific.bufcConfig.bufId = bufId;
}

/**
 * Function setup BUFC for CRYPTO.
 *
 * @details
 *  Function assumes that same buffer is used for input and output (in place).
 *  Additionally, it supports packet authentication&encryption where first part
 *  of the packet is only authenticated (CCM,GCM).
 *
 * @param pData
 *  Address of input-output buffer.
 *
 * @param authDataLen
 *  Length of authentication part.
 *
 * @warning
 *  Function is modifying CRYPTO_CTRL register by doing
 *  OR operation assuming that bit fields which are set have previously been
 *  cleared. It must be ensured that access to this register is done in
 *  correct order and settings are not overwritten.
 */
static void aesdrvBufcSetup(AESDRV_Context_t* pAesdrvContext,
                            uint8_t const*    pData,
                            uint32_t          authDataLength)
{
  uint8_t         bufId  = pAesdrvContext->ioModeSpecific.bufcConfig.bufId;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  uint32_t        ctrl;

  EFM_ASSERT(bufId != AESDRV_UTILS_BUFC_BUFFER_NOT_SET);
  
  /* Setup BUFC */
  BUFC->BUF[bufId].CTRL = BUFC_BUF_CTRL_SIZE_SIZE2048;
  BUFC->BUF[bufId].ADDR = (uint32_t)pData;
  BUFC->BUF[bufId].CMD = BUFC_BUF_CMD_CLEAR;
  BUFC->BUF[bufId].WRITEOFFSET = 2048 + (authDataLength);
  BUFC->BUF[bufId].CMD = BUFC_BUF_CMD_PREFETCH;

  ctrl  = crypto->CTRL;
  ctrl &= ~_CRYPTO_CTRL_READBUFSEL_MASK & ~_CRYPTO_CTRL_WRITEBUFSEL_MASK;
  ctrl |= bufId << _CRYPTO_CTRL_READBUFSEL_SHIFT
    | bufId << _CRYPTO_CTRL_WRITEBUFSEL_SHIFT;
  crypto->CTRL = ctrl;
}
#endif

#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )

/***************************************************************************//**
 * Function resets DMA settings for CRYPTO.
 *
 * @details
 *   Function clears trigger settings for channel setup to be used by CRYPTO.
 *
 ******************************************************************************/
static void aesdrvDmaReset(AESDRV_Context_t* pAesdrvContext)
{
  AESDRV_DmaConfig_t* dmaConfig= &pAesdrvContext->ioModeSpecific.dmaConfig;
    
  if (dmaConfig->dmaChIn != AESDRV_UTILS_DMA_CHANNEL_NOT_SET)
  {
    DMADRV_FreeChannel(dmaConfig->dmaChIn);
  }

  if (dmaConfig->dmaChOut != AESDRV_UTILS_DMA_CHANNEL_NOT_SET)
  {
    DMADRV_FreeChannel(dmaConfig->dmaChOut);
  }
  dmaConfig->dmaChIn = AESDRV_UTILS_DMA_CHANNEL_NOT_SET;
  dmaConfig->dmaChOut = AESDRV_UTILS_DMA_CHANNEL_NOT_SET;
}

/***************************************************************************//**
 * Function initializes DMADRV for CRYPTO and allocates two channels.
 *
 * @details
 *  Function assumes that same buffer is used for input and output (in place).
 *  Additionally, it supports packet authentication&encryption where first part
 *  of the packet is only authenticated (CCM,GCM).
 *
 * @return
 *  OK when DMA channel allocation completed, OUT_OF_RESOURCES if not
 *
 ******************************************************************************/
static Ecode_t aesdrvDmaInit( AESDRV_Context_t* pAesdrvContext )
{
  Ecode_t retval;
  AESDRV_DmaConfig_t* dmaConfig = &pAesdrvContext->ioModeSpecific.dmaConfig;
  
  retval = DMADRV_Init();
  if ( retval == ECODE_EMDRV_DMADRV_ALREADY_INITIALIZED || 
       retval == ECODE_EMDRV_DMADRV_OK ) 
  {
    retval = ECODE_OK;
  }
  else
  {
    return MBEDTLS_ECODE_AESDRV_OUT_OF_RESOURCES;
  }
  
  // Allocate first DMA channel
  retval = DMADRV_AllocateChannel(&dmaConfig->dmaChIn, NULL);
  if ( retval != ECODE_EMDRV_DMADRV_OK )
  {
    return MBEDTLS_ECODE_AESDRV_OUT_OF_RESOURCES;
  }
  // Allocate second DMA channel
  retval = DMADRV_AllocateChannel(&dmaConfig->dmaChOut, NULL);
  if ( retval != ECODE_EMDRV_DMADRV_OK ) {
    DMADRV_FreeChannel(dmaConfig->dmaChIn);
    dmaConfig->dmaChIn = AESDRV_UTILS_DMA_CHANNEL_NOT_SET;
    return MBEDTLS_ECODE_AESDRV_OUT_OF_RESOURCES;
  }

  return ECODE_OK;
}

/**
 * Function setup DMA for CRYPTO.
 *
 * @details
 *  Function assumes that same buffer is used for input and output (in place).
 *  Additionally, it supports packet authentication&encryption where first part
 *  of the packet is only authenticated (CCM,GCM).
 *
 * @param authDataLen
 *  Length of authentication part.
 *
 * @param textLength
 *  Length of encrypted part.
 *
 * @warning
 *  Function is modifying CRYPTO_SEQCTRL and CRYPTO_SEQCTRLB registers. It must
 *  be ensured that access to those registers is done in
 *  correct order and settings are not overwritten.
 *
 */
static void aesdrvDmaSetup(AESDRV_Context_t* pAesdrvContext,
                            uint8_t const*   pData,
                            uint32_t         authDataLength,
                            uint32_t         textLength)
{
  uint8_t * _authData = (uint8_t*)pData;
  uint8_t * _textData = (uint8_t*)((uint32_t)pData + authDataLength);
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  AESDRV_DmaConfig_t* dmaConfig = &pAesdrvContext->ioModeSpecific.dmaConfig;
  uint16_t lenIn, lenOut;
  uint32_t seqctrl;
    
  /* Ensure that dma for crypto was initialized. */
  EFM_ASSERT((dmaConfig->dmaChIn != AESDRV_UTILS_DMA_CHANNEL_NOT_SET) &&
             (dmaConfig->dmaChOut != AESDRV_UTILS_DMA_CHANNEL_NOT_SET));

  crypto->CTRL &= (~_CRYPTO_CTRL_DMA0RSEL_MASK) & (~_CRYPTO_CTRL_DMA1RSEL_MASK);
  seqctrl  = crypto->SEQCTRL;
  seqctrl &= (~_CRYPTO_SEQCTRL_DMA0SKIP_MASK) & (~_CRYPTO_SEQCTRL_DMA0PRESA_MASK);
  seqctrl |= (uint32_t)_authData % 4 << _CRYPTO_SEQCTRL_DMA0SKIP_SHIFT;
  crypto->SEQCTRL = seqctrl;
  crypto->SEQCTRLB |= CRYPTO_SEQCTRLB_DMA0PRESB;
  aesdrvDmaAddrLenGet((uint8_t **)&_authData,
                       (uint8_t **)&_textData,
                       authDataLength,
                       textLength,
                       &lenIn, &lenOut);
  
  DMADRV_MemoryPeripheral( dmaConfig->dmaChIn,
#ifdef EMDRV_DMADRV_LDMA
                           pAesdrvContext->cryptodrvContext.device->dmaReqSigChIn,
#else
#error "UDMA + CRYPTO is a non-exisiting combination"
#endif
                           (void*)&crypto->DATA0,
                           (void *)_authData,
                           true,
                           lenIn / 4,
                           dmadrvDataSize4,
                           NULL,
                           NULL );
  
  if (textLength)
  {
    DMADRV_PeripheralMemory( dmaConfig->dmaChOut,
                             pAesdrvContext->cryptodrvContext.device->dmaReqSigChOut,
                             (void *)_textData,
                             (void*)&crypto->DATA0,
                             true,
                             lenOut / 4,
                             dmadrvDataSize4,
                             NULL,
                             NULL );
  }
}

/**
 * Function calculates length transfers for reading to crypto and from crypto.
 *
 * @details
 *  Function takes into account address alignment for input and output buffer.
 *  It overwrites provided addresses applying alignment.
 *
 * @param pBufIn
 *  Address of input buffer. Will be modified by the function if not word
 *  aligned.
 *
 * @param pBufOut
 *  Address of output buffer. Will be modified by the function if not word
 *  aligned.
 *
 * @param lengthA
 *  Length of authentication part of CCM.
 * @param lengthB
 *  Length of payload to be encrypted.
 * @return
 */
static void aesdrvDmaAddrLenGet(uint8_t**  pBufIn,
                                uint8_t**  pBufOut,
                                uint16_t   lengthA,
                                uint16_t   lengthB,
                                uint16_t*  dmaLengthIn,
                                uint16_t*  dmaLengthOut)
{
  uint8_t* pIn     = *pBufIn;
  uint8_t* pOut    = *pBufOut;
  uint8_t  moduloA = (uint32_t)pIn % 4;
  uint8_t  moduloB = (uint32_t)pOut % 4;

  *dmaLengthIn  = lengthA+lengthB;
  *dmaLengthOut = lengthB;

  pIn -= moduloA;
  *dmaLengthIn += moduloA;
  *dmaLengthIn = (*dmaLengthIn+3) & 0xFFFC;

  pOut -= moduloB;
  *dmaLengthOut += moduloB;
  *dmaLengthOut = (*dmaLengthOut+3) & 0xFFFC;

  *pBufIn = pIn;
  *pBufOut = pOut;

  return;
}

#endif /* #if defined( MBEDTLS_INCLUDE_IO_MODE_DMA ) */

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
