/*! *****************************************************************************
 * @file:    adi_uart_v1.c
 * @brief:   This is the primary source file for the sport Driver for  ADuC-302x  family processor.
 *           Contains the implementation of the APIs for the SPORT peripheral driver.
 * @version: $Revision:
 * @date:    $Date:
 -----------------------------------------------------------------------------
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

/** @addtogroup UART_Driver UART Device Driver
 *  @{
 */


#ifndef _ADI_UART_V1_C_

/*! \cond PRIVATE */
#define _ADI_UART_V1_C_

#include <adi_uart_config.h>

#include <ssdd_common/common_def.h>
#include <drivers/uart/adi_uart.h>

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

#include "adi_uart_data_v1.c"

ADI_INT_HANDLER(UART_Int_Handler);
#ifdef ADI_DEBUG
static ADI_UART_RESULT  ValidateHandle(ADI_UART_DEVICE *pInDevice);
#endif /* ADI_DEBUG */

#if defined ( __GNUC__ )
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif
/*! \endcond */

/**********************************************************
  General UART APIs
**********************************************************/
/**
 * @brief       Initialization  function  for the UART device.
 * @details     Initialize the specified UARRT. By default, Device is configured with the following
 *              settings. This function must be called before operating any UART device.
 *
 *
 * @param [in]  nDeviceNum     UART Device instance to be opened.
 * @param [in]  eDirection     Direction of the UART operation. (i.e Rx or Tx)
 * @param [in]  pMemory        Pointer to a 32 bit aligned buffer of size ADI_UART_INT_MEMORY_SIZE required by the
 *                             driver for the operation of specified UART device in interrupt mode. Buffer Size should be
 *                             "ADI_UART_DMA_MEMORY_SIZE" bytes If the device is expected to operate in DMA mode and device
 *                             is opened in "one" direction. If the device is opened in bidirectional, the memory size of
 *                             the buffer should be  ADI_UART_DMA_BIDIR_MEMORY_SIZE.
 *
 * @param [in]  nMemSize       Size of the buffer to which "pMemory" points. DMA mode can be enabled only if *pMemory  points to the
 *                             buffer whose size is sufficient to operate in the DMA mode.  This parameter ("nMemSize") indicate whether the
 *                             user wants to operate in DMA mode or in Interrupt mode.
 *
 * @param [out] phDevice       Pointer to a location where UART device handle to be written. This handle will be used
 *                             as an identity of device and need to be passed  as one of the argument to functions
 *                             which manage the  device.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                     Successfully initialized the specified UART device.
 *    - #ADI_UART_FAILED                      Failed to initialize the specified UART device.
 *    - #ADI_UART_DEVICE_IN_USE           [D] Specified UART device  is already initialized.
 *    - #ADI_UART_INVALID_DIR             [D] Specified direction is invalid.
 *    - #ADI_UART_INVALID_DEVICE_NUM      [D] Specified device instance is invalid(valid range 0 - 2).
 *    - #ADI_UART_INVALID_NULL_POINTER    [D] Specified pointer is invalid.
 *    - #ADI_UART_INT_REGISTRATION_FAILED [D] Failed to register specified UART device with the interrupt dispatcher.
 *    - #ADI_UART_ISUFFICIENT_MEMORY      [D] Supplied memory is insufficient for the operation of specified UART device.
 *
 * @sa  adi_uart_Close()
 *
 * @note: Memory supplied will be used by the driver for managing the UART device. This memory can be reused once
 *        device is closed.
 *
 * */
ADI_UART_RESULT adi_uart_Open(
                              uint32_t        const     nDeviceNum,
                              ADI_UART_DIRECTION  const eDirection,
                              void                     *pMemory,
                              uint32_t         const    nMemSize,
                              ADI_UART_HANDLE  *const   phDevice
                           )
{
    /* Pointer to the UART Device Instance Data */
    register ADI_UART_DEVICE         *pDevice;
    uint8_t  *pDeviceMemory;
    ADI_UART_RESULT        eResult= ADI_UART_SUCCESS;
#ifdef ADI_DEBUG
    /* Check if the given device number is within the range of number UART
     * devices present in the processor
     */
    if(nDeviceNum >(ADI_UART_NUM_DEVICES-1U))
    {
        return ADI_UART_INVALID_DEVICE_NUM;
    }
    /*check the direction */
    if( (eDirection != ADI_UART_DIR_TRANSMIT) && (eDirection != ADI_UART_DIR_RECEIVE) &&( eDirection != ADI_UART_DIR_BIDIRECTION))
    {
        return ADI_UART_INVALID_DIR;
    }

    /*check the direction */
    if((pMemory == NULL) ||(phDevice == NULL) )
    {
        return ADI_UART_INVALID_NULL_POINTER;
    }

    /* check the memory size when device is expected to operate in one direction with minimum
     */
    if((nMemSize < (sizeof(ADI_UART_DEVICE)+sizeof (UART_DATA_CHANNEL))))
    {
        return(ADI_UART_ISUFFICIENT_MEMORY);
    }
     if((eDirection ==  ADI_UART_DIR_BIDIRECTION) &&(nMemSize < (sizeof(ADI_UART_DEVICE)+(sizeof (UART_DATA_CHANNEL)*2u))))
    {
        return(ADI_UART_ISUFFICIENT_MEMORY);
    }

    /* Check if the device instance is in use */
    ADI_ENTER_CRITICAL_REGION();
    if(uart_device_info[nDeviceNum].eState == ADI_UART_STATE_UNINITIALIZED)
    {
        uart_device_info[nDeviceNum].eState =   ADI_UART_STATE_INITIALIZED;
    }
    else
    {
        eResult = ADI_UART_DEVICE_IN_USE;
    }
    ADI_EXIT_CRITICAL_REGION();
    if(eResult != ADI_UART_SUCCESS)
    {
          return(ADI_UART_DEVICE_IN_USE);
    }
#endif /* ADI_DEBUG */
    
    /* Initialize the device pointer to NULL */
    *phDevice = NULL;
    memset(pMemory,0,nMemSize);
    /* Assign the pointers with the supplied memory. */
    pDevice = pMemory;
    /* Initialize the DMA channel handle to NULL */

    /* Advance the memory pointer with the size of the UART device structure */
    pDeviceMemory = ((uint8_t *)pMemory +(sizeof(ADI_UART_DEVICE)));
    /* Set the device info table */
    pDevice->pUartInfo   =   &uart_device_info[nDeviceNum];
    /* Set the base of the register address */
    pDevice->pUARTRegs   =   uart_device_info[nDeviceNum].pUartRegs;
    /* Store  direction of the operation  */
    pDevice->eDirection  =   eDirection;
    /* Initialize the TX-channel */
    if( (ADI_UART_DIR_TRANSMIT == eDirection) || (ADI_UART_DIR_BIDIRECTION == eDirection))
    {
      /* MISRA does not like casting one pointer type onto another in case there is 
       * a mistake. In order to work around that we use the void* pointer that we
       * already had 
       */
       pMemory = pDeviceMemory;
       pDevice->pChannelTx  = (UART_DATA_CHANNEL *)pMemory;
       pDeviceMemory +=  sizeof (UART_DATA_CHANNEL);
       if((eResult = uart_channel_init(pDevice,
                                       pDevice->pChannelTx,
                                       pDevice->pUartInfo->eDMATx
                                       )) != ADI_UART_SUCCESS)
       {
               pDevice->pUartInfo->eState =   ADI_UART_STATE_UNINITIALIZED;
               return eResult;

        }
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
       pDevice->pChannelTx->pDMAmemory = pDeviceMemory;
       pDeviceMemory +=  ADI_DMA_MEMORY_SIZE;
#endif
    }
    /* Initialize the RX-channel */
    if( (ADI_UART_DIR_RECEIVE == eDirection) ||  (ADI_UART_DIR_BIDIRECTION == eDirection))
    {
       /* MISRA does not like casting one pointer type onto another in case there is 
       * a mistake. In order to work around that we use the void* pointer that we
       * already had 
       */
        pMemory = pDeviceMemory;
        pDevice->pChannelRx  = (UART_DATA_CHANNEL *)pMemory;
        pDeviceMemory +=  sizeof (UART_DATA_CHANNEL);
        if((eResult = uart_channel_init(pDevice,
                                        pDevice->pChannelRx,
                                        pDevice->pUartInfo->eDMARx
                                        )) != ADI_UART_SUCCESS)
        {
               pDevice->pUartInfo->eState =   ADI_UART_STATE_UNINITIALIZED;
               return eResult;

        }
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
        pDevice->pChannelRx->pDMAmemory = pDeviceMemory;
#endif
    }
    /* Initialize the device with default values.*/
    uart_init(pDevice);
    *phDevice   =   pDevice;
    ADI_ENABLE_INT(UART_EVT_IRQn);
    ADI_INSTALL_HANDLER(UART_EVT_IRQn,UART_Int_Handler);
    ADI_ENTER_CRITICAL_REGION();
    /* Store the device handle. This is for validating the device
      in DEBUG mode*/
    uart_device_info[nDeviceNum].hDevice = pDevice;
    ADI_EXIT_CRITICAL_REGION();
#if (ADI_UART_CFG_ENABLE_DMA == 1)

    if(adi_uart_EnableDMAMode(*phDevice,true) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_FAILED);
    }
#else
#if (ADI_UART_CFG_ENABLE_DMA_TX == 1)
    if(adi_uart_EnableDMAModeForTx(*phDevice,true) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_FAILED);
    }
#endif

#if (ADI_UART_CFG_ENABLE_DMA_RX == 1)
    if(adi_uart_EnableDMAModeForRx(*phDevice,true) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_FAILED);
    }
#endif
    
#endif


    /* Return SUCCESS */
    return ADI_UART_SUCCESS;
}

/**
 * @brief       Closes the operation of  specified UART device.
 *
 * @param [in]  hDevice  UART device handle whose operation is to be closed. This handle was obtained when a UART
 *                       device is  opened successfully.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS             Successfully closed the specified UART device.
 *    - #ADI_UART_FAILED              Failed to close the specified UART device.
 *    - #ADI_UART_DMA_FAILED          Failed to close the specified UART device due to error while closing DMA
 *                                    associated with it.
 *    - #ADI_UART_INVALID_HANDLE [D]  Invalid  UART device handle.
 * @details     Closes the operation of specified UART device. Associated DMA will be closed if the device is
 *              operating in DMA mode.Device need to be opened again for any  further use.
 *
 * @sa  adi_uart_Open()
 *
 * @note: It is user's responsibility to free/reuse the memory supplied during the opening of the device.
 *
*/
ADI_UART_RESULT adi_uart_Close(
                           ADI_UART_HANDLE const hDevice
                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE     *pDevice = (ADI_UART_DEVICE *)hDevice;
    /* Return code */
    ADI_UART_RESULT        eResult = ADI_UART_SUCCESS;
    volatile uint16_t UNUSED nTemp;
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */

    /* Unregister the status interrupt handler */

    if((pDevice->eDirection == ADI_UART_DIR_TRANSMIT) || (pDevice->eDirection ==  ADI_UART_DIR_BIDIRECTION))
    {
        eResult = uart_CloseChannel(pDevice->pChannelTx);
    }
    if((pDevice->eDirection == ADI_UART_DIR_RECEIVE) ||(pDevice->eDirection ==  ADI_UART_DIR_BIDIRECTION))
    {
        eResult = uart_CloseChannel(pDevice->pChannelRx);
    }
    /* clear the STAT register by reading it */
    nTemp = pDevice->pUARTRegs->COMLSR;
    /* clear the IER register */
    pDevice->pUARTRegs->COMIEN    = 0x00U;
    /* Free up the device */
    pDevice->pUartInfo->eState =   ADI_UART_STATE_UNINITIALIZED;
#ifdef ADI_DEBUG
     ADI_ENTER_CRITICAL_REGION();
     pDevice->pUartInfo->hDevice= NULL;
     ADI_EXIT_CRITICAL_REGION();
#endif
    return eResult;
}

