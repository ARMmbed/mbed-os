/*! *****************************************************************************
 * @file:    adi_spi.c
 * @brief:   SPI  device  driver global file.
 * @details: This a global file which includes a specific file based on the processor family.
 *           This included file will be  containing  SPI device  driver functions.
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

/** @addtogroup SPI_Driver SPI Driver
 *  @{
 * @brief Serial Peripheral Interface (SPI) Driver
 * @details The SPI driver manages all instances of the SPI peripheral.
 * @note The application must include drivers/spi/adi_spi.h to use this driver.
 * @note This driver requires the DMA driver.The application must include the DMA driver sources to avoid link errors.
 * @note Also note that the SPI will be configured by default to operate in Master mode. 
 * @note To configure the driver to operate in slave mode the static configuration file adi_spi_config.h must be modified. 
 * @note Specifically, the macro ADI_SPIx_MASTER_MODE must be set to '0' to indicate that slave mode functionality is needed. 
 * @note Since there are three SPI devices there are three macros, ADI_SPI0_MASTER_MODE, ADI_SPI1_MASTER_MODE and ADI_SPI2_MASTER_MODE to control the functionality of each SPI controller. 
 * @note Each instance of the SPI operates independently from all other instances.
 * @note
 * @note When operating the SPI at high bit rates the application may need to modify the IRQ interrupt mode. The API adi_spi_SetIrqmode() can be used for this.
 * @note At higher bit rates the ISR could mask a TX/RX interrupt. Specifically, it is possible that while servicing a TX/RX event another TX/RX event could occur. It is
 * @note possible, therefore, that when the ISR clears the interrupt status it will not only be clearing the current TX event but the next TX/RX event as well. The result
 * @note could that a final TX/RX event will not be processed. One way to work around this would be to set IRQMODE such that TX/RX events will occur only after N bytes
 * @note are in the FIFO. This will only work for short bursts less than the size of the FIFO. For larger transfer DMA mode, which will not have any of these issues,  should be used.
 * @note Finally, if interrupt mode is required at hight bit rates note that the SPI ISR has been designed with minimal cycle count as the highest priority. 
 * @note The ISR could certainly be modified to re-examine the FIFO before existing at the cost of additional cycles.
 */

 /*! \cond PRIVATE */
#include <adi_processor.h>
/*! \endcond */

#include <stdlib.h>  /* for 'NULL" definition */
#include <string.h>

#include <drivers/spi/adi_spi.h>
#include <drivers/pwr/adi_pwr.h>
#include <drivers/general/adi_drivers_general.h>
#include <adi_callback.h>
#include <rtos_map/adi_rtos_map.h>
#include "adi_spi_config.h"
#include <adi_cyclecount.h>


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
* Pm088 (rule 17.4): pointer arithmetic should not be used.
*   Relying on pointer arithmetic for buffer handling.
*
* Pm152: (MISRA C 2004 rule 17.4) array indexing shall only be applied to objects defined as an array type  
*   Accessing the DMA descriptors, which are defined in the system as a pointer to an array of descriptors
*
* Pm151 (rule 17.4): array indexing shall only be applied to objects of array type
* Pm123 (rule 18.5): there shall be no definition of objects in a header file
*
* Pm50: (MISRA C 2004 rule 14.3) a null statement shall only occur on a line by itself, and shall not have any other text on the same line
*    Some Macros, such as ISR_PROLOGUE, may not have any expansion resulting in just the terminating ';'
*
*Pm140: (MISRA C 2004 rule 11.3) a cast should not be performed between a pointer type and an integral type
*    MMR addresses are defined as simple constants. Accessing the MMR requires casting to a pointer type
*
* Pm031: (MISRA C 2004 rule 12.7)  bitwise operations shall not be performed on signed integer types 
*    MMR macros are beyond the control of the driver.
*
*/
#pragma diag_suppress=Pm050,Pm073,Pm088,Pm123,Pm143,Pm152,Pm140,Pm031

#endif /* __ICCARM__ */

#include "adi_spi_data.c"

/*! \cond PRIVATE */

/* handle checker for debug mode */
#define ADI_SPI_VALIDATE_HANDLE(h) ((spi_device_info[0].hDevice != (h)) && (spi_device_info[1].hDevice != (h)) && (spi_device_info[2].hDevice != (h)))

/*! \endcond */

/*
 * Local prototypes
 */
static void common_SPI_Int_Handler (ADI_SPI_DEV_DATA_TYPE* pDD);
static void StartTransaction       (ADI_SPI_HANDLE const hDevice, const ADI_SPI_TRANSCEIVER* const pXfr);
static void TxDmaErrorCallback     (void *pCBParam, uint32_t Event, void *pArg);
static void RxDmaErrorCallback     (void *pCBParam, uint32_t Event, void *pArg);

/* ISR forward declarations */
/*! \cond PRIVATE */
void SPI0_Int_Handler(void);
void SPI1_Int_Handler(void);
void SPI2_Int_Handler(void);
void DMA_SPI0_TX_Int_Handler(void);
void DMA_SPI0_RX_Int_Handler(void);
void DMA_SPI1_TX_Int_Handler(void);
void DMA_SPI1_RX_Int_Handler(void);
void DMA_SPIH_TX_Int_Handler(void);
void DMA_SPIH_RX_Int_Handler(void);
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
 * @param [in]  pDevMemory   Pointer to a buffer of size ADI_SPI_MEMORY_SIZE
 *\n                         required by the driver for the operation of specified SPI device.
 *
 * @param [in]  nMemorySize  Size of the buffer to which "pMemory" points. 
 *
 * @param[out]  phDevice     The caller's device handle pointer for storing the initialized device instance data pointer.
 *
 * @return            Status
 *                    - #ADI_SPI_INVALID_DEVICE_NUM [D]     Invalid device index.
 *                    - #ADI_SPI_INVALID_PARAM [D]          Invalid parameter.
 *                    - #ADI_SPI_SEMAPHORE_FAILED           Semaphore creation failed.
 *                    - #ADI_SPI_DMA_REG_FAILED             Failed to register DMA callbacks with common DMA service.
 *                    - #ADI_SPI_IN_USE                     SPI is already open and in use.
 *                    - #ADI_SPI_SUCCESS                    Call completed successfully.
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
ADI_SPI_RESULT adi_spi_Open(uint32_t nDeviceNum,
                            void *pDevMemory,
                            uint32_t nMemorySize,
                            ADI_SPI_HANDLE* const phDevice)
{

#ifdef ADI_DEBUG

    if (nDeviceNum >= ADI_SPI_NUM_INSTANCES)
    {
        return ADI_SPI_INVALID_DEVICE_NUM;
    }

    if (nMemorySize != sizeof(struct __ADI_SPI_DEV_DATA_TYPE))
    {
        return ADI_SPI_INVALID_PARAM;
    }
    
    if( spi_device_info[nDeviceNum].hDevice != NULL )
    {
        return ADI_SPI_IN_USE;
    }

#endif

    ADI_SPI_HANDLE hDevice = pDevMemory;
    
    /* 
     * Link the two data structures together.
     *   
     *         ADI_SPI_DEVICE_INFO  <==>   ADI_SPI_HANDLE
     *
     * Clear the ADI_SPI_HANDLE memory. This also sets all bool 
     * structure members to false so we do not need to waste cycles 
     * setting these explicitly (e.g. hDevice->bDMA = false)
     *
     * Other fields, such as callback related fields, are also zeroed
     * and therefore properly initialized.
     */
    
    spi_device_info[nDeviceNum].hDevice = (ADI_SPI_DEV_DATA_TYPE *)pDevMemory;
    memset(pDevMemory,0,nMemorySize);
    hDevice->pDevInfo =  &spi_device_info[nDeviceNum];


    /* 
     * Although the ADI_SPI_DEVICE_INFO struct has the address of the SPI registers
     * for this instance, copying it to the ADI_SPI_HANDLE struct will minimize
     * the runtime footprint and cycle count when accessing the SPI registers
     */
    hDevice->pSpi = spi_device_info[nDeviceNum].pSpiRegs;

    SEM_CREATE(hDevice, "SPI_SEM", ADI_SPI_SEMAPHORE_FAILED);

   /* Static Configuration */
    /* Initialize the device based on the given configuration parameters */
    ADI_SPI_CFG_TYPE const* pSPICfg = &gSPICfg[nDeviceNum];
    hDevice->pSpi->CTL = pSPICfg->SPI_CTL;
    hDevice->pSpi->DIV = pSPICfg->SPI_DIV;

    /* write the device data pointer into the caller's handle */
    *phDevice = hDevice;
    hDevice->pSpi->CTL |= BITM_SPI_CTL_SPIEN;
    
    /* Make sure the DMA controller and its SRAM based descriptors are initialized */
    adi_dma_Init();

    /* Setup the DMA TX callback */
    if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback((DMA_CHANn_TypeDef) hDevice->pDevInfo->dmaTxChannelNumber, TxDmaErrorCallback, (void *) hDevice)) 
    {
         return  ADI_SPI_DMA_REG_FAILED;
    }

    /* Setup the DMA RX callback */
    if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback((DMA_CHANn_TypeDef) hDevice->pDevInfo->dmaRxChannelNumber, RxDmaErrorCallback, (void *) hDevice)) 
    {
         return  ADI_SPI_DMA_REG_FAILED;
    }

    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Uninitialize and deallocate an SPI device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_spi_Open().
 *
 * @return      Status
 *                - #ADI_SPI_INVALID_HANDLE [D]       Invalid device handle parameter.
 *                - #ADI_SPI_SUCCESS                  Call completed successfully.
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

