/*
 *  AES block cipher algorithms implementation using the CRYPTO hw module.
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
 * This is an implementation of block cipher function using CRYPTO hw module.
 *
 * The module implements following public interfaces:
 * AESDRV_CBCx()
 * AESDRV_CFBx()
 * AESDRV_CTRx()
 * AESDRV_DecryptKey128()
 * AESDRV_DecryptKey256()
 * AESDRV_ECBx()
 * AESDRV_OFBx()
 */

#include "em_device.h"

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "aesdrv_internal.h"
#include "aesdrv_common_crypto.h"
#include "cryptodrv_internal.h"
#include "em_assert.h"
#include "em_crypto.h"

/*******************************************************************************
 *******************************   MACROS   ************************************
 ******************************************************************************/
#define AES_BLOCKSIZE    16

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/

/** AESDRV list of instruction used by block cipher modes. It is limited to
 * 8 instruction (2 SEQ registers) because none of cipher modes requires more
 * to execute. */
typedef const uint8_t AESDRV_BlockCipherInstrSeq_t[8];

/*******************************************************************************
 **************************   LOCAL FUNCTIONS   ********************************
 ******************************************************************************/
static inline Ecode_t aesdrvProcessLoopMCU
(
 AESDRV_Context_t* pAesdrvContext,
 uint32_t          len,
 const uint8_t*    in,
 uint8_t*          out
 );
static inline Ecode_t aesdrvProcessLoopHW
(
 AESDRV_Context_t*  pAesdrvContext
 );
static void aesdrvBlockCipherPrepare
(
 AESDRV_Context_t*             pAesdrvContext,
 const uint8_t*                key,
 const uint8_t*                iv,
 CRYPTO_KeyWidth_TypeDef       keyWidth,
 AESDRV_BlockCipherInstrSeq_t* instrCode,
 AESDRV_IoMode_t               ioMode
 );
static void aesdrvBlockCipherHwSetup
(
 AESDRV_Context_t* pAesdrvContext,
 uint32_t          len,
 uint32_t *        inout
 );
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
static void aesdrvAsynchCallback
(
 void* asynchCallbackArgument
 );
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
static Ecode_t aesdrvBlockCipher
(
 AESDRV_Context_t*             pAesdrvContext,
 uint8_t*                      out,
 const uint8_t*                in,
 unsigned int                  len,
 const uint8_t*                key,
 uint8_t*                      iv,
 CRYPTO_KeyWidth_TypeDef       keyWidth,
 AESDRV_BlockCipherInstrSeq_t* instrCode
 );
static Ecode_t aesdrvDecryptKey
(
 AESDRV_Context_t*             pAesdrvContext,
 uint8_t*                      out,
 const uint8_t*                in,
 CRYPTO_KeyWidth_TypeDef       keyWidth
 );

/*******************************************************************************
 **************************  STATIC VARIABLES  *********************************
 ******************************************************************************/

/** CRYPTO sequencer instruction for CBC encryption.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_CBC_Encrypt[] = {
  /* MCU */
  {
    CRYPTO_CMD_INSTR_DATA1TODATA0XOR,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TODATA1,
    CRYPTO_CMD_INSTR_DATA0TODATA2
  },

  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA1TODATA0XOR,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA1TODATA0XOR,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  }
#endif
};

/** CRYPTO sequencer instruction for CBC decryption.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_CBC_Decrypt[] =
{
  /*MCU*/
  {
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_AESDEC,
    CRYPTO_CMD_INSTR_DATA1TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA3TODATA1,
    CRYPTO_CMD_INSTR_DATA0TODATA2
  },
  /*BUFC*/
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_AESDEC,
    CRYPTO_CMD_INSTR_DATA1TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_DATA2TODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /*DMA*/
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_AESDEC,
    CRYPTO_CMD_INSTR_DATA1TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_DATA2TODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  }
#endif
};

/** CRYPTO sequencer instruction for CFB encryption.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_CFB_Encrypt[] =
{
  /* MCU */
  {
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_DATA0TODATA1
  }
