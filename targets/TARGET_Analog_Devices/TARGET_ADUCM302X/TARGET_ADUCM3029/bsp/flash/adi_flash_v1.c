/*!
 *****************************************************************************
 @file:    adi_flash.c
 @brief:   Flash Driver Implementations for ADuCM302x
 @version: $Revision: 34991 $
 @date:    $Date: 2016-06-30 13:06:37 +0100 (Thu, 30 Jun 2016) $
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

#ifndef _ADI_FLASH_V1_C_
#define _ADI_FLASH_V1_C_

/** @addtogroup Flash_Driver Flash Controller Device Driver
 *  @{
 */

/*========  I N C L U D E  ========*/

#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <services/int/adi_int.h>

#include <ssdd_common/common_def.h>
#include <drivers/flash/adi_flash.h>
#include "adi_flash_def_v1.h"

/*=============  M I S R A   =============*/

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
*   Relying on pointer arithmetic for buffer handling.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   Required for MMR accesses, determining pointer alignment, and a callback argument.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm088,Pm140
#endif /* __ICCARM__ */

/*========  D E F I N E S   ========*/

/*! \cond PRIVATE */

/* Utility Macros */
#define CLR_BITS(REG,BITS)         ((REG) &= ~(BITS))
#define SET_BITS(REG,BITS)         ((REG) |= (BITS))
#define IS_ANY_BIT_SET(REG,BITS)   (((REG) & (BITS)) != 0u)


/* Number of crypto device for the given processor */
#define NUM_DEVICES             (1u)


/* Flash Size and Page/Block shifts */
#define FEE_PAGE_SHIFT          (11u)
#define FEE_BLOCK_SHIFT         (13u)
#define FEE_FLASH_SIZE          (0x40000u)
#define FEE_PAGE_MAX            (FEE_FLASH_SIZE >> FEE_PAGE_SHIFT)

/* Debug Macros */
#ifdef ADI_DEBUG
#define ASSERT(X)   assert(X)
#else
#define ASSERT(X)
#endif

#if (ADI_FEE_CFG_ECC_START_PAGE >= FEE_PAGE_MAX)
#error "ADI_FEE_CFG_ECC_START_PAGE range is invalid"
#endif

/*========  L O C A L    F U N C    D E C L  ========*/

/* Initialize the device */
static void Initialize (
                        uint32_t        const nDeviceNum,
                        FLASH_DEVICE*   const drv
                        );

#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 1)
/* wait for the given interrupt to occur */
static uint32_t waitForInterrupt (
                                  FLASH_DEVICE* const drv,
                                  uint32_t      const StatusFlag
                                  );
#endif

#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 0)
/* Wait for the given status */
static uint32_t WaitForStatus (
                               FLASH_DEVICE* const drv,
                               uint32_t      const nStatusFlag
                               );
#endif

/* Send a command to the flash controller and
 * wait for the command to occur
 */
static ADI_FEE_RESULT DoCmd (
                             FLASH_DEVICE* const drv,
                             uint32_t      const cmd
                             );

/* Write data from the active buffer to the flash controller */
static void WriteData(FLASH_DEVICE* const drv);

/* PIO Transfer to FIFO */
static ADI_FEE_RESULT PIO_Transfer (
                          FLASH_DEVICE* const drv,
                          bool_t        const bStart,
                          bool_t        const bInterruptRegion
                          );

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
/* DMA Transfer to FIFO */
static ADI_FEE_RESULT DMA_Transfer (
                          FLASH_DEVICE* const drv,
                          bool_t        const bStart,
                          bool_t        const bInterruptRegion
                          );
#endif


/* Initiate a Transfer */
static void InitiateTransfer (FLASH_DEVICE* const drv);

/* Process Completed Buffers */
static void ProcessCompletedBuffers(FLASH_DEVICE* drv);

/* Manage FIFO in the interrupt region */
static void IntManageFIFO (FLASH_DEVICE* drv);

/**** Bufffer Utility Functiion ****/
/* Pop a buffer from the buffer list */
static FLASH_BUFFER* pop_buffer (FLASH_BUFFER**  pBufferList);

/* Push a buffer to the buffer list */
static void bpush_buffer (
                          FLASH_BUFFER** pBufferList,
                          FLASH_BUFFER*  pBuffer
                          );

#ifdef ADI_DEBUG
/* Validate Device Handle */
static ADI_FEE_RESULT ValidateHandle (FLASH_DEVICE* const drv);

/* Check whether the given address is write protected */
static bool_t isWriteProtected (
                                FLASH_DEVICE* const drv,
                                uint32_t      const nRelAddress
                                );
#endif

/* forward declarations... */
/* These are needed in the case where ADI_CFG_ENABLE_RTOS_SUPPORT=1              */
/* The ADI_INSTALL_HANDLER macro will expand to a call to adi_int_InstallHandler */
/* and the handler will be referenced                                            */
/* The non-OSAL expansion of ADI_INSTALL_HANDLER results in a NULL string        */
ADI_INT_HANDLER (Flash0_Int_Handler);

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
/* DMA Handler */
static void DMAHandler (void *pcbParam, uint32_t nEvent, void *pArg);
#endif

/* Wait for the FLASH to be free */
static void waitWhileBusy (FLASH_DEVICE* drv, uint32_t busyflags);

/*========  I N L I N E    F U N C T I O N S  ========*/

static void waitWhileBusy (FLASH_DEVICE* drv, uint32_t busyflags)
{
    while ((drv->pReg->STAT & busyflags) != 0u) {}
}
/*! \endcond */

/*========  D A T A  ========*/
/* Internal device structure */
static FLASH_INFO FlashDevInfo[] = {
    {pADI_FLCC0,  NULL}
};

/*========  C O D E  ========*/
/*
 * API Implementation
 */

/* Open the flash controller */
/**
 * @brief    Open the flash controller
 *
 * @param [in]  nDeviceNum    The Device instance number of flash controller to be opened
 * @param [in]  pMemory       Application supplied memory space for use by the driver
 * @param [in]  nMemorySize   Size of the application supplied memory (in bytes)
 * @param [in]  phDevice      A pointer to a location where the handle to the opened
 *                            device is written
 *
 * @return  Status
 *          - #ADI_FEE_SUCCESS                      The device is opened successfully
 *          - #ADI_FEE_ERR_BAD_DEVICE_NUM       [D] The device number passed is invalid
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Some pointer(s) passed to the function is NULL
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_ALREADY_INITIALIZED  [D] The device is already initialized and hence
 *                                                  cannot be opened
 *          - #ADI_FEE_ERR_INSUFFICIENT_MEM     [D] The memory passed to the driver is insufficient.
 */
ADI_FEE_RESULT adi_fee_Open (
                             uint32_t         const nDeviceNum,
                             void*            const pMemory,
                             uint32_t         const nMemorySize,
                             ADI_FEE_HANDLE*  const phDevice
                             )
{
    FLASH_DEVICE *drv;

#ifdef ADI_DEBUG
    if (nDeviceNum >= NUM_DEVICES) {
        return ADI_FEE_ERR_BAD_DEVICE_NUM;
    }

    if ((pMemory == NULL) || (phDevice == NULL)) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }

    if (nMemorySize < ADI_FEE_MEMORY_SIZE) {
        return ADI_FEE_ERR_INSUFFICIENT_MEM;
    }

    assert (ADI_FEE_MEMORY_SIZE == sizeof(FLASH_DEVICE));

    if (FlashDevInfo[nDeviceNum].pDevice != NULL) {
        return ADI_FEE_ERR_ALREADY_INITIALIZED;
    }
