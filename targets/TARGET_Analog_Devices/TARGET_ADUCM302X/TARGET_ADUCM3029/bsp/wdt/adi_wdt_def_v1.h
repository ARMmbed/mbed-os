
/*! *****************************************************************************
 * @file:    adi_wdt_def_v1.h
 * @brief:   WDT Device Driver definition
 * @version: $Revision$
 * @date:    $Date$
 -----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

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

#ifndef _ADI_WDT_DEF_H_
#define _ADI_WDT_DEF_H_

/*! \cond PRIVATE */

/*!
 *****************************************************************************
 * \enum ADI_WDT_STATE
 *
 * WDT driver state.  Used for internal tracking of the WDT device initialization
 * progress during the adi_wdt_Init().  Also used to insure the WDT device has been
 * properly initialized as a prerequisite to using the balance of the WDT API.
 *
 *****************************************************************************/
typedef enum
{
    ADI_WDT_STATE_UNINITIALIZED= 0,        /*!< WDT is not initialized.        */
    ADI_WDT_STATE_INITIALIZED              /*!< WDT is initialized.            */
} ADI_WDT_STATE;

/*! \struct ADI_WDT_DEV_DATA

    Watchdog Private Data typedef and initialization.

    This structure contains the "state" information for the
    instance of the device.  For watchdog there is only one
    of these objects
*/
typedef struct _ADI_WDT_DEV_DATA
{
    ADI_WDT_STATE InitState;      /*!< Initialisation State        */
    IRQn_Type     IRQn;           /*!< RTC interrupt number        */
    bool_t        bBusSyncWait;   /*!< Wait for Bus-Sync (if true) */
    ADI_CALLBACK  pfCallback;     /*!< Application supplied callback function */
    void*         pCBParam;       /*!< Callback parm passed back in callbacks */
} ADI_WDT_DEV_DATA;

/*! \struct ADI_WDT_DRIVER_STRUCT
 *  WDT Device Structure
 */
typedef struct _ADI_WDT_DRIVER_STRUCT
{
    ADI_WDT_TypeDef  *const pReg;  /*!< Pointer to register base                 */
    ADI_WDT_DEV_DATA *pData;       /*!< Pointer to device data structure         */
} ADI_WDT_DRIVER_STRUCT;

#if (ADI_WDT_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
/*! \struct ADI_WDT_STATIC_INIT
 *  conditionally create static initialization data based on adi_wdt_config.h settings
 */
typedef struct _ADI_WDT_STATIC_INIT
{
    uint16_t wdtLoadRegister;     /*!< WDT load register    */
    uint16_t wdtControlRegister;  /*!< WDT control register */
} ADI_WDT_STATIC_INIT;
#endif

/* alias for the actual device structure */
typedef ADI_WDT_DRIVER_STRUCT      ADI_WDT_DRIVER;

/*! \endcond */

#endif /* _ADI_WDT_DEF_H_ */
