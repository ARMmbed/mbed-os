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
#define US_TICKER_TIMER			TIMER1
#define US_TICKER_TIMER_CLOCK	cmuClock_TIMER1
#define US_TICKER_TIMER_IRQ		TIMER1_IRQn

/* PWM */
#define PWM_TIMER TIMER0
#define PWM_TIMER_CLOCK cmuClock_TIMER0
#define PWM_ROUTE TIMER_ROUTE_LOCATION_LOC0

/* USB */
#define USB_TIMER USB_TIMER2

#endif
