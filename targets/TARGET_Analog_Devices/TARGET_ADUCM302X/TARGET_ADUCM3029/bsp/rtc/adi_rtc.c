/*!
 *****************************************************************************
 * @file:    adi_rtc.c
 * @brief:   Real-Time Clock Device Implementations.
 * @version: $Revision: 35155 $
 * @date:    $Date: 2016-07-26 13:09:22 -0400 (Tue, 26 Jul 2016) $
 *----------------------------------------------------------------------------
 *
Copyright (c) 2010-2018 Analog Devices, Inc.

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
/*! \addtogroup RTC_Driver RTC Driver
 *  @{
 * @brief Real Time Clock (RTC) Driver
 * @details The RTC driver manages all instances of the RTC peripheral.
 * @note The application must include drivers/rtc/adi_rtc.h to use this driver
 */


/*! \cond PRIVATE */

#if defined(__ADUCM302x__)
#define BITP_CLKG_OSC_CTL_LFX_FAIL_STA BITP_CLKG_OSC_CTL_LFX_FAIL_STAT
#endif  /* __ADUCM302x__ */

#if defined ( __ADSPGCC__ )
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

#include <stdlib.h>  /* for 'NULL" definition */
#include <assert.h>
#include <string.h>
#include <rtos_map/adi_rtos_map.h>



#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): Types which specify sign and size should be used
*   We use bool which is accepted by MISRA but the toolchain does not accept it
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
* Pm129 (rule 12.7): bitwise operations shall not be performed on signed integer types
*   The rule makes an exception for valid expressions.
*
* Pm029: this bitwise operation is in a boolean context - logical operators should not be confused with bitwise operators
*   The rule is suppressed as the bitwise and logical operators are being used correctly and are not being confused
*
* Pm126: if the bitwise operators ~ and << are applied to an operand of underlying type 'unsigned char' or 'unsigned short', the result shall be immediately cast to the underlying type of the operand
*   The behaviour as described is correct
*
* Pm031: bitwise operations shall not be performed on signed integer types
*   Device drivers often require bit banging on MMRs that are defined as signed

*/
#pragma diag_suppress=Pm011,Pm123,Pm073,Pm143,Pm050,Pm109,Pm150,Pm140,Pm129,Pm029,Pm126,Pm031
#endif /* __ICCARM__ */
/*! \endcond */


#include <drivers/rtc/adi_rtc.h>


/*! \cond PRIVATE */


#include "adi_rtc_data.c"


#if defined(__ADUCM302x__)
#define BITP_RTC_SSMSK_SS1MSK   BITP_RTC_SSMSK_SSMSK
#endif /* __ADUCM302x__ */

#if defined(__ADUCM4x50__)

/* Data structures used to manage the enabling of all RTC interrupts */
static uint16_t cr0 = 0u, cr1 = 0u, cr3oc = 0u, cr4oc = 0u, cr2ic = 0u, cr5ocs = 0u;

struct xxx
{
  uint16_t *cr;
  uint16_t  bitPositionl;
}

Interrupt_Details[ADI_RTC_NUM_INTERRUPTS] =
{
  { &cr0, BITP_RTC_CR0_ALMINTEN },
  { &cr0, BITP_RTC_CR0_MOD60ALMINTEN },
  { &cr0, BITP_RTC_CR0_ISOINTEN },
  { &cr0, BITP_RTC_CR0_WPNDERRINTEN },
  { &cr0, BITP_RTC_CR0_WSYNCINTEN },
  { &cr0, BITP_RTC_CR0_WPNDINTEN },
  { &cr1, BITP_RTC_CR1_CNTINTEN },
  { &cr1, BITP_RTC_CR1_PSINTEN },
  { &cr1, BITP_RTC_CR1_TRMINTEN },
  { &cr1, BITP_RTC_CR1_CNTROLLINTEN },
  { &cr1, BITP_RTC_CR1_CNTMOD60ROLLINTEN },
  { &cr3oc, BITP_RTC_CR3SS_SS1IRQEN },
  { &cr3oc, BITP_RTC_CR3SS_SS2IRQEN },
  { &cr3oc, BITP_RTC_CR3SS_SS2IRQEN },
  { &cr3oc, BITP_RTC_CR3SS_SS4IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC0IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC2IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC3IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC4IRQEN },
  { &cr2ic, BITP_CLKG_OSC_CTL_LFX_FAIL_STA },
  { &cr3oc, BITM_RTC_CR3SS_SS4FEIRQEN},
  { &cr3oc, BITM_RTC_CR3SS_SS3FEIRQEN},
  { &cr3oc, BITM_RTC_CR3SS_SS2FEIRQEN},
  { &cr3oc, BITM_RTC_CR3SS_SS1FEIRQEN},
  { &cr4oc, BITP_RTC_CR4SS_SS4MSKEN},
  { &cr4oc, BITP_RTC_CR4SS_SS3MSKEN},
  { &cr4oc, BITP_RTC_CR4SS_SS2MSKEN},
  { &cr4oc, BITP_RTC_CR4SS_SS1MSKEN},
  { &cr5ocs, BITP_RTC_CR5SSS_SS3SMPMTCHIRQEN},
  { &cr5ocs, BITP_RTC_CR5SSS_SS2SMPMTCHIRQEN},
  { &cr5ocs, BITP_RTC_CR5SSS_SS1SMPMTCHIRQEN}

};
#elif defined(__ADUCM302x__)

/* Data structures used to manage the enabling of all RTC interrupts */
static uint16_t cr0 = 0u, cr1 = 0u, cr3oc = 0u, cr4oc = 0u, cr2ic = 0u;

struct xxx
{
  uint16_t *cr;
  uint16_t  bitPositionl;
}

Interrupt_Details[ADI_RTC_NUM_INTERRUPTS] =
{
  { &cr0, BITP_RTC_CR0_ALMINTEN },
  { &cr0, BITP_RTC_CR0_MOD60ALMINTEN },
  { &cr0, BITP_RTC_CR0_ISOINTEN },
  { &cr0, BITP_RTC_CR0_WPNDERRINTEN },
  { &cr0, BITP_RTC_CR0_WSYNCINTEN },
  { &cr0, BITP_RTC_CR0_WPNDINTEN },
  { &cr1, BITP_RTC_CR1_CNTINTEN },
  { &cr1, BITP_RTC_CR1_PSINTEN },
  { &cr1, BITP_RTC_CR1_TRMINTEN },
  { &cr1, BITP_RTC_CR1_CNTROLLINTEN },
  { &cr1, BITP_RTC_CR1_CNTMOD60ROLLINTEN },
  { &cr3oc, BITP_RTC_CR3SS_SS1IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC0IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC2IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC3IRQEN },
  { &cr2ic, BITP_RTC_CR2IC_IC4IRQEN },
};
#else
#error RTC is not ported for this processor
#endif


