/*! *****************************************************************************
 * @file:    adi_crc_v1.h
 * @brief:   Cyclic Redundancy Check (CRC) peripheral definitions and API
 * @version: $Revision:
 * @date:    $Date:
 -----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

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

/** @addtogroup CRC_Driver CRC Device Driver
 *  @{
 */

#ifndef __ADI_CRC_V1_H__
#define __ADI_CRC_V1_H__


/*=============  I N C L U D E S   =============*/
#include <string.h>
/* Memory size check */
#include <assert.h>

/* DMA Manager includes */
#include <services/int/adi_int.h>
/* DMA Manager includes */
#include <services/dma/adi_dma.h>
/* Include the config file for CRC */
#include <adi_crc_config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==============  D E F I N E S  ===============*/

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 1)

/** The amount of application supplied memory required to operate a core driven CRC device */
#define ADI_CRC_MEMORY_SIZE    (160u)

#else

/** The amount of application supplied memory required to operate a core driven CRC device */
#define ADI_CRC_MEMORY_SIZE    (36u)

#endif

/** A device handle used in all API functions to identify the CRC device */
typedef struct __ADI_CRC_DEVICE*  ADI_CRC_HANDLE;

/**
 * CRC events
 */
typedef enum __ADI_CRC_EVENT
{
    /*! CRC peripheral has completed processing a buffer */
    ADI_CRC_EVENT_BUFFER_PROCESSED = ADI_DMA_EVENT_BUFFER_PROCESSED
} ADI_CRC_EVENT;

/**
 *  CRC driver return codes
 */
typedef enum
{
    ADI_CRC_SUCCESS = 0,                /*!< 0x00 - Generic success */
    ADI_CRC_FAILURE,                    /*!< 0x01 - Generic failure */
    ADI_CRC_IN_USE,                     /*!< 0x02 - Supplied CRC device number is already open and in use */
    ADI_CRC_NOT_OPEN,                   /*!< 0x03 - CRC device is not open to preform the requested task */
    ADI_CRC_INSUFFICIENT_MEMORY,        /*!< 0x04 - Supplied memory is insufficient to operate the CRC device */
    ADI_CRC_FN_NOT_SUPPORTED,           /*!< 0x05 - Function not supported */
    ADI_CRC_INVALID_FN_CALL,            /*!< 0x06 - Invalid function call for the current CRC mode */
    ADI_CRC_FN_NOT_PERMITTED,           /*!< 0x07 - Function not permitted at current stage */
    ADI_CRC_BAD_HANDLE,                 /*!< 0x08 - Bad CRC device handle */
    ADI_CRC_BAD_DEVICE_NUMBER,          /*!< 0x09 - Bad CRC device number */
    ADI_CRC_DMA_ERROR,                  /*!< 0x0A - Error with CRC DMA */
    ADI_CRC_INVALID_DMA_CHANNEL,        /*!< 0x0B - Invalid DMA channel */
    ADI_CRC_INVALID_PARAMETER,          /*!< 0x0C - Invalid parameter */
} ADI_CRC_RESULT;

/*=======  P U B L I C   P R O T O T Y P E S  ========*/
/*            (globally-scoped functions)             */

/* Opens a CRC device instance */
ADI_CRC_RESULT adi_crc_Open(
	uint32_t                DeviceNum,
	void                    *pMemory,
	uint32_t                MemorySize,
	ADI_CRC_HANDLE          *phDevice);

/* Closes a CRC device instance */
ADI_CRC_RESULT adi_crc_Close(
    ADI_CRC_HANDLE const    hDevice);


/* Enables/Disables DMA driven CRC operation */
ADI_CRC_RESULT adi_crc_EnableDmaMode(
    ADI_CRC_HANDLE const    hDevice,
    bool_t                  bEnable);

/* Sets the 32-bit polynomial for CRC operations */
ADI_CRC_RESULT adi_crc_SetPolynomialVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                PolynomialVal);


/* Submits data buffer for CRC operation */
ADI_CRC_RESULT adi_crc_Compute(
    ADI_CRC_HANDLE const    hDevice,
    void                    *pCrcBuf,
    uint32_t                NumBytes);

/* Submits data buffer for CRC operation, extended version */
ADI_CRC_RESULT adi_crc_ComputeWithPartialByte(
    ADI_CRC_HANDLE const    hDevice,
    void                    *pCrcBuf,
    uint32_t                NumBytes,
    uint32_t                NumBits);

/* Gets the current CRC peripheral status */
ADI_CRC_RESULT adi_crc_IsCrcInProgress(
    ADI_CRC_HANDLE const    hDevice,
    bool_t                  *pbCrcInProgress);

/* Gets the final CRC result computed for a data stream */
ADI_CRC_RESULT adi_crc_GetFinalCrcVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                *pFinalCrcVal);

/* Gets the current/intermediate CRC result computed for a data stream */
ADI_CRC_RESULT adi_crc_GetCurrentCrcVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                *pCurrentCrcVal);

ADI_CRC_RESULT  adi_crc_SetBitMirroring(
    ADI_CRC_HANDLE const    hDevice,
    const bool_t            bEnable);

ADI_CRC_RESULT  adi_crc_SetByteMirroring(
    ADI_CRC_HANDLE const    hDevice,
    const bool_t            bEnable);

ADI_CRC_RESULT  adi_crc_EnableWordSwap(
    ADI_CRC_HANDLE const    hDevice,
    const bool_t            bEnable);

ADI_CRC_RESULT adi_crc_SetCrcSeedVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                CrcSeedVal);

ADI_CRC_RESULT  adi_crc_SetLSBFirst(
    ADI_CRC_HANDLE const    hDevice,
    const bool_t            bEnable);

/* Registers or unregisters a callback with the CRC device */
ADI_CRC_RESULT adi_crc_RegisterCallback(
    ADI_CRC_HANDLE const    hDevice,
    ADI_CALLBACK            pfCallback,
    void *const             pCBParam);

ADI_CRC_RESULT adi_crc_SelectDmaChannel(
    ADI_CRC_HANDLE const    hDevice,
    ADI_DMA_CHANNEL_ID      eDMaChannelID);

#ifdef __cplusplus
}
#endif

#endif  /* __ADI_CRC_V1_H__ */

/*****/

/*@}*/
