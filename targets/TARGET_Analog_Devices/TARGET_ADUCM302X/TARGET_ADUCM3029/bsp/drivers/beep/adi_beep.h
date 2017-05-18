/*! *****************************************************************************
 * @file    adi_beep.h
 * @brief   Main include file for BEEP device driver definitions
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
/** @addtogroup BEEP_Driver BEEP Driver
*  @{
*/
#ifndef ADI_BEEP_H
#define ADI_BEEP_H

#include "adi_processor.h"

#include <adi_beep_config.h>
#include <adi_callback.h>
#include <rtos_map/adi_rtos_map.h> 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  
/*! Amount of memory(In bytes) required by the Beep device driver for managing the operation.
 * This memory is completely owned by the driver till the end of the operation.
 */
#if ADI_BEEP_INCLUDE_PLAY_SEQUENCE == 1
/*! @hideinitializer Indicates the size of the BEEP memory to be used */
#define ADI_BEEP_MEMORY_SIZE        (20u + ADI_SEM_SIZE)
#else
/*! @hideinitializer Indicates the size of the BEEP memory to be used */
#define ADI_BEEP_MEMORY_SIZE        (12u + ADI_SEM_SIZE)
#endif
   
/*!
 * \enum ADI_BEEP_RESULT
 * Beeper API return codes
 */
typedef enum
{
    ADI_BEEP_SUCCESS = 0,            /*!<  No Error, API suceeded */

    ADI_BEEP_FAILURE,                /*!<  An unknown error was detected */
    ADI_BEEP_ALREADY_INITIALIZED,    /*!<  BEEP is already initialized */
    ADI_BEEP_BAD_DEV_HANDLE,         /*!<  Invalid device handle passed */
    ADI_BEEP_BAD_DEV_ID,             /*!<  Asking to initialize an unknown device num */
    ADI_BEEP_NOT_INITIALIZED,        /*!<  BEEP not yet initialized */
    ADI_BEEP_PARAM_OUT_OF_RANGE,     /*!<  Parameter is out of range. */
    ADI_BEEP_INVALID_COUNT,          /*!<  Invalid count for supplied beep sequence */
    ADI_BEEP_NULL_PTR,               /*!<  Null pointer supplied. */
    ADI_BEEP_SEMAPHORE_FAILED,       /*!<  BEEP semaphore failure. */
} ADI_BEEP_RESULT;


/*!
 * \enum ADI_BEEP_DEV_ID
 *  @brief  Beeper Device IDs.
 *  @details List of all Beeper Device IDs for the current part
 */
typedef enum
{
    ADI_BEEP_DEVID_0 = 0,       /*!<  BEEP Timer Device 0 */
    ADI_BEEP_MAX_DEVID          /*!<  max number of BEEP devices */
} ADI_BEEP_DEV_ID;

/*!
 * \enum ADI_BEEP_INTERRUPT
 *  @brief  Beeper Interrupt Bits.
 *  @details List of all Beeper interrupt (enables and status) bits.
 */
typedef enum
{
    ADI_BEEP_INTERRUPT_SEQUENCE_END            = BITM_BEEP_CFG_SEQATENDIRQ,   /*!<  Beeper sequence has finished      */
    ADI_BEEP_INTERRUPT_NOTE_END                = BITM_BEEP_CFG_AENDIRQ,       /*!<  Beeper note has finished        */
} ADI_BEEP_INTERRUPT;


#define LFCLK_FREQ           32768.0f                                         /*!< Beeper main clock frequency.            */
#define FREQUENCY_ENCODE(x) (uint8_t)(LFCLK_FREQ/(x) + 0.5f)                  /*!< Beeper tone frequency encoder macro     */

/*!
 * \enum ADI_BEEP_NOTE_FREQUENCY
 *  @brief  Beeper tone frequency list.
 *  @details List of possible Beeper tone frequencies.
 */