#endif
    
    
    /* disable Interrupt    */
    NVIC_DisableIRQ(hDevice->pDevInfo->eIRQn);

    
     /* destroy semaphore */
    SEM_DELETE((ADI_SPI_HANDLE) hDevice,ADI_SPI_SEMAPHORE_FAILED);

    /* invalidate initialization state */
    hDevice->pDevInfo->hDevice  = NULL;
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
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 */
ADI_SPI_RESULT adi_spi_RegisterCallback (ADI_SPI_HANDLE const hDevice,  ADI_CALLBACK const pfCallback, void *const pCBParam )
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice)) {
        return ADI_SPI_INVALID_HANDLE;
    }

#endif
    /* Save the application provided callback and callback parameters */
    hDevice->pfCallback = pfCallback;
    hDevice->pCBParam   = pCBParam;

    return ADI_SPI_SUCCESS;
}

/*!
 * @brief  Set the IRQ mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    nMode        IRQ mode value to set.
*                - true     Set continuous transfer mode.
*                - false    Clear continuous transfer mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * These bits configure when the Tx/Rx interrupts occur in a transfer. 
 * For DMA Rxtransfer, these bits should be 0. 
 * Value values are 0-7
 *      Tx interrupt occurs when (nMode+1) byte(s) has been transferred.
 *      Rx interrupt occurs when (nMode+1) or more bytes have been received into the FIFO.
 *
 *  @note The application will have to carefully manage IRQMODE relative to a transaction's buffer size.
 *  @note Specifically, the application must ensure that the last byte causes an interrupt else the 
 *  @note transaction will not terminate. As explained in the SPI driver overview, this functionality
 *  @note is typically needed when operating in interrupt mode with a high SPI bit rate (typically issues
 *  @note are seen at SPI clock rates of 4MHz or greater). The max clock rate will vary depending on the application.
 *  @note The max clock rate is a function of the SPI ISR cycle count plus any other delay that might be caused
 *  @note by other parts of the system. Finally, please note that while sustaining interrupt mode SPI transaction
 *  @note at high bit rates will work buffers that are the size of the SPI FIFO or less, transactions that are 
 *  @note larger that the size of the FIFO may run into issues associated with masked/lost interrupts. If this
 *  @note does prove to be an issue for an applicatoon then the SPI ISR could be modified to examine the FIFO
 *  @note status on a continuous basis in the ISR (as opposed to examining the FIFO status just once at the start
 *  @note of the ISR). However, adding this functionality to the ISR will increase the ISR cycle count and footprint. 
 *
 */
ADI_SPI_RESULT adi_spi_SetIrqmode (ADI_SPI_CONST_HANDLE const hDevice, const uint8_t nMode)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice)) {
        return ADI_SPI_INVALID_HANDLE;
    }

#endif
    
    uint16_t ien = hDevice->pSpi->IEN;
    ien = ien & (uint16_t)~BITM_SPI_IEN_IRQMODE;
    ien = ien | (nMode & BITM_SPI_IEN_IRQMODE);
    hDevice->pSpi->IEN = ien;

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
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Setting this mode causes the SPI controller to drive the Chip Select signal continuously until the transaction
 * is complete.  Clearing it causes Chip Select to cycle between bytes.
 *
 *
 */