/**
 * @brief       Enable the specified UART device for  transmitting the data.
 *              Associated DMA with the device  will be enabled if the DMA mode is selected.
 *
 * @param [in]  hDevice   Device handle to UART device obtained when an UART device opened successfully.
 * @param [in]  bEnable   Boolean flag to indicate whether to enable/disable the UART  as below.
 * \n
 * \n                      true  : Enable the device for data transfer in Tx direction.
 * \n
 * \n                      false : Disable the device for data transfer in Tx direction.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS             Successfully enabled(when "bEnable" is true)/disabled(when "bEnable" is false)
 *                                    the specified UART and DMA.
 *    - #ADI_UART_DMA_FAILED          Failed to enabled/disabled the DMA associated with specified UART for
 *                                    transmitting the data.
 *    - #ADI_UART_INVALID_HANDLE [D]  Invalid  UART device handle.
 *
 * @sa  adi_uart_EnableDMAMode()
 * @sa  adi_uart_EnableRx()
 *
*/
ADI_UART_RESULT adi_uart_EnableTx(
                           ADI_UART_HANDLE const hDevice,
                           bool_t              bEnable
                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#if  defined (ADI_DEBUG) || (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    /* Return code */
    ADI_UART_RESULT         eResult;
#endif
    
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* Check if the data flow is already enabled/disabled */
    if((ADI_UART_STATE_DATA_FLOW_ENABLED == pDevice->pUartInfo->eState) &&( bEnable == true) )
    {
        /* If it already in the required state do nothing */
        return ADI_UART_SUCCESS;
    }
#endif /* ADI_DEBUG */
    /* Place the TX under pause state if there is no buffer to process 
       User may wish to submit the buffer at later stage.
    */ 
    if((0U == pDevice->pChannelTx->nActiveBufferCount) && (bEnable == true) )
    {
        pDevice->pChannelTx->eChannelStatus = CHANNEL_STATE_DATA_FLOW_PAUSED;
        return(ADI_UART_SUCCESS);
    }
    ADI_ENTER_CRITICAL_REGION();
    if(bEnable == true)
    {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
        if(pDevice->pChannelTx->bDMAMode == true)
        {
            if((eResult = pDevice->pChannelTx->pfEnableDMA(pDevice->pChannelTx,&pDevice->pChannelTx->oDMADescriptor,ADI_DMA_CHANNEL_UART_TX,true)) != ADI_UART_SUCCESS)
            {
                return(eResult);
            }
            pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_EDMAT);
        }
        else
#endif
        {
             ADI_UART_ENABLE_TX();
        }
         pDevice->pChannelTx->eChannelStatus = CHANNEL_STATE_DATA_FLOW_ENABLED;
         /* Enabling the IER register */
    }
    else
    {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
        if(pDevice->pChannelTx->bDMAMode == true)
        {
           if((eResult = pDevice->pChannelTx->pfEnableDMA(pDevice->pChannelTx,&pDevice->pChannelTx->oDMADescriptor,ADI_DMA_CHANNEL_UART_TX,false)) != ADI_UART_SUCCESS)
           {
                return(eResult);
            }
           pDevice->pUARTRegs->COMIEN &= ((uint16_t)(~(BITM_UART_COMIEN_EDMAT)));
        }
        else
#endif
        {
             ADI_UART_DISABLE_TX();
        }
        if(pDevice->pChannelTx->nGetbufferCount >0u)  
        {
            POST_EVENT(pDevice->pChannelTx);   
        }
        pDevice->pChannelTx->nActiveBufferCount =0u;
        pDevice->pChannelTx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
        uart_InitChannelDescs(ADI_NUM_UART_BUFFER,pDevice->pChannelTx);
        pDevice->pChannelTx->eChannelStatus = CHANNEL_STATE_DATA_FLOW_DISABLED;
    }
    ADI_EXIT_CRITICAL_REGION();
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Enable the specified UART device for receiving the data.
 *              DMA associated with the device  will be enabled if the DMA mode is selected.
 *
 * @param [in]  hDevice   Device handle to UART device obtained when an UART device opened successfully.
 * @param [in]  bEnable   Boolean flag to indicate whether to enable/disable the UART.
 * \n
 * \n                      true  : Enable the device for data transfer in Rx direction.
 * \n
 * \n                      false : Disable the device for data transfer in Rx direction.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS            Successfully enable(when bEnable is true)/disable(when bEnable is false)
 *                                   the specified UART and DMA.
 *    - #ADI_UART_DMA_FAILED         Failed to enable/disable the DMA associated with specified UART for
 *                                   receiving the data.
 *    - #ADI_UART_INVALID_HANDLE [D] Invalid  UART device handle.
 *
 * @sa   adi_uart_EnableDMAMode()
 * @sa   adi_uart_EnableTx()
 *
*/
ADI_UART_RESULT adi_uart_EnableRx(
                           ADI_UART_HANDLE const hDevice,
                           bool_t              bEnable
                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#if  defined (ADI_DEBUG) || (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    /* Return code */
    ADI_UART_RESULT         eResult=ADI_UART_SUCCESS;
#endif
    
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* Check if the data flow is already enabled/disabled */
    if( (CHANNEL_STATE_DATA_FLOW_ENABLED == pDevice->pChannelRx->eChannelStatus) && (bEnable == true))
    {
        /* If it already in the required state do nothing */
        return ADI_UART_SUCCESS;
    }

#endif /* ADI_DEBUG */
    if((0U == pDevice->pChannelRx->nActiveBufferCount) && (bEnable == true) )
    {
        pDevice->pChannelRx->eChannelStatus = CHANNEL_STATE_DATA_FLOW_PAUSED;
        return(ADI_UART_SUCCESS);
    }
    
    ADI_ENTER_CRITICAL_REGION();
    if(bEnable == true)
    {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)

        if(pDevice->pChannelRx->bDMAMode == true)
        {
            if((eResult = pDevice->pChannelRx->pfEnableDMA(pDevice->pChannelRx,&pDevice->pChannelRx->oDMADescriptor,ADI_DMA_CHANNEL_UART_RX,true)) != ADI_UART_SUCCESS)
            {
                return(eResult);
            }
            ADI_ENABLE_INT(DMA0_CH9_DONE_IRQn);
            pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_EDMAR);

        }
        else
#endif
        {
         /* Enabling the IER register */
          ADI_UART_ENABLE_RX();
        }
         pDevice->pChannelRx->eChannelStatus = CHANNEL_STATE_DATA_FLOW_ENABLED;
    }
    else
    {
        if(pDevice->pChannelRx->bDMAMode == true)
        {
           ADI_DISABLE_INT(DMA0_CH9_DONE_IRQn);
           pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_EDMAR;
        }
        else
        {
          ADI_UART_DISABLE_RX();
        }
        if(pDevice->pChannelRx->nGetbufferCount >0u)  
        {
            POST_EVENT(pDevice->pChannelRx);   
        }
        pDevice->pChannelRx->nActiveBufferCount =0u;
        pDevice->pChannelRx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
        uart_InitChannelDescs(ADI_NUM_UART_BUFFER,pDevice->pChannelRx);
        pDevice->pChannelRx->eChannelStatus = CHANNEL_STATE_DATA_FLOW_DISABLED;    }
    ADI_EXIT_CRITICAL_REGION();
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Submit the "filled" buffer for transmitting the data in NON BLOCKING MODE.This function will be returned
 *              after successfully submitting the buffer for transmitting the data.
 *
 * @param [in]  hDevice    Device handle to UART device is obtained when an UART device is opened successfully.
 * @param [in]  pBuffer    Pointer to buffer from where data need to be transmitted.
 * @param [in]  nBufSize   Size of the data to be transmitted(In bytes).
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                        Successfully  submitted the buffer for transmission.
 *    - #ADI_UART_INVALID_DATA_TRANSFER_MODE     Failed to submit the buffer since device is operating in
 *                                               "BLOCKING" mode.
 *    - #ADI_UART_SUBMIT_BUFFER_FAILED           Failed to submit the buffer
 *    - #ADI_UART_INVALID_HANDLE             [D] Invalid  UART device handle.
 *
 * @sa  adi_uart_IsTxBufferAvailable()
 * @sa  adi_uart_GetTxBuffer()
 * @sa  adi_uart_SubmitRxBuffer()
 *
 * @note  This function returns error when device is operating the "BLOCKING" mode.
*/
ADI_UART_RESULT adi_uart_SubmitTxBuffer(
                                         ADI_UART_HANDLE const hDevice,
                                         void    *const   pBuffer,
                                         uint32_t         nBufSize
                                       )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
    /* Return code */
    ADI_UART_RESULT         eResult;


#ifdef ADI_DEBUG

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if(pBuffer == NULL)
    {
        return(ADI_UART_INVALID_NULL_POINTER);
    }
    if(nBufSize == 0U )
    {
        return(ADI_UART_FAILED);
    }
    if(ADI_UART_DIR_RECEIVE == pDevice->eDirection)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif /* ADI_DEBUG */
    /* Check for the data transfer mode */
    if(pDevice->pChannelTx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
    }
    /* Check for the data transfer mode */
    pDevice->pChannelTx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING;
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT ==1)
    pDevice->pChannelTx->oDMADescriptor.pSrcData = pBuffer;
    pDevice->pChannelTx->oDMADescriptor.NumTransfers = nBufSize;
    if((eResult = pDevice->pChannelTx->pfSubmitBuffer(pDevice->pChannelTx,pBuffer,nBufSize,&pDevice->pChannelTx->oDMADescriptor)) != ADI_UART_SUCCESS)
#else
    if((eResult = pDevice->pChannelTx->pfSubmitBuffer(pDevice->pChannelTx,pBuffer,nBufSize,NULL)) != ADI_UART_SUCCESS)
#endif
    {
        return(eResult);
    }
    if((pDevice->pChannelTx->eChannelStatus == CHANNEL_STATE_DATA_FLOW_DISABLED)||(pDevice->pChannelTx->eChannelStatus == CHANNEL_STATE_DATA_FLOW_PAUSED))
    {
         eResult = adi_uart_EnableTx(hDevice,true);
    }
    return(eResult);
}

/**
 * @brief       Submit an "empty" buffer for  receiving  data in NON BLOCKING MODE.
 *
 * @param [in]  hDevice    Device handle to UART device, obtained when an UART device is opened successfully.
 * @param [in]  pBuffer    Pointer to the buffer where the data need to be written.
 * @param [in]  nBufSize   Size of the data to be received(In bytes).
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS             Successfully  submitted the buffer to the specified UART device  for
 *                                    receiving the data.
 *    - #ADI_UART_FAILED              Failed to  submit the buffer for receiving the data .
 *    - #ADI_UART_INVALID_HANDLE [D]  Invalid  UART device handle.
 *
 * @sa  adi_uart_IsRxBufferAvailable()
 * @sa  adi_uart_GetRxBuffer()
 * @sa  adi_uart_SubmitTxBuffer()
 *
*/
ADI_UART_RESULT adi_uart_SubmitRxBuffer(
                                         ADI_UART_HANDLE const hDevice,
                                         void    *const   pBuffer,
                                         uint32_t         nBufSize
                                       )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
    /* Return code */
    ADI_UART_RESULT         eResult;


#ifdef ADI_DEBUG

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if(pBuffer == NULL)
    {
        return(ADI_UART_INVALID_NULL_POINTER);
    }
    if(nBufSize  ==  0U)
    {
        return(ADI_UART_FAILED);
    }
    if(ADI_UART_DIR_TRANSMIT == pDevice->eDirection)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

#endif /* ADI_DEBUG */
    /* Check for the data transfer mode */
    if(pDevice->pChannelRx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
    }
    /* Check for the data transfer mode */
    pDevice->pChannelRx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING;

#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT ==1)
    pDevice->pChannelRx->oDMADescriptor.pDstData = pBuffer;
    pDevice->pChannelRx->oDMADescriptor.NumTransfers = nBufSize;
    if((eResult = pDevice->pChannelRx->pfSubmitBuffer(pDevice->pChannelRx,pBuffer,nBufSize,&pDevice->pChannelRx->oDMADescriptor)) != ADI_UART_SUCCESS)
#else
    if((eResult = pDevice->pChannelRx->pfSubmitBuffer(pDevice->pChannelRx,pBuffer,nBufSize,NULL)) != ADI_UART_SUCCESS)
#endif
    {
        return(eResult);
    }
    if((pDevice->pChannelRx->eChannelStatus == CHANNEL_STATE_DATA_FLOW_DISABLED)||(pDevice->pChannelRx->eChannelStatus == CHANNEL_STATE_DATA_FLOW_PAUSED))
    {
       eResult = adi_uart_EnableRx(hDevice,true);
    }
    return(eResult);
}

/**
 * @brief       Get an "empty" buffer from the device manager. This function returns the address  of
 *              a processed buffer(A buffer whose content is transmitted).
 *
 * @param [in]  hDevice    Device handle to UART device, obtained when an UART device is opened successfully.
 * @param [in]  ppBuffer   Pointer to a location where the the address of the buffer is to be written.
 *                         Contains the address of an "empty" buffer only if the direction is
 *                         either  "ADI_UART_DIR_TRANSMIT" OR "ADI_UART_DIR_BIDIRECTION"
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS               Successfully returned. ppBuffer points to the address of a valid empty buffer
 *                                        and pBuffSize is initialized to the size of the buffer.
 *    - #ADI_UART_FAILED                Failed to get the buffer. ppBuffer points to NULL.
 *    - #ADI_UART_PERIPHERAL_ERROR      Specified device encountered hardware error(s).
 *    - #ADI_UART_OPERATION_NOT_ALLOWED Call to this function is not allowed when a callback is registerd.
 *    - #ADI_UART_INVALID_HANDLE [D]    Invalid  UART device handle.
 *
 * @sa  adi_uart_IsTxBufferAvailable()
 * @sa  adi_uart_SubmitTxBuffer()
 *
 */
ADI_UART_RESULT adi_uart_GetTxBuffer(
                    ADI_UART_HANDLE const   hDevice,
                    void           **const  ppBuffer
                    )

{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
    /* Return code */
    ADI_UART_RESULT         eResult;

#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if((pDevice->pChannelTx->nActiveBufferCount == 0uL)&&(pDevice->pChannelTx->pFreeDesc->pStartAddress == NULL))
    {
        return(ADI_UART_BUFFER_NOT_SUBMITTED);
    }
    
#endif /* ADI_DEBUG */
    /* This function valid only when the device is operating in BLOCKING Mode. */
    if((pDevice->pfCallback != NULL )|| (pDevice->pChannelTx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING))
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    eResult = uart_getbuffer(pDevice,pDevice->pChannelTx,ppBuffer);

    return(eResult);
}

/**
 * @brief       Get the "filled" buffer from device manager. This function returns the address
 *              a processed buffer(A buffer which has filled with data).
 *
 * @param [in]  hDevice    Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  ppBuffer   Pointer to a location where the the address of the "filled"  buffer is to be
 *                         written. On successful return, ppBuffer points to a filled buffer. This function is
 *                         valid only when the device is opened in ADI_UART_DIR_RECEIVE OR ADI_UART_DIR_BIDRECTIONAL mode.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS               Successfully returned. ppBuffer points to the address of the buffer.
 *    - #ADI_UART_FAILED                Failed to get the buffer.ppBuffer points to NULL.
 *    - #ADI_UART_PERIPHERAL_ERROR      Specified device encountered hardware error(s).
 *    - #ADI_UART_OPERATION_NOT_ALLOWED Call to this function is not allowed when a callback is registerd.
 *    - #ADI_UART_INVALID_HANDLE [D]    Invalid  UART device handle.
 *
 * @sa  adi_uart_IsRxBufferAvailable()
 * @sa  adi_uart_SubmitRxBuffer()
 *
*/
ADI_UART_RESULT adi_uart_GetRxBuffer(
                    ADI_UART_HANDLE  const   hDevice,
                    void           **const   ppBuffer
                    )

