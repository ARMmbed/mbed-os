/*! *****************************************************************************
 * @file:    adi_i2c.c
 * @brief:   I2C  device  driver global file.
 * @details: This a global file which includes a specific file based on the processor family.
 *           This file contains the I2C device driver functions.
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

/** @addtogroup I2C_Driver I2C Driver
 *  @{
 * @brief Inter-Integrated Circuit (I2C) Driver
 * @details The I2C Master device driver manages the on-chip I2C hardware to
 * control the external two-wire I2C Bus interface, allowing communication with
 * multiple I2C slave devices through the I2C slave device addressing scheme.
 * @note The application must include drivers/i2c/adi_i2c.h to use this driver
 */

 /*! \cond PRIVATE */
#include <adi_processor.h>
#include <assert.h>
#include <string.h>     /* for "memset" */
/*! \endcond */

#include <drivers/general/adi_drivers_general.h>
#include <drivers/i2c/adi_i2c.h>

 /*! \cond PRIVATE */

#include <drivers/pwr/adi_pwr.h>

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
*
* Pm011 (rule 6.3): Types which specify sign and size should be used
*   We use bool which is accepted by MISRA but the toolchain does not accept it
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   It is used in the _data.h file which isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm088 (rule 17.4): pointer arithmetic should not be used.
*   Relying on pointer arithmetic for buffer handling.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/

#pragma diag_suppress=Pm011,Pm123,Pm073,Pm143,Pm088,Pm140
#endif /* __ICCARM__ */

/* pull in internal data structures */
#include "adi_i2c_data.c"


/* handy type-safe zero */
uint16_t uZero16 = 0u;

/* central busy checker */
#define I2C_BUSY (uZero16 != ((hDevice->pDev->MSTAT) & (uint16_t)(BITM_I2C_MSTAT_MBUSY | BITM_I2C_MSTAT_LINEBUSY)))

/*!
 * Read/write bit.
 */
 #define READ_NOT_WRITE (1u)

/* Override "weak" default binding in startup.c */
/*! \cond PRIVATE */
extern void I2C0_Master_Int_Handler(void);
/*! \endcond */

#if defined(ADI_DEBUG)
/*
 * Verifies a pointer to a driver points to one of the driver
 * struct's internal to this file.
 */
static bool IsDeviceHandle(ADI_I2C_HANDLE const hDevice);
static bool IsDeviceHandle(ADI_I2C_HANDLE const hDevice)
{
    if ((i2c_device_info[0].hDevice != (hDevice)) && ((hDevice)->pDevInfo->hDevice != NULL)) {
        return true;
    } else {
        return false;
    }
}
#endif


/*! \endcond */


/**********************************************************************************\
|**********************************USER INTERFACE**********************************|
\**********************************************************************************/


/*!
 * @brief  Initialize and allocate an I2C device for use in Master Mode.
 *
 * @param[in]   DeviceNum    Zero-based device index designating the I2C device to initialize.
 *
 * @param [in]  pMemory      Pointer to a 32-bit aligned buffer of size ADI_I2C_MEMORY_SIZE
 *                           required by the driver for the operation of specified I2C device.
 *
 * @param [in]  MemorySize   Size of the buffer to which "pMemory" points.
 *
 * @param[out]  phDevice     The caller's device handle pointer for storing the initialized
 *                           device instance data pointer.
 *
 * @return            Status
 *                    - #ADI_I2C_SUCCESS                    Call completed successfully.
 *                    - #ADI_I2C_BAD_DEVICE_NUMBER [D]      Invalid device index.
 *                    - #ADI_I2C_DEVICE_IN_USE [D]          Device is already opened.
 *                    - #ADI_I2C_INSUFFICIENT_MEMORY [D]    Device memory is not sufficient.
 *
 * Initialize an I2C device using default user configuration settings (from adi_i2c_config.h)
 * and allocate the device for use.  Device is opened in Master mode only.
 *
 * No other I2C APIs may be called until the device open function is called.  The returned
 * device handle is required to be passed to all subsequent I2C API calls to identify the
 * physical device instance to use.  The user device handle (pointed to by phDevice) is set
 * to NULL on failure.
 *
 * @note Currently, only a singular I2C physical device instance (device ID "0") exists.
 *
 * @sa      adi_spi_Close().
 */
