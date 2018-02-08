/*! *****************************************************************************
 * @file    adi_tmr_config.h
 * @brief   GP and RGB timer device driver configuration
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


#ifndef ADI_TMR_CONFIG_H
#define ADI_TMR_CONFIG_H


#include <adi_global_config.h>


/** @addtogroup TMR_Driver_Config Static Configuration
 *  @ingroup TMR_Driver
 *  @{
 */


/*! Static configuration allows all 3 GP timers and the RGB timer to be configured
    with the parameters in this file by simply calling #adi_tmr_Init. The user can
    then call any of the configuration API's to override the static configuration,
    or simply call #adi_tmr_Enable to start the timer. Since all of these parameters
    must be stored in arrays for abstraction, using static configuration will increase the 
    data footprint. If the user doesn't call any of the runtime configuration API's, the
    linker will throw them out and the code footprint will be reduced significantly. Using
    static configuration also reduces cycle count and simplifies the user application. 
    Static configuration should be used if the timers need to be configured once and do not 
    need to be changed during the system lifetime.

    0 - Disable static confiscation support. User must call #adi_tmr_ConfigTimer and other
        configuration API's after calling #adi_tmr_Init and prior to calling #adi_tmr_Enable
        in order to set up the timer.

    1 - Enable static configuration support. The timer registers will be set based on the 
        settings in this file when #adi_tmr_Init is called.
*/
#define ADI_TIMER_ENABLE_STATIC_CONFIG_SUPPORT          (0u)


/*************************************************************
                        GP Timer 0 Configuration
 *************************************************************/

 /** @addtogroup GPTimer0_Driver_Config GP Timer 0 Static Configuration
 *  @ingroup TMR_Driver_Config
 *  @{
 */

 
/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR0_CFG_COUNT_UP                               (0u)

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR0_CFG_MODE                                   (1u)

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR0_CFG_PRESCALE_FACTOR                        (0u)

/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select PCLK\n
    1 - Select HFOSC\n
    2 - Select LFOSC\n
    3 - Select LFXTAL
*/
#define TMR0_CFG_CLOCK_SOURCE                           (0u)

/*! Timer load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR0_CFG_LOAD_VALUE                             (0x8F9Cu)

/*! Timer asynchrounous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing Asynchronous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR0_CFG_ASYNC_LOAD_VALUE                       (0x8F9Cu)

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR0_CFG_ENABLE_RELOADING                       (0u)

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR0_CFG_ENABLE_SYNC_BYPASS                     (0u)

/*************************************************************
                        GP Timer 0 Event Configuration
 *************************************************************/

/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR0_CFG_ENABLE_EVENT_CAPTURE                   (1u)

/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR0_CFG_ENABLE_PRESCALE_RESET                  (0u)    

/*! Event to be captured. One of the selected 40 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 39. Please refer hardware reference manual to know
    which events can be captured by a particular GP timer.
*/
#define TMR0_CFG_EVENT_CAPTURE                          (27u)

/*************************************************************
                        GP Timer 0 PWM0 Configuration
 *************************************************************/

/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    Toggle mode provides a 50% duty cycle and match mode provides a configurable
    duty cycle by using the match value. This vlaue can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR0_CFG_ENABLE_PWM0_MATCH_MODE                 (1u)


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR0_CFG_PWM0_IDLE_STATE                        (1u)


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR0_CFG_PWM0_MATCH_VALUE                       (0x0E5Cu)

/*! @} */


/*************************************************************
                        GP Timer 1 Configuration
 *************************************************************/

 /** @addtogroup GPTimer1_Driver_Config GP Timer 1 Static Configuration
 *  @ingroup TMR_Driver_Config
 *  @{
 */

 
/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR1_CFG_COUNT_UP                               (0u)

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR1_CFG_MODE                                   (1u)

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR1_CFG_PRESCALE_FACTOR                        (0u)

/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select PCLK\n
    1 - Select HFOSC\n
    2 - Select LFOSC\n
    3 - Select LFXTAL
*/
#define TMR1_CFG_CLOCK_SOURCE                           (0u)

/*! Timer load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR1_CFG_LOAD_VALUE                             (0x23E7u)

/*! Timer asynchronous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing Asynchronous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR1_CFG_ASYNC_LOAD_VALUE                       (0x23E7u)

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR1_CFG_ENABLE_RELOADING                       (0u)

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR1_CFG_ENABLE_SYNC_BYPASS                     (0u)


/*************************************************************
                        GP Timer 1 Event Configuration
 *************************************************************/
 
/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR1_CFG_ENABLE_EVENT_CAPTURE                   (1u)

/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR1_CFG_ENABLE_PRESCALE_RESET                  (0u)

/*! Event to be captured. One of the selected 40 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 39. Please refer hardware reference manual to know
    which events can be captured by a particular GP timer.
*/
#define TMR1_CFG_EVENT_CAPTURE                          (28u)