ADI_SPI_RESULT adi_spi_SetContinuousMode (ADI_SPI_CONST_HANDLE const hDevice, const bool bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice)) {
        return ADI_SPI_INVALID_HANDLE;
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
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Set or clear the internal SPI loopback mode.  Primarily used for testing.
 *
 */
ADI_SPI_RESULT adi_spi_SetLoopback (ADI_SPI_CONST_HANDLE const hDevice, const bool bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
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
 * @brief  Set SPI Master-Mode operation.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to select either Master-Mode or Slave-Mode operation.
 *\n                - true     Enable Master-Mode. Default.
 *\n                - false    Enable Slave-Mode.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Controls SPI Master/Slave mode of operation, set for Master-Mode, clear for Slave-Mode.
 *
 */
ADI_SPI_RESULT adi_spi_SetMasterMode (ADI_SPI_CONST_HANDLE const hDevice, const bool bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

#endif
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    if (true == bFlag) {        /* hardware default */
        hDevice->pSpi->CTL |= (ADI_SPI_MASTERCON_INITIALIZER);
   } else {
        hDevice->pSpi->CNT = 0u;
        hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_MASEN;
        hDevice->pSpi->CTL |= (ADI_SPI_SLAVECON_INITIALIZER);
    }
    ADI_EXIT_CRITICAL_REGION();
    return ADI_SPI_SUCCESS;
}


/*!
 * @brief  Set the SPI receive FIFO overflow mode.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open().
 * @param[in]    bFlag        Flag to manage receive FIFO overflow behaviour.
 *\n              - true     Discard old data on receive FIFO overflow.
 *\n              - false    Discard new data on receive FIFO overflow.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Controls what to do with excess incoming data when the receive FIFO becomes full.
 * Either the new data or the old data is discarded.  Set the receive FIFO overflow mode
 * to replace data in the RX register (top of receive FIFO) with the incoming new data.
 * Clear it to discard incoming new data and preserve old unread data.

 *
 */
ADI_SPI_RESULT adi_spi_SetReceiveOverflow (ADI_SPI_CONST_HANDLE const hDevice, const bool bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
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
 * @param[in]    bFlag        Flag to manage transmit FIFO underflow behaviour.
 *\n               - true     Send zeroes on transmit FIFO underflow.
 *\n               - false    Resend last data on transmit FIFO underflow.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 *\n  Controls what to transmit when lacking valid data because the transmit FIFO is empty.
 *\n  Either zeros or the last valid data are transmitted.  Set transmit FIFO underflow mode to send zeros.
 *\n Clear it to resend the last transmitted data.
 *
 */
ADI_SPI_RESULT adi_spi_SetTransmitUnderflow (ADI_SPI_CONST_HANDLE const hDevice, const bool bFlag)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
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
 * @brief  Set the SPI serial clock frequency.
 *
 * @param[in]    hDevice      Device handle obtained from adi_spi_Open()
 * @param[in]    Hertz        Target frequency (in Hz) for SPI bitrate.
 *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_INVALID_PARAM              Specified frequency is out of range.
 *                - #ADI_SPI_BAD_SYS_CLOCK              Unable to obtain PCLK which is needed to calculate the new bit rate.
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *
 * Compute and set the internal SPI clock rate divider register to produce
 *\n the desired serial clock frequency.  Resulting frequency is subject to arithmetic rounding errors.
 *\n Use #adi_spi_GetBitrate() to obtain the exact frequency produced, including rounding errors.
 *
 * @sa        adi_spi_GetBitrate().
 */
ADI_SPI_RESULT adi_spi_SetBitrate (ADI_SPI_CONST_HANDLE const hDevice, const uint32_t Hertz)
{
    uint32_t incoming_clock;
    uint16_t Div;

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif

    if( adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &incoming_clock) != ADI_PWR_SUCCESS)
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    /* requested rate needs to be 2x or less than incoming clock */
    if ((2U * Hertz) > incoming_clock)
    {
        return ADI_SPI_BAD_SYS_CLOCK;
    }

     /* compute the SPI divider value */
     Div = (uint16_t) ((incoming_clock / Hertz) >> 1U) - 1U;  /* '>>1' is really a divide by 2 */

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
 * @param[out]    pnBitrate   Pointer to the location where Bitrate need to be written.
 *
 * @return
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Get the current serial clock frequency.  The returned value is exact but
 *\n may not exactly match the value set with #adi_spi_SetBitrate() due to
 *\n computational round-off errors resulting from fixed register size and
 *\n finite-precision arithmetic.
 *
 * @sa        adi_spi_SetBitrate().
 */
ADI_SPI_RESULT adi_spi_GetBitrate (ADI_SPI_CONST_HANDLE const hDevice, uint32_t* const pnBitrate)
{
    uint32_t incoming_clock;
    ADI_PWR_RESULT ePwrResult;
    uint32_t Div;

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif
    Div = hDevice->pSpi->DIV;  /* assumes this is always a right-justified value */

    ePwrResult = adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &incoming_clock);
    if(ePwrResult != ADI_PWR_SUCCESS)
    {
        *pnBitrate= 0u;
         return(ADI_SPI_FAILURE);
    }
    *pnBitrate= (incoming_clock / (Div + 1U)) >> 1U;  /* '>>1' is divide by 2 */
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
 *                - #ADI_SPI_SUCCESS                        Call completed successfully.
 *
 * Sets the desired chip select to use for activating an external slave device.
 *
 * @note      Chip select \a ADI_SPI0_CSn is reserved for SPI device 0 (SPI0) internal chip select line
 *            dedicated for communications with the UHF device.
 *
 */
ADI_SPI_RESULT adi_spi_SetChipSelect (ADI_SPI_HANDLE const hDevice, const ADI_SPI_CHIP_SELECT eChipSelect)
{

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif

    hDevice->ChipSelect = eChipSelect;

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
 *                - #ADI_SPI_INVALID_POINTER [D]       Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]         Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                   Call completed successfully.
 *
 *\n
 *\n Request a non-blocking mode transmit and receive of multiple data bytes
 *\n over the SPI serial channel.  
 *\n Buffer allocations are made by the calling code (the application).
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *\n
 *\n
 * @sa adi_spi_MasterSubmitBuffer().
 * @sa ADI_SPI_TRANSCEIVER
 */
ADI_SPI_RESULT adi_spi_MasterReadWrite (ADI_SPI_HANDLE const hDevice, const ADI_SPI_TRANSCEIVER* const pXfr)
{
    ADI_SPI_RESULT eResult;
    hDevice->bBlockingMode = true;
    eResult = adi_spi_MasterSubmitBuffer(hDevice,pXfr);
    hDevice->bBlockingMode = false;
    if( (eResult == ADI_SPI_SUCCESS) && (hDevice->HWErrors != 0u))
    {
      eResult = ADI_SPI_HW_ERROR_OCCURRED;
    }
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
 *                - #ADI_SPI_IN_USE [D]    DMA transaction already under way.
 *                - #ADI_SPI_INVALID_POINTER [D]     Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]    Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                       Call completed successfully.
 *
 *\n Request a blocking mode transmit and receive of multiple data bytes
 *\n over the SPI serial channel.  
 *\n Buffer allocations are made by the calling code (the application).
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *
 *
 * @sa adi_spi_MasterReadWrite().
 * @sa adi_spi_isBufferAvailable()
 * @sa ADI_SPI_TRANSCEIVER
 */

ADI_SPI_RESULT adi_spi_MasterSubmitBuffer (ADI_SPI_HANDLE const hDevice, const ADI_SPI_TRANSCEIVER* const pXfr)
{
    ADI_SPI_RESULT result = ADI_SPI_SUCCESS;
    volatile uint16_t nStatus;

#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }

    if ((NULL == pXfr->pTransmitter) && (NULL == pXfr->pReceiver))
    {
        return ADI_SPI_INVALID_POINTER;
    }
    
    if( (pXfr->bRD_CTL == true) && (pXfr->TransmitterBytes > 16u))
    {
      return ADI_SPI_INVALID_PARAM;
    }

#endif /* ADI_DEBUG */

    /* Initialize the transaction. 'hDevice' must hold the transaction values as pXfr is owned by the application */
    hDevice->pTxBuffer    = pXfr->pTransmitter;
    hDevice->pRxBuffer    = pXfr->pReceiver;
    hDevice->TxRemaining  = pXfr->TransmitterBytes;
    hDevice->RxRemaining  = pXfr->ReceiverBytes;
    hDevice->TxIncrement  = (uint8_t)pXfr->nTxIncrement;
    hDevice->RxIncrement  = (uint8_t)pXfr->nRxIncrement;
    hDevice->bDmaMode     = pXfr->bDMA;
    hDevice->bRdCtlMode   = pXfr->bRD_CTL;
    hDevice->bTransferComplete   = false;
    hDevice->HWErrors            = ADI_SPI_HW_ERROR_NONE;

    
    /* 
     *
     * TIM
     *   If set:   initiate transfer with write to SPI_TX register
     *   If clear:  initiate transfer with a read from SPI_RX register
     *
     * RFLUSH
     *   Clear this bit to ensure that incoming data is ignored
     *
     * TFLUSH
     *   Clear this not to ensure that transmitted data is not a zero (if SPI_CTL.ZEN is set) or last transmitted byte
     *
     */
    

    hDevice->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_TIM | BITM_SPI_CTL_RFLUSH | BITM_SPI_CTL_TFLUSH);

    /*
     * If in DMA mode then make sure XFRDONE interrupt is not set. DMA mode will generate three interrupts
     *   TX DMA
     *   RX DMA
     *   XFRDONE
     *
     * There is a race condition between XFRDONE and DMA interrupts. They are on different clocks.
     * 
     * SPI XfrDone is counted on SPI clock (SCL) edge, which is a fixed timing related to SPI bit protocol. 
     * But the DMA works upon system clock (HCLK) and it could finish on various timing upon SCL/HCLK ratio.
     * And bus bandwidth (e.g., DMA hold off until processor frees up the bus). So SPI RX DMA done interrupt 
     * could be issued earlier or later than SPI XferDone interrupt.
     *
     */
    if( hDevice->bDmaMode==true ) {
      /* The race condition has been between RX and XFRDONE. If there are no bytes to receive then */
      /* do not clear XFRDONE                                                                      */
      if( hDevice->RxRemaining != 0u) {
        hDevice->pSpi->IEN &= (uint16_t)~(BITM_SPI_IEN_XFRDONE);
      } else {
        hDevice->pSpi->IEN |= (BITM_SPI_IEN_XFRDONE);
      }
      
    } else {
      
      /* In interrupt mode always enable XFRDONE */
      uint16_t activeInterrupts = BITM_SPI_IEN_XFRDONE;
      /* Enable underflow on;y if sending bytes */
      if( hDevice->TxRemaining ) {
        activeInterrupts |= BITM_SPI_IEN_TXUNDR;
      }
      /* Enable overflow only if receiving bytes */
      if( hDevice->RxRemaining ) {
        activeInterrupts |= BITM_SPI_IEN_RXOVR;
      }
      
      hDevice->pSpi->IEN |= activeInterrupts;
      
      /* 
       * In interrupt mode, when there is nothing to receive, need to initiate a transaction
       * on an TX write only. Initiating on an RX read will start the transaction, but just for
       * a single byte (and we're not sure why this is true)
       */
      
      if( hDevice->RxRemaining == 0u) {
        hDevice->pSpi->CTL |= ( BITM_SPI_CTL_TIM ); 
      }
        
    }


    /* STAT bits are cleared by writing a '1' to them. Clear any residual status*/
    nStatus = hDevice->pSpi->STAT;
    hDevice->pSpi->STAT = nStatus;

    /* Make sure we are in master mode */
    hDevice->pSpi->CTL |= ( BITM_SPI_CTL_MASEN);

    /* Set ChipSelect */
    hDevice->pSpi->CS_CTL  = hDevice->ChipSelect;
  
    StartTransaction(hDevice, pXfr);
    
    
    /* block if required  */
    if (hDevice->bBlockingMode == true)
    {
	SEM_PEND(hDevice,ADI_SPI_PEND_FAILED);
    }
    
    return result;
}

/*********************************************************************************************************/
/*                                                                                                       */
/*                        SPI DRIVER Master Mode transaction start                                       */
/*                                                                                                       */
/*********************************************************************************************************/

static void StartTransaction(ADI_SPI_HANDLE const hDevice, const ADI_SPI_TRANSCEIVER* const pXfr)
{ 
    /* Transaction completion is determined by the number of bytes to be received */
    uint16_t nCount;
    
    /* Effectively flush the FIFOs before the start of the next transaction */
    hDevice->pSpi->CTL |= (BITM_SPI_CTL_RFLUSH|BITM_SPI_CTL_TFLUSH);
    hDevice->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_RFLUSH|BITM_SPI_CTL_TFLUSH);

    /* Disable any prior notion of DMA */    
    hDevice->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);

   
    /*
     * If the transaction is DMA based then set up the DMA descriptors for this transaction
     */

    uint16_t dmaFlags = 0u;
    
    if( hDevice->bDmaMode == true) 
    {
      dmaFlags = BITM_SPI_DMA_EN;

      uint16_t sz = pXfr->TransmitterBytes;
      if( sz )
      {
        uint16_t TxChanNum = hDevice->pDevInfo->dmaTxChannelNumber;
    
        /* Enable the interrupt for the given DMA */
        NVIC_EnableIRQ((IRQn_Type)(hDevice->pDevInfo->dmaTxIrqNumber));

        /* Disables source address decrement for TX channel */
        pADI_DMA0->SRCADDR_CLR = 1U << TxChanNum;
      
        /* Enable the channel */
        pADI_DMA0->EN_SET = 1U << TxChanNum;
          
        /* Enables SPI peripheral to generate DMA requests. */
        pADI_DMA0->RMSK_CLR = 1U << TxChanNum;

        /* Set the primary as the current DMA descriptor */
        pADI_DMA0->ALT_CLR = 1U << TxChanNum;
     
        /* fill in the DMA RAM descriptors */
        if( (sz & 1U) != 0u )
        {
          /* DMA is performed on 16-bit data. Make sure the DMA engine is properly aligned to even counts */
          /* The SPI_CNT register will hold the "real" transfer count                                     */
          sz++;
        }
        
        pPrimaryCCD[TxChanNum].DMASRCEND =   (uint32_t)(pXfr->pTransmitter + (sz - 2U));
              
        pPrimaryCCD[TxChanNum].DMADSTEND =   (uint32_t)&hDevice->pSpi->TX;

        pPrimaryCCD[TxChanNum].DMACDC = ((uint32_t)ADI_DMA_INCR_NONE << DMA_BITP_CTL_DST_INC) |
                              (ADI_DMA_INCR_2_BYTE << DMA_BITP_CTL_SRC_INC) |
                              (ADI_DMA_WIDTH_2_BYTE << DMA_BITP_CTL_SRC_SIZE) |
                              ((sz/2U -1U)<< DMA_BITP_CTL_N_MINUS_1) |
                              (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL);
      
        dmaFlags |= (BITM_SPI_DMA_TXEN);
      }

      sz = pXfr->ReceiverBytes;
      if( sz )
      {
        
        uint16_t RxChanNum = hDevice->pDevInfo->dmaRxChannelNumber;
        NVIC_EnableIRQ((IRQn_Type)(hDevice->pDevInfo->dmaRxIrqNumber));

        /* Disables destination address decrement for RX channel */
        pADI_DMA0->DSTADDR_CLR = 1U << RxChanNum;
        
        /* Enable the channel */
        pADI_DMA0->EN_SET = 1U << RxChanNum;
        
        /* Enables SPI peripheral to generate DMA requests. */
        pADI_DMA0->RMSK_CLR = 1U << RxChanNum;

        /* Set the primary as the current DMA descriptor */
        pADI_DMA0->ALT_CLR = 1U << RxChanNum;
        
        if( (sz & 1U) != 0u )
        {
          /* DMA is performed on 16-bit data. Make sure the DMA engine is properly aligned to even counts */
          /* The SPI_CNT register will hold the "real" transfer count                                     */
          sz++;
        }
        
        pPrimaryCCD[RxChanNum].DMASRCEND =   (uint32_t)&hDevice->pSpi->RX;
              
        pPrimaryCCD[RxChanNum].DMADSTEND =   (uint32_t)(pXfr->pReceiver + (sz - 2U));

        pPrimaryCCD[RxChanNum].DMACDC = (ADI_DMA_INCR_2_BYTE << DMA_BITP_CTL_DST_INC) |
                              (ADI_DMA_INCR_NONE << DMA_BITP_CTL_SRC_INC) |
                              (ADI_DMA_WIDTH_2_BYTE << DMA_BITP_CTL_SRC_SIZE) |
                              ((sz/2U -1U) << DMA_BITP_CTL_N_MINUS_1) |
                              (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL);
        
        dmaFlags |= (BITM_SPI_DMA_RXEN );

      }
    }

    /*
     * SPI CNT register
     *     Non Read Mode:  Size of the entire transactions
     *     Read Mode: Size of the RX transaction 
     *
     * RD_CTL.SZ
     *     Read Mode: Size of the TX transaction
     */
    
    hDevice->pSpi->RD_CTL = 0u;
    if( hDevice->bRdCtlMode)
    {
      /* "Half Duplex Mode" */
      
      /* The number of bytes to be transmitted */
      uint32_t nBytes = hDevice->TxRemaining - 1U;
      
      /* Enable RD_CTL and set the TX count for the half-duplex mode of operation */
      hDevice->pSpi->RD_CTL &=  (uint16_t)~((uint16_t)(BITM_SPI_RD_CTL_TXBYTES <<  BITP_SPI_RD_CTL_TXBYTES));

      hDevice->pSpi->RD_CTL |=  (uint16_t)( (uint16_t)(nBytes << BITP_SPI_RD_CTL_TXBYTES) |
                                            (uint16_t)(1 << BITP_SPI_RD_CTL_CMDEN));
      
      /* RD_CTL requires continuous mode operation. */
      hDevice->pSpi->CTL |= (BITM_SPI_CTL_CON);

      /* CNT represent the number of bytes to receive */
      hDevice->pSpi->CNT =  hDevice->RxRemaining;
      
    }
    else
    {
      /* Full duplex mode of operation */
      if(hDevice->RxRemaining == 0u)
      {
        /* There is nothing to receive. Flush the RX FIFO and to ignore all incoming data */
         hDevice->pSpi->CTL |= (BITM_SPI_CTL_RFLUSH);
      } 
      else if(hDevice->TxRemaining == 0u)
      {
        /* If there is nothing to transmit then clear the TX FIFO */
        hDevice->pSpi->CTL |= (BITM_SPI_CTL_TFLUSH);
      }
      else
      {
        /* Misra compliance: All if/else chains should end with a final else clause */
      }
      
      /* Set CNT to MAX of RX/TX */

      nCount = hDevice->RxRemaining >  hDevice->TxRemaining ? hDevice->RxRemaining : hDevice->TxRemaining;
      hDevice->pSpi->CNT = (uint16_t)nCount;

    }
    
    
    if( hDevice->bDmaMode == false) 
    {
      /* Make sure that the application passed in a TX Buffer */
      if( hDevice->pTxBuffer != NULL)
      {
        /* interrupt mode: Fill in the FIFO  */
        nCount = 0u;
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
      }
      
    } else {
      
      hDevice->pSpi->DMA |= dmaFlags;
    }
    
    if((hDevice->pSpi->CTL & BITM_SPI_CTL_TIM) != BITM_SPI_CTL_TIM)
    {
      uint16_t byte ADI_UNUSED_ATTRIBUTE = hDevice->pSpi->RX;
    }
    
    
    NVIC_EnableIRQ(hDevice->pDevInfo->eIRQn);
   
    return;
}

/*!
 * @brief  Block until the SPI transaction is complete.
 *
 * @param[in]     hDevice      Device handle obtained from adi_spi_Open().
 *\n
 * @param[out]   pHWErrors     Pointer to hardware error return variable.
 *\n
 * @return         Status
 *                - #ADI_SPI_SUCCESS                    Call completed successfully.
 *                - #ADI_SPI_SEMAPHORE_FAILED           Semaphore Pend failed
 *                - #ADI_SPI_INVALID_HANDLE [D]         Invalid device handle parameter.
 *
 * @sa        adi_spi_MasterSubmitBuffer().
 * @sa        adi_spi_SlaveSubmitBuffer().
 */
ADI_SPI_RESULT adi_spi_GetBuffer(
               ADI_SPI_HANDLE const hDevice,
               uint32_t * const pHWErrors
              )
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        *pHWErrors = ADI_SPI_HW_ERROR_NONE;
        return ADI_SPI_INVALID_HANDLE;
    }
#endif

    SEM_PEND(hDevice,ADI_SPI_SEMAPHORE_FAILED);
    *pHWErrors = hDevice->HWErrors;
    return(ADI_SPI_SUCCESS);
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
 * @sa        adi_spi_MasterSubmitBuffer().
 * @sa        adi_spi_SlaveSubmitBuffer().
 */

ADI_SPI_RESULT  adi_spi_isBufferAvailable(ADI_SPI_CONST_HANDLE const hDevice,  bool* const bComplete)
{
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
#endif

    *bComplete = hDevice->bTransferComplete;
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
 *                - #ADI_SPI_IN_USE [D]    DMA transaction already under way.
 *                - #ADI_SPI_INVALID_POINTER [D]     Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]    Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                       Call completed successfully.
 *
 *\n Request a non-blocking transmit and receive of multiple data bytes
 *\n over the SPI serial channel.  Honours current blocking and DMA modes.
 *\n Buffer allocations are made by the calling code (the application).
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *\n
 *\n The application must make a call to adi_spi_GetBuffer() to retrieve the buffer
 *\n
 *\n @note:
 *
 * @sa adi_spi_MasterReadWrite().
 * @sa adi_spi_EnableDmaMode().
 * @sa adi_spi_isBufferAvailable().
 * @sa adi_spi_GetBuffer().
 */
ADI_SPI_RESULT adi_spi_SlaveSubmitBuffer (ADI_SPI_HANDLE const hDevice, const ADI_SPI_TRANSCEIVER* const pXfr)
{
    volatile uint16_t ADI_UNUSED_ATTRIBUTE byte;
    uint32_t nCount = 0u;
    
#ifdef ADI_DEBUG
    if (ADI_SPI_VALIDATE_HANDLE(hDevice))
    {
        return ADI_SPI_INVALID_HANDLE;
    }
    if ((NULL == pXfr->pTransmitter) && (NULL == pXfr->pReceiver))
    {
        return ADI_SPI_INVALID_POINTER;
    }
    
    if ((0u == pXfr->pTransmitter) && (0u == pXfr->pReceiver) )
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
    if ((pXfr->bDMA==true) && (pXfr->TransmitterBytes != 0u) &&(((uint32_t)pXfr->pTransmitter&0x1U) !=0u ) )
    {
        return ADI_SPI_INVALID_PARAM;
    }

#endif /* ADI_DEBUG */

    /* Effectively flush the FIFOs before the start of the next transaction */
    hDevice->pSpi->CTL |= (BITM_SPI_CTL_RFLUSH|BITM_SPI_CTL_TFLUSH);
    hDevice->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_RFLUSH|BITM_SPI_CTL_TFLUSH);

    /* Shut down any DMA enables that are still lingering from a prior transaction */
    hDevice->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
    
    hDevice->bTransferComplete   = false;
    hDevice->pTxBuffer           = pXfr->pTransmitter;
    hDevice->pRxBuffer           = pXfr->pReceiver;
    hDevice->TxRemaining         = pXfr->TransmitterBytes;
    hDevice->RxRemaining         = pXfr->ReceiverBytes;
    hDevice->TxIncrement         = (uint8_t)pXfr->nTxIncrement;
    hDevice->RxIncrement         = (uint8_t)pXfr->nRxIncrement;
    hDevice->pSpi->CNT           = (uint16_t)nCount;
    hDevice->bDmaMode            = pXfr->bDMA;
    hDevice->bRdCtlMode          = pXfr->bRD_CTL;
    hDevice->HWErrors            = ADI_SPI_HW_ERROR_NONE;

    
    /* Configure SPI. First step is to clear CTL bits that may have been set previously  */
    hDevice->pSpi->CTL &= (uint16_t)~(BITM_SPI_CTL_TIM | BITM_SPI_CTL_RFLUSH | BITM_SPI_CTL_TFLUSH | BITM_SPI_CTL_CON);
    if( hDevice->TxRemaining == 0u )
    {
      /* This will prevent TX underflow interrupts from occurring */
      hDevice->pSpi->CTL |= BITM_SPI_CTL_TFLUSH;
    }
    if( hDevice->RxRemaining == 0u )
    {
      /* This will prevent data from entering RX. Also prevents overflow interrupts from occurring */
      hDevice->pSpi->CTL |= BITM_SPI_CTL_RFLUSH;
      
      /* If SPI_CTL.TIM is set, the Tx FIFO status causes the interrupt. */
      if( hDevice->bDmaMode != true) {
        hDevice->pSpi->CTL |= BITM_SPI_CTL_TIM;
      }
      
    }
      
    hDevice->pSpi->CNT    = (uint16_t) hDevice->TxRemaining > hDevice->RxRemaining ? hDevice->TxRemaining : hDevice->RxRemaining;
    
    uint16_t nDMAFlags = 0u;
      
    if( hDevice->bDmaMode == true) 
    {
      uint16_t sz = pXfr->TransmitterBytes;
      if( sz )
      {
        uint16_t TxChanNum = hDevice->pDevInfo->dmaTxChannelNumber;
    
        /* Enable the interrupt for the given DMA */
        NVIC_EnableIRQ((IRQn_Type)(hDevice->pDevInfo->dmaTxIrqNumber));

        /* Disables source address decrement for TX channel */
        pADI_DMA0->SRCADDR_CLR = 1U << TxChanNum;
      
        /* Enable the channel */
        pADI_DMA0->EN_SET = 1U << TxChanNum;
          
        /* Enables SPI peripheral to generate DMA requests. */
        pADI_DMA0->RMSK_CLR = 1U << TxChanNum;

        /* Set the primary as the current DMA descriptor */
        pADI_DMA0->ALT_CLR = 1U << TxChanNum;
     
        /* fill in the DMA RAM descriptors */
        if( (sz & 1U) != 0u )
        {
          /* DMA is performed on 16-bit data. Make sure the DMA engine is properly aligned to even counts */
          /* The SPI_CNT register will hold the "real" transfer count                                     */
          sz++;
        }
        
        pPrimaryCCD[TxChanNum].DMASRCEND =   (uint32_t)(pXfr->pTransmitter + (sz - 2U));
              
        pPrimaryCCD[TxChanNum].DMADSTEND =   (uint32_t)&hDevice->pSpi->TX;

        pPrimaryCCD[TxChanNum].DMACDC = ((uint32_t)ADI_DMA_INCR_NONE << DMA_BITP_CTL_DST_INC) |
                              (ADI_DMA_INCR_2_BYTE << DMA_BITP_CTL_SRC_INC) |
                              (ADI_DMA_WIDTH_2_BYTE << DMA_BITP_CTL_SRC_SIZE) |
                              ((sz/2U -1U)<< DMA_BITP_CTL_N_MINUS_1) |
                              (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL);
      
        nDMAFlags |= (BITM_SPI_DMA_TXEN);
      }

      sz = pXfr->ReceiverBytes;
      if( sz )
      {
        
        uint16_t RxChanNum = hDevice->pDevInfo->dmaRxChannelNumber;
        NVIC_EnableIRQ((IRQn_Type)(hDevice->pDevInfo->dmaRxIrqNumber));

        /* Disables destination address decrement for RX channel */
        pADI_DMA0->DSTADDR_CLR = 1U << RxChanNum;
        
        /* Enable the channel */
        pADI_DMA0->EN_SET = 1U << RxChanNum;
        
        /* Enables SPI peripheral to generate DMA requests. */
        pADI_DMA0->RMSK_CLR = 1U << RxChanNum;

        /* Set the primary as the current DMA descriptor */
        pADI_DMA0->ALT_CLR = 1U << RxChanNum;
        
        if( (sz & 1U) != 0u )
        {
          /* DMA is performed on 16-bit data. Make sure the DMA engine is properly aligned to even counts */
          /* The SPI_CNT register will hold the "real" transfer count                                     */
          sz++;
        }
        
        pPrimaryCCD[RxChanNum].DMASRCEND =   (uint32_t)&hDevice->pSpi->RX;
              
        pPrimaryCCD[RxChanNum].DMADSTEND =   (uint32_t)(pXfr->pReceiver + (sz - 2U));

        pPrimaryCCD[RxChanNum].DMACDC = (ADI_DMA_INCR_2_BYTE << DMA_BITP_CTL_DST_INC) |
                              (ADI_DMA_INCR_NONE << DMA_BITP_CTL_SRC_INC) |
                              (ADI_DMA_WIDTH_2_BYTE << DMA_BITP_CTL_SRC_SIZE) |
                              ((sz/2U -1U) << DMA_BITP_CTL_N_MINUS_1) |
                              (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL);
        
        nDMAFlags |= (BITM_SPI_DMA_RXEN );

      }
    }

    /* Make sure XFRDONE is shut down. This IEN has no affect in slave mode */
    hDevice->pSpi->IEN &= (uint16_t)~BITM_SPI_IEN_XFRDONE;

    if( hDevice->bDmaMode == false) {
      /* Make sure we are not in continuous mode from a prior DMA transaction  */
      hDevice->pSpi->CTL &= (uint16_t)~BITM_SPI_CTL_CON;

    
      /* interrupt mode: Enable the UNDERFLOW and OVERFLOW interrupts */
      /* XFRDONE is invalid in slave mode */
      uint16_t activeInterrupts = 0u;
      /* Enable underflow on;y if sending bytes */
      if( hDevice->TxRemaining ) {
        activeInterrupts |= BITM_SPI_IEN_TXUNDR;
      }
      /* Enable overflow only if receiving bytes */
      if( hDevice->RxRemaining ) {
        activeInterrupts |= BITM_SPI_IEN_RXOVR;
      }
      hDevice->pSpi->IEN |= activeInterrupts;

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
    } else {

      /* DMA mode. Enable the controller */
      hDevice->pSpi->DMA |= (uint16_t)(BITM_SPI_DMA_EN | nDMAFlags);
    }
     
    if((hDevice->pSpi->CTL & BITM_SPI_CTL_TIM) != BITM_SPI_CTL_TIM)
    {
         byte = hDevice->pSpi->RX;    
    }
    NVIC_EnableIRQ(hDevice->pDevInfo->eIRQn);

    if (hDevice->bBlockingMode == true)
    {
	SEM_PEND(hDevice,ADI_SPI_SEMAPHORE_FAILED);
    }

    return ADI_SPI_SUCCESS;
}



/*!
 * @brief  Submit data buffers for SPI Slave-Mode transaction in "Blocking mode".This function
 *\n          returns only after the data transfer is complete
 *
 * @param[in]     hDevice     Device handle obtained from adi_spi_Open().
 * @param[in]     pXfr        Pointer to transfer data struct #ADI_SPI_TRANSCEIVER.
  *
 * @return         Status
 *                - #ADI_SPI_INVALID_HANDLE [D]        Invalid device handle parameter.
 *                - #ADI_SPI_BUFFER_NOT_SUBMITTED [D]  Failed to submit the buffer.
 *                - #ADI_SPI_INVALID_POINTER [D]       Invalid data pointer detected (NULL).
 *                - #ADI_SPI_INVALID_PARAM [D]         Invalid size parameter detected (0).
 *                - #ADI_SPI_SUCCESS                   Call completed successfully.
 *
 *\n
 *\n Request a non-blocking mode transmit and receive of multiple data bytes
 *\n over the SPI serial channel.  
 *\n Buffer allocations are made by the calling code (the application).
 *\n
 *\n The transmit buffer is sent and the receive buffer is written according
 *\n to the size and increment information contained by the \a pXft transfer
 *\n data structure parameter.
 *\n
 *\n
 * @sa adi_spi_SlaveSubmitBuffer().
 * @sa ADI_SPI_TRANSCEIVER
 */
ADI_SPI_RESULT adi_spi_SlaveReadWrite (ADI_SPI_HANDLE const hDevice, const ADI_SPI_TRANSCEIVER* const pXfr)
{
    ADI_SPI_RESULT eResult;
    hDevice->bBlockingMode = true;
    eResult = adi_spi_SlaveSubmitBuffer(hDevice,pXfr);
    hDevice->bBlockingMode = false;
    if( (eResult == ADI_SPI_SUCCESS) && (hDevice->HWErrors != 0u))
    {
      eResult = ADI_SPI_HW_ERROR_OCCURRED;
    }
    return(eResult);
}

/*
 *****************************************************************************
 * SPI Internal Static Support Functions
 *****************************************************************************/


 /*! \cond PRIVATE */


/*-----------------------------------------------------------------------------
 *
 * SPI ISR
 *
 *----------------------------------------------------------------------------*/



static void common_SPI_Int_Handler (ADI_SPI_DEV_DATA_TYPE* pDD)
{
   
  /* read status register - first thing */
  volatile  uint16_t nFifoStatus = pDD->pSpi->FIFO_STAT;
  uint16_t nErrorStatus = pDD->pSpi->STAT;

  uint16_t writableBytes;
  uint16_t readableBytes;


  
  /* Trap overflow/underflow errors and terminate the current transaction if there is an error. */
  if(  BITM_SPI_STAT_RXOVR == (BITM_SPI_STAT_RXOVR & nErrorStatus)) {
      pDD->HWErrors |= (uint32_t)ADI_SPI_HW_ERROR_RX_OVERFLOW;
  } else if( BITM_SPI_STAT_TXUNDR == (BITM_SPI_STAT_TXUNDR & nErrorStatus)) {
      pDD->HWErrors |= (uint32_t)ADI_SPI_HW_ERROR_TX_UNDERFLOW;
  }
  else
  {
    
    /* calculate number of bytes that can be written to tx fifo */
    writableBytes = ADI_SPI_FIFO_SIZE - ((BITM_SPI_FIFO_STAT_TX & nFifoStatus) >> BITP_SPI_FIFO_STAT_TX);
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
    
    /*
     * Now focus on the RX FIFO but only if we are not in RD_CTL mode OR, if we
     * are in RD_CTL mode, TX bytes are all transmitted 
     */
    
    if( (pDD->bRdCtlMode==false) || (pDD->TxRemaining==0u) )
    {
      /* empty rx fifo */
      while ((readableBytes != 0u) &&(pDD->RxRemaining != 0u))
      {
      
        *pDD->pRxBuffer = (uint8_t) pDD->pSpi->RX;
        pDD->pRxBuffer += pDD->RxIncrement;
        pDD->RxRemaining--;
        readableBytes--;
      }
    }
  }

  
  /* Terminate the transaction and notify the caller 
   *   1) Master mode: If there are no more bytes to RX or TX and XFRDONE is set
   *   2) Slave mode: If there are no more bytes to RX or TX  (XFRDONE is invalid in slave mode)
   *   3) If there was a HW error 
   */
  bool terminate = false;
  if( (pDD->RxRemaining == 0u) && (pDD->TxRemaining == 0u))
  {
    if( BITM_SPI_CTL_MASEN == (pDD->pSpi->CTL & BITM_SPI_CTL_MASEN ))
    {
      /* Master mode */
      if( BITM_SPI_STAT_XFRDONE == (pDD->pSpi->STAT & BITM_SPI_STAT_XFRDONE ))
      {
        /* Master mode XFRDONE */
        terminate = true;
      }
    } else {
      /* Slave mode - we're all done here */
        terminate = true;
    }
  }

  if( terminate || (pDD->HWErrors != (uint32_t)ADI_SPI_HW_ERROR_NONE))  
  {
      
    /* Clear possible interrupt sources: XFRDONE and underflow and overflow */  
    pDD->pSpi->IEN &= ~(BITM_SPI_IEN_XFRDONE|BITM_SPI_IEN_RXOVR|BITM_SPI_IEN_TXUNDR);  
    pDD->bTransferComplete  = true;
    NVIC_DisableIRQ(pDD->pDevInfo->eIRQn);
    
    /* If a callback is registered notify the buffer processed event to the application */
    if(NULL != pDD->pfCallback ){
        pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
    }
    else
    {
        SEM_POST(pDD);
    }
      
  }
  
  /* All interrupts are cleared by a write of 1 to the status register bits (W1C)  */
  pDD->pSpi->STAT = nErrorStatus;

#if defined(ADI_CYCLECOUNT_SPI_ISR_ENABLED) && (ADI_CYCLECOUNT_SPI_ISR_ENABLED == 1u)
  ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_SPI);    
#endif


  
}


