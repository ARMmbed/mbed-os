/*
 *  Authenticated encryption algorithms implementation using AES hw module.
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

#if defined( AES_CTRL_KEYBUFEN )

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "aesdrv_internal.h"
#include "aesdrv_authencr.h"
#include "aesdrv_common_aes.h"
#include "em_aes.h"
#include "em_assert.h"
#include "string.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/
static Ecode_t aesdrv_CCM_MICCompute(AESDRV_Context_t* pAesdrvContext,
                                     const uint8_t*    pDataInput,
                                     const uint32_t    dataLength,
                                     const uint8_t*    pHdr,
                                     const uint32_t    hdrLength,
                                     const uint8_t*    pAuthTag,
                                     const uint8_t     authTagLength,
                                     const bool        encryptedPayload,
                                     const bool        encrypt);
static inline void aesdrv_CCM_CtrInit(const uint8_t* pNonce,
                                      uint32_t*      ctr,
                                      bool           ctrToZero);
static inline void aesdrv_CCM_Nonce(uint32_t* ccmCounter,
                                    uint32_t* ctr0,
                                    uint8_t   authTagLength,
                                    uint32_t  la,
                                    uint32_t  lm);
static inline void aesdrv_CCM_AMICCompute(const uint8_t* pHdr,
                                          uint32_t       hdrLength);
static inline void aesdrv_CCM_PMICCompute(const uint8_t* pDataInput,
                                          uint32_t       length);
static inline void aesdrv_CCM_CTR128(uint32_t* ccmCounter,
                                     uint32_t* out,
                                     uint32_t* in,
                                     uint32_t  loopLength);
static inline void aesdrv_CCM_CTR128Single(uint32_t* ccmCounter,
                                           uint32_t* out,
                                           uint32_t* in);
static inline void aesdrv_CCM_DataWrite(const uint32_t*  in);
static inline void aesdrv_CCM_DataRead(uint32_t*         out);
static inline void aesdrv_CCM_DataRevRead(uint32_t*      out);
static inline void aesdrv_CCM_XorDataWrite(const uint32_t*    in);
static inline void aesdrv_CCM_XorDataRevWrite(const uint32_t* in);

/** @endcond */

/*******************************************************************************
 ***************************   GLOBAL FUNCTIONS   ******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *  CCM optimized for BLE
 *
 * @details
 *  This function is an implementation of CCM optimized for Bluetooth Low Energy
 *  (BLE). This function assumes fixed header size (1 byte),
 *  fixed authentication tag (4bytes), fixed length field size (2 bytes)
 *
 * @param pData
 *  Pointer to data
 *
 * @param dataLength
 *  length of data (max. 27)
 *
 * @param hdr
 *  1 byte header
 *
 * @param pKey
 *  10 byte Security Key. If pKey is NULL, the current key will be used.
 *
 * @param pNonce
 *  13 byte nonce
 *
 * @param encrypt
 *  true - encrypt
 *  false - decrypt
 *
 * @return
 *   ECODE_OK if success. Error code if failure.
 *   Encryption will always succeed.
 *   Decryption may fail if the authentication fails.
 */
