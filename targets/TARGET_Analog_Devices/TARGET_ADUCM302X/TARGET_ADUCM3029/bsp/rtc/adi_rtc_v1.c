/*!
 *****************************************************************************
 * @file:    adi_rtc_v1.c
 * @brief:   Real-Time Clock Device Implementations.
 * @version: $Revision: 35155 $
 * @date:    $Date: 2016-07-26 18:09:22 +0100 (Tue, 26 Jul 2016) $
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
/*! \addtogroup RTC_Service RTC Service
 *  @{
 */

#ifndef _ADI_RTC_V1_C_
/*! \cond PRIVATE */
#define _ADI_RTC_V1_C_

#if defined ( __GNUC__ )
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

#include <stdlib.h>  /* for 'NULL" definition */
#include <assert.h>
#include <string.h>

#include <ssdd_common/common_def.h>
#include <services/rtc/adi_rtc.h>

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
* Pm109 (rule 20.12): the time handling functions of library <time.h> shall not be used
* Pm150 (rule 20.2): the names of standard library macros, objects and function shall not be reused
*   Needed to implement the <time.h> functions here.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm109,Pm150,Pm140
#endif /* __ICCARM__ */

#ifdef ADI_RTC_USE_IAR_SYSTEM_TIME_HOOKS
#include <time.h>
#include <yfuns.h>
#endif

/*! Interrupt Mask */
#define ADI_RTC_INTERRUPT_OFFSET_MASK 0XFFFFu

/* Interrupt Mask */
#define ADI_RTC_INTERRUPT_INPUT_CHANNEL_MASK 0X74000000u

/* Interrupt Mask */
#define ADI_RTC_INTERRUPT_OUTPUT_CHANNEL_MASK 0X02000000u

/* Interrupt Mask */
#define ADI_RTC_INTERRUPT_CR1_MASK 0X001F0000u

#include "adi_rtc_data_v1.c"

#ifdef ADI_DEBUG
static ADI_RTC_RESULT ValidateHandle( ADI_RTC_DEVICE *pInDevice)
{
    /* Return code */
    ADI_RTC_RESULT    nResult = ADI_RTC_INVALID_HANDLE;
    uint32_t i;
    for(i = 0u; i < ADI_RTC_NUM_INSTANCE; i++)
    {
        if(aRTCDeviceInfo[i].hDevice == pInDevice)
        {
            return(ADI_RTC_SUCCESS);
        }
    }
    return (nResult);
}
#endif
/*! \endcond */

/*!
    @brief RTC Initialization

 * @param[in]   DeviceNumber    The RTC device instance number to be opened.
 * @param[in]   pDeviceMemory   The pointer to the device  memory passed by application.
 * @param[in]   MemorySize      The memory size passed by application.
 * @param[out]  phDevice        The pointer to a location where the handle to the opened RTC device is written.
    @return    Status
               - #ADI_RTC_SUCCESS                      Success: RTC device driver initialized successfully.
               - #ADI_RTC_INVALID_INSTANCE [D]          The instance number is invalid.

            The RTC contoller interrupt enable state is unaltered during driver initialization.
            Use the #adi_rtc_EnableInterrupts API to manage interrupting.

    @note   The contents of phDevice will be set to NULL upon failure.\n\n

    @note   On #ADI_RTC_SUCCESS the RTC device driver is initialized and made ready for use,
            though pending interrupts may be latched.  During initialization, the content of the
            various RTC control, count, alarm and status registers are untouched to preserve prior
            RTC initializations and operation.  The core NVIC RTC interrupt is enabled.\n\n

    @note   The RTC device driver may initialize with pending interrupt statu which may require
            processing at the application level before using the RTC.  The application should
            the #adi_rtc_GetInterruptStatus() API to query the RTC interrupt status detail and
            reconcile any pending interrupts before using the RTC.  After processing the pending
            interrupts, the application should explicitly clear the corresponding interrupt status
            bits(s) with the #adi_rtc_ClearInterruptStatus() API.\n\n

    @note   SAFE WRITES: The "safe write" mode is enabled by default and can be changed using the switch
                         "ADI_RTC_CFG_ENABLE_SAFE_WRITE" defined in adi_rtc_config.h file.

    @sa    adi_rtc_Enable().
    @sa    adi_rtc_EnableInterrupts().
    @sa    adi_rtc_GetInterruptStatus().
    @sa    adi_rtc_SetCount().
    @sa    adi_rtc_ClearInterruptStatus().
    @sa    adi_rtc_Close()
*/
ADI_RTC_RESULT adi_rtc_Open(
               uint32_t         DeviceNumber,
	       void            *pDeviceMemory,
               uint32_t         MemorySize,
               ADI_RTC_HANDLE  *phDevice
               )
{
    ADI_RTC_DEVICE *pDevice = pDeviceMemory;

    /* store a bad handle in case of failure */
    *phDevice = (ADI_RTC_HANDLE) NULL;

#ifdef ADI_DEBUG
    if ( DeviceNumber >= ADI_RTC_NUM_INSTANCE)
    {
        return ADI_RTC_INVALID_INSTANCE;
    }
    assert(ADI_RTC_MEMORY_SIZE == sizeof(ADI_RTC_DEVICE));
    if (aRTCDeviceInfo[DeviceNumber].hDevice != NULL)
    {
        return ADI_RTC_IN_USE;
    }
    if(MemorySize < ADI_RTC_MEMORY_SIZE)
    {
      return(ADI_RTC_FAILURE);
    }
#endif

    memset(pDeviceMemory,0,MemorySize);
    /* initialize device data entries */
    pDevice->pRTCRegs    = aRTCDeviceInfo[DeviceNumber].pRTCRegs;
    
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

    pDevice->pRTCRegs->CR0 = 0u;
    pDevice->pRTCRegs->CR1 = 0u;
    
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    

    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDSR0)

    pDevice->pRTCRegs->SR0 = ADI_RTC_INT_SOURCE_MASK;
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCSR0)    
    
    pDevice->pRTCRegs->CNT0 = 0u;
    pDevice->pRTCRegs->CNT1 = 0u;
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCNT0)    
    
    switch(DeviceNumber)
    {
        case 0:
            ADI_INSTALL_HANDLER(aRTCDeviceInfo[DeviceNumber].eIRQn,RTC0_Int_Handler);
            break;
        case 1:
            ADI_INSTALL_HANDLER(aRTCDeviceInfo[DeviceNumber].eIRQn,RTC1_Int_Handler);
            break;
        default:
            return(ADI_RTC_INVALID_INSTANCE);
    }
     
    /* local pointer to instance data */
    aRTCDeviceInfo[DeviceNumber].hDevice = pDevice;
    pDevice->pDeviceInfo = &aRTCDeviceInfo[DeviceNumber];
    
#if (ADI_RTC_STATIC_CONFIG_SUPPORT_ENABLE ==1)
    rtc_init(pDevice,&aRTCConfig[DeviceNumber]);
#else
    pDevice->pRTCRegs->CR2IC = 0u;
    pDevice->pRTCRegs->CR3OC = 0u;
    pDevice->pRTCRegs->CR4OC = 0u;
    pDevice->pRTCRegs->SR2   = 0u;
    pDevice->pRTCRegs->CR3OC = 0u;
    pDevice->pRTCRegs->CR4OC = 0u;
#endif   
    /* store handle at application handle pointer */
    *phDevice = pDevice;
     /* clear the qualified interrupt(s) */
    adi_rtc_ClearInterruptStatus(pDevice, (ADI_RTC_INT_TYPE)ADI_RTC_INT_SOURCE_MASK);
    pDevice->eIRQn =  aRTCDeviceInfo[DeviceNumber].eIRQn;
    /* Enable RTC interrupts in NVIC */
    ADI_ENABLE_INT(pDevice->eIRQn);

    return ADI_RTC_SUCCESS;            /* initialized */
}