#endif
};

/** CRYPTO sequencer instruction for CFB decryption.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_CFB_Decrypt[] =
{
  /* MCU */
  {
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_DATA3TODATA1
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_DATA3TODATA1
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_DATA3TODATA1
  }
#endif
};

/** CRYPTO sequencer instruction for CTR.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_CTR[] =
{
  /*MCU*/
  {
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA1INC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TODATA2
  },
  /*BUFC*/
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA1INC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_END
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /*DMA*/
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA1INC,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_END
  }
#endif
};

/** CRYPTO sequencer instruction for ECB encryption.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_ECB_Encrypt[] =
{
  /* MCU */
  {
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  }
#endif
};

/** CRYPTO sequencer instruction for ECB decryption.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_ECB_Decrypt[] =
{
  /* MCU */
  {
    CRYPTO_CMD_INSTR_AESDEC,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_AESDEC,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_AESDEC,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  }
#endif
};

/** CRYPTO sequencer instruction for OFB.
 * There are 3 variants: MCU,BUFC,DMA. */
static AESDRV_BlockCipherInstrSeq_t aesdrv_OFB[] =
{
  /* MCU */
  {
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TODATA1,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TODATA2,
    CRYPTO_CMD_INSTR_END,
    CRYPTO_CMD_INSTR_END
  },
  /* BUFC */
  {
    CRYPTO_CMD_INSTR_BUFTODATA0,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TODATA1,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATA0TOBUF,
    CRYPTO_CMD_INSTR_END
  },
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  /* DMA */
  {
    CRYPTO_CMD_INSTR_DMA0TODATA,
    CRYPTO_CMD_INSTR_DATA0TODATA3,
    CRYPTO_CMD_INSTR_DATA1TODATA0,
    CRYPTO_CMD_INSTR_AESENC,
    CRYPTO_CMD_INSTR_DATA0TODATA1,
    CRYPTO_CMD_INSTR_DATA3TODATA0XOR,
    CRYPTO_CMD_INSTR_DATATODMA0,
    CRYPTO_CMD_INSTR_END
  }
#endif
};

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*
 *   Cipher-block chaining (CBC) cipher mode encryption/decryption, 128 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CBC128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t * instr = encrypt ?
    &aesdrv_CBC_Encrypt[ioMode] : &aesdrv_CBC_Decrypt[ioMode];
  
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, iv, cryptoKey128Bits, instr);
}

/*
 *   Cipher-block chaining (CBC) cipher mode encryption/decryption, 256 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CBC256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t * instr = encrypt ?
    &aesdrv_CBC_Encrypt[ioMode] : &aesdrv_CBC_Decrypt[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, iv, cryptoKey256Bits, instr);
}

/*
 *   Cipher feedback (CFB) cipher mode encryption/decryption, 128 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CFB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t * instr = encrypt ?
    &aesdrv_CFB_Encrypt[ioMode] : &aesdrv_CFB_Decrypt[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, iv, cryptoKey128Bits, instr);
}

/*
 *   Cipher feedback (CFB) cipher mode encryption/decryption, 256 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CFB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t * instr = encrypt ?
    &aesdrv_CFB_Encrypt[ioMode] : &aesdrv_CFB_Decrypt[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, iv, cryptoKey256Bits, instr);
}

/*
 *   Counter (CTR) cipher mode encryption/decryption, 128 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CTR128(AESDRV_Context_t*    pAesdrvContext,
                      uint8_t*             out,
                      const uint8_t*       in,
                      unsigned int         len,
                      const uint8_t*       key,
                      uint8_t*             ctr,
                      AESDRV_CtrCallback_t ctrCallback)
{
  (void)ctrCallback; /* We do not support user specified counter callback
                        for CRYPTO.*/
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t *instr = &aesdrv_CTR[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, ctr, cryptoKey128Bits, instr);
}

