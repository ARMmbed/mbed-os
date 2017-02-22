/*!
 *****************************************************************************
 @file:    adi_flash.c
 @brief:   Flash Device Driver Implementation
 @date:    $Date: 2016-06-30 08:06:37 -0400 (Thu, 30 Jun 2016) $
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

/** @addtogroup Flash_Driver Flash Driver
 *  @{
 *
 *  @brief <b>Flash (FEE) Driver</b>
 *
 *  @details
 *
 *  The flash controller provides access to the embedded flash memory. The embedded
 *  flash has a 72-bit wide data bus providing for two 32-bit words of data and
 *  one corresponding 8-bit ECC byte per access.
 *
 *  <b>Flash Driver Hardware Errors</b>
 *
 * Many of the Flash Controller APIs can result in hardware errors.  Each such API has a
 * a hardware error parameter (pHwErrors), which is a pointer to an application-defined
 * variable into which the failing API will store the failing hardware error status.\n
 *
 * APIs failing with hardware errors are flagged with the #ADI_FEE_ERR_HW_ERROR_DETECTED
 * return code.\n
 *
 * Hardware error details may be decoded according to the flash controller status register
 * ("STAT") bit-map, documented in the Hardware Reference Manual (HRM).  Flash hardware
 * errors are separate and distinct from DMA errors, which have separate and distinct
 * return codes (#ADI_FEE_ERR_DMA_BUS_FAULT, #ADI_FEE_ERR_DMA_INVALID_DESCR, and
 * #ADI_FEE_ERR_DMA_UNKNOWN_ERROR).
 *
 *  <b>Flash Driver Static Configuration</b>
 *
 * A number of flash driver APIs manage configurations that very likely do not require
 * dynamic (run-time) management.  Such cases are documented with the respective APIs.
 * In all such cases, the user is encouraged to consider using the static configuration
 * equivalents (provided in the adi_flash_config.h file) in lieu of the dynamic APIs.
 * In so doing, linker elimination may reduce the resulting code image footprint
 * (provided the API is not called).
 *
 * @note - The application must include drivers/flash/adi_flash.h to use this driver.
 * @note - This driver also requires the DMA driver.  The application must include
 *         the DMA driver sources to avoid link errors.
 */

/*========  I N C L U D E  ========*/

 /*! \cond PRIVATE */
#include <adi_processor.h>
#include <assert.h>
#include <string.h>     /* for "memset" */
/*! \endcond */

#include <drivers/flash/adi_flash.h>

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
*
* Pm026 (rule 12.4): the right hand operand of an && or || operator shall not contain side effects
*   Side effects being mis-reported due to added volatile storage class.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm088,Pm140,Pm026
#endif /* __ICCARM__ */

/* pull in internal data structures */
#include "adi_flash_data.c"

/*========  D E F I N E S   ========*/

/*! \cond PRIVATE */

#ifdef ADI_DEBUG
#define ASSERT(X)   assert(X)
#else
#define ASSERT(X)
#endif

/* internal utility macros */
#define CLR_BITS(REG, BITS) ((REG) &= ~(BITS))
#define SET_BITS(REG, BITS) ((REG) |= (BITS))

#ifdef ADI_DEBUG
/* Validate Device Handle */
static bool IsDeviceHandle (ADI_FEE_HANDLE const hDevice);
static bool IsDeviceHandle (ADI_FEE_HANDLE const hDevice)
{
    if ( (fee_device_info[0].hDevice == (hDevice)) && ((hDevice)->pDevInfo->hDevice != NULL) ) {
        return true;
    } else {
        return false;
    }
}
#endif

/* Wait for specified flash status to be clear */
static void BusyWait (ADI_FEE_HANDLE const hDevice, uint32_t const status);
static void BusyWait (ADI_FEE_HANDLE const hDevice, uint32_t const status)
{
    while ((hDevice->pDev->STAT & status) != 0u) {}
}

/* Internal DMA Callback for receiving DMA faults from common DMA error handler */
static void dmaCallback(void *pCBParam, uint32_t Event, void *pArg);
static void dmaCallback(void *pCBParam, uint32_t Event, void *pArg) {

    /* recover the device handle */
    ADI_FEE_HANDLE hDevice = (ADI_FEE_HANDLE)pCBParam;

    /* save the DMA error */
    switch (Event) {
        case ADI_DMA_EVENT_ERR_BUS:
            hDevice->dmaError = ADI_FEE_ERR_DMA_BUS_FAULT;
            break;
        case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            hDevice->dmaError = ADI_FEE_ERR_DMA_INVALID_DESCR;
            break;
        default:
            hDevice->dmaError = ADI_FEE_ERR_DMA_UNKNOWN_ERROR;
            break;
    }

    /* transfer is toast... post and callback any waiters */

    SEM_POST(hDevice);

    if (0u != hDevice->pfCallback) {
        hDevice->pfCallback (hDevice->pCBParam, (uint32_t)hDevice->dmaError, (void*)NULL);
    }
}

/*! \endcond */


/*========  C O D E  ========*/
/*
 * API Implementation
 */


/**
 * @brief    Open the flash controller.
 *
 * @param [in]     nDeviceNum    The zero-based device instance number of flash controller to be opened.
 * @param [in]     pMemory       Application supplied memory space for use by the driver.
 * @param [in]     nMemorySize   Size of the application supplied memory (in bytes).
 * @param [in,out] phDevice      The caller's device handle pointer for storing the initialized
 *                               device instance data pointer.
 *
 * @return  Status
 *          - #ADI_FEE_SUCCESS                      The device is opened successfully.
 *          - #ADI_FEE_ERR_BAD_DEVICE_NUM       [D] The device number passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Some pointer(s) passed to the function is NULL.
 *          - #ADI_FEE_ERR_ALREADY_INITIALIZED  [D] The device is already initialized and hence cannot be opened.
 *          - #ADI_FEE_ERR_INSUFFICIENT_MEM     [D] The memory passed to the driver is insufficient.
 *          - #ADI_FEE_ERR_DMA_REGISTER             The required DMA common error handler registration failed.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore create operation failed.
 *
 * Initialize an instance of the flash device driver using default user configuration settings
 * (from adi_flash_config.h) and allocate the device for use.
 *
 * No other flash APIs may be called until the device open function is called.  The returned
 * device handle is required to be passed to all subsequent flash API calls to identify the
 * physical device instance in use.  The user device handle (pointed to by phDevice) is set
 * to NULL on failure.
 *
 * @note Currently, only a singular flash physical device instance (device ID "0") exists.
 *
 * @sa      adi_fee_Close().
 */
ADI_FEE_RESULT adi_fee_Open (uint32_t const nDeviceNum, void* const pMemory, uint32_t const nMemorySize, ADI_FEE_HANDLE* const phDevice)
{
    ADI_FEE_HANDLE hDevice = NULL;  /* initially */

#ifdef ADI_DEBUG
    if (nDeviceNum >= ADI_FEE_NUM_INSTANCES) {
        return ADI_FEE_ERR_BAD_DEVICE_NUM;
    }

    /* verify device is not already open */
    if (fee_device_info[nDeviceNum].hDevice != NULL) {
        return ADI_FEE_ERR_ALREADY_INITIALIZED;
    }

    if ((pMemory == NULL) || (phDevice == NULL)) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }

    if (nMemorySize < ADI_FEE_MEMORY_SIZE) {
        return ADI_FEE_ERR_INSUFFICIENT_MEM;
    }

    assert (ADI_FEE_MEMORY_SIZE == sizeof(ADI_FEE_DEV_DATA_TYPE));
