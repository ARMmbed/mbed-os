/*
 *  Common functions for AES based algorithms for the AES hw module.
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

#if defined(AES_COUNT) && (AES_COUNT > 0)

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "aesdrv_internal.h"
#include "aesdrv_common_aes.h"
#include "em_assert.h"
#include <string.h>

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*
 *   Initializes an AESDRV context structure.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_Init(AESDRV_Context_t* pAesdrvContext)
{
  /* Clear the driver context. */
  memset(pAesdrvContext, 0, sizeof(AESDRV_Context_t));
  return ECODE_OK;
}

/*
 *   DeInitializes an AESDRV context structure.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_DeInit(AESDRV_Context_t* pAesdrvContext)
{
  /* Clear the driver context. */
  memset(pAesdrvContext, 0, sizeof(AESDRV_Context_t));
  return ECODE_OK;
}

/*
 *   Set the AES encryption key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_SetKey(AESDRV_Context_t* pAesdrvContext,
                      const uint8_t*    pKey,
                      uint32_t          keyLength)
{
  (void) pAesdrvContext; /* Key is stored directly in AES hw module and
                            not set in context structure. */
  Ecode_t retval = ECODE_OK;
  const uint32_t * const _pKey = (const uint32_t *)pKey;

  if (pKey == NULL)
  {
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  }

  /* Enable AES clock. */
  AESDRV_CLOCK_ENABLE;

  /* Load key */
#if defined( AES_CTRL_AES256 )
  if (32==keyLength)
  {
    int i, j;
    for (i = 3, j = 7; i >= 0; i--, j--)
    {
      AES->KEYLA = __REV(_pKey[j]);
      AES->KEYHA = __REV(_pKey[i]);
    }
  }
  else
#endif
  {
    if (16==keyLength)
    {
      int i;
#if defined( AES_CTRL_KEYBUFEN )
      AES->CTRL |= AES_CTRL_KEYBUFEN;
      /* Load key into high key for key buffer usage */
      for (i = 3; i >= 0; i--)
      {
        AES->KEYHA = __REV(_pKey[i]);
      }
#else
      for (i = 3; i >= 0; i--)
      {
        AES->KEYLA = __REV(_pKey[i]);
      }
#endif
    }
    else
    {
      retval = MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
    }
  }

  /* Disable AES clock. */
  AESDRV_CLOCK_DISABLE;
  
  return retval;
}

/*
 *   Set the AES device instance.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_SetDeviceInstance(AESDRV_Context_t*  pAesdrvContext,
                                 unsigned int       devno)
{
  (void) pAesdrvContext; /* Multiple instances not supported for AES module. */
  
  if (devno >= AES_COUNT)
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  else
    return ECODE_OK;
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
  (void) pAesdrvContext;  /* Asynch mode not supported for AES module. */
  (void) cipherMode;  /* Asynch mode not supported for AES module. */
  (void) asynchCallback;  /* Asynch mode not supported for AES module. */
  (void) asynchCallbackArgument;  /* Asynch mode not supported for AES module. */

  if (pAsynchContext)
  {
    /* The EFM32 AES implementation does not support asynchronous mode yet. */
    return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
  }
  else
  {
    return ECODE_OK;
  }
}

#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

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
  (void) pAesdrvContext;  /* Not supported for AES module. */
  (void) ioModeSpecific;  /* Not supported for AES module. */
  if (ioMode == aesdrvIoModeCore)
  {
    return ECODE_OK;
  }
  else
  {
    return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
  }
}

#endif /* #if defined(AES_COUNT) && (AES_COUNT > 0) */