ADI_I2C_RESULT adi_i2c_Open (uint32_t const DeviceNum, void* const pMemory, uint32_t const MemorySize, ADI_I2C_HANDLE* const phDevice) {

    /* make a device handle out of the user memory */
    ADI_I2C_HANDLE hDevice = (ADI_I2C_HANDLE)pMemory;

#if defined(ADI_DEBUG)
    /* check requested device number */
    if (DeviceNum >= (uint32_t)ADI_I2C_NUM_INSTANCES) {
        return ADI_I2C_BAD_DEVICE_NUMBER;
    }

    /* verify device is not already open */
    if (i2c_device_info[DeviceNum].hDevice != NULL) {
        return ADI_I2C_DEVICE_IN_USE;
    }

    /* verify memory size macro value */
    assert(ADI_I2C_MEMORY_SIZE == sizeof(ADI_I2C_DEV_DATA_TYPE));

    /* verify user-provided memory meets requirement */
    if ((NULL == pMemory) || (MemorySize < (uint32_t)ADI_I2C_MEMORY_SIZE)) {
        return ADI_I2C_INSUFFICIENT_MEMORY;
    }
#endif

    /* store a bad handle in case of failure */
    *phDevice = NULL;

    /*
     * Link user memory (handle) to ADI_I2C_DEVICE_INFO data structure.
     *
     *       ADI_I2C_DEVICE_INFO  <==>   ADI_I2C_HANDLE
     *
     * Clear the ADI_I2C_HANDLE memory. This also sets all bool
     * structure members to false so we do not need to waste cycles
     * setting these explicitly (e.g. hDevice->bRepearStart = false)
     */
    i2c_device_info[DeviceNum].hDevice = (ADI_I2C_DEV_DATA_TYPE *)pMemory;
    memset(pMemory, 0, MemorySize);

    /* also link device handle within __ADI_I2C_DEV_DATA_TYPE data structure */
    hDevice->pDevInfo = &i2c_device_info[DeviceNum];
    /*
     * Although the ADI_I2C_DEVICE_INFO struct has the physical device pointer
     * for this instance, copying it to the ADI_I2C_HANDLE struct (in user memory)
     * will minimize the runtime footprint and cycle count when accessing the I2C
     * registers.
     */
    hDevice->pDev = i2c_device_info[DeviceNum].pDev;

    /* store a pointer to user's static configuration settings */
    hDevice->pDevInfo->pConfig = (ADI_I2C_CONFIG*)&gConfigInfo[DeviceNum];

    /* create the semaphore */
    SEM_CREATE(hDevice, "i2c_sem", ADI_I2C_SEMAPHORE_FAILED)
        ;

    /* reset the driver and HW state */
    ADI_I2C_RESULT ignore ADI_UNUSED_ATTRIBUTE = i2cReset(hDevice);

    /* store device handle into user handle */
    *phDevice = (ADI_I2C_HANDLE)hDevice;

    return ADI_I2C_SUCCESS;
}


/*!
 * @brief  Uninitialize and deallocate an I2C device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_spi_Open().
 *
 * @return      Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *
 * Uninitialize and release an allocated I2C device, and memory associated with it
 * for other use.
 *
 * @note The user memory is released from use by the I2C driver, but is not freed.
 *
 * @sa  adi_spi_Open().
 */
ADI_I2C_RESULT adi_i2c_Close (ADI_I2C_HANDLE const hDevice) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
#endif

    /* destroy semaphore */
    SEM_DELETE(hDevice,ADI_I2C_SEMAPHORE_FAILED)
        ;

    /* reset the driver and HW state */
    ADI_I2C_RESULT ignore ADI_UNUSED_ATTRIBUTE = i2cReset(hDevice);

    /* stub handle */
    hDevice->pDevInfo->hDevice = NULL;

    return ADI_I2C_SUCCESS;
}


/*!
 * @brief  Blocking I2C Master-Mode data read/write API.
 *
 * @param[in]    hDevice        Device handle obtained from adi_spi_Open().
 * @param[in]    pTransaction   Pointer to I2C transaction data struct.
 * @param[out]   pHwErrors      Pointer to hardware error return variable.
 *
 * @return       Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_DEVICE_IN_USE [D]      An I2C transaction is already underway.
 *                - #ADI_I2C_INVALID_PARAMETER [D]  Invalid data pointer or count is detected.
 *                - #ADI_I2C_HW_ERROR_DETECTED      A hardware error occurred, check \a pHwErrors.
 *
 * Request a blocking I2C data transfer (read or write, not both as I2C is unidirectional bus)
 * with or without preceding prologue transmitted.  Control is not returned to the calling
 * application until the transfer is complete.  Buffer allocations are made by the calling code
 * (the application).
 *
 * The optional prologue (if present) and MANDATORY transaction data pointers are used to read or
 * write data over the I2C serial bus according to the prologue and data pointers and corresponding
 * size information contained in the \a pTransaction parameter block.  The most recently set slave
 * target address (set statically with user configuration settings contained in adi_i2c_config.h file
 * or set dynamically (at run-time) via the #adi_i2c_SetSlaveAddress() API) is used to address the
 * specific destination slave device on the I2C bus.
 *
 * If present, the prologue (typically, an addressing phase conveying a memory/register address or
 * slave device command) is transmitted prior to the data read or write phase, with or without
 * an intervening I2C STOP condition.  The prologue data is entirely slave device dependent.
 *
 * In the case of a prologue followed by a data read operation, the I2C bus direction must be
 * reversed following the prologue transmit.  In this case, The usual I2C STOP condition following
 * the prologue (if present) transmit may be suppressed by setting the \a bRepeatStart transaction
 * parameter "true".  In this case, a second (repeat) START condition is "transmitted" between the
 * addressing phase (prologue transmit) and the data phase of the read sequence... \a without an
 * intervening STOP condition.  This is commonly referred to as the "combined format" in which the
 * I2C bus direction is reversed halfway through the transaction without releasing control of the
 * I2C bus arbitration.  The REPEAT-START condition is a common I2C bus protocol required by many
 * I2C slave devices.
 *
 * In the case of a prologue followed by a data write operation, there is no need to turn the bus
 * around and so the \a bRepeatStart parameter is ignored.
 *
 * @note Application must check the return code to verify if any I2C Bus errors occurred.  Hardware
 * errors (I2C Protocol errors) are indicated with the #ADI_I2C_HW_ERROR_DETECTED return code, and
 * the set of hardware errors (enum #ADI_I2C_HW_ERRORS) that occurred (there may be multiple) are
 * indicated in the value set to user variable pointed to by \a pHwErrors.
 *
 * @sa adi_i2c_SetSlaveAddress().
 * @sa adi_i2c_SubmitBuffer().
 * @sa adi_i2c_IsBufferAvailable().
 * @sa adi_i2c_GetBuffer().
 * @sa ADI_I2C_TRANSACTION.
 * @sa ADI_I2C_HW_ERRORS.
 */
