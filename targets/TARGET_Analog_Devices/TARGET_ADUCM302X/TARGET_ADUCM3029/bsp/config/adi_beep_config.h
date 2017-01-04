/*!
 *****************************************************************************
   @file:    adi_beep_config.h
   @brief:   Configuration options for BEEP driver.
             This is specific to the BEEP driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 33205 $
   @date:    $Date: 2016-01-11 10:46:07 +0000 (Mon, 11 Jan 2016) $
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

#ifndef __ADI_BEEP_CONFIG_H__
#define __ADI_BEEP_CONFIG_H__
#include <adi_global_config.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   IAR compiler supports longer identifiers.
 */
#pragma diag_suppress=Pm009
#endif /* __ICCARM__ */

/** @defgroup BEEP_Driver_Cfg BEEP Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/*! \addtogroup BEEP_Driver_Cfg BEEP Device Driver Configuration
 *  @{
 */

/************* BEEP Driver configurations ***************/

/************* BEEP controller static configurations ***************/

/* CONFIGURATION REGISTER */

/*! Configure beeper to generate interrupts on sequence end event.\n
    0 -  No interrupt.\n
    1 -  Generate Sequence End interrupt. */
#define ADI_BEEP_CFG_INTERRUPT_ON_SEQUENCE_END                 1

/*! Configure beeper to generate interrupts on sequence near end event.\n
    0 -  No interrupt.\n
    1 -  Generate Sequence Near End interrupt. */
#define ADI_BEEP_CFG_INTERRUPT_ON_SEQUENCE_NEAR_END            0

/*! Configure beeper to generate interrupts on ToneB end event.\n
    0 -  No interrupt.\n
    1 -  Generate ToneB End interrupt. */
#define ADI_BEEP_CFG_INTERRUPT_ON_TONEB_END                    0

/*! Configure beeper to generate interrupts on ToneB start event.\n
    0 -  No interrupt.\n
    1 -  Generate ToneB Start interrupt. */
#define ADI_BEEP_CFG_INTERRUPT_ON_TONEB_START                  0

/*! Configure beeper to generate interrupts on ToneA end event.\n
    0 -  No interrupt.\n
    1 -  Generate ToneA End interrupt. */
#define ADI_BEEP_CFG_INTERRUPT_ON_TONEA_END                    0

/*! Configure beeper to generate interrupts on ToneA start event.\n
    0 -  No interrupt.\n
    1 -  Generate ToneA Start interrupt. */
#define ADI_BEEP_CFG_INTERRUPT_ON_TONEA_START                  1

/*! Configure beeper disable.\n
    0 -  Beeper enabled.\n
    1 -  Beeper disabled. */
#define ADI_BEEP_CFG_BEEPER_DISABLE                            0

/*! Configure beeper for pulse/repeat mode.
    (assume sequential mode, but need to set length dynamically)\n
    0     -  Pulse mode.\n
    1-255 -  Sequence mode repeat count. */
#define ADI_BEEP_CFG_SEQUENCE_REPEAT_VALUE                     5



/* STATUS REGISTER */

/*! Clear the sequence end interrupt bit.\n
    0 -  Do not clear the interrupt.\n
    1 -  Clear the interrupt. */
#define ADI_BEEP_STAT_CLEAR_SEQUENCE_END_INTERRUPT             1

/*! Clear the sequence near end interrupt bit.\n
    0 -  Do not clear the interrupt.\n
    1 -  Clear the interrupt. */
#define ADI_BEEP_STAT_CLEAR_SEQUENCE_NEAR_END_INTERRUPT        0

/*! Clear the ToneB end interrupt bit.\n
    0 -  Do not clear the interrupt.\n
    1 -  Clear the interrupt. */
#define ADI_BEEP_STAT_CLEAR_TONEB_END_INTERRUPT                0

/*! Clear the ToneB start interrupt bit.\n
    0 -  Do not clear the interrupt.\n
    1 -  Clear the interrupt. */
#define ADI_BEEP_STAT_CLEAR_TONEB_START_INTERRUPT              0

