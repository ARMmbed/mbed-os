/*!
 *****************************************************************************
 @file:    adi_i2c.c
 @brief:   I2C device driver implementation
 @version: $Revision: 33333 $
 @date:    $Date: 2016-01-22 16:39:48 +0000 (Fri, 22 Jan 2016) $
 -----------------------------------------------------------------------------
Copyright (c) 2014-2015 Analog Devices, Inc.

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

/** @addtogroup I2C_Driver I2C Device Driver
 *  @{
 *
I2C controller allows a device to interface to an inter IC bus. It
was designed with a high level of functionality and is compatible with
multi-master, multi-slave bus configurations.

The I2C controller includes these features:

- Simultaneous master and slave operation on multiple device systems
- Support for multi-master bus arbitration
- 10,7-bit addressing
 Clock stretching supported for the slave and the master.
- Support for DMA
- Support for internal and external loopback.
 */

#ifndef _ADI_I2C_V1_C_
/* \cond PRIVATE */
#define _ADI_I2C_V1_C_

/*==========  I N C L U D E  ==========*/
#if defined ( __GNUC__ )
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

#include <services/int/adi_int.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include <ssdd_common/common_def.h>
#include <drivers/i2c/adi_i2c.h>
#include "adi_i2c_def_v1.h"

#if defined(__ADUCM30xx__)
#define NUM_I2C_DEVICES (1u)
#else
#error "Unknown processor family"
#endif

/*==========  M I S R A  ==========*/

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
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm088,Pm140
#endif /* __ICCARM__ */

/*==========  D A T A  ==========*/
static ADI_I2C_DRIVER adi_i2c_Device[] =
{
    /* I2C 0 Master */
    {
        pADI_I2C0,    /* Base I2C register address */
        NULL          /* Data storage pointer for Master */
    },
    /* I2C 0 Slave */
    {
        pADI_I2C0,    /* Base I2C register address */
        NULL          /* Data storage pointer for Slave */
    }

};

/*==========  S T A T I C    F U N C    D E C L A R A T I O N S  ==========*/
/* Initialize the device structure */
static void Initialize(ADI_I2C_MODE const  eMode, ADI_I2C_DRIVER *drv);

/* Stop the I2C Device */
static void StopDevice(ADI_I2C_DRIVER *drv);

/* Flush the Transmit buffer */
static void FlushTx(ADI_I2C_DRIVER const *drv);

/* Flush the Receive buffer */
static void FlushRx(ADI_I2C_DRIVER const *drv);

/* Coinfigure the NVIC */
static void configureNVIC(ADI_I2C_DRIVER const *drv, bool_t bEnable);

/* Stop the device */
static void StopDevice(ADI_I2C_DRIVER *drv);

/* Enable/Disable the given interrupt */
static void EnableInterrupt(IRQn_Type nIRQNum, bool_t bEnable);

/* Configure the NVIC */
static void configureNVIC(ADI_I2C_DRIVER const *drv, bool_t bEnable);

/* Configure the device */
static ADI_I2C_RESULT ConfigureDevice (ADI_I2C_DRIVER* drv);

/* Setup the device */
static ADI_I2C_RESULT SetupDevice(ADI_I2C_DRIVER *drv);

/* Notify Buffer Completion */
static void NotifyBufferCompletion (ADI_I2C_DRIVER const *drv);

/* Flush the Tx Buffer */
static void FlushTx(ADI_I2C_DRIVER const *drv);

/* Flush the Rx Buffer */
static void FlushRx(ADI_I2C_DRIVER const *drv);

#if (ADI_I2C_CFG_ENABLE_PIO_SUPPORT == 1)
/* Read the Rx FIFO in PIO mode */
static void ReadRxFifo(ADI_I2C_DRIVER const *drv);

/* Fill the transmit FIFO */
static void FillTransmitFifo(ADI_I2C_DRIVER const *drv);

/* Process the active buffer for PIO mode */
static ADI_I2C_RESULT PioManageFIFO(ADI_I2C_DRIVER const *drv, bool_t bStart);
#endif

#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
/* Un Initialize the DMA */
static void UnInitDma(ADI_I2C_DRIVER const *drv);

/* Process the FIFO for DMA mode */
static ADI_I2C_RESULT DMAManageFIFO(ADI_I2C_DRIVER const *drv, bool_t bStart);
#endif

/* Init active buffer transfer */
static ADI_I2C_RESULT InitActiveBufferTransfer(ADI_I2C_DRIVER const *drv);

/* Start or continue processing.  Processing stops when all buffers are complete. */
static ADI_I2C_RESULT ProcessBuffers(ADI_I2C_DRIVER const *drv);

/* Non-blocking Get */
static  ADI_I2C_RESULT I2C_NonBlockingGet (
                                           ADI_I2C_HANDLE   const   hDevice,
                                           uint8_t            const  nDirection,
                                           void                   **ppBuffer
                                           );

/* Non-blocking IO function */
static ADI_I2C_RESULT I2C_BlockingIO (
                                      ADI_I2C_HANDLE     const hDevice,
                                      uint8_t            const  nDirection,
                                      void                     *pBuffer,
                                      uint32_t           const  nBufSize
                                      );

/* Non-blocking submit */
static ADI_I2C_RESULT I2C_NonBlockingSubmit (
                                             ADI_I2C_HANDLE    const hDevice,
                                             uint8_t           const nDirection,
                                             void                   *pBuffer,
                                             uint32_t          const nBufSize,
                                             bool_t              const bRestart
                                             );

/* Push a buffer to the back of a buffer list */
static void bpush_buffer(ADI_I2C_BUFFER_STRUCT** pList, ADI_I2C_BUFFER_STRUCT* pBuffer);

/* Pop a buffer from the front of a buffer list */
static ADI_I2C_BUFFER_STRUCT* fpop_buffer(ADI_I2C_BUFFER_STRUCT** pList);

#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
/* Start the master transfer */
static void CommenceMasterTransfer(ADI_I2C_DRIVER const *drv, uint16_t dir);

/* Process master address NACK interrupt */
static __INLINE void onMasterNACKADDR    (ADI_I2C_DRIVER const *drv);

/* Process master arbitration lost interrupt */
static __INLINE void onMasterALOST    (ADI_I2C_DRIVER const *drv);

/* Process Master Complete interrupt */
static __INLINE void onMasterCOMP    (ADI_I2C_DRIVER const *drv);

/* Process master data NACK interrupt */
static __INLINE void onMasterNACKDATA    (ADI_I2C_DRIVER const *drv);

/* Enable the master */
static void EnableMaster (ADI_I2C_DRIVER const *drv);

/* Do callback for the given info */
static void InfoSetCallback(ADI_I2C_DRIVER const *drv, BUFF_INFO info, uint32_t event);
#endif

#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
/* Set the given glboal Event */
static void SetGlobalEvent (ADI_I2C_DRIVER const *drv, uint16_t nEvent);

/* Process slave STOP interrupt */
static __INLINE void onSlaveSTOP (ADI_I2C_DRIVER const *drv);

/* Process slave general call interrupt */
static __INLINE void onSlaveGCINT    (ADI_I2C_DRIVER const *drv);

/* Process slave repeat start interrupt */
static __INLINE void onSlaveREPSTART    (ADI_I2C_DRIVER const *drv);

/* Enable the Slave */
static void EnableSlave (ADI_I2C_DRIVER const *drv);
#endif

/* Process completion of the buffer */
static void ProcessBufferCompletion (ADI_I2C_DRIVER const *drv);

/* Manage buffers in the interrupt region */
static void IntManageBuffers(ADI_I2C_DRIVER const *drv);

/* Slave Tx DMA interrupt handler */
ADI_INT_HANDLER (DMA_I2C0_STX_Int_Handler);

/* Slave Rx DMA interrupt handler */
ADI_INT_HANDLER (DMA_I2C0_SRX_Int_Handler);

/* Master DMA interrupt handler */
ADI_INT_HANDLER (DMA_I2C0_MX_Int_Handler);


/* Master Interrupt Handler */
ADI_INT_HANDLER (I2C0_Master_Int_Handler);

/* Dispatch Master interrupt  */
static void DispatchMasterInterrupt (ADI_I2C_DRIVER *drv);

/* Dispatch Slave interrupts */
static void DispatchSlaveInterrupt(ADI_I2C_DRIVER *drv);

/* Slave interrupt handler */
ADI_INT_HANDLER (I2C0_Slave_Int_Handler);


#if defined(ADI_DEBUG)
/* Return whether the given handle is a valid device handle */
static bool_t IsDeviceHandle(ADI_I2C_DRIVER const *pDevice);
#endif

/*==========  D E F I N E S  ==========*/

/* Common configurations bits for the master */
#define COMMON_CFG_MBITS   (BITM_I2C_MCTL_IENALOST | BITM_I2C_MCTL_IENACK)

/* Common configurations bits for the slave */
#define COMMON_CFG_SBITS   (BITM_I2C_SCTL_IENREPST)

/* Direction #defines */
#define OUTBOUND      (0u)
#define INBOUND       (1u)

/* Master-Mode internal state machine idle test */
#define MASTER_BUSY   (drv->pReg->I2CMSTA & I2CMSTA_BUSY)

/* Slave-Mode internal state machine idle test */
#define SLAVE_BUSY    (drv->pReg->I2CSSTA & I2CSSTA_BUSY)

/*==========  M A C R O S  ==========*/
/*
 *  Various MACROS used by the driver
 */

#ifdef __ICCARM__
/*
* Pm154 (rule 19.10): in the definition of a function-like macro, each instance
*                     of a parameter shall be enclosed in parentheses
*   Parameter use without parentheses needed for struct field name in register access macro.
*/
#pragma diag_suppress=Pm154
#endif /* __ICCARM__ */

#define REG(reg)                        (drv->pReg->reg)

#ifdef __ICCARM__
#pragma diag_default=Pm154
#endif /* __ICCARM__ */

#define SET_BITS_U16(VAR,BITS)          ((VAR) |=  (BITS))
#define CLR_BITS_U16(VAR,BITS)          ((VAR) &= (uint16_t)~(BITS))

#define BMASK(VAR,MASK)                 ((VAR) & (MASK))
#define IS_BIT_SET(VAR,BIT)             (BMASK((VAR),(BIT)) != 0u)
#define IS_ANY_BITS_SET(VAR,BITS)       (BMASK((VAR),(BITS)) != 0u)
#define IS_BITS_SET(VAR,BITS)           (BMASK((VAR),(BITS)) == (BITS))

#define SET_REG_BITS(RNAME,BITS)         SET_BITS_U16(REG(RNAME),(BITS))
#define CLR_REG_BITS(RNAME,BITS)         CLR_BITS_U16(REG(RNAME),(BITS))

#define DATA                            (drv->pData)
#define CFG                             (&drv->pData->config)
#define ABUFF                           (drv->pData->activeBuffer)

#define IS_BUFF_AVAIL(X)                ((X) != NULL)

#ifdef ADI_DEV
#define ASSERT(X)   assert(X)
#else
#define ASSERT(X)
#endif

