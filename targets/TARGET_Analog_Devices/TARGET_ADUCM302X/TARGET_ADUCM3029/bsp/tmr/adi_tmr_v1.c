/*!
 *****************************************************************************
 * @file:    adi_tmr_v1.c
 * @brief:   GP Timer driver  implementation
 * @version: $Revision$
 * @date:    $Date$
 *-----------------------------------------------------------------------------
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

#ifndef _ADI_TMR_V1_C_
#define _ADI_TMR_V1_C_

/*! \addtogroup GP_Timer_Service
 *  @{
 */

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include <services/tmr/adi_tmr.h>
#include <ssdd_common/common_def.h>
#include "adi_tmr_def_v1.h"

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
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050
#endif /* __ICCARM__ */

#include "adi_tmr_data_v1.c"

#ifdef ADI_DEBUG
ADI_TMR_RESULT ValidateTmrHandle(ADI_TMR_HANDLE hDevice);
#endif

ADI_TMR_RESULT  adi_tmr_ClearEventStatus(
		ADI_TMR_HANDLE hDevice,
		ADI_TMR_EVENT eEventID
		);
static void CommonIntHandler(ADI_TMR_DEVICE *pDevice);


#ifdef ADI_DEBUG
/*
 *   @brief    To Validate the device handle.

 *    @param[in]  hDevice   The handle of the timer device.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if the device handle is valid
 *               - #ADI_TMR_INVALID_HANDLE if device handle is invalid
 *
 *
 */

ADI_TMR_RESULT ValidateTmrHandle(ADI_TMR_HANDLE hDevice)
{
  uint32_t i;

  for(i=0u;i<ADI_TMR_NUM_INSTANCE;i++)
  {
    if(aTimerDeviceInfo[i].hDevice == hDevice)
    {
      return(ADI_TMR_SUCCESS);
    }
  }
  return(  ADI_TMR_INVALID_HANDLE);
}
#endif

 /*!
 * @brief       The adi_tmr_Open API opens a specified timer device.
 *
 * @param[in]   DeviceNumber    The timer device instance number to be opened.
 * @param[in]   pDeviceMemory   The pointer to the timer memory passed by application.
 * @param[in]   MemorySize      The memory size passed by application.
 * @param[out]  phDevice        The pointer to a location where the handle to the opened device is written.
 *
 * @return      Status
 *              - #ADI_TMR_SUCCESS                The device is successfully opened for the given instance.
 *              - #ADI_TMR_FAILURE                 A generalised API failure.
 *              - #ADI_TMR_IN_USE                 The specified device has already been initialised.
 *              - #ADI_TMR_INVALID_TIMERID [D]    The given device number is greater than the number of timer devices.
 *              - #ADI_TMR_INVALID_HANDLE [D]     The pointer to the timer device handle is NULL.
 *
 * Opens a specified timer device and passed back to the application a handle.  It initialises the device's
 * default data parameters.  The Open API also specifies an optional callback function and callback parameter.
 *
 * @sa          adi_tmr_Close().
 */
