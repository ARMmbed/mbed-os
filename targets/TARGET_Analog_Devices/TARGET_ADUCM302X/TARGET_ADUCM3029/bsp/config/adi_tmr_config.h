/*!
 *****************************************************************************
   @file:    adi_tmr_config.h
   @brief:   Configuration options for General Purpose Timer driver.
             This is specific to the TMR driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 35142 $
   @date:    $Date: 2016-07-26 07:30:19 +0100 (Tue, 26 Jul 2016) $
  -----------------------------------------------------------------------------

Copyright (c) 2012-2016 Analog Devices, Inc.

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

#ifndef __ADI_TMR_CONFIG_H__
#define __ADI_TMR_CONFIG_H__
#include <adi_global_config.h>
/** @defgroup TMR_Driver_Cfg TMR Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/*! @addtogroup TMR_Driver_Cfg TMR Device Driver Configuration
 *  @{
 */

/*!
   Set this macro to 1 for to enable static timer  initializations
   during the driver initialization rutine #adi_tmr_Open().
   To eliminate static driver configuration, set this macro to 0.*/

#define ADI_TIMER_ENABLE_STATIC_CONFIG_SUPPORT  1
/*************************************************************
                 Timer 0 configurations
 *************************************************************/

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR0_CFG_PRESCALE_FACTOR                        2

/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR0_CFG_COUNT_UP                               0

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR0_CFG_MODE                                   1


/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select CLK Source 0.\n
    1 - Select CLK Source 1.\n
    2 - Select CLK Source 2.\n
    3 - Select CLK Source 3.
*/
#define TMR0_CFG_CLOCK_SOURCE                           0

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR0_CFG_RELOAD_CONTROL                         0


/*! Event to be captured. One of the selected 16 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 15. Please refer hardware reference manual to know
    which events can be catpured by a particular GP timer.
*/
#define TMR0_CFG_EVENT_CAPTURE                          0


/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR0_CFG_ENABLE_EVENT_CAPTURE                   0

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR0_CFG_ENABLE_SYNC_BYPASS                   0


/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR0_CFG_ENABLE_PRESCALE_RESET                 0


/*! Timer asynchrounous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing Asyncrounous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR0_CFG_ASYNC_LOAD_VALUE                       0XF000


/*! Timer load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR0_CFG_LOAD_VALUE                             0X0000


/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    It can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR0_CFG_ENABLE_PWM_MATCH_MODE                   0


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR0_CFG_PWM_IDLE_STATE                         0


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR0_CFG_PWM_MATCH_VALUE                        0


/*************************************************************
                Timer 1 configurations
 *************************************************************/

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR1_CFG_PRESCALE_FACTOR                        3

/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR1_CFG_COUNT_UP                               0

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR1_CFG_MODE                                   1


/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select CLK Source 0.\n
    1 - Select CLK Source 1.\n
    2 - Select CLK Source 2.\n
    3 - Select CLK Source 3.
*/
#define TMR1_CFG_CLOCK_SOURCE                           0

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR1_CFG_RELOAD_CONTROL                         0


/*! Event to be captured. One of the selected 16 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 15. Please refer hardware reference manual to know
    which events can be catpured by a particular GP timer.
*/
#define TMR1_CFG_EVENT_CAPTURE                          0


/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR1_CFG_ENABLE_EVENT_CAPTURE                   0

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR1_CFG_ENABLE_SYNC_BYPASS                   0

/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR1_CFG_ENABLE_PRESCALE_RESET                 0

/*! Timer asynchrounous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing Asyncrounous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR1_CFG_ASYNC_LOAD_VALUE                       0XF000


/*! Timer asynchrounous load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR1_CFG_LOAD_VALUE                             0XF00


/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    It can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR1_CFG_ENABLE_PWM_MATCH_MODE                       0


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR1_CFG_PWM_IDLE_STATE                         0


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR1_CFG_PWM_MATCH_VALUE                        0

/*************************************************************
                 Timer 2 configurations
 *************************************************************/

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR2_CFG_PRESCALE_FACTOR                        3

/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR2_CFG_COUNT_UP                               0

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR2_CFG_MODE                                   1


/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select CLK Source 0.\n
    1 - Select CLK Source 1.\n
    2 - Select CLK Source 2.\n
    3 - Select CLK Source 3.
*/
#define TMR2_CFG_CLOCK_SOURCE                           0

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR2_CFG_RELOAD_CONTROL                         0


/*! Event to be captured. One of the selected 16 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 15. Please refer hardware reference manual to know
    which events can be catpured by a particular GP timer.
*/
#define TMR2_CFG_EVENT_CAPTURE                          0


/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR2_CFG_ENABLE_EVENT_CAPTURE                   0

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR2_CFG_ENABLE_SYNC_BYPASS                   0

/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR2_CFG_ENABLE_PRESCALE_RESET                 0

/*! Timer asynchrounous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing Asyncrounous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR2_CFG_ASYNC_LOAD_VALUE                       0


/*! Timer asynchrounous load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR2_CFG_LOAD_VALUE                             0


/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    It can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR2_CFG_ENABLE_PWM_MATCH_MODE                  0


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR2_CFG_PWM_IDLE_STATE                         0


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR2_CFG_PWM_MATCH_VALUE                        0

/************** Macro validation *****************************/

/*************************************************************
                        GP Timer 0 validation macros.
**************************************************************/

#if (TMR0_CFG_PRESCALE_FACTOR > 3)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_COUNT_UP > 1)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_MODE > 1)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_CLOCK_SOURCE > 3)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_RELOAD_CONTROL > 1)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_EVENT_CAPTURE > 15)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_ENABLE_EVENT_CAPTURE > 1)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_ASYNC_LOAD_VALUE > 65535)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_LOAD_VALUE  > 65535)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_ENABLE_PWM_MATCH_MODE > 1)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_PWM_IDLE_STATE > 1)
#error "Invalid configuration"
#endif

#if (TMR0_CFG_PWM_MATCH_VALUE > 65535)
#error "Invalid configuration"
#endif

/*************************************************************
                        GP Timer 1 validation macros.
**************************************************************/

#if (TMR1_CFG_PRESCALE_FACTOR > 3)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_COUNT_UP > 1)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_MODE > 1)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_CLOCK_SOURCE > 3)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_RELOAD_CONTROL > 1)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_EVENT_CAPTURE > 15)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_ENABLE_EVENT_CAPTURE > 1)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_ASYNC_LOAD_VALUE > 65535)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_LOAD_VALUE  > 65535)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_ENABLE_PWM_MATCH_MODE > 1)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_PWM_IDLE_STATE > 1)
#error "Invalid configuration"
#endif

#if (TMR1_CFG_PWM_MATCH_VALUE > 65535)
#error "Invalid configuration"
#endif

/*************************************************************
                        GP Timer 2 validation macros.
**************************************************************/

#if (TMR2_CFG_PRESCALE_FACTOR > 3)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_COUNT_UP > 1)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_MODE > 1)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_CLOCK_SOURCE > 3)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_RELOAD_CONTROL > 1)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_EVENT_CAPTURE > 15)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_ENABLE_EVENT_CAPTURE > 1)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_ASYNC_LOAD_VALUE > 65535)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_LOAD_VALUE  > 65535)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_ENABLE_PWM_MATCH_MODE > 1)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_PWM_IDLE_STATE > 1)
#error "Invalid configuration"
#endif

#if (TMR2_CFG_PWM_MATCH_VALUE > 65535)
#error "Invalid configuration"
#endif


/*! @} */

#endif /* __ADI_TMR_CONFIG_H__ */