/*==========  C O N F I G    D E F I N E S  ==========*/
#if ((ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1) && (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1))
#define ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
#endif

/* \endcond */


/*==========  C O D E  ==========*/

/*
 * API implementation:
 */

/**
 * @brief       Open a I2C device instance.
 *
 *  If successful the returned ADI_I2C_HANDLE parameter will hold a device handle
 *  to be used as an input to other I2C API functions.  The I2C device must be
 *  opened before using any other I2C API functions.
 *
 *  The amount of memory required by the driver is specified by the ADI_I2C_MEMORY_SIZE
 *  definition.
 *
 * @param [in]  DeviceNum           The I2C device instance number to be opened.
 * @param [in]  eMode               The I2C operation mode.
 * @param [in]  pMemory             Application supplied memory space for use by the driver.
 * @param [in]  MemorySize          Application supplied memory size (bytes).
 * @param [out] phDevice            A pointer to a location where the handle to the
 *                                  opened device is written.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                 Successfully opened the given instance.
 *              - #ADI_I2C_BAD_DEVICE_NUMBER   [D] The device number is invalid.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED  [D] The I2C mode is invalid mode.
 *              - #ADI_I2C_INSUFFICIENT_MEMORY [D] The application supplied memory size is insufficient.
 *              - #ADI_I2C_DEVICE_IN_USE       [D] The I2C device is already open.
 *
 * @sa          adi_i2c_Close()
 */
ADI_I2C_RESULT adi_i2c_Open(
                            uint32_t               const  DeviceNum,
                            ADI_I2C_MODE           const  eMode,
                            void                 * const  pMemory,
                            uint32_t               const  MemorySize,
                            ADI_I2C_HANDLE       * const  phDevice)
{
    ADI_I2C_RESULT result = ADI_I2C_SUCCESS;
    ADI_I2C_DRIVER *drv;

#if defined(ADI_DEBUG)
    if (DeviceNum >= (uint32_t)NUM_I2C_DEVICES)
    {
        return ADI_I2C_BAD_DEVICE_NUMBER;
    }

    if (eMode > ADI_I2C_SLAVE)
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }

#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 0)
    if (eMode == ADI_I2C_MASTER)
    {
      return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 0)
    if (eMode == ADI_I2C_SLAVE)
    {
      return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

#endif

    /* Get the pointer to the internal structure */
    drv = (eMode == ADI_I2C_MASTER)
                ?   &adi_i2c_Device[DeviceNum*2u]
                  : &adi_i2c_Device[DeviceNum*2u+1u];

#if defined(ADI_DEBUG)
    /* check if device is in use */
    if (drv->pData != NULL)
    {
        return ADI_I2C_DEVICE_IN_USE;
    }

    /* Asserts to validate the memory size macros */
    assert(ADI_I2C_MEMORY_SIZE >= DATA_MEM_SIZE);

    if (MemorySize < (uint32_t)ADI_I2C_MEMORY_SIZE)
    {
        return ADI_I2C_INSUFFICIENT_MEMORY;
    }
#endif /* ADI_DEBUG */

    /* store a bad handle in case of failure */
    *phDevice = NULL;

    /* initialize the memory for I2C parameters */
    drv->pData = pMemory;

    /* Do common init */
    Initialize(eMode,drv);

#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* create a semaphore for buffer management */
    CREATE_SEM(drv->pData,ADI_I2C_ERR_RTOS);
#endif

    /* init successful */
    drv->pData->config.mode = eMode;

    /*write back the device handle */
    *phDevice = (ADI_I2C_HANDLE)drv;

    return result;
}


/**
 * @brief       Close the given I2C device.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS               Successfully closed the given instance.
 *              - #ADI_I2C_FAILURE               Failed to close the given instance.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D] The device is not open.
 *
 * @sa          adi_i2c_Open()
 */
ADI_I2C_RESULT adi_i2c_Close(ADI_I2C_HANDLE const hDevice)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

    /* Check the function parameters */
#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
#endif

    /* IF (The device is enabled) */
    if (DATA->bDeviceEnabled) {
        /* Disable the device */
        adi_i2c_Enable(drv, false);
    }

#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* Destroy the semaphore */
    DELETE_SEM(drv->pData,ADI_I2C_ERR_RTOS);
#endif

    /* Clear the callback parameters */
    DATA->Callback   = (void*)0;
    DATA->pCallParam = (void*)0;

    /* Remove driver data */
    drv->pData = NULL;

    return ADI_I2C_SUCCESS;
}


/**
 * @brief       Register an application defined callback function.
 *
 * An application defined callback function is used to notify the application of
 * device related events.
 *
 * @param [in]  hDevice             Handle to the I2C device.
 * @param [in]  pfCallback          Application supplied callback function which is
 *                                  called to notify device related events.  A value of
 *                                  NULL disables application callbacks.
 * @param [in]  pCBParam            Application supplied callback parameter which
 *                                  will be passed back in the callback function.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS               Successfully set the application defined callback function.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D] The device is not open.
 *
 * @sa  #ADI_I2C_EVENT
 */
ADI_I2C_RESULT adi_i2c_RegisterCallback(
    ADI_I2C_HANDLE         const hDevice,
    ADI_CALLBACK           pfCallback,
    void                  *pCBParam)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
#endif

    /* Set the callback information */
    drv->pData->Callback = pfCallback;
    drv->pData->pCallParam = pCBParam;

    return ADI_I2C_SUCCESS;
}

/**
 * @brief       Write data to a I2C device (non-blocking).
 *
 * This function returns immediately (does not wait for the write to complete).
 * An interrupt is generated when the transfer is complete or an error occurs.
 * The supplied buffer is owned by the driver during the data transfer process.
 * This buffer should not be modified or deleted by the application until the
 * data transfer is complete.  It's recommended that the buffer not be a
 * local variable.
 *
 * @param [in]  hDevice             Handle to the I2C device.
 * @param [in]  *pBuffer            The buffer that contains the data to transmit.
 * @param [in]  nBufSize            The number of bytes to write.
 * @param [in]  bRestart            Issue a repeat start condition at the conclusion of the current transfer.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                Successfully submitted the buffer for writing.
 *              - #ADI_I2C_INVALID_PARAMETER [D]  The buffer pointer passes is NULL or buffer size is 0.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D]  The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D]  The device is not open.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID  The blocking mode is invalid for this API.
 *              - #ADI_I2C_BUFFER_NOT_AVAILABLE   No internal buffer is available to store the submitted buffer.
 *
 *
 * @sa          adi_i2c_SubmitRxBuffer()
 */
ADI_I2C_RESULT adi_i2c_SubmitTxBuffer(
        ADI_I2C_HANDLE         const hDevice,
        void                        *pBuffer,
        uint32_t               const nBufSize,
        bool_t                 const bRestart)
{
    ADI_I2C_RESULT eResult;

    /* Submit to the common non-blocking submit API */
    eResult = I2C_NonBlockingSubmit(hDevice, OUTBOUND, pBuffer, nBufSize, bRestart);

    return eResult;
}


/**
 * @brief       Read data from a I2C device (non-blocking).
 *
 * This function returns immediately (does not wait for the read to complete).
 * An interrupt is generated when the transfer is complete or an error occurs.
 * The supplied buffer is owned by the driver during the data transfer process.
 * This buffer should not be modified or deleted by the application until the
 * data transfer is complete.  It's recommended that the buffer not be a
 * local variable.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 * @param [in]  pBuffer             The data buffer that will contain the received data.
 * @param [in]  nBufSize            The size (in bytes) of the data buffer.
 * @param [in]  bRestart            Issue a repeat start condition at the conclusion of the current transfer.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                Successfully submitted the buffer for reading.
 *              - #ADI_I2C_INVALID_PARAMETER [D]  The buffer pointer passes is NULL or buffer size is 0.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D] The device is not open.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID  The blocking mode is invalid for this API.
 *              - #ADI_I2C_BUFFER_NOT_AVAILABLE   No internal buffer is available to store the submitted buffer.
 *
 * @sa          adi_i2c_SubmitTxBuffer()
 */
ADI_I2C_RESULT adi_i2c_SubmitRxBuffer(
        ADI_I2C_HANDLE         const hDevice,
        void                        *pBuffer,
        uint32_t               const nBufSize,
        bool_t                 const bRestart)
{
    ADI_I2C_RESULT eResult;

    /* Submit to the common non-blocking submit API */
    eResult = I2C_NonBlockingSubmit(hDevice, INBOUND, pBuffer, nBufSize, bRestart);

    return eResult;
}

/**
 * @brief Checks if a Rx Buffer is available for processing.
 *
 * The Rx buffer is available if a completed Rx buffer is available in the driver.
 *
 * @param [in]  hDevice             Handle to the I2C device.
 * @param [out] pbAvailable         True if a completed Rx buffer is available, false otherwise.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                Successfully checked if the Rx Buffer is available for processing.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D] The device is not open.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID  The blocking mode is invalid for this API.
 */
ADI_I2C_RESULT adi_i2c_IsRxBufferAvailable(
                                           ADI_I2C_HANDLE    const hDevice,
                                           bool_t*           const pbAvailable
                                           )
{
    ADI_I2C_RESULT result = ADI_I2C_SUCCESS;
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (pbAvailable == NULL) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    /* Set thie default bAvailable value */
    *pbAvailable = false;

    /* IF (Mode of operation is non-blocking) */
    if (drv->pData->eBlockMode == BLOCK_MODE_NON_BLOCKING) {
        /* IF (Completed buffers are available in the completed buffer queue) */
        if (IS_BUFF_AVAIL(drv->pData->completedBuffers)) {
            /* IF (A completed buffer available is a Rx buffer) */
            if (drv->pData->completedBuffers->Direction == INBOUND) {
                *pbAvailable = true;
            }
        }
    }
    else {
        result = ADI_I2C_BLOCKING_MODE_INVALID;
    }

    return result;
}


/**
 * @brief Checks if the Tx Buffer is available for processing.
 *
 * The Tx buffer is available if no Tx buffer transfer is in progress.
 *
 * @param [in]  hDevice             Handle to the I2C device.
 * @param [out] pbAvailable         True if a Tx buffer transfer is not in progress, false otherwise.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                Successfully checked if the Tx Buffer is available for processing.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D] The device is not open.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID  The blocking mode is invalid for this API.
 */
ADI_I2C_RESULT adi_i2c_IsTxBufferAvailable(
        ADI_I2C_HANDLE    const hDevice,
        bool_t*           const pbAvailable)
{

    ADI_I2C_RESULT result = ADI_I2C_SUCCESS;
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (pbAvailable == NULL) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif


    /* Set thie default bAvailable value */
    *pbAvailable = false;

    /* IF (Mode of operation is non-blocking) */
    if (drv->pData->eBlockMode == BLOCK_MODE_NON_BLOCKING) {
        /* IF (Completed buffers are available in the completed buffer queue */
        if (IS_BUFF_AVAIL(drv->pData->completedBuffers)) {
            /* IF (A completed buffer available is a Tx buffer) */
            if (drv->pData->completedBuffers->Direction == OUTBOUND) {
                *pbAvailable = true;
            }
        }
    }
    else {
        result = ADI_I2C_BLOCKING_MODE_INVALID;
    }

    return result;
}


