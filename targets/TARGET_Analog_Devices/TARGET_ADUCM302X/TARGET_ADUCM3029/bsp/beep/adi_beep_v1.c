/*!
 *****************************************************************************
 * @file:    adi_beep_v1.c
 * @brief:   Beeper Device Implementations for ADuC302X
 * @version: $Revision: 33333 $
 * @date:    $Date: 2016-01-22 16:39:48 +0000 (Fri, 22 Jan 2016) $
 *----------------------------------------------------------------------------
 *
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

/*! \addtogroup BEEP_Driver BEEP Device Driver
 *  @{
 */

#ifndef _ADI_BEEP_V1_C_

/* \cond PRIVATE */
#define _ADI_BEEP_V1_C_

#include <services/int/adi_int.h>
#include <stddef.h>
#include <assert.h>

#include <ssdd_common/common_def.h>

#include <drivers/beep/adi_beep.h>
#include "adi_beep_def_v1.h"

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit.
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function.
*   Multiple returns are used for error handling.
*
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ADI_INSTALL_HANDLER and others.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   Required for MMR addresses and callback parameters.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm140
#endif /* __ICCARM__ */

/*==========  D A T A  ==========*/
static ADI_BEEP_DRIVER adi_beep_Device[] =
{
    {
        pADI_BEEP0,    /* Base BEEP register address */
        NULL           /* Data storage pointer */
    }
};

/* forward declarations... */
/* These are needed in the case where ADI_CFG_ENABLE_RTOS_SUPPORT=1              */
/* The ADI_INSTALL_HANDLER macro will expand to a call to adi_int_InstallHandler */
/* and the handler will be referenced                                            */
/* The non-OSAL expansion of ADI_INSTALL_HANDLER results in a NULL string        */
ADI_INT_HANDLER(Beep_Int_Handler);

/* debug handle checker */
#ifdef ADI_DEBUG
#define ADI_BEEP_INVALID_HANDLE(h) (&adi_beep_Device[0] != (h))
#endif

#if (ADI_BEEP_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
static const ADI_BEEP_STATIC_INIT gBeeperStaticConfigData[ADI_BEEP_MAX_DEVID] = {
	/* single instance of Beeper device */
	{
		/* configuration register */
		( (ADI_BEEP_CFG_INTERRUPT_ON_SEQUENCE_END          << BITP_BEEP_CFG_SEQATENDIRQ)
		| (ADI_BEEP_CFG_INTERRUPT_ON_SEQUENCE_NEAR_END     << BITP_BEEP_CFG_SEQNEARENDIRQ)
		| (ADI_BEEP_CFG_INTERRUPT_ON_TONEB_END             << BITP_BEEP_CFG_BENDIRQ)
		| (ADI_BEEP_CFG_INTERRUPT_ON_TONEB_START           << BITP_BEEP_CFG_BSTARTIRQ)
		| (ADI_BEEP_CFG_INTERRUPT_ON_TONEA_END             << BITP_BEEP_CFG_AENDIRQ)
		| (ADI_BEEP_CFG_INTERRUPT_ON_TONEA_START           << BITP_BEEP_CFG_ASTARTIRQ)
		| (ADI_BEEP_CFG_BEEPER_DISABLE                     << BITP_BEEP_CFG_EN)
		| (ADI_BEEP_CFG_SEQUENCE_REPEAT_VALUE              << BITP_BEEP_CFG_SEQREPEAT)
		),

		/* Status register (interrupt clears) */
		( (ADI_BEEP_STAT_CLEAR_SEQUENCE_END_INTERRUPT      << BITP_BEEP_STAT_SEQENDED)
		| (ADI_BEEP_STAT_CLEAR_SEQUENCE_NEAR_END_INTERRUPT << BITP_BEEP_STAT_SEQNEAREND)
		| (ADI_BEEP_STAT_CLEAR_TONEB_END_INTERRUPT         << BITP_BEEP_STAT_BENDED)
		| (ADI_BEEP_STAT_CLEAR_TONEB_START_INTERRUPT       << BITP_BEEP_STAT_BSTARTED)
		| (ADI_BEEP_STAT_CLEAR_TONEA_END_INTERRUPT         << BITP_BEEP_STAT_AENDED)
		| (ADI_BEEP_STAT_CLEAR_TONEA_START_INTERRUPT       << BITP_BEEP_STAT_ASTARTED)
		),

		/* ToneA control register */
		( ((uint32_t)ADI_BEEP_TONEA_DISABLE                << BITP_BEEP_TONEA_DIS)
		| ((uint32_t)ADI_BEEP_TONEA_FREQUENCY              << BITP_BEEP_TONEA_FREQ)
		| ((uint32_t)ADI_BEEP_TONEA_DURATION               << BITP_BEEP_TONEA_DUR)
		),

		/* ToneB control register */
		( ((uint32_t)ADI_BEEP_TONEB_DISABLE                << BITP_BEEP_TONEB_DIS)
		| ((uint32_t)ADI_BEEP_TONEB_FREQUENCY              << BITP_BEEP_TONEB_FREQ)
		| ((uint32_t)ADI_BEEP_TONEB_DURATION               << BITP_BEEP_TONEB_DUR)
		)
	}
};
#endif
/*! \endcond */