ADI_I2C_RESULT adi_i2c_ReadWrite (ADI_I2C_HANDLE const hDevice, ADI_I2C_TRANSACTION* const pTransaction, uint32_t* const pHwErrors) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (I2C_BUSY) {
        return ADI_I2C_DEVICE_IN_USE;
    }
    /* NULL transaction data pointer or zero transaction data count */
    if ((NULL == pTransaction->pData) || (0u == pTransaction->nDataSize)) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    /* reset submit/get safeguard flag */
    hDevice->bSubmitCalled = false;

    /* submit/commence the transaction */
    submitTransaction(hDevice, pTransaction);

    /* block on internal transaction completion/error semaphore */
    if (ADI_I2C_SUCCESS == hDevice->result) {

        SEM_PEND(hDevice, ADI_I2C_SEMAPHORE_FAILED);

        /* completion interrupt comes as FIFO unloads, but serialization may not be complete yet... */
        /* must also wait for hardware busy status to clear before giving back control */
        /* i.e., allow any transmit serialization to complete after last FIFO unload */
        while (I2C_BUSY) {
            ;
        }
    }

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->hwErrors;
    if (0u != hDevice->hwErrors) {
        /* set the HW error return code */
        hDevice->result = ADI_I2C_HW_ERROR_DETECTED;
    }

    /* return transaction result code */
    return hDevice->result;
}


/*!
 * @brief  Non-Blocking I2C Master-Mode data read or data write API.
 *
 * @param[in]    hDevice        Device handle obtained from adi_spi_Open().
 * @param[in]    pTransaction   Pointer to I2C transaction data struct.
 *
 * @return       Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_DEVICE_IN_USE [D]      An I2C transaction is already underway.
 *                - #ADI_I2C_INVALID_PARAMETER [D]  Invalid data pointer or count is detected.
 *
 * Request a non-blocking I2C data transfer (read or write) with or without preceding prologue
 * transmitted.  Control is returned to the calling application immediately, allowing the application
 * process other tasks.  The transaction result code is retrieved by #adi_i2c_GetBuffer().
 *
 * The application may optionally poll the I2C driver via the #adi_i2c_IsBufferAvailable() API while
 * the transaction is underway to determine if and when the submitted transaction is complete.
 * Eventually, the application \a MUST call the \a MANDATORY #adi_i2c_GetBuffer() API to obtain the
 * transaction result and complete the transaction.  Buffer allocations are made by the calling
 * code (the application).
 *
 * The #adi_i2c_GetBuffer() API may be called at any time, even if the transaction is incomplete;
 * the #adi_i2c_GetBuffer() call will simply block in incomplete transactions until the
 * transaction does complete... at which point #adi_i2c_GetBuffer() returns control with
 * the transaction result code.  Submitting background transactions is useful if the application has
 * housekeeping chores to perform when the I2C transaction is started, but later the application
 * decides to just block until the transaction is complete.
 *
 * The prologue and data buffers are handled as they are in the blocking #adi_i2c_ReadWrite() call,
 * it's just that the #adi_i2c_SubmitBuffer() API does not block on the data phase.
 *
 * @note The non-blocking #adi_i2c_SubmitBuffer() call \a REQUIRES a matching #adi_i2c_GetBuffer() call
 * to obtain the final transaction result code and to inform the driver that the application wants to
 * regain ownership of the buffers.  The application should be prepared to wait for this ownership
 * until the current transaction completes.  The matching #adi_i2c_GetBuffer() call is required even if
 * the transaction may have already completed.  The #adi_i2c_GetBuffer() call allows the driver to block
 * on completion or error events and then synchronize its internal blocking object.  The intermediate
 * #adi_i2c_IsBufferAvailable() API is optional.\n\n
 *
 * @note The #adi_i2c_SubmitBuffer() API is singular, i.e., only a single transaction may be submitted
 * at a time.  Simultaneous submits (e.g., ping-pong mode) are not supported by the I2C driver.
 *
 * @sa adi_i2c_ReadWrite().
 * @sa adi_i2c_SetSlaveAddress().
 * @sa adi_i2c_IsBufferAvailable().
 * @sa adi_i2c_GetBuffer().
 * @sa ADI_I2C_TRANSACTION.
 */
ADI_I2C_RESULT adi_i2c_SubmitBuffer (ADI_I2C_HANDLE const hDevice, ADI_I2C_TRANSACTION* const pTransaction) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (I2C_BUSY) {
        return ADI_I2C_DEVICE_IN_USE;
    }
    /* NULL transaction data pointer or zero transaction data count */
    if ((NULL == pTransaction->pData) || (0u == pTransaction->nDataSize)) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    /* set submit/get safeguard flag */
    hDevice->bSubmitCalled = true;

    /* submit/commence the transaction */
    submitTransaction(hDevice, pTransaction);

    /* no blocking on submit... just return the submit result */
    return hDevice->result;
}