/**
 * @brief Return the Rx buffer if a filled buffer is available, otherwise
 *        wait until a buffer is filled. This function fails if a callback is
 *        already registered.
 *
 * @note This API is used for non-blocking mode only.
 *
 * @param [in]  hDevice       Handle to the I2C device.
 * @param [out] ppBuffer      The available Rx buffer.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                       Successfully returned a Rx buffer.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE         [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN           [D] The device is not open.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED        [D] Function cannot be called when the device is not enabled or
 *                                                       if a callback function is registered.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID         The blocking mode is invalid for this API.
 *              - #ADI_I2C_GLOBAL_EVENT_OCCURRED         A global event occured. Call adi_i2c_GetEvents()
 *                                                       to get the events.
 *              - #ADI_I2C_DIFF_BUFFER_TYPE_AVAILABLE    Tx buffer is available instead of an Rx buffer.
 *              - #ADI_I2C_NACK_ADDR                     Address NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_NACK_DATA                     Data NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_ARBITRATION_LOST              Arbitration is lost during the given buffer transfer.
 *
 * @sa adi_i2c_SubmitRxBuffer()
 */
ADI_I2C_RESULT adi_i2c_GetRxBuffer (
                    ADI_I2C_HANDLE   const   hDevice,
                    void                   **ppBuffer)
{
    ADI_I2C_RESULT result;

    /* Call the common non-blocking get function */
    result = I2C_NonBlockingGet (hDevice, INBOUND, ppBuffer);

    return result;
}

/**
 * @brief Return the Tx buffer if a filled buffer is available, otherwise
 *        wait until a buffer is filled.
 *
 * @note This API is used for non-blocking mode only.
 *
 * @param [in]  hDevice       Handle to the I2C device.
 * @param [out] ppBuffer      The available Tx buffer.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                       Successfully returned a Tx buffer.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE         [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN           [D] The device is not open.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED        [D] Function cannot be called when the device is not enabled or
 *                                                       if a callback function is registered.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID         The blocking mode is invalid for this API.
 *              - #ADI_I2C_GLOBAL_EVENT_OCCURRED         A global event occured. Call adi_i2c_GetEvents()
 *                                                       to get the events.
 *              - #ADI_I2C_DIFF_BUFFER_TYPE_AVAILABLE    Rx buffer is available instead of an Tx buffer.
 *              - #ADI_I2C_NACK_ADDR                     Address NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_NACK_DATA                     Data NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_ARBITRATION_LOST              Arbitration is lost during the given buffer transfer.
 *
 * @sa adi_i2c_SubmitTxBuffer()
 */
ADI_I2C_RESULT adi_i2c_GetTxBuffer (
                    ADI_I2C_HANDLE   const hDevice,
                    void                 **ppBuffer)
{
    ADI_I2C_RESULT result;

    /* Call the common non-blocking get function */
    result = I2C_NonBlockingGet (hDevice, OUTBOUND, ppBuffer);

    return result;
}

/*!
 * @brief This function is used to initiate I2C data transfers in non-blocking mode.
 *
 * This function is called after submitting one or two buffers.
 *
 * @note This API is used for non-blocking mode only.
 *
 * @param [in] hDevice        Handle to the I2C device.
 *
 * @param [in] bEnable        'true' to Enable and 'false' to disable the device.
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                 Successfully enabled data flow.
 *              - #ADI_I2C_INVALID_PARAMETER       If application try to enable when already enabled
 *                                                 or vice versa
 *              - #ADI_I2C_BAD_DEVICE_HANDLE   [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN     [D] The device is not open.
 *              - #ADI_I2C_BAD_BITRATE         [D] The bitrate is too low to be configured for the current
 *                                                 Peripheral clock.
 *
 * @sa adi_i2c_SubmitTxBuffer()
 * @sa adi_i2c_SubmitRxBuffer()
 */
ADI_I2C_RESULT adi_i2c_Enable (
                               ADI_I2C_HANDLE   const   hDevice,
                               bool_t           const   bEnable
                               )
{
    ADI_I2C_RESULT result;
    ADI_I2C_BUFFER_STRUCT* pBuff;
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (bEnable == drv->pData->bDeviceEnabled)
    {
        return ADI_I2C_INVALID_PARAMETER;
    }

#endif

    if (bEnable)
    {
        /* Setup the deivce */
        result = SetupDevice(drv);
        if (ADI_I2C_SUCCESS != result) {
          return result;
        }

        /* Enable the master before processing buffers */
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        if (drv->pData->config.mode == ADI_I2C_MASTER)
        {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
            EnableMaster(drv);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        }
        else
        {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
            EnableSlave(drv);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        }
#endif

        /* Process the buffers */
        result = ProcessBuffers(drv);
        if (ADI_I2C_SUCCESS != result) {
            return result;
        }

        /* Enable the device */
        drv->pData->bDeviceEnabled = true;
    }
    else
    {
        /* Stop the device */
        StopDevice(drv);

        /* Free all the active buffer */
        if (DATA->activeBuffer != NULL) {
             bpush_buffer(&DATA->freeBuffers, DATA->activeBuffer);
             DATA->activeBuffer = NULL;
        }

        /* Free all the pending buffers */
        if (IS_BUFF_AVAIL(DATA->pendingBuffers)) {
             pBuff = fpop_buffer(&DATA->pendingBuffers);
             bpush_buffer(&DATA->freeBuffers, pBuff);
        }

        /* Free all the completed buffers */
        if (IS_BUFF_AVAIL(DATA->completedBuffers)) {
             pBuff = fpop_buffer(&DATA->completedBuffers);
             bpush_buffer(&DATA->freeBuffers, pBuff);
        }

        /* Disable the device */
        drv->pData->bDeviceEnabled = false;

        /* Get the blocking mode as None */
        DATA->eBlockMode = BLOCK_MODE_NONE;
    }

    return ADI_I2C_SUCCESS;
}


/**
 * @brief    Get the ORed value of all the global events that occurred.
 *
 * @param [in]  hDevice    Handle to the I2C device.
 * @param [out] pnEvents   Pointer to the location where the ORed value will be written.
 *
 * @return   Status
 *              - #ADI_I2C_SUCCESS                 Successfully retrieved the global events
 *              - #ADI_I2C_INVALID_PARAMETER   [D] pnEvents is NULL
 *              - #ADI_I2C_BAD_DEVICE_HANDLE   [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN     [D] The device is not open.
 */
ADI_I2C_RESULT adi_i2c_GetEvents (
                                  ADI_I2C_HANDLE   const   hDevice,
                                  uint32_t*        const   pnEvents
                                  )
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
    if (pnEvents == NULL)
    {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    /* Get the global events */
    *pnEvents = CFG->nGlobalEvent;

    /* Clear the global events */
    CFG->nGlobalEvent = ADI_I2C_GLOBAL_EVENT_NONE;

    return ADI_I2C_SUCCESS;
}


/**
 * @brief    Get whether a global event has occurred.
 *
 * @param [in]  hDevice       Handle to the I2C Device
 * @param [out] pbAvailable   Pointer to location to which the buffer availability
 *                            flag will be written
 *
 * @return   Status
 *              - #ADI_I2C_SUCCESS                 Successfully retrieved the global event status
 *              - #ADI_I2C_INVALID_PARAMETER   [D] pbAvailable is NULL
 *              - #ADI_I2C_BAD_DEVICE_HANDLE   [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN     [D] The device is not open.
 *
 */
ADI_I2C_RESULT adi_i2c_IsEventAvailable (
                                         ADI_I2C_HANDLE   const hDevice,
                                         bool_t*          const pbAvailable
                                         )
{

    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
    if (pbAvailable == NULL)
    {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    /* Set the default return status as 'false' */
    *pbAvailable = false;

    /* IF (Global Event is available) */
    if (CFG->nGlobalEvent) {
        /* Set the return status as 'true' */
        *pbAvailable = true;
    }

    /* Return success */
    return ADI_I2C_SUCCESS;
}


/**
 * @brief       Write data to a I2C device (blocking).
 *
 * This function blocks (does not return) until the write is complete.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 * @param [in]  pBuffer             The buffer that contains the data to transmit.
 * @param [in]  nBufSize            The number of bytes to write.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                  Successfully submitted the buffer for writing.
 *              - #ADI_I2C_INVALID_PARAMETER        pBuffer is NULL or nBufSize is 0
 *              - #ADI_I2C_BLOCKING_MODE_INVALID    The blocking mode is invalid for this API.
 *              - #ADI_I2C_BUFFER_NOT_AVAILABLE     Internal buffers are not available to store the given buffer.
 *              - #ADI_I2C_NACK_ADDR                Address NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_NACK_DATA                Data NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_ARBITRATION_LOST         Arbitration is lost during the given buffer transfer.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D]    The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D]    The device is not open.
 *
 * @sa          adi_i2c_Read()
 */
ADI_I2C_RESULT adi_i2c_Write(
    ADI_I2C_HANDLE         const hDevice,
    void                        *pBuffer,
    uint32_t               const nBufSize
    )
{
    ADI_I2C_RESULT          result;

    /* Call the blocking IO function to submit the buffer */
    result = I2C_BlockingIO(hDevice, OUTBOUND, pBuffer, nBufSize);

    return result;
}


/**
 * @brief       Read data from a I2C device (blocking).
 *
 * This function blocks (does not return) until the read is complete.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 * @param [out] pBuffer             The data buffer that will contain the received data.
 * @param [in]  nBufSize            The size (in bytes) of the data buffer.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS                 Successfully submitted the buffer for reading.
 *              - #ADI_I2C_BLOCKING_MODE_INVALID   The blocking mode is invalid for this API.
 *              - #ADI_I2C_BUFFER_NOT_AVAILABLE    Internal buffers are not available to store the given buffer.
 *              - #ADI_I2C_NACK_ADDR               Address NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_NACK_DATA               Data NACK occurred during the given buffer transfer.
 *              - #ADI_I2C_ARBITRATION_LOST        Arbitration is lost during the given buffer transfer.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D]   The device is not open.
 *
 * @sa          adi_i2c_Write()
 */
ADI_I2C_RESULT adi_i2c_Read(
    ADI_I2C_HANDLE         const hDevice,
    void                        *pBuffer,
    uint32_t               const nBufSize
    )
{
    ADI_I2C_RESULT          result;

    /* Call the blocking IO function to submit the buffer */
    result = I2C_BlockingIO(hDevice, INBOUND, pBuffer, nBufSize);

    return result;
}



/**
 * @brief    Set the Hardware Address Width
 *
 * @param [in]  hDevice      Handle to the I2C device
 * @param [in]  eAddrWidth   The hardware address width enum
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                 Successfully set the hardware address width.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D]   The device is not open.
 */
ADI_I2C_RESULT adi_i2c_SetHWAddressWidth (
                                          ADI_I2C_HANDLE         const hDevice,
                                          ADI_I2C_HWADDR_WIDTH   const eAddrWidth
                                          )
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
#endif

    /* Set the hardware width in the device */
    if (eAddrWidth == ADI_I2C_HWADDR_WIDTH_10_BITS)
    {
        CFG->b10bitSlaveAddr = true;
    }
    else
    {
        CFG->b10bitSlaveAddr = false;
    }

    return ADI_I2C_SUCCESS;
}

