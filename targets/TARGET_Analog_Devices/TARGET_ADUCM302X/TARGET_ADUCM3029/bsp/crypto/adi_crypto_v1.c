/*!
 *****************************************************************************
 @file:    adi_crypto_v1.c
 @brief:   Crypto Device Implementations for ADuCM302x
 @version: $Revision$
 @date:    $Date$
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
/*! \addtogroup Crypto_Driver Crypto Device Driver
 *  Crypto Device Driver
 *  @{
 *
 *  <b>Crypto peripheral driver</b>
 *
 *  The Crypto block is an accelerator implementing the AES Cipher and different
 *  block cipher modes of operation of the Cipher. Hardware support is provided for
 *  almost the entire encryption/decryption process.
 *
 *  <b>Memory DMA driver Crypto operation</b>
 *
 *  The crypto module can use DMA stream to perform its operations. The Crypto
 *  DMA mode can be enabled using adi_crpto_EnableDmaMode() API.
 *
 */

#ifndef _ADI_CRYPTO_V1_C_

/*! \cond PRIVATE */
#define _ADI_CRYPTO_V1_C_

/*========  I N C L U D E  ========*/

#include <string.h>
#include <assert.h>

#include <services/int/adi_int.h>
#include <ssdd_common/common_def.h>
#include <drivers/crypto/adi_crypto.h>
#include "adi_crypto_config.h"
#include "adi_crypto_def_v1.h"

/*========  D E F I N E S   ========*/

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ADI_INSTALL_HANDLER and others.
*
* Pm088 (rule 17.4): pointer arithmetic should not be used.
* Pm152 (rule 17.4): array indexing shall only be applied to objects defined as an array type
*   Relying on pointer arithmetic for buffer handling.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm088,Pm152
#endif /* __ICCARM__ */

/* Macros used for denoting the direction */
#define DIR_INPUT               (0u)
#define DIR_OUTPUT              (1u)
#define DIR_IN_OUT              (2u)

/* Utility Macros */
#define CLR_BITS(REG,BITS)         ((REG) &= ~(BITS))
#define SET_BITS(REG,BITS)         ((REG) |= (BITS))
#define IS_ANY_BIT_SET(REG,BITS)   (((REG) & (BITS)) != 0u)


/* Number of crypto device for the given processor */
#define NUM_DEVICES             (1u)

#ifdef ADI_DEBUG
#define ASSERT(X)   assert(X)
#else
#define ASSERT(X)
#endif

#define __ADI_BYTE_SWAP(X)  __REV(X)


/*========  L O C A L    F U N C    D E C L  ========*/
/* Initialize the device */
static void Initialize (
                        uint32_t nDeviceNum,
                        CRYPTO_DEVICE* dev
                        );

/* Stop the device */
static void StopDevice (CRYPTO_DEVICE* dev);

/* Discard the buffers */
static void DiscardBuffers (CRYPTO_DEVICE* dev);

/* Submit the buffer to the device */
static ADI_CRYPTO_RESULT Crypto_SubmitBuffer (
                                              ADI_CRYPTO_HANDLE   hDevice,
                                              CRYPTO_MODE         eMode,
                                              void*               pBuffer
                                              );
/* Get buffer (pending) from the device */
static ADI_CRYPTO_RESULT Crypto_GetBuffer (
                                           ADI_CRYPTO_HANDLE        hDevice,
                                           CRYPTO_MODE              eMode,
                                           void**                   ppBuffer
                                           );

/* Check whether a completed buffer is available in the device */
static ADI_CRYPTO_RESULT Crypto_IsBufferAvailable (
                                                   ADI_CRYPTO_HANDLE     hDevice,
                                                   CRYPTO_MODE           eMode,
                                                   bool_t*               pbAvailable
                                                   );

/* Initialize the computation for a buffer */
static void InitBufferCompute (CRYPTO_DEVICE* dev);

/* DMA FIFO Management function */
static ADI_CRYPTO_RESULT DmaFIFOManage (
                                        CRYPTO_DEVICE    *dev,
                                        uint32_t          nDir,
                                        CRYPTO_FIFO_SEQ   eFifoSeq
                                        );

/* PIO mode FIFO management function */
static ADI_CRYPTO_RESULT PioFIFOManage (
                                        CRYPTO_DEVICE   *dev,
                                        uint32_t         nDir,
                                        CRYPTO_FIFO_SEQ  eFifoSeq
                                        );

/* Transfer Block Info the input FIFO */
static void TransferBlockToInputFIFO(CRYPTO_DEVICE *dev, bool_t bDma);

/* Flush the input and output buffers */
static void FlushBuffers(CRYPTO_DEVICE *dev);

/* Manage FIFO in the interrupt region */
static void ManageFiFoInterrupt(CRYPTO_DEVICE* dev, uint32_t nDir);

/* Check for input buffer done */
static bool_t IsInputBufferDone(CRYPTO_DEVICE *dev);

/* Static process buffer completion */
static void ProcessBufferCompletion (CRYPTO_DEVICE* dev);

/* DMA Callback Handler */
static void DMAHandler (void *pcbParam, uint32_t Event, void *pArg, uint32_t nDir);

/** Buffer Utility Functions **/

/* Pop a buffer from the buffer list */
static CRYPTO_BUFFER* pop_buffer (CRYPTO_BUFFER** ppBuffList);

/* Push a buffer to the back of the buffer list */
static void bpush_buffer (
                          CRYPTO_BUFFER** ppBuffList,
                          CRYPTO_BUFFER*  pBuffer
                          );

static void FillCryptoBufferFromModeBuffer(
                                         uint32_t                 nMode,
                                         void*                    pExtBuffer,
                                         CRYPTO_BUFFER*           pIntBuffer,
                                         ADI_CRYPTO_CONFIG**      ppConfig
                                         );

#ifdef ADI_DEBUG
static ADI_CRYPTO_RESULT ValidateIntCryptoBuffer(CRYPTO_BUFFER* pIntBuff);


/* Validate the given device handle */
static ADI_CRYPTO_RESULT ValidateHandle(CRYPTO_DEVICE* dev);
#endif

/* Un-initialize the DMA */
static void UnInitDMA (CRYPTO_DEVICE *dev);


/* forward declarations... */
/* These are needed in the case where ADI_CFG_ENABLE_RTOS_SUPPORT=1              */
/* The ADI_INSTALL_HANDLER macro will expand to a call to adi_int_InstallHandler */
/* and the handler will be referenced                                            */
/* The non-OSAL expansion of ADI_INSTALL_HANDLER results in a NULL string        */
ADI_INT_HANDLER(Crypto_Int_Handler);

/* DMA Handler */
static void DMAInpHandler (void *pcbParam, uint32_t Event, void *pArg);
static void DMAOutHandler (void *pcbParam, uint32_t Event, void *pArg);

/* Generate a u32 from a pointer to u8 buffer */
static uint32_t u32FromU8p (uint8_t *pData);


/*========  D A T A  ========*/
/* Internal device structure */

#ifdef __ICCARM__
/*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for MMR address casts as in the pADI_CRYPTO macro here.
*/
#pragma diag_suppress=Pm140
#endif /* __ICCARM__ */

static CRYPTO_INFO CryptoDevInfo[] = {
    {pADI_CRYPT0,  NULL}
};

#ifdef __ICCARM__
#pragma diag_default=Pm140
#endif /* __ICCARM__ */

/*! \endcond */

/*========  C O D E  ========*/
/**
 * @brief    Opens a Crypto device instance
 *
 * @param [in]  nDeviceNum   Device number to open
 * @param [in]  pMemory      Pointer to a #ADI_CRYPTO_MEMORY_SIZE sized buffer to manage the device
 *                           instance.
 * @param [in]  nMemorySize  Size of the buffer to which "pMemory" points
 * @param [out] phDevice     Pointer to a location where CRC device handle is to be written.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Call completed successfully
 *           - #ADI_CRYPTO_ERR_BAD_DEVICE_NUM       [D] Error: The device number is invalid
 *           - #ADI_CRYPTO_ERR_INSUFFICIENT_MEM     [D] Error: The memory passeed to the device is insufficient
 *           - #ADI_CRYPTO_ERR_ALREADY_INITIALIZED  [D] Error: The device is already opened
 */
ADI_CRYPTO_RESULT adi_crypto_Open(
                                  uint32_t               nDeviceNum,
                                  void*                  pMemory,
                                  uint32_t               nMemorySize,
                                  ADI_CRYPTO_HANDLE*     phDevice
                                  )
{
    CRYPTO_DEVICE *dev;

#ifdef ADI_DEBUG
    if (nDeviceNum >= NUM_DEVICES) {
        return ADI_CRYPTO_ERR_BAD_DEVICE_NUM;
    }

    if ((pMemory == NULL) || (phDevice == NULL)) {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }

    if (nMemorySize < ADI_CRYPTO_MEMORY_SIZE) {
        return ADI_CRYPTO_ERR_INSUFFICIENT_MEM;
    }

    if (CryptoDevInfo[nDeviceNum].pDevice != NULL) {
        return ADI_CRYPTO_ERR_ALREADY_INITIALIZED;
    }
#ifdef ADI_DEV
    if (ADI_CRYPTO_MEMORY_SIZE != sizeof(CRYPTO_DEVICE)) {
      printf("sizeof(CRYPTO_DEVICE) = %d, ADI_CRYPTO_MEMORY_SIZE = %d", sizeof(CRYPTO_DEVICE), ADI_CRYPTO_MEMORY_SIZE);
    }
#endif /* ADI_DEV */
    assert (ADI_CRYPTO_MEMORY_SIZE == sizeof(CRYPTO_DEVICE));
#endif /* ADI_DEBUG */

    /* Get the device */
    dev = (CRYPTO_DEVICE*)pMemory;

    /* Initialize the device structure */
    Initialize(nDeviceNum, dev);

    /* Set the semaphore */
#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* create a semaphore for buffer management */
    CREATE_SEM(dev,ADI_CRYPTO_ERR_RTOS);
#endif

    /* Set the register mapping for the device */
    dev->pReg = CryptoDevInfo[nDeviceNum].pReg;

    /* Install interrupt handles */
    ADI_INSTALL_HANDLER(CRYPT_EVT_IRQn, Crypto_Int_Handler);

    /* Store the device memory in the internal structure */
    CryptoDevInfo[nDeviceNum].pDevice = dev;

    /* Give the handle back to the application */
    *phDevice = dev;

    /* Return success */
    return ADI_CRYPTO_SUCCESS;
}

