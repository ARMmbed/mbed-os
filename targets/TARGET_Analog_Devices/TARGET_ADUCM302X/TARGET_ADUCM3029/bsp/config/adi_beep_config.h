/*!
 *****************************************************************************
   @file:    adi_beep_config.h
   @brief:   Configuration options for BEEP driver.
             This is specific to the BEEP driver and will be included by the driver.
             It is not required for the application to include this header file.
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

#ifndef ADI_BEEP_CONFIG_H
#define ADI_BEEP_CONFIG_H
#include <adi_global_config.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   IAR compiler supports longer identifiers.
 */
#pragma diag_suppress=Pm009
#endif /* __ICCARM__ */

/** @addtogroup BEEP_Driver_Config Static Configuration
 *  @ingroup BEEP_Driver
 *  @{
 */

/************* BEEP Driver configurations ***************/
/*! Enable the inclusion of adi_beep_PlaySequence().  This    \n
    API requires more data in the device structures to manage \n
    the longer playing sequences, along with extra code in    \n
    the interrupt handler.                                    \n
    0 -  adi_beep_PlaySequence() omitted.\n
    1 -  adi_beep_PlaySequence() is included. */
#define ADI_BEEP_INCLUDE_PLAY_SEQUENCE                         1
   
/************* BEEP controller static configurations ***************/

/*! Configure beeper disable.\n
    0 -  Beeper enabled.\n
    1 -  Beeper disabled. */
#define ADI_BEEP_CFG_BEEPER_DISABLE                            0

/*! Configure beeper sequence, when using static configuration. \n
    0     -  Single note (Tone A only).\n
    1-255 -  Sequence mode repeat count (Tone A then B sequentially). */
#define ADI_BEEP_CFG_SEQUENCE_REPEAT_VALUE                     5


/* TONEA CONTROL REGISTER */

/*! Initial ToneA Disable.\n
    0 -  ToneA Enabled.\n
    1 -  ToneA Disabled. */
#define ADI_BEEP_TONEA_DISABLE                                 0

/*! Initial ToneA Frequency.\n
    0-3   -  Rest Tone (no oscillation).\n
    4-127 -  Oscillate at 32kHz/freq Hz. */
#define ADI_BEEP_TONEA_FREQUENCY                               20

/*! Initial ToneA Duration.\n
    0-254 -  Play for 4ms*duration.\n
    255   -  Play for infinite duration. */
#define ADI_BEEP_TONEA_DURATION                                2



/* TONEB CONTROL REGISTER */

/*! Initial ToneB Disable.\n
    0 -  ToneB Enabled.\n
    1 -  ToneB Disabled. */
#define ADI_BEEP_TONEB_DISABLE                                 0

/*! Initial ToneB Frequency. \n
    0-3   -  Rest Tone (no oscillation).\n
    4-127 -  Oscillate at 32kHz/freq Hz. */
#define ADI_BEEP_TONEB_FREQUENCY                               50

/*! Initial ToneB Duration.\n
    0-254 -  Play for 4ms*duration.\n
    255   -  Play for infinite duration. */
#define ADI_BEEP_TONEB_DURATION                                2



#ifdef __ICCARM__
/*
* Pm085 (rule 19.11): identifiers in pre-processor directives should be defined before use
*   The macros in the the following #if directives are defined to enum constants by default.
*/
#pragma diag_suppress=Pm085
#endif /* __ICCARM__ */

#if (ADI_BEEP_TONEA_DISABLE > 1)
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEA_FREQUENCY  > 127 )
#error "Invalid configuration"
#endif

#if ( ADI_BEEP_TONEA_DURATION > 255 )
#error "Invalid configuration"
#endif

#if (ADI_BEEP_TONEB_DISABLE > 1)
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

#endif /* ADI_BEEP_CONFIG_H */
