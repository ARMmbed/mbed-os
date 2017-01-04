/*!
 *****************************************************************************
 * @file:    adi_spi_v1.c
 * @brief:   SPI Device Implementations for ADuCRFxxx
 * @version: $Revision: 35861 $
 * @date:    $Date: 2016-11-22 09:07:07 +0000 (Tue, 22 Nov 2016) $
 *----------------------------------------------------------------------------
 *
Copyright (c) 2010-2016 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef _ADI_SPI_V1_C_
#define _ADI_SPI_V1_C_

/** @addtogroup SPI_Driver SPI Device Driver
 *  @{
 */

 /*! \cond PRIVATE */

#include <stdlib.h>  /* for 'NULL" definition */
#include <string.h>

#include <drivers/spi/adi_spi.h>
#include <services/dma/adi_dma.h>
#include <services/pwr/adi_pwr.h>
#include <services/int/adi_int.h>

#include <ssdd_common/common_def.h>

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

#include "adi_spi_data_v1.c"

#if defined ( __GNUC__ )
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

/* handle checker for debug mode */
#define ADI_SPI_VALIDATE_HANDLE(h) ((spi_device_info[0].hDevice != (h)) && (spi_device_info[1].hDevice != (h)) && (spi_device_info[2].hDevice != (h)))
/*! \endcond */

  /* Internal API... */
static ADI_SPI_RESULT ConfigureCoreAsMaster    (ADI_SPI_HANDLE const hDevice);

static ADI_SPI_RESULT CommonInit               (ADI_SPI_HANDLE const hDevice,
                                                     uint32_t nDeviceNum);

static void common_SPI_Int_Handler             (ADI_SPI_DEV_DATA_TYPE* pDD);


/* pointer table function prototypes... */
static ADI_SPI_RESULT intInit                  (ADI_SPI_HANDLE const hDevice);
static ADI_SPI_RESULT intUnInit                (ADI_SPI_HANDLE const hDevice);
static ADI_SPI_RESULT intInitializeDescriptors (ADI_SPI_HANDLE const hDevice);
static void            intCommonHandler         (ADI_SPI_HANDLE pDD,ADI_DMA_CHANNEL_ID ChanId);

#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_SPI_RESULT dmaInit                  (ADI_SPI_HANDLE const hDevice);
static ADI_SPI_RESULT dmaClose                 (ADI_SPI_HANDLE const hDevice);
static ADI_SPI_RESULT dmaInitializeDescriptors (ADI_SPI_HANDLE const hDevice);
static void           dmaCommonHandler         (ADI_SPI_DEV_DATA_TYPE* pDD,ADI_DMA_CHANNEL_ID ChanId);
static ADI_SPI_RESULT EnableDma                (ADI_SPI_HANDLE const hDevice, const bool_t bFlag,uint16_t nDMAFlags);
static ADI_SPI_RESULT EnableDmaTx               (ADI_SPI_HANDLE const hDevice, const bool_t bFlag);
static ADI_SPI_RESULT EnableDmaRx               (ADI_SPI_HANDLE const hDevice, const bool_t bFlag);
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */

/*! \endcond */

/*
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////   API IMPLEMENTATIONS   ///////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
*/

/*!
 * @brief  Initialize and allocate an SPI device for use in Master Mode.
 *
 * @param[in]   nDeviceNum   Zero-based device index designating which device to initialize.
 *\n
 * @param [in]  pDevMemory   Pointer to a 32 bit aligned buffer of size ADI_SPORT_INT_MEMORY_SIZE
 *\n                         required by the driver for the operation of specified SPORT device.
 *\n                         Buffer Size should be "ADI_SPORT_DMA_MEMORY_SIZE" bytes if the device is
 *\n                         expected to operate in DMA mode.
 *
 * @param [in]  nMemorySize  Size of the buffer to which "pMemory" points. DMA mode can be enabled
 *\n                         only if *pMemory points to the buffer whose size is sufficient to
 *\n                         operate in the DMA mode. This parameter ("nMemSize") indicate whether
 *\n                         the device is expected to operate in DMA mode or in interrupt mode.
 *
 * @param[out]  phDevice     The caller's device handle pointer for storing the initialized device instance data pointer.
 *
 * @return            Status
 *                    - #ADI_SPI_IN_USE   [D]    Specified device has already been initialized.
 *                    - #ADI_SPI_INVALID_DEVICE_NUM [D]             Invalid device index.
 *                    - #ADI_SPI_DMA_ERROR             Internal DMA initializations failed.
 *                    - #ADI_SPI_SEMAPHORE_FAILED           Semaphore creation failed.
 *                    - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
* @note :  No other SPI APIs may be called until the device open function is called.
 *\n       Initialize an SPI device using internal default configuration settings and allocate the
 *\n       device for use.The returned device handle is required to be passed to all subsequent
 *\n       calls to convey which device instance to operate on.
 *\n       The contents of phDevice will be set to NULL upon failure. Device is opened in Master mode.
 *\n
 * @sa      adi_spi_SetMasterMode()
 * @sa      adi_spi_Close().
 */
ADI_SPI_RESULT adi_spi_Open(uint32_t nDeviceNum,void *pDevMemory,uint32_t nMemorySize,
                                        ADI_SPI_HANDLE* const phDevice)
{
    ADI_SPI_HANDLE hDevice = pDevMemory;
    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;

    /* store a bad handle in case of failure */
    *phDevice = (ADI_SPI_HANDLE) NULL;

#ifdef ADI_DEBUG

    if (nDeviceNum >= ADI_SPI_NUM_INSTANCES)
    {
        return ADI_SPI_INVALID_DEVICE_NUM;
    }

    if (ADI_SPI_STATE_UNINITIALIZED != spi_device_info[nDeviceNum].eState)
    {
        return ADI_SPI_IN_USE;
    }

#endif
    spi_device_info[nDeviceNum].hDevice = (ADI_SPI_DEV_DATA_TYPE *)pDevMemory;
    memset(pDevMemory,0,nMemorySize);
    hDevice->pDevInfo =  &spi_device_info[nDeviceNum];
    spi_device_info[nDeviceNum].hDevice = hDevice;
    result = CommonInit(hDevice, nDeviceNum);
    if( result != ADI_SPI_SUCCESS )
    {
      return result;
    }

    hDevice->pSpi->RD_CTL  = 0u;

    /* mark device fully initialized */
    hDevice->eDevState = ADI_SPI_STATE_MASTER;

#if (ADI_SPI_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
    {
      ADI_SPI_CFG_TYPE const* pSPICfg = &gSPICfg[nDeviceNum];

      /* Initialize the device based on the given configuration parameters */
      hDevice->pSpi->CTL = pSPICfg->SPI_CTL;
      hDevice->pSpi->DIV = pSPICfg->SPI_DIV;
    }
#else
	/* configure core for Master-Mode operation */
    if (ADI_SPI_SUCCESS != (result = ConfigureCoreAsMaster (hDevice))) {
        hDevice->eDevState = ADI_SPI_STATE_UNINITIALIZED;
        return result;
    }
#endif  /* ADI_SPI_CFG_ENABLE_STATIC_CONFIG_SUPPORT */

    hDevice->pSpi->CTL |= BITM_SPI_CTL_SPIEN;
    /* write the device data pointer into the caller's handle */
    *phDevice = hDevice;

#if (ADI_SPI_CFG_ENABLE_DMA == 1)

    if(adi_spi_EnableDmaMode(*phDevice,true) != ADI_SPI_SUCCESS)
    {
        return(ADI_SPI_FAILURE);
    }
#endif
   return result;
}
/*!
 * @brief  Uninitialize and deallocate an SPI device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_spi_Open().
 *
 * @return      Status
 *                - #ADI_SPI_INVALID_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_SPI_DMA_ERROR         Internal DMA operations failed.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]    Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *
 * Uninitialize and release an allocated SPI device,and memory associated with it for other use.
 *
 * @sa  adi_spi_Open().
 */
ADI_SPI_RESULT adi_spi_Close (ADI_SPI_HANDLE const hDevice)
{

    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED ;
    }
#endif
    /* disable Interrupt    */
    ADI_DISABLE_INT(hDevice->pDevInfo->eIRQn);
    ADI_UNINSTALL_HANDLER((uint32_t) hDevice->pDevInfo->eIRQn);

#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
     /* destroy semaphore */
     DELETE_SEM(hDevice,ADI_SPI_SEMAPHORE_FAILED);
#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT */

    /* uninit internals */
    if (ADI_SPI_SUCCESS != hDevice->FunctionTable.pUnInitFcn(hDevice))
    {
        return ADI_SPI_DMA_ERROR;
    }
    /* invalidate initialization state */
    hDevice->eDevState = ADI_SPI_STATE_UNINITIALIZED;
    hDevice->pDevInfo->hDevice  = NULL;
    hDevice->pDevInfo->eState = ADI_SPI_STATE_UNINITIALIZED;
    return result;
}

/*!
 * @brief  Register or unregister the callback.
 *
 * @param [in]   hDevice        Device handle obtained from adi_spi_Open().
 * @param [in]   pfCallback     Pointer to the callback function. Can be passed as NULL to unregister the
 *\n                            previously registered callback.
 * @param [in]   pCBParam       Callback parameter which will be passed back to the application when the
 *\n                            callback is called.
 *
 * @return      Status
 *                - #ADI_SPI_INVALID_HANDLE [D]  Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED   [D]  Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 */
ADI_SPI_RESULT adi_spi_RegisterCallback (ADI_SPI_HANDLE const hDevice,  ADI_CALLBACK const pfCallback, void *const pCBParam )
{
#ifdef ADI_DEBUG
    if (ADI_SPI_STATE_UNINITIALIZED == hDevice->eDevState)
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }

