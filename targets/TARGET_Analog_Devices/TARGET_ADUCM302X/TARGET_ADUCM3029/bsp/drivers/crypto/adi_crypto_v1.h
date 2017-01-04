/*!
 *****************************************************************************
 @file:    adi_crypto_v1.h
 @brief:   Crypto Device Implementations for ADuCM302x
 @version: $Revision: 35030 $
 @date:    $Date: 2016-07-05 05:49:46 +0100 (Tue, 05 Jul 2016) $
 -----------------------------------------------------------------------------
Copyright (c) 2012-2015 Analog Devices, Inc.

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

/*! \addtogroup Crypto_Driver Crypto Device Driver
 *  Crypto Device Driver
 *  @{
 *
 *  <b>Crypto peripheral driver</b>
 *
 *  The Crypto block is an accelerator implementing the AES Cipher and different
 *  block cipher modes of operation of the Cipher. The Hardware support is provided for
 *  almost the entire encryption/decryption process. The block also support CMAC and SHA.
 *
 *  <b>Memory DMA driver Crypto operation</b>
 *
 *  The crypto module can use DMA stream to perform its operations. The Crypto
 *  DMA mode can be enabled using adi_crpto_EnableDmaMode() API.
 *
 */

#ifndef __ADI_CRYPTO_V1_H__
#define __ADI_CRYPTO_V1_H__

#include <adi_types.h>
#include <services/int/adi_int.h>
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
    ADI_CRYPTO_SUCCESS = 0,                /*!<  No Error, API suceeded */

    ADI_CRYPTO_ERR_RTOS,                   /*!<  Failure in RTOS functions */
    ADI_CRYPTO_ERR_UNKNOWN_ERROR,          /*!<  An unknown error was detected */
    ADI_CRYPTO_ERR_BUFFER_NOT_AVAIL,       /*!<  Buffer not available  */
    ADI_CRYPTO_ERR_INSUFFICIENT_MEM,       /*!<  Insufficient memory passed to the driver */
    ADI_CRYPTO_ERR_DMA,                    /*!<  DMA Error Occurred */
    ADI_CRYPTO_ERR_FIFO,                   /*!<  Overflow/Underflow happened in the device */
    ADI_CRYPTO_ERR_ALREADY_INITIALIZED,    /*!<  Crypto is already initialized */
    ADI_CRYPTO_ERR_INVALID_PARAM,          /*!<  Invalid function parameter */
    ADI_CRYPTO_ERR_BAD_DEV_HANDLE,         /*!<  Invalid device handle passed */
    ADI_CRYPTO_ERR_BAD_CONFIG,             /*!<  Invalid device config parameters passed */
    ADI_CRYPTO_ERR_BAD_BUFFER,             /*!<  Invalid buffer parameters */
    ADI_CRYPTO_ERR_BUFFER_FULL,            /*!<  The internal buffers are full */
    ADI_CRYPTO_ERR_BAD_DEVICE_NUM,         /*!<  Asking to initialize an unknown instance */
    ADI_CRYPTO_ERR_INVALID_STATE,          /*!<  Operation failed since the device is in an invalid state */
    ADI_CRYPTO_ERR_NOT_SUPPORTED           /*!<  Given feature is not supported */
} ADI_CRYPTO_RESULT;

/*! The amount of application supplied memory required by the CRYPTO driver */
#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
#define ADI_CRYPTO_MEMORY_SIZE          (400u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#else
#define ADI_CRYPTO_MEMORY_SIZE          (192u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#endif

/*! Crypto device handle */
typedef void* ADI_CRYPTO_HANDLE;


/*! Computation mode(Encryption/Decryption) for given buffers */
typedef enum
{
    ADI_CRYPTO_ENC_MODE_ENCRYPT,    /*!< Encoding mode is encryption */
    ADI_CRYPTO_ENC_MODE_DECRYPT     /*!< Encoding mode is decryption */
} ADI_CRYPTO_ENC_MODE;

