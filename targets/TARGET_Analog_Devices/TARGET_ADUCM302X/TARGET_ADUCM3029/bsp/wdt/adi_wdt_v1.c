/*!
 ******************************************************************************
 * @file:    adi_wdt_v1.c
 * @brief:   WDT Device Definitions for ADuC302x
 * @version: $Revision$
 * @date:    $Date$
 *-----------------------------------------------------------------------------

Copyright (c) 2014 Analog Devices, Inc.

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

 *****************************************************************************/

#ifndef _ADI_WDT_V1_C_
#define _ADI_WDT_V1_C_

/*==========  I N C L U D E  ==========*/

#include <string.h>
#include <services/int/adi_int.h>
#include <stddef.h>

#include <ssdd_common/common_def.h>
#include <services/wdt/adi_wdt.h>

#include "adi_wdt_def_v1.h"

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

/* write sync macros */
#define ADI_WDT_SYNC_BITS ((1u<<BITP_WDT_STAT_COUNTING) | (1u<<BITP_WDT_STAT_LOADING) | (1u<<BITP_WDT_STAT_CLRIRQ)) /*!< bits used for testing write sync */


#define ADI_WDT_CLR_VALUE 0xCCCCu;

/* Watchdog Timer interrupt handler */
ADI_INT_HANDLER(WDog_Tmr_Int_Handler);



/*==========  D A T A  ==========*/
#ifdef __ICCARM__
/*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for MMR address casts as in the pADI_WDT0 macro here.
*/
#pragma diag_suppress=Pm140
#endif /* __ICCARM__ */
static ADI_WDT_DRIVER adi_wdt_Device[] =
{
    {
        pADI_WDT0,    /* Base WDT register address */
        NULL          /* Data storage pointer */
    }
};
#ifdef __ICCARM__
#pragma diag_default=Pm140
#endif /* __ICCARM__ */

/* handle checker for debug mode */
#ifdef ADI_DEBUG
#define ADI_WDT_INVALID_HANDLE(h) (&adi_wdt_Device[ADI_WDT_DEVID_0] != (h))
#endif

#if (ADI_WDT_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
static const ADI_WDT_STATIC_INIT gWdtStaticInitData[ADI_WDT_MAX_DEVID] = {
	/* single instance of WDT device */
    {
		/* load register */
		(ADI_WDT_LOAD_VALUE),

		/* control register*/
		( (ADI_WDT_CONTROL_TIMER_MODE      << BITP_WDT_CTL_MODE)
		| (ADI_WDT_CONTROL_DEVICE_ENABLE   << BITP_WDT_CTL_EN)
		| (ADI_WDT_CONTROL_CLOCK_PRESCALER << BITP_WDT_CTL_PRE)
		| (ADI_WDT_CONTROL_TIMEOUT_MODE    << BITP_WDT_CTL_IRQ)
		)
    }
};
#endif

/*! \addtogroup WDT_Service
 *  @{
 */

/*!
    @brief Initialize and allocate the WDT device for use.

    @param[in] DeviceNum    Integer specifying the ID of watchdog to use
                            This number MUST be zero (watchdog only supports
                            one instance).
    @param[in] pMemory      Pointer to the memory to be used by the driver.
                            Size of the memory should be atleast #ADI_WDT_MEMORY_SIZE bytes.
    @param[in] MemorySize   Size of the memory passed in pMemory parameter.
    @param[out] phDevice    Pointer to a location that the device data pointer
                            will be written upon successful initialization.
    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.
               - #ADI_WDT_ALREADY_INITIALIZED [D] if watchdog already initialized.
               - #ADI_WDT_INITIALIZATION_FAIL watchdog failed to initialize.
               - #ADI_WDT_BAD_DEV_ID if DeviceNum is not zero.


    No other WDT APIs may be called until the device init function is called.

    Initialize the WDT device using internal default configuration settings and allocate the device for use.  The caller's
    device handle is written with the device instance data pointer.  The returned device handle is required to be passed
    to all subsequent calls to convey which device instance to operate on.

    @note      The contents of phDevice will be set to NULL upon failure.

    @warning   This API will put the watchdog in preconfigured mode when ADI_WDT_CFG_ENABLE_STATIC_CONFIG_SUPPORT is defined
               as 1 in adi_wdt_config.h file.
               User can pre-configure Timer mode, Load Count, Prescaler and timeout mode by using adi_wdt_config.h file.
               Also watchdog timer can be enabled in this API by defining ADI_WDT_CONTROL_DEVICE_ENABLE as 1 in adi_wdt_config.h file.

    @sa        adi_wdt_Close().
*/
ADI_WDT_RESULT adi_wdt_Open(ADI_WDT_DEV_ID       const DeviceNum,
                            void*                const pMemory,
                            uint32_t             const MemorySize,
                            ADI_WDT_HANDLE*      const phDevice)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_DEV_DATA *pData;
    ADI_WDT_TypeDef *pReg;