/**
 * @brief    Enable/Disable General Call in the slave device
 *
 * @param [in]  hDevice   Handle to the I2C Device
 * @param [in]  bEnable   'true' to enable and 'false' to disable general call
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                  Successfully enabled/disabled general call.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED [D]   Device is enabled or Mode of operation is not slave.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D]   The device is not open.
 *
 */
ADI_I2C_RESULT adi_i2c_EnableGeneralCall(
    ADI_I2C_HANDLE        const hDevice,
    bool_t                  const bEnable
)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (   (CFG->mode != ADI_I2C_SLAVE)
        || (DATA->bDeviceEnabled))
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

    /* IF (flag is true) */
    if (bEnable) {
        /* Set the flag in the internal control variable */
        SET_BITS_U16(CFG->nRegControl, BITM_I2C_SCTL_GCEN);
    } else {
        /* Clear the flag in the internal control variable */
        CLR_BITS_U16(CFG->nRegControl, BITM_I2C_SCTL_GCEN);
    }

    return ADI_I2C_SUCCESS;
}


/*
 *
 */


/**
 * @brief    Set the alternate Device ID required for Hardware General Call
 *
 * @param [in]  hDevice      Handle o the I2C Device
 * @param [in]  nDeviceID    Alternate Device ID
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                  Successfully set the alternate device ID.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED [D]   Device is enabled or Mode of operation is not slave.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D]   The device is not open.
 *
 */
ADI_I2C_RESULT adi_i2c_SetAlternateDevID(
    ADI_I2C_HANDLE         const hDevice,
    uint8_t                const nDeviceID
)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;
#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (   (CFG->mode != ADI_I2C_SLAVE)
        || (DATA->bDeviceEnabled))
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

    /* Set the alternate device ID */
    REG(ALT) = nDeviceID;

    return ADI_I2C_SUCCESS;
}

/**
 * @brief    Sets the maximum automatic SCL clock stretching
 *
 * @param [in]  hDevice        Handle to the I2C Device
 * @param [in]  nExpBitTimes   '0' disables clock stretching and for a value
 *                             greater than that the clock stretching will be
 *                             2^nExpBitTimes.
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                  Successfully set automatic clock stretching value.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D]   The device is not open.
 *
 */
ADI_I2C_RESULT adi_i2c_SetAutoStretch (
                                       ADI_I2C_HANDLE const hDevice,
                                       uint8_t        const nExpBitTimes
                                       )
{

    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
    if (nExpBitTimes > 15u) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */

    /* IF (Mode of operation is master) */
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        /* Set the stretch mode for master */
        REG(ASTRETCH_SCL) = (uint16_t)(REG(ASTRETCH_SCL) & BITM_I2C_ASTRETCH_SCL_MST) | (uint16_t)((uint16_t)nExpBitTimes << BITP_I2C_ASTRETCH_SCL_MST);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        /* Set the stretch mode for slave */
        REG(ASTRETCH_SCL) = (uint16_t)(REG(ASTRETCH_SCL) & BITM_I2C_ASTRETCH_SCL_SLV) | (uint16_t)((uint16_t)nExpBitTimes << BITP_I2C_ASTRETCH_SCL_SLV);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    return ADI_I2C_SUCCESS;
}



/**
 * @brief       Set the I2C device address.
 *
 * In master mode this is the address transmitted to the slave device.
 * In slave mode this is the address that the slave responds to.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 * @param [in]  Address             In master mode this is the address transmitted to the slave device.
 *                                  In slave mode this is the address that the slave responds to.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS               Successfully set the address.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D] The device is not open.
 */
ADI_I2C_RESULT adi_i2c_SetHardwareAddress(
        ADI_I2C_HANDLE         const hDevice,
        uint16_t               const Address)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
#endif

    /* Set the hardware address */
    drv->pData->config.slaveAddr = Address;

    return ADI_I2C_SUCCESS;
}



/**
 * @brief       Set the I2C serial clock bit rate.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 * @param [in]  BitRate             The serial clock bit rate in kHz.
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS               Successfully set the bit rate.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D] The device is not open.
 *              - #ADI_I2C_BAD_BITRATE       [D] The given bit rate is invalid.
 */
ADI_I2C_RESULT adi_i2c_SetBitRate(
                                  ADI_I2C_HANDLE         const hDevice,
                                  uint16_t               const BitRate
                                  )
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (DATA->bDeviceEnabled)
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }

    if (BitRate == 0u)
    {
        return ADI_I2C_BAD_BITRATE;
    }
#endif

    /* Set the frequency in the device */
    CFG->frequency = (uint32_t)BitRate * 1000u;

    return ADI_I2C_SUCCESS;
}

/**
 * @brief       Set the I2C duty cycle.
 *
 * @param [in]  hDevice             The handle to the I2C device.
 * @param [in]  DutyCycle           The serial clock duty cycle (valid range 1-99).
 *
 * @return      Status
 *              - #ADI_I2C_SUCCESS               Successfully set the duty cycle.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE [D] The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN   [D] The device is not open.
 *              - #ADI_I2C_BAD_DUTYCYCLE     [D] The given duty cycle is invalid.
 */
ADI_I2C_RESULT adi_i2c_SetDutyCycle(
        ADI_I2C_HANDLE         const hDevice,
        uint16_t               const DutyCycle)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (DATA->bDeviceEnabled)
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }

    /* check the requested I2C duty cycle values */
    if ( (DutyCycle < 1u) || (DutyCycle > 99u) )
    {
        return ADI_I2C_BAD_DUTYCYCLE;
    }
#endif

    /* Set the duty cycle */
    CFG->duty_cycle = DutyCycle;

    return ADI_I2C_SUCCESS;
}


/**
 * @brief    Enable/Disable Internal Loopback
 *
 * @param [in]  hDevice   Handle to the I2C Device
 * @param [in]  bEnable   'true' to Enable and 'false' to disable internal loopback
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                  Successfully enable/disable internal loopback.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED [D]   Device is already enabled.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D]   The device is not open.
 *
 */
ADI_I2C_RESULT adi_i2c_EnableInternalLoopback (
                                               ADI_I2C_HANDLE const hDevice,
                                               bool_t           const bEnable)
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (DATA->bDeviceEnabled)
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

    if (bEnable)
    {
        /* Set loopback bit in the internal control variable */
        SET_BITS_U16(CFG->nRegControl, BITM_I2C_MCTL_LOOPBACK);
    }
    else
    {
        /* Clear loopback bit in the internal control variable */
        CLR_BITS_U16(CFG->nRegControl, BITM_I2C_MCTL_LOOPBACK);
    }


    return ADI_I2C_SUCCESS;
}

#if ((ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1) && (ADI_I2C_CFG_ENABLE_PIO_SUPPORT == 1))
/**
 * @brief    Enable/Disable DMA
 *
 * @param [in]  hDevice   Handle to the I2C Device
 * @param [in]  bEnable   'true' to Enable and 'false' to disable DMA
 *
 * @return  Status
 *              - #ADI_I2C_SUCCESS                  Successfully enable/disable DMA.
 *              - #ADI_I2C_MODE_NOT_SUPPORTED [D]   Device is already enabled.
 *              - #ADI_I2C_BAD_DEVICE_HANDLE  [D]   The device handle is invalid.
 *              - #ADI_I2C_DEVICE_NOT_OPEN    [D]   The device is not open.
 *
 */
ADI_I2C_RESULT adi_i2c_EnableDMA (
                                  ADI_I2C_HANDLE const hDevice,
                                  bool_t           const bEnable
                                  )
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (DATA->bDeviceEnabled)
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

    if (bEnable)
    {
        /* Enable the DMA */
        CFG->bDmaEnabled = true;
        DATA->pfManageFIFO = DMAManageFIFO;
    }
    else
    {
        /* Disable the DMA */
        CFG->bDmaEnabled = false;
        DATA->pfManageFIFO = PioManageFIFO;
    }

    return ADI_I2C_SUCCESS;
}
#endif

/******************************************************
 Internal functions:
 *******************************************************/

/* Push a buffer to the back of a buffer list */
static void bpush_buffer(ADI_I2C_BUFFER_STRUCT** pList, ADI_I2C_BUFFER_STRUCT* pBuffer)
{
    ASSERT (pBuffer->pNext == NULL);

    ADI_ENTER_CRITICAL_REGION();
    if (*pList == NULL) {
        *pList = pBuffer;
    } else {
        ADI_I2C_BUFFER_STRUCT* List = *pList;
        while (List->pNext != NULL) {
            List = List->pNext;
        }

        ASSERT (List->pNext == NULL);
        List->pNext = pBuffer;
    }

    pBuffer->pNext = NULL;
    ADI_EXIT_CRITICAL_REGION();
}

/* Pop a buffer from the front of a buffer list */
static ADI_I2C_BUFFER_STRUCT* fpop_buffer(ADI_I2C_BUFFER_STRUCT** pList)
{
    ADI_I2C_BUFFER_STRUCT *buff;

    ADI_ENTER_CRITICAL_REGION();
    buff = *pList;
    *pList = (*pList)->pNext;
    buff->pNext = NULL;
    ADI_EXIT_CRITICAL_REGION();

    return buff;
}


/*
 * Initialize the driver.
 */
static void Initialize(ADI_I2C_MODE const  eMode,ADI_I2C_DRIVER *drv)
{
    int32_t x;

    /* Clear the driver data structure */
    memset(drv->pData, 0, sizeof(ADI_I2C_DATA_STRUCT));

    /* Chain the buffers */
    drv->pData->freeBuffers   = &drv->pData->Buffers[0];
    for (x = 0; x < (NUM_I2C_BUFFERS-1); x++)
    {
        drv->pData->Buffers[x].pNext = &drv->pData->Buffers[x+1];
    }
    drv->pData->Buffers[NUM_I2C_BUFFERS-1].pNext = NULL;

    /* Deafult Variables */
    CFG->frequency = ADI_CFG_I2C_BIT_RATE_IN_KHZ * 1000u;
    CFG->duty_cycle = ADI_CFG_I2C_DUTY_CYCLE;
    drv->pData->bDeviceEnabled = false;

#if (ADI_I2C_CFG_ENABLE_DMA == 1)
    CFG->bDmaEnabled = true;
    DATA->pfManageFIFO = DMAManageFIFO;
#else
    CFG->bDmaEnabled = false;
    DATA->pfManageFIFO = PioManageFIFO;
#endif


#if (ADI_CFG_I2C_HW_ADDR_LEN_IN_BITS == 10)
    CFG->b10bitSlaveAddr = true;
#endif

    CFG->nRegControl = 0u;
#if (ADI_CFG_I2C_ENABLE_GENERAL_CALL == 1)
    SET_BITS_U16(CFG->nRegControl, BITM_I2C_SCTL_GCEN);
#endif

    REG(ALT) = ADI_CFG_I2C_ALT_DEV_ID;
    CFG->slaveAddr = ADI_CFG_I2C_HWADDR;

    if (eMode == ADI_I2C_MASTER) {
        REG(ASTRETCH_SCL) =  (uint16_t)(REG(ASTRETCH_SCL) & BITM_I2C_ASTRETCH_SCL_MST)
                           | (uint16_t)((uint16_t)ADI_CFG_I2C_MASTER_AUTO_STRETCH_EXP_LEN << BITP_I2C_ASTRETCH_SCL_MST);
    } else {
        REG(ASTRETCH_SCL) = (uint16_t)(REG(ASTRETCH_SCL) & BITM_I2C_ASTRETCH_SCL_SLV)
                           | (uint16_t)((uint16_t)ADI_CFG_I2C_SLAVE_AUTO_STRETCH_EXP_LEN << BITP_I2C_ASTRETCH_SCL_SLV);
    }

    ADI_INSTALL_HANDLER(I2C_MST_EVT_IRQn, I2C0_Master_Int_Handler);
    ADI_INSTALL_HANDLER(I2C_SLV_EVT_IRQn, I2C0_Slave_Int_Handler);
}