#endif

    /* store a bad handle in case of failure */
    *phDevice = NULL;

    /* Link user memory (handle) into ADI_FEE_DEVICE_INFO data structure.
     *
     *       ADI_FEE_DEVICE_INFO  <==>   ADI_FEE_HANDLE
     */
    fee_device_info[nDeviceNum].hDevice = (ADI_FEE_DEV_DATA_TYPE *)pMemory;

    /* Clear the ADI_FEE_HANDLE memory. This also sets all bool
     * structure members to false so we do not need to waste cycles
     * setting these explicitly (e.g. hDevice->bUseDma = false)
     */
    memset(pMemory, 0, nMemorySize);

    /* initialize local device handle and link up device info for this device instance */
    hDevice = (ADI_FEE_HANDLE)pMemory;
    hDevice->pDevInfo = &fee_device_info[nDeviceNum];

    /* Although the ADI_FEE_DEVICE_INFO struct has the physical device pointer
     * for this instance, copying it to the ADI_FEE_HANDLE struct (in user memory)
     * will minimize the runtime footprint and cycle count when accessing the FEE
     * registers.
     */
    hDevice->pDev = fee_device_info[nDeviceNum].pDev;

    /* store a pointer to user's static configuration settings for this device instance */
    hDevice->pDevInfo->pConfig = (ADI_FEE_CONFIG*)&gConfigInfo[nDeviceNum];

    /* create the semaphore */
    SEM_CREATE(hDevice, "fee_sem", ADI_FEE_ERR_SEMAPHORE_FAILED);

    /* grant keyed access */
    hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;

        /* apply the static initializers */
        hDevice->pDev->IEN         = hDevice->pDevInfo->pConfig->eccIrqEnables;
        hDevice->pDev->TIME_PARAM0 = hDevice->pDevInfo->pConfig->param0;
        hDevice->pDev->TIME_PARAM1 = hDevice->pDevInfo->pConfig->param1;
        hDevice->pDev->ABORT_EN_LO = hDevice->pDevInfo->pConfig->abortEnableLo;
        hDevice->pDev->ABORT_EN_HI = hDevice->pDevInfo->pConfig->abortEnableHi;
        hDevice->pDev->ECC_CFG     = hDevice->pDevInfo->pConfig->eccConfig;

        /* clear auto-increment and dma enable bits */
        CLR_BITS (hDevice->pDev->UCFG, (BITM_FLCC_UCFG_AUTOINCEN | BITM_FLCC_UCFG_KHDMAEN));

    /* close keyed access */
    hDevice->pDev->KEY = 0u;

    /* store device handle into user handle */
    *phDevice = (ADI_FEE_HANDLE)hDevice;

    /* initialize DMA service */
    adi_dma_Init();

    if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(hDevice->pDevInfo->dmaChanNum, dmaCallback, (void*)hDevice)) {
        /* uninitialize flash driver and fail */
        adi_fee_Close(hDevice);
        return  ADI_FEE_ERR_DMA_REGISTER;
    }

    /* NVIC enables */
    NVIC_EnableIRQ(hDevice->pDevInfo->pioIrqNum);
    NVIC_EnableIRQ(hDevice->pDevInfo->dmaIrqNum);

    /* return success */
    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Close the flash controller.
 *
 * @param [in] hDevice   The handle to the flash controller device
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The device is closed successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore delete operation failed.
 *
 * Uninitialize and release an allocated flash device, and memory associated with it
 * for other use.
 *
 * @note The user memory is released from use by the flash driver, but is not freed.
 *
 * @sa  adi_fee_Open().
 */
ADI_FEE_RESULT adi_fee_Close (ADI_FEE_HANDLE const hDevice)
{
    uint32_t dev;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }
#endif

    /* Destroy the semaphore */
    SEM_DELETE(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    /* Remove the device handle from the list of possible device instances */
    for (dev = 0u; dev < ADI_FEE_NUM_INSTANCES; dev++)
    {
        if (fee_device_info[dev].hDevice == hDevice)
        {
            fee_device_info[dev].hDevice = NULL;
            break;
        }
    }

    /* NVIC disables */
    NVIC_DisableIRQ(hDevice->pDevInfo->pioIrqNum);
    NVIC_DisableIRQ(hDevice->pDevInfo->dmaIrqNum);

    return ADI_FEE_SUCCESS;
}


/**
 * @brief   Register an application-defined callback function.
 *
 * @param [in]  hDevice     The handle to the flash controller device.
 * @param [in]  pfCallback  A pointer to an application-supplied calllback function
 *                          which is called to notify the application of device-related
 *                          events.  A value of NULL disables driver callbacks.
 * @param [in]  pCBParam    An application-supplied callback parameter which will be passed
 *                          back to the callback function.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The callback is registered successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] A flash write operation is in progress and
 *                                                  the callback registration is ignored.
 *
 * Links the user-provided callback function into the #adi_fee_SubmitBuffer() API such that
 * rather than polling for buffer completion (with #adi_fee_IsBufferAvailable()) and eventually
 * reacquiring the buffer (with #adi_fee_GetBuffer()), the user can simply register a callback
 * function that will be called upon buffer completion with no further action needed.\n
 *
 * Error conditions are also passed to the callback, including DMA errors if DMA is active.  Make sure
 * to always check the event value passed to the callback, just as the various API return codes should
 * always be checked.\n
 *
 * However, callbacks are always made in context of an interrupt, so applications are strongly encouraged
 * to exit the callback as quickly as possible so normal interrupt processing is disrupted as little as
 * possible.  This is also an argument for not using callbacks at at all.
 *
 * @note When using callbacks to reacquire buffers, DO NOT use the #adi_fee_GetBuffer() API.  The two
 * methods are mutually exclusive.
 *
 * @sa adi_fee_SubmitBuffer().
 * @sa adi_fee_IsBufferAvailable().
 * @sa adi_fee_GetBuffer().
 */
ADI_FEE_RESULT adi_fee_RegisterCallback (ADI_FEE_HANDLE const hDevice, ADI_CALLBACK const pfCallback, void* const pCBParam)
{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }
#endif

    /* Set the callback function and param in the device */
    hDevice->pfCallback = pfCallback;
    hDevice->pCBParam   = pCBParam;

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Erase the given range of (2kB) page(s) within the flash user space memory.  This is a blocking call.
 *
 * @param [in]     hDevice         The handle to the flash controller device.
 * @param [in]     nPageNumStart   Start page number.
 * @param [in]     nPageNumEnd     End page number.
 * @param [in,out] pHwErrors       Pointer to user location into which any flash hardware errors are reported.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The page(s) is(are) cleared successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] The page(s) number(s) is(are) incorrect.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is in progress.
 *          - #ADI_FEE_ERR_HW_ERROR_DETECTED        An internal flash controller hardware error was detected.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *
 * Erases entire page(s).  Callers are expected to save/restore any partial page data prior
 * to erasure, as needed.  Translate literal flash addresses into flash start and end page
 * numbers with #adi_fee_GetPageNumber().
 *
 * @note Flash hardware errors are flagged with the #ADI_FEE_ERR_HW_ERROR_DETECTED return code.
 * Flash hardware error details are written to the location pointed to by the pHwErrors parameter.
 * Hardware error details may be decoded according to the flash controller status register ("STAT")
 * bit-map, documented in the Hardware Reference Manual (HRM).
 *
 * @sa adi_fee_GetPageNumber().
 * @sa adi_fee_MassErase().
 */