/**
 * @brief    Close the given device instance
 *
 * @param [in]  hDevice  Handle to the device instance
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully closed the device
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_Close(ADI_CRYPTO_HANDLE hDevice)
{
    uint32_t x;
    ADI_CRYPTO_RESULT result;
    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }
#endif /* ADI_DEBUG */

    /* IF (The device is enabled) */
    if (dev->bDeviceEnabled) {
        result = adi_crypto_Enable(hDevice, false);
        if (result != ADI_CRYPTO_SUCCESS) {
            return result;
        }
    }

#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* Destroy the semaphore */
    DELETE_SEM(dev,ADI_CRYPTO_ERR_RTOS);
#endif

    /* Close the device */
    for (x=0u; x < NUM_DEVICES; x++) {
        if (CryptoDevInfo[x].pDevice == dev) {
            CryptoDevInfo[x].pDevice = NULL;
            break;
        }
    }

    return ADI_CRYPTO_SUCCESS;
}

/**
 * @brief   Registering a callback function
 *
 * @param [in]  hDevice     Handle to the device instance
 * @param [in]  pfCallback  Function pointer to callback function. Passing a NULL pointer will
 *                          unregister the call back function.
 * @param [in]  pCBParam    Call back function parameter
 *
 * @details  This function registers a call back function. Registered function will be called when
 *           the given computation is over. This will enable the NON-BLOCKING-CALLBACK mode and any
 *           further call to adi_crypto_GetECBBuffer or adi_crypto_GetCCMBuffer will fail.
 *
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully registerd the callback
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 */
 ADI_CRYPTO_RESULT adi_crypto_RegisterCallback (
                                                ADI_CRYPTO_HANDLE      hDevice,
                                                ADI_CALLBACK           pfCallback,
                                                void*                  pCBParam
                                                )
{

    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }
#endif /* ADI_DEBUG */

    /* Set the values */
    dev->pfCallback = pfCallback;
    dev->pCBParam = pCBParam;

    return ADI_CRYPTO_SUCCESS;
}


#if (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)

/**
 * @brief    Submit the ECB buffer for processing to the CRYPTO Modules
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_ECB_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer
 *           - #ADI_CRYPTO_ERR_BUFFER_FULL          Error: No more free internal buffers are available.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid
 *           - #ADI_CRYPTO_ERR_BAD_CONFIG       [D] Error: The configuration structure passes is invalid
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: One or more parameters submitted is invalid
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 * @note    The driver will take ownership of the ADI_CRYPTO_ECB_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitECBBuffer (
                                            ADI_CRYPTO_HANDLE         hDevice,
                                            ADI_CRYPTO_ECB_BUFFER*    pBuffer
                                            )
{
    ADI_CRYPTO_RESULT result;

    /* Submit buffer to the device */
    result = Crypto_SubmitBuffer(
                                 hDevice,
                                 CRYPTO_MODE_ECB,
                                 pBuffer
                                 );
   return result;
}

/**
 * @brief   Get the ECB buffer back from the driver after computation. It is a blocking call. This function
 *          should not be called if a callback function is registered.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer
 *           - #ADI_CRYPTO_ERR_FIFO                     Error: If overflow error occurred in the device while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_DMA                      Error: If DMA error occurred while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_BUFFER_PROCESSING        Error: Error occurred in processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE        [D] Error: Invalid call since a callback is registered
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_GetECBBuffer (
                                        ADI_CRYPTO_HANDLE          hDevice,
                                        ADI_CRYPTO_ECB_BUFFER**    ppBuffer
                                        )
{
    ADI_CRYPTO_RESULT result;

    /* Call common get buffer function */
    result = Crypto_GetBuffer(
                              hDevice,
                              CRYPTO_MODE_ECB,
                              (void *)ppBuffer
                              );

    return result;
}



/**
 * @brief    Peek function to know whether a completed ECB buffer is available
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a boolean variable. Contains "True" if there is a completed
 *                            buffer and a call to adi_crypto_GetECBBuffer is ensured to be successful.
 *                            Contains "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_IsECBBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                )
{
    ADI_CRYPTO_RESULT result;

    /* Call the common buffer peek function */
    result = Crypto_IsBufferAvailable(
                                      hDevice,
                                      CRYPTO_MODE_ECB,
                                      pbAvailable
                                      );

    return result;
}

#endif /* ADI_CRYPTO_ENABLE_ECB_SUPPORT */


#if (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)

/**
 * @brief    Submit the CTR buffer for processing to the CRYPTO Modules
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_CTR_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer
 *           - #ADI_CRYPTO_ERR_BUFFER_FULL          Error: No more free internal buffers are available.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid
 *           - #ADI_CRYPTO_ERR_BAD_CONFIG       [D] Error: The configuration structure passes is invalid
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: One or more parameters submitted is invalid
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 * @note    The driver will take ownership of the ADI_CRYPTO_CTR_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCTRBuffer (
                                            ADI_CRYPTO_HANDLE         hDevice,
                                            ADI_CRYPTO_CTR_BUFFER*    pBuffer
                                            )
{
    ADI_CRYPTO_RESULT result;

    /* Submit buffer to the device */
    result = Crypto_SubmitBuffer(
                                 hDevice,
                                 CRYPTO_MODE_CTR,
                                 pBuffer
                                 );
   return result;
}

/**
 * @brief   Get the CTR buffer back from the driver after computation. It is a blocking call. This function
 *          should not be called if a callback function is registered.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer
 *           - #ADI_CRYPTO_ERR_FIFO                     Error: If overflow error occurred in the device while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_DMA                      Error: If DMA error occurred while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_BUFFER_PROCESSING        Error: Error occurred in processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE        [D] Error: Invalid call since a callback is registered
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_GetCTRBuffer (
                                        ADI_CRYPTO_HANDLE          hDevice,
                                        ADI_CRYPTO_CTR_BUFFER**    ppBuffer
                                        )
{
    ADI_CRYPTO_RESULT result;

    /* Call common get buffer function */
    result = Crypto_GetBuffer(
                              hDevice,
                              CRYPTO_MODE_CTR,
                              (void *)ppBuffer
                              );

    return result;
}



/**
 * @brief    Peek function to know whether a completed CTR buffer is available
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a boolean variable. Contains "True" if there is a completed
 *                            buffer and a call to adi_crypto_GetCTRBuffer is ensured to be successful.
 *                            Contains "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_IsCTRBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                )
{
    ADI_CRYPTO_RESULT result;

    /* Call the common buffer peek function */
    result = Crypto_IsBufferAvailable(
                                      hDevice,
                                      CRYPTO_MODE_CTR,
                                      pbAvailable
                                      );

    return result;
}

#endif /* ADI_CRYPTO_ENABLE_CTR_SUPPORT */


#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)

/**
 * @brief    Submit the CBC buffer for processing to the CRYPTO Modules
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_CBC_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer
 *           - #ADI_CRYPTO_ERR_BUFFER_FULL          Error: No more free internal buffers are available.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid
 *           - #ADI_CRYPTO_ERR_BAD_CONFIG       [D] Error: The configuration structure passes is invalid
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: One or more parameters submitted is invalid
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 * @note    The driver will take ownership of the ADI_CRYPTO_CBC_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCBCBuffer (
                                            ADI_CRYPTO_HANDLE         hDevice,
                                            ADI_CRYPTO_CBC_BUFFER*    pBuffer
                                            )
{
    ADI_CRYPTO_RESULT result;

    /* Submit buffer to the device */
    result = Crypto_SubmitBuffer(
                                 hDevice,
                                 CRYPTO_MODE_CBC,
                                 pBuffer
                                 );
   return result;
}

/**
 * @brief   Get the CBC buffer back from the driver after computation. It is a blocking call. This function
 *          should not be called if a callback function is registered.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer
 *           - #ADI_CRYPTO_ERR_FIFO                     Error: If overflow error occurred in the device while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_DMA                      Error: If DMA error occurred while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_BUFFER_PROCESSING        Error: Error occurred in processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE        [D] Error: Invalid call since a callback is registered
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_GetCBCBuffer (
                                        ADI_CRYPTO_HANDLE          hDevice,
                                        ADI_CRYPTO_CBC_BUFFER**    ppBuffer
                                        )
{
    ADI_CRYPTO_RESULT result;

    /* Call common get buffer function */
    result = Crypto_GetBuffer(
                              hDevice,
                              CRYPTO_MODE_CBC,
                              (void *)ppBuffer
                              );

    return result;
}



/**
 * @brief    Peek function to know whether a completed CBC buffer is available
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a boolean variable. Contains "True" if there is a completed
 *                            buffer and a call to adi_crypto_GetCBCBuffer is ensured to be successful.
 *                            Contains "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_IsCBCBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                )
{
    ADI_CRYPTO_RESULT result;

    /* Call the common buffer peek function */
    result = Crypto_IsBufferAvailable(
                                      hDevice,
                                      CRYPTO_MODE_CBC,
                                      pbAvailable
                                      );

    return result;
}

#endif /* ADI_CRYPTO_ENABLE_CBC_SUPPORT */


#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)

/**
 * @brief    Submit the CCM buffer for processing to the CRYPTO Modules
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_CCM_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer
 *           - #ADI_CRYPTO_ERR_BUFFER_FULL          Error: No more free internal buffers are available.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid
 *           - #ADI_CRYPTO_ERR_BAD_CONFIG       [D] Error: The configuration structure passes is invalid
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: One or more parameters submitted is invalid
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 * @note    The driver will take ownership of the ADI_CRYPTO_CCM_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCCMBuffer (
                                            ADI_CRYPTO_HANDLE         hDevice,
                                            ADI_CRYPTO_CCM_BUFFER*    pBuffer
                                            )
{
    ADI_CRYPTO_RESULT result;

    /* Submit buffer to the device */
    result = Crypto_SubmitBuffer(
                                 hDevice,
                                 CRYPTO_MODE_CCM,
                                 pBuffer
                                 );
   return result;
}

/**
 * @brief   Get the CCM buffer back from the driver after computation. It is a blocking call. This function
 *          should not be called if a callback function is registered.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer
 *           - #ADI_CRYPTO_ERR_FIFO                     Error: If overflow error occurred in the device while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_DMA                      Error: If DMA error occurred while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_BUFFER_PROCESSING        Error: Error occurred in processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE        [D] Error: Invalid call since a callback is registered
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_GetCCMBuffer (
                                        ADI_CRYPTO_HANDLE          hDevice,
                                        ADI_CRYPTO_CCM_BUFFER**    ppBuffer
                                        )
{
    ADI_CRYPTO_RESULT result;

    /* Call common get buffer function */
    result = Crypto_GetBuffer(
                              hDevice,
                              CRYPTO_MODE_CCM,
                              (void *)ppBuffer
                              );

    return result;
}



