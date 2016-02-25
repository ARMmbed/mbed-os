/***************************************************************************//**
 * @file device_peripherals.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef MBED_DEVICE_PERIPHERALS_H
#define MBED_DEVICE_PERIPHERALS_H

/* us ticker */
#define US_TICKER_TIMER         TIMER0
#define US_TICKER_TIMER_CLOCK   cmuClock_TIMER0
#define US_TICKER_TIMER_IRQ     TIMER0_IRQn

/* PWM */
#define PWM_TIMER        TIMER1
#define PWM_TIMER_CLOCK  cmuClock_TIMER1
#define PWM_ROUTE        TIMER_ROUTE_LOCATION_LOC1

/* Clocks */

/* Clock definitions */
#define LFXO    0
#define HFXO    1
#define LFRCO   2
#define HFRCO   3
#define ULFRCO  4


/* Low Energy peripheral clock source.
 * Options:
 *  * LFXO: external crystal, please define frequency.
 *  * LFRCO: internal RC oscillator (32.768kHz)
 *  * ULFRCO: internal ultra-low power RC oscillator (available down to EM3) (1kHz)
 */
#define LOW_ENERGY_CLOCK_SOURCE     LFXO

/** Core clock source.
 * Options:
 *  * HFXO: external crystal, please define frequency.
 *  * HFRCO: High-frequency internal RC oscillator. Please select frequency as well.
 */
#define CORE_CLOCK_SOURCE       HFXO

/** HFRCO frequency selection
 * Options:
 *   ** HFRCO_FREQUENCY_ENUM ** HFRCO_FREQUENCY **
 *  *   cmuHFRCOFreq_1M0Hz   ==    1000000        *
 *  *   cmuHFRCOFreq_2M0Hz   ==    2000000        *
 *  *   cmuHFRCOFreq_4M0Hz   ==    4000000        *
 *  *   cmuHFRCOFreq_7M0Hz   ==    7000000        *
 *  *   cmuHFRCOFreq_13M0Hz  ==   13000000        *
 *  *   cmuHFRCOFreq_16M0Hz  ==   16000000        *
 *  *   cmuHFRCOFreq_19M0Hz  ==   19000000        *
 *  *   cmuHFRCOFreq_26M0Hz  ==   26000000        *
 *  *   cmuHFRCOFreq_32M0Hz  ==   32000000        *
 *  *   cmuHFRCOFreq_38M0Hz  ==   38000000        *
 *   *********************************************
 */
 
/* Make sure the settings of HFRCO_FREQUENCY and HFRCO_FREQUENCY_ENUM match, or timings will be faulty! */
#define HFRCO_FREQUENCY_ENUM    cmuHFRCOFreq_19M0Hz
#define HFRCO_FREQUENCY         19000000

#define LFXO_FREQUENCY          32768
#define HFXO_FREQUENCY          40000000

#if (LOW_ENERGY_CLOCK_SOURCE == LFXO)
#define LOW_ENERGY_CLOCK_FREQUENCY  LFXO_FREQUENCY
#elif (LOW_ENERGY_CLOCK_SOURCE == LFRCO)
#define LOW_ENERGY_CLOCK_FREQUENCY  32768
#elif (LOW_ENERGY_CLOCK_SOURCE == ULFRCO)
#define LOW_ENERGY_CLOCK_FREQUENCY  1000
#else
#error "Unknown Low Energy Clock selection"
#endif

#endif