Ecode_t AESDRV_CCMBLE(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          pData,
                      const uint32_t    dataLength,
                      uint8_t           hdr,
                      const uint8_t*    pKey,
                      const uint8_t*    pNonce,
                            uint8_t*    pAuthTag,
                      const bool        encrypt
                      )
{
  return AESDRV_CCM(pAesdrvContext,
                    pData, pData, dataLength,
                    &hdr,         1,
                    pKey,         128/8, /* keyLength */
                    pNonce,       13,
                    pAuthTag,     4,
                    encrypt);
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

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
  int32_t i;
  uint8_t lastBlock[16];
  uint32_t lastBlockLen;
  uint32_t wholeBlockLen;
  Ecode_t status = ECODE_OK;
  const uint32_t * const _pKey = (const uint32_t *)pKey;

  if ( (keyLength != 128/8) ||
       (nonceLength != 13) ||
       (dataLength > CCM_MAX_DATA_LENGTH) )
  {
    return MBEDTLS_ECODE_AESDRV_INVALID_PARAM;
  }
  
  /* Enable AES clock. */
  AESDRV_CLOCK_ENABLE;

  AES->CTRL = AES_CTRL_KEYBUFEN | AES_CTRL_XORSTART | AES_CTRL_DATASTART;

  /* Load key into high key for key buffer usage */
  for (i = 3; i >= 0; i--)
  {
    AES->KEYHA = __REV(_pKey[i]);
  }

  /* Compute counter and store in CCM counter in context structure. */
  aesdrv_CCM_CtrInit(pNonce,
                     pAesdrvContext->ccmCounter,
                     authTagLength && encrypt);
  if (authTagLength)
  {
    if (encrypt)
    {
      status =
        aesdrv_CCM_MICCompute(pAesdrvContext,
                              pDataInput, dataLength,
                              pHdr,       hdrLength,
                              pAuthTag,   authTagLength,
                              encryptedPayload,
                              encrypt);
    }
  }

  /* Encrypt / decrypt data */
  if (encryptedPayload)
  {
    lastBlockLen = dataLength%16;
    wholeBlockLen = dataLength/16;

    /* Handle whole blocks first. Last one needs zero padding if it is smaller
     * than block size (16 bytes).*/
    if (wholeBlockLen)
    {
      aesdrv_CCM_CTR128(pAesdrvContext->ccmCounter,
                        (uint32_t*)pDataOutput,
                        (uint32_t*)pDataInput,
                        wholeBlockLen);
    }

    if (lastBlockLen)
    {
      aesdrv_CCM_CTR128Single(pAesdrvContext->ccmCounter,
                              (uint32_t*)lastBlock,
                              (uint32_t*)&pDataInput[dataLength-lastBlockLen]);
      (void) memcpy(&pDataOutput[dataLength-lastBlockLen],
                    lastBlock,
                    lastBlockLen);
    }
  }

  /* Compute authentication part for decryption */
  if (!encrypt && authTagLength)
  {
    aesdrv_CCM_CtrInit(pNonce, pAesdrvContext->ccmCounter, !encrypt);
    /* Compute the authentication tag MACTag from decrypted data */
    status = aesdrv_CCM_MICCompute(pAesdrvContext,
                                   pDataOutput,      dataLength,
                                   pHdr,             hdrLength,
                                   pAuthTag,         authTagLength,
                                   encryptedPayload,
                                   encrypt);
    if (MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == status)
    {
      memset(pDataOutput, 0, dataLength);
    }
  }

  /* Disable AES clock. */
  AESDRV_CLOCK_DISABLE;
  return status;
}

/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/

/**
 * Function calculates CCM MIC. Function assumes that CTR IV counter is already
 * initialized. In case of encryption calculated tag is appended to pData. In
 * case of decryption calculated tag is compared against tag located at the end
 * of pData.
 *
 * @param pDataInput
 *  A pointer to PlainText.
 *
 * @param dataLength
 *  PlainText Length.
 *
 * @param pHdr
 *  A pointer to AuthData.
 *
 * @param hdrLength
 *  AuthData length.
 *
 * @param authTagLength
 *  Length of AuthenticationTag
 *
 * @param encryptedPayload
 *  true - authentication and encryption
 *  false - authentication only
 *
 * @param encrypt
 *  true - encryption
 *  false - decryption
 *
 * @return Error code
 */