/**
 * @brief    Peek function to know whether a completed CCM buffer is available
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a boolean variable. Contains "True" if there is a completed
 *                            buffer and a call to adi_crypto_GetCCMBuffer is ensured to be successful.
 *                            Contains "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_IsCCMBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                )
{
    ADI_CRYPTO_RESULT result;

    /* Call the common buffer peek function */
    result = Crypto_IsBufferAvailable(
                                      hDevice,
                                      CRYPTO_MODE_CCM,
                                      pbAvailable
                                      );

    return result;
}

#endif /* ADI_CRYPTO_ENABLE_CCM_SUPPORT */


#if (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)

/**
 * @brief    Submit the CMAC buffer for processing to the CRYPTO Modules
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_CMAC_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer
 *           - #ADI_CRYPTO_ERR_BUFFER_FULL          Error: No more free internal buffers are available.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid
 *           - #ADI_CRYPTO_ERR_BAD_CONFIG       [D] Error: The configuration structure passes is invalid
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: One or more parameters submitted is invalid
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 * @note    The driver will take ownership of the ADI_CRYPTO_CMAC_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitCMACBuffer (
                                            ADI_CRYPTO_HANDLE         hDevice,
                                            ADI_CRYPTO_CMAC_BUFFER*    pBuffer
                                            )
{
    ADI_CRYPTO_RESULT result;

    /* Submit buffer to the device */
    result = Crypto_SubmitBuffer(
                                 hDevice,
                                 CRYPTO_MODE_CMAC,
                                 pBuffer
                                 );
   return result;
}

/**
 * @brief   Get the CMAC buffer back from the driver after computation. It is a blocking call. This function
 *          should not be called if a callback function is registered.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer
 *           - #ADI_CRYPTO_ERR_FIFO                     Error: If overflow error occurred in the device while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_DMA                      Error: If DMA error occurred while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_BUFFER_PROCESSING        Error: Error occurred in processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE        [D] Error: Invalid call since a callback is registered
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_GetCMACBuffer (
                                        ADI_CRYPTO_HANDLE          hDevice,
                                        ADI_CRYPTO_CMAC_BUFFER**    ppBuffer
                                        )
{
    ADI_CRYPTO_RESULT result;

    /* Call common get buffer function */
    result = Crypto_GetBuffer(
                              hDevice,
                              CRYPTO_MODE_CMAC,
                              (void *)ppBuffer
                              );

    return result;
}



/**
 * @brief    Peek function to know whether a completed CMAC buffer is available
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a boolean variable. Contains "True" if there is a completed
 *                            buffer and a call to adi_crypto_GetCMACBuffer is ensured to be successful.
 *                            Contains "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_IsCMACBufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                )
{
    ADI_CRYPTO_RESULT result;

    /* Call the common buffer peek function */
    result = Crypto_IsBufferAvailable(
                                      hDevice,
                                      CRYPTO_MODE_CMAC,
                                      pbAvailable
                                      );

    return result;
}

#endif /* ADI_CRYPTO_ENABLE_CMAC_SUPPORT */


#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)

/**
 * @brief    Submit the SHA buffer for processing to the CRYPTO Modules
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_SHA_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer
 *           - #ADI_CRYPTO_ERR_BUFFER_FULL          Error: No more free internal buffers are available.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid
 *           - #ADI_CRYPTO_ERR_BAD_CONFIG       [D] Error: The configuration structure passes is invalid
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: One or more parameters submitted is invalid
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 * @note    The driver will take ownership of the ADI_CRYPTO_SHA_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitSHABuffer (
                                            ADI_CRYPTO_HANDLE         hDevice,
                                            ADI_CRYPTO_SHA_BUFFER*    pBuffer
                                            )
{
    ADI_CRYPTO_RESULT result;

    /* Submit buffer to the device */
    result = Crypto_SubmitBuffer(
                                 hDevice,
                                 CRYPTO_MODE_SHA,
                                 pBuffer
                                 );
   return result;
}

/**
 * @brief   Get the SHA buffer back from the driver after computation. It is a blocking call. This function
 *          should not be called if a callback function is registered.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer
 *           - #ADI_CRYPTO_ERR_FIFO                     Error: If overflow error occurred in the device while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_DMA                      Error: If DMA error occurred while processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_BUFFER_PROCESSING        Error: Error occurred in processing the buffer.
 *                                                             ppBuffer will be filled in this case.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE        [D] Error: Invalid call since a callback is registered
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_GetSHABuffer (
                                        ADI_CRYPTO_HANDLE          hDevice,
                                        ADI_CRYPTO_SHA_BUFFER**    ppBuffer
                                        )
{
    ADI_CRYPTO_RESULT result;

    /* Call common get buffer function */
    result = Crypto_GetBuffer(
                              hDevice,
                              CRYPTO_MODE_SHA,
                              (void *)ppBuffer
                              );

    return result;
}



/**
 * @brief    Peek function to know whether a completed SHA buffer is available
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a boolean variable. Contains "True" if there is a completed
 *                            buffer and a call to adi_crypto_GetSHABuffer is ensured to be successful.
 *                            Contains "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 */
ADI_CRYPTO_RESULT adi_crypto_IsSHABufferAvailable (
                                                ADI_CRYPTO_HANDLE        hDevice,
                                                bool_t*                  pbAvailable
                                                )
{
    ADI_CRYPTO_RESULT result;

    /* Call the common buffer peek function */
    result = Crypto_IsBufferAvailable(
                                      hDevice,
                                      CRYPTO_MODE_SHA,
                                      pbAvailable
                                      );

    return result;
}

#endif /* ADI_CRYPTO_ENABLE_SHA_SUPPORT */


/**
 * @brief    Enable/Disable the device. Enabling the device cause the buffers submitted to the device to
 *           be processed.
 *
 * @param [in]  hDevice   Handle to the device instance
 * @param [in]  bEnable   'true' to enable and 'false' to disable the device.
 *
* @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully enabled/disabled the device
 *           - #ADI_CRYPTO_ERR_INVALID_STATE    [D] Error: Calling enable when device is already enabled or
 *                                                         disable when the device is already disabled.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 */
ADI_CRYPTO_RESULT adi_crypto_Enable (
                                     ADI_CRYPTO_HANDLE        hDevice,
                                     bool_t                   bEnable
                                     )
{

    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }

    if (bEnable == dev->bDeviceEnabled) {
        return ADI_CRYPTO_ERR_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    if (bEnable) {

        /* Enable the IRQs */
        ADI_ENABLE_INT(CRYPT_EVT_IRQn);

        /* Mark the device as enabled */
        dev->bDeviceEnabled = true;

        /* Start processing buffer */
        InitBufferCompute(dev);
    } else {

        /* Disable the IRQs */
        ADI_DISABLE_INT(CRYPT_EVT_IRQn);

        /* Stop the device */
        StopDevice(dev);

        /* Discard all the buffers */
        DiscardBuffers(dev);

        /* Mark the device as disabled */
        dev->bDeviceEnabled = false;
    }

    /* Return success */
    return ADI_CRYPTO_SUCCESS;
}

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/**
 * @brief   Enable/Disable the DMA mode for the device
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  bEnable   'true' will enable DMA and 'false' disables the DMA
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully enabled/disabled the DMA
 *           - #ADI_CRYPTO_ERR_INVALID_STATE    [D] Error: DMA cannot be enabled or disabled when the device is enabled.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid
 *
 */
ADI_CRYPTO_RESULT adi_crypto_EnableDmaMode (
                                            ADI_CRYPTO_HANDLE       hDevice,
                                            bool_t                  bEnable
                                            )
{

    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }
    if (dev->bDeviceEnabled) {
        return ADI_CRYPTO_ERR_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    /* IF (DMA is to be enabled) */
    if (bEnable)
    {
        /* Enable the DMA */
        dev->bDmaEnabled = true;
        dev->pfFifoManage = DmaFIFOManage;
    }
    else
    {
        /* Disable the DMA */
        dev->bDmaEnabled = false;
        dev->pfFifoManage = PioFIFOManage;
    }

    /* Return success */
    return ADI_CRYPTO_SUCCESS;
}

#endif





/*========  L O C A L    F U N C T I O N    D E F I N I T I O N S  ========*/

/* Generate a u32 from a pointer to u8 buffer */
static uint32_t u32FromU8p(uint8_t *pData)
{
    int32_t x = 0;
    uint32_t nValue = pData[3];

    for (x = 2; x >= 0; x--) {
        nValue = (nValue << 8u) | pData[x];
    }
    return nValue;
}

/* Initialize the device structure */
static void Initialize (
                        uint32_t        nDeviceNum,
                        CRYPTO_DEVICE*  dev
                        )
{
    int32_t x;

    /* Clear the device structure */
    memset(dev, 0, sizeof(CRYPTO_DEVICE));

    /* Initialize the buffer lists */
    for (x=0; x < (NUM_BUFFERS-1); x++) {
        dev->buffers[x].pNext = &dev->buffers[x+1];
    }
    dev->buffers[NUM_BUFFERS-1].pNext = NULL;
    dev->pFreeBuffers = &dev->buffers[0];

    if (nDeviceNum == 0u)
    {
#if (ADI_CRYPTO_ENABLE_DMA == 1)
        dev->bDmaEnabled = true;
        dev->pfFifoManage = DmaFIFOManage;
#else
        dev->bDmaEnabled = false;
        dev->pfFifoManage = PioFIFOManage;
#endif
    }
}

