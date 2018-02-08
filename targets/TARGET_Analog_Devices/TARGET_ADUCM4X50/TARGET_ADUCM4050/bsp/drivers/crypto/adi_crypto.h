/*! *****************************************************************************
 * @file    adi_crypto.h
 * @brief   Main include file for CRYPTO Device driver definitions
 -----------------------------------------------------------------------------
Copyright (c) 2010-2016 Analog Devices, Inc.

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


/** @addtogroup Crypto_Driver Crypto Driver
*  @{
*/

#ifndef ADI_CRYPTO_H
#define ADI_CRYPTO_H

 /*! \cond PRIVATE */
#include <adi_processor.h>
#include <adi_callback.h>
#include <rtos_map/adi_rtos_map.h>  /* for ADI_SEM_SIZE */
/*! \endcond */
#include <adi_crypto_config.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \enum ADI_CRYPTO_RESULT
 * Crypto API return codes
 */
typedef enum
{
    ADI_CRYPTO_SUCCESS = 0,              /*!<  No Error, API suceeded.                                   */
    ADI_CRYPTO_ERR_ALREADY_INITIALIZED,  /*!<  Crypto is already initialized.                            */
    ADI_CRYPTO_ERR_BAD_BUFFER,           /*!<  Invalid buffer parameters.                                */
    ADI_CRYPTO_ERR_BAD_CONFIG,           /*!<  Invalid device config parameters passed.                  */
    ADI_CRYPTO_ERR_BAD_DEVICE_NUM,       /*!<  Invalid device instance number.                           */
    ADI_CRYPTO_ERR_BAD_DEV_HANDLE,       /*!<  Invalid device handle passed.                             */
    ADI_CRYPTO_ERR_COMPUTE_ACTIVE,       /*!<  Computation underway.                                     */
    ADI_CRYPTO_ERR_DMA_BUS_FAULT,        /*!<  Runtime DMA bus fault detected.                           */
    ADI_CRYPTO_ERR_DMA_INVALID_DESCR,    /*!<  Runtime DMA invalid descriptor detected.                  */
    ADI_CRYPTO_ERR_DMA_REGISTER,         /*!<  Error registering DMA error callback function.            */
    ADI_CRYPTO_ERR_DMA_UNKNOWN_ERROR,    /*!<  Unknown runtime DMA error detected.                       */
    ADI_CRYPTO_ERR_INSUFFICIENT_MEM,     /*!<  Insufficient memory passed to the driver.                 */
    ADI_CRYPTO_ERR_INVALID_PARAM,        /*!<  Invalid function parameter.                               */
    ADI_CRYPTO_ERR_INVALID_STATE,        /*!<  Operation failed since the device is in an invalid state. */
    ADI_CRYPTO_ERR_SEMAPHORE_FAILED,     /*!<  Failure in semaphore functions.                           */
} ADI_CRYPTO_RESULT;

/*!
 * \enum ADI_CRYPTO_EVENT
 * Crypto callback events
 */
typedef enum
{
    /* successful buffer completion events */
    ADI_CRYPTO_EVENT_STATUS_CBC_DONE,        /*!<  CBC operation is complete.         */
    ADI_CRYPTO_EVENT_STATUS_CCM_DONE,        /*!<  CCM operation is complete.         */
    ADI_CRYPTO_EVENT_STATUS_CMAC_DONE,       /*!<  CMAC operation is complete.        */
    ADI_CRYPTO_EVENT_STATUS_CTR_DONE,        /*!<  CTR operation is complete.         */
    ADI_CRYPTO_EVENT_STATUS_ECB_DONE,        /*!<  ECB operation is complete.         */
    ADI_CRYPTO_EVENT_STATUS_HMAC_DONE,       /*!<  HMAC operation is complete.        */
    ADI_CRYPTO_EVENT_STATUS_SHA_DONE,        /*!<  SHA operation is complete.         */

    /* other events */
    ADI_CRYPTO_EVENT_DMA_BUS_ERROR,          /*!<  DMA bus error encountered.         */
    ADI_CRYPTO_EVENT_DMA_DESCRIPTOR_ERROR,   /*!<  DMA descriptor error encountered.  */
    ADI_CRYPTO_EVENT_DMA_UNKNOWN_ERROR,      /*!<  DMA unknown error encountered.     */
    ADI_CRYPTO_EVENT_STATUS_INPUT_OVERFLOW,  /*!<  Input overflow error encountered.  */
    ADI_CRYPTO_EVENT_STATUS_UNKNOWN,         /*!<  Unknown error encountered.         */
} ADI_CRYPTO_EVENT;