ADI_TMR_RESULT adi_tmr_Open (
               uint32_t         DeviceNumber,
	           void            *pDeviceMemory,
               uint32_t         MemorySize,
               ADI_TMR_HANDLE  *phDevice
               )
{
    /* assume a return code */
    ADI_TMR_RESULT Result = ADI_TMR_SUCCESS                 ;

    /* Timer Device */
    ADI_TMR_DEVICE    *pDevice =pDeviceMemory;

    /* debug build only */
#ifdef ADI_DEBUG
    /* check if device ID is invalid */
    if (DeviceNumber >= ADI_TMR_NUM_INSTANCE)
    {
        /* Bad Device Number */
        return(ADI_TMR_INVALID_TIMERID);
    }

    /* Check if the given pointer to device handle is valid */
    if(phDevice == NULL)
    {
        return (ADI_TMR_INVALID_HANDLE);
    }
    /* check if device is not in use) */
    if (aTimerDeviceInfo[DeviceNumber].hDevice != NULL)
    {
        return ADI_TMR_IN_USE;
    }
    /* check allocated memory */
    if(MemorySize < (uint32_t)ADI_TMR_MEMORY_SIZE)
    {
    	return ADI_TMR_INSUFFICIENT_MEMORY;
    }
#endif
    /* Clear the given memory */
    memset(pDeviceMemory, 0, sizeof(MemorySize));

    /* get timer device instance */
    pDevice->pTmrRegs = aTimerDeviceInfo[DeviceNumber].pTmrRegs;
    pDevice->pDeviceInfo = &aTimerDeviceInfo[DeviceNumber];
    /* check memory macro */
    assert(ADI_TMR_MEMORY_SIZE == sizeof(ADI_TMR_DEVICE));

    /* save parameters for device instance    */
    pDevice->pfCallback = NULL;

    pDevice->pCBParam   = NULL;

    /* return the device handle to the application */
    *phDevice = (ADI_TMR_HANDLE)pDevice;
#if (ADI_TIMER_ENABLE_STATIC_CONFIG_SUPPORT ==1)
    pDevice->pTmrRegs->LOAD      = aTimerConfig[DeviceNumber].LOAD;
    pDevice->pTmrRegs->CTL       = aTimerConfig[DeviceNumber].CTL;
    pDevice->pTmrRegs->ALOAD     = aTimerConfig[DeviceNumber].ALOAD;
    pDevice->pTmrRegs->PWMCTL    = aTimerConfig[DeviceNumber].PWMCTL;
    pDevice->pTmrRegs->PWMMATCH  = aTimerConfig[DeviceNumber].PWMMATCH;
#endif
    ADI_ENABLE_INT(pDevice->pDeviceInfo->eIRQn);
    ADI_INSTALL_HANDLER(pDevice->pDeviceInfo->eIRQn,pDevice->pDeviceInfo->pIntHndler);
    aTimerDeviceInfo[DeviceNumber].hDevice = pDevice;
    /* return */
    return (Result);
}
 /*!
 * @brief       The adi_tmr_Close API closes a specified timer device.
 *
 * @param[in]   hDevice     The handle of the timer device.
 *
 * @return      Status
 *              - #ADI_TMR_SUCCESS          The device is successfully opened for the given instance.
 *              - #ADI_TMR_INVALID_HANDLE   An invalid device handle.
 *
 * Closes a specified timer device.
 *
 * @sa          adi_tmr_Open().
 */
ADI_TMR_RESULT  adi_tmr_Close(ADI_TMR_HANDLE hDevice)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

/* debug build only */
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    /* disable the timer before closing the device */
    pDevice->pTmrRegs->CTL &= (uint16_t)~BITM_TMR_CTL_EN;
    ADI_DISABLE_INT(pDevice->pDeviceInfo->eIRQn);
    /* free device memory */
    pDevice->pDeviceInfo->hDevice = NULL;
    /* return */
    return (ADI_TMR_SUCCESS);
}

/*!
 *   @brief    Clear the timer's Event Status
 *   @details  The timer event interrupt status is sticky. This function  will clear the
 *             capture event interrupt.
 *
 *
 *    @param[in]  hDevice      The handle of the timer device.
 *    @param[in]  eEventID     Specify which event interrupt status need to be cleared.
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *   @sa adi_tmr_IsCaptureEventPending
 *   @sa adi_tmr_IsTimeoutEventPending
 */

ADI_TMR_RESULT  adi_tmr_ClearEventStatus(
		ADI_TMR_HANDLE hDevice,
		ADI_TMR_EVENT eEventID
		)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    pDevice->pTmrRegs->CLRINT |=eEventID;
    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    Clear the timer's Event Status
 *   @details  The timer event interrupt status is sticky. This function  will clear the
 *             capture event interrupt.
 *
 *
 *    @param[in]  hDevice         The handle of the timer device.
 *    @param[out] pCapturedValue  Pointer to a location where the timer capture value need to be stored.
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 */
ADI_TMR_RESULT  adi_tmr_GetCapturedValue(
		ADI_TMR_HANDLE hDevice,
		uint16_t *pCapturedValue
		)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    *pCapturedValue = pDevice->pTmrRegs->CAPTURE;

     return(ADI_TMR_SUCCESS);
}
/*!
 *   @brief    To know  whether timer device is busy OR can be configured.
 *
 *    @param[in]  hDevice      The handle of the timer device.
 *    @param[out] pbBusy       Pointer to a boolean variable. Contains "True" if  the timer is busy
 *                             "false" if the device is not busy and can configure the timer device.
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 */
ADI_TMR_RESULT adi_tmr_IsBusy(ADI_TMR_HANDLE hDevice,bool_t *pbBusy)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

    *pbBusy = false;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    if((pDevice->pTmrRegs->STAT & BITM_TMR_STAT_BUSY) != 0u)
    {
       *pbBusy = true;
    }
    return(ADI_TMR_SUCCESS);
}
/**
 * @brief       Registers a Callback function with the timer device driver. The registered call
 *              back function will be called when an event is detected.
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
 *  - #ADI_TMR_SUCCESS                    Successfully registered specified callback function.
 *
 *  - #ADI_TMR_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *
 */