/* Init buffer processing */
static void InitBufferCompute(CRYPTO_DEVICE* dev)
{
    uint32_t x;
    CRYPTO_BUFFER* pIntBuff;

    /* Get pointer to the buffer in process */
    CRYPTO_COMPUTE* pCompute = &dev->ActCompute;

    if (   ((dev->ActCompute.state & (uint32_t)CRYPTO_COMPUTE_STATE_PROCESSING) != 0u) /* IF (    (Crypto engine is processing a buffer) */
        || (dev->pPendingBuffers == NULL)                                              /*      OR (No buffers in the pending list)       */
        || (!dev->bDeviceEnabled))                                                     /*      OR (Device is not enabled))                */
    {
        return;
    }

    /* Clear the Interrupts */
    dev->pReg->STAT = (BITM_CRYPT_STAT_SHADONE | BITM_CRYPT_STAT_INOVR);

    /* Pop a pending internal buffer */
    pIntBuff = pop_buffer(&dev->pPendingBuffers);

    /* Configure the buffer structure (By default set the status as succesful and will be changed if any error occurred)*/
    pIntBuff->eStatus = ADI_CRYPTO_SUCCESS;

    /* Configure the compute structure */
    pCompute->state = (uint32_t)CRYPTO_COMPUTE_STATE_PROCESSING;
    pCompute->pIntBuff = pIntBuff;

    /* Change the CFG to default */
    dev->pReg->CFG =  (uint32_t)pIntBuff->eBuffDataFormat | BITM_CRYPT_CFG_SHAINIT;

#if CRYPTO_SUPPORT_REQ_KEY_PROCESSING
    /* IF Key is present */
    if (pIntBuff->pKey != NULL)
    {
        uint32_t nKeyLenInWords;

        dev->pReg->CFG |=  (uint32_t)pIntBuff->eKeyLen
                         | ((pIntBuff->eEncMode == ADI_CRYPTO_ENC_MODE_ENCRYPT) ? BITM_CRYPT_CFG_ENCR : 0u);

        /* Set the number of key words to write to the register */
        if (pIntBuff->eKeyLen == (uint32_t)ADI_CRYPTO_KEY_LEN_128_BIT) {
            nKeyLenInWords = 4u;  /* ADI_CRYPTO_KEY_LEN_128_BIT */
        } else {
            nKeyLenInWords = 8u;  /* ADI_CRYPTO_KEY_LEN_256_BIT */
        }

        /* Configure the Key */
        for (x = 0u; x < nKeyLenInWords; x++)
        {
            volatile uint32_t* pKeyPos[8] = {
                &dev->pReg->AESKEY0,
                &dev->pReg->AESKEY1,
                &dev->pReg->AESKEY2,
                &dev->pReg->AESKEY3,
                &dev->pReg->AESKEY4,
                &dev->pReg->AESKEY5,
                &dev->pReg->AESKEY6,
                &dev->pReg->AESKEY7
            };
            *pKeyPos[x] = u32FromU8p(&pIntBuff->pKey[x*4u]);
        }
    }
#endif

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
    /* nMode is CBC, CCM, CTR */
    if (((1u << pIntBuff->nMode) & ((1u << CRYPTO_MODE_CBC) | (1u << CRYPTO_MODE_CCM) | (1u << CRYPTO_MODE_CTR))) != 0u)
    {
        assert (pIntBuff->pNonceIV != NULL);

        /* Configure Counter Init and NONCE value */
        dev->pReg->CNTRINIT = pIntBuff->nCounterInit;

        dev->pReg->NONCE0 =  u32FromU8p(&pIntBuff->pNonceIV[0]);
        dev->pReg->NONCE1 =  u32FromU8p(&pIntBuff->pNonceIV[4]);
        dev->pReg->NONCE2 =  u32FromU8p(&pIntBuff->pNonceIV[8]);

        dev->pReg->NONCE3 =  (uint32_t)pIntBuff->pNonceIV[12] <<  0u
                           | (uint32_t)pIntBuff->pNonceIV[13] <<  8u;

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
        if (pIntBuff->nMode == CRYPTO_MODE_CBC) {
            dev->pReg->NONCE3 =  dev->pReg->NONCE3
                               | (uint32_t)pIntBuff->pNonceIV[14] << 16u
                               | (uint32_t)pIntBuff->pNonceIV[15] << 24u;
        }
#endif    /* (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) */
    }
#endif    /* (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
            || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1) */

#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
#if CRYPTO_SUPPORT_MODE_ANY_NON_CCM
    if (pIntBuff->nMode == CRYPTO_MODE_CCM)
#endif
    {
        if ((pIntBuff->pPrefixBuffer != NULL) && (pIntBuff->nPrefixBuffLenInBits > 0u))
        {
            pCompute->pInput = pIntBuff->pPrefixBuffer;
            pCompute->nInpBitsRemaining = pIntBuff->nPrefixBuffLenInBits;
        }
        else
        {
            pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_PREFIX_DONE;

            pCompute->pInput = pIntBuff->pDataBuffer;
            pCompute->nInpBitsRemaining = pIntBuff->nDataBuffLenInBits;
        }
    }
#if CRYPTO_SUPPORT_MODE_ANY_NON_CCM
    else
#endif
#endif    /* (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) */
#if CRYPTO_SUPPORT_MODE_ANY_NON_CCM
    {
        /* Set the compute structures */
        pCompute->pInput = pIntBuff->pDataBuffer;
        pCompute->nInpBitsRemaining = pIntBuff->nDataBuffLenInBits;

        pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_PREFIX_DONE;
    }
#endif

    pCompute->pOutput = pIntBuff->pOutputBuffer;
    pCompute->nOutBitsRemaining = pIntBuff->nOutputBuffLenInBits;

    /* Enable the correct Mode */
    switch(pIntBuff->nMode)
    {
#if (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)
    case CRYPTO_MODE_ECB:
        /* Enable ECB Operation */
        dev->pReg->CFG |=  BITM_CRYPT_CFG_ECBEN;
        break;
#endif
#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
    case CRYPTO_MODE_CBC:
        /* Enable CBC Operation */
        dev->pReg->CFG |=  BITM_CRYPT_CFG_CBCEN;
        break;
#endif

#if (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
    case CRYPTO_MODE_CTR:
        /* Enable CTR Operation */
        dev->pReg->CFG |=  BITM_CRYPT_CFG_CTREN;
        break;
#endif
#if (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)
    case CRYPTO_MODE_CMAC:
        /* Enable CMAC Operation */
        dev->pReg->CFG |=  BITM_CRYPT_CFG_CMACEN;
        dev->pReg->DATALEN = pIntBuff->nDataBuffLenInBits / BLK_SIZE_IN_BITS;
        break;
#endif
#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
    case  CRYPTO_MODE_CCM:
        /* Enable CCM Operation */
        dev->pReg->CFG |=  BITM_CRYPT_CFG_CCMEN;
        dev->pReg->DATALEN   = pIntBuff->nDataBuffLenInBits / BLK_SIZE_IN_BITS;
        dev->pReg->PREFIXLEN = pIntBuff->nPrefixBuffLenInBits / BLK_SIZE_IN_BITS;
        dev->pReg->CCM_NUM_VALID_BYTES = pIntBuff->nValidBytesInLastBlock;
        break;
#endif
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    case CRYPTO_MODE_SHA:
        /* Enable SHA */
        dev->pReg->CFG  |= BITM_CRYPT_CFG_SHA256EN;
        break;
#endif

    default:
        ASSERT(0);
        break;
    }

    /* Call the FIFO management function to transfer the buffer to/from the device */
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    if (pIntBuff->nMode != CRYPTO_MODE_SHA)
#endif
    {
        dev->pfFifoManage(dev, DIR_OUTPUT, CRYPTO_FIFO_SEQ_BEFORE_BLK_EN);
    }
    dev->pfFifoManage(dev, DIR_INPUT, CRYPTO_FIFO_SEQ_BEFORE_BLK_EN);

    /* Enable the Device */
    SET_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_BLKEN);
    
    /* Call the FIFO management function to transfer the buffer to/from the device */
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    if (pIntBuff->nMode != CRYPTO_MODE_SHA)
#endif
    {
        dev->pfFifoManage(dev, DIR_OUTPUT, CRYPTO_FIFO_SEQ_AFTER_BLK_EN);
    }
    dev->pfFifoManage(dev, DIR_INPUT, CRYPTO_FIFO_SEQ_AFTER_BLK_EN);
    

    if (!dev->bDmaEnabled)
    {
        /* Enable the interrupts */
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
        if (pIntBuff->nMode == CRYPTO_MODE_SHA)
        {
            SET_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_SHADONEN);
        }
        else
#endif
        {
            SET_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_OUTRDYEN);
            SET_BITS(dev->pReg->INTEN, BITM_CRYPT_STAT_INRDY);
        }
    }

}

/* Get a completed buffer from the device (blocking call) */
static ADI_CRYPTO_RESULT Crypto_GetBuffer (
                                           ADI_CRYPTO_HANDLE        hDevice,
                                           CRYPTO_MODE                   eMode,
                                           void**                   ppBuffer
                                           )

{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_SUCCESS;
    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;
    CRYPTO_BUFFER* pIntBuff;

#ifdef ADI_DEBUG
    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }

    if (ppBuffer == NULL) {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }

    if (dev->pfCallback) {
        return ADI_CRYPTO_ERR_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    /* Set ppBuffer as NULL initially */
    *ppBuffer = NULL;

    while (1) {
        ADI_ENTER_CRITICAL_REGION();
        
        /* This code parse through the completed buffer and try to find a 
           buffer of given mode 
        */
        CRYPTO_BUFFER* pIntBuffPrev = NULL;
        pIntBuff = dev->pCompletedBuffers;
        while (pIntBuff != NULL)
        {
            /* IF (Buffer of given mode is found) */
            if (pIntBuff->nMode == eMode) {
                /* Take out the buffer from the complted list and break */
                if (pIntBuffPrev == NULL) {
                    dev->pCompletedBuffers = pIntBuff->pNext;
                } else {
                    pIntBuffPrev->pNext = pIntBuff->pNext;
                }
                pIntBuff->pNext = NULL;
                break;
            }

            pIntBuffPrev = pIntBuff;
            pIntBuff = pIntBuff->pNext;
        }

        /* IF (Buffer of given mode is available) */
        if (pIntBuff != NULL) {
            /* Exit Critical Region and break the loop so that the given buffer can be processed */
            ADI_EXIT_CRITICAL_REGION();
            break;
        } else {
            /* This condition is only reached when there is no buffer of given mode in the completed buffer list */
            
            /* IF (Is Crypto Driver not processing buffer) */
            if ((dev->ActCompute.state & (uint32_t)CRYPTO_COMPUTE_STATE_PROCESSING) == 0u) {
                /* When the control reach here it mean
                   -> There is no buffer of given mode in the completed buffers 
                   -> The driver is not processing any buffers 
                  
                  Since there is no buffer of given mode in the completed buffers and since there is no buffers being processed,
                  that mean in the current situation, no buffer of given mode will be generated and there is no point in waiting
                  for buffer of given mode. This condition only occurs if the user did not submit any buffer of given mode
                  and yet issued a get of the given mode.
              
                  The code will return Buffer Not Available (ADI_CRYPTO_ERR_BUFFER_NOT_AVAIL) Error on the occurance of this condition 
                */
              
                ADI_EXIT_CRITICAL_REGION();
                return ADI_CRYPTO_ERR_BUFFER_NOT_AVAIL;
            }
        }
        ADI_EXIT_CRITICAL_REGION();
        
        PEND_EVENT(dev,ADI_CRYPTO_ERR_RTOS);
    }
    assert (pIntBuff != NULL);

    /* The result returned is same as the buffer status */
    result = pIntBuff->eStatus;

    /* Give back the buffer to the application */
    *ppBuffer = pIntBuff->pBuffer;

    /* Free the internal buffer */
    bpush_buffer(&dev->pFreeBuffers, pIntBuff);

    return result;
}