#endif

    /* Get the device */
    drv = (FLASH_DEVICE*)pMemory;

    /* Initialize the device structure */
    Initialize (nDeviceNum, drv);

#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* create a semaphore for buffer management */
    CREATE_SEM(drv,ADI_FEE_ERR_RTOS);
#endif

    /* Install the interrupt handlers */
    ADI_INSTALL_HANDLER (FLCC_EVT_IRQn, Flash0_Int_Handler);

    /* Enable Interrupts */
#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 1)
    ADI_ENABLE_INT (FLCC_EVT_IRQn);
#endif

    /* Store the device memory in the internal structure */
    FlashDevInfo[nDeviceNum].pDevice = drv;

    /* Give the handle back to the application */
    *phDevice = drv;

    /* return success */
    return ADI_FEE_SUCCESS;
}

/* Close the frash controller */
/**
 * @brief    Close the flash controller
 *
 * @param [out] hDevice   The handle to the flash controller device
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The device is closed successfully
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_Close (ADI_FEE_HANDLE hDevice)
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    uint32_t x;

#ifdef ADI_DEBUG

    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }
#endif

#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* Destroy the semaphore */
    DELETE_SEM(drv,ADI_FEE_ERR_RTOS);
#endif

    /* Remove the device handle */
    for (x = 0u; x < NUM_DEVICES; x++)
    {
        if (FlashDevInfo[x].pDevice == drv)
        {
            FlashDevInfo[x].pDevice = NULL;
            break;
        }
    }

    return eResult;
}

/**
 * @brief   Register an application defined callback function
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [in]  pfCallback  Application supplied calllback function which is called
 *                          to notify device related events. A value of NULL disables the
 *                          callback
 * @param [in]  pCBParam    Application supplied callback parameter which will be passed back
 *                          in the callback function.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The callback is registered succcessfully
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_RegisterCallback (
        ADI_FEE_HANDLE   const hDevice,
        ADI_CALLBACK     const pfCallback,
        void*            const pCBParam
        )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Set the callback function and param in the device */
    drv->pfCallback = pfCallback;
    drv->pCBParam = pCBParam;

    return eResult;
}



/**
 * @brief     Write to the flash
 *
 * This function block (does not return) until the write operation is completed
 *
 * @param [in]  hDevice         The handle to the flash controller device
 * @param [in]  nStartAddress   The start address in the flash to which the content of the
 *                              buffer is written. (Should be 64-bit aligned)
 * @param [in]  pBufferData     Pointer to the buffer (should be 32-bit aligned)
 * @param [in]  nBufferSize     Size of the buffer (Should be a multiple of 8 bytes)
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The buffer is successfully written to the flash
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_ALIGNMENT            [D] Alignment of the buffer/start_address is invalid
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Buffer size is not a multiple of 8-bytes
 *          - #ADI_FEE_ERR_INVALID_MODE         [D] Another background write is happening
 *          - #ADI_FEE_ERR_BUFFER_ERR           [D] Error occurred in processing the buffer
 */
ADI_FEE_RESULT adi_fee_Write (
                              ADI_FEE_HANDLE    const  hDevice,
                              uint32_t          const  nStartAddress,
                              void*             const  pBufferData,
                              uint32_t          const  nBufferSize
                              )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    FLASH_BUFFER* pBuffer;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (   ((nStartAddress & 0x7u) != 0u)
        || ((((uint32_t)pBufferData) & 0x3u) != 0u))
    {
        return ADI_FEE_ERR_ALIGNMENT;
    }

    if ((nBufferSize & 0x7u) != 0u) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Get a free buffer */
    pBuffer = pop_buffer(&drv->pFreeBuffers);
    ASSERT(pBuffer != NULL);

    /* Fill in the buffer structures */
    pBuffer->pBuffer = pBufferData;
    pBuffer->pCurBuffPos = pBufferData;
    pBuffer->nStartAddress = nStartAddress;
    pBuffer->nTotalBytes = nBufferSize;
    pBuffer->nRemainingBytes = nBufferSize;

    /* Move the buffer to the pending buffers */
    bpush_buffer (&drv->pPendingBuffers, pBuffer);

    /* Initiate a transfer */
    InitiateTransfer (drv);

    /* Wait for the completed buffer */
    while (1)
    {
#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 1)
        PEND_EVENT(drv,ADI_FEE_ERR_RTOS);
#endif
        if (drv->pCompletedBuffers != NULL) {
            break;
        }
    }

    /* Debug assert */
    ASSERT (pBuffer == drv->pCompletedBuffers);

    /* Pop the buffer from the completed queue */
    pBuffer = pop_buffer(&drv->pCompletedBuffers);

    /* Check for errors in buffer write */
    if (pBuffer->nRemainingBytes != 0u) {
        eResult = ADI_FEE_ERR_BUFFER_ERR;
    }

    /* Return the buffer back to the free list */
    bpush_buffer(&drv->pFreeBuffers, pBuffer);

    return eResult;
}

/**
 * @brief     Write to the flash (non-blocking)
 *
 * This function copy the given block of buffer to the flash. This function is non-blocking function
 * and return immediately. The buffer have to obtained back by calling the adi_fee_GetTxBuffer()
 * function
 *
 * @param [in]  hDevice         The handle to the flash controller device
 * @param [in]  nStartAddress   The start address in the flash to which the content of the
 *                              buffer is written. (Should be 64-bit aligned)
 * @param [in]  pBufferData     Pointer to the buffer (Should be 32-bit aligned)
 * @param [in]  nBufferSize     Size of the buffer (Should be a multiple of 8 bytes)
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The buffer is successfully submitted for transfer.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_ALIGNMENT            [D] Alignment of the buffer/start_address is invalid
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Buffer size is not a multiple of 8-bytes
 *          - #ADI_FEE_ERR_INVALID_MODE         [D] Another blocking write is happening
 *          - #ADI_FEE_ERR_NO_FREE_BUFFERS      [D] No free internal buffers to hold the buffer
 */
ADI_FEE_RESULT adi_fee_SubmitTxBuffer (
                                       ADI_FEE_HANDLE    const  hDevice,
                                       uint32_t          const  nStartAddress,
                                       void*             const  pBufferData,
                                       uint32_t          const  nBufferSize
                                       )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    FLASH_BUFFER* pBuffer;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (   ((nStartAddress & 0x7u) != 0u)
        || ((((uint32_t)pBufferData) & 0x3u) != 0u))
    {
        return ADI_FEE_ERR_ALIGNMENT;
    }

    if ((nBufferSize & 0x7u) != 0u) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif


    /* Get a free buffer */
    pBuffer = pop_buffer(&drv->pFreeBuffers);

    if (pBuffer)
    {
        /* Fill in the buffer structures */
        pBuffer->pBuffer = pBufferData;
        pBuffer->pCurBuffPos = pBufferData;
        pBuffer->nStartAddress = nStartAddress;
        pBuffer->nTotalBytes = nBufferSize;
        pBuffer->nRemainingBytes = nBufferSize;

        /* Move the buffer to the pending buffers */
        bpush_buffer (&drv->pPendingBuffers, pBuffer);

        /* Try to initiate a transfer */
        InitiateTransfer (drv);
    }
    else
    {
        /* Return no free buffers error */
        eResult = ADI_FEE_ERR_NO_FREE_BUFFERS;
    }

    return eResult;
}