#endif
    /* Save the application provided callback and callback parameters */
    hDevice->pfCallback = pfCallback;
    hDevice->pCBParam   = pCBParam;

    return ADI_SPI_SUCCESS;
}
/*!
 * @brief  Set SPI IRQ mode.
 *
 * @param[in]      hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]      Mode         Designating new FIFO interrupt mode, based on their relative fullness or emptiness.
 *
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Set the IRQ mode successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_INVALID_OPERATION [D]      This function should be called only when device is operating in
 *                                                       interrupt mode.
 *@note
 *\n Configures when the SPI FIFOs generate interrupts, based on their relative "fullness".
 *\n See user manual for mode descriptions.This function should be called only when device
 *\n is operating in interrupt mode.
 *
 *
 */
ADI_SPI_RESULT adi_spi_SetInterruptMode (ADI_SPI_HANDLE const hDevice, const ADI_SPI_IRQ_MODE Mode)
{
#ifndef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if (hDevice->bDmaMode == true)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
#endif
    /* stop interrupts while changing interrupt mode */
    ADI_ENTER_CRITICAL_REGION();

    /* clear existing, then set new mode... */
    hDevice->pSpi->IEN &= (uint16_t)~BITM_SPI_IEN_IRQMODE;
    hDevice->pSpi->IEN |= Mode;

    /* reenable interrupts */
    ADI_EXIT_CRITICAL_REGION();

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the SPI transmit FIFO flush mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage FIFO transmit flush mode.
 *                - true     Set transmit flush mode.
 *                - false    Clear transmit flush mode.
 *
 * @return        Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 * @note
 *\n Set the transmit FIFO flush mode to flush the FIFO and/or keep it flushed.
 *\n The transmit FIFO is essentially disabled while this mode is set.  Clear it
 *\n for normal FIFO operation.  A one-time flush is obtained by setting this
 *\n mode and then clearing it. This mode is NOT self-clearing.
 *\n Clear this mode to prevent continuous transmit FIFO flushing.
 * @sa        adi_spi_SetFifoRxFlush().
 */
ADI_SPI_RESULT adi_spi_SetFifoTxFlush (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_TFLUSH);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_TFLUSH;
    }

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the SPI receive FIFO flush mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage FIFO receive flush mode.
 *                - true     Set receive flush mode.
 *                - false    Clear receive flush mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 *\n Set the receive FIFO flush mode to flush the FIFO and/or keep it flushed.
 *\n The receive FIFO is essentially disabled while this mode is set.  Clear it
 *\n for normal FIFO operation.  A one-time flush is obtained by setting this
 *\n mode and then clearing it.This mode is NOT self-clearing.  Clear this mode
 *\n to prevent continuous receive FIFO flushing.
 *
 * @sa        adi_spi_SetFifoTxFlush().
 */
ADI_SPI_RESULT adi_spi_SetFifoRxFlush (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice)) {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE)) {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_RFLUSH);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_RFLUSH;
    }

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the continuous transfer mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage SPI continuous transfer mode.
 *                - true     Set continuous transfer mode.
 *                - false    Clear continuous transfer mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Setting this mode causes the SPI controller to drive the Chip Select signal continuously until the transmit
 * FIFO is emptied.  Clearing it causes Chip Select to cycle between bytes.  If the transmit FIFO is allowed to
 * become empty, Chip Select will de-assert regardless of this mode.
 *
 * @note
 *\n  If the transmit FIFO is allowed to empty during a send, the hardware-based (SPI controller-driven)
 *\n Chip Select will de-assert which may disrupt extended transactions.  For this reason, the SPI device
 *\n driver explicitly manages the Chip Select pin using GPIO protocol so that the chip remains selected
 *\n throughout the entire transaction... regardless of whether the transmit FIFO may momentarily empty.
 *
 */
ADI_SPI_RESULT adi_spi_SetContinousMode (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice)) {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE)) {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_CON);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_CON;
    }

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the internal loopback mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage internal SPI loopback mode.
 *                - true     Set internal loopback mode.
 *                - false    Clear internal loopback mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Set or clear the internal SPI loopback mode.  Primairly used for testing.
 *
 */
ADI_SPI_RESULT adi_spi_SetLoopback (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_LOOPBACK);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_LOOPBACK;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set SPI Slave-Mode MISO output enable.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage Slave-Mode MISO output ebable.
 *\n               - true     Enable "normal" opersation (driven output).
 *\n               - false    Disable the MISO output driver (open circuit).
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Set or clear this mode to manage the MISO pin output driver enable.
 *
 */
ADI_SPI_RESULT adi_spi_SetMisoOutput (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_OEN);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_OEN;
    }

    ADI_EXIT_CRITICAL_REGION();

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set the SPI receive FIFO overflow mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage recieve FIFO overflow behavior.
 *\n              - true     Discard old data on receive FIFO overflow.
 *\n              - false    Discard new data on receive FIFO overflow.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Controls what to do with excess incoming data when the receive FIFO becomes full.
 * Either the new data or the old data is discarded.  Set the receive FIFO overflow mode
 * to replace data in the RX register (top of receive FIFO) with the incoming new data.
 * Clear it to discard incoming new data and preserve old undead data.

 *
 */
ADI_SPI_RESULT adi_spi_SetReceiveOverflow (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_RXOF);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_RXOF;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set the SPI transmit FIFO underflow mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage transmit FIFO underflow behavior.
 *\n               - true     Send zeroes on transmit FIFO underflow.
 *\n               - false    Resend last data on transmit FIFO underflow.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 *\n  Controls what to transmit when lacking valid data because the transmit FIFO is empty.
 *\n  Either zeros or the last valid data are transmitted.  Set transmit FIFO underflow mode to send zeros.
 *\n Clear it to resend the last transmitted data.
 *
 */
ADI_SPI_RESULT adi_spi_SetTransmitUnderflow (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_ZEN);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_ZEN;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set SPI transfer initiate and interrupt mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage transfer initiate and interrupt mode.
 *                - true     Initiate transfers on write to transmit FIFO and interrupt when empty.
 *                - false    Initiate transfers on read from receive FIFO and interrupt when full.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 *\n Set to initiate transfers on a write to the transmit data register
 *\n and interrupt when the transmit FIFO is empty.  Clear to initiate
 *\n transfers with a read from the receive data register and interrupt when the
 *\n receive FIFO is full.
 *
 */
ADI_SPI_RESULT adi_spi_SetTransferInitiateMode (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_TIM);   /* initiate on tx write */
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_TIM;  /* initiate on rx read */
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set SPI LSB or MSB first transfer mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open()
 * @param[in]    bFlag        Flag to manage serialization direction of transfer.
 *\n                - true     For LSB first.
 *\n                - false    For MSB first.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Configures whether data is transferred as LSB first or MSB first.
 *
 */
ADI_SPI_RESULT adi_spi_SetLsbFirst (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_LSB);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_LSB;
    }

    return ADI_SPI_SUCCESS;
}



/*!
 * @brief  Set the SPI "Wired-OR" output mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to select output circuit configuration.
 *\n               - true     For "wired-or" (open-collector) output mode.
 *\n               - false    For "normal" (fully-driven) output levels.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Controls the output driver configuration.  Set to enable an "Open-Collector" output circuit
 * for performing external "Wired-OR" logic, requiring an external pull-up resistor.
 * Clear for normal, fully-driven outputs.
 *
 */
ADI_SPI_RESULT adi_spi_SetWiredOrMode (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_WOM);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_WOM;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set the clock polarity.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open()
 * @param[in]    bFlag        Flag to manage the idle state of the serial data clock between samples.
 *\n               - true     Clock is idled high.
 *\n               - false    Clock is idled low.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Sets the SPI clock polarity control bit (CPOL).  Used in conjunction with clock phase (CPHA) to program
 *\n the exact timing of serial data capture and transmit.  Both clock phase and polarity must be considered in
 *\n selecting the data transfer mode best suited to the Master/Slave device pair, typically dependant on the
 *\n manufacturer and timing requirements of the external SPI device.
 *
 *\n Both Master and Slave devices must use the same settings for clock phase and polarity.
 *
 *\n If the phase of the clock is zero (CPHA=0), receive data are latched on the rising-clock-edge with
 *\n CPOL=0 (idle-low) polarity, and on the falling-clock-edge with CPOL=1 (idle high) ploarity.
 *
 *\n If CPHA=1, the effective clock edges are reversed; CPOL=0 latches receive data on the falling-clock-edge
 *\n and CPOL=1 latches receive data on the rising-clock-edge.
 *
 *\n Data are transmitted on the opposite clock edge as the receive, i.e., receive and transmit are out of phase.
 *
 * @sa        adi_spi_SetClockPhase().
 */
ADI_SPI_RESULT adi_spi_SetClockPolarity (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_CPOL);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_CPOL;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set the clock phase.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage the phase of the serial data clock.
 *\n                - true     Sample data on trailing-edge of each serial bit.
 *\n                - false    Sample data on leading-edge of each serial bit.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 *\n Sets the SPI clock polarity phase bit (CPHA).  Used in conjunction with clock polarity (CPOL) to program
 *\n the exact timing of serial data capture and transmit.  Both clock phase and polarity must be considered in
 *\n selecting the data transfer mode best suited to the Master/Slave device pair, typically dependant on the
 *\n manufacturer and timing requirements of the external SPI device.
 *
 *\n Both Master and Slave devices must use the same settings for clock phase and polarity.
 *
 *\n If the phase of the clock is zero (CPHA=0), receive data are latched on the rising-clock-edge with
 *\n CPOL=0 (idle-low) polarity, and on the falling-clock-edge with CPOL=1 (idle high) ploarity.
 *
 *\n If CPHA=1, the effective clock edges are reversed; CPOL=0 latches receive data on the falling-clock-edge
 *\n and CPOL=1 latches receive data on the rising-clock-edge.
 *
 *\n Data are transmitted on the opposite clock edge as the receive, i.e., receive and transmit are out of phase.
 *
 * @sa        adi_spi_SetClockPolarity().
 */