typedef enum {
	/* Constants are pre-computed note frequencies (Hz).                 */
	/* See http://www.phy.mtu.edu/~suits/notefreqs.html.                 */
	/* Encodings are clock divider values for that note.                 */
	/* Flats are the same as the lower sharp, so only sharps are listed. */
	/* Even though octaves are simple frequency doublings/halvings       */
	/* of adjuacient octaves, we pre-compute each constant (as opposed   */
	/* to halving/doubling the encodings between octaves) to             */
	/* minimize repeated doubling/halving errors across all octaves.     */
	/* !!!ALL ENCODINGS MUST BE IN THE RANGE 4-127!!!                    */

	ADI_BEEP_FREQ_REST = (0),                       /*!< silence */

	ADI_BEEP_FREQ_C4  = FREQUENCY_ENCODE(261.63f),  /*!< Middle C (lowest representable frequency @ 32KHz) */
	ADI_BEEP_FREQ_Cs4 = FREQUENCY_ENCODE(277.18f),
	ADI_BEEP_FREQ_D4  = FREQUENCY_ENCODE(293.66f),
	ADI_BEEP_FREQ_Ds4 = FREQUENCY_ENCODE(311.13f),
	ADI_BEEP_FREQ_E4  = FREQUENCY_ENCODE(329.63f),
	ADI_BEEP_FREQ_F4  = FREQUENCY_ENCODE(349.23f),
	ADI_BEEP_FREQ_Fs4 = FREQUENCY_ENCODE(369.99f),
	ADI_BEEP_FREQ_G4  = FREQUENCY_ENCODE(392.00f),
	ADI_BEEP_FREQ_Gs4 = FREQUENCY_ENCODE(415.30f),
	ADI_BEEP_FREQ_A4  = FREQUENCY_ENCODE(440.00f),
	ADI_BEEP_FREQ_As4 = FREQUENCY_ENCODE(466.16f),
	ADI_BEEP_FREQ_B4  = FREQUENCY_ENCODE(493.88f),

	ADI_BEEP_FREQ_C5  = FREQUENCY_ENCODE(523.25f),
	ADI_BEEP_FREQ_Cs5 = FREQUENCY_ENCODE(554.37f),
	ADI_BEEP_FREQ_D5  = FREQUENCY_ENCODE(587.33f),
	ADI_BEEP_FREQ_Ds5 = FREQUENCY_ENCODE(622.25f),
	ADI_BEEP_FREQ_E5  = FREQUENCY_ENCODE(659.26f),
	ADI_BEEP_FREQ_F5  = FREQUENCY_ENCODE(698.46f),
	ADI_BEEP_FREQ_Fs5 = FREQUENCY_ENCODE(739.99f),
	ADI_BEEP_FREQ_G5  = FREQUENCY_ENCODE(783.99f),
	ADI_BEEP_FREQ_Gs5 = FREQUENCY_ENCODE(830.61f),
	ADI_BEEP_FREQ_A5  = FREQUENCY_ENCODE(880.00f),
	ADI_BEEP_FREQ_As5 = FREQUENCY_ENCODE(932.33f),
	ADI_BEEP_FREQ_B5  = FREQUENCY_ENCODE(987.77f),

	ADI_BEEP_FREQ_C6  = FREQUENCY_ENCODE(1046.50f),
	ADI_BEEP_FREQ_Cs6 = FREQUENCY_ENCODE(1108.73f),
	ADI_BEEP_FREQ_D6  = FREQUENCY_ENCODE(1174.66f),
	ADI_BEEP_FREQ_Ds6 = FREQUENCY_ENCODE(1244.51f),
	ADI_BEEP_FREQ_E6  = FREQUENCY_ENCODE(1318.51f),
	ADI_BEEP_FREQ_F6  = FREQUENCY_ENCODE(1396.91f),
	ADI_BEEP_FREQ_Fs6 = FREQUENCY_ENCODE(1479.98f),
	ADI_BEEP_FREQ_G6  = FREQUENCY_ENCODE(1567.98f),
	ADI_BEEP_FREQ_Gs6 = FREQUENCY_ENCODE(1661.22f),
	ADI_BEEP_FREQ_A6  = FREQUENCY_ENCODE(1760.00f),
	ADI_BEEP_FREQ_As6 = FREQUENCY_ENCODE(1864.66f),
	ADI_BEEP_FREQ_B6  = FREQUENCY_ENCODE(1975.53f),

	ADI_BEEP_FREQ_C7  = FREQUENCY_ENCODE(2093.00f),
	ADI_BEEP_FREQ_Cs7 = FREQUENCY_ENCODE(2217.46f),
	ADI_BEEP_FREQ_D7  = FREQUENCY_ENCODE(2349.32f),
	ADI_BEEP_FREQ_Ds7 = FREQUENCY_ENCODE(2489.02f),
	ADI_BEEP_FREQ_E7  = FREQUENCY_ENCODE(2637.02f),
	ADI_BEEP_FREQ_F7  = FREQUENCY_ENCODE(2793.83f),
	ADI_BEEP_FREQ_Fs7 = FREQUENCY_ENCODE(2959.96f),
	ADI_BEEP_FREQ_G7  = FREQUENCY_ENCODE(3135.96f),
	ADI_BEEP_FREQ_Gs7 = FREQUENCY_ENCODE(3322.44f),
	ADI_BEEP_FREQ_A7  = FREQUENCY_ENCODE(3520.00f),
	ADI_BEEP_FREQ_As7 = FREQUENCY_ENCODE(3729.31f),
	ADI_BEEP_FREQ_B7  = FREQUENCY_ENCODE(3951.07f),

	ADI_BEEP_FREQ_C8  = FREQUENCY_ENCODE(4186.01f),
	ADI_BEEP_FREQ_Cs8 = FREQUENCY_ENCODE(4434.92f),
	ADI_BEEP_FREQ_D8  = FREQUENCY_ENCODE(4698.64f),
	ADI_BEEP_FREQ_Ds8 = FREQUENCY_ENCODE(4978.03f),
	ADI_BEEP_FREQ_E8  = FREQUENCY_ENCODE(5274.04f),
	ADI_BEEP_FREQ_F8  = FREQUENCY_ENCODE(5587.65f),
	ADI_BEEP_FREQ_Fs8 = FREQUENCY_ENCODE(5919.91f),
	ADI_BEEP_FREQ_G8  = FREQUENCY_ENCODE(6271.93f),
} ADI_BEEP_NOTE_FREQUENCY;