/*************************************************************
                        GP Timer 1 PWM0 Configuration
 *************************************************************/

/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    Toggle mode provides a 50% duty cycle and match mode provides a configurable
    duty cycle by using the match value. This value can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR1_CFG_ENABLE_PWM0_MATCH_MODE                 (1u)


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR1_CFG_PWM0_IDLE_STATE                        (1u)


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR1_CFG_PWM0_MATCH_VALUE                       (0x08F9u)

/*! @} */

/*************************************************************
                        GP Timer 2 Configuration
 *************************************************************/

 /** @addtogroup GPTimer2_Driver_Config GP Timer 2 Static Configuration
 *  @ingroup TMR_Driver_Config
 *  @{
 */

 
/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR2_CFG_COUNT_UP                               (0u)

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR2_CFG_MODE                                   (1u)

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR2_CFG_PRESCALE_FACTOR                        (0u)

/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select PCLK\n
    1 - Select HFOSC\n
    2 - Select LFOSC\n
    3 - Select LFXTAL
*/
#define TMR2_CFG_CLOCK_SOURCE                           (0u)

/*! Timer load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR2_CFG_LOAD_VALUE                             (0x0E5Cu)

/*! Timer asynchronous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing Asynchronous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR2_CFG_ASYNC_LOAD_VALUE                       (0x0E5Cu)

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR2_CFG_ENABLE_RELOADING                       (0u)

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR2_CFG_ENABLE_SYNC_BYPASS                     (0u)

/*************************************************************
                        GP Timer 2 Event Configuration
 *************************************************************/

/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR2_CFG_ENABLE_EVENT_CAPTURE                   (1u)

/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR2_CFG_ENABLE_PRESCALE_RESET                  (0u)

/*! Event to be captured. One of the selected 40 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 39. Please refer hardware reference manual to know
    which events can be captured by a particular GP timer.
*/
#define TMR2_CFG_EVENT_CAPTURE                          (27u)

/*************************************************************
                        GP Timer 2 PWM0 Configuration
 *************************************************************/

/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    Toggle mode provides a 50% duty cycle and match mode provides a configurable
    duty cycle by using the match value. This value can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR2_CFG_ENABLE_PWM0_MATCH_MODE                 (1u)


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR2_CFG_PWM0_IDLE_STATE                        (1u)


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR2_CFG_PWM0_MATCH_VALUE                       (0x02DFu)

/*! @} */


/*************************************************************
                        RGB Timer Configuration
 *************************************************************/

/** @addtogroup RGBTimer_Driver_Config RGB Timer Static Configuration
 *  @ingroup TMR_Driver_Config
 *  @{
 */

 
/*! Count up or down. Used to control whether the timer increments (counts up)
    or decrements (counts down) the Up/Down counter, it can be set to\n
    0 -  Timer is set to count down.\n
    1 -  Timer is set to count up.
*/
#define TMR3_CFG_COUNT_UP                               (0u)

/*! Timer mode. Used to control whether the timer runs in periodic or
    free running mode, it can be set to\n
    0 -  Timer is in free running mode.\n
    1 -  Timer is in periodic mode.
*/
#define TMR3_CFG_MODE                                   (1u)

/*! Prescale factor. Controls the prescaler division factor
    to the timer's selected clock. It can be set to\n

    0 - source_clock/[1 or 4]\n
    1 - source_clock/16\n
    2 - source_clock/64\n
    3 - source_clock/256
*/
#define TMR3_CFG_PRESCALE_FACTOR                        (0u)

/*! Timer clock source. Used to select a timer clock from the four
    available clock sources, it can be set to\n
    0 - Select PCLK\n
    1 - Select HFOSC\n
    2 - Select LFOSC\n
    3 - Select LFXTAL
*/
#define TMR3_CFG_CLOCK_SOURCE                           (0u)

/*! Timer load value. The Up/Down counter is periodically loaded with this
    value if periodic mode is selected. LOAD writes during Up/Down counter timeout events
    are delayed until the event has passed. It can be set to any value from 0 to 65535.

*/
#define TMR3_CFG_LOAD_VALUE                             (0x47CEu)

/*! Timer asynchronous load value. The Up/Down counter is periodically loaded with
    this value if periodic mode is selected. Writing asynchronous Load value takes
    advantage of having the timer run on PCLK by bypassing clock synchronization
    logic otherwise required. It can be set to any value from 0 to 65535.

*/
#define TMR3_CFG_ASYNC_LOAD_VALUE                       (0x47CEu)

/*! Reload control. This allows the user to select whether the Up/Down counter should be
    reset only on a timeout event or also when interrupt is cleared. It can be set to\n
    0 - Up/down counter is only reset on a time out event.\n
    1 - Resets the up/down counter when the interrupt is cleared.
*/
#define TMR3_CFG_ENABLE_RELOADING                       (0u)