/* Submit the given buffer for processing to the device */
static ADI_CRYPTO_RESULT Crypto_SubmitBuffer (
                                              ADI_CRYPTO_HANDLE      hDevice,
                                              CRYPTO_MODE            eMode,
                                              void*                  pBuffer
                                              )
{
    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;
    CRYPTO_BUFFER* pIntBuff;
    ADI_CRYPTO_CONFIG* pConfig = NULL;


#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }

    if (pBuffer == NULL)
    {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }
#endif

    /* Pop a free internal buffer */
    pIntBuff = pop_buffer(&dev->pFreeBuffers);

    /* IF (No free internal buffers are available to hold the buffer */
    if (pIntBuff == NULL) {
        return ADI_CRYPTO_ERR_BUFFER_FULL;
    }

    /* Fill the Internal Crypto Buffer from the submitted buffer */
    FillCryptoBufferFromModeBuffer((uint32_t)eMode, pBuffer, pIntBuff, &pConfig);

#ifdef ADI_DEBUG
    /* Validate the Internal Crypto Buffer */
    if ((result = ValidateIntCryptoBuffer(pIntBuff)) != ADI_CRYPTO_SUCCESS) {
        /* Return the buffer back to Free Buffers due to failure */
        bpush_buffer(&dev->pFreeBuffers, pIntBuff);
        return result;
    }
#endif

    /* Push to the pending queue */
    bpush_buffer(&dev->pPendingBuffers, pIntBuff);

    /* Start buffer processing (the init buffer will return if not enabled) */
    InitBufferCompute(dev);

    return ADI_CRYPTO_SUCCESS;
}

/* Peek function to check completed buffer availability */
static ADI_CRYPTO_RESULT Crypto_IsBufferAvailable (
                                                   ADI_CRYPTO_HANDLE        hDevice,
                                                   CRYPTO_MODE                   eMode,
                                                   bool_t*                  pbAvailable
                                                   )
{
    CRYPTO_DEVICE* dev = (CRYPTO_DEVICE*)hDevice;
    CRYPTO_BUFFER* pIntBuffer;

#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(dev)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }

    if (pbAvailable == NULL)
    {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }
#endif /* ADI_DEBUG */

    /* Set the availability as false by default */
    *pbAvailable = false;

    ADI_ENTER_CRITICAL_REGION();
    pIntBuffer = dev->pCompletedBuffers;
    while (pIntBuffer != NULL) {
        if (pIntBuffer->nMode == eMode) {
            *pbAvailable = true;
            break;
        }
        pIntBuffer = pIntBuffer->pNext;
    }
    ADI_EXIT_CRITICAL_REGION();

    return ADI_CRYPTO_SUCCESS;
}

/* Stop the device */
static void StopDevice (CRYPTO_DEVICE* dev)
{
    /* Un-initialize the DMA */
    UnInitDMA(dev);

    /* Flush the buffers */
    FlushBuffers(dev);

    /* Disable the device */
    CLR_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_BLKEN);

    /* Move the buffer to completed list */
    if (dev->ActCompute.pIntBuff) {
        bpush_buffer(&dev->pCompletedBuffers, dev->ActCompute.pIntBuff);
    }
    dev->ActCompute.pIntBuff = NULL;

    /* Clear the compute state */
    dev->ActCompute.state = 0u;
}

/* Discard the buffers */
static void DiscardBuffers (CRYPTO_DEVICE* dev)
{
    /* This function should not be called with any active buffers */
    assert (dev->ActCompute.pIntBuff == NULL);

    /* Clear the pending buffers */
    while (dev->pPendingBuffers) {
        CRYPTO_BUFFER* pBuff = pop_buffer(&dev->pPendingBuffers);
        bpush_buffer(&dev->pFreeBuffers, pBuff);
    }

    /* Clear the completed buffers */
    while (dev->pCompletedBuffers) {
        CRYPTO_BUFFER* pBuff = pop_buffer(&dev->pCompletedBuffers);
        bpush_buffer(&dev->pFreeBuffers, pBuff);
    }
}

/* Flush the input and output buffers */
static void FlushBuffers(CRYPTO_DEVICE *dev)
{
    /* Set and clear the flush bits to flush the input and output buffers */
    SET_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_INFLUSH | BITM_CRYPT_CFG_OUTFLUSH);
    CLR_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_INFLUSH | BITM_CRYPT_CFG_OUTFLUSH);
}

/* Static process buffer completion */
static void ProcessBufferCompletion (CRYPTO_DEVICE* dev)
{
    ASSERT (dev->ActCompute.pIntBuff != NULL);

    /* IF (Callback function is registered) */
    if(dev->pfCallback)
    {
        uint32_t nEvent = 0u;
        CRYPTO_BUFFER* pIntBuff;

        /* Get the completed buffer */
        pIntBuff = dev->ActCompute.pIntBuff;
        dev->ActCompute.pIntBuff = NULL;

        if (pIntBuff->eStatus == ADI_CRYPTO_SUCCESS) {
            /* nMode value maps to Event value */
            nEvent = pIntBuff->nMode;

            /* Call the callback function */
            dev->pfCallback(
                            dev->pCBParam,
                            nEvent,
                            pIntBuff->pBuffer
                            );
        } else {
            /* Set the corresponding event for the error occurred */
            if (pIntBuff->eStatus == ADI_CRYPTO_ERR_DMA) {
                nEvent = ADI_CRYPTO_EVENT_ERR_DMA;
            } else {
                nEvent = ADI_CRYPTO_EVENT_ERR_FIFO;
            }

            /* Call the callback function */
            dev->pfCallback(
                            dev->pCBParam,
                            nEvent,
                            NULL
                            );

            /* Disable the device */
            adi_crypto_Enable(dev, false);
        }


        /* Return the free internal buffer to free pool */
        bpush_buffer(&dev->pFreeBuffers, pIntBuff);
    }
    else
    {
        /* Move the buffer to completed list */
        bpush_buffer(&dev->pCompletedBuffers, dev->ActCompute.pIntBuff);
        dev->ActCompute.pIntBuff = NULL;

        /* Post semaphore */
        POST_EVENT(dev);
    }

    /* Clear the compute state */
    dev->ActCompute.state    = 0u;

    /* Start a new computation if any */
    InitBufferCompute(dev);
}

/* Manage the FIFO in the interrupt region */
static void ManageFiFoInterrupt(CRYPTO_DEVICE* dev, uint32_t nDir)
{
    uint32_t nAllBuffCompState = (
                                    (uint32_t)CRYPTO_COMPUTE_STATE_IN_PREFIX_DONE
                                  | (uint32_t)CRYPTO_COMPUTE_STATE_IN_PAYLOAD_DONE
                                  | (uint32_t)CRYPTO_COMPUTE_STATE_OUT_PAYLOAD_DONE
                                  );
    if (dev->ActCompute.pIntBuff->nMode == CRYPTO_MODE_SHA) {
      nAllBuffCompState &= ~(uint32_t)CRYPTO_COMPUTE_STATE_OUT_PAYLOAD_DONE;
      nAllBuffCompState |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_SHA_LAST_WORD_DONE;
      nAllBuffCompState |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_SHA_READY;
    }

    /* Call the fifo manage function */
    dev->pfFifoManage(dev,nDir, CRYPTO_FIFO_SEQ_INTERRUPT);

    /* IF (All buffers are done) */
    if ((dev->ActCompute.state & nAllBuffCompState) == nAllBuffCompState)
    {

#if  ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1
        /* Copy the SHA is enabled */
        if (dev->ActCompute.pIntBuff->nMode == CRYPTO_MODE_SHA)
        {
            if (dev->ActCompute.pIntBuff->pOutputBuffer != NULL)
            {
#if ADI_CRYPTO_SHA_OUTPUT_FORMAT == 0 /* Little Endian */              
                dev->ActCompute.pIntBuff->pOutputBuffer[0] = dev->pReg->SHAH7;
                dev->ActCompute.pIntBuff->pOutputBuffer[1] = dev->pReg->SHAH6;
                dev->ActCompute.pIntBuff->pOutputBuffer[2] = dev->pReg->SHAH5;
                dev->ActCompute.pIntBuff->pOutputBuffer[3] = dev->pReg->SHAH4;
                dev->ActCompute.pIntBuff->pOutputBuffer[4] = dev->pReg->SHAH3;
                dev->ActCompute.pIntBuff->pOutputBuffer[5] = dev->pReg->SHAH2;
                dev->ActCompute.pIntBuff->pOutputBuffer[6] = dev->pReg->SHAH1;
                dev->ActCompute.pIntBuff->pOutputBuffer[7] = dev->pReg->SHAH0;
#else
                dev->ActCompute.pIntBuff->pOutputBuffer[0] = __ADI_BYTE_SWAP(dev->pReg->SHAH0);
                dev->ActCompute.pIntBuff->pOutputBuffer[1] = __ADI_BYTE_SWAP(dev->pReg->SHAH1);
                dev->ActCompute.pIntBuff->pOutputBuffer[2] = __ADI_BYTE_SWAP(dev->pReg->SHAH2);
                dev->ActCompute.pIntBuff->pOutputBuffer[3] = __ADI_BYTE_SWAP(dev->pReg->SHAH3);
                dev->ActCompute.pIntBuff->pOutputBuffer[4] = __ADI_BYTE_SWAP(dev->pReg->SHAH4);
                dev->ActCompute.pIntBuff->pOutputBuffer[5] = __ADI_BYTE_SWAP(dev->pReg->SHAH5);
                dev->ActCompute.pIntBuff->pOutputBuffer[6] = __ADI_BYTE_SWAP(dev->pReg->SHAH6);
                dev->ActCompute.pIntBuff->pOutputBuffer[7] = __ADI_BYTE_SWAP(dev->pReg->SHAH7);

#endif                
            }
        }
#endif
        /* Flush the buffers */
        FlushBuffers(dev);

        /* Disable the device */
        CLR_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_BLKEN);

        /* Process the completed buffer */
        ProcessBufferCompletion(dev);
    }
}