ADI_SPI_RESULT adi_spi_SetClockPhase (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    if (true == bFlag) {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_CPHA);
    } else {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_CPHA;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set SPI Master-Mode operation.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to select either Master-Mode or Slave-Mode operation.
 *\n                - true     Enable Master-Mode.
 *\n                - false    Enable Slave-Mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Controls SPI Master/Slave mode of operation, set for Master-Mode, clear for Slave-Mode.
 *
 */
ADI_SPI_RESULT adi_spi_SetMasterMode (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif
    ADI_ENTER_CRITICAL_REGION();
    if (true == bFlag) {
        hDevice->pSpi->CTL |= (ADI_SPI_MASTERCON_INITIALIZER);
        hDevice->eDevState = ADI_SPI_STATE_MASTER;
   } else {
        hDevice->pSpi->CNT = 0u;
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_MASEN;
        hDevice->pSpi->CTL |= (ADI_SPI_SLAVECON_INITIALIZER);
        hDevice->eDevState = ADI_SPI_STATE_SLAVE;
    }
    ADI_EXIT_CRITICAL_REGION();
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the SPI serial clock frequency.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open()
 * @param[in]    Hertz        Target frequency (in Hz) for SPI bitrate.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_INVALID_PARAM         Specified frequence out of range.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Compute and set the internal SPI clock rate divider register to produce
 *\n the desired serial clock frequency.  Resulting frequency is subject to arithmetic rounding errors.
 *\n Use #adi_spi_GetBitrate() to obtain the exact frequency produced, including rounding errors.
 *
 * @sa        adi_spi_GetBitrate().
 */
ADI_SPI_RESULT adi_spi_SetBitrate (ADI_SPI_HANDLE const hDevice, const uint32_t Hertz)
{
    uint32_t incoming_clock;
    uint16_t Div;

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if (ADI_SPI_STATE_UNINITIALIZED == hDevice->eDevState)
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

     adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &incoming_clock);

    /* requested rate needs to be 2x less than incomming clock */
    if (2u*Hertz >= incoming_clock)
    {
        return ADI_SPI_INVALID_PARAM;
    }

     /* compute the SPI divider value */
     Div = (uint16_t) ((incoming_clock / Hertz) >> 1u) - 1u;  /* '>>1' is really a divide by 2 */

    /* range check that computed divider fits */
    if (Div != (Div & BITM_SPI_DIV_VALUE))
    {
        return ADI_SPI_INVALID_PARAM;
    }

    /* store it in core */
    hDevice->pSpi->DIV = Div;

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Get the SPI serial clock frequency.
 *
 * @param[in]    hDevice     Device handle obtained from adi_spi_Open()
 * \n
 * @param[in]    pnBitrate   Pointer to the location where Bitrate need to be written.
 *
 * @return       Returns the exact serial clock frequence (in Hertz).
 *
 * Get the current serial clock frequency.  The returned value is exact but
 *\n may not exactly match the value set with #adi_spi_SetBitrate() due to
 *\n computational round-off errors resulting from fixed register size and
 *\n finite-precision arithmetic.
 *
 * @sa        adi_spi_SetBitrate().
 */
ADI_SPI_RESULT adi_spi_GetBitrate (ADI_SPI_HANDLE const hDevice, uint32_t* const pnBitrate)
{
    uint32_t incoming_clock;
    ADI_PWR_RESULT ePwrResult;
    uint32_t Div;

    Div = hDevice->pSpi->DIV;  /* assumes this is always a right-justified value */

    ePwrResult = adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &incoming_clock);
    if(ePwrResult != ADI_PWR_SUCCESS)
    {
        *pnBitrate= 0u;
         return(ADI_SPI_FAILURE);
    }
    *pnBitrate= (incoming_clock / (Div + 1u)) >> 1u;  /* '>>1' is divide by 2 */
     return(ADI_SPI_SUCCESS);

}


/*!
 * @brief  Set the chip select.
 *
 * @param[in]    hDevice       Device handle obtained from adi_spi_Open().
 * @param[in]    eChipSelect   An enum value representing the requested Chip Select.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Sets the desired chip select to use for activating an external slave device.
 *
 * @note      Chip select \a ADI_SPI0_CSn is reserved for SPI device 0 (SPI0) internal chip select line
 *            dedicated for communications with the UHF device.
 *
 * @sa        adi_spi_GetChipSelect().
 */
ADI_SPI_RESULT adi_spi_SetChipSelect (ADI_SPI_HANDLE const hDevice, const ADI_SPI_CHIP_SELECT eChipSelect)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->ChipSelect = eChipSelect;

    hDevice->pSpi->CS_CTL = hDevice->ChipSelect;

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Get the chip select.
 *
 * @param[in]    hDevice       Device handle obtained from adi_spi_Open().
 * \n
 * @param[in]    peChipSelect  Pointer to location where current ChipSelect(CS) need to be written.
 *
 * @return       Returns an enumeration value representing the chip select line currently configured.
 *
 * Get the current state of the SPI chip select.
 *
 * @sa        adi_spi_SetChipSelect().
 */
ADI_SPI_RESULT adi_spi_GetChipSelect (ADI_SPI_HANDLE const hDevice, ADI_SPI_CHIP_SELECT *peChipSelect )
{
   *peChipSelect =  hDevice->ChipSelect;
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Enable the specified interrupt(s) for the device..
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    eInterrupt   Specify which interrupt(s) need to be enabled.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 */
ADI_SPI_RESULT adi_spi_SetInterrruptMask (ADI_SPI_HANDLE const hDevice, ADI_SPI_INT_TYPE eInterrupt )
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif
    
    /*returns an invalid operation when CS edge interrupt is enabled in Master mode*/
    if((hDevice->eDevState == ADI_SPI_STATE_MASTER) && (((uint16_t)eInterrupt & BITM_SPI_IEN_CS) == BITM_SPI_IEN_CS))
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    /*returns an invalid operation when ready signal edge interrupt is enabled in Slave mode*/
    if((hDevice->eDevState == ADI_SPI_STATE_SLAVE) && (((uint16_t)eInterrupt & BITM_SPI_IEN_RDY) == BITM_SPI_IEN_RDY))
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    /*xfrdone is limted to driver and access to this is not supported from application*/
    if(((uint16_t)eInterrupt & BITM_SPI_IEN_XFRDONE) == BITM_SPI_IEN_XFRDONE)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    
    hDevice->pSpi->IEN &= ~ADI_INTERRUPT_MASK;
    /*check whether CS edge interrupt is enabled */
    if(((uint16_t)eInterrupt & BITM_SPI_IEN_CS) == BITM_SPI_IEN_CS) 
    {
    /*check whether continuous mode is enabled*/
      if(((uint16_t)hDevice->pSpi->CTL & BITM_SPI_CTL_CON) != BITM_SPI_CTL_CON)
      {
        return ADI_SPI_UNSUPPORTED_MODE; 
      }
    }
    /*Enable the desired interrupts*/
    hDevice->pSpi->IEN |= (uint16_t)eInterrupt;
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  This function configure the device in Three pin SPI mode OR four pin SPI mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open()
 * @param[in]    bFlag        Specify which interrupt(s) need to be enabled.
 * \n              - true     Configure the device as 3-pin interface.
 * \n              - false     Configure the device as 4-pin interface.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * @note: By default, device is configured for 4-pin interface.
 */
ADI_SPI_RESULT adi_spi_SetThreepinMode (ADI_SPI_HANDLE const hDevice,const bool_t bFlag )
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->RD_CTL &=  (uint16_t)~BITM_SPI_RD_CTL_THREEPIN;
    if(bFlag == true)
    {
        hDevice->pSpi->RD_CTL |=(BITM_SPI_RD_CTL_THREEPIN);
    }
    return ADI_SPI_SUCCESS;
}
/*!
 * @brief  To overlap the  Tx and Rx.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Specify which interrupt(s) need to be enabled.
 * \n              - true     Configure the device in overlap mode.
 * \n              - false    Configure the device in non-overlap mode
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * @note:  In most of the slaves, the read data starts only after the master completes the transmission.This is a
 * \n      'non-overlapping' transfer.. In some slaves, there might be status bytes sent out while the command is being
 * \n      received and hence it may be required to  start receiving the bytes from the beginning of the CS frame.
 * \n      This is called 'overlapping' mode.
 */
ADI_SPI_RESULT adi_spi_SetOverlapMode (ADI_SPI_HANDLE const hDevice,const bool_t bFlag )
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->RD_CTL &= (uint16_t)~BITM_SPI_RD_CTL_OVERLAP;
    if(bFlag == true)
    {
        hDevice->pSpi->RD_CTL |=(BITM_SPI_RD_CTL_OVERLAP);
    }
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  To enable the read command mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Specify which interrupt(s) need to be enabled.
 * \n              - true     Enable  the read command mode.
 * \n              - false    Disable the read command mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * @note:  SPI read command mode where a 'command + address' is transmitted and read data is expected in the same CS frame
 */
ADI_SPI_RESULT adi_spi_SetReadCommand (ADI_SPI_HANDLE const hDevice,const bool_t bFlag )
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->RD_CTL &=  (uint16_t)~BITM_SPI_RD_CTL_CMDEN;
    if(bFlag == true)
    {
        hDevice->pSpi->RD_CTL |=(BITM_SPI_RD_CTL_CMDEN);
    }
    return ADI_SPI_SUCCESS;
}
/*!
 * @brief  To configure Polarity of RDY/MISO line.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Specify which interrupt(s) need to be enabled.
 * \n              - true     Polarity is active LOW. SPI master waits until RDY/MISO becomes LOW.
 * \n              - false    Polarity is active HIGH. SPI master waits until RDY/MISO becomes HIGH.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * @note:  SPI read command mode where a 'command + address' is transmitted and read data is expected in the same CS frame
 */
