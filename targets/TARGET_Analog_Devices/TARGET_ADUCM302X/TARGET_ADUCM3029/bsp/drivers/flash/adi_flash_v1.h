/*!
 *****************************************************************************
 @file:    adi_flash_v1.h
 @brief:   Flash Driver Implementations for ADuCM302x
 @version: $Revision$
 @date:    $Date$
 -----------------------------------------------------------------------------
Copyright (c) 2012-2014 Analog Devices, Inc.

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

/*! \addtogroup Flash_Driver Flash Controller Device Driver
 *  @{
 *
 *  <b>Flash controller driver</b>
 *
 *  Flash controller provides access to the embedded flash memory. The embedded
 *  flash has a 72-bit wide data bus providing for two 32-bit words of data and
 *  one corresponding 8-bit ECC byte per access.
 */

#ifndef __ADI_FLASH_V1_H__
#define __ADI_FLASH_V1_H__

#include <adi_flash_config.h>
#include <services/int/adi_int.h>
#include <adi_types.h>

#ifndef __ADUCM30xx__
#error "Not supported processor"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \enum ADI_FEE_RESULT
 * Flash Controller return codes
 */
 typedef enum {
     ADI_FEE_SUCCESS = 0,                /*!< The function completed successfully */
     ADI_FEE_UNKNOWN_ERROR,              /*!< An Unknown error has occurred */
     ADI_FEE_ERR_RTOS,                   /*!< Error Occurred in RTOS functions */
     ADI_FEE_ERR_BAD_DEVICE_NUM,         /*!< Device number passed is invalid */
     ADI_FEE_ERR_INVALID_HANDLE,         /*!< Device Handle is invalid */
     ADI_FEE_ERR_INVALID_PARAM,          /*!< Parameter passed to the function is invalid */
     ADI_FEE_ERR_INSUFFICIENT_MEM,       /*!< The memory passes is insufficient */
     ADI_FEE_ERR_ALREADY_INITIALIZED,    /*!< The device is already initialized */
     ADI_FEE_ERR_DEVICE_BUSY,            /*!< The device is busy */
     ADI_FEE_ERR_WRITE_PROTECTED,        /*!< The address is write protected */
     ADI_FEE_ERR_READ_VERIFY_ERROR,      /*!< Verify/Sign error */
     ADI_FEE_ABORTED,                    /*!< Command was aborted */
     ADI_FEE_ERR_NO_FREE_BUFFERS,        /*!< No free internal buffers */
     ADI_FEE_ERR_ALIGNMENT,              /*!< Alignment is incorrect */
     ADI_FEE_ERR_BUFFER_ERR,             /*!< Error occured while processing the buffer */
     ADI_FEE_ERR_TRANSFER_IN_PROGRESS,   /*!< Operation cannot be completed as a transfer
                                             is in progress
                                             */
     ADI_FEE_ERR_INVALID_MODE,           /*!< Invalid Mode */
     ADI_FEE_ERR_DMA                     /*!< Error occurred in DMA functions */
 } ADI_FEE_RESULT;

/*! The amount of application supplied memory required by the Flash Controller */
#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
#define ADI_FEE_MEMORY_SIZE            (196u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#else
#define ADI_FEE_MEMORY_SIZE            (92u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#endif

/*! Flash Controller handle */
typedef void* ADI_FEE_HANDLE;

/*! Enum for the Argument passed the callback function when ADI_FEE_EVENT_ECC_ERR occurred */
typedef enum {
    ADI_FEE_ECC_ERROR_2_BIT_ERROR       = 0x1,   /*!< 2-Bit Error occurred */
    ADI_FEE_ECC_ERROR_1_BIT_CORRECTED   = 0x2,   /*!< 1-Bit Error occurred and is corrected */
    ADI_FEE_ECC_ERROR_1_OR_2_BIT        = 0x3    /*!< 1 or 2 Bit error occurred */
} ADI_FEE_ECC_ERROR;

/*! Enum for the callback events  */
typedef enum {
     ADI_FEE_EVENT_BUFFER_PROCESSED,    /*!< Buffer processed successfully event */
     ADI_FEE_EVENT_BUFFER_ERR,          /*!< Buffer processed with error event */
     ADI_FEE_EVENT_ECC_ERR              /*!< ECC Error occurred */
} ADI_FEE_EVENT;

/*! Enum for the ECC Event Type */
typedef enum {
    ADI_FEE_ECC_EVENT_TYPE_ERROR,       /*!< ECC Error Event */
} ADI_FEE_ECC_EVENT_TYPE;

/*! Response required to ECC events */
typedef enum {
    ADI_FEE_ECC_RESPONSE_NONE       = 0x0,      /*!< No Response */
    ADI_FEE_ECC_RESPONSE_BUS_ERROR  = 0x1,      /*!< Generate a Bus Error */
    ADI_FEE_ECC_RESPONSE_IRQ        = 0x2       /*!< Generate an IRQ */
} ADI_FEE_ECC_RESPONSE;

/*================ E X T E R N A L S ==================*/
/* Flash Controller API */

/* Open the flash controller */
ADI_FEE_RESULT adi_fee_Open (
                             uint32_t         const nDeviceNum,
                             void*            const pMemory,
                             uint32_t         const nMemorySize,
                             ADI_FEE_HANDLE*  const phDevice
                             );