#if defined(ADI_DEBUG)
    if (DeviceNum >= (uint32_t)ADI_WDT_MAX_DEVID)
    {
        return ADI_WDT_BAD_DEV_ID;
    }
    if (adi_wdt_Device[DeviceNum].pData != NULL)
    {
        return ADI_WDT_ALREADY_INITIALIZED;
    }
    if (MemorySize < sizeof(ADI_WDT_DEV_DATA))
    {
        return ADI_WDT_BAD_DEV_DATA_SIZE;
    }
#endif
    memset(pMemory,0,MemorySize);
    pDevice = &adi_wdt_Device[DeviceNum];
    pData  = (ADI_WDT_DEV_DATA*)pMemory;

    /* store a bad handle in case of failure */
    *phDevice = (ADI_WDT_HANDLE) NULL;
    pData->bBusSyncWait = true;
    pData->InitState = ADI_WDT_STATE_UNINITIALIZED;
    /* save parameters for device instance    */
    pData->pfCallback = NULL;

    pData->pCBParam   = NULL;

#if (ADI_WDT_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
    pReg = pDevice->pReg;

    /* if WDT not locked, and disabled (i.e. timer is not active) ... */
    if ((pReg->STAT & BITM_WDT_STAT_LOCKED) == 0u)
    {
        if ((pReg->CTL & BITM_WDT_CTL_EN) == 0u)
        {
            /* WDT is not locked and not enabled, so proceed to configure it... */

            /* assert pre-configured static data... */
            ADI_WDT_STATIC_INIT const* pInitData = &gWdtStaticInitData[DeviceNum];

            pReg->LOAD  = pInitData->wdtLoadRegister;
            pReg->CTL = pInitData->wdtControlRegister;
        }
    }  /* end of unlocked & disabled WDT init block */

    /* else, WDT is either:
       a) locked (i.e. its already configured and running), in which case
          we can't do anything (the user can still use these APIs to reset
          the timer and perform status checks), OR,
       b) not locked, but is enabled (i.e. the timer is running), in which
          case we also can't make any changes until the user decides to disable
          and re-configure the timer
      bottom line: no further WDT configurations to perform at this time...
    */
#endif

    /* install interrupt handler */
    pData->IRQn = WDT_EXP_IRQn;
    ADI_INSTALL_HANDLER(pData->IRQn, WDog_Tmr_Int_Handler);

    /* Enable WDT interrupts in NVIC */
    ADI_ENABLE_INT(pData->IRQn);

    pData->InitState = ADI_WDT_STATE_INITIALIZED;  /* mark device as initialized */
    *phDevice = (ADI_WDT_HANDLE)pDevice;                             /* write user handle */
    pDevice->pData = pData;
    /* kick the dog on the way out, maximizing application time to configure... */
    if (ADI_WDT_SUCCESS != adi_wdt_ResetTimer(pDevice))
    {
        return ADI_WDT_INITIALIZATION_FAIL;
    }


    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Uninitialize and deallocate the WDT device.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().

    @return     Status
                - #ADI_WDT_SUCCESS if successfully un-initialized.
                - #ADI_WDT_NOT_INITIALIZED [D] if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.
                - #ADI_WDT_LOCKED [D] if watchdog timer is locked.

    @sa         adi_wdt_Open().