ADI_SPI_RESULT adi_spi_SetReadySignalPolarity (ADI_SPI_HANDLE const hDevice,const bool_t bFlag )
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->FLOW_CTL &=  (uint16_t)~BITM_SPI_FLOW_CTL_RDYPOL;
    if(bFlag == true)
    {
        hDevice->pSpi->FLOW_CTL |=(BITM_SPI_FLOW_CTL_RDYPOL);
    }
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  To configure PFlow control mode for data reads.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    eFlowMode    Specify the flow mode to be configured.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
* @note:  When #ADI_SPI_FLOW_MODE_RDY is used for flow-control, it could be any signal which is tied to this RDY input of SPI device.
 */
ADI_SPI_RESULT  adi_spi_SetFlowMode(ADI_SPI_HANDLE const hDevice,ADI_SPI_FLOW_MODE eFlowMode)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->FLOW_CTL &= (uint16_t)~BITM_SPI_FLOW_CTL_MODE;
    hDevice->pSpi->FLOW_CTL |=(eFlowMode);
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief   To specifies the number of SCLK cycles to wait before continuing the SPI read.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    nWaitTime    Specifies the number of SCLK cycles to wait before continuing the SPI read.
 *
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]    Device has not been previously configured for use.
 *
 * @note: This is not valid if when Flow control is set to #ADI_SPI_FLOW_MODE_TIMER and ignored for rest of the flow control mode.
 * \n     Value of "0" implies a wait time of 1 SCLK cycle
 */
ADI_SPI_RESULT  adi_spi_WaitTime(ADI_SPI_HANDLE const hDevice,uint16_t nWaitTime)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->WAIT_TMR = nWaitTime ;

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  To Configure the number of bytes to be received in a single burst from a slave before waiting for flow-control.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    nBytes       Specify the number of bytes to be transmitted before reading data from a slave.
 *                            Valid range is from  "0" to "15" for configuring "1" to "16".
 *
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_INVALID_PARAM [D]          Specified number of bytes are not valid.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]    Device has not been previously configured for use.

 *
* @note: This is not valid if when Flow control is set to #ADI_SPI_FLOW_MODE_NONE.
 */
ADI_SPI_RESULT  adi_spi_SetBurstSize(ADI_SPI_HANDLE const hDevice,uint8_t nBytes)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if(nBytes >0xFu)
    {
        return(ADI_SPI_INVALID_PARAM);
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->FLOW_CTL &= (uint16_t)~BITM_SPI_FLOW_CTL_RDBURSTSZ;
    hDevice->pSpi->FLOW_CTL |= (uint16_t)((uint16_t)nBytes << BITP_SPI_FLOW_CTL_RDBURSTSZ );
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Configure number of bytes to be transmitted before reading data from a slave.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    nBytes       Specify the number of bytes to be transmitted before reading data from a slave.
 *                            Valid range is from  "0" to "15" for configuring "1" to "16".
 *
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_INVALID_PARAM [D]          Specified number of bytes are not valid.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]    Device has not been previously configured for use.

 *
 */
ADI_SPI_RESULT  adi_spi_SetTransmitBytes(ADI_SPI_HANDLE const hDevice,uint8_t nBytes)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if(nBytes >0xFu)
    {
        return(ADI_SPI_INVALID_PARAM);
    }
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->RD_CTL &=  (uint16_t)~BITM_SPI_RD_CTL_TXBYTES;
    hDevice->pSpi->RD_CTL |=  (uint16_t)((uint16_t)nBytes << BITP_SPI_RD_CTL_TXBYTES );
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the DMA mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage whether to use DMA to effect SPI transfers (SPI1 only).
 *                - true     Enable DMA mode.
 *                - false    Disable DMA mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]        Device has not been previously configured for use.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 *\n Manages whether data transfers on SPI device will employ the DMA controller manage the transfer as a background process.
 *\n When coupled with non-blocking-mode, DMA transfers require polling calls to ascertain completion of the transfer.
 *
 * @sa        adi_spi_MasterComplete().
 * @sa        adi_spi_ReadWrite().
 */
ADI_SPI_RESULT adi_spi_EnableDmaMode (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    /* call whichever uninitializer is currently active */
    if (ADI_SPI_SUCCESS != hDevice->FunctionTable.pUnInitFcn(hDevice))
    {
        return ADI_SPI_DMA_ERROR;
    }

    hDevice->bDmaMode = bFlag;

    /* map DMA/interrupt function table */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)

    if(bFlag == true)
    {
      hDevice->FunctionTable.pInitFcn            = &dmaInit;
      hDevice->FunctionTable.pUnInitFcn          = &dmaClose;
      hDevice->FunctionTable.pInitDescriptorsFcn = &dmaInitializeDescriptors;
      hDevice->FunctionTable.pDmaHandlerFcn      = &dmaCommonHandler;
      hDevice->pSpi->IEN &= (uint16_t)~(BITM_SPI_IEN_IRQMODE);
    }

#endif /* ADI_I2S_CFG_ENABLE_DMA_SUPPORT */

    if(bFlag == false)
    {
      hDevice->FunctionTable.pInitFcn            = &intInit;
      hDevice->FunctionTable.pUnInitFcn          = &intUnInit;
      hDevice->FunctionTable.pInitDescriptorsFcn = &intInitializeDescriptors;
      hDevice->FunctionTable.pDmaHandlerFcn      = &intCommonHandler;
    }

    /* call the newly activated initializer */
    if (ADI_SPI_SUCCESS != hDevice->FunctionTable.pInitFcn(hDevice))
    {
        return ADI_SPI_DMA_ERROR;
    }

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Submit data buffers for SPI Master-Mode transaction in "Blocking mode".This function
 *\n          returns only after the data transfer is complete
 *
 * @param[in]     hDevice     Device handle obtained from adi_spi_Open().
 * @param[in]     pXfr        Pointer to transfer data struct #ADI_SPI_TRANSCEIVER.
  *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]        Invalid device handle parameter.
 *                - #ADI_SPI_BUFFER_NOT_SUBMITTED [D]  Failed to submit the buffer.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]   Device has not been previously configured for use.
 *                - #ADI_SPI_INVALID_POINTER [D]       Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]         Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                   Call completed successfully.
 *
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *\n
 *\n The #ADI_SPI_TRANSCEIVER contains all the parameters describing the
 *\n transfer.  There are two main parts to it: a the transmit data
 *\n and the receive data.  Each part has a size attribute, and each data pointer
 *\n has an increment associated with it.
 *\n
 *\n
 *\n Each call to adi_spi_MasterTransfer() must populate the following fields of the
 *\n #ADI_SPI_TRANSCEIVER block:
 *\n
 *\n @par TransmitterBytes
 *\n The number of  bytes to be transmitted. If the value is zero, data will not be transmitted from the
 *\n buffer pointed by pTransmitter.
 *\n
 *\n @par ReceiverBytes
 *\n The number of  bytes to be received. If the value is zero, data will not be stored in the
 *\n buffer pointed by pReceiver.
 *\n
 *\n @par pTransmitter
 *\n Pointer to the application-defined transmit data buffer.  This is the data sent out
 *\n over the SPI transmit wire (MOSI for Master-mode, MISO for Slave-mode) during the SPI transaction.
 *\n For  SPI DMA mode (which is 16-bit based), the transmit buffer must be 16-bit aligned.
 *\n
 *\n @par pReceiver
 *\n Pointer to the application-defined receive data buffer.  This is where the receive data
 *\n will be stored from the SPI receive wire (MISO for Master-mode, MOSI for Slave-mode)
 *\n during the SPI transaction.
 *\n For SPI DMA mode (which is 16-bit based), the receive buffer must be 16-bit aligned.
 *\n
 *\n @par bTxIncrement
 *\n Increment to be done for the transmit buffer after every transaction . The transmit data buffer
 *\n pointer is advanced as each byte is sent.  If it is set to zero, the transmit data pointer is stationary.
 *\ A stationary buffer pointer is useful for sending the same data to an external device or if
 *\n the source data is from a fixed memory address.
 *\n
 *\n @par bRxIncrement
 *\n Increment to be done for the receive buffer.   The transmit data buffer
 *\n pointer is advanced as each byte is sent. If it is value is set to zero, the receive
 *\n data pointer is stationary.  A stationary buffer pointer is useful for monitoring commands
 *\n from an external device or if the receive data is going to a fixed memory address.
 *\n
 *\n @note:
 *\n All data buffer allocations are made by the application and passed by pointer.
 *\ For \b blocking-mode calls,the driver will complete the transaction before returning.
 *\n For \b non-blocking calls, the application must call the appropriate polling function to
 *\n determine when the transaction is complete.  Between non-blocking polling calls, the
 *\n application may perform other tasks (as the transfer is interrupt-driven), so long as other SPI
 *\n transactions are not initiated.
 *\n
 *\n For  SPI DMA mode (which is 16-bit based), TransmitterBytes/ReceiverBytes is rounded up to an
 *\n even number by the SPI driver before submitting to DMA. Though the byte-centric API is retained
 *\n (for compatibility with previous integrations),the DMA mode processes all data as 16-bit.
 *\n Pliease align the buffer to 16 bit word boundary since the data transfer is 16bit.
 * @sa adi_spi_EnableDmaMode().
 * @sa adi_spi_MasterComplete().
 */
ADI_SPI_RESULT adi_spi_ReadWrite (ADI_SPI_HANDLE const hDevice, ADI_SPI_TRANSCEIVER* const pXfr)
{
    ADI_SPI_RESULT eResult;
    hDevice->bBlockingMode = true;
    eResult = adi_spi_MasterTransfer(hDevice,pXfr);
    hDevice->bBlockingMode = false;
    return(eResult);
}

