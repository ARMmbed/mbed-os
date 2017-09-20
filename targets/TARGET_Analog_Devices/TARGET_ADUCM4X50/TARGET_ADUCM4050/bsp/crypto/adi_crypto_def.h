/*!
 *****************************************************************************
 @file:    adi_crypto_def.h
 @brief:   Crypto Device Driver definitions for ADuCM4x50 processor
 -----------------------------------------------------------------------------
Copyright (c) 2012-2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.
THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/
#ifndef ADI_CRYPTO_DEF_H
#define ADI_CRYPTO_DEF_H

/*! \cond PRIVATE */

#include <drivers/dma/adi_dma.h>
#include <adi_crypto_config.h>

/* pick up compiler-specific alignment directives */
#include <drivers/general/adi_drivers_general.h>
#define ALIGN4 ALIGNED_PRAGMA(4)

/* Support Check MACROS */
#define CRYPTO_SUPPORT_KEY_REQUIRED  (                                                  \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)    \
                                            )

#define CRYPTO_SUPPORT_MODE_CCM_ONLY       (                                             \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT  != 1)     \
                                            && (ADI_CRYPTO_ENABLE_CTR_SUPPORT  != 1)     \
                                            && (ADI_CRYPTO_ENABLE_CBC_SUPPORT  != 1)     \
                                            && (ADI_CRYPTO_ENABLE_CCM_SUPPORT  == 1)     \
                                            && (ADI_CRYPTO_ENABLE_CMAC_SUPPORT != 1)     \
                                            && (ADI_CRYPTO_ENABLE_SHA_SUPPORT  != 1)     \
                                            )

#define CRYPTO_SUPPORT_MODE_ANY_NON_CCM    (                                             \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CBC_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_SHA_SUPPORT  == 1)     \
                                            )

#define CRYPTO_SUPPORT_MODE_ANY_NON_SHA    (                                             \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CBC_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CCM_SUPPORT  == 1)     \
                                            )

/* define local MIN/MAX macros, if not already... */
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* various size macros */
#define MAX_CRYPTO_DMA_BYTES        (DMA_TRANSFER_LIMIT * sizeof(uint32_t))

/* SHA hardware max chunk size attributes */
#define SHA_CHUNK_MAX_BYTES         (64u)
#define SHA_CHUNK_MAX_BITS          (SHA_CHUNK_MAX_BYTES * 8U)
#define SHA_CHUNK_MAX_WORDS         (16u)

#define FIFO_WIDTH_IN_BITS          (32u)
#define FIFO_WIDTH_IN_BYTES         (FIFO_WIDTH_IN_BITS/8u)
#define FIFO_DEPTH                  (4u)

#define CRYPTO_INPUT_SIZE_IN_BITS   (128u)
#define CRYPTO_INPUT_SIZE_IN_BYTES  (CRYPTO_INPUT_SIZE_IN_BITS/8u)

#define SHA_OUTPUT_SIZE_IN_BITS     (256u)
#define SHA_OUTPUT_SIZE_IN_BYTES    (SHA_OUTPUT_SIZE_IN_BITS/8u)