/*! Enum for the Key Length */
typedef enum
{
    ADI_CRYPTO_KEY_LEN_128_BIT = (0x0 << BITP_CRYPT_CFG_KEYLEN),     /*!< Key length is 128 bits */
    ADI_CRYPTO_KEY_LEN_256_BIT = (0x2 << BITP_CRYPT_CFG_KEYLEN),     /*!< Key length is 256 bits */
} ADI_CRYPTO_KEY_LEN;


/*! Enum for the data format used for the buffers */
typedef enum
{
    /*! The data format is set to little endian (default) */
    ADI_CRYPTO_DATA_FMT_LITTLE_ENDIAN = ENUM_CRYPT_CFG_LITTLE_ENDIAN,

    /*! The data format is set to big endian */
    ADI_CRYPTO_DATA_FMT_BIG_ENDIAN    = ENUM_CRYPT_CFG_BIG_ENDIAN,

} ADI_CRYPTO_DATA_FMT;


/*! Enum for the callback events */
typedef enum {
    /*!
     *  ECB Buffer Processed Event. The ADI_CRYPTO_ECB_BUFFER structure corresponding to the computation will be
     *  given back to the application.
     */
    ADI_CRYPTO_EVENT_ECB_BUFFER_PROCESSED,

    /*!
     *  CTR Buffer Processed Event. The ADI_CRYPTO_CTR_BUFFER structure corresponding to the computation will be
     *  given back to the application.
     */
    ADI_CRYPTO_EVENT_CTR_BUFFER_PROCESSED,

    /*!
     *  CBC Buffer Processed Event. The ADI_CRYPTO_CBC_BUFFER structure corresponding to the computation will be
     *  given back to the application.
     */
    ADI_CRYPTO_EVENT_CBC_BUFFER_PROCESSED,

    /*!
     *  CCM Buffer Processed Event. The ADI_CRYPTO_CCM_BUFFER structure corresponding to the computation will be
     *  given back to the application.
     */
    ADI_CRYPTO_EVENT_CCM_BUFFER_PROCESSED,

    /*!
     *  CMAC Buffer Processed Event. The ADI_CRYPTO_CMAC_BUFFER structure corresponding to the computation will be
     *  given back to the application.
     */
    ADI_CRYPTO_EVENT_CMAC_BUFFER_PROCESSED,

    /*!
     *  SHA Buffer Processed Event. The ADI_CRYPTO_SHA_BUFFER structure corresponding to the computation will be
     *  given back to the application.
     */
    ADI_CRYPTO_EVENT_SHA_BUFFER_PROCESSED,

    /*! FIFO underflow/overflow occurred. The device will be disabled and all buffers discarded when this occurs */
    ADI_CRYPTO_EVENT_ERR_FIFO,
    
    /*! DMA error occurred. The device will be disabled and all buffers discarded when this occurs */
    ADI_CRYPTO_EVENT_ERR_DMA

} ADI_CRYPTO_EVENT;


/*! Crypto Configuration */
typedef struct _ADI_CRYPTO_CONFIG
{
    ADI_CRYPTO_DATA_FMT eBuffDataFormat;  /*!< Endianess of the buffer passed */
    ADI_CRYPTO_KEY_LEN  eKeyLen;          /*!< The length of the key */
    uint8_t*            pKey;             /*!< Pointer to the key. The key should be of size as configured in eKeyLen (Assumes little endian format). */
} ADI_CRYPTO_CONFIG;


/*! ECB Buffer Structure */
typedef struct _ADI_CRYPTO_ECB_BUFFER
{
    ADI_CRYPTO_CONFIG  *pConfig;            /*!<    Pointer to the config structure for the given buffer.
                                                    The application can reuse the config structure for multiple
                                                    buffers as long it can guarantee that config structure will not
                                                    change if any buffer associated with the config is owned by
                                                    the driver.
                                            */
    
    ADI_CRYPTO_ENC_MODE eEncMode;         /*!< Encoding Mode (Encryption or Decryption) */

    void*         pDataBuffer;              /*!< Pointer to the data buffer */
    uint32_t      nDataBuffLen;             /*!< Length of the data buffer in bytes (should be a multiple of 16bytes) */

    void*         pOutputBuffer;            /*!< Pointer to the output buffer */
    uint32_t      nOutputBuffLen;           /*!< Length of the output buffer in bytes (should be a multiple of 16bytes) */

} ADI_CRYPTO_ECB_BUFFER;


