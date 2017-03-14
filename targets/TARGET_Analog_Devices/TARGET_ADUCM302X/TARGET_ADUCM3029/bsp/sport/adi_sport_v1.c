/*! *****************************************************************************
 * @file:    adi_sport_v1.c
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


/** @addtogroup SPORT_Driver SPORT Device Driver
 *  @{
 */

#ifndef _ADI_SPORT_V1_C_
/*! \cond PRIVATE */
#define _ADI_SPORT_V1_C_

/*********************************************************************

Include files

*********************************************************************/
#include <ssdd_common/common_def.h>
#include <drivers/sport/adi_sport.h>

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

#define SPORT_eSTATE(deviceNum)    sport_device_info[(deviceNum)][eChannel].eState
#include "adi_sport_data_v1.c"
#ifdef ADI_DEBUG
static ADI_SPORT_RESULT  ValidateHandle(ADI_SPORT_DEVICE *pInDevice);
#endif /* ADI_DEBUG */

/*! \endcond */

/**********************************************************
  General SPORT APIs
**********************************************************/
/**
 * @brief       Initialization function for the SPORT device.
 *
 * @details     Initialize the specified SPORT. By default, Device is configured with the following
 *              settings. This function must be called before operating any SPORT device.
 *
 * @param [in]  nDeviceNum  SPORT Device instance to be opened.
 *
 * @param [in]  eChannel    Channel ID of the SPORT device( A or B)
 *
 * @param [in]  eDirection  Direction of the SPORT operation. (i.e Rx or Tx)
 *
 * @param [in]  pMemory     Pointer to a 32 bit aligned buffer of size ADI_SPORT_MEMORY_SIZE
 *                          required by the driver for the operation of specified SPORT device.
 *                          The "ADI_SPORT_MEMORY_SIZE" varies based on the configuration.
 *
 * @param [in]  nMemSize    Size of the buffer to which "pMemory" points. 
 *
 * @param [out] phDevice    Pointer to a location where SPORT device handle to be written. This
 *                          handle will be used as an identity of device and need to be passed as
 *                          one of the argument to functions which manage the device.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                      Successfully initialized the specified SPORT device.
 *
 *  - #ADI_SPORT_DEVICE_IN_USE                Specified SPORT is already initialized.
 *
 *  - #ADI_SPORT_FAILED                       Failed initialize a semaphore for managing device.
 *
 *  - #ADI_SPORT_INVALID_CHANNEL_ID      [D]  Invalid channel ID.
 *
 *  - #ADI_SPORT_INVALID_MODE            [D]  Invalid SPORT mode.
 *
 *  - #ADI_SPORT_INVALID_DIR             [D]  Invalid direction.
 *
 *  - #ADI_SPORT_INVALID_DEVICE_NUM      [D]  Specified device instance is invalid(valid range 0 -
 *                                            2).
 *
 *  - #ADI_SPORT_INVALID_NULL_POINTER    [D]  Specified pointer is invalid(either callback function
 *                                            or device handle).
 *
 * @sa          adi_sport_Close()
 *
 */

ADI_SPORT_RESULT adi_sport_Open(
                                 uint32_t             nDeviceNum,
                                 ADI_SPORT_CHANNEL    eChannel,
                                 ADI_SPORT_DIRECTION  eDirection,
                                 void                *pMemory,
                                 uint32_t             nMemSize,
                                 ADI_SPORT_HANDLE *const   phDevice
                                  )
{
    /* Pointer to the SPORT Device Instance Data */
    ADI_SPORT_DEVICE       *pDevice;
    
#ifdef ADI_DEBUG

    /* Check if the given device number is within the range of number SPORT
     * devices present in the processor
     */
    if(nDeviceNum >= ADI_SPORT_NUM_INSTANCES)
    {
        return ADI_SPORT_INVALID_DEVICE_NUM;
    }

    /* Check if the callback function pointer and  pointer to device handle is valid */
    if((phDevice == NULL))
    {
        return ADI_SPORT_INVALID_NULL_POINTER;
    }

    if( (eDirection != ADI_SPORT_DIR_RX) && (eDirection != ADI_SPORT_DIR_TX))
    {
        return ADI_SPORT_INVALID_DIR;
    }

    if(eChannel > ADI_HALF_SPORT_B)
    {
        return ADI_SPORT_INVALID_CHANNEL_ID;
    }

    /* Asserts to validate the memory size macros */
    assert(ADI_SPORT_MEMORY_SIZE == sizeof(ADI_SPORT_DEVICE));

    /* Check if the device instance is in use */
    if(SPORT_eSTATE(nDeviceNum) != ADI_SPORT_STATE_UNINITIALIZED)
    {
        return ADI_SPORT_DEVICE_IN_USE;
    }

#endif /* ADI_DEBUG */
    memset(pMemory, 0, nMemSize);
    /* Assign the pointers with the supplied memory. */
    pDevice = pMemory;
    /* Initialize the  pointers which provides the device information. */
    pDevice->pSportInfo   =   &sport_device_info[nDeviceNum][eChannel];
    /* Initialize the base address of the device */
    pDevice->pSPORTRegs   =   sport_device_info[nDeviceNum][eChannel].pSportRegs;
    /* Store the direction */
    pDevice->eDirection   =   eDirection;
    /* By default, Load the function table for interrupt mode.
       function adi_sport_EnableDMAMode should be called  to operated
       SPORT in DMA mode. */
    /* Initialize other parameters */
    ADI_INSTALL_HANDLER(sport_device_info[nDeviceNum][eChannel].eIRQn,(ADI_INT_HANDLER_PTR)sport_device_info[nDeviceNum][eChannel].pfInthandler);
    /* clear the Hw event status */
    pDevice->nHwError  = (uint32_t) ADI_SPORT_HW_NO_ERR;
    /* Initialize  the buffer management pointers */
    InitDevice(pDevice);
    /* Create the semaphore  */
#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* create a semaphore for buffer management */
     CREATE_SEM(pDevice,ADI_SPORT_FAILED);
#endif
#if (ADI_SPORT_CFG_ENABLE_STATIC_CONFIG_SUPPORT ==1)
    pDevice->pSPORTRegs->CTL_A          =  gSPORTfg[nDeviceNum][eChannel].SPORT_CTL;
    pDevice->pSPORTRegs->DIV_A          =  gSPORTfg[nDeviceNum][eChannel].SPORT_DIV;
    pDevice->pSPORTRegs->CNVT_A         =  gSPORTfg[nDeviceNum][eChannel].TIM_CONVT;
#else
    pDevice->pSPORTRegs->CTL_A    = 0x00U;
    pDevice->pSPORTRegs->DIV_A    = 0x00U;
    pDevice->pSPORTRegs->IEN_A    = 0x00U;
    pDevice->pSPORTRegs->CNVT_A   = 0x00U;
#endif
    pDevice->pSPORTRegs->NUMTRAN_A    = 0x00U;
    /* Set the direction of operation */
    if(eDirection == ADI_SPORT_DIR_TX)
    {
        pDevice->pSPORTRegs->CTL_A |= BITM_SPORT_CTL_A_SPTRAN;
    }    
    
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
    if(eDirection == ADI_SPORT_DIR_TX)
    {
       pDevice->oDMADescriptor.pDstData = &pDevice->pSPORTRegs->TX_A;
       pDevice->oDMADescriptor.DstInc = ADI_DMA_INCR_NONE;
    }
    else
    {
       pDevice->oDMADescriptor.pSrcData = &pDevice->pSPORTRegs->RX_A;
       pDevice->oDMADescriptor.SrcInc = ADI_DMA_INCR_NONE;
    }
#if (ADI_SPORT_CFG_ENABLE_STATIC_CONFIG_SUPPORT ==1)
    pDevice->oDMADescriptor.DataWidth = gSPORTfg[nDeviceNum][eChannel].DMA_WIDTH;             
    if(eDirection == ADI_SPORT_DIR_TX)
    {
       pDevice->oDMADescriptor.SrcInc  = gSPORTfg[nDeviceNum][eChannel].DMA_INC;
    }
    else
    {
       pDevice->oDMADescriptor.DstInc = gSPORTfg[nDeviceNum][eChannel].DMA_INC;
    }
#endif
    
#if (ADI_SPORT_CFG_ENABLE_DMA == 1)    
    if(adi_sport_EnableDMAMode(pDevice,true)!= ADI_SPORT_SUCCESS)
    {
        return(ADI_SPORT_DMA_FAILED);
    }
#endif
    pDevice->pfSubmitBuffer    =  SubmitBufferDmaMode;
    pDevice->pfEnableDMA       =  EnableDma;
#else
    pDevice->pfSubmitBuffer    =  SubmitBufferIntMode;
    pDevice->pfEnableDMA       =  NULL;
    ADI_ENABLE_INT( sport_device_info[nDeviceNum][eChannel].eIRQn);

#endif
    /* Change the state of the specified device */
    ADI_ENTER_CRITICAL_REGION();
    SPORT_eSTATE(nDeviceNum) =   ADI_SPORT_STATE_INITIALIZED;
    sport_device_info[nDeviceNum][eChannel].hDevice = pDevice;
    ADI_EXIT_CRITICAL_REGION();
  
    /* Return the device handle to the application */
    *phDevice   =   pDevice;

    return ADI_SPORT_SUCCESS;
}

