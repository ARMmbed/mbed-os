/*!
 ******************************************************************************
 * @file:    adi_wdt_v1.h
 * @brief:   Watchdog Timer Device Definitions
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

#ifndef __ADI_WDT_V1_H__
#define __ADI_WDT_V1_H__

/* disable misra diagnostics as necessary */

/*! \addtogroup WDT_Service WDT Service
 *  @{
 */
#include <adi_types.h>
#include <services/int/adi_int.h>
#include <adi_wdt_config.h>

#if defined (__ADUCM30xx__)
#else
#error "Unknown processor family"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! Amount of memory(In bytes) required by the WDT device driver for managing the operation.
 * This memory is completely owned by the driver till the end of the operation. 
 */
#define ADI_WDT_MEMORY_SIZE        (12u)

/*! \enum ADI_WDT_DEV_ID Watchdog Timer Device ID. */
typedef enum
{
    ADI_WDT_DEVID_0 = 0,             /*!< The one and only watchdog timer device instance */
    ADI_WDT_MAX_DEVID                /*!< maximum number of watchdog timer device instances */
} ADI_WDT_DEV_ID;


/*! \enum ADI_WDT_RESULT Watchdog status/result codes. */
typedef enum
{
    ADI_WDT_SUCCESS = 0,             /*!< The API call succeeded. */
    ADI_WDT_FAILURE,             /*!< The API call failed. */
    ADI_WDT_ALREADY_INITIALIZED, /*!< Watchdog is already initialized (if debug mode). */
    ADI_WDT_BAD_DEV_DATA,        /*!< Pointer does not point to valid watchdog data. */
    ADI_WDT_BAD_DEV_DATA_SIZE,   /*!< Insufficient device data size provided. */
    ADI_WDT_BAD_DEV_ID,          /*!< There is only a single Watchdog instance. */
    ADI_WDT_BAD_PRESCALE,        /*!< Invalid prescale value specified. */
    ADI_WDT_INITIALIZATION_FAIL, /*!< Watchdog initialization failed. */
    ADI_WDT_LOCKED,              /*!< Watchdog is Locked. */
    ADI_WDT_NOT_INITIALIZED,     /*!< Watchdog not yet initialized (if debug mode). */
    ADI_WDT_SYNC_IN_PROGRESS         /*!< Bus Sync after write is in progress. */
} ADI_WDT_RESULT;

/*! \enum ADI_WDT_MODE Watchdog Timer Mode Definition. */
typedef enum
{
    ADI_WDT_MODE_FREE_RUNNING= 0,        /*!< WDT mode is free running.        */
    ADI_WDT_MODE_PERIODIC                /*!< WDT mode is periodic.            */
} ADI_WDT_MODE;

/*! \enum ADI_WDT_PRESCALE Watchdog Prescale Values Definition. */
typedef enum
{
    ADI_WDT_PRESCALE_1   =  ENUM_WDT_CTL_DIV1,     /*!< Clock is not scaled.    */
    ADI_WDT_PRESCALE_16  =  ENUM_WDT_CTL_DIV16,    /*!< Clock/16 (i.e 2048Hz).  */
    ADI_WDT_PRESCALE_256 =  ENUM_WDT_CTL_DIV256    /*!< Clock/256 (i.e. 128Hz). */
} ADI_WDT_PRESCALE;

/*! \enum ADI_WDT_IRQ_MODE Watchdog Timer interrupt. */
typedef enum
{
    ADI_WDT_IRQ_MODE_RESET= 0,                /*!< WDT asserts reset when timeout.       */
    ADI_WDT_IRQ_MODE_INTRPT                   /*!< WDT generates interrupt when timeout. */
} ADI_WDT_IRQ_MODE;

/*! Enumeration of possible watchdog timer event modes */
typedef enum
{
    ADI_WD_EVENT_GP_INT                       /*!< Generate a general purpose interrupt event. */
} ADI_WD_EVENT;

/*! A device handle used in all API functions to identify the WDT device. */
typedef void * ADI_WDT_HANDLE;


