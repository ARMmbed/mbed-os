/***************************************************************************//**
 * @file sleep.c
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

#include "device.h"
#if DEVICE_SLEEP

#include "sleep_api.h"
#include "em_emu.h"

/**
 * Sleep mode.
 * Stop the core clock using a WFI.
 */
void hal_sleep(void)
{
    EMU_EnterEM1();
}

/**
 * Deep Sleep mode.
 * Enter Energy Mode 2, turning off all high-frequency clocks.
 *
 * In EM2 the high frequency oscillator is turned off, but with the 32.768 kHz
 * oscillator running, selected low energy peripherals (LCD, RTC, LETIMER,
 * PCNT, LEUART, I2C, LESENSE, OPAMP, USB, WDOG and ACMP) are still
 * available. This gives a high degree of autonomous operation with a current
 * consumption as low as 1.1 μA with RTC enabled. Power-on Reset, Brown-out
 * Detection and full RAM and CPU retention is also included.
 */
void hal_deepsleep(void)
{
    EMU_EnterEM2(true);
}

#endif