/*!
 * @brief  Uninitialize and deallocate an RTC device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Uninitialize and release an allocated RTC device for other use.  The core NVIC RTC interrupt is disabled.
 *
 * @sa        adi_rtc_Open().
 */
ADI_RTC_RESULT adi_rtc_Close(ADI_RTC_HANDLE const hDevice)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* uninitialize */
    ADI_DISABLE_INT( pDevice->eIRQn);
    ADI_UNINSTALL_HANDLER(pDevice->eIRQn);

    pDevice->pRTCRegs   = NULL;
    pDevice->pfCallback = NULL;
    pDevice->pCBParam   = NULL;
    pDevice->cbWatch    = 0u;
   
    pDevice->pDeviceInfo->hDevice = NULL;
    return ADI_RTC_SUCCESS;
}


/*************************************************************************************************
**************************************************************************************************
****************************************  ENABLE APIS  *******************************************
**************************************************************************************************
*************************************************************************************************/


/*!
 * @brief  Enable RTC alarm.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   bEnable    Boolean Flag to enable/disable  alarm logic.
 *                \n true  :   Enable alarm logic.
 *                \n false :   Disable alarm logic.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]     Invalid device handle parameter.
 *
 * Enable/disable operation of RTC internal alarm logic.
 *
 * Alarm events and interrupt notifications are gated by the alarm logic enablement.
 * RTC alarm interrupts require both RTC device and RTC alarm interrupt enables
 * to have been set.
 *
 * The alarm is relative to some future alarm value match against the RTC counter.
 *
 * @note The RTC device driver does not modify the alarm enable on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_EnableInterrupts().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_GetCount().
 * @sa        adi_rtc_SetAlarm().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_EnableAlarm(ADI_RTC_HANDLE const hDevice, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)
      
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear RTC alarm enable */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR0  |= BITM_RTC_CR0_ALMEN;
    }
    else
    {
        pDevice->pRTCRegs->CR0 &= (uint16_t)(~BITM_RTC_CR0_ALMEN);
    }
    ADI_EXIT_CRITICAL_REGION();
    
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Enable MOD60 RTC alarm.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   bEnable    Boolean Flag for enable/disable mod60  alarm logic.
 *               \n true   :  Enable mod60 alarm logic.
 *               \n false  :  Disable mod60 alarm logic.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Enable/disable operation of RTC internal alarm logic.
 *
 * Alarm events and interrupt notifications are gated by the alarm logic enablement.
 * RTC alarm interrupts require both RTC device and RTC alarm interrupt enables
 * to have been set.
 *
 * The alarm is relative to some future alarm value match against the RTC counter.
 *
 * @note The RTC device driver does not modify the alarm enable on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_EnableInterrupts().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_GetCount().
 * @sa        adi_rtc_SetAlarm().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_EnableMod60Alaram(ADI_RTC_HANDLE const hDevice, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    /* Mod-60 Alaram is present only in RTC-1 */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }

#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

    ADI_ENTER_CRITICAL_REGION();
    /* set/clear RTC alarm enable */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR0  |= BITM_RTC_CR0_MOD60ALMEN;
    }
    else
    {
        pDevice->pRTCRegs->CR0 &= (uint16_t)(~BITM_RTC_CR0_MOD60ALMEN);
    }
    ADI_EXIT_CRITICAL_REGION();
    
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Enable RTC device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   bEnable    Boolean Flag for enabling/disabling  the RTC device.
 *                \n true   :     Enable rtc device.
 *                \n false  :     Disable rtc device.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Global enable/disable of the RTC controller.  Enables counting of elapsed real time and acts
 * as a master enable for the RTC.
 *
 * @note When enabled, the RTC input clock prescaler  and trim interval  are realigned.
 *
 * @note The RTC device driver does not modify the device enable on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_EnableAlaram().
 */

ADI_RTC_RESULT adi_rtc_Enable(ADI_RTC_HANDLE const hDevice, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)
    

    ADI_ENTER_CRITICAL_REGION();
    /* set/clear RTC device enable */
    if (bEnable)
    {
         pDevice->pRTCRegs->CR0 |= BITM_RTC_CR0_CNTEN;
    }
    else
    {
        pDevice->pRTCRegs->CR0 &=(uint16_t)(~BITM_RTC_CR0_CNTEN);
    }
    ADI_EXIT_CRITICAL_REGION();
    
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    
      
    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Manage interrupt enable/disable in the RTC and NVIC controller.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   Interrupts Conveys which interrupts are affected.
 * @param[in]   bEnable      Flag controlling RTC interrupt enablement.
 *               \n true  :  Enable RTC interrupts.
 *               \n false :  Disable RTC interrupts.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Enable/disable RTC interrupt enable bits as well as manage global NVIC enable/disable for the RTC.
 * Input parameter \a Interrupts is a bitfield of type #ADI_RTC_INT_TYPE designating the set of
 * interrupts to be enabled or disabled.  The interrupts parameter may be zero, which will then simply
 * manage the NVIC RTC enable and leave the indivigual RTC interrupt enables unchanges.
 * Input parameter \a bFlag controls whether to enable or disable the designated set of interrupts.
 *
 * @note The RTC device driver does not modify the interrupt enables on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_GetInterruptStatus().
 */
