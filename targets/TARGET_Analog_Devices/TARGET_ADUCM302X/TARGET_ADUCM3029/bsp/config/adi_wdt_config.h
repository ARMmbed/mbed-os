/*! *****************************************************************************
 * @file    adi_wdt_config.h
 * @brief   WDT device driver configuration
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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


#ifndef ADI_WDT_CONFIG_H
#define ADI_WDT_CONFIG_H


/** @addtogroup WDT_Driver_Config Static Configuration
 *  @ingroup WDT_Driver
 *  @{
 */


/************* WDT Static Configuration ***************/

/*! WDT Timer Reload Value\n
    Value used to reload the WDT count register after count expires.\n
   0-65535 - WDT reload value (default is 0x0100).
*/
#define ADI_WDT_LOAD_VALUE                                  (0x1000u)

/*! WDT Timer Mode\n
   Selects WDT operating mode.\n
   0 -  WDT operates in free-running mode.\n
   1 -  WDT operates in periodic mode (default).
*/
#define ADI_WDT_CONTROL_TIMER_MODE                               (1u)

/*! WDT Clock Prescaler\n
   Controls WDT clock prescale.\n
   0 -  WDT operates at (source clock)/1.\n
   1 -  WDT operates at (source clock)/16.\n
   2 -  WDT operates at (source clock)/256 (default).\n
*/
#define ADI_WDT_CONTROL_CLOCK_PRESCALER                          (2u)

/*! WDT Timeout Mode\n
   Controls WDT timeout behaviour.\n
   0 -  WDT issues RESET on timeout (default).\n
   1 -  WDT issues INTERRUPT on timeout.
*/
#define ADI_WDT_CONTROL_TIMEOUT_MODE                             (0u)

/*! WDT Power Mode Disable\n
   Controls WDT countdown in hibernate or halted mode.\n
   0 -  WDT continues to count down when core is halted or in hibernate.\n
   1 -  WDT pauses count down when core is halted or in hibernate (default).\n
*/
#define ADI_WDT_CONTROL_POWER_MODE                               (1u)

/************** Macro Validation *****************************/

#if ( ADI_WDT_LOAD_VALUE  > 65535u )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_TIMER_MODE > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_CLOCK_PRESCALER > 2u )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_TIMEOUT_MODE > 1u )
#error "Invalid configuration"
#endif

#if ( ADI_WDT_CONTROL_POWER_MODE > 1u )
#error "Invalid configuration"
#endif

/**
 *  @}
 */

#endif /* ADI_WDT_CONFIG_H */