/* Non-blocking Get */
static  ADI_I2C_RESULT I2C_NonBlockingGet (
                                           ADI_I2C_HANDLE   const   hDevice,
                                           uint8_t            const  nDirection,
                                           void                   **ppBuffer
                                           )
{
    ADI_I2C_RESULT result = ADI_I2C_SUCCESS;
    uint32_t Info = BUFF_INFO_NONE;
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }
    if ((!drv->pData->bDeviceEnabled) || (drv->pData->Callback != NULL))
    {
        return ADI_I2C_MODE_NOT_SUPPORTED;
    }
#endif

    if (drv->pData->eBlockMode == BLOCK_MODE_BLOCKING) {
        return ADI_I2C_BLOCKING_MODE_INVALID;
    }

    while (1)
    {
        /* IF (A global event occurred) */
        if (CFG->nGlobalEvent) {
            return ADI_I2C_GLOBAL_EVENT_OCCURRED;
        }

        /* IF(Completed buffers are available) */
        if (IS_BUFF_AVAIL(DATA->completedBuffers))  {
            break;
        }

        /* Pend for the semaphore */
        PEND_EVENT(DATA,ADI_I2C_ERR_RTOS);
    }

    /* IF (completed buffer is of required direction) */
    if (DATA->completedBuffers->Direction == nDirection) {
        ADI_I2C_BUFFER_STRUCT *pBuffer;

        pBuffer = fpop_buffer(&DATA->completedBuffers);
        *ppBuffer = pBuffer->pData;
        Info = pBuffer->Info;
        result = (pBuffer->BytesRemaining == 0u) ? ADI_I2C_SUCCESS : ADI_I2C_PARTIAL_BUFFER;
        bpush_buffer(&DATA->freeBuffers, pBuffer);


    }
    else
    {
        result = ADI_I2C_DIFF_BUFFER_TYPE_AVAILABLE;
    }

    /* IF (There is any buffer errors) */
    if ((Info & BUFF_INFO_ERR) != 0u)
    {
        if ((Info & BUFF_INFO_NACK_ADDR) == BUFF_INFO_NACK_ADDR) {
            result = ADI_I2C_NACK_ADDR;
        }

        if ((Info & BUFF_INFO_NACK_DATA) == BUFF_INFO_NACK_DATA) {
            result = ADI_I2C_NACK_DATA;
        }

        if ((Info & BUFF_INFO_NACK_ALOST) == BUFF_INFO_NACK_ALOST) {
            result = ADI_I2C_ARBITRATION_LOST;
        }
    }

    return result;
}


/* Blocking IO function */
static ADI_I2C_RESULT I2C_BlockingIO (
                                      ADI_I2C_HANDLE     const hDevice,
                                      uint8_t            const  nDirection,
                                      void                     *pBuffer,
                                      uint32_t           const  nBufSize
                                      )
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;
    ADI_I2C_BUFFER_STRUCT*  pBuff;
    ADI_I2C_RESULT          result;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }

    if ((drv->pData == NULL) || (nBufSize == 0u))
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if (pBuffer == NULL) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    if (drv->pData->eBlockMode != BLOCK_MODE_NONE) {
        return ADI_I2C_BLOCKING_MODE_INVALID;
    }

    if (nBufSize == 0u) {
        return ADI_I2C_INVALID_PARAMETER;
    }

    drv->pData->eBlockMode = BLOCK_MODE_BLOCKING;

    if (IS_BUFF_AVAIL(drv->pData->freeBuffers))
    {
        /* Get a free buffer */
        pBuff = fpop_buffer(&DATA->freeBuffers);

        /* Fill in the buffer details */
        pBuff->pData = pBuffer;
        pBuff->pCurData = pBuffer;
        pBuff->BytesRemaining = (uint16_t)nBufSize;
        pBuff->Info = BUFF_INFO_NONE;
        pBuff->Direction = nDirection;
        pBuff->Restart = false;
        pBuff->FastProcess = false;

        /* Submit the buffer to the pending buffers to be processed */
        bpush_buffer(&DATA->pendingBuffers,pBuff);

    }
    else
    {
        return ADI_I2C_BUFFER_NOT_AVAILABLE;
    }

    /* Enable the device */
    result = adi_i2c_Enable(hDevice, true);
    if (result != ADI_I2C_SUCCESS) {
        return result;
    }

    /* Pend for the semaphore */
     PEND_EVENT(DATA,ADI_I2C_ERR_RTOS);

    /* Disable the device */
    result = adi_i2c_Enable(hDevice, false);
    if (result != ADI_I2C_SUCCESS) {
        return result;
    }

    result = (pBuff->BytesRemaining == 0u) ? ADI_I2C_SUCCESS : ADI_I2C_PARTIAL_BUFFER;

    /* Determine whether any errors have occurred during the processing */
    if ((pBuff->Info & BUFF_INFO_ERR) != 0u)
    {
        if ((pBuff->Info & BUFF_INFO_NACK_ADDR) == BUFF_INFO_NACK_ADDR) {
            result = ADI_I2C_NACK_ADDR;
        }

        if ((pBuff->Info & BUFF_INFO_NACK_DATA) == BUFF_INFO_NACK_DATA) {
            result = ADI_I2C_NACK_DATA;
        }

        if ((pBuff->Info & BUFF_INFO_NACK_ALOST) == BUFF_INFO_NACK_ALOST) {
            result = ADI_I2C_ARBITRATION_LOST;
        }
    }

    drv->pData->eBlockMode = BLOCK_MODE_NONE;

    return result;
}

/* Non-blocking submit */
static ADI_I2C_RESULT I2C_NonBlockingSubmit (
                                             ADI_I2C_HANDLE    const hDevice,
                                             uint8_t           const nDirection,
                                             void                   *pBuffer,
                                             uint32_t          const nBufSize,
                                             bool_t              const bRestart
                                             )
{
    ADI_I2C_DRIVER *drv = (ADI_I2C_DRIVER *)hDevice;

#if defined(ADI_DEBUG)
    if (!IsDeviceHandle(drv))
    {
        return ADI_I2C_BAD_DEVICE_HANDLE;
    }
    if (drv->pData == NULL)
    {
        return ADI_I2C_DEVICE_NOT_OPEN;
    }

    if ((pBuffer == NULL) || (nBufSize == 0u)) {
        return ADI_I2C_INVALID_PARAMETER;
    }
#endif

    if (drv->pData->eBlockMode == BLOCK_MODE_BLOCKING) {
        return ADI_I2C_BLOCKING_MODE_INVALID;
    }


    if (IS_BUFF_AVAIL(drv->pData->freeBuffers))
    {
        ADI_I2C_BUFFER_STRUCT* pBuff;

        pBuff = fpop_buffer(&DATA->freeBuffers);

        pBuff->pData = pBuffer;
        pBuff->pCurData = pBuffer;
        pBuff->BytesRemaining = (uint16_t)nBufSize;
        pBuff->Info = BUFF_INFO_NONE;
        pBuff->Direction = nDirection;
        pBuff->Restart = bRestart;
        pBuff->FastProcess = bRestart;

        bpush_buffer(&DATA->pendingBuffers,pBuff);

    }
    else
    {
        return ADI_I2C_BUFFER_NOT_AVAILABLE;
    }

    if (drv->pData->eBlockMode == BLOCK_MODE_NONE) {
        drv->pData->eBlockMode = BLOCK_MODE_NON_BLOCKING;
    }

    if (DATA->bDeviceEnabled) {
        ProcessBuffers(drv);
    }

    return ADI_I2C_SUCCESS;
}