ADI_FEE_RESULT adi_fee_PageErase (ADI_FEE_HANDLE const hDevice, uint32_t const nPageNumStart, uint32_t const nPageNumEnd, uint32_t* const pHwErrors)

{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

    uint32_t page;

#ifdef ADI_DEBUG

    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

    uint32_t nRelAddrStart = (nPageNumStart << FEE_PAGE_SHIFT);
    uint32_t nRelAddrStop  = (nPageNumEnd   << FEE_PAGE_SHIFT);

    if (   (nPageNumStart > nPageNumEnd)
        || (nRelAddrStart >= FEE_FLASH_SIZE)
        || (nRelAddrStop  >= FEE_FLASH_SIZE))
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif /* defined (ADI_DEBUG) */

    for (page = nPageNumStart; page <= nPageNumEnd; page++)
    {
        /* Wait until not busy */
        BusyWait(hDevice, (BITM_FLCC_STAT_CMDBUSY | BITM_FLCC_STAT_WRCLOSE));

        /* Set the page address */
        hDevice->pDev->PAGE_ADDR0 = (page << FEE_PAGE_SHIFT);

        /* Issue a page erase command */
        result = SendCommand (hDevice, ENUM_FLCC_CMD_ERASEPAGE);

        /* block on command */
        SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

        if (result != ADI_FEE_SUCCESS) {
            break;
        }
    }

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->feeError;
    if (0u != hDevice->feeError) {
        /* return the HW error return code */
        return ADI_FEE_ERR_HW_ERROR_DETECTED;
    }

    return result;
}


/**
 * @brief    Erase the entire flash user space memory.  This is a blocking call.
 *
 * @param [in]     hDevice    The handle to the flash controller device.
 * @param [in,out] pHwErrors  Pointer to user location into which any flash hardware errors are reported.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The flash is cleared successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is in progress.
 *          - #ADI_FEE_ERR_HW_ERROR_DETECTED        An internal flash controller hardware error was detected.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *
 * @note Do not call mass erase on or from code that is running from flash.  Doing so will leave
 * an indeterminate machine state.
 *
 * @note Flash hardware errors are flagged with the #ADI_FEE_ERR_HW_ERROR_DETECTED return code.
 * Flash hardware error details are written to the location pointed to by the pHwErrors parameter.
 * Hardware error details may be decoded according to the flash controller status register ("STAT")
 * bit-map, documented in the Hardware Reference Manual (HRM).
 *
 * @sa adi_fee_PageErase().
 */
ADI_FEE_RESULT adi_fee_MassErase (ADI_FEE_HANDLE const hDevice, uint32_t* const pHwErrors)
{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }
#endif

    /* Call the mass erase command */
    result = SendCommand (hDevice, ENUM_FLCC_CMD_MASSERASE);

    /* block on command */
    SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->feeError;
    if (0u != hDevice->feeError) {
        /* return the HW error return code */
        return ADI_FEE_ERR_HW_ERROR_DETECTED;
    }

    return result;
}


/**
 * @brief     Perform a blocking flash data write operation.
 *
 * @param [in]  hDevice         The handle to the flash controller device.
 * @param [in]  pTransaction    Pointer to a user-defined control block describing the data to be transferred, containing:
 *                                - pWriteAddr; Pointer to a 64-bit-aligned destination address in flash.
 *                                - pWriteData; Pointer to a 32-bit-aligned source data buffer in user memory.
 *                                - nSize;      Number of bytes to write (must be an integral multiple of 8).
 *                                - bUseDma;    Flag controlling use of DMA to perform the write.
 * @param [in,out] pHwErrors    Pointer to user location into which any flash hardware errors are reported.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The buffer is successfully written to the flash.
 *          - #ADI_FEE_ERR_ALIGNMENT            [D] The flash write source data pointer is misaligned.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Buffer size is not a multiple of 8-bytes (or too large for DMA).
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *          - #ADI_FEE_ERR_BUFFER_ERR               Error occurred in processing the buffer.
 *          - #ADI_FEE_ERR_DEVICE_BUSY              The flash controller is busy.
 *          - #ADI_FEE_ERR_DMA_BUS_FAULT            A runtime DMA bus fault was detected.
 *          - #ADI_FEE_ERR_DMA_INVALID_DESCR        A runtime DMA invalid descriptor was detected.
 *          - #ADI_FEE_ERR_DMA_UNKNOWN_ERROR        An unknown runtime DMA error was detected.
 *          - #ADI_FEE_ERR_HW_ERROR_DETECTED        An internal flash controller hardware error was detected.
 *          - #ADI_FEE_ERR_NO_DATA_TO_TRANSFER      Transfer ran out of write data unexpectedly.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *
 * Perform a blocking flash data write operation.  This API does not return until the write operation is completed.
 *
 * @note Flash hardware errors are flagged with the #ADI_FEE_ERR_HW_ERROR_DETECTED return code.
 * Flash hardware error details are written to the location pointed to by the pHwErrors parameter.
 * Hardware error details may be decoded according to the flash controller status register ("STAT")
 * bit-map, documented in the Hardware Reference Manual (HRM).  Flash hardware errors are separate
 * and distinct from DMA errors, which have separate and distinct return codes, as described above.
 */
ADI_FEE_RESULT adi_fee_Write (ADI_FEE_HANDLE const hDevice, ADI_FEE_TRANSACTION* const pTransaction, uint32_t* const pHwErrors)
{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

	/* check address is 64-bit aligned and data pointer is 32-bit aligned */
    if ( (((uint32_t)pTransaction->pWriteAddr & 0x7u) != 0u) || ((((uint32_t)pTransaction->pWriteData) & 0x3u) != 0u) )
    {
        return ADI_FEE_ERR_ALIGNMENT;
    }

	/* make sure size is a multiple of 8 */
    if ((pTransaction->nSize & 0x7u) != 0u) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }

    if (true == pTransaction->bUseDma) {
        /* check for max DMA units (32-bit chunks, i.e., 4 bytes at a whack) */
        if (DMA_TRANSFER_LIMIT < (pTransaction->nSize / sizeof(uint32_t))) {
            return ADI_FEE_ERR_INVALID_PARAM;
        }
    }
#endif

    /* reset submit/get safeguard flag */
    hDevice->bSubmitCalled = false;

    /* Fill in the transfer params */
    hDevice->pNextWriteAddress = pTransaction->pWriteAddr;
    hDevice->pNextReadAddress  = pTransaction->pWriteData;
    hDevice->nRemainingBytes   = pTransaction->nSize;
    hDevice->bUseDma           = pTransaction->bUseDma;

    /* Initiate a transfer */
    result = InitiateTransfer (hDevice);

    /* Wait for the completed transfer */
    SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    /* issue any flash DMA error status codes... */
    if (0u != hDevice->dmaError) {
        return hDevice->dmaError;
    }

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->feeError;
    if (0u != hDevice->feeError) {
        /* return the HW error return code */
        return ADI_FEE_ERR_HW_ERROR_DETECTED;
    }

    /* Check for errors in buffer write */
    if (hDevice->nRemainingBytes != 0u) {
        return ADI_FEE_ERR_BUFFER_ERR;
    }

    return result;
}