/*!
 * @brief  Query if a non-blocking I2C transfer is complete.
 *
 * @param[in]     hDevice             Device handle obtained from adi_spi_Open().
 * @param[out]    pbCompletionState   Pointer to Boolean into which the I2C bus state is written.
 *
 * @return         Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_INVALID_SUBMIT_API     No matching submit call.
 *
 * Sets the application-provided Boolean variable pointed to by pbCompletionState either:
 *     - true, when the non-blocking transactions is complete, or
 *     - false, while the non-blocking transactions is still underway.
 *
 * This API is used in conjunction with a non-blocking #adi_i2c_SubmitBuffer() transfer to
 * determine when the transaction is complete.  Typically, non-blocking calls are used when the
 * calling application has other work to do while I2C controller serializes data over the I2C bus,
 * which is an interrupt-driven process.  The transaction is submitted as a non-blocking call and
 * the submitting API returns immediately, allowing the calling application to perform its other tasks.
 * The I2C driver services the interrupts to transfer data while the application performs its
 * other tasks.
 *
 * Non-blocking calls can be polled with this API for completion, or if the application has completed
 * its other tasks and wants to just wait on the I2C completion without further polling, it may call
 * the associated #adi_i2c_GetBuffer() API to convert the currently unblocked transaction to
 * a blocking one.
 *
 * @note This API is inappropriate in context of blocking calls to #adi_i2c_ReadWrite().
 *
 * @sa adi_i2c_ReadWrite().
 * @sa adi_i2c_SubmitBuffer().
 * @sa adi_i2c_GetBuffer().
 * @sa ADI_I2C_TRANSACTION.
 */
ADI_I2C_RESULT adi_i2c_IsBufferAvailable (ADI_I2C_HANDLE const hDevice, bool* const pbCompletionState) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
#endif

    /* fail if not a submit-based transaction */
    if (false == hDevice->bSubmitCalled) {
        return ADI_I2C_INVALID_SUBMIT_API;
    }

    /* return true when bus goes quiet */
    if (I2C_BUSY) {
        *pbCompletionState = false;
    } else {
        *pbCompletionState = true;
    }

    return ADI_I2C_SUCCESS;
}


/*!
 * @brief  Request ownership of a submitted buffer.
 *
 * @param[in]     hDevice             Device handle obtained from adi_spi_Open().
 * @param[out]    pHwErrors           Pointer to hardware error return variable.
 *
 * @return         Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_INVALID_SUBMIT_API     No matching submit call.
 *                - #ADI_I2C_HW_ERROR_DETECTED      A hardware error occurred, check \a pHwErrors.
 *
 * This is a potentially blocking MANDATORY call that the application MUST use to reclaim
 * ownership of any "submitted" transaction (submitted via a previous #adi_i2c_SubmitBuffer()
 * call) and obtain the transaction success/failure result code.  This API blocks until the
 * transaction is complete and returns the transaction result code.  If the transaction is
 * already complete, the blocking is trivial and control is returned immediately.
 *
 * Non-blocking calls can also be (optionally) polled with the non-blocking
 * #adi_i2c_IsBufferAvailable() API to see if and when the transaction is complete.
 *
 * The #adi_i2c_GetBuffer() call is a MANDATORY compliment to #adi_i2c_SubmitBuffer() and
 * allows the I2C driver to synchronize its internal blocking object.
 *
 * @note Application must check the return code to verify if any I2C Bus errors occurred.  Hardware
 * errors (I2C Protocol errors) are indicated with the #ADI_I2C_HW_ERROR_DETECTED return code, and
 * the set of hardware errors (enum #ADI_I2C_HW_ERRORS) that occurred (there may be multiple) are
 * indicated in the value set to user variable pointed to by \a pHwErrors.
 *
 * @sa adi_i2c_ReadWrite().
 * @sa adi_i2c_SubmitBuffer().
 * @sa adi_i2c_IsBufferAvailable().
 * @sa ADI_I2C_TRANSACTION.
 * @sa ADI_I2C_HW_ERRORS.
 */
ADI_I2C_RESULT adi_i2c_GetBuffer (ADI_I2C_HANDLE const hDevice, uint32_t* const pHwErrors) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
#endif

    /* fail if not a submit-based transaction */
    if (false == hDevice->bSubmitCalled) {
        return ADI_I2C_INVALID_SUBMIT_API;
    }

    /* block until complete or error interrupt sets the semaphore */
    SEM_PEND(hDevice, ADI_I2C_SEMAPHORE_FAILED);

    /* delay until bus goes quiet */
    while (I2C_BUSY) {
        ;
    }

    /* copy out any hardware errors... */
    *pHwErrors = hDevice->hwErrors;
    if (0u != hDevice->hwErrors) {
        /* set the HW error return code */
        hDevice->result = ADI_I2C_HW_ERROR_DETECTED;
    }

    /* return transaction result code */
    return hDevice->result;
}

/*!
 * @brief  Reset an I2C device and driver instance.
 *
 * @param[in]   hDevice    Device handle obtained from adi_spi_Open().
 *
 * @return      Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *
 * Reset the I2C physical controller and device driver internals.
 */
ADI_I2C_RESULT adi_i2c_Reset (ADI_I2C_HANDLE const hDevice) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
#endif

    /* destroy/recreate the semaphore to force a clear state */
    SEM_DELETE(hDevice, ADI_I2C_SEMAPHORE_FAILED)
        ;
    SEM_CREATE(hDevice, "i2c_sem", ADI_I2C_SEMAPHORE_FAILED)
        ;

    /* reset the driver and HW state */
    return i2cReset(hDevice);
}