/* Stop the device */
static void StopDevice(ADI_I2C_DRIVER *drv)
{
    /* Disable the interrupts in the device */
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        /* disable complete,rx and tx interrupt */
        CLR_REG_BITS(MCTL,  BITM_I2C_MCTL_IENCMP
                          | BITM_I2C_MCTL_IENMRX
                          | BITM_I2C_MCTL_IENMTX
                          | BITM_I2C_MCTL_MTXDMA
                          | BITM_I2C_MCTL_MRXDMA
                          );
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        /* Disable all the slave interrupts */
        CLR_REG_BITS(SCTL,  BITM_I2C_SCTL_IENSTOP
                          | BITM_I2C_SCTL_IENSRX
                          | BITM_I2C_SCTL_IENSTX
                          | BITM_I2C_SCTL_STXDMA
                          | BITM_I2C_SCTL_SRXDMA
                          );
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    /* Disable the NVIC */
    configureNVIC(drv,false);

    /* Close the DMA */
#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
    UnInitDma(drv);
#endif

    /* Flush the Tx and Rx buffers */
    FlushTx(drv);
    FlushRx(drv);

    /* Disable the device */
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (drv->pData->config.mode == ADI_I2C_MASTER) {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        CLR_REG_BITS(MCTL, BITM_I2C_MCTL_MASEN);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    } else {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        CLR_REG_BITS(SCTL, BITM_I2C_SCTL_SLVEN);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif
}

/* Enable/Disable the given interrupt */
static void EnableInterrupt(IRQn_Type nIRQNum, bool_t bEnable) {
    if (bEnable) {
        ADI_ENABLE_INT(nIRQNum);
    } else {
        ADI_DISABLE_INT(nIRQNum);
    }
}

/* Configure the NVIC */
static void configureNVIC(ADI_I2C_DRIVER const *drv, bool_t bEnable)
{
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (drv->pData->config.mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        EnableInterrupt(I2C_MST_EVT_IRQn,bEnable);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        EnableInterrupt(I2C_SLV_EVT_IRQn,bEnable);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif
}

/* Configure the device */
static ADI_I2C_RESULT ConfigureDevice (ADI_I2C_DRIVER* drv)
{
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (drv->pData->config.mode == ADI_I2C_MASTER) {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        uint32_t incoming_clock;
        uint8_t  ltime;
        uint8_t  htime;
        uint32_t  ltime32;
        uint32_t  htime32;
        ADI_PWR_RESULT ePwrResult;

        ePwrResult = adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &incoming_clock);

        /* IF (Error in getting the incoming clock) */
        if (ePwrResult != ADI_PWR_SUCCESS) {
            /* Set default o highest clock of 26Mhz */
            incoming_clock = 26u * 1000u * 1000u;
        }

        /* Calculate the low clock time */
        ltime32 = (uint32_t) (((incoming_clock * (100u - (uint32_t)CFG->duty_cycle) / (CFG->frequency * 100u)) - 1u));

        /* Calculate the high clock time */
        htime32 = (uint32_t) (((incoming_clock * CFG->duty_cycle / (CFG->frequency * 100u)) - 2u));

#ifdef ADI_DEBUG
        if ((ltime32 > 255u) || (htime32 > 255u))
        {
           return ADI_I2C_BAD_BITRATE;
        }
        /* Annomaly Work Around */
        if ((ltime32 + htime32) < 16u) 
        {
            return ADI_I2C_BAD_BITRATE;
        }
#endif
        ltime = (uint8_t)ltime32;
        htime = (uint8_t)htime32;

        /* Set the high and low fields */
        drv->pReg->DIV = (uint16_t)(((uint16_t)htime) << 8u) | (uint16_t)ltime;

        /* Set the common configuration bits for the master */
        drv->pReg->MCTL = COMMON_CFG_MBITS | CFG->nRegControl;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    } else {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        uint16_t slaveAddr = drv->pData->config.slaveAddr;

        if (drv->pData->config.b10bitSlaveAddr) {
            /* 10-bit slave addressing spans two registers... */

            /* 1st addr reg gets the upper-two address bits + binary code '11110'    */
            /* so '00000000011110xx', where 'xx' is the upper two address bits.      */
            /* NOTE absence of single-bit left-shift (used in Master-Mode) to encode */
            /* r/w bit... there IS no r/w bit to encode in Slave-Mode.               */
            drv->pReg->ID0 = 0x0078u | ((slaveAddr & 0x0300u) >> 8);

            /* 2nd addr reg gets lower 8-bits of address */
            drv->pReg->ID1 = slaveAddr & 0x00ffu;
        } else {
            /* seven-bit address (upshifted by one) without r/w bit (don't care) */
            drv->pReg->ID0 = (slaveAddr & 0x007fu) << 1u;
        }

        /* Set the common configuration bits for the slave */
        drv->pReg->SCTL = COMMON_CFG_SBITS | CFG->nRegControl;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    return ADI_I2C_SUCCESS;
}

/*
 * Starts the I2C device
 *      -   uses the config part of the data struct to setup the I2C
 *      -   set FAST mode if SCL is > 100 kHz
 */
static ADI_I2C_RESULT SetupDevice(ADI_I2C_DRIVER *drv)
{
    ADI_I2C_RESULT result = ADI_I2C_SUCCESS;

    /* check to see if its already started */
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (drv->pData->config.mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
       if ((drv->pReg->MCTL & BITM_I2C_MCTL_MASEN) != 0u) {
            return ADI_I2C_SUCCESS;
       }
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        if ((drv->pReg->SCTL & BITM_I2C_SCTL_SLVEN) != 0u) {
            return ADI_I2C_SUCCESS;
       }
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    /* Flush the Tx and Rx FIFO */
    FlushTx(drv);
    FlushRx(drv);

    /* Configure the Device */
    if (ADI_I2C_SUCCESS != (result = ConfigureDevice(drv))) {
        return result;
    }

    /* Enable the NVIC */
    configureNVIC(drv, true);

    return result;
}

/* Enable the master */
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
static void EnableMaster (ADI_I2C_DRIVER const *drv)
{
    /* Enter Critical Region */
    ADI_ENTER_CRITICAL_REGION();

    /* Enable the Master */
    drv->pReg->MCTL |= BITM_I2C_MCTL_MASEN;

    /* Exit Critical Region */
    ADI_EXIT_CRITICAL_REGION();
}
#endif

/* Enable the Slave */
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
static void EnableSlave (ADI_I2C_DRIVER const *drv)
{
    /* Enter Critical Region */
    ADI_ENTER_CRITICAL_REGION();

    /* Enable the slave */
    drv->pReg->SCTL |= BITM_I2C_SCTL_SLVEN;

    /* Exit Critical Region */
    ADI_EXIT_CRITICAL_REGION();
}
#endif

/* Flush the Tx Buffer */
static void FlushTx(ADI_I2C_DRIVER const *drv)
{
    ADI_ENTER_CRITICAL_REGION();

#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        /* flush both master master tx fifo's */
        drv->pReg->STAT |= BITM_I2C_STAT_MFLUSH;
        drv->pReg->STAT &= (uint16_t)~BITM_I2C_STAT_MFLUSH;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        /* flush both master and slave tx fifo's */
        drv->pReg->STAT |= BITM_I2C_STAT_SFLUSH;
        drv->pReg->STAT &= (uint16_t)~BITM_I2C_STAT_SFLUSH;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    ADI_EXIT_CRITICAL_REGION();
}

/* Flush the Rx Buffer */
static void FlushRx(ADI_I2C_DRIVER const *drv)
{
    uint16_t volatile UNUSED dummyRead;

    ADI_ENTER_CRITICAL_REGION();

#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        /* "flush" master receive FIFO by dummy reads (no explicit flush bits for MRX) */
        dummyRead = drv->pReg->MRX;
        dummyRead = drv->pReg->MRX;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    } else {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        /* "flush" slave receive FIFO by dummy reads (no explicit flush bits for SRX) */
        dummyRead = drv->pReg->SRX;
        dummyRead = drv->pReg->SRX;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    ADI_EXIT_CRITICAL_REGION();
}




/* Start the master transfer */
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
static void CommenceMasterTransfer(ADI_I2C_DRIVER const *drv, uint16_t dir)
{
    uint16_t slaveAddr = drv->pData->config.slaveAddr;

    /*
        Initiates a master-mode transfer.
        Program the master control byte with control code, chip select bits (slaveID)
        and r/w bit.  Call this AFTER device enable because writing ADR0 is what actually
        initiates the transfer.  Note that in 10-bit addressing mode, ADR1 write MUST occur
        BEFORE writing ADR0.

        Note that repeat starts are generated by rewriting the first master address byte register in
        the first master-mode TX/Rx interrupt... so save it as firstMasterAddressByte for later.
    */

    if (drv->pData->config.b10bitSlaveAddr) {

        /* 10-bit slave addressing spans two registers... */

        /* 2nd addr reg gets the lower-order 8 bits of slave address */
        drv->pReg->ADDR2 = slaveAddr  & 0x00FFu;

        /*   1st addr reg gets "0000000011110xxy"b
         * + upper two ID bits (9:8) upshifted by one (xx)
         * + r/w bit in LS position (y)
         */
        drv->pData->config.firstMasterAddressByte =  ( 0x00F0u | ((slaveAddr & 0x0300u) >> (8-1)) )
                                                   | (uint16_t)((dir == (uint16_t)OUTBOUND ? (uint16_t)0x00000u : (uint16_t)0x0001u));

    } else {

        /* seven-bit address (upshifted by one) + w/r bit */
        drv->pData->config.firstMasterAddressByte =  ((slaveAddr & 0x007Fu) << 1u)
                                                   | (dir == OUTBOUND ? (uint16_t)0x00000 : (uint16_t)0x0001);
    }

    /* this is the actual launch */
    drv->pReg->ADDR1 = drv->pData->config.firstMasterAddressByte;
}
#endif





#if (ADI_I2C_CFG_ENABLE_PIO_SUPPORT == 1)

/* Read the Rx FIFO in PIO mode */
static void ReadRxFifo(ADI_I2C_DRIVER const *drv)
{
    ASSERT ((CFG->mode == ADI_I2C_MASTER) || (CFG->mode == ADI_I2C_SLAVE));

    uint16_t nMask;
    volatile const uint16_t* FIFOReg;

#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER) {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        FIFOReg = &REG(MRX);
        nMask = BITM_I2C_STAT_MRXF;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    } else  {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        FIFOReg = &REG(SRX);
        nMask = BITM_I2C_STAT_SRXF;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    /* Read data from FIFO till empty */
    while (BMASK(REG(STAT),nMask) != 0u) {
        if (ABUFF->BytesRemaining > 0u) {
            /* Read the FIFO Reg */
            *ABUFF->pCurData = (uint8_t)*FIFOReg;
            ABUFF->pCurData++;
            ABUFF->BytesRemaining--;
           } else {
            /* If you reached the end of the buffer then exit */
            break;
        }

#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        if ((CFG->mode == ADI_I2C_MASTER) && (CFG->MRxCountRemaining > 256u)) {
            if (REG(MCRXCNT) == 0u) {
                CFG->MRxCountRemaining = CFG->MRxCountRemaining - 256u;
                if (CFG->MRxCountRemaining > 256u) {
                    REG(MRXCNT) = 0x100u;
                } else {
                    REG(MRXCNT) = (uint16_t)(CFG->MRxCountRemaining - 1u);
                }
            }
        }
#endif

    }
}


/* Fill the transmit FIFO in PIO mode */
static void FillTransmitFifo(ADI_I2C_DRIVER const *drv)
{
   ASSERT ((CFG->mode == ADI_I2C_MASTER) || (CFG->mode == ADI_I2C_SLAVE));

    uint16_t nFreeBytes, nMask;
    int32_t nPos;
    volatile uint16_t* FIFOReg;

#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        FIFOReg = &REG(MTX);
        nMask = BITM_I2C_STAT_MTXF;
        nPos = BITP_I2C_STAT_MTXF;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        FIFOReg = &REG(STX);
        nMask = BITM_I2C_STAT_STXF;
        nPos = BITP_I2C_STAT_STXF;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    /* Get the number of free bytes available in the master/slave buffer */
    nFreeBytes = 2u - (BMASK(REG(STAT),nMask) >> nPos);

    while (nFreeBytes != 0u) {
        nFreeBytes--;
        if (ABUFF->BytesRemaining > 0u) {
            *FIFOReg = *ABUFF->pCurData;
            ABUFF->pCurData++;
            ABUFF->BytesRemaining--;
        } else {
            /* If you reached the end of the buffer then exit */
            break;
        }
    }
}

/* Process the FIFO for PIO mode */
static ADI_I2C_RESULT PioManageFIFO(ADI_I2C_DRIVER const *drv, bool_t bStart)
{
    if (ABUFF->Direction == OUTBOUND)
    {
        /* Fill the Transmit Buffer */
        FillTransmitFifo(drv);
    }
    else
    {
        if (!bStart)
        {
            ReadRxFifo(drv);
        }
        else
        {
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
            if (CFG->mode == ADI_I2C_MASTER) {
                CFG->MRxCountRemaining = 0u;
                if (ABUFF->BytesRemaining > 256u) {
                    CFG->MRxCountRemaining = ABUFF->BytesRemaining;
                    REG(MRXCNT) = 0x100u;
                } else {
                    REG(MRXCNT) = (uint16_t)(ABUFF->BytesRemaining - 1u);
                }
            }
#endif
        }
    }

    return ADI_I2C_SUCCESS;
}
#endif


static void DMAMasterHandler (void *pcbParam, uint32_t Event, void *pArg);
static void DMASlaveTxHandler (void *pcbParam, uint32_t Event, void *pArg);
static void DMASlaveRxHandler (void *pcbParam, uint32_t Event, void *pArg);

/* Process the FIFO for DMA mode */
#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_I2C_RESULT DMAManageFIFO(ADI_I2C_DRIVER const *drv, bool_t bStart)
{
    ADI_DMA_CHANNEL_ID eChannelID;
    ADI_CALLBACK       pfCallback;
    void              *pCBParam;

    /* Initialize the DMA is not initialized yet */
    if (!CFG->bDmaInitialized) {
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        if (CFG->mode == ADI_I2C_MASTER)
        {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
            eChannelID = ADI_DMA_CHANNEL_I2CM;
            pfCallback = DMAMasterHandler;
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        }
        else
        {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
            if (ABUFF->Direction == OUTBOUND)
            {
                eChannelID = ADI_DMA_CHANNEL_I2CS_TX;
                pfCallback = DMASlaveTxHandler;
            }
            else
            {
                eChannelID = ADI_DMA_CHANNEL_I2CS_RX;
                pfCallback = DMASlaveRxHandler;
            }
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        }
#endif

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm142 (rule 11.5): A cast shall not be performed that removes any 'const' or 'volatile'
*                    qualification from the type addressed by the pointer
*   Needed for passing generic callback parameter.
*/
#pragma diag_suppress=Pm142
#endif /* __ICCARM__ */
        
        pCBParam = (void *)drv;

#ifdef __ICCARM__
#pragma diag_default=Pm142
#endif /* __ICCARM__ */
        
        /* Open the DMA */
        if (adi_dma_Open (
                          eChannelID,
                          DATA->DMAMemory,
                          &DATA->hDMAHandle,
                          pfCallback,
                          pCBParam
                          ) != ADI_DMA_SUCCESS)
        {
           return ADI_I2C_DMA_ERROR;
        }

        if (adi_dma_Enable(DATA->hDMAHandle, true) != ADI_DMA_SUCCESS)
        {
            return ADI_I2C_DMA_ERROR;
        }

        /* Mark the DMA as initialized */
        CFG->bDmaInitialized = true;
    }

    if (bStart)
    {
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        if ((CFG->mode == ADI_I2C_MASTER) && (ABUFF->Direction == INBOUND)) {
            CFG->MRxCountRemaining = ABUFF->BytesRemaining;
            if (CFG->MRxCountRemaining > 256u) {
                REG(MRXCNT) = 0x100u;
            } else {
                REG(MRXCNT) = (uint16_t)CFG->MRxCountRemaining - 1u;
            }
        }
#endif
    }
    else
    {
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        if (   (CFG->mode == ADI_I2C_MASTER)
            && (ABUFF->Direction == INBOUND)
            && (CFG->MRxCountRemaining > 256u))
        {
            CFG->MRxCountRemaining = CFG->MRxCountRemaining & 0xFFu;
            if (CFG->MRxCountRemaining == 0u)  {
                CFG->MRxCountRemaining = 256u;
            }
            REG(MRXCNT) = (uint16_t)(CFG->MRxCountRemaining - 1u);
        }
#endif
    }

    /* Setup the transfer */
    while (ABUFF->BytesRemaining > 0u) {
        ADI_DMA_TRANSFER Transfer;
        uint32_t DmaTransferSize;

        DmaTransferSize = ABUFF->BytesRemaining;

#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        if ((CFG->mode == ADI_I2C_MASTER) && (ABUFF->Direction == INBOUND)) {
            if (DmaTransferSize > 256u) {
              if ((DmaTransferSize & 0xFFu) == 0u) {
                  DmaTransferSize = DmaTransferSize - 256u;
              } else {
                  DmaTransferSize = DmaTransferSize & ~0xFFu;
              }
            }
        }
#endif

        Transfer.DataWidth = ADI_DMA_WIDTH_1_BYTE;
        Transfer.NumTransfers = DmaTransferSize;

#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        if (CFG->mode == ADI_I2C_MASTER)
        {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
            if (ABUFF->Direction == OUTBOUND) {
                Transfer.pSrcData = ABUFF->pCurData;
                Transfer.SrcInc = ADI_DMA_INCR_1_BYTE;

                Transfer.pDstData = &REG(MTX);;
                Transfer.DstInc = ADI_DMA_INCR_NONE;
            } else {
                Transfer.pSrcData = &REG(MRX);
                Transfer.SrcInc = ADI_DMA_INCR_NONE;

                Transfer.pDstData = ABUFF->pCurData;
                Transfer.DstInc = ADI_DMA_INCR_1_BYTE;
            }
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        }
        else
        {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
            if (ABUFF->Direction == OUTBOUND) {
                Transfer.pSrcData = ABUFF->pCurData;
                Transfer.SrcInc = ADI_DMA_INCR_1_BYTE;

                Transfer.pDstData = &REG(STX);
                Transfer.DstInc = ADI_DMA_INCR_NONE;
            } else {
                Transfer.pSrcData = &REG(SRX);
                Transfer.SrcInc = ADI_DMA_INCR_NONE;

                Transfer.pDstData = ABUFF->pCurData;
                Transfer.DstInc = ADI_DMA_INCR_1_BYTE;
            }
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
        }
#endif

        ABUFF->BytesRemaining = ABUFF->BytesRemaining - DmaTransferSize;
        ABUFF->pCurData = (uint8_t*)ABUFF->pCurData + DmaTransferSize;

        /* Submit the buffer to the DMA manager for transfer */
        if (adi_dma_SubmitTransfer(
                                   DATA->hDMAHandle,
                                   &Transfer
                                   ) != ADI_DMA_SUCCESS)
        {
            return ADI_I2C_DMA_ERROR;
        }
    }

    return ADI_I2C_SUCCESS;
}
#endif

/* Init active buffer transfer */
static ADI_I2C_RESULT InitActiveBufferTransfer(ADI_I2C_DRIVER const *drv)
{
    ASSERT (drv->pData->activeBuffer != NULL);

    if (ABUFF->Direction == OUTBOUND) {

        /* Flush the Tx Buffers */
        FlushTx(drv);
    }

    /* Call the FIFO Manage function */
    DATA->pfManageFIFO(drv,true);

#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    if (CFG->mode == ADI_I2C_MASTER)
    {
#endif
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
        if (CFG->bDmaEnabled) {
            if (ABUFF->Direction == OUTBOUND) {
                SET_REG_BITS(MCTL,BITM_I2C_MCTL_MTXDMA | BITM_I2C_MCTL_IENCMP);
            } else {

                SET_REG_BITS(MCTL,BITM_I2C_MCTL_MRXDMA | BITM_I2C_MCTL_IENCMP);
            }
        } else {
            if (ABUFF->Direction == OUTBOUND) {
                SET_REG_BITS(MCTL,BITM_I2C_MCTL_IENMTX | BITM_I2C_MCTL_IENCMP);
            } else {
                SET_REG_BITS(MCTL,BITM_I2C_MCTL_IENMRX | BITM_I2C_MCTL_IENCMP);
            }
        }

        /* Commense Master Transfer */
        CommenceMasterTransfer(drv, (uint16_t)ABUFF->Direction);
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
    else
    {
#endif
#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
        if (CFG->bDmaEnabled) {
            /* Enable Tx/Rx Interrupt */
            SET_REG_BITS(SCTL,(uint16_t)(BITM_I2C_SCTL_IENSTOP | ((uint16_t)(ABUFF->Direction == OUTBOUND ? BITM_I2C_SCTL_STXDMA: BITM_I2C_SCTL_SRXDMA))));
        } else {
            /* Enable Tx/Rx Interrupt */
            SET_REG_BITS(SCTL,(uint16_t)(BITM_I2C_SCTL_IENSTOP | ((uint16_t)(ABUFF->Direction == OUTBOUND ? BITM_I2C_SCTL_IENSTX: BITM_I2C_SCTL_IENSRX))));
        }
#endif
#ifdef ADI_I2C_CFG_ENABLE_MASTER_SLAVE_SUPPORT
    }
#endif

    return ADI_I2C_SUCCESS;
}


/* Start or continue processing.  Processing stops when all buffers are complete. */
static ADI_I2C_RESULT ProcessBuffers(ADI_I2C_DRIVER const *drv)
{
    ADI_I2C_RESULT result = ADI_I2C_FAILURE;

    ASSERT (DATA->activeBuffer == NULL);

    /* A buffer is currently under process and hence there is nothing to be done here */
    if (IS_BUFF_AVAIL(drv->pData->activeBuffer))
    {
        return ADI_I2C_FAILURE;
    }

    /* If pending buffers are available */
    if (IS_BUFF_AVAIL(drv->pData->pendingBuffers)) {
        /* Pop buffer from the pending buffers */
        DATA->activeBuffer = fpop_buffer(&DATA->pendingBuffers);

        result = InitActiveBufferTransfer(drv);
    }

    return result;
}

static void NotifyBufferCompletion (ADI_I2C_DRIVER const *drv)
{
    /* Callback enabled */
    if ((DATA->Callback) && (DATA->eBlockMode != BLOCK_MODE_NONE))
    {
        ADI_I2C_BUFFER_STRUCT *pBuffer;

        pBuffer = fpop_buffer(&DATA->completedBuffers);

        if (pBuffer)
        {
            uint32_t nEvent;

            if (pBuffer->BytesRemaining == 0u)
            {
                nEvent = (pBuffer->Direction == OUTBOUND) ? (uint32_t)ADI_I2C_EVENT_XMT_PROCESSED : (uint32_t)ADI_I2C_EVENT_RCV_PROCESSED;
            }
            else
            {
               nEvent = (pBuffer->Direction == OUTBOUND) ? (uint32_t)ADI_I2C_EVENT_PARTIAL_XMT_PROCESSED : (uint32_t)ADI_I2C_EVENT_PARTIAL_RCV_PROCESSED;
            }

            drv->pData->Callback(
                                 DATA->pCallParam,
                                 nEvent,
                                 pBuffer
                                 );

            bpush_buffer(&DATA->freeBuffers, pBuffer);
        }
    }
    else
    {
        /* Post Semaphore */
        POST_EVENT(DATA);
    }
}


/* Un-initialize the DMA */
#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
static void UnInitDma(ADI_I2C_DRIVER const *drv)
{
    if ((CFG->bDmaEnabled) && (CFG->bDmaInitialized)) {
        /* Close the DMA Handle */
        adi_dma_Close(DATA->hDMAHandle);
        CFG->bDmaInitialized = false;
    }
}
#endif

#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
/* Process slave STOP interrupt */
static __INLINE void onSlaveSTOP (ADI_I2C_DRIVER const *drv) {

    if (ABUFF->Direction == OUTBOUND) {
        CLR_BITS_U16(REG(SCTL),BITM_I2C_SCTL_IENSTOP | BITM_I2C_SCTL_IENSTX | BITM_I2C_SCTL_STXDMA);
    } else {
        CLR_BITS_U16(REG(SCTL),BITM_I2C_SCTL_IENSTOP | BITM_I2C_SCTL_IENSRX | BITM_I2C_SCTL_SRXDMA);
    }

    /* IF (Active buffer is still available) */
    if (IS_BUFF_AVAIL(drv->pData->activeBuffer))
    {
        /* This condition only occurs if the completion happens before the
           buffer is fully transmitted or received */
        ProcessBufferCompletion(drv);
    }

    /* Notify Buffer Completion */
    NotifyBufferCompletion(drv);

    /* Process the interrupt stop */
    ProcessBuffers(drv);
}
#endif

/* Process Master Complete interrupt */
#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
static __INLINE void onMasterCOMP    (ADI_I2C_DRIVER const *drv)
{
    /* disable complete,rx and tx interrupt */
    CLR_BITS_U16(REG(MCTL),BITM_I2C_MCTL_IENCMP | BITM_I2C_MCTL_IENMRX | BITM_I2C_MCTL_IENMTX | BITM_I2C_MCTL_MTXDMA | BITM_I2C_MCTL_MRXDMA);

    /* IF (Active buffer is still available) */
    if (IS_BUFF_AVAIL(drv->pData->activeBuffer))
    {
        /* This condition only occurs if the completion happens before the
           buffer is fully transmitted or received */
        ProcessBufferCompletion(drv);
    }

    /* Notify Buffer Completion */
    NotifyBufferCompletion(drv);

    /* Process the interrupt stop */
    ProcessBuffers(drv);
}

/* Do callback for the given info */
static void InfoSetCallback(ADI_I2C_DRIVER const *drv, BUFF_INFO info, uint32_t event) {
    SET_BITS_U16(ABUFF->Info, (uint16_t)info);
    if (DATA->Callback) {
        DATA->Callback(DATA->pCallParam, event, NULL);
    }
}

/* Process master arbitration lost interrupt */
static __INLINE void onMasterALOST    (ADI_I2C_DRIVER const *drv) {
    InfoSetCallback(drv, BUFF_INFO_NACK_ALOST, ADI_I2C_EVENT_LOSTARB);
}

/* Process master address NACK interrupt */
static __INLINE void onMasterNACKADDR    (ADI_I2C_DRIVER const *drv) {
    InfoSetCallback(drv, BUFF_INFO_NACK_ADDR, ADI_I2C_EVENT_ANAK);
}

/* Process master data NACK interrupt */
static __INLINE void onMasterNACKDATA    (ADI_I2C_DRIVER const *drv) {
    InfoSetCallback(drv, BUFF_INFO_NACK_DATA, ADI_I2C_EVENT_DNAK);
}
#endif

#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
/* Set the given glboal Event */
static void SetGlobalEvent (ADI_I2C_DRIVER const *drv, uint16_t nEvent)
{
    CFG->nGlobalEvent |= nEvent;
    POST_EVENT(DATA);
}

/* Process slave repeat start interrupt */
static __INLINE void onSlaveREPSTART    (ADI_I2C_DRIVER const *drv) {
    if (DATA->Callback) {
        DATA->Callback(DATA->pCallParam, ADI_I2C_EVENT_REP_START, NULL);
    } else {
        SetGlobalEvent (drv, ADI_I2C_GLOBAL_EVENT_REP_START);
    }
}

/* Process slave general call interrupt */
static __INLINE void onSlaveGCINT    (ADI_I2C_DRIVER const *drv) {
    uint16_t nGCID = (REG(SSTAT) & BITM_I2C_SSTAT_GCID) >> BITP_I2C_SSTAT_GCID;

    if (nGCID != 0u) {
        if (DATA->Callback) {
            DATA->Callback(DATA->pCallParam, ADI_I2C_EVENT_GCALL, (void *)(uint32_t)nGCID);
        } else {
            uint16_t GEvent = 0u;
            if (nGCID == 1u) {GEvent = ADI_I2C_GLOBAL_EVENT_GC_RESET_PRGN_ADDR;}
            if (nGCID == 2u) {GEvent = ADI_I2C_GLOBAL_EVENT_GC_PRGN_ADDR;}
            if (nGCID == 3u) {GEvent = ADI_I2C_GLOBAL_EVENT_GC_HARDWARE;}

            SetGlobalEvent(drv, GEvent);
        }
    }
}
#endif

/* Process Completion of the buffer */
static void ProcessBufferCompletion (ADI_I2C_DRIVER const *drv)
{
    if (CFG->mode == ADI_I2C_MASTER) {
        /* disable master transmission/receive interrupts */
        CLR_BITS_U16(REG(MCTL),BITM_I2C_MCTL_IENMTX | BITM_I2C_MCTL_MTXDMA | BITM_I2C_MCTL_IENMRX | BITM_I2C_MCTL_MRXDMA);
    } else {
        /* INFO: All bits may not be cleared as RX and TX can work together. */
        if (ABUFF->Direction == INBOUND) {
            CLR_BITS_U16(REG(SCTL),BITM_I2C_SCTL_IENSRX | BITM_I2C_SCTL_SRXDMA);
        } else {
            CLR_BITS_U16(REG(SCTL), BITM_I2C_SCTL_IENSTX | BITM_I2C_SCTL_STXDMA);
        }
    }

    bool_t bFastProcess = ABUFF->FastProcess;

    /* Push the active buffer to completed buffer list */
    bpush_buffer(&DATA->completedBuffers,DATA->activeBuffer);

#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
    /* Close the DMA if there is no pending buffers or the next slave transfer require a different DMA */
    if (DATA->pendingBuffers == NULL)
    {
        UnInitDma(drv);
    }
    else
    {
        if(ABUFF->Direction != DATA->pendingBuffers->Direction)
        {
            /* UnInit the DMA */
            UnInitDma(drv);
        }
    }
#endif

    /* Clear the active buffer */
    DATA->activeBuffer = NULL;

    /* If the next buffer need to be processed immediately */
    if (bFastProcess)
    {
        /* Process the buffer */
        ProcessBuffers(drv);

        /* Notify Buffer Completion */
        NotifyBufferCompletion(drv);
    }
}

/* Manage buffers in the interrupt region */
static void IntManageBuffers(ADI_I2C_DRIVER const *drv)
{
    bool_t IntCanBeCleared = true;

    if (CFG->mode == ADI_I2C_MASTER) {
        if (ABUFF->Restart) {
            REG(ADDR1) = CFG->firstMasterAddressByte;

            ABUFF->Restart = false;
        }
    }

    DATA->pfManageFIFO(drv,false);

#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
    if (CFG->bDmaEnabled) {

        bool_t IsTransferInProgress = false;
        adi_dma_IsTransferInProgress(DATA->hDMAHandle, &IsTransferInProgress);

        if (IsTransferInProgress) {
            IntCanBeCleared = false;
        }
    }
#endif

    if ((IntCanBeCleared == true) && (ABUFF->BytesRemaining == 0u)) {
        ProcessBufferCompletion(drv);
    }
}

#if (ADI_I2C_CFG_ENABLE_MASTER_SUPPORT == 1)
/* Dispatch Master interrupt  */
static void DispatchMasterInterrupt (ADI_I2C_DRIVER *drv)
{
    register uint16_t status = REG(MSTAT);

    /* priortized master-mode event handlers */
    if (!CFG->bDmaEnabled) {
        if (IS_ANY_BITS_SET(status,BITM_I2C_MSTAT_MRXREQ | BITM_I2C_MSTAT_MTXREQ)) {
            IntManageBuffers(drv);
        }
    }
    if (IS_BIT_SET(status,BITM_I2C_MSTAT_ALOST)) {
        onMasterALOST(drv);
    }

    if (IS_BIT_SET(status,BITM_I2C_MSTAT_NACKADDR)) {
        onMasterNACKADDR(drv);
    }

    if (IS_BIT_SET(status,BITM_I2C_MSTAT_NACKDATA)) {
        onMasterNACKDATA(drv);
    }

    if (IS_BIT_SET(status,BITM_I2C_MSTAT_TCOMP)) {
        onMasterCOMP(drv);
    }

}

/* Master Interrupt Handler */
ADI_INT_HANDLER (I2C0_Master_Int_Handler) {
    ISR_PROLOG();
    DispatchMasterInterrupt(&adi_i2c_Device[0*2 + 0]);
    ISR_EPILOG();
}
#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)
/* Master DMA handler */
static void DMAMasterHandler (void *pcbParam, uint32_t Event, void *pArg)
{
    ADI_I2C_DRIVER const *drv = &adi_i2c_Device[0*2 + 0];

    IntManageBuffers(drv);
}
#endif

#endif



#if (ADI_I2C_CFG_ENABLE_SLAVE_SUPPORT == 1)
/* Dispatch Slave interrupts */
static void DispatchSlaveInterrupt(ADI_I2C_DRIVER *drv) {

    register uint16_t status = REG(SSTAT);

    /* priortized slave-mode event handlers */
    if (!CFG->bDmaEnabled) {
        if (IS_ANY_BITS_SET(status, BITM_I2C_SSTAT_SRXREQ | BITM_I2C_SSTAT_STXREQ)) {
            IntManageBuffers(drv);
        }
    }
    if (IS_BIT_SET(status,BITM_I2C_SSTAT_REPSTART))  {
        onSlaveREPSTART(drv);
    }

    if (IS_BIT_SET(status,BITM_I2C_SSTAT_STOP)) {
        onSlaveSTOP(drv);
    }

    if (IS_BIT_SET(status,BITM_I2C_SSTAT_GCINT)) {
        onSlaveGCINT(drv);
    }
}

/* Slave interrupt handler */
ADI_INT_HANDLER (I2C0_Slave_Int_Handler) {
    ISR_PROLOG();
    DispatchSlaveInterrupt(&adi_i2c_Device[0*2 + 1]);
    ISR_EPILOG();   
}

#if (ADI_I2C_CFG_ENABLE_DMA_SUPPORT == 1)

static void DMASlaveTxHandler (void *pcbParam, uint32_t Event, void *pArg)
{
    ADI_I2C_DRIVER const *drv = &adi_i2c_Device[0*2 + 1];

    ASSERT (ABUFF->Direction == OUTBOUND);

    IntManageBuffers(drv);
}

static void DMASlaveRxHandler (void *pcbParam, uint32_t Event, void *pArg)
{
    ADI_I2C_DRIVER const *drv = &adi_i2c_Device[0*2 + 1];

    ASSERT (ABUFF->Direction == INBOUND);

    IntManageBuffers(drv);
}
#endif

#endif

#if defined(ADI_DEBUG)
/*
 * Verifies a pointer to a driver points to one of the driver
 * struct's internal to this file.
 */
static bool_t IsDeviceHandle(ADI_I2C_DRIVER const *pDevice)
{
    uint32_t i = 0u;
    bool_t bFound = false;
    for (i=0u; i<(NUM_I2C_DEVICES*2u); i++)
    {
        if (pDevice == &adi_i2c_Device[i])
        {
            bFound = true;
            break;
        }
    }
    return bFound;
}
#endif

#endif /* _ADI_I2C_V1_C_ */

/* @} */