/*! Clear the ToneA end interrupt bit.\n
    0 -  Do not clear the interrupt.\n
    1 -  Clear the interrupt. */
#define ADI_BEEP_STAT_CLEAR_TONEA_END_INTERRUPT                1

/*! Clear the ToneA start interrupt bit.\n
    0 -  Do not clear the interrupt.\n
    1 -  Clear the interrupt. */
#define ADI_BEEP_STAT_CLEAR_TONEA_START_INTERRUPT              1



/* TONEA CONTROL REGISTER */

/*! Initial ToneA Disable.\n
    0 -  ToneA Enabled.\n
    1 -  ToneA Disabled. */
#define ADI_BEEP_TONEA_DISABLE                                 0

/*! Initial ToneA Frequency.\n
    0-3   -  Rest Tone (no oscillation).\n
    4-127 -  Oscillate at 32kHz/freq Hz. */
#define ADI_BEEP_TONEA_FREQUENCY                               ADI_BEEP_FREQ_D6

/*! Initial ToneA Duration.\n
    0-254 -  Play for 4ms*duration.\n
    255   -  Play for infinite duration. */
#define ADI_BEEP_TONEA_DURATION                                ADI_BEEP_DUR_16_32



/* TONEB CONTROL REGISTER */

/*! Initial ToneB Disable.\n
    0 -  ToneB Enabled.\n
    1 -  ToneB Disabled. */
#define ADI_BEEP_TONEB_DISABLE                                 0

/*! Initial ToneB Frequency.\n
    0-3   -  Rest Tone (no oscillation).\n
    4-127 -  Oscillate at 32kHz/freq Hz. */
#define ADI_BEEP_TONEB_FREQUENCY                               ADI_BEEP_FREQ_REST

/*! Initial ToneB Duration.\n
    0-254 -  Play for 4ms*duration.\n
    255   -  Play for infinite duration. */
#define ADI_BEEP_TONEB_DURATION                                1

/*!
   Set this macro to 1 for to enable static controller initializations
   during the driver initialization routing.
   To eliminate static driver configuration, set this macro to 0.
*/
#define ADI_BEEP_CFG_ENABLE_STATIC_CONFIG_SUPPORT              1


/************** Macro validation *****************************/

#if ( ADI_BEEP_CFG_INTERRUPT_ON_SEQUENCE_END > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_INTERRUPT_ON_SEQUENCE_NEAR_END > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_INTERRUPT_ON_TONEB_END > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_INTERRUPT_ON_TONEB_START > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_INTERRUPT_ON_TONEA_END > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_INTERRUPT_ON_TONEA_START > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_BEEPER_DISABLE > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_CFG_SEQUENCE_REPEAT_VALUE > 255 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_STAT_CLEAR_SEQUENCE_END_INTERRUPT > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_STAT_CLEAR_SEQUENCE_NEAR_END_INTERRUPT > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_STAT_CLEAR_TONEB_END_INTERRUPT > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_STAT_CLEAR_TONEB_START_INTERRUPT > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_STAT_CLEAR_TONEA_END_INTERRUPT > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_STAT_CLEAR_TONEA_START_INTERRUPT > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEA_DISABLE  > 1 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEB_DISABLE > 1 )
#error "Invalid configuration"
#endif

#ifdef __ICCARM__
/*
* Pm085 (rule 19.11): identifiers in pre-processor directives should be defined before use
*   The macros in the the following #if directives are defined to enum constants by default.
*/
#pragma diag_suppress=Pm085
#endif /* __ICCARM__ */

#if ( ADI_BEEP_TONEA_FREQUENCY  > 127 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEA_DURATION > 255 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEB_FREQUENCY > 127 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEB_DURATION  >  255 )
#error "Invalid configuration"
#endif

#ifdef __ICCARM__
#pragma diag_default=Pm009,Pm085
#endif /* __ICCARM__ */

/*! @} */

#endif /* __ADI_BEEP_CONFIG_H__ */
