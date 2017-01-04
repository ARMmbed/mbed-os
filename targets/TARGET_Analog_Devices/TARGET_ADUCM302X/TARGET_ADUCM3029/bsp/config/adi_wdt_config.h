/*!
 *****************************************************************************
   @file:    adi_wdt_config.h
   @brief:   Configuration options for WDT driver.
             This is specific to the WDT driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 29360 $
   @date:    $Date: 2014-12-23 15:28:37 +0000 (Tue, 23 Dec 2014) $
  -----------------------------------------------------------------------------

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

#ifndef __ADI_WDT_CONFIG_H__
#define __ADI_WDT_CONFIG_H__
#include <adi_global_config.h>
/** @defgroup WDT_Driver_Cfg WDT Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/*! @addtogroup  WDT_Driver_Cfg WDT Device Driver Configuration
 *  @{
 */

/************* WDT Driver configurations ***************/


/************* WDT controller static configurations ***************/

/* NOTE: these values are not the defaults,
         they are tweaked to optimize the static
         config of the wdt_test example.
         Select suitable settings as needed...
*/

/*! WDT Timer Reload Value\n
    Value used to reload the WDT count register after count expires.\n
   0-65535 - WDT reload value (default is 0x0100).
*/
#define ADI_WDT_LOAD_VALUE                                0x1000

/*! WDT Timer Mode\n
   Selects WDT operating mode.\n
   0 -  WDT operates in free-running mode.\n
   1 -  WDT operates in periodic mode (default).
*/
#define ADI_WDT_CONTROL_TIMER_MODE                             1

/*! WDT Device Enable\n
   Controls WDT device enable.\n
   0 -  WDT device disabled.\n
   1 -  WDT device enabled (default).
*/
#define ADI_WDT_CONTROL_DEVICE_ENABLE                          0

/*! WDT Clock Prescaler\n
   Controls WDT clock prescale.\n
   0 -  WDT operates at (source clock)/1.\n
   1 -  WDT operates at (source clock)/16.\n
   2 -  WDT operates at (source clock)/256 (default).\n
   3 -  WDT operates at (source clock)/4096.
*/
#define ADI_WDT_CONTROL_CLOCK_PRESCALER                        0

/*! WDT Timeout Mode\n
   Controls WDT timeout behavior.\n
   0 -  WDT issues RESET on timeout (default).\n
   1 -  WDT issues INTERRUPT on timeout
*/
#define ADI_WDT_CONTROL_TIMEOUT_MODE                           1

/*!
   Set this macro to 1 for to enable static controller initializations
   during the driver initialization routing.
   To eliminate static driver configuration, set this macro to 0.
*/
#define ADI_WDT_CFG_ENABLE_STATIC_CONFIG_SUPPORT           1

/************** Macro validation *****************************/

#if ( ADI_WDT_LOAD_VALUE  > 65535 )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_TIMER_MODE > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_DEVICE_ENABLE > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_CLOCK_PRESCALER > 3 )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_TIMEOUT_MODE > 1 )
#error "Invalid configuration"
#endif

/**@}*/

#endif /* __ADI_WDT_CONFIG_H__ */