/**
 * @brief       Closes the operation of specified SPORT device.
 *
 * @details     Closes the operation of specified SPORT device. Associated DMA will be closed if
 *              the device is operating in DMA mode. Device need to be opened again for any further
 *              use.
 *
 * @param [in]  hDevice  SPORT device handle whose operation is to be closed. This handle was
 *                       obtained when a SPORT device is opened successfully.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                      Successfully closed the specified SPORT device.
 *
 *  - #ADI_SPORT_DMA_FAILED                   Failed to close the DMA associated with the specified
 *                                            SPORT device.
 *
 *
 *  - #ADI_SPORT_INVALID_HANDLE          [D]  Invalid SPORT device handle.
 *
 * @note        It is user's responsibility to free/reuse the memory supplied during the opening of
 *              the device.
 *
 * @sa          adi_sport_Open()
 *
 */
ADI_SPORT_RESULT adi_sport_Close(
                                  ADI_SPORT_HANDLE const hDevice
                                )
  {
      /* Pointer to SPORT device instance */
      ADI_SPORT_DEVICE     *pDevice = (ADI_SPORT_DEVICE *)hDevice;
  #ifdef ADI_DEBUG
      /* Return code */
      ADI_SPORT_RESULT     eResult;

      /* Validate the given handle */
      if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
      {
          return eResult;
      }
  #endif /* ADI_DEBUG */

      /* Initialize  the buffer management pointers */
      InitDevice(pDevice);
      /* Close the DMA Channel associated with the specified SPORT device */
#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
      DELETE_SEM(pDevice,ADI_SPORT_FAILED); 
#endif

#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
      if(adi_dma_Close(pDevice->hDma) != ADI_DMA_SUCCESS)
      {
        return(ADI_SPORT_DMA_FAILED);
      }
#endif
      /* Free up the device */
      ADI_ENTER_CRITICAL_REGION();
      pDevice->pSportInfo->eState  = ADI_SPORT_STATE_UNINITIALIZED;
      pDevice->pSportInfo->hDevice = NULL;
      ADI_EXIT_CRITICAL_REGION();
      return ADI_SPORT_SUCCESS;
  }

/**
 * @brief       This function Enable/Disable the specified SPORT device.Associated DMA with the
 *              device will be enabled if the DMA mode is selected.
 *
 * @param [in]  hDevice  Device handle to SPORT device obtained when a SPORT device opened
 *                       successfully.
 *
 * @param [in]  bEnable  Boolean flag to indicate whether to enable/disable the SPORT as below.
 * \n
 * \n                    true : Enable the device for data transfer.
 * \n
 * \n                    false : Disable the device for data transfer.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully enable/disable the specified SPORT and DMA
 *                                          if the device is operating in DMA mode.
 *
 *  - #ADI_SPORT_DMA_FAILED                 Failed to enable the DMA associated with the device.
 *
 *  - #ADI_SPORT_FAILED                     Failed to enable/disable the specified SPORT device and
 *                                          DMA.
 *
 *  - #ADI_SPORT_BUFFERS_NOT_SUBMITTED [D]      Buffers are not submitted for data transfer.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 * @sa          adi_sport_EnableDMAMode()
 *
 */
ADI_SPORT_RESULT adi_sport_Enable(
                                   ADI_SPORT_HANDLE  const hDevice,
                                   bool_t            bEnable
                                 )
{
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_SPORT_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    /* Check if buffers are submitted before enabling the device */
     if((pDevice->pActiveDesc->pStartAddress == NULL) && (bEnable == true))
     {
         return ADI_SPORT_BUFFERS_NOT_SUBMITTED;
     }

#endif /* ADI_DEBUG */
    /* Check if the data flow is already enabled/disabled */
    if((ADI_SPORT_STATE_DATA_FLOW_ENABLED == pDevice->pSportInfo->eState) && (bEnable == true))
    {
        /* If it already in the required state do nothing */
        return ADI_SPORT_SUCCESS;
    }
    /*Enable the device ? */
    if(bEnable == true)
    {
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
          if(pDevice->pfEnableDMA(pDevice,true) != ADI_SPORT_SUCCESS)
            {
                return ADI_SPORT_DMA_FAILED;
            }
#endif
            NVIC_ClearPendingIRQ(pDevice->pSportInfo->eIRQn);
            pDevice->pSPORTRegs->IEN_A  |= BITM_SPORT_IEN_A_DATA;
            pDevice->pSPORTRegs->CTL_A |= BITM_SPORT_CTL_A_SPEN;
            ADI_ENABLE_INT(pDevice->pSportInfo->eIRQn);
            /* Enable the interrupt for the specified  SPORT */
            pDevice->pSportInfo->eState =   ADI_SPORT_STATE_DATA_FLOW_ENABLED;

    }
    /* Disable the device since bEnable is false */
    else
    {
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
           if((pDevice->pfEnableDMA(pDevice,false)) != ADI_SPORT_SUCCESS)
           {
                return ADI_SPORT_DMA_FAILED;
           }
#endif
           pDevice->pSPORTRegs->CTL_A &= ~BITM_SPORT_CTL_A_SPEN;
           pDevice->pSPORTRegs->IEN_A &= ~(BITM_SPORT_IEN_A_TF | BITM_SPORT_IEN_A_DATA);
           pDevice->pSPORTRegs->STAT_A = BITM_SPORT_STAT_A_SYSDATERR | BITM_SPORT_STAT_A_FSERR | BITM_SPORT_STAT_A_DERR | BITM_SPORT_STAT_A_TFI;
           pDevice->pSPORTRegs->NUMTRAN_A = 0x00U;
           ADI_DISABLE_INT(pDevice->pSportInfo->eIRQn);
           pDevice->pSportInfo->eState = ADI_SPORT_STATE_DATA_FLOW_DISABLED;
    }
    return ADI_SPORT_SUCCESS;
}

/**
 * @brief       Submit the buffer for transmitting/receiving the data. This function can
 *              be used to submit the buffers for both transmitting and receiving. This function
 *              will be returned after successfully submitting the buffer for transmitting the data.
 *              User will be notified if an call back function is registered with an event code
 *              #ADI_SPORT_EVENT_RX_BUFFER_PROCESSED or #ADI_SPORT_EVENT_RX_BUFFER_PROCESSED"
 *              depending on the direction in which device is operating.
 *
 * @param [in]  hDevice   Device handle to SPORT device is obtained when a SPORT device is opened
 *                        successfully.
 *
 * @param [in]  pBuffer   Pointer to buffer from where data need to be transmitted OR to which
 *                        received data need to to be written.
 *
 * @param [in]  nBufSize  Size of the data to be transmitted (In bytes)/ received.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS             Successfully submitted the buffer to the specified device.
 *
 *  - #ADI_SPORT_FAILED              Failed to submit the buffer for data transmission.
 *
 *  - #ADI_SPORT_INVALID_HANDLE [D]  Invalid SPORT device handle.
 *
 * @sa          adi_sport_GetBuffer()
 *
 */