/**
 * @brief   Check if a buffer is available which is done processing
 *
 * If a buffer submited to adi_fee_SubmitTxBuffer() is done processing and available
 * to be retrieved then a value of 'true' will be written to pbAvailable else 'false'
 *
 * @param [in]  hDevice          The handle to the flash controller device
 * @param [in]  pbAvailable      True if buffer is available else false
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The status of buffer is returned successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Pointer passed is NULL
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_IsTxBufferAvailable (
                                            ADI_FEE_HANDLE    const  hDevice,
                                            bool_t*           const  pbAvailable
                                            )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (pbAvailable == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* If there is anything in the completed buffer list
     * then buffer is available for retrieval else no
     */
    if (drv->pCompletedBuffers != NULL)
    {
        *pbAvailable = true;
    }
    else
    {
        *pbAvailable = false;
    }

    return eResult;
}

/**
 * @brief    Get a processed buffer from the flash driver
 *
 * This function return a buffer submited to the adi_fee_SubmitTxBuffer() function
 * if it is done processing. This function is a blocking call and will only return
 * if a buffer is available.
 *
 * @param [in]  hDevice    The handle to the flash controller device
 * @param [out] ppBuffer   Pointer to the variable to which the buffer
 *                         pointer is written
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      A buffer is successfully retrieved
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Pointer is NULL
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_BUFFER_ERR           [D] Error occurred while processing the
 *                                                  returned buffer.
 */
ADI_FEE_RESULT adi_fee_GetTxBuffer (
                                    ADI_FEE_HANDLE    const  hDevice,
                                    void**            const  ppBuffer
                                    )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    FLASH_BUFFER* pBuffer;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (ppBuffer == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    while(1)
    {
        /* Pend for a semaphore */
        PEND_EVENT(drv,ADI_FEE_ERR_RTOS);

        if (drv->pCompletedBuffers != NULL) {
            break;
        }
    }

    /* Get the buffer from the completed buffer */
    pBuffer = pop_buffer(&drv->pCompletedBuffers);

    /* Check for errors in buffer write */
    if (pBuffer->nRemainingBytes != 0u) {
        eResult = ADI_FEE_ERR_BUFFER_ERR;
    }

    /* Give back the buffer */
    *ppBuffer = pBuffer->pBuffer;

    /* Return the buffer back to the free list */
    bpush_buffer(&drv->pFreeBuffers, pBuffer);

    return eResult;
}

/**
 * @brief    Erase the whole flash.
 *
 * @param [in]  hDevice   The handle to the flash controller device
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The flash is cleared successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is in progress.
 *          - #ADI_FEE_ERR_WRITE_PROTECTED          The function failed as some pages are
 *                                                  write protected
 *          - #ADI_FEE_ERR_READ_VERIFY_ERROR        Read verify error occurred
 *          - #ADI_FEE_ABORTED                      Command was aborted
 */
ADI_FEE_RESULT adi_fee_MassErase (ADI_FEE_HANDLE   const hDevice)
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (drv->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }
#endif


    /* Call the mass erase command */
    eResult = DoCmd (drv,  ENUM_FLCC_CMD_MASSERASE);

    return eResult;
}

/**
 * @brief    Erase the given page(s) of the flash.
 *
 * @param [in]  hDevice         The handle to the flash controller device
 * @param [in]  nPageNumStart   Start page number
 * @param [in]  nPageNumEnd     End page number
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The page(s) is(are) cleared successfully.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] The page(s) number(s) is(are) incorrect.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is in progress.
 *          - #ADI_FEE_ERR_WRITE_PROTECTED          The function failed as some pages are write
 *                                                  protected
 *          - #ADI_FEE_ERR_READ_VERIFY_ERROR        Read verify error occurred
 *          - #ADI_FEE_ABORTED                      Command was aborted
 */
ADI_FEE_RESULT adi_fee_PageErase (
                                  ADI_FEE_HANDLE   const hDevice,
                                  uint32_t         const nPageNumStart,
                                  uint32_t         const nPageNumEnd
                                  )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

    uint32_t x;

#ifdef ADI_DEBUG

    uint32_t nRelAddrStart = (nPageNumStart << FEE_PAGE_SHIFT);
    uint32_t nRelAddrStop  = (nPageNumEnd   << FEE_PAGE_SHIFT);

    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (drv->bTransferInProgress)
    {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

    if (   (nPageNumStart > nPageNumEnd)
        || (nRelAddrStart >= FEE_FLASH_SIZE)
        || (nRelAddrStop  >= FEE_FLASH_SIZE))
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }

    for (x = nPageNumStart; x <= nPageNumEnd; x++)
    {
        uint32_t nRelAddr = (x << FEE_PAGE_SHIFT);
        if (isWriteProtected(drv, nRelAddr))
        {
            return ADI_FEE_ERR_WRITE_PROTECTED;
        }
    }
#endif /* defined (ADI_DEBUG) */

    for (x = nPageNumStart; x <= nPageNumEnd; x++)
    {
        /* Wait until not busy */
        waitWhileBusy(drv, (BITM_FLCC_STAT_CMDBUSY | BITM_FLCC_STAT_WRCLOSE));

        /* Set the page address */
        drv->pReg->PAGE_ADDR0 = ( x << FEE_PAGE_SHIFT);

        /* Issue a page erase command */
        eResult = DoCmd (drv,  ENUM_FLCC_CMD_ERASEPAGE);

        if (eResult != ADI_FEE_SUCCESS) {
            break;
        }
    }

    return eResult;
}

/**
 * @brief    Get the block number corresponding to the given flash address
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [in]  nAddress    The flash address for which the block number is required
 * @param [in]  pnBlockNum  Pointer to a variable to which the block number corresponding
 *                          to the nAddress is written.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The block number is returned successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameter(s) are invalid
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_GetBlockNumber (
                                       ADI_FEE_HANDLE    const  hDevice,
                                       uint32_t          const  nAddress,
                                       uint32_t*         const  pnBlockNum
                                       )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (   (pnBlockNum == NULL)
        || (nAddress >= FEE_FLASH_SIZE))
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Set the block number */
    *pnBlockNum = (nAddress >> FEE_BLOCK_SHIFT);

    return eResult;
}