/* Internal DMA Callback for receiving DMA faults from common DMA error handler. */
static void RxDmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg) {

    /* Recover the device handle. */
    ADI_SPI_HANDLE hDevice = (ADI_SPI_HANDLE) pCBParam;

    /* Save the DMA error. */
    switch (Event) {
        case ADI_DMA_EVENT_ERR_BUS:
            hDevice->HWErrors |= ADI_SPI_HW_ERROR_RX_CHAN_DMA_BUS_FAULT;
            break;
        case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            hDevice->HWErrors |= ADI_SPI_HW_ERROR_RX_CHAN_DMA_INVALID_DESCR;
            break;
        default:
            hDevice->HWErrors |= ADI_SPI_HW_ERROR_RX_CHAN_DMA_UNKNOWN_ERROR;
            break;
    }
}


/* Internal DMA Callback for receiving DMA faults from common DMA error handler. */
static void TxDmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg) {

    /* Recover the device handle. */
    ADI_SPI_HANDLE hDevice = (ADI_SPI_HANDLE) pArg;

    /* Save the DMA error. */
    switch (Event) {
        case ADI_DMA_EVENT_ERR_BUS:
            hDevice->HWErrors |= ADI_SPI_HW_ERROR_TX_CHAN_DMA_BUS_FAULT;
            break;
        case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            hDevice->HWErrors |= ADI_SPI_HW_ERROR_TX_CHAN_DMA_INVALID_DESCR;
            break;
        default:
            hDevice->HWErrors |= ADI_SPI_HW_ERROR_TX_CHAN_DMA_UNKNOWN_ERROR;
            break;
    }   
}