/*!
 * @brief  Submit data buffers for SPI Master-Mode transaction.
 *
 * @param[in]     hDevice     Device handle obtained from adi_spi_Open().
 * @param[in]     pXfr        Pointer to transfer data struct.
  *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]        Invalid device handle parameter.
 *                - #ADI_SPI_IN_USE [D]    DMA transaction already underway.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]       Device has not been previously configured for use.
 *                - #ADI_SPI_INVALID_POINTER [D]     Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]    Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                       Call completed successfully.
 *
 *\n Request a synchronous transmit and receive of multiple data bytes
 *\n over the SPI serial channel.  Honors current blocking and DMA modes.
 *\n Buffer allocations are made by the calling code (the application).
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *
 *\n The #ADI_SPI_TRANSCEIVER contains all the parameters describing the
 *\n transfer.  It contains pointer to the transmit buffer and receive buffer.
 *\n Number of bytes to be transmitted and received need to be specified along with increment.
 *
 *\n Each call to adi_spi_MasterTransfer() must populate the following fields of the
 *\n #ADI_SPI_TRANSCEIVER block:
 *
 *\n @par TransmitterBytes
 *\n The number of  bytes to be transmitted. If the value is zero, data will not be transmitted from the
 *\n buffer pointed by pTransmitter. User need to  set this field  zero when there is nothing to transmit
 *\n by SPI device.
 *\n
 *\n @par ReceiverBytes
 *\n The number of  bytes to be received. If the value is zero, data will not be stored in the
 *\n buffer pointed by pReceiver. User need to  set this field  zero when there is nothing to receive
 *\n by SPI device.
 *\n
 *\n @par pTransmitter
 *\n Pointer to the application-defined transmit data buffer.  This is the data sent out
 *\n over the SPI transmit wire (MOSI for Master-mode, MISO for Slave-mode) during the SPI transaction.
 *\n For  SPI DMA mode (which is 16-bit based), the transmit buffer must be 16-bit aligned.
 *\n User need to  set this field to NULL if there is nothing to transmit.
 *\n
 *\n @par pReceiver
 *\n Pointer to the application-defined receive data buffer.  This is where the receive data
 *\n will be stored from the SPI receive wire (MISO for Master-mode, MOSI for Slave-mode)
 *\n during the SPI transaction. For SPI DMA mode (which is 16-bit based), the receive buffer
  *\n must be 16-bit aligned.User need to  set this field to NULL if there is nothing to receive.
 *\n
 *\n @par bTxIncrement
 *\n Increment to be done for the transmit buffer after every transaction . The transmit data buffer
 *\n pointer is advanced as each byte is sent.  If it is set to zero, the transmit data pointer is stationary.
 *\ A stationary buffer pointer is useful for sending the same data to an external device or if
 *\n the source data is from a fixed memory address.
 *\n
 *\n @par bRxIncrement
 *\n Increment to be done for the receive buffer.   The transmit data buffer
 *\n pointer is advanced as each byte is sent. If it is value is set to zero, the receive
 *\n data pointer is stationary.  A stationary buffer pointer is useful for monitoring commands
 *\n from an external device or if the receive data is going to a fixed memory address.
 *\n
 *\n @note:
 *\n All data buffer allocations are made by the application and passed by pointer.
 *\ For \b blocking-mode calls,the driver will complete the transaction before returning.
 *\n For \b non-blocking calls, the application must call the appropriate polling function to
 *\n determine when the transaction is complete.  Between non-blocking polling calls, the
 *\n application may perform other tasks (as the transfer is interrupt-driven), so long as other SPI
 *\n transactions are not initiated.
 *\n
  *\n For  SPI DMA mode (which is 16-bit based),TransmitterBytes/ReceiverBytes  is rounded up to an
 *\n even number by the SPI driver before submitting to DMA. Though the byte-centric API is retained
 *\n (for compatibility with previous integrations),the DMA mode processes all data as 16-bit.
 *\n Pliease align the buffer to 16 bit word boundary since the data transfer is 16bit.
 *\n flagged as an error.
 *
 * @sa adi_spi_ReadWrite().
 * @sa adi_spi_EnableDmaMode().
 * @sa adi_spi_MasterComplete().
 */
ADI_SPI_RESULT adi_spi_MasterTransfer (ADI_SPI_HANDLE const hDevice, ADI_SPI_TRANSCEIVER* const pXfr)
{
    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;
    volatile uint16_t nStatus;

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }

    if ((NULL == pXfr->pTransmitter) && (NULL == pXfr->pReceiver))
    {
        return ADI_SPI_INVALID_POINTER;
    }

    /* yell about odd byte counts in debug mode for  DMA requests */
    if ((hDevice->bDmaMode== true) && ((pXfr->TransmitterBytes&1u)!=0u))
    {
        return ADI_SPI_INVALID_PARAM;
    }

    /* yell about odd byte counts in debug mode for  DMA requests */
    if ((hDevice->bDmaMode== true) && ((pXfr->ReceiverBytes&1u)!=0u))
    {
        return ADI_SPI_INVALID_PARAM;
    }
    /* Return error if the RX buffer is not null and count is equal to zero or vice versa.*/
    if (((pXfr->pReceiver != NULL) && (pXfr->ReceiverBytes == 0u)) || ((pXfr->pReceiver == NULL) && ((pXfr->ReceiverBytes > 0u))))
    {
        return ADI_SPI_INVALID_PARAM;
    }
    
    /* Return error if the Tx buffer is not null and count is equal to zero or vice versa.*/
    if (((pXfr->pTransmitter != NULL) && (pXfr->TransmitterBytes == 0u)) || ((pXfr->pTransmitter == NULL) && (pXfr->TransmitterBytes > 0u)))
    {
        return ADI_SPI_INVALID_PARAM;
    }

	/* DMA count register is only 8 bits, so block size is limited to 255 */
    if ((hDevice->bDmaMode==true) && (pXfr->TransmitterBytes != 0u) &&(((uint32_t)pXfr->pTransmitter&0x1u) !=0u ) )
    {
        return ADI_SPI_INVALID_PARAM;
    }

    /* yell about (non-zero) odd byte counts in debug mode for DMA requests */
    if (   (hDevice->bDmaMode == true)
        && ((pXfr->ReceiverBytes != 0u) && (((uint32_t)pXfr->pReceiver & 0x1u) != 0u) ))
    {
        return ADI_SPI_INVALID_PARAM;
    }


     /* DMA count register is only 8 bits, so block size is limited to 255 */
    if (   (hDevice->bDmaMode==true)
        && (   ((pXfr->ReceiverBytes    & (~(uint32_t)BITM_SPI_CNT_VALUE)) !=0u)
            || ((pXfr->TransmitterBytes & (~(uint32_t)BITM_SPI_CNT_VALUE)) !=0u)))
    {
        return ADI_SPI_INVALID_PARAM;
    }

    if (((pXfr->ReceiverBytes == 0u) && (pXfr->TransmitterBytes == 0u)))
    {
        return ADI_SPI_INVALID_PARAM;
    }
#endif /* ADI_DEBUG */

    /* disable interrupts during manual prologue phase */
    ADI_DISABLE_INT(hDevice->pDevInfo->eIRQn);
    hDevice->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_TIM | BITM_SPI_CTL_RFLUSH | BITM_SPI_CTL_TFLUSH);
    if( (hDevice->bDmaMode==true)) 
    {
         hDevice->pSpi->IEN &=(uint16_t) ~(BITM_SPI_IEN_XFRDONE);
    } else 
    {
         hDevice->pSpi->IEN |= (uint16_t)(BITM_SPI_IEN_XFRDONE);
    }

    /* initialize the data phase */
    hDevice->pTxBuffer    = pXfr->pTransmitter;
    hDevice->pRxBuffer    = pXfr->pReceiver;
    hDevice->TxRemaining  = pXfr->TransmitterBytes;
    hDevice->RxRemaining  = pXfr->ReceiverBytes;
    hDevice->TxIncrement  = (uint8_t)pXfr->nTxIncrement;
    hDevice->RxIncrement  = (uint8_t)pXfr->nRxIncrement;

    /* clear any residual */
    nStatus = hDevice->pSpi->STAT;
    hDevice->pSpi->STAT = nStatus;

    hDevice->pSpi->CTL |= ( BITM_SPI_CTL_MASEN) ;

	/* initialize DMA descriptors and channel enables through the function pointer table */
	if (ADI_SPI_SUCCESS != (result = hDevice->FunctionTable.pInitDescriptorsFcn(hDevice)))
    {
		return result;
	}

			/* block as required on both channels */
	if (hDevice->bBlockingMode == true)
    {
#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
	PEND_EVENT(hDevice,ADI_SPI_SEMAPHORE_FAILED);
#else
#if (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT == 1)
        /* Instead of busy-waiting, we enter core sleep mode while waiting. Any
   		 * interrupt will also bring the processor out of core sleep and cause this call to return.
		*/

		PEND_EVENT(hDevice,ADI_SPI_FAILED);
#else
	   {
		   bool_t bComplete;
		   adi_spi_MasterComplete(hDevice,&bComplete);
		   while (bComplete == false)
		    {
				adi_spi_MasterComplete(hDevice,&bComplete);
            }
	    }
#endif
#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT  */
	}
    return result;
}

/*!
 * @brief  Get the SPI transaction completion status.
 *
 * @param[in]     hDevice      Device handle obtained from adi_spi_Open().
 *\n
 * @param[out] bComplete      Pointer to boolean variable that indicates
 *\n                - true    DMA transmit sequence is complete.
 *\n                - false   DMA transmit sequence is incomplete.
 *\n
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *
 * @sa        adi_spi_MasterTransfer().
 * @sa        adi_spi_ReadWrite().
 */
ADI_SPI_RESULT  adi_spi_MasterComplete(ADI_SPI_HANDLE const hDevice, bool_t* const bComplete)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif

    *bComplete = false;
    if (hDevice->bTxComplete == true)
    {
        if (hDevice->bRxComplete == true)
        {
            *bComplete = true;
        }
    }
    return(ADI_SPI_SUCCESS);
}