/**
 * @brief   Get the page number corresponding to the given flash address.
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [in]  nAddress    The flash address for which the page number is required
 * @param [in]  pnPageNum   The pointer to the variable to which the page number
 *                          of the flash address is written
 *
 * @return  Status
 *          - #ADI_FEE_SUCCESS                      The page number is returned successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameter(s) are invalid
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_GetPageNumber (
                                       ADI_FEE_HANDLE    const  hDevice,
                                       uint32_t          const  nAddress,
                                       uint32_t*         const  pnPageNum
                                       )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (   (pnPageNum == NULL)
        || (nAddress >= FEE_FLASH_SIZE))
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Set the page number for the given flash address */
    *pnPageNum = (nAddress >> FEE_PAGE_SHIFT);

    return eResult;
}



/* Generate and Verify the signature for the pages
 * from nLowerPage to nUpperPage
 */
/**
 * @brief    Verify the signature of the given range of page(s)
 *
 * The function compute the signature for the given range of page(s) and verify it with
 * the signature at the start of the lower page. It also return the signature to the application.
 *
 * @param [in]  hDevice      The handle to the flash controller device
 * @param [in]  nLowerPage   The lower page of the signature range
 * @param [in]  nUpperPage   The upper page of the signature range
 * @param [in]  pSigResult   Pointer to the variable to which the computed
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The signature is verified successfully.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] The page(s) number(s) is(are) incorrect.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_WRITE_PROTECTED          The function failed as some pages are write protected
 *          - #ADI_FEE_ERR_READ_VERIFY_ERROR        Verify error occurred
 *          - #ADI_FEE_ABORTED                      Command was aborted
 */
ADI_FEE_RESULT adi_fee_VerifySignature (
                                        ADI_FEE_HANDLE    const  hDevice,
                                        uint32_t          const  nLowerPage,
                                        uint32_t          const  nUpperPage,
                                        uint32_t*         const  pSigResult
                                        )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (   (pSigResult == NULL)
        || (nLowerPage >  nUpperPage)
        || (nLowerPage >= FEE_PAGE_MAX)
        || (nUpperPage >= FEE_PAGE_MAX)
        )
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Wait until not busy */
    waitWhileBusy (drv, (BITM_FLCC_STAT_CMDBUSY | BITM_FLCC_STAT_WRCLOSE));

    /* Set the lower and upper page */
    drv->pReg->PAGE_ADDR0 = nLowerPage << FEE_PAGE_SHIFT;
    drv->pReg->PAGE_ADDR1 = nUpperPage << FEE_PAGE_SHIFT;

    /* Do a SIGN command */
    eResult = DoCmd(drv, ENUM_FLCC_CMD_SIGN);

    /* Return the signature to the application */
    *pSigResult = drv->pReg->SIGNATURE;

    return eResult;
}


/**
 * @brief    Make a block as write protected.
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [in]  nBlockNum   The block number
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The block is write protected successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Block number is invalid
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_WriteProtectBlock (
                                          ADI_FEE_HANDLE  const  hDevice,
                                          uint32_t        const  nBlockNum
                                          )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (nBlockNum > 32u) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Set the write protection for the given block */
    drv->pReg->WRPROT |= (uint32_t)1u << nBlockNum;

    return eResult;
}


/* Forcefully ABORT the current flash operation */
/**
 * @brief    Forcefully ABORT the current flash operation
 *
 * @param [in]  hDevice   The handle to the flash controller device
 *
 * @return   Statuus
 *          - #ADI_FEE_SUCCESS                      The command is successfully aborted.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_Abort (ADI_FEE_HANDLE const hDevice)

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Issue the command */
    drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
    drv->pReg->CMD = ENUM_FLCC_CMD_ABORT;
    drv->pReg->KEY = 0u;

    return eResult;
}

/**
 * @brief    Configure ECC for the device
 *
 * @param [in]  hDevice         The handle to the flash controller device
 * @param [in]  nStartPage      The page from which ECC will be enabled
 * @param [in]  bInfoECCEnable  'true' to enable and 'false' to disable ECC
 *                              for info space
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC is configured successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Start page is invalid
 */
ADI_FEE_RESULT adi_fee_ConfigECC (
                                  ADI_FEE_HANDLE   const hDevice,
                                  uint32_t         const nStartPage,
                                  bool_t           const bInfoECCEnable
                                  )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    uint32_t nRelAddress = nStartPage << FEE_PAGE_SHIFT;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (nStartPage >= FEE_PAGE_MAX) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Clear the configuration bits */
    CLR_BITS (
              drv->pReg->ECC_CFG,
              (BITM_FLCC_ECC_CFG_PTR | BITM_FLCC_ECC_CFG_INFOEN)
              );


    /* Set the start page address in the ECC Cfg register */
    drv->pReg->ECC_CFG |= (nRelAddress & BITM_FLCC_ECC_CFG_PTR);

    /* IF (ECC for Info Space need to be configured) */
    if (bInfoECCEnable)
    {
        SET_BITS (drv->pReg->ECC_CFG, BITM_FLCC_ECC_CFG_INFOEN);
    }


    return eResult;
}


/**
 * @brief    Enable/Disable ECC for the device
 *
 * @param [in]  hDevice      The handle to the flash controller device
 * @param [in]  bEnable      'true' to enable and 'false' to disable
 *                           ECC for the device
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC is enabled/disabled successfully
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_EnableECC (
                                  ADI_FEE_HANDLE     const hDevice,
                                  bool_t             const bEnable
                                  )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    /* Validate device handle */
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }
#endif

    /* IF (ECC need to enabled) */
    if (bEnable)
    {
        /* Set the ECC Enable bit */
        SET_BITS(drv->pReg->ECC_CFG, BITM_FLCC_ECC_CFG_EN);
    }
    else
    {
        /* Clear the ECC Enable bit */
        CLR_BITS(drv->pReg->ECC_CFG, BITM_FLCC_ECC_CFG_EN);
    }

    return eResult;
}


/**
 * @brief    Confifure ECC event reporting behaviour
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [in]  eEvent      ECC event - Error/Correction
 * @param [in]  eResponse   The response to the eEvent
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC events are configured successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameters are invalid
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_ConfigECCEvents (
                                        ADI_FEE_HANDLE         const hDevice,
                                        ADI_FEE_ECC_EVENT_TYPE const eEvent,
                                        ADI_FEE_ECC_RESPONSE   const eResponse
                                        )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    uint32_t nBitMask;
    int32_t nBitPos;

#ifdef ADI_DEBUG
    /* Validate Device Handle */
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    /* Check the function parameters */
    if (   (   (eEvent != ADI_FEE_ECC_EVENT_TYPE_ERROR)
            && (eEvent != ADI_FEE_ECC_EVENT_TYPE_CORRECT))
        || (   (eResponse != ADI_FEE_ECC_RESPONSE_NONE)
            && (eResponse != ADI_FEE_ECC_RESPONSE_BUS_ERROR)
            && (eResponse != ADI_FEE_ECC_RESPONSE_IRQ))
        )
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Select the correct bit mask and bit pos */
    if (eEvent == ADI_FEE_ECC_EVENT_TYPE_ERROR)
    {
        nBitMask = BITM_FLCC_IEN_ECC_ERROR;
        nBitPos  = BITP_FLCC_IEN_ECC_ERROR;
    }
    else
    {
        nBitMask = BITM_FLCC_IEN_ECC_CORRECT;
        nBitPos  = BITP_FLCC_IEN_ECC_CORRECT;
    }

    /* Clear the bits */
    CLR_BITS (drv->pReg->IEN, nBitMask);

    /* Configure the ECC events */
    drv->pReg->IEN |= (((uint32_t)eResponse) << nBitPos);

    return eResult;
}