/**
 * @brief     Submit a non-blocking flash data write operation for background processing.
 *
 * @param [in]  hDevice         The handle to the flash controller device.
 * @param [in]  pTransaction    Pointer to a user-defined control block describing the data to be transferred, containing:
 *                                - pWriteAddr; Pointer to a 64-bit-aligned destination address in flash.
 *                                - pWriteData; Pointer to a 32-bit-aligned source data buffer in user memory.
 *                                - nSize;      Number of bytes to write (must be an integral multiple of 8).
 *                                - bUseDma;    Flag controlling use of DMA to perform the write.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The buffer is successfully written to the flash.
 *          - #ADI_FEE_ERR_ALIGNMENT            [D] The flash write source data pointer is misaligned.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Buffer size is not a multiple of 8-bytes (or too large for DMA).
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *          - #ADI_FEE_ERR_BUFFER_ERR               Error occurred in processing the buffer.
 *          - #ADI_FEE_ERR_DEVICE_BUSY              The flash controller is busy.
 *          - #ADI_FEE_ERR_NO_DATA_TO_TRANSFER      Transfer ran out of write data unexpectedly.
 *
 * Submit a flash data write transaction. This is a non-blocking function which returns immediately.
 * The application may either: poll for transaction completion through the non-blocking #adi_fee_IsBufferAvailable()
 * API, and/or await transaction completion through the blocking mode #adi_fee_GetBuffer() API.  If an application
 * callback has been registered, the application is advised of completion status through the callback.
 *
 * @note If using callback mode, DO NOT USE the #adi_fee_GetBuffer() API, which are mutually exclusive protocols.
 *
 * @sa adi_fee_IsBufferAvailable().
 * @sa adi_fee_GetBuffer().
 */
ADI_FEE_RESULT adi_fee_SubmitBuffer (ADI_FEE_HANDLE const hDevice, ADI_FEE_TRANSACTION* const pTransaction)
{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

	/* check address is 64-bit aligned and data pointer is 32-bit aligned */
    if ( (((uint32_t)pTransaction->pWriteAddr & 0x7u) != 0u) || ((((uint32_t)pTransaction->pWriteData) & 0x3u) != 0u) )
    {
        return ADI_FEE_ERR_ALIGNMENT;
    }

	/* make sure size is a multiple of 8 */
    if ((pTransaction->nSize & 0x7u) != 0u) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }

    if (true == pTransaction->bUseDma) {
        /* check for max DMA units (32-bit channel width means 4 bytes at a whack) */
        if (DMA_TRANSFER_LIMIT < (pTransaction->nSize / sizeof(uint32_t))) {
            return ADI_FEE_ERR_INVALID_PARAM;
        }
    }
#endif

    /* set submit/get safeguard flag */
    hDevice->bSubmitCalled = true;

    /* Fill in the transfer params */
    hDevice->pNextWriteAddress = pTransaction->pWriteAddr;
    hDevice->pNextReadAddress  = pTransaction->pWriteData;
    hDevice->nRemainingBytes   = pTransaction->nSize;
    hDevice->bUseDma           = pTransaction->bUseDma;

    /* initiate a transfer */
    result = InitiateTransfer (hDevice);

    /* no pend here... just return */

    return result;
}


/**
 * @brief   Non-blocking check if a write transaction complete.
 *
 * @param [in]      hDevice           The handle to the flash controller device.
 * @param [in,out]  pbCompletionState True if transfer is complete, false if not.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The status of buffer is returned successfully.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Pointer passed is NULL.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_UNMATCHED_SUBMIT_QUERY   No matching buffer submit call found.
 *
 * Check if a non-blocking write transaction that was submitted via adi_fee_SubmitBuffer() is complete.
 *
 * @sa adi_fee_SubmitBuffer().
 * @sa adi_fee_GetBuffer().
 */
ADI_FEE_RESULT adi_fee_IsBufferAvailable (ADI_FEE_HANDLE const hDevice, bool* const pbCompletionState)