{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
    /* Return code */
    ADI_UART_RESULT         eResult;

#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if((pDevice->pChannelRx->nActiveBufferCount == 0uL)&&(pDevice->pChannelRx->pFreeDesc->pStartAddress == NULL))
    {
        return(ADI_UART_BUFFER_NOT_SUBMITTED);
    }
    
#endif /* ADI_DEBUG */
    if((pDevice->pfCallback != NULL) ||( pDevice->pChannelRx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING))
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
    eResult= uart_getbuffer(pDevice,pDevice->pChannelRx,ppBuffer);
    return(eResult);
}

/**
 * @brief       Enables or disables the  RX status interrupt
 *
 * @details     The following line status events are enabled/disabled 
 *              - Parity Error.
 *              - Framing Error.
 *              - Break Interrupt.
 *
 * @param [in]  hDevice             Handle to the UART.
 * @param [in]  bEnable             flag which indicates whether to enable or disable.
 *                                  'true' - to enable, 'false' to disable.
 *
 * @return      Status
 *   - #ADI_UART_SUCCESS                    if successfully enabled/disabled RX status events.
 *   - #ADI_UART_INVALID_HANDLE     [D]     if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_EnableRxStatusInterrupt(
    ADI_UART_HANDLE         const hDevice,
    bool_t                          bEnable
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */

    /* If line status events to be enabled */
    if(bEnable)
    {
        pDevice->pUARTRegs->COMIEN |= (uint16_t)BITM_UART_COMIEN_ELSI;
    }
    else
    {
        /* Set the IMASK Clear register to disable the line status interrupt */
        pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_ELSI;
    }

    return ADI_UART_SUCCESS;
}
/**
 * @brief       Enables or disables modem  status interrupt
 *
 *
 * @param [in]  hDevice             Handle to the UART.
 * @param [in]  bEnable             flag which indicates whether to enable or disable.
 *                                  'true' - to enable, 'false' to disable.
 *
 * @return      Status
 *   - #ADI_UART_SUCCESS                    if successfully enabled/disabled modem status events.
 *   - #ADI_UART_INVALID_HANDLE     [D]     if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_EnableModemStatusInterrupt(
    ADI_UART_HANDLE         const hDevice,
    bool_t                          bEnable
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */

    /* If line status events to be enabled */
    if(bEnable)
    {
        pDevice->pUARTRegs->COMIEN |= (uint16_t)BITM_UART_COMIEN_EDSSI;
    }
    else
    {
        /* Set the IMASK Clear register to disable the line status interrupt */
        pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_EDSSI;
    }

    return ADI_UART_SUCCESS;
}

/**
 * @brief       Submit the buffer for transmitting  the data in BLOCKING MODE. Call to this function
 *              will not return until the entire buffer is transmitted. Returns error if this function is called when
 *              device is operating in NON-BLOCKING mode. i.e Function "adi_uart_SubmitTxBuffer()"  is called
 *              and the transfer is not yet complete.
 * @param [in]  hDevice    Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  pBuffer    Pointer to buffer from where data need to be transmitted.
 * @param [in]  nBufSize   Size of the data to be transmitted(In bytes).
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS             Successfully transmitted the specified size of data from the submitted buffer.
 *    - #ADI_UART_FAILED              Failed to  read the data .
 *    - #ADI_UART_PERIPHERAL_ERROR      Specified device encountered hardware error(s).
 *    - #ADI_UART_OPERATION_NOT_ALLOWED Call to this function is not allowed when a callback is registerd.
 *    - #ADI_UART_INVALID_HANDLE [D]  Invalid  UART device handle.
 *
 * @sa  adi_uart_Read()
 * @sa  adi_uart_SubmitTxBuffer()
 *
 * @note: This function is a blocking function which means that the function returns only after the completion of
 *        buffer transmission.
 *
*/
ADI_UART_RESULT adi_uart_Write(
                           ADI_UART_HANDLE const hDevice,
                           void         *const    pBuffer,
                           uint32_t               nBufSize
                           )
{
       /* Pointer to UART device instance */
        register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
        /* Return code */
        ADI_UART_RESULT         eResult;
#ifdef ADI_DEBUG
        /* Validate the given handle */
        if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
        {
            return eResult;
        }
#endif /* ADI_DEBUG */
        /* Check for the data transfer mode */
        if((pDevice->pChannelTx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING)&&( pDevice->pChannelTx->eChannelStatus != CHANNEL_STATE_DATA_FLOW_PAUSED ))
        {
            return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
        }
        /* Check for the data transfer mode */
        pDevice->pChannelTx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_BLOCKING;
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT ==1)
        pDevice->pChannelTx->oDMADescriptor.pSrcData = pBuffer;
        pDevice->pChannelTx->oDMADescriptor.NumTransfers = nBufSize;
        eResult=pDevice->pChannelTx->pfSubmitBuffer(pDevice->pChannelTx,pBuffer,nBufSize,&pDevice->pChannelTx->oDMADescriptor);
#else
        eResult=pDevice->pChannelTx->pfSubmitBuffer(pDevice->pChannelTx,pBuffer,nBufSize,NULL);
#endif

        if(eResult ==ADI_UART_SUCCESS)
        {
            return(uart_pendForBuffer(pDevice,pDevice->pChannelTx,pBuffer,adi_uart_EnableTx,ADI_UART_DIR_TRANSMIT));
         }
        else
        {
            return(ADI_UART_FAILED);
        }
}

/**
 * @brief       Submit the buffer for reading the data in BLOCKING MODE. Call to this function will not
 *              return until the  entire buffer is filled up. Returns error if this function is called when
 *              device is operating in NON-BLOCKING mode. i.e The function "adi_uart_SubmitRxBuffer()"  is called
 *              and the transfer is not yet complete.
 *
 * @param [in]  hDevice    Device handle to UART device obtained when an UART device opened successfully.
 * @param [in]  pBuffer    Pointer to buffer where data need to be written.
 * @param [in]  nBufSize   Size of the data to be transmitted(In bytes).
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS             Successfully read specified size of  data from the device.
 *    - #ADI_UART_FAILED              Failed to  read the data into the specified buffer.
 *    - #ADI_UART_PERIPHERAL_ERROR      Specified device encountered hardware error(s).
 *    - #ADI_UART_OPERATION_NOT_ALLOWED Call to this function is not allowed when a callback is registerd.
 *    - #ADI_UART_INVALID_HANDLE [D]  Invalid  UART device handle.
 *
 * @sa  adi_uart_Write()
 * @sa  adi_uart_SubmitTxBuffer()
 *
*/
ADI_UART_RESULT adi_uart_Read(
                           ADI_UART_HANDLE const hDevice,
                           void       *const     pBuffer,
                           uint32_t              nBufSize
                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
    /* Return code */
    ADI_UART_RESULT         eResult;
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    /* Check for the data transfer mode */
    if((pDevice->pChannelRx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING) && (pDevice->pChannelRx->eChannelStatus != CHANNEL_STATE_DATA_FLOW_PAUSED ))
    {
        return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
    }
    /* set the data transfer mode as Blocking mode*/
    pDevice->pChannelRx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_BLOCKING;
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT ==1)
    pDevice->pChannelRx->oDMADescriptor.pDstData = pBuffer;
    pDevice->pChannelRx->oDMADescriptor.NumTransfers = nBufSize;
    eResult=pDevice->pChannelRx->pfSubmitBuffer(pDevice->pChannelRx,pBuffer,nBufSize,&pDevice->pChannelRx->oDMADescriptor);

#else
    eResult=pDevice->pChannelRx->pfSubmitBuffer(pDevice->pChannelRx,pBuffer,nBufSize,NULL);

#endif
    if(eResult ==ADI_UART_SUCCESS)
    {
        return(uart_pendForBuffer(pDevice,pDevice->pChannelRx,pBuffer,adi_uart_EnableRx,ADI_UART_DIR_RECEIVE));
    }
    else
    {
            return(ADI_UART_FAILED);
    }
}

/**
 * @brief                  Peek function to know whether an empty buffer is avilable.
 *
 * @param [in]  hDevice      Device handle to UART device obtained when an UART device is opened successfully.
 * @param [out] pbAvailable  Pointer to a boolean variable. Contains "True" if there is an empty  buffer
 *                           and a call to "adi_uart_GetTxBuffer" is ensured to be successful. Contains
 *                           "false" if there is no empty buffer.
 * @return      Status
 *    - #ADI_UART_SUCCESS    Successfully retrieved  the status of availability of the buffer.
 *    - #ADI_UART_FAILED     Failed to  retrieve  the status of the buffer availability.
 *
 * @sa  adi_uart_GetTxBuffer()
 *
 */

ADI_UART_RESULT    adi_uart_IsTxBufferAvailable(
                                                 ADI_UART_HANDLE const hDevice,
                                                 bool      *const       pbAvailable
                                                )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    ADI_UART_RESULT eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if((pDevice->pfCallback != NULL) || (pDevice->pChannelRx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING))
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

#endif /* ADI_DEBUG */
    *pbAvailable = false;
    if(pDevice->pChannelTx->pFreeDesc->pStartAddress != NULL)
    {
        if (pDevice->pChannelTx->pFreeDesc->bInUse == false)
        {
            *pbAvailable =true;
        }
    }
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Peek function to know whether a filled buffer is available.
 *
 *
 * @param [in]  hDevice      Device handle to UART device.
 * @param [out] pbAvailable  Pointer to a boolean variable. Contains "True" if there is a filled buffer
 *                           and a call to "adi_uart_GetRxBuffer" is ensured to be successful. Contains
 *                           "false" if there is no filled buffer.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS    Successfully returned the status of availability of the buffer.
 *    - #ADI_UART_FAILED     Failed to  retrieve  the status of the buffer availability.
 *
 * @sa  adi_uart_GetRxBuffer()
 *
 */
ADI_UART_RESULT    adi_uart_IsRxBufferAvailable(
                                                 ADI_UART_HANDLE const hDevice,
                                                 bool      *const      pbAvailable
                                                )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    ADI_UART_RESULT eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if((pDevice->pfCallback != NULL) || (pDevice->pChannelRx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING))
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

#endif /* ADI_DEBUG */
    *pbAvailable = false;
    if(pDevice->pChannelRx->pFreeDesc->pStartAddress != NULL)
    {
        if (pDevice->pChannelRx->pFreeDesc->bInUse == false)
        {
            *pbAvailable =true;
        }
    }
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Registering  a Callback function.
 *
 * @param [in]  hDevice       Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  pfCallback    Function pointer to Callback function. Passing a NULL pointer  will unregister
 *                            the call back function.
 * @param [in]  pCBParam      Call back function parameter.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                    Successfully  registered callback function.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED [D]  This operation is not allowed when the dataflow is enabled.
 *    - #ADI_UART_INVALID_HANDLE        [D]  Invalid  UART device handle.
 *
 * @details     This function registers a call back function. Registered function will be called when
 *              the DATA transfer of the submitted buffer is over OR when the device encounters any hardware errors. This
 *              will enable the NON-BLOCKING-CALLBACK  mode and any further call to "adi_uart_GetTxBuffer" OR
 *              "adi_uart_GetRxBuffer" will result in error. Registering a callback when the data flow is
 *              enabled is not permitted and this function returns error.
 *