ADI_RTC_RESULT adi_rtc_EnableInterrupts (ADI_RTC_HANDLE const hDevice, ADI_RTC_INT_TYPE Interrupts, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nInterrupts;
    uint16_t cr0, cr1, cr2ic, cr3oc;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    if( (pDevice->pRTCRegs == pADI_RTC0) &&(((uint16_t)((ADI_RTC_MOD60ALM_INT | ADI_RTC_ISO_DONE_INT|
                                             ADI_RTC_COUNT_INT    | ADI_RTC_MOD_1_INT    |
                                             ADI_RTC_TRIM_INT    | ADI_RTC_COUNT_ROLLOVER_INT |
                                             ADI_RTC_MOD60_ROLLOVER_INT
                                             )) & (uint16_t)Interrupts) != 0u))
    {
        return(ADI_RTC_INVALID_PARAM);
    }
#endif
     
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)
    PEND_BEFORE_WRITE(SR2,BITM_RTC_SR2_WPNDCR1MIR)
    
    nInterrupts = (uint32_t)Interrupts;
    cr0 = (uint16_t)nInterrupts;
    cr1 = (uint16_t)((nInterrupts & ADI_RTC_INTERRUPT_CR1_MASK) >> ADI_RTC_INTERRUPT_OFFSET);
    cr2ic = (uint16_t)((nInterrupts & ADI_RTC_INTERRUPT_INPUT_CHANNEL_MASK) >> ADI_RTC_INTERRUPT_OFFSET);
    cr3oc = (uint16_t)((nInterrupts & ADI_RTC_INTERRUPT_OUTPUT_CHANNEL_MASK) >> ADI_RTC_INTERRUPT_OFFSET);
    
    /* set/clear interrupt enable bit(s) in control register */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR0 |= cr0;
        pDevice->pRTCRegs->CR1 |= cr1;
        if(cr2ic != 0u)
        {
            pDevice->pRTCRegs->CR2IC |= cr2ic;
        }
        if(cr3oc != 0u)
        {
            pDevice->pRTCRegs->CR3OC |= cr3oc;
        }
    } else
    {
        pDevice->pRTCRegs->CR0 &= (uint16_t)~cr0;
        pDevice->pRTCRegs->CR1 &= (uint16_t)~cr1;
        if(cr2ic != 0u)
        {
            pDevice->pRTCRegs->CR2IC &= (uint16_t)~cr2ic;
        }
        if(cr3oc != 0u)
        {
            pDevice->pRTCRegs->CR3OC &= (uint16_t)~cr3oc;
        }
    }
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    
    SYNC_AFTER_WRITE(SR2,BITM_RTC_SR2_WSYNCCR1MIR)        
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Enable RTC automatic clock trimming.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   bEnable      Flag controlling RTC trim enablement.
 *                \n true     Enable RTC trimming.
 *                \n false    Disable RTC trimming.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Enable/disable automatic application of trim values to the main RTC clock.  Allows application
 * of periodic real-time RTC clock adjustments to correct for drift.  Trim values are pre-calibrated
 * and stored at manufacture.  Trim values may be recalibrated by monitoring the RTC clock externally
 * and computing/storing new trim values (see #adi_rtc_SetTrim).
 *
 * @note The trim interval is reset with device enable, #adi_rtc_Enable().
 *
 * @note The RTC device driver does not modify the trim enable on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_GetTrim().
 * @sa        adi_rtc_SetTrim().
 */
ADI_RTC_RESULT adi_rtc_EnableTrim (ADI_RTC_HANDLE const hDevice, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear trim enable bit(s) in control register */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR0 |= BITM_RTC_CR0_TRMEN;
    }
    else
    {
        pDevice->pRTCRegs->CR0 &=(uint16_t)(~BITM_RTC_CR0_TRMEN);
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    
    
    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Enable input capture for the specified channel.
 *
 * @param[in]   hDevice       Device handle obtained from adi_rtc_Open().
 * @param[in]   eInpChannel   Specify input compare channel. 
 * @param[in]   bEnable      Flag for enabling  RTC input capture for specified channel.
 *                \n true     Enable input capture.
 *                \n false    Disable input capture.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableInputCapture (ADI_RTC_HANDLE const hDevice,ADI_RTC_INPUT_CHANNEL eInpChannel, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR2IC)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear trim input capture enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR2IC |=(uint16_t)eInpChannel;
    }
    else
    {
        pDevice->pRTCRegs->CR2IC &= (uint16_t)(~(uint16_t)eInpChannel);
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR2IC)    
    
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Enable Overwrite of Unread Snapshots for all RTC Input Capture Channels.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   bEnable      Flag for enabling  overwriting the unread snapshot.
 *                \n true     Enable overwrite snapshot.
 *                \n false    Disable overwrite of snapshot.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableOverwriteSnapshot (ADI_RTC_HANDLE const hDevice, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR2IC)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear trim input capture enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR2IC |= BITM_RTC_CR2IC_RTCICOWUSEN;
    }
    else
    {
        pDevice->pRTCRegs->CR2IC &= (uint16_t)~BITM_RTC_CR2IC_RTCICOWUSEN;
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR2IC)    
    
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Set  input capture polarity  for the specified channel.
 *
 * @param[in]   hDevice        Device handle obtained from adi_rtc_Open().
 * @param[in]   eInpChannel    Specify which input capture  channel.  
 * @param[in]   bEnable        Flag for selecting   RTC input capture polarity.
 *                \n false     channel uses a *high-to-low* transition on its GPIO pin to signal an input capture event
 *                \n true      channel uses a *low-to-high* transition on its GPIO pin to signal an input capture event.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_SetInputCapturePolarity (ADI_RTC_HANDLE const hDevice,ADI_RTC_INPUT_CHANNEL eInpChannel, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nInpChannel = (uint16_t)eInpChannel;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR2IC)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear trim input capture enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR2IC |= (uint16_t)(nInpChannel << BITP_RTC_CR2IC_RTCIC0LH);
    }
    else
    {
        pDevice->pRTCRegs->CR2IC &= (uint16_t)~(nInpChannel << BITP_RTC_CR2IC_RTCIC0LH);
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR2IC)    
    
    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Enable output compare for the specified output channel.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eOutChannel   Specify which output compare channel.
 * @param[in]   bEnable       Flag for enabling  RTC output compare for specified channel.
 *                \n true     Enable output compare.
 *                \n false    Disable output compare.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableOutputCompare (ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eOutChannel, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR3OC)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear output compare  enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR3OC |=(uint16_t)eOutChannel;
    }
    else
    {
        pDevice->pRTCRegs->CR3OC &= (uint16_t)(~(uint16_t)eOutChannel);
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR3OC)    
    
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Enable auto reload for output compare channel-1.
 *
 * @param[in]   hDevice        Device handle obtained from adi_rtc_Open().
 * @param[in]   eOutChannel    Specify output compare channel.
 * @param[in]   bEnable        Flag for enabling  auto reload for output compare for channel-1.
 *                \n true      Enable output compare.
 *                \n false     Disable output compare.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableAutoReload(ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eOutChannel, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR4OC)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear auto reload enable options */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR4OC |= BITM_RTC_CR4OC_RTCOC1ARLEN;
    }
    else
    {
        pDevice->pRTCRegs->CR4OC &= (uint16_t)~BITM_RTC_CR4OC_RTCOC1ARLEN;
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR4OC)    
    
    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set auto reload value for  output compare channel-1.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eOutChannel Output channel for which auto reload to be set. 
 * @param[in]   nValue     Auto reload value to be set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 * 
 *
 */
ADI_RTC_RESULT adi_rtc_SetAutoReloadValue(ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eOutChannel, uint16_t nValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDOC1ARL)
    
    pDevice->pRTCRegs->OC1ARL =nValue;
    /* Wait till  write to Control Register to take effect */    
   SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCOC1ARL)    
    
    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Enable output channel mask for specified channel.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eOutChannel    Specify output compare channel. 
 * @param[in]   bEnable      Flag for enabling  masking  for output compare for channel-1.
 *                \n true     Enable output channel mask .
 *                \n false    Disable output channel mask.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableOutputChannelMask(ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eOutChannel, bool_t bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR4OC)
    
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear auto reload enable options */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR4OC |= (uint16_t)eOutChannel;
    }
    else
    {
        pDevice->pRTCRegs->CR4OC &= (uint16_t)~(uint16_t)eOutChannel;
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR4OC)    
    
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  To Set output channel mask for output compare channel.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eOutChannel  Output channel for which auto reload to be set. 
 * @param[in]   nMask        Channel Mask to be set for output compare channel.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 * 
 *
 */
ADI_RTC_RESULT adi_rtc_SetOutputChannelMask(ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eOutChannel, uint8_t nMask)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDOCMSK)
    
    pDevice->pRTCRegs->OCMSK = (uint16_t)nMask & 0xFu;
    /* Wait till  write to Control Register to take effect */    
   SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCOCMSK)    
    
    return ADI_RTC_SUCCESS;
}

/*************************************************************************************************
**************************************************************************************************
******************************************   GET APIS   ******************************************
**************************************************************************************************
*************************************************************************************************/


/*!
 * @brief  Get current RTC alarm value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pAlarm     Pointer to application memory where the alarm value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the currently programmed 32-bit RTC alarm value and write it to the address provided by parameter \a pAlarm.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_SetAlarm().
 */
ADI_RTC_RESULT adi_rtc_GetAlarm (ADI_RTC_HANDLE hDevice, uint32_t *pAlarm)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nAlaram;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDALM0|BITM_RTC_SR1_WPNDALM1))

    /* disable interrupts during paired read */
    ADI_DISABLE_INT(pDevice->eIRQn);
        nAlaram  =(uint32_t) pDevice->pRTCRegs->ALM1 << 16u;
        nAlaram |= (uint32_t)pDevice->pRTCRegs->ALM0;
    ADI_ENABLE_INT(pDevice->eIRQn);

    *pAlarm = nAlaram;

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get current RTC alarm value  with fractional part also.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pAlarm     Pointer to application memory where the alarm value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the currently programmed 32-bit RTC alarm value and write it to the address provided by parameter \a pAlarm.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_SetAlarm().
 */