ADI_SPORT_RESULT adi_sport_SubmitBuffer(
                                         ADI_SPORT_HANDLE  const hDevice,
                                         void     *const   pBuffer,
                                         uint32_t          nBufSize
                                       )
{
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;
    /* Return code */
    ADI_SPORT_RESULT         eResult;


#ifdef ADI_DEBUG

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    if(nBufSize <= 2u)
    {
         if((pDevice->pSPORTRegs->CTL_A & BITM_SPORT_CTL_A_OPMODE) != 0u)
         {
             return(ADI_SPORT_INVALID_PARAMETER);
         }
    }
    if((nBufSize&~(BITM_SPORT_NUMTRAN_A_VALUE)) != 0u) 
    {
         return(ADI_SPORT_INVALID_PARAMETER);
    }
    
#endif /* ADI_DEBUG */
    /* call an appropriate function based on mode in which device is operating */
    eResult = pDevice->pfSubmitBuffer(hDevice,pBuffer,nBufSize);
    return(eResult);
}

/**
 * @brief       Gets an "empty" or "filled" buffer from SPORT device manager. This function returns
 *              the address of a processed buffer. This is a blocking function and waits till an
 *              buffer is avilable. This function returns error if an callback function is
 *              registered. #adi_sport_IsBufferAvailable can be used as a peek function to know
 *              whether an buffer is avilable.
 *
 * @param [in]  hDevice   Device handle to SPORT device, obtained when a SPORT device is opened
 *                        successfully.
 *
 * @param [out] ppBuffer  Pointer to a location where the the address of the buffer is to be
 *                        written. Contains the address of an "empty" buffer (i.e the content of the
 *                        buffer is transmitted) OR "filled" buffer which contains the received
 *                        data.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS               Successfully returned. ppBuffer points to the address of the
 *                                     buffer.
 *
 *  - #ADI_SPORT_FAILED                Failed to get the buffer since device is operating in call
 *                                     back mode. ppBuffer points NULL.
 *
 *  - #ADI_SPORT_PERIPHERAL_ERROR      Device encountered an peripheral error.
 *
 *  - #ADI_SPORT_INVALID_HANDLE   [D]  Invalid SPORT device handle.
 *
 * @sa          adi_sport_SubmitBuffer()
 * @sa          adi_sport_IsBufferAvailable()
 *
 */
ADI_SPORT_RESULT adi_sport_GetBuffer(
                                      ADI_SPORT_HANDLE  const hDevice,
                                      void     **const      ppBuffer
                                    )
{
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
    ADI_SPORT_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    *ppBuffer = NULL;

    if(pDevice->pfCallback != NULL)
    {
       return(ADI_SPORT_FAILED);
    }
    PEND_EVENT(pDevice,ADI_SPORT_FAILED);
    if(pDevice->nHwError != (uint32_t)ADI_SPORT_HW_NO_ERR)
    {
        return(ADI_SPORT_PERIPHERAL_ERROR);
    }
     ADI_ENTER_CRITICAL_REGION();
    *ppBuffer  = pDevice->pProcDesc->pStartAddress;
     /* set the address as NULL */
     pDevice->pProcDesc->pStartAddress = NULL;
     pDevice->pProcDesc = pDevice->pProcDesc->pNext;
     ADI_EXIT_CRITICAL_REGION();
     return(ADI_SPORT_SUCCESS);
}

/**
 * @brief       Peek function to know whether an empty/filled buffer is avilable. Call to this
 *              function is valid only if the call back function is not registered. Call to this
 *              function results in error if an call back function is registered.
 *
 * @param [in]  hDevice      Device handle to SPORT device obtained when a SPORT device is opened
 *                           successfully.
 *
 * @param [out] pbAvailable  Pointer to a boolean variable. Contains "True" if there is an
 *                           empty/filled buffer and a call to #adi_sport_GetBuffer is ensured to be
 *                           successful. Contains "false" if there is no empty buffer.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS             Successfully retrieved the status of availability of the
 *                                   buffer.
 *
 *  - #ADI_SPORT_INVALID_HANDLE [D]  Failed to retrieve the status of the buffer availability.
 *
 * @sa          adi_sport_GetBuffer()
 * @sa          adi_sport_GetBuffer()
 *
 */
ADI_SPORT_RESULT    adi_sport_IsBufferAvailable(
                                                 ADI_SPORT_HANDLE const hDevice,
                                                 bool_t          *const pbAvailable
                                               )
{
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;
    /* Return code */
    *pbAvailable = false;
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if(ValidateHandle(pDevice) != ADI_SPORT_SUCCESS)
    {
        return ADI_SPORT_INVALID_HANDLE;
    }

#endif /* ADI_DEBUG */
    /* An empty/filled buffer is avilable since "bInuse" flag is false */
    if((pDevice->pProcDesc->pStartAddress != NULL) && (pDevice->pProcDesc->bInUse == false))
    {
        *pbAvailable =true;
    }
    return(ADI_SPORT_SUCCESS);
}

/**
 * @brief       Registers a Callback function with the SPORT device driver. The registered call
 *              back function will be called when an buffer is processed OR hardware error(s)
 *              encountered.
 *
 * @param [in]  hDevice     Device handle to SPORT device is obtained when a SPORT device is opened
 *                          successfully.
 *
 * @param [in]  pfCallback  Function pointer to Callback function. Passing a NULL pointer will
 *                          unregister the call back function.
 *
 * @param [in]  pCBparam    Call back function parameter.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully registered specified callback function.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_RegisterCallback(
                                              ADI_SPORT_HANDLE const  hDevice,
                                              ADI_CALLBACK   const   pfCallback,
                                              void        *const     pCBparam
                                           )
{
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    ADI_SPORT_RESULT eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    /* Check if the data flow is already enabled */
    if(ADI_SPORT_STATE_DATA_FLOW_ENABLED == pDevice->pSportInfo->eState )
    {
        /* Not allowed to register a callback if the data flow is enabled. */
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
    /* Store the address of the callback function */
    pDevice->pfCallback =  pfCallback;
    /* Store the call back parameter */
    pDevice->pCBParam    =  pCBparam;
    /* Return success */
    return(ADI_SPORT_SUCCESS);
}
/*! \cond PRIVATE */
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
/*! \endcond */
/**
 * @brief       Enable the DMA mode associated with specified SPORT device.
 *              To use this function ADI_SPORT_CFG_ENABLE_DMA_SUPPORT should be enabled.
 *
 * @param [in]  hDevice         Device handle to SPORT device is obtained when a SPORT device is
 *                              opened successfully.
 *
 * @param [in]  bEnableDMAmode  Boolean flag which indicates whether to enable/disable the DMA mode
 *                              for the device.
 * \n
 * \n                           true : Enable the DMA mode.
 * \n
 * \n                           false : Disable the DMA mode. This makes it to work with interrupt
 *                              mode.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                      Successfully enable/disabled the DMA mode of
 *                                            operation for the specified SPORT.
 *
 *  - #ADI_SPORT_DMA_FAILED                   Failed to enable/disable the DMA associated with
 *                                            specified SPORT.
 *
 *  - #ADI_SPORT_INVALID_HANDLE          [D]  Invalid SPORT device handle.
 *
 * @sa          adi_sport_Enable()
 *
 */