*/
ADI_UART_RESULT adi_uart_RegisterCallback(
                                         ADI_UART_HANDLE const hDevice,
                                         const ADI_CALLBACK   pfCallback,
                                         void           *const  pCBParam
                                       )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    ADI_UART_RESULT eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* Check if the data flow is already enabled */
    if(ADI_UART_STATE_DATA_FLOW_ENABLED == pDevice->pUartInfo->eState )
    {
        /* Not allowed to register a callback if the data flow is enabled. */
        return ADI_UART_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
    pDevice->pfCallback =   pfCallback;
    pDevice->pCBParam    =  pCBParam;
    return(ADI_UART_SUCCESS);
}
/**
 * @brief       Registering  a dedicated Callback function for Hardware event.
 *
 * @param [in]  hDevice       Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  pfCallback    Function pointer to Callback function. Passing a NULL pointer  will un-register
 *                            the call back function.
 * @param [in]  pCBParam      Call back function parameter.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                    Successfully  registered callback function.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED [D]  This operation is not allowed when the dataflow is enabled.
 *    - #ADI_UART_INVALID_HANDLE        [D]  Invalid  UART device handle.
 *
 * @details     This function registers a call back function to notify only Hardware event such as Break,Parity error,
                Framing error.Registered function will be called when device encounters any hardware errors.  
                Registering a callback when the data flow is enabled is not permitted and this function returns error.
 *
*/
ADI_UART_RESULT adi_uart_RegisterHWEventCallback(
                                         ADI_UART_HANDLE const hDevice,
                                         const ADI_CALLBACK   pfCallback,
                                         void           *const  pCBParam
                                       )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    ADI_UART_RESULT eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* Check if the data flow is already enabled */
    if(ADI_UART_STATE_DATA_FLOW_ENABLED == pDevice->pUartInfo->eState )
    {
        /* Not allowed to register a callback if the data flow is enabled. */
        return ADI_UART_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
    pDevice->pfEventCallback =   pfCallback;
    pDevice->pEventCBParam    =  pCBParam;
    return(ADI_UART_SUCCESS);
}
/**
 * @brief        Configuration of UART.
 *
 * @details      Sets the configuration parameter for the specified UART device like wordlength, whether to
 *               enable/disable the parity. This function returns an error if data flow is is already enabled.
 *
 * @param [in]  hDevice      Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  eParity      Specify the type parity check for the UART.
 * @param [in]  eStopBits    Specify the STOP-Bits for specified UART device
 * @param [in]  eWordLength  Specify the word size of the data.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                Successfully configured the device to operate in specified data format.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED  This operation is not allowed when device is operating.
 *    - #ADI_UART_INVALID_WORD_LENGTH    Invalid word length.
 *    - #ADI_UART_INVALID_STOP_BITS      Invalid stop-bit for the specified word-length.
 *    - #ADI_UART_INVALID_HANDLE [D] Invalid  UART device handle.
 *
*/
ADI_UART_RESULT adi_uart_SetConfiguration(
                           ADI_UART_HANDLE  const hDevice,
                           ADI_UART_PARITY  const eParity,
                           ADI_UART_STOPBITS const eStopBits,
                           ADI_UART_WORDLEN const eWordLength
                           )
{
     uint16_t nDataCfg=0U;
     /* Pointer to UART device instance */
     register ADI_UART_DEVICE        *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
     /* Return code */
     ADI_UART_RESULT        eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* Not allowed to change when data flow is enabled */
    if(pDevice->pUartInfo->eState == ADI_UART_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_UART_OPERATION_NOT_ALLOWED;
    }
    /* check the valid word length */
    if((eWordLength > ADI_UART_WORDLEN_8BITS) )
    {
        return ADI_UART_INVALID_WORD_LENGTH;
    }
    /* check the valid word length */
    if(((eWordLength == ADI_UART_WORDLEN_5BITS)&& (eStopBits ==ADI_UART_ONE_AND_HALF_TWO_STOPBITS)) ||
         (eStopBits > ADI_UART_ONE_AND_HALF_TWO_STOPBITS)
      )
    {
        return ADI_UART_INVALID_STOP_BITS;
    }

#endif /* ADI_DEBUG */
    /* clear all the fields(i.e Set to "0" ) */
    nDataCfg = pDevice->pUARTRegs->COMLCR & (uint16_t)(~(BITM_UART_COMLCR_WLS |BITM_UART_COMLCR_STOP |BITM_UART_COMLCR_PEN));

    /* construct the configuration word */
    nDataCfg |= (uint16_t)((uint16_t)( (uint16_t)eWordLength
                                      |(uint16_t)eStopBits)
                           |(uint16_t)eParity
                           ) ;
    /* Write to the register */
    pDevice->pUARTRegs->COMLCR = nDataCfg;
    /* Return Success */
    return(ADI_UART_SUCCESS);
}
/**
 * @brief       Enable the DMA mode (for both TX and RX)  associated with specified  UART device. 
 *
 * @param [in]  hDevice         Device handle to UART device  obtained when an UART device is opened successfully.
 * @param [in]  bEnable         Boolean flag  which indicates whether to Enable/disable the DMA mode for the device.
 * \n
 * \n                           true  : Enable the DMA mode.
 * \n
 * \n                           false : Disable the DMA mode.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                    Successfully initialized the DMA associated with the  specified UART device.
 *    - #ADI_UART_DMA_FAILED                 Failed to  initialize  the DMA associated with the specified UART device.
 *    - #ADI_UART_INVALID_HANDLE [D]         Invalid  UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED      DMA mode is not supported since device is not configured to operate in DMA mode.
 *
 * @sa  adi_uart_EnableTx()
 * @sa  adi_uart_EnableRx()
 * @sa  adi_uart_EnableDMAModeForTx()
 * @sa  adi_uart_EnableDMAModeForRx()
 *
 * @note #ADI_UART_CFG_ENABLE_DMA_SUPPORT  need to be set to 1 if UART is expected to operate in DMA mode. 
*/
ADI_UART_RESULT adi_uart_EnableDMAMode(ADI_UART_HANDLE const  hDevice,
                                         bool_t                bEnable
                                        )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE     *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT        eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* User need to use ADI_UART_CFG_ENABLE_DMA_SUPPORT to enable DMA mode of operation.*/
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 0)
    if(bEnable == true)
    {
       return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif    
    
#endif
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    if(bEnable == true)
    {
#if (ADI_UART_CFG_ENABLE_DMA == 1)
        /* Device is already in DMA mode. Initialized in adi_uart_open function. return success */ 
       if(((pDevice->pChannelTx->hDma !=NULL)&&((pDevice->eDirection == ADI_UART_DIR_TRANSMIT) ||( pDevice->eDirection == ADI_UART_DIR_BIDIRECTION ))) || 
          ((pDevice->pChannelRx->hDma!= NULL)&&((pDevice->eDirection == ADI_UART_DIR_RECEIVE) ||( pDevice->eDirection == ADI_UART_DIR_BIDIRECTION ))))
       {
          return(ADI_UART_DMA_ALREADY_INITIALIZED);
       }
#endif
        /* check Direction in which device is operating for TX */
        if((pDevice->eDirection == ADI_UART_DIR_TRANSMIT) ||( pDevice->eDirection == ADI_UART_DIR_BIDIRECTION ))
        {
            if(pDevice->pChannelTx->hDma == NULL)
            {
                pDevice->pChannelTx->eDMAChannel = ADI_DMA_CHANNEL_UART_TX;
                /* Open the TX DMA */
                if((adi_dma_Open(pDevice->pChannelTx->eDMAChannel,pDevice->pChannelTx->pDMAmemory,&pDevice->pChannelTx->hDma,(ADI_CALLBACK)uart_TxDMAHandler,pDevice)) != ADI_DMA_SUCCESS)
                {
                       return(ADI_UART_DMA_FAILED);
                }
            }
            pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_EDMAT);
            pDevice->pChannelTx->bDMAMode  = bEnable;
            pDevice->pChannelTx->pfSubmitBuffer = uart_SubmittBufferDMAmode;
            pDevice->pChannelTx->pfEnableDMA = uart_EnableDMA;
        }
        /* check Direction in which device is operating for RX */
        if((pDevice->eDirection == ADI_UART_DIR_RECEIVE) ||( pDevice->eDirection == ADI_UART_DIR_BIDIRECTION ))
        {
            if(pDevice->pChannelRx->hDma == NULL)
            {
        
                pDevice->pChannelRx->eDMAChannel = ADI_DMA_CHANNEL_UART_RX;
                /* Open the RX DMA */
                if((adi_dma_Open(pDevice->pChannelRx->eDMAChannel,pDevice->pChannelRx->pDMAmemory,&pDevice->pChannelRx->hDma,(ADI_CALLBACK)uart_RxDMAHandler,pDevice)) != ADI_DMA_SUCCESS)
                {
                    return(ADI_UART_DMA_FAILED);
                }
            }
             pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_EDMAR);
             pDevice->pChannelRx->pfSubmitBuffer = uart_SubmittBufferDMAmode;
             pDevice->pChannelRx->pfEnableDMA = uart_EnableDMA;
             pDevice->pChannelRx->bDMAMode  = bEnable;             
        }
        
    }
    else
#endif
    {
        /* Set the interrupt function table */
        if((pDevice->eDirection == ADI_UART_DIR_TRANSMIT) ||( pDevice->eDirection == ADI_UART_DIR_BIDIRECTION) )
        {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)          
  	          if(pDevice->pChannelTx->hDma != NULL)
	          {
	              adi_dma_Enable(pDevice->pChannelTx->hDma,false);
	          }
#endif
              pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_EDMAT;
              pDevice->pChannelTx->bDMAMode  = bEnable;
              pDevice->pChannelTx->pfSubmitBuffer = uart_SubmittBufferIntmode;
              pDevice->pChannelTx->pfEnableDMA = NULL;
              
        }
        /* check Direction in which device is operating for RX */
        if((pDevice->eDirection == ADI_UART_DIR_RECEIVE) || (pDevice->eDirection == ADI_UART_DIR_BIDIRECTION) )
        {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)          
          
 	         if(pDevice->pChannelRx->hDma != NULL)
	         {
	             adi_dma_Enable(pDevice->pChannelRx->hDma,false);
	         }
#endif                 
              pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_EDMAR;
              pDevice->pChannelRx->bDMAMode  = bEnable;
              pDevice->pChannelRx->pfSubmitBuffer = uart_SubmittBufferIntmode;
              pDevice->pChannelRx->pfEnableDMA = NULL;
              
        }
       
    }
    
    return(ADI_UART_SUCCESS);
 }

/**
 * @brief       Enable the DMA mode for transmitter of UART device (TX channel).
 *
 * @param [in]  hDevice         Device handle to UART device  obtained when an UART device is opened successfully.
 * @param [in]  bEnable         Boolean flag  which indicates whether to Enable/disable the DMA mode for TX channel.
 * \n
 * \n                           true  : Enable the DMA mode.
 * \n
 * \n                           false : Disable the DMA mode.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                    Successfully initialized the DMA associated with the  specified UART device.
 *    - #ADI_UART_DMA_FAILED                 Failed to  initialize  the DMA associated with the specified UART device.
 *    - #ADI_UART_INVALID_HANDLE [D]         Invalid  UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED      DMA mode is not supported since device is not configured to operate in DMA mode.
 *
 * @sa  adi_uart_EnableTx()
 * @sa  adi_uart_EnableRx()
 * @sa  adi_uart_EnableDMAModeForRx()
 *
 * @note #ADI_UART_CFG_ENABLE_DMA_SUPPORT  need to be set to 1 if UART is expected to operate in DMA mode. 
*/
ADI_UART_RESULT adi_uart_EnableDMAModeForTx(ADI_UART_HANDLE const  hDevice,
                                            bool_t                bEnable
                                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE     *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT        eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* User need to use ADI_UART_CFG_ENABLE_DMA_SUPPORT to enable DMA mode of operation.*/
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 0)
    if(bEnable == true)
    {
       return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif    
    if((pDevice->eDirection == ADI_UART_DIR_RECEIVE))
    {
       return(ADI_UART_OPERATION_NOT_ALLOWED);    
    }    
    
#endif
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    if(bEnable == true)
    {
        if(pDevice->pChannelTx->hDma == NULL)
        {
            pDevice->pChannelTx->eDMAChannel = ADI_DMA_CHANNEL_UART_TX;
            /* Open the TX DMA */
            if((adi_dma_Open(pDevice->pChannelTx->eDMAChannel,pDevice->pChannelTx->pDMAmemory,&pDevice->pChannelTx->hDma,(ADI_CALLBACK)uart_TxDMAHandler,pDevice)) != ADI_DMA_SUCCESS)
            {
            return(ADI_UART_DMA_FAILED);
            }
        }
        pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_EDMAT);
        pDevice->pChannelTx->pfSubmitBuffer = uart_SubmittBufferDMAmode;
        pDevice->pChannelTx->pfEnableDMA = uart_EnableDMA;
            
    }
    else
#endif
    {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)          
        if(pDevice->pChannelTx->hDma != NULL)
        {
            adi_dma_Enable(pDevice->pChannelTx->hDma,false);
        }
#endif        
        pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_EDMAT;
        pDevice->pChannelTx->pfSubmitBuffer = uart_SubmittBufferIntmode;
        pDevice->pChannelTx->pfEnableDMA = NULL;        
    }
    pDevice->pChannelTx->bDMAMode  = bEnable;
    return(ADI_UART_SUCCESS);
 }
/**
 * @brief       Enable the DMA mode for transmitter of UART device (RX channel).
 *
 * @param [in]  hDevice         Device handle to UART device  obtained when an UART device is opened successfully.
 * @param [in]  bEnable         Boolean flag  which indicates whether to Enable/disable the DMA mode for TX channel.
 * \n
 * \n                           true  : Enable the DMA mode.
 * \n
 * \n                           false : Disable the DMA mode.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                    Successfully initialized the DMA associated with the  specified UART device.
 *    - #ADI_UART_DMA_FAILED                 Failed to  initialize  the DMA associated with the specified UART device.
 *    - #ADI_UART_INVALID_HANDLE [D]         Invalid  UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED      DMA mode is not supported since device is not configured to operate in DMA mode.
 *
 * @sa  adi_uart_EnableTx()
 * @sa  adi_uart_EnableRx()
 * @sa  adi_uart_EnableDMAModeForTx()
 * @note #ADI_UART_CFG_ENABLE_DMA_SUPPORT  need to be set to 1 if UART is expected to operate in DMA mode. 
*/
ADI_UART_RESULT adi_uart_EnableDMAModeForRx(ADI_UART_HANDLE const  hDevice,
                                            bool_t                bEnable
                                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE     *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT        eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    /* User need to use ADI_UART_CFG_ENABLE_DMA_SUPPORT to enable DMA mode of operation.*/
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 0)
    if(bEnable == true)
    {
       return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif    
    if((pDevice->eDirection == ADI_UART_DIR_TRANSMIT))
    {
       return(ADI_UART_OPERATION_NOT_ALLOWED);    
    }    
    
#endif
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    if(bEnable == true)
    {
        if(pDevice->pChannelRx->hDma == NULL)
        {
            pDevice->pChannelRx->eDMAChannel = ADI_DMA_CHANNEL_UART_RX;
            /* Open the RX DMA */
            if((adi_dma_Open(pDevice->pChannelRx->eDMAChannel,pDevice->pChannelRx->pDMAmemory,&pDevice->pChannelRx->hDma,(ADI_CALLBACK)uart_RxDMAHandler,pDevice)) != ADI_DMA_SUCCESS)
            {
                return(ADI_UART_DMA_FAILED);
            }
        }
        pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_EDMAR);
        pDevice->pChannelRx->pfSubmitBuffer = uart_SubmittBufferDMAmode;
        pDevice->pChannelRx->pfEnableDMA = uart_EnableDMA;
    }
    else
#endif
    {
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)          
      
        if(pDevice->pChannelRx->hDma != NULL)
        {
            adi_dma_Enable(pDevice->pChannelRx->hDma,false);
        }
#endif        
        pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_EDMAR;
        pDevice->pChannelRx->pfSubmitBuffer = uart_SubmittBufferIntmode;
        pDevice->pChannelRx->pfEnableDMA = NULL;
    }
    pDevice->pChannelRx->bDMAMode  = bEnable;
    return(ADI_UART_SUCCESS);
 }
 