/*!
 * @brief  SPI0 Interrupt Handler.
 *
 * @return void.
 *
 * Overrides default SPI0 interrupt handler.
 */
void SPI0_Int_Handler(void) {
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
void SPI1_Int_Handler(void) {
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
void SPI2_Int_Handler(void) {
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
 * SPI DMA interrupt handlers
 */


#if defined(ADI_SPI0_MASTER_MODE) && (ADI_SPI0_MASTER_MODE==1u)
void DMA_SPI0_TX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[0].hDevice;
   pDD->TxRemaining = 0u;
   ISR_EPILOG();       
}

/* Master mode DMA ISR */
void DMA_SPI0_RX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[0].hDevice;
   pDD->RxRemaining = 0u;
   /* Disable DMA */
   pDD->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
   /* Master mode: Now allow the XFRDONE interrupt to occur. It's the SPI ISR that really ends the transaction */
   /* The slave mode is not affected by this setting */
   pDD->pSpi->IEN |= BITM_SPI_IEN_XFRDONE;
   ISR_EPILOG();       
}
#endif
#if defined(ADI_SPI0_MASTER_MODE) && (ADI_SPI0_MASTER_MODE==0u)
/* Slave mode DMA ISRs */
void DMA_SPI0_TX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[0].hDevice;
   pDD->TxRemaining = 0u;
   if( pDD->RxRemaining == 0)
   {
     /* If a callback is registered notify the buffer processed event to the application */
     if(NULL != pDD->pfCallback ){
         pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
     }
     else
     {
         SEM_POST(pDD);
     }
   }
   ISR_EPILOG();       
}
void DMA_SPI0_RX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[0].hDevice;
   pDD->RxRemaining = 0u;
   /* Disable DMA */
   pDD->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
   pDD->pSpi->IEN &= ~(BITM_SPI_IEN_XFRDONE|BITM_SPI_IEN_RXOVR|BITM_SPI_IEN_TXUNDR);  
   pDD->bTransferComplete  = true;
   NVIC_DisableIRQ(pDD->pDevInfo->eIRQn);
    
   /* If a callback is registered notify the buffer processed event to the application */
   if(NULL != pDD->pfCallback ){
       pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
   }
   else
   {
       SEM_POST(pDD);
   }
   ISR_EPILOG();       
}
#endif




