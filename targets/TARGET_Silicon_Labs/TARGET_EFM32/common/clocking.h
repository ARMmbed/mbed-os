/***************************************************************************//**
 * @file clocking.h
 * @brief Clock selection calculations
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef MBED_CLOCKING_H
#define MBED_CLOCKING_H

/* Clock definitions */
#define LFXO    0
#define HFXO    1
#define LFRCO   2
#define HFRCO   3
#if !defined(_EFM32_GECKO_FAMILY)
#define ULFRCO  4
#endif

/* Low Energy peripheral clock source.
 * Options:
 *  * LFXO: external crystal, please define frequency.
 *  * LFRCO: internal RC oscillator (32.768kHz)
 *  * ULFRCO: internal ultra-low power RC oscillator (available down to EM3) (1kHz)
 */
#ifndef LOW_ENERGY_CLOCK_SOURCE
#define LOW_ENERGY_CLOCK_SOURCE LFXO
#endif

/** Core clock source.
 * Options:
 *  * HFXO: external crystal, please define frequency.
 *  * HFRCO: High-frequency internal RC oscillator. Please select band as well.
 */
#ifndef CORE_CLOCK_SOURCE
#define CORE_CLOCK_SOURCE   HFRCO
#if defined(_CMU_HFRCOCTRL_BAND_MASK)
#define HFRCO_FREQUENCY_ENUM   _CMU_HFRCOCTRL_BAND_21MHZ 
#define HFRCO_FREQUENCY        21000000 
#elif defined(_CMU_HFRCOCTRL_FREQRANGE_MASK)
#define HFRCO_FREQUENCY_ENUM   cmuHFRCOFreq_32M0Hz
#define HFRCO_FREQUENCY        32000000
#endif
#endif // CORE_CLOCK_SOURCE

#if !defined(LFXO_FREQUENCY) && (LOW_ENERGY_CLOCK_SOURCE == LFXO)
#error "LFXO frequency is undefined!"
#endif

#if !defined(HFXO_FREQUENCY) && (CORE_CLOCK_SOURCE == HFXO)
#error "HFXO frequency is undefined!"
#endif

#if (LOW_ENERGY_CLOCK_SOURCE == LFXO)
#define LOW_ENERGY_CLOCK_FREQUENCY  LFXO_FREQUENCY
#elif (LOW_ENERGY_CLOCK_SOURCE == LFRCO)
#define LOW_ENERGY_CLOCK_FREQUENCY  32768
#elif (LOW_ENERGY_CLOCK_SOURCE == ULFRCO)
#define LOW_ENERGY_CLOCK_FREQUENCY  1000
#else
#error "Unknown Low Energy Clock selection"
#endif

#if( CORE_CLOCK_SOURCE == HFXO)
# define REFERENCE_FREQUENCY HFXO_FREQUENCY
#elif( CORE_CLOCK_SOURCE == HFRCO)
#if !defined(HFRCO_FREQUENCY)
# error "HFRCO frequency is not defined!"
#else
# define REFERENCE_FREQUENCY HFRCO_FREQUENCY
#endif
#endif

#if ( LOW_ENERGY_CLOCK_SOURCE == LFXO )
# define LEUART_USING_LFXO
# if ( (defined(CMU_CTRL_HFLE) || defined(CMU_CTRL_WSHFLE) ) && (REFERENCE_FREQUENCY > 24000000) )
#  define LEUART_HF_REF_FREQ (REFERENCE_FREQUENCY / 4)
# else
#  define LEUART_HF_REF_FREQ (REFERENCE_FREQUENCY / 2)
# endif
# define LEUART_LF_REF_FREQ LFXO_FREQUENCY
#else
# if ( (defined(CMU_CTRL_HFLE) || defined(CMU_CTRL_WSHFLE) ) && (REFERENCE_FREQUENCY > 24000000) )
#  define LEUART_REF_FREQ (REFERENCE_FREQUENCY / 4)
# else
#  define LEUART_REF_FREQ (REFERENCE_FREQUENCY / 2)
# endif
#endif

#endif