/*!
 *  @brief BEEP Initialization
 *
 *  @param[in] DeviceNum    Integer specifying the ID of Beeper to use
 *                          This number MUST be zero (Beeper only supports
 *                          one instance).
 *  @param[in] pMemory      Pointer to the memory to be used by the driver.
 *                          Size of the memory should be atleast #ADI_BEEP_MEMORY_SIZE bytes.
 *  @param[in] MemorySize   Size of the memory passed in pMemory parameter.
 *  @param[in] pfCallback   The application supplied callback which will be called to notify device
 *                          related events.
 *  @param[in] pCBParam     The application supplied callback parameter which can be passed back in
 *                          the callback function.
 *  @param[out] phDevice    Pointer to a location that the device data pointer
 *                          will be written upon successful initialization.
 *
 *  @return    Status
 *             - #ADI_BEEP_SUCCESS                  Success: BEEP device driver initialized successfully.
 *             - #ADI_BEEP_ALREADY_INITIALIZED [D]  Error: The BEEP is already initialized.
 *             - #ADI_BEEP_BAD_DEV_ID [D]           Error: The device number is invalid.
 *
 *  Initialize and allocate a BEEP device for other use.  The core NVIC BEEP interrupt is enabled.  This API
 *  must preceed all other beeper API calls and the handle returned must be passed to all other beeper API
 *  calls.
 *
 *
 *  @note      The contents of \a phDevice will be set to NULL upon failure.\n\n
 *
 *  @note      The BEEP device driver will clear all pending interrupts and disable all beeper
 *             interrupts during beeper device initialization.
 *
 *  @note      CALLBACKS: The callback registration takes an interrupt mask (of type #ADI_BEEP_INTERRUPT)
 *             that designates the set of interrupts to watch (upon which to predicate the callback).
 *             When any of the watched interrupts occur, the BEEP device driver will make the callback to
 *             the application for application-based interrupt handling.  The BEEP device driver clears the
 *             interrupt after the callback exits, as the beeper device driver interrupt handler exits.
 *             The application callback should \b avoid \b extended \b processing during the callback
 *             as the callback is made context of the initiating interrupt and will block lower-priority
 *             interrupts.  If extended application-level interrupt processing is required, the application
 *             should schedule it for the main application loop and exit the callback as soon as possible.\n\n
 *
 *  @warning   This API will put the beeper in preconfigured mode when ADI_BEEP_CFG_ENABLE_STATIC_CONFIG_SUPPORT is
 *             defined as 1 in adi_beep_config.h file.
 *             Refer adi_beep_config.h file to see which all features can be preconfigured.
 *
 *  @sa    adi_beep_Close().
 */