/*! Enable or disable Synchronization bypass\n
    0 - Disable Synchronization bypass.\n
    1 - Enable Synchronization bypass.
*/
#define TMR3_CFG_ENABLE_SYNC_BYPASS                     (0u)

/*************************************************************
                        RGB Timer Event Configuration
 *************************************************************/

/*! Enable or disable event capture. It can be set to\n
    0 - Disable event capturing.\n
    1 - Enable event capturing.
*/
#define TMR3_CFG_ENABLE_EVENT_CAPTURE                   (1u)

/*! Enable or disable prescale reset\n
    0 - Disable rescale reset.\n
    1 - Enable rescale reset.
*/
#define TMR3_CFG_ENABLE_PRESCALE_RESET                  (0u)

/*! Event to be captured. One of the selected 40 events associated
    with a general purpose time can be captured. It can be set to
    a value of 0 - 39. Please refer hardware reference manual to know
    which events can be captured by a particular GP timer.
*/
#define TMR3_CFG_EVENT_CAPTURE                          (28u)

/*************************************************************
                        RGB Timer PWM0 Configuration
 *************************************************************/

/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    Toggle mode provides a 50% duty cycle and match mode provides a configurable
    duty cycle by using the match value. This value can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR3_CFG_ENABLE_PWM0_MATCH_MODE                 (1u)


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR3_CFG_PWM0_IDLE_STATE                        (1u)


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR3_CFG_PWM0_MATCH_VALUE                       (0x23E7u)

/*************************************************************
                        RGB Timer PWM1 Configuration
 *************************************************************/

/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    Toggle mode provides a 50% duty cycle and match mode provides a configurable
    duty cycle by using the match value. This value can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR3_CFG_ENABLE_PWM1_MATCH_MODE                 (0u)


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR3_CFG_PWM1_IDLE_STATE                        (0u)


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR3_CFG_PWM1_MATCH_VALUE                       (0u)

/*************************************************************
                        RGB Timer PWM2 Configuration
 *************************************************************/

/*! Timer PWM Enable Match. This will control PWM operation mode of the timer.
    Toggle mode provides a 50% duty cycle and match mode provides a configurable
    duty cycle by using the match value. This value can be set to\n
    0 - PWM in toggle mode.\n
    1 - PWM in match mode.
*/
#define TMR3_CFG_ENABLE_PWM2_MATCH_MODE                 (0u)


/*! Timer PWM Idle state. This will control PWM idle state. It can be set to\n
    0 - PWM idles low.\n
    1 - PWM idles high.
*/
#define TMR3_CFG_PWM2_IDLE_STATE                        (0u)


/*! PWM Match value. The value is used when the PWM is operating in match mode.
    The PWM output is asserted when the Up/Down counter is equal to this match value.
    PWM output is deasserted again when a timeout event occurs.
    If the match value is never reached, or occurs simultaneous to a timeout event,
    the PWM output remains idle. It can be any value from 0 to 65535.
*/
#define TMR3_CFG_PWM2_MATCH_VALUE                       (0u)

/*! @} */

/*************************************************************
                        GP Timer 0 Macro Validation
**************************************************************/

#if TMR0_CFG_COUNT_UP > 1u
#error "Invalid configuration"
#endif

#if TMR0_CFG_MODE > 1u
#error "Invalid configuration"
#endif

#if TMR0_CFG_PRESCALE_FACTOR > 3u
#error "Invalid configuration"
#endif

#if TMR0_CFG_CLOCK_SOURCE > 3u
#error "Invalid configuration"
#endif

#if TMR0_CFG_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR0_CFG_ASYNC_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR0_CFG_ENABLE_RELOADING > 1u
#error "Invalid configuration"
#endif

#if TMR0_CFG_ENABLE_SYNC_BYPASS > 1u
#error "Invalid configuration"
#endif

#if TMR0_CFG_ENABLE_PRESCALE_RESET > 1u
#error "Invalid configuration"
#endif

#if TMR0_CFG_ENABLE_EVENT_CAPTURE > 1u
#error "Invalid configuration"
#endif

#if TMR0_CFG_EVENT_CAPTURE > 39u
#error "Invalid configuration"
#endif

#if TMR0_CFG_ENABLE_PWM0_MATCH_MODE > 1u
#error "Invalid configuration"
#endif                   

#if TMR0_CFG_PWM0_IDLE_STATE > 1u
#error "Invalid configuration"
#endif  

#if TMR0_CFG_PWM0_MATCH_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif 

/*************************************************************
                        GP Timer 1 Macro Validation
**************************************************************/

#if TMR1_CFG_COUNT_UP > 1u
#error "Invalid configuration"
#endif

#if TMR1_CFG_MODE > 1u
#error "Invalid configuration"
#endif