#if defined(ADI_SPI1_MASTER_MODE) && (ADI_SPI1_MASTER_MODE==1u)
/* Master mode DMA ISR */
void DMA_SPI1_TX_Int_Handler(void) 
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[1].hDevice;
   pDD->TxRemaining = 0u;
   ISR_EPILOG();       
}

void DMA_SPI1_RX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[1].hDevice;
   pDD->RxRemaining = 0u;
   /* Disable DMA */
   pDD->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
   /* Master mode: Now allow the XFRDONE interrupt to occur. It's the SPI ISR that really ends the transaction */
   /* The slave mode is not affected by this setting */
   pDD->pSpi->IEN |= BITM_SPI_IEN_XFRDONE;
   ISR_EPILOG();       
}
#endif


#if defined(ADI_SPI1_MASTER_MODE) && (ADI_SPI1_MASTER_MODE==0u)
/* Slave mode DMA ISRs */
void DMA_SPI1_TX_Int_Handler(void) 
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[1].hDevice;
   pDD->TxRemaining = 0u;
   if( pDD->RxRemaining == 0)
   {
     /* If a callback is registered notify the buffer processed event to the application */
     if(NULL != pDD->pfCallback ){
         pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
     }
     else
     {
         SEM_POST(pDD);
     }
   }
   ISR_EPILOG();       
}