*/
ADI_WDT_RESULT adi_wdt_Close(ADI_WDT_HANDLE const hDevice)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_DEV_DATA *pData;
    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pData = pDevice->pData;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }
    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif


    /* uninitialize */
    ADI_DISABLE_INT(pData->IRQn);
    ADI_UNINSTALL_HANDLER(pData->IRQn);
    pData->IRQn = (IRQn_Type) 0;
    pDevice->pData = NULL;
    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Enable/Disable the watch dog timer.

    @details    Enables the WDT and starts the timer counting,
                or Disables the watchdog after initial core startup.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[in]  bEnable    Indicates the desired setting of the ENABLE bit
                           in the CTL register.
                - True - Enable.
                - False - Disable.

    @return     Status
                - #ADI_WDT_SUCCESS if mode set as requested.
                - #ADI_WDT_SYNC_IN_PROGRESS WDT Bus-sync in progress.
                - #ADI_WDT_LOCKED if the timer is "locked" (see: #adi_wdt_GetLockedStatus()).
                - #ADI_WDT_NOT_INITIALIZED [D] if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @warning    The watchdog can only be disabled once directly after core
                startup.  This allows re-configuration from startup default
                and re-start.

                Once re-enabled, the WDT is "locked" and no
                further changes to the watchdog configuration
                can be made (to prevent accidental modification
                of the WDT).

    @note       If wait mode of the WDT is set to true, this API will
                block until the bus-sync has completed during the register write.

    @sa         adi_wdt_SetWaitMode().
    @sa         adi_wdt_GetLockedStatus().
*/
ADI_WDT_RESULT adi_wdt_Enable(ADI_WDT_HANDLE const hDevice, const bool_t bEnable)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_TypeDef *pReg;

    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }
    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    /* ensure that the watchdog is not locked out ... */
    if (((pReg->STAT & BITM_WDT_STAT_LOCKED) != 0u) && bEnable)
    {
        return ADI_WDT_LOCKED;
    }

    /* wait on WDT sync if requested */
    if (pDevice->pData->bBusSyncWait)
    {
        while((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u) {}
    }
    else
    {
        if ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u)
        {
            return ADI_WDT_SYNC_IN_PROGRESS;
        }
    }

    ADI_ENTER_CRITICAL_REGION();
    if (bEnable)
    {
        /* set periodic mode and enable bits */
        pReg->CTL |= (BITM_WDT_CTL_MODE | BITM_WDT_CTL_EN);
    } else
    {
        /* Clear the enable bit to disable the timer */
        pReg->CTL &= (uint16_t)~BITM_WDT_CTL_EN;
    }
    ADI_EXIT_CRITICAL_REGION();

    /* NOTE: This code assumes that the user does not need to know when the enable or disable
             actually occurs as a result of the write to CTL, hence this does not do a bus-sync
             check after the write.  Once enabled, it cannot be disabled, or reconfigured, so its
             irrelevant and they do not need to wait for the sync to occur */

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Set the WDT interrupt mode on/off.

    @details    Used to enable the IRQ mode in the timer to generate
                an interrupt \a instead of generating a processor reset.
                Useful during debugging, where a reset would
                destroy internal state necessary for diagnostics.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[in]  IRQMode    Desired IRQ Mode.
                - ADI_WDT_IRQ_MODE_INTRPT - Generate an interrupt instead of reset.
                - ADI_WDT_IRQ_MODE_RESET - Generate a processor reset.

    @return     Status
                - #ADI_WDT_SUCCESS if mode set as requested.
                - #ADI_WDT_SYNC_IN_PROGRESS WDT Bus-sync in progress.
                - #ADI_WDT_LOCKED if the timer is "locked" (see: #adi_wdt_GetLockedStatus()).
                - #ADI_WDT_NOT_INITIALIZED if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @note       This API will also enable/disable the WDT interrupt, as appropriate.

    @warning    The application is recommended to override the weak-bound default \a WDog_Tmr_Int_Handler()
                interrupt handler to process the interrupt as desired, otherwise the default
                handler will be used which simply traps.

    @sa         adi_wdt_GetIRQMode().
