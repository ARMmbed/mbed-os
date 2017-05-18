/*!
 *****************************************************************************
 @file:    adi_flash.h
 @brief:   Flash device driver definitions
 @date:    $Date: 2016-07-05 00:49:46 -0400 (Tue, 05 Jul 2016) $
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

/*! @addtogroup Flash_Driver Flash Driver
 *  @{
 */

#ifndef ADI_FLASH_H
#define ADI_FLASH_H

 /*! \cond PRIVATE */
#include <adi_processor.h>
#include <adi_callback.h>
#include <rtos_map/adi_rtos_map.h>  /* for ADI_SEM_SIZE */
/*! \endcond */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \enum ADI_FEE_RESULT
 * Flash Controller return codes.
 */
 typedef enum {
    ADI_FEE_SUCCESS = 0,                /*!< The function completed successfully.               */
    ADI_FEE_ERR_ALIGNMENT,              /*!< The flash write source data pointer is misaligned. */
    ADI_FEE_ERR_ALREADY_INITIALIZED,    /*!< The flash device driver is already initialized.    */
    ADI_FEE_ERR_BAD_DEVICE_NUM,         /*!< Device number passed is invalid.                   */
    ADI_FEE_ERR_BUFFER_ERR,             /*!< An error occurred while processing a write buffer. */
    ADI_FEE_ERR_DEVICE_BUSY,            /*!< The device is busy.                                */
    ADI_FEE_ERR_DMA_BUS_FAULT,          /*!< Runtime DMA bus fault detected.                    */
    ADI_FEE_ERR_DMA_INVALID_DESCR,      /*!< Runtime DMA invalid descriptor detected.           */
    ADI_FEE_ERR_DMA_REGISTER,           /*!< Error registering DMA error callback function.     */
    ADI_FEE_ERR_DMA_UNKNOWN_ERROR,      /*!< Unknown runtime DMA error detected.                */
    ADI_FEE_ERR_HW_ERROR_DETECTED,      /*!< An FEE hardware error occurred (pHwErrors param).  */
    ADI_FEE_ERR_INSUFFICIENT_MEM,       /*!< The memory passed is undersized.                   */
    ADI_FEE_ERR_INVALID_HANDLE,         /*!< Device Handle is invalid.                          */
    ADI_FEE_ERR_INVALID_PARAM,          /*!< A function parameter is invalid.                   */
    ADI_FEE_ERR_NO_DATA_TO_TRANSFER,    /*!< No transfer data detected.                         */
    ADI_FEE_ERR_TRANSFER_IN_PROGRESS,   /*!< Operation already in progress.                     */
    ADI_FEE_ERR_UNMATCHED_SUBMIT_QUERY, /*!< Unmatched read/write vs. submit/get API call.      */
    ADI_FEE_ERR_SEMAPHORE_FAILED,       /*!< An semaphore operation failed.                     */
 } ADI_FEE_RESULT;


/*! A device handle used in all API functions to identify the flash device. */
typedef struct __ADI_FEE_DEV_DATA_TYPE* ADI_FEE_HANDLE;


/*! Applications use the "ADI_FEE_MEMORY_SIZE" macro to allocate
    required flash driver memory.  This memory (and size) are passed
    to the flash driver during the "adi_fee_Open()" driver initialization
    call.  This memory is used to store internal flash driver state.
*/
#define ADI_FEE_MEMORY_SIZE            (44u + ADI_SEM_SIZE)


/*!
 * \enum ADI_FEE_CALLBACK_EVENT
 * Enum for the callback events.
 */
typedef enum {
     ADI_FEE_CALLBACK_EVENT_BUFFER_PROCESSED,    /*!< Buffer processed successfully event. */
     ADI_FEE_CALLBACK_EVENT_DEVICE_ERROR,        /*!< Device error(s) detected during command. */
} ADI_FEE_CALLBACK_EVENT;

/*!
 * \enum ADI_FEE_ECC_EVENT_TYPE
 * Enum for the Error-Correction-Code event type.
 */
typedef enum {
    ADI_FEE_ECC_EVENT_TYPE_ERROR,       /*!< ECC Error Event. */
    ADI_FEE_ECC_EVENT_TYPE_CORRECT      /*!< ECC correction event. */
} ADI_FEE_ECC_EVENT_TYPE;

/*!
 * \enum ADI_FEE_ECC_RESPONSE
 * Error-Correction-Code configuration codes.
 */