/* MAKE SURE THIS STRUCT REMAINS *******PERFECTLY ALIGNED******* WITH USER
   ADI_CRYPTO_TRANSACTION BECAUSE WE USE BCOPY TO INITIALIZE EACH NEW SUBMIT!

   Internal compute structure reflecting mostly, user ADI_CRYPTO_TRANSACTION,
   except for moving data pointers and remaining counts.  Contents initialized
   directly from from ADI_CRYPTO_TRANSACTION during buffer submit.
*/
typedef struct _CRYPTO_COMPUTE {
	ADI_CRYPTO_CIPHER_MODE    eCipherMode;              /*!< Cipher mode to use */
    ADI_CRYPTO_CODING_MODE    eCodingMode;              /*!< Coding Mode (Encryption or Decryption) */

    ADI_CRYPTO_KEY_BYTE_SWAP  eKeyByteSwap;             /*!< KEY endianness */
    ADI_CRYPTO_SHA_BYTE_SWAP  eShaByteSwap;             /*!< SHA endianness */
    ADI_CRYPTO_AES_BYTE_SWAP  eAesByteSwap;             /*!< AES endianness */

    uint8_t                  *pKey;                     /*!< Pointer to the key data pre-formatted as a byte array, according to eAesKeyLen. */
    ADI_CRYPTO_AES_KEY_LEN    eAesKeyLen;               /*!< The length of the key */

    uint32_t                 *pNextAuthInput;           /* CCM mode: pointer to user prefix buffer */
    uint32_t                  numAuthBytesRemaining;    /* Length of the prefix buffer in bytes (should be a multiple of 16 bytes) */

    uint32_t                 *pNextInput;               /* Pointer to next user 32-bit input location */
    uint32_t                  numInputBytesRemaining;   /* Number of input bytes remaining */

    uint32_t                 *pNextOutput;              /* Pointer to next user 32-bit output location */
    uint32_t                  numOutputBytesRemaining;  /* Number of output bytes remaining */

    uint8_t                  *pNonceIV;                 /*!< Pointer to user 16-byte array containing one of three values, depending on cipher mode:
                                                                 CTR mode = 108-bit NONCE
                                                                 CCM mode = 112-bit NONCE
                                                                 CBC mode = 128-bit IV (Initialization Vector)
                                                             NONCE and IV assume little endian format, for example: CTR NONCE packing is:
                                                                NONCE[0]            ->   7:0
                                                                NONCE[1]            ->  15:8
                                                                  ...
                                                                NONCE[13]           -> 103:96
                                                                NONCE[14](Bits 3:0) -> 107:104 */
    uint32_t                  CounterInit;              /*!< CTR/CCM mode: Counter Initialization Value (CTR=20-bit, CCM=16-bit) */
    uint32_t                  numValidBytes;            /*!< CCM mode: Number of valid bytes in the last (padding) block (1-16) */
    uint32_t                  numShaBitsRemaining;      /*!< SHA mode: Number of bits remaining in the SHA payload, which may be odd-sized */
} CRYPTO_COMPUTE;


/* Crypto device attributes */
typedef struct _CRYPTO_INFO {
    ADI_CRYPT_TypeDef          *pDev;				/* Pointer to physical Crypto controller */
    ADI_CRYPTO_HANDLE           hDevice;			/* Device Handle */
#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    IRQn_Type                   dmaInputIrqNum;
    IRQn_Type                   dmaOutputIrqNum;
    DMA_CHANn_TypeDef           dmaInputChanNum;
    DMA_CHANn_TypeDef           dmaOutputChanNum;
    volatile ADI_CRYPTO_RESULT  dmaError;			/* DMA error collector. */
#endif
} CRYPTO_INFO;


#ifdef __ICCARM__
/*
* Pm123 (RULE 8.5) there shall be no definition of objects or functions in a header file.
* Exception is to allow the Crypto device data type and instance to be declared simultaniously.
*/
#pragma diag_suppress=Pm123
#endif /* __ICCARM__ */

/* Crypto driver internal data */
struct __ADI_CRYPTO_DEV_DATA_TYPE {
    bool                     bDeviceEnabled;         /* Boolean flag to signify whether the device is enable/disabled */
    bool                     bDmaEnabled;            /* Boolean flag to signify whether the DMA is enable/disabled */
    bool                     bCompletion;            /* Boolean flag to signify whether a transaction is complete */

    ADI_CRYPT_TypeDef       *pDev;                   /* Pointer to physical Crypto controller */

    CRYPTO_INFO             *pDevInfo;               /* access to device info */

    CRYPTO_COMPUTE           Computation;            /* Active computation structure */

	ADI_CRYPTO_TRANSACTION  *pUserBuffer;            /* saved user buffer pointer from submit */
    ADI_CALLBACK             pfCallback;             /* User defined callback function */
    void                    *pCBParam;               /* User defined callback param */
    ADI_CRYPTO_RESULT        dmaErrorCode;           /* saved DMA error code to return via user API */


    SEM_VAR_DECLR        /* Blocking object abstraction: "Semaphore" for rtos,  "bLowPowerExitFlag" for non-rtos, etc.    */
} ADI_CRYPTO_DEV_DATA_TYPE;

#ifdef __ICCARM__
#pragma diag_default=Pm123
#endif /* __ICCARM__ */

/*! \endcond */

#endif /* ADI_CRYPTO_DEF_H */