/*
 *   Counter (CTR) cipher mode encryption/decryption, 256 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_CTR256(AESDRV_Context_t*    pAesdrvContext,
                      uint8_t*             out,
                      const uint8_t*       in,
                      unsigned int         len,
                      const uint8_t*       key,
                      uint8_t*             ctr,
                      AESDRV_CtrCallback_t ctrCallback)
{
  (void) ctrCallback;  /* The ctrCallback parameter is not supported for
                          devices with CRYPTO.*/
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t *instr = &aesdrv_CTR[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, ctr, cryptoKey256Bits, instr);
}

/*
 *   Electronic Codebook (ECB) cipher mode encryption/decryption, 128 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_ECB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      bool              encrypt)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t *instr =
    encrypt ? &aesdrv_ECB_Encrypt[ioMode] : &aesdrv_ECB_Decrypt[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, 0, cryptoKey128Bits, instr);
}

/*
 *   Electronic Codebook (ECB) cipher mode encryption/decryption, 256 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_ECB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      bool              encrypt)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t *instr =
    encrypt ? &aesdrv_ECB_Encrypt[ioMode] : &aesdrv_ECB_Decrypt[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, 0, cryptoKey256Bits, instr);
}

/*
 *   Output feedback (OFB) cipher mode encryption/decryption, 128 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_OFB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t *instr = &aesdrv_OFB[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, iv, cryptoKey128Bits, instr);
}

/*
 *   Output feedback (OFB) cipher mode encryption/decryption, 256 bit key.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_OFB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv)
{
  AESDRV_IoMode_t ioMode = pAesdrvContext->ioMode;
  AESDRV_BlockCipherInstrSeq_t *instr = &aesdrv_OFB[ioMode];
  return aesdrvBlockCipher(pAesdrvContext,
                           out, in, len, key, iv, cryptoKey256Bits, instr);
}

/*
 *   Generate 128 bit decryption key from 128 bit encryption key. The decryption
 *   key is used for some cipher modes when decrypting.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_DecryptKey128(AESDRV_Context_t* pAesdrvContext,
                             uint8_t*                out,
                             const uint8_t*          in)
{
  return aesdrvDecryptKey(pAesdrvContext, out, in, cryptoKey128Bits);
}


/*
 *   Generate 256 bit decryption key from 256 bit encryption key. The decryption
 *   key is used for some cipher modes when decrypting.
 *   Please refer to aesdrv.h for detailed description.
 */
Ecode_t AESDRV_DecryptKey256(AESDRV_Context_t* pAesdrvContext,
                             uint8_t*                out,
                             const uint8_t *         in)
{
  return aesdrvDecryptKey(pAesdrvContext, out, in, cryptoKey256Bits);
}

/***************************************************************************//**
 * @brief
 *   Generate decryption key from encryption key. The decryption
 *   key is used for some cipher modes when decrypting.
 *
 * @details
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES context structure.
 *
 * @param[out] out
 *   Buffer to place 128/256 bit decryption key. Must be at least 16/32 bytes
 *   long. It may be set equal to @p in, in which case the input buffer is
 *   overwritten.
 *
 * @param[in] in
 *   Buffer holding 128/256 bit encryption key. Must be at least 32 bytes long.
 *
 * @param[in] keyWidth
 *   Key width - 128 or 256 bits.
 ******************************************************************************/
static Ecode_t aesdrvDecryptKey(AESDRV_Context_t*       pAesdrvContext,
                                uint8_t*                out,
                                const uint8_t*          in,
                                CRYPTO_KeyWidth_TypeDef keyWidth)
{
  uint32_t*           _out = (uint32_t *) out;
  const uint32_t*     _in  = (const uint32_t *) in;
  Ecode_t              status, retval;
  CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;
  CRYPTO_TypeDef*      crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  EFM_ASSERT(((uint32_t)_in&0x3)==0);
  EFM_ASSERT(((uint32_t)_out&0x3)==0);

  status = CRYPTODRV_Arbitrate(pCryptodrvContext);
  if (ECODE_OK != status)
    return status;

  status = CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  if (ECODE_OK == status)
  {

    CRYPTO_KeyBufWrite(crypto, (uint32_t *)_in, keyWidth);

    CRYPTO_EXECUTE_1(crypto, CRYPTO_CMD_INSTR_AESENC);

    CRYPTO_KeyRead(crypto, _out, keyWidth);

    status = CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  }
  retval = CRYPTODRV_Release(pCryptodrvContext);

  return ECODE_OK == retval ? status : retval;
}

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)

