/***************************************************************************//**
 * @file device_peripheral.h
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
#define US_TICKER_TIMER			TIMER1
#define US_TICKER_TIMER_CLOCK	cmuClock_TIMER1
#define US_TICKER_TIMER_IRQ		TIMER1_IRQn

/* PWM */
#define PWM_TIMER TIMER0
#define PWM_TIMER_CLOCK cmuClock_TIMER0
#define PWM_ROUTE TIMER_ROUTE_LOCATION_LOC0

/* Clocks */

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
 * 	* LFXO: external crystal, please define frequency.
 * 	* LFRCO: internal RC oscillator (32.768kHz)
 * 	* ULFRCO: internal ultra-low power RC oscillator (available down to EM3) (1kHz)
 */
#define LOW_ENERGY_CLOCK_SOURCE	LFXO

/** Core clock source.
 * Options:
 * 	* HFXO: external crystal, please define frequency.
 * 	* HFRCO: High-frequency internal RC oscillator. Please select band as well.
 */
#define CORE_CLOCK_SOURCE		HFXO

/** HFRCO frequency band
 * Options:
 * 	* _CMU_HFRCOCTRL_BAND_21MHZ
 * 	* _CMU_HFRCOCTRL_BAND_14MHZ
 * 	* _CMU_HFRCOCTRL_BAND_11MHZ
 * 	* _CMU_HFRCOCTRL_BAND_7MHZ
 * 	* _CMU_HFRCOCTRL_BAND_1MHZ
 */
#define HFRCO_FREQUENCY 		_CMU_HFRCOCTRL_BAND_21MHZ

#define LFXO_FREQUENCY			32768
#define HFXO_FREQUENCY			24000000

#if (LOW_ENERGY_CLOCK_SOURCE == LFXO)
#define LOW_ENERGY_CLOCK_FREQUENCY	LFXO_FREQUENCY
#elif (LOW_ENERGY_CLOCK_SOURCE == LFRCO)
#define LOW_ENERGY_CLOCK_FREQUENCY	32768
#elif (LOW_ENERGY_CLOCK_SOURCE == ULFRCO)
#define LOW_ENERGY_CLOCK_FREQUENCY	1000
#else
#error "Unknown Low Energy Clock selection"
#endif

#endif