ADI_TMR_RESULT adi_tmr_RegisterCallback(
               ADI_TMR_HANDLE const  hDevice,
               ADI_CALLBACK   const   pfCallback,
               void        *const     pCBparam
               )

{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif
    /* Store the address of the callback function */
    pDevice->pfCallback =  pfCallback;
    /* Store the call back parameter */
    pDevice->pCBParam    =  pCBparam;
    /* Return success */
    return(ADI_TMR_SUCCESS);
}


/*!
 *   @brief    Set the Timer's 16-bit load value.
 *
 *    @param[in]  hDevice       The handle of the timer device.
 *    @param[in]  nLoadValue    Value to be written to the timer's load  register
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 */
ADI_TMR_RESULT adi_tmr_SetLoadValue(ADI_TMR_HANDLE hDevice, const uint16_t nLoadValue)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif
    pDevice->pTmrRegs->LOAD = nLoadValue;

    return(ADI_TMR_SUCCESS);
}
/*!
 *   @brief    Get the Timer's 16-bit load value.
 *
 *    @param[in]  hDevice       The handle of the timer device.
 *    @param[out]  pTimerLdVal   Pointer  to a location where the timer  load value need to be stored.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 */
ADI_TMR_RESULT adi_tmr_GetLoadValue(ADI_TMR_HANDLE hDevice, uint16_t *pTimerLdVal)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    *pTimerLdVal = pDevice->pTmrRegs->LOAD;

    return(ADI_TMR_SUCCESS);

}
/*!
 *   @brief    Get the Timer's current value.
 *
 *    @param[in]  hDevice       The handle of the timer device.
 *    @param[out]  pTimerValue   Pointer  to a location where the timer  current Up/Down value need to be stored.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 */
ADI_TMR_RESULT adi_GPT_GetCurrentValue(ADI_TMR_HANDLE hDevice, uint16_t *pTimerValue)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }

#endif
    *pTimerValue = pDevice->pTmrRegs->CURCNT;

    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    To Enable the event capture by the timer device.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  bEnable   To enable/disable the Event capture..
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 * @sa adi_tmr_SetEventToCapture
 */
ADI_TMR_RESULT adi_tmr_EnableEventCapture(ADI_TMR_HANDLE hDevice, bool_t bEnable)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_EVTEN;
    if(true == bEnable)
    {
        nTemp = nTemp | BITM_TMR_CTL_EVTEN;
    }
	pDevice->pTmrRegs->CTL = nTemp;
    return(ADI_TMR_SUCCESS);

}


/*!
 *   @brief    To enable the reloading for the timer device. This options should be used only in periodic
 *             mode of operation.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  bEnable   To enable/disable the reloading option.
 *    \n
 *    \n                      True: To enable the reloading.
 *    \b                      false: To Disable the reloading
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *
 */
ADI_TMR_RESULT adi_tmr_EnableReloading(ADI_TMR_HANDLE hDevice, bool_t bEnable)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif

    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_RLD;

    if(true == bEnable)
    {
        nTemp  = nTemp | BITM_TMR_CTL_RLD;
    }
	pDevice->pTmrRegs->CTL = nTemp;
    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    To set prescaler value for the clock scaling.
 *
 *    @param[in]  hDevice    The handle of the timer device.
 *    @param[in]  ePreScaler Prescaler emum value for clock scaling.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa adi_tmr_SetEventToCapture
 */

ADI_TMR_RESULT adi_tmr_SetPrescaler(ADI_TMR_HANDLE hDevice, ADI_TMR_PRESCALER ePreScaler)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif
    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_PRE;

    pDevice->pTmrRegs->CTL = nTemp | ePreScaler;

    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    Timer Set Event to Capture.
 *              Each timer can be configured to capture one of various events.
 *
 *    @param[in]  hDevice    The handle of the timer device.
 *    @param[in]  eEvent      Event to be captured.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa adi_tmr_EnableEventCapture
 */

ADI_TMR_RESULT adi_tmr_SetEventToCapture(ADI_TMR_HANDLE hDevice, ADI_TMR_CAPTURE_EVENTS eEvent)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }

#endif
    ADI_ENTER_CRITICAL_REGION();
    
    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_EVTRANGE;

    pDevice->pTmrRegs->CTL = nTemp | (uint16_t)eEvent;
    
    ADI_EXIT_CRITICAL_REGION();
    return(ADI_TMR_SUCCESS);
}


/*!
 *   @brief    To enable the timer device.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  bEnable   Specify whether to enable/disable the timer device.
 *
 *                          true  : Enable the timer.
 *
 *                          false : Disable the timer.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa adi_tmr_SetEventToCapture
 */
ADI_TMR_RESULT adi_tmr_Enable(ADI_TMR_HANDLE hDevice, bool_t bEnable)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    ADI_ENTER_CRITICAL_REGION();
    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_EN;
    if(true == bEnable)
    {
      nTemp  = nTemp | BITM_TMR_CTL_EN;
    }
    pDevice->pTmrRegs->CTL = nTemp;
    ADI_EXIT_CRITICAL_REGION();
    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    To configure the device in the periodic/free-running mode.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  eMode     Specify whether the device should operate in "Free running mode" or
 *                          periodic mode.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa adi_tmr_close
 */
ADI_TMR_RESULT adi_tmr_SetRunMode(ADI_TMR_HANDLE hDevice, ADI_TMR_RUNNING_MODE eMode)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_MODE;
    pDevice->pTmrRegs->CTL = nTemp | eMode;
    return(ADI_TMR_SUCCESS);
}
/*!
 *   @brief    To configure the device to count-up/count-down.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  eMode     Specify whether timer device should operate in  count-up/count-down mode.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa adi_tmr_SetEventToCapture
 */
ADI_TMR_RESULT adi_tmr_SetCountMode(ADI_TMR_HANDLE hDevice, ADI_TMR_COUNT_MODE eMode)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_UP;
    pDevice->pTmrRegs->CTL = nTemp | eMode;
    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    To configure the input clock source for the timer device..
 *
 *    @param[in]  hDevice       The handle of the timer device.
 *    @param[in]  eClockSource  Specify the input clock  source for the timer device.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa
 */

ADI_TMR_RESULT adi_tmr_SetClockSource(ADI_TMR_HANDLE hDevice, ADI_TMR_CLOCK_SOURCE eClockSource)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp = pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_CLK;
    pDevice->pTmrRegs->CTL = nTemp | eClockSource;
    return(ADI_TMR_SUCCESS);

}
/*!
 *   @brief    To enable  the Synchronization bypass
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  bEnable   Specify whether to enable/disable the Synchronization bypass.
 *
 *                          true  : To enable the Synchronization bypass.
 *
 *                          false : To disable the Synchronization bypass.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa
 */
ADI_TMR_RESULT adi_tmr_EnableSyncBypass (ADI_TMR_HANDLE hDevice, bool_t bEnable)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp =  pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_SYNCBYP;

    if(true == bEnable)
    {
        nTemp = nTemp | BITM_TMR_CTL_SYNCBYP;
    }
    pDevice->pTmrRegs->CTL = nTemp;
    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    To enable/disable prescale reset.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  bEnable   Specify whether to enable/disable prescale reset.
 *
 *                          true  : To enable prescale reset.
 *
 *                          false : To disable prescale reset
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa
 */
ADI_TMR_RESULT adi_tmr_EnablePrescaleReset (ADI_TMR_HANDLE hDevice, bool_t bEnable)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp =  pDevice->pTmrRegs->CTL & (uint16_t)~BITM_TMR_CTL_RSTEN;

    if(true == bEnable)
    {
        nTemp = nTemp | BITM_TMR_CTL_RSTEN;
    }
    pDevice->pTmrRegs->CTL = nTemp;
    return(ADI_TMR_SUCCESS);
}
/*!
 *   @brief    To place the PWM output HIGH/LOW during the idle.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  bIdleHigh   Specify whether to place the PWM output HIGH/LOW during the idle.
 *
 *                          true  : HIGH state during the idle.
 *
 *                          false : LOW state during the idle.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa
 */
ADI_TMR_RESULT adi_tmr_SetPWMidle (ADI_TMR_HANDLE hDevice, bool_t bIdleHigh)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp = pDevice->pTmrRegs->PWMCTL & (uint16_t)~BITM_TMR_PWMCTL_IDLESTATE;

    if(true == bIdleHigh)
    {
        nTemp = nTemp | BITM_TMR_PWMCTL_IDLESTATE;
    }
    pDevice->pTmrRegs->PWMCTL = nTemp;
    return(ADI_TMR_SUCCESS);
}
/*!
 *   @brief    To place the PWM output HIGH/LOW during the idle.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  eMode     Specify the PWM mode of operation.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa adi_tmr_close
 */