/***************************************************************************//**
 * @brief
 *   Callback function for asynchronous operation.
 *
 * @param[in] asynchCallbackArgument
 *   Callback argument.
 ******************************************************************************/
static void aesdrvAsynchCallback (void* asynchCallbackArgument)
{
  AESDRV_Context_t* pAesdrvContext =
    (AESDRV_Context_t*) asynchCallbackArgument;

  if (pAesdrvContext)
  {
    AESDRV_BlockCipherAsynchContext_t* pAsynchContext =
      (AESDRV_BlockCipherAsynchContext_t*) pAesdrvContext->pAsynchContext;
    CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

    if (pAsynchContext->remainingBlocks)
    {
      /* Save encrypted/decrypted data */
      CRYPTODRV_DataReadUnaligned(&crypto->DATA2, (uint8_t*)pAsynchContext->pBlockOut);
    
      pAsynchContext->pBlockOut += 4;
      pAsynchContext->pBlockIn  += 4;
      pAsynchContext->remainingBlocks--;
    }
  
    if (pAsynchContext->remainingBlocks)
    {
      /* Load data and trigger encryption */
      CRYPTODRV_DataWriteUnaligned(&crypto->DATA0, (uint8_t*)pAsynchContext->pBlockIn);
      CRYPTO_InstructionSequenceExecute(crypto);
    }
    else
    {
      Ecode_t  status;
      uint8_t* pInitialVector = pAsynchContext->pInitialVector;
      CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;

      /* Turn off interrupts. */
      CRYPTODRV_SetAsynchCallback(pCryptodrvContext, 0, 0);

      /* Read the last counter value from DATA1 to user the counter pointer. */
      if (pInitialVector)
      {
        CRYPTODRV_DataReadUnaligned(&crypto->DATA1, pInitialVector);
      }

      CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
      
      status = CRYPTODRV_Release(pCryptodrvContext);
    
      /* Finally call the user callback */
      if (pAsynchContext->asynchCallback)
      {
        pAsynchContext->asynchCallback((int)status,
                                       pAsynchContext->asynchCallbackArgument);
      }
    }
  }
}

#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

/***************************************************************************//**
 * @brief
 *   Generic function for performing AES block ciphers.
 *
 * @details
 *   Block cipher mode is identified by CRYPTO sequencer code
 *   provided. Each sequencer code assumes that:
 *   - IV/CTR is loaded into DATA1
 *   - Input is loaded into DATA0
 *   - Output is read from DATA2
 *
 * @param[out] out
 *   Buffer to place 128/256 bit decryption key. Must be at least 16/32 bytes
 *   long. It may be set equal to @p in, in which case the input buffer is
 *   overwritten.
 *
 * @param[in] in
 *   Buffer holding 128/256 bit encryption key. Must be at least 32 bytes long.
 *
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   256 bit encryption key.
 *
 * @param[in] iv
 *   128 bit initalization vector to use.
 *
 * @param[in] keyWidth
 *   Key width - 128 or 256 bits.
 *
 * @param[in] instrCode
 *   CRYPTO sequencer instructions.
 ******************************************************************************/