/* Forward prototypes */
void RTC0_Int_Handler(void);
void RTC1_Int_Handler(void);



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
               - #ADI_RTC_SUCCESS                   RTC device driver initialized successfully.
               - #ADI_RTC_INVALID_INSTANCE [D]         The RTC instance number is invalid.
               - #ADI_RTC_FAILURE                      General RTC initialization failure.

            The RTC controller interrupt enable state is unaltered during driver initialization.
            Use the #adi_rtc_EnableInterrupts API to manage interrupting.

    @note   The contents of phDevice will be set to NULL upon failure.\n\n

    @note   On #ADI_RTC_SUCCESS the RTC device driver is initialized and made ready for use,
            though pending interrupts may be latched.  During initialization, the content of the
            various RTC control, count, alarm and status registers are untouched to preserve prior
            RTC initializations and operation.  The core NVIC RTC interrupt is enabled.\n\n


    @note   SAFE WRITES: The "safe write" mode is enabled by default and can be changed using the macro
                         "ADI_RTC_CFG_ENABLE_SAFE_WRITE" defined in adi_rtc_config.h file.

    @sa    adi_rtc_Enable().
    @sa    adi_rtc_EnableInterrupts().
    @sa    adi_rtc_SetCount().
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

    pDevice->pRTCRegs->SR0 = ADI_RTC_SR3_IRQ_STATUS_MASK;
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCSR0)
    /* Preserve RTC counter value while re-initialization
    pDevice->pRTCRegs->CNT0 = 0u;
    pDevice->pRTCRegs->CNT1 = 0u;
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCNT0)
    */
    /* local pointer to instance data */
    aRTCDeviceInfo[DeviceNumber].hDevice = pDevice;
    pDevice->pDeviceInfo = &aRTCDeviceInfo[DeviceNumber];

    /* Do not use static configuration to
       preserve RTC counter value while re-initialization
    rtc_init(pDevice,&aRTCConfig[DeviceNumber]);
    */
    /* store handle at application handle pointer */
    *phDevice = pDevice;
    pDevice->eIRQn =  aRTCDeviceInfo[DeviceNumber].eIRQn;
    /* Enable RTC interrupts in NVIC */
    NVIC_EnableIRQ((IRQn_Type)(pDevice->eIRQn));

    return ADI_RTC_SUCCESS;            /* initialized */
}


/*!
 * @brief  Uninitialize and deallocate an RTC device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
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
    NVIC_DisableIRQ( pDevice->eIRQn);

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
 * @param[in]   bEnable    boolean Flag to enable/disable  alarm logic.
 *              - true  :   Enable alarm logic.
 *              - false :   Disable alarm logic.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]     Invalid device handle parameter.
 *
 * Enable/disable operation of RTC internal alarm logic.
 *
 * Alarm events and interrupt notifications are gated by enabling the alarm logic.
 * RTC alarm interrupts require both RTC device and RTC alarm interrupt to be enabled
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
ADI_RTC_RESULT adi_rtc_EnableAlarm(ADI_RTC_HANDLE const hDevice, bool bEnable)
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

    ADI_INT_STATUS_ALLOC();
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
 * @param[in]   bEnable    boolean Flag for enable/disable mod60  alarm logic.
 *               - true   :  Enable mod60 alarm logic.
 *               - false  :  Disable mod60 alarm logic.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Enable/disable operation of RTC internal MOD60 alarm logic.
 *
 * Alarm events and interrupt notifications are gated by enabling the alarm logic.
 * RTC alarm interrupts require both RTC device and RTC alarm interrupt to be enabled
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
ADI_RTC_RESULT adi_rtc_EnableMod60Alarm(ADI_RTC_HANDLE const hDevice, bool bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    /* Mod-60 Alarm is present only in RTC-1 */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }

#endif

    /* Wait till previously posted write to Control Register to complete */
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

    ADI_INT_STATUS_ALLOC();
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
 * @param[in]   bEnable    boolean Flag for enabling/disabling  the RTC device.
 *                - true   :     Enable RTC device.
 *                - false  :     Disable RTC device.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Global enable/disable of the RTC controller.  Enables counting of elapsed real time and acts
 * as a master enable for the RTC.
 *
 * @note When enabled, the RTC input clock pre-scaler and trim interval  are realigned.
 *
 * @note The RTC device driver does not modify the device enable on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_EnableAlarm().
 */

ADI_RTC_RESULT adi_rtc_Enable(ADI_RTC_HANDLE const hDevice, bool bEnable)
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


    ADI_INT_STATUS_ALLOC();
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
 * @param[in]   bEnable     Flag which controls whether to enable or disable RTC interrupt.
 *               - true  :  Enable RTC interrupts.
 *               - false :  Disable RTC interrupts.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Enable/disable RTC interrupt as well as manage global NVIC enable/disable for the RTC.
 * Input parameter \a Interrupts is a interrupt ID of type #ADI_RTC_INT_TYPE designating the
 * interrupt to be enabled or disabled.  The interrupt parameter may be zero, which will then simply
 * manage the NVIC RTC enable and leave the individual RTC interrupt enables unchanged.
 * Input parameter \a bEnable controls whether to enable or disable the designated set of interrupts.
 *
 * @note The RTC device driver does not modify the interrupt enables on the hardware except through use of this API.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 */
ADI_RTC_RESULT adi_rtc_EnableInterrupts (ADI_RTC_HANDLE const hDevice, ADI_RTC_INT_TYPE Interrupts, bool bEnable)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    if( (pDevice->pRTCRegs == pADI_RTC0) &&(((uint16_t)((ADI_RTC_MOD60ALM_INT | ADI_RTC_ISO_DONE_INT|
                                             ADI_RTC_COUNT_INT    |
                                             ADI_RTC_TRIM_INT    | ADI_RTC_COUNT_ROLLOVER_INT |
                                             ADI_RTC_MOD60_ROLLOVER_INT
                                             )) & (uint16_t)Interrupts) != 0u))
    {
        return(ADI_RTC_INVALID_PARAM);
    }

    assert(sizeof(Interrupt_Details)/sizeof(Interrupt_Details[0]) == ADI_RTC_NUM_INTERRUPTS);
#endif

    /* TODO - more sync for new registers */
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)
    PEND_BEFORE_WRITE(SR2,BITM_RTC_SR2_WPNDCR1MIR)

    uint8_t ndx = 0u;
    cr0 = 0u; cr1 = 0u; cr3oc = 0u; cr4oc = 0u; cr2ic = 0u;