/**
 * `@brief    Get the address for which the ECC error occurred.
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [in]  pnAddress   Pointer to which the address is written
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC error address is retrieved successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameters are invalid
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_GetECCErrAddr (
                                      ADI_FEE_HANDLE     const hDevice,
                                      uint32_t*          const pnAddress
                                      )

{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (pnAddress == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Write the address of the last ECC error/correction */
    *pnAddress = drv->pReg->ECC_ADDR;

    return eResult;
}



/**
 * @brief    Get the address for which the abort write occurred
 *
 * @param [in]  hDevice     The handle to the flash controller device
 * @param [out] pnAddress   Pointer to which the address is writtend
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The abort address is retrieved successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Pointer passed is NULL
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_GetAbortAddr (
                                     ADI_FEE_HANDLE     const hDevice,
                                     uint32_t*          const pnAddress
                                     )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (pnAddress == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Write the address of the last write abort to the pointer
     * supplied by the application
     */
    *pnAddress = drv->pReg->WR_ABORT_ADDR;

    return eResult;
}

/**
 * @brief    Get the number of 1-bit errors which are corrected.
 *
 * @param [in]  hDevice            The handle to the flash controller device
 * @param [in]  pnNumCorrections   Pointer to which the number of corrections
 *                                 are written.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The number of ECC corrections are successfully
 *                                                  retrieved.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameters are invalid
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_GetECCCorrections (
    ADI_FEE_HANDLE  const hDevice,
    uint32_t*       const pnNumCorrections
    )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }

    if (pnNumCorrections == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Get the number of ECC Error corrections */
    *pnNumCorrections = (drv->pReg->STAT & BITM_FLCC_STAT_ECCERRCNT)
                                            >> BITP_FLCC_STAT_ECCERRCNT;

    return eResult;
}

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
/**
 * @brief    Enable/Disable the DMA for the flash controller
 *
 * @param [in]  hDevice   The handle to the flash controller device
 * @param [in]  bEnable   'true' to enable and 'false' to disable
 *                        the DMA
 *
 * @return   Status
 *
 */
ADI_FEE_RESULT adi_fee_EnableDMA (
                                  ADI_FEE_HANDLE  const hDevice,
                                  bool_t          const bEnable
                                  )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Set the FIFO function for transfer */
    drv->pfManageFIFO = (bEnable) ? DMA_Transfer : PIO_Transfer;
    drv->bUseDmaForTransfer = bEnable;

    return eResult;
}
#endif

/**
 * @brief    Sleep the device or Wake up the device
 *
 * @param [in]  hDevice   The handle to the flash controller device
 * @param [in]  bSleep    'true' to enable to sleep the flash device
 *                        and 'false' to wake up the device
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The flash controller is moved to sleep/wake
 *                                                  up sate successfully
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 */
ADI_FEE_RESULT adi_fee_Sleep (
                              ADI_FEE_HANDLE const hDevice,
                              bool_t         const bSleep
                              )
{
    ADI_FEE_RESULT eResult = ADI_FEE_SUCCESS;
    FLASH_DEVICE* drv = (FLASH_DEVICE*)hDevice;
    uint32_t cmd;

#ifdef ADI_DEBUG
    if ((eResult = ValidateHandle(drv)) != ADI_FEE_SUCCESS)
    {
        return eResult;
    }
#endif

    /* TODO: Check that IDLE can take the controller
     *       out of sleep
     */

    if (bSleep) {
        cmd = ENUM_FLCC_CMD_SLEEP;
    } else {
        cmd = ENUM_FLCC_CMD_IDLE;
    }

    /* Call the mass erase command */
    eResult = DoCmd (drv,  cmd);

    return eResult;
}


/*========  L O C A L    F U N C T I O N    D E F I N I T I O N S  ========*/
/* Initialize the device structure */
static void Initialize (
                        uint32_t        const nDeviceNum,
                        FLASH_DEVICE*   const drv
                        )
{
    int32_t x;

    /* Clear the device structure */
    memset(drv, 0, sizeof(FLASH_DEVICE));

    /* Initialize the structure */
    drv->pReg = FlashDevInfo[nDeviceNum].pReg;

    /* Form the Free Buffer List */
    for (x = 0; x < (NUM_BUFFERS-1); x++)
    {
        /* Link to the next buffers */
        drv->Buffers[x].pNext = &drv->Buffers[x+1];
    }

    /* Close the buffer list */
    drv->Buffers[NUM_BUFFERS-1].pNext = NULL;

    /* Set the buffer list */
    drv->pFreeBuffers = &drv->Buffers[0];

    /* Set the FIFO function */
#if (ADI_FEE_CFG_ENABLE_DMA == 0)
    drv->pfManageFIFO = PIO_Transfer;
#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
    drv->bUseDmaForTransfer = false;
#endif
#else
    drv->pfManageFIFO = DMA_Transfer;
#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
    drv->bUseDmaForTransfer = false;
#endif
#endif
    /* Configure the registers */
    CLR_BITS (
              drv->pReg->IEN,
              (  BITM_FLCC_IEN_ECC_ERROR
               | BITM_FLCC_IEN_ECC_CORRECT
               ));

    SET_BITS (
              drv->pReg->IEN,
              (  ((uint32_t)ADI_FEE_CFG_ECC_ERROR_RESPONSE << BITP_FLCC_IEN_ECC_ERROR)
               | ((uint32_t)ADI_FEE_CFG_ECC_CORRECTION_RESPONSE << BITP_FLCC_IEN_ECC_CORRECT)
               ));


    CLR_BITS (
              drv->pReg->ECC_CFG,
              (  BITM_FLCC_ECC_CFG_PTR
               | BITM_FLCC_ECC_CFG_INFOEN
               | BITM_FLCC_ECC_CFG_EN
               ));

    SET_BITS (
              drv->pReg->ECC_CFG,
              (
                 ((ADI_FEE_CFG_ECC_START_PAGE << FEE_PAGE_SHIFT) & BITM_FLCC_ECC_CFG_PTR)

#if (ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE == 1)
               | (BITM_FLCC_ECC_CFG_INFOEN)
#endif /*(ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE == 1) */

#if (ADI_FEE_CFG_ENABLE_ECC == 1)
               | (BITM_FLCC_ECC_CFG_EN)
#endif /* (ADI_FEE_CFG_ENABLE_ECC == 1) */

               ));
}

#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 1)
/* wait for the given interrupt to occur */
static uint32_t waitForInterrupt (
                                  FLASH_DEVICE* const drv,
                                  uint32_t      const StatusFlag
                                  )
{
    while (1)
    {
        /* PEND for Semaphore */
         PEND_EVENT(drv,ADI_FEE_ERR_RTOS);

        /* If required status is set */
        if((drv->FlashStatusCopy & StatusFlag) == StatusFlag) {

            /* Clear the status flag */
            CLR_BITS (drv->FlashStatusCopy, StatusFlag);

            break;
        }
    }

    return drv->FlashStatusCopy;
}
#endif

