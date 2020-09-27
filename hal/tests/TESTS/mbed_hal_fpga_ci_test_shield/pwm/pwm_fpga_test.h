/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/** \addtogroup hal_pwmout_tests */
/** @{*/

#ifndef MBED_FPGA_PWM_TEST_H
#define MBED_FPGA_PWM_TEST_H

#if DEVICE_PWM

#ifdef __cplusplus
extern "C" {
#endif

/** Test that the PWM can be initialized/de-initialized using all possible
 *  PWM pins.
 *
 * Given board provides PWM support.
 * When PWM is initialized (and then de-initialized) using valid PWM pin.
 * Then the operation is successfull.
 *
 */
void fpga_pwm_init_free(PinName pin);

/** Test that pwmout_period, pwmout_period_ms, pwmout_period_us functions sets the
 * PWM period correctly and pwmout_write, pwmout_pulsewidth, pwmout_pulsewidth_ms,
 * pwmout_pulsewidth_us functions sets the pulse width correctly.
 *
 * Given board provides PWM support.
 * When PWM period/width is set using pwmout_period, pwmout_period_ms, pwmout_period_us/pwmout_write, pwmout_pulsewidth, pwmout_pulsewidth_ms, pwmout_pulsewidth_us
 * Then the valid PWM puswidth and period is on output.
 *
 */
void fpga_pwm_period_fill_test(PinName pin);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
