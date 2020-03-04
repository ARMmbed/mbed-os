/* mbed Microcontroller Library
 * Copyright (c) 2019 STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
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

#if DEVICE_SLEEP

#include "mbed_critical.h"

extern void save_timer_ctx(void);
extern void restore_timer_ctx(void);
extern int serial_is_tx_ongoing(void);
extern void PWR_EnterStopMode(void);
extern void PWR_ExitStopMode(void);


extern int mbed_sdk_inited;

/*  STM32WB has very specific needs to handling STOP mode.
 *  The target has a cortex-M4 and cortex-M0 and needs to
 *  handle shared ressources btw both cores.
 *  Each core can sleep independantly, but the first one that
 *  wakes-up needs to restore the clock tree.
 */
void hal_deepsleep(void)
{
    /*  WORKAROUND:
     *  MBED serial driver does not handle deepsleep lock
     *  to prevent entering deepsleep until HW serial FIFO is empty.
     *  This is tracked in mbed issue 4408.
     *  For now, we're checking all Serial HW FIFO. If any transfer is ongoing
     *  we're not entering deep sleep and returning immediately. */
    if (serial_is_tx_ongoing()) {
        return;
    }

    // Disable IRQs
    core_util_critical_section_enter();

    save_timer_ctx();

    /* Prevent HAL_GetTick() from using ticker_read_us() to read the
     * us_ticker timestamp until the us_ticker context is restored. */
    mbed_sdk_inited = 0;

    PWR_EnterStopMode();
    PWR_ExitStopMode();

    restore_timer_ctx();

    /* us_ticker context restored, allow HAL_GetTick() to read the us_ticker
     * timestamp via ticker_read_us() again. */
    mbed_sdk_inited = 1;

    // Enable IRQs
    core_util_critical_section_exit();
}

#endif