ADI_RTC_RESULT adi_rtc_GetAlarmEx (ADI_RTC_HANDLE hDevice, float32_t *pAlarm)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nAlaram,nTemp;
    uint16_t nPreScale;
    float32_t fFraction;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDALM0|BITM_RTC_SR1_WPNDALM1))
    nPreScale = (pDevice->pRTCRegs->CR1&BITM_RTC_CR1_PRESCALE2EXP)>>BITP_RTC_CR1_PRESCALE2EXP;
    /* disable interrupts during paired read */
    ADI_DISABLE_INT(pDevice->eIRQn);
    nAlaram  = (uint32_t)pDevice->pRTCRegs->ALM1 << 16u;
    nAlaram |= (uint32_t)pDevice->pRTCRegs->ALM0;
    ADI_ENABLE_INT(pDevice->eIRQn);
    nTemp = 1lu<<nPreScale;
    fFraction = (float32_t)pDevice->pRTCRegs->ALM2 /(float32_t)(nTemp);

    *pAlarm = (float32_t)nAlaram+fFraction;

    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Get current RTC control register value.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eRegister    Specify which register content need to be returned.
 *
 * @param[out]  pControl   Pointer to application memory where the control register value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the currently programmed 16-bit RTC control register value and write it to the address provided by parameter \a pControl.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtcSetControl().
 */
