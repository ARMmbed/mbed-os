/**
 * \file aesdrv.h
 *
 * \brief Definitions for AES based ciphers with CRYPTO hw acceleration 
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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

#ifndef __SILICON_LABS_AESDRV_H
#define __SILICON_LABS_AESDRV_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "em_device.h"
#if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) || \
  ( defined(AES_COUNT) && (AES_COUNT > 0) )

#include "mbedtls_ecode.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************* ERROR CODES  **********************************
 ******************************************************************************/

#define MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED         (MBEDTLS_ECODE_AESDRV_BASE | 0x1)
#define MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED (MBEDTLS_ECODE_AESDRV_BASE | 0x2)
#define MBEDTLS_ECODE_AESDRV_OUT_OF_RESOURCES      (MBEDTLS_ECODE_AESDRV_BASE | 0x3)
#define MBEDTLS_ECODE_AESDRV_INVALID_PARAM         (MBEDTLS_ECODE_AESDRV_BASE | 0x4)

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

/** Enum defines which data I/O mode to use for moving data to/from the
    AES/CRYPTO hardware module. */
typedef enum
{
  aesdrvIoModeCore,  /** Core CPU moves data to/from the data registers. */
                        
  aesdrvIoModeBufc,  /** Buffer Controller moves data to/from the CRYPTO
                         data registers. */
  
  aesdrvIoModeDma    /** DMA moves data to/from the CRYPTO data registers. */
} AESDRV_IoMode_t;

/** DMA I/O mode specific configuration structure. */
typedef struct
{
  unsigned int dmaChIn;  /**< DMA input channel. Allocated by AESDRV. */
  unsigned int dmaChOut; /**< DMA output channel. Allocated by AESDRV. */
} AESDRV_DmaConfig_t;

#if defined(BUFC_PRESENT)
/** BUFC I/O mode specific configuration structure. */
typedef struct
{
  uint8_t bufId;    /**< BUFC buffer id. Must be setup by user. */
} AESDRV_BufcConfig_t;
#endif
  
/** Data I/O mode specific configuration structure. */
typedef union
{
  AESDRV_DmaConfig_t  dmaConfig;
#if defined(BUFC_PRESENT)
  AESDRV_BufcConfig_t bufcConfig;
#endif
} AESDRV_IoModeSpecific_t;

/** Prototype of counter callback function provided by user. */
typedef void (*AESDRV_CtrCallback_t)(uint8_t *ctr);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  
/***************************************************************************//**
 * @brief
 *  AESDRV asynchronous (non-blocking) operation completion callback function.
 *
 * @details
 *  The callback function is called when an asynchronous (non-blocking)
 *  AES operation has completed.
 *
 * @param[in] result
 *  The result of the AES operation.
 *
 * @param[in] userArgument
 *  Optional user defined argument supplied when starting the asynchronous
 *  AES operation.
 ******************************************************************************/
typedef void (*AESDRV_AsynchCallback_t)(int result, void* userArgument);

/** AESDRV cipher modes, used to specify type of asynchronous context. */
typedef enum
{
  cipherModeNone,
  cipherModeBlockCipher, /**< Basic AES block cipher modes (CBC, CFB etc.) */
  cipherModeCcm,         /**< CCM and CCM* - Counter Mode with CBC-MAC */
  cipherModeCcmBle,      /**< CCM optimized for BLE. */
  cipherModeCmac,        /**< Cipher-based Message Authentication Code -
                            variation of CBC-MAC, equivalent to OMAC1 */
  cipherModeGcm          /**< Galois/Counter mode including message
                            authentication code (GMAC).*/
} AESDRV_CipherMode_t;

/** AESDRV Context structures.*/

/** Context structure for asynchronous basic AES block cipher operations. */
typedef struct
{
  unsigned int               remainingBlocks;
  uint32_t*                  pBlockIn;
  uint32_t*                  pBlockOut;
  uint8_t*                   pInitialVector;
  AESDRV_AsynchCallback_t    asynchCallback;
  void*                      asynchCallbackArgument;
} AESDRV_BlockCipherAsynchContext_t;

/** Context structure for asynchronous CCM(*) and GCM operations. */
typedef struct
{
  const uint8_t*             pHdr;
  const uint8_t*             pDataInput;
  uint8_t*                   pDataOutput;
  uint32_t                   la;
  uint32_t                   lm;
  uint32_t                   hdrLength;
  uint32_t                   dataLength;
  uint8_t*                   pAuthTag;
  uint8_t                    authTagLength;
  bool                       encryptingHeader;
  bool                       encrypt;
  AESDRV_AsynchCallback_t    asynchCallback;
  void*                      asynchCallbackArgument;
} AESDRV_CCM_AsynchContext_t, AESDRV_GCM_AsynchContext_t;
  
/** Context structure for asynchronous CMAC operations. */
typedef struct
{
  uint32_t*                  dataPointer;
  uint32_t                   dataBlocks;
  uint32_t                   lastBlock[4];
  uint8_t*                   digest;
  uint16_t                   digestLengthBits;
  bool                       encrypt;
  AESDRV_AsynchCallback_t    asynchCallback;
  void*                      asynchCallbackArgument;
} AESDRV_CMAC_AsynchContext_t;

#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
#if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) )
#include "cryptodrv.h"

/* Main AESDRV context structure. */
typedef struct
{
  CRYPTODRV_Context_t     cryptodrvContext; /**< CRYPTO driver context */
  AESDRV_IoMode_t         ioMode;           /**< Data I/O mode. */
  AESDRV_IoModeSpecific_t ioModeSpecific;   /**< Data I/O mode specific
                                               config. */
  bool                    authTagOptimize;  /**< Enable/disable optimized 
                                               handling of authentication
                                               tag in CCM/GCM. Tag
                                               optimization expects tag size
                                               0,4,8,12 or 16 bytes.*/
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  
  AESDRV_CipherMode_t     cipherMode;       /**< Asynch mode */
  void*                   pAsynchContext;   /**< Asynch context */
  
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
} AESDRV_Context_t;
  
#elif ( defined(AES_COUNT) && (AES_COUNT > 0) )

typedef struct
{
  uint32_t                ccmCounter[4];    /**< CCM counter */
} AESDRV_Context_t;
  
#endif

#ifdef __cplusplus
}
#endif

#endif /* #if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) || \
          ( defined(AES_COUNT) && (AES_COUNT > 0) ) */

#endif /* __SILICON_LABS_AESDRV_H */