ADI_BEEP_RESULT adi_beep_Open(ADI_BEEP_DEV_ID          const DeviceNum,
                              void*                    const pMemory,
                              uint32_t                 const MemorySize,
                              ADI_CALLBACK             const pfCallback,
                              void*                    const pCBParam,
                              ADI_BEEP_HANDLE*         const phDevice)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_DEV_DATA  *pData;
    /* store a bad handle in case of failure */
    *phDevice = (ADI_BEEP_HANDLE) NULL;

#ifdef ADI_DEBUG
    if (DeviceNum >= ADI_BEEP_MAX_DEVID)
    {
        return ADI_BEEP_BAD_DEV_ID;
    }

    assert (MemorySize >= sizeof(ADI_BEEP_DRIVER));
#endif

    /* local pointer to instance data */
    pDevice = &adi_beep_Device[DeviceNum];
    pDevice->pData = (ADI_BEEP_DEV_DATA*)pMemory;
    pData = pDevice->pData;

#ifdef ADI_DEBUG
    if (ADI_BEEP_STATE_UNINITIALIZED != adi_beep_Device[DeviceNum].pData->InitState)
    {
        return ADI_BEEP_ALREADY_INITIALIZED;
    }
#endif

    /* first thing: make sure BEEP has a clock so it can respond */
    /* TODO: Check this */
    /* SystemClockSourcePowerUp (ADI_SYS_CLOCK_SOURCE_LFXTAL, ADI_SYS_CLOCK_TIMEOUT_USE_DEFAULT) */

    /* initialize internal device data */
    pData->IRQn = BEEP_EVT_IRQn;

    /* if registering a callback */
    if (pfCallback != NULL)
    {
        /* make sure the watch bits are supported */
        if ((((uint32_t)pCBParam) & ADI_BEEP_ALL_INTERRUPTS) == 0u)
	{
            return ADI_BEEP_PARAM_OUT_OF_RANGE;
	}
    }

    pData->cbFunc  = pfCallback;
    pData->cbWatch = pCBParam;

#if (ADI_BEEP_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
    {
	/* assert statically configured initialization data */
	ADI_BEEP_STATIC_INIT const* pInitData = &gBeeperStaticConfigData[DeviceNum];
        ADI_BEEP_TypeDef   *pReg = pDevice->pReg;

	pReg->CFG   = pInitData->BEEP_CFG;
	pReg->STAT  = pInitData->BEEP_STAT;
	pReg->TONEA = pInitData->BEEP_TONEA;
	pReg->TONEB = pInitData->BEEP_TONEB;
    }
#endif

    /* install interrupt handler (see abstraction macro for OS in adi_rtos.h) */
    ADI_INSTALL_HANDLER(pData->IRQn, Beep_Int_Handler);

    /* enable beeper interrupts in NVIC */
    /* TODO: Check this */
    NVIC_EnableIRQ(pData->IRQn);

    /* mark driver initialized */
    pData->InitState = ADI_BEEP_STATE_INITIALIZED;

    /* store handle at application handle pointer */
    *phDevice = (ADI_BEEP_HANDLE)pDevice;

    return ADI_BEEP_SUCCESS;            /* initialized */
}


/*!
 * @brief  Uninitialize and deallocate a BEEP device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_beep_Open().
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Uninitialize and release an allocated BEEP device for other use.  The core NVIC BEEP interrupt is disabled.
 *
 * @sa        adi_beep_Open().
 */