/*!
 * @brief  Submit data buffers for SPI Slave-Mode transaction.
 *
 * @param[in]     hDevice     Device handle obtained from adi_spi_Open().
 * @param[in]     pXfr        Pointer to transfer data struct.
  *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]        Invalid device handle parameter.
 *                - #ADI_SPI_IN_USE [D]    DMA transaction already underway.
 *                - #ADI_SPI_ERR_NOT_INITIALIZED [D]       Device has not been previously configured for use.
 *                - #ADI_SPI_INVALID_POINTER [D]     Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]    Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                       Call completed successfully.
 *
 *\n Request a synchronous transmit and receive of multiple data bytes
 *\n over the SPI serial channel.  Honors current blocking and DMA modes.
 *\n Buffer allocations are made by the calling code (the application).
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *
 *\n The #ADI_SPI_TRANSCEIVER contains all the parameters describing the
 *\n transfer.  It contains pointer to the transmit buffer and receive buffer.
 *\n Number of bytes to be transmitted and received need to be specified along with increment.
 *
 *\n Each call to adi_spi_MasterTransfer() must populate the following fields of the
 *\n #ADI_SPI_TRANSCEIVER block:
 *
 *\n @par TransmitterBytes
 *\n The number of  bytes to be transmitted. If the value is zero, data will not be transmitted from the
 *\n buffer pointed by pTransmitter. User need to  set this field  zero when there is nothing to transmit
 *\n by SPI device.
 *\n
 *\n @par ReceiverBytes
 *\n The number of  bytes to be received. If the value is zero, data will not be stored in the
 *\n buffer pointed by pReceiver.  User need to  set this field  zero when there is nothing to receive
 *\n by SPI device.
 *\n @par pTransmitter
 *\n Pointer to the application-defined transmit data buffer.  This is the data sent out
 *\n over the SPI transmit wire (MOSI for Master-mode, MISO for Slave-mode) during the SPI transaction.
 *\n For  SPI DMA mode (which is 16-bit based), the transmit buffer must be 16-bit aligned.
 *\n User need to  set this field to NULL if there is nothing to transmit.
 *\n
 *\n @par pReceiver
 *\n Pointer to the application-defined receive data buffer.  This is where the receive data
 *\n will be stored from the SPI receive wire (MISO for Master-mode, MOSI for Slave-mode)
 *\n during the SPI transaction. User need to  set this field to NULL if there is nothing to receive.
 *\n For SPI DMA mode (which is 16-bit based), the receive buffer must be 16-bit aligned.
 *\n
 *\n @par bTxIncrement
 *\n Increment to be done for the transmit buffer after every transaction . The transmit data buffer
 *\n pointer is advanced as each byte is sent.  If it is set to zero, the transmit data pointer is stationary.
 *\ A stationary buffer pointer is useful for sending the same data to an external device or if
 *\n the source data is from a fixed memory address.
 *\n
 *\n @par bRxIncrement
 *\n Increment to be done for the receive buffer.   The transmit data buffer
 *\n pointer is advanced as each byte is sent. If it is value is set to zero, the receive
 *\n data pointer is stationary.  A stationary buffer pointer is useful for monitoring commands
 *\n from an external device or if the receive data is going to a fixed memory address.
 *\n
 *\n @note:
 *\n All data buffer allocations are made by the application and passed by pointer.
 *\ For \b blocking-mode calls,the driver will complete the transaction before returning.
 *\n For \b non-blocking calls, the application must call the appropriate polling function to
 *\n determine when the transaction is complete.  Between non-blocking polling calls, the
 *\n application may perform other tasks (as the transfer is interrupt-driven), so long as other SPI
 *\n transactions are not initiated.
 *\n
 *\n For  SPI DMA mode (which is 16-bit based),TransmitterBytes/ReceiverBytes  is rounded up to an
 *\n even number by the SPI driver before submitting to DMA. Though the byte-centric API is retained
 *\n (for compatibility with previous integrations),the DMA mode processes all data as 16-bit.
 *\n Pliease align the buffer to 16 bit word boundary since the data transfer is 16bit.
 *\n flagged as an error.
 *
 * @sa adi_spi_ReadWrite().
 * @sa adi_spi_EnableDmaMode().
 * @sa adi_spi_SlaveComplete    ().
 */
ADI_SPI_RESULT adi_spi_SlaveTransfer (ADI_SPI_HANDLE const hDevice, ADI_SPI_TRANSCEIVER* const pXfr)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if (ADI_SPI_STATE_SLAVE != hDevice->eDevState)
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }

    if ((NULL == pXfr->pTransmitter) && (NULL == pXfr->pReceiver))
    {
        return ADI_SPI_INVALID_POINTER;
    }
    
    if ((0u == pXfr->pTransmitter)&&(0u == pXfr->ReceiverBytes) )
    {
        return ADI_SPI_INVALID_PARAM;
    }
    /* yell about odd byte counts in debug mode for  DMA requests */
    if ((hDevice->bDmaMode== true) && ((pXfr->TransmitterBytes&1u)!=0u))
    {
        return ADI_SPI_INVALID_PARAM;
    }
    /* Return error if the RX buffer is not null and count is equal to zero or vice versa.*/
    if (((pXfr->pReceiver != NULL) && (pXfr->ReceiverBytes == 0u)) || ((pXfr->pReceiver == NULL) && ((pXfr->ReceiverBytes > 0u))))
    {
        return ADI_SPI_INVALID_PARAM;
    }
    
    /* Return error if the Tx buffer is not null and count is equal to zero or vice versa.*/
    if (((pXfr->pTransmitter != NULL) && (pXfr->TransmitterBytes == 0u)) || ((pXfr->pTransmitter == NULL) && (pXfr->TransmitterBytes > 0u)))
    {
        return ADI_SPI_INVALID_PARAM;
    }

    /* yell about odd byte counts in debug mode for  DMA requests */
    if ((hDevice->bDmaMode== true) && ((pXfr->ReceiverBytes&1u)!=0u))
    {
        return ADI_SPI_INVALID_PARAM;
    }

    if ((hDevice->bDmaMode) && (pXfr->TransmitterBytes > 255u))
    {
        return ADI_SPI_INVALID_PARAM;
    }

	/* DMA count register is only 8 bits, so block size is limited to 255 */
    if ((hDevice->bDmaMode==true) && (pXfr->TransmitterBytes != 0u) &&(((uint32_t)pXfr->pTransmitter&0x1u) !=0u ) )
    {
        return ADI_SPI_INVALID_PARAM;
    }

    /* yell about (non-zero) odd byte counts in debug mode for DMA requests */
    if ((hDevice->bDmaMode == true) && ((pXfr->ReceiverBytes != 0u) && (((uint32_t)pXfr->pReceiver&0x1u) !=0u) ))
    {
        return ADI_SPI_INVALID_PARAM;
    }

#endif /* ADI_DEBUG */

    /* enable SPI */
    hDevice->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_TIM | BITM_SPI_CTL_RFLUSH | BITM_SPI_CTL_TFLUSH);
    hDevice->pSpi->CNT = (uint16_t) pXfr->ReceiverBytes;
    hDevice->bTransferComplete   = false;
    hDevice->pTxBuffer    = pXfr->pTransmitter;
    hDevice->pRxBuffer    = pXfr->pReceiver;
    hDevice->TxRemaining  = pXfr->TransmitterBytes;
    hDevice->RxRemaining  = pXfr->ReceiverBytes;
    hDevice->TxIncrement  = (uint8_t)pXfr->nTxIncrement;
    hDevice->RxIncrement  = (uint8_t)pXfr->nRxIncrement;

    if (ADI_SPI_SUCCESS != (hDevice->FunctionTable.pInitDescriptorsFcn(hDevice)))
    {
            return ADI_SPI_ERR_NO_DMA_FOR_DEVICE;
    }
	if (hDevice->bBlockingMode == true)
    {
#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
		PEND_EVENT(hDevice,ADI_SPI_SEMAPHORE_FAILED);
#else
#if (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT == 1)
        /* Instead of busy-waiting, we enter core sleep mode while waiting. Any
   		 * interrupt will also bring the processor out of core sleep and cause this call to return.
		*/

		PEND_EVENT(hDevice,ADI_SPI_SEMAPHORE_FAILED);
#else
	   {
		   bool_t bComplete;
		   adi_spi_SlaveComplete(hDevice,&bComplete);
		   while (bComplete == false)
		    {
				adi_spi_SlaveComplete(hDevice,&bComplete);
            }
	    }
#endif
#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT  */
	}

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Get the SPI slave transaction completion status.
 *
 * @param[in]     hDevice      Device handle obtained from adi_spi_SlaveInit().
 *\n
 * @param[out] bComplete      Pointer to boolean variable that indicates
 *\n                - true    DMA transmit sequence is complete.
 *\n                - false   DMA transmit sequence is incomplete.
 *\n
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *
 * @sa        adi_spi_SlaveTransfer().
 */
ADI_SPI_RESULT adi_spi_SlaveComplete (ADI_SPI_HANDLE const hDevice,bool_t * const bComplete)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    *bComplete = hDevice->bTransferComplete;
    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Assert or Deassert the Chip Select signal for the specified SPI device.
 *
 * @param[in]     hDevice      Device handle obtained from adi_spi_SlaveInit().
 *                CSOverride   Option to select the type of override.
 *
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *
 * @sa        adi_spi_SlaveTransfer().
 */
ADI_SPI_RESULT adi_spi_AssertChipSelect(ADI_SPI_HANDLE const hDevice, ADI_SPI_CS_OVERRIDE CSOverride)
{
  
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
    
#endif
    
    if (ADI_SPI_STATE_MASTER != hDevice->eDevState)
    {
        return ADI_SPI_INVALID_OPERATION;
    }
    hDevice->pSpi->CS_OVERRIDE  = CSOverride;

    return ADI_SPI_SUCCESS;
}
/*! \cond PRIVATE */

/*
 *****************************************************************************
 * SPI Internal Static Support Functions
 *****************************************************************************/


/*
 * Initialize the device state
 */