#if TMR1_CFG_PRESCALE_FACTOR > 3u
#error "Invalid configuration"
#endif

#if TMR1_CFG_CLOCK_SOURCE > 3u
#error "Invalid configuration"
#endif

#if TMR1_CFG_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR1_CFG_ASYNC_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR1_CFG_ENABLE_RELOADING > 1u
#error "Invalid configuration"
#endif

#if TMR1_CFG_ENABLE_SYNC_BYPASS > 1u
#error "Invalid configuration"
#endif

#if TMR1_CFG_ENABLE_PRESCALE_RESET > 1u
#error "Invalid configuration"
#endif

#if TMR1_CFG_ENABLE_EVENT_CAPTURE > 1u
#error "Invalid configuration"
#endif

#if TMR1_CFG_EVENT_CAPTURE > 39u
#error "Invalid configuration"
#endif

#if TMR1_CFG_ENABLE_PWM0_MATCH_MODE > 1u
#error "Invalid configuration"
#endif                   

#if TMR1_CFG_PWM0_IDLE_STATE > 1u
#error "Invalid configuration"
#endif  

#if TMR1_CFG_PWM0_MATCH_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif 

/*************************************************************
                        GP Timer 2 Macro Validation
**************************************************************/

#if TMR2_CFG_COUNT_UP > 1u
#error "Invalid configuration"
#endif

#if TMR2_CFG_MODE > 1u
#error "Invalid configuration"
#endif

#if TMR2_CFG_PRESCALE_FACTOR > 3u
#error "Invalid configuration"
#endif

#if TMR2_CFG_CLOCK_SOURCE > 3u
#error "Invalid configuration"
#endif

#if TMR2_CFG_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR2_CFG_ASYNC_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR2_CFG_ENABLE_RELOADING > 1u
#error "Invalid configuration"
#endif

#if TMR2_CFG_ENABLE_SYNC_BYPASS > 1u
#error "Invalid configuration"
#endif

#if TMR2_CFG_ENABLE_PRESCALE_RESET > 1u
#error "Invalid configuration"
#endif

#if TMR2_CFG_ENABLE_EVENT_CAPTURE > 1u
#error "Invalid configuration"
#endif

#if TMR2_CFG_EVENT_CAPTURE > 39u
#error "Invalid configuration"
#endif

#if TMR2_CFG_ENABLE_PWM0_MATCH_MODE > 1u
#error "Invalid configuration"
#endif                   

#if TMR2_CFG_PWM0_IDLE_STATE > 1u
#error "Invalid configuration"
#endif  

#if TMR2_CFG_PWM0_MATCH_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif 

/*************************************************************
                        RGB Timer Macro Validation
**************************************************************/

#if TMR3_CFG_COUNT_UP > 1u
#error "Invalid configuration"
#endif

#if TMR3_CFG_MODE > 1u
#error "Invalid configuration"
#endif

#if TMR3_CFG_PRESCALE_FACTOR > 3u
#error "Invalid configuration"
#endif

#if TMR3_CFG_CLOCK_SOURCE > 3u
#error "Invalid configuration"
#endif

#if TMR3_CFG_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR3_CFG_ASYNC_LOAD_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif

#if TMR3_CFG_ENABLE_RELOADING > 1u
#error "Invalid configuration"
#endif

#if TMR3_CFG_ENABLE_SYNC_BYPASS > 1u
#error "Invalid configuration"
#endif

#if TMR3_CFG_ENABLE_PRESCALE_RESET > 1u
#error "Invalid configuration"
#endif

#if TMR3_CFG_ENABLE_EVENT_CAPTURE > 1u
#error "Invalid configuration"
#endif

#if TMR3_CFG_EVENT_CAPTURE > 39u
#error "Invalid configuration"
#endif

#if TMR3_CFG_ENABLE_PWM0_MATCH_MODE > 1u
#error "Invalid configuration"
#endif                   

#if TMR3_CFG_PWM0_IDLE_STATE > 1u
#error "Invalid configuration"
#endif  

#if TMR3_CFG_PWM0_MATCH_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif 

#if TMR3_CFG_ENABLE_PWM1_MATCH_MODE > 1u
#error "Invalid configuration"
#endif                   

#if TMR3_CFG_PWM1_IDLE_STATE > 1u
#error "Invalid configuration"
#endif  

#if TMR3_CFG_PWM1_MATCH_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif 

#if TMR3_CFG_ENABLE_PWM2_MATCH_MODE > 1u
#error "Invalid configuration"
#endif                   

#if TMR3_CFG_PWM2_IDLE_STATE > 1u
#error "Invalid configuration"
#endif  

#if TMR3_CFG_PWM2_MATCH_VALUE > 0xFFFFu
#error "Invalid configuration"
#endif 

/*! @} */


#endif /* ADI_TMR_CONFIG_H */