*/
ADI_WDT_RESULT adi_wdt_SetIRQMode(ADI_WDT_HANDLE const hDevice, const ADI_WDT_IRQ_MODE IRQMode)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_TypeDef *pReg;

    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }
    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    /* ensure that the watchdog is not locked out ... */
   if ((pReg->STAT & BITM_WDT_STAT_LOCKED) != 0u)
   {
       return ADI_WDT_LOCKED;
   }

    /* wait on WDT sync if requested */
    if (pDevice->pData->bBusSyncWait)
    {
        while ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u) {}
    }
    else
    {
        if ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u)
        {
            return ADI_WDT_SYNC_IN_PROGRESS;
        }
    }

    ADI_ENTER_CRITICAL_REGION();
    if (IRQMode == ADI_WDT_IRQ_MODE_INTRPT)
    {
        /* set the IRQ bit in CTL to generate an interrupt */
        pReg->CTL |= BITM_WDT_CTL_IRQ;
        /* enable the watchdog interrupt */
        ADI_ENABLE_INT(pDevice->pData->IRQn);
    }
    else
    {
        /* clear the IRQ bit in CTL to generate a reset */
        pReg->CTL &= (uint16_t)~BITM_WDT_CTL_IRQ;
        /* Disable the watchdog interrupt */
        ADI_DISABLE_INT(pDevice->pData->IRQn);
    }
    ADI_EXIT_CRITICAL_REGION();

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Set the watchdog Load Count.

    @details    Used to set the contents of LOAD register. This value in conjunction with
                the prescale sets the timeout value of the watchdog.

    @param[in]  hDevice     Device handle obtained from adi_wdt_Open().
    @param[in]  LoadCount   The 16-bit value to load into LOAD register.

    @return     Status
                - #ADI_WDT_SUCCESS if mode set as requested.
                - #ADI_WDT_SYNC_IN_PROGRESS WDT Bus-sync in progress.
                - #ADI_WDT_LOCKED if the timer is "locked" (see: #adi_wdt_GetLockedStatus).
                - #ADI_WDT_NOT_INITIALIZED [D] if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @note       If wait mode of the WDT is set to true, this API will
                block until the bus-sync has completed during the register write.

    @sa         adi_wdt_GetCount().
*/
ADI_WDT_RESULT adi_wdt_SetLoadCount(ADI_WDT_HANDLE const hDevice, const uint16_t LoadCount)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_TypeDef *pReg;

    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }

    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    /* ensure that the watchdog is not locked out ... */
    if ((pReg->STAT & BITM_WDT_STAT_LOCKED) != 0u)
    {
        return ADI_WDT_LOCKED;
    }

    /* wait on WDT sync if requested */
    if (pDevice->pData->bBusSyncWait)
    {
        while((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u) {}
    }
    else
    {
        if ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u)
        {
            return ADI_WDT_SYNC_IN_PROGRESS;
        }
    }

    pReg->LOAD = LoadCount;             /* Set the load value */

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Set the WDT prescale value.

    @details    Used to set the clock prescale value which in conjunction
                with the Load Count determines the overall timeout of the
                watchdog.

    @param[in]  hDevice     Device handle obtained from adi_wdt_Open().
    @param[in]  Prescale    Required prescale value.

    @return     Status
                - #ADI_WDT_SUCCESS if mode set as requested.
                - #ADI_WDT_SYNC_IN_PROGRESS WDT Bus-sync in progress.
                - #ADI_WDT_LOCKED if the timer is "locked" (see: #adi_wdt_GetLockedStatus()).
                - #ADI_WDT_NOT_INITIALIZED [D] if watchdog not initialized.
                - #ADI_WDT_BAD_PRESCALE if prescale value is invalid.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @note Example timer resolution and range given a timer clock of 3,2768 Hz:
        - if scale = 1
            - freq = 3,2768 Hz
            - min timeout = 1/32768 = 30.5us
            - max timeout = 65535/32768 = ~2 s
        - if scale = 16
            - freq = 2,048 Hz
            - min timeout = 1/2048 = ~488usec
            - max timeout = 65535/2048 = ~32 s
        - if scale = 256
            - freq = 128 Hz
            - min timeout = 1/128 = ~7.8ms
            - max timeout = 65535/128 = ~512 s = ~8.5 minutes

    @sa         adi_wdt_GetPrescale().