/*! CBC Buffer Structure */
typedef struct _ADI_CRYPTO_CBC_BUFFER
{
    ADI_CRYPTO_CONFIG  *pConfig;            /*!<    Pointer to the config structure for the given buffer.
                                                    The application can reuse the config structure for multiple
                                                    buffers as long it can guarantee that config structure will not
                                                    change if any buffer associated with the config is owned by
                                                    the driver.
                                            */
    
    ADI_CRYPTO_ENC_MODE eEncMode;         /*!< Encoding Mode (Encryption or Decryption) */

    void*         pDataBuffer;              /*!< Pointer to the data buffer */
    uint32_t      nDataBuffLen;             /*!< Length of the data buffer in bytes (should be a multiple of 16bytes) */

    void*         pOutputBuffer;            /*!< Pointer to the output buffer */
    uint32_t      nOutputBuffLen;           /*!< Length of the output buffer in bytes (should be a multiple of 16bytes) */

    uint8_t       InitVector[16];           /*!< Initialization Vector (Assumes little endian format) */

} ADI_CRYPTO_CBC_BUFFER;

/*! CTR Buffer Structure */
typedef struct _ADI_CRYPTO_CTR_BUFFER
{
    ADI_CRYPTO_CONFIG  *pConfig;            /*!<    Pointer to the config structure for the given buffer.
                                                    The application can reuse the config structure for multiple
                                                    buffers as long it can guarantee that config structure will not
                                                    change if any buffer associated with the config is owned by
                                                    the driver.
                                            */
    
    ADI_CRYPTO_ENC_MODE eEncMode;         /*!< Encoding Mode (Encryption or Decryption) */

    void*         pDataBuffer;              /*!< Pointer to the data buffer */
    uint32_t      nDataBuffLen;             /*!< Length of the data buffer in bytes (should be a multiple of 16bytes) */

    void*         pOutputBuffer;            /*!< Pointer to the output buffer */
    uint32_t      nOutputBuffLen;           /*!< Length of the output buffer in bytes (should be a multiple of 16bytes) */

    uint8_t       NONCE[14];                /*!< NONCE (Should of size 108-bits). Assumes little endian format
                                                    Mapping will be
                                                        NONCE[0]            ->   7:0
                                                        NONCE[1]            ->  15:8
                                                        .....
                                                        NONCE[13]           -> 103:96
                                                        NONCE[14](Bits 3:0) -> 107:104
                                            */

    uint32_t     nCounterInit;              /*!< Counter Initialization Value (only 20 bits) */

} ADI_CRYPTO_CTR_BUFFER;


