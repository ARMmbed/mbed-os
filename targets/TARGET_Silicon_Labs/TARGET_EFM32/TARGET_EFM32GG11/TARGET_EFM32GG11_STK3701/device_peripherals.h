/***************************************************************************//**
 * @file device_peripherals.h
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

/* Set SWO location */
#if DEVICE_ITM
/* Set location 0 */
    #define SWO_LOCATION (GPIO->SWO_ROUTE & ~_GPIO_ROUTELOC0_SWVLOC_MASK) | GPIO_ROUTELOC0_SWVLOC_LOC0
/* Enable output on pin */
    #define GPIO_PORT 5 // GPIO port number (A = 0, B = 1, ...)
    #define SWO_MODE MODEL
    #define SWO_ENABLE_OUTPUT_PIN (GPIO->P[GPIO_PORT].SWO_MODE & ~_GPIO_P_MODEL_MODE2_MASK) | GPIO_P_MODEL_MODE2_PUSHPULL

/* Set location 1 */
/*
    #define SWO_LOCATION (GPIO->SWO_ROUTE & ~_GPIO_ROUTELOC0_SWVLOC_MASK) | GPIO_ROUTELOC0_SWVLOC_LOC1
// Enable output on pin 
    #define GPIO_PORT 2 // GPIO port number (A = 0, B = 1, ...)
    #define SWO_MODE MODEH
    #define SWO_ENABLE_OUTPUT_PIN (GPIO->P[GPIO_PORT].SWO_MODE & ~_GPIO_P_MODEH_MODE15_MASK) | GPIO_P_MODEH_MODE15_PUSHPULL
*/
#endif

/* Crystal calibration */
#if !defined(CMU_HFXOINIT_STK_DEFAULT)
#define CMU_HFXOINIT_STK_DEFAULT                   \
  {                                                \
    _CMU_HFXOSTARTUPCTRL_CTUNE_DEFAULT,            \
    0x84,                                          \
    _CMU_HFXOSTARTUPCTRL_IBTRIMXOCORE_DEFAULT,     \
    _CMU_HFXOSTEADYSTATECTRL_IBTRIMXOCORE_DEFAULT, \
    _CMU_HFXOTIMEOUTCTRL_PEAKDETTIMEOUT_DEFAULT,   \
    _CMU_HFXOTIMEOUTCTRL_STEADYTIMEOUT_DEFAULT,    \
    _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_DEFAULT,   \
    cmuOscMode_Crystal,                            \
  }
#endif

#if !defined(EMU_DCDCINIT_STK_DEFAULT)
#define EMU_DCDCINIT_STK_DEFAULT EMU_DCDCINIT_DEFAULT
#endif
#endif