ADI_RTC_RESULT adi_rtc_GetControl (ADI_RTC_HANDLE hDevice, ADI_RTC_CONTROL_REGISTER eRegister ,uint32_t *pControl)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

     switch(eRegister)
     {
     case ADI_RTC_CONTROL_REGISTER_0:
         *pControl = pDevice->pRTCRegs->CR0;
         break;
     case ADI_RTC_CONTROL_REGISTER_1:
         *pControl = pDevice->pRTCRegs->CR1;
         break;
     default:
         return(ADI_RTC_FAILURE);
     }
    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Get current RTC count value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pCount     Pointer to application memory where the count value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the current 32-bit RTC count value and write it to the address provided by parameter \a pCount.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetCount(ADI_RTC_HANDLE const hDevice, uint32_t *pCount)
{
    uint32_t nCount;
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Wait till previously posted write to couunt Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDCNT0|BITM_RTC_SR1_WPNDCNT1))

    /* disable interrupts during paired read */
    ADI_DISABLE_INT(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->CNT1 << 16u;
    nCount |= pDevice->pRTCRegs->CNT0;
    *pCount = nCount;
    ADI_ENABLE_INT(pDevice->eIRQn);

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get current RTC count value with fraction.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pfCount    Pointer to application memory where the count(with fraction) value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the current 32-bit RTC count value and write it to the address provided by parameter \a pCount.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetCountEx(ADI_RTC_HANDLE const hDevice, float32_t *pfCount)
{
    uint32_t nCount,nTemp;
    uint16_t nPrescale;
    ADI_RTC_DEVICE *pDevice = hDevice;
    float32_t fFraction;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to couunt Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDCNT0|BITM_RTC_SR1_WPNDCNT1))
    nPrescale = (pDevice->pRTCRegs->CR1&BITM_RTC_CR1_PRESCALE2EXP)>>BITP_RTC_CR1_PRESCALE2EXP;
    /* disable interrupts during paired read */
    ADI_DISABLE_INT(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->CNT1 << 16u;
    nCount |= pDevice->pRTCRegs->CNT0;
    nTemp = (1lu<<nPrescale);
    fFraction = (float32_t)pDevice->pRTCRegs->CNT2/(float32_t)(nTemp); 
    ADI_ENABLE_INT(pDevice->eIRQn);
    *pfCount = (float32_t)nCount+ fFraction;

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get current RTC count value of all registers.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pfCount    Pointer to application memory where the count(with fraction) value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the current 32-bit RTC count integer value and fractional value in the integer format.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetCountRegs(ADI_RTC_HANDLE const hDevice, uint32_t *pnCount,uint32_t *pfCount)
{
    uint32_t nCount;
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to couunt Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDCNT0|BITM_RTC_SR1_WPNDCNT1))
    /* disable interrupts during paired read */
    ADI_DISABLE_INT(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->CNT1 << 16u;
    nCount |= pDevice->pRTCRegs->CNT0;
    *pnCount= nCount;
    *pfCount = (uint32_t)pDevice->pRTCRegs->CNT2; 
    ADI_ENABLE_INT(pDevice->eIRQn);
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Get current RTC interrupt source status.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pStatus    Pointer to application memory where the interrupt status is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the current RTC pending interrupt source status register and write it to the address provided by parameter \a pStatus.
 * Result is of type ADI_RTC_INT_SOURCE_TYPE, which is a bit field.
 *
 * @note Interrupt source status is not gated with the interrupt enable bits; source status is set regardless of interrupt enables.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_ClearInterruptStatus().
 */
ADI_RTC_RESULT adi_rtc_GetInterruptStatus(ADI_RTC_HANDLE const hDevice, ADI_RTC_INT_TYPE *pStatus)
{

    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nInterrupt,nTemp;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

     /* Read from SR2 register which has additional interrupts */
     nInterrupt = (uint32_t)pDevice->pRTCRegs->SR2 & ADI_RTC_SR2_IRQ_STATUS_MASK;
     /* Place them into their position */
     nInterrupt = nInterrupt << ADI_RTC_INTERRUPT_OFFSET;
     /* Read from the SR1 register */
     nInterrupt |= (uint32_t)pDevice->pRTCRegs->SR0 & ADI_RTC_INT_SOURCE_MASK;
    if( pDevice->pRTCRegs == pADI_RTC1) 
    {
        nTemp = (uint32_t)pDevice->pRTCRegs->SR3 & ADI_RTC_SR3_IRQ_STATUS_MASK;
        if((nTemp & BITM_RTC_SR3_RTCOC1IRQ) != 0u)
        {
            nInterrupt |= (uint32_t)ADI_RTC_OUTPUT_COMPARE_CH1_INT;
        }
        if((nTemp & (ADI_RTC_SR3_ICIRQ_STATUS_MASK)) != 0u)
        {
            nInterrupt |= (nTemp & ADI_RTC_SR3_ICIRQ_STATUS_MASK) << (ADI_RTC_INTERRUPT_OFFSET + BITP_RTC_CR2IC_RTCIC0IRQEN);
        }
        
    }
     
     *pStatus =  (ADI_RTC_INT_TYPE)nInterrupt;
     return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Get current RTC clock trim value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  peTrim     Pointer to #ADI_RTC_TRIM_VALUE where the  trim value is to be  written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 * Read the current 16-bit RTC trim value and write it to the address provided by parameter \a pTrim.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_EnableInterrupts().
 * @sa        adi_rtc_EnableTrim().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_GetWriteSyncStatus().
 * @sa        adi_rtc_ClearInterruptStatus().
 * @sa        adi_rtc_SetTrim().
 */
ADI_RTC_RESULT adi_rtc_GetTrim (ADI_RTC_HANDLE hDevice, ADI_RTC_TRIM_VALUE *peTrim)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    if(peTrim == NULL)
    {
        return( ADI_RTC_INVALID_PARAM);
    }
#endif

    /* Wait till previously posted write to couunt Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDTRM);

    *peTrim =(ADI_RTC_TRIM_VALUE)(pDevice->pRTCRegs->TRM & BITM_RTC_TRM_VALUE);

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get output compare value for specified output channel.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eChannel   Specify the output compare channel.
 * @param[out]  pValue     Pointer to application memory where the output compare value to be written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetOutputCompareValue(ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eChannel, uint16_t *pValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDOC1)
    *pValue = pDevice->pRTCRegs->OC1;

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set output compare value for specified output channel.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eOutChannel   Specify which output compare channel. 
 * @param[out]  nValue      Output compare value to be set for specified output channel .
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_SetOutputCompareValue(ADI_RTC_HANDLE const hDevice,ADI_RTC_OUTPUT_CHANNEL eOutChannel, uint16_t nValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDOC1)
    
    pDevice->pRTCRegs->OC1 = nValue;
    
   /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCOC1)    
    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Get input capture value for specified input channel.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eChannel   Specify which input capture  channel. 
 * @param[out]  pValue     Pointer to application memory where the input capture value to be written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *                - #ADI_RTC_INVALID_CHANNEL [D]     Input channel-0 is not valid for this operation since 
 *                                                   channel-0 can provide precised (47bit) capture value.
 *
 *
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetInputCaptureValue(ADI_RTC_HANDLE const hDevice,ADI_RTC_INPUT_CHANNEL eChannel, uint16_t *pValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    ADI_RTC_RESULT eResult= ADI_RTC_SUCCESS;
    
#ifdef ADI_DEBUG
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    switch(eChannel)
    {
        case ADI_RTC_INPUT_CHANNEL_2:
           *pValue = pDevice->pRTCRegs->IC2;
            break;
        case ADI_RTC_INPUT_CHANNEL_3:
           *pValue = pDevice->pRTCRegs->IC3;
            break;
          
        case ADI_RTC_INPUT_CHANNEL_4:
           *pValue = pDevice->pRTCRegs->IC4;
            break;
        default:  
           eResult = ADI_RTC_INVALID_CHANNEL;
           break;
    }
    return(eResult);
}
/*!
 * @brief  Get snapshot of the value of RTC .
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eChannel   Specify input channel from which captured value to be obtained.
 * @param[in]   pFraction  Pointer to application memory where the  fractional part of snap shot value to be written.
 * @param[out]  pValue     Pointer to application memory where the snap shot value  of RTC  to be written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 *
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetSnapShot(ADI_RTC_HANDLE const hDevice,ADI_RTC_INPUT_CHANNEL eChannel, uint32_t *pValue, uint16_t *pFraction)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    ADI_RTC_RESULT eResult= ADI_RTC_SUCCESS;
    uint32_t nCount = 0u;
#ifdef ADI_DEBUG
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* disable interrupts during paired read */
    ADI_DISABLE_INT(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->SNAP1 << 16u;
    nCount |= pDevice->pRTCRegs->SNAP0;
    *pFraction = pDevice->pRTCRegs->SNAP2;
    *pValue = nCount;
    ADI_ENABLE_INT(pDevice->eIRQn);
    return(eResult);
}


/*!
 * @brief  Get current RTC posted write pending status.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pPendBits  Pointer to application memory where the posted write status is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 *
 * \b Pending \b Writes: Register writes to internal RTC registers take time to complete because the RTC controller
 * clock is running at a much slower (32kHz) rate than the core processor clock.  So each RTC write register has a
 * one-deep FIFO to hold write values until the RTC can effect them.  This gives rise to the notion of a \a pending
 * \a write state: if a write is already pending and another write from the core comes along before the first (pending)
 * write has cleared to its destination register, the second write may be lost because the FIFO is full already.
 *
 * To avoid data loss, the user may tell the RTC device driver to enforce safe writes with the configuration switch
 *  ADI_RTC_CFG_ENABLE_SAFE_WRITE.  Enabeling safe writes (on be default) insures write data is never lost by
 * detecting and pausing on pending writes prior writing new data.  The penalty in using safe writes is the stall
 * overhead in execution (which is not incurred if there is nothing pending).  Additionally, \a all pending writes
 * may also be synchronized manually with the #adi_rtc_SynchronizeAllWrites() API, which will pause until all
 * pending RTC writes have completed.
 *
 * The distinction between "pend" status (#adi_rtc_GetWritePendStatus()) and "sync" (#adi_rtc_GetWriteSyncStatus())
 * status is that the \a pend state is normally clear and is set only while no room remains in a register's write FIFO,
 * whereas \a sync state is normally set and is clear only while the effects of the write are not yet apparent.
 *
 * Each write error
 * source may be configured to interrupt the core by enabeling the appropriate
 * write error interrupt mask bit in the RTC control register (see the
 * #adi_rtc_EnableInterrupts() API), at which time, the RTC interrupt handler
 * will be dispatched.
 *
 * @sa        adi_rtc_Open().
 * @sa        #adi_rtc_EnableInterrupts().
 * @sa        adi_rtc_GetWriteSyncStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_GetWritePendStatus (ADI_RTC_HANDLE const hDevice, ADI_RTC_WRITE_STATUS *pPendBits)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint16_t nPendBits;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* get the value */
    nPendBits = pDevice->pRTCRegs->SR1 & ADI_RTC_WRITE_STATUS_MASK;
    *pPendBits = (ADI_RTC_WRITE_STATUS)nPendBits;

    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Get current RTC posted write synchronization status.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pSyncBits  Pointer to application memory where the posted write status is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 *
 * \b Pending \b Writes: Register writes to internal RTC registers take time to complete because the RTC controller
 * clock is running at a much slower (32kHz) rate than the core processor clock.  So each RTC write register has a
 * one-deep FIFO to hold write values until the RTC can effect them.  This gives rise to the notion of a \a pending
 * \a write state: if a write is already pending and another write from the core comes along before the first (pending)
 * write has cleared to its destination register, the second write may be lost because the FIFO is full already.
 *
 * To avoid data loss, the user may tell the RTC device driver to enforce safe writes with the
 * #ADI_RTC_CFG_ENABLE_SAFE_WRITE switch.  Enabling safe writes (on be default) insures write data is never lost by
 * detecting and pausing on pending writes prior writing new data.  The penalty in using safe writes is the stall
 * overhead in execution (which is not incurred if there is nothing pending).  Additionally, \a all pending writes
 * may also be synchronized manually with the #adi_rtc_SynchronizeAllWrites() API, which will pause until all
 * pending RTC writes have completed.
 *
 * The distinction between "pend" status (#adi_rtc_GetWritePendStatus()) and "sync" (#adi_rtc_GetWriteSyncStatus())
 * status is that the \a pend state is normally clear is set only while no room remains in a register's write FIFO,
 * whereas \a sync state is normally set and is clear only while the effects of the write are not yet apparent.
 *
 * Each write error source may be configured to interrupt the core by enabling
 * the appropriate write error interrupt mask bit in the RTC control register
 * (see the #adi_rtc_EnableInterrupts() API), at which thime, the RTC interrupt
 * handler will be dispatched.
 *
 * @sa        adi_rtc_Open().
 * @sa        #adi_rtc_EnableInterrupts().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtcStallOnPendingWrites().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT  adi_rtc_GetWriteSyncStatus (ADI_RTC_HANDLE const hDevice, ADI_RTC_WRITE_STATUS *pSyncBits)
{
   ADI_RTC_DEVICE *pDevice = hDevice;
   uint16_t nSyncBits;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to couunt Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDSR0);

    /* get the value */
    nSyncBits = pDevice->pRTCRegs->SR0 & ADI_RTC_WRITE_STATUS_MASK;
    *pSyncBits = (ADI_RTC_WRITE_STATUS)nSyncBits;

    return ADI_RTC_SUCCESS;
}


/*************************************************************************************************
**************************************************************************************************
******************************************   SET APIS   ******************************************
**************************************************************************************************
*************************************************************************************************/


/*!
 * @brief  Set a new RTC alarm value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   nAlarm     New alarm value to set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 32-bit RTC alarm comparator with the value provided by \a Alarm.
 *
 * Honors the safe write mode if set.  Otherwise, it is the application's responsibility to
 * synchronize any multiple writes to the same register.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_EnableAlarm().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetAlarm (ADI_RTC_HANDLE const  hDevice, uint32_t nAlarm)
{
   ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    /* Wait till previously posted write to Alram Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDALM0|BITM_RTC_SR1_WPNDALM1))

    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    pDevice->pRTCRegs->ALM0 = (uint16_t)nAlarm;
    pDevice->pRTCRegs->ALM1 = (uint16_t)(nAlarm >> 16);
    pDevice->pRTCRegs->ALM2 = 0u;
    ADI_EXIT_CRITICAL_REGION();
    
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,(BITM_RTC_SR0_WSYNCALM0|BITM_RTC_SR0_WSYNCALM1))    

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Set Prescale. This is  power of 2 division factor for the RTC base clock.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   nPreScale  Prescale value to be set. if "nPreScale" is 5, RTC base clock is
                           divided by 32.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_EnableAlarm().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetPreScale(ADI_RTC_HANDLE const  hDevice, uint8_t nPreScale )
{
   ADI_RTC_DEVICE *pDevice = hDevice;
   uint16_t nTemp;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    /* Pre scale is valid only in RTC-1 */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }
#endif
    PEND_BEFORE_WRITE(SR2,BITM_RTC_SR2_WPNDCR1MIR)
    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    /* format is Alaram1(16-32) alaram0(0-16).Alaram2(fraction)*/
    nTemp = pDevice->pRTCRegs->CR1 & (uint16_t)~BITM_RTC_CR1_PRESCALE2EXP;
    nTemp |= (uint16_t)((uint16_t)nPreScale << BITP_RTC_CR1_PRESCALE2EXP);
    pDevice->pRTCRegs->CR1 = nTemp;
    ADI_EXIT_CRITICAL_REGION();
    
    SYNC_AFTER_WRITE(SR2,BITM_RTC_SR2_WSYNCCR1MIR)   
    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set Prescale. This is  power of 2 division factor for the RTC base clock.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   nPeriod    Periodic, modulo-60 alarm time in prescaled RTC time units beyond a modulo-60 boundary.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 *  @note     This API helps  the CPU to position a periodic (repeating) alarm interrupt from the RTC at any integer number of prescaled RTC time units from a modulo-60 boundary (roll-over event) of the value of count.
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_EnableAlarm().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetMod60AlarmPeriod(ADI_RTC_HANDLE const hDevice, uint8_t nPeriod )
{
   ADI_RTC_DEVICE *pDevice = hDevice;
   uint16_t nTemp;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }

    /* Mod60 alaram is valid only in RTC-1 */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }

#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    /* format is Alaram1(16-32) alaram0(0-16).Alaram2(fraction)*/
    nTemp = pDevice->pRTCRegs->CR0 & BITM_RTC_CR0_MOD60ALM;
    nTemp |= (uint16_t)((uint16_t)nPeriod << BITP_RTC_CR0_MOD60ALM);
    pDevice->pRTCRegs->CR0 = nTemp;
    ADI_EXIT_CRITICAL_REGION();
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set a new RTC alarm value with fractional value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   fAlarm     New alarm value to set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 32-bit RTC alarm comparator with the value provided by \a Alarm.
 *
 * Honors the safe write mode if set.  Otherwise, it is the application's responsibility to
 * synchronize any multiple writes to the same register.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_EnableAlarm().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetAlarmEx(ADI_RTC_HANDLE const hDevice, float32_t fAlarm)
{
   ADI_RTC_DEVICE *pDevice = hDevice;
   uint32_t nAlaram = (uint32_t)fAlarm,nTemp;
   uint16_t nPreScale;
   float32_t fFraction;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    /* Only 1Hz clocking is supported in RTC-0.So no fractional alaram. */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }

#endif

    /* Wait till previously posted write to Alaram Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDALM0|BITM_RTC_SR1_WPNDALM1))
    nPreScale = (pDevice->pRTCRegs->CR1&BITM_RTC_CR1_PRESCALE2EXP)>>BITP_RTC_CR1_PRESCALE2EXP;
    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    /* format is Alaram1(16-32) alaram0(0-16).Alaram2(fraction)*/
    pDevice->pRTCRegs->ALM0 = (uint16_t)nAlaram;
    pDevice->pRTCRegs->ALM1 = (uint16_t)(nAlaram >> 16);
    nTemp = 1lu<<nPreScale;
    fFraction = (fAlarm - (float32_t)nAlaram) *(float32_t)(nTemp);
    pDevice->pRTCRegs->ALM2 = (uint16_t)(fFraction);
    ADI_EXIT_CRITICAL_REGION();
    /* Wait till  write to Alaram Register to take effect */    
    SYNC_AFTER_WRITE(SR0,(BITM_RTC_SR0_WSYNCALM0|BITM_RTC_SR0_WSYNCALM1))    

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Set a new RTC control register value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eRegister  Specify which register need to be initalized.
 * @param[in]   Control    New control register value to set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 16-bit RTC control register with the value provided by \a Control.
 *
 * Honors the safe write mode if set.  Otherwise, it is the application's responsibility to
 * synchronize any multiple writes to the same register.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetControlRegister().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetControlRegister(ADI_RTC_HANDLE const hDevice,ADI_RTC_CONTROL_REGISTER eRegister, uint32_t Control)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

     switch(eRegister)
     {
     case ADI_RTC_CONTROL_REGISTER_0:
          pDevice->pRTCRegs->CR0 = (uint16_t)Control;
         break;
     case ADI_RTC_CONTROL_REGISTER_1:
         pDevice->pRTCRegs->CR1 = (uint16_t)Control;
         break;
     default:
         return(ADI_RTC_FAILURE);
     }
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)    
    
    return ADI_RTC_SUCCESS;

}