/* Un-initialize the DMA */
static void UnInitDMA (CRYPTO_DEVICE *dev)
{
#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    if (dev->bDmaEnabled)
    {
        if (dev->hDmaInpHandle != NULL)
        {
            adi_dma_Close(dev->hDmaInpHandle);
            dev->hDmaInpHandle = NULL;
        }

        if (dev->hDmaOutHandle != NULL)
        {
            adi_dma_Close(dev->hDmaOutHandle);
            dev->hDmaOutHandle = NULL;
        }
    }
#endif
}

/*! \private The crypto interrupt handler */
ADI_INT_HANDLER(Crypto_Int_Handler)
{
    ISR_PROLOG();   

    CRYPTO_DEVICE *dev = CryptoDevInfo[0].pDevice;
    
    if (IS_ANY_BIT_SET(dev->pReg->STAT, BITM_CRYPT_STAT_INOVR))
    {
        if (dev->ActCompute.pIntBuff != NULL)
        {
            /* Un-initialize the DMA */
            UnInitDMA(dev);

            /* Set the status as Fifo Error */
            dev->ActCompute.pIntBuff->eStatus = ADI_CRYPTO_ERR_FIFO;

            /* Process the completed buffer */
            ProcessBufferCompletion(dev);
        }
    }

    if (!dev->bDmaEnabled)
    {
        ManageFiFoInterrupt(dev, DIR_IN_OUT);
    }
    else {
        if (dev->ActCompute.pIntBuff->nMode == (uint32_t)CRYPTO_MODE_SHA)
        {
            if ((dev->pReg->STAT & BITM_CRYPT_STAT_SHADONE) != 0u)
            {
                ManageFiFoInterrupt(dev, DIR_INPUT);
            }
        }
    }
    
    ISR_EPILOG();   
}

static void DMAHandler (void *pcbParam, uint32_t Event, void *pArg, uint32_t nDir)
{
    CRYPTO_DEVICE *dev = pcbParam;

    if (dev->ActCompute.pIntBuff != NULL)
    {
        switch (Event)
        {
        case ADI_DMA_EVENT_BUFFER_PROCESSED:
            ManageFiFoInterrupt(dev, nDir);
            break;

        default:
            /* Un-initialize the DMA */
            UnInitDMA(dev);

            /* Set the status */
            dev->ActCompute.pIntBuff->eStatus = ADI_CRYPTO_ERR_DMA;

            /* Process the completed buffer */
            ProcessBufferCompletion(dev);
            break;
        }
    }
}

static void DMAInpHandler (void *pcbParam, uint32_t Event, void *pArg)
{
    DMAHandler(pcbParam, Event, pArg, DIR_INPUT);
}

static void DMAOutHandler (void *pcbParam, uint32_t Event, void *pArg)
{
    DMAHandler(pcbParam, Event, pArg, DIR_OUTPUT);
}

/* Checks for the input buffer and return whether it is done or not */
static bool_t IsInputBufferDone(CRYPTO_DEVICE *dev)
{
    CRYPTO_COMPUTE* pCompute = &dev->ActCompute;
    CRYPTO_BUFFER* pIntBuff = pCompute->pIntBuff;

    if (pCompute->nInpBitsRemaining > 0u) {
        return false;
    }

#if ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1
    /* IF (In CCM mode if authentication is not yet done) */
    if (   (pIntBuff->nMode == CRYPTO_MODE_CCM)
        && ((pCompute->state & (uint32_t)CRYPTO_COMPUTE_STATE_IN_PREFIX_DONE) == 0u))
    {
        /* Then the current input that finished is prefix */
        pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_PREFIX_DONE;

        /* So set payload as the next transfer set */
        if (pIntBuff->pDataBuffer)
        {
            pCompute->pInput = pIntBuff->pDataBuffer;
            pCompute->nInpBitsRemaining = pIntBuff->nDataBuffLenInBits;

            return false;
        }
    }
#endif

    return true;
}

/* Transfer Block Info the input FIFO */
static void TransferBlockToInputFIFO(CRYPTO_DEVICE *dev, bool_t bDma)
{
    CRYPTO_COMPUTE* pCompute = &dev->ActCompute;
    CRYPTO_BUFFER* pIntBuff = pCompute->pIntBuff;
    uint32_t nWordsRemainingInBlock;
    
    if ((pCompute->nInpBitsRemaining == 0u) && 
        (pIntBuff->nMode != CRYPTO_MODE_SHA)) 
    {
        return;
    }

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    if (pIntBuff->nMode == CRYPTO_MODE_SHA)
    {
        nWordsRemainingInBlock = (SHA_BLK_SIZE_IN_BITS / FIFO_WORD_SIZE_IN_BITS);
    }
    else
#endif /* (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1) */
    {
        nWordsRemainingInBlock = (BLK_SIZE_IN_BITS / FIFO_WORD_SIZE_IN_BITS);
    }

    ASSERT(!(
               ((pCompute->nInpBitsRemaining % BLK_SIZE_IN_BITS) != 0u)
            && (pIntBuff->nMode != CRYPTO_MODE_SHA)));

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)    
    if (pIntBuff->nMode == CRYPTO_MODE_SHA) 
    {
      if (bDma) {
          if (pCompute->nInpBitsRemaining >= FIFO_WORD_SIZE_IN_BITS) {
              dev->pReg->INBUF = *pCompute->pInput;
              pCompute->pInput++;
              pCompute->nInpBitsRemaining = pCompute->nInpBitsRemaining - FIFO_WORD_SIZE_IN_BITS;
              nWordsRemainingInBlock = 0u;
          }
      } else {
          while (
                      /*Note: This condition will suffice for non-SHA modes since it is multiple of FIFO_WORD_SIZE_IN_BITS */
                      (pCompute->nInpBitsRemaining >= FIFO_WORD_SIZE_IN_BITS)
                  &&  (nWordsRemainingInBlock > 0u))
          {
              dev->pReg->INBUF = *pCompute->pInput;
              pCompute->pInput++;
              pCompute->nInpBitsRemaining = pCompute->nInpBitsRemaining - FIFO_WORD_SIZE_IN_BITS;
              nWordsRemainingInBlock--;
          }
      }
    } 
    else 
#endif      
    {
        while (
                    /*Note: This condition will suffice for non-SHA modes since it is multiple of FIFO_WORD_SIZE_IN_BITS */
                    (pCompute->nInpBitsRemaining >= FIFO_WORD_SIZE_IN_BITS)
                &&  (nWordsRemainingInBlock > 0u))
        {
            dev->pReg->INBUF = *pCompute->pInput;
            pCompute->pInput++;
            pCompute->nInpBitsRemaining = pCompute->nInpBitsRemaining - FIFO_WORD_SIZE_IN_BITS;
            nWordsRemainingInBlock--;
        }
    }
    

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    /* This will only be Entered in SHA mode as for other modes all transfers will be Block Size */
    if (nWordsRemainingInBlock > 0u)
     {
         ASSERT (pIntBuff->nMode == CRYPTO_MODE_SHA);

         if ((pCompute->state & (uint32_t)CRYPTO_COMPUTE_STATE_IN_SHA_LAST_WORD_DONE) == 0u)
         {
             dev->pReg->SHA_LAST_WORD =  (pCompute->nInpBitsRemaining << BITP_CRYPT_SHA_LAST_WORD_O_BITS_VALID)
                                        | BITM_CRYPT_SHA_LAST_WORD_O_LAST_WORD;
             if (pCompute->nInpBitsRemaining == 0u) {
                 /* Dummy Write */
                 dev->pReg->INBUF = 0u;
             } else {
                 dev->pReg->INBUF = *pCompute->pInput;
                 pCompute->pInput++;
             }
             pCompute->nInpBitsRemaining = 0u;
             pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_SHA_LAST_WORD_DONE;
         } else {
             CLR_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_SHADONEN);
             pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_SHA_READY;
         }
    }
#endif /* (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1) */

    if (IsInputBufferDone(dev)) {
        /* Clear the input ready interrupt */
        CLR_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_INRDYEN);
        
        /* Clear the input ready status flag also */
        pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_PAYLOAD_DONE;
    }
}