/*!
 * @brief  Set the I2C serial bus speed.
 *
 * @param[in]     hDevice             Device handle obtained from adi_spi_Open().
 * @param[in]     requestedBitRate32  Requested I2C bus clock rate (in Hz).
 *
 * @return         Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_DEVICE_IN_USE [D]      Device is busy.
 *                - #ADI_I2C_BAD_SYS_CLOCK          Failure to obtain the current PCLK rate.
 *                - #ADI_I2C_BAD_BITRATE            Requested clock speed exceeds operational specification.
 *
 * Sets the I2C bus clock speed to the requested user parameter, \a requestedBitRate.
 *
 * @note Any I2C Bus clock rate may be requested up to and including the "FAST" mode I2C clock
 * rate (400 kHz), including the "STANDARD" mode (100 kHz).  Faster clock rates beyond "FAST"
 * mode (e.g., "FAST+" or "HIGH-SPEED" modes) are not supported by the hardware.  Slower clock
 * rates below approximately 55 kHz (assuming a 26 MHz system clock) are physically unrealizable
 * due to the fixed 8-bit field-width of the 8-bit I2C clock rate divide register.\n\n
 *
 * @note Default clock rate may be specified statically in the default user configuration file,
 * "adi_i2c_config.h".
 */
ADI_I2C_RESULT adi_i2c_SetBitRate (ADI_I2C_HANDLE const hDevice, uint32_t const requestedBitRate32) {

    uint32_t clockFrequency32, halfClock32;
    uint16_t halfClock16;
    uint16_t highTime16, lowTime16;

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (I2C_BUSY) {
        return ADI_I2C_DEVICE_IN_USE;
    }
#endif

    /* get input clockrate from power service */
    if (ADI_PWR_SUCCESS != adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &clockFrequency32)) {
        return ADI_I2C_BAD_SYS_CLOCK;
    }

    /* block requests above max rated 400kHz operation */
    if (ADI_I2C_MAX_RATE < requestedBitRate32) {
        return ADI_I2C_BAD_BITRATE;
    }

    /* compute half-cycle period in 32-bits (">>1" is divide by 2) */
    halfClock32 = (clockFrequency32 / requestedBitRate32) >> 1;  /* HRM equation */

    /* downcast to 16-bit to match destination field */
    halfClock16 = (uint16_t)(halfClock32 & 0x0000ffffu);

    /* check for lost precision in conversion */
    if (halfClock32 != halfClock16) {
        return ADI_I2C_BAD_BITRATE;
    }

    /* adjust high and low durations per HRM */
    highTime16 = halfClock16 - 7u;  /* empirical: varies with board layout, pullups, etc */
    lowTime16  = halfClock16 - 1u;

    /* shift values into their clock rate divider register positions */
    highTime16 <<= BITP_I2C_DIV_HIGH;
    lowTime16  <<= BITP_I2C_DIV_LOW;

    /* check for divider overflows beyond designated (8-bit) field masks */
    if (    (uZero16 != ((uint16_t)highTime16 & (uint16_t)(~(BITM_I2C_DIV_HIGH))))
            ||
            (uZero16 != ((uint16_t)lowTime16 & (uint16_t)(~(BITM_I2C_DIV_LOW))))
       ) {
       return ADI_I2C_BAD_BITRATE;
    }

    /* program new values */
    hDevice->pDev->DIV = highTime16 | lowTime16;

    return ADI_I2C_SUCCESS;
}


/*!
 * @brief  Set the I2C serial bus slave address.
 *
 * @param[in]     hDevice             Device handle obtained from adi_spi_Open().
 * @param[in]     SlaveAddress        New 7-bit address for targeting a slave device.
 *
 * @return         Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_DEVICE_IN_USE [D]      Device is busy.
 *                - #ADI_I2C_INVALID_SLAVE_ADDRESS  Slave address exceeds the 7-bit limit.
 *
 * Sets the 7-bit (unformatted) slave address for which all subsequent I2C bus traffic is directed.
 * Read/write address formatting is performed by the driver, depending on bus direction.
 *
 * @note This driver does not support the I2C 10-bit extended addressing scheme.\n\n
 *
 * @note Default slave address may be specified statically in the default user configuration file,
 * "adi_i2c_config.h".
 */
ADI_I2C_RESULT adi_i2c_SetSlaveAddress (ADI_I2C_HANDLE const hDevice, uint16_t const SlaveAddress) {

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (I2C_BUSY) {
        return ADI_I2C_DEVICE_IN_USE;
    }
#endif

    /* verify no slave address bits fall outside the 7-bit addressing model (10-bit addressing not supported) */
    if (uZero16 != (SlaveAddress & (uint16_t)(~(BITM_I2C_ADDR1_VALUE >> 1)))) {
        return ADI_I2C_INVALID_SLAVE_ADDRESS;
    }

    /* save new address */
    hDevice->i2cDeviceAddress = SlaveAddress;

    return ADI_I2C_SUCCESS;
}


/*!
 * @brief  Transmit a General Call command to all slave devices on the I2C bus.
 *
 * @param[in]     hDevice            Device handle obtained from adi_spi_Open().
 * @param[in]     pData              Pointer to data buffer to transmit.
 * @param[in]     nDataSize          Size of data buffer to transmit.
 * @param[out]    pHwErrors          Pointer to hardware error return variable.
 *
 * @return         Status
 *                - #ADI_I2C_SUCCESS                Call completed successfully.
 *                - #ADI_I2C_BAD_DEVICE_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_I2C_DEVICE_IN_USE [D]      Device is busy.
 *
 * Broadcasts the given command buffer across the I2C bus to reserved General Call (GC)
 * address (address zero).  All, some, or none of the slave devices on the I2C bus will
 * respond, depending on their capabilities.  All responding slave devices will process
 * the GC command according to their capabilities.
 *
 * The GC command is a blocking transaction.
 *
 * The application is responsible for formatting the GC command into the data buffer
 * according to various Philips Semiconductor (now, NXP) documents, such as the 2014
 * Revision 6 document: "UM10204 I2C-Bus Specification and User Manual"
 * (see www.nxp.com/documents/user_manual/UM10204.pdf).
 *
 * No prologue precedes the GC command data; the GC command data is transmitted verbatim.
 *
 * @note The currently active slave address is saved and restored when transmitting GC
 * commands to the reserved GC address (address zero).
 *
 */