/* Initialize and allocate the WDT device for use */
ADI_WDT_RESULT           adi_wdt_Open                 (ADI_WDT_DEV_ID       const DeviceNum,
                                                       void*                const pMemory,
                                                       uint32_t             const MemorySize,
                                                       ADI_WDT_HANDLE*      const phDevice);
													   
/* Uninitialize and deallocate the WDT device */
ADI_WDT_RESULT           adi_wdt_Close                (ADI_WDT_HANDLE       const hDevice);

/* Set the WDT Enable bit */
ADI_WDT_RESULT           adi_wdt_Enable               (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t               const bEnable);
													   
/* Set the WDT interrupt mode on/off */
ADI_WDT_RESULT           adi_wdt_SetIRQMode           (ADI_WDT_HANDLE       const hDevice,
                                                       ADI_WDT_IRQ_MODE     const IRQMode);
													   
/* Set the watchdog Load Count */
ADI_WDT_RESULT           adi_wdt_SetLoadCount         (ADI_WDT_HANDLE       const hDevice,
                                                       uint16_t             const LoadCount);
													   												   
 /* Set the WDT prescale value */
ADI_WDT_RESULT           adi_wdt_SetPrescale          (ADI_WDT_HANDLE       const hDevice,
                                                       ADI_WDT_PRESCALE     const Prescale);
													   
/* Set the WDT bus-sync wait mode */
ADI_WDT_RESULT           adi_wdt_SetWaitMode          (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t               const bWaitMode);

/* Get WDT current count value */
ADI_WDT_RESULT           adi_wdt_GetCount             (ADI_WDT_HANDLE       const hDevice,
                                                       uint16_t*            const pCurCount);

/* Get the WDT device enable setting */
ADI_WDT_RESULT           adi_wdt_GetEnable            (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbEnable);

/* Get the WDT interrupt mode on/off */
ADI_WDT_RESULT           adi_wdt_GetIRQMode           (ADI_WDT_HANDLE       const hDevice,
                                                       ADI_WDT_IRQ_MODE*    const pIRQMode);

/* Get the WDT prescale value */
ADI_WDT_RESULT           adi_wdt_GetPrescale          (ADI_WDT_HANDLE       const hDevice,
                                                       ADI_WDT_PRESCALE*    const pPrescale);

/* Get the WDT bus-sync wait mode */
ADI_WDT_RESULT           adi_wdt_GetWaitMode          (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbWaitMode);

/* Get the WDT Bus Sync Status for CLRI register */
ADI_WDT_RESULT           adi_wdt_GetCLRISyncStatus    (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbSyncStatus);

/* Get the WDT Bus Sync Status for CON register */
ADI_WDT_RESULT           adi_wdt_GetCTLSyncStatus     (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbSyncStatus);

/* Get the WDT Bus Sync Status for LD register */
ADI_WDT_RESULT           adi_wdt_GetLDSyncStatus      (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbSyncStatus);

/* Get the WDT IRQ Pending Status */
ADI_WDT_RESULT           adi_wdt_GetIRQPendStatus     (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbPendStatus);

/* Get the WDT Reset Control Status */
ADI_WDT_RESULT           adi_wdt_GetResetCtrl         (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbResetCtlStatus);

/* Get the WDT device Locked Status */
ADI_WDT_RESULT           adi_wdt_GetLockedStatus      (ADI_WDT_HANDLE       const hDevice,
                                                       bool_t*              const pbLckStatus);

/* Reset the WDT */
ADI_WDT_RESULT           adi_wdt_ResetTimer           (ADI_WDT_HANDLE       const hDevice);

/* Force an immediate a WDT Reset/Interrupt sequence */
ADI_WDT_RESULT           adi_wdt_Trigger              (ADI_WDT_HANDLE       const hDevice);

ADI_WDT_RESULT adi_wdt_RegisterCallback(ADI_WDT_HANDLE const hDevice,ADI_CALLBACK   const   pfCallback,void *const pCBparam);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __ADI_WDT_V1_H__ */