static ADI_SPI_RESULT CommonInit (ADI_SPI_HANDLE const hDevice, uint32_t nDeviceNum)
{
    /* (re)initialize device state */
    hDevice->eDevState        = ADI_SPI_STATE_UNINITIALIZED;
    hDevice->bBlockingMode    = false;
    hDevice->bDmaMode         = false;
    hDevice->bTransferComplete   = false;

    /* init callback */
    hDevice->pfCallback             = NULL;
    hDevice->pCBParam               = NULL;
    /* initialize function table entries to point to non-dma implimentations */
    hDevice->FunctionTable.pInitFcn            = &intInit;
    hDevice->FunctionTable.pUnInitFcn          = &intUnInit;
    hDevice->FunctionTable.pInitDescriptorsFcn = &intInitializeDescriptors;
    hDevice->FunctionTable.pDmaHandlerFcn      = &intCommonHandler;
    hDevice->pSpi                              = spi_device_info[nDeviceNum].pSpiRegs;
#if (ADI_SPI_CFG_INTERRUPT_MODE_SUPPORT == 1)
    ADI_INSTALL_HANDLER((uint32_t)hDevice->pDevInfo->eIRQn, hDevice->pDevInfo->pfInthandler);
#endif /* ADI_SPI_CFG_INTERRUPT_MODE_SUPPORT  */

#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    CREATE_SEM(hDevice, ADI_SPI_SEMAPHORE_FAILED);
#elif (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT ==1)
    /* init interrupt flag for low power mode */
    hDevice->bLowPowerExitFlag = false;

#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT  */

	return ADI_SPI_SUCCESS;
}


/*
 * Transfer internal default settings to SPI controller.
 */
static ADI_SPI_RESULT ConfigureCoreAsMaster (ADI_SPI_HANDLE const hDevice)
{

    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;

    /* initialize UCLK divider register for default frequency */
    if (ADI_SPI_SUCCESS != (result = adi_spi_SetBitrate(hDevice, ADI_SPI_BITRATE_INITIALIZER)))
    {
        return result;  /* failure */
    }

    ADI_ENTER_CRITICAL_REGION();

    /* initialize with FIFOs flushed, then set per initializer macro */
    hDevice->pSpi->CTL |= (BITM_SPI_CTL_TFLUSH | BITM_SPI_CTL_RFLUSH);

    /* initialize control register */
    hDevice->pSpi->CTL = ADI_SPI_MASTERCON_INITIALIZER;

    ADI_EXIT_CRITICAL_REGION();

    /* initialize the DMA register */
    hDevice->pSpi->DMA = ADI_SPI_DMA_INITIALIZER;

    return result;
}

/*
    //////////////////////////////////////////////////////////////////////////////
    ////////////////////   POINTER TABLE FUNCTIONS   /////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
*/


/* interrupt-mode initializer */
static ADI_SPI_RESULT intInit(ADI_SPI_HANDLE const hDevice) {
    return ADI_SPI_SUCCESS;
}


/* interrupt-mode uninitializer */
static ADI_SPI_RESULT intUnInit(ADI_SPI_HANDLE const hDevice) {
    return ADI_SPI_SUCCESS;
}


