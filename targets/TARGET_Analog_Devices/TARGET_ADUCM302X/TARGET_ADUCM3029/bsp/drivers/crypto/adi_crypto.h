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
    ADI_CRYPTO_ERR_INVALID_KEY_SIZE,     /*!<  bad key size fault detected.                              */
#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)
    ADI_CRYPTO_PK_ALREADY_ENABLED,       /*!<  PKSTOR is already enabled.                                */
    ADI_CRYPTO_PK_ALREADY_DISABLED,      /*!<  PKSTOR is already disabled.                               */
    ADI_CRYPTO_PK_NOT_ENABLED,           /*!<  PKSTOR operation attempted while PKSTOR is disabled.      */
    ADI_CRYPTO_PK_INVALID_KUWLEN,        /*!<  Invalid KUW length parameter.                             */
    ADI_CRYPTO_PK_INVALID_KEY_INDEX,     /*!<  PKSTOR key index overflow.                                */
    ADI_CRYPTO_PK_CMD_BUSY,              /*!<  command busy bit set after PKSTOR command done.           */
    ADI_CRYPTO_PK_CMD_FAULT,             /*!<  command fault bit set during PKSTOR command.              */
    ADI_CRYPTO_PK_CMD_ECC_FAULT,         /*!<  ECC errors detected during PKSTOR command.                */
#endif
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
#if defined (__ADUCM4x50__)
    ADI_CRYPTO_EVENT_STATUS_HMAC_DONE,       /*!<  HMAC operation is complete.        */
#else
    ADI_CRYPTO_RESERVED_EVENT,               /*!<  reserved: preserves ordering       */
#endif /*__ADUCM4x50*/
    ADI_CRYPTO_EVENT_STATUS_SHA_DONE,        /*!<  SHA operation is complete.         */

    /* other events */
    ADI_CRYPTO_EVENT_DMA_BUS_ERROR,          /*!<  DMA bus error encountered.         */
    ADI_CRYPTO_EVENT_DMA_DESCRIPTOR_ERROR,   /*!<  DMA descriptor error encountered.  */
    ADI_CRYPTO_EVENT_DMA_UNKNOWN_ERROR,      /*!<  DMA unknown error encountered.     */
    ADI_CRYPTO_EVENT_STATUS_INPUT_OVERFLOW,  /*!<  Input overflow error encountered.  */
    ADI_CRYPTO_EVENT_STATUS_UNKNOWN,         /*!<  Unknown error encountered.         */
} ADI_CRYPTO_EVENT;

/*! The amount of application supplied memory used by the CRYPTO driver to store internal state. */
#if defined (__ADUCM4x50__)
#define ADI_CRYPTO_MEMORY_SIZE          (96u + ADI_SEM_SIZE)  /*!< Required user memory size for ADuCM4x50 processor family. */
#elif defined (__ADUCM302x__)
#define ADI_CRYPTO_MEMORY_SIZE          (84u + ADI_SEM_SIZE)  /*!< Required user memory size for ADuCM302x processor family. */
#else
#error Crypto driver is not ported to this proccesor
#endif

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

#if defined (__ADUCM4x50__)
/*! Enable byte swapping for KEY writes */
typedef enum
{
    ADI_CRYPTO_KEY_LITTLE_ENDIAN = (0u << BITP_CRYPT_CFG_KEY_BYTESWAP),    /*!< Do not apply KEY write byte swaps. */
    ADI_CRYPTO_KEY_BIG_ENDIAN    = (1u << BITP_CRYPT_CFG_KEY_BYTESWAP),    /*!< Apply KEY write byte swaps.        */
} ADI_CRYPTO_KEY_BYTE_SWAP;
#endif /*__ADUCM4x50__*/

#if defined (__ADUCM4x50__)
/*! Byte-swap the SHA Input Data */
typedef enum
{
    ADI_CRYPTO_SHA_LITTLE_ENDIAN = (0u << BITP_CRYPT_CFG_SHA_BYTESWAP),    /*!< Do not apply SHA data write byte swaps. */
    ADI_CRYPTO_SHA_BIG_ENDIAN    = (1u << BITP_CRYPT_CFG_SHA_BYTESWAP),    /*!< Apply SHA data write byte swaps. */
} ADI_CRYPTO_SHA_BYTE_SWAP;
#endif /*__ADUCM4x50__*/

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
#if defined (__ADUCM4x50__)
    ADI_CRYPTO_MODE_HMAC = BITM_CRYPT_CFG_HMACEN,    /*!< Select HMAC cipher mode. */
#endif /*__ADUCM4x50__*/
    ADI_CRYPTO_MODE_SHA  = BITM_CRYPT_CFG_SHA256EN,  /*!< Select SHA cipher mode. */
} ADI_CRYPTO_CIPHER_MODE;


#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)
/*! PKSTOR Key Wrap/Unwrap key lengths */
typedef enum
{
	ADI_PK_KUW_LEN_128       = (1u << BITP_CRYPT_CFG_KUWKEYLEN),       /*!< key wrap/unwrap size is 128-bit. */
	ADI_PK_KUW_LEN_256       = (2u << BITP_CRYPT_CFG_KUWKEYLEN),       /*!< key wrap/unwrap size is 256-bit. */
	ADI_PK_KUW_LEN_512       = (3u << BITP_CRYPT_CFG_KUWKEYLEN),       /*!< key wrap/unwrap size is 512-bit (compute-only; not store). */
} ADI_CRYPTO_PK_KUW_LEN;
#endif /*ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT*/


