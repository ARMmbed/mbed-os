/*
 *  AES block cipher algorithms implementation using AES hw module
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
/*
 * This is an implementation of block cipher function using the AES hw
 * module for acceleration.
 *
 * The module implements following interfaces:
 *
 * AESDRV_CBCx()
 * AESDRV_CFBx()
 * AESDRV_CTRx()
 * AESDRV_DecryptKey128()
 * AESDRV_DecryptKey256()
 * AESDRV_ECBx()
 * AESDRV_OFBx()
 *
 * Functions are mostly wrappers of emlib AES (em_aes.h) since block cipher
 * functionality is provided by emlib.
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
#include "em_aes.h"
#include <string.h>

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*
 *  Generate 128 bit decryption key from 128 bit encryption key. The decryption
 *  key is used for some cipher modes when decrypting.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_DecryptKey128(AESDRV_Context_t* pAesdrvContext,
                             uint8_t*          out,
                             const uint8_t*    in)
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
  AESDRV_CLOCK_ENABLE;
  AES_DecryptKey128(out,in);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
}

/*
 *  Generate 256 bit decryption key from 256 bit encryption key. The decryption
 *  key is used for some cipher modes when decrypting.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_DecryptKey256(AESDRV_Context_t* pAesdrvContext,
                             uint8_t*          out,
                             const uint8_t *   in)
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
#if defined( AES_CTRL_AES256 )
  AESDRV_CLOCK_ENABLE;
  AES_DecryptKey256(out,in);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
#else
  (void) out; (void) in;
  return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
}

/*
 * Cipher-block chaining (CBC) cipher mode encryption/decryption, 128 bit key.
 * Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CBC128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
  uint8_t tmpIv[16];
  if (false == encrypt)
  {
    if (len>=16)
      memcpy(tmpIv, &in[len-16], 16);
  }
  AESDRV_CLOCK_ENABLE;
  AES_CBC128(out,in,len,key,iv,encrypt);
  AESDRV_CLOCK_DISABLE;
  if (len>=16)
  {
    if (encrypt)
      memcpy(iv, &out[len-16], 16);
    else
      memcpy(iv, tmpIv, 16);
  }
  return ECODE_OK;
}

 /*
  * Cipher-block chaining (CBC) cipher mode encryption/decryption, 256 bit key.
  * Please refer to aesdrv.h for detailed description.
  */
Ecode_t AESDRV_CBC256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
#if defined( AES_CTRL_AES256 )
  uint8_t tmpIv[16];
  if (false == encrypt)
  {
    if (len>=16)
      memcpy(tmpIv, &in[len-16], 16);
  }

  AESDRV_CLOCK_ENABLE;
  AES_CBC256(out,in,len,key,iv,encrypt);
  AESDRV_CLOCK_DISABLE;

  if (len>=16)
  {
    if (encrypt)
      memcpy(iv, &out[len-16], 16);
    else
      memcpy(iv, tmpIv, 16);
  }
  return ECODE_OK;
#else
  (void) out; (void) in; (void) len; (void) key; (void) iv; (void) encrypt;
  return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
}

/*
 *  Cipher feedback (CFB) cipher mode encryption/decryption, 128 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CFB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
  uint8_t tmpIv[16];
  if (false == encrypt)
  {
    if (len>=16)
      memcpy(tmpIv, &in[len-16], 16);
  }

  AESDRV_CLOCK_ENABLE;
  AES_CFB128(out,in,len,key,iv,encrypt);
  AESDRV_CLOCK_DISABLE;

  if (len>=16)
  {
    if (encrypt)
      memcpy(iv, &out[len-16], 16);
    else
      memcpy(iv, tmpIv, 16);
  }
  return ECODE_OK;
}

/*
 *  Cipher feedback (CFB) cipher mode encryption/decryption, 256 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CFB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
#if defined( AES_CTRL_AES256 )
  uint8_t tmpIv[16];
  if (false == encrypt)
  {
    if (len>=16)
      memcpy(tmpIv, &in[len-16], 16);
  }

  AESDRV_CLOCK_ENABLE;
  AES_CFB256(out,in,len,key,iv,encrypt);
  AESDRV_CLOCK_DISABLE;

  if (len>=16)
  {
    if (encrypt)
      memcpy(iv, &out[len-16], 16);
    else
      memcpy(iv, tmpIv, 16);
  }
  return ECODE_OK;
#else
  (void) out; (void) in; (void) len; (void) key; (void) iv; (void) encrypt;
  return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
}

/*
 *  Counter (CTR) cipher mode encryption/decryption, 128 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CTR128(AESDRV_Context_t*    pAesdrvContext,
                      uint8_t*             out,
                      const uint8_t*       in,
                      unsigned int         len,
                      const uint8_t*       key,
                      uint8_t*             ctr,
                      AESDRV_CtrCallback_t ctrCallback
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
  AESDRV_CtrCallback_t pCtrFunc;

  if (ctrCallback)
  {
    pCtrFunc = ctrCallback;
  }
  else
  {
    pCtrFunc = AES_CTRUpdate32Bit;
  }

  AESDRV_CLOCK_ENABLE;
  AES_CTR128(out,in,len,key,ctr,pCtrFunc);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
}

/*
 *  Counter (CTR) cipher mode encryption/decryption, 256 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CTR256(AESDRV_Context_t*    pAesdrvContext,
                      uint8_t*             out,
                      const uint8_t*       in,
                      unsigned int         len,
                      const uint8_t*       key,
                      uint8_t*             ctr,
                      AESDRV_CtrCallback_t ctrCallback
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
#if defined( AES_CTRL_AES256 )
  AESDRV_CtrCallback_t pCtrFunc;

  if (ctrCallback)
  {
    pCtrFunc = ctrCallback;
  }
  else
  {
    pCtrFunc = AES_CTRUpdate32Bit;
  }

  AESDRV_CLOCK_ENABLE;
  AES_CTR256(out,in,len,key,ctr,pCtrFunc);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
#else
  (void) out; (void) in; (void) len; (void) key; (void) ctr; (void) ctrCallback;
  return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
}

/*
 *  Electronic Codebook (ECB) cipher mode encryption/decryption, 128 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_ECB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      bool              encrypt
                      )
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
  AESDRV_CLOCK_ENABLE;
  AES_ECB128(out,in,len,key,encrypt);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
}

/*
 *  Electronic Codebook (ECB) cipher mode encryption/decryption, 256 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_ECB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      bool              encrypt)
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
#if defined( AES_CTRL_AES256 )
  AESDRV_CLOCK_ENABLE;
  AES_ECB256(out,in,len,key,encrypt);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
#else
  (void) out; (void) in; (void) len; (void) key; (void) encrypt;
  return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
}

/*
 *  Output feedback (OFB) cipher mode encryption/decryption, 128 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_OFB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv)
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
  AESDRV_CLOCK_ENABLE;
  AES_OFB128(out,in,len,key,iv);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
}

/*
 *  Output feedback (OFB) cipher mode encryption/decryption, 256 bit key.
 *  Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_OFB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv)
{
  (void) pAesdrvContext;  /* The pAesdrvContext parameter is not used for
                             basic AES block cipher mode. */
#if defined( AES_CTRL_AES256 )
  AESDRV_CLOCK_ENABLE;
  AES_OFB256(out,in,len,key,iv);
  AESDRV_CLOCK_DISABLE;
  return ECODE_OK;
#else
  (void) out; (void) in; (void) len; (void) key; (void) iv;
  return MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED;
#endif
}

#endif /* #if defined(AES_COUNT) && (AES_COUNT > 0) */