/**
 * @brief        To set divisor for which in turn determines the baudrate for device
 *
 * @details      Baudrate is calculated as per below equation.
 *
 * \n            Baudrate = (UARTCLK / (nDivM + nDivN/2048)*pow(2,nOSR+2)* nDivC)). 
 * \n
 * \n i.e        Baudrate = (UARTCLK*pow(2,9-nOSR)) / ((nDivM *pow(2,11)+ nDivN)* nDivC).  
 *
 * @param [in]  hDevice      Device handle to UART device obtained when an UART device opened successfully.
 * @param [in]  nDivC        Specify the "nDivC" in the above equation.
 * @param [in]  nDivM        Specify the "nDivM" in the above equation.
 * @param [in]  nDivN        Specify the "nDivN" in the above equation.
 * @param [in]  nOSR         Specify the "nOSR" " in the above equation.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS             Successfully set the baudrate for the device .
 *    - #ADI_UART_FAILED              Failed to set the specified baudrate.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED specified operation is not allowed.
 *    - #ADI_UART_INVALID_HANDLE [D]  Invalid UART device handle.
 *    - #ADI_UART_INVALID_PARAMETER [D]  One of the parameter is invalid.
 *
 *   @sa  adi_uart_GetBaudRate()
 *   @sa  adi_uart_EnableAutobaud();
 *
 *   @note It is expected that Initialization of power management
 *         service is done before calling this function.
 *
 */
ADI_UART_RESULT adi_uart_ConfigBaudRate(
                           ADI_UART_HANDLE const hDevice,
                           uint16_t         nDivC,
                           uint8_t          nDivM,
                           uint16_t         nDivN,
                           uint8_t          nOSR  
                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE        *pDevice = (ADI_UART_DEVICE *)hDevice;
    uint32_t nClock;
    uint16_t nTemp=0u;
#ifdef ADI_DEBUG
     /* Return code */
     ADI_UART_RESULT        eResult;

    /* Check if the given baud rate is valid */
    if( (nDivM < 1u) || (nDivM > 3u)|| (nDivN > 2047u ) || (nOSR > 3u))
    {
        return ADI_UART_INVALID_PARAMETER;
    }

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to change the baud rate when auto baud is in progress
     * or when data flow is enabled.
     */
    if(pDevice->pUartInfo->eState == ADI_UART_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_UART_OPERATION_NOT_ALLOWED;
    }
#endif /* ADI_DEBUG */
    /* Write back the register contents */
    pDevice->pUARTRegs->COMDIV   =  nDivC;
    /* Write back the register contents */
    nTemp = (BITM_UART_COMFBR_FBEN) | nDivN ;
    
    pDevice->pUARTRegs->COMFBR   =    nTemp | ((uint16_t)((uint16_t)nDivM << (uint16_t) BITP_UART_COMFBR_DIVM));
    
    pDevice->pUARTRegs->COMLCR2 = nOSR;
    adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK,&nClock);								 
    pDevice->nBaudRate = (nClock * (1ul<<6)) / (((nDivM * (1ul<<11)) + nDivN) * nDivC);
    
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Get Baudrate of  the specified UART device.
 *
 * @param [in]  hDevice     Device handle to UART device obtained when an UART device opened successfully.
 * @param [in]  pnBaudRate  Pointer to a location where baudrate is to be written.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                Successfully retrieved the baudrate.
 *    - #ADI_UART_FAILED                 Failed to get baudrate.
 *    - #ADI_UART_INVALID_HANDLE [D]     Invalid UART device handle.
 *
 *
*/
ADI_UART_RESULT adi_uart_GetBaudRate( ADI_UART_HANDLE const hDevice,
                                      uint32_t              *pnBaudRate
                                    )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
    if(pnBaudRate == NULL)
    {
        return (ADI_UART_INVALID_NULL_POINTER);
    }
#endif /* ADI_DEBUG */
    if(pDevice->pUartInfo->eState == ADI_UART_STATE_AUTOBAUD_IN_PROGRESS)
    {
        return ADI_UART_AUTOBAUD_IN_PROGRESS;
    }

    *pnBaudRate =  pDevice->nBaudRate;
    return(ADI_UART_SUCCESS);
}
/**
 * @brief       Enables/Disables UART Auto Baud detection.
 *
 * @details     The baud rate is detected using the hardware support (GP Timer).
*               After the baud rate is detected an event #ADI_UART_EVENT_AUTOBAUD_COMPLETE
 *              is notified to the application via call back function (if registered).  
 *              When a callback is not registered with UART driver,the API adi_uart_GetBaudRate 
 *				can be used to know if the autobaud is complete. If the API adi_uart_GetBaudRate 
 *              returns baudrate other than 0, it indicates that the autobaud complete, otherwise 
 *  			autobaud is still in progress.
 *
 * @param [in]  hDevice    Handle to UART device whose autobaud detection to be enabled/disabled.
 * @param [in]  bEnable    Boolean flag to indicate whether to enable or disable the autobaud.
 *                         - true to Enable the Autobaud
 *                         - false to Disable the Autobaud
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                       Successfully enabled/disabled Autobaud detection.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED     [D] If trying to enable/disable Autobaud when
 *                                              dataflow is enabled.
 *    - #ADI_UART_INVALID_HANDLE            [D] Invalid UART device handle.
 *
 * @sa  adi_uart_GetBaudRate()
 */
ADI_UART_RESULT adi_uart_EnableAutobaud(
   ADI_UART_HANDLE          const hDevice,
   bool_t                           bEnable
)
{
    /* Pointer to UART device instance */
     ADI_UART_DEVICE        *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
     /* Return code */
     ADI_UART_RESULT        eResult;

     /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to enable/disable autobaud when data flow is enabled.
     */
    if((pDevice->pChannelTx != NULL)&&
       (pDevice->pChannelTx->eChannelStatus != CHANNEL_STATE_DATA_FLOW_DISABLED))
    {
    	return ADI_UART_OPERATION_NOT_ALLOWED;
    }
    if((pDevice->pChannelRx != NULL)&&
       (pDevice->pChannelRx->eChannelStatus != CHANNEL_STATE_DATA_FLOW_DISABLED))
    {
    	return ADI_UART_OPERATION_NOT_ALLOWED;
    }
    

#endif /* ADI_DEBUG */

    if(bEnable)
    {
        pDevice->pUARTRegs->COMACR |=(BITM_UART_COMACR_ABE | BITM_UART_COMACR_DNIEN | BITM_UART_COMACR_TOIEN);
        pDevice->nBaudRate = 0u;
        /* Change the state to indicate autobaud in progress */
        pDevice->pUartInfo->eState = ADI_UART_STATE_AUTOBAUD_IN_PROGRESS;
    }
    else
    {
        /* Change the state back */
        pDevice->pUartInfo->eState = ADI_UART_STATE_INITIALIZED;
    }

    return ADI_UART_SUCCESS;
}
/**
 * @brief       Configure UART device for autobaud detection.
 *
 * @details     This function specifies the  character used for the
 *              autobaud detection.
 *
 * @param [in]  hDevice         Handle to the UART device to be configured for Autobaud.
 * @param [in]  cAutoBaud       Character for Autobaud.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                   Successfully configured for Autobaud detection.
 *    - #ADI_UART_INVALID_HANDLE        [D] Invalid  UART device handle.
 *    - ADI_UART_OPERATION_NOT_ALLOWED  [D] When trying to configure Autobaud when
 *                                          Autobaud is in progress or dataflow is enabled.
 *
 * @sa  adi_uart_EnableAutobaud()
 *
 * @note:  Characters are transmitted in little-endian bit order.
                         
*/

ADI_UART_RESULT adi_uart_ConfigAutobaud(
    ADI_UART_HANDLE         const hDevice,
    uint8_t                 cAutoBaud
)
{
    /* Pointer to UART device instance */
     ADI_UART_DEVICE        *pDevice = (ADI_UART_DEVICE *)hDevice;
     /* Mask value for detecting number of bits in charecter */
     uint16_t               nMask;

#ifdef ADI_DEBUG
     /* Return code */
     ADI_UART_RESULT        eResult;
     
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }

    /* Don't allow to change the configuration when auto baud is in progress
     * or when data flow is enabled.
     */
    if((pDevice->pChannelTx != NULL)&&(pDevice->pChannelTx->eChannelStatus != CHANNEL_STATE_DATA_FLOW_DISABLED))
    {
    	return ADI_UART_OPERATION_NOT_ALLOWED;
    }
    if((pDevice->pChannelRx != NULL)&&(pDevice->pChannelRx->eChannelStatus != CHANNEL_STATE_DATA_FLOW_DISABLED))
    {
    	return ADI_UART_OPERATION_NOT_ALLOWED;
    }
    if(pDevice->pUartInfo->eState == ADI_UART_STATE_AUTOBAUD_IN_PROGRESS)
    {
        return ADI_UART_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */

    /* calculate divisor bits from the character value */
    nMask = 0x1u;

    pDevice->nNumbits = 0x1u;

    /* check each bit of the character for first occurrence of 1. 
       key character is in little-endian format.
    */
    while ((cAutoBaud & nMask) == 0u)
    {
        nMask <<= 1u;
        pDevice->nNumbits += 1u;

        /* character set is a NULL character */
        if (pDevice->nNumbits == 9u)
        {
            break;
        }
    }

    return ADI_UART_SUCCESS;
}

/**
 * @brief       Forces the UART to send out a break.
 *
 * @details     Sets the UART Tx pin to a logic-low/high (depending upon the
 *              Tx polarity) asynchronously. The UART keeps transmitting break
 *              until it is disabled to send the break.
 *
 * @param [in]  hDevice             Handle to the UART whose Tx is forced to
 *                                  send a break.
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable transmitting the break.
 *                                  'true' - to enable, 'false' - to disable.
 *
 * @return      Status
 *
 * - #ADI_UART_SUCCESS              if successfully enabled or disabled sending break.
 *
 * - #ADI_UART_INVALID_HANDLE [D]   if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_ForceTxBreak(
    ADI_UART_HANDLE         const hDevice,
    bool_t                          bEnable
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE        *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT        eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */

    if(bEnable == true)
    {
        /* Set the force break bit */
        pDevice->pUARTRegs->COMLCR    |= BITM_UART_COMLCR_BRK;
    }
    else
    {
        /* Clear the force break bit */
        pDevice->pUARTRegs->COMLCR    &= (uint16_t)~(BITM_UART_COMLCR_BRK);
    }

    return ADI_UART_SUCCESS;
}


/**
 * @brief        This function helps to find the Hardware error(s) encountered by the specified device.
 *
 * @param [in]  hDevice     Device handle to UART device, obtained when an UART device opened successfully.
 * @param [in]  pHwError    Pointer to  a variable where the  combined error event(s) to be written.
 * @param [in]  pArg        Additional parameter passed to the user. For UART device it is NULL since there is
 *                          nothing to be sent to user.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS              Successfully returned hardware error(s) occurred for the specified device.
 *    - #ADI_UART_FAILED               Failed to return the hardware error for the specified  UART device.
 *    - #ADI_UART_INVALID_HANDLE [D]   Invalid  UART device handle.
 *
*/
ADI_UART_RESULT adi_uart_GetHWErrorStatus (
                   ADI_UART_HANDLE               const   hDevice,
                   uint32_t                      *pHwError,
                   void *                        *pArg
                   )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */

    *pHwError = pDevice->nHwError;
    *pArg     = NULL;
    pDevice->nHwError  = (uint32_t)ADI_UART_HW_NO_ERR;
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Enable/Disable the loopback for the specified  UART device.
 *
 * @param [in]  hDevice   Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  bEnable   Boolean flag to indicate whether Enable or Disable the loopback mode.
 * \n
 * \n                      true  : Enable the loopback.
 * \n
 * \n                      false : Disable the loopback.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS            Successfully enable/disable the loopback.
 *    - #ADI_UART_INVALID_HANDLE [D] Invalid  UART device handle.
 *
*/
ADI_UART_RESULT adi_uart_EnableLoopBack(
                           ADI_UART_HANDLE const hDevice,
                           bool_t                  bEnable
                           )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    if(true == bEnable)
    {
        pDevice->pUARTRegs->COMMCR |= (BITM_UART_COMMCR_LOOPBACK);
    }
    else
    {
        pDevice->pUARTRegs->COMMCR &= (uint16_t)~(BITM_UART_COMMCR_LOOPBACK);
    }
    return(ADI_UART_SUCCESS);
}

/**
 * @brief                   Function to know whether data transfer from the device is complete and
 *                          no more data is left in the device to transmit.
 *
 * @param [in]  hDevice      Device handle to UART device obtained when an UART device is opened successfully.
 * @param [out] pbComplete    Pointer to a boolean variable. Contains "True" if there is no data left in the device to transmit
 *                           and device can be disable without data loss. Contains "false" if device is not completed the data transmission.
 * @return      Status
 *    - #ADI_UART_SUCCESS          Successfully retrieved  the status of data transmission.
 *    - #ADI_UART_INVALID_HANDLE   Specified handle is invalid.
 *
 * @note  A adi_uart_getTxBuffer or UART callBack may indicate that a write transaction is complete when the device
 *        is operating in the DMA mode.However, there may still be a some data in the UART FIFO that is being
 *        transmitted by the harwdare. If the transmit channel needs to be closed then the application must
 *        poll the transmit channel to see if all data has indeed been transmitted before
 *        shutting down the channel.
 *
 */

ADI_UART_RESULT    adi_uart_IsTxComplete(
                                                 ADI_UART_HANDLE const hDevice,
                                                 bool_t      *const       pbComplete
                                                )
{
    /* Pointer to UART device instance */
    register ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    ADI_UART_RESULT eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    *pbComplete = false;
    if((pDevice->pUARTRegs->COMLSR & BITM_UART_COMLSR_TEMT) == BITM_UART_COMLSR_TEMT)
    {
        *pbComplete = true;
    }
    return(ADI_UART_SUCCESS);
}