#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)
/*! PKSTOR commands */
typedef enum
{
	ADI_PK_CMD_WRAP_KUW      = (0x1 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< KUW wrap command. */
	ADI_PK_CMD_UNWRAP_KUW    = (0x2 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< KUW unwrap command. */
	ADI_PK_CMD_RESET_KUW     = (0x3 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< clear all KUW registers command. */
	ADI_PK_CMD_USE_KEY       = (0x4 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< load Key registers from KUW registers command. */
	ADI_PK_CMD_USE_DEV_KEY   = (0x5 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< load Key registers with devide key command. */
	/* gap */
	ADI_PK_CMD_RETRIEVE_KEY  = (0x8 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< load KUW registers command. */
	ADI_PK_CMD_STORE_KEY     = (0x9 << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< program KUW registers into flash command. */
	ADI_PK_CMD_ERASE_KEY     = (0xA << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< erase single key set from flash command. */
	ADI_PK_CMD_ERASE_PAGE    = (0xB << BITP_CRYPT_PRKSTORCFG_CMD),       /*!< erase entire key page command. */
} ADI_CRYPTO_PK_CMD;
#endif /*ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT*/


/*! superset user Crypto transaction structure (different elements used for different modes) */
typedef struct
{
    ADI_CRYPTO_CIPHER_MODE    eCipherMode;        /*!< Cipher mode to use */
    ADI_CRYPTO_CODING_MODE    eCodingMode;        /*!< Coding Mode (Encryption or Decryption) */
#if defined (__ADUCM4x50__)
    ADI_CRYPTO_KEY_BYTE_SWAP  eKeyByteSwap;       /*!< KEY endianness */
    ADI_CRYPTO_SHA_BYTE_SWAP  eShaByteSwap;       /*!< SHA endianness */
#endif /*__ADUCM4x50__*/
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

#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)
	/* PKSTOR extensions used only in context of overriding above key info with protected keys stored in flash. */
    /* Assumes previously wrapped keys have already been stored using adi_crypto_pk_Xxx APIs. */
    /* NOTE: Enabeling PKSTOR boolean results in explicit key loads being replaced with PKSTOR keys prior to all Crypto operations */
    /* When enabled, the PKSTOR sequence is to RETRIEVE, UNWRAP and USE whichever key index and size is designated below. */

    bool                      bUsePKSTOR;        /*!< Flag that controls use of PKSTOR key overrides. */
    ADI_CRYPTO_PK_KUW_LEN     pkKuwLen;          /*!< KUW key size */
    uint8_t                   pkIndex;           /*!< Flash index within PKSTOR for storing/rettrieving keys. */
#endif
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

#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)
ADI_CRYPTO_RESULT adi_crypto_pk_EnablePKSTOR       (ADI_CRYPTO_HANDLE const hDevice, bool const bEnable);

ADI_CRYPTO_RESULT adi_crypto_pk_SetValString       (ADI_CRYPTO_HANDLE const hDevice, uint8_t * const pValStr);
ADI_CRYPTO_RESULT adi_crypto_pk_GetValString       (ADI_CRYPTO_HANDLE const hDevice, uint8_t * const pValStr);

ADI_CRYPTO_RESULT adi_crypto_pk_SetKuwLen          (ADI_CRYPTO_HANDLE const hDevice, ADI_CRYPTO_PK_KUW_LEN const kuwDataLen);
ADI_CRYPTO_RESULT adi_crypto_pk_SetKuwReg          (ADI_CRYPTO_HANDLE const hDevice, uint8_t * const pKuwData);
ADI_CRYPTO_RESULT adi_crypto_pk_WrapKuwReg         (ADI_CRYPTO_HANDLE const hDevice);
ADI_CRYPTO_RESULT adi_crypto_pk_UnwrapKuwReg       (ADI_CRYPTO_HANDLE const hDevice);
ADI_CRYPTO_RESULT adi_crypto_pk_ResetKuwReg        (ADI_CRYPTO_HANDLE const hDevice);

ADI_CRYPTO_RESULT adi_crypto_pk_UseDecryptedKey    (ADI_CRYPTO_HANDLE const hDevice);
ADI_CRYPTO_RESULT adi_crypto_pk_LoadDeviceKey      (ADI_CRYPTO_HANDLE const hDevice);

ADI_CRYPTO_RESULT adi_crypto_pk_RetrieveKey        (ADI_CRYPTO_HANDLE const hDevice, uint8_t const index);
ADI_CRYPTO_RESULT adi_crypto_pk_StoreKey           (ADI_CRYPTO_HANDLE const hDevice, uint8_t const index);
ADI_CRYPTO_RESULT adi_crypto_pk_DestroyKey         (ADI_CRYPTO_HANDLE const hDevice, uint8_t const index);
ADI_CRYPTO_RESULT adi_crypto_pk_ErasePage          (ADI_CRYPTO_HANDLE const hDevice, uint8_t const index);
#endif  /* ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT */


#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/