ADI_I2C_RESULT adi_i2c_IssueGeneralCall (ADI_I2C_HANDLE const hDevice, uint8_t* const pData, uint8_t const nDataSize, uint32_t* const pHwErrors) {

	ADI_I2C_RESULT result;
	ADI_I2C_TRANSACTION xfr;

#ifdef ADI_DEBUG
    if (IsDeviceHandle(hDevice)) {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (I2C_BUSY) {
        return ADI_I2C_DEVICE_IN_USE;
    }
#endif

    /* force general call reserved target address of zero */
    uint16_t savedSlaveAddress = hDevice->i2cDeviceAddress;
    hDevice->i2cDeviceAddress = 0u;

    /* setup the transfer */
    xfr.pPrologue        = NULL;
    xfr.nPrologueSize    = 0u;
    xfr.pData            = pData;
    xfr.nDataSize        = nDataSize;
    xfr.bReadNotWrite    = false;
    xfr.bRepeatStart     = false;

    /* dispatch as a blocking transmit call */
    result = adi_i2c_ReadWrite(hDevice, &xfr, pHwErrors);

    /* always restore saved slave address */
    hDevice->i2cDeviceAddress = savedSlaveAddress;

    if (ADI_I2C_SUCCESS != result) {
		return result;  /* read/write failure... */
	} else {
		return hDevice->result;  /* actual result */
	}
}


 /*! \cond PRIVATE */


/**********************************************************************************\
|*****************************static helper functions******************************|
\**********************************************************************************/

static void submitTransaction(ADI_I2C_HANDLE const hDevice, ADI_I2C_TRANSACTION* const pTransaction) {

    /* reset internal return code */
    hDevice->result = ADI_I2C_SUCCESS;

    /* reset hardware error code */
    hDevice->hwErrors = ADI_I2C_HW_ERROR_NONE;

    /* wait for HW to be ready */
    while (I2C_BUSY) {
        ;
    }

    /* save common user parameters */
    hDevice->pNextPrologueByte      = pTransaction->pPrologue;
    hDevice->remainingPrologueCount = pTransaction->nPrologueSize;
    hDevice->bRepeatStart           = pTransaction->bRepeatStart;

    /* encode (mask and upshift) the slave address, leaving room for the r/w control bit (LSB) */
    hDevice->i2cEncodedDeviceAddress = (hDevice->i2cDeviceAddress & (BITM_I2C_ADDR1_VALUE >> 1)) << 1;

    /* dispatch */
    if (pTransaction->bReadNotWrite) {

        /* setup read parameters */
        hDevice->pNextReadByte           = pTransaction->pData;
        hDevice->remainingReadCount      = pTransaction->nDataSize;
        hDevice->pNextWriteByte          = NULL;
        hDevice->remainingWriteCount     = 0u;

        /* set read bit */
        hDevice->i2cEncodedDeviceAddress |= READ_NOT_WRITE;

        /* commence receive */
        commenceReceive(hDevice);

    } else {

        /* setup write parameters */
        hDevice->pNextReadByte           = NULL;
        hDevice->remainingReadCount      = 0u;
        hDevice->pNextWriteByte          = pTransaction->pData;
        hDevice->remainingWriteCount     = pTransaction->nDataSize;

        /* clear read bit */
        hDevice->i2cEncodedDeviceAddress &= (~READ_NOT_WRITE);

        /* commence transmit */
        commenceTransmit(hDevice);
    }
}


static void commenceTransmit(ADI_I2C_HANDLE const hDevice) {

    /* transmit is always pure transmit, whether we have a prologue or not... */

    /* enable PIO interrupts */
    NVIC_EnableIRQ(hDevice->pDevInfo->pioIRQn);

    /* enable i2c for PIO-based transmit interrupts */
    hDevice->pDev->MCTL |= (BITM_I2C_MCTL_IENMTX | BITM_I2C_MCTL_MASEN);

    /* how many bytes are available in the transmit FIFO (2-deep) */
    uint16_t writableBytes = 2u - (hDevice->pDev->MSTAT & (uint16_t)BITM_I2C_MSTAT_MTXF);

    /* prime transmit FIFO with any prologue data */
    while ((0u < writableBytes) && (hDevice->remainingPrologueCount)) {
        hDevice->pDev->MTX = *hDevice->pNextPrologueByte;
        hDevice->pNextPrologueByte++;
        hDevice->remainingPrologueCount--;
        writableBytes--;
    }

    /* flesh out any remaining FIFO space with transmit data */
    while ((0u < writableBytes) && (hDevice->remainingWriteCount)) {
        hDevice->pDev->MTX = *hDevice->pNextWriteByte;
        hDevice->pNextWriteByte++;
        hDevice->remainingWriteCount--;
        writableBytes--;
    }

    /* launch the transmit */
    hDevice->pDev->ADDR1 = hDevice->i2cEncodedDeviceAddress;
}


/* initiate receive addressing phase */
static void commenceReceive(ADI_I2C_HANDLE const hDevice) {

    /* receive can be either pure receive (no prologue),
       or a transmit (of prologue) followed by a receive */

    /* enable PIO interrupts */
    NVIC_EnableIRQ(hDevice->pDevInfo->pioIRQn);

    /* enable i2c for PIO-based receive interrupts */
    hDevice->pDev->MCTL |= (uint16_t)(BITM_I2C_MCTL_IENMRX | BITM_I2C_MCTL_MASEN);

    /* program HW receive count */
    if (hDevice->remainingReadCount  > BITM_I2C_MRXCNT_EXTEND) {
        hDevice->pDev->MRXCNT        = BITM_I2C_MRXCNT_EXTEND;
        hDevice->remainingReadCount -= BITM_I2C_MRXCNT_EXTEND;
    } else {
        hDevice->pDev->MRXCNT        = hDevice->remainingReadCount - 1u;
        hDevice->remainingReadCount  = 0u;
    }

    /* if we have prologue (the dreaded "COMBINED FORMAT"), transmit the prologue prior to data receive... */
    if (hDevice->remainingPrologueCount) {

        /* -OR- in transmit interrupt enable if we have prologue data to send */
        hDevice->pDev->MCTL |= BITM_I2C_MCTL_IENMTX;

        /* how many bytes are available in the transmit FIFO (should be 2) */
        uint16_t writableBytes = 2u - (hDevice->pDev->MSTAT & (uint16_t)BITM_I2C_MSTAT_MTXF);

        /* prime transmit FIFO with any prologue data (memory address or command) first */
        while ((0u < writableBytes) && (hDevice->remainingPrologueCount)) {
            hDevice->pDev->MTX = *hDevice->pNextPrologueByte;
            hDevice->pNextPrologueByte++;
            hDevice->remainingPrologueCount--;
            writableBytes--;
        }

        /* initiate prologue transmit with read bit cleared (for prologue write) */
        /* (read sequence is initiated by transmit handler, *after* prologue is transmitted...) */
        hDevice->pDev->ADDR1 = hDevice->i2cEncodedDeviceAddress & (uint16_t)(~READ_NOT_WRITE);

    } else {

        /* no prologue... initiate pure receive (read bit already set) */
        hDevice->pDev->ADDR1 = hDevice->i2cEncodedDeviceAddress;
    }
}


/* reset the I2C HW */
static ADI_I2C_RESULT i2cReset(ADI_I2C_HANDLE const hDevice) {

	volatile uint16_t temp;
    /* disable interrupts */
    NVIC_DisableIRQ(hDevice->pDevInfo->pioIRQn);

    /* reset any pending interrupts and TX FIFO (W1C) */
    temp = hDevice->pDev->MSTAT;
    hDevice->pDev->MSTAT = temp;

    /* discard any rogue RX FIFO data */
    while (uZero16 != (hDevice->pDev->STAT & (uint16_t)BITM_I2C_STAT_MRXF)) {
        volatile uint16_t delme ADI_UNUSED_ATTRIBUTE = hDevice->pDev->MTX;
    }

    /* reset i2c control register */
    hDevice->pDev->MCTL = 0u;

    /* reset repeat start logic */
    hDevice->pDev->SHCTL = 1u;

    /* (re)assert controller defaults from user config values */
    hDevice->pDev->MCTL         = hDevice->pDevInfo->pConfig->MasterControlRegister;
    hDevice->pDev->DIV          = hDevice->pDevInfo->pConfig->ClockDividerRegister;
    hDevice->pDev->SHCTL        = hDevice->pDevInfo->pConfig->SharedControlRegister;
    hDevice->pDev->TCTL         = hDevice->pDevInfo->pConfig->TimingControlRegister;
    hDevice->pDev->ASTRETCH_SCL = hDevice->pDevInfo->pConfig->ClockStretchRegister;
    hDevice->i2cDeviceAddress   = hDevice->pDevInfo->pConfig->TargetSlaveAddress;

    return ADI_I2C_SUCCESS;
}


/**********************************************************************************\
|********************************interrupt handlers********************************|
\**********************************************************************************/


/* transmit interrupt handler */
static void transmitHandler(ADI_I2C_HANDLE const hDevice) {

    /* how much room in transmit FIFO? */
    /* DO ***NOT*** USE MSTAT:MTXF... FALSELY INDICATES MOSTLY FULL FIFO! */
    uint16_t writableBytes = 2u - ((hDevice->pDev->STAT & (uint16_t)BITM_I2C_STAT_MTXF) >> BITP_I2C_STAT_MTXF);

    /* for extended prologues, continue pushing prologue data out */
    while ((0u < writableBytes) && (hDevice->remainingPrologueCount)) {
        hDevice->pDev->MTX = *hDevice->pNextPrologueByte;
        hDevice->pNextPrologueByte++;
        hDevice->remainingPrologueCount--;
        writableBytes--;
    }

    /* once the prologue is done... */
    if (0u == hDevice->remainingPrologueCount) {

        /* if we have a completed prologue associated with a read sequence...  */
        if (0u < hDevice->remainingReadCount) {

            /* initiate the read (subsequently driven by receive interrupt handler) */
            hDevice->pDev->ADDR1 = hDevice->i2cEncodedDeviceAddress;

        } else {

            /* normal transmit interrupt: just push transmit data */
            while ((0u < writableBytes) && (hDevice->remainingWriteCount)) {
                hDevice->pDev->MTX = *hDevice->pNextWriteByte;
                hDevice->pNextWriteByte++;
                hDevice->remainingWriteCount--;
                writableBytes--;
            }
        }
    }

    /* clear TX interrupt as we complete transmit writes */
    if (0u == hDevice->remainingWriteCount) {
        hDevice->pDev->MSTAT = BITM_I2C_MSTAT_MTXREQ;
    }
}


/* receive interrupt handler */
static void receiveHandler(ADI_I2C_HANDLE const hDevice) {

    /* note: we never need to deal with prologue data here... it will already be transmitted... */

    /* how many bytes in receive FIFO? */
    uint16_t readableBytes = (hDevice->pDev->STAT & (uint16_t)BITM_I2C_STAT_MRXF) >> BITP_I2C_STAT_MRXF;

    /* pull bytes from fifo */
    while (0u < readableBytes) {

        readableBytes--;

        /* pull one byte */
        *hDevice->pNextReadByte = (uint8_t)hDevice->pDev->MRX;
        hDevice->pNextReadByte++;

        if ((0u == hDevice->pDev->MCRXCNT) && (hDevice->remainingReadCount)) {

            /* if HW read counter goes to zero with remaining data to read, reprogram read count */
            if (hDevice->remainingReadCount  > BITM_I2C_MRXCNT_EXTEND) {
                /* use extended count flag for large remaining counts... */
                hDevice->pDev->MRXCNT        = BITM_I2C_MRXCNT_EXTEND;
                hDevice->remainingReadCount -= BITM_I2C_MRXCNT_EXTEND;
            } else {
                /* new count fits... no need for extended count */
                hDevice->pDev->MRXCNT        = hDevice->remainingReadCount - 1u;
                hDevice->remainingReadCount  = 0u;
            }
        }
    }
}

/* completion interrupt handler */
static void completeHandler(ADI_I2C_HANDLE const hDevice) {

    /* block on busy until all transmit data has both left
       the fifo AND has been fully serialized to the bus. */
    while (I2C_BUSY) {
        ;
    }

    /* disable interrupts */
    NVIC_DisableIRQ(hDevice->pDevInfo->pioIRQn);

    /* reset controller to default user config state */
    hDevice->pDev->MCTL = (uint16_t)gConfigInfo->MasterControlRegister;
}


/* error interrupt handler */
static void errorHandler(ADI_I2C_HANDLE const hDevice) {

    /* accumulate I2C bus errors */

    if (uZero16 != (hDevice->hwStatus & (uint16_t)BITM_I2C_MSTAT_NACKADDR)) {
        hDevice->hwErrors |= ADI_I2C_HW_ERROR_NACK_ADDR;
    }

    if (uZero16 != (hDevice->hwStatus & (uint16_t)BITM_I2C_MSTAT_NACKDATA)) {
        hDevice->hwErrors |= ADI_I2C_HW_ERROR_NACK_DATA;
    }

    if (uZero16 != (hDevice->hwStatus & (uint16_t)BITM_I2C_MSTAT_ALOST)) {
        hDevice->hwErrors |= ADI_I2C_HW_ERROR_ARBITRATION_LOST;
    }

    /* if no other errors exist, note we had an unexpected error */
    if (hDevice->hwErrors == ADI_I2C_HW_ERROR_NONE) {
        hDevice->hwErrors = ADI_I2C_HW_ERROR_UNEXPECTED_ERROR;
    }
}


/**********************************************************************************\
|*****************************I2C INTERRUPT HANDLER********************************|
\**********************************************************************************/


/* PIO mode I2C interrupt handler */
void I2C0_Master_Int_Handler(void) {

    bool bPost = false;

    /* rtos prologue */
    ISR_PROLOG()
        ;

    /* recover device handle */
    ADI_I2C_HANDLE const hDevice = (ADI_I2C_HANDLE)i2c_device_info[0].hDevice;

    /* save destructive status read... */
    hDevice->hwStatus = hDevice->pDev->MSTAT;

    /* if RepeatStart request is pending, rewrite address register ASAP (and only once) to block stop bit */
    if (hDevice->bRepeatStart) {
        hDevice->pDev->ADDR1 = hDevice->i2cEncodedDeviceAddress;
        hDevice->bRepeatStart = false;  /* just do it once on 1st interrupt */
    }

    /* forward TX interrupts to TX handler */
    if (uZero16 != (hDevice->hwStatus & (uint16_t)BITM_I2C_MSTAT_MTXREQ)) {
        transmitHandler(hDevice);
    }

    /* forward RX interrupts to RX handler */
    if (uZero16 != (hDevice->hwStatus & (uint16_t)BITM_I2C_MSTAT_MRXREQ)) {
        receiveHandler(hDevice);
    }

    /* dispatch any errors */
    if (uZero16 != (hDevice->hwStatus & ADI_I2C_STATUS_ERROR_MASK)) {
        errorHandler(hDevice);

      /* post on bus error */
      bPost = true;
    }

    /* transmit complete */
    if (uZero16 != (hDevice->hwStatus & BITM_I2C_MSTAT_TCOMP)) {
        completeHandler(hDevice);

      /* post on completion */
      bPost = true;
    }

    /* just post once */
    if (true == bPost) {
        SEM_POST(hDevice);
    }

    /* rtos epilogue */
    ISR_EPILOG()
        ;
}

/*! \endcond */


/* @} */