/**
 * @brief       Sets the trigger level RX FIFO .
 *
 *
 * @param [in]  hDevice         Device handle to UART device obtained when an UART device is opened successfully.
 *
 * @param [in]  eTriglevel      Trigger  level to be set in terms of number  of bytes.
 * @return      Status
 *  - #ADI_UART_SUCCESS             if successfully set the trigger level.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_SetRxFifoTriggerLevel(
    ADI_UART_HANDLE         const hDevice,
    ADI_UART_TRIG_LEVEL           eTriglevel
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;
    uint16_t                comfcr;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    /* clear existing FIFO trigger level */
    comfcr  = pDevice->pUARTRegs->COMFCR;
    comfcr &= (uint16_t)~BITM_UART_COMFCR_RFTRIG;
    comfcr |= (uint16_t)eTriglevel;
    pDevice->pUARTRegs->COMFCR = comfcr;

    return ADI_UART_SUCCESS;
}
/**
 * @brief       Enables internal  FIFO  as to work in 16550 mode. This helps to minimize system overhead 
 *              and maximize system efficiency
 *
 *
 * @param [in]  hDevice       Device handle to UART device obtained when an UART device is opened successfully.
 *
 * @param [in]  bEnable       Boolean flag to indicate whether to enable or disable FIFO.
 *\n                          true   : To Enables internal FIFO for TX and RX.
 *\n                          false :  To Disable  internal FIFO for TX and RX.
 
 * @return      Status
 *  - #ADI_UART_SUCCESS             if successfully enabled FIFO for UART device.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_EnableFifo(
    ADI_UART_HANDLE         const hDevice,
    bool_t                  bEnable
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    
    if(bEnable == true)
    {
       /* Enable TX/RX FIFO */
       pDevice->pUARTRegs->COMFCR    |=  BITM_UART_COMFCR_FIFOEN;
    }
    else
    {
       /* Disable TX/RX FIFO */    
       pDevice->pUARTRegs->COMFCR    &= (uint16_t)~(BITM_UART_COMFCR_FIFOEN);
    }
       
    return ADI_UART_SUCCESS;
}

/**
 * @brief      To clear the TX FIFO. 
 *
 *
 * @param [in]  hDevice         Device handle to UART device obtained when an UART device is opened successfully.
 *
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully cleared TX Fifo.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_ClearTxFifo(
    ADI_UART_HANDLE         const hDevice
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    pDevice->pUARTRegs->COMFCR    |=  BITM_UART_COMFCR_TFCLR;
      
    return ADI_UART_SUCCESS;
}
/**
 * @brief      To clear the RX FIFO. 
 *
 *
 * @param [in]  hDevice           Device handle to UART device obtained when an UART device is opened successfully.
 *
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully cleared RX Fifo.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_ClearRxFifo(
    ADI_UART_HANDLE         const hDevice
)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    pDevice->pUARTRegs->COMFCR    |=  BITM_UART_COMFCR_RFCLR;
      
    return ADI_UART_SUCCESS;
}
/**
 * @brief      To Set the polarity of SOUT signal.
 *
 *
 * @param [in]  hDevice         Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  bEnable       Boolean flag to indicate the polarity of SOUT signal.
 *\n                          false   :  To set high  active polarity for SOUT signal.
 *\n                          true    :  To set low  active polarity for SOUT signal.
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully set polarity of SOUT.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_SetSoutPolarity(ADI_UART_HANDLE  const hDevice, bool_t bEnable)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    if(bEnable == true)
    {
        pDevice->pUARTRegs->COMRSC    |=  BITM_UART_COMRSC_OENP;
    }
    else
    {
        pDevice->pUARTRegs->COMRSC    &=  (uint16_t)~(BITM_UART_COMRSC_OENP);
    } 
    return ADI_UART_SUCCESS;
}

/**
 * @brief      To control the  de-assertion of SOUT signal
 *
 * @param [in]  hDevice         Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  bEnable       Boolean flag to indicate the polarity of SOUT signal.
 *\n                          false   :  SOUT_EN de-assert same time as full stop bit(s).
 *\n                          true   :   SOUT_EN de-assert half-bit earlier than full stop bit(s)
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully set de-assertion for the device.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_SetDeassertHalfBit(ADI_UART_HANDLE  const hDevice, bool_t bEnable)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    if(bEnable == true)
    {
        pDevice->pUARTRegs->COMRSC    |=  BITM_UART_COMRSC_OENSP;
    }
    else
    {
        pDevice->pUARTRegs->COMRSC    &=  (uint16_t)~(BITM_UART_COMRSC_OENSP);
    } 
    return ADI_UART_SUCCESS;
}

/**
 * @brief      To hold the transmit when data being received.
 *
 *
 * @param [in]  hDevice       Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  bEnable       Boolean flag to indicate whether to hold the TX or transmit whenever needed.
 *\n                          false   :  transmit whenever needed.
 *\n                          true   :   Hold off TX when receiving
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully set polarity of SOUT.
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_HoldTxDuringRx(ADI_UART_HANDLE  const hDevice, bool_t bEnable)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    if(bEnable == true)
    {
        pDevice->pUARTRegs->COMRSC    |=  BITM_UART_COMRSC_DISTX;
    }
    else
    {
        pDevice->pUARTRegs->COMRSC    &=  (uint16_t)~(BITM_UART_COMRSC_DISTX);
    } 
    return ADI_UART_SUCCESS;
}

/**
 * @brief      To disable  the receiving when TX is active.
 *
 *
 * @param [in]  hDevice         Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  bEnable       Boolean flag to indicate whether to disable  the RX or to keep RX activity.
 *\n                          false   : keep RX active when transmitting
 *\n                          true   : Disable RX when transmitting
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully set RX activities while transmitting. 
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_DisableRxDuringTx(ADI_UART_HANDLE  const hDevice, bool_t bEnable)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    if(bEnable == true)
    {
        pDevice->pUARTRegs->COMRSC    |=  BITM_UART_COMRSC_DISRX;
    }
    else
    {
        pDevice->pUARTRegs->COMRSC    &=  (uint16_t)~(BITM_UART_COMRSC_DISRX);
    } 
    return ADI_UART_SUCCESS;
}

/**
 * @brief      To invert the Receiver line.
 *
 *
 * @param [in]  hDevice         Device handle to UART device obtained when an UART device is opened successfully.
 * @param [in]  bEnable       Boolean flag to indicate whether to invert the receiver line  or not to invert the receiver line.
 *\n                          false   : Not to invert invert receiver line 
 *\n                          true   :  To invert receiver line 
 * 
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully  enable/disable the RX line inversion. 
 *  - #ADI_UART_INVALID_HANDLE  [D] if the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_InvertRxLine(ADI_UART_HANDLE  const hDevice, bool_t bEnable)
{
    /* Pointer to UART device instance */
    ADI_UART_DEVICE         *pDevice = (ADI_UART_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_UART_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_UART_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    if(bEnable == true)
    {
        pDevice->pUARTRegs->COMCTL    |=  BITM_UART_COMCTL_RXINV;
    }
    else
    {
        pDevice->pUARTRegs->COMCTL    &=  (uint16_t)~(BITM_UART_COMCTL_RXINV);
    } 
    return ADI_UART_SUCCESS;
}

/*! \cond PRIVATE */
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)

/*
 * @brief       Submit the buffer for Data transfer in  DMA Mode.
 *
 * @param [in]  hDevice    handle  UART device to which data buffer need to be submitted.
 *                         This handle was returned by the driver  when an UART device is opened successfully.
 * @param [in]  pBuffer    Pointer to buffer from  where DMA reads or writes.
 * @param [in]  nSize      Size of the data to be transferred(In bytes).
 * @param [in]  eDirection Direction of data transfer.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS  Successfully  submitted the buffer to DMA associated with UART.
 *    - #ADI_UART_SUBMIT_BUFFER_FAILED   Failed to  submit the buffer to DMA associated with UART.
 *
*/
static ADI_UART_RESULT uart_SubmittBufferDMAmode(UART_DATA_CHANNEL *pChannel, void      *const pBuffer,uint32_t nSize,ADI_DMA_TRANSFER *pDMADesc)
{
       /* Pointer to the descriptor that caused the interrupt */
        ADI_UART_BUFF_INFO  *pDesc = pChannel->pCurrDesc;

        if(pDesc->pStartAddress == NULL)
        {
            /* Store the address */
            pDesc->pStartAddress    =   pBuffer;
            /* Set the flag to indicate that buffer is in use */
            pDesc->bInUse           =   true;
            pDesc->nCount = nSize;
            /* Increment the active buffer count */
            pChannel->nActiveBufferCount+= 1U;
            if(adi_dma_SubmitTransfer(pChannel->hDma,&pChannel->oDMADescriptor) != ADI_DMA_SUCCESS)
            {
                 return(ADI_UART_SUBMIT_BUFFER_FAILED);
            }
            /*  Submit the buffer to DMA service based on the direction. */
            pChannel->pCurrDesc = pChannel->pCurrDesc->pNext;
            return(ADI_UART_SUCCESS);
        }
        else
        {
            return (ADI_UART_SUBMIT_BUFFER_FAILED);
        }
}
#endif
/*
 * @brief       Submits the buffers for data transfer in interrupt mode.
 *
 * @param [in]  hDevice     UART device handle whose operation is to be closed. This handle was returned by the driver
 *                          when an UART device is opened successfully.
 * @param [in]  pBuffer    Pointer to buffer from  where DMA reads or writes.
 * @param [in]  nSize      Size of the data to be transferred(In bytes).
 * @param [in]  eDirection Direction of data transfer.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                Successfully  submitted the buffer for data transfer.
 *    - #ADI_UART_SUBMIT_BUFFER_FAILED   Failed to submit the buffer for data transfer.
 *
*/
static ADI_UART_RESULT uart_SubmittBufferIntmode(UART_DATA_CHANNEL *pChannel, void *const pBuffer,uint32_t nSize,ADI_DMA_TRANSFER *pDMADesc)
{
        ADI_UART_BUFF_INFO    *pCurDesc = pChannel->pCurrDesc;
        /*Active  Buffer counter */
       if(pCurDesc->pStartAddress != NULL)
       {
           return(ADI_UART_SUBMIT_BUFFER_FAILED);
       }
       else
       {
           ADI_ENTER_CRITICAL_REGION();
           /* set the Descriptor parameters for interrupt mode */
           pCurDesc->pStartAddress =   pBuffer;
           /* Always writes 32 bit word */
           pCurDesc->nCount        =   nSize;
           pCurDesc->nIndex        =   0U;
           pCurDesc->bInUse        =   true;
           pChannel->nActiveBufferCount += 1U;
           pChannel->pCurrDesc = pChannel->pCurrDesc->pNext;
           ADI_EXIT_CRITICAL_REGION();
           return(ADI_UART_SUCCESS);
       }

}
/*
 * @brief      uart interrupt handler.
 *
 * @param [in]  IID        Interrupt ID.
 * @param [in]  pCBParam   Callback parameter from interrupt manager.
 *
*/