typedef enum {
    ADI_FEE_ECC_RESPONSE_NONE       = 0x0,      /*!< No Response. */
    ADI_FEE_ECC_RESPONSE_BUS_ERROR  = 0x1,      /*!< Generate a Bus Error. */
    ADI_FEE_ECC_RESPONSE_IRQ        = 0x2       /*!< Generate an IRQ. */
} ADI_FEE_ECC_RESPONSE;


/*!
 * \struct ADI_FEE_TRANSACTION
 * Flash write data transaction block.
 */
typedef struct {
    uint32_t   *pWriteAddr;  /*!< Pointer to flash-space (destination) write location. */
    uint32_t   *pWriteData;  /*!< Pointer to user-space (source) write Data.          */
    uint32_t    nSize;       /*!< Write data size (in bytes).                          */
    bool        bUseDma;     /*!< DMA flag controlling use of DMA or not.              */
} ADI_FEE_TRANSACTION;


/*================ E X T E R N A L S ==================*/
/* Flash Controller API */

ADI_FEE_RESULT adi_fee_Open              (uint32_t const nDeviceNum, void* const pMemory, uint32_t const nMemorySize, ADI_FEE_HANDLE* const phDevice);
ADI_FEE_RESULT adi_fee_Close             (ADI_FEE_HANDLE const hDevice);
ADI_FEE_RESULT adi_fee_RegisterCallback  (ADI_FEE_HANDLE const hDevice, ADI_CALLBACK const pfCallback, void* const pCBParam);

ADI_FEE_RESULT adi_fee_PageErase         (ADI_FEE_HANDLE const hDevice, uint32_t const nPageNumStart, uint32_t const nPageNumEnd, uint32_t* const pHwErrors);
ADI_FEE_RESULT adi_fee_MassErase         (ADI_FEE_HANDLE const hDevice, uint32_t* const pHwErrors);

ADI_FEE_RESULT adi_fee_Write             (ADI_FEE_HANDLE const hDevice, ADI_FEE_TRANSACTION* const pTransaction, uint32_t* const pHwErrors);
ADI_FEE_RESULT adi_fee_SubmitBuffer      (ADI_FEE_HANDLE const hDevice, ADI_FEE_TRANSACTION* const pTransaction);

ADI_FEE_RESULT adi_fee_IsBufferAvailable (ADI_FEE_HANDLE const hDevice, bool* const pbCompletionState);
ADI_FEE_RESULT adi_fee_GetBuffer         (ADI_FEE_HANDLE const hDevice, uint32_t* const pHwErrors);

ADI_FEE_RESULT adi_fee_GetPageNumber     (ADI_FEE_HANDLE const hDevice, uint32_t const nAddress, uint32_t* const pnPageNum);
ADI_FEE_RESULT adi_fee_GetBlockNumber    (ADI_FEE_HANDLE const hDevice, uint32_t const nAddress, uint32_t* const pnBlockNum);

ADI_FEE_RESULT adi_fee_VerifySignature   (ADI_FEE_HANDLE const hDevice, uint32_t const nStartPage, uint32_t const nEndPage, uint32_t* const pSigResult, uint32_t* const pHwErrors);
ADI_FEE_RESULT adi_fee_WriteProtectBlock (ADI_FEE_HANDLE const hDevice, uint32_t const  nBlockNum);

ADI_FEE_RESULT adi_fee_Sleep             (ADI_FEE_HANDLE const hDevice, bool const bSleep);
ADI_FEE_RESULT adi_fee_Abort             (ADI_FEE_HANDLE const hDevice);
ADI_FEE_RESULT adi_fee_GetAbortAddr      (ADI_FEE_HANDLE const hDevice, uint32_t* const pnAddress);

ADI_FEE_RESULT adi_fee_ConfigECC         (ADI_FEE_HANDLE const hDevice, uint32_t const nStartPage, bool const bInfoECCEnable);
ADI_FEE_RESULT adi_fee_EnableECC         (ADI_FEE_HANDLE const hDevice, bool const bEnable);
ADI_FEE_RESULT adi_fee_ConfigECCEvents   (ADI_FEE_HANDLE const hDevice, ADI_FEE_ECC_EVENT_TYPE const eEvent, ADI_FEE_ECC_RESPONSE const eResponse);
ADI_FEE_RESULT adi_fee_GetECCErrAddr     (ADI_FEE_HANDLE const hDevice, uint32_t* const pnAddress);
ADI_FEE_RESULT adi_fee_GetECCCorrections (ADI_FEE_HANDLE const hDevice, uint32_t* const pnNumCorrections);

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/