#define ADI_BEEP_DUR_ZERO      (0)                       /*!<  Beeper zero tone duration value          */
#define ADI_BEEP_DUR_MIN       (1)                       /*!<  Beeper minimum tone duration value       */
#define ADI_BEEP_DUR_MAX       (254)                     /*!<  Beeper maximum tone duration value       */
#define ADI_BEEP_DUR_INFINITE  (255)                     /*!<  Beeper infinite tone duration value      */

/*! A device handle used in all API functions to identify the BEEP device. */
typedef void * ADI_BEEP_HANDLE;

#define DURATION_ENCODE(x) (uint8_t)((float)ADI_BEEP_DUR_MAX/(float)(x) + 0.5f)    /*!<  Beeper tone duration encoder macro      */

/*!
 * \enum ADI_BEEP_NOTE_DURATION
 *  @brief  Beeper tone duration list.
 *  @details List of possible Beeper tone durations.
 */
typedef enum {
	ADI_BEEP_DUR_0         = ADI_BEEP_DUR_ZERO,        /*!<  stop                       */
	ADI_BEEP_DUR_32_32     = DURATION_ENCODE(1),       /*!<  whole note (1.016 seconds) */
	ADI_BEEP_DUR_16_32     = DURATION_ENCODE(2),       /*!<  half note                  */
	ADI_BEEP_DUR_12_32     = DURATION_ENCODE(8/3),     /*!<  three eights note          */
	ADI_BEEP_DUR_8_32      = DURATION_ENCODE(4),       /*!<  one quarter note           */
	ADI_BEEP_DUR_6_32      = DURATION_ENCODE(16/3),    /*!<  three sixteenth note       */
	ADI_BEEP_DUR_4_32      = DURATION_ENCODE(8),       /*!<  one eighth note            */
	ADI_BEEP_DUR_2_32      = DURATION_ENCODE(16),      /*!<  one sixteenth note         */
	ADI_BEEP_DUR_1_32      = DURATION_ENCODE(32),      /*!<  one thirty-secondth note   */
	ADI_BEEP_DUR_N         = ADI_BEEP_DUR_INFINITE,    /*!<  continuous play            */
} ADI_BEEP_NOTE_DURATION;

/*!
 * \struct ADI_BEEP_NOTE
 *  @brief  Beeper note structure.
 *  @details Describes a note in terms of frequency and duration.
 */
typedef struct {
   ADI_BEEP_NOTE_FREQUENCY frequency;  /*!<  Frequency of the note */
   ADI_BEEP_NOTE_DURATION  duration;   /*!<  Duration of the note */
} ADI_BEEP_NOTE;


/*================ E X T E R N A L S ==================*/

/*
 * Beeper API
 */

ADI_BEEP_RESULT adi_beep_Open                   (ADI_BEEP_DEV_ID          const DeviceNum,
                                                 void*                    const pMemory,
                                                 uint32_t                 const MemorySize,
                                                 ADI_BEEP_HANDLE*         const phDevice);

ADI_BEEP_RESULT adi_beep_RegisterCallback       (ADI_BEEP_HANDLE const hDevice, 
                                                 ADI_CALLBACK          pfCallback,
                                                 void* const           pCBParam);

ADI_BEEP_RESULT adi_beep_PlayNote               (ADI_BEEP_HANDLE          const hDevice,
                                                 ADI_BEEP_NOTE            note);

ADI_BEEP_RESULT adi_beep_PlayTwoTone            (ADI_BEEP_HANDLE          const hDevice, 
                                                 ADI_BEEP_NOTE            noteA,
                                                 ADI_BEEP_NOTE            noteB,
                                                 uint8_t                  count);

ADI_BEEP_RESULT adi_beep_PlaySequence           (ADI_BEEP_HANDLE const hDevice, 
                                                 ADI_BEEP_NOTE aSequence[], 
                                                 uint8_t count);

ADI_BEEP_RESULT adi_beep_Enable                 (ADI_BEEP_HANDLE          const hDevice,
                                                 bool                     const bFlag);

ADI_BEEP_RESULT adi_beep_Wait                   (ADI_BEEP_HANDLE          const hDevice);

ADI_BEEP_RESULT adi_beep_Close                  (ADI_BEEP_HANDLE          const hDevice);

#ifdef __cplusplus
}
#endif


#endif /* ADI_BEEP_H */
/*@}*/