#if defined(__ADUCM4x50__)
    cr5ocs = 0u;
#endif /* __ADUCM4x50__ */

    while( Interrupts )
    {
      if( 0u != (Interrupts & 1u) )
      {
        uint16_t *cr = Interrupt_Details[ndx].cr;
        uint16_t  enableBitPosition = Interrupt_Details[ndx].bitPositionl;
        *cr = *cr | (1u << enableBitPosition);
      }
      Interrupts >>= 1;
      ndx++;
    }
    /* set/clear interrupt enable bit(s) in control register */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR0 |= cr0;
        pDevice->pRTCRegs->CR1 |= cr1;
        pDevice->pRTCRegs->CR3SS |= cr3oc;
        pDevice->pRTCRegs->CR4SS |= cr4oc;
        pDevice->pRTCRegs->CR2IC |= cr2ic;

#if defined(__ADUCM4x50__)
        pDevice->pRTCRegs->CR5SSS |= cr5ocs;
#endif /* __ADUCM4x50__ */
    }
    else
    {
        pDevice->pRTCRegs->CR0 &= ~cr0;
        pDevice->pRTCRegs->CR1 &= ~cr1;
        pDevice->pRTCRegs->CR3SS &= ~cr3oc;
        pDevice->pRTCRegs->CR4SS &= ~cr4oc;
        pDevice->pRTCRegs->CR2IC &= ~cr2ic;
#if defined(__ADUCM4x50__)
        pDevice->pRTCRegs->CR5SSS &= ~cr5ocs;
#endif /*  __ADUCM4x50__ */
    }
    SYNC_AFTER_WRITE(SR0,BITM_RTC_SR0_WSYNCCR0)
    SYNC_AFTER_WRITE(SR2,BITM_RTC_SR2_WSYNCCR1MIR)
    return ADI_RTC_SUCCESS;
}


/*!
 * @brief  Enable RTC automatic clock trimming.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   bEnable      Flag controlling RTC enabling trim.
 *                - true     Enable RTC trimming.
 *                - false    Disable RTC trimming.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
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
ADI_RTC_RESULT adi_rtc_EnableTrim (ADI_RTC_HANDLE const hDevice, bool bEnable)
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

    ADI_INT_STATUS_ALLOC();
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
 *                - true     Enable input capture.
 *                - false    Disable input capture.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableInputCapture (ADI_RTC_HANDLE const hDevice,ADI_RTC_INPUT_CHANNEL eInpChannel, bool bEnable)
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

    ADI_INT_STATUS_ALLOC();
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
 *                - true     Enable overwrite snapshot.
 *                - false    Disable overwrite of snapshot.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableOverwriteSnapshot (ADI_RTC_HANDLE const hDevice, bool bEnable)
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

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear trim input capture enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR2IC |= BITM_RTC_CR2IC_ICOWUSEN;
    }
    else
    {
        pDevice->pRTCRegs->CR2IC &= (uint16_t)~BITM_RTC_CR2IC_ICOWUSEN;
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
 *                - false     channel uses a *high-to-low* transition on its GPIO pin to signal an input capture event
 *                - true      channel uses a *low-to-high* transition on its GPIO pin to signal an input capture event.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_SetInputCapturePolarity (ADI_RTC_HANDLE const hDevice,ADI_RTC_INPUT_CHANNEL eInpChannel, bool bEnable)
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

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear trim input capture enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR2IC |= (uint16_t)(nInpChannel << BITP_RTC_CR2IC_IC0LH);
    }
    else
    {
        pDevice->pRTCRegs->CR2IC &= (uint16_t)~(nInpChannel << BITP_RTC_CR2IC_IC0LH);
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR2IC)

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Enable output for the specified Sensor Strobe Channel.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel   Specify which Sensor Strobe channel.
 * @param[in]   bEnable      Flag for enabling  output for specified Sensor Strobe channel.
 *                - true     Enable output.
 *                - false    Disable output.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableSensorStrobeOutput (ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, bool bEnable)
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
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR3SS)

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear Sensor Strobe enable for specified channel*/
    if (bEnable)
    {
        pDevice->pRTCRegs->CR3SS |=(uint16_t)eSSChannel;
    }
    else
    {
        pDevice->pRTCRegs->CR3SS &= (uint16_t)(~(uint16_t)eSSChannel);
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR3SS)

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Enable auto reload for given Sensor Strobe Channel.
 *
 * @param[in]   hDevice        Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel     Sensor Strobe Channel number.
 * @param[in]   bEnable        Flag to enable auto reload for given Sensor Strobe Channel.
 *                - true       Enable auto reload.
 *                - false      Disable auto reload.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableAutoReload(ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, bool bEnable)
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
    PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDCR4SS)

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear auto reload enable options */
    /* Note that channel 4 does not have this feature */
    if (bEnable)
    {
      switch( eSSChannel)
      {
      case ADI_RTC_SS_CHANNEL_1:
        pDevice->pRTCRegs->CR4SS |= BITM_RTC_CR4SS_SS1ARLEN;
        break;
#if defined(__ADUCM4x50__)
      case ADI_RTC_SS_CHANNEL_2:
        pDevice->pRTCRegs->CR4SS |= BITM_RTC_CR4SS_SS2ARLEN;
        break;
      case ADI_RTC_SS_CHANNEL_3:
        pDevice->pRTCRegs->CR4SS |= BITM_RTC_CR4SS_SS3ARLEN;
        break;
#endif /* __ADUCM4x50__ */
      default:
        return ADI_RTC_FAILURE;
      }

    }
    else
    {
      switch( eSSChannel)
      {
      case ADI_RTC_SS_CHANNEL_1:
        pDevice->pRTCRegs->CR4SS &= (uint16_t)~BITM_RTC_CR4SS_SS1ARLEN;
        break;
#if defined(__ADUCM4x50__)
      case ADI_RTC_SS_CHANNEL_2:
        pDevice->pRTCRegs->CR4SS &= (uint16_t)~BITM_RTC_CR4SS_SS2ARLEN;
        break;
      case ADI_RTC_SS_CHANNEL_3:
        pDevice->pRTCRegs->CR4SS &= (uint16_t)~BITM_RTC_CR4SS_SS3ARLEN;
        break;
#endif /* __ADUCM4x50__ */
      default:
        return ADI_RTC_FAILURE;
      }
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR4SS)

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set auto reload value for the given Sensor Strobe channel.
 *
 * @param[in]   hDevice     Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel  Sensor Strobe channel for which auto reload to be set.
 * @param[in]   nValue      Auto reload value to be set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 *
 */