/* Manage FIFO for PIO mode */
static ADI_CRYPTO_RESULT PioFIFOManage(CRYPTO_DEVICE *dev, uint32_t nDir, CRYPTO_FIFO_SEQ eFifoSeq)
{
    uint32_t nStatus;
    CRYPTO_COMPUTE* pCompute = &dev->ActCompute;
    CRYPTO_BUFFER* pIntBuff = pCompute->pIntBuff;

    if (eFifoSeq == CRYPTO_FIFO_SEQ_INTERRUPT) {
        /* Get the status register */
        nStatus = dev->pReg->STAT;

    #if CRYPTO_SUPPORT_MODE_ANY_NON_SHA
        /* IF (Output buffer have data) */
        if ((nStatus & BITM_CRYPT_STAT_OUTRDY) != 0u)
        {
            if (pCompute->nOutBitsRemaining > 0u)
            {
                /* in C89 the for loop counter declared outside the loop */
                uint32_t x;

                /*
                 * Since crypto works in blocks of 128bits, we can read
                 * 16bytes (128 bits) in one go
                 */
                ASSERT((pCompute->nOutBitsRemaining % BLK_SIZE_IN_BITS) == 0);

                /* FIFO writes happens in block of 32bit words */
                for (x = 0u; x < (BLK_SIZE_IN_BITS / 32u); x++)
                {
                    *pCompute->pOutput = dev->pReg->OUTBUF;
                    pCompute->pOutput++;
                }
                pCompute->nOutBitsRemaining = pCompute->nOutBitsRemaining - BLK_SIZE_IN_BITS;
            }

            if (pCompute->nOutBitsRemaining == 0u)
            {
                /* Disable the output ready interrupt */
                CLR_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_OUTRDYEN);
                pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_OUT_PAYLOAD_DONE;
            }
        }
    #endif

         if (pIntBuff->nMode == CRYPTO_MODE_SHA) {
            /* IF (Input buffer requires more data) */
            if ((nStatus & BITM_CRYPT_STAT_SHADONE) != 0u)
            {
                /* Clear SHA Done Bits */
                dev->pReg->STAT = BITM_CRYPT_STAT_SHADONE;
                
                /* Transfer SHA block to FIFO */
                TransferBlockToInputFIFO(dev, false);
            }
         } else {
            /* IF (Input buffer requires more data) */
            if ((nStatus & BITM_CRYPT_STAT_INRDY) != 0u)
            {
                if (pCompute->nInpBitsRemaining > 0u) {    
                    TransferBlockToInputFIFO(dev, false);
                }
            }
         }
    } else if (eFifoSeq == CRYPTO_FIFO_SEQ_BEFORE_BLK_EN) {
        return ADI_CRYPTO_SUCCESS;
    } else if (eFifoSeq == CRYPTO_FIFO_SEQ_AFTER_BLK_EN) {
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
        if (pIntBuff->nMode == CRYPTO_MODE_SHA) {
            TransferBlockToInputFIFO(dev, false);
        }
#endif /* (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1) */        
        return ADI_CRYPTO_SUCCESS;
    } else {
      /* Control should not have come here */
      assert(0);
    }

    return ADI_CRYPTO_SUCCESS;
}

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/* Manage the FIFO in DMA mode */
static ADI_CRYPTO_RESULT DmaFIFOManage(CRYPTO_DEVICE *dev, uint32_t nDir, CRYPTO_FIFO_SEQ   eFifoSeq)
{
    ADI_DMA_CHANNEL_HANDLE* phDMA;
    ADI_DMA_TRANSFER DmaTransfer;

    /* Get pointer to the buffer in process */
    CRYPTO_COMPUTE* pCompute = &dev->ActCompute;
    CRYPTO_BUFFER* pIntBuff = pCompute->pIntBuff;

    /* Get the DMA Handle based on the direction */
    if (nDir == DIR_INPUT) {
        phDMA = &dev->hDmaInpHandle;
    } else {
        phDMA = &dev->hDmaOutHandle;
    }

    /* IF (The function is called at the start of a buffer transaction */
    if (eFifoSeq == CRYPTO_FIFO_SEQ_BEFORE_BLK_EN)
    {
        /* IF (DMA is not yet initialized) */
        if (*phDMA == NULL)
        {
            uint8_t* pDmaMemory;
            ADI_DMA_CHANNEL_ID eDmaChannelID;
            ADI_CALLBACK pfCallback;

            if (nDir == DIR_INPUT)
            {
                pDmaMemory = dev->DmaMemoryIn;
                eDmaChannelID = ADI_DMA_CHANNEL_AES0_IN;
                pfCallback = DMAInpHandler;
            }
            else
            {
                pDmaMemory = dev->DmaMemoryOut;
                eDmaChannelID = ADI_DMA_CHANNEL_AES0_OUT;
                pfCallback = DMAOutHandler;
            }

            /* Open the DMA */
            if (adi_dma_Open (eDmaChannelID, pDmaMemory, phDMA, pfCallback, dev) != ADI_DMA_SUCCESS)
            {
                return ADI_CRYPTO_ERR_DMA;
            }

            if (adi_dma_SetArbitrationRate(*phDMA, ADI_DMA_RPOWER_4) != ADI_DMA_SUCCESS)
            {
                return ADI_CRYPTO_ERR_DMA;
            }
            
            /* Enable the DMA */
            if (adi_dma_Enable (*phDMA, true) != ADI_DMA_SUCCESS)
            {
                return ADI_CRYPTO_ERR_DMA;
            }
        }
        
        while (1)
        {
            /* Common DMA Transfer configurations */
            DmaTransfer.DataWidth = ADI_DMA_WIDTH_4_BYTE;

            if (nDir == DIR_INPUT)
            {
                /* Configure the DMA structure */
                DmaTransfer.pSrcData = pCompute->pInput;
                DmaTransfer.pDstData = &dev->pReg->INBUF;
                DmaTransfer.SrcInc = ADI_DMA_INCR_4_BYTE;
                DmaTransfer.DstInc = ADI_DMA_INCR_NONE;
                
                if (pIntBuff->nMode == CRYPTO_MODE_SHA) {
                    if ((pCompute->nInpBitsRemaining % SHA_BLK_SIZE_IN_BITS) < FIFO_WORD_SIZE_IN_BITS) {
                        if (pCompute->nInpBitsRemaining < FIFO_WORD_SIZE_IN_BITS) {
                            DmaTransfer.NumTransfers = 0u;
                            CLR_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_INDMAEN);
                        } else {
                            DmaTransfer.NumTransfers = pCompute->nInpBitsRemaining / FIFO_WORD_SIZE_IN_BITS - 1u;
                            pCompute->nInpBitsRemaining = pCompute->nInpBitsRemaining - DmaTransfer.NumTransfers * FIFO_WORD_SIZE_IN_BITS;
                        }
                    } else {
                        DmaTransfer.NumTransfers = pCompute->nInpBitsRemaining / FIFO_WORD_SIZE_IN_BITS;
                        pCompute->nInpBitsRemaining = pCompute->nInpBitsRemaining - DmaTransfer.NumTransfers * FIFO_WORD_SIZE_IN_BITS;
                    }
                } else {
                    DmaTransfer.NumTransfers = pCompute->nInpBitsRemaining / FIFO_WORD_SIZE_IN_BITS;
                    pCompute->nInpBitsRemaining = 0u;
                }
            }
            else
            {
                /* Configure the DMA structure */
                DmaTransfer.pSrcData = &dev->pReg->OUTBUF;
                DmaTransfer.pDstData = pCompute->pOutput;
                DmaTransfer.NumTransfers = pCompute->nOutBitsRemaining / FIFO_WORD_SIZE_IN_BITS;
                DmaTransfer.SrcInc = ADI_DMA_INCR_NONE;
                DmaTransfer.DstInc = ADI_DMA_INCR_4_BYTE;

                pCompute->nOutBitsRemaining = 0u;
            }


            if (DmaTransfer.NumTransfers == 0u) {
                break;
            }

            /* Submit the buffer to the device */
            if (adi_dma_SubmitTransfer(*phDMA, &DmaTransfer) != ADI_DMA_SUCCESS) {
                return ADI_CRYPTO_ERR_DMA;
            }

#ifdef __ICCARM__
/*
* Pm144 (rule 8.5): for any iteration statement there shall be at most one 'break'
*   Using multiple breaks here is more comprehensible than a single complex condition.
*/
#pragma diag_suppress=Pm144
#endif /* __ICCARM__ */

            if (nDir == DIR_INPUT) {
                pCompute->pInput += DmaTransfer.NumTransfers;
                pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_DMA_SCHEDULED;
                if (pIntBuff->nMode == CRYPTO_MODE_SHA) { 
                    break;
                } else {
                    if (IsInputBufferDone(dev)) 
                    {
                        break;
                    }
                }
            } else {
                pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_OUT_DMA_SCHEDULED;
            }

            if (nDir == DIR_OUTPUT) {
                break;
            }
#ifdef __ICCARM__
#pragma diag_default=Pm144
#endif /* __ICCARM__ */
        }
    } else if(eFifoSeq == CRYPTO_FIFO_SEQ_AFTER_BLK_EN) {
        if (nDir == DIR_INPUT) {
            if ((pCompute->state & (uint32_t)CRYPTO_COMPUTE_STATE_IN_DMA_SCHEDULED) != 0u) 
            {
                SET_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_INDMAEN);
            }
            else 
            {
                ASSERT (pIntBuff->nMode == CRYPTO_MODE_SHA);
                SET_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_SHADONEN);
                TransferBlockToInputFIFO(dev, true);
            }
        } else {
            if ((pCompute->state & (uint32_t)CRYPTO_COMPUTE_STATE_IN_DMA_SCHEDULED) != 0u) 
            {
                ASSERT (pIntBuff->nMode != CRYPTO_MODE_SHA);
                SET_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_OUTDMAEN);
            }
        }
    } else {
        bool_t bTransferInProgress;

        if (adi_dma_IsTransferInProgress(*phDMA, &bTransferInProgress) != ADI_DMA_SUCCESS)
        {
            return ADI_CRYPTO_ERR_DMA;
        }

        if (!bTransferInProgress)
        {
            if (nDir == DIR_INPUT)
            {
                /* Disables the input DMA */
                CLR_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_INDMAEN);
                
                if (pCompute->pIntBuff->nMode == CRYPTO_MODE_SHA) 
                {
                    /* Enable the SHA Done Interrupt */
                    SET_BITS(dev->pReg->INTEN, BITM_CRYPT_INTEN_SHADONEN);
                    TransferBlockToInputFIFO(dev, true);
                } 
                else 
                {
                    /* Set the state as payload input done */
                    pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_IN_PAYLOAD_DONE;
                }                    
            }
            else
            {
                /* Disables the output DMA */
                CLR_BITS(dev->pReg->CFG, BITM_CRYPT_CFG_OUTDMAEN);

                /* Set the output payload state as done */
                pCompute->state |= (uint32_t)CRYPTO_COMPUTE_STATE_OUT_PAYLOAD_DONE;
            }
        }
    }

    return ADI_CRYPTO_SUCCESS;
}
#endif

/* Pop a buffer from the buffer list */
static CRYPTO_BUFFER* pop_buffer(CRYPTO_BUFFER** ppBuffList)
{
    assert (ppBuffList != NULL);

    ADI_ENTER_CRITICAL_REGION();
    /* Get the buffer list */
    CRYPTO_BUFFER* pBuff =  *ppBuffList;
    if (pBuff)
    {
        /* Pop a buffer from the start of the buffer list */
        *ppBuffList = pBuff->pNext;
        pBuff->pNext = NULL;
    }
    ADI_EXIT_CRITICAL_REGION();

    return pBuff;
}

/* Push a buffer to the back of the buffer list */
static void bpush_buffer(CRYPTO_BUFFER** ppBuffList, CRYPTO_BUFFER* pBuffer)
{
    assert ((ppBuffList != NULL) && (pBuffer != NULL));
    assert (pBuffer->pNext == NULL);

    ADI_ENTER_CRITICAL_REGION();
    /* Get the buffer list */
    CRYPTO_BUFFER* pBuffList =  *ppBuffList;

    if (pBuffList)
    {
        while (pBuffList->pNext != NULL) {
            pBuffList = pBuffList->pNext;
        }
        pBuffList->pNext = pBuffer;
    }
    else
    {
        *ppBuffList = pBuffer;
    }
    ADI_EXIT_CRITICAL_REGION();
}