/* dma-mode initializer */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_SPI_RESULT dmaInit(ADI_SPI_HANDLE const hDevice) {

    /* allocate TX/RX channels for this device */
    if (ADI_DMA_SUCCESS != adi_dma_Open(hDevice->pDevInfo->eDMAChnlTxID,&hDevice->nDMAMemTx[0],&hDevice->hDmaTx,(ADI_CALLBACK)spi_DmaTx_InterruptHandler,hDevice))
    {
        return ADI_SPI_DMA_ERROR;
    }

    if (ADI_DMA_SUCCESS != adi_dma_Open(hDevice->pDevInfo->eDMAChnlRxID,hDevice->nDMAMemRx,&hDevice->hDmaRx,(ADI_CALLBACK)spi_DmaRx_InterruptHandler,hDevice))
    {
        return ADI_SPI_DMA_ERROR;
    }
     /* Configure the Tx descriptror */
     hDevice->gDmaDescriptorTx.pDstData  =  &hDevice->pSpi->TX;
     hDevice->gDmaDescriptorTx.SrcInc    =  ADI_DMA_INCR_2_BYTE;
     hDevice->gDmaDescriptorTx.DstInc    =  ADI_DMA_INCR_NONE;
     hDevice->gDmaDescriptorTx.DataWidth =  ADI_DMA_DATA_WIDTH;
     /* Configure the Rx descriptror */
     hDevice->gDmaDescriptorRx.pSrcData  =  &hDevice->pSpi->RX;
     hDevice->gDmaDescriptorRx.SrcInc    =  ADI_DMA_INCR_NONE ;
     hDevice->gDmaDescriptorRx.DstInc    =  ADI_DMA_INCR_2_BYTE;
     hDevice->gDmaDescriptorRx.DataWidth =  ADI_DMA_DATA_WIDTH;
    return ADI_SPI_SUCCESS;
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */


/* dma-mode uninitializer */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_SPI_RESULT dmaClose(ADI_SPI_HANDLE const hDevice) {

    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;
    /* shut down any DMA action */
    if (ADI_SPI_SUCCESS != (result = EnableDma(hDevice, false,(BITM_SPI_DMA_TXEN|BITM_SPI_DMA_RXEN ))))
    {
        return result;
    }
    if (ADI_DMA_SUCCESS != adi_dma_Close(hDevice->hDmaTx))
    {
        return ADI_SPI_DMA_ERROR;
    }
    if (ADI_DMA_SUCCESS != adi_dma_Close(hDevice->hDmaRx))
    {
        return ADI_SPI_DMA_ERROR;
    }
    return result;
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */

static void common_SPI_Int_Handler (ADI_SPI_DEV_DATA_TYPE* pDD)
{
    /* read status register - first thing */
    volatile  uint16_t nFifoStatus = pDD->pSpi->FIFO_STAT;
    uint16_t nErrorStatus = pDD->pSpi->STAT;

#if (ADI_SPI_CFG_INTERRUPT_MODE_SUPPORT == 1)
    uint16_t writableBytes;
    uint16_t readableBytes;

#ifdef ADI_DEBUG
    {
        /* Trap overflow/underflow errors in debug mode only. */
        if ((BITM_SPI_STAT_RXOVR & nErrorStatus) != 0u) {
          /* empty */
        }
        if ((BITM_SPI_STAT_TXUNDR & nErrorStatus) != 0u){
          /* empty */
        }
    }
#endif /* ADI_DEBUG */

    /* calculate number of bytes that can be written to tx fifo */
    writableBytes = 8u - ((BITM_SPI_FIFO_STAT_TX & nFifoStatus) >> BITP_SPI_FIFO_STAT_TX);
    /* calculate number of bytes to read from rx fifo */
    readableBytes = ((BITM_SPI_FIFO_STAT_RX & nFifoStatus) >> BITP_SPI_FIFO_STAT_RX);

    /* fill tx fifo */
    while ((writableBytes != 0u) && (pDD->TxRemaining != 0u))
    {
            pDD->pSpi->TX = *pDD->pTxBuffer;
            pDD->pTxBuffer  += pDD->TxIncrement;
            pDD->TxRemaining--;
            writableBytes--;
    }

    /* empty rx fifo */
    while ((readableBytes != 0u) &&(pDD->RxRemaining != 0u))
    {
            *pDD->pRxBuffer = (uint8_t) pDD->pSpi->RX;
            pDD->pRxBuffer += pDD->RxIncrement;
            pDD->RxRemaining--;
            readableBytes--;
    }
#endif /* ADI_SPI_CFG_INTERRUPT_MODE_SUPPORT  */

    /* termination */
    if((pDD->RxRemaining == 0u) && (pDD->TxRemaining == 0u)){
        /* only master controls CS */
        if (pDD->eDevState == ADI_SPI_STATE_MASTER) {
          if (BITM_SPI_STAT_XFRDONE == (nErrorStatus &BITM_SPI_STAT_XFRDONE ))
         {
         pDD->pSpi->IEN &= (BITM_SPI_IEN_XFRDONE);
         pDD->bRxComplete  = pDD->bTxComplete = true;
        }
          }
        else if (pDD->eDevState == ADI_SPI_STATE_SLAVE) {
            /* set slave complete flag */
            pDD->bTransferComplete = true;
            pDD->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_RFLUSH | BITM_SPI_CTL_TFLUSH);

        }
        else {

        }
        if (pDD->bBlockingMode == true) {

#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
            POST_EVENT(pDD);
#elif (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT ==1)
            /* Any interrupt that changes the SPI device state needs to bring the
             * processor out of low-power mode, as functions may be waiting for the
             * state to change.
             */
            POST_EVENT(pDD);

#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT  */
        }
        /* If a callback is registered notify the buffer processed event to the application */
        if(pDD->pfCallback){
            pDD->pfCallback(pDD->pCBParam, (uint32_t) ADI_SPI_EVENT_BUFFER_PROCESSED, NULL);
        }
    }

    /*All interrupts are cleared by a read of the status register */
     pDD->pSpi->STAT = nErrorStatus;
}


/*!
 * @brief  SPI0 Interrupt Handler.
 *
 * @return void.
 *
 * Overrides default SPI0 interrupt handler.
 */
ADI_INT_HANDLER(SPI0_Int_Handler) {
  ISR_PROLOG();
    common_SPI_Int_Handler(spi_device_info[0].hDevice );
  ISR_EPILOG();
}


/*!
 * @brief  SPI1 Interrupt Handler.
 *
 * @return void.
 *
 * Overrides default SPI1 interrupt handler.
 */
ADI_INT_HANDLER(SPI1_Int_Handler) {
  ISR_PROLOG();
    common_SPI_Int_Handler(spi_device_info[1].hDevice);
  ISR_EPILOG();
}

/*!
 * @brief  SPI2 Interrupt Handler.
 *
 * @return void.
 *
 * Overrides default SPI2 interrupt handler.
 */
ADI_INT_HANDLER(SPI2_Int_Handler) {
ISR_PROLOG();    
    common_SPI_Int_Handler(spi_device_info[2].hDevice );
ISR_EPILOG();
}


/*
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////   DMA-RELATED   ///////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
*/

/*
 * Directly manipulates the DMA eneble bit in the SPI DMA control register.
 */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_SPI_RESULT EnableDma (ADI_SPI_HANDLE const hDevice, const bool_t bFlag, uint16_t nDMAFlags)
{
    uint16_t mask;
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif

    mask = BITM_SPI_DMA_EN | nDMAFlags;

    if (true == bFlag)
    {
        hDevice->pSpi->DMA |= mask;
    }
    else
    {
        hDevice->pSpi->DMA &= (uint16_t)~mask;
    }

    return ADI_SPI_SUCCESS;
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */


/*
 * Enable or disable DMA interrupts on the transmit DMA channel.
 */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_SPI_RESULT EnableDmaTx(ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif
    if(adi_dma_Enable(hDevice->hDmaTx,bFlag) != ADI_DMA_SUCCESS)
    {
         return(ADI_SPI_DMA_ERROR);
    }
    if (false == bFlag) 
    {
        hDevice->pSpi->DMA &= (uint16_t)~BITM_SPI_DMA_TXEN;
    }

    return ADI_SPI_SUCCESS;
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */


/*
 * Enable or disable DMA interrupts on the receive DMA channel.
 */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_SPI_RESULT EnableDmaRx (ADI_SPI_HANDLE const hDevice, const bool_t bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((hDevice->eDevState != ADI_SPI_STATE_MASTER) && (hDevice->eDevState != ADI_SPI_STATE_SLAVE))
    {
        return ADI_SPI_ERR_NOT_INITIALIZED;
    }
#endif
    if(adi_dma_Enable(hDevice->hDmaRx,bFlag) != ADI_DMA_SUCCESS)
    {
         return(ADI_SPI_DMA_ERROR);
    }
    /*DMA enabling is done in the EnableDma function */
    if (false == bFlag) 
    {
        hDevice->pSpi->DMA &= (uint16_t)~BITM_SPI_DMA_RXEN;
    }

    return ADI_SPI_SUCCESS;
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */


/* stub for interrupt mode descriptor initialization (should never land here) */
static ADI_SPI_RESULT intInitializeDescriptors( ADI_SPI_HANDLE const hDevice)
{
    volatile uint16_t UNUSED byte;
    uint32_t nCount;
    /* enable SPI for receive initiate */
    hDevice->bTxComplete  = false;
    hDevice->bRxComplete  = false;
    nCount = hDevice->RxRemaining;
    if(hDevice->RxRemaining == 0u)
    {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_TIM |BITM_SPI_CTL_RFLUSH);
        nCount = hDevice->TxRemaining;
    }
    if(hDevice->TxRemaining == 0u)
    {
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_TFLUSH);
    }
    if(nCount < hDevice->TxRemaining)
    {
        nCount = hDevice->TxRemaining;
    }
    
    hDevice->pSpi->CNT = (uint16_t)nCount;
    nCount = 0u;

    /* interrupt mode: Fill in the FIFO and enable the TX by a dummy read. */
    while((nCount < ADI_SPI_FIFO_SIZE) && (hDevice->TxRemaining != 0u))
    {
        /* grab the lead byte */
        hDevice->pSpi->TX = *hDevice->pTxBuffer;
        /* modify tx pointer and buffer count prior to interrupt */
        hDevice->pTxBuffer += hDevice->TxIncrement;
        /* decrement the byte count */
        hDevice->TxRemaining--;
        nCount++;
    }
    
    if((hDevice->RxRemaining != 0u) || (hDevice->eDevState == ADI_SPI_STATE_SLAVE))
    {
           byte = hDevice->pSpi->RX;
    }
    ADI_ENABLE_INT(hDevice->pDevInfo->eIRQn);
    return ADI_SPI_SUCCESS;
}


/* DMA mode descriptor initialization */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
/*
 * Programs the DMA transmit and receive descriptors for ping-pong mode transfers, as needed.
 * We always use ping-pong transfer mode and the first two descriptors are always programmed,
 * even if the transfer would fit in one.  This is because ping-pong mode will always switch
 * to the next descriptor, even if the last one completed the transfer and the switch is to
 * a zero-sized descriptor.  The DMA interrupt at the end of each descriptor will program the
 * next one while the new descriptor is already underway.  Thats why we need to pre-program
 * two.
 */
static ADI_SPI_RESULT dmaInitializeDescriptors(ADI_SPI_HANDLE const hDevice)
{
    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;
    uint32_t nCount = hDevice->RxRemaining;
    volatile uint16_t UNUSED byte;
    uint16_t nSpiDMAFlags = 0u;
    /* initialize the transfer completion flags */
    if(hDevice->RxRemaining == 0u)
    {
       hDevice->pSpi->CTL |= (BITM_SPI_CTL_RFLUSH);
       nCount = hDevice->TxRemaining;
    }
    if(hDevice->TxRemaining == 0u)
    {
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_TIM;
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_TFLUSH);
    }
    if(nCount < hDevice->TxRemaining)
    {
        nCount = hDevice->TxRemaining;
    }
        
    hDevice->pSpi->CNT = (uint16_t)nCount;
    hDevice->gDmaDescriptorTx.pSrcData     = hDevice->pTxBuffer;
    hDevice->gDmaDescriptorTx.NumTransfers = (hDevice->TxRemaining + 1u) >> 1;
    hDevice->gDmaDescriptorRx.pDstData     = hDevice->pRxBuffer;
    hDevice->gDmaDescriptorRx.NumTransfers = (hDevice->RxRemaining + 1u) >> 1;
    hDevice->bTxComplete = hDevice->bRxComplete = true;
    if(hDevice->pTxBuffer != NULL)
    {
         if (ADI_DMA_SUCCESS != adi_dma_SubmitTransfer(hDevice->hDmaTx,&hDevice->gDmaDescriptorTx))
         {
             return(ADI_SPI_BUFFER_NOT_SUBMITTED);
         }
         EnableDmaTx(hDevice,true);
         nSpiDMAFlags |= BITM_SPI_DMA_TXEN;         
         hDevice->bTxComplete = false;
    }
    
    if(hDevice->pRxBuffer != NULL)
    {
         if (ADI_DMA_SUCCESS != adi_dma_SubmitTransfer(hDevice->hDmaRx,&hDevice->gDmaDescriptorRx))
         {
             return(ADI_SPI_BUFFER_NOT_SUBMITTED);
         }
         EnableDmaRx(hDevice,true);
         nSpiDMAFlags |= BITM_SPI_DMA_RXEN;
         hDevice->bRxComplete = false;

    }

    /* launch transfer with SPI DMA enable bit */
    if (ADI_SPI_SUCCESS != (result = EnableDma(hDevice, true,nSpiDMAFlags))) {
        return result;
    }
    if((hDevice->pSpi->CTL & BITM_SPI_CTL_TIM) != BITM_SPI_CTL_TIM)
    {
         byte = hDevice->pSpi->RX;    
    }
    ADI_ENABLE_INT(hDevice->pDevInfo->eIRQn);
    return result;
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */


/* Common DMA Handler for non-dma mode operation (nop) */
static void intCommonHandler(ADI_SPI_HANDLE pDD,ADI_DMA_CHANNEL_ID ChanId)
{
    /* No dma interrupts should ever come through this path.   */
    /* Only here for symmetry with the dmaCommonHandler() call */
    /* dispatched through function pointer pDmaHandlerFcn so   */
    /* we trap any such calls as an error and avoid silent     */
    /* failure.                                                */
    while (1) {}
    }


/* Common DMA Handler code for DMA-mode operation*/
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static void dmaCommonHandler (ADI_SPI_DEV_DATA_TYPE* pDD,ADI_DMA_CHANNEL_ID ChanId)
{
    /* recover device pointer */
    switch (ChanId) {
        case ADI_DMA_CHANNEL_SPI0_TX:
        case ADI_DMA_CHANNEL_SPI1_TX:
        case ADI_DMA_CHANNEL_SPI2_TX:
        pDD->pSpi->DMA &= (uint16_t)~BITM_SPI_DMA_TXEN;
        pDD->TxRemaining =0u;
        break;

        case ADI_DMA_CHANNEL_SPI0_RX:
        case ADI_DMA_CHANNEL_SPI1_RX:
        case ADI_DMA_CHANNEL_SPI2_RX:
             pDD->pSpi->DMA &= (uint16_t)~BITM_SPI_DMA_RXEN;
             pDD->RxRemaining =0u;
             pDD->bTransferComplete = true;
             break;
        default:
            break;

    }
}

/*!
 * @brief  SPI0 DMA Transmit Interrupt Handler.
 *
 * @return void.
 *
 * Overrides default SPI0 DMA transmit interrupt handler.
 */
static void spi_DmaTx_InterruptHandler(void *pCbparam, uint32_t Event, void *pArg)
{
    ADI_SPI_DEV_DATA_TYPE *pSPI_DevData = pCbparam;
    pSPI_DevData->FunctionTable.pDmaHandlerFcn(pSPI_DevData,ADI_DMA_CHANNEL_SPI0_TX);
    if(pSPI_DevData->RxRemaining == 0u)
    {
        pSPI_DevData->pSpi->IEN |= (BITM_SPI_IEN_XFRDONE);    
    }
}


/*!
 * @brief  SPI0 DMA Receive Interrupt Handler.
 *
 * @return void.
 *
 * Overrides default SPI0 DMA receive interrupt handler.
 */
static void spi_DmaRx_InterruptHandler(void *pCbparam, uint32_t Event, void *pArg)
{
    ADI_SPI_DEV_DATA_TYPE *pSPI_DevData = pCbparam;
    pSPI_DevData->FunctionTable.pDmaHandlerFcn(pSPI_DevData,ADI_DMA_CHANNEL_SPI0_RX);
    pSPI_DevData->pSpi->IEN |= (BITM_SPI_IEN_XFRDONE);       
   
}
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */

/*! \endcond */
/*
** EOF
*/

/*@}*/

#endif /* _ADI_SPI_V1_C_ */