ADI_RTC_RESULT adi_rtc_SetAutoReloadValue(ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, uint16_t nValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    switch( eSSChannel )
    {
        case ADI_RTC_SS_CHANNEL_1:
            /* Wait till previously posted write to Control Register to complete */
            PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS1)
            pDevice->pRTCRegs->SS1 = nValue;
            /* Wait till  write to Control Register to take effect */
            SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS1)
            break;
#if defined(__ADUCM4x50__)
        case ADI_RTC_SS_CHANNEL_2:
            /* Wait till previously posted write to Control Register to complete */
            PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS2)
            pDevice->pRTCRegs->SS2 = nValue;
            /* Wait till  write to Control Register to take effect */
            SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS2)
            break;

        case ADI_RTC_SS_CHANNEL_3:
            /* Wait till previously posted write to Control Register to complete */
            PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS3)
            pDevice->pRTCRegs->SS3 = nValue;
            /* Wait till  write to Control Register to take effect */
            SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS3)
            break;

        case ADI_RTC_SS_CHANNEL_4:
            PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS4)
            pDevice->pRTCRegs->SS4 = nValue;
            /* Wait till  write to Control Register to take effect */
            SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS4)
            break;
#endif /* __ADUCM4x50__ */

        default:
            return ADI_RTC_FAILURE;

    }

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Enable or disable thermometer-code masking for the given Sensor Strobe Channel.
 *
 * @param[in]   hDevice         Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel      Sensor Strobe channel for which thermometer-code masking to be enabled or disabled.
 * @param[in]   bEnable         Flag to enable or disable masking for the given Sensor Strobe channel.
 *                - true        Enable masking .
 *                - false       Disable masking.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 */
ADI_RTC_RESULT adi_rtc_EnableSensorStrobeChannelMask(ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, bool bEnable)
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
    PEND_BEFORE_WRITE(SR5, BITM_RTC_SR5_WPENDCR4SS)

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* set/clear auto reload enable options */
    if (bEnable)
    {
        pDevice->pRTCRegs->CR4SS |= (uint16_t)eSSChannel;
    }
    else
    {
        pDevice->pRTCRegs->CR4SS &= (uint16_t)~(uint16_t)eSSChannel;
    }
    ADI_EXIT_CRITICAL_REGION();

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCCR4SS)

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  To set channel mask for the given Sensor Strobe channel.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel   Sensor Strobe Channel for which the mask to be set.
 * @param[in]   nMask        Channel Mask to be set for Sensor Strobe channel.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_CHANNEL         The given channel is invalid.
 */
ADI_RTC_RESULT adi_rtc_SetSensorStrobeChannelMask(ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, uint8_t nMask)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint16_t        MaskPos = 0u;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    switch( eSSChannel )
    {
        case ADI_RTC_SS_CHANNEL_1:
            MaskPos  = (uint16_t)BITP_RTC_SSMSK_SS1MSK;
            break;
#if defined(__ADUCM4x50__)
        case ADI_RTC_SS_CHANNEL_2:
            MaskPos  = (uint16_t)BITP_RTC_SSMSK_SS2MSK;
            break;

        case ADI_RTC_SS_CHANNEL_3:
            MaskPos  = (uint16_t)BITP_RTC_SSMSK_SS3MSK;
            break;

        case ADI_RTC_SS_CHANNEL_4:
            MaskPos  = (uint16_t)BITP_RTC_SSMSK_SS4MSK;
            break;
#endif /* __ADUCM4x50__ */
        default:
            return ADI_RTC_INVALID_CHANNEL;
    }

    /* Wait till previously posted write to Control Register to complete */
    PEND_BEFORE_WRITE(SR5, BITM_RTC_SR5_WPENDSSMSK)

    pDevice->pRTCRegs->SSMSK = ((uint16_t)nMask & 0xFu) << MaskPos;

    /* Wait till  write to Control Register to take effect */
    SYNC_AFTER_WRITE(SR4, BITM_RTC_SR4_WSYNCSSMSK)

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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 * Read the currently programmed 32-bit RTC alarm value and write it to the address provided by parameter \a pAlarm.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_SetAlarm().
 */
ADI_RTC_RESULT adi_rtc_GetAlarm (ADI_RTC_HANDLE hDevice, uint32_t *pAlarm)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nAlarm;
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
    NVIC_DisableIRQ(pDevice->eIRQn);
        nAlarm  =(uint32_t) pDevice->pRTCRegs->ALM1 << 16u;
        nAlarm |= (uint32_t)pDevice->pRTCRegs->ALM0;
    NVIC_EnableIRQ((IRQn_Type)(pDevice->eIRQn));

    *pAlarm = nAlarm;

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get current RTC alarm value  with fractional part also.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pAlarm     Pointer to application memory where the alarm value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 * Read the currently programmed 32-bit RTC alarm value and write it to the address provided by parameter \a pAlarm.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_SetAlarm().
 */