static void FillCryptoBufferFromModeBuffer(
                                           uint32_t                 nMode,
                                           void*                    pExtBuffer,
                                           CRYPTO_BUFFER*           pIntBuffer,
                                           ADI_CRYPTO_CONFIG**      ppConfig
                                           )
{
    ADI_CRYPTO_CONFIG* pConfig = NULL;

    memset(pIntBuffer, 0, sizeof(CRYPTO_BUFFER));

    pIntBuffer->pBuffer = pExtBuffer;
    pIntBuffer->nMode = nMode;

    switch(nMode)
    {
#if ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1
    case CRYPTO_MODE_ECB:
        {
            ADI_CRYPTO_ECB_BUFFER  *pModeBuffer   = (ADI_CRYPTO_ECB_BUFFER*)pExtBuffer;
            pConfig = pModeBuffer->pConfig;
            pIntBuffer->eEncMode = pModeBuffer->eEncMode;            
            pIntBuffer->pDataBuffer = pModeBuffer->pDataBuffer;
            pIntBuffer->nDataBuffLenInBits = pModeBuffer->nDataBuffLen * 8u;
            pIntBuffer->pOutputBuffer = pModeBuffer->pOutputBuffer;
            pIntBuffer->nOutputBuffLenInBits = pModeBuffer->nOutputBuffLen * 8u;
        }
        break;
#endif
#if ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1
    case CRYPTO_MODE_CBC:
        {
            ADI_CRYPTO_CBC_BUFFER  *pModeBuffer   = (ADI_CRYPTO_CBC_BUFFER*)pExtBuffer;
            pConfig = pModeBuffer->pConfig;
            pIntBuffer->eEncMode = pModeBuffer->eEncMode;            
            pIntBuffer->pDataBuffer = pModeBuffer->pDataBuffer;
            pIntBuffer->nDataBuffLenInBits = pModeBuffer->nDataBuffLen * 8u;
            pIntBuffer->pOutputBuffer = pModeBuffer->pOutputBuffer;
            pIntBuffer->nOutputBuffLenInBits = pModeBuffer->nOutputBuffLen * 8u;
            pIntBuffer->pNonceIV = &pModeBuffer->InitVector[0];
        }
        break;
#endif
#if ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1
    case CRYPTO_MODE_CTR:
        {
            ADI_CRYPTO_CTR_BUFFER  *pModeBuffer   = (ADI_CRYPTO_CTR_BUFFER*)pExtBuffer;
            pConfig = pModeBuffer->pConfig;
            pIntBuffer->eEncMode = pModeBuffer->eEncMode;            
            pIntBuffer->pDataBuffer = pModeBuffer->pDataBuffer;
            pIntBuffer->nDataBuffLenInBits = pModeBuffer->nDataBuffLen * 8u;
            pIntBuffer->pOutputBuffer = pModeBuffer->pOutputBuffer;
            pIntBuffer->nOutputBuffLenInBits = pModeBuffer->nOutputBuffLen * 8u;
            pIntBuffer->pNonceIV = &pModeBuffer->NONCE[0];
            pIntBuffer->nCounterInit = pModeBuffer->nCounterInit;
        }
        break;
#endif
#if ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1
    case CRYPTO_MODE_CCM:
        {
            ADI_CRYPTO_CCM_BUFFER  *pModeBuffer   = (ADI_CRYPTO_CCM_BUFFER*)pExtBuffer;
            pConfig = pModeBuffer->pConfig;
            pIntBuffer->eEncMode = pModeBuffer->eEncMode;            
            pIntBuffer->pDataBuffer = pModeBuffer->pDataBuffer;
            pIntBuffer->nDataBuffLenInBits = pModeBuffer->nDataBuffLen * 8u;
            pIntBuffer->pOutputBuffer = pModeBuffer->pOutputBuffer;
            pIntBuffer->nOutputBuffLenInBits = pModeBuffer->nOutputBuffLen * 8u;
            pIntBuffer->pPrefixBuffer = pModeBuffer->pPrefixBuffer;
            if (pModeBuffer->pPrefixBuffer == NULL) {
                pIntBuffer->nPrefixBuffLenInBits = 0u;
            } else {
                pIntBuffer->nPrefixBuffLenInBits = pModeBuffer->nPrefixBuffLen * 8u;
            }
            pIntBuffer->nValidBytesInLastBlock = pModeBuffer->nValidBytesInLastBlock;
            pIntBuffer->pNonceIV = &pModeBuffer->NONCE[0];
            pIntBuffer->nCounterInit = pModeBuffer->nCounterInit;

        }
        break;
#endif
#if ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1
    case CRYPTO_MODE_CMAC:
        {
            ADI_CRYPTO_CMAC_BUFFER  *pModeBuffer   = (ADI_CRYPTO_CMAC_BUFFER*)pExtBuffer;
            pConfig = pModeBuffer->pConfig;
            pIntBuffer->pDataBuffer = pModeBuffer->pDataBuffer;
            pIntBuffer->nDataBuffLenInBits = pModeBuffer->nDataBuffLen * 8u;
            pIntBuffer->pOutputBuffer = pModeBuffer->pMACBuffer;
            pIntBuffer->nOutputBuffLenInBits = pModeBuffer->nMACBuffLen * 8u;
        }
        break;
#endif
#if ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1
    case CRYPTO_MODE_SHA:
        {
            ADI_CRYPTO_SHA_BUFFER  *pModeBuffer   = (ADI_CRYPTO_SHA_BUFFER*)pExtBuffer;
            pIntBuffer->eBuffDataFormat =  ENUM_CRYPT_CFG_LITTLE_ENDIAN;
            pIntBuffer->pDataBuffer = pModeBuffer->pDataBuffer;
            pIntBuffer->nDataBuffLenInBits = pModeBuffer->nDataBuffLenInBits;
            pIntBuffer->pOutputBuffer = pModeBuffer->pSHABuffer;
            pIntBuffer->nOutputBuffLenInBits = pModeBuffer->nSHABuffLen * 8u;
        }
        break;
#endif
    default:
        assert(0);
        break;
    }

    if (pConfig != NULL) {
        pIntBuffer->eBuffDataFormat = pConfig->eBuffDataFormat;
        pIntBuffer->eKeyLen = (uint32_t)pConfig->eKeyLen;
        pIntBuffer->pKey = pConfig->pKey;
    } else {
        pIntBuffer->pKey = NULL;
    }
}

#ifdef ADI_DEBUG
static ADI_CRYPTO_RESULT ValidateIntCryptoBuffer(CRYPTO_BUFFER* pIntBuff)
{
    if (
           (pIntBuff->pDataBuffer == NULL)
        || (pIntBuff->nDataBuffLenInBits == 0u)
        || (pIntBuff->pOutputBuffer == NULL)
        || (pIntBuff->nOutputBuffLenInBits == 0u)
        || (
               (pIntBuff->eBuffDataFormat != ADI_CRYPTO_DATA_FMT_LITTLE_ENDIAN)
            && (pIntBuff->eBuffDataFormat != ADI_CRYPTO_DATA_FMT_BIG_ENDIAN))
            )
    {
        return ADI_CRYPTO_ERR_BAD_BUFFER;
    }

#if ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1
    if (pIntBuff->nMode == CRYPTO_MODE_SHA)
    {
        /* SHA Support is 256bit and hence the output buffer should be atleast of size 256 */
        if (pIntBuff->nOutputBuffLenInBits < 256u) {
            return ADI_CRYPTO_ERR_BAD_BUFFER;
        }
    }
    else
#endif
    {
        if (
               ((pIntBuff->nDataBuffLenInBits & (BLK_SIZE_IN_BITS-1u)) != 0u)
            || ((pIntBuff->nOutputBuffLenInBits & (BLK_SIZE_IN_BITS-1u)) != 0u))
        {
            return ADI_CRYPTO_ERR_BAD_BUFFER;
        }

#if ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1
        if (pIntBuff->nMode == CRYPTO_MODE_CMAC) {
            if (pIntBuff->nOutputBuffLenInBits < BLK_SIZE_IN_BITS) {
                return ADI_CRYPTO_ERR_BAD_BUFFER;
            }
        }
        else
#endif
        {
            if (
                   (pIntBuff->pKey == NULL)
                || (   (pIntBuff->eKeyLen != (uint32_t)ADI_CRYPTO_KEY_LEN_128_BIT)
                    && (pIntBuff->eKeyLen != (uint32_t)ADI_CRYPTO_KEY_LEN_256_BIT))
                || (   (pIntBuff->eEncMode != ADI_CRYPTO_ENC_MODE_ENCRYPT)
                    && (pIntBuff->eEncMode != ADI_CRYPTO_ENC_MODE_DECRYPT)))
            {
                return ADI_CRYPTO_ERR_BAD_CONFIG;
            }

#if ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1
            if (pIntBuff->nMode == CRYPTO_MODE_CTR)
            {
                if ((pIntBuff->nCounterInit & (0xFFF00000u)) != 0u) {
                    return ADI_CRYPTO_ERR_BAD_BUFFER;
                }
            }
#endif

#if ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1
            if (pIntBuff->nMode == CRYPTO_MODE_CCM)
            {
                if (
                       ((pIntBuff->nCounterInit & (0xFFFF0000u)) != 0u)
                    || (   (pIntBuff->pPrefixBuffer != NULL)
                        && (   (pIntBuff->nPrefixBuffLenInBits == 0u)
                            || ((pIntBuff->nPrefixBuffLenInBits & (BLK_SIZE_IN_BITS - 1u)) != 0u))
                        )
                    || (pIntBuff->nValidBytesInLastBlock == 0u)
                    || (pIntBuff->nValidBytesInLastBlock > BLK_SIZE_IN_BYTES)
                    || (pIntBuff->nOutputBuffLenInBits < (pIntBuff->nDataBuffLenInBits + BLK_SIZE_IN_BITS)))
                {
                    return ADI_CRYPTO_ERR_BAD_BUFFER;
                }
            }
            else
#endif
            {
                if (pIntBuff->nOutputBuffLenInBits < pIntBuff->nDataBuffLenInBits)
                {
                    return ADI_CRYPTO_ERR_BAD_BUFFER;
                }
            }
        }
    }

    return ADI_CRYPTO_SUCCESS;
}


/* Validate the given handle */
static ADI_CRYPTO_RESULT ValidateHandle(CRYPTO_DEVICE* dev)
{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_ERR_BAD_DEV_HANDLE;
    uint32_t x;

    for (x = 0u; x < NUM_DEVICES; x++) {
        if (CryptoDevInfo[x].pDevice == dev) {
            result = ADI_CRYPTO_SUCCESS;
            break;
        }
    }

    return result;
}
#endif

#endif /* _ADI_CRYPTO_V1_C_ */

/*@}*/