ADI_INT_HANDLER(UART_Int_Handler)
{
    ISR_PROLOG();
    register  ADI_UART_DEVICE       *pDevice =    uart_device_info[0].hDevice;
    uint16_t  nStatus;
    uint16_t  acr;
    uint16_t  iir = pDevice->pUARTRegs->COMIIR;
    uint32_t nEvent = 0u;
    switch( iir & BITM_UART_COMIIR_STA )
    {
        case ENUMM_UART_IIR_TXBUF_EMPTY:
          uart_TxDataHandler(pDevice);
          break;
        /* rx buffer FIFO interrupt */
        case ENUMM_UART_IIR_RXFIFO_TIMEOUT:
        /* rx buffer full interrupt */
        case ENUMM_UART_IIR_RXBUF_FULL:
          
          uart_RxDataHandler(pDevice);
          break;

        /* modem status interrupt */
        case ENUMM_UART_IIR_LINE_STAT:
            nStatus =   pDevice->pUARTRegs->COMLSR;
            if((BITM_UART_COMLSR_BI & nStatus) == BITM_UART_COMLSR_BI)
            {
                ADI_UART_RBR_GET();
                pDevice->pChannelRx->pProcDesc->nIndex=0u; 
                nEvent  =(uint32_t) ADI_UART_EVENT_BREAK_INTERRUPT;
            }
            /* Ignore the framing error if the break is asserted. */   
            if(((BITM_UART_COMLSR_FE & nStatus) == BITM_UART_COMLSR_FE)&&(nEvent == 0u))
            {
                nEvent |=  (uint32_t)ADI_UART_HW_ERR_FRAMING;
            }
            if((BITM_UART_COMLSR_PE & nStatus) == BITM_UART_COMLSR_PE)
            {
                nEvent  |=  (uint32_t)ADI_UART_HW_ERR_PARITY;
            }
            if((BITM_UART_COMLSR_OE & nStatus) == BITM_UART_COMLSR_OE)
            {
                nEvent |= (uint32_t)ADI_UART_HW_ERR_OVERRUN;
            }

            if( nEvent != (uint32_t)ADI_UART_HW_NO_ERR)
            {
                if(pDevice->pfEventCallback != NULL)
                {
                    pDevice->pfEventCallback(pDevice->pEventCBParam,nEvent,NULL);
                    /* Reset the state  */
                    pDevice->nHwError =(uint32_t) ADI_UART_HW_NO_ERR;
                }
                else if(pDevice->pfCallback != NULL)
                {
                    pDevice->pfCallback(pDevice->pCBParam, nEvent, NULL);
                    pDevice->nHwError =(uint32_t) ADI_UART_HW_NO_ERR;                    
                }
                else
                {
                    if(nEvent != (uint32_t)ADI_UART_EVENT_BREAK_INTERRUPT)
                    {
                        pDevice->nHwError = nEvent;
                        if(pDevice->pChannelRx->nActiveBufferCount > 0U)
                        {
                            POST_EVENT(pDevice->pChannelRx);
                        }
                        if(pDevice->pChannelTx->nActiveBufferCount > 0U )
                        {
                            POST_EVENT(pDevice->pChannelTx);
                        }
                    }
                }
            }
            break;
         case ENUMM_UART_IIR_MODEM_STAT:
             nStatus =   (pDevice->pUARTRegs->COMASRL & AUTO_BAUD_INTERRUPT_STATUS);
             acr = pDevice->pUARTRegs->COMACR;
#if (ADI_UART_CFG_ENABLE_AUTOBAUD ==1)             
            if( (nStatus != 0u) && ((acr & BITM_UART_COMACR_ABE) != 0U))
            {
                uint32_t nDivN = 0u, nDivC = 1u;
                uint8_t nOSR = 0u;
                if((nStatus & BITM_UART_COMASRL_DONE) == BITM_UART_COMASRL_DONE)
                {
                    uint32_t nClock; 
                    uint32_t nCount;
                    adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK,&nClock);
                    nCount = (uint32_t)(pDevice->pUARTRegs->COMASRH) << AUTO_BAUD_HIGHER_BIT_POSITION;
                    nCount += (uint32_t)(pDevice->pUARTRegs->COMASRL) >> AUTO_BAUD_LOWER_BIT_POSITION;
                    pDevice->nBaudRate  =  (nClock*pDevice->nNumbits)/nCount;                
                    /*if CNT < 8×CountedBits, OSR = 0, DIV=1, DIVN = (512×CNT+256)÷ CountedBits -2048*/
                    if(nCount<(pDevice->nNumbits<<3))
                    {
                         nDivN = (((nCount<<9))/pDevice->nNumbits)-2048u;
                         nOSR = 0u; 
                    }
                    /*if CNT < 16×CountedBits, OSR = 1, DIV=1, DIVN = (256×CNT+128)÷ CountedBits -2048 */
                    else if(nCount<(pDevice->nNumbits<<4))
                    {
                         nDivN = (((nCount<<8))/pDevice->nNumbits)-2048u;
                         nOSR = 1u; 
                    }                
                    /*if CNT < 32×CountedBits, OSR = 2, DIV=1, DIVN = (128×CNT+64)÷ CountedBits -2048 */
                    else if(nCount<(pDevice->nNumbits<<5))
                    {
                         nDivN = (((nCount<<7))/pDevice->nNumbits)-2048u;
                         nOSR = 2u; 
                    }   
                    /*OSR = 3,DIV= pow(2,log2(cnt/(32*Numbits)) */                   
                    else
                    {
                         uint8_t nTemp = (uint8_t)(nCount/(32u*pDevice->nNumbits));
                         nDivC = 0u;
                         for (;;)
                         {
                             nTemp = nTemp >> 1;
                             if (nTemp == 0u)
                             {
                                 break;
                             }
                             nDivC++;
                         }
                         nDivC =(uint32_t)1u<<nDivC;
                         nDivN = ((((nCount<<6)+32u))/(pDevice->nNumbits*nDivC))-2048u;
                         nOSR = 3u; 
                    }
                }
                adi_uart_ConfigBaudRate(pDevice,(uint16_t)nDivC,1u,(uint16_t)nDivN,nOSR);
                pDevice->pUartInfo->eState = ADI_UART_STATE_DATA_FLOW_DISABLED;
                if(pDevice->pfCallback != NULL)
                {
                    pDevice->pfCallback(pDevice->pCBParam, (uint32_t)nStatus << AUTO_BAUD_EVENT_BIT_POSITION, NULL);                                
                }
                /* Dummy read to flush the RX register */
                pDevice->pUARTRegs->COMRX;
            }
#endif
             pDevice->pUARTRegs->COMACR &=(uint16_t)~( BITM_UART_COMACR_ABE   | 
                                                       BITM_UART_COMACR_DNIEN |
                                                       BITM_UART_COMACR_TOIEN );
            
             break;
        default:
          break;
    }
    ISR_EPILOG();
    
    return;
}
/*
 * @brief                  uart interrupt handler for receiving the data in interrupt mode.
 *
 * @param [in]  IID        Interrupt ID.
 * @param [in]  pCBParam   Callback parameter from interrupt manager.
 *
*/
static void uart_RxDataHandler(ADI_UART_DEVICE       *pDevice)
{
    volatile uint8_t *pNextData;
    if(pDevice->pChannelRx->pProcDesc->pStartAddress != NULL)
    {
          /* UART-DLL register is mapped to same address as the UART-THR and UART-RBR */
          pNextData = (uint8_t *)pDevice->pChannelRx->pProcDesc->pStartAddress;
           pNextData[pDevice->pChannelRx->pProcDesc->nIndex] =(uint8_t) ADI_UART_RBR_GET();
           pDevice->pChannelRx->pProcDesc->nIndex++;
        if(pDevice->pChannelRx->pProcDesc->nIndex >= pDevice->pChannelRx->pProcDesc->nCount)
        {
                  uart_mange_buffer(pDevice,pDevice->pChannelRx,ADI_UART_EVENT_RX_BUFFER_PROCESSED,pDevice->pChannelRx->pProcDesc->pStartAddress);
                  if( pDevice->pChannelRx->eChannelStatus == CHANNEL_STATE_DATA_FLOW_PAUSED)
                  {
                       ADI_UART_DISABLE_RX();
                  }
        }
    }
    else
    {
           /* Read the data in case we don't any buffer to process */
           ADI_UART_RBR_GET();
    }
    return;
}

/*
 * @brief      uart interrupt handler transmitting the data in interrupt mode.
 *
 * @param [in]  IID        Interrupt ID.
 * @param [in]  pCBParam   Callback parameter from interrupt manager.
 *
*/
static void uart_TxDataHandler(ADI_UART_DEVICE       *pDevice)
{
    volatile uint8_t *pNextData;
    if(pDevice->pChannelTx->pProcDesc->pStartAddress != NULL)
    {
            /* UART-DLL register is mapped to same address as the UART-THR and UART-RBR */
            pNextData = (uint8_t *)pDevice->pChannelTx->pProcDesc->pStartAddress;
            ADI_UART_THR_SET(pNextData[pDevice->pChannelTx->pProcDesc->nIndex]);
            pDevice->pChannelTx->pProcDesc->nIndex++;
            if(pDevice->pChannelTx->pProcDesc->nIndex >= pDevice->pChannelTx->pProcDesc->nCount)
            {
                uart_mange_buffer(pDevice,pDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED,pDevice->pChannelTx->pProcDesc->pStartAddress);
                  if( pDevice->pChannelTx->eChannelStatus == CHANNEL_STATE_DATA_FLOW_PAUSED)
                  {
                      ADI_UART_DISABLE_TX();
                  }

            }
    }
    return;
}

#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
/*
 * @brief       Callback function for DMA manager. This will be called when DMA associated with
 *              "transmit" completes.
 *
 * @param [in]  pCBParam    Callback parameter from DMA manager.
 * @param [in]  Event       Event ID.
 * @param [in]  pArg        Callback argument.
 *
*/
static void uart_TxDMAHandler(void *pCBParam,uint32_t Event,void *pArg)
{
    /* Pointer to the UART device instance data */
    register  ADI_UART_DEVICE       *pDevice =    pCBParam;
#ifdef ADI_DEBUG
    /* This should not happen but a safety check */
    if(pDevice == NULL)
    {
        return;
    }
#endif /* ADI_DEBUG */
    switch(Event)
    {
       /* The given buffer processed */
        case (uint32_t)ADI_DMA_EVENT_BUFFER_PROCESSED:
            uart_mange_buffer(pDevice,pDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED,pArg);
            break;
        case  (uint32_t)ADI_DMA_EVENT_ERR_BUS:
        case  (uint32_t)ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            if(pDevice->pfEventCallback != NULL)
            {
                 pDevice->pfEventCallback(pDevice->pCBParam,(uint32_t)ADI_UART_EVENT_TX_DMA_ERROR,NULL);
            }
            else
            {
                 pDevice->nHwError |= (uint32_t) Event;
                 POST_EVENT(pDevice->pChannelTx);
            }
            break;
        default:
            /* We don't care about other events */
            break;
    }
    return;
}
/*
 * @brief       Callback function for DMA manager. This will be called when DMA associated with
 *              "receive" completes.
 *
 * @param [in]  pCBParam    Callback parameter from DMA manager.
 * @param [in]  Event       Event ID.
 * @param [in]  pArg        Callback argument.
 *
*/
static void uart_RxDMAHandler(void *pCBParam,uint32_t Event,void *pArg)
{
    /* Pointer to the UART device instance data */
    register  ADI_UART_DEVICE       *pDevice =    pCBParam;
#ifdef ADI_DEBUG
    /* This should not happen but a safety check */
    if(pDevice == NULL)
    {
        return;
    }
#endif /* ADI_DEBUG */
    switch(Event)
    {
       /* The given buffer processed */
        case (uint32_t)ADI_DMA_EVENT_BUFFER_PROCESSED:
            uart_mange_buffer(pDevice,pDevice->pChannelRx,ADI_UART_EVENT_RX_BUFFER_PROCESSED,pArg);
            break;
        case  (uint32_t)ADI_DMA_EVENT_ERR_BUS:
        case  (uint32_t)ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            if(pDevice->pfEventCallback != NULL)
            {
                 pDevice->pfEventCallback(pDevice->pCBParam,(uint32_t)ADI_UART_EVENT_RX_DMA_ERROR,NULL);
            }
            else
            {
                 pDevice->nHwError |= (uint32_t) Event;

                 POST_EVENT(pDevice->pChannelRx);
            }
            break;
        default:
            /* We don't care about other events */
            break;
    }
    return;
}
#endif
/*
 * @brief       function for managing the processed buffer when device is operating in NON-BLOCKING mode.
 *
 * @param [in]  pDevice    Callback parameter from DMA manager.
 * @param [in]  pChannel   Channel handler for the Tx or Rx.
 * @param [in]  eEvent     Indicate the event ID to be passed to registered callback function.
 * @param [in]  pArg       Address of the call back function.
 *
*/

static void uart_mange_buffer(ADI_UART_DEVICE *pDevice,UART_DATA_CHANNEL *pChannel,ADI_UART_EVENT eEvent,void *pArg)
{
    /* Pointer to the buffer address */
    void *pProcessedBuff;
    uint32_t i;
    ADI_UART_BUFF_INFO    *pProcDesc = pChannel->pProcDesc;
    pProcessedBuff    =   (void *)pArg;

    ADI_ENTER_CRITICAL_REGION();
    for(i=0u;i<ADI_NUM_UART_BUFFER;i++)
    {
        if(pChannel->pProcDesc->pStartAddress == pProcessedBuff)
        {
            pChannel->pProcDesc->bInUse = false;
            /*Active  Buffer counter */
            pChannel->nActiveBufferCount -= 1U;
            /* Point to next Processing Buffer Node*/
            pChannel->pProcDesc = pChannel->pProcDesc->pNext;
            if(pDevice->pfCallback == NULL)
            {
                 POST_EVENT(pChannel);
                {
                /*  To indicate the application that error has happened */
                }
            }
            break;
        }
    }
   ADI_EXIT_CRITICAL_REGION ();
    if(pDevice->pfCallback != NULL)
    {
        pDevice->pfCallback(pDevice->pCBParam,(uint32_t)eEvent,pProcessedBuff);
        pProcDesc->pStartAddress = NULL;
    }

    if(pChannel->nActiveBufferCount == 0U)
    {
        if(pChannel->eDataTranferMode ==ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING)
        {
            pChannel->eChannelStatus = CHANNEL_STATE_DATA_FLOW_PAUSED;

        }
        else
        {
            pChannel->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
        }

    }
}
/*
 * @brief       Create a circular linked list for buffer management.
 *
 * @param [in]  pChannel   Pointer to UART_DATA_CHANNEL for channel specific initialization.
 * @param [in]  NumDesc    Number of descriptorS.
 *
 */
static void uart_InitChannelDescs(uint32_t NumDesc, UART_DATA_CHANNEL *pChannel)
{
    uint32_t                i;
    ADI_UART_BUFF_INFO     *pDesc;
    pChannel->pProcDesc =  &pChannel->oBufferInfo[0];
    pChannel->pFreeDesc = pChannel->pProcDesc;
    pChannel->pCurrDesc = pChannel->pProcDesc;
    pDesc = pChannel->pProcDesc;
    /* Initialize the all descriptors. Make it circular. */
    for(i = 0U; i < NumDesc; i++)
    {
        pDesc[i].pStartAddress      =   NULL;
        pDesc[i].nCount             =   0u;
        pDesc[i].nIndex             =   0u;
        pDesc[i].pNext              =   &pDesc[i+1U];
        pDesc[i].bInUse             =    false;  
    }
    /* create a circular list */
    pDesc[i-1U].pNext                =   &pDesc[0];
    return;
}

#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)

/*
 * @brief       Enable the specified UART device for  transmitting the data.
 *              Associated DMA with the device will be enabled if the DMA mode is selected.
 *
 * @param [in]  hDevice   UART device handle whose operation is to be enabled/disabled. This handle is returned by the driver
 *                        when an UART device is opened successfully.
 * @param [in]  bEnable   Boolean flag to indicate whether to enable(true)/disable(false) the UART.
 *
 * @param [in]  hChannel  Pointer to "UART_DATA_CHANNEL" for  which data flow need to be enabled/disabled.

 * @return      Status
 *    - #ADI_UART_SUCCESS     Successfully enable/disable the specified UART and DMA.
 *    - #ADI_UART_DMA_FAILED  Failed to enable/disable the DMA associated with  UART.
 *
 * @sa  adi_UART_EnableDMAMode()
 *
*/
static ADI_UART_RESULT uart_EnableDMA(UART_DATA_CHANNEL *pChannel,ADI_DMA_TRANSFER *pDMADesc,ADI_DMA_CHANNEL_ID eDMAChannel,bool_t bEnable)
{
     if((pChannel->pProcDesc->pStartAddress == NULL) && (bEnable == true))
     {
          return(ADI_UART_SUBMIT_BUFFER_FAILED);
     }
     if(adi_dma_Enable(pChannel->hDma, bEnable)!= ADI_DMA_SUCCESS)
     {
          return(ADI_UART_DMA_FAILED);
     }
     return ADI_UART_SUCCESS;
}
#endif
/*
 * @brief       Initialize the specified UART device for default value.
 *
 * @param [in]  pDevice     Pointer to "ADI_UART_DEVICE" which is defined to manage the UART device.
 *
*/