/*!
 * @brief      Registers a Callback function with the RTC device driver. The registered call
 *              back function will be called when an event is detected.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param [in]  pfCallback  Function pointer to Callback function. Passing a NULL pointer will
 *                          unregister the call back function.
 *
 * @param [in]  pCBparam    Call back function parameter.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * @sa        adi_rtc_Open().
 */
ADI_RTC_RESULT adi_rtc_RegisterCallback(
               ADI_RTC_HANDLE const  hDevice,
               ADI_CALLBACK   const   pfCallback,
               void        *const     pCBparam
               )

{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

#if (ADI_RTC_CFG_ENABLE_SAFE_WRITE == 1)
    /* pause on pending writes to CR to avoid data loss */
        while((pDevice->pRTCRegs->SR1 & (uint32_t)ADI_RTC_WRITE_STATUS_CONTROL0)!=0u)
        {
        }
#endif
    /* Store the address of the callback function */
    pDevice->pfCallback =  pfCallback;
    /* Store the call back parameter */
    pDevice->pCBParam    =  pCBparam;

    return ADI_RTC_SUCCESS;

}

/*!
 * @brief  Set a new RTC count value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   nCount     New count value to set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the main 32-bit RTC counter with the value provided by \a Count.
 *
 * Honors the safe write mode if set.  Otherwise, it is the application's responsibility to
 * synchronize any multiple writes to the same register.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_SetCount().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetCount (ADI_RTC_HANDLE const hDevice, uint32_t nCount)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

#if (ADI_RTC_CFG_ENABLE_SAFE_WRITE == 1)
    /* pause on pending writes to CR to avoid data loss */
        while((pDevice->pRTCRegs->SR1 & (uint32_t)(ADI_RTC_WRITE_STATUS_COUNT0 | ADI_RTC_WRITE_STATUS_COUNT1)) !=0u)
        {

        }
#endif

    /* Wait till previously posted write to count Register to complete */ 
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDCNT0|BITM_RTC_SR1_WPNDCNT1))

    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    pDevice->pRTCRegs->CNT0 = (uint16_t)nCount;
    pDevice->pRTCRegs->CNT1 = (uint16_t)(nCount >> 16);
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to count Register to take effect */    
    SYNC_AFTER_WRITE(SR0,(BITM_RTC_SR0_WSYNCCNT0|BITM_RTC_SR0_WSYNCCNT1))    

    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Set an RTC gateway command.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   Command    Gateway command value.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 16-bit RTC gateway register with the command provided by \a Command.
 *
 * The gateway register is used to force the RTC to perform some urgent action.
 *
 * Currently, only the #ADI_RTC_GATEWAY_FLUSH command is defined, which will cancel all
 * RTC register write transactions, both pending and executing.  It is intended to truncate
 * all core interactions in preparation for an imminent power loss when the RTC power
 * isolation barrier will be activated.
 *
 * @sa        adi_rtc_Open().
 */