static Ecode_t aesdrv_CCM_MICCompute(AESDRV_Context_t* pAesdrvContext,
                                     const uint8_t*    pDataInput,
                                     const uint32_t    dataLength,
                                     const uint8_t*    pHdr,
                                     const uint32_t    hdrLength,
                                     const uint8_t*    pAuthTag,
                                     const uint8_t     authTagLength,
                                     const bool        encryptedPayload,
                                     const bool        encrypt)
{
  uint32_t tmpBuf[4];
  uint32_t tmpCtrl;
  uint32_t *pTag = (uint32_t*)pAuthTag;
  uint32_t lm;
  uint32_t la;
  Ecode_t  status = ECODE_OK;

  if (encryptedPayload)
  {
    lm = dataLength;
    la = hdrLength;
  }
  else
  {
    la = hdrLength + dataLength;
    lm = 0;
  }

  aesdrv_CCM_Nonce(pAesdrvContext->ccmCounter, tmpBuf, authTagLength, la, lm);

  /* Calculate authenticaton part of MIC. */
  if (la)
  {      
    aesdrv_CCM_AMICCompute(pHdr, la);
  }
  
  /* Calculate plaintext part of MIC. */
  aesdrv_CCM_PMICCompute(pDataInput, lm);

  /* Disable AES functionality - auto start after writing to XORDATA. Writing
   * to XORDATA will just do XOR. */
  tmpCtrl = AES->CTRL;
  AES->CTRL = tmpCtrl & (~AES_CTRL_XORSTART);
  
  /* AES_DATA register contains MIC which is not CTR encrypted. Xor already
   * calculated CTR cipher block with clear MIC. After that operation AES_DATA
   * contains proper MIC.*/
  aesdrv_CCM_XorDataWrite(tmpBuf);
  
  /* Reenable auto AES start after writing to XORDATA. */
  AES->CTRL = tmpCtrl;
  
  /* Read out 16 byte long authentication tag. */
  aesdrv_CCM_DataRevRead(tmpBuf);

  if (encrypt)
  {
    /* In case of encryption, copy authentication tag to packet. */
    memcpy(pTag,tmpBuf,authTagLength);
  }
  else
  {
    /* In case of decryption compare provided authentication tag with one
     * which was calculated. */
    if (memcmp(pTag,tmpBuf,authTagLength))
    {
      status = MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED;
    }
  }

  return status;
}

/**
 * @brief
 *  Function initializes counter. It can initialize counter to 0 or 1 based on
 *  function parameter.
 *
 * @param pNonce
 *  A pointer to 13 bytes long nonce.
 *
 * @param ctr
 *  A pointer to 16 byte CTR counter.
 *
 * @param ctrToZero
 *  true - counter initialized to 0
 *  false - counter initialized to 1
 */
static inline void aesdrv_CCM_CtrInit(const uint8_t* pNonce,
                                      uint32_t* ctr,
                                      bool ctrToZero)
{
  uint32_t ctrValue = ctrToZero ? 0 : 0x01000000;

  /* CCM Counter consists of:
   * flags - 1 byte
   * Nonce - 13 bytes
   * counter - 2 bytes
   * Counter is initialized taking into account that AES is working with reversed
   *  byte order. */
  ctr[0] = __REV(ctrValue | *(uint16_t *)(&pNonce[11]));
  ctr[1] = __REV(*(uint32_t *)(&pNonce[7]));
  ctr[2] = __REV(*(uint32_t *)(&pNonce[3]));
  ctr[3] = __REV((2-1) | (*(uint32_t *)(&pNonce[0]) << 8));
}

static inline void aesdrv_CCM_DataWrite(const uint32_t* in)
{
  AES->DATA = in[0];
  AES->DATA = in[1];
  AES->DATA = in[2];
  AES->DATA = in[3];
}

static inline void aesdrv_CCM_DataRead(uint32_t * out)
{
  out[0] = AES->DATA;
  out[1] = AES->DATA;
  out[2] = AES->DATA;
  out[3] = AES->DATA;
}

static inline void aesdrv_CCM_DataRevRead(uint32_t * out)
{
  out[3] = __REV(AES->DATA);
  out[2] = __REV(AES->DATA);
  out[1] = __REV(AES->DATA);
  out[0] = __REV(AES->DATA);
}

static inline void aesdrv_CCM_XorDataWrite(const uint32_t* in)
{
  AES->XORDATA = in[0];
  AES->XORDATA = in[1];
  AES->XORDATA = in[2];
  AES->XORDATA = in[3];
}

static inline void aesdrv_CCM_XorDataRevWrite(const uint32_t* in)
{
  AES->XORDATA = __REV(in[3]);
  AES->XORDATA = __REV(in[2]);
  AES->XORDATA = __REV(in[1]);
  AES->XORDATA = __REV(in[0]);
}