ADI_SPORT_RESULT adi_sport_EnableDMAMode(
                                          ADI_SPORT_HANDLE  const hDevice,
                                          bool_t              bEnableDMAmode
                                        )
{
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE     *pDevice = (ADI_SPORT_DEVICE *)hDevice;
#ifdef ADI_DEBUG

#endif
    /* check whether DMA is already opened for the specified device */
    if(true == bEnableDMAmode)
    {
        /* Return success if device is already in DMA mode */ 
        if(pDevice->hDma == NULL)
        {
            if(adi_dma_Open(pDevice->pSportInfo->eDMAChnlID,pDevice->DMAmemory,&pDevice->hDma,&SPORTCommonDMAHandler,pDevice)!= ADI_DMA_SUCCESS)
            {
                return(ADI_SPORT_DMA_FAILED);
            }
        }
         pDevice->pSPORTRegs->CTL_A |=  (BITM_SPORT_CTL_A_DMAEN);
         pDevice->pfSubmitBuffer  = SubmitBufferDmaMode;
         pDevice->pfEnableDMA     = EnableDma;
    }
    else
    {
         /* Uninstall the current interrupt handler */
        pDevice->pSPORTRegs->CTL_A &=  ~(BITM_SPORT_CTL_A_DMAEN);
        ADI_INSTALL_HANDLER(pDevice->pSportInfo->eIRQn,pDevice->pSportInfo->pfInthandler);
        ADI_ENABLE_INT( pDevice->pSportInfo->eIRQn);
        /* Install interrupt mode function table */
        pDevice->pfSubmitBuffer  = SubmitBufferDmaMode;
        pDevice->pfEnableDMA     = NULL;
    }
    return(ADI_SPORT_SUCCESS);
}
/*! \cond PRIVATE */
#endif
/*! \endcond */
/**
 * @brief       Sets data format for the specified SPORT device.
 *
 * @details     Sets data type,Big endian (MSB first) OR Little endian (LSB first) and word
 *              length(in bits) for the specified SPORT device. This function returns an error if the
 *              device is already enabled.
 *
 * @param [in]  hDevice              Device handle to SPORT device.
 *
 * @param [in]  nWordLength          Specify the word size of the data. Valid range is from
 *                                   4(nWordLength = 3) to 32(nWordLength =31).
 *
 * @param [in]  bLSBFirst            Configure the specified SPORT device to operate either LSB
 *                                   first or MSB first.
 * \n
 * \n                                true : LSB first (Little endian) .
 * \n
 * \n                                false : MSB first (Big endian)
 *
 * @param [in]  ePackMode            Mode of packging need to configured. Please refer #ADI_SPORT_PACKING_MODE.
 *
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully configured the device to operate in
 *                                          specified data format.
 *
 *  - #ADI_SPORT_FAILED                     Failed set the specified data format.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_INVALID_WORD_LENGTH   [D]  Invalid word size.
 *
 *  - #ADI_SPORT_INVALID_DATA_TYPE     [D]  Invalid word size.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */

ADI_SPORT_RESULT adi_sport_ConfigData(
                                       ADI_SPORT_HANDLE const   hDevice,
                                       uint8_t            nWordLength,
                                       ADI_SPORT_PACKING_MODE  ePackMode,
                                       bool_t               bLSBFirst
                                     )
{
     uint32_t nDataCfg=0U;
     /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;
#ifdef ADI_DEBUG
     /* Return code */
     ADI_SPORT_RESULT        eResult;
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    /* Not allowed to change when data flow is enabled */
    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
#endif /* ADI_DEBUG */
    /* clear all the fields(i.e Set to "0" ) */
    pDevice->pSPORTRegs->CTL_A &=(uint32_t) (~(BITM_SPORT_DATA_CONFIG));
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
    {
        ADI_DMA_INCR_TYPE eInc;
        ADI_DMA_WIDTH_TYPE eWidth;

        if(nWordLength <= SPORT_BYTE_TRANSFER_LENGTH )
       {
           eInc =   ADI_DMA_INCR_1_BYTE;
           eWidth = ADI_DMA_WIDTH_1_BYTE;
       }
       else if(nWordLength <= SPORT_HALFWORD_TRANSFER_LENGTH )
       {
           eInc = ADI_DMA_INCR_2_BYTE;
           eWidth = ADI_DMA_WIDTH_2_BYTE;
       }
       else  if (nWordLength <= SPORT_WORD_TRANSFER_LENGTH)
       {
           eInc = ADI_DMA_INCR_4_BYTE;
           eWidth = ADI_DMA_WIDTH_4_BYTE;
       }
       else
       {
           return ADI_SPORT_INVALID_WORD_LENGTH;
       }
       if(pDevice->eDirection == ADI_SPORT_DIR_TX)
       {
           pDevice->oDMADescriptor.SrcInc =eInc;
       }
       else
       {
           pDevice->oDMADescriptor.DstInc = eInc;
       }
       pDevice->oDMADescriptor.DataWidth = eWidth;
    }
#endif
    if(bLSBFirst == true)
    {
        /* set the the LSB first field */
        nDataCfg |= BITM_SPORT_CTL_A_LSBF;
    }
        /* Enable the packing */
    nDataCfg |= (uint32_t)ePackMode | (((uint32_t)nWordLength - 1u) << BITP_SPORT_CTL_A_SLEN) ;
    pDevice->pSPORTRegs->CTL_A |= nDataCfg;
    return(ADI_SPORT_SUCCESS);
}

/**
 * @brief       Configure the clock for the specified SPORT device.
 *
 * @details     Configure the SPORT device to use the "internal/external " rising/falling clock
 *              edge,clock edge and for enabling the gated Clock Mode.
 *
 * @details     fspclk = fsclk/(2*( nClockRatio + 1))
 *
 * @details     fspclk: frequency of sport clock
 *
 * @param [in]  hDevice        Device handle to SPORT device.
 *
 * @param [in]  nClockRatio    The value which determines the ratio between System clock and sport
 *                             clock as explained above.
 *
 *
 * @param [in]  bUseIntlClock  Boolean flag to indicate whether to use internal clock or external
 *                             clock for data transmission. By default, device is configured to use
 *                             the external clock.
 * \n
 * \n                          true : Device configured to use Internal clock.
 * \n
 * \n                          false : Device configured to use external clock..
 *
 * @param [in]  bRisingEdge     Boolean flag to indicate whether to drive data and internal frame
 *                             sync with rising edge OR falling edge of SP clock.
 * \n
 * \n                          true : Use falling edge of the clock.
 * \n
 * \n                          false : Use rising edge of the clock.
 *
 * @param [in]  bGatedClk      Boolean flag to indicate whether to enable/disable gated clock for
 *                             the specified SPORT channel.Ignored in Multi channel mode. Clock will
 *                             be active only when active data is getting transmitted or received
 *                             when this mode is enabled.
 * \n                          true : Enable gated clock mode.
 * \n
 * \n                          false : Disable gated clock mode.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully configured clock for the specified device.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigClock(
                                        ADI_SPORT_HANDLE const hDevice,
                                        uint16_t          nClockRatio,
                                        bool_t              bUseIntlClock,
                                        bool_t              bRisingEdge,
                                        bool_t              bGatedClk
                                      )

{

     uint32_t nClkCfg=0U;
     /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
     ADI_SPORT_RESULT        eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */

    pDevice->pSPORTRegs->DIV_A &= ~BITM_SPORT_DIV_A_CLKDIV;
    /* Set the clock divisior */
    pDevice->pSPORTRegs->DIV_A |= nClockRatio & BITM_SPORT_DIV_A_CLKDIV;

    /* clear all clock configuration  fields */
    pDevice->pSPORTRegs->CTL_A &= ~BITM_SPORT_CLOCK_CONFIG;
    if(bUseIntlClock == true)
    {
        /* select the internal clock */
        nClkCfg  |= BITM_SPORT_CTL_A_ICLK;
    }
    if(bRisingEdge == true)
    {
        /* select the rising edge of the  clock */
        nClkCfg |= BITM_SPORT_CTL_A_CKRE;
    }
    if(bGatedClk  == true )
    {
        /* Enable the Gated clock */
        nClkCfg |= BITM_SPORT_CTL_A_GCLKEN;
    }
    /* set the configuration word */
    pDevice->pSPORTRegs->CTL_A |= nClkCfg;
    /* Return from the function */
    return (ADI_SPORT_SUCCESS);
}