ADI_BEEP_RESULT adi_beep_Close(ADI_BEEP_HANDLE const hDevice)
{

    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_DEV_DATA  *pData;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pData = pDevice->pData;
    pReg =  pDevice->pReg;
#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    /* uninitialize */
    /* TODO: Check this */
    NVIC_DisableIRQ(pData->IRQn);

    ADI_UNINSTALL_HANDLER(pData->IRQn);
    pData->InitState = ADI_BEEP_STATE_UNINITIALIZED;
    pData->IRQn      = (IRQn_Type)0;
    pData->cbFunc    = NULL;
    pData->cbWatch   = 0;
    pReg->CFG   = 0u;
    pReg->STAT  = 0u;
    pReg->TONEA = 0u;
    pReg->TONEB = 0u;
    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Enable/Disable the Beeper device.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   bFlag         Flag designating whether to enable or disable the device:
 *                              - true  = device enable.
 *                              - false = device disable.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Enable or disable the beeper device.
 * Use the #adi_beep_GetBusyStatus() API to query current beeper device active/inactive state.
 * The beeper does not operate unless the device is enabled.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_GetBusyStatus().
 */
ADI_BEEP_RESULT adi_beep_Enable(ADI_BEEP_HANDLE const hDevice, bool_t const bFlag)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    ADI_ENTER_CRITICAL_REGION();
	if (bFlag) {
		pReg->CFG |= BITM_BEEP_CFG_EN;
	} else {
		pReg->CFG &= (uint16_t)~(BITM_BEEP_CFG_EN);
	}
    ADI_EXIT_CRITICAL_REGION();

	return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Configure BEEP device interrupt mask.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   Interrupts    The set of beeper interrupts to operate upon.
 * @param[in]   bFlag         Flag designating whether to enable or disable \a Interrupts.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Using this API, user can configure the Beeper device interrupts for which core will be interrupted.
 * Following beeper interrupts are available:
 * - Beeper sequence end
 * - Beeper sequence near end
 * - Beeper tone B end interrupt
 * - Beeper tone B start
 * - Beeper tone A end
 * - Beeper tone A start
 *
 * Beep Interrupt handler will check the status of these beeper events and will call the application callback with the Event status.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_GetInterruptMask().
 */
ADI_BEEP_RESULT adi_beep_ConfigInterruptMask(ADI_BEEP_HANDLE const hDevice, ADI_BEEP_INTERRUPT const Interrupts, bool_t const bFlag)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    ADI_ENTER_CRITICAL_REGION();
    if (bFlag)
    {
	    pReg->CFG |= Interrupts;
    }
    else
    {
        pReg->CFG &= (uint16_t)~(Interrupts);
    }
    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Retrieve the current BEEP device interrupt mask.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[out]  pInterrupts   Pointer to an application-defined variable into which to write the result.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Retrieve the current beeper interrupt mask.  Writes the application-defined variable pointer (\a pInterrupts) object with the result.
 *
 * Beep Interrupt handler will check the status of these beeper events and will call the application callback with the Event status.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_ConfigInterruptMask().
 */
ADI_BEEP_RESULT adi_beep_GetInterruptMask(ADI_BEEP_HANDLE const hDevice, ADI_BEEP_INTERRUPT* const pInterrupts)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    *pInterrupts = (ADI_BEEP_INTERRUPT)(pReg->CFG & ADI_BEEP_ALL_INTERRUPTS);

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Retrieve the current BEEP activity state.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[out]  pbFlag        Pointer to an application-defined Boolean variable into which to write the result:
 *                              - true  = beeper is currently active.
 *                              - false = beeper is inactive.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Retrieve the current beeper activity (BUSY) state.  Writes the application-defined \a pbFlag variable pointer object with the result.
 * This API allows applications to query whether the beeper is currently busy playing a tone or a tone sequence.
 *
 * Interrupts may also advise on beeper events which can be hooked to the application space through callbacks.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_GetInterruptMask().
 */
ADI_BEEP_RESULT adi_beep_GetBusyStatus(ADI_BEEP_HANDLE const hDevice, bool_t* const pbFlag)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    if ((pReg->STAT & BITM_BEEP_STAT_BUSY) != 0u)
    {
        *pbFlag = true;
    }
    else
    {
        *pbFlag = false;
    }

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Set or clear beeper tone enablement state.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   Channel       ADI_BEEP_TONE parameter designating the tone (A/B) to operate upon.
 * @param[in]   bFlag         Flag designating whether to enable or disable the tone:
 *                              - true  = tone enable.
 *                              - false = tone disable.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Enable or disable a beeper tone.
 * Use the #adi_beep_GetToneEnable() API to query current beeper tone enablement state.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_GetToneEnable().
 */
ADI_BEEP_RESULT adi_beep_ToneEnable(ADI_BEEP_HANDLE const hDevice, ADI_BEEP_TONE const Channel, bool_t const bFlag)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    uint16_t volatile  *pToneReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
	if(Channel >= ADI_BEEP_MAX_TONES)
	{
	    return ADI_BEEP_PARAM_OUT_OF_RANGE;
	}
#endif

    pToneReg = (ADI_BEEP_TONEA == Channel) ? &pReg->TONEA : &pReg->TONEB;

    ADI_ENTER_CRITICAL_REGION();
    /* note negative logic: tone DISABLE bit */
    if (bFlag)
    {
        *pToneReg &= (uint16_t)~(ADI_BEEP_TONE_DISABLE);
    }
    else
    {
        *pToneReg |= ADI_BEEP_TONE_DISABLE;
    }
    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Retrieve the current beeper tone enablement state.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   Channel       ADI_BEEP_TONE parameter designating the tone (A/B) to operate upon.
 * @param[out]  pbFlag        Pointer to an application-defined Boolean variable into which to write the result:
 *                              - true  = tone is currently enabled.
 *                              - false = tone is currently disabled.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Retrieve the current beeper device enablement state.  Writes the application-defined \a pbFlag variable pointer object with the result.
 * Use the #adi_beep_Enable() API to set beeper device enablement state.
 * The beeper does not operate unless the device is enabled.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_ToneEnable().
 */
ADI_BEEP_RESULT adi_beep_GetToneEnable(ADI_BEEP_HANDLE const hDevice, ADI_BEEP_TONE const Channel, bool_t* const pbFlag)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    uint16_t volatile  *pToneReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
	if(Channel >= ADI_BEEP_MAX_TONES)
	{
	    return ADI_BEEP_PARAM_OUT_OF_RANGE;
	}
#endif

    pToneReg = (ADI_BEEP_TONEA == Channel) ? &pReg->TONEA : &pReg->TONEB;

    /* note negative logic */
    if ((*pToneReg & ADI_BEEP_TONE_DISABLE) == 0u)
    {
        *pbFlag = true;
    }
    else
    {
        *pbFlag = false;
    }

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Set a beeper tone.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   Channel       ADI_BEEP_TONE parameter designating the tone (A/B) to operate upon.
 * @param[in]   Frequency     ADI_BEEP_NOTE_FREQUENCY parameter designating the tone frequency.
 * @param[in]   Duration      ADI_BEEP_NOTE_DURATION parameter designating the tone duration.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *                - #ADI_BEEP_PARAM_OUT_OF_RANGE [D] Error: Parameter is out of range.
 *
 * Programs a beeper tone and enables the tone channel.  Sets the tone frequency (which is actually a clock divider value,
 * so lower values make for higher frequencies).  Simultaneously, sets the tone duration.  Both the frequency and duration
 * values are bit-fields of the same register and so they are restricted to their respective bitfield widths.
 *
 * Tonal values are right-justified.
 *
 * Use the #adi_beep_GetTone() API to query current beeper tone frequency and duration.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_GetTone().
 */
ADI_BEEP_RESULT adi_beep_SetTone(ADI_BEEP_HANDLE         const hDevice,
                                 ADI_BEEP_TONE           const Channel,
                                 ADI_BEEP_NOTE_FREQUENCY const Frequency,
                                 ADI_BEEP_NOTE_DURATION  const Duration)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    uint16_t volatile  *pToneReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
    {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_BEEP_NOT_INITIALIZED;
    }
    if ((Channel >= ADI_BEEP_MAX_TONES)     ||
        ((uint32_t)Frequency > 127u))                  /* only 7-bit field */
    {
        return ADI_BEEP_PARAM_OUT_OF_RANGE;
    }
#endif

    pToneReg = (ADI_BEEP_TONEA == Channel) ? &pReg->TONEA : &pReg->TONEB;

	ADI_ENTER_CRITICAL_REGION();
    /* set the frequency, duration (clears tone disable) all at once (no critical region needed) */
    *pToneReg =  (  (uint16_t)((uint16_t)Frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                   |(uint16_t)((uint16_t)Duration << ADI_BEEP_TONE_DUR_BITPOS)   );
     ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}

/*!
 * @brief  Retrieve a beeper tone.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   Channel       ADI_BEEP_TONE parameter designating the tone (A/B) to operate upon.
 * @param[out]  pFrequency    Pointer to an application-defined ADI_BEEP_NOTE_FREQUENCY variable into which to write the tone frequency.
 * @param[out]  pDuration     Pointer to an application-defined ADI_BEEP_NOTE_FREQUENCY variable into which to write the tone duration.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Retrieves a beeper tone frequency and duration.  Writes the application-defined \a pFrequency and \a pDuration variable pointer objects with the result.
 * Both the frequency and duration values are bit-fields of the same register and so they are restricted to their respective bitfield widths.
 *
 * Tonal values are right-justified.
 *
 * Use the #adi_beep_SetTone() API to set beeper tone frequency and duration.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_SetTone().
 */
ADI_BEEP_RESULT adi_beep_GetTone(ADI_BEEP_HANDLE          const hDevice,
                                 ADI_BEEP_TONE            const Channel,
                                 ADI_BEEP_NOTE_FREQUENCY* const pFrequency,
                                 ADI_BEEP_NOTE_DURATION*  const pDuration)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    uint16_t volatile  *pToneReg;
    uint16_t tmp;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
	if(Channel >= ADI_BEEP_MAX_TONES)
	{
	    return ADI_BEEP_PARAM_OUT_OF_RANGE;
	}
#endif

    pToneReg = (ADI_BEEP_TONEA == Channel)? &pReg->TONEA : &pReg->TONEB;

    /* get tone reg once... */
    tmp = *pToneReg;
    *pFrequency = (ADI_BEEP_NOTE_FREQUENCY) ( (tmp & (uint16_t)ADI_BEEP_TONE_FREQ_MASK) >> ADI_BEEP_TONE_FREQ_BITPOS);
    *pDuration  = (ADI_BEEP_NOTE_DURATION)  ( (tmp & (uint16_t)ADI_BEEP_TONE_DUR_MASK)  >> ADI_BEEP_TONE_DUR_BITPOS);

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Set the beeper tone sequence count.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[in]   Count         Number of A/B tone pairs to play in sequence.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Programs the A/B tone pair sequence count.
 *
 * Use the #adi_beep_GetSequence() API to query the remaining beeper sequence count.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_GetSequence().
 */
ADI_BEEP_RESULT adi_beep_SetSequence(ADI_BEEP_HANDLE const hDevice, uint8_t const Count)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    uint16_t tmp;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    ADI_ENTER_CRITICAL_REGION();
    /* make a hole */
    tmp = pReg->CFG & (uint16_t)~(BITM_BEEP_CFG_SEQREPEAT);

    /* program new sequence count, while preserving everything else */
    pReg->CFG = tmp | (uint16_t)((uint16_t)Count << BITP_BEEP_CFG_SEQREPEAT);
    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Get the current beeper tone sequence count.
 *
 * @param[in]   hDevice       Device handle obtained from adi_beep_Open().
 * @param[out]  pCount        Pointer to an application-defined variable into which to write the result.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Retrieves the currently remaining A/B tone pair sequence count.  Writes the application-defined \a pCount variable pointer object with the result.
 *
 * Use the #adi_beep_SetSequence() API to set the beeper sequence count.
 *
 * @note The beeper hardware automatically resets the ramaining sequence count to the most previously set sequence value upon sequence completion.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_SetSequence().
 */
ADI_BEEP_RESULT adi_beep_GetSequence(ADI_BEEP_HANDLE const hDevice, uint8_t* const pCount)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
        return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_INITIALIZED != pDevice->pData->InitState)
	{
        return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    *pCount = (uint8_t)((pReg->STAT & BITM_BEEP_STAT_SEQREMAIN) >> BITP_BEEP_STAT_SEQREMAIN);

    return ADI_BEEP_SUCCESS;
}


