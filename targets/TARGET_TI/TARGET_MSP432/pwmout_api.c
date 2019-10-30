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

/* Low-level implementation of PWM functionality for MSP432.
   Only TIMER A0..A2 are used for PWM, because TIMER A3 is
   already used for the us ticker. SMCLK is typically 3 MHz,
   and the maximum pre-divider is 64. With TIMER A being a
   16 bit counter, we have a maximum period of
   64 * 65536 / 3000000 = 1.39 seconds!
   In total there are 12 PWM outputs. Each 4 outputs
   from one Timer-A module (A0..A2) shared the same period!
 */

#include "pwmout_api.h"
#include "mbed_assert.h"
#include <stdbool.h>

#if DEVICE_PWMOUT

/** Initialize the pwm out peripheral and configure the pin
 *
 * @param obj The pwmout object to initialize
 * @param pin The pwmout pin to initialize
 */
void pwmout_init(pwmout_t *obj, PinName pin)
{
    /* Get the peripheral name */
    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(obj->pwm != (PWMName)NC);
    /* Fill obj (get the index of the Capture/Compare Register (CCR)) */
    obj->pin = pin;
    obj->ccr_index = GET_DATA_CHAN(pinmap_function(pin, PinMap_PWM));
    obj->pulse     = 0;
    obj->dutyCycle = 0.0f;
    /* Configure PWM pin */
    pinmap_pinout(pin, PinMap_PWM);
    /* Get the TIMER A base */
    Timer_A_Type *TA = (Timer_A_Type *)obj->pwm;
    /* Configure the TIMER A and the CCR */
    TA->CTL = TIMER_A_CTL_SSEL__SMCLK | /* SMCLK clock source      */
              TIMER_A_CTL_MC__UP      | /* Up-mode (count to CCR0) */
              TIMER_A_CTL_CLR;          /* Reset counter           */
    /* Use reset/set mode (mode 7) */
    TA->CCTL[obj->ccr_index] = TIMER_A_CCTLN_OUTMOD_7;
    // Set default period (20ms)
    pwmout_period_us(obj, 20000);
}

/** Deinitialize the pwmout object
 *
 * @param obj The pwmout object
 */
void pwmout_free(pwmout_t *obj)
{
    /* Get the TIMER A base */
    Timer_A_Type *TA = (Timer_A_Type *)obj->pwm;
    /* Stop the timer */
    TA->CTL = 0;
    /* De-configure the pin */
    pin_function(obj->pin, MSP432_PIN_DATA(SEL0, PIN_INPUT, PullNone, 0));
}

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * Value 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * @param obj     The pwmout object
 * @param percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent)
{
    obj->dutyCycle = percent;
    obj->pulse     = (float)obj->ccr0 * percent;
    /* Get the TIMER A base */
    Timer_A_Type *TA = (Timer_A_Type *)obj->pwm;
    // Set the CCR register
    TA->CCR[obj->ccr_index] = obj->pulse;
}

/** Read the current float-point output duty-cycle
 *
 * @param obj The pwmout object
 * @return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj)
{
    return obj->dutyCycle;
}

/** Set the PWM period specified in seconds, keeping the duty cycle the same
 *
 * Periods smaller than microseconds (the lowest resolution) are set to zero.
 * @param obj     The pwmout object
 * @param seconds The floating-point seconds period
 */
void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int us)
{
    /* Calculate dividers and CCR0 */
    uint32_t clock_MHz = SubsystemMasterClock / 1000000;
    clock_MHz *= us;
    uint16_t id, ex0;
    bool found = false;
    for (id = 0; id < 4; id++) {
        for (ex0 = 0; ex0 < 8; ex0++) {
            obj->divider = (1 << id) * (ex0 + 1);
            obj->ccr0 = clock_MHz / obj->divider;
            if (obj->ccr0 < 0x10000) {
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    MBED_ASSERT(found);
    /* Get the TIMER A base */
    Timer_A_Type *TA = (Timer_A_Type *)obj->pwm;
    // Set the calculated results
    TA->CTL   &= ~TIMER_A_CTL_ID_MASK;
    TA->CTL   |= (id << TIMER_A_CTL_ID_OFS);
    TA->EX0    = ex0;
    TA->CCR[0] = obj->ccr0 - 1;
    /* Set the former duty cycle */
    pwmout_write(obj, obj->dutyCycle);
}

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * @param obj     The pwmout object
 * @param seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    obj->pulse  = SubsystemMasterClock / 1000000;
    obj->pulse *= us;
    obj->pulse /= obj->divider;
    MBED_ASSERT(obj->pulse <= obj->ccr0);
    /* Set the new duty cycle */
    obj->dutyCycle = (float)obj->pulse / (float)obj->ccr0;
    /* Get the TIMER A base */
    Timer_A_Type *TA = (Timer_A_Type *)obj->pwm;
    // Set the CCR register
    TA->CCR[obj->ccr_index] = obj->pulse;
}

/** Get the pins that support PWM
 *
 * Return a PinMap array of pins that support PWM.
 * The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *pwmout_pinmap(void)
{
    return PinMap_PWM;
}

#endif