/**
 * @brief       Frame Sync(FS) configuration for the specified sport. .
 *
 * @details     Configure the SPORT to use internal/external frame sync,level/edge sensitive
 *              early/late frame sync etc.
 *
 * @param [in]  hDevice           Device handle to SPORT device.
 *
 * @param [in]  nFsDivisor        The value which decides the number of sport clock cycles between
 *                                each frame count.
 *
 * @param [in]  bFSRequired       Boolean flag to indicate whether frame sync required OR not to
 *                                the frame sync for the data transfer.
 * \n
 * \n                             true : Device requires a frame sync for its operation.
 * \n
 * \n                             false : Device does not requires a frame sync for its operation
 * \n
 * \n
 *
 * @param [in]  bInternalFS       Boolean flag to indicate whether to configure the specified SPORT
 *                                device to use the internal frame sync OR external frame sync as
 *                                below.
 * \n
 * \n                             true : Use internal frame sync.
 * \n
 * \n                             false : Use external frame sync
 * \n
 * \n
 *
 * @param [in]  bDataFS           Boolean flag to indicate whether to configure the specified SPORT
 *                                device to use the data-independent frame sync OR Serial port uses
 *                                a data-dependent frame sync. Valid only if the specified device is
 *                                in "transmit"(TX)mode . Ignored if the device is opened in
 *                                "receive"(RX) mode.
 * \n
 * \n                             true : Use data-independent frame sync.
 * \n
 * \n                             false : Use data-dependent frame sync.
 * \n
 * \n
 *
 * @param [in]  bActiveLowFS      Boolean flag to indicate whether to configure the specified SPORT
 *                                device for active low frame sync OR active high frame sync. A call
 *                                to this function will return an error if SPORT is configured in I2S
 *                                mode.
 * \n
 * \n                             true : Use active low frame sync.
 * \n
 * \n                             false : Use active high frame sync.
 * \n
 * \n
 *
 * @param [in]  bLateFS           Boolean flag to indicate whether to use the late frame sync OR
 *                                Early frame sync.
 * \n
 * \n                             true : Use late frame sync.
 * \n
 * \n                             false : Use Early frame sync.
 * \n
 * \n
 *
* @param [in]  bFSErrorOperation  Frame Sync Error Operation.  This
 *\n                              decides the way the SPORT responds when a frame sync error occurs.
 * \n
 * \n                             true : When frame Sync error occurs, discard the receive data.
 * \n
 * \n                             false : Flag the Frame Sync error and continue normal operation
    * \n
 * \n
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully configured the frame sync requirement.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigFrameSync(
                                             ADI_SPORT_HANDLE const hDevice,
                                             uint16_t          nFsDivisor,
                                             bool_t              bFSRequired,
                                             bool_t              bInternalFS,
                                             bool_t              bDataFS,
                                             bool_t              bActiveLowFS,
                                             bool_t              bLateFS,
                                             bool_t              bFSErrorOperation
                                           )
{
     uint32_t nFsCfg=0U;
     /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* Return code */
     ADI_SPORT_RESULT        eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
     pDevice->pSPORTRegs->DIV_A &= ~BITM_SPORT_DIV_A_FSDIV;

     pDevice->pSPORTRegs->DIV_A |= (uint32_t)nFsDivisor << BITP_SPORT_DIV_A_FSDIV;

    /* clear all the fields of frame sync */
    pDevice->pSPORTRegs->CTL_A  &= ~BITM_SPORT_FS_CONFIG;
    /* Select the data independent frame sync. This bit should be set when device is opened in
       RX mode */
    if((pDevice->eDirection == ADI_SPORT_DIR_RX )||( bDataFS == true) )
    {
         nFsCfg = BITM_SPORT_CTL_A_DIFS ;
    }
    /* "Frame sync required" is reserved when device is operating in I2S and MC mode */
    if(bFSRequired == true)
    {
        /* Frame Sync(FS) is required */
        nFsCfg |= BITM_SPORT_CTL_A_FSR;
    }
    if(bInternalFS == true)
    {
        /* Select the internal Frame Sync(FS)*/
        nFsCfg |= BITM_SPORT_CTL_A_IFS;
    }
    if(bActiveLowFS == true)
    {
        /* Select the Active High Frame Sync(FS)*/
        nFsCfg |= BITM_SPORT_CTL_A_LFS;
    }
    if(bLateFS == true)
    {
        /* Select the Late  Frame Sync(FS)*/
        nFsCfg |= BITM_SPORT_CTL_A_LAFS;
    }
    if(bFSErrorOperation == true)
    {
        /* Select the edge sensitive   Frame Sync(FS)*/
        nFsCfg |= BITM_SPORT_CTL_A_FSERRMODE;
    }
    /* write the Frame sync configuration to control register */
    pDevice->pSPORTRegs->CTL_A |= nFsCfg;
    /* return success */
    return ADI_SPORT_SUCCESS;
}

/**
 * @brief       This function helps to find the Hardware error(s) encountered by the specified
 *              sport device.
 *
 * @param [in]  hDevice   Device handle to SPORT device, obtained when a SPORT device opened
 *                        successfully.
 *
 * @param [out] pHwError  Pointer to a variable where the error to be written.
 *
 * @param [out] pArg      Additional parameter passed to the user. For SPORT device it is NULL
 *                        since there is nothing to be sent to user.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully returned hardware error(s) occurred by the
 *                                          specified device.
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid sport device handle.
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Not allowed to be called when the call back function is registered.
 *
 */
ADI_SPORT_RESULT adi_sport_GetHWErrorStatus (
                                               ADI_SPORT_HANDLE  const  hDevice,
                                               uint32_t                *pHwError,
                                               void *                  *pArg
                                            )
{
    /* Pointer to SPORT device instance */
    register ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    /* Return code */
    ADI_SPORT_RESULT         eResult;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    if(pDevice->pfCallback != NULL)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
     /* Return the encountered Hardware error(s) */
    *pHwError = pDevice->nHwError;
    *pArg     = NULL;
    /* Clear */
    pDevice->nHwError  = (uint32_t) ADI_SPORT_HW_NO_ERR;
    /* Return success */
    return(ADI_SPORT_SUCCESS);
}
/**
 * @brief       Configure the SPORT use the Clocks and Frame Sync of other Half-Sport
 *
 * @param [in]  hDevice       Device handle to SPORT device.
 *
 * @param [in]  bUseOtherFS   Boolean flag to indicate whether to use own Frame Sync(false) OR to
 *                            use frame sync of other half sport (true).
 * \n
 * \n                         true : Use frame sync of other half sport device.
 * \n
 * \n                         false : Use own frame sync.
 *
 * @param [in]  bUseOtherClk  Boolean flag to indicate whether to use own clock clock(false) OR to
 *                            use clock of other half sport(true).
 * \n
 * \n                         true : Use clock of other half sport device.
 * \n
 * \n                         false : Use own clock.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully enabled the specified SPORT to use the clk
 *                                          and FS of other half sport.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_MultiplexSportSignal(
                                          ADI_SPORT_HANDLE  hDevice,
                                          bool_t            bUseOtherFS,
                                          bool_t            bUseOtherClk
                                        )
{
     uint32_t nCfg=0U;
     /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;

#ifdef ADI_DEBUG
     /* Return code */
     ADI_SPORT_RESULT        eResult=ADI_SPORT_FAILED;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }

    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
     /* clear the muxing fields of the control register 2 */
    pDevice->pSPORTRegs->CTL_A &=(uint32_t)(~(BITM_SPORT_CTL_A_CKMUXSEL |  BITM_SPORT_CTL_A_FSMUXSEL ));
    if(bUseOtherFS  == true)
    {
        /* Use the  the frame sync of  other half sport*/
        nCfg |= BITM_SPORT_CTL_A_FSMUXSEL;
    }
    if(bUseOtherClk  == true)
    {
        /* Use the  the clock of  other half sport*/
        nCfg |= BITM_SPORT_CTL_A_CKMUXSEL;
    }

    pDevice->pSPORTRegs->CTL_A  |= nCfg;

    return(ADI_SPORT_SUCCESS);
}
/**
 * @brief       Configure the SPORT use the Clocks and Frame Sync of other Half-Sport
 *
 * @param [in]  hDevice       Device handle to SPORT device.
 *
 * @param [in]  nFSDuration  Specify the value of the number of clocks which would be programmed corresponding to the
 *                           desired time duration from assertion of CONVT signal to Frame sync signal
 *
 * @param [in]  nWidth       Specify the value of the number of serial clocks for which CONVT signal should be active.

 *
 * @param [in]  bActiveLow    Boolean flag to indicate the polarity of the Convt signal.
 * \n
 * \n                         true :  Active low Polarity.
 * \n
 * \n                         false : Active High Polarity.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully enabled the specified SPORT to use the clk
 *                                          and FS of other half sport.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigTimerMode(
                 ADI_SPORT_HANDLE   hDevice,
                 uint8_t nFSDuration,
                 uint8_t nWidth,
                 bool_t bActiveLow
                 )
{
     uint32_t nCfg=0U;
     /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;

#ifdef ADI_DEBUG
     /* Return code */
     ADI_SPORT_RESULT        eResult=ADI_SPORT_FAILED;

    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }

    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }

#endif /* ADI_DEBUG */
     /* clear the muxing fields of the control register 2 */
    pDevice->pSPORTRegs->CNVT_A &=(uint32_t)(~(BITM_SPORT_CNVT_A_POL |  BITM_SPORT_CNVT_A_WID | BITM_SPORT_CNVT_A_CNVT2FS ));
    nCfg  = (((uint32_t)nFSDuration << BITP_SPORT_CNVT_A_CNVT2FS) | (uint32_t)(nWidth));
    if(bActiveLow  == true)
    {
        /* Use the  the clock of  other half sport*/
        nCfg    |= BITM_SPORT_CNVT_A_POL;
    }
    pDevice->pSPORTRegs->CNVT_A  |= nCfg;

    return(ADI_SPORT_SUCCESS);
}

/**
 * @brief       Sets the control word to specified SPORT device.
 *
 * @details     Sets the control word to specified SPORT device. It can be done either writing a
 *              control word OR by calling other functional APIs. If only few fields to be
 *              modified,call adi_sport_GetControlReg function to get the current register value and
 *              modify the required fields.
 *
 * @param [in]  hDevice       Device handle to which control word to be set.
 *
 * @param [in]  eRegister     Indicate which control register to be set.
 *
 * @param [in]  nControlWord  Sets the Control word for the specified SPORT device. There are
 *                            functional APIs are are provided to set the some of the most commonly
 *                            used fields. This function will overwrite the previous settings of the
 *                            entire control word.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully set the control word to the specified
 *                                          device.
 *
 *  - #ADI_SPORT_INVALID_REGISTER           Specified register is invalid.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_SetRegister(
                                            ADI_SPORT_HANDLE    const    hDevice,
                                            ADI_SPORT_REGISTER     eRegister,
                                            uint32_t                nControlWord
                                        )
{
    /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;
     /* Return code */
     ADI_SPORT_RESULT        eResult = ADI_SPORT_SUCCESS;
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
     /* Word length */
     uint32_t nWordLength;
     
#endif
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
    /* Don't allow to change the control register when the Data flow is enabled.
     */
    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        return ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
#endif /* ADI_DEBUG */
    /* set the control register. */
    switch(eRegister)
    {
    case ADI_SPORT_CONTROL_REGISTER:
         /* Set the main control register */
         pDevice->pSPORTRegs->CTL_A  =  nControlWord;
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
         nWordLength = (nControlWord & BITM_SPORT_CTL_A_SLEN) >> BITP_SPORT_CTL_A_SLEN;
         if(nWordLength < SPORT_BYTE_TRANSFER_LENGTH )
         {
            pDevice->oDMADescriptor.SrcInc = ADI_DMA_INCR_1_BYTE;
            pDevice->oDMADescriptor.DataWidth = ADI_DMA_WIDTH_1_BYTE;
            
         }
         else if(nWordLength < SPORT_HALFWORD_TRANSFER_LENGTH )
         {
            pDevice->oDMADescriptor.SrcInc = ADI_DMA_INCR_2_BYTE;
            pDevice->oDMADescriptor.DataWidth = ADI_DMA_WIDTH_2_BYTE;

         }
         else /*(nWordLength <= SPORT_WORD_TRANSFER_LENGTH )*/
         {
             pDevice->oDMADescriptor.SrcInc = ADI_DMA_INCR_4_BYTE;
             pDevice->oDMADescriptor.DataWidth = ADI_DMA_WIDTH_4_BYTE;             
         }
#endif
         break;
    case ADI_SPORT_DIVISIOR_REGISTER:
         /* Set the multi channel control register */
         pDevice->pSPORTRegs->DIV_A  =  nControlWord;
         break;
    case ADI_SPORT_INTERRUPT_REGISTER:
         /* Set the error control register. Can be used to disable the status interrupt. */
        pDevice->pSPORTRegs->IEN_A = nControlWord;
         break;
    case ADI_SPORT_TIMER_CONVT:
         /* Set the error control register. Can be used to disable the status interrupt. */
        pDevice->pSPORTRegs->CNVT_A = nControlWord;
         break;
   default:
         /* Specified register is invalid */
       eResult=ADI_SPORT_INVALID_REGISTER;
       break;
    }
    /* Return the success code */
    return eResult;
}


/**
 * @brief       Gets the the control word of the specified SPORT device.
 *
 * @details     Gets the content of the control register (control word) of the specified SPORT
 *              device if it is opened. This function can be used to know the current content of
 *              control register. This function can be helpful to modify few fields in the control
 *              word.
 *
 * @param [in]  hDevice       Device handle to SPORT device.
 *
 * @param [in]  eRegister     Indicate from which control register content to be retrieved.
 *
 * @param [out] pControlWord  Pointer to the location where the content of the control register to
 *                            be written.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS               Successfully retrieved the control register of the specified
 *                                     device.
 *
 *  - #ADI_SPORT_INVALID_REGISTER      Specified register is invalid.
 *
 *  - #ADI_SPORT_INVALID_HANDLE   [D]  Invalid SPORT device handle.
 *
 */