#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 0)
/* Wait for the given status */
static uint32_t WaitForStatus (
                               FLASH_DEVICE* const drv,
                               uint32_t      const nStatusFlag
                               )
{
    /* Wait for the status to occur */
    while ((drv->pReg->STAT & nStatusFlag) != nStatusFlag) { }

    /* Return the status */
    return drv->pReg->STAT;
}
#endif

/* Send a command to the flash controller and
 * wait for the command to occur
 */
static ADI_FEE_RESULT DoCmd (
                             FLASH_DEVICE* const drv,
                             uint32_t      const cmd
                             )
{
    ADI_FEE_RESULT eResult;
    uint32_t status;

    /* Wait for the flash to be free */
    waitWhileBusy (
                   drv,
                     BITM_FLCC_STAT_CMDBUSY
                   | BITM_FLCC_STAT_WRCLOSE
                   );

#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 1)

    /* Clear the command completion status bit
     * by acknowledging it
     */
    drv->pReg->STAT = BITM_FLCC_STAT_CMDCOMP;

    /* Enable completion interrupts */
    SET_BITS(drv->pReg->IEN, BITM_FLCC_IEN_CMDCMPLT);

    /* Enable notification for the completion status */
    SET_BITS(drv->WaitStatus,BITM_FLCC_STAT_CMDCOMP);

    /* Issue the command */
    drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
    drv->pReg->CMD = cmd;
    drv->pReg->KEY = 0u;

    /* Wait for the given interrupt to occur */
    status = waitForInterrupt(drv, BITM_FLCC_STAT_CMDCOMP);

    /* Disable notification for the completion status */
    CLR_BITS(drv->WaitStatus, BITM_FLCC_STAT_CMDCOMP);

    /* Clear the completion interrupts */
    CLR_BITS(drv->pReg->IEN, BITM_FLCC_IEN_CMDCMPLT);
#else
    /* Clear the command completion status bit
     * by acknowledging it
     */
    drv->pReg->STAT = BITM_FLCC_STAT_CMDCOMP;

    /* Issue the command */
    drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
    drv->pReg->CMD  = cmd;
    drv->pReg->KEY = 0u;

    /* Wait for the command to complete */
    status = WaitForStatus(drv, BITM_FLCC_STAT_CMDCOMP);

    /* Clear the command completion status bit
     * by acknowledging it
     */
    drv->pReg->STAT = BITM_FLCC_STAT_CMDCOMP;
#endif

    /* Get the status of the command */
    {
        uint32_t nResultPos = (status & BITM_FLCC_STAT_CMDFAIL)
                                    >> BITP_FLCC_STAT_CMDFAIL;
        ADI_FEE_RESULT eResultArray[4] = {
            ADI_FEE_SUCCESS,
            ADI_FEE_ERR_WRITE_PROTECTED,
            ADI_FEE_ERR_READ_VERIFY_ERROR,
            ADI_FEE_ABORTED
        };

        eResult = eResultArray[nResultPos];
    }

    return eResult;
}


/* Pop a buffer from the buffer list */
static FLASH_BUFFER* pop_buffer (FLASH_BUFFER**  pBufferList)
{
    FLASH_BUFFER* pBuffer = NULL;
    ASSERT (pBufferList != NULL);

    if ((*pBufferList) != NULL)
    {
        /* Enter critical region */
        ADI_ENTER_CRITICAL_REGION();

        /* Pop the  list from the buffer list */
        pBuffer = (*pBufferList);
        *pBufferList = (*pBufferList)->pNext;
        pBuffer->pNext = NULL;

        /* Exit Critical Region */
        ADI_EXIT_CRITICAL_REGION();
    }

    return pBuffer;
}

/* Push a buffer into the buffer list */
static void bpush_buffer (
                          FLASH_BUFFER** pBufferList,
                          FLASH_BUFFER*  pBuffer
                          )
{
    ASSERT (pBuffer != NULL);
    ASSERT (pBuffer->pNext == NULL);
    ASSERT (pBufferList != NULL);

    FLASH_BUFFER* pBuff = *pBufferList;

    /* Enter critical region */
    ADI_ENTER_CRITICAL_REGION();

    /* IF (The buffer list is not empty) */
    if (pBuff != NULL)
    {
        /* Move to the last entry in the list */
        while (pBuff->pNext != NULL) {
            pBuff = pBuff->pNext;
        }

        /* Append the buffer to the last entry in the list */
        pBuff->pNext = pBuffer;

    }
    /* ELSE (The buffer list is empty) */
    else
    {
        /* Set the buffer itself as the buffer list entry */
        *pBufferList = pBuffer;
    }

    /* Exit Critical Region */
    ADI_EXIT_CRITICAL_REGION();
}

/* Write data to the flash registers */
static void WriteData(FLASH_DEVICE* const drv)
{
    uint32_t Data0, Data1;

    Data0 = *drv->pActiveBuffer->pCurBuffPos;
    drv->pActiveBuffer->pCurBuffPos++;
    Data1 = *drv->pActiveBuffer->pCurBuffPos;
    drv->pActiveBuffer->pCurBuffPos++;

    /* Set the Data to be written */
    drv->pReg->KH_DATA1 = Data1;
    drv->pReg->KH_DATA0 = Data0;

    /* Issue a write command */
    drv->pReg->CMD = ENUM_FLCC_CMD_WRITE;
}

#if (ADI_FEE_CFG_ENABLE_INTERRUPT_SUPPORT == 0)
/* PIO Transfer */
static ADI_FEE_RESULT PIO_Transfer (
                          FLASH_DEVICE* const drv,
                          bool_t        const bStart,
                          bool_t        const bInterruptRegion
                          )
{
    ASSERT ((bStart == true) && (bInterruptRegion == false));

    /* Set the start address */
    drv->pReg->KH_ADDR = drv->pActiveBuffer->nStartAddress;

    /* Set the auto increment address flag */
    drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
    SET_BITS (drv->pReg->UCFG, BITM_FLCC_UCFG_AUTOINCEN);
    drv->pReg->KEY = 0u;

    while (drv->pActiveBuffer->nRemainingBytes > 0u)
    {
        /* Clear the write almost complete flag and cmd fail
         * by writing one to that */
        drv->pReg->STAT = BITM_FLCC_STAT_WRALCOMP | BITM_FLCC_STAT_CMDFAIL;

        /* Enter Critical Region */
        ADI_ENTER_CRITICAL_REGION();

        /* Issue a write to the device */
        WriteData(drv);

        /* Exit Critical Region */
        ADI_EXIT_CRITICAL_REGION();

        /* Wait for the write almost complete or an error  */
        while ((drv->pReg->STAT & (BITM_FLCC_STAT_WRALCOMP | BITM_FLCC_STAT_CMDFAIL)) == 0u)
        { }

        /* Break if any error has occurred */
        if ((drv->pReg->STAT & BITM_FLCC_STAT_CMDFAIL) != 0u) {
            break;
        }

        drv->pActiveBuffer->nRemainingBytes -= 8u;
    }

    /* Clear the auto increment address flag */
    drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
    CLR_BITS (drv->pReg->UCFG, BITM_FLCC_UCFG_AUTOINCEN);
    drv->pReg->KEY = 0u;

    /* Move the buffer to the completed buffers */
    bpush_buffer(&drv->pCompletedBuffers, drv->pActiveBuffer);
    drv->pActiveBuffer = NULL;

    /* Post an Event to mark that buffer is complete */
    POST_EVENT(drv);


    /* Change the transfer in progress boolean flag */
    drv->bTransferInProgress = false;

    return ADI_FEE_SUCCESS;
}

