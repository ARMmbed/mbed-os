/*! *****************************************************************************
 * @file    adi_crc.h
 * @brief   CRC (Cyclic Redundancy Check) Device driver global include file
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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
    or more patent holders. This license does not release you from the
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

#ifndef ADI_CRC_H
#define ADI_CRC_H

/** @addtogroup CRC_Driver CRC Device Driver
 *  @{
 */

#include <adi_processor.h>

/*=============  I N C L U D E S   =============*/
#include <string.h>
/* Memory size check */
#include <assert.h>

/* DMA Manager includes */
#include <drivers/dma/adi_dma.h>

/* Include the config file for CRC */
#include <adi_crc_config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==============  D E F I N E S  ===============*/

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 0)


/**
 * The size of types may vary between building tools (int, char, enumerator, etc.).
 * This impacts the memory size required by a CRC driver.
 * Consequently, ADI_CRC_MEMORY_SIZE is environment dependent.
 */
#if defined(__ICCARM__)
/**
 * The amount of application supplied memory required to operate a core driven CRC device
 * using a CRC driver built in IAR environment.
 */
#define ADI_CRC_MEMORY_SIZE    (32u)
#else
/**
 * The amount of application supplied memory required to operate a core driven CRC device
 * using a CRC driver built in a generic built environment.
 * Note: Create a new macro definition for your targetted development environment
 *       if this generic value was not appropriate in your development environment.
 */
#define ADI_CRC_MEMORY_SIZE    (32u)
#endif


#else   /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */


/**
 * The size of types may vary between building tools (int, char, enumerator, etc.).
 * This impacts the memory size required by a CRC driver.
 * Consequently, ADI_CRC_MEMORY_SIZE is environment dependent.
 */
#if defined(__ICCARM__)
/**
 * The amount of application supplied memory required to operate a DMA driven CRC device
 * using a CRC driver built in IAR environment.
 */
#define ADI_CRC_MEMORY_SIZE    (32u)
#else
/**
 * The amount of application supplied memory required to operate a DMA driven CRC device
 * using a CRC driver built in a generic built environment.
 * Note: Create a new macro definition for your targetted development environment
 *       if this generic value was not appropriate in your development environment.
 */
#define ADI_CRC_MEMORY_SIZE    (32u)
#endif

/** Check that a DMA channel can be used with CRC */
#define ADI_CRC_VALID_DMA_CHANNEL(DMA_CHANNEL_ID) ((SIP0_CHANn<=(DMA_CHANNEL_ID)) && ((DMA_CHANNEL_ID)<=SIP7_CHANn))

/**
 * CRC events used in CRC callback functions to report
 * - the completion of a DMA driven CRC request
 * - errors detected when executing a DMA driven CRC request
 */
typedef enum __ADI_CRC_EVENT
{
    /*! DMA driven CRC peripheral has completed processing a request */
    ADI_CRC_EVENT_BUFFER_PROCESSED = ADI_DMA_EVENT_BUFFER_PROCESSED,

    /*! DMA driven CRC peripheral has encountered a problem when processing a request */
    ADI_CRC_EVENT_ERROR
} ADI_CRC_EVENT;

#endif  /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */

/**
 * A device handle used in all API functions to identify a CRC device.
 * This handle is obtained when opening a CRC driver using adi_crc_Open.
 * It stops being valid after closing the CRC driver using adi_crc_Close.
 */
typedef struct __ADI_CRC_DEVICE*  ADI_CRC_HANDLE;

/**
 *  CRC driver return codes
 */
typedef enum
{
    ADI_CRC_SUCCESS = 0,                /*!< 0x00 - Generic success */
    ADI_CRC_FAILURE,                    /*!< 0x01 - Generic failure */
    ADI_CRC_IN_USE,                     /*!< 0x02 - Supplied CRC device number is already open and in use */
    ADI_CRC_INSUFFICIENT_MEMORY,        /*!< 0x03 - Supplied memory is insufficient to operate the CRC device */
    ADI_CRC_FN_NOT_SUPPORTED,           /*!< 0x04 - Function not supported */
    ADI_CRC_FN_NOT_PERMITTED,           /*!< 0x05 - Function not permitted at current stage */
    ADI_CRC_BAD_HANDLE,                 /*!< 0x06 - Bad CRC device handle (can be caused by a CRC device not opened)*/
    ADI_CRC_BAD_DEVICE_NUMBER,          /*!< 0x07 - There is no CRC device identified by this number */
    ADI_CRC_INVALID_DMA_CHANNEL,        /*!< 0x08 - Invalid DMA channel assigned to a CRC driver */
    ADI_CRC_INVALID_PARAMETER,          /*!< 0x09 - Invalid parameter used in a CRC function */
} ADI_CRC_RESULT;

/*=======  P U B L I C   P R O T O T Y P E S  ========*/
/*            (globally-scoped functions)             */

/* Opens a CRC device instance */
ADI_CRC_RESULT adi_crc_Open(
    uint32_t                    DeviceNum,
    void                        *pMemory,
    uint32_t                    MemorySize,
    ADI_CRC_HANDLE              *phDevice);

/* Closes a CRC device instance */
ADI_CRC_RESULT adi_crc_Close(
    ADI_CRC_HANDLE const        hDevice);

/* Registers or unregisters a callback, used by the CRC interrupt handler or with DMA driven operations, with the CRC device */
ADI_CRC_RESULT adi_crc_RegisterCallback(
    ADI_CRC_HANDLE const        hDevice,
    ADI_CALLBACK                pfCallback,
    void *const                 pCBParam);

/* Sets the 32-bit polynomial for CRC operations */
ADI_CRC_RESULT adi_crc_SetPolynomialVal(
    ADI_CRC_HANDLE const        hDevice,
    uint32_t                    PolynomialVal);

/* Submits data buffer for CRC operation */
ADI_CRC_RESULT adi_crc_Compute(
    ADI_CRC_HANDLE const        hDevice,
    void                        *pCrcBuf,
    uint32_t                    NumBytes,
    uint32_t                    NumBits);

/* Gets the current CRC peripheral status */
ADI_CRC_RESULT adi_crc_IsCrcInProgress(
    ADI_CRC_HANDLE const        hDevice,
    bool                        *pbCrcInProgress);

/* Gets the final CRC result computed for a data stream */
ADI_CRC_RESULT adi_crc_GetFinalCrcVal(
    ADI_CRC_HANDLE const        hDevice,
    uint32_t                    *pFinalCrcVal);

/* Gets the current/intermediate CRC result computed for a data stream */
ADI_CRC_RESULT adi_crc_GetCurrentCrcVal(
    ADI_CRC_HANDLE const        hDevice,
    uint32_t                    *pCurrentCrcVal);

ADI_CRC_RESULT  adi_crc_SetBitMirroring(
    ADI_CRC_HANDLE const        hDevice,
    const bool                  bEnable);

ADI_CRC_RESULT  adi_crc_SetByteMirroring(
    ADI_CRC_HANDLE const        hDevice,
    const bool                  bEnable);

ADI_CRC_RESULT  adi_crc_EnableWordSwap(
    ADI_CRC_HANDLE const        hDevice,
    const bool                  bEnable);

ADI_CRC_RESULT adi_crc_SetCrcSeedVal(
    ADI_CRC_HANDLE const        hDevice,
    uint32_t                    CrcSeedVal);

ADI_CRC_RESULT  adi_crc_SetLSBFirst(
    ADI_CRC_HANDLE const        hDevice,
    const bool                  bEnable);

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* ADI_CRC_H */