ADI_RTC_RESULT adi_rtc_SetGateway(ADI_RTC_HANDLE const hDevice, uint16_t Command)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    /* set the command */
    pDevice->pRTCRegs->GWY = Command;
    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Clear an RTC interrupt status bit(s).
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   Status     Interrupt status bits to write.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                  Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       Input parameter out of range.
 *
 * Clear (write-one-to-clear) the RTC interrupt status field with the value provided by \a Status.
 * Bits that are set in the \a Status value will clear their corresponding interrupt status.
 * Bits that are clear in the \a Status value will have no effect.
 * Use the #ADI_RTC_INT_TYPE enumeration to encode interrupts to clear.
 *
 * Honors the safe write mode if set.  Otherwise, it is the application's responsibility to
 * synchronize any multiple writes to the same register.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtcGet_InterruptStatus().
 */
ADI_RTC_RESULT adi_rtc_ClearInterruptStatus (ADI_RTC_HANDLE const hDevice, ADI_RTC_INT_TYPE Status)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nStatus;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }

    /* range check against interrupt mask */
    if (((uint32_t)Status & (uint32_t)(ADI_RTC_INT_SOURCE_MASK     |
          (ADI_RTC_SR2_IRQ_STATUS_MASK<<ADI_RTC_INTERRUPT_OFFSET)  |
         (ADI_RTC_SR3_IRQ_STATUS_MASK<<ADI_RTC_INTERRUPT_OFFSET_IO_CAPTURE))) == 0u)
    {
        return ADI_RTC_INVALID_PARAM;
    }
    
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
         nStatus =((uint32_t)Status >>ADI_RTC_INTERRUPT_OFFSET);
         
         if((nStatus &ADI_RTC_SR2_IRQ_STATUS_MASK) != 0u)
         { 
              return(ADI_RTC_INVALID_PARAM);
         }
         
         nStatus = ( (uint32_t)Status >>ADI_RTC_INTERRUPT_OFFSET_IO_CAPTURE);
      
         if((nStatus &ADI_RTC_SR3_IRQ_STATUS_MASK) != 0u)
         {
              return(ADI_RTC_INVALID_PARAM);
         }
      
    }
    
#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDSR0)

    pDevice->pRTCRegs->SR0 |= (uint16_t)Status & ADI_RTC_INT_SOURCE_MASK;
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCSR0)    

     /* Only 1Hz clocking is supported in RTC-0.So no fractional alaram. */
    if(pDevice->pRTCRegs == pADI_RTC1)
    {
         nStatus = ( (uint32_t)Status >>ADI_RTC_INTERRUPT_OFFSET);
         
         if((nStatus & ADI_RTC_SR2_IRQ_STATUS_MASK) != 0u)
         {
               pDevice->pRTCRegs->SR2 |= ADI_RTC_SR2_IRQ_STATUS_MASK;
         }
      
         if((nStatus & ADI_RTC_SR3_IRQ_STATUS_MASK) != 0u)
         {
               pDevice->pRTCRegs->SR3 |= ADI_RTC_SR3_IRQ_STATUS_MASK;
         }
      
    }
    return ADI_RTC_SUCCESS;
}



/*!
 * @brief  Set a new RTC trim value.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eInterval    Specify the trimming interval and will always in the range of (2^2 to S^17 prescaled RTC clock ).
 * @param[in]   eTrimValue   Specify the trimming value.
 * @param[in]   eOperation   Specify the operatin(Add or subtract) need to be performed for trimming.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       Input parameter out of range.
 *
 * The RTC hardware has the ability to automatically trim the clock to compensate for variations
 * in oscillator tolerance .   Automatic trimming is enabled with the #adi_rtc_EnableTrim() API.
 *
 * @note Alarms are not affected by automatic trim operations.
 *
 * @note The trim boundary (interval) alignment is reset when new trim values are written.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_EnableTrim().
 * @sa        adi_rtc_GetTrim().
 */
ADI_RTC_RESULT adi_rtc_SetTrim(ADI_RTC_HANDLE const hDevice,  ADI_RTC_TRIM_INTERVAL eInterval, ADI_RTC_TRIM_VALUE  eTrimValue, ADI_RTC_TRIM_POLARITY eOperation)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t trm = (uint32_t)eInterval | (uint32_t)eTrimValue | (uint32_t)eOperation;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }

#endif

    /* Wait till previously posted write to Control Register to complete */ 
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDTRM)

    pDevice->pRTCRegs->TRM = (uint16_t)trm;
    
    /* Wait till  write to Control Register to take effect */    
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCTRM)    

    return ADI_RTC_SUCCESS;
}


/*************************************************************************************************
**************************************************************************************************
************************************   SYNCHRONIZATION API   *************************************
**************************************************************************************************
*************************************************************************************************/


/*!
 * @brief  Force synchronization of all pending writes.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                    Success: Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]     Invalid device handle parameter.
 *
 * Blocking call to coerce all outstanding posted RTC register writes to fully flush and synchronize.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_GetWriteSyncStatus().
*/
ADI_RTC_RESULT adi_rtc_SynchronizeAllWrites (ADI_RTC_HANDLE const hDevice)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }

#endif

    /* forced block until all SYNC bits are set (ignore bSafe) */
    while (ADI_RTC_WRITE_STATUS_MASK != (pDevice->pRTCRegs->SR0 & ADI_RTC_WRITE_STATUS_MASK))
    {

    }

    return ADI_RTC_SUCCESS;
}

#if (ADI_RTC_STATIC_CONFIG_SUPPORT_ENABLE == 1) 
/*! \cond PRIVATE */

/*
 * @brief       Initializes the device  using static configuration 
 *
 * @param[in]   pDevice    Pointer to RTC device .
                pConfig    Pointer to static configuration device structure.
 *
*/

static void rtc_init(ADI_RTC_DEVICE *pDevice,ADI_RTC_CONFIG *pConfig)
{
    /* Control register -0 which controls all main stream activity of RTC0 */
    pDevice->pRTCRegs->CR0  =   pConfig->CONTROL0;
    /* Control register -1  which is  granularity of RTC control register */    
    pDevice->pRTCRegs->CR1  =   pConfig->CONTROL1;
    /*CNT0 contains the lower 16 bits of the RTC counter  */    
    pDevice->pRTCRegs->CNT0 =   pConfig->COUNT0;
    /*CNT1 contains the lower 16 bits of the RTC counter  */        
    pDevice->pRTCRegs->CNT1 =   pConfig->COUNT1;
    /* ALM0 contains the lower 16 bits of the Alarm register */
    pDevice->pRTCRegs->ALM0 =   pConfig->ALARAM0;
    /* ALM1 contains the upper  16 bits of the Alarm register */
    pDevice->pRTCRegs->ALM1 =   pConfig->ALARAM1;
     /* ALM1 contains the fractional part of  the Alarm register */
    pDevice->pRTCRegs->ALM2 =   pConfig->ALARAM2;
    /* Set Input capture/output compare registers only for RTC1 */
    if(pDevice->pRTCRegs == pADI_RTC1)
    {
        pDevice->pRTCRegs->CR2IC =   pConfig->CONTROL2;
        pDevice->pRTCRegs->CR3OC =   pConfig->CONTROL3;
        pDevice->pRTCRegs->CR4OC =   pConfig->CONTROL4;
        pDevice->pRTCRegs->OCMSK =   pConfig->MASK;
        pDevice->pRTCRegs->OC1ARL =   pConfig->AUTORELOAD;
    }
}
#endif