/* Close the frash controller */
ADI_FEE_RESULT adi_fee_Close (ADI_FEE_HANDLE hDevice);

/* Register Callback */
ADI_FEE_RESULT adi_fee_RegisterCallback (
                                         ADI_FEE_HANDLE   const hDevice,
                                         ADI_CALLBACK     const pfCallback,
                                         void*            const pCBParam
                                         );

/* Mass Erase the flash controller */
ADI_FEE_RESULT adi_fee_MassErase (ADI_FEE_HANDLE   const hDevice);

/* Erase the given page in the flash controller */
ADI_FEE_RESULT adi_fee_PageErase (
                                  ADI_FEE_HANDLE   const hDevice,
                                  uint32_t         const nPageNumStart,
                                  uint32_t         const nPageNumEnd
                                  );

/* Sleep/Wake Up the Flash Controller */
ADI_FEE_RESULT adi_fee_Sleep (
                              ADI_FEE_HANDLE const hDevice,
                              bool_t         const bSleep
                              );

/* Blocking write(copy) nBytes of data from pData to flash memory starting at the nStartAddress */
ADI_FEE_RESULT adi_fee_Write (
                              ADI_FEE_HANDLE    const  hDevice,
                              uint32_t          const  nStartAddress,
                              void*             const  pBufferData,
                              uint32_t          const  nBufferSize
                              );

/* Non-Blocking write(copy) nBytes of data from pData to flash memory starting at the nStartAddress */
ADI_FEE_RESULT adi_fee_SubmitTxBuffer (
                                       ADI_FEE_HANDLE    const  hDevice,
                                       uint32_t          const  nStartAddress,
                                       void*             const  pBufferData,
                                       uint32_t          const  nBufferSize
                                       );

/* Pooling for the completion of Tx Operation */
ADI_FEE_RESULT adi_fee_IsTxBufferAvailable (
                                            ADI_FEE_HANDLE    const  hDevice,
                                            bool_t*           const  pbAvailable
                                            );

/* Get the buffers submitted by the adi_fee_SubmitTxBuffer call */
ADI_FEE_RESULT adi_fee_GetTxBuffer (
                                    ADI_FEE_HANDLE    const  hDevice,
                                    void**            const  ppBuffer
                                    );


/* Get the block number to which the given address belongs in the flash */
ADI_FEE_RESULT adi_fee_GetBlockNumber (
                                       ADI_FEE_HANDLE    const  hDevice,
                                       uint32_t          const  nAddress,
                                       uint32_t*         const  pnBlockNum
                                       );

/* Get the page number to which the given address belongs in the flash */
ADI_FEE_RESULT adi_fee_GetPageNumber (
                                       ADI_FEE_HANDLE    const  hDevice,
                                       uint32_t          const  nAddress,
                                       uint32_t*         const  pnPageNum
                                       );

/* Generate and Verify the signature for the pages from nLowerPage to nUpperPage */
ADI_FEE_RESULT adi_fee_VerifySignature (
                                        ADI_FEE_HANDLE    const  hDevice,
                                        uint32_t          const  nLowerPage,
                                        uint32_t          const  nUpperPage,
                                        uint32_t*         const  pSigResult
                                        );

/* Protect the given block from write */
ADI_FEE_RESULT adi_fee_WriteProtectBlock (
                                          ADI_FEE_HANDLE    const  hDevice,
                                          uint32_t          const  nBlockNum
                                          );

/* Forcefully ABORT the current flash operation */
ADI_FEE_RESULT adi_fee_Abort (ADI_FEE_HANDLE const hDevice);

/* Configure ECC */
ADI_FEE_RESULT adi_fee_ConfigECC (
                                  ADI_FEE_HANDLE   const hDevice,
                                  uint32_t         const nStartPage,
                                  bool_t           const bInfoECCEnable
                                  );

/* Enable/Disable ECC */
ADI_FEE_RESULT adi_fee_EnableECC (
                                  ADI_FEE_HANDLE     const hDevice,
                                  bool_t             const bEnable
                                  );

/* Configure ECC event reporting behaviour */
ADI_FEE_RESULT adi_fee_ConfigECCEvents (
                                        ADI_FEE_HANDLE         const hDevice,
                                        ADI_FEE_ECC_EVENT_TYPE const eEvent,
                                        ADI_FEE_ECC_RESPONSE   const eResponse
                                        );

/* Get the address for which the ECC error occurred */
ADI_FEE_RESULT adi_fee_GetECCErrAddr (
                                      ADI_FEE_HANDLE     const hDevice,
                                      uint32_t*          const pnAddress
                                      );

/* Get the number of 1-bit errors which are corrected */
ADI_FEE_RESULT adi_fee_GetECCCorrections (
                                          ADI_FEE_HANDLE  const hDevice,
                                          uint32_t*       const pnNumCorrections
                                          );

/* Get the address for which the abort write occurred */
ADI_FEE_RESULT adi_fee_GetAbortAddr (
                                     ADI_FEE_HANDLE     const hDevice,
                                     uint32_t*          const pnAddress
                                     );

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
/* Enable/Disable DMA */
ADI_FEE_RESULT adi_fee_EnableDMA (
                                  ADI_FEE_HANDLE  const hDevice,
                                  bool_t          const bEnable
                                  );
#endif

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/