static Ecode_t aesdrvBlockCipher
(
 AESDRV_Context_t*             pAesdrvContext,
 uint8_t*                      out,
 const uint8_t*                in,
 unsigned int                  len,
 const uint8_t*                key,
 uint8_t*                      iv,
 CRYPTO_KeyWidth_TypeDef       keyWidth,
 AESDRV_BlockCipherInstrSeq_t* instrCode
 )
{
  Ecode_t              status, retval;
  CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_BlockCipherAsynchContext_t* pAsynchContext =
    (AESDRV_BlockCipherAsynchContext_t*) pAesdrvContext->pAsynchContext;
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  CRYPTO_TypeDef*      crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  AESDRV_IoMode_t      ioMode = pAesdrvContext->ioMode;

  EFM_ASSERT(!(len % AES_BLOCKSIZE));

  status = CRYPTODRV_Arbitrate(pCryptodrvContext);
  if (ECODE_OK != status)
    return status;
  
  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  aesdrvBlockCipherPrepare(pAesdrvContext, key, iv, keyWidth, instrCode, ioMode);
  CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  /* If asynchronous non-blocking mode, register completion callback */
  if (pAsynchContext)
  {
    pAsynchContext->pInitialVector = iv;
    CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
    CRYPTODRV_SetAsynchCallback (pCryptodrvContext,
                                 aesdrvAsynchCallback, pAesdrvContext);
    CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  if (ioMode == aesdrvIoModeCore)
  {
    retval = aesdrvProcessLoopMCU(pAesdrvContext, len, in, out);
  }
  else
  {
    EFM_ASSERT(in==out);
    CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
    aesdrvBlockCipherHwSetup(pAesdrvContext, len, (uint32_t*)in);
    CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
    retval = aesdrvProcessLoopHW(pAesdrvContext);
  }

  /* Release CRYPTO if blocking mode. */
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  if (0 == pAsynchContext)
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
    /* If the 'iv' pointer is set, read last iv value. */
    if (iv)
    {
      CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
      CRYPTODRV_DataReadUnaligned(&crypto->DATA1, iv);
      CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
    }

    status = CRYPTODRV_Release(pCryptodrvContext);
    if (ECODE_OK != status)
      retval = status;
  }
  
  return retval;
}

/***************************************************************************//**
 * @brief
 *   Generic function for blockCipher preparation. It sets initial value in
 *   DATA1 register, loads the key to KEYBUF and loads CRYPTO sequence
 *   instructions. It is independent of I/O mode used.
 *
 * @param[in] key
 *   128/256 bit encryption key.
 *
 * @param[in] iv
 *   128 bit initalization vector to use.
 *
 * @param[in] keyWidth
 *   Key width - 128 or 256 bits.
 *
 * @param[in] instrCode
 *   CRYPTO sequencer instructions.
 ******************************************************************************/
static void aesdrvBlockCipherPrepare
(
 AESDRV_Context_t*             pAesdrvContext,
 const uint8_t*                key,
 const uint8_t*                iv,
 CRYPTO_KeyWidth_TypeDef       keyWidth,
 AESDRV_BlockCipherInstrSeq_t* instrCode,
 AESDRV_IoMode_t               ioMode
 )
{
  const uint32_t* _instr = (const uint32_t *)instrCode;
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;

  /* Setup CRYPTO for basic AES block cipher operation:
     - width of counter in CTR cipher mode to 4 bytes.
  */
  if (instrCode == &aesdrv_CTR[ioMode])
  {
    crypto->CTRL = CRYPTO_CTRL_INCWIDTH_INCWIDTH4;
  }
  else
  {
    crypto->CTRL = 0;
  }
  crypto->SEQCTRL  = 0;
  crypto->SEQCTRLB = 0;
  
  if (key)
  {
    CRYPTO_KeyBufWrite( crypto, (uint32_t *)key, keyWidth );
  }

  if (iv)
  {
    CRYPTODRV_DataWriteUnaligned( &crypto->DATA1, iv );
  }

  crypto->SEQ0 = _instr[0];
  crypto->SEQ1 = _instr[1];
}

/***************************************************************************//**
 * @brief
 *   Function setups hw I/O mode (BUFC or DMA) and setup sequence length in
 *   CRYPTO. In case of DMA channels are configured and in case of BUFC buffer
 *   is setup.
 *
 * @param[in] len
 *   Number of bytes to process.
 *
 * @param[in] inout
 *   Pointer to input data. This location is also used to store the output.
 *
 ******************************************************************************/