*/
ADI_WDT_RESULT adi_wdt_SetPrescale(ADI_WDT_HANDLE const hDevice, const ADI_WDT_PRESCALE Prescale)
{
    uint16_t tcon;
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_TypeDef *pReg;

    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }
    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    /* ensure that the watchdog is not locked out ... */
    if ((pReg->STAT & BITM_WDT_STAT_LOCKED) != 0u)
    {
        return ADI_WDT_LOCKED;
    }

    switch (Prescale)
    {
        case ADI_WDT_PRESCALE_1:
        case ADI_WDT_PRESCALE_16:
        case ADI_WDT_PRESCALE_256:

            /* wait on WDT sync if requested */
            if (pDevice->pData->bBusSyncWait)
            {
                while ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u) {}
            }
            else
            {
                if ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u)
                {
                    return ADI_WDT_SYNC_IN_PROGRESS;
                }
            }

            /* critical section */
            ADI_ENTER_CRITICAL_REGION();
            tcon = pReg->CTL;                            /* temp copy */
            tcon &= (uint16_t)~BITM_WDT_CTL_PRE;         /* make a hole */
            tcon |= Prescale;                            /* rollin new prescale */
            pReg->CTL = tcon;                            /* write it back */
            ADI_EXIT_CRITICAL_REGION();

            break;

        default:
            return ADI_WDT_BAD_PRESCALE;    /* invalid prescale value specified */
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Set the WDT bus-sync wait mode.

    @details    Sets the behaviour of writes to the CTL, LOAD and RESTART registers,
                particularly when enabling, or clearing the timer.

    @param[in]  hDevice     Device handle obtained from adi_wdt_Open().
    @param[in]  bWaitMode   Required bus-sync wait.
                - True - Wait for Bus Sync to complete (APIs block).
                - False - Do not wait (APIs return immediately).

    @return     Status
                - #ADI_WDT_SUCCESS if mode set as requested.
                - #ADI_WDT_NOT_INITIALIZED if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @note       If set to "False" then the affected APIs will always
                return the status code #ADI_WDT_SYNC_IN_PROGRESS if a sync is in
                progress to inform the caller that the action due to the write
                is delayed.

    @sa         adi_wdt_Enable().
    @sa         adi_wdt_ResetTimer().
    @sa         adi_wdt_Trigger().
*/
ADI_WDT_RESULT adi_wdt_SetWaitMode(ADI_WDT_HANDLE const hDevice, const bool_t bWaitMode)
{

    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }

    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    pDevice->pData->bBusSyncWait = bWaitMode;

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get WDT current count value.

    @details    Obtains the count remaining in the WDT.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[out] pCurCount  The 16-bit contents of the ACOUNT, current count register.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       The actual time remaining is a factor of this value, the clock rate
                and the clock prescale value currently in use.

    @note       This API is always successful.

    @sa         adi_wdt_SetLoadCount().
    @sa         adi_wdt_SetPrescale().
*/
ADI_WDT_RESULT adi_wdt_GetCount(ADI_WDT_HANDLE const hDevice, uint16_t* const pCurCount)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* determine the remaining count in the timer by reading
       the value of the ACOUNT register ... */
    *pCurCount = (pDevice->pReg->CCNT);
    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT device enable setting.

    @details    Determine if the timer is enabled and running.
                This is used in conjunction with #adi_wdt_GetLockedStatus()
                to determine if the time is running in the initial
                startup state.  Can also be used to determine if the
                timer is disabled and can be reconfigured.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[out] pbEnable   Current State of the ENABLE bit in the CTL register.
                - True - timer is enabled.
                - False - timer is disabled.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_GetLockedStatus().
