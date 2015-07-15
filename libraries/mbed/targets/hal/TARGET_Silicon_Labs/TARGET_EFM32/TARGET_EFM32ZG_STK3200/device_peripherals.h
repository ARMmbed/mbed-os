/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
#define LFXO	cmuSelect_LFXO
#define HFXO	cmuSelect_HFXO
#define LFRCO	cmuSelect_LFRCO
#define HFRCO	cmuSelect_HFRCO
#if !defined(_EFM32_GECKO_FAMILY)
#define ULFRCO	cmuSelect_ULFRCO
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
 * 	* CMU_HFRCOCTRL_BAND_28MHZ
 * 	* CMU_HFRCOCTRL_BAND_21MHZ
 * 	* CMU_HFRCOCTRL_BAND_14MHZ
 * 	* CMU_HFRCOCTRL_BAND_11MHZ
 * 	* CMU_HFRCOCTRL_BAND_7MHZ
 * 	* CMU_HFRCOCTRL_BAND_1MHZ
 */
#define HFRCO_FREQUENCY 		CMU_HFRCOCTRL_BAND_14MHZ

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