/*! The amount of application supplied memory used by the CRYPTO driver to store internal state. */
#define ADI_CRYPTO_MEMORY_SIZE          (88u + ADI_SEM_SIZE)

/*! A device handle used in all API functions to identify the flash device. */
typedef struct __ADI_CRYPTO_DEV_DATA_TYPE* ADI_CRYPTO_HANDLE;

/*! Number of bytes to allocate for SHA256 hash outputs */
#define ADI_CRYPTO_SHA_HASH_BYTES        (256u/8u)

/*! Computation mode(Encryption/Decryption) for given buffers */
typedef enum
{
    ADI_CRYPTO_DECODE = (0u << BITP_CRYPT_CFG_ENCR),    /*!< Encoding mode is decryption. */
    ADI_CRYPTO_ENCODE = (1u << BITP_CRYPT_CFG_ENCR),    /*!< Encoding mode is encryption. */
} ADI_CRYPTO_CODING_MODE;

/*! Enum for the AES KEY Length */
typedef enum
{
    ADI_CRYPTO_AES_KEY_LEN_128_BIT = (0u << BITP_CRYPT_CFG_AESKEYLEN),     /*!< KEY length is 128 bits. */
    ADI_CRYPTO_AES_KEY_LEN_256_BIT = (2u << BITP_CRYPT_CFG_AESKEYLEN),     /*!< KEY length is 256 bits. */
} ADI_CRYPTO_AES_KEY_LEN;

/*! Enable byte swapping for KEY writes */
typedef enum
{
    ADI_CRYPTO_KEY_LITTLE_ENDIAN = (0u << BITP_CRYPT_CFG_KEY_BYTESWAP),    /*!< Do not apply KEY write byte swaps. */
    ADI_CRYPTO_KEY_BIG_ENDIAN    = (1u << BITP_CRYPT_CFG_KEY_BYTESWAP),    /*!< Apply KEY write byte swaps.        */
} ADI_CRYPTO_KEY_BYTE_SWAP;

/*! Byte-swap the SHA Input Data */
typedef enum
{
    ADI_CRYPTO_SHA_LITTLE_ENDIAN = (0u << BITP_CRYPT_CFG_SHA_BYTESWAP),    /*!< Do not apply SHA data write byte swaps. */
    ADI_CRYPTO_SHA_BIG_ENDIAN    = (1u << BITP_CRYPT_CFG_SHA_BYTESWAP),    /*!< Apply SHA data write byte swaps. */
} ADI_CRYPTO_SHA_BYTE_SWAP;

/*! Byte-swap the AES Input Data */
typedef enum
{
    ADI_CRYPTO_AES_LITTLE_ENDIAN = (0u << BITP_CRYPT_CFG_AES_BYTESWAP),    /*!< Do not apply AES data write byte swaps. */
    ADI_CRYPTO_AES_BIG_ENDIAN    = (1u << BITP_CRYPT_CFG_AES_BYTESWAP),    /*!< Apply AES data write byte swaps. */
} ADI_CRYPTO_AES_BYTE_SWAP;

/*!
 * \enum ADI_CRYPTO_CIPHER_MODE
 * Enum for the cipher modes.
 */
typedef enum {
    ADI_CRYPTO_MODE_CBC  = BITM_CRYPT_CFG_CBCEN,     /*!< Select CBC cipher mode. */
    ADI_CRYPTO_MODE_CCM  = BITM_CRYPT_CFG_CCMEN,     /*!< Select CCM cipher mode. */
    ADI_CRYPTO_MODE_CMAC = BITM_CRYPT_CFG_CMACEN,    /*!< Select CMAC cipher mode. */
    ADI_CRYPTO_MODE_CTR  = BITM_CRYPT_CFG_CTREN,     /*!< Select CTR cipher mode. */
    ADI_CRYPTO_MODE_ECB  = BITM_CRYPT_CFG_ECBEN,     /*!< Select ECB cipher mode. */
    ADI_CRYPTO_MODE_HMAC = BITM_CRYPT_CFG_HMACEN,    /*!< Select HMAC cipher mode. */
    ADI_CRYPTO_MODE_SHA  = BITM_CRYPT_CFG_SHA256EN,  /*!< Select SHA cipher mode. */
} ADI_CRYPTO_CIPHER_MODE;