#else
static ADI_FEE_RESULT PIO_Transfer (
                          FLASH_DEVICE* const drv,
                          bool_t        const bStart,
                          bool_t        const bInterruptRegion
                          )
{
    bool_t bReturnBuffer = false;

    if (bStart)
    {
        /* Set the start address */
        drv->pReg->KH_ADDR = drv->pActiveBuffer->nStartAddress;

        /* Set the auto increment address flag */
        drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
        SET_BITS (drv->pReg->UCFG, BITM_FLCC_UCFG_AUTOINCEN);
        drv->pReg->KEY = 0u;

        /* Clear the write almost complete flag and cmd fail
         * by writing one to that */
        drv->pReg->STAT =   BITM_FLCC_STAT_WRALCOMP
                          | BITM_FLCC_STAT_CMDFAIL
                          | BITM_FLCC_STAT_CMDCOMP;

        /* Enable write almost completed and
         * command completion interrupts and
         * failure interrupt for the flash
         */
        drv->pReg->IEN |=   BITM_FLCC_IEN_WRALCMPLT
                          | BITM_FLCC_IEN_CMDCMPLT
                          | BITM_FLCC_IEN_CMDFAIL;

        /* Write a data to the flash to start the transaction */
        WriteData(drv);
    }
    else
    {
        /* IF (No error has occurred) */
        if ((drv->pReg->STAT & BITM_FLCC_STAT_CMDFAIL) == 0u)
        {
            /* We have reached here because of
             * completetion interrupt or write almost complete
             */

            if (drv->pActiveBuffer->nRemainingBytes > 8u)
            {
                /* Reduce the remaining bytes for previous successfull write */
                drv->pActiveBuffer->nRemainingBytes -= 8u;

                /* Clear write almost complete bit in status */
                drv->pReg->STAT = BITM_FLCC_STAT_WRALCOMP;

                /* Write new data */
                WriteData(drv);

                /* Note: Write Data will clear any command complete bit in case
                 *       it is set.
                 * TODO: Check this
                 */
            }
            else
            {
                /* Clear the write almost complete interrupt */
                CLR_BITS (drv->pReg->IEN, BITM_FLCC_IEN_WRALCMPLT);

                /* if command has been completed */
                if ((drv->pReg->STAT & BITM_FLCC_STAT_CMDCOMP) != 0u)
                {
                    /* Clear the remaining bytes as the whole buffer is done */
                    drv->pActiveBuffer->nRemainingBytes = 0u;

                    /* Mark the buffer to be returned */
                    bReturnBuffer = true;
                }
            }
        }
        else
        {
            bReturnBuffer = true;
        }

        /* IF (Buffer has to be returned) */
        if (bReturnBuffer)
        {
            /* Clear all the interrupts */
            CLR_BITS(drv->pReg->IEN,  BITM_FLCC_IEN_WRALCMPLT
                                    | BITM_FLCC_IEN_CMDCMPLT
                                    | BITM_FLCC_IEN_CMDFAIL);

            drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;
            CLR_BITS (drv->pReg->UCFG, BITM_FLCC_UCFG_AUTOINCEN);
            drv->pReg->KEY = 0u;


            /* Move the buffer to the completed buffers */
            bpush_buffer(&drv->pCompletedBuffers, drv->pActiveBuffer);
            drv->pActiveBuffer = NULL;

            /* Change the transfer in progress boolean */
            drv->bTransferInProgress = false;
        }
    }
    return(ADI_FEE_SUCCESS);
}
#endif

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
/* DMA Transfer to FIFO */
static ADI_FEE_RESULT DMA_Transfer (
                          FLASH_DEVICE* const drv,
                          bool_t        const bStart,
                          bool_t        const bInterruptRegion
                          )
{
    ADI_DMA_RESULT eDmaResult;
    ADI_DMA_TRANSFER Transfer;

    if (bStart)
    {
        if (drv->hDmaHandle == NULL)
        {
            /* Open the DMA Handle */
            eDmaResult = adi_dma_Open (
                                       ADI_DMA_CHANNEL_FLASH,
                                       &drv->DMAMemory[0],
                                       &drv->hDmaHandle,
                                       DMAHandler,
                                       drv
                    );
            if(eDmaResult != ADI_DMA_SUCCESS)
            {
              return (ADI_FEE_ERR_DMA);
            }

            /* Set the R_POWER as 1 for 2 transfers per request */
            eDmaResult = adi_dma_SetArbitrationRate (drv->hDmaHandle, ADI_DMA_RPOWER_2);
            if(eDmaResult != ADI_DMA_SUCCESS)
            {
              return (ADI_FEE_ERR_DMA);
            }

            /* Enable the DMA for transfer */
            eDmaResult = adi_dma_Enable (drv->hDmaHandle, true);
            if(eDmaResult != ADI_DMA_SUCCESS)
            {
              return (ADI_FEE_ERR_DMA);
            }
        }
        /* Set the start address */
        drv->pReg->KH_ADDR = drv->pActiveBuffer->nStartAddress;

        /* Set the Key */
        drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;

        /* Set the auto increment address flag */
        SET_BITS (drv->pReg->UCFG, BITM_FLCC_UCFG_AUTOINCEN);

        /* Setup the DMA transfer */
        Transfer.pSrcData = drv->pActiveBuffer->pBuffer;
        Transfer.pDstData = &drv->pReg->KH_DATA1;
        Transfer.DataWidth = ADI_DMA_WIDTH_4_BYTE;
        Transfer.NumTransfers = drv->pActiveBuffer->nTotalBytes / 4u;
        Transfer.SrcInc = ADI_DMA_INCR_4_BYTE;
        Transfer.DstInc = ADI_DMA_INCR_NONE;

        /* Submit the buffer for transfer */
        eDmaResult = adi_dma_SubmitTransfer (
                                             drv->hDmaHandle,
                                             &Transfer
                                             );
        ASSERT (eDmaResult == ADI_DMA_SUCCESS);

        /* Enable the DMA request in Flash */
        SET_BITS(drv->pReg->UCFG, BITM_FLCC_UCFG_KHDMAEN);

        /* Clear the Key */
        drv->pReg->KEY = 0u;
    }
    else
    {
        /* The DMA would have already completed when called from callback
         * or error would have occurred.
         *
         * The Interrupt routine should clear the remaining bytes to 0
         * if the buffer is done correctly.
         */

        /* Close the DMA */
        eDmaResult = adi_dma_Close (drv->hDmaHandle);
        ASSERT (eDmaResult == ADI_DMA_SUCCESS);

        /* Clear the DMA Handle */
        drv->hDmaHandle = NULL;

        /* Set the Key */
        drv->pReg->KEY = ENUM_FLCC_KEY_USERKEY;

        /* Dsiable the DMA request in Flash */
        CLR_BITS(drv->pReg->UCFG, BITM_FLCC_UCFG_KHDMAEN);

        /* Set the auto increment address flag */
        CLR_BITS (drv->pReg->UCFG, BITM_FLCC_UCFG_AUTOINCEN);

        /* Clear the Key */
        drv->pReg->KEY = 0u;



        /* Move the buffer to the completed buffers */
        bpush_buffer(&drv->pCompletedBuffers, drv->pActiveBuffer);
        drv->pActiveBuffer = NULL;

        /* Change the transfer in progress boolean */
        drv->bTransferInProgress = false;
    }
    return(ADI_FEE_SUCCESS);
}
#endif