ADI_RTC_RESULT adi_rtc_GetAlarmEx (ADI_RTC_HANDLE hDevice, float *pAlarm)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
    uint32_t nAlarm,nTemp;
    uint16_t nPreScale;
    float fFraction;
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
    NVIC_DisableIRQ(pDevice->eIRQn);
    nAlarm  = (uint32_t)pDevice->pRTCRegs->ALM1 << 16u;
    nAlarm |= (uint32_t)pDevice->pRTCRegs->ALM0;
    NVIC_EnableIRQ((IRQn_Type)pDevice->eIRQn);
    nTemp = 1lu<<nPreScale;
    fFraction = (float)pDevice->pRTCRegs->ALM2 /(float)(nTemp);

    *pAlarm = (float)nAlarm+fFraction;

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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
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
    NVIC_DisableIRQ(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->CNT1 << 16u;
    nCount |= pDevice->pRTCRegs->CNT0;
    *pCount = nCount;
    NVIC_EnableIRQ((IRQn_Type)pDevice->eIRQn);

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get current RTC count value with fraction.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pfCount    Pointer to application memory where the count(with fraction) value is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 * Read the current 32-bit RTC count value and write it to the address provided by parameter \a pCount.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetCountEx(ADI_RTC_HANDLE const hDevice, float *pfCount)
{
    uint32_t nCount,nTemp;
    uint16_t nPrescale;
    ADI_RTC_DEVICE *pDevice = hDevice;
    float fFraction;
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
    NVIC_DisableIRQ(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->CNT1 << 16u;
    nCount |= pDevice->pRTCRegs->CNT0;
    nTemp = (1lu<<nPrescale);
    fFraction = (float)pDevice->pRTCRegs->CNT2/(float)(nTemp);
    NVIC_EnableIRQ((IRQn_Type)pDevice->eIRQn);
    *pfCount = (float)nCount+ fFraction;

    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Get current RTC count value of all registers.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pnCount    Pointer to application memory where the count's 32 MSB are written.
 * @param[out]  pfCount    Pointer to application memory where the count's 16 LSB are written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]  NULL pointer for input parameter.
 *
 * Read the current 32-bit RTC count integer value and fractional value in the integer format.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetCountRegs(ADI_RTC_HANDLE const hDevice, uint32_t *pnCount, uint32_t *pfCount)
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
    NVIC_DisableIRQ(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->CNT1 << 16u;
    nCount |= pDevice->pRTCRegs->CNT0;
    *pnCount= nCount;
    *pfCount = (uint32_t)pDevice->pRTCRegs->CNT2;
    NVIC_EnableIRQ((IRQn_Type)pDevice->eIRQn);
    return ADI_RTC_SUCCESS;
}



/*!
 * @brief  Get current RTC clock trim value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  peTrim     Pointer to #ADI_RTC_TRIM_VALUE where the  trim value is to be  written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
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
 * @brief  Get Sensor Strobe value for the given Sensor Strobe channel.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel Sensor Strobe Channel whose value to be read.
 * @param[out]  pValue     Pointer to application memory where the Sensor Strobe value to be written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_GetSensorStrobeValue(ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, uint16_t *pValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;

#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif
    switch( eSSChannel )
    {
    case ADI_RTC_SS_CHANNEL_1:
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS1)
      *pValue = pDevice->pRTCRegs->SS1;
      break;
#if defined(__ADUCM4x50__)
    case ADI_RTC_SS_CHANNEL_2:
      /* Wait till previously posted write to Control Register to complete */
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS2)
      *pValue = pDevice->pRTCRegs->SS2;
      break;

    case ADI_RTC_SS_CHANNEL_3:
      /* Wait till previously posted write to Control Register to complete */
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS3)
      *pValue = pDevice->pRTCRegs->SS3;
      break;

    case ADI_RTC_SS_CHANNEL_4:
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS4)
      *pValue = pDevice->pRTCRegs->SS4;
      break;
#endif /* __ADUCM4x50__ */
    default:
      return ADI_RTC_FAILURE;
    }



    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set Sensor Strobe value for the given Sensor Strobe channel.
 *
 * @param[in]   hDevice         Device handle obtained from adi_rtc_Open().
 * @param[in]   eSSChannel      Sensor Strobe Channel.
 * @param[out]  nValue          Sensor Strobe value to be set for the given Sensor Strobe channel .
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_Enable().
 * @sa        adi_rtc_SetCount().
 */
ADI_RTC_RESULT adi_rtc_SetSensorStrobeValue(ADI_RTC_HANDLE const hDevice, ADI_RTC_SS_CHANNEL eSSChannel, uint16_t nValue)
{
    ADI_RTC_DEVICE *pDevice = hDevice;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
#endif

    switch( eSSChannel )
    {
    case ADI_RTC_SS_CHANNEL_1:
      /* Wait till previously posted write to Control Register to complete */
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS1)
      pDevice->pRTCRegs->SS1 = nValue;
      /* Wait till  write to Control Register to take effect */
      SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS1)
      break;

#if defined(__ADUCM4x50__)
    case ADI_RTC_SS_CHANNEL_2:
      /* Wait till previously posted write to Control Register to complete */
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS2)
      pDevice->pRTCRegs->SS2 = nValue;
      /* Wait till  write to Control Register to take effect */
      SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS2)
      break;

    case ADI_RTC_SS_CHANNEL_3:
      /* Wait till previously posted write to Control Register to complete */
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS3)
      pDevice->pRTCRegs->SS3 = nValue;
      /* Wait till  write to Control Register to take effect */
      SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS3)
      break;

    case ADI_RTC_SS_CHANNEL_4:
      /* Wait till previously posted write to Control Register to complete */
      PEND_BEFORE_WRITE(SR5,BITM_RTC_SR5_WPENDSS4)
      pDevice->pRTCRegs->SS4 = nValue;
      /* Wait till  write to Control Register to take effect */
      SYNC_AFTER_WRITE(SR4,BITM_RTC_SR4_WSYNCSS4)
      break;
#endif /* __ADUCM4x50__ */
    default:
      return ADI_RTC_FAILURE;
    }

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
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *                - #ADI_RTC_INVALID_PARAM [D]       NULL pointer for input parameter.
 *                - #ADI_RTC_INVALID_CHANNEL [D]     Input channel-0 is not valid for this operation since
 *                                                   channel-0 can provide precise (47bit) capture value.
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
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
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
    NVIC_DisableIRQ(pDevice->eIRQn);
    nCount  = (uint32_t)pDevice->pRTCRegs->SNAP1 << 16u;
    nCount |= pDevice->pRTCRegs->SNAP0;
    *pFraction = pDevice->pRTCRegs->SNAP2;
    *pValue = nCount;
    NVIC_EnableIRQ((IRQn_Type)pDevice->eIRQn);
    return(eResult);
}


/*!
 * @brief  Get current RTC posted write pending status.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[out]  pPendBits  Pointer to application memory where the posted write status is written.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
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
 * source may be configured to interrupt the core by enabling the appropriate
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
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
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
 * (see the #adi_rtc_EnableInterrupts() API), at which time, the RTC interrupt
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 32-bit RTC alarm comparator with the value provided by \a Alarm.
 *
 * Honours the safe write mode if set.  Otherwise, it is the application's responsibility to
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

    ADI_INT_STATUS_ALLOC();
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
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
    /* Pre scale is invalid for RTC0 */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }
#endif
    PEND_BEFORE_WRITE(SR2,BITM_RTC_SR2_WPNDCR1MIR)
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    /* format is Alarm1(16-32) Alarm0(0-16).Alarm2(fraction)*/
    nTemp = pDevice->pRTCRegs->CR1 & (uint16_t)~BITM_RTC_CR1_PRESCALE2EXP;
    nTemp |= (uint16_t)((uint16_t)nPreScale << BITP_RTC_CR1_PRESCALE2EXP);
    pDevice->pRTCRegs->CR1 = nTemp;
    ADI_EXIT_CRITICAL_REGION();

    SYNC_AFTER_WRITE(SR2,BITM_RTC_SR2_WSYNCCR1MIR)
    return ADI_RTC_SUCCESS;
}
/*!
 * @brief  Set the pre-scale. This is  power of 2 division factor for the RTC base clock.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   nPeriod    Periodic, modulo-60 alarm time in pre-scaled RTC time units beyond a modulo-60 boundary.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 *  @note     This API helps  the CPU to position a periodic (repeating) alarm interrupt from the RTC at any integer number of pre-scaled RTC time units from a modulo-60 boundary (roll-over event) of the value of count.
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

    /* Mod60 Alarm is valid only in RTC-1 */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }

#endif

    /* Wait till previously posted write to Control Register to complete */
    PEND_BEFORE_WRITE(SR1,BITM_RTC_SR1_WPNDCR0)

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    /* format is Alarm1(16-32) Alarm0(0-16).Alarm2(fraction)*/
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 32-bit RTC alarm comparator with the value provided by \a Alarm.
 *
 * Honours the safe write mode if set.  Otherwise, it is the application's responsibility to
 * synchronize any multiple writes to the same register.
 *
 * @sa        adi_rtc_Open().
 * @sa        adi_rtc_GetAlarm().
 * @sa        adi_rtc_EnableAlarm().
 * @sa        adi_rtc_GetWritePendStatus().
 * @sa        adi_rtc_SynchronizeAllWrites().
 */
ADI_RTC_RESULT adi_rtc_SetAlarmEx(ADI_RTC_HANDLE const hDevice, float fAlarm)
{
   ADI_RTC_DEVICE *pDevice = hDevice;
   uint32_t nAlarm = (uint32_t)fAlarm,nTemp;
   uint16_t nPreScale;
   float fFraction;
#ifdef ADI_DEBUG
    ADI_RTC_RESULT eResult;
    if((eResult = ValidateHandle(pDevice)) != ADI_RTC_SUCCESS)
    {
        return eResult;
    }
    /* Only 1Hz clocking is supported in RTC-0.So no fractional Alarm. */
    if(pDevice->pRTCRegs == pADI_RTC0)
    {
        return(ADI_RTC_OPERATION_NOT_ALLOWED);
    }

#endif

    /* Wait till previously posted write to Alarm Register to complete */
    PEND_BEFORE_WRITE(SR1,(BITM_RTC_SR1_WPNDALM0|BITM_RTC_SR1_WPNDALM1))
    nPreScale = (pDevice->pRTCRegs->CR1&BITM_RTC_CR1_PRESCALE2EXP)>>BITP_RTC_CR1_PRESCALE2EXP;
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
    /* RTC hardware insures paired write, so no need to disable interrupts */
    /* format is Alarm1(16-32) Alarm0(0-16).Alarm2(fraction)*/
    pDevice->pRTCRegs->ALM0 = (uint16_t)nAlarm;
    pDevice->pRTCRegs->ALM1 = (uint16_t)(nAlarm >> 16);
    nTemp = 1lu<<nPreScale;
    fFraction = (fAlarm - (float)nAlarm) *(float)(nTemp);
    pDevice->pRTCRegs->ALM2 = (uint16_t)(fFraction);
    ADI_EXIT_CRITICAL_REGION();
    /* Wait till  write to Alarm Register to take effect */
    SYNC_AFTER_WRITE(SR0,(BITM_RTC_SR0_WSYNCALM0|BITM_RTC_SR0_WSYNCALM1))

    return ADI_RTC_SUCCESS;
}

/*!
 * @brief  Set a new RTC control register value.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rtc_Open().
 * @param[in]   eRegister  Specify which register need to be initialized.
 * @param[in]   Control    New control register value to set.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the 16-bit RTC control register with the value provided by \a Control.
 *
 * Honours the safe write mode if set.  Otherwise, it is the application's responsibility to
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
 *                - #ADI_RTC_SUCCESS              Call completed successfully.
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
 *                - #ADI_RTC_INVALID_HANDLE [D]      Invalid device handle parameter.
 *
 * Writes the main 32-bit RTC counter with the value provided by \a Count.
 *
 * Honours the safe write mode if set.  Otherwise, it is the application's responsibility to
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

    ADI_INT_STATUS_ALLOC();
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
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
 * @brief  Set a new RTC trim value.
 *
 * @param[in]   hDevice      Device handle obtained from adi_rtc_Open().
 * @param[in]   eInterval    Specify the trimming interval and will always in the range of (2^2 to S^17 pre-scaled RTC clock ).
 * @param[in]   eTrimValue   Specify the trimming value.
 * @param[in]   eOperation   Specify the operation(Add or subtract) need to be performed for trimming.
 *
 * @return      Status
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
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
 *                - #ADI_RTC_SUCCESS                 Call completed successfully.
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

  /* FIXME - static init is even more now */

    /* Control register -0 which controls all main stream activity of RTC0 */
    pDevice->pRTCRegs->CR0  =   pConfig->CR0;
    /* Control register -1  which is  granularity of RTC control register */
    pDevice->pRTCRegs->CR1  =   pConfig->CR1;
    /*CNT0 contains the lower 16 bits of the RTC counter  */
    pDevice->pRTCRegs->CNT0 =   pConfig->CNT0;
    /*CNT1 contains the lower 16 bits of the RTC counter  */
    pDevice->pRTCRegs->CNT1 =   pConfig->CNT1;
    /* ALM0 contains the lower 16 bits of the Alarm register */
    pDevice->pRTCRegs->ALM0 =   pConfig->ALM0;
    /* ALM1 contains the upper  16 bits of the Alarm register */
    pDevice->pRTCRegs->ALM1 =   pConfig->ALM1;
     /* ALM1 contains the fractional part of  the Alarm register */
    pDevice->pRTCRegs->ALM2 =   pConfig->ALM2;
    /* Set Input capture/sensor strobe registers only for RTC1 */
    if(pDevice->pRTCRegs == pADI_RTC1)
    {
        pDevice->pRTCRegs->CR2IC  =   pConfig->CR2IC;
        pDevice->pRTCRegs->CR3SS  =   pConfig->CR3SS;
        pDevice->pRTCRegs->CR4SS  =   pConfig->CR4SS;
        pDevice->pRTCRegs->SSMSK  =   pConfig->SSMSK;
        pDevice->pRTCRegs->SS1    =   pConfig->SS1;
#if defined(__ADUCM4x50__)
        pDevice->pRTCRegs->CR5SSS =   pConfig->CR5SSS;
        pDevice->pRTCRegs->CR6SSS =   pConfig->CR6SSS;
        pDevice->pRTCRegs->CR7SSS =   pConfig->CR7SSS;
        pDevice->pRTCRegs->GPMUX0 =   pConfig->GPMUX0;
        pDevice->pRTCRegs->GPMUX1 =   pConfig->GPMUX1;
#endif /* __ADUCM4x50__ */
    }
}