{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    if (pbCompletionState == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* fail if not a submit-based transaction */
    if (false == hDevice->bSubmitCalled) {
        return ADI_FEE_ERR_UNMATCHED_SUBMIT_QUERY;
    }

    if (true == hDevice->bTransferInProgress) {
        *pbCompletionState = false;
    } else {
        *pbCompletionState = true;
    }

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Blocking mode call to await transaction completion.
 *
 * @param [in]     hDevice    The handle to the flash controller device.
 * @param [in,out] pHwErrors  Pointer to user location into which any flash hardware errors are reported.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The buffer is successfully written to the flash.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_BUFFER_ERR               Error occurred in processing the buffer.
 *          - #ADI_FEE_ERR_DMA_BUS_FAULT            A runtime DMA bus fault was detected.
 *          - #ADI_FEE_ERR_DMA_INVALID_DESCR        A runtime DMA invalid descriptor was detected.
 *          - #ADI_FEE_ERR_DMA_UNKNOWN_ERROR        An unknown runtime DMA error was detected.
 *          - #ADI_FEE_ERR_HW_ERROR_DETECTED        An internal flash controller hardware error was detected.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *          - #ADI_FEE_ERR_UNMATCHED_SUBMIT_QUERY   No matching buffer submit call found.
 *
 * This function blocks until a previously-submitted flash write operation has completed.
 *
 * @note Flash hardware errors are flagged with the #ADI_FEE_ERR_HW_ERROR_DETECTED return code.
 * Flash hardware error details are written to the location pointed to by the pHwErrors parameter.
 * Hardware error details may be decoded according to the flash controller status register ("STAT")
 * bit-map, documented in the Hardware Reference Manual (HRM).
 *
 * @sa adi_fee_SubmitBuffer().
 * @sa adi_fee_IsBufferAvailable().
 */
ADI_FEE_RESULT adi_fee_GetBuffer (ADI_FEE_HANDLE const hDevice, uint32_t* const pHwErrors)

{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }
#endif

    /* fail if not a submit-based transaction */
    if (false == hDevice->bSubmitCalled) {
        return ADI_FEE_ERR_UNMATCHED_SUBMIT_QUERY;
    }

    /* Pend for the semaphore */
    SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    /* issue any flash DMA error status codes... */
    if (0u != hDevice->dmaError) {
        return hDevice->dmaError;
    }

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->feeError;
    if (0u != hDevice->feeError) {
        /* return the HW error return code */
        return ADI_FEE_ERR_HW_ERROR_DETECTED;
    }

    /* Check for errors in buffer write or transfer still in progress */
    if ((0u != hDevice->nRemainingBytes) || (true == hDevice->bTransferInProgress)) {
        return ADI_FEE_ERR_BUFFER_ERR;
    }

    return ADI_FEE_SUCCESS;
}


/**
 * @brief   Get the (2kB) page number within which a flash address resides.
 *
 * @param [in]      hDevice     The handle to the flash controller device.
 * @param [in]      nAddress    The flash address for which the page number is required.
 * @param [in,out]  pnPageNum   Pointer to a variable into which the page number corresponding
 *                              to the provided flash address is written.
 *
 * @return  Status
 *          - #ADI_FEE_SUCCESS                      The page number is returned successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameter(s) are invalid.
 *
 * Translates a literal flash address into a page number for use with various page-based flash operations.
 *
 * @sa adi_fee_PageErase().
 * @sa adi_fee_VerifySignature().
 * @sa adi_fee_ConfigECC().
 * @sa adi_fee_GetBlockNumber().
 *
 */
ADI_FEE_RESULT adi_fee_GetPageNumber (ADI_FEE_HANDLE const hDevice, uint32_t const nAddress, uint32_t* const pnPageNum)
{
#ifdef ADI_DEBUG

    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    if (   (pnPageNum == NULL)
        || (nAddress >= FEE_FLASH_SIZE))
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Set the page number for the given flash address */
    *pnPageNum = (nAddress >> FEE_PAGE_SHIFT);

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Get the (16kB) block number within which a flash address resides.
 *
 * @param [in]      hDevice     The handle to the flash controller device.
 * @param [in]      nAddress    The flash address for which the block number is required.
 * @param [in,out]  pnBlockNum  Pointer to a variable into which the block number corresponding
 *                              to the provided flash address is written.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The block number is returned successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameter(s) are invalid.
 *
 * Translates a literal flash address into a block number for use with setting flash write protection on a block.
 *
 * @sa adi_fee_WriteProtectBlock().
 * @sa adi_fee_GetPageNumber().
 */
ADI_FEE_RESULT adi_fee_GetBlockNumber (ADI_FEE_HANDLE const hDevice, uint32_t const nAddress, uint32_t* const pnBlockNum)
{
#ifdef ADI_DEBUG

    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    if (   (pnBlockNum == NULL)
        || (nAddress >= FEE_FLASH_SIZE))
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Set the block number */
    *pnBlockNum = (nAddress >> FEE_BLOCK_SHIFT);

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Generate the CRC signature for a range of flash data page(s).  This is a blocking call.
 *
 * @param [in]     hDevice      The handle to the flash controller device.
 * @param [in]     nStartPage   The lower page number of the signature range.
 * @param [in]     nEndPage     The upper page number of the signature range.
 * @param [in,out] pSigResult   Pointer to a variable into which the computed signature is stored.
 * @param [in,out] pHwErrors    Pointer to user location into which any flash hardware errors are reported.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The signature is verified successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM       [D] The page(s) number(s) is(are) incorrect.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] A flash write operation is in progress.
 *          - #ADI_FEE_ERR_HW_ERROR_DETECTED        An internal flash controller hardware error was detected.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *
 * Compute and return a CRC over a range of contiguous whole flash memory pages(s).  The computed CRC
 * signature may subsequently be written into the most-significant word of the region over which the
 * signature was calculated.  This is done in context of enabling bootloader enforcement of CRC signature
 * verification during system startup.  See HRM for signature storage programming requirements and
 * bootloader operation.
 *
 * @note Flash hardware errors are flagged with the #ADI_FEE_ERR_HW_ERROR_DETECTED return code.
 * Flash hardware error details are written to the location pointed to by the pHwErrors parameter.
 * Hardware error details may be decoded according to the flash controller status register ("STAT")
 * bit-map, documented in the Hardware Reference Manual (HRM).
 *
 * @sa adi_fee_GetPageNumber().
 */
ADI_FEE_RESULT adi_fee_VerifySignature (ADI_FEE_HANDLE const hDevice, uint32_t const nStartPage, uint32_t const nEndPage, uint32_t* const pSigResult, uint32_t* const pHwErrors)

{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

    if (   (pSigResult == NULL)
        || (nStartPage >  nEndPage)
        || (nStartPage >= FEE_MAX_NUM_PAGES)
        || (nEndPage >= FEE_MAX_NUM_PAGES)
        )
    {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Wait until not busy */
    BusyWait (hDevice, (BITM_FLCC_STAT_CMDBUSY | BITM_FLCC_STAT_WRCLOSE));

    /* Set the lower and upper page */
    hDevice->pDev->PAGE_ADDR0 = nStartPage << FEE_PAGE_SHIFT;
    hDevice->pDev->PAGE_ADDR1 = nEndPage << FEE_PAGE_SHIFT;

    /* Do a SIGN command */
    result = SendCommand(hDevice, ENUM_FLCC_CMD_SIGN);

    /* block on command */
    SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    /* Return the signature to the application */
    if (ADI_FEE_SUCCESS == result) {
        *pSigResult = hDevice->pDev->SIGNATURE;
    } else {
        *pSigResult = 0u;
    }

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->feeError;
    if (0u != hDevice->feeError) {
        /* return the HW error return code */
        return ADI_FEE_ERR_HW_ERROR_DETECTED;
    }

    return result;
}


/**
 * @brief    Set write protection on an (16kB) block.
 *
 * @param [in]  hDevice     The handle to the flash controller device.
 * @param [in]  nBlockNum   The block number.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The block is write protected successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Block number is invalid.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *
 * Assert memory write-protection for specified block.  Note that only entire blocks are protectable,
 * with each block spanning 8 pages.
 *
 * @note Blocks may only be write-protected during user run-time code.  Unprotecting is only
 * possible with a power-on-reset or a mass erase; write-protection is not otherwise clearable.
 *
 * @warning Flash-based code that write-protects blocks will cause the write-protection (and data at
 * time of write-protect assertion) to apparently not clear... even after a mass erase or power-on-reset.
 * This apparently "stuck" write-protection results from the flash-based write-protect code running
 * after reset (as usual), but still prior to the debugger halting the target through the debug
 * interrupt.  The debugger target halt occurs WELL AFTER the flash code has already run, thereby
 * relocking the block and making it appear the write-protection was never reset.  This can be difficult
 * Catch-22 situation to recover from, requiring repeated hardware resets and reflashing new code that
 * does not assert the write-protection.
 *
 * @sa adi_fee_GetBlockNumber().
 */
ADI_FEE_RESULT adi_fee_WriteProtectBlock (ADI_FEE_HANDLE const hDevice, uint32_t const nBlockNum)

{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

    if (nBlockNum > FEE_MAX_NUM_BLOCKS) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Set the write protection (by clearing the bit) for the given block */
    hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
    CLR_BITS (hDevice->pDev->WRPROT, 1u << nBlockNum);
    hDevice->pDev->KEY = 0u;

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Sleep or awake the flash controller.  This is a blocking call.
 *
 * @param [in]  hDevice   The handle to the flash controller device.
 * @param [in]  bSleep    'true' to enable to sleep the flash device
 *                        and 'false' to wake up the device.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The flash controller is moved to sleep/wake
 *                                                  up sate successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *
 * Places the flash controller into a low-power sleep mode - see details in Hardware Reference Manual (HRM).
 * Default wakeup time is approximately 5us, and is configurable with static configuration parameter
 * ADI_FEE_CFG_PARAM1_TWK in adi_flash_config.h file.
 */
ADI_FEE_RESULT adi_fee_Sleep (ADI_FEE_HANDLE const hDevice, bool const bSleep)
{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }
#endif

    /* TODO: Check that IDLE can take the controller
     *       out of sleep
     */

    if (true == bSleep) {
        result = SendCommand (hDevice, ENUM_FLCC_CMD_SLEEP);
    } else {
        result = SendCommand (hDevice, ENUM_FLCC_CMD_IDLE);
    }

    /* block on command */
    SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    return result;
}


/**
 * @brief    Forcefully ABORT an ongoing flash operation.  This is a blocking call.
 *
 * @param [in]  hDevice   The handle to the flash controller device.
 *
 * @return   Statuus
 *          - #ADI_FEE_SUCCESS                      The command is successfully aborted.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_SEMAPHORE_FAILED         The semaphore pend operation failed.
 *
 * @warning Use this command sparingly and as a last resort to satisfy critical
 * time-sensitive events.  Aborting any flash command results in prematurely ending the
 * current flash access and may result in corrupted flash data.
 *
 * @sa adi_fee_GetAbortAddr().
 */
ADI_FEE_RESULT adi_fee_Abort (ADI_FEE_HANDLE const hDevice)

{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }
#endif
    /* Issue the command (abort is keyed) directly */
    /* (avoid SendCommand() here, as it does a busy wait, which may not clear if we're in a recovery mode) */
    hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
    hDevice->pDev->CMD = ENUM_FLCC_CMD_ABORT;
    hDevice->pDev->KEY = 0u;

    SEM_PEND(hDevice, ADI_FEE_ERR_SEMAPHORE_FAILED);

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Get the address of recently aborted write command.
 *
 * @param [in]     hDevice     The handle to the flash controller device.
 * @param [in,out] pnAddress   Pointer to which the address is written.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The abort address is retrieved successfully
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Pointer passed is NULL
 *
 * Users may use this result to determine the flash location(s) affected by a write abort command.
 * Subsequent flash commands invalidate the write abort address register.
 *
 *
 * @sa adi_fee_Abort().
 */
ADI_FEE_RESULT adi_fee_GetAbortAddr (ADI_FEE_HANDLE const hDevice, uint32_t* const pnAddress)
{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    if (pnAddress == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Write the address of the last write abort to the pointer
     * supplied by the application
     */
    *pnAddress = hDevice->pDev->WR_ABORT_ADDR;

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Configure ECC start page and enablement.
 *
 * @param [in]  hDevice         The handle to the flash controller device.
 * @param [in]  nStartPage      The start page for which ECC will be performed.
 * @param [in]  bInfoECCEnable  Info space ECC enable:
 *                                  - 'true' to enable info space ECC, or
 *                                  - 'false' to disable info space ECC.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC was configured successfully
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Start page is invalid
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *
 * @note The settings this API manages are very likely not needed to be modified dynamically (at run-time).
 * If so, consider using the static configuration equivalents (see adi_flash_config.h) in lieu of
 * this API... which will reduce the resulting code image footprint through linker elimination.
 *
 * @warning This API leaves user space ECC disabled.  Use #adi_fee_EnableECC() to manage ECC enable/disable.
 *
 * @sa adi_fee_EnableECC().
 * @sa adi_fee_ConfigECCEvents().
 * @sa adi_fee_GetECCErrAddr().
 * @sa adi_fee_GetECCCorrections().
 */
ADI_FEE_RESULT adi_fee_ConfigECC (ADI_FEE_HANDLE const hDevice, uint32_t const nStartPage, bool const bInfoECCEnable)
{
    uint32_t nRelAddress = nStartPage << FEE_PAGE_SHIFT;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }

    if (nStartPage >= FEE_MAX_NUM_PAGES) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Clear the ECC config bits */
    CLR_BITS (hDevice->pDev->ECC_CFG, (BITM_FLCC_ECC_CFG_PTR | BITM_FLCC_ECC_CFG_INFOEN));

    /* Set the start page address in the ECC Cfg register */
    hDevice->pDev->ECC_CFG |= (nRelAddress & BITM_FLCC_ECC_CFG_PTR);

    /* enable ECC on info space... if requested */
    if (true == bInfoECCEnable) {
        SET_BITS (hDevice->pDev->ECC_CFG, BITM_FLCC_ECC_CFG_INFOEN);
    }

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Enable/Disable user space ECC for the device.
 *
 * @param [in]  hDevice  The handle to the flash controller device.
 * @param [in]  bEnable  User space ECC enable:
 *                           - 'true' to enable user space ECC, or
 *                           - 'false' to disable user space ECC.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC is enabled/disabled successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *
 * Manage enablement of user space ECC function.
 *
 * @note The settings this API manages are very likely not needed to be modified dynamically (at run-time).
 * If so, consider using the static configuration equivalents (see adi_flash_config.h) in lieu of
 * this API... which will reduce the resulting code image footprint through linker elimination.
 *
 * @sa adi_fee_ConfigECC().
 * @sa adi_fee_ConfigECCEvents().
 * @sa adi_fee_GetECCErrAddr().
 * @sa adi_fee_GetECCCorrections().
 */
ADI_FEE_RESULT adi_fee_EnableECC (ADI_FEE_HANDLE const hDevice, bool const bEnable)
{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
    }
#endif

    /* manage flash ECC enable */
    if (true == bEnable) {
        SET_BITS(hDevice->pDev->ECC_CFG, BITM_FLCC_ECC_CFG_EN);
    } else {
        CLR_BITS(hDevice->pDev->ECC_CFG, BITM_FLCC_ECC_CFG_EN);
    }

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Confifure ECC event response.
 *
 * @param [in]  hDevice     The handle to the flash controller device.
 * @param [in]  eEvent      ECC event - Either error or correction event.
 * @param [in]  eResponse   The response to the eEvent - One of none, bus error, or interrupt.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC events are configured successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameters are invalid.
 *          - #ADI_FEE_ERR_TRANSFER_IN_PROGRESS [D] Another transfer is already in progress.
 *
 * Configures two major aspects of ECC event response:
 *     - On ECC (2-bit) Error events, generate one of: no response, bus error, or flash interrupt.
 *     - On ECC (1-bit) Correction events, generate one of: no response, bus error, or flash interrupt.
 *
 * @note The settings this API manages are very likely not needed to be modified dynamically (at run-time).
 * If so, consider using the static configuration equivalents (see adi_flash_config.h) in lieu of
 * this API... which will reduce the resulting code image footprint through linker elimination.
 *
 * @sa adi_fee_ConfigECC().
 * @sa adi_fee_EnableECC().
 * @sa adi_fee_GetECCErrAddr().
 * @sa adi_fee_GetECCCorrections().
 */
ADI_FEE_RESULT adi_fee_ConfigECCEvents (ADI_FEE_HANDLE const hDevice, ADI_FEE_ECC_EVENT_TYPE const eEvent, ADI_FEE_ECC_RESPONSE const eResponse)

{
    uint32_t nBitMask;
    int32_t nBitPos;

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    /* reject while a transfer is in progress */
    if (true == hDevice->bTransferInProgress) {
        return ADI_FEE_ERR_TRANSFER_IN_PROGRESS;
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

    /* Select the correct bit mask and bit pos for the event type */
    if (eEvent == ADI_FEE_ECC_EVENT_TYPE_ERROR) {
        nBitMask = BITM_FLCC_IEN_ECC_ERROR;
        nBitPos  = BITP_FLCC_IEN_ECC_ERROR;
    } else {
        nBitMask = BITM_FLCC_IEN_ECC_CORRECT;
        nBitPos  = BITP_FLCC_IEN_ECC_CORRECT;
    }

    /* clear the bits */
    CLR_BITS (hDevice->pDev->IEN, nBitMask);

    /* set the response */
       SET_BITS (hDevice->pDev->IEN, ((uint32_t)eResponse) << nBitPos);

    return ADI_FEE_SUCCESS;
}


/**
 * `@brief    Get the address for which the ECC event is detected.
 *
 * @param [in]      hDevice     The handle to the flash controller device.
 * @param [in,out]  pnAddress   Pointer to which the address is written.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The ECC error address is retrieved successfully.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameters are invalid.
 *
 * Returns the address of the first ECC error or correction event to generate an
 * interrupt since the last time ECC status bits were cleared (or since reset).
 *
 * @sa adi_fee_ConfigECC().
 * @sa adi_fee_EnableECC().
 * @sa adi_fee_ConfigECCEvents().
 * @sa adi_fee_GetECCCorrections().
 */
ADI_FEE_RESULT adi_fee_GetECCErrAddr (ADI_FEE_HANDLE const hDevice, uint32_t* const pnAddress)

{
#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    if (pnAddress == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Write the address of the last ECC error/correction */
    *pnAddress = hDevice->pDev->ECC_ADDR;

    return ADI_FEE_SUCCESS;
}


/**
 * @brief    Get the number of 1-bit error corrections.
 *
 * @param [in]      hDevice            The handle to the flash controller device.
 * @param [in,out]  pnNumCorrections   Pointer to which the number of corrections are written.
 *
 * @return   Status
 *          - #ADI_FEE_SUCCESS                      The number of ECC corrections are successfully retrieved.
 *          - #ADI_FEE_ERR_INVALID_HANDLE       [D] The device handle passed is invalid.
 *          - #ADI_FEE_ERR_INVALID_PARAM        [D] Parameters are invalid.
 *
 * See HRM for details on how current ECC configuration affects this reporting.
 *
 * @sa adi_fee_ConfigECC().
 * @sa adi_fee_EnableECC().
 * @sa adi_fee_ConfigECCEvents().
 * @sa adi_fee_GetECCErrAddr().
 */
ADI_FEE_RESULT adi_fee_GetECCCorrections (ADI_FEE_HANDLE const hDevice, uint32_t* const pnNumCorrections)
{

#ifdef ADI_DEBUG
    if (true != IsDeviceHandle(hDevice)) {
        return ADI_FEE_ERR_INVALID_HANDLE;
    }

    if (pnNumCorrections == NULL) {
        return ADI_FEE_ERR_INVALID_PARAM;
    }
#endif

    /* Get the number of ECC Error corrections */
    *pnNumCorrections = (hDevice->pDev->STAT & BITM_FLCC_STAT_ECCERRCNT) >> BITP_FLCC_STAT_ECCERRCNT;

    return ADI_FEE_SUCCESS;
}


/*========  L O C A L    F U N C T I O N    D E F I N I T I O N S  ========*/


/* Send a command to the flash controller... bot don't block on it...
 */
static ADI_FEE_RESULT SendCommand (ADI_FEE_HANDLE const hDevice, uint32_t const cmd)
{
    /* Wait for the flash to be free */
    BusyWait (hDevice, (BITM_FLCC_STAT_CMDBUSY | BITM_FLCC_STAT_WRCLOSE));

    /* Clear the command completion status bit
     * by acknowledging it
     */
    hDevice->pDev->STAT = BITM_FLCC_STAT_CMDCOMP;

    /* Enable command-complete and command-fail interrupt */
    SET_BITS(hDevice->pDev->IEN, (BITM_FLCC_IEN_CMDCMPLT | BITM_FLCC_IEN_CMDFAIL));

    /* Issue the command (most commands are keyed) */
    hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
    hDevice->pDev->CMD = cmd;
    hDevice->pDev->KEY = 0u;

    return ADI_FEE_SUCCESS;
}


static ADI_FEE_RESULT InitiatePioTransfer (ADI_FEE_HANDLE const hDevice)
{

    /* use PIO interrupt mode in non-burst-mode (burst-mode only spans 256-bytes).
       Initiate the 1st write here, then let the interrupt handler feed
       the remaining data as we process "almost-complete" interrupts.
    */

    /* write the 1st 64-bits of data */
    if (0u != hDevice->nRemainingBytes) {

        /* enable command interrupts */
        SET_BITS (hDevice->pDev->IEN, (BITM_FLCC_IEN_WRALCMPLT | BITM_FLCC_IEN_CMDCMPLT | BITM_FLCC_IEN_CMDFAIL));

        /* set initial write address*/
        hDevice->pDev->KH_ADDR = (uint32_t)hDevice->pNextWriteAddress;
        hDevice->pNextWriteAddress += 2;

        /* set key-hole data registers */
        hDevice->pDev->KH_DATA0 = *hDevice->pNextReadAddress;
        hDevice->pNextReadAddress++;
        hDevice->pDev->KH_DATA1 = *hDevice->pNextReadAddress;
        hDevice->pNextReadAddress++;
        hDevice->nRemainingBytes -= sizeof(uint64_t);

        /* write the command register which launches the burst write */
        hDevice->pDev->CMD = ENUM_FLCC_CMD_WRITE;

    } else {
        return ADI_FEE_ERR_NO_DATA_TO_TRANSFER;
    }

    return ADI_FEE_SUCCESS;
}


/* DMA Transfer to FIFO */
static ADI_FEE_RESULT InitiateDmaTransfer (ADI_FEE_HANDLE const hDevice)
{
    ADI_DCC_TypeDef* pCCD = pPrimaryCCD;  /* pointer to primary DMA descriptor array */

    if (0u != hDevice->nRemainingBytes) {

        /* local channel number */
        uint16_t chan = hDevice->pDevInfo->dmaChanNum;

        /* disable endpointer decrement modes */
        pADI_DMA0->SRCADDR_CLR =  1u << chan;
        pADI_DMA0->DSTADDR_CLR =  1u << chan;

        /* enable the channel */
        pADI_DMA0->EN_SET = 1u << chan;

        /* allow flash to request DMA service */
        pADI_DMA0->RMSK_CLR = 1u << chan;

        /* activate primary descriptor */
        pADI_DMA0->ALT_CLR = 1u << chan;

        /* Note: DMA width is 32-bit for the flash controller, but flash writes require
           64-bit writes at a whack.  Set DMA R_Power (bus rearbitration rate) to two so
           we get two uninterrupted 32-bit DMA writes to the flash with each DMA transfer.
        */

        /* set DMA source endpoint */
        pCCD += chan;  /* offset descriptor pointer to flash channel */
        pCCD->DMASRCEND = (uint32_t)hDevice->pNextReadAddress + hDevice->nRemainingBytes - sizeof(uint32_t);

        /* set DMA destination endpoint (no increment) */
        pCCD->DMADSTEND = (uint32_t)&hDevice->pDev->KH_DATA1;

        /* set the initial write address */
        hDevice->pDev->KH_ADDR = (uint32_t)hDevice->pNextWriteAddress;

        /* set the DMA Control Data Configuration register */
        pCCD->DMACDC =
            ( ((uint32_t)ADI_DMA_INCR_NONE                                << DMA_BITP_CTL_DST_INC)
            | ((uint32_t)ADI_DMA_INCR_4_BYTE                              << DMA_BITP_CTL_SRC_INC)
            | ((uint32_t)ADI_DMA_WIDTH_4_BYTE                             << DMA_BITP_CTL_SRC_SIZE)
            | ((uint32_t)ADI_DMA_RPOWER_2                                 << DMA_BITP_CTL_R_POWER)
            | (uint32_t)((hDevice->nRemainingBytes/sizeof(uint32_t) - 1u) << DMA_BITP_CTL_N_MINUS_1)
            | ((uint32_t)DMA_ENUM_CTL_CYCLE_CTL_BASIC                     << DMA_BITP_CTL_CYCLE_CTL) );

        /* set auto-increment and DMA enable bits, launching transder */
        hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
        SET_BITS (hDevice->pDev->UCFG, (BITM_FLCC_UCFG_AUTOINCEN | BITM_FLCC_UCFG_KHDMAEN));
        hDevice->pDev->KEY = 0u;

    } else {
        return ADI_FEE_ERR_NO_DATA_TO_TRANSFER;
    }

    return ADI_FEE_SUCCESS;
}


/* Initiate transfer */
static ADI_FEE_RESULT InitiateTransfer (ADI_FEE_HANDLE const hDevice)
{
    ADI_FEE_RESULT result = ADI_FEE_SUCCESS;

    /* If a transfer is in progress or if the pending buffers are empty
     * the return as there is nothing to be done now
     */
    if (true == hDevice->bTransferInProgress)
    {
        return ADI_FEE_ERR_DEVICE_BUSY;
    }

    /* Wait for the flash to not be busy */
    BusyWait (hDevice, BITM_FLCC_STAT_CMDBUSY);

    /* clear internal errors */
    hDevice->feeError = 0u;
    hDevice->dmaError = ADI_FEE_SUCCESS;

    /* Set the bool variable to signify that a transfer is in progress */
    hDevice->bTransferInProgress = true;

    /* clear any command interrupt enables */
    CLR_BITS(hDevice->pDev->IEN,  (BITM_FLCC_IEN_WRALCMPLT | BITM_FLCC_IEN_CMDCMPLT | BITM_FLCC_IEN_CMDFAIL));

    /* clear any dangeling command-related status */
    hDevice->pDev->STAT = BITM_FLCC_STAT_WRALCOMP | BITM_FLCC_STAT_CMDCOMP | BITM_FLCC_STAT_CMDFAIL;

    /* clear auto-increment and dma enable bits */
    hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
    CLR_BITS (hDevice->pDev->UCFG, (BITM_FLCC_UCFG_AUTOINCEN | BITM_FLCC_UCFG_KHDMAEN));
    hDevice->pDev->KEY = 0u;

    /* Call the corresponding Transfer functions */
    if (true == hDevice->bUseDma) {
        result = InitiateDmaTransfer(hDevice);
    } else {
        result = InitiatePioTransfer(hDevice);
    }

    return result;
}


/* hide the interrupt handlers from DoxyGen */
/*! \cond PRIVATE */

/* Flash PIO interrupt handler */
void Flash0_Int_Handler(void)
{
    ISR_PROLOG();

    /* post flag */
    bool bPost = false;
    bool bError = false;

    /* recover the driver handle */
    ADI_FEE_HANDLE hDevice = fee_device_info[0].hDevice;

#ifdef ADI_DEBUG
    /* Return if the device is not opened - spurious interrupts */
    if (hDevice == NULL) {
        return;
    }
#endif

    /* update status cache and clear it right away on the controller */
    hDevice->FlashStatusCopy = hDevice->pDev->STAT;
    hDevice->pDev->STAT = hDevice->FlashStatusCopy;

    /* check for flash device errors */
    hDevice->feeError = (ADI_FEE_STATUS_ERROR_MASK & hDevice->FlashStatusCopy);
    if (0u != hDevice->feeError) {
        bError = true;
    }

    /* if no errors */
    if (false == bError) {

        if (0u != (BITM_FLCC_STAT_WRALCOMP & hDevice->FlashStatusCopy)) {

            /* write-almost-complete */

            /* if more data to write... */
            if (0u != hDevice->nRemainingBytes) {

                /* set next write the address */
                hDevice->pDev->KH_ADDR = (uint32_t)hDevice->pNextWriteAddress;
                hDevice->pNextWriteAddress += 2;

                /* set next key-hole data */
                hDevice->pDev->KH_DATA0 = *hDevice->pNextReadAddress;
                hDevice->pNextReadAddress++;
                hDevice->pDev->KH_DATA1 = *hDevice->pNextReadAddress;
                hDevice->pNextReadAddress++;
                hDevice->nRemainingBytes -= sizeof(uint64_t);

                /* initiate next write */
                hDevice->pDev->CMD = ENUM_FLCC_CMD_WRITE;

            } else {

                /* no more data to write...
                   wait for current write-almost-complete status to transition to not busy */
                BusyWait (hDevice, BITM_FLCC_STAT_CMDBUSY);

                /* set post flag */
                bPost = true;
            }

        } else if (0u != (BITM_FLCC_STAT_CMDCOMP & hDevice->FlashStatusCopy)) {

            /* command-complete */

            /* this path is for blocking-mode commands (erase, verify, abort, etc.) */

            /* set post flag */
            bPost = true;

        } else {
            /* no other interrupt types expected */
        }
    } else {
        /* error(s) detected... set the post flag */
        bPost = true;
    }

    /* singular post */
    if (true == bPost) {

        /* clear the command interrupt enables */
        CLR_BITS(hDevice->pDev->IEN,  (BITM_FLCC_IEN_WRALCMPLT | BITM_FLCC_IEN_CMDCMPLT | BITM_FLCC_IEN_CMDFAIL));

        /* clear auto-increment and dma enable bits */
        hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
        CLR_BITS (hDevice->pDev->UCFG, (BITM_FLCC_UCFG_AUTOINCEN | BITM_FLCC_UCFG_KHDMAEN));
        hDevice->pDev->KEY = 0u;

        /* mark transfer complete */
        hDevice->bTransferInProgress = false;

        /* dispatch callback (if we have one...) */
        if (0u != hDevice->pfCallback) {
            if (false == bError) {
                /* no error, pass success flag to callback */
                hDevice->pfCallback (hDevice->pCBParam, (uint32_t)ADI_FEE_CALLBACK_EVENT_BUFFER_PROCESSED, (void*)NULL);
            } else {
                /* error condition, pass error flag and error status to callback */
                hDevice->pfCallback (hDevice->pCBParam, (uint32_t)ADI_FEE_CALLBACK_EVENT_DEVICE_ERROR, (void*)hDevice->feeError);
            }
        }

        /* post the semaphore */
        SEM_POST(hDevice);
    }

    ISR_EPILOG();
}


/* Flash DMA interrupt handler */
void DMA_FLASH0_Int_Handler (void)
{
    /* rtos prologue */
    ISR_PROLOG()
        ;

    /* recover the driver handle */
    ADI_FEE_HANDLE hDevice = fee_device_info[0].hDevice;

    /* update status cache and clear it right away on the controller */
    hDevice->FlashStatusCopy = hDevice->pDev->STAT;
    hDevice->pDev->STAT = hDevice->FlashStatusCopy;

    /* capture any hw error status */
    hDevice->feeError = (ADI_FEE_STATUS_ERROR_MASK & hDevice->FlashStatusCopy);

    /* clear auto-increment and dma enable bits */
    hDevice->pDev->KEY = ENUM_FLCC_KEY_USERKEY;
    CLR_BITS (hDevice->pDev->UCFG, (BITM_FLCC_UCFG_AUTOINCEN | BITM_FLCC_UCFG_KHDMAEN));
    hDevice->pDev->KEY = 0u;

    /* clear the remaining count, as it should all have gone in one swoop */
    hDevice->nRemainingBytes = 0u;

    /* mark transfer complete */
    hDevice->bTransferInProgress = false;

    /* dispatch callback (if we have one...) */
    if (0u != hDevice->pfCallback) {

        /* no errors, notify success */
        if ((0u == hDevice->feeError) && (0u == hDevice->dmaError)) {
            hDevice->pfCallback (hDevice->pCBParam, (uint32_t)ADI_FEE_CALLBACK_EVENT_BUFFER_PROCESSED, (void*)NULL);

        /* flash hardware error */
        } else if (0u == hDevice->feeError) {
            hDevice->pfCallback (hDevice->pCBParam, (uint32_t)ADI_FEE_CALLBACK_EVENT_DEVICE_ERROR, (void*)hDevice->feeError);

        /* flash dma error */
        } else if (0u == hDevice->dmaError) {
            /* DMA error */
            hDevice->pfCallback (hDevice->pCBParam, (uint32_t)hDevice->dmaError, NULL);
        } else {
            /* no other cases... */
        }
    }

    /* post the semaphore */
    SEM_POST(hDevice);

    ISR_EPILOG();
}

/*! \endcond */
/*@}*/