/*! @brief  BEEP device driver interrupt handler.  Overrides weakly-bound default interrupt handler in startup.c. */
ADI_INT_HANDLER (Beep_Int_Handler)
{
    ISR_PROLOG();    
    ADI_BEEP_DRIVER *pDevice = &adi_beep_Device[ADI_BEEP_DEVID_0];  /* so far, there is only one BEEP, so this is safe */
    ADI_BEEP_TypeDef *pReg;

    register uint16_t candidate;
    register ADI_BEEP_INTERRUPT fired = (ADI_BEEP_INTERRUPT)0;

    pReg = pDevice->pReg;

    /* if we have an initialized driver and a registered callback... */
    if ((pDevice->pData->InitState == ADI_BEEP_STATE_INITIALIZED) &&
        (pDevice->pData->cbFunc              != NULL))
    {
        /* determine candidate interrupt source(s) */
        ADI_ENTER_CRITICAL_REGION();
        /* read both status and mask registers without other interrupts in between */
        candidate = pReg->CFG & ADI_BEEP_ALL_INTERRUPTS;
        candidate = candidate & pReg->STAT;
        ADI_EXIT_CRITICAL_REGION();

        fired = (ADI_BEEP_INTERRUPT)0;

        /* need to test each interrupt source and whether it is enabled before notifying  */
        /* because each source is latched regardless of whether it is enabled or not :-(  */
        /* eventually, 'fired' now represents interrupts that are both set and enabled... */
        if ((ADI_BEEP_INTERRUPT_SEQ_END & candidate) != 0u)
        {
                fired |= ADI_BEEP_INTERRUPT_SEQ_END;
        }

        if ((ADI_BEEP_INTERRUPT_SEQ_NEAR_END & candidate) != 0u)
        {
                fired |= ADI_BEEP_INTERRUPT_SEQ_NEAR_END;
        }

        if ((ADI_BEEP_INTERRUPT_TONEB_END & candidate) != 0u)
        {
                fired |= ADI_BEEP_INTERRUPT_TONEB_END;
        }

        if ((ADI_BEEP_INTERRUPT_TONEB_START & candidate) != 0u)
        {
                fired |= ADI_BEEP_INTERRUPT_TONEB_START;
        }

        if ((ADI_BEEP_INTERRUPT_TONEA_END & candidate) != 0u)
        {
                fired |= ADI_BEEP_INTERRUPT_TONEA_END;
        }

        if ((ADI_BEEP_INTERRUPT_TONEA_START & candidate) != 0u)
        {
                fired |= ADI_BEEP_INTERRUPT_TONEA_START;
        }

        /* forward the interrupt to the user if he is watching it and it has fired */
        /* pass the "fired" value as the event.  argument param is not used */
        if (((uint32_t)pDevice->pData->cbWatch & fired) != 0u)
        {
            pDevice->pData->cbFunc (pDevice, (uint32_t)fired, NULL);
        }
    }

    /* clear the watched interrupt(s) that fired */
    ADI_ENTER_CRITICAL_REGION();
    pReg->STAT |= (uint16_t)(fired & ADI_BEEP_ALL_INTERRUPTS);  /* only write allowed interrupt bits */
    ADI_EXIT_CRITICAL_REGION();
    ISR_EPILOG();    
}

/*
** EOF
*/

/*@}*/

#endif /* _ADI_BEEP_V1_C_ */