/*! @brief  RTC device driver interrupt handler.  Overrides weakly-bound default interrupt handler in <Device>_startup.c. */
void RTC0_Int_Handler(void)
{
    ISR_PROLOG();
    uint16_t nIntSrc0, nIntSrc2, nIntSrc3;
    uint32_t fired = 0u, enables = 0u;
    ADI_RTC_DEVICE *pDevice = aRTCDeviceInfo[0].hDevice;

    /* determine qualified interrupt source(s) */
    /* need to test each interrupt source and whether it is enabled before notifying */
    /* because each source is latched regardless of whether it is enabled or not :-( */

    /* CR0   SR0 */
    enables = (uint32_t)pDevice->pRTCRegs->CR0  & ADI_RTC_INT_ENA_MASK_CR0;
    nIntSrc0 = pDevice->pRTCRegs->SR0 & ADI_RTC_INT_SOURCE_MASK_SR0;
    if( nIntSrc0 && enables )
    {
      if( (enables & BITM_RTC_CR0_MOD60ALMEN) && (nIntSrc0 & BITM_RTC_SR0_MOD60ALMINT))
      {
        fired |= ADI_RTC_MOD60ALM_INT;
      }
      if( (enables & BITM_RTC_CR0_ALMINTEN) && (nIntSrc0 & BITM_RTC_SR0_ALMINT))
      {
        fired |= ADI_RTC_ALARM_INT;
      }
      if( (enables & BITM_RTC_CR0_ISOINTEN) && (nIntSrc0 & BITM_RTC_SR0_ISOINT))
      {
        fired |= ADI_RTC_ISO_DONE_INT;
      }
      if( (enables & BITM_RTC_CR0_WPNDINTEN) && (nIntSrc0 & BITM_RTC_SR0_WPNDINT))
      {
        fired |= ADI_RTC_WRITE_PEND_INT;
      }
      if( (enables & BITM_RTC_CR0_WSYNCINTEN) && (nIntSrc0 & BITM_RTC_SR0_WSYNCINT))
      {
        fired |= ADI_RTC_WRITE_SYNC_INT;
      }
      if( (enables & BITM_RTC_CR0_WPNDERRINTEN) && (nIntSrc0 & BITM_RTC_SR0_WPNDERRINT))
      {
        fired |= ADI_RTC_WRITE_PENDERR_INT;
      }
    }

    /* CR1  SR2 */
    enables = (uint32_t)pDevice->pRTCRegs->CR1  & ADI_RTC_INT_ENA_MASK_CR1;
    nIntSrc2 = pDevice->pRTCRegs->SR2 & ADI_RTC_INT_SOURCE_MASK_SR2;
    if( nIntSrc2 && enables )
    {
      if( (enables & BITM_RTC_CR1_CNTMOD60ROLLINTEN) && (nIntSrc2 & BITM_RTC_SR2_CNTMOD60ROLLINT))
      {
        fired |= ADI_RTC_MOD60_ROLLOVER_INT;
      }
      if( (enables & BITM_RTC_CR1_CNTROLLINTEN) && (nIntSrc2 & BITM_RTC_SR2_CNTROLLINT))
      {
        fired |= ADI_RTC_COUNT_ROLLOVER_INT;
      }
      if( (enables & BITM_RTC_CR1_TRMINTEN) && (nIntSrc2 & BITM_RTC_SR2_TRMINT))
      {
        fired |= ADI_RTC_TRIM_INT;
      }
      if( (enables & BITM_RTC_CR1_PSINTEN) && (nIntSrc2 & BITM_RTC_SR2_PSINT))
      {
        fired |= ADI_RTC_PSI_INT;
      }
      if( (enables & BITM_RTC_CR1_CNTINTEN) && (nIntSrc2 & BITM_RTC_SR2_CNTINT))
      {
        fired |= ADI_RTC_COUNT_INT;
      }
    }

    /* CR3OC, CR2IC  SR3*/
    enables = pDevice->pRTCRegs->CR3SS  &  (uint16_t)ADI_RTC_INT_ENA_MASK_CR3SS;
    nIntSrc3 = pDevice->pRTCRegs->SR3 & ADI_RTC_SR3_IRQ_STATUS_MASK;
    if( nIntSrc3 && enables )
    {
#if defined(__ADUCM4x50__)
      if( (enables & BITM_RTC_CR3SS_SS4IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS4IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH4_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS3IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS3IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH3_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS2IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS2IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH2_INT;
      }
#endif /* __ADUCM4x50__ */

      if( (enables & BITM_RTC_CR3SS_SS1IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS1IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH1_INT;
      }

#if defined(__ADUCM4x50__)
      if( (enables & BITM_RTC_CR3SS_SS4FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS4FEIRQ))
      {
        fired |= ADI_RTC_RTCSS4_FE_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS3FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS2FEIRQ))
      {
        fired |= ADI_RTC_RTCSS3_FE_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS2FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS3FEIRQ))
      {
        fired |= ADI_RTC_RTCSS2_FE_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS1FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS1FEIRQ))
      {
        fired |= ADI_RTC_RTCSS1_FE_INT;
      }
#endif /* __ADUCM4x50__ */
   }
   enables = pDevice->pRTCRegs->CR3SS  & (uint16_t)ADI_RTC_INT_ENA_MASK_CR2IC;
   if( nIntSrc3 && enables )
   {
      if( (enables & BITM_RTC_CR2IC_IC4IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC4IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH4_INT;
      }
      if( (enables & BITM_RTC_CR2IC_IC3IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC3IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH3_INT;
      }
      if( (enables & BITM_RTC_CR2IC_IC2IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC2IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH2_INT;
      }
      if( (enables & BITM_RTC_CR2IC_IC0IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC0IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH0_INT;
      }
   }


    if (pDevice->pfCallback != NULL) {

        /* forward to the user if he is watching this interrupt */
        /* pass the "fired" value as the event.  argument param is not used */
        if ( fired)
        {
            pDevice->pfCallback (pDevice->pCBParam, fired, NULL);
        }
    }

    /* Write 1 to clear the interrupts */
    pDevice->pRTCRegs->SR0 |= nIntSrc0;
    pDevice->pRTCRegs->SR2 |= nIntSrc2;
    pDevice->pRTCRegs->SR3 |= nIntSrc3;
    ISR_EPILOG();
}

/*! @brief  RTC device driver interrupt handler.  Overrides weakly-bound default interrupt handler in <Device>_startup.c. */
void RTC1_Int_Handler(void)
{
    ISR_PROLOG();
    uint16_t nIntSrc0, nIntSrc2, nIntSrc3;
    uint32_t fired = 0u, enables = 0u;
    ADI_RTC_DEVICE *pDevice = aRTCDeviceInfo[1].hDevice;

    /* determine qualified interrupt source(s) */
    /* need to test each interrupt source and whether it is enabled before notifying */
    /* because each source is latched regardless of whether it is enabled or not :-( */

    /* CR0   SR0 */
    enables = (uint32_t)pDevice->pRTCRegs->CR0  & ADI_RTC_INT_ENA_MASK_CR0;
    nIntSrc0 = pDevice->pRTCRegs->SR0 & ADI_RTC_INT_SOURCE_MASK_SR0;
    if( nIntSrc0 && enables )
    {
      if( (enables & BITM_RTC_CR0_MOD60ALMEN) && (nIntSrc0 & BITM_RTC_SR0_MOD60ALMINT))
      {
        fired |= ADI_RTC_MOD60ALM_INT;
      }
      if( (enables & BITM_RTC_CR0_ALMINTEN) && (nIntSrc0 & BITM_RTC_SR0_ALMINT))
      {
        fired |= ADI_RTC_ALARM_INT;
      }
      if( (enables & BITM_RTC_CR0_ISOINTEN) && (nIntSrc0 & BITM_RTC_SR0_ISOINT))
      {
        fired |= ADI_RTC_ISO_DONE_INT;
      }
      if( (enables & BITM_RTC_CR0_WPNDINTEN) && (nIntSrc0 & BITM_RTC_SR0_WPNDINT))
      {
        fired |= ADI_RTC_WRITE_PEND_INT;
      }
      if( (enables & BITM_RTC_CR0_WSYNCINTEN) && (nIntSrc0 & BITM_RTC_SR0_WSYNCINT))
      {
        fired |= ADI_RTC_WRITE_SYNC_INT;
      }
      if( (enables & BITM_RTC_CR0_WPNDERRINTEN) && (nIntSrc0 & BITM_RTC_SR0_WPNDERRINT))
      {
        fired |= ADI_RTC_WRITE_PENDERR_INT;
      }
    }

    /* CR1  SR2 */
    enables = (uint32_t)pDevice->pRTCRegs->CR1  & ADI_RTC_INT_ENA_MASK_CR1;
    nIntSrc2 = pDevice->pRTCRegs->SR2 & ADI_RTC_INT_SOURCE_MASK_SR2;
    if( nIntSrc2 && enables )
    {
      if( (enables & BITM_RTC_CR1_CNTMOD60ROLLINTEN) && (nIntSrc2 & BITM_RTC_SR2_CNTMOD60ROLLINT))
      {
        fired |= ADI_RTC_MOD60_ROLLOVER_INT;
      }
      if( (enables & BITM_RTC_CR1_CNTROLLINTEN) && (nIntSrc2 & BITM_RTC_SR2_CNTROLLINT))
      {
        fired |= ADI_RTC_COUNT_ROLLOVER_INT;
      }
      if( (enables & BITM_RTC_CR1_TRMINTEN) && (nIntSrc2 & BITM_RTC_SR2_TRMINT))
      {
        fired |= ADI_RTC_TRIM_INT;
      }
      if( (enables & BITM_RTC_CR1_PSINTEN) && (nIntSrc2 & BITM_RTC_SR2_PSINT))
      {
        fired |= ADI_RTC_PSI_INT;
      }
      if( (enables & BITM_RTC_CR1_CNTINTEN) && (nIntSrc2 & BITM_RTC_SR2_CNTINT))
      {
        fired |= ADI_RTC_COUNT_INT;
      }
    }

    /* CR3OC, CR2IC  SR3*/
    enables = pDevice->pRTCRegs->CR3SS  & (uint32_t)ADI_RTC_INT_ENA_MASK_CR3SS;
    nIntSrc3 = pDevice->pRTCRegs->SR3 & ADI_RTC_SR3_IRQ_STATUS_MASK;
    if( nIntSrc3 && enables )
    {
#if defined(__ADUCM4x50__)
      if( (enables & BITM_RTC_CR3SS_SS4IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS4IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH4_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS3IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS3IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH3_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS2IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS2IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH2_INT;
      }
#endif /* __ADUCM4x50__ */
      if( (enables & BITM_RTC_CR3SS_SS1IRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS1IRQ))
      {
        fired |= ADI_RTC_SENSOR_STROBE_CH1_INT;
      }
#if defined(__ADUCM4x50__)
      if( (enables & BITM_RTC_CR3SS_SS4FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS4FEIRQ))
      {
        fired |= ADI_RTC_RTCSS4_FE_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS3FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS2FEIRQ))
      {
        fired |= ADI_RTC_RTCSS3_FE_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS2FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS3FEIRQ))
      {
        fired |= ADI_RTC_RTCSS2_FE_INT;
      }
      if( (enables & BITM_RTC_CR3SS_SS1FEIRQEN) && (nIntSrc3 & BITM_RTC_SR3_SS1FEIRQ))
      {
        fired |= ADI_RTC_RTCSS1_FE_INT;
      }
#endif /* __ADUCM4x50__ */
   }
   enables = pDevice->pRTCRegs->CR2IC  & (uint32_t)ADI_RTC_INT_ENA_MASK_CR2IC;
   if( nIntSrc3 && enables )
   {
      if( (enables & BITM_RTC_CR2IC_IC4IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC4IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH4_INT;
      }
      if( (enables & BITM_RTC_CR2IC_IC3IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC3IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH3_INT;
      }
      if( (enables & BITM_RTC_CR2IC_IC2IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC2IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH2_INT;
      }
      if( (enables & BITM_RTC_CR2IC_IC0IRQEN) && (nIntSrc3 & BITM_RTC_SR3_IC0IRQ))
      {
        fired |= ADI_RTC_INPUT_CAPTURE_CH0_INT;
      }
   }

    if (pDevice->pfCallback != NULL) {

        /* forward to the user if he is watching this interrupt */
        /* pass the "fired" value as the event.  argument param is not used */
        if ( fired)
        {
            pDevice->pfCallback (pDevice->pCBParam, fired, NULL);
        }
    }

    /* Write 1 to clear the interrupts */
    pDevice->pRTCRegs->SR0 |= nIntSrc0;
    pDevice->pRTCRegs->SR2 |= nIntSrc2;
    pDevice->pRTCRegs->SR3 |= nIntSrc3;

    ISR_EPILOG();
}

/*! \endcond */

/* @} */
