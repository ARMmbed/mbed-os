
/** \addtogroup hal */
/** @{*/
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
#ifndef MBED_SLEEP_API_H
#define MBED_SLEEP_API_H

#include "device.h"

#if DEVICE_SLEEP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_sleep sleep hal requirements
 * Low level interface to the sleep mode of a target.
 *
 * # Defined behaviour
 *
 * * Sleep mode
 *   * wake-up time should be less than 10 us - Verified by sleep_usticker_test().
 *   * the processor can be woken up by any internal peripheral interrupt  - Verified by sleep_usticker_test().
 *   * all peripherals operate as in run mode - not verified.
 *   * the processor can be woken up by external pin interrupt - not verified.
 * * Deep sleep
 *   * the wake-up time should be less than 10 ms - Verified by deepsleep_lpticker_test().
 *   * lp ticker should wake up a target from this mode - Verified by deepsleep_lpticker_test().
 *   * RTC should wake up a target from this mode - not verified.
 *   * an external interrupt on a pin should wake up a target from this mode - not verified.
 *   * a watchdog timer should wake up a target from this mode - not verified.
 *   * High-speed clocks are turned off - Verified by deepsleep_high_speed_clocks_turned_off_test().
 *   * RTC keeps time - Verified by rtc_sleep_test().
 *
 * # Undefined behaviour
 *
 * * peripherals aside from RTC, GPIO and lp ticker result in undefined behaviour in deep sleep.
 * @{
 */

/**
 * \defgroup hal_sleep_tests sleep hal tests
 * The sleep HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the sleep hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-sleep*
 *
 */

/** Send the microcontroller to sleep
 *
 * The processor is setup ready for sleep, and sent to sleep. In this mode, the
 * system clock to the core is stopped until a reset or an interrupt occurs. This eliminates
 * dynamic power used by the processor, memory systems and buses. The processor, peripheral and
 * memory state are maintained, and the peripherals continue to work and can generate interrupts.
 *
 * The processor can be woken up by any internal peripheral interrupt or external pin interrupt.
 *
 * The wake-up time shall be less than 10 us.
 * 
 */
void hal_sleep(void);

/** Send the microcontroller to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and high frequency clocks.
 * All state is still maintained.
 *
 * The processor can only be woken up by low power ticker, RTC, an external interrupt on a pin or a watchdog timer.
 *
 * The wake-up time shall be less than 10 ms.
 */
void hal_deepsleep(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