/*! CCM Buffer Structure */
typedef struct _ADI_CRYPTO_CCM_BUFFER
{
    ADI_CRYPTO_CONFIG  *pConfig;            /*!<    Pointer to the config structure for the given buffer.
                                                    The application can reuse the config structure for multiple
                                                    buffers as long it can guarantee that config structure will not
                                                    change if any buffer associated with the config is owned by
                                                    the driver.
                                            */
    
    ADI_CRYPTO_ENC_MODE eEncMode;           /*!< Encoding Mode (Encryption or Decryption) */


    void*         pPrefixBuffer;            /*!< Pointer to the prefix buffer */
    uint32_t      nPrefixBuffLen;           /*!< Length of the prefix buffer in bytes (should be a multiple of 16bytes) */

    void*         pDataBuffer;              /*!< Pointer to the data buffer */
    uint32_t      nDataBuffLen;             /*!< Length of the data buffer in bytes (should be a multiple of 16bytes) */

    uint32_t      nValidBytesInLastBlock;   /*!< Number of valid bytes in the last block (1-16) */

    void*         pOutputBuffer;            /*!< Pointer to the output buffer */
    uint32_t      nOutputBuffLen;           /*!< Length of the output buffer in bytes (should be a multiple of 16bytes) */

    uint8_t       NONCE[14];                /*!< NONCE (Should of size 112-bits). Assumes little endian format
                                                    Mapping will be
                                                        NONCE[0]   ->   7:0
                                                        NONCE[1]   ->  15:8
                                                        .....
                                                        NONCE[13]  -> 103:96
                                                        NONCE[14]  -> 111:104
                                            */

    uint16_t     nCounterInit;              /*!< Counter Initialization Value */

} ADI_CRYPTO_CCM_BUFFER;

/*! CMAC Buffer Structure */
typedef struct _ADI_CRYPTO_CMAC_BUFFER
{
    ADI_CRYPTO_CONFIG  *pConfig;             /*!<    Pointer to the config structure for the given buffer.
                                                    The application can reuse the config structure for multiple
                                                    buffers as long it can guarantee that config structure will not
                                                    change if any buffer associated with the config is owned by
                                                    the driver.
                                             */

    void*         pDataBuffer;              /*!< Pointer to the data buffer */
    uint32_t      nDataBuffLen;             /*!< Length of the data buffer in bytes (should be a multiple of 16 bytes) */

    void*         pMACBuffer;               /*!< Pointer to the buffer to which MAC will be written (Should be of size 16bytes) */
    uint32_t      nMACBuffLen;               /*!< Length of the MAC buffer in bytes */

} ADI_CRYPTO_CMAC_BUFFER;

/*! SHA Buffer Structure */
typedef struct _ADI_CRYPTO_SHA_BUFFER
{
    void*         pDataBuffer;              /*!< Pointer to the data buffer */
    uint32_t      nDataBuffLenInBits;       /*!< Length of the data buffer in bits */
    void*         pSHABuffer;               /*!< Pointer to the buffer to which SHA will be written */
    uint32_t      nSHABuffLen;              /*!< Length of the SHA buffer in bytes */  

} ADI_CRYPTO_SHA_BUFFER;




/*================ E X T E R N A L S ==================*/

/* Open a Crypto device instance */
ADI_CRYPTO_RESULT adi_crypto_Open(
                                  uint32_t               nDeviceNum ,
                                  void*                  pMemory,
                                  uint32_t               nMemorySize,
                                  ADI_CRYPTO_HANDLE*     phDevice
                                  );

/* Close the device instance */
ADI_CRYPTO_RESULT adi_crypto_Close(ADI_CRYPTO_HANDLE hDevice);

/* Register a callback function */
 ADI_CRYPTO_RESULT adi_crypto_RegisterCallback (
                                                ADI_CRYPTO_HANDLE      hDevice,
                                                ADI_CALLBACK           pfCallback,
                                                void*                  pCBParam
                                                );

/* Enable the device to start processing */
ADI_CRYPTO_RESULT adi_crypto_Enable (
                                     ADI_CRYPTO_HANDLE        hDevice,
                                     bool_t                   bEnable
                                     );

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/* Enable DMA for the device */
ADI_CRYPTO_RESULT adi_crypto_EnableDmaMode (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            bool_t                  bEnable
                                            );
#endif


/*******  ECB Encryption/Decryption APIs  *******/

#if (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)
/* Submit an ECB buffer for processing */
 ADI_CRYPTO_RESULT adi_crypto_SubmitECBBuffer (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            ADI_CRYPTO_ECB_BUFFER*  pBuffer
                                            );

/* Get the ECB buffer back */
ADI_CRYPTO_RESULT adi_crypto_GetECBBuffer (
                                        ADI_CRYPTO_HANDLE         hDevice,
                                        ADI_CRYPTO_ECB_BUFFER**   ppBuffer
                                        );