ADI_TMR_RESULT adi_tmr_SetPWMMode (ADI_TMR_HANDLE hDevice, ADI_TMR_PWM_MODE  eMode)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {

	    return ADI_TMR_INVALID_HANDLE;
    }
    if((pDevice->pTmrRegs->CTL & BITM_TMR_CTL_EN)== BITM_TMR_CTL_EN)
    {
       return(ADI_TMR_OPERATION_NOT_ALLOWED);
    }

#endif

    nTemp = pDevice->pTmrRegs->PWMCTL & (uint16_t)~BITM_TMR_PWMCTL_MATCH;
    pDevice->pTmrRegs->PWMCTL = nTemp | eMode;
    return(ADI_TMR_SUCCESS);
}

/*!
 *   @brief    To set PWM match value for the device.
 *
 *   @details  The value is used when the PWM is operating in match mode. The PWM output is asserted when
 *             the Up/Down counter is equal to this match value. PWM output is deasserted again when a
 *             timeout event occurs.
 *
 *    @param[in]  hDevice   The handle of the timer device.
 *    @param[in]  nValue     Specify the PWM mode of operation.
 *
 *    @return Status
 *               - #ADI_TMR_SUCCESS if successful
 *               - #ADI_TMR_INVALID_HANDLE [D] if device handle is invalid
 *
 *  @sa
 */

ADI_TMR_RESULT adi_tmr_SetPWMMatchValue (ADI_TMR_HANDLE hDevice, uint16_t  nValue)
{
    /* Pointer to device instance */
    ADI_TMR_DEVICE *pDevice = (ADI_TMR_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    /* validate device handle */
    if(ValidateTmrHandle(hDevice) != ADI_TMR_SUCCESS)
    {
        return ADI_TMR_INVALID_HANDLE;
    }
#endif
    pDevice->pTmrRegs->PWMMATCH = nValue;

    return(ADI_TMR_SUCCESS);
}

/*! \cond PRIVATE */
/* Common interrupt handler, which notifies events to the application  */
static void CommonIntHandler(ADI_TMR_DEVICE *pDevice)
{
   /* Status register value */
   uint16_t     IntStatus = pDevice->pTmrRegs->STAT;

    /* if a callback is installed notify the events */
   if(pDevice->pfCallback)
   {
       /* Check if timeout occured and notify of timeout event */
       if((IntStatus & BITM_TMR_STAT_TIMEOUT) != 0u)
       {
          pDevice->pfCallback(pDevice->pCBParam, ADI_TMR_EVENT_TIMEOUT, NULL);
       }

       /* Check if event is captured and notify of captured event */
       if((IntStatus & BITM_TMR_STAT_CAPTURE) != 0u)
       {
          pDevice->pfCallback(pDevice->pCBParam, ADI_TMR_EVENT_CAPTURED, NULL);
       }
   }
   pDevice->pTmrRegs->CLRINT = (BITM_TMR_CLRINT_EVTCAPT|BITM_TMR_CLRINT_TIMEOUT);
   return;
}
/*!
    @brief GP Timer0 ISR.
           The Timer ISR simply clears the ISR

*/
ADI_INT_HANDLER(GP_Tmr0_Int_Handler)
{
   /* Call the common interrupt handler */
    ISR_PROLOG();   
    CommonIntHandler(aTimerDeviceInfo[0].hDevice);
    ISR_EPILOG();   
}


/*
    @brief GP Timer1 ISR.
           The Timer ISR simply clears the ISR

*/
ADI_INT_HANDLER(GP_Tmr1_Int_Handler)
{
   /* Call the common interrupt handler */
   ISR_PROLOG();   
   CommonIntHandler(aTimerDeviceInfo[1].hDevice);
   ISR_EPILOG();   
}

/*!
    @brief GP Timer2 ISR.
           The Timer ISR simply clears the ISR
*/
ADI_INT_HANDLER(GP_Tmr2_Int_Handler)
{
   /* Call the common interrupt handler */
   ISR_PROLOG();   
   CommonIntHandler(aTimerDeviceInfo[2].hDevice);
   ISR_EPILOG();  
}

/*! \endcond */
/*
** EOF
*/

/*! @} */

#endif /* _ADI_TMR_V1_C_ */