static void uart_init(ADI_UART_DEVICE  *pDevice)
{
     volatile uint16_t UNUSED nTemp;

#if (ADI_UART_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
        pDevice->pUARTRegs->COMLCR = (uint16_t)(
            ((uint32_t)ADI_UART_CFG_WORD_LENGTH          << BITP_UART_COMLCR_WLS)  |
            ((uint32_t)ADI_UART_CFG_STOP_BIT             << BITP_UART_COMLCR_STOP) |
            ((uint32_t)ADI_UART_CFG_ENABLE_PARITY        << BITP_UART_COMLCR_PEN)  |
            ((uint32_t)ADI_UART_CFG_PARITY_SELECTION     << BITP_UART_COMLCR_EPS)  |
            ((uint32_t)ADI_UART_CFG_ENABLE_STICKY_PARITY << BITP_UART_COMLCR_SP));
        /* Div-C register */
       pDevice->pUARTRegs->COMDIV = (uint16_t)ADI_UART_CFG_DIVC;
       /* Div-M and Div-N */
       pDevice->pUARTRegs->COMFBR    =  (uint16_t)(
            ((uint32_t)ADI_UART_CFG_DIVN << BITP_UART_COMFBR_DIVN) |
            ((uint32_t)ADI_UART_CFG_DIVM << BITP_UART_COMFBR_DIVM) |
            BITM_UART_COMFBR_FBEN);
       pDevice->pUARTRegs->COMLCR2 = (uint16_t)ADI_UART_CFG_OSR;
                                     
       pDevice->nBaudRate = 
           (uint32_t)ADI_UART_CFG_CLOCK * (1u<<(9-ADI_UART_CFG_OSR)) /
           (((uint32_t)ADI_UART_CFG_DIVM * (1u<<11) + (uint32_t)ADI_UART_CFG_DIVN) * (uint32_t)ADI_UART_CFG_DIVC);       
       pDevice->nNumbits = (uint32_t)ADI_UART_CFG_NUM_BITS;
       pDevice->pUARTRegs->COMLCR2 = (uint16_t)ADI_UART_CFG_OSR;
       pDevice->pUARTRegs->COMFCR  = (uint16_t)(
           ((uint32_t)ADI_UART_CFG_ENABLE_FIFO << BITP_UART_COMFCR_FIFOEN) |
           ((uint32_t)ADI_UART_CFG_TRIG_LEVEL << BITP_UART_COMFCR_RFTRIG));
       pDevice->pUARTRegs->COMRSC  = (uint16_t)(
           ((uint32_t)ADI_UART_CFG_SOUT_POLARITY << BITP_UART_COMRSC_OENP) |
           ((uint32_t)ADI_UART_CFG_DEASSERTION << BITP_UART_COMRSC_OENSP)  |
           ((uint32_t)ADI_UART_CFG_DISABLE_RX << BITP_UART_COMRSC_DISRX)   |
           ((uint32_t)ADI_UART_CFG_HOLD_TX << BITP_UART_COMRSC_DISTX));
       
       pDevice->pUARTRegs->COMIEN  = (uint16_t)(
           ((uint32_t)ADI_UART_CFG_ENABLE_MODEM_STATUS_INTERRUPT << BITP_UART_COMIEN_EDSSI) |
           ((uint32_t)ADI_UART_CFG_ENABLE_RX_STATUS_INTERRUPT << BITP_UART_COMIEN_ELSI));
       
#else
    /* Reset all required UART registers to reset value */
    pDevice->pUARTRegs->COMLCR    =(uint16_t) (ADI_UART_NO_PARITY | (uint16_t)ADI_UART_ONE_AND_HALF_TWO_STOPBITS | (uint16_t)ADI_UART_WORDLEN_8BITS);
    pDevice->pUARTRegs->COMMCR    = 0u;
    nTemp= pDevice->pUARTRegs->COMLSR;
     nTemp= pDevice->pUARTRegs->COMRX;
    pDevice->pUARTRegs->COMSCR    = 0u;
    pDevice->nHwError             = (uint32_t)ADI_UART_HW_NO_ERR;
    pDevice->pUARTRegs->COMIEN    = 0u;
       /*Default baudrate divider values.DIv-C  Div-M and Div-N */
    pDevice->pUARTRegs->COMDIV    = 22u;
    pDevice->pUARTRegs->COMFBR    = 1734u << BITP_UART_COMFBR_DIVN |
                                    3u << BITP_UART_COMFBR_DIVM |
                                    BITM_UART_COMFBR_FBEN;
    pDevice->pUARTRegs->COMLCR2 = (uint16_t)ADI_UART_CFG_OSR;  
    pDevice->nBaudRate = ((uint32_t)ADI_UART_CFG_CLOCK * (1u << (9 - ADI_UART_CFG_OSR))) /
                         ((((uint32_t)ADI_UART_CFG_DIVM * (1u << 11)) + (uint32_t)ADI_UART_CFG_DIVN) * (uint32_t)ADI_UART_CFG_DIVC);
    pDevice->pUARTRegs->COMFCR  = 0u;
    pDevice->nNumbits = DEFAULT_NUM_BITS;    
#endif
}
/*
 * @brief                  Does the channel specific initialization.
 *
 * @param [in]  pDevice    Pointer to "ADI_UART_DEVICE" which is defnied to manage the UART device.
 * @param [in]  pChannel  "pointer" to "UART_DATA_CHANNEL" which is defined for managing the
 *                         data transfer in Tx and Rx direction.
 * @param [in]  eDMAMap       Peripheral Map id .
 * @param [in]  pfDataHandler Function pointer for the handling the data transfer in interrupt mode.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS     Successfully enable/disable the specified UART and DMA.
 *    - #ADI_UART_FAILED      Failed to create the semaphore for the specified channel.
 *    - #ADI_UART_INT_REGISTRATION_FAILED Failed to register the device with interrupt manager.
 *
 * @sa  adi_UART_EnableDMAMode()
 *
*/

static ADI_UART_RESULT uart_channel_init(ADI_UART_DEVICE  *pDevice,
                                         UART_DATA_CHANNEL *pChannel,
                                         ADI_DMA_CHANNEL_ID eDMAChannel
                                         )
{
    pChannel->eDataTranferMode   = ADI_UART_DATA_TRANSFER_MODE_NONE;
    pChannel->eChannelStatus    = CHANNEL_STATE_DATA_FLOW_DISABLED;
    pChannel->nActiveBufferCount = 0x00U;
    pChannel->bDMAMode = false;
    /* By default, Load the function table for interrupt mode.
       function adi_uart_EnalbeDMAMode should be called  to operated
       UART in DMA mode. */
     pChannel->pfSubmitBuffer = uart_SubmittBufferIntmode;

     pChannel->pfEnableDMA    = NULL;
    
    uart_InitChannelDescs(ADI_NUM_UART_BUFFER,pChannel);
#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* create a semaphore for buffer management */
    CREATE_SEM(pChannel, ADI_UART_FAILED); 
#endif
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    {
    ADI_DMA_TRANSFER *pDMADesc = &pChannel->oDMADescriptor ;
    pDMADesc->DataWidth    = ADI_DMA_WIDTH_1_BYTE;
    if(eDMAChannel == ADI_DMA_CHANNEL_UART_TX)
    {
        pDMADesc->pDstData     = &pDevice->pUARTRegs->COMTX;
        pDMADesc->DstInc       = ADI_DMA_INCR_NONE;
        pDMADesc->SrcInc       = ADI_DMA_INCR_1_BYTE;
    }
    else
    {
        pDMADesc->pSrcData     = &pDevice->pUARTRegs->COMRX;
        pDMADesc->SrcInc       = ADI_DMA_INCR_NONE;
        pDMADesc->DstInc       = ADI_DMA_INCR_1_BYTE;
    }
    }
#endif

    return(ADI_UART_SUCCESS);
}
/*
 * @brief       Pends for data transfer to complete.
 *
 * @param [in]  pDevice    Pointer to "ADI_UART_DEVICE" which is defnied to manage the UART device.
 * @param [in]  pChannel   "pointer" to "UART_DATA_CHANNEL" which is defined for managing the
 *                         data transfer in Tx and Rx direction.
 * @param [out] pBuffer    Address of buffer on which data transfer being carried out.
 * @param [out] pfEnable   Function pointer for enabling/disabling TX/RX.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS            Device obtained a buffer for the specified channel
 *    - #ADI_UART_FAILED             Failed to obtain a processed  buffer
 *    - #ADI_UART_PERIPHERAL_ERROR   An hardware error encountered by the device.
 *
 * @sa  adi_UART_EnableDMAMode()
 *
*/
static ADI_UART_RESULT uart_pendForBuffer(ADI_UART_DEVICE  *pDevice,
                                          UART_DATA_CHANNEL *pChannel,
                                          void *const pBuffer,
                                          UART_ENABLE pfEnable,
                                          ADI_UART_DIRECTION eDirection
                                          )
{
    ADI_UART_RESULT eResult= ADI_UART_FAILED;
    uint32_t i;
    bool_t bTxcomplete = true;
    /* Enable the device in the Tx direction */
    eResult = pfEnable(pDevice,true);
    if(eResult != ADI_UART_SUCCESS)
    {
        return(eResult);
    }
#if (ADI_CFG_ENABLE_RTOS_SUPPORT ==1)
    {
       PEND_EVENT(pChannel,ADI_UART_FAILED);
    }
#else
        while(pChannel->pProcDesc->bInUse == true){}

#endif
        if(pDevice->nHwError != (uint32_t)ADI_UART_HW_NO_ERR)
        {
             return(ADI_UART_PERIPHERAL_ERROR);
        }

        if(pBuffer ==  pChannel->pFreeDesc->pStartAddress)
        {
             ADI_ENTER_CRITICAL_REGION();
            pChannel->pFreeDesc->pStartAddress = NULL;
            /* Enable the device in the specified direction */
            pChannel->pCurrDesc = pChannel->pProcDesc = pChannel->pFreeDesc;
            pChannel->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
            ADI_EXIT_CRITICAL_REGION();
        }
        else
        {
            /* Should never enter here since semaphore pend ensured the
               availability of the buffer
             */
        }
        if(ADI_UART_DIR_TRANSMIT == eDirection)
        {
            bTxcomplete = false;
            for(i=0u;i<MAX_WAIT_TIME;i++)
            {
                /* Wait till the data is completely drained */
                if((pDevice->pUARTRegs->COMLSR & BITM_UART_COMLSR_TEMT) == BITM_UART_COMLSR_TEMT)
                {
                    bTxcomplete = true;
                    break;
                }
            }
        }
        eResult = pfEnable(pDevice,false);
        if(true == bTxcomplete)
        {
            return(eResult);
        }
        return(ADI_UART_FAILED);

}
/*
 * @brief      Closes the specified channel.
 *
 * @param [in]  pChannel   "pointer" to "UART_DATA_CHANNEL" which is defined for managing the
 *                         data transfer in Tx and Rx direction.
 * @param [in]  bDMAMode   Boolean flag which indicate whether device is opened in DMA mode.
 * @return      Status
 *    - #ADI_UART_SUCCESS            Device obtained a buffer for the specified channel
 *    - #ADI_UART_FAILED             Failed to obtain a processed  buffer
 *
 *
*/
static ADI_UART_RESULT uart_CloseChannel(UART_DATA_CHANNEL *pChannel)
{
#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    DELETE_SEM(pChannel, ADI_UART_FAILED);
#endif
    return(ADI_UART_SUCCESS);
}
/*
 * @brief                  A blocking function for the specified channel. This function pends on a semaphore
 *                         untill it is posted.
 *
 * @param [in]  pDevice    Pointer to "ADI_UART_DEVICE" which is defined to manage the UART device.
 * @param [in]  pChannel   "pointer" to "UART_DATA_CHANNEL" which is defined for managing the
 *                         data transfer in Tx and Rx direction.
 * @param [out] ppBuffer   Pointer to a location to where the the address of the buffer to be  written.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS     Successfully enable/disable the specified UART and DMA.
 *    - #ADI_UART_FAILED      Failed to get an buffer.
 *
 * @sa  adi_UART_EnableDMAMode()
 *
*/
static ADI_UART_RESULT uart_getbuffer(ADI_UART_DEVICE  *pDevice ,
                                      UART_DATA_CHANNEL *pChannel,
                                      void **ppBuffer
                                      )
{
    /* Set the ppBuffer and pBUfSize to NULL and zero in case there is an error */
    *ppBuffer = NULL;
     pChannel->nGetbufferCount+=1u;
     {
         PEND_EVENT(pChannel,ADI_UART_FAILED);
     }
    pChannel->nGetbufferCount-=1u;        
    if(pDevice->nHwError !=(uint32_t) ADI_UART_HW_NO_ERR)
    {
        return(ADI_UART_PERIPHERAL_ERROR);
    }
    if(pChannel->eChannelStatus != CHANNEL_STATE_DATA_FLOW_DISABLED)
    {
        ADI_ENTER_CRITICAL_REGION();
        *ppBuffer = pChannel->pFreeDesc->pStartAddress;
        /* set the address as NULL */
        pChannel->pFreeDesc->pStartAddress = NULL;
        pChannel->pFreeDesc = pChannel->pFreeDesc->pNext;
        ADI_EXIT_CRITICAL_REGION();
    }
    return(ADI_UART_SUCCESS);
}
#ifdef ADI_DEBUG
/*
 * @brief       Validate the device handle
 *
 * @param [in]  pDevice    Device handle to ve validated.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS          Specified handle is valid.
 *    - #ADI_UART_INVALID_HANDLE   Specified handle is invalid.
 *
*/

static ADI_UART_RESULT ValidateHandle( ADI_UART_DEVICE *pInDevice)
{
    /* Return code */
    ADI_UART_RESULT    nResult = ADI_UART_INVALID_HANDLE;
    /* Pointer to UART device instance */
    uint32_t i;

    for(i = 0U; i <  ADI_UART_NUM_DEVICES; i++)
    {

      if(pInDevice == uart_device_info[i].hDevice)
       {
           nResult = ADI_UART_SUCCESS;
           break;
       }
    }
    return (nResult);
}

#endif /* ADI_DEBUG */
/*! \endcond */

#endif /* _ADI_UART_V1_C_ */

/*@}*/