/* Peek function to know whether a completed ECB Buffer is available */
ADI_CRYPTO_RESULT adi_crypto_IsECBBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                );


#endif /* ADI_CRYPTO_ENABLE_ECB_SUPPORT */



/*******  CTR Encryption/Decryption APIs  *******/


#if (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
/* Submit an CTR buffer for processing */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCTRBuffer (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            ADI_CRYPTO_CTR_BUFFER*  pBuffer
                                            );

/* Get the CTR buffer back */
ADI_CRYPTO_RESULT adi_crypto_GetCTRBuffer (
                                        ADI_CRYPTO_HANDLE         hDevice,
                                        ADI_CRYPTO_CTR_BUFFER**   ppBuffer
                                        );

/* Peek function to know whether a completed CTR Buffer is available */
ADI_CRYPTO_RESULT adi_crypto_IsCTRBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                );

#endif /* ADI_CRYPTO_ENABLE_CTR_SUPPORT */

/*******  CBC Encryption/Decryption APIs  *******/

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
/* Submit an CBC buffer for processing */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCBCBuffer (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            ADI_CRYPTO_CBC_BUFFER*  pBuffer
                                            );

/* Get the CBC buffer back */
ADI_CRYPTO_RESULT adi_crypto_GetCBCBuffer (
                                        ADI_CRYPTO_HANDLE         hDevice,
                                        ADI_CRYPTO_CBC_BUFFER**   ppBuffer
                                        );

/* Peek function to know whether a completed CBC Buffer is available */
ADI_CRYPTO_RESULT adi_crypto_IsCBCBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                );

#endif /* ADI_CRYPTO_ENABLE_CBC_SUPPORT */

/*******  CCM Encryption/Decryption APIs  *******/

#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
/* Submit an CCM buffer for processing */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCCMBuffer (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            ADI_CRYPTO_CCM_BUFFER*  pBuffer
                                            );

/* Get the CCM buffer back */
ADI_CRYPTO_RESULT adi_crypto_GetCCMBuffer (
                                        ADI_CRYPTO_HANDLE         hDevice,
                                        ADI_CRYPTO_CCM_BUFFER**   ppBuffer
                                        );

/* Peek function to know whether a completed CCM Buffer is available */
ADI_CRYPTO_RESULT adi_crypto_IsCCMBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                );

#endif /* ADI_CRYPTO_ENABLE_CCM_SUPPORT */


/*******  CMAC APIs  *******/

#if (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)
/* Submit an CMAC buffer for processing */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCMACBuffer (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            ADI_CRYPTO_CMAC_BUFFER*  pBuffer
                                            );

/* Get the CMAC buffer back */
ADI_CRYPTO_RESULT adi_crypto_GetCMACBuffer (
                                        ADI_CRYPTO_HANDLE         hDevice,
                                        ADI_CRYPTO_CMAC_BUFFER**   ppBuffer
                                        );

/* Peek function to know whether a completed CMAC Buffer is available */
ADI_CRYPTO_RESULT adi_crypto_IsCMACBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                );

#endif /* ADI_CRYPTO_ENABLE_CMAC_SUPPORT */


/*******  SHA Only APIs  *******/

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
/* Submit an SHA buffer for processing */
 ADI_CRYPTO_RESULT adi_crypto_SubmitSHABuffer (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            ADI_CRYPTO_SHA_BUFFER*  pBuffer
                                            );

/* Get the SHA buffer back */
ADI_CRYPTO_RESULT adi_crypto_GetSHABuffer (
                                        ADI_CRYPTO_HANDLE         hDevice,
                                        ADI_CRYPTO_SHA_BUFFER**   ppBuffer
                                        );

/* Peek function to know whether a completed SHA Buffer is available */
ADI_CRYPTO_RESULT adi_crypto_IsSHABufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                );

#endif /* ADI_CRYPTO_ENABLE_SHA_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/