/* Initiate transfer */
static void InitiateTransfer (FLASH_DEVICE* const drv)
{
    FLASH_BUFFER* pBuffer;

    /* If a transfer is in progress or if the pending buffers are empty
     * the return as there is nothing to be done now
     */
    if (    (drv->bTransferInProgress)
         || (drv->pPendingBuffers == NULL)
         )
    {
        return;
    }

    /* Wait for the flash to be not busy */
    /* TODO: Change this interrupt mode */
    while (1)
    {
        /* IF (Flash is not busy) */
        if ((drv->pReg->STAT & BITM_FLCC_STAT_CMDBUSY) == 0u) {
            break;
        }
    }

    /* Set the bool variable to signify that a transfer is in progress */
    drv->bTransferInProgress = true;

    /* Get the pending buffer */
    pBuffer = pop_buffer (&drv->pPendingBuffers);

    /* Set the buffer as active buffer */
    drv->pActiveBuffer = pBuffer;

    /* Call the corresponding Transfer functions */
    drv->pfManageFIFO (drv, true, false);
}

/* Process Completed Buffers */
static void ProcessCompletedBuffers(FLASH_DEVICE* drv)
{
    /* If (Callback is registered) */
    if (drv->pfCallback) {
        /* Get the completed buffer */
        FLASH_BUFFER* pBuffer = pop_buffer(&drv->pCompletedBuffers);
        ASSERT(pBuffer != NULL);

        /* Get the event to be send to the application */
        ADI_FEE_EVENT eEvent = (pBuffer->nRemainingBytes == 0u)
                                      ? ADI_FEE_EVENT_BUFFER_PROCESSED : ADI_FEE_EVENT_BUFFER_ERR;
        drv->pfCallback (
                         drv->pCBParam,
                         (uint32_t)eEvent,
                         pBuffer->pBuffer
                        );

        /* Free the buffer */
        bpush_buffer(&drv->pFreeBuffers, pBuffer);
    }
    else
    {
        POST_EVENT(drv);
    }

    /* Initiate a new transfer if there is any */
    InitiateTransfer(drv);
}

/* Manage FIFO in the interrupt region */
static void IntManageFIFO (FLASH_DEVICE* drv)
{
    /* Call the transmission function */
    drv->pfManageFIFO(drv,false,true);

    /* IF (There is no active buffer then it means the current transfer is done) */
    if (drv->pActiveBuffer == NULL) {
        ProcessCompletedBuffers(drv);
    }
}

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
/* DMA Callback Handler */
static void DMAHandler (void *pcbParam, uint32_t nEvent, void *pArg)
{
    /* Get the driver structure */
    FLASH_DEVICE* drv = (FLASH_DEVICE*)pcbParam;

    switch(nEvent)
    {
    /* Buffer completed successfully */
    case ADI_DMA_EVENT_BUFFER_PROCESSED:
        drv->pActiveBuffer->nRemainingBytes = 0u;
        break;
    /* Error occurred */
    default:
        break;
    }

    /* Call the Manage FIFO function */
    IntManageFIFO(drv);
}
#endif


/*! \cond PRIVATE */
/* Flash interrupt handler */
ADI_INT_HANDLER (Flash0_Int_Handler)
{
    ISR_PROLOG();
    /* Get the driver structure */
    FLASH_DEVICE* drv = FlashDevInfo[0].pDevice;

#ifdef ADI_DEBUG
    /* Return if the device is not opened - spurious interrupts */
    if (drv == NULL) {
        return;
    }
#endif

    /* IF (Transfer is in progress) */
    if (drv->bTransferInProgress)
    {
#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
        /* IF (DMA is not in use for transfer) */
        if (!drv->bUseDmaForTransfer)
        {
#endif
            /* Call the manage FIFO Function */
            IntManageFIFO(drv);
#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
        }
#endif
    }
    else
    {
        /* Copy the controller status to the device structure */
        drv->FlashStatusCopy = drv->pReg->STAT;

        /* Post Sem for the wait status */
        if ((drv->WaitStatus & drv->FlashStatusCopy) != 0u)
        {
            /* Post semaphore */
            POST_EVENT(drv);
        }

        /* Acknowledge the interrupts for the waits */
        drv->pReg->STAT = drv->WaitStatus;
    }

    /* TODO:Check Error occurred while buffer is transfering */
    /* IF (There is any ECC errors to be reported) */
    if ((drv->pReg->STAT & BITM_FLCC_STAT_ECCRDERR) != 0u) {
        if (drv->pfCallback) {
            uint32_t nArg = ((drv->pReg->STAT & BITM_FLCC_STAT_ECCRDERR) >> BITP_FLCC_STAT_ECCRDERR);
            drv->pfCallback (
                             drv->pCBParam,
                             ADI_FEE_EVENT_ECC_ERR,
                             (void*)nArg
                             );
        }
    }
    ISR_EPILOG();
}

/*! \endcond */
#ifdef ADI_DEBUG
/* Validate the device handle */
static ADI_FEE_RESULT ValidateHandle (FLASH_DEVICE* const drv)
{
    uint32_t x;
    ADI_FEE_RESULT eResult = ADI_FEE_ERR_INVALID_HANDLE;

    for (x = 0u; x < NUM_DEVICES; x++)
    {
        if (FlashDevInfo[x].pDevice == drv)
        {
            eResult = ADI_FEE_SUCCESS;
            break;
        }
    }

    return eResult;
}

/* Check whether the given address is write protected */
static bool_t isWriteProtected (
                                FLASH_DEVICE* const drv,
                                uint32_t      const nRelAddress
                                )
{
    uint32_t nWriteProtectedReg;
    uint32_t nBlockNum;

    /* Get the write protected register */
    nWriteProtectedReg = drv->pReg->WRPROT;

    /* Translate address to a block number */
    nBlockNum = nRelAddress >> FEE_BLOCK_SHIFT;

    /* Write protection is enabled if the bit for block is cleared */
    if ((nWriteProtectedReg & ((uint32_t)1u << nBlockNum)) == 0u)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif

#endif /* _ADI_FLASH_V1_C_ */

/**@}*/