/**
 * Function prepares IV value for CBC according to CCM spec and calculates
 * counter key to be XOR'ed with authentication tag.
 *
 * @param ctr0
 *  A pointer to memory where encrypted counter will be stored. It is later used
 *  to encrypt authentication tag.
 *
 * @param authTagLength
 *  Length of Authentication tag. Can be 0,4,8,16
 *
 * @param lm
 *  Lm parameter from CCM spec.
 *
 */
static inline void aesdrv_CCM_Nonce(uint32_t* ccmCounter,
                                    uint32_t* ctr0,
                                    uint8_t   authTagLength,
                                    uint32_t  la,
                                    uint32_t  lm)
{
  uint8_t flags;
  uint8_t lenEnc;
  
  /* Counter is written to AES, AES starts encryption. While AES is processing
   * first block for CCM CBC (B0) can be prepared utilizing similarities between
   * CTR counter and B0.
   */
  aesdrv_CCM_DataWrite(ccmCounter);

  /* Prepare flags to form B0 block. */
  flags = la ? 0x41 : 1;
  if (authTagLength)
  {
    flags |= ((authTagLength-2)/2)<<3;
  }

  /* Modify CTR counter to form B0. */
  ccmCounter[0] |= lm;
  lenEnc = ccmCounter[3] >> 24;
  ccmCounter[3] &= 0x00FFFFFF;
  ccmCounter[3] |= (flags << 24);

  while (AES->STATUS & AES_STATUS_RUNNING)
         ;

  /* Read out encrypted counter to be used to encrypt authentication tag. */
  aesdrv_CCM_DataRead(ctr0);

  /* Process block B0. */
  aesdrv_CCM_DataWrite(ccmCounter);

  /* Transform back B0 to CTR counter. Setting counter to 1. It will be later on
   * used when payload will be encrypted. It's done while waiting for AES to
   * complete B0 encryption.
   */
  ccmCounter[3] &= 0x00FFFFFF;
  ccmCounter[3] |= lenEnc<<24;
  ccmCounter[0] &= 0xFFFF0000;
  ccmCounter[0] |= 0x00000001;

  while (AES->STATUS & AES_STATUS_RUNNING)
       ;
}

/**
 * Function calculates a Auth part of CBC Authentication tag. Once function
 * terminates AES->DATA register contains partial authentication tag.
 *
 * @param pHdr
 *  A pointer to AuthData.
 *
 * @param hdrLength
 *  Length of AuthData
 */
static inline void aesdrv_CCM_AMICCompute(const uint8_t * pHdr, uint32_t hdrLength)
{
  bool firstBlock = true;
  uint16_t length = hdrLength+2;
  uint8_t * _pData = (uint8_t *)pHdr;
  uint32_t tmp = 0;
  uint32_t padBuf[4];

  /* Prepare first block of AuthData - starting with L(a) field. */
  if (length > 16)
  {
    hdrLength = __REV16(hdrLength);
    tmp = __REV(hdrLength | ((*(uint16_t*)_pData)<<16));
    _pData -= 2;
  }

  while (length)
  {
    if (length > 16)
    {
      /* For each AuthData block perform CBC. Last word (first in CCM spec
       * byte order) is different for first block (contains length field). It's
       * handled in loop prolog. */
      AES->XORDATA = __REV(((uint32_t *)_pData)[3]);
      AES->XORDATA = __REV(((uint32_t *)_pData)[2]);
      AES->XORDATA = __REV(((uint32_t *)_pData)[1]);
      AES->XORDATA = tmp;

      _pData +=16;
      length -= 16;

      firstBlock = false;
      /* Clear padBuf while waiting for AES completion - to be used in last,
       * incomplete block. */
      memset(padBuf,0,16);

      /* Prepare in advance last word for next block - it is done while waiting
       * for AES completion. */
      tmp = __REV(((uint32_t *)_pData)[0]);

      while (AES->STATUS & AES_STATUS_RUNNING)
             ;
    }
    else
    {
      if (firstBlock)
      {
        /* In case first block is also last block length field L(a) must be
         * concatenated with AuthData. */
        memset(padBuf,0,16);
        padBuf[0] = __REV16(hdrLength);
        memcpy( &((uint8_t*)padBuf)[2], _pData, length-2);
      }
      else
      {
        /* Copy last portion of data to zero padded buffer.*/
        memcpy(padBuf,_pData,length);
      }

      aesdrv_CCM_XorDataRevWrite(padBuf);
      length = 0;

      while (AES->STATUS & AES_STATUS_RUNNING)
             ;
    }
  }
}