void DMA_SPI1_RX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[1].hDevice;
   pDD->RxRemaining = 0u;
   /* Disable DMA */
   pDD->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
   pDD->pSpi->IEN &= ~(BITM_SPI_IEN_XFRDONE|BITM_SPI_IEN_RXOVR|BITM_SPI_IEN_TXUNDR);  
   pDD->bTransferComplete  = true;
   NVIC_DisableIRQ(pDD->pDevInfo->eIRQn);
    
   /* If a callback is registered notify the buffer processed event to the application */
   if(NULL != pDD->pfCallback ){
       pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
   }
   else
   {
       SEM_POST(pDD);
   }
   ISR_EPILOG();       
}
#endif


#if defined(ADI_SPI2_MASTER_MODE) && (ADI_SPI2_MASTER_MODE==1u)
/* Master mode DMA ISR */

void DMA_SPIH_TX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[2].hDevice;
   pDD->TxRemaining = 0u;
   ISR_EPILOG();       
}

void DMA_SPIH_RX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[2].hDevice;
   pDD->RxRemaining = 0u;
   /* Disable DMA */
   pDD->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
   /* Master mode: Now allow the XFRDONE interrupt to occur. It's the SPI ISR that really ends the transaction */
   /* The slave mode is not affected by this setting */
   pDD->pSpi->IEN |= BITM_SPI_IEN_XFRDONE;
   ISR_EPILOG();       
}
#endif
#if defined(ADI_SPI2_MASTER_MODE) && (ADI_SPI2_MASTER_MODE==0u)
/* Master mode DMA ISRs */