/*! @brief  RTC device driver interrupt handler.  Overrides weakly-bound default interrupt handler in startup.c. */
ADI_INT_HANDLER(RTC0_Int_Handler)
{
    ISR_PROLOG();    
    uint16_t nIntSrc1;
    uint32_t fired = 0u, enables = 0u;
    ADI_RTC_DEVICE *pDevice = aRTCDeviceInfo[0].hDevice;

    /* determine qualified interrupt source(s) */

    nIntSrc1 = pDevice->pRTCRegs->SR0 & ADI_RTC_INT_SOURCE_MASK;
    enables = (uint32_t)pDevice->pRTCRegs->CR0  & ADI_RTC_INT_ENA_MASK_CR0;

    /* need to test each interrupt source and whether it is enabled before notifying */
    /* because each source is latched regardless of whether it is enabled or not :-( */
    if (((BITM_RTC_SR0_ALMINT & nIntSrc1) != 0u)
        && (((uint32_t)ADI_RTC_ALARM_INT & enables) != 0u))
    {
            fired |= (uint32_t)ADI_RTC_ALARM_INT;
    }
    enables = (enables>>(BITP_RTC_CR0_MOD60ALMINTEN-2)); 
    if ((nIntSrc1 & enables) != 0u)
    {
            fired |= (uint32_t)(nIntSrc1 & enables);
    }
    if (pDevice->pfCallback != NULL) {

        /* forward to the user if he is watching this interrupt */
        /* pass the "fired" value as the event.  argument param is not used */
        if ( fired)
        {
            pDevice->pfCallback (pDevice->pCBParam, fired, NULL);
        }
    }
    
     pDevice->pRTCRegs->SR0 |= nIntSrc1;
    ISR_EPILOG();        
}

/*! @brief  RTC device driver interrupt handler.  Overrides weakly-bound default interrupt handler in startup.c. */
ADI_INT_HANDLER(RTC1_Int_Handler)
{
    ISR_PROLOG();    
    uint16_t nIntSrc1,nIntSrc2,nIntSrc3;
    uint32_t fired = 0u, enables = 0u,nOscCtl=0u;
    ADI_RTC_DEVICE *pDevice = aRTCDeviceInfo[1].hDevice;

    /* determine qualified interrupt source(s) */

    nIntSrc1 = pDevice->pRTCRegs->SR0 & ADI_RTC_INT_SOURCE_MASK;
    enables = (uint32_t)pDevice->pRTCRegs->CR0 & ADI_RTC_INT_ENA_MASK_CR0;
    nIntSrc3 = pDevice->pRTCRegs->SR3;
    
    /* need to test each interrupt source and whether it is enabled before notifying */
    /* because each source is latched regardless of whether it is enabled or not :-( */
    if (((BITM_RTC_SR0_ALMINT & nIntSrc1) != 0u)
        && (((uint32_t)ADI_RTC_ALARM_INT & enables) != 0u))
    {
            fired |= (uint32_t)ADI_RTC_ALARM_INT;
    }
    enables = (enables>>(BITP_RTC_CR0_MOD60ALMINTEN-2)); 
    if ((nIntSrc1 & enables) != 0u)
    {
            fired |= (nIntSrc1 & enables);
    }
    nIntSrc2 = pDevice->pRTCRegs->SR2;
    enables = (uint32_t)pDevice->pRTCRegs->CR1 & ADI_RTC_INT_ENA_MASK_CR1;
    
    if ((nIntSrc2 & enables) != 0u)
    {
            fired |= (nIntSrc2 & enables)<<ADI_RTC_INTERRUPT_OFFSET;
    }
    nIntSrc3 = pDevice->pRTCRegs->SR3;
    enables = ((uint32_t)pDevice->pRTCRegs->CR2IC & ADI_RTC_INT_ENA_MASK_CR2) >> BITP_RTC_CR2IC_RTCIC0IRQEN;
    
    if ((nIntSrc3 & enables) != 0u)
    {
            fired |= (nIntSrc3 & enables)<<(ADI_RTC_INTERRUPT_OFFSET + BITP_RTC_CR2IC_RTCIC0IRQEN);
    }
    enables = (uint32_t)pDevice->pRTCRegs->CR3OC & ADI_RTC_INT_ENA_MASK_CR3;
    
    if (((BITM_RTC_SR3_RTCOC1IRQ & nIntSrc3)!=0u)
        && ((BITM_RTC_CR3OC_RTCOC1IRQEN & enables)!=0u))
    {
            fired |= (uint32_t)ADI_RTC_OUTPUT_COMPARE_CH1_INT;
    }
    nOscCtl = pADI_CLKG0_OSC->CTL ;
    if(((nOscCtl & BITM_CLKG_OSC_CTL_LFXTAL_MON_EN)== BITM_CLKG_OSC_CTL_LFXTAL_MON_EN) 
       && (( nOscCtl& (uint32_t)BITM_CLKG_OSC_CTL_LFXTAL_MON_FAIL_STAT) == BITM_CLKG_OSC_CTL_LFXTAL_MON_FAIL_STAT))
      {
            fired |= (uint32_t)ADI_LFXTL_FAILURE_INT;
      }
    if (pDevice->pfCallback!= NULL) {

        /* forward to the user if he is watching this interrupt */
        /* pass the "fired" value as the event.  argument param is not used */
        if ( fired)
        {
            pDevice->pfCallback (pDevice->pCBParam, fired, NULL);
        }
    }
   
     pDevice->pRTCRegs->SR0 |= nIntSrc1;
     pDevice->pRTCRegs->SR2 |= nIntSrc2;
     pDevice->pRTCRegs->SR3 |= nIntSrc3;
     
     if((fired&(uint32_t)ADI_LFXTL_FAILURE_INT)!=0u)
     {
       
       pADI_CLKG0_OSC->CTL|= BITM_CLKG_OSC_CTL_LFXTAL_MON_FAIL_STAT;
       
     }
     ISR_EPILOG();
      
}

/*************************************************************************************************
**************************************************************************************************
*****************************************  TIME.H STUFF  *****************************************
**************************************************************************************************
*************************************************************************************************/

/* Define this macro (in rtc.h) to use IAR time.h, the time_t type and associated
   system time interfaces, which rely on underlying RTC run-time library overrides. */
#ifdef ADI_RTC_USE_IAR_SYSTEM_TIME_HOOKS

#ifdef __ICCARM__
/*
* Pm008 (rule 2.4): sections of code should not be 'commented out'
*   Allow code examples in comments.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm008
#endif /* __ICCARM__ */

#define CLOCKS_PER_SEC 1   /*!< number of RTC clock ticks in a second */

/*
    Note: in this implementation, clock_t and __time32_t are both
    32-bit integer types reflecting the absolute RTC seconds count.

    Further, both __time32 and clock return the same RTC count.

    Simple use case:
    main() {
        __time32_t t;
        __time32(&t);
        printf("Time: %s", asctime(localtime(&t)));
    }
*/



/*
    clock_t: a integer 32-bit type supporting arithmetic operations.
    clock():
        query the RTC interface for current time.
        return seconds since last time setting.
        return -1 if "reliable" time is not available or RTC is uninitialized.
*/
/*! @brief Returns the current time. */
_STD_BEGIN
clock_t (clock)(void)
{
    ADI_RTC_HANDLE pDev = aRTCDeviceInfo[0].hDevice;  /* OK for singular RTC... */
    uint32_t t;
    if (adi_rtc_GetCount(pDev, &t)) {
        t = (uint32_t)-1;
    }
    return (clock_t)t;
}
_STD_END


/*
    __time32_t: an integral 32-bit type supporting arithmetic operations.
    __time32(__time32_t *t):
        return seconds since last time set.
        return -1 if "reliable" time is not available.
        sets time to address of t, if provided.
*/
/*! @brief Returns and gets the current time into param t. */
__time32_t (__time32)(__time32_t *t)
{
    clock_t now = clock();
    if (t) {
        *t = (__time32_t) now;
    }
    return (__time32_t) now;
}


#endif  /* ADI_RTC_USE_IAR_SYSTEM_TIME_HOOKS */

/*! \endcond */

/*! @} */

#endif /* _ADI_RTC_V1_C_ */

/*
** EOF
*/
