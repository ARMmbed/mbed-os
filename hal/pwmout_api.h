
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_PWMOUT_API_H
#define MBED_PWMOUT_API_H

#include "device.h"
#include "pinmap.h"

#if DEVICE_PWMOUT

#ifdef __cplusplus
extern "C" {
#endif

/** Pwmout hal structure. pwmout_s is declared in the target's hal
 */
typedef struct pwmout_s pwmout_t;

/**
 * \defgroup hal_pwmout Pwmout hal functions
 *
 * # Defined behavior
 * * ::pwmout_init initializes the pwmout_t control structure
 * * ::pwmout_free deinitializes the pwmout object
 * * ::pwmout_write sets the output duty-cycle in range <0.0f, 1.0f>
 * * ::pwmout_read returns the current float-point output duty-cycle in range <0.0f, 1.0f>
 * * ::pwmout_period sets the PWM period specified in seconds, keeping the duty cycle the same
 * * ::pwmout_period_ms sets the PWM period specified in miliseconds, keeping the duty cycle the same
 * * ::pwmout_period_us sets the PWM period specified in microseconds, keeping the duty cycle the same
 * * ::pwmout_pulsewidth sets the PWM pulsewidth specified in seconds, keeping the period the same
 * * ::pwmout_pulsewidth_ms sets the PWM pulsewidth specified in miliseconds, keeping the period the same
 * * ::pwmout_pulsewidth_us sets the PWM pulsewidth specified in microseconds, keeping the period the same
 * * The accuracy of the PWM is +/- 10%
 * * The PWM operations ::pwmout_write, ::pwmout_read, ::pwmout_read, ::pwmout_period_ms, ::pwmout_period_us
 *   ::pwmout_pulsewidth, ::pwmout_pulsewidth_ms, ::pwmout_pulsewidth_us take less than 20us to complete
 *
 * # Undefined behavior
 * * Calling other function before ::pwmout_init
 * * Calling ::pwmout_init with NC as pwmout pin
 *
 * @{
 */

/**
 * \defgroup hal_pwmout_tests GPIO IRQ HAL tests
 * The Pwmout HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the Pwmout hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal_fpga_ci_test_shield-pwm
 *
 */

/** Initialize the pwm out peripheral and configure the pin
 *
 * @param obj The pwmout object to initialize
 * @param pinmap pointer to structure which holds static pinmap
 */
void pwmout_init_direct(pwmout_t *obj, const PinMap *pinmap);

/** Initialize the pwm out peripheral and configure the pin
 *
 * @param obj The pwmout object to initialize
 * @param pin The pwmout pin to initialize
 */
void pwmout_init(pwmout_t *obj, PinName pin);

/** Deinitialize the pwmout object
 *
 * @param obj The pwmout object
 */
void pwmout_free(pwmout_t *obj);

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * Value 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * @param obj     The pwmout object
 * @param percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent);

/** Read the current float-point output duty-cycle
 *
 * @param obj The pwmout object
 * @return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj);

/** Set the PWM period specified in seconds, keeping the duty cycle the same
 *
 * Periods smaller than microseconds (the lowest resolution) are set to zero.
 * @param obj     The pwmout object
 * @param seconds The floating-point seconds period
 */
void pwmout_period(pwmout_t *obj, float seconds);

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int ms);

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int us);

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * @param obj     The pwmout object
 * @param seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float seconds);

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int ms);

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int us);

/** Get the pins that support PWM
 *
 * Return a PinMap array of pins that support PWM.
 * The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *pwmout_pinmap(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