ADI_SPORT_RESULT adi_sport_GetRegister(
                                          ADI_SPORT_HANDLE     const hDevice,
                                          ADI_SPORT_REGISTER   eRegister,
                                          uint32_t            *pControlWord
                                        )
{
    /* Pointer to SPORT device instance */
     ADI_SPORT_DEVICE        *pDevice = (ADI_SPORT_DEVICE *)hDevice;
     ADI_SPORT_RESULT        eResult = ADI_SPORT_SUCCESS;
#ifdef ADI_DEBUG
     /* Return code */
    /* Validate the given handle */
    if((eResult = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS)
    {
        return eResult;
    }
#endif /* ADI_DEBUG */
    /* set the control register. */
    switch(eRegister)
    {
        case ADI_SPORT_CONTROL_REGISTER:
             /* Read from control register */
             *pControlWord = pDevice->pSPORTRegs->CTL_A;
             break;
        case ADI_SPORT_DIVISIOR_REGISTER:
              /* Read from control register-2 */
             *pControlWord = pDevice->pSPORTRegs->DIV_A;
             break;
        case ADI_SPORT_INTERRUPT_REGISTER:
              /* Read from multi channel control register */
             *pControlWord = pDevice->pSPORTRegs->IEN_A;
             break;
        case ADI_SPORT_TIMER_CONVT:
              /* Read from multi channel control register */
             *pControlWord = pDevice->pSPORTRegs->CNVT_A;
             break;

        default:
             /* Invalid register specified*/
            eResult= ADI_SPORT_INVALID_REGISTER;
            break;
    }
    return eResult;
}
/*! \cond PRIVATE */
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
/*
 * @brief       Submit the buffer for Data transfer in DMA Mode.
 *
 * @param [in]  hDevice  Device handle to SPORT device.
 *
 * @param [in]  pBuffer  Pointer to buffer from where DMA reads or writes.
 *
 * @param [in]  nSize    Size of the data to be transferred(In bytes).
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                 Successfully submitted the buffer to DMA associated with
 *                                       SPORT.
 *
 *  - #ADI_SPORT_BUFFERS_NOT_SUBMITTED   Failed to submit the buffer to DMA associated with SPORT.
 *
 */
static ADI_SPORT_RESULT SubmitBufferDmaMode(
                                           ADI_SPORT_HANDLE  hDevice,
                                           void             *pBuffer,
                                           uint32_t          nSize
                                          )
{
       /* Pointer to SPORT device instance */
        ADI_SPORT_DEVICE         *pDevice = (ADI_SPORT_DEVICE *)hDevice;

       /* Pointer to the descriptor that caused the interrupt */
        ADI_SPORT_BUFF_LIST  *pDesc;

        pDesc    =   pDevice->pCurrDesc;
        /* Initialize common parameters */
        if(pDesc->pStartAddress == NULL)
        {   if(pDevice->eDirection == ADI_SPORT_DIR_TX)
            {
                pDevice->oDMADescriptor.pSrcData = pBuffer;
            }
            else
            {
                pDevice->oDMADescriptor.pDstData = pBuffer;
            }
            pDevice->oDMADescriptor.NumTransfers = (nSize>>pDevice->oDMADescriptor.DataWidth);
            
            pDevice->pSPORTRegs->NUMTRAN_A = pDevice->oDMADescriptor.NumTransfers;

            if(adi_dma_SubmitTransfer(pDevice->hDma,&pDevice->oDMADescriptor) != ADI_DMA_SUCCESS)
            {
                return(ADI_SPORT_BUFFERS_NOT_SUBMITTED);
            }
            /* Update the information in the list */
            pDesc->pStartAddress    =   pBuffer;
            pDesc->bInUse           =   true;
            pDesc->nCount = nSize;
            /* Advance the current pointer */
            pDevice->pCurrDesc = pDevice->pCurrDesc->pNext;

            if(pDevice->pSportInfo->eState ==ADI_SPORT_STATE_PAUSED)
            {
                ADI_ENTER_CRITICAL_REGION();
                pDevice->pSportInfo->eState  = ADI_SPORT_STATE_DATA_FLOW_ENABLED;
                ADI_EXIT_CRITICAL_REGION();
            }

        }
        else
        {
             return (ADI_SPORT_BUFFERS_NOT_SUBMITTED);
        }
        return(ADI_SPORT_SUCCESS);

}
#else
/*
 * @brief       Submits the buffers for data transfer in interrupt mode.
 *
 * @param [in]  hDevice  Device handle to SPORT device.
 *
 * @param [in]  pBuffer  Pointer to buffer from where DMA reads or writes.
 *
 * @param [in]  nSize    Size of the data to be transferred(In bytes).
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                   Successfully submitted the buffer for data transfer.
 *
 *  - #ADI_SPORT_BUFFERS_NOT_SUBMITTED     No free descriptor for data transfer.
 *
 *
 */
static ADI_SPORT_RESULT SubmitBufferIntMode(
                                              ADI_SPORT_HANDLE hDevice,
                                              void            *pBuffer,
                                              uint32_t         nSize
                                           )
{
        ADI_SPORT_DEVICE       *pDevice = (ADI_SPORT_DEVICE  *)hDevice;
        ADI_SPORT_BUFF_LIST    *pCurDesc;
       if(pDevice->pCurrDesc->pStartAddress != NULL)
       {
           return(ADI_SPORT_BUFFERS_NOT_SUBMITTED);
       }
       else
       {
           ADI_ENTER_CRITICAL_REGION();
           pCurDesc = pDevice->pCurrDesc;
           /* set the Descriptor parameters for interrupt mode */
           pCurDesc->pStartAddress =   pBuffer;
           /* Always writes 32 bit word */
           pCurDesc->nCount        =   nSize >> 2;
           pCurDesc->nIndex        =   0U;
           pCurDesc->bInUse        =   true;
           pDevice->pCurrDesc = pDevice->pCurrDesc->pNext;
           pDevice->pSPORTRegs->NUMTRAN_A = pCurDesc->nCount;
           ADI_EXIT_CRITICAL_REGION();
           if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_PAUSED)
           {
                 if(adi_sport_ClearInterruptPendStatus((uint32_t)pDevice->pSportInfo->eIRQn)!= ADI_SPORT_SUCCESS)
                {
                    return(ADI_SPORT_FAILED);
                 }
                ADI_ENABLE_INT(pDevice->pSportInfo->eIRQn);
                ADI_ENTER_CRITICAL_REGION();
                pDevice->pSportInfo->eState = ADI_SPORT_STATE_DATA_FLOW_ENABLED;
                ADI_EXIT_CRITICAL_REGION();
            }
       }
       return(ADI_SPORT_SUCCESS);
}
#endif

/*
 * @brief       Sport interrupt handler for data transfer.
 *
 * @param [in]  IID       Interrupt ID.
 *
 * @param [in]  pCBParam  Callback parameter from interrupt manager.
 *
 */
static void sport_DataHandler(ADI_SPORT_DEVICE     *pDevice)
{
    ADI_SPORT_BUFF_LIST  *pProcessingDesc = pDevice->pActiveDesc;
    ADI_SPORT_EVENT eEvent;
    volatile uint32_t *pNextWord = (uint32_t *)pProcessingDesc->pStartAddress;
#ifdef ADI_DEBUG
    /* This should not happen but a safety check */
    if(pDevice == NULL)
    {
        return;
    }
#endif /* ADI_DEBUG */
    /* Is processing buffer is NULL */
    if(pDevice->pActiveDesc->pStartAddress!= NULL)
    {
        if(ADI_SPORT_DIR_RX == pDevice->eDirection)
        {
            /* Read the data from RX register if the direction is ADI_SPORT_DIR_RX */
            pNextWord[pProcessingDesc->nIndex] = pDevice->pSPORTRegs->RX_A;
            pProcessingDesc->nIndex++;
            eEvent = ADI_SPORT_EVENT_RX_BUFFER_PROCESSED;
        }
        else
        {
            /* Read the data into  TX register if the direction is ADI_SPORT_DIR_TX */
            pDevice->pSPORTRegs->TX_A = pNextWord[pProcessingDesc->nIndex];
            pProcessingDesc->nIndex++;
            eEvent = ADI_SPORT_EVENT_TX_BUFFER_PROCESSED;
        }

        if(pProcessingDesc->nIndex >= pProcessingDesc->nCount)
        {
            ADI_ENTER_CRITICAL_REGION();
            /* Advance the processed buffer pointer in the list */
            pDevice->pActiveDesc = pDevice->pActiveDesc->pNext;
            /* Restore the count */
            pProcessingDesc->nCount = pProcessingDesc->nCount << 2;
            /* Set the bInUse flag as false since we processed the buffer  */
            pProcessingDesc->bInUse = false;
            ADI_EXIT_CRITICAL_REGION();
            /* Call the callback function if the callback function is registered. */
            if(pDevice->pfCallback != NULL)
            {
                 pDevice->pfCallback(pDevice->pCBParam,(uint32_t)eEvent,pProcessingDesc->pStartAddress);
                 /* No need to keep the processed buffer address */
                 pProcessingDesc->pStartAddress = NULL;
            }
            else
            {
	        POST_EVENT(pDevice);
            }
        }
    }
    else
    {
        ADI_DISABLE_INT(pDevice->pSportInfo->eIRQn);
        ADI_ENTER_CRITICAL_REGION();
        pDevice->pSportInfo->eState = ADI_SPORT_STATE_PAUSED;
        ADI_EXIT_CRITICAL_REGION();

    }
    return;
}
/*
 * @brief       Sport interrupt handler.
 *
 * @param [in]  IID       Interrupt ID.
 *
 * @param [in]  pCBParam  Callback parameter from interrupt manager.
 *
 */