/*! superset user Crypto transaction structure (different elements used for different modes) */
typedef struct
{
    ADI_CRYPTO_CIPHER_MODE    eCipherMode;        /*!< Cipher mode to use */
    ADI_CRYPTO_CODING_MODE    eCodingMode;        /*!< Coding Mode (Encryption or Decryption) */

    ADI_CRYPTO_KEY_BYTE_SWAP  eKeyByteSwap;       /*!< KEY endianness */
    ADI_CRYPTO_SHA_BYTE_SWAP  eShaByteSwap;       /*!< SHA endianness */
    ADI_CRYPTO_AES_BYTE_SWAP  eAesByteSwap;       /*!< AES endianness */

    uint8_t                  *pKey;               /*!< Pointer to the KEY data: pre-formatted as a byte array, according to eAesKeyLen. */
    ADI_CRYPTO_AES_KEY_LEN    eAesKeyLen;         /*!< The length of the AES KEY */

    uint32_t                 *pAuthData;          /*!< CCM mode: pointer to user prefix buffer */
    uint32_t                  numAuthBytes;       /*!< Length of the prefix buffer in bytes (should be a multiple of 16 bytes) */

    uint32_t                 *pInputData;         /*!< Pointer to user input data buffer */
    uint32_t                  numInputBytes;      /*!< Length of the data buffer in bytes (should be a multiple of 16bytes) */

    uint32_t                 *pOutputData;        /*!< Pointer to user output buffer */
    uint32_t                  numOutputBytes;     /*!< Length of the output buffer in bytes (should be a multiple of 16bytes) */

    uint8_t                  *pNonceIV;           /*!< Pointer to user 16-byte array containing one of three values, depending on cipher mode:\n
                                                       - CTR mode = 108-bit NONCE\n
                                                       - CCM mode = 112-bit NONCE\n
                                                       - CBC mode = 128-bit IV (Initialization Vector)\n\n
                                                       NONCE and IV assume little endian format, for example: CTR NONCE packing is:\n
                                                       - NONCE[0]            ->   7:0\n
                                                       - NONCE[1]            ->  15:8\n
                                                       -   ...\n
                                                       - NONCE[13]           -> 103:96\n
                                                       - NONCE[14](Bits 3:0) -> 107:104\n
                                                   */
    uint32_t                  CounterInit;        /*!< CTR/CCM mode: Counter Initialization Value (CTR=20-bit, CCM=16-bit) */
    uint32_t                  numValidBytes;      /*!< CCM mode: Number of valid bytes in the last (padding) block (1-16) */
    uint32_t                  numShaBits;         /*!< SHA mode: Number of bits in the SHA payload, which may be odd-sized */
} ADI_CRYPTO_TRANSACTION;


/*================ PUBLIC API ==================*/


ADI_CRYPTO_RESULT adi_crypto_Open                  (uint32_t const nDeviceNum, void * const pMemory, uint32_t const nMemorySize, ADI_CRYPTO_HANDLE * const phDevice);
ADI_CRYPTO_RESULT adi_crypto_Close                 (ADI_CRYPTO_HANDLE const hDevice);
ADI_CRYPTO_RESULT adi_crypto_RegisterCallback      (ADI_CRYPTO_HANDLE const hDevice, ADI_CALLBACK const pfCallback, void * const pCBParam);
ADI_CRYPTO_RESULT adi_crypto_Enable                (ADI_CRYPTO_HANDLE const hDevice, bool const bEnable);

ADI_CRYPTO_RESULT adi_crypto_SubmitBuffer          (ADI_CRYPTO_HANDLE const hDevice, ADI_CRYPTO_TRANSACTION * const pBuffer);
ADI_CRYPTO_RESULT adi_crypto_GetBuffer             (ADI_CRYPTO_HANDLE const hDevice, ADI_CRYPTO_TRANSACTION ** const ppBuffer);
ADI_CRYPTO_RESULT adi_crypto_IsBufferAvailable     (ADI_CRYPTO_HANDLE const hDevice, bool * const pbAvailable);

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
ADI_CRYPTO_RESULT adi_crypto_EnableDmaMode         (ADI_CRYPTO_HANDLE const hDevice, bool const bEnable);
#endif


#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/