/**
 * Function calculates PlainText part of CBC Authentication tag.
 *
 * @param pDataInput
 *  A pointer to PlainText
 *
 * @param length
 *  Length of PlainText
 */
static inline void aesdrv_CCM_PMICCompute(const uint8_t * pDataInput, uint32_t length)
{
  uint32_t padBuf[4];
  uint32_t * _pDataInput = (uint32_t *)pDataInput;

  while (length)
  {
    /* First process all full blcoks. */
    if (length > 16)
    {
      aesdrv_CCM_XorDataRevWrite(_pDataInput);

      _pDataInput +=4;
      length -= 16;
      while (AES->STATUS & AES_STATUS_RUNNING)
             ;
    }
    else
    {
      /* Zero pad last, incomplete block. */
      memset(padBuf,0,16);
      memcpy(padBuf,_pDataInput,length);
      aesdrv_CCM_XorDataRevWrite(padBuf);
      length = 0;

      while (AES->STATUS & AES_STATUS_RUNNING)
             ;
    }
  }
}

/**
 * Function performs CTR with 128bit AES on multiple blocks.
 *
 * @param out
 *  A pointer to output data.
 *
 * @param in
 *  A pointer to input data.
 *
 * @param loopLength
 *  Number of blocks
 */
static inline void aesdrv_CCM_CTR128(uint32_t* ccmCounter,
                                     uint32_t* out,
                                     uint32_t* in,
                                     uint32_t  loopLength)
{
  uint32_t ctrProduct[4];

  memset(ctrProduct,0,16);

  /* Encrypt counter */
  aesdrv_CCM_DataWrite(ccmCounter);
  /* Increment counter during encryption. */
  ccmCounter[0]++;
  while (AES->STATUS & AES_STATUS_RUNNING);
  aesdrv_CCM_DataRead(ctrProduct);

  while (--loopLength)
  {
    aesdrv_CCM_DataWrite(ccmCounter);

    /* Perform XOR with CTR product of previous block while waiting for AES
     * completion. */
    out[0] = in[0] ^ __REV(ctrProduct[3]);
    out[1] = in[1] ^ __REV(ctrProduct[2]);
    out[2] = in[2] ^ __REV(ctrProduct[1]);
    out[3] = in[3] ^ __REV(ctrProduct[0]);
    
    /* Increment counter */
    ccmCounter[0]++;

    while (AES->STATUS & AES_STATUS_RUNNING)
             ;

    aesdrv_CCM_DataRead(ctrProduct);

    out += 4;
    in  += 4;
  }

  /* Perform XOR with CTR product */
  out[0] = in[0] ^ __REV(ctrProduct[3]);
  out[1] = in[1] ^ __REV(ctrProduct[2]);
  out[2] = in[2] ^ __REV(ctrProduct[1]);
  out[3] = in[3] ^ __REV(ctrProduct[0]);
}

/**
 * Function performs 128bit AES CTR on Single block.
 *
 * @param out
 *  A pointer to output data.
 *
 * @param in
 *  A pointer to input data.
 */
static inline void aesdrv_CCM_CTR128Single(uint32_t* ccmCounter,
                                           uint32_t* out,
                                           uint32_t* in)
{

  aesdrv_CCM_DataWrite(ccmCounter);

  ccmCounter[0]++;

  while (AES->STATUS & AES_STATUS_RUNNING)
           ;

  out[3] = in[3] ^ __REV(AES->DATA);
  out[2] = in[2] ^ __REV(AES->DATA);
  out[1] = in[1] ^ __REV(AES->DATA);
  out[0] = in[0] ^ __REV(AES->DATA);
}
/** @endcond */

#endif /* #if defined( AES_CTRL_KEYBUFEN ) */

#endif /* #if defined(AES_COUNT) && (AES_COUNT > 0) */