static void SPORTCommonStatusHandler(ADI_SPORT_DEVICE     *pDevice)
{
    uint32_t            Mask;
    uint32_t            StatRegValue;
    uint32_t            TmpStatRegValue;
    uint32_t            eEvent = (uint32_t)ADI_SPORT_HW_NO_ERR;
#ifdef ADI_DEBUG
    /* This should not happen but a safety check */
    if(pDevice == NULL)
    {
        return;
    }
#endif /* ADI_DEBUG */

    /* Get the contents of status register */

    StatRegValue    = pDevice->pSPORTRegs->STAT_A;
    TmpStatRegValue = StatRegValue & (BITM_SPORT_STAT_A_TFI | BITM_SPORT_STAT_A_SYSDATERR | BITM_SPORT_STAT_A_FSERR | BITM_SPORT_STAT_A_DERR | BITM_SPORT_STAT_A_DATA );

    /* For all the bits in the status register */
    for(Mask = BITM_SPORT_STAT_A_TFI; Mask <= BITM_SPORT_STAT_A_SYSDATERR; Mask <<= 1)
    {

        /* Take action only when the bit is set */
        if((TmpStatRegValue & Mask) != 0u)
        {
            switch(Mask)
            {
                /* Frame sync error*/
                case BITM_SPORT_STAT_A_FSERR:
                    eEvent |=(uint32_t) ADI_SPORT_HW_ERR_FS;
                    break;

                /* Error in secondary channel error */
                case BITM_SPORT_STAT_A_DERR:
                   if(pDevice->eDirection == ADI_SPORT_DIR_RX)
                   {
                         eEvent |=(uint32_t) ADI_SPORT_HW_ERR_RX_OVERFLOW;
                   }
                   else
                   {
                         eEvent |=(uint32_t) ADI_SPORT_HW_ERR_TX_UNDERFLOW;

                   }
                   break;
                case BITM_SPORT_STAT_A_SYSDATERR:
                         eEvent |=(uint32_t) ADI_SPORT_HW_ERR_SYSDATAERR;
                   break;
                case BITM_SPORT_STAT_A_DATA:
                   sport_DataHandler(pDevice);
                   break;
                default:
                   break;
            }
            /* Clear bit in local copy */
            TmpStatRegValue &= (~Mask);
        }

        /* If there are no more status bits set stop the loop */
        if(TmpStatRegValue == 0u)
        {
            break;
        }
    }
     if( pDevice->nHwError != 0u)
     {
        /* Clear the status register bits (W1C) */
        if(pDevice->pfCallback != NULL)
        {
            pDevice->pfCallback( pDevice->pCBParam,
                                 (uint32_t) eEvent,
                                 NULL
                               );
        }
        else
        {
            pDevice->nHwError = eEvent;
	    POST_EVENT(pDevice);
        }
     }
    pDevice->pSPORTRegs->STAT_A = StatRegValue & (BITM_SPORT_STAT_A_SYSDATERR | BITM_SPORT_STAT_A_FSERR | BITM_SPORT_STAT_A_DERR);
    return;
}

/*
 * @brief       Callback function for DMA manager.
 *
 * @param [in]  pCBParam  Callback parameter from DMA manager.
 *
 * @param [in]  Event     Event ID.
 *
 * @param [in]  pArg      Callback argument.
 *
 */
static void SPORTCommonDMAHandler(void *pCBParam, uint32_t Event, void *pArg)
{
    ADI_SPORT_DEVICE  *pDevice = pCBParam;
    ADI_SPORT_BUFF_LIST  *pProcessedDesc = pDevice->pActiveDesc;
    ADI_SPORT_EVENT       eSportEvent    = ADI_SPORT_EVENT_TX_BUFFER_PROCESSED;
    /* Pointer to the buffer address */
    void *pProcessedBuff = pDevice->pActiveDesc->pStartAddress;

#ifdef ADI_DEBUG
    /* This should not happen but a safety check */
    if(pDevice == NULL)
    {
        return;
    }
#endif /* ADI_DEBUG */
    ADI_ENTER_CRITICAL_REGION();
    pDevice->pActiveDesc->bInUse = false;
    pDevice->pActiveDesc = pDevice->pActiveDesc->pNext;
    ADI_EXIT_CRITICAL_REGION();

    if(pDevice->pfCallback != NULL)
    {
        if(pDevice->eDirection == ADI_SPORT_DIR_RX)
        {
            eSportEvent = ADI_SPORT_EVENT_RX_BUFFER_PROCESSED;
        }
        pDevice->pfCallback(pDevice->pCBParam,(uint32_t)eSportEvent,pProcessedBuff);
        pProcessedDesc->pStartAddress = NULL;
    }
    else
    {
	POST_EVENT(pDevice);
    }
}
/*
 * @brief       Create a circular linked list for buffer management.
 *
 * @param [in]  hDevice  Device handle to SPORT device.
 *
 * @param [in]  NumDesc  Number of descriptorS.
 *
 */
static void InitDevice (ADI_SPORT_DEVICE *pDevice)
{
    int32_t                i;
    ADI_SPORT_BUFF_LIST  *pDesc = &pDevice->BufInfo[0];
    /* Initialize the all descriptors. Make it circular. */
    for(i = 0; i < NUM_SPORT_BUFFER; i++)
    {
        pDesc[i].pStartAddress      =   NULL;
        pDesc[i].nCount             =   0u;
        pDesc[i].nIndex             =   0u;
        pDesc[i].pNext              =   &pDesc[i+1];
    }
    /* create a circular list */
    pDesc[i-1].pNext                =   &pDesc[0];

    pDevice->pActiveDesc =  &pDevice->BufInfo[0];
    pDevice->pProcDesc   =  &pDevice->BufInfo[0];
    pDevice->pCurrDesc   =  &pDevice->BufInfo[0];
    return;
}
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
/*
 * @brief       Enable the DMA
 *
 * @param [in]  hDevice  SPORT device handle.
 *
 */
static ADI_SPORT_RESULT EnableDma(ADI_SPORT_HANDLE hDevice,bool_t bEnable)
{
    ADI_SPORT_DEVICE       *pDevice = (ADI_SPORT_DEVICE  *)hDevice;
    if(adi_dma_Enable(pDevice->hDma,bEnable)!=ADI_DMA_SUCCESS)
    {
        return(ADI_SPORT_DMA_FAILED);
    }
    return(ADI_SPORT_SUCCESS);
}
#endif
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 0)

/**
 * @brief       Clears pending interrupt status.
 *
 * @details     This API is usually not required to called. This can be useful
 *              in a scenario when a device is disabled and enabled back. To
 *              make sure that there no pending interrupts when the device was
 *              previously enabled, the the pending interrupt status can be
 *              cleared.
 *
 * @param[in]   SID             The system interrupt ID.
 *
 * @return      Status
 *              - #ADI_SPORT_SUCCESS if successfully cleared the pending interrupt
 *                 status.
 *              - #ADI_SPORT_FAILED [D] if the SID is not valid.
 */

static ADI_SPORT_RESULT adi_sport_ClearInterruptPendStatus(uint32_t SID)
{

 #ifdef ADI_DEBUG

     /* Check if the given SID is valid */
     if( SID > (uint32_t)127)
     {
         return ADI_SPORT_FAILED;
     }
 #endif /* ADI_DEBUG */
     NVIC_ClearPendingIRQ((IRQn_Type)SID);
     return ADI_SPORT_SUCCESS;
}
#endif

#ifdef ADI_DEBUG
static ADI_SPORT_RESULT ValidateHandle( ADI_SPORT_DEVICE *pInDevice)
{
    /* Return code */
    ADI_SPORT_RESULT    nResult = ADI_SPORT_INVALID_HANDLE;
    /* Pointer to SPORT device instance */
    ADI_SPORT_DEVICE_INFO *poDeviceInfo = &sport_device_info[0][0];
    uint32_t i;
    for(i=0U; i< (ADI_SPORT_NUM_INSTANCES *2U); i++)
    {
        if(pInDevice == poDeviceInfo->hDevice)
        {
            return(ADI_SPORT_SUCCESS);
        }
        poDeviceInfo++;
    }
    return (nResult);
}

#endif /* ADI_DEBUG */

ADI_INT_HANDLER(SPORT0A_Int_Handler)
{
     ISR_PROLOG();
     SPORTCommonStatusHandler(sport_device_info[0][0].hDevice);
     ISR_EPILOG();
}
ADI_INT_HANDLER(SPORT0B_Int_Handler)
{
     ISR_PROLOG();
     SPORTCommonStatusHandler(sport_device_info[0][1].hDevice);
     ISR_EPILOG();
}
/*! \endcond */

#endif /* _ADI_SPORT_V1_C_ */

/*@}*/
