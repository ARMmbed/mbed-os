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
#include "sleepmodes.h"
#include "em_emu.h"
#include "mbed_critical.h"

uint32_t sleep_block_counter[NUM_SLEEP_MODES] = {0};

/**
 * Sleep mode.
 * Enter the lowest possible sleep mode that is not blocked by ongoing activity.
 */
void hal_sleep(void)
{
    if (sleep_block_counter[0] > 0) {
        /* Blocked everything below EM0, so just return */
        return;
    } else if (sleep_block_counter[1] > 0) {
        /* Blocked everything below EM1, enter EM1 */
        EMU_EnterEM1();
    } else if (sleep_block_counter[2] > 0) {
        /* Blocked everything below EM2, enter EM2 */
        EMU_EnterEM2(true);
    } else {
        /* Blocked everything below EM3, enter EM3 */
        EMU_EnterEM3(true);
    } /* Never enter EM4, as mbed has no way of configuring EM4 wakeup */
    return;
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

/** Block the microcontroller from sleeping below a certain mode
 *
 * This will block sleep() from entering an energy mode below the one given.
 * -- To be called by peripheral HAL's --
 *
 * After the peripheral is finished with the operation, it should call unblock with the same state
 *
 */
void blockSleepMode(sleepstate_enum minimumMode)
{
    core_util_critical_section_enter();
    sleep_block_counter[minimumMode]++;
    core_util_critical_section_exit();
}

/** Unblock the microcontroller from sleeping below a certain mode
 *
 * This will unblock sleep() from entering an energy mode below the one given.
 * -- To be called by peripheral HAL's --
 *
 * This should be called after all transactions on a peripheral are done.
 */
void unblockSleepMode(sleepstate_enum minimumMode)
{
    core_util_critical_section_enter();
    if(sleep_block_counter[minimumMode] > 0) {
        sleep_block_counter[minimumMode]--;
    }
    core_util_critical_section_exit();
}

#endif