*/
ADI_WDT_RESULT adi_wdt_GetEnable(ADI_WDT_HANDLE const hDevice, bool_t* const pbEnable)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* determine if the watchdog has been enabled ... */
    if ((pDevice->pReg->CTL & BITM_WDT_CTL_EN) != 0u)
    {
        *pbEnable = true;
    }
    else
    {
        *pbEnable = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT interrupt mode.

    @details    Determine whether the timer is set for
                reset operation (normal) or interrupt generation.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[out] pIRQMode   Current state of the IRQ bit in the CTL register.
                - #ADI_WDT_IRQ_MODE_INTRPT - Generate an interrupt instead of reset.
                - #ADI_WDT_IRQ_MODE_RESET - Generate a processor reset.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_SetIRQMode().
*/
ADI_WDT_RESULT adi_wdt_GetIRQMode(ADI_WDT_HANDLE const hDevice, ADI_WDT_IRQ_MODE* const pIRQMode)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* determine the current IRQ mode ...
        if bit set, interrupt mode, else reset mode */
    *pIRQMode = ((pDevice->pReg->CTL & BITM_WDT_CTL_IRQ) != 0u) ? ADI_WDT_IRQ_MODE_INTRPT:ADI_WDT_IRQ_MODE_RESET;
    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT clock prescale value.

    @details    Determine the current clock prescale value in use by the timer.
                This is used in conjunction with the current count and the
                clock frequency to determine the timeout.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[out] pPrescale  One of:
                - #ADI_WDT_PRESCALE_1 - clock not scaled.
                - #ADI_WDT_PRESCALE_16 - clock/16.
                - #ADI_WDT_PRESCALE_256 - clock/256.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_SetPrescale().
*/
ADI_WDT_RESULT adi_wdt_GetPrescale(ADI_WDT_HANDLE const hDevice,  ADI_WDT_PRESCALE* const pPrescale)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* extract the pre-scale value from the CTL register */
    *pPrescale = (ADI_WDT_PRESCALE)(pDevice->pReg->CTL & BITM_WDT_CTL_PRE);
    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT Bus-sync wait mode.

    @details    Obtain the current setting of the bus-sync wait to determine the
                blocking behavoior of affected APIs.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[out] pbWaitMode  Current state of the wait mode.
                - True - wait for bus sync to complete (APIs block).
                - False - don't wait for bus-sync (APIs return immediately).

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_Enable().
    @sa         adi_wdt_Trigger().
    @sa         adi_wdt_ResetTimer().
*/
ADI_WDT_RESULT adi_wdt_GetWaitMode(ADI_WDT_HANDLE const hDevice, bool_t* const pbWaitMode)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    *pbWaitMode = pDevice->pData->bBusSyncWait;
    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT Bus Sync Status for RESTART register.

    @details    Determine if a bus sync is still in progress after a write to RESTART.
                This is only important to ensure that the writes to the RESTART register
                is complete before the timer becomes reset or triggered.

    @param[in]  hDevice      Device handle obtained from adi_wdt_Open().
    @param[out] pbSyncStatus The contents of the CLR_IRQ bit in the STAT register.
                - True - bus-sync in progress.
                - False - bus-sync complete.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_Trigger().
    @sa         adi_wdt_ResetTimer().
*/
ADI_WDT_RESULT adi_wdt_GetCLRISyncStatus(ADI_WDT_HANDLE const hDevice, bool_t* const pbSyncStatus)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;
    /* if the CLRI bit is set in the STAT register,
       then a bus-write-sync is in progress */

    if ((pDevice->pReg->STAT & BITM_WDT_STAT_CLRIRQ) != 0u)
    {
        *pbSyncStatus = true;
    }
    else
    {
        *pbSyncStatus = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT Bus Sync Status for CTL register.

    @details    Determine if a bus sync is still in progress after a write to CTL.
                This is only important to ensure that the writes to configuration
                is complete before enabling the timer.

    @param[in]  hDevice      Device handle obtained from adi_wdt_Open().
    @param[out] pbSyncStatus  The contents of the COUNTING bit in the STAT register.
                - True - bus-sync in progress.
                - False - bus-sync complete.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_Enable().
*/
ADI_WDT_RESULT adi_wdt_GetCTLSyncStatus(ADI_WDT_HANDLE const hDevice, bool_t* const pbSyncStatus)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* if the COUNTING bit is set in the STAT register,
       then a bus-write-sync is in progress */
    if ((pDevice->pReg->STAT & BITM_WDT_STAT_COUNTING) != 0u)
    {
        *pbSyncStatus = true;
    }
    else
    {
        *pbSyncStatus = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT Bus Sync Status for LOAD register.

    @details    Determine if a bus sync is still in progress after a write to LOAD.
                This is only important to ensure that the writes to configuration
                is complete before enabling the timer, or resetting the timer.

    @param[in]  hDevice      Device handle obtained from adi_wdt_Open().
    @param[out] pbSyncStatus  The contents of the LOAD bit in the STAT register.
                - True - bus-sync in progress.
                - False - bus-sync complete.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.

    @sa         adi_wdt_Enable().
    @sa         adi_wdt_ResetTimer().
*/
ADI_WDT_RESULT adi_wdt_GetLDSyncStatus(ADI_WDT_HANDLE const hDevice, bool_t* const pbSyncStatus)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;
    /* if the LD bit is set in the STAT register,
       then a bus-write-sync is in progress */
    if ((pDevice->pReg->STAT & BITM_WDT_STAT_LOADING) != 0u)
    {
        *pbSyncStatus = true;
    }
    else
    {
        *pbSyncStatus = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT IRQ Pending Status.

    @details    Determine if the WDT has an interrupt pending.

    @param[in]  hDevice      Device handle obtained from adi_wdt_Open().
    @param[out] pbPendStatus Current state of the IRQ bit of the STAT register.
                - True - interrupt is pending.
                - False - interrupt is not pending.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.
*/
ADI_WDT_RESULT adi_wdt_GetIRQPendStatus(ADI_WDT_HANDLE const hDevice, bool_t* const pbPendStatus)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* determine if an IRQ is pending ... */
    if ((pDevice->pReg->STAT & BITM_WDT_STAT_IRQ) != 0u)
    {
        *pbPendStatus = true;
    }
    else
    {
        *pbPendStatus = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT IRQ Pending Status.

    @details    Determine if the Reset Control register is written and locked.

    @param[in]  hDevice           Device handle obtained from adi_wdt_Open().
    @param[out] pbResetCtlStatus  Current state of the IRQ bit of the STAT register.
                - True - ResetCtrl written and locked.
                - False - ResetCtrl never written yet.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       This API is always successful.
*/
ADI_WDT_RESULT adi_wdt_GetResetCtrl(ADI_WDT_HANDLE const hDevice, bool_t* const pbResetCtlStatus)
{
    ADI_WDT_DRIVER *pDevice;
    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* determine if an IRQ is pending ... */
    if ((pDevice->pReg->STAT & BITM_WDT_STAT_RSTCTL) != 0u)
    {
        *pbResetCtlStatus = true;
    }
    else
    {
        *pbResetCtlStatus = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Get the WDT device Locked Status.

    @details    Determine if the timer has been started and configuration changes
                are locked out.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().
    @param[out] pbLckStatus Current state of the LOCK bit in the STAT register.
                - True - Watchdog is locked.
                - False - Watchdog can be configured.

    @return    Status
               - #ADI_WDT_SUCCESS if successfully initialized.

    @note       A processor restart is required to return the timer to
                a non-locked state such that changes to the configuration
                can be made.

    @sa         adi_wdt_Enable().
*/
ADI_WDT_RESULT adi_wdt_GetLockedStatus(ADI_WDT_HANDLE const hDevice, bool_t* const pbLckStatus)
{
    ADI_WDT_DRIVER *pDevice;

    pDevice = (ADI_WDT_DRIVER*)hDevice;

    /* determine if the watchdog is locked out ... */
    if ((pDevice->pReg->STAT & BITM_WDT_STAT_LOCKED) != 0u)
    {
        *pbLckStatus = true;
    }
    else
    {
        *pbLckStatus = false;
    }

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Reset the WDT.

    @details    Clears the counter and restarts the timer using the
                pre-configured values. Also clears IRQ if interrupt enabled.
                This is the API used to "kick" the watchdog to prevent
                the timeout and reset of the processor.  This is the main
                device API that holds off the WDT from expiring and should be
                called periodically to refresh the WDT so it does not expire.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().

    @return     Status
                - #ADI_WDT_SUCCESS if watchdog successfully reset.
                - #ADI_WDT_SYNC_IN_PROGRESS WDT Bus-sync in progress.
                - #ADI_WDT_NOT_INITIALIZED if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @warning    If this API is not called periodically, before the timer
                expires, then the processor will reset (or generate an
                interrupt if in IRQ mode).

    @note       If watchdog bus-sync wait mode flag is set to true, this API
                will block until the bus-sync has completed during the
                register write.

    @sa         adi_wdt_GetIRQMode().
*/
ADI_WDT_RESULT adi_wdt_ResetTimer(ADI_WDT_HANDLE const hDevice)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_TypeDef *pReg;

    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }
    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    /* wait on WDT sync if requested */
    if (pDevice->pData->bBusSyncWait)
    {
        while ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u) {}
    }
    else
    {
        if ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u)
        {
            return ADI_WDT_SYNC_IN_PROGRESS;
        }
    }

    /* Reset the timer (i.e. kick the dog) */
    pReg->RESTART = ADI_WDT_CLR_VALUE;

    return ADI_WDT_SUCCESS;
}


/*!
    @brief      Force an immediate a WDT Reset/Interrupt sequence.

    @details    This API can be used to invoke an immediate reset of the
                processor, or to cause a watchdog interrupt if in IRQ mode.

    @param[in]  hDevice    Device handle obtained from adi_wdt_Open().

    @return     Status
                - #ADI_WDT_SUCCESS if the trigger of the interrupt was
                    successful (in IRQMode).
                - #ADI_WDT_SYNC_IN_PROGRESS WDT Bus-sync in progress.
                - #ADI_WDT_NOT_INITIALIZED if watchdog not initialized.
                - #ADI_WDT_BAD_DEV_DATA [D] if watchdog handle is corrupt.

    @note       If watchdog bus-sync wait mode is set to true, this API will block
                until the bus-sync has completed during the register write.
                If the watchdog is in reset mode, then this function will not
                return if successful.

    @sa         adi_wdt_GetIRQMode().
*/
ADI_WDT_RESULT adi_wdt_Trigger(ADI_WDT_HANDLE const hDevice)
{
    ADI_WDT_DRIVER *pDevice;
    ADI_WDT_TypeDef *pReg;

    pDevice = (ADI_WDT_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }

    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif

    /* wait on WDT sync if requested */
    if (pDevice->pData->bBusSyncWait)
    {
        while ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u) {}
    }
    else
    {
        if ((pReg->STAT & ADI_WDT_SYNC_BITS) != 0u)
        {
            return ADI_WDT_SYNC_IN_PROGRESS;
        }
    }

    /* NOTE: a write of any value other than T3CLRI_VALUE_CLR to the
             RESTART register causes a watchdog reset or IRQ */
    pReg->RESTART = 0u;      /* trigger the timer reset or IRQ */

    return ADI_WDT_SUCCESS;
}
/**
 * @brief       Registers a Callback function with the watch dog timer  device. The registered call
 *              back function will be called when an event is detected to notify the user.
 *
 * @param [in]  hDevice     Device handle to Watch dog  device which is obtained when a watch dog  device is opened
 *                          successfully.
 *
 * @param [in]  pfCallback  Function pointer to Callback function. Passing a NULL pointer will
 *                          unregister the call back function.
 *
 * @param [in]  pCBparam    Call back function parameter.
 *
 * @return      Status
 *
 *  - #ADI_WDT_SUCCESS                    Successfully registered specified callback function.
 *
 *  - #ADI_WDT_BAD_DEV_DATA        [D]  Invalid wdt device handle.
 *
 *  - #ADI_WDT_NOT_INITIALIZED     [D]  The device was not initialized
 *
 *
 */
ADI_WDT_RESULT adi_wdt_RegisterCallback(
               ADI_WDT_HANDLE const hDevice,
               ADI_CALLBACK   const   pfCallback,
               void        *const     pCBparam
               )

{
    ADI_WDT_DRIVER *pDevice = (ADI_WDT_DRIVER*)hDevice;

#ifdef ADI_DEBUG
    /* ensure handle points to a valid watchdog device ... */
    if (ADI_WDT_INVALID_HANDLE(hDevice))
    {
        return ADI_WDT_BAD_DEV_DATA;
    }

    /* check that watchdog device is initialized ... */
    if (ADI_WDT_STATE_INITIALIZED != pDevice->pData->InitState)
    {
        return ADI_WDT_NOT_INITIALIZED;
    }
#endif
    /* Store the address of the callback function */
    pDevice->pData->pfCallback =  pfCallback;
    /* Store the call back parameter */
    pDevice->pData->pCBParam    =  pCBparam;
    /* Return success */
    return(ADI_WDT_SUCCESS);
}

/*! \cond PRIVATE */
/*! @brief  WDT device driver interrupt handler.  Overrides weakly-bound default interrupt handler in startup.c. */
ADI_INT_HANDLER (WDog_Tmr_Int_Handler)
{
    ISR_PROLOG();    
    ADI_WDT_DRIVER *pDevice = &adi_wdt_Device[0];
    ADI_WDT_DEV_DATA *pData = pDevice->pData;
    /* Reset WDT. This will also clear the WDT interrupt status bit */
    adi_wdt_ResetTimer(&adi_wdt_Device[ADI_WDT_DEVID_0]);

    /* Call the application callback */
    if(pData->pfCallback)
    {
      pData->pfCallback(pData->pCBParam, ADI_WD_EVENT_GP_INT, NULL);
    }
    ISR_EPILOG();       
}
/*! \endcond */

#endif /* _ADI_WDT_V1_C_ */

/*! @} */

/*
** EOF
*/