static void aesdrvBlockCipherHwSetup
(
 AESDRV_Context_t* pAesdrvContext,
 uint32_t          len,
 uint32_t*         inout
 )
{
  CRYPTO_TypeDef* crypto = pAesdrvContext->cryptodrvContext.device->crypto;
  EFM_ASSERT(len<=_CRYPTO_SEQCTRL_LENGTHA_MASK);
  crypto->SEQCTRL = len;
  AESDRV_HwIoSetup(pAesdrvContext, (uint8_t*)inout, 0, len);
}

/***************************************************************************//**
 * @brief
 *   Process loop function to be used in MCU mode.
 *
 * @details
 *   Please refer to general comments on layout and byte ordering of parameters.
 *   Function is processing requested number of blocks. Data transfers are
 *   handled by MCU.
 *
 * @param[in] len
 *   Length in bytes.
 *
 * @param[in] in
 *   Input buffer.
 *
 * @param[out] out
 *   Output buffer.
 *
 * @return Error code
 ******************************************************************************/
static inline Ecode_t aesdrvProcessLoopMCU
(
 AESDRV_Context_t* pAesdrvContext,
 uint32_t          len,
 const uint8_t*    in,
 uint8_t*          out
 )
{
  CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;
  CRYPTO_TypeDef*      crypto            = pCryptodrvContext->device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_BlockCipherAsynchContext_t* pAsynchContext =
    (AESDRV_BlockCipherAsynchContext_t*) pAesdrvContext->pAsynchContext;
  
  if (pAsynchContext)
  {
    pAsynchContext->remainingBlocks = len / AES_BLOCKSIZE;
    pAsynchContext->pBlockIn        = (uint32_t*)in;
    pAsynchContext->pBlockOut       = (uint32_t*)out;

    CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);

    /* Load data and trigger encryption */
    CRYPTODRV_DataWriteUnaligned(&crypto->DATA0, in);

    CRYPTO_InstructionSequenceExecute(crypto);
  }
  else
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
    len /= AES_BLOCKSIZE;
    while (len--)
    {
      CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
      crypto->SEQCTRL = 16 << _CRYPTO_SEQCTRL_LENGTHA_SHIFT;
      
      /* Load data and trigger encryption */
      CRYPTODRV_DataWriteUnaligned(&crypto->DATA0, in);
      
      CRYPTO_InstructionSequenceExecute(crypto);
      
      /* Save encrypted/decrypted data */
      CRYPTODRV_DataReadUnaligned(&crypto->DATA2, out);
      
      CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);

      out += 16;
      in  += 16;
    }

  }
  return ECODE_OK;
}

/***************************************************************************//**
 * @brief
 *   Process loop function to be used in HW I/O mode.
 *
 * @details
 *   Please refer to general comments on layout and byte ordering of parameters.
 *   Function is processing requested number of blocks. Data transfers are
 *   handled by BUFC or DMA.
 *
 * @return Error code
 ******************************************************************************/
static inline Ecode_t aesdrvProcessLoopHW
(
 AESDRV_Context_t*     pAesdrvContext
 )
{
  CRYPTODRV_Context_t* pCryptodrvContext = &pAesdrvContext->cryptodrvContext;
  CRYPTO_TypeDef*      crypto            = pCryptodrvContext->device->crypto;
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  AESDRV_BlockCipherAsynchContext_t* pAsynchContext =
    (AESDRV_BlockCipherAsynchContext_t*) pAesdrvContext->pAsynchContext;
  
  if (pAsynchContext)
  {
    pAsynchContext->remainingBlocks = 0;
  }
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

  CRYPTODRV_EnterCriticalRegion(pCryptodrvContext);
  
  CRYPTO_InstructionSequenceExecute(crypto);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  if (0 == pAsynchContext)
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  {
    CRYPTO_InstructionSequenceWait(crypto);
    CRYPTODRV_ExitCriticalRegion(pCryptodrvContext);
  }
  return ECODE_OK;
}

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