void DMA_SPIH_TX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[2].hDevice;
   pDD->TxRemaining = 0u;
   ISR_EPILOG();       
   if( pDD->RxRemaining == 0)
   {
     /* If a callback is registered notify the buffer processed event to the application */
     if(NULL != pDD->pfCallback ){
         pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
     }
     else
     {
         SEM_POST(pDD);
     }
   }
   ISR_EPILOG();   
}

void DMA_SPIH_RX_Int_Handler(void)  
{ 
   ISR_PROLOG(); 
   ADI_SPI_HANDLE pDD = spi_device_info[2].hDevice;
   pDD->RxRemaining = 0u;
   /* Disable DMA */
   pDD->pSpi->DMA &= (uint16_t)~(BITM_SPI_DMA_EN | BITM_SPI_DMA_RXEN | BITM_SPI_DMA_TXEN);
   pDD->pSpi->IEN &= ~(BITM_SPI_IEN_XFRDONE|BITM_SPI_IEN_RXOVR|BITM_SPI_IEN_TXUNDR);  
   pDD->bTransferComplete  = true;
   NVIC_DisableIRQ(pDD->pDevInfo->eIRQn);
    
   /* If a callback is registered notify the buffer processed event to the application */
   if(NULL != pDD->pfCallback ){
       pDD->pfCallback(pDD->pCBParam, pDD->HWErrors, NULL);
   }
   else
   {
       SEM_POST(pDD);
   }
   ISR_EPILOG();       
}
#endif




/*! \endcond */


/* @} */

